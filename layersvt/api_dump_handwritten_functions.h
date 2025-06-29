/* Copyright (c) 2015-2023 The Khronos Group Inc.
 * Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (C) 2015-2016 Google Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: David Pinedo <david@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

// Implementation file for specifically implemented functions

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include "generated/api_dump_implementation.h"

extern "C" {

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                                VkInstance* pInstance) {
    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    ApiDumpInstance::current().initLayerSettings(pCreateInfo, pAllocator);
    dump_function_head(ApiDumpInstance::current(), "vkCreateInstance", "pCreateInfo, pAllocator, pInstance", "VkResult");
    if (ApiDumpInstance::current().settings().shouldPreDump() &&
        ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text && ApiDumpInstance::current().shouldDumpOutput()) {
        dump_before_pre_dump_formatting<ApiDumpFormat::Text>(ApiDumpInstance::current().settings());
        dump_params_vkCreateInstance<ApiDumpFormat::Text>(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);
    }

    // Get the function pointer
    VkLayerInstanceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr != 0);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance)fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if (fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result == VK_SUCCESS) {
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
    }

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {
        switch (ApiDumpInstance::current().settings().format()) {
            case ApiDumpFormat::Text:
                dump_return_value<ApiDumpFormat::Text>(ApiDumpInstance::current().settings(), "VkResult", result,
                                                       dump_return_value_VkResult<ApiDumpFormat::Text>);
                dump_pre_function_formatting<ApiDumpFormat::Text>(ApiDumpInstance::current().settings());
                dump_params_vkCreateInstance<ApiDumpFormat::Text>(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);
                dump_post_function_formatting<ApiDumpFormat::Text>(ApiDumpInstance::current().settings());
                break;
            case ApiDumpFormat::Html:
                dump_return_value<ApiDumpFormat::Html>(ApiDumpInstance::current().settings(), "VkResult", result,
                                                       dump_return_value_VkResult<ApiDumpFormat::Html>);
                dump_pre_function_formatting<ApiDumpFormat::Html>(ApiDumpInstance::current().settings());
                dump_params_vkCreateInstance<ApiDumpFormat::Html>(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);
                dump_post_function_formatting<ApiDumpFormat::Html>(ApiDumpInstance::current().settings());
                break;
            case ApiDumpFormat::Json:
                dump_return_value<ApiDumpFormat::Json>(ApiDumpInstance::current().settings(), "VkResult", result,
                                                       dump_return_value_VkResult<ApiDumpFormat::Json>);
                dump_pre_function_formatting<ApiDumpFormat::Json>(ApiDumpInstance::current().settings());
                dump_params_vkCreateInstance<ApiDumpFormat::Json>(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);
                dump_post_function_formatting<ApiDumpFormat::Json>(ApiDumpInstance::current().settings());
                break;
        }
        flush(ApiDumpInstance::current().settings());
    }

    return result;
}

}

template <ApiDumpFormat Format>
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    dump_function_head(ApiDumpInstance::current(), "vkCreateDevice", "physicalDevice, pCreateInfo, pAllocator, pDevice",
                       "VkResult");
    if (ApiDumpInstance::current().settings().shouldPreDump() &&
        ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text && ApiDumpInstance::current().shouldDumpOutput()) {
        dump_before_pre_dump_formatting<ApiDumpFormat::Text>(ApiDumpInstance::current().settings());
        dump_params_vkCreateDevice<ApiDumpFormat::Text>(ApiDumpInstance::current(), physicalDevice, pCreateInfo, pAllocator,
                                                        pDevice);
    }

    // Get the function pointer
    VkLayerDeviceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance vk_instance = ApiDumpInstance::current().get_vk_instance(physicalDevice);
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice)fpGetInstanceProcAddr(vk_instance, "vkCreateDevice");
    if (fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if (result == VK_SUCCESS) {
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {
        dump_return_value<Format>(ApiDumpInstance::current().settings(), "VkResult", result, dump_return_value_VkResult<Format>);
        dump_pre_function_formatting<Format>(ApiDumpInstance::current().settings());
        dump_params_vkCreateDevice<Format>(ApiDumpInstance::current(), physicalDevice, pCreateInfo, pAllocator, pDevice);
        dump_post_function_formatting<Format>(ApiDumpInstance::current().settings());

        flush(ApiDumpInstance::current().settings());
    }
    return result;
}

extern "C" {

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName,
                                                                                      uint32_t* pPropertyCount,
                                                                                      VkExtensionProperties* pProperties) {
    return util_GetExtensionProperties(0, NULL, pPropertyCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount,
                                                                                  VkLayerProperties* pProperties) {
    static const VkLayerProperties layerProperties[] = {{
        "VK_LAYER_LUNARG_api_dump",
        VK_MAKE_VERSION(1, 4, VK_HEADER_VERSION),  // specVersion
        VK_MAKE_VERSION(0, 2, 0),                  // implementationVersion
        "layer: api_dump",
    }};

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice,
                                                                                uint32_t* pPropertyCount,
                                                                                VkLayerProperties* pProperties) {
    static const VkLayerProperties layerProperties[] = {{
        "VK_LAYER_LUNARG_api_dump",
        VK_MAKE_VERSION(1, 4, VK_HEADER_VERSION),
        VK_MAKE_VERSION(0, 2, 0),
        "layer: api_dump",
    }};

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}

}
