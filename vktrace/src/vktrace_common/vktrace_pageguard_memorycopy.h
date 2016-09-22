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

//use PPL parallel_invoke call(on windows for now, but PPL also has a PPLx for Linux), or use cross-platform memcpy multithread which exclude PPL
//#define PAGEGUARD_MEMCPY_USE_PPL_LIB
#include "vktrace_platform.h"

#if defined(WIN32)
    #define USE_PAGEGUARD_SPEEDUP
    #if defined(PAGEGUARD_MEMCPY_USE_PPL_LIB)
        #include <ppl.h>
        using namespace concurrency;
    #endif
#else
    #include <semaphore.h>
    #include <pthread.h>
#endif


typedef struct __PageGuardChangedBlockInfo
{
    uint32_t offset;
    uint32_t length;
    uint32_t reserve0;
    uint32_t reserve1;
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
#endif

#define PAGEGUARD_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES 0X00000001
