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

vktrace_settings g_settings;
vktrace_settings g_default_settings;

vktrace_SettingInfo g_settings_info[] =
{
    // common command options
    { "p", "Program", VKTRACE_SETTING_STRING, &g_settings.program, &g_default_settings.program, TRUE, "The program to trace."},
    { "a", "Arguments", VKTRACE_SETTING_STRING, &g_settings.arguments, &g_default_settings.arguments, TRUE, "Cmd-line arguments to pass to trace program."},
    { "w", "WorkingDir", VKTRACE_SETTING_STRING, &g_settings.working_dir, &g_default_settings.working_dir, TRUE, "The program's working directory."},
    { "o", "OutputTrace", VKTRACE_SETTING_STRING, &g_settings.output_trace, &g_default_settings.output_trace, TRUE, "Path to the generated output trace file."},
    { "s", "ScreenShot", VKTRACE_SETTING_STRING, &g_settings.screenshotList, &g_default_settings.screenshotList, TRUE, "Comma separated list of frames to take a snapshot of."},
    { "ptm", "PrintTraceMessages", VKTRACE_SETTING_BOOL, &g_settings.print_trace_messages, &g_default_settings.print_trace_messages, TRUE, "Print trace messages to vktrace console."},
#if _DEBUG
    { "v", "Verbosity", VKTRACE_SETTING_STRING, &g_settings.verbosity, &g_default_settings.verbosity, TRUE, "Verbosity mode. Modes are \"quiet\", \"errors\", \"warnings\", \"full\", \"debug\"."},
#else
    { "v", "Verbosity", VKTRACE_SETTING_STRING, &g_settings.verbosity, &g_default_settings.verbosity, TRUE, "Verbosity mode. Modes are \"quiet\", \"errors\", \"warnings\", \"full\"."},
#endif

    //{ "z", "pauze", VKTRACE_SETTING_BOOL, &g_settings.pause, &g_default_settings.pause, TRUE, "Wait for a key at startup (so a debugger can be attached)" },
};

vktrace_SettingGroup g_settingGroup =
{
    "vktrace",
    sizeof(g_settings_info) / sizeof(g_settings_info[0]),
    &g_settings_info[0]
};

// ------------------------------------------------------------------------------------------------
#if defined(WIN32)
void MessageLoop()
{
    MSG msg = { 0 };
    bool quit = false;
    while (!quit)
    {
        if (GetMessage(&msg, NULL, 0, 0) == FALSE)
        {
            quit = true;
        }
        else
        {
            quit = (msg.message == VKTRACE_WM_COMPLETE);
        }
    }
}
#endif

int PrepareTracers(vktrace_process_capture_trace_thread_info** ppTracerInfo)
{
    unsigned int num_tracers = 1;

    assert(ppTracerInfo != NULL && *ppTracerInfo == NULL);
    *ppTracerInfo = VKTRACE_NEW_ARRAY(vktrace_process_capture_trace_thread_info, num_tracers);
    memset(*ppTracerInfo, 0, sizeof(vktrace_process_capture_trace_thread_info) * num_tracers);

    // we only support Vulkan tracer
    (*ppTracerInfo)[0].tracerId = VKTRACE_TID_VULKAN;

    return num_tracers;
}

