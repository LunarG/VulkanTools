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

#include "vktrace_platform.h"

#include "vktrace_tracelog.h"
#include "vktrace_trace_packet_utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

// filelike thing that is used for outputting trace messages
static FileLike* g_pFileOut = NULL;

VKTRACE_TRACER_ID g_tracelog_tracer_id = VKTRACE_TID_RESERVED;

void vktrace_trace_set_trace_file(FileLike* pFileLike) { g_pFileOut = pFileLike; }

// set initial value to 0 but once we read the trace file version
// we will update this and use for version checks
static uint32_t g_trace_version_num = 0;

void vktrace_set_trace_version(uint32_t version) { g_trace_version_num = version; }

BOOL vktrace_check_min_version(uint32_t version) { return ((g_trace_version_num) >= (version) ? true : false); }

FileLike* vktrace_trace_get_trace_file() { return g_pFileOut; }

void vktrace_tracelog_set_tracer_id(uint8_t tracerId) { g_tracelog_tracer_id = (VKTRACE_TRACER_ID)tracerId; }

VKTRACE_REPORT_CALLBACK_FUNCTION s_reportFunc;
VktraceLogLevel s_logLevel = VKTRACE_LOG_ERROR;

const char* vktrace_LogLevelToString(VktraceLogLevel level) {
    switch (level) {
        case VKTRACE_LOG_NONE:
            return "Quiet";
        case VKTRACE_LOG_DEBUG:
            return "Debug";
        case VKTRACE_LOG_ERROR:
            return "Errors";
        case VKTRACE_LOG_WARNING:
            return "Warnings";
        case VKTRACE_LOG_VERBOSE:
            return "Info";
        default:
            return "Unknown";
    }
}

const char* vktrace_LogLevelToShortString(VktraceLogLevel level) {
    switch (level) {
        case VKTRACE_LOG_NONE:
            return "Quiet";
        case VKTRACE_LOG_DEBUG:
            return "Debug";
        case VKTRACE_LOG_ERROR:
            return "Errors";
        case VKTRACE_LOG_WARNING:
            return "Warnings";
        case VKTRACE_LOG_VERBOSE:
            return "Info";
        default:
            return "Unknown";
    }
}

// For use by both tools and libraries.
void vktrace_LogSetCallback(VKTRACE_REPORT_CALLBACK_FUNCTION pCallback) { s_reportFunc = pCallback; }

void vktrace_LogSetLevel(VktraceLogLevel level) {
    vktrace_LogDebug("Log Level = %u (%s)", level, vktrace_LogLevelToString(level));
    s_logLevel = level;
}

BOOL vktrace_LogIsLogging(VktraceLogLevel level) { return (level <= s_logLevel) ? TRUE : FALSE; }

void LogGuts(VktraceLogLevel level, const char* fmt, va_list args) {
#if defined(WIN32)
    int requiredLength = _vscprintf(fmt, args) + 1;
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    int requiredLength;
    va_list argcopy;
    va_copy(argcopy, args);
    requiredLength = vsnprintf(NULL, 0, fmt, argcopy) + 1;
    va_end(argcopy);
#endif
    static VKTRACE_THREAD_LOCAL BOOL logging = FALSE;

    // Don't recursively log problems found during logging
    if (logging) {
        return;
    }
    logging = TRUE;

    char* message = (char*)vktrace_malloc(requiredLength);
#if defined(WIN32)
    _vsnprintf_s(message, requiredLength, requiredLength - 1, fmt, args);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    vsnprintf(message, requiredLength, fmt, args);
#endif

    if (s_reportFunc != NULL) {
        s_reportFunc(level, message);
    } else {
#ifdef ANDROID
#include <android/log.h>
        __android_log_print(ANDROID_LOG_INFO, "vktrace", "%s: %s\n", vktrace_LogLevelToString(level), message);
#else
        printf("%s: %s\n", vktrace_LogLevelToString(level), message);
#endif
    }

    vktrace_free(message);
    logging = FALSE;
}

void vktrace_LogAlways(const char* format, ...) {
    va_list args;
    va_start(args, format);
    LogGuts(VKTRACE_LOG_VERBOSE, format, args);
    va_end(args);
}

void vktrace_LogDebug(const char* format, ...) {
    if (vktrace_LogIsLogging(VKTRACE_LOG_DEBUG)) {
        va_list args;
        va_start(args, format);
        LogGuts(VKTRACE_LOG_DEBUG, format, args);
        va_end(args);
    }
}

void vktrace_LogError(const char* format, ...) {
    if (vktrace_LogIsLogging(VKTRACE_LOG_ERROR)) {
        va_list args;
        va_start(args, format);
        LogGuts(VKTRACE_LOG_ERROR, format, args);
        va_end(args);
    }
}

void vktrace_LogWarning(const char* format, ...) {
    if (vktrace_LogIsLogging(VKTRACE_LOG_WARNING)) {
        va_list args;
        va_start(args, format);
        LogGuts(VKTRACE_LOG_WARNING, format, args);
        va_end(args);
    }
}

void vktrace_LogVerbose(const char* format, ...) {
    if (vktrace_LogIsLogging(VKTRACE_LOG_VERBOSE)) {
        va_list args;
        va_start(args, format);
        LogGuts(VKTRACE_LOG_VERBOSE, format, args);
        va_end(args);
    }
}
