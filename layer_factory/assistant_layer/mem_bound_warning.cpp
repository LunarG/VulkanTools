/*
 * Copyright (c) 2015-2018 Valve Corporation
 * Copyright (c) 2015-2018 LunarG, Inc.
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
 * Author: Camden Stocker <camden@lunarg.com>
 */

#include "mem_bound_warning.h"
#include <iostream>
#include <assert.h>

// helper function that update all the dictionaries for both image and buffer handles
template <typename T>
void MemBoundWarning::SetBindMemoryState(T handle, VkDeviceMemory memory) {
    uint64_t handle_64 = HandleToUint64(handle);

    if (memory != VK_NULL_HANDLE) {
        auto &map_pair = handle_memory_map[handle_64];
        map_pair.insert(memory);

        auto &handle_set = memory_handle_map[memory];
        handle_set.insert(handle_64);
    }
}

// 3 buffer memory state tracker entry points
VkResult MemBoundWarning::PostCallBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                                   VkDeviceSize memoryOffset, VkResult result) {
    if (result == VK_SUCCESS) SetBindMemoryState<VkBuffer>(buffer, memory);

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindBufferMemory2(VkDevice device, uint32_t bindInfoCount,
                                                    const VkBindBufferMemoryInfo *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkBuffer>(pBindInfos[i].buffer, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount,
                                                       const VkBindBufferMemoryInfo *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkBuffer>(pBindInfos[i].buffer, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

// 3 buffer memory state tracker entry points
VkResult MemBoundWarning::PostCallBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset,
                                                  VkResult result) {
    if (result == VK_SUCCESS) SetBindMemoryState<VkImage>(image, memory);

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindImageMemory2(VkDevice device, uint32_t bindInfoCount,
                                                   const VkBindImageMemoryInfoKHR *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkImage>(pBindInfos[i].image, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount,
                                                      const VkBindImageMemoryInfo *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkImage>(pBindInfos[i].image, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

// intercept for the sparse memory to track memory state
VkResult MemBoundWarning::PostCallQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo *pBindInfo,
                                                  VkFence fence, VkResult result) {
    for (uint32_t bindIdx = 0; bindIdx < bindInfoCount; bindIdx++) {
        const VkBindSparseInfo &bindInfo = pBindInfo[bindIdx];

        for (uint32_t j = 0; j < bindInfo.bufferBindCount; j++) {
            for (uint32_t k = 0; k < bindInfo.pBufferBinds[j].bindCount; k++) {
                auto &sparse_binding = bindInfo.pBufferBinds[j].pBinds[k];
                SetBindMemoryState<VkBuffer>(bindInfo.pBufferBinds[j].buffer, sparse_binding.memory);
            }
        }
        for (uint32_t j = 0; j < bindInfo.imageOpaqueBindCount; j++) {
            for (uint32_t k = 0; k < bindInfo.pImageOpaqueBinds[j].bindCount; k++) {
                auto &sparse_binding = bindInfo.pImageOpaqueBinds[j].pBinds[k];
                SetBindMemoryState<VkImage>(bindInfo.pImageOpaqueBinds[j].image, sparse_binding.memory);
            }
        }
        for (uint32_t j = 0; j < bindInfo.imageBindCount; j++) {
            for (uint32_t k = 0; k < bindInfo.pImageBinds[j].bindCount; k++) {
                auto &sparse_binding = bindInfo.pImageBinds[j].pBinds[k];
                SetBindMemoryState<VkImage>(bindInfo.pImageBinds[j].image, sparse_binding.memory);
            }
        }
    }

    return VK_SUCCESS;
}

// Get memory associated with the handle being destroyed then find handle set using this memory and erase it
template <typename T>
void MemBoundWarning::RemoveBindedMemory(T handle) {
    assert(handle);
    uint64_t handle_64 = HandleToUint64(handle);
    auto map_pair = handle_memory_map.find(handle_64);
    if (map_pair == handle_memory_map.end()) return;  // this will happen if handle was never Binded to memory...prevents segfault
    const auto &memory_set = map_pair->second;

    // remove all memory in the memory set. If it is not sparse memory, there will only be one item in set
    for (VkDeviceMemory memory : memory_set) {
        auto map_pair = memory_handle_map.find(memory);
        if (map_pair == memory_handle_map.end())
            continue;  // this will happen if handle was never Binded to memory...prevents segfault
        auto &handle_set = map_pair->second;
        handle_set.erase(handle_64);
    }
}

void MemBoundWarning::PreCallDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks *pAllocator) {
    RemoveBindedMemory<VkBuffer>(buffer);
}

void MemBoundWarning::PreCallDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks *pAllocator) {
    RemoveBindedMemory<VkImage>(image);
}

void MemBoundWarning::PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
    assert(memory);
    auto map_pair = memory_handle_map.find(memory);
    if (map_pair == memory_handle_map.end()) return;  // this will happen if handle was never Binded to memory...prevents segfault
    const auto &handle_set = map_pair->second;

    for (const auto &handle : handle_set) {
        char handle_string[64];
        char memory_string[64];
        uint64_t handle_64 = HandleToUint64(handle);
        uint64_t memory_64 = HandleToUint64(memory);
        sprintf(handle_string, "0x%" PRIxLEAST64, handle_64);
        sprintf(memory_string, "0x%" PRIxLEAST64, memory_64);

        std::stringstream message;
        message << "Vk Object " << handle_string << " still has a reference to Memory Object " << memory_string;
        Information(message.str());
    }
}

MemBoundWarning mem_bound_warning;