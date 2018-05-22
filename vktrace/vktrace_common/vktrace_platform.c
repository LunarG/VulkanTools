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
#include "vktrace_platform.h"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
#include "vktrace_common.h"
#include <pthread.h>
#endif

#if defined(PLATFORM_OSX)
#include <libproc.h>
#endif

vktrace_process_id vktrace_get_pid() {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    return getpid();
#elif defined(WIN32)
    return GetCurrentProcessId();
#endif
}

char* vktrace_platform_get_current_executable_directory() {
    char* exePath = (char*)vktrace_malloc(_MAX_PATH);
#if defined(WIN32)
    DWORD s = GetModuleFileName(NULL, exePath, MAX_PATH);
#elif defined(PLATFORM_LINUX)
    ssize_t s = readlink("/proc/self/exe", exePath, _MAX_PATH);
    if (s >= 0) {
        exePath[s] = '\0';
    } else {
        exePath[0] = '\0';
    }
#elif defined(PLATFORM_OSX)
    ssize_t s = proc_pidpath(getpid(), exePath, _MAX_PATH);
    if (s >= 0) {
        exePath[s] = '\0';
    } else {
        exePath[0] = '\0';
    }
#endif

    while (s > 0) {
        if (exePath[s] == '/' || exePath[s] == '\\') {
            // NULL this location and break so that the shortened string can be returned.
            exePath[s] = '\0';
            break;
        }

        --s;
    }

    if (s <= 0) {
        assert(!"Unexpected path returned in vktrace_platform_get_current_executable_directory");
        vktrace_free(exePath);
        exePath = NULL;
    }

    return exePath;
}

BOOL vktrace_is_loaded_into_vktrace() {
    char exePath[_MAX_PATH];

#if defined(WIN32)
    char* substr = ((sizeof(void*) == 4) ? "vktrace32.exe" : "vktrace.exe");
    GetModuleFileName(NULL, exePath, MAX_PATH);
#elif defined(PLATFORM_LINUX)
    char* substr = ((sizeof(void*) == 4) ? "vktrace32" : "vktrace");
    ssize_t s = readlink("/proc/self/exe", exePath, _MAX_PATH);
    if (s >= 0) {
        exePath[s] = '\0';
    } else {
        exePath[0] = '\0';
    }
#elif defined(PLATFORM_OSX)
    char* substr = ((sizeof(void*) == 4) ? "vktrace32" : "vktrace");
    ssize_t s = proc_pidpath(getpid(), exePath, _MAX_PATH);
    if (s >= 0) {
        exePath[s] = '\0';
    } else {
        exePath[0] = '\0';
    }
#endif
    return (strstr(exePath, substr) != NULL);
}

BOOL vktrace_platform_get_next_lib_sym(void** ppFunc, const char* name) {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    if ((*ppFunc = dlsym(RTLD_NEXT, name)) == NULL) {
        vktrace_LogError("dlsym: failed to find symbol %s %s", name, dlerror());
        return FALSE;
    }
#elif defined(WIN32)
    vktrace_LogError("unimplemented");
    assert(0);
    return FALSE;
#endif
    return TRUE;
}

vktrace_thread_id vktrace_platform_get_thread_id() {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    // return (vktrace_thread_id)syscall(SYS_gettid);
    return (vktrace_thread_id)pthread_self();
#elif defined(WIN32)
    return GetCurrentThreadId();
#endif
}

#if defined(ANDROID)
const uint32_t MAX_BUFFER_SIZE = 255;
static char android_env[MAX_BUFFER_SIZE] = {};
char* AndroidGetEnv(const char* key) {
    const char* command = "getprop ";
    const size_t len_command = strlen(command);
    const size_t len_key = strlen(key);
    char* full_command = malloc(len_command + len_key + 1);
    memcpy(full_command, command, len_command);
    memcpy(full_command + len_command, key, len_key + 1);

    FILE* pipe = NULL;
    pipe = popen(full_command, "r");
    if (pipe != NULL) {
        fgets(android_env, MAX_BUFFER_SIZE, pipe);
        pclose(pipe);
    }

    if (strlen(android_env) > 0) {
        android_env[strcspn(android_env, "\r\n")] = '\0';
        vktrace_LogAlways("%s: %s", full_command, android_env);
        free(full_command);
        return android_env;
    }

    free(full_command);
    return NULL;
}

