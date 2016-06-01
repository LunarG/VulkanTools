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
#include <unordered_map>
#include "vk_dispatch_table_helper.h"
#include "vulkan/vk_layer.h"
#include "vk_layer_table.h"

static std::unordered_map<dispatch_key, VkInstance> basic_instance_map;

typedef VkResult(VKAPI_PTR *PFN_vkLayerBasicEXT)(VkDevice device);
static PFN_vkLayerBasicEXT pfn_layer_extension;

VKAPI_ATTR VkResult VKAPI_CALL basic_LayerBasicEXT(VkDevice device) {
    printf("In vkLayerBasicEXT() call w/ device: %p\n", (void *)device);
    if (pfn_layer_extension) {
        printf("In vkLayerBasicEXT() call down chain\n");
        return pfn_layer_extension(device);
    }
    printf("vkLayerBasicEXT returning SUCCESS\n");
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL
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

    basic_instance_map[get_dispatch_key(*pInstance)] = *pInstance;
    initInstanceTable(*pInstance, fpGetInstanceProcAddr);

    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL
basic_EnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices) {
    printf("At start of wrapped vkEnumeratePhysicalDevices() call w/ inst: %p\n", (void *)instance);
    VkResult result = instance_dispatch_table(instance)->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    printf("Completed wrapped vkEnumeratePhysicalDevices() call w/ count %u\n", *pPhysicalDeviceCount);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL basic_CreateDevice(VkPhysicalDevice physicalDevice,
                                                  const VkDeviceCreateInfo *pCreateInfo,
                                                  const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    printf("VK_LAYER_LUNARG_Basic: At start of vkCreateDevice() call w/ gpu: %p\n", (void *)physicalDevice);

    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance instance = basic_instance_map[get_dispatch_key(physicalDevice)];
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice)fpGetInstanceProcAddr(instance, "vkCreateDevice");
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
VKAPI_ATTR void VKAPI_CALL basic_DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(device);
    device_dispatch_table(device)->DestroyDevice(device, pAllocator);
    destroy_device_dispatch_table(key);
}

/* hook DestroyInstance to remove tableInstanceMap entry */
VKAPI_ATTR void VKAPI_CALL basic_DestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(instance);
    instance_dispatch_table(instance)->DestroyInstance(instance, pAllocator);
    destroy_instance_dispatch_table(key);
    basic_instance_map.erase(key);
}

VKAPI_ATTR void VKAPI_CALL
basic_GetPhysicalDeviceFormatProperties(VkPhysicalDevice gpu, VkFormat format, VkFormatProperties *pFormatInfo) {
    printf("At start of wrapped vkGetPhysicalDeviceFormatProperties() call w/ gpu: %p\n", (void *)gpu);
    instance_dispatch_table(gpu)->GetPhysicalDeviceFormatProperties(gpu, format, pFormatInfo);
    printf("Completed wrapped vkGetPhysicalDeviceFormatProperties() call w/ gpu: %p\n", (void *)gpu);
}

static const VkLayerProperties basic_LayerProps = {
    "VK_LAYER_LUNARG_basic",
    VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
    1,              // implementationVersion
    "LunarG Sample Layer",
};

static const VkExtensionProperties basic_physicaldevice_extensions[] = {{
    "vkLayerBasicEXT", 1,
}};

template<typename T>
VkResult EnumerateProperties(uint32_t src_count, const T *src_props, uint32_t *dst_count, T *dst_props) {
    if (!dst_props || !src_props) {
        *dst_count = src_count;
        return VK_SUCCESS;
    }

    uint32_t copy_count = (*dst_count < src_count) ? *dst_count : src_count;
    memcpy(dst_props, src_props, sizeof(T) * copy_count);
    *dst_count = copy_count;

    return (copy_count == src_count) ? VK_SUCCESS : VK_INCOMPLETE;
}

VKAPI_ATTR VkResult VKAPI_CALL
basic_EnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties) {
    return EnumerateProperties(1, &basic_LayerProps, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL
basic_EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties *pProperties) {
    return EnumerateProperties(1, &basic_LayerProps, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL
basic_EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount, VkExtensionProperties *pProperties) {
    if (pLayerName && !strcmp(pLayerName, basic_LayerProps.layerName))
        return EnumerateProperties<VkExtensionProperties>(0, NULL, pCount, pProperties);

    return VK_ERROR_LAYER_NOT_PRESENT;
}

VKAPI_ATTR VkResult VKAPI_CALL basic_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                                                        const char *pLayerName, uint32_t *pCount,
                                                                        VkExtensionProperties *pProperties) {
    if (pLayerName && !strcmp(pLayerName, basic_LayerProps.layerName)) {
        uint32_t count = sizeof(basic_physicaldevice_extensions) /
            sizeof(basic_physicaldevice_extensions[0]);
        return EnumerateProperties(count, basic_physicaldevice_extensions, pCount, pProperties);
    }

    return instance_dispatch_table(physicalDevice)
        ->EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pCount, pProperties);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL basic_GetDeviceProcAddr(VkDevice device, const char *pName) {
    if (!strcmp("vkGetDeviceProcAddr", pName))
        return (PFN_vkVoidFunction)basic_GetDeviceProcAddr;
    if (!strcmp("vkDestroyDevice", pName))
        return (PFN_vkVoidFunction)basic_DestroyDevice;
    if (!strcmp("vkLayerBasicEXT", pName))
        return (PFN_vkVoidFunction)basic_LayerBasicEXT;

    if (device == NULL)
        return NULL;

    if (device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return NULL;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL basic_GetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (!strcmp("vkEnumerateInstanceLayerProperties", pName))
        return (PFN_vkVoidFunction)basic_EnumerateInstanceLayerProperties;
    if (!strcmp("vkEnumerateDeviceLayerProperties", pName))
        return (PFN_vkVoidFunction)basic_EnumerateDeviceLayerProperties;
    if (!strcmp("vkEnumerateInstanceExtensionProperties", pName))
        return (PFN_vkVoidFunction)basic_EnumerateInstanceExtensionProperties;
    if (!strcmp("vkEnumerateDeviceExtensionProperties", pName))
        return (PFN_vkVoidFunction)basic_EnumerateDeviceExtensionProperties;
    if (!strcmp("vkGetInstanceProcAddr", pName))
        return (PFN_vkVoidFunction)basic_GetInstanceProcAddr;
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

    assert(instance);

    PFN_vkVoidFunction proc = basic_GetDeviceProcAddr(VK_NULL_HANDLE, pName);
    if (proc)
        return proc;

    if (instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return NULL;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}

// loader-layer interface v0, just wrappers since there is only a layer

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties) {
    return basic_EnumerateInstanceLayerProperties(pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties *pProperties) {
    // the layer command handles VK_NULL_HANDLE just fine internally
    assert(physicalDevice == VK_NULL_HANDLE);
    return basic_EnumerateDeviceLayerProperties(VK_NULL_HANDLE, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount, VkExtensionProperties *pProperties) {
    return basic_EnumerateInstanceExtensionProperties(pLayerName, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                                                                    const char *pLayerName, uint32_t *pCount,
                                                                                    VkExtensionProperties *pProperties) {
    // the layer command handles VK_NULL_HANDLE just fine internally
    assert(physicalDevice == VK_NULL_HANDLE);
    return basic_EnumerateDeviceExtensionProperties(VK_NULL_HANDLE, pLayerName, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice dev, const char *funcName) {
    return basic_GetDeviceProcAddr(dev, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char *funcName) {
    return basic_GetInstanceProcAddr(instance, funcName);
}
