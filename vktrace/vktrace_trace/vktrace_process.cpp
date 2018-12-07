/*
 * Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
 * Copyright (c) 2014-2016 Valve Corporation. All rights reserved.
 * Copyright (C) 2014-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 */

#include <string>
#include "vktrace_process.h"
#include "vktrace.h"

#if defined(PLATFORM_LINUX)
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#if defined(PLATFORM_OSX)
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include <vector>

#if defined(WIN32)
#include <TlHelp32.h>
#endif

extern "C" {
#include "vktrace_filelike.h"
#include "vktrace_interconnect.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_packet_id.h"
}

const unsigned long kWatchDogPollTime = 250;

#if defined(WIN32)
void SafeCloseHandle(HANDLE& _handle) {
    if (_handle) {
        CloseHandle(_handle);
        _handle = NULL;
    }
}
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
// Needs to be static because Process_RunWatchdogThread passes the address of rval to pthread_exit
static int rval;
#endif
// ------------------------------------------------------------------------------------------------
VKTRACE_THREAD_ROUTINE_RETURN_TYPE Process_RunWatchdogThread(LPVOID _procInfoPtr) {
    vktrace_process_info* pProcInfo = (vktrace_process_info*)_procInfoPtr;

#if defined(WIN32)

    DWORD processExitCode = 0;
    DWORD waitingResult = WAIT_FAILED;
    while (WAIT_TIMEOUT == (waitingResult = WaitForSingleObject(pProcInfo->hProcess, kWatchDogPollTime))) {
        if (pProcInfo->serverRequestsTermination) {
            vktrace_LogVerbose("Vktrace has requested exit.");
            return 0;
        }
    }
    if (WAIT_OBJECT_0 == waitingResult) {
        std::vector<vktrace_process_id> childProcesses;

        // The process is finished, now we need to check all its child proess
        // also be finished. When capturing steam game and that game cannot
        // run without steam client, if launch the game binary directly, the
        // binary will just start steam client and quit, then steam client
        // launch the game binary.

        HANDLE processSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (processSnapShot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 processEntry;
            memset(&processEntry, 0, sizeof(processEntry));
            processEntry.dwSize = sizeof(PROCESSENTRY32);
            bool queryProcess = (TRUE == Process32First(processSnapShot, &processEntry));
            while (queryProcess) {
                if (processEntry.th32ParentProcessID == pProcInfo->processId) {
                    childProcesses.push_back(processEntry.th32ProcessID);
                }
                queryProcess = (TRUE == Process32Next(processSnapShot, &processEntry));
            }
            CloseHandle(processSnapShot);
            if (childProcesses.size() != 0) {
                HANDLE childProcessHandle;
                // The target process has one or more child processes, we're
                // going to get their handles and wait them all signaled.
                std::vector<HANDLE> childProcessHandles;
                childProcessHandles.reserve(childProcesses.size());
                for (vktrace_process_id childProcessId : childProcesses) {
                    childProcessHandle = OpenProcess(SYNCHRONIZE, false, childProcessId);
                    if (childProcessHandle != nullptr) {
                        childProcessHandles.push_back(childProcessHandle);
                    }
                }

                WaitForMultipleObjects(childProcessHandles.size(), childProcessHandles.data(), TRUE, INFINITE);

                for (HANDLE finishedProcessHandle : childProcessHandles) {
                    CloseHandle(finishedProcessHandle);
                }
            }
        }
    }
    vktrace_LogVerbose("Child process has terminated.");
    GetExitCodeProcess(pProcInfo->hProcess, &processExitCode);
    PostThreadMessage(pProcInfo->parentThreadId, VKTRACE_WM_COMPLETE, processExitCode, 0);
    pProcInfo->serverRequestsTermination = TRUE;
    return 0;

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    int status = 0;
    int options = 0;

    // Check to see if process exists
    rval = waitpid(pProcInfo->processId, &status, WNOHANG);
    if (rval == pProcInfo->processId) {
        vktrace_LogVerbose("Child process was terminated.");
        rval = 1;
        pthread_exit(&rval);
    }

    rval = 1;
    while (waitpid(pProcInfo->processId, &status, options) != -1) {
        if (WIFEXITED(status)) {
            vktrace_LogVerbose("Child process exited.");
            rval = WEXITSTATUS(status);
            break;
        } else if (WCOREDUMP(status)) {
            vktrace_LogError("Child process crashed.");
            break;
        } else if (WIFSIGNALED(status)) {
            vktrace_LogVerbose("Child process was signaled.");
        } else if (WIFSTOPPED(status)) {
            vktrace_LogVerbose("Child process was stopped.");
        } else if (WIFCONTINUED(status))
            vktrace_LogVerbose("Child process was continued.");
    }
    pthread_exit(&rval);
#endif
}

// ------------------------------------------------------------------------------------------------
bool terminationSignalArrived = false;
void terminationSignalHandler(int sig) { terminationSignalArrived = true; }