void AndroidSetEnv(const char* key, const char* val) {
    const char* command = "setprop ";
    const char* space = " ";
    const size_t len_command = strlen(command);
    const size_t len_key = strlen(key);
    const size_t len_space = strlen(space);
    const size_t len_val = strlen(val);
    char* full_command = malloc(len_command + len_key + len_space + len_val + 1);
    memcpy(full_command, command, len_command);
    memcpy(full_command + len_command, key, len_key);
    memcpy(full_command + len_command + len_key, space, len_space);
    memcpy(full_command + len_command + len_key + len_space, val, len_val + 1);

    FILE* pipe = NULL;
    pipe = popen(full_command, "r");
    if (pipe != NULL) {
        vktrace_LogAlways("%s", full_command);
        pclose(pipe);
    }

    free(full_command);
}
#endif

char* vktrace_get_global_var(const char* name) {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
#if defined(ANDROID)
    return AndroidGetEnv(name);
#else
    return getenv(name);
#endif
#else
    // TODO: add code for reading from Windows registry
    // For now we just return the result from getenv
    return getenv(name);
#endif
}

void vktrace_set_global_var(const char* name, const char* val) {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
#if defined(ANDROID)
    AndroidSetEnv(name, val);
#else
    setenv(name, val, 1);
#endif
#else
    // TODO add code for writing to Windows registry
    // For now we just do _putenv_s
    _putenv_s(name, val);
#endif
}

size_t vktrace_platform_rand_s(uint32_t* out_array, size_t out_array_length) {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    static __thread unsigned int s_seed = 0;
    size_t i = 0;

    if (s_seed == 0) {
        // Try to seed rand_r() with /dev/urandom.
        size_t nbytes = 0;
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd != -1) {
            nbytes = read(fd, &s_seed, sizeof(s_seed));
            close(fd);
        }

        // If that didn't work, fallback to time and thread id.
        if (nbytes != sizeof(s_seed)) {
            struct timeval time;
            gettimeofday(&time, NULL);
            s_seed = vktrace_platform_get_thread_id() ^ ((time.tv_sec * 1000) + (time.tv_usec / 1000));
        }
    }

    for (i = 0; i < out_array_length; ++i) {
        out_array[i] = rand_r(&s_seed);
    }

    return out_array_length;
#elif defined(WIN32)
    // VKTRACE_ASSUME(sizeof(uint32_t) == sizeof(unsigned int));

    size_t ret_values = 0;
    for (ret_values = 0; ret_values < out_array_length; ++ret_values) {
        if (FAILED(rand_s(&out_array[ret_values]))) return ret_values;
    }

    return ret_values;
#endif
}

void* vktrace_platform_open_library(const char* libPath) {
#if defined(WIN32)
    return LoadLibrary(libPath);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    return dlopen(libPath, RTLD_LAZY);
#endif
}

void* vktrace_platform_get_library_entrypoint(void* libHandle, const char* name) {
// Get func ptr to library entrypoint. We don't log an error if
// we don't find the entrypoint, because cross-platform support
// causes vkreplay to query the address of all api entrypoints,
// even the wsi-specific ones.
#ifdef WIN32
    FARPROC proc = GetProcAddress((HMODULE)libHandle, name);
#else
    void* proc = dlsym(libHandle, name);
#endif
    return proc;
}

void vktrace_platform_close_library(void* pLibrary) {
#if defined(WIN32)
    FreeLibrary((HMODULE)pLibrary);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    dlclose(pLibrary);
#endif
}

void vktrace_platform_full_path(const char* partPath, unsigned long bytes, char* buffer) {
    assert(buffer != NULL);
#if defined(WIN32)
    GetFullPathName(partPath, bytes, buffer, NULL);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    char* ptr = realpath(partPath, buffer);
    (void)ptr;
#endif
}

