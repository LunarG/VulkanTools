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

//  Optimization by using page-guard for speed up capture and capture persistent mapped memory
//
//  Background:
//
//     The speed is extremely slow when use vktrace to capture DOOM4. It took over half a day and 900G of trace for a capture from beginning to the game menu.
//     The reason that caused such slow capture is DOOM updates a big mapped memory(over 67M) frequently, vktrace copies this memory block to harddrive when DOOM calls vkFlushmappedMemory to update it every time.
//     Here we use page guard to record which page of big memory block has been changed and only save those changed pages, it make the capture time reduce to round 15 minutes, the trace file size is round 40G, 
//     The Playback time for these trace file is round 7 minutes(on Win10/AMDFury/32GRam/I5 system).
//
//     The page guard method also has been tested ok for capture/playback SaschaWillems(July20) applications. Test system:  CPU: AMD FX-9590, GPU: R9 200 Series, RAM: 16 GB, Driver: Crimson Public Driver 16.7.3.
//
//  Page guard process:
//
//     1. The target app call map buffer and get a mapped memory pointer(for example pMemReal).
//
//     2. The capturer then allocates a duplicated memory of at least the same size(for example pMemCopy). Copies all data from pMemReal to pMemCopy
//
//     3. The capturer adds page guard to pMemCopy for every page.Then returns the pMemCopy(not pMemReal) to the target app.
//
//     4. When the target app uses pMemCopy to write, page guard will be triggered and page guard handler will records which page has been changed.
//
//     5. When the target app uses pMemCopy to read data, page guard will be triggered and page guard handler records which page the target app wants to read. And only copy that page from real mapped memory to copied memory so after page guard exception, target app will read right data.
//
//     6. When the target app calls CPU->GPU synchronization(vkFlushMappedMemoryRanges and submits queue), the capturer saves all changed pages and also copies back these changed pages to pMemReal from pMemCopy, resets all page guard which triggered by write.
//
//     7. When the target app calls GPU->CPU synchronization(vkInvalidateMappedMemoryRanges, also include vkQueueSubmit which is before that synchronization happen), clear all read array and resets all page guard which triggered by read.
//
//     8. When the target app calls to unmap the memory, the capturer saves all changed pages to HD and also copies back these changed pages to pMemReal from pMemCopy. Finally removes all page guards and frees pMemCopy.
//
//  Known limitations:
//
//     1. for a page which is in mapped memory, if target app first read it before write, then do GPU->CPU synchronization like vkFlushMappedMemoryRanges trigger flush copied mapped memory( pMemCopy ) to real mapped memory( pMemReal ), that page will not be recorded as changed page.
//
//     2. one page accessed by a thread and the access just happen when another thread already finish copying date to real mapped memory for that page but haven't reset page guard of that page, that page will not be recorded as changed page.

#pragma once

#include <stdbool.h>
#include <unordered_map>
#include "vulkan/vulkan.h"
#include "vktrace_platform.h"
#include "vktrace_common.h"

#include "vktrace_interconnect.h"
#include "vktrace_filelike.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_exts.h"
#include <stdio.h>

#include "vktrace_pageguard_memorycopy.h"


#define PAGEGUARD_PAGEGUARD_ENABLE_ENV "_VKTRACE_OPTIMIZE_PMB"
#define PAGEGUARD_PAGEGUARD_TARGET_RANGE_SIZE_ENV "VKTRACE_PAGEGUARDTARGETSIZE"

// VKTRACE_PAGEGUARD_ENABLE_READ_PMB env var enables read PMB support. Only
// supported on Windows. Not yet tested.
#define PAGEGUARD_PAGEGUARD_ENABLE_READ_PMB_ENV "VKTRACE_PAGEGUARD_ENABLE_READ_PMB"

// Usefull macro for handling fatal errors during tracing
#define VKTRACE_FATAL_ERROR(_m) \
    do { \
        vktrace_LogError(_m " Fatal error."); \
        exit(1); \
    } while (0)


VkDeviceSize& ref_target_range_size();
bool getPageGuardEnableFlag();
bool getEnableReadPMBFlag();
#if defined(WIN32)
void setPageGuardExceptionHandler();
void removePageGuardExceptionHandler();
#endif
size_t pageguardGetAdjustedSize(size_t size);
void* pageguardAllocateMemory(size_t size);
void pageguardFreeMemory(void* pMemory);
DWORD pageguardGetSystemPageSize();

void pageguardEnter();
void pageguardExit();


void setFlagTovkFlushMappedMemoryRangesSpecial(PBYTE pOPTPackageData);


void flushAllChangedMappedMemory(vkFlushMappedMemoryRangesFunc pFunc);

void flushTargetChangedMappedMemory(LPPageGuardMappedMemory TargetMappedMemory, vkFlushMappedMemoryRangesFunc pFunc, VkMappedMemoryRange*  pMemoryRanges);

void resetAllReadFlagAndPageGuard();

#if defined(WIN32)
LONG WINAPI PageGuardExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo);
#endif

VkResult vkFlushMappedMemoryRangesWithoutAPICall(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);

PageGuardCapture& getPageGuardControlInstance();