// There are multiple processes be created and their running time have overlap
// when start some titles such as some steam titles. These processes all call
// vulkan API and trace layer already got loaded, so we only need to take care
// how to handle their connection request and receive their packet data.
//
// Every process in the case will be a client and the server side need to
// accept their connections within system timeout. Because both side also
// need to keep the connection for a relatively long time for trace file
// recording, so multiple threads are needed to response to multiple clients.
// Here the function is used to create these additional recording threads.
bool CreateAdditionalRecordTraceThread(vktrace_process_info* pInfo) {
    bool create_additional_record_trace_thread = true;
    assert(pInfo != nullptr);
    // prepare data for capture threads
    uint32_t new_thread_trace_file_index = pInfo->currentCaptureThreadsCount;
    if (new_thread_trace_file_index < pInfo->maxCaptureThreadsNumber) {
        pInfo->pCaptureThreads[new_thread_trace_file_index].pProcessInfo = pInfo;
        pInfo->pCaptureThreads[new_thread_trace_file_index].recordingThread = VKTRACE_NULL_THREAD;
        pInfo->pCaptureThreads[new_thread_trace_file_index].traceFileIndex = new_thread_trace_file_index;
        // create thread to record trace packets from the tracer
        pInfo->pCaptureThreads[new_thread_trace_file_index].recordingThread =
            vktrace_platform_create_thread(Process_RunRecordTraceThread, &(pInfo->pCaptureThreads[new_thread_trace_file_index]));
        if (pInfo->pCaptureThreads[new_thread_trace_file_index].recordingThread == VKTRACE_NULL_THREAD) {
            vktrace_LogError("Failed to create additional trace recording thread.");
            create_additional_record_trace_thread = false;
        } else {
            pInfo->currentCaptureThreadsCount++;
        }
    } else {
        vktrace_LogError(
            "Unable to create additional trace recording thread because the trace recording threads number reach the limit.");
        create_additional_record_trace_thread = false;
    }
    return create_additional_record_trace_thread;
}

// ------------------------------------------------------------------------------------------------
VKTRACE_THREAD_ROUTINE_RETURN_TYPE Process_RunRecordTraceThread(LPVOID _threadInfo) {
    vktrace_process_capture_trace_thread_info* pInfo = (vktrace_process_capture_trace_thread_info*)_threadInfo;
    FileLike* fileLikeSocket;
    uint64_t fileHeaderSize;
    vktrace_trace_file_header file_header;
    vktrace_trace_packet_header* pHeader = NULL;
    uint64_t bytes_written;
    uint64_t fileOffset;
#if defined(WIN32)
    BOOL rval;
#elif defined(PLATFORM_LINUX)
    sighandler_t rval __attribute__((unused));
#elif defined(PLATFORM_OSX)
    sig_t rval __attribute__((unused));
#endif

    MessageStream* pMessageStream = vktrace_MessageStream_create(TRUE, "", VKTRACE_BASE_PORT + pInfo->tracerId);
    if (pMessageStream == NULL) {
        vktrace_LogError("Thread_CaptureTrace() cannot create message stream.");
        return 1;
    } else {
        // Now we get a valid pMessageStream which mean a connection
        // request from a client already be accepted and a private socket
        // will be used to record the following API packets in the current
        // thread until the trace file recording be terminated. So we
        // create another thread to serve other possible clients.
        if (false == CreateAdditionalRecordTraceThread(pInfo->pProcessInfo)) {
            vktrace_LogError(
                "Some process of the title will not be captured into trace file due to failure on creating record thread!");
        }
    }

    // create trace file
    pInfo->pTraceFile = vktrace_open_trace_file(pInfo);

    if (pInfo->pTraceFile == NULL) {
        // open of trace file generated an error, no sense in continuing.
        vktrace_LogError("Error cannot create trace file.");
        return 1;
    }

    // Open the socket
    fileLikeSocket = vktrace_FileLike_create_msg(pMessageStream);

    // Read the size of the header packet from the socket
    fileHeaderSize = 0;
    vktrace_FileLike_ReadRaw(fileLikeSocket, &fileHeaderSize, sizeof(fileHeaderSize));

    // Read the header, not including gpu_info
    file_header.first_packet_offset = 0;
    vktrace_FileLike_ReadRaw(fileLikeSocket, &file_header, sizeof(file_header));
    if (fileHeaderSize != sizeof(fileHeaderSize) + sizeof(file_header) + file_header.n_gpuinfo * sizeof(struct_gpuinfo) ||
        file_header.first_packet_offset != sizeof(file_header) + file_header.n_gpuinfo * sizeof(struct_gpuinfo)) {
        // Trace file header we received is the wrong size
        vktrace_LogError("Error creating trace file header. Are vktrace and trace layer the same version?");
        return 1;
    }

    vktrace_enter_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);

    // Write the trace file header to the file
    bytes_written = fwrite(&file_header, 1, sizeof(file_header), pInfo->pTraceFile);

    // Read and write the gpu_info structs
    struct_gpuinfo gpuinfo;
    for (uint64_t i = 0; i < file_header.n_gpuinfo; i++) {
        vktrace_FileLike_ReadRaw(fileLikeSocket, &gpuinfo, sizeof(struct_gpuinfo));
        bytes_written += fwrite(&gpuinfo, 1, sizeof(struct_gpuinfo), pInfo->pTraceFile);
    }
    fflush(pInfo->pTraceFile);
    vktrace_leave_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);

    if (bytes_written != sizeof(file_header) + file_header.n_gpuinfo * sizeof(struct_gpuinfo)) {
        vktrace_LogError("Unable to write trace file header - fwrite failed.");
        return 1;
    }
    fileOffset = file_header.first_packet_offset;

