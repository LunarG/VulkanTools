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

#include <sstream>
#include <unordered_map>

static uint32_t display_rate = 60;

class MemAllocLevel : public layer_factory {
   public:
    // Constructor for interceptor
    MemAllocLevel() : number_mem_objects_(0), total_memory_(0), present_count_(0){};

    // Intercept the memory allocation calls and increment the counter
    VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory, VkResult result) {
        number_mem_objects_++;
        total_memory_ += pAllocateInfo->allocationSize;
        mem_size_map_[*pMemory] = pAllocateInfo->allocationSize;
        return VK_SUCCESS;
    };

    // Intercept the free memory calls and update totals
    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
        if (memory != VK_NULL_HANDLE) {
            number_mem_objects_--;
            VkDeviceSize this_alloc = mem_size_map_[memory];
            total_memory_ -= this_alloc;
        }
    }

    VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
        present_count_++;
        if (present_count_ >= display_rate) {
            present_count_ = 0;
            std::stringstream message;
            message << "Memory Allocation Count: " << number_mem_objects_ << "\n";
            message << "Total Memory Allocation Size: " << total_memory_ << "\n\n";
            Information(message.str());
        }
        return VK_SUCCESS;
    }

   private:
    // Counter for the number of currently active memory allocations
    uint32_t number_mem_objects_;
    VkDeviceSize total_memory_;
    uint32_t present_count_;
    std::unordered_map<VkDeviceMemory, VkDeviceSize> mem_size_map_;
};

MemAllocLevel memory_allocation_stats;
