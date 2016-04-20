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

extern "C" {
#include "vktrace_filelike.h"
#include "vktrace_interconnect.h"
#include "vktrace_trace_packet_utils.h"
}

const unsigned long kWatchDogPollTime = 250;

#if defined(WIN32)
void SafeCloseHandle(HANDLE& _handle)
{
    if (_handle) {
        CloseHandle(_handle);
        _handle = NULL;
    }
}
#endif

// ------------------------------------------------------------------------------------------------
VKTRACE_THREAD_ROUTINE_RETURN_TYPE Process_RunWatchdogThread(LPVOID _procInfoPtr)
{
    vktrace_process_info* pProcInfo = (vktrace_process_info*)_procInfoPtr;

#if defined(WIN32)

    while (WaitForSingleObject(pProcInfo->hProcess, kWatchDogPollTime) == WAIT_TIMEOUT)
    {
        if (pProcInfo->serverRequestsTermination)
        {
            vktrace_LogVerbose("Vktrace has requested exit.");
            return 0;
        }
    }

    vktrace_LogVerbose("Child process has terminated.");

    PostThreadMessage(pProcInfo->parentThreadId, VKTRACE_WM_COMPLETE, 0, 0);
    pProcInfo->serverRequestsTermination = TRUE;
    
#elif defined(PLATFORM_LINUX)
    int status = 0;
    int options = 0;
    while (waitpid(pProcInfo->processId, &status, options) != -1)
    {
        if (WIFEXITED(status))
        {
            vktrace_LogVerbose("Child process exited.");
            break;
        }
        else if (WCOREDUMP(status))
        {
            vktrace_LogError("Child process crashed.");
            break;
        }
        else if (WIFSIGNALED(status))
            vktrace_LogVerbose("Child process was signaled.");
        else if (WIFSTOPPED(status))
            vktrace_LogVerbose("Child process was stopped.");
        else if (WIFCONTINUED(status))
            vktrace_LogVerbose("Child process was continued.");
    }
#endif
    return 0;
}

// ------------------------------------------------------------------------------------------------
VKTRACE_THREAD_ROUTINE_RETURN_TYPE Process_RunRecordTraceThread(LPVOID _threadInfo)
{
    vktrace_process_capture_trace_thread_info* pInfo = (vktrace_process_capture_trace_thread_info*)_threadInfo;

    MessageStream* pMessageStream = vktrace_MessageStream_create(TRUE, "", VKTRACE_BASE_PORT + pInfo->tracerId);
    if (pMessageStream == NULL)
    {
        vktrace_LogError("Thread_CaptureTrace() cannot create message stream.");
        return 1;
    }

    // create trace file
    pInfo->pProcessInfo->pTraceFile = vktrace_write_trace_file_header(pInfo->pProcessInfo);

    if (pInfo->pProcessInfo->pTraceFile == NULL) {
        // writing trace file generated an error, no sense in continuing.
        vktrace_LogError("Error cannot create trace file and write header.");
        vktrace_process_info_delete(pInfo->pProcessInfo);
        return 1;
    }

    FileLike* fileLikeSocket = vktrace_FileLike_create_msg(pMessageStream);
    unsigned int total_packet_count = 0;
    vktrace_trace_packet_header* pHeader = NULL;
    size_t bytes_written;

    while (pInfo->pProcessInfo->serverRequestsTermination == FALSE)
    {
        // get a packet
        //vktrace_LogDebug("Waiting for a packet...");

        // read entire packet in
        pHeader = vktrace_read_trace_packet(fileLikeSocket);
        ++total_packet_count;
        if (pHeader == NULL)
        {
            if (pMessageStream->mErrorNum == WSAECONNRESET)
            {
                vktrace_LogError("Network Connection Reset");
            }
            else
            {
                vktrace_LogError("Network Connection Failed");
            }
            break;
        }

        //vktrace_LogDebug("Received packet id: %hu", pHeader->packet_id);
        
        if (pHeader->pBody == (uintptr_t) NULL)
        {
            vktrace_LogWarning("Received empty packet body for id: %hu", pHeader->packet_id);
        }
        else
        {
            // handle special case packets
            if (pHeader->packet_id == VKTRACE_TPI_MESSAGE)
            {
                if (g_settings.print_trace_messages == TRUE)
                {
                    vktrace_trace_packet_message* pPacket = vktrace_interpret_body_as_trace_packet_message(pHeader);
                    vktrace_LogAlways("Packet %lu: Traced Message (%s): %s", pHeader->global_packet_index, vktrace_LogLevelToShortString(pPacket->type), pPacket->message);
                    vktrace_finalize_buffer_address(pHeader, (void **) &(pPacket->message));
                }
            }

            if (pHeader->packet_id == VKTRACE_TPI_MARKER_TERMINATE_PROCESS)
            {
                pInfo->pProcessInfo->serverRequestsTermination = true;
                vktrace_delete_trace_packet(&pHeader);
                vktrace_LogVerbose("Thread_CaptureTrace is exiting.");
                break;
            }

            if (pInfo->pProcessInfo->pTraceFile != NULL)
            {
                vktrace_enter_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);
                bytes_written = fwrite(pHeader, 1, (size_t)pHeader->size, pInfo->pProcessInfo->pTraceFile);
                fflush(pInfo->pProcessInfo->pTraceFile);
                vktrace_leave_critical_section(&pInfo->pProcessInfo->traceFileCriticalSection);
                if (bytes_written != pHeader->size)
                {
                    vktrace_LogError("Failed to write the packet for packet_id = %hu", pHeader->packet_id);
                }
            }
        }

        // clean up
        vktrace_delete_trace_packet(&pHeader);
    }

    VKTRACE_DELETE(fileLikeSocket);
    vktrace_MessageStream_destroy(&pMessageStream);

    return 0;
}
