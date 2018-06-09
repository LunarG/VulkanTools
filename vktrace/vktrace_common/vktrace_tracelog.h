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

#pragma once

#include <stdint.h>

typedef struct FileLike FileLike;

typedef enum {
    VKTRACE_LOG_NONE = 0,
    VKTRACE_LOG_ERROR,
    VKTRACE_LOG_WARNING,
    VKTRACE_LOG_VERBOSE,
    VKTRACE_LOG_DEBUG
} VktraceLogLevel;

#ifdef __cplusplus
extern "C" {
#endif

const char* vktrace_LogLevelToString(VktraceLogLevel level);
const char* vktrace_LogLevelToShortString(VktraceLogLevel level);

void vktrace_trace_set_trace_file(FileLike* pFileLike);
FileLike* vktrace_trace_get_trace_file();
void vktrace_tracelog_set_tracer_id(uint8_t tracerId);

void vktrace_set_trace_version(uint32_t version);
BOOL vktrace_check_min_version(uint32_t version);

// Logging is done by reporting the messages back to a callback.
// Plugins should register a callback from the parent tool;
// Tools should register their own callback so that they can output messages as desired.
typedef void (*VKTRACE_REPORT_CALLBACK_FUNCTION)(VktraceLogLevel level, const char* pMsg);
extern VKTRACE_REPORT_CALLBACK_FUNCTION s_reportFunc;
extern VktraceLogLevel s_logLevel;

void vktrace_LogSetCallback(VKTRACE_REPORT_CALLBACK_FUNCTION pCallback);
void vktrace_LogSetLevel(VktraceLogLevel level);

// Allows checking if a level is being logged so that string-related functions
// can be skipped if they will not reported.
BOOL vktrace_LogIsLogging(VktraceLogLevel level);

// Always log the message, no matter what the ReportingLevel is.
void vktrace_LogAlways(const char* format, ...);

// Log debug information that is primarily helpful for Vktrace developers.
// This will also always be logged, no matter what the ReportingLevel is.
void vktrace_LogDebug(const char* format, ...);

// Log an error message.
void vktrace_LogError(const char* format, ...);

// Log a warning.
void vktrace_LogWarning(const char* format, ...);

// Log any misc information that might help a user understand what is going on.
void vktrace_LogVerbose(const char* format, ...);

#ifdef __cplusplus
}
#endif
