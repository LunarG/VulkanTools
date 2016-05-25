/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * Copyright (C) 2015 Google Inc.
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
 * Author: David Pinedo <david@lunarg.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "vk_dispatch_table_helper.h"
#include "vulkan/vk_layer.h"
#include "vk_layer_table.h"
#include "vk_layer_extension_utils.h"

typedef VkResult(VKAPI_PTR *PFN_vkLayerBasicEXT)(VkDevice device);
static PFN_vkLayerBasicEXT pfn_layer_extension;

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkLayerBasicEXT(VkDevice device) {
    printf("In vkLayerBasicEXT() call w/ device: %p\n", (void *)device);
    if (pfn_layer_extension) {
        printf("In vkLayerBasicEXT() call down chain\n");
        return pfn_layer_extension(device);
    }
    printf("vkLayerBasicEXT returning SUCCESS\n");
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
basic_CreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance)fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if (fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result != VK_SUCCESS)
        return result;

    initInstanceTable(*pInstance, fpGetInstanceProcAddr);

    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
basic_EnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices) {
    printf("At start of wrapped vkEnumeratePhysicalDevices() call w/ inst: %p\n", (void *)instance);
    VkResult result = instance_dispatch_table(instance)->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    printf("Completed wrapped vkEnumeratePhysicalDevices() call w/ count %u\n", *pPhysicalDeviceCount);
    return result;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL basic_CreateDevice(VkPhysicalDevice physicalDevice,
                                                                  const VkDeviceCreateInfo *pCreateInfo,
                                                                  const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    printf("VK_LAYER_LUNARG_Basic: At start of vkCreateDevice() call w/ gpu: %p\n", (void *)physicalDevice);

    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice)fpGetInstanceProcAddr(NULL, "vkCreateDevice");
    if (fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if (result != VK_SUCCESS) {
        return result;
    }

    initDeviceTable(*pDevice, fpGetDeviceProcAddr);

    pfn_layer_extension = (PFN_vkLayerBasicEXT)fpGetDeviceProcAddr(*pDevice, "vkLayerBasicEXT");
    printf("VK_LAYER_LUNARG_Basic: Completed vkCreateDevice() call w/ pDevice, Device %p: %p\n", (void *)pDevice, (void *)*pDevice);
    return result;
}

/* hook DestroyDevice to remove tableMap entry */
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL basic_DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(device);
    device_dispatch_table(device)->DestroyDevice(device, pAllocator);
    destroy_device_dispatch_table(key);
}

/* hook DestroyInstance to remove tableInstanceMap entry */
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL basic_DestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(instance);
    instance_dispatch_table(instance)->DestroyInstance(instance, pAllocator);
    destroy_instance_dispatch_table(key);
}

VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL
basic_GetPhysicalDeviceFormatProperties(VkPhysicalDevice gpu, VkFormat format, VkFormatProperties *pFormatInfo) {
    printf("At start of wrapped vkGetPhysicalDeviceFormatProperties() call w/ gpu: %p\n", (void *)gpu);
    instance_dispatch_table(gpu)->GetPhysicalDeviceFormatProperties(gpu, format, pFormatInfo);
    printf("Completed wrapped vkGetPhysicalDeviceFormatProperties() call w/ gpu: %p\n", (void *)gpu);
}

static const VkLayerProperties globalLayerProps[] = {{
    "VK_LAYER_LUNARG_basic",
    VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
    1,              // implementationVersion
    "LunarG Sample Layer",
}};

static const VkLayerProperties basic_physicaldevice_layers[] = {{
    "VK_LAYER_LUNARG_basic", VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), 1, "Sample layer: basic, implements vkLayerBasicEXT",
}};

static const VkExtensionProperties basic_physicaldevice_extensions[] = {{
    "vkLayerBasicEXT", 1,
}};

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties) {
    return util_GetLayerProperties(ARRAY_SIZE(globalLayerProps), globalLayerProps, pCount, pProperties);
}

/* Must use Vulkan name so that loader finds it */
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties *pProperties) {
    /* Mem tracker's physical device layers are the same as global */
    return util_GetLayerProperties(ARRAY_SIZE(basic_physicaldevice_layers), basic_physicaldevice_layers, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount, VkExtensionProperties *pProperties) {
    return util_GetExtensionProperties(0, NULL, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                                                                    const char *pLayerName, uint32_t *pCount,
                                                                                    VkExtensionProperties *pProperties) {
    if (pLayerName == NULL) {
        return instance_dispatch_table(physicalDevice)
            ->EnumerateDeviceExtensionProperties(physicalDevice, NULL, pCount, pProperties);
    } else {
        return util_GetExtensionProperties(ARRAY_SIZE(basic_physicaldevice_extensions), basic_physicaldevice_extensions, pCount,
                                           pProperties);
    }
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char *pName) {
    if (!strcmp("vkGetDeviceProcAddr", pName))
        return (PFN_vkVoidFunction)vkGetDeviceProcAddr;
    if (!strcmp("vkDestroyDevice", pName))
        return (PFN_vkVoidFunction)basic_DestroyDevice;
    if (!strcmp("vkLayerBasicEXT", pName))
        return (PFN_vkVoidFunction)vkLayerBasicEXT;

    if (device == NULL)
        return NULL;

    if (device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return NULL;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (!strcmp("vkEnumerateInstanceLayerProperties", pName))
        return (PFN_vkVoidFunction)vkEnumerateInstanceLayerProperties;
    if (!strcmp("vkEnumerateDeviceLayerProperties", pName))
        return (PFN_vkVoidFunction)vkEnumerateDeviceLayerProperties;
    if (!strcmp("vkEnumerateInstanceExtensionProperties", pName))
        return (PFN_vkVoidFunction)vkEnumerateInstanceExtensionProperties;
    if (!strcmp("vkEnumerateDeviceExtensionProperties", pName))
        return (PFN_vkVoidFunction)vkEnumerateDeviceExtensionProperties;
    if (!strcmp("vkGetInstanceProcAddr", pName))
        return (PFN_vkVoidFunction)vkGetInstanceProcAddr;
    if (!strcmp("vkGetPhysicalDeviceFormatProperties", pName))
        return (PFN_vkVoidFunction)basic_GetPhysicalDeviceFormatProperties;
    if (!strcmp("vkCreateInstance", pName))
        return (PFN_vkVoidFunction)basic_CreateInstance;
    if (!strcmp("vkDestroyInstance", pName))
        return (PFN_vkVoidFunction)basic_DestroyInstance;
    if (!strcmp("vkCreateDevice", pName))
        return (PFN_vkVoidFunction)basic_CreateDevice;
    if (!strcmp("vkEnumeratePhysicalDevices", pName))
        return (PFN_vkVoidFunction)basic_EnumeratePhysicalDevices;

    if (instance == NULL)
        return NULL;

    if (instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return NULL;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}
