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

#include <string>
#include <sstream>
#include <algorithm>

class ExtensionTypeWarning : public layer_factory {
   public:
    // Constructor for interceptor
    ExtensionTypeWarning() : layer_factory(this){};

    // Intercept CreateInstance to warn for device extensions
    VkResult PreCallCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                   VkInstance* pInstance) {
        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            if (white_list(pCreateInfo->ppEnabledExtensionNames[i], kDeviceExtensionNames)) {
                std::stringstream message;
                message << "vkCreateInstance:  Attempting to enable Device Extension " << pCreateInfo->ppEnabledExtensionNames[i]
                        << " at CreateInstance time.";
                Warning(message.str());
            }
        }
        return VK_SUCCESS;
    };

    // Intercept CreateDevice to warn for instance extensions
    VkResult PostCallCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                                  const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            if (white_list(pCreateInfo->ppEnabledExtensionNames[i], kInstanceExtensionNames)) {
                std::stringstream message;
                message << "vkCreateDevice:  Attempting to enable Instance Extension " << pCreateInfo->ppEnabledExtensionNames[i]
                        << " at CreateDevice time.";
                Warning(message.str());
            }
        }
        return VK_SUCCESS;
    }
};

ExtensionTypeWarning extension_type_warning;
