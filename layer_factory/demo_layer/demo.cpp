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

#include "demo.h"
#include <sstream>

static uint32_t display_rate = 60;

// This function will be called for every API call
void MemDemo::PreCallApiFunction(const char *api_name) { printf("Calling %s\n", api_name); }

// Intercept the memory allocation calls and increment the counter
VkResult MemDemo::PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                         const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory, VkResult result) {
    number_mem_objects_++;
    total_memory_ += pAllocateInfo->allocationSize;
    mem_size_map_[*pMemory] = pAllocateInfo->allocationSize;
    return VK_SUCCESS;
}

// Intercept the free memory calls and update totals
void MemDemo::PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
    if (memory != VK_NULL_HANDLE) {
        number_mem_objects_--;
        VkDeviceSize this_alloc = mem_size_map_[memory];
        total_memory_ -= this_alloc;
    }
}

VkResult MemDemo::PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
    present_count_++;
    if (present_count_ >= display_rate) {
        present_count_ = 0;

        std::stringstream message;
        message << "Memory Allocation Count: " << number_mem_objects_ << "\n";
        message << "Total Memory Allocation Size: " << total_memory_ << "\n\n";

        // Various text output options:
        // Call through simplified interface to log_msg()
        MemDemo::Information(message.str());

        // Call log_msg() directly
        log_msg(vlf_report_data, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, 0,
                "kVUIDUndefined", "Memory Allocation Count: %d\nTotal Memory Allocation Size: 0x%" PRIx64 ".\n",
                number_mem_objects_, total_memory_);

#ifdef _WIN32
        // On Windows, call OutputDebugString to send output to the MSVC output window or debug out
        std::string str = message.str();
        LPCSTR cstr = str.c_str();
        OutputDebugStringA(cstr);
#endif

        // Option 3, use printf to stdout
        printf("Demo layer: %s\n", message.str().c_str());
    }

    return VK_SUCCESS;
}

MemDemo demo_mem_layer;
