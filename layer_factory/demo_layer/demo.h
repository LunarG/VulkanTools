/*
 * Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

#pragma once

#include <unordered_map>
#include "vulkan/vulkan.h"
#include "vk_layer_logging.h"
#include "layer_factory.h"

class MemDemo : public layer_factory {
   public:
    // Constructor for state_tracker
    MemDemo() : number_mem_objects_(0), total_memory_(0), present_count_(0){};

    void PreCallApiFunction(const char *api_name);

    VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory, VkResult result);

    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator);

    VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo);

   private:
    uint32_t number_mem_objects_;
    VkDeviceSize total_memory_;
    uint32_t present_count_;
    std::unordered_map<VkDeviceMemory, VkDeviceSize> mem_size_map_;
};
