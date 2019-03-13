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

#include "vktrace_pageguard_memorycopy.h"
#include "vktrace_lib_pageguardmappedmemory.h"

#define PAGEGUARD_TARGET_RANGE_SIZE_CONTROL

// PAGEGUARD_ADD_PAGEGUARD_ON_REAL_MAPPED_MEMORY is a compile flag for add page guard on real mapped memory.
// If comment this flag, pageguard will be added on a copy of mapped memory, with the flag, page guard will be added to
// real mapped memory.
// for some hareware, add to mapped memory not the copy of it may not be allowed, so turn on this flag just for you are already sure
// page guard can work on that hardware.
// If add page guard to the copy of mapped memory, it's always allowed but need to do synchonization between the mapped memory and
// its copy.

//#define PAGEGUARD_ADD_PAGEGUARD_ON_REAL_MAPPED_MEMORY

typedef VkResult (*vkFlushMappedMemoryRangesFunc)(VkDevice device, uint32_t memoryRangeCount,
                                                  const VkMappedMemoryRange* pMemoryRanges);

typedef class PageGuardCapture {
   private:
    PageGuardChangedBlockInfo EmptyChangedInfoArray;
    std::unordered_map<VkDeviceMemory, PageGuardMappedMemory> MapMemory;
    std::unordered_map<VkDeviceMemory, PBYTE> MapMemoryPtr;
    std::unordered_map<VkDeviceMemory, VkDeviceSize> MapMemorySize;
    std::unordered_map<VkDeviceMemory, VkDeviceSize> MapMemoryOffset;
    std::unordered_map<VkDeviceMemory, VkMemoryAllocateInfo> MapMemoryAllocateInfo;
    std::unordered_map<VkDevice, VkPhysicalDevice> MapDevice;
    std::unordered_map<VkDeviceMemory, void *> MapMemoryExtHostPointer;

   public:
    PageGuardCapture();

    std::unordered_map<VkDeviceMemory, PageGuardMappedMemory>& getMapMemory();

    /// Get memory object to its createinfo map reference
    std::unordered_map<VkDeviceMemory, VkMemoryAllocateInfo>& getMapMemoryAllocateInfo();
    std::unordered_map<VkDeviceMemory, void *>& getMapMemoryExtHostPointer();

    /// Get device to its physical device map reference
    std::unordered_map<VkDevice, VkPhysicalDevice>& getMapDevice();

    /// The method is used to track device creation to record what physical
    /// device is based to create the device. It's called when target app
    /// create device.
    void vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

    /// The method is used to maintain the device to its physical device map,
    /// it's called when target app destroy device.
    void vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator);

    /// The method is used to track memory allocation to record what
    /// VkMemoryAllocateInfo is based to allocate the memory. It's called when
    /// target app allocate memory.
    void vkAllocateMemoryPageGuardHandle(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo,
                                         const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory, void *pExternalHostPointer);

    /// The method is used to maintain the memory to its createinfo map,
    /// it's called when target app free memory.
    void vkFreeMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);

    void vkMapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkFlags flags,
                                    void** ppData);

    void vkUnmapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, void** MappedData,
                                      vkFlushMappedMemoryRangesFunc pFunc);

    void SyncRealMappedMemoryToMemoryCopyHandle(VkDevice device, VkDeviceMemory memory);

    void* getMappedMemoryPointer(VkDevice device, VkDeviceMemory memory);

    VkDeviceSize getMappedMemoryOffset(VkDevice device, VkDeviceMemory memory);

    VkDeviceSize getMappedMemorySize(VkDevice device, VkDeviceMemory memory);

    /// return: if it's target mapped memory and no change at all;
    /// PBYTE *ppPackageDataforOutOfMap, must be an array include memoryRangeCount elements
    bool vkFlushMappedMemoryRangesPageGuardHandle(VkDevice device, uint32_t memoryRangeCount,
                                                  const VkMappedMemoryRange* pMemoryRanges, PBYTE* ppPackageDataforOutOfMap);

    LPPageGuardMappedMemory findMappedMemoryObject(VkDevice device, VkDeviceMemory memory);

    LPPageGuardMappedMemory findMappedMemoryObject(PBYTE addr, VkDeviceSize* pOffsetOfAddr = nullptr, PBYTE* ppBlock = nullptr,
                                                   VkDeviceSize* pBlockSize = nullptr);

    LPPageGuardMappedMemory findMappedMemoryObject(VkDevice device, const VkMappedMemoryRange* pMemoryRange);

    /// get size of all changed package in array of pMemoryRanges
    VkDeviceSize getALLChangedPackageSizeInMappedMemory(VkDevice device, uint32_t memoryRangeCount,
                                                        const VkMappedMemoryRange* pMemoryRanges, PBYTE* ppPackageDataforOutOfMap);

    /// get ptr and size of OPTChangedDataPackage;
    PBYTE getChangedDataPackageOutOfMap(PBYTE* ppPackageDataforOutOfMap, DWORD dwRangeIndex, VkDeviceSize* pSize);

    void clearChangedDataPackageOutOfMap(PBYTE* ppPackageDataforOutOfMap, DWORD dwRangeIndex);

    bool isHostWriteFlagSetInMemoryBarriers(uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers);

    bool isHostWriteFlagSetInBufferMemoryBarrier(uint32_t memoryBarrierCount, const VkBufferMemoryBarrier* pMemoryBarriers);

    bool isHostWriteFlagSetInImageMemoryBarrier(uint32_t memoryBarrierCount, const VkImageMemoryBarrier* pMemoryBarriers);

    bool isHostWriteFlagSet(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                            uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                            const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                            const VkImageMemoryBarrier* pImageMemoryBarriers);

    bool isReadyForHostReadInMemoryBarriers(uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers);

    bool isReadyForHostReadInBufferMemoryBarrier(uint32_t memoryBarrierCount, const VkBufferMemoryBarrier* pMemoryBarriers);

    bool isReadyForHostReadInImageMemoryBarrier(uint32_t memoryBarrierCount, const VkImageMemoryBarrier* pMemoryBarriers);

    bool isReadyForHostRead(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                            uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                            const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                            const VkImageMemoryBarrier* pImageMemoryBarriers);
    VkMemoryPropertyFlags getMemoryPropertyFlags(VkDevice device, uint32_t memoryTypeIndex);
} PageGuardCapture;
