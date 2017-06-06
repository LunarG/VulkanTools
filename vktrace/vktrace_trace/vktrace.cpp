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
#include "vktrace.h"

#include "vktrace_process.h"

extern "C" {
#include "vktrace_common.h"
#include "vktrace_filelike.h"
#include "vktrace_interconnect.h"
#include "vktrace_trace_packet_identifiers.h"
#include "vktrace_trace_packet_utils.h"
}

#include <sys/types.h>
#include <sys/stat.h>
#include "screenshot_parsing.h"

vktrace_settings g_settings;
vktrace_settings g_default_settings;

vktrace_SettingInfo g_settings_info[] = {
    // common command options
    {"p", "Program", VKTRACE_SETTING_STRING, {&g_settings.program}, {&g_default_settings.program}, TRUE, "The program to trace."},
    {"a",
     "Arguments",
     VKTRACE_SETTING_STRING,
     {&g_settings.arguments},
     {&g_default_settings.arguments},
     TRUE,
     "Command line arguments to pass to trace program."},
    {"w",
     "WorkingDir",
     VKTRACE_SETTING_STRING,
     {&g_settings.working_dir},
     {&g_default_settings.working_dir},
     TRUE,
     "The program's working directory."},
    {"o",
     "OutputTrace",
     VKTRACE_SETTING_STRING,
     {&g_settings.output_trace},
     {&g_default_settings.output_trace},
     TRUE,
     "Path to the generated output trace file."},
    {"s",
     "ScreenShot",
     VKTRACE_SETTING_STRING,
     {&g_settings.screenshotList},
     {&g_default_settings.screenshotList},
     TRUE,
     "Generate screenshots. <string> is one of:\n\
                                         comma separated list of frames\n\
                                         <start>-<count>-<interval>\n\
                                         \"all\""},
    {"sf",
     "ScreenshotFormat",
     VKTRACE_SETTING_STRING,
     {&g_settings.screenshotColorFormat},
     {&g_default_settings.screenshotColorFormat},
     TRUE,
     "Color Space format of screenshot files. Formats are UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB"},
    {"ptm",
     "PrintTraceMessages",
     VKTRACE_SETTING_BOOL,
     {&g_settings.print_trace_messages},
     {&g_default_settings.print_trace_messages},
     TRUE,
     "Print trace messages to vktrace console."},
    {"P",
     "PMB",
     VKTRACE_SETTING_BOOL,
     {&g_settings.enable_pmb},
     {&g_default_settings.enable_pmb},
     TRUE,
     "Enable tracking of persistently mapped buffers, default is TRUE."},
#if _DEBUG
    {"v",
     "Verbosity",
     VKTRACE_SETTING_STRING,
     {&g_settings.verbosity},
     {&g_default_settings.verbosity},
     TRUE,
     "Verbosity mode. Modes are \"quiet\", \"errors\", \"warnings\", \"full\", "
     "\"debug\"."},
#else
    {"v",
     "Verbosity",
     VKTRACE_SETTING_STRING,
     {&g_settings.verbosity},
     {&g_default_settings.verbosity},
     TRUE,
     "Verbosity mode. Modes are \"quiet\", \"errors\", \"warnings\", "
     "\"full\"."},
#endif

    {"tr",
     "TraceTrigger",
     VKTRACE_SETTING_STRING,
     {&g_settings.traceTrigger},
     {&g_default_settings.traceTrigger},
     TRUE,
     "(Alpha) Start/stop trim by hotkey or frame range:\n\
                                         hotkey-<keyname>\n\
                                         frames-<startFrame>-<endFrame>"},
    //{ "z", "pauze", VKTRACE_SETTING_BOOL, &g_settings.pause,
    //&g_default_settings.pause, TRUE, "Wait for a key at startup (so a debugger
    // can be attached)" },
};

vktrace_SettingGroup g_settingGroup = {"vktrace", sizeof(g_settings_info) / sizeof(g_settings_info[0]), &g_settings_info[0]};

// ------------------------------------------------------------------------------------------------
#if defined(WIN32)
uint64_t MessageLoop() {
    MSG msg = {0};
    bool quit = false;
    while (!quit) {
        if (GetMessage(&msg, NULL, 0, 0) == FALSE) {
            quit = true;
        } else {
            quit = (msg.message == VKTRACE_WM_COMPLETE);
        }
    }
    return msg.wParam;
}
#endif

