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
#include <iomanip>

class ApiVersionWarning : public layer_factory {
   public:
    // Constructor for interceptor
    ApiVersionWarning(){};

    // get the API name is proper format
    std::string GetAPIVersionName(uint32_t version) {
        std::stringstream version_name;
        uint32_t major = VK_VERSION_MAJOR(version);
        uint32_t minor = VK_VERSION_MINOR(version);
        uint32_t patch = VK_VERSION_PATCH(version);

        version_name << major << "." << minor << "." << patch << " (0x" << std::setfill('0') << std::setw(8) << std::hex << version
                     << ")";

        return version_name.str();
    }

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
        device_api_version = physical_device_properties.apiVersion;

        // check api versions and warn if instance api Version is higher than version on device.
        // NOTE: Should be able to use ">" operator as other places in validation layers does so
        if (instance_api_version > device_api_version) {
            std::stringstream message;
            std::string inst_api_name = GetAPIVersionName(instance_api_version);
            std::string dev_api_name = GetAPIVersionName(device_api_version);

            message << "vkCreateDevice: API Version of current instance, " << inst_api_name
                    << " is higher than API Version on device, " << dev_api_name << ".";
            std::cout << message.str() << std::endl;
            Warning(message.str());
        }

        return VK_SUCCESS;
    }

   private:
    // api version for the instance and device
    uint32_t instance_api_version;
    uint32_t device_api_version;
};

ApiVersionWarning api_version_warning;
