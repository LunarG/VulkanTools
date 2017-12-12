/*
* Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>

// use PPL parallel_invoke call(on windows for now, but PPL also has a PPLx for Linux), or use cross-platform memcpy multithread
// which exclude PPL
//#define PAGEGUARD_MEMCPY_USE_PPL_LIB
#include "vktrace_platform.h"

// Pageguard is only used on Windows, but we use lots of the pageguard code
// to implement support for persistently mapped buffers on Linux, so we set
// USE_PAGEGUARD_SPEEDUP.
//
// The pmb implementation on Windows involves setting a page guard on a pmb
// and an exception handler getting called when the pmb is modified by the
// user program. On Linux, instead of using an exception handler, the
// file /proc/<pid>/pagemap is read to determine what pages have been
// modified in the pmb.

#define USE_PAGEGUARD_SPEEDUP
#if defined(WIN32)
#if defined(PAGEGUARD_MEMCPY_USE_PPL_LIB)
#include <ppl.h>
using namespace concurrency;
#endif
#else
#include <semaphore.h>
#include <pthread.h>
#endif

typedef struct __PageGuardChangedBlockInfo {
    size_t offset;
    size_t length;
    size_t reserve0;
    size_t reserve1;
} PageGuardChangedBlockInfo, *pPageGuardChangedBlockInfo;

#if defined(WIN32)
typedef HANDLE vktrace_pageguard_thread_id;
typedef HANDLE vktrace_sem_id;
#else
typedef pthread_t vktrace_pageguard_thread_id;
typedef sem_t* vktrace_sem_id;
#endif

#ifdef __cplusplus
bool vktrace_sem_create(vktrace_sem_id *sem_id, uint32_t initvalue);
void vktrace_sem_delete(vktrace_sem_id sid);
void vktrace_sem_wait(vktrace_sem_id sid);
void vktrace_sem_post(vktrace_sem_id sid);
void vktrace_pageguard_memcpy_multithread(void *dest, const void *src, size_t n);
extern "C" void *vktrace_pageguard_memcpy(void *destination, const void *source, size_t size);
#else
void* vktrace_pageguard_memcpy(void* destination, const void* source, size_t size);
#endif

#define PAGEGUARD_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES 0X00000001