int PrepareTracers(vktrace_process_capture_trace_thread_info** ppTracerInfo) {
    unsigned int num_tracers = 1;

    assert(ppTracerInfo != NULL && *ppTracerInfo == NULL);
    *ppTracerInfo = VKTRACE_NEW_ARRAY(vktrace_process_capture_trace_thread_info, num_tracers);
    memset(*ppTracerInfo, 0, sizeof(vktrace_process_capture_trace_thread_info) * num_tracers);

    // we only support Vulkan tracer
    (*ppTracerInfo)[0].tracerId = VKTRACE_TID_VULKAN;

    return num_tracers;
}

bool InjectTracersIntoProcess(vktrace_process_info* pInfo) {
    bool bRecordingThreadsCreated = true;
    vktrace_thread tracingThread;
    if (vktrace_platform_remote_load_library(pInfo->hProcess, NULL, &tracingThread, NULL)) {
        // prepare data for capture threads
        pInfo->pCaptureThreads[0].pProcessInfo = pInfo;
        pInfo->pCaptureThreads[0].recordingThread = VKTRACE_NULL_THREAD;

        // create thread to record trace packets from the tracer
        pInfo->pCaptureThreads[0].recordingThread =
            vktrace_platform_create_thread(Process_RunRecordTraceThread, &(pInfo->pCaptureThreads[0]));
        if (pInfo->pCaptureThreads[0].recordingThread == VKTRACE_NULL_THREAD) {
            vktrace_LogError("Failed to create trace recording thread.");
            bRecordingThreadsCreated = false;
        }

    } else {
        // failed to inject a DLL
        bRecordingThreadsCreated = false;
    }
    return bRecordingThreadsCreated;
}

void loggingCallback(VktraceLogLevel level, const char* pMessage) {
    if (level == VKTRACE_LOG_NONE) return;

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

#if defined(WIN32)
#if _DEBUG
    OutputDebugString(pMessage);
#endif
#endif
}

// ------------------------------------------------------------------------------------------------
char* append_index_to_filename(const char* base, uint32_t index, const char* extension) {
    char num[17];
#ifdef PLATFORM_LINUX
    snprintf(num, 17, "-%u", index);
#elif defined(WIN32)
    _snprintf_s(num, 17, _TRUNCATE, "-%u", index);
#endif
    return vktrace_copy_and_append(base, num, extension);
}

// ------------------------------------------------------------------------------------------------
static uint32_t s_fileIndex = 0;
char* find_available_filename(const char* originalFilename, bool bForceOverwrite) {
    char* pOutputFilename = NULL;

    if (bForceOverwrite) {
        if (s_fileIndex == 0) {
            pOutputFilename = vktrace_allocate_and_copy(g_settings.output_trace);
        } else {
            const char* pExtension = strrchr(g_settings.output_trace, '.');
            char* basename = vktrace_allocate_and_copy_n(
                g_settings.output_trace,
                (int)((pExtension == NULL) ? strlen(g_settings.output_trace) : pExtension - g_settings.output_trace));
            pOutputFilename = append_index_to_filename(basename, s_fileIndex, pExtension);
            vktrace_free(basename);
        }
    } else  // don't overwrite
    {
        const char* pExtension = strrchr(g_settings.output_trace, '.');
        char* basename = vktrace_allocate_and_copy_n(
            g_settings.output_trace,
            (int)((pExtension == NULL) ? strlen(g_settings.output_trace) : pExtension - g_settings.output_trace));
        pOutputFilename = vktrace_allocate_and_copy(g_settings.output_trace);
        FILE* pFile = NULL;
        while ((pFile = fopen(pOutputFilename, "rb")) != NULL) {
            fclose(pFile);
            ++s_fileIndex;

            vktrace_free(pOutputFilename);
            pOutputFilename = append_index_to_filename(basename, s_fileIndex, pExtension);
        }
        vktrace_free(basename);
    }

    // increment to the next available fileIndex to prep for the next trace file
    ++s_fileIndex;
    return pOutputFilename;
}

