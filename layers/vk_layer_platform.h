/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */
#pragma once

#if defined(_WIN32)
// WinSock2.h must be included *BEFORE* windows.h
#include <WinSock2.h>
#endif // _WIN32

#include "vulkan/vk_platform.h"
#include "vulkan/vk_sdk_platform.h"

#if defined(__linux__)
/* Linux-specific common code: */

// Headers:
//#define _GNU_SOURCE 1
// TBD: Are the contents of the following file used?
#include <unistd.h>
// Note: The following file is for dynamic loading:
#include <dlfcn.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libgen.h>

// C99:
#define PRINTF_SIZE_T_SPECIFIER    "%zu"

// File IO
static inline bool layer_platform_file_exists(const char *path)
{
    if (access(path, F_OK))
        return false;
    else
        return true;
}

static inline bool layer_platform_is_path_absolute(const char *path)
{
    if (path[0] == '/')
        return true;
    else
        return false;
}

static inline char *layer_platform_dirname(char *path)
{
    return dirname(path);
}

// Environment variables

static inline char *layer_platform_getenv(const char *name)
{
    return getenv(name);
}

static inline void layer_platform_free_getenv(const char *val)
{
}

// Dynamic Loading of libraries:
typedef void * layer_platform_dl_handle;
static inline layer_platform_dl_handle layer_platform_open_library(const char* libPath)
{
    return dlopen(libPath, RTLD_LAZY | RTLD_LOCAL);
}
static inline const char * layer_platform_open_library_error(const char* libPath)
{
    return dlerror();
}
static inline void layer_platform_close_library(layer_platform_dl_handle library)
{
    dlclose(library);
}
static inline void * layer_platform_get_proc_address(layer_platform_dl_handle library,
                                                      const char *name)
{
    assert(library);
    assert(name);
    return dlsym(library, name);
}
static inline const char * layer_platform_get_proc_address_error(const char *name)
{
    return dlerror();
}

// Threads:
typedef pthread_t layer_platform_thread;
#define THREAD_LOCAL_DECL __thread
#define LAYER_PLATFORM_THREAD_ONCE_DECLARATION(var) \
    pthread_once_t var = PTHREAD_ONCE_INIT;
#define LAYER_PLATFORM_THREAD_ONCE_DEFINITION(var) \
    pthread_once_t var;
static inline void layer_platform_thread_once(pthread_once_t *ctl, void (* func) (void))
{
    assert(func != NULL);
    assert(ctl != NULL);
    pthread_once(ctl, func);
}

// Thread IDs:
typedef pthread_t layer_platform_thread_id;
static inline layer_platform_thread_id layer_platform_get_thread_id()
{
    return pthread_self();
}

// Thread mutex:
typedef pthread_mutex_t layer_platform_thread_mutex;
static inline void layer_platform_thread_create_mutex(layer_platform_thread_mutex* pMutex)
{
    pthread_mutex_init(pMutex, NULL);
}
static inline void layer_platform_thread_lock_mutex(layer_platform_thread_mutex* pMutex)
{
    pthread_mutex_lock(pMutex);
}
static inline void layer_platform_thread_unlock_mutex(layer_platform_thread_mutex* pMutex)
{
    pthread_mutex_unlock(pMutex);
}
static inline void layer_platform_thread_delete_mutex(layer_platform_thread_mutex* pMutex)
{
    pthread_mutex_destroy(pMutex);
}
typedef pthread_cond_t layer_platform_thread_cond;
static inline void layer_platform_thread_init_cond(layer_platform_thread_cond* pCond)
{
    pthread_cond_init(pCond, NULL);
}
static inline void layer_platform_thread_cond_wait(layer_platform_thread_cond* pCond, layer_platform_thread_mutex* pMutex)
{
    pthread_cond_wait(pCond, pMutex);
}
static inline void layer_platform_thread_cond_broadcast(layer_platform_thread_cond* pCond)
{
    pthread_cond_broadcast(pCond);
}

#elif defined(_WIN32) // defined(__linux__)
/* Windows-specific common code: */
// WinBase.h defines CreateSemaphore and synchapi.h defines CreateEvent
//  undefine them to avoid conflicts with VkLayerDispatchTable struct members.
#ifdef CreateSemaphore
#undef CreateSemaphore
#endif
#ifdef CreateEvent
#undef CreateEvent
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <stdbool.h>
#include <shlwapi.h>
#ifdef __cplusplus
#include <iostream>
#include <string>
using namespace std;
#endif // __cplusplus

// VK Library Filenames, Paths, etc.:
#define PATH_SEPERATOR ';'
#define DIRECTORY_SYMBOL '\\'
#define DEFAULT_VK_REGISTRY_HIVE HKEY_LOCAL_MACHINE
#define DEFAULT_VK_DRIVERS_INFO "SOFTWARE\\Khronos\\Vulkan\\Drivers"
// TODO: Are these the correct paths
#define DEFAULT_VK_DRIVERS_PATH "C:\\Windows\\System32;C:\\Windows\\SysWow64"
#define DEFAULT_VK_ELAYERS_INFO "SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers"
#define DEFAULT_VK_ILAYERS_INFO "SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers"
#define DEFAULT_VK_LAYERS_PATH "C:\\Windows\\System32;C:\\Windows\\SysWow64"
#define LAYERS_PATH_ENV "VK_LAYER_PATH"

#define PRINTF_SIZE_T_SPECIFIER    "%Iu"

// File IO
static bool layer_platform_file_exists(const char *path)
{
    if ((_access(path, 0)) == -1)
        return false;
    else
        return true;
}

