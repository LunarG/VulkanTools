/*
 * Copyright 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
 */

#include "vktrace_common.h"
#include "vktrace_filelike.h"
#include "vktrace_interconnect.h"
#include "vktrace_vk_vk.h"
#include "vktrace_lib_trim.h"
#include "vktrace_lib_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

// Environment variables
// These are needed because Windows may not have getenv available.
// See the Windows man page for getenv to find out why.

#if defined(_WIN32)
static inline char *vktrace_layer_getenv(const char *name) {
    char *retVal;
    DWORD valSize;
    valSize = GetEnvironmentVariableA(name, NULL, 0);
    // valSize DOES include the null terminator, so for any set variable
    // will always be at least 1. If it's 0, the variable wasn't set.
    if (valSize == 0) return NULL;
    retVal = (char *)malloc(valSize);
    GetEnvironmentVariableA(name, retVal, valSize);
    return retVal;
}

static inline void vktrace_layer_free_getenv(const char *val) { free((void *)val); }
#else
static inline char* vktrace_layer_getenv(const char* name) { return getenv(name); }

static inline void vktrace_layer_free_getenv(const char* val) {}
#endif

VKTRACER_EXIT TrapExit(void) { vktrace_LogVerbose("vktrace_lib TrapExit."); }

void loggingCallback(VktraceLogLevel level, const char *pMessage) {
    switch (level) {
        case VKTRACE_LOG_DEBUG:
            printf("vktrace debug: %s\n", pMessage);
            break;
        case VKTRACE_LOG_ERROR:
            printf("vktrace error: %s\n", pMessage);
            break;
        case VKTRACE_LOG_WARNING:
            printf("vktrace warning: %s\n", pMessage);
            break;
        case VKTRACE_LOG_VERBOSE:
            printf("vktrace info: %s\n", pMessage);
            break;
        default:
            printf("%s\n", pMessage);
            break;
    }
    fflush(stdout);

    if (vktrace_trace_get_trace_file() != NULL) {
        uint32_t requiredLength = (uint32_t)ROUNDUP_TO_4(strlen(pMessage) + 1);
        vktrace_trace_packet_header *pHeader = vktrace_create_trace_packet(VKTRACE_TID_VULKAN, VKTRACE_TPI_MESSAGE,
                                                                           sizeof(vktrace_trace_packet_message), requiredLength);
        vktrace_trace_packet_message *pPacket = vktrace_interpret_body_as_trace_packet_message(pHeader);
        pPacket->type = level;
        pPacket->length = requiredLength;

        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&pPacket->message, strlen(pMessage) + 1, pMessage);
        vktrace_finalize_buffer_address(pHeader, (void **)&pPacket->message);
        vktrace_set_packet_entrypoint_end_time(pHeader);
        vktrace_finalize_trace_packet(pHeader);

        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }

#if defined(WIN32)
#if _DEBUG
    OutputDebugString(pMessage);
#endif
#endif

#if defined(ANDROID)
#include <android/log.h>
    switch (level) {
        case VKTRACE_LOG_DEBUG:
            __android_log_print(ANDROID_LOG_DEBUG, "vktrace", "%s", pMessage);
            break;
        case VKTRACE_LOG_ERROR:
            __android_log_print(ANDROID_LOG_ERROR, "vktrace", "%s", pMessage);
            break;
        case VKTRACE_LOG_WARNING:
            __android_log_print(ANDROID_LOG_WARNING, "vktrace", "%s", pMessage);
            break;
        case VKTRACE_LOG_VERBOSE:
            __android_log_print(ANDROID_LOG_INFO, "vktrace", "%s", pMessage);
            break;
        default:
            __android_log_print(ANDROID_LOG_INFO, "vktrace", "%s", pMessage);
            break;
    }
#endif
}

extern VKTRACER_ENTRY _Load(void) {
    // only do the hooking and networking if the tracer is NOT loaded by vktrace
    if (vktrace_is_loaded_into_vktrace() == FALSE) {
        char *verbosity;
        vktrace_LogSetCallback(loggingCallback);
        verbosity = vktrace_layer_getenv(_VKTRACE_VERBOSITY_ENV);
        if (verbosity && !strcmp(verbosity, "quiet"))
            vktrace_LogSetLevel(VKTRACE_LOG_NONE);
        else if (verbosity && !strcmp(verbosity, "warnings"))
            vktrace_LogSetLevel(VKTRACE_LOG_WARNING);
        else if (verbosity && !strcmp(verbosity, "full"))
            vktrace_LogSetLevel(VKTRACE_LOG_VERBOSE);
#ifdef _DEBUG
        else if (verbosity && !strcmp(verbosity, "debug"))
            vktrace_LogSetLevel(VKTRACE_LOG_DEBUG);
#endif
        else
            // Either verbosity=="errors", or it wasn't specified
            vktrace_LogSetLevel(VKTRACE_LOG_ERROR);

        vktrace_layer_free_getenv(verbosity);

        vktrace_LogVerbose("vktrace_lib library loaded into PID %d", vktrace_get_pid());
        atexit(TrapExit);

// If you need to debug startup, build with this set to true, then attach and change it to false.
#ifdef _DEBUG
        {
            bool debugStartup = false;
            while (debugStartup)
                ;
        }
#endif
    }
}

#if defined(WIN32)
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    hModule;
    lpReserved;

    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            _Load();
            break;
        }
        case DLL_PROCESS_DETACH: {
            _Unload();
            break;
        }
        default:
            break;
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
}
#endif