// Portability table - Table of trace file offsets to packets
// we need to access to determine what memory index should be used
// in vkAllocateMemory during trace playback. This table is appended
// to the trace file.
std::vector<size_t> portabilityTable;
uint32_t lastPacketThreadId;
uint64_t lastPacketIndex;
uint64_t lastPacketEndTime;

static void vktrace_appendPortabilityPacket(FILE* pTraceFile) {
    vktrace_trace_packet_header hdr;
    uint64_t one_64 = 1;

    if (pTraceFile == NULL) {
        vktrace_LogError("tracefile was not created");
        return;
    }

    vktrace_LogVerbose("Post processing trace file");

    // Add a word containing the size of the table to the table.
    // This will be the last word in the file.
    portabilityTable.push_back(portabilityTable.size());

    // Append the table packet to the trace file.
    hdr.size = sizeof(hdr) + portabilityTable.size() * sizeof(size_t);
    hdr.global_packet_index = lastPacketIndex + 1;
    hdr.tracer_id = VKTRACE_TID_VULKAN;
    hdr.packet_id = VKTRACE_TPI_PORTABILITY_TABLE;
    hdr.thread_id = lastPacketThreadId;
    hdr.vktrace_begin_time = hdr.entrypoint_begin_time = hdr.entrypoint_end_time = hdr.vktrace_end_time = lastPacketEndTime;
    hdr.next_buffers_offset = 0;
    hdr.pBody = (uintptr_t)NULL;
    if (0 == fseek(pTraceFile, 0, SEEK_END) && 1 == fwrite(&hdr, sizeof(hdr), 1, pTraceFile) &&
        portabilityTable.size() == fwrite(&portabilityTable[0], sizeof(size_t), portabilityTable.size(), pTraceFile)) {
        // Set the flag in the file header that indicates the portability table has been written
        if (0 == fseek(pTraceFile, offsetof(vktrace_trace_file_header, portability_table_valid), SEEK_SET))
            fwrite(&one_64, sizeof(uint64_t), 1, pTraceFile);
    }
    portabilityTable.clear();
    vktrace_LogVerbose("Post processing of trace file completed");
}

