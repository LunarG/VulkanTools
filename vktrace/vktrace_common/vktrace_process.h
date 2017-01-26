/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
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
 **************************************************************************/
#pragma once

#include "vktrace_platform.h"
#include "vktrace_trace_packet_identifiers.h"

typedef struct vktrace_process_capture_trace_thread_info vktrace_process_capture_trace_thread_info;

typedef struct vktrace_process_info
{
    char* exeName;
    char* processArgs;
    char* fullProcessCmdLine;
    char* workingDirectory;
    char* traceFilename;
    FILE* pTraceFile;

    // vktrace's thread id
    vktrace_thread_id parentThreadId;

    VKTRACE_CRITICAL_SECTION traceFileCriticalSection;

    volatile BOOL serverRequestsTermination;

    vktrace_process_capture_trace_thread_info* pCaptureThreads;

    // process id, handle, and main thread
    vktrace_process_id processId;
    vktrace_process_handle hProcess;
    vktrace_thread hThread;
    vktrace_thread watchdogThread;
} vktrace_process_info;


typedef struct vktrace_process_tracer_dll
{
    char * dllPath;
    BOOL bLoaded;
    VKTRACE_TRACER_ID tid;
} vktrace_process_tracer_dll;

struct vktrace_process_capture_trace_thread_info
{
    vktrace_thread recordingThread;
    vktrace_process_info* pProcessInfo;
    VKTRACE_TRACER_ID tracerId;
};

BOOL vktrace_process_spawn(vktrace_process_info* pInfo);
void vktrace_process_info_delete(vktrace_process_info* pInfo);