static bool layer_platform_is_path_absolute(const char *path)
{
    return !PathIsRelative(path);
}

// WIN32 runtime doesn't have dirname().
static inline char *layer_platform_dirname(char *path)
{
    char *current, *next;

    // TODO/TBD: Do we need to deal with the Windows's ":" character?

    for (current = path; *current != '\0'; current = next) {
        next = strchr(current, DIRECTORY_SYMBOL);
        if (next == NULL) {
            if (current != path)
                *(current - 1) = '\0';
            return path;
        } else {
            // Point one character past the DIRECTORY_SYMBOL:
            next++;
        }
    }
    return path;
}

// WIN32 runtime doesn't have basename().
// Microsoft also doesn't have basename().  Paths are different on Windows, and
// so this is just a temporary solution in order to get us compiling, so that we
// can test some scenarios, and develop the correct solution for Windows.
  // TODO: Develop a better, permanent solution for Windows, to replace this
  // temporary code:
static char *layer_platform_basename(char *pathname)
{
    char *current, *next;

// TODO/TBD: Do we need to deal with the Windows's ":" character?

    for (current = pathname; *current != '\0'; current = next) {
        next = strchr(current, DIRECTORY_SYMBOL);
        if (next == NULL) {
            // No more DIRECTORY_SYMBOL's so return p:
            return current;
        } else {
            // Point one character past the DIRECTORY_SYMBOL:
            next++;
        }
    }
    // We shouldn't get to here, but this makes the compiler happy:
    return current;
}

// Environment variables

static inline char *layer_platform_getenv(const char *name)
{
    char *retVal;
    DWORD valSize;

    valSize = GetEnvironmentVariableA(name, NULL, 0);

    // valSize DOES include the null terminator, so for any set variable
    // will always be at least 1. If it's 0, the variable wasn't set.
    if (valSize == 0)
        return NULL;

    //TODO; FIXME This should be using any app defined memory allocation
    retVal = (char *)malloc(valSize);

    GetEnvironmentVariableA(name, retVal, valSize);

    return retVal;
}

static inline void layer_platform_free_getenv(const char *val)
{
    free((void *)val);
}

// Dynamic Loading:
typedef HMODULE layer_platform_dl_handle;
static layer_platform_dl_handle layer_platform_open_library(const char* libPath)
{
    return LoadLibrary(libPath);
}
static char * layer_platform_open_library_error(const char* libPath)
{
    static char errorMsg[120];
    snprintf(errorMsg, 119, "Failed to open dynamic library \"%s\"", libPath);
    return errorMsg;
}
static void layer_platform_close_library(layer_platform_dl_handle library)
{
    FreeLibrary(library);
}
static void * layer_platform_get_proc_address(layer_platform_dl_handle library,
                                               const char *name)
{
    assert(library);
    assert(name);
    return GetProcAddress(library, name);
}
static char * layer_platform_get_proc_address_error(const char *name)
{
    static char errorMsg[120];
    snprintf(errorMsg, 119, "Failed to find function \"%s\" in dynamic library", name);
    return errorMsg;
}

// Threads:
typedef HANDLE layer_platform_thread;
#define THREAD_LOCAL_DECL __declspec(thread)
#define LAYER_PLATFORM_THREAD_ONCE_DECLARATION(var) \
    INIT_ONCE var = INIT_ONCE_STATIC_INIT;
#define LAYER_PLATFORM_THREAD_ONCE_DEFINITION(var) \
    INIT_ONCE var;
static BOOL CALLBACK InitFuncWrapperLayer(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context)
{
    void (*func)(void) = (void (*)(void))Parameter;
    func();
    return TRUE;
}

static void layer_platform_thread_once(void *ctl, void (* func) (void))
{
    assert(func != NULL);
    assert(ctl != NULL);
    InitOnceExecuteOnce((PINIT_ONCE) ctl, InitFuncWrapperLayer, func, NULL);
}

// Thread IDs:
typedef DWORD layer_platform_thread_id;
static layer_platform_thread_id layer_platform_get_thread_id()
{
    return GetCurrentThreadId();
}

// Thread mutex:
typedef CRITICAL_SECTION layer_platform_thread_mutex;
static void layer_platform_thread_create_mutex(layer_platform_thread_mutex* pMutex)
{
    InitializeCriticalSection(pMutex);
}
static void layer_platform_thread_lock_mutex(layer_platform_thread_mutex* pMutex)
{
    EnterCriticalSection(pMutex);
}
static void layer_platform_thread_unlock_mutex(layer_platform_thread_mutex* pMutex)
{
    LeaveCriticalSection(pMutex);
}
static void layer_platform_thread_delete_mutex(layer_platform_thread_mutex* pMutex)
{
    DeleteCriticalSection(pMutex);
}
typedef CONDITION_VARIABLE layer_platform_thread_cond;
static void layer_platform_thread_init_cond(layer_platform_thread_cond* pCond)
{
    InitializeConditionVariable(pCond);
}
static void layer_platform_thread_cond_wait(layer_platform_thread_cond* pCond, layer_platform_thread_mutex* pMutex)
{
    SleepConditionVariableCS(pCond, pMutex, INFINITE);
}
static void layer_platform_thread_cond_broadcast(layer_platform_thread_cond* pCond)
{
    WakeAllConditionVariable(pCond);
}


#else // defined(_WIN32)

#error The "vk_layer_platform.h" file must be modified for this OS.


#endif // defined(_WIN32)