// ------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    uint64_t exitval = 0;
    memset(&g_settings, 0, sizeof(vktrace_settings));

    vktrace_LogSetCallback(loggingCallback);
    vktrace_LogSetLevel(VKTRACE_LOG_ERROR);

    // setup defaults
    memset(&g_default_settings, 0, sizeof(vktrace_settings));
    g_default_settings.output_trace = vktrace_allocate_and_copy("vktrace_out.vktrace");
    g_default_settings.verbosity = "errors";
    g_default_settings.screenshotList = NULL;
    g_default_settings.screenshotColorFormat = NULL;
    g_default_settings.enable_pmb = true;

    // Check to see if the PAGEGUARD_PAGEGUARD_ENABLE_ENV env var is set.
    // If it is set to anything but "1", set the default to false.
    // Note that the command line option will override the env variable.
    char* pmbEnableEnv = vktrace_get_global_var(VKTRACE_PMB_ENABLE_ENV);
    if (pmbEnableEnv && strcmp(pmbEnableEnv, "1")) g_default_settings.enable_pmb = false;

    if (vktrace_SettingGroup_init(&g_settingGroup, NULL, argc, argv, &g_settings.arguments) != 0) {
        // invalid cmd-line parameters
        vktrace_SettingGroup_delete(&g_settingGroup);
        vktrace_free(g_default_settings.output_trace);
        return -1;
    } else {
        // Validate vktrace inputs
        BOOL validArgs = TRUE;

        if (g_settings.output_trace == NULL || strlen(g_settings.output_trace) == 0) {
            validArgs = FALSE;
        }

        if (strcmp(g_settings.verbosity, "quiet") == 0)
            vktrace_LogSetLevel(VKTRACE_LOG_NONE);
        else if (strcmp(g_settings.verbosity, "errors") == 0)
            vktrace_LogSetLevel(VKTRACE_LOG_ERROR);
        else if (strcmp(g_settings.verbosity, "warnings") == 0)
            vktrace_LogSetLevel(VKTRACE_LOG_WARNING);
        else if (strcmp(g_settings.verbosity, "full") == 0)
            vktrace_LogSetLevel(VKTRACE_LOG_VERBOSE);
#if _DEBUG
        else if (strcmp(g_settings.verbosity, "debug") == 0)
            vktrace_LogSetLevel(VKTRACE_LOG_DEBUG);
#endif
        else {
            vktrace_LogSetLevel(VKTRACE_LOG_ERROR);
            validArgs = FALSE;
        }
        vktrace_set_global_var(_VKTRACE_VERBOSITY_ENV, g_settings.verbosity);

        if (g_settings.screenshotList) {
            if (!screenshot::checkParsingFrameRange(g_settings.screenshotList)) {
                vktrace_LogError("Screenshot range error");
                validArgs = FALSE;
            } else {
                // Export list to screenshot layer
                vktrace_set_global_var("VK_SCREENSHOT_FRAMES", g_settings.screenshotList);
            }
        } else {
            vktrace_set_global_var("VK_SCREENSHOT_FRAMES", "");
        }

        // Set up environment for screenshot color space format
        if (g_settings.screenshotColorFormat != NULL && g_settings.screenshotList != NULL) {
            vktrace_set_global_var("VK_SCREENSHOT_FORMAT", g_settings.screenshotColorFormat);
        }else if (g_settings.screenshotColorFormat != NULL && g_settings.screenshotList == NULL) {
            vktrace_LogWarning("Screenshot format should be used when screenshot enabled!");
            vktrace_set_global_var("VK_SCREENSHOT_FORMAT", "");
        } else {
            vktrace_set_global_var("VK_SCREENSHOT_FORMAT", "");
        }

        if (validArgs == FALSE) {
            vktrace_SettingGroup_print(&g_settingGroup);
            return -1;
        }

        if (g_settings.program == NULL || strlen(g_settings.program) == 0) {
            vktrace_LogWarning("No program (-p) parameter found: Will run vktrace as server.");
            printf("Running vktrace as server...\n");
            fflush(stdout);
            g_settings.arguments = NULL;
        } else {
            if (g_settings.working_dir == NULL || strlen(g_settings.working_dir) == 0) {
                CHAR* buf = VKTRACE_NEW_ARRAY(CHAR, 4096);
                vktrace_LogVerbose("No working directory (-w) parameter found: Assuming executable's path as working directory.");
                vktrace_platform_full_path(g_settings.program, 4096, buf);
                g_settings.working_dir = vktrace_platform_extract_path(buf);
                VKTRACE_DELETE(buf);
            }

            vktrace_LogVerbose("Running vktrace as parent process will spawn child process: %s", g_settings.program);
            if (g_settings.arguments != NULL && strlen(g_settings.arguments) > 0) {
                vktrace_LogVerbose("Args to be passed to child process: '%s'", g_settings.arguments);
            }
        }
    }

    vktrace_set_global_var(VKTRACE_PMB_ENABLE_ENV, g_settings.enable_pmb ? "1" : "0");

    if (g_settings.traceTrigger) {
        // Export list to screenshot layer
        vktrace_set_global_var(VKTRACE_TRIM_TRIGGER_ENV, g_settings.traceTrigger);
    } else {
        vktrace_set_global_var(VKTRACE_TRIM_TRIGGER_ENV, "");
    }

    unsigned int serverIndex = 0;
    do {
        // Create and start the process or run in server mode

        BOOL procStarted = TRUE;
        vktrace_process_info procInfo;
        memset(&procInfo, 0, sizeof(vktrace_process_info));
        if (g_settings.program != NULL) {
            procInfo.exeName = vktrace_allocate_and_copy(g_settings.program);
            procInfo.processArgs = vktrace_allocate_and_copy(g_settings.arguments);
            procInfo.fullProcessCmdLine = vktrace_copy_and_append(g_settings.program, " ", g_settings.arguments);
            procInfo.workingDirectory = vktrace_allocate_and_copy(g_settings.working_dir);
            procInfo.traceFilename = vktrace_allocate_and_copy(g_settings.output_trace);
        } else {
            procInfo.traceFilename = find_available_filename(g_settings.output_trace, true);
        }

        procInfo.parentThreadId = vktrace_platform_get_thread_id();

        // setup tracer, only Vulkan tracer suppported
        PrepareTracers(&procInfo.pCaptureThreads);

        if (g_settings.program != NULL) {
            char* instEnv = vktrace_get_global_var("VK_INSTANCE_LAYERS");
            // Add ScreenShot layer if enabled
            if (g_settings.screenshotList && (!instEnv || !strstr(instEnv, "VK_LAYER_LUNARG_screenshot"))) {
                if (!instEnv || strlen(instEnv) == 0)
                    vktrace_set_global_var("VK_INSTANCE_LAYERS", "VK_LAYER_LUNARG_screenshot");
                else {
                    char* newEnv = vktrace_copy_and_append(instEnv, VKTRACE_LIST_SEPARATOR, "VK_LAYER_LUNARG_screenshot");
                    vktrace_set_global_var("VK_INSTANCE_LAYERS", newEnv);
                }
                instEnv = vktrace_get_global_var("VK_INSTANCE_LAYERS");
            }
            char* devEnv = vktrace_get_global_var("VK_DEVICE_LAYERS");
            if (g_settings.screenshotList && (!devEnv || !strstr(devEnv, "VK_LAYER_LUNARG_screenshot"))) {
                if (!devEnv || strlen(devEnv) == 0)
                    vktrace_set_global_var("VK_DEVICE_LAYERS", "VK_LAYER_LUNARG_screenshot");
                else {
                    char* newEnv = vktrace_copy_and_append(devEnv, VKTRACE_LIST_SEPARATOR, "VK_LAYER_LUNARG_screenshot");
                    vktrace_set_global_var("VK_DEVICE_LAYERS", newEnv);
                }
                devEnv = vktrace_get_global_var("VK_DEVICE_LAYERS");
            }
            // Add vktrace_layer enable env var if needed
            if (!instEnv || strlen(instEnv) == 0) {
                vktrace_set_global_var("VK_INSTANCE_LAYERS", "VK_LAYER_LUNARG_vktrace");
            } else if (instEnv != strstr(instEnv, "VK_LAYER_LUNARG_vktrace")) {
                char* newEnv = vktrace_copy_and_append("VK_LAYER_LUNARG_vktrace", VKTRACE_LIST_SEPARATOR, instEnv);
                vktrace_set_global_var("VK_INSTANCE_LAYERS", newEnv);
            }
            if (!devEnv || strlen(devEnv) == 0) {
                vktrace_set_global_var("VK_DEVICE_LAYERS", "VK_LAYER_LUNARG_vktrace");
            } else if (devEnv != strstr(devEnv, "VK_LAYER_LUNARG_vktrace")) {
                char* newEnv = vktrace_copy_and_append("VK_LAYER_LUNARG_vktrace", VKTRACE_LIST_SEPARATOR, devEnv);
                vktrace_set_global_var("VK_DEVICE_LAYERS", newEnv);
            }
            // call CreateProcess to launch the application
            procStarted = vktrace_process_spawn(&procInfo);
        }
        if (procStarted == FALSE) {
            vktrace_LogError("Failed to set up remote process.");
            exit(1);
        } else {
            if (InjectTracersIntoProcess(&procInfo) == FALSE) {
                vktrace_LogError("Failed to set up tracer communication threads.");
                exit(1);
            }

            // create watchdog thread to monitor existence of remote process
            if (g_settings.program != NULL)
            {
                procInfo.watchdogThread = vktrace_platform_create_thread(Process_RunWatchdogThread, &procInfo);
            }

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)

            // Sync wait for local threads and remote process to complete.
            if (g_settings.program != NULL) {
                vktrace_linux_sync_wait_for_thread(&procInfo.watchdogThread);
            }

            vktrace_linux_sync_wait_for_thread(&(procInfo.pCaptureThreads[0].recordingThread));

#else
            vktrace_platform_resume_thread(&procInfo.hThread);

            // Now into the main message loop, listen for hotkeys to send over.
            exitval = MessageLoop();
#endif
        }
        vktrace_appendPortabilityPacket(procInfo.pTraceFile);
        vktrace_process_info_delete(&procInfo);
        serverIndex++;
    } while (g_settings.program == NULL);

    vktrace_SettingGroup_delete(&g_settingGroup);
    vktrace_free(g_default_settings.output_trace);

    exit(exitval);
}
