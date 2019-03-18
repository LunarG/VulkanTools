/*
 * Copyright (c) 2016-2019 Advanced Micro Devices, Inc. All rights reserved.
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

static const int BLOCK_FLAG_ARRAY_CHANGED = 0;
static const int BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT = 1;
static const int BLOCK_FLAG_ARRAY_READ = 2;
static const int BLOCK_FLAG_ARRAY_READ_SNAPSHOT = 3;

typedef class PageStatusArray {
   public:
    PageStatusArray(uint64_t pageCount);
    ~PageStatusArray();

    void toggleChangedArray();
    void toggleReadArray();

    bool getBlockChangedArray(uint64_t index);
    bool getBlockChangedArraySnapshot(uint64_t index);
    bool getBlockReadArray(uint64_t index);
    bool getBlockReadArraySnapshot(uint64_t index);
    bool getBlockFirstTimeLoadArray(uint64_t index);
    void setBlockChangedArray(uint64_t index, bool changed);
    void setBlockChangedArraySnapshot(uint64_t index, bool changed);
    void setBlockReadArray(uint64_t index, bool changed);
    void setBlockReadArraySnapshot(uint64_t index, bool changed);
    void setBlockFirstTimeLoadArray(uint64_t index, bool loaded);
    void backupChangedArray();
    void backupReadArray();
    void clearAll();
    void clearActiveChangesArray();

   private:
    const static uint64_t PAGE_FLAG_AMOUNT_PER_BYTE;
    const static uint64_t PAGE_NUMBER_FROM_BIT_SHIFT;
    uint64_t ByteCount;
    uint8_t *activeChangesArray;
    uint8_t *capturedChangesArray;
    uint8_t *activeReadArray;
    uint8_t *capturedReadArray;
    uint8_t *pChangedArray[2];  /// include two array, one for page guard handler to record which block has been changed from
                                /// vkMap.. or last time vkFlush..., the other one for flush data and reset pageguard
    uint8_t *pReadArray[2];     /// include two array, one for page guard handler to record which block has been read by host from
                                /// vkMap.. or last time vkinvalidate or vkpipelinebarrier with specific para..., the other one for
                                /// reset page guard

    uint8_t *firstTimeLoadArray;
    /// the array is used for remove initial memcpy real mapped memory to shadow
    /// mapped memory in map process. When target app call map/unmap memory,
    /// some title use large mapped size and only access small part of mapped
    /// memory, if do memcpy for whole mapped size, that cause target title
    /// capture speed extremely slow, so we remove the memcpy from map process,
    /// and put the work in page guard exception handler at first time access to
    /// the page, and only for those pages which are truly accessed.
    ///
    /// we give every page a flag in this array to indicate that page is loaded
    /// or not. In pageguard handler, if the page hasn't been loaded, we copy
    /// the page from real mapped memory to shadow memory to finish the work that
    /// we do in map process previously and set the flag to loaded. The optimization
    /// significantly improve some title capture time from hours to minutes.
    ///
    /// The titles which use big size multiple map/unmap can get capture speed
    /// improved by this optimization, especially if the title only access small
    /// part of mapped size during pair of map/unmap.
    ///
    /// currently the optimization is only used in Windows because it depend on
    /// the page memcpy is inserted just before first access to the page
    /// which is implemented by page guard handler, once we find a way on other
    /// platforms to capture read/write a page, we can also use it on those
    /// platforms.

} PageStatusArray;
