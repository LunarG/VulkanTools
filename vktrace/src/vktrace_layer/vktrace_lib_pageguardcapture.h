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
#include "vktrace_lib_pageguardmappedmemory.h"

#if defined(WIN32) /// page guard solution for windows

typedef VkResult(*vkFlushMappedMemoryRangesFunc)(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange*  pMemoryRanges);

typedef class PageGuardCapture
{
private:
    PageGuardChangedBlockInfo EmptyChangedInfoArray;
    std::unordered_map< VkDeviceMemory, PageGuardMappedMemory > MapMemory;
    std::unordered_map< VkDeviceMemory, PBYTE > MapMemoryPtr;
public:

    PageGuardCapture();

    std::unordered_map< VkDeviceMemory, PageGuardMappedMemory >& getMapMemory();

    void vkMapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkFlags flags, void** ppData);

    void vkUnmapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, void** MappedData, vkFlushMappedMemoryRangesFunc pFunc);

    void* getMappedMemoryPointer(VkDevice device, VkDeviceMemory memory);

    /// return: if it's target mapped memory and no change at all;
    /// PBYTE *ppPackageDataforOutOfMap, must be an array include memoryRangeCount elements
    bool vkFlushMappedMemoryRangesPageGuardHandle(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges, PBYTE *ppPackageDataforOutOfMap);

    LPPageGuardMappedMemory findMappedMemoryObject(VkDevice device, VkDeviceMemory memory);

    LPPageGuardMappedMemory findMappedMemoryObject(PBYTE addr, VkDeviceSize *pOffsetOfAddr = nullptr, PBYTE *ppBlock = nullptr, VkDeviceSize *pBlockSize = nullptr);

    LPPageGuardMappedMemory findMappedMemoryObject(VkDevice device, const VkMappedMemoryRange* pMemoryRange);

    /// get size of all changed package in array of pMemoryRanges
    VkDeviceSize getALLChangedPackageSizeInMappedMemory(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges, PBYTE *ppPackageDataforOutOfMap);

    /// get ptr and size of OPTChangedDataPackage;
    PBYTE getChangedDataPackageOutOfMap(PBYTE *ppPackageDataforOutOfMap, DWORD dwRangeIndex, VkDeviceSize  *pSize);

    void clearChangedDataPackageOutOfMap(PBYTE *ppPackageDataforOutOfMap, DWORD dwRangeIndex);

    bool isHostWriteFlagSetInMemoryBarriers(uint32_t  memoryBarrierCount, const VkMemoryBarrier*  pMemoryBarriers);

    bool isHostWriteFlagSetInBufferMemoryBarrier(uint32_t  memoryBarrierCount, const VkBufferMemoryBarrier*  pMemoryBarriers);

    bool isHostWriteFlagSetInImageMemoryBarrier(uint32_t  memoryBarrierCount, const VkImageMemoryBarrier*  pMemoryBarriers);

    bool isHostWriteFlagSet(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags  dependencyFlags,
        uint32_t   memoryBarrierCount, const VkMemoryBarrier*   pMemoryBarriers,
        uint32_t  bufferMemoryBarrierCount, const VkBufferMemoryBarrier*  pBufferMemoryBarriers,
        uint32_t  imageMemoryBarrierCount, const VkImageMemoryBarrier*  pImageMemoryBarriers);

    bool isReadyForHostReadInMemoryBarriers(uint32_t  memoryBarrierCount, const VkMemoryBarrier*  pMemoryBarriers);

    bool isReadyForHostReadInBufferMemoryBarrier(uint32_t  memoryBarrierCount, const VkBufferMemoryBarrier*  pMemoryBarriers);

    bool isReadyForHostReadInImageMemoryBarrier(uint32_t  memoryBarrierCount, const VkImageMemoryBarrier*  pMemoryBarriers);

    bool isReadyForHostRead(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags  dependencyFlags,
        uint32_t   memoryBarrierCount, const VkMemoryBarrier*   pMemoryBarriers,
        uint32_t  bufferMemoryBarrierCount, const VkBufferMemoryBarrier*  pBufferMemoryBarriers,
        uint32_t  imageMemoryBarrierCount, const VkImageMemoryBarrier*  pImageMemoryBarriers);

} PageGuardCapture;
//page guard for windows end

#endif//page guard solution
