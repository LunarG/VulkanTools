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

#include <sstream>
#include <iostream>

class ApiVersionWarning : public layer_factory {
   public:
    // Constructor for interceptor
    ApiVersionWarning(){};

    // Intercept CreateInstance to capture API Version
    VkResult PreCallCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                   VkInstance* pInstance) {
        instance_api_version = pCreateInfo->pApplicationInfo->apiVersion;
        return VK_SUCCESS;
    }

    VkResult PreCallCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
        // get API version of physical device passed when creating device.
        instance_layer_data* instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
        VkPhysicalDeviceProperties physical_device_properties{};
        instance_data->dispatch_table.GetPhysicalDeviceProperties(physicalDevice, &physical_device_properties);

        // check api versions and warn if instance api Version is higher than version on device.
        // NOTE: Should be able to use ">" operator as other places in validation layers does so
        if (instance_api_version > physical_device_properties.apiVersion) {
            std::stringstream message;
            message << "vkCreateDevice: API Version of current instance is higher than API Version on device.";
            Warning(message.str());
        }

        return VK_SUCCESS;
    }

   private:
    // api version for the instance that was created
    uint32_t instance_api_version;
};

ApiVersionWarning api_version_warning;