#if defined(WIN32)
    rval = SetConsoleCtrlHandler((PHANDLER_ROUTINE)terminationSignalHandler, TRUE);
    assert(rval);
#else
    rval = signal(SIGHUP, terminationSignalHandler);
    assert(rval != SIG_ERR);
    rval = signal(SIGINT, terminationSignalHandler);
    assert(rval != SIG_ERR);
    rval = signal(SIGTERM, terminationSignalHandler);
    assert(rval != SIG_ERR);
#endif

    while (!terminationSignalArrived && pInfo->pProcessInfo->serverRequestsTermination == FALSE) {
        // get a packet
        // vktrace_LogDebug("Waiting for a packet...");

        // read entire packet in
        pHeader = vktrace_read_trace_packet(fileLikeSocket);

        if (pHeader == NULL) {
            if (pMessageStream->mErrorNum == WSAECONNRESET) {
                vktrace_LogVerbose("Network connection closed");
            } else {
                vktrace_LogError("Network connection failed");
            }
            break;
        }

        // vktrace_LogDebug("Received packet id: %hu", pHeader->packet_id);

        if (pHeader->pBody == (uintptr_t)NULL) {
            vktrace_LogWarning("Received empty packet body for id: %hu", pHeader->packet_id);
        } else {
            // handle special case packets
            if (pHeader->packet_id == VKTRACE_TPI_MESSAGE) {
                if (g_settings.print_trace_messages == TRUE) {
                    vktrace_trace_packet_message* pPacket = vktrace_interpret_body_as_trace_packet_message(pHeader);
                    vktrace_LogAlways("Packet %lu: Traced Message (%s): %s", pHeader->global_packet_index,
                                      vktrace_LogLevelToShortString(pPacket->type), pPacket->message);
                    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->message));
                }
            }

            if (pHeader->packet_id == VKTRACE_TPI_MARKER_TERMINATE_PROCESS) {
                pInfo->pProcessInfo->serverRequestsTermination = true;
                vktrace_delete_trace_packet_no_lock(&pHeader);
                vktrace_LogVerbose("Thread_CaptureTrace is exiting.");
                break;
            }

            if (pInfo->pTraceFile != NULL) {
                vktrace_enter_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);
                bytes_written = fwrite(pHeader, 1, (size_t)pHeader->size, pInfo->pTraceFile);
                fflush(pInfo->pTraceFile);
                vktrace_leave_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);
                if (bytes_written != pHeader->size) {
                    vktrace_LogError("Failed to write the packet for packet_id = %hu", pHeader->packet_id);
                }

                // If the packet is one we need to track, add it to the table
                if (pHeader->packet_id == VKTRACE_TPI_VK_vkBindImageMemory ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkBindBufferMemory ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkBindImageMemory2KHR ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkBindBufferMemory2KHR ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkAllocateMemory || pHeader->packet_id == VKTRACE_TPI_VK_vkDestroyImage ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkDestroyBuffer || pHeader->packet_id == VKTRACE_TPI_VK_vkFreeMemory ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkCreateBuffer || pHeader->packet_id == VKTRACE_TPI_VK_vkCreateImage) {
                    vktrace_LogDebug("Add packet to portability table: %s",
                                     vktrace_vk_packet_id_name((VKTRACE_TRACE_PACKET_ID_VK)pHeader->packet_id));
                    portabilityTable.push_back(fileOffset);
                }
                lastPacketIndex = pHeader->global_packet_index;
                lastPacketThreadId = pHeader->thread_id;
                lastPacketEndTime = pHeader->vktrace_end_time;
                fileOffset += bytes_written;
            }
        }

        // clean up
        vktrace_delete_trace_packet_no_lock(&pHeader);
    }

    vktrace_appendPortabilityPacket(pInfo->pTraceFile);
#if defined(WIN32)
    PostThreadMessage(pInfo->pProcessInfo->parentThreadId, VKTRACE_WM_COMPLETE, 0, 0);
#endif

    VKTRACE_DELETE(fileLikeSocket);
    vktrace_MessageStream_destroy(&pMessageStream);

// Restore signal handling to default.
#if defined(WIN32)
    rval = SetConsoleCtrlHandler((PHANDLER_ROUTINE)terminationSignalHandler, FALSE);
    assert(rval);
#else
    rval = signal(SIGHUP, SIG_DFL);
    assert(rval != SIG_ERR);
    rval = signal(SIGINT, SIG_DFL);
    assert(rval != SIG_ERR);
    rval = signal(SIGTERM, SIG_DFL);
    assert(rval != SIG_ERR);
#endif

    return 0;
}