char* vktrace_platform_extract_path(char* _path) {
    // These functions actually work on const strings, but the C decl version exposed by the macro
    // takes non-const TCHAR*.
    char* pDir;
    size_t newLen;
    char* pathSepBack = strrchr(_path, '\\');
    char* pathSepFor = strrchr(_path, '/');
    char* lastPathSep = pathSepBack > pathSepFor ? pathSepBack : pathSepFor;

    if (lastPathSep == NULL) {
        return vktrace_allocate_and_copy(".\\");
    }

    pDir = VKTRACE_NEW_ARRAY(char, strlen(_path) + 1);
    newLen = strlen(_path) - strlen(lastPathSep);
    strncpy(pDir, _path, newLen);
    pDir[newLen] = '\0';
    return pDir;
}

// The following linux paths are based on:
// standards.freedesktop.org/basedir-spec/basedir-spec-0.8.html
char* vktrace_platform_get_settings_path() {
#if defined(__linux__)
    char* xdgConfigHome = getenv("XDG_CONFIG_HOME");
    if (xdgConfigHome != NULL && strlen(xdgConfigHome) > 0) {
        return vktrace_copy_and_append(xdgConfigHome, VKTRACE_PATH_SEPARATOR, "vktrace");
    } else {
        return vktrace_copy_and_append(getenv("HOME"), VKTRACE_PATH_SEPARATOR, ".config/vktrace");
    }
#elif defined(WIN32)
    DWORD reqLength = GetEnvironmentVariable("localappdata", NULL, 0);
    TCHAR* localAppData = VKTRACE_NEW_ARRAY(TCHAR, reqLength);
    GetEnvironmentVariable("localappdata", localAppData, reqLength);
    TCHAR* localVktraceData = vktrace_copy_and_append(localAppData, VKTRACE_PATH_SEPARATOR, "vktrace");
    VKTRACE_DELETE(localAppData);
    return localVktraceData;
#else
    assert(!"not implemented");
#endif
}

char* vktrace_platform_get_data_path() {
#if defined(__linux__)
    char* xdgDataHome = getenv("XDG_DATA_HOME");
    if (xdgDataHome != NULL && strlen(xdgDataHome) > 0) {
        return vktrace_copy_and_append(xdgDataHome, VKTRACE_PATH_SEPARATOR, "vktrace");
    } else {
        return vktrace_copy_and_append(getenv("HOME"), VKTRACE_PATH_SEPARATOR, ".local/share/vktrace");
    }
#elif defined(WIN32)
    DWORD reqLength = GetEnvironmentVariable("localappdata", NULL, 0);
    TCHAR* localAppData = VKTRACE_NEW_ARRAY(TCHAR, reqLength);
    GetEnvironmentVariable("localappdata", localAppData, reqLength);
    TCHAR* localVktraceData = vktrace_copy_and_append(localAppData, VKTRACE_PATH_SEPARATOR, "vktrace");
    VKTRACE_DELETE(localAppData);
    return localVktraceData;
#else
    assert(!"not implemented");
#endif
}

vktrace_thread vktrace_platform_create_thread(VKTRACE_THREAD_ROUTINE_RETURN_TYPE (*start_routine)(LPVOID), void* args) {
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    vktrace_thread thread = 0;
    if (pthread_create(&thread, NULL, (void* (*)(void*))start_routine, args) != 0) {
        vktrace_LogError("Failed to create thread");
    }
    return thread;
#elif defined(WIN32)
    return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, args, 0, NULL);
#endif
}

void vktrace_platform_resume_thread(vktrace_thread* pThread) {
    assert(pThread != NULL);
#if defined(PLATFORM_LINUX)
    assert(!"Add code to resume threads on Linux");
#elif defined(PLATFORM_OSX)
    assert(!"Add code to resume threads on macOS");
#elif defined(WIN32)
    if (*pThread != NULL) ResumeThread(*pThread);
#endif
}

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
int64_t vktrace_linux_sync_wait_for_thread(vktrace_thread* pThread) {
    void* retval;
    assert(pThread != NULL);
    if (pthread_join(*pThread, &retval) != 0) {
        vktrace_LogError("Error occurred while waiting for thread to end.");
    }
    return retval ? *((int64_t*)retval) : 0;
}
#endif

void vktrace_platform_delete_thread(vktrace_thread* pThread) {
    assert(pThread != NULL);
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
// Don't have to do anything!
#elif defined(WIN32)
    CloseHandle(*pThread);
    *pThread = NULL;
#endif
}

