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

#include "api_dump.h"

#include "generated/api_dump_text.h"
#include "generated/api_dump_html.h"
#include "generated/api_dump_json.h"

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    ApiDumpInstance::current().initLayerSettings(pCreateInfo, pAllocator);
    if (ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text) {
        dump_function_head(ApiDumpInstance::current(), "vkCreateInstance", "pCreateInfo, pAllocator, pInstance");
        if (ApiDumpInstance::current().shouldDumpOutput()) {
            dump_text_params_vkCreateInstance(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);
        }
        dump_return_preamble(ApiDumpInstance::current(), "VkResult");
    } else {
        dump_function_head(ApiDumpInstance::current(), "vkCreateInstance", "pCreateInfo, pAllocator, pInstance", "VkResult");
    }

    // Get the function pointer
    VkLayerInstanceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr != 0);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if(fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if(result == VK_SUCCESS) {
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
    }

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {
        switch(ApiDumpInstance::current().settings().format())
        {
            case ApiDumpFormat::Text:
                dump_text_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
            case ApiDumpFormat::Html:
                dump_html_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
            case ApiDumpFormat::Json:
                dump_json_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
        }
    }
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    if (ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text) {
        dump_function_head(ApiDumpInstance::current(), "vkCreateDevice", "physicalDevice, pCreateInfo, pAllocator, pDevice");
        if (ApiDumpInstance::current().shouldDumpOutput()) {
            dump_text_params_vkCreateDevice(ApiDumpInstance::current(), physicalDevice, pCreateInfo, pAllocator, pDevice);
        }
        dump_return_preamble(ApiDumpInstance::current(), "VkResult");
    } else {
        dump_function_head(ApiDumpInstance::current(), "vkCreateDevice", "physicalDevice, pCreateInfo, pAllocator, pDevice", "VkResult");
    }

    // Get the function pointer
    VkLayerDeviceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance vk_instance = ApiDumpInstance::current().get_vk_instance(physicalDevice);
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(vk_instance, "vkCreateDevice");
    if(fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if(result == VK_SUCCESS) {
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {
        switch(ApiDumpInstance::current().settings().format())
        {
            case ApiDumpFormat::Text:
                dump_text_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
            case ApiDumpFormat::Html:
                dump_html_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
            case ApiDumpFormat::Json:
                dump_json_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
        }
    }
    return result;
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    return util_GetExtensionProperties(0, NULL, pPropertyCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    static const VkLayerProperties layerProperties[] = {
        {
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 4, VK_HEADER_VERSION), // specVersion
            VK_MAKE_VERSION(0, 2, 0), // implementationVersion
            "layer: api_dump",
        }
    };

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    static const VkLayerProperties layerProperties[] = {
        {
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 4, VK_HEADER_VERSION),
            VK_MAKE_VERSION(0, 2, 0),
            "layer: api_dump",
        }
    };

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}


EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{
    auto instance_func = api_dump_known_instance_functions(instance, pName);
    if (instance_func) return instance_func;

    // Make sure that device functions queried through GIPA works
    auto device_func = api_dump_known_device_functions(NULL, pName);
    if (device_func) return device_func;

    // Haven't created an instance yet, exit now since there is no instance_dispatch_table
    if(instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return nullptr;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName)
{
    auto device_func = api_dump_known_device_functions(device, pName);
    if (device_func) return device_func;

    // Haven't created a device yet, exit now since there is no device_dispatch_table
    if(device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return nullptr;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}
