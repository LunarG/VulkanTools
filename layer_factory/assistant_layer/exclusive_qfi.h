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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

#pragma once

#include <string>
#include <sstream>

// This interceptor will warn if the user specifies a queuFamilyIndexCount > 1 while specifying a sharingMode of EXCLUSIVE.

class ExclusiveQfiCheck : public layer_factory {
   public:
    // Constructor for interceptor
    ExclusiveQfiCheck() : layer_factory(this){};

    VkResult PreCallCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                 VkBuffer* pBuffer) {
        if ((pCreateInfo->queueFamilyIndexCount > 1) && (pCreateInfo->sharingMode == VK_SHARING_MODE_EXCLUSIVE)) {
            std::stringstream message;
            message << "Warning:  Buffer (0x" << std::hex << HandleToUint64(pBuffer)
                    << ") specifies a sharing mode of VK_SHARING_MODE_EXCLUSIVE while specifying multiple queues "
                       "(queueFamilyIndexCount of "
                    << pCreateInfo->queueFamilyIndexCount << ").";
            Warning(message.str());
        }
        return VK_SUCCESS;
    };

    VkResult PreCallCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                VkImage* pImage) {
        if ((pCreateInfo->queueFamilyIndexCount > 1) && (pCreateInfo->sharingMode == VK_SHARING_MODE_EXCLUSIVE)) {
            std::stringstream message;
            message << "Warning:  Image (0x" << std::hex << HandleToUint64(pImage)
                    << ") specifies a sharing mode of VK_SHARING_MODE_EXCLUSIVE while specifying multiple queues "
                       "(queueFamilyIndexCount of "
                    << pCreateInfo->queueFamilyIndexCount << ").";
            Warning(message.str());
        }
        return VK_SUCCESS;
    };

    VkResult PreCallCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
        if ((pCreateInfo->queueFamilyIndexCount > 1) && (pCreateInfo->imageSharingMode == VK_SHARING_MODE_EXCLUSIVE)) {
            std::stringstream message;
            message << "Warning:  A Swapchain is being created which specifies a sharing mode of VK_SHARING_MODE_EXCLUSIVE while "
                       "specifying multiple queues (queueFamilyIndexCount of "
                    << pCreateInfo->queueFamilyIndexCount << ").";
            Warning(message.str());
        }
        return VK_SUCCESS;
    }

    VkResult vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos,
                                         const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) {
        for (uint32_t i = 0; i < swapchainCount; i++) {
            if ((pCreateInfos[i].queueFamilyIndexCount > 1) && (pCreateInfos[i].imageSharingMode == VK_SHARING_MODE_EXCLUSIVE)) {
                std::stringstream message;
                message << "Warning:  A Shared Swapchain (index " << i
                        << ") is being created which specifies a sharing mode of VK_SHARING_MODE_EXCLUSIVE while "
                           "specifying multiple queues (queueFamilyIndexCount of "
                        << pCreateInfos[i].queueFamilyIndexCount << ").";
                Warning(message.str());
            }
        }
        return VK_SUCCESS;
    }
};

ExclusiveQfiCheck exclusive_qfi_check;
