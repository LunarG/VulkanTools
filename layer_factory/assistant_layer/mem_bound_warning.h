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

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "vulkan/vulkan.h"
#include "vk_layer_logging.h"
#include "layer_factory.h"

class MemBoundWarning : public layer_factory {
   public:
    // Constructor for interceptor
    MemBoundWarning(){};

    VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory, VkResult result) {
        if (result == VK_SUCCESS) {
            // create new memory entry into the map but give it no values
            std::unordered_set<uint64_t> set;
            memory_handle_map.insert(std::make_pair(*pMemory, set));
        }
    }

    // helper functin that update all the dictionaries for both image and buffer handles
    template <typename T>
    void SetBindMemoryState(T handle, VkDeviceMemory memory) {
        uint64_t handle_64 = HandleToUint64(handle);

        if (memory != VK_NULL_HANDLE) {
            // If the entry does not exist yet, then create it
            if (handle_memory_map.count(handle_64) < 0) {
                std::unordered_set<VkDeviceMemory> set;
                handle_memory_map.insert(std::make_pair(handle_64, set));
            }

            auto memory_set = handle_memory_map.find(handle_64)->second;
            memory_set.insert(memory);

            auto handle_set = memory_handle_map.find(memory)->second;
            handle_set.insert(handle_64);
        }
    }

    // 3 buffer memory state tracker entry points
    VkResult PostCallBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset,
                                      VkResult result) {
        if (result == VK_SUCCESS) SetBindMemoryState<VkBuffer>(buffer, memory);
    }

    VkResult PostCallBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo *pBindInfos,
                                       VkResult result) {
        if (result == VK_SUCCESS) {
            for (uint32_t i = 0; i < bindInfoCount; i++) {
                SetBindMemoryState<VkBuffer>(pBindInfos[i].buffer, pBindInfos[i].memory);
            }
        }
    }

    VkResult PostCallBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo *pBindInfos,
                                          VkResult result) {
        if (result == VK_SUCCESS) {
            for (uint32_t i = 0; i < bindInfoCount; i++) {
                SetBindMemoryState<VkBuffer>(pBindInfos[i].buffer, pBindInfos[i].memory);
            }
        }
    }

    // 3 buffer memory state tracker entry points
    VkResult PostCallBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset,
                                     VkResult result) {
        if (result == VK_SUCCESS) SetBindMemoryState<VkImage>(image, memory);
    }

    VkResult PostCallBindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfoKHR *pBindInfos,
                                      VkResult result) {
        if (result == VK_SUCCESS) {
            for (uint32_t i = 0; i < bindInfoCount; i++) {
                SetBindMemoryState<VkImage>(pBindInfos[i].image, pBindInfos[i].memory);
            }
        }
    }

    VkResult PostCallBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo *pBindInfos,
                                         VkResult result) {
        if (result == VK_SUCCESS) {
            for (uint32_t i = 0; i < bindInfoCount; i++) {
                SetBindMemoryState<VkImage>(pBindInfos[i].image, pBindInfos[i].memory);
            }
        }
    }

    // intercept for the Sparse memory to track memory state
    VkResult PostCallQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo *pBindInfo, VkFence fence,
                                     VkResult result) {
        for (uint32_t bindIdx = 0; bindIdx < bindInfoCount; bindIdx++) {
            const VkBindSparseInfo &bindInfo = pBindInfo[bindIdx];

            for (uint32_t j = 0; j < bindInfo.bufferBindCount; j++) {
                for (uint32_t k = 0; k < bindInfo.pBufferBinds[j].bindCount; k++) {
                    auto sparse_binding = bindInfo.pBufferBinds[j].pBinds[k];
                    SetBindMemoryState<VkBuffer>(bindInfo.pBufferBinds[j].buffer, sparse_binding.memory);
                }
            }
            for (uint32_t j = 0; j < bindInfo.imageOpaqueBindCount; j++) {
                for (uint32_t k = 0; k < bindInfo.pImageOpaqueBinds[j].bindCount; k++) {
                    auto sparse_binding = bindInfo.pImageOpaqueBinds[j].pBinds[k];
                    SetBindMemoryState<VkImage>(bindInfo.pImageOpaqueBinds[j].image, sparse_binding.memory);
                }
            }
            for (uint32_t j = 0; j < bindInfo.imageBindCount; j++) {
                for (uint32_t k = 0; k < bindInfo.pImageBinds[j].bindCount; k++) {
                    auto sparse_binding = bindInfo.pImageBinds[j].pBinds[k];
                    SetBindMemoryState<VkImage>(bindInfo.pImageBinds[j].image, sparse_binding.memory);
                }
            }
        }
    }

    // Get memory associated with the handle being destroyed then find handle set using this memory and erase it
    template <typename T>
    void RemoveBindedMemory(T handle) {
        uint64_t handle_64 = HandleToUint64(handle);
        auto memory_set = handle_memory_map.find(handle_64)->second;
        // remove all memory in the memory set. If it is not sparse memory, there will only be one item in set
        for (VkDeviceMemory memory : memory_set) {
            auto handle_set = memory_handle_map.find(memory)->second;
            handle_set.erase(handle_64);
        }
    }

    VkResult PostCallGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pSwapchainImageCount,
                                           VkImage *pSwapchainImages, VkResult result) {
        auto images = swapchain_image_map.find(swapchain)->second;

        if (*pSwapchainImageCount > images.size()) images.resize(*pSwapchainImageCount);

        if (pSwapchainImages) {
            for (uint32_t i = 0; i < *pSwapchainImageCount; i++) {
                if (images[i] != VK_NULL_HANDLE) continue;  // Already retrieved image
                images[i] = pSwapchainImages[i];
            }
        }
    }

    void PostCallDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks *pAllocator,
                                     VkResult result) {
        if (!swapchain) return;

        auto images = swapchain_image_map.find(swapchain)->second;
        if (images.size() > 0) {
            for (auto swapchain_image : images) {
                RemoveBindedMemory<VkImage>(swapchain_image);
            }
        }

        swapchain_image_map.erase(swapchain);
    }

    void PreCallDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks *pAllocator) {
        RemoveBindedMemory<VkBuffer>(buffer);
    }

    void PreCallDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks *pAllocator) {
        RemoveBindedMemory<VkImage>(image);
    }

    // TODO: You know what TO DO :)
    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
        auto handle_set = memory_handle_map.find(memory)->second;

        for (const auto &handle : handle_set) {
            uint64_t handle_64 = HandleToUint64(handle);
            uint64_t memory_64 = HandleToUint64(memory);

            char handle_string[64];
            char memory_string[64];
            sprintf(handle_string, "0x%" PRIxLEAST64, handle_64);
            sprintf(memory_string, "0x%" PRIxLEAST64, memory_64);

            std::stringstream message;
            message << "Vk Object" << handle_string << "still has a reference to mem obj" << memory_string;
            Information(message.str());
        }
    }

   private:
    // double linked maps from device memory to a set containing the uint64_t versions of the handle.
    std::unordered_map<VkDeviceMemory, std::unordered_set<uint64_t>> memory_handle_map;
    std::unordered_map<uint64_t, std::unordered_set<VkDeviceMemory>> handle_memory_map;
    std::unordered_map<VkSwapchainKHR, std::vector<VkImage>> swapchain_image_map;
};

MemBoundWarning mem_bound_warning;