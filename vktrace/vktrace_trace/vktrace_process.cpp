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

    DWORD rv;
    while (WaitForSingleObject(pProcInfo->hProcess, kWatchDogPollTime) == WAIT_TIMEOUT) {
        if (pProcInfo->serverRequestsTermination) {
            vktrace_LogVerbose("Vktrace has requested exit.");
            return 0;
        }
    }

    vktrace_LogVerbose("Child process has terminated.");
    GetExitCodeProcess(pProcInfo->hProcess, &rv);
    PostThreadMessage(pProcInfo->parentThreadId, VKTRACE_WM_COMPLETE, rv, 0);
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

// ------------------------------------------------------------------------------------------------
VKTRACE_THREAD_ROUTINE_RETURN_TYPE Process_RunRecordTraceThread(LPVOID _threadInfo) {
    vktrace_process_capture_trace_thread_info* pInfo = (vktrace_process_capture_trace_thread_info*)_threadInfo;
    FileLike* fileLikeSocket;
    uint64_t fileHeaderSize;
    vktrace_trace_file_header file_header;
    vktrace_trace_packet_header* pHeader = NULL;
    size_t bytes_written;
    size_t fileOffset;
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
    }

    // create trace file
    pInfo->pProcessInfo->pTraceFile = vktrace_open_trace_file(pInfo->pProcessInfo);

    if (pInfo->pProcessInfo->pTraceFile == NULL) {
        // open of trace file generated an error, no sense in continuing.
        vktrace_LogError("Error cannot create trace file.");
        vktrace_process_info_delete(pInfo->pProcessInfo);
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
        vktrace_process_info_delete(pInfo->pProcessInfo);
        return 1;
    }

    vktrace_enter_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);

    // Write the trace file header to the file
    bytes_written = fwrite(&file_header, 1, sizeof(file_header), pInfo->pProcessInfo->pTraceFile);

    // Read and write the gpu_info structs
    struct_gpuinfo gpuinfo;
    for (uint64_t i = 0; i < file_header.n_gpuinfo; i++) {
        vktrace_FileLike_ReadRaw(fileLikeSocket, &gpuinfo, sizeof(struct_gpuinfo));
        bytes_written += fwrite(&gpuinfo, 1, sizeof(struct_gpuinfo), pInfo->pProcessInfo->pTraceFile);
    }
    fflush(pInfo->pProcessInfo->pTraceFile);
    vktrace_leave_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);

    if (bytes_written != sizeof(file_header) + file_header.n_gpuinfo * sizeof(struct_gpuinfo)) {
        vktrace_LogError("Unable to write trace file header - fwrite failed.");
        vktrace_process_info_delete(pInfo->pProcessInfo);
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
                vktrace_delete_trace_packet(&pHeader);
                vktrace_LogVerbose("Thread_CaptureTrace is exiting.");
                break;
            }

            if (pInfo->pProcessInfo->pTraceFile != NULL) {
                vktrace_enter_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);
                bytes_written = fwrite(pHeader, 1, (size_t)pHeader->size, pInfo->pProcessInfo->pTraceFile);
                fflush(pInfo->pProcessInfo->pTraceFile);
                vktrace_leave_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);
                if (bytes_written != pHeader->size) {
                    vktrace_LogError("Failed to write the packet for packet_id = %hu", pHeader->packet_id);
                }

                // If the packet is one we need to track, add it to the table
                if (pHeader->packet_id == VKTRACE_TPI_VK_vkBindImageMemory ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkBindBufferMemory ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkAllocateMemory || pHeader->packet_id == VKTRACE_TPI_VK_vkDestroyImage ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkDestroyBuffer || pHeader->packet_id == VKTRACE_TPI_VK_vkFreeMemory ||
                    pHeader->packet_id == VKTRACE_TPI_VK_vkCreateBuffer || pHeader->packet_id == VKTRACE_TPI_VK_vkCreateImage) {
                    portabilityTable.push_back(fileOffset);
                }
                lastPacketIndex = pHeader->global_packet_index;
                lastPacketThreadId = pHeader->thread_id;
                lastPacketEndTime = pHeader->vktrace_end_time;
                fileOffset += bytes_written;
            }
        }

        // clean up
        vktrace_delete_trace_packet(&pHeader);
    }

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
