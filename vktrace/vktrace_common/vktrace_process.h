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

typedef struct vktrace_process_info {
    char* exeName;
    char* processArgs;
    char* fullProcessCmdLine;
    char* workingDirectory;
    char* traceFilename;

    // vktrace's thread id
    vktrace_thread_id parentThreadId;

    VKTRACE_CRITICAL_SECTION traceFileCriticalSection;
    BOOL traceFileCriticalSectionCreated;

    uint32_t maxCaptureThreadsNumber;
    uint32_t currentCaptureThreadsCount;
    vktrace_process_capture_trace_thread_info* pCaptureThreads;
    struct MessageStream* messageStream;
    // process id, handle, and main thread
    vktrace_process_id processId;
    vktrace_process_handle hProcess;
    vktrace_thread hThread;
    vktrace_thread watchdogThread;
} vktrace_process_info;

typedef struct vktrace_process_tracer_dll {
    char* dllPath;
    BOOL bLoaded;
    VKTRACE_TRACER_ID tid;
} vktrace_process_tracer_dll;

struct vktrace_process_capture_trace_thread_info {
    vktrace_thread recordingThread;
    vktrace_process_info* pProcessInfo;
    VKTRACE_TRACER_ID tracerId;
    FILE* pTraceFile;
    uint32_t traceFileIndex;  // The index of the record thread trace file
                              // within all trace files of the current
                              // target title. Starting one title may cause
                              // multiple processes be created, every process
                              // will has a record thread running on server
                              // side writing to a trace file, any record
                              // thread has an index in this struct based on
                              // the position of its trace file creation within
                              // all trace file creations by the create order.
                              // For example, some title has several record
                              // threads running in server, according to the
                              // creation order, traceFileIndex will be 0
                              // for the first created trace file.
    BOOL serverRequestsTermination;
};

BOOL vktrace_process_spawn(vktrace_process_info* pInfo);
void vktrace_process_info_delete(vktrace_process_info* pInfo);
BOOL GetTraceFileName(char* trace_file_name, const uint32_t trace_file_name_buffer_size, const char* base_name,
                      const int trace_file_index);