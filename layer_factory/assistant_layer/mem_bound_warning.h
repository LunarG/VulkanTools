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
    //     // Constructor for interceptor
    MemBoundWarning(){};

    VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory, VkResult result);

    template <typename T>
    void SetBindMemoryState(T handle, VkDeviceMemory memory);
    VkResult PostCallBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset,
                                      VkResult result);
    VkResult PostCallBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo *pBindInfos,
                                       VkResult result);
    VkResult PostCallBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo *pBindInfos,
                                          VkResult result);
    VkResult PostCallBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset,
                                     VkResult result);
    VkResult PostCallBindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfoKHR *pBindInfos,
                                      VkResult result);
    VkResult PostCallBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo *pBindInfos,
                                         VkResult result);
    VkResult PostCallQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo *pBindInfo, VkFence fence,
                                     VkResult result);

    template <typename T>
    void RemoveBindedMemory(T handle);
    VkResult PostCallCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                                        const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain, VkResult result);
    VkResult PostCallGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pSwapchainImageCount,
                                           VkImage *pSwapchainImages, VkResult result);
    void PostCallDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks *pAllocator);
    void PreCallDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks *pAllocator);
    void PreCallDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks *pAllocator);

    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator);

   private:
    // double linked maps from device memory to a set containing the uint64_t versions of the handle.
    std::unordered_map<VkDeviceMemory, std::unordered_set<uint64_t>> memory_handle_map;
    std::unordered_map<uint64_t, std::unordered_set<VkDeviceMemory>> handle_memory_map;
    std::unordered_map<VkSwapchainKHR, std::vector<VkImage>> swapchain_image_map;
};