bool InjectTracersIntoProcess(vktrace_process_info* pInfo)
{
    bool bRecordingThreadsCreated = true;
    vktrace_thread tracingThread;
    if (vktrace_platform_remote_load_library(pInfo->hProcess, NULL, &tracingThread, NULL)) {
        // prepare data for capture threads
        pInfo->pCaptureThreads[0].pProcessInfo = pInfo;
        pInfo->pCaptureThreads[0].recordingThread = VKTRACE_NULL_THREAD;

        // create thread to record trace packets from the tracer
        pInfo->pCaptureThreads[0].recordingThread = vktrace_platform_create_thread(Process_RunRecordTraceThread, &(pInfo->pCaptureThreads[0]));
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

void loggingCallback(VktraceLogLevel level, const char* pMessage)
{
    if (level == VKTRACE_LOG_NONE)
        return;

    switch(level)
    {
    case VKTRACE_LOG_DEBUG: printf("vktrace debug: %s\n", pMessage); break;
    case VKTRACE_LOG_ERROR: printf("vktrace error: %s\n", pMessage); break;
    case VKTRACE_LOG_WARNING: printf("vktrace warning: %s\n", pMessage); break;
    case VKTRACE_LOG_VERBOSE: printf("vktrace info: %s\n", pMessage); break;
    default:
        printf("%s\n", pMessage); break;
    }
    fflush(stdout);

#if defined(WIN32)
#if _DEBUG
    OutputDebugString(pMessage);
#endif
#endif
}

// ------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    memset(&g_settings, 0, sizeof(vktrace_settings));

    vktrace_LogSetCallback(loggingCallback);
    vktrace_LogSetLevel(VKTRACE_LOG_ERROR);

    // setup defaults
    memset(&g_default_settings, 0, sizeof(vktrace_settings));
    g_default_settings.output_trace = vktrace_allocate_and_copy("vktrace_out.vktrace");
    g_default_settings.verbosity = "errors";
    g_default_settings.screenshotList = NULL;

    if (vktrace_SettingGroup_init(&g_settingGroup, NULL, argc, argv, &g_settings.arguments) != 0)
    {
        // invalid cmd-line parameters
        vktrace_SettingGroup_delete(&g_settingGroup);
        vktrace_free(g_default_settings.output_trace);
        return -1;
    }
    else
    {
        // Validate vktrace inputs
        BOOL validArgs = TRUE;

        if (g_settings.output_trace == NULL || strlen (g_settings.output_trace) == 0)
        {
            validArgs = FALSE;
        }
        else
        {
            size_t len = strlen(g_settings.output_trace);
            if (strncmp(&g_settings.output_trace[len-8], ".vktrace", 8) != 0)
            {
                // output trace filename does not end in .vktrace
                vktrace_LogError("Output trace file specified with -o parameter must have a '.vktrace' extension.");
                validArgs = FALSE;
            }
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
        else
        {
            vktrace_LogSetLevel(VKTRACE_LOG_ERROR);
            validArgs = FALSE;
        }
		vktrace_set_global_var("_VK_TRACE_VERBOSITY", g_settings.verbosity);

        if (validArgs == FALSE)
        {
            vktrace_SettingGroup_print(&g_settingGroup);
            return -1;
        }

        if (g_settings.program == NULL || strlen(g_settings.program) == 0)
        {
            vktrace_LogWarning("No program (-p) parameter found: Will run vktrace as server.");
            printf("Running vktrace as server...\n");
            fflush(stdout);
            g_settings.arguments = NULL;
        }
        else
        {
            if (g_settings.working_dir == NULL || strlen(g_settings.working_dir) == 0)
            {
                CHAR* buf = VKTRACE_NEW_ARRAY(CHAR, 4096);
                vktrace_LogVerbose("No working directory (-w) parameter found: Assuming executable's path as working directory.");
                vktrace_platform_full_path(g_settings.program, 4096, buf);
                g_settings.working_dir = vktrace_platform_extract_path(buf);
                VKTRACE_DELETE(buf);
            }

            vktrace_LogVerbose("Running vktrace as parent process will spawn child process: %s", g_settings.program);
            if (g_settings.arguments != NULL && strlen(g_settings.arguments) > 0)
            {
                vktrace_LogVerbose("Args to be passed to child process: '%s'", g_settings.arguments);
            }
        }
    }

    if (g_settings.screenshotList)
    {
        // Export list to screenshot layer
        vktrace_set_global_var("_VK_SCREENSHOT", g_settings.screenshotList);
    }
    else
    {
        vktrace_set_global_var("_VK_SCREENSHOT","");
    }


    unsigned int serverIndex = 0;
    do {
        // Create and start the process or run in server mode

        BOOL procStarted = TRUE;
        vktrace_process_info procInfo;
        memset(&procInfo, 0, sizeof(vktrace_process_info));
        if (g_settings.program != NULL)
        {
            procInfo.exeName = vktrace_allocate_and_copy(g_settings.program);
            procInfo.processArgs = vktrace_allocate_and_copy(g_settings.arguments);
            procInfo.fullProcessCmdLine = vktrace_copy_and_append(g_settings.program, " ", g_settings.arguments);
            procInfo.workingDirectory = vktrace_allocate_and_copy(g_settings.working_dir);
            procInfo.traceFilename = vktrace_allocate_and_copy(g_settings.output_trace);
        } else
        {
            const char *pExtension = strrchr(g_settings.output_trace, '.');
            char *basename = vktrace_allocate_and_copy_n(g_settings.output_trace, (int) ((pExtension == NULL) ? strlen(g_settings.output_trace) : pExtension - g_settings.output_trace));
            char num[16];
#ifdef PLATFORM_LINUX
            snprintf(num, 16, "%u", serverIndex);
#elif defined(WIN32)
            _snprintf_s(num, 16, _TRUNCATE, "%u", serverIndex);
#endif
            procInfo.traceFilename = vktrace_copy_and_append(basename, num, pExtension);
         }

        procInfo.parentThreadId = vktrace_platform_get_thread_id();

        // setup tracer, only Vulkan tracer suppported
        PrepareTracers(&procInfo.pCaptureThreads);

        if (g_settings.program != NULL)
        {
            char *instEnv = vktrace_get_global_var("VK_INSTANCE_LAYERS");
            // Add ScreenShot layer if enabled
            if (g_settings.screenshotList && (!instEnv || !strstr(instEnv, "VK_LAYER_LUNARG_screenshot")))
            {
                if (!instEnv || strlen(instEnv)  == 0)
                    vktrace_set_global_var("VK_INSTANCE_LAYERS", "VK_LAYER_LUNARG_screenshot");
                else
                {
                    char *newEnv = vktrace_copy_and_append(instEnv, VKTRACE_LIST_SEPARATOR, "VK_LAYER_LUNARG_screenshot");
                    vktrace_set_global_var("VK_INSTANCE_LAYERS", newEnv);
                }
                instEnv = vktrace_get_global_var("VK_INSTANCE_LAYERS");
            }
            char *devEnv = vktrace_get_global_var("VK_DEVICE_LAYERS");
            if (g_settings.screenshotList && (!devEnv || !strstr(devEnv, "VK_LAYER_LUNARG_screenshot")))
            {
                if (!devEnv || strlen(devEnv) == 0)
                    vktrace_set_global_var("VK_DEVICE_LAYERS", "VK_LAYER_LUNARG_screenshot");
                else
                {
                    char *newEnv = vktrace_copy_and_append(devEnv, VKTRACE_LIST_SEPARATOR, "VK_LAYER_LUNARG_screenshot");
                    vktrace_set_global_var("VK_DEVICE_LAYERS", newEnv);
                }
                devEnv = vktrace_get_global_var("VK_DEVICE_LAYERS");
            }
            // Add vktrace_layer enable env var if needed
            if (!instEnv || strlen(instEnv) == 0)
            {
                vktrace_set_global_var("VK_INSTANCE_LAYERS", "VK_LAYER_LUNARG_vktrace");
            }
            else if (instEnv != strstr(instEnv, "VK_LAYER_LUNARG_vktrace"))
            {
                char *newEnv = vktrace_copy_and_append("VK_LAYER_LUNARG_vktrace", VKTRACE_LIST_SEPARATOR, instEnv);
                vktrace_set_global_var("VK_INSTANCE_LAYERS", newEnv);
            }
            if (!devEnv || strlen(devEnv) == 0)
            {
                vktrace_set_global_var("VK_DEVICE_LAYERS", "VK_LAYER_LUNARG_vktrace");
            }
            else if (devEnv != strstr(devEnv, "VK_LAYER_LUNARG_vktrace"))
            {
                char *newEnv = vktrace_copy_and_append("VK_LAYER_LUNARG_vktrace", VKTRACE_LIST_SEPARATOR, devEnv);
                vktrace_set_global_var("VK_DEVICE_LAYERS", newEnv);
            }
            // call CreateProcess to launch the application
            procStarted = vktrace_process_spawn(&procInfo);
        }
        if (procStarted == FALSE)
        {
            vktrace_LogError("Failed to setup remote process.");
        }
        else
        {
            if (InjectTracersIntoProcess(&procInfo) == FALSE)
            {
                vktrace_LogError("Failed to setup tracer communication threads.");
                return -1;
            }

            // create watchdog thread to monitor existence of remote process
            if (g_settings.program != NULL)
                procInfo.watchdogThread = vktrace_platform_create_thread(Process_RunWatchdogThread, &procInfo);

#if defined(PLATFORM_LINUX)
            // Sync wait for local threads and remote process to complete.

            vktrace_platform_sync_wait_for_thread(&(procInfo.pCaptureThreads[0].recordingThread));

            if (g_settings.program != NULL)
                vktrace_platform_sync_wait_for_thread(&procInfo.watchdogThread);
#else
            vktrace_platform_resume_thread(&procInfo.hThread);

            // Now into the main message loop, listen for hotkeys to send over.
            MessageLoop();
#endif
        }

        vktrace_process_info_delete(&procInfo);
        serverIndex++;
    } while (g_settings.program == NULL);

    vktrace_SettingGroup_delete(&g_settingGroup);
    vktrace_free(g_default_settings.output_trace);

    return 0;
}

