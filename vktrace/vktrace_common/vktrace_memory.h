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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define VKTRACE_NEW(type) (type*)vktrace_malloc(sizeof(type))
#define VKTRACE_NEW_ARRAY(type, count) (type*)vktrace_malloc(sizeof(type) * count)
#define VKTRACE_DELETE(ptr) vktrace_free(ptr);
#define VKTRACE_REALLOC(ptr, size) vktrace_realloc(ptr, size);

static void* vktrace_malloc(size_t size) {
    void* pMemory;
    if (size == 0) return NULL;

    pMemory = malloc(size);

    return pMemory;
}

static void vktrace_free(void* ptr) {
    free(ptr);
    ptr = NULL;
}

static void* vktrace_realloc(void* ptr, size_t size) {
    void* pMemory;
    if (size == 0) return NULL;

    pMemory = realloc(ptr, size);
    return pMemory;
}

static char* vktrace_allocate_and_copy(const char* _src) {
    if (_src == NULL) {
        return NULL;
    } else {
        size_t bufferSize = 1 + strlen(_src);

        char* retVal = VKTRACE_NEW_ARRAY(char, bufferSize);
#if defined(WIN32)
        strcpy_s(retVal, bufferSize, _src);
#else  // linux
        strncpy(retVal, _src, bufferSize);
#endif

        return retVal;
    }
}

static char* vktrace_allocate_and_copy_n(const char* _src, int _count) {
    size_t bufferSize = 1 + _count;

    char* retVal = VKTRACE_NEW_ARRAY(char, bufferSize);

#if defined(WIN32)
    strncpy_s(retVal, bufferSize, _src, _count);
#else  // linux
    strncpy(retVal, _src, _count);
    retVal[_count] = '\0';
#endif

    return retVal;
}

static char* vktrace_copy_and_append(const char* pBaseString, const char* pSeparator, const char* pAppendString) {
    size_t baseSize = (pBaseString != NULL) ? strlen(pBaseString) : 0;
    size_t separatorSize = ((pAppendString != NULL) && strlen(pAppendString) && (pSeparator != NULL)) ? strlen(pSeparator) : 0;
    size_t appendSize = (pAppendString != NULL) ? strlen(pAppendString) : 0;
    size_t bufferSize = baseSize + separatorSize + appendSize + 1;
    char* retVal = VKTRACE_NEW_ARRAY(char, bufferSize);
    if (retVal != NULL) {
#if defined(WIN32)
        strncpy_s(retVal, bufferSize, pBaseString, baseSize);
        strncpy_s(&retVal[baseSize], bufferSize - baseSize, pSeparator, separatorSize);
        strncpy_s(&retVal[baseSize + separatorSize], bufferSize - baseSize - separatorSize, pAppendString, appendSize);
#else  // linux
        strncpy(retVal, pBaseString, baseSize);
        strncpy(&retVal[baseSize], pSeparator, separatorSize);
        strncpy(&retVal[baseSize + separatorSize], pAppendString, appendSize);
#endif
    }
    retVal[bufferSize - 1] = '\0';
    return retVal;
}

static char* vktrace_copy_and_append_args(const char* pBaseString, const char* pSeparator, const char* pAppendFormat,
                                          va_list args) {
    size_t baseSize = (pBaseString != NULL) ? strlen(pBaseString) : 0;
    size_t separatorSize = (pSeparator != NULL) ? strlen(pSeparator) : 0;
    size_t appendSize = 0;
    size_t bufferSize = 0;
    char* retVal = NULL;

#if defined(WIN32)
    appendSize = _vscprintf(pAppendFormat, args);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_OSX) || defined(PLATFORM_BSD)
    va_list argcopy;
    va_copy(argcopy, args);
    appendSize = vsnprintf(NULL, 0, pAppendFormat, argcopy);
    va_end(argcopy);
#endif

    bufferSize = baseSize + separatorSize + appendSize + 1;
    retVal = VKTRACE_NEW_ARRAY(char, bufferSize);
    if (retVal != NULL) {
#if defined(WIN32)
        strncpy_s(retVal, bufferSize, pBaseString, baseSize);
        strncpy_s(&retVal[baseSize], bufferSize - baseSize, pSeparator, separatorSize);
        _vsnprintf_s(&retVal[baseSize + separatorSize], bufferSize - baseSize - separatorSize, appendSize, pAppendFormat, args);
#else  // linux
        strncpy(retVal, pBaseString, baseSize);
        strncpy(&retVal[baseSize], pSeparator, separatorSize);
        vsnprintf(&retVal[baseSize + separatorSize], appendSize, pAppendFormat, args);
#endif
    }
    return retVal;
}
