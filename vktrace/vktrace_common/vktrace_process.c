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
 * Author: David Pinedo <david@lunarg.com>
 **************************************************************************/
#include "vktrace_process.h"

BOOL vktrace_process_spawn(vktrace_process_info* pInfo) {
    assert(pInfo != NULL);

    char fullExePath[_MAX_PATH];
#if defined(WIN32)
    {
        unsigned long processCreateFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
        PROCESS_INFORMATION processInformation;
        STARTUPINFO si = {0};
        si.cb = sizeof(si);

        memset(&processInformation, 0, sizeof(PROCESS_INFORMATION));
        memset(fullExePath, 0, sizeof(char) * _MAX_PATH);
        fullExePath[0] = 0;

        SetLastError(0);
        if (0 == SearchPath(NULL, pInfo->exeName, ".exe", ARRAYSIZE(fullExePath), fullExePath, NULL)) {
            vktrace_LogVerbose("Failed to spawn '%s'.", pInfo->exeName);
            return FALSE;
        }

        if (!CreateProcess(fullExePath, pInfo->fullProcessCmdLine, NULL, NULL, TRUE, processCreateFlags, NULL,
                           pInfo->workingDirectory, &si, &processInformation)) {
            vktrace_LogVerbose("Failed to spawn '%s'.", fullExePath);
            return FALSE;
        }

        pInfo->hProcess = processInformation.hProcess;
        pInfo->hThread = processInformation.hThread;
        pInfo->processId = processInformation.dwProcessId;
        // TODO : Do we need to do anything with processInformation.dwThreadId?
    }
#elif defined(PLATFORM_LINUX)
    pInfo->processId = fork();
    if (pInfo->processId == -1) {
        vktrace_LogError("Failed to spawn process.");
        return FALSE;
    } else if (pInfo->processId == 0) {
        // Inside new process
        char* args[128];
        const char delim[] = " \t";
        unsigned int idx;

        realpath(pInfo->exeName, fullExePath);

        // Change process name so the the tracer DLLs will behave as expected when loaded.
        // NOTE: Must be 15 characters or less.
        const char* tmpProcName = "vktraceChildProcess";
        prctl(PR_SET_NAME, (unsigned long)tmpProcName, 0, 0, 0);

        // Change working directory
        if (chdir(pInfo->workingDirectory) == -1) {
            vktrace_LogError("Failed to set working directory.");
        }

        args[0] = fullExePath;
        args[127] = NULL;
        idx = 1;
        args[idx] = pInfo->processArgs ? strtok(pInfo->processArgs, delim) : NULL;
        while (args[idx] != NULL && idx < 128) {
            idx++;
            args[idx] = strtok(NULL, delim);
        }
        vktrace_LogDebug("exec process=%s argc=%u\n", fullExePath, idx);
#if 0  // uncoment to print out list of env vars
        char *env = environ[0];
        idx = 0;
        while (env && strlen(env)) {
            if (strstr(env, "VK") || strstr(env, "LD"))
                vktrace_LogDebug("env[%d] = %s", idx++, env);
            else
                idx++;
            env = environ[idx];
        }
#endif
        if (execv(fullExePath, args) < 0) {
            vktrace_LogError("Failed to spawn process.");
            perror(NULL);
            exit(1);
        }
    }
#endif

    return TRUE;
}

void vktrace_process_info_delete(vktrace_process_info* pInfo) {
    if (pInfo->pCaptureThreads != NULL) {
        vktrace_platform_delete_thread(&(pInfo->pCaptureThreads[0].recordingThread));
        VKTRACE_DELETE(pInfo->pCaptureThreads);
    }

#ifdef WIN32
    vktrace_platform_delete_thread(&(pInfo->watchdogThread));
#endif

    if (pInfo->pTraceFile != NULL) {
        vktrace_LogDebug("Closing trace file: '%s'", pInfo->traceFilename);
        fclose(pInfo->pTraceFile);
    }

    VKTRACE_DELETE(pInfo->traceFilename);
    VKTRACE_DELETE(pInfo->workingDirectory);
    VKTRACE_DELETE(pInfo->processArgs);
    VKTRACE_DELETE(pInfo->fullProcessCmdLine);
    VKTRACE_DELETE(pInfo->exeName);

    vktrace_delete_critical_section(&(pInfo->traceFileCriticalSection));
}