#if defined(WIN32)
void vktrace_platform_thread_once(void* ctl, BOOL(CALLBACK* func)(PINIT_ONCE, PVOID, PVOID*)) {
    assert(func != NULL);
    assert(ctl != NULL);
    InitOnceExecuteOnce((PINIT_ONCE)ctl, (PINIT_ONCE_FN)func, NULL, NULL);
}
#else
void vktrace_platform_thread_once(void* ctl, void (*func)(void)) {
    assert(func != NULL);
    assert(ctl != NULL);
    pthread_once((pthread_once_t*)ctl, func);
}
#endif

void vktrace_create_critical_section(VKTRACE_CRITICAL_SECTION* pCriticalSection) {
#if defined(WIN32)
    InitializeCriticalSection(pCriticalSection);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    pthread_mutexattr_t thread_mutexattr_t;
    if (pthread_mutexattr_init(&thread_mutexattr_t) == 0) {
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
	// If the non-portable version is available, continue to use it
        pthread_mutexattr_settype(&thread_mutexattr_t, PTHREAD_MUTEX_RECURSIVE_NP);
#else
	// Otherwise use the more standard version
        pthread_mutexattr_settype(&thread_mutexattr_t, PTHREAD_MUTEX_RECURSIVE);
#endif
        pthread_mutex_init(pCriticalSection, &thread_mutexattr_t);
        pthread_mutexattr_destroy(&thread_mutexattr_t);
    } else {
        vktrace_LogError(
            "Failed to call pthread_mutexattr_init, call pthread_mutex_init with default mutex attribute, this may cause deadlock "
            "for application that expect no blocking for additional pthread_mutex_lock calls from same thread.");
        pthread_mutex_init(pCriticalSection, NULL);
    }
#endif
}

void vktrace_enter_critical_section(VKTRACE_CRITICAL_SECTION* pCriticalSection) {
#if defined(WIN32)
    EnterCriticalSection(pCriticalSection);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    pthread_mutex_lock(pCriticalSection);
#endif
}

void vktrace_leave_critical_section(VKTRACE_CRITICAL_SECTION* pCriticalSection) {
#if defined(WIN32)
    LeaveCriticalSection(pCriticalSection);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    pthread_mutex_unlock(pCriticalSection);
#endif
}

void vktrace_delete_critical_section(VKTRACE_CRITICAL_SECTION* pCriticalSection) {
#if defined(WIN32)
    DeleteCriticalSection(pCriticalSection);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    pthread_mutex_destroy(pCriticalSection);
#endif
}

BOOL vktrace_platform_remote_load_library(vktrace_process_handle pProcessHandle, const char* dllPath,
                                          vktrace_thread* pTracingThread, char** ldPreload) {
    if (dllPath == NULL) return TRUE;
#if defined(WIN32)
    SIZE_T bytesWritten = 0;
    void* targetProcessMem = NULL;
    vktrace_thread thread = NULL;
    size_t byteCount = sizeof(char) * (strlen(dllPath) + 1);
    targetProcessMem = VirtualAllocEx(pProcessHandle, 0, byteCount, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!targetProcessMem) {
        vktrace_LogError("Failed to inject ourselves into target process--couldn't allocate process memory.");
        return FALSE;
    }

    if (!WriteProcessMemory(pProcessHandle, targetProcessMem, dllPath, byteCount, &bytesWritten)) {
        vktrace_LogError("Failed to inject ourselves into target process--couldn't write inception DLL name into process.");
        return FALSE;
    }

    thread = CreateRemoteThread(pProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, targetProcessMem, 0, NULL);
    if (thread == NULL) {
        vktrace_LogError("Failed to inject ourselves into target process--couldn't spawn thread.");
        return FALSE;
    }
    assert(pTracingThread != NULL);
    *pTracingThread = thread;

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
    char* tmp;
    if (ldPreload == NULL) return TRUE;
    if (*ldPreload == NULL) {
        tmp = vktrace_copy_and_append("LD_PRELOAD", "=", dllPath);
    } else {
        tmp = vktrace_copy_and_append(*ldPreload, " ", dllPath);
        VKTRACE_DELETE((void*)*ldPreload);
    }
    *ldPreload = tmp;
#endif

    return TRUE;
}
