/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unordered_map>
#include "vk_loader_platform.h"
#include "vulkan/vk_layer.h"
#include "vk_layer_table.h"

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

#ifdef __cplusplus
extern "C" {
#endif

enum {
    LAYER_MULTI1,
    LAYER_MULTI2,
    LAYER_COUNT,
};

static const VkLayerProperties all_layer_props[LAYER_COUNT] = {
    {
        "VK_LAYER_LUNARG_multi1",
        VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
        1,              // implementationVersion
        "LunarG Sample multiple layer per library",
    },
    {
        "VK_LAYER_LUNARG_multi2",
        VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
        1,              // implementationVersion
        "LunarG Sample multiple layer per library",
    },
};

static const struct {
    uint32_t count;
    const VkExtensionProperties* extensions;
} all_extension_props[LAYER_COUNT] = {
    { 0, NULL, },
    { 0, NULL, },
};

static device_table_map multi1_device_table_map;
static instance_table_map multi1_instance_table_map;
static std::unordered_map<dispatch_key, VkInstance> multi1_instance_map;
/******************************** Layer multi1 functions **************************/
VKAPI_ATTR VkResult VKAPI_CALL
multi1CreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
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

    multi1_instance_map[get_dispatch_key(*pInstance)] = *pInstance;
    initInstanceTable(*pInstance, fpGetInstanceProcAddr, multi1_instance_table_map);

    return result;
}

/* hook DestroyInstance to remove tableInstanceMap entry */
VKAPI_ATTR void VKAPI_CALL multi1DestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi1_instance_table_map, instance);
    dispatch_key key = get_dispatch_key(instance);

    printf("At start of wrapped multi1 vkDestroyInstance()\n");
    pDisp->DestroyInstance(instance, pAllocator);
    multi1_instance_table_map.erase(key);
    multi1_instance_map.erase(key);
    printf("Completed multi1 layer vkDestroyInstance()\n");
}

VKAPI_ATTR VkResult VKAPI_CALL multi1CreateDevice(VkPhysicalDevice physicalDevice,
                                                                  const VkDeviceCreateInfo *pCreateInfo,
                                                                  const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    printf("At start of multi1 layer vkCreateDevice()\n");

    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance instance = multi1_instance_map[get_dispatch_key(physicalDevice)];
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

    initDeviceTable(*pDevice, fpGetDeviceProcAddr, multi1_device_table_map);

    printf("Completed multi1 layer vkCreateDevice() call");
    return result;
}

/* hook DestroyDevice to remove tableMap entry */
VKAPI_ATTR void VKAPI_CALL multi1DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    VkLayerDispatchTable *pDisp = get_dispatch_table(multi1_device_table_map, device);
    dispatch_key key = get_dispatch_key(device);

    printf("At start of multi1 layer vkDestroyDevice()\n");
    pDisp->DestroyDevice(device, pAllocator);
    multi1_device_table_map.erase(key);
    printf("Completed multi1 layer vkDestroyDevice()\n");
}

VKAPI_ATTR VkResult VKAPI_CALL multi1CreateSampler(VkDevice device, const VkSamplerCreateInfo *pCreateInfo,
                                                                   const VkAllocationCallbacks *pAllocator, VkSampler *pSampler) {
    VkLayerDispatchTable *pDisp = get_dispatch_table(multi1_device_table_map, device);

    printf("At start of multi1 layer vkCreateSampler()\n");
    VkResult result = pDisp->CreateSampler(device, pCreateInfo, pAllocator, pSampler);
    printf("Completed multi1 layer vkCreateSampler()\n");
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL
multi1CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t count,
                              const VkGraphicsPipelineCreateInfo *pCreateInfos, const VkAllocationCallbacks *pAllocator,
                              VkPipeline *pPipelines) {
    VkLayerDispatchTable *pDisp = get_dispatch_table(multi1_device_table_map, device);

    printf("At start of multi1 layer vkCreateGraphicsPipeline()\n");
    VkResult result = pDisp->CreateGraphicsPipelines(device, pipelineCache, count, pCreateInfos, pAllocator, pPipelines);
    printf("Completed multi1 layer vkCreateGraphicsPipeline()\n");
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL
multi1EnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties)
{
    return EnumerateProperties(1, &all_layer_props[LAYER_MULTI1], pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL
multi1EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties *pProperties)
{
    return EnumerateProperties(1, &all_layer_props[LAYER_MULTI1], pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL
multi1EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount, VkExtensionProperties *pProperties)
{
    if (pLayerName && !strcmp(pLayerName, all_layer_props[LAYER_MULTI1].layerName)) {
        return EnumerateProperties(all_extension_props[LAYER_MULTI1].count,
                all_extension_props[LAYER_MULTI1].extensions, pCount, pProperties);
    }

    return VK_ERROR_LAYER_NOT_PRESENT;
}

VKAPI_ATTR VkResult VKAPI_CALL
multi1EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                         const char *pLayerName, uint32_t *pCount,
                                         VkExtensionProperties *pProperties)
{
    if (pLayerName && !strcmp(pLayerName, all_layer_props[LAYER_MULTI1].layerName)) {
        return EnumerateProperties(all_extension_props[LAYER_MULTI1].count,
                all_extension_props[LAYER_MULTI1].extensions, pCount, pProperties);
    }

    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi1_instance_table_map, physicalDevice);
    return pDisp->EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pCount, pProperties);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL multi1GetDeviceProcAddr(VkDevice device, const char *pName) {
    if (!strcmp(pName, "multi1GetDeviceProcAddr") || !strcmp(pName, "vkGetDeviceProcAddr"))
        return (PFN_vkVoidFunction)multi1GetDeviceProcAddr;
    if (!strcmp("vkDestroyDevice", pName))
        return (PFN_vkVoidFunction)multi1DestroyDevice;
    if (!strcmp("vkCreateSampler", pName))
        return (PFN_vkVoidFunction)multi1CreateSampler;
    if (!strcmp("vkCreateGraphicsPipelines", pName))
        return (PFN_vkVoidFunction)multi1CreateGraphicsPipelines;

    if (device == NULL)
        return NULL;

    VkLayerDispatchTable *pTable = get_dispatch_table(multi1_device_table_map, device);
    if (pTable->GetDeviceProcAddr == NULL)
        return NULL;
    return pTable->GetDeviceProcAddr(device, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL multi1GetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (!strcmp("vkEnumerateInstanceLayerProperties", pName))
        return (PFN_vkVoidFunction)multi1EnumerateInstanceLayerProperties;
    if (!strcmp("vkEnumerateDeviceLayerProperties", pName))
        return (PFN_vkVoidFunction)multi1EnumerateDeviceLayerProperties;
    if (!strcmp("vkEnumerateInstanceExtensionProperties", pName))
        return (PFN_vkVoidFunction)multi1EnumerateInstanceExtensionProperties;
    if (!strcmp("vkEnumerateDeviceExtensionProperties", pName))
        return (PFN_vkVoidFunction)multi1EnumerateDeviceExtensionProperties;
    if (!strcmp(pName, "multi1GetInstanceProcAddr") || !strcmp(pName, "vkGetInsatnceProcAddr"))
        return (PFN_vkVoidFunction)multi1GetInstanceProcAddr;
    if (!strcmp("vkCreateInstance", pName))
        return (PFN_vkVoidFunction)multi1CreateInstance;
    if (!strcmp("vkCreateDevice", pName))
        return (PFN_vkVoidFunction)multi1CreateDevice;
    if (!strcmp("vkDestroyInstance", pName))
        return (PFN_vkVoidFunction)multi1DestroyInstance;

    assert(instance);

    PFN_vkVoidFunction proc = multi1GetDeviceProcAddr(VK_NULL_HANDLE, pName);
    if (proc)
        return proc;

    VkLayerInstanceDispatchTable *pTable = get_dispatch_table(multi1_instance_table_map, instance);
    if (pTable->GetInstanceProcAddr == NULL)
        return NULL;
    return pTable->GetInstanceProcAddr(instance, pName);

}

static instance_table_map multi2_instance_table_map;
/******************************** Layer multi2 functions **************************/
VKAPI_ATTR VkResult VKAPI_CALL
multi2CreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
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

    initInstanceTable(*pInstance, fpGetInstanceProcAddr, multi2_instance_table_map);

    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL
multi2EnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices) {
    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi2_instance_table_map, instance);

    printf("At start of wrapped multi2 vkEnumeratePhysicalDevices()\n");
    VkResult result = pDisp->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    printf("Completed multi2 layer vkEnumeratePhysicalDevices()\n");
    return result;
}

VKAPI_ATTR void VKAPI_CALL
multi2GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties *pProperties) {
    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi2_instance_table_map, physicalDevice);
    printf("At start of wrapped multi2 vkGetPhysicalDeviceProperties()\n");
    pDisp->GetPhysicalDeviceProperties(physicalDevice, pProperties);
    printf("Completed multi2 layer vkGetPhysicalDeviceProperties()\n");
}

VKAPI_ATTR void VKAPI_CALL
multi2GetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures *pFeatures) {
    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi2_instance_table_map, physicalDevice);
    printf("At start of wrapped multi2 vkGetPhysicalDeviceFeatures()\n");
    pDisp->GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
    printf("Completed multi2 layer vkGetPhysicalDeviceFeatures()\n");
}

/* hook DestroyInstance to remove tableInstanceMap entry */
VKAPI_ATTR void VKAPI_CALL multi2DestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi2_instance_table_map, instance);
    dispatch_key key = get_dispatch_key(instance);

    printf("At start of wrapped multi2 vkDestroyInstance()\n");
    pDisp->DestroyInstance(instance, pAllocator);
    multi2_instance_table_map.erase(key);
    printf("Completed multi2 layer vkDestroyInstance()\n");
}

VKAPI_ATTR VkResult VKAPI_CALL
multi2EnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties)
{
    return EnumerateProperties(1, &all_layer_props[LAYER_MULTI2], pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL
multi2EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties *pProperties)
{
    return EnumerateProperties(1, &all_layer_props[LAYER_MULTI2], pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL
multi2EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount, VkExtensionProperties *pProperties)
{
    if (pLayerName && !strcmp(pLayerName, all_layer_props[LAYER_MULTI2].layerName)) {
        return EnumerateProperties(all_extension_props[LAYER_MULTI2].count,
                all_extension_props[LAYER_MULTI2].extensions, pCount, pProperties);
    }

    return VK_ERROR_LAYER_NOT_PRESENT;
}

VKAPI_ATTR VkResult VKAPI_CALL
multi2EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                         const char *pLayerName, uint32_t *pCount,
                                         VkExtensionProperties *pProperties)
{
    if (pLayerName && !strcmp(pLayerName, all_layer_props[LAYER_MULTI2].layerName)) {
        return EnumerateProperties(all_extension_props[LAYER_MULTI2].count,
                all_extension_props[LAYER_MULTI2].extensions, pCount, pProperties);
    }

    VkLayerInstanceDispatchTable *pDisp = get_dispatch_table(multi2_instance_table_map, physicalDevice);
    return pDisp->EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pCount, pProperties);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL multi2GetInstanceProcAddr(VkInstance inst, const char *pName) {
    if (!strcmp("vkEnumerateInstanceLayerProperties", pName))
        return (PFN_vkVoidFunction)multi2EnumerateInstanceLayerProperties;
    if (!strcmp("vkEnumerateDeviceLayerProperties", pName))
        return (PFN_vkVoidFunction)multi2EnumerateDeviceLayerProperties;
    if (!strcmp("vkEnumerateInstanceExtensionProperties", pName))
        return (PFN_vkVoidFunction)multi2EnumerateInstanceExtensionProperties;
    if (!strcmp("vkEnumerateDeviceExtensionProperties", pName))
        return (PFN_vkVoidFunction)multi2EnumerateDeviceExtensionProperties;
    if (!strcmp("vkCreateInstance", pName))
        return (PFN_vkVoidFunction)multi2CreateInstance;
    if (!strcmp(pName, "multi2GetInstanceProcAddr") || !strcmp(pName, "vkGetInstanceProcAddr"))
        return (PFN_vkVoidFunction)multi2GetInstanceProcAddr;
    if (!strcmp("vkEnumeratePhysicalDevices", pName))
        return (PFN_vkVoidFunction)multi2EnumeratePhysicalDevices;
    if (!strcmp("GetPhysicalDeviceProperties", pName))
        return (PFN_vkVoidFunction)multi2GetPhysicalDeviceProperties;
    if (!strcmp("GetPhysicalDeviceFeatures", pName))
        return (PFN_vkVoidFunction)multi2GetPhysicalDeviceFeatures;
    if (!strcmp("vkDestroyInstance", pName))
        return (PFN_vkVoidFunction)multi2DestroyInstance;

    if (inst == NULL)
        return NULL;

    VkLayerInstanceDispatchTable *pTable = get_dispatch_table(multi2_instance_table_map, inst);
    if (pTable->GetInstanceProcAddr == NULL)
        return NULL;
    return pTable->GetInstanceProcAddr(inst, pName);
}

// loader-layer interface v0

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties)
{
    return EnumerateProperties(LAYER_COUNT, all_layer_props, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties *pProperties)
{
    // multi2 does not intercept any device-level command
    return multi1EnumerateDeviceLayerProperties(physicalDevice, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount, VkExtensionProperties *pProperties)
{
    assert(pLayerName);

    for (int i = 0; i < LAYER_COUNT; i++) {
        if (!strcmp(pLayerName, all_layer_props[i].layerName)) {
            return EnumerateProperties(all_extension_props[i].count,
                    all_extension_props[i].extensions, pCount, pProperties);
        }
    }

    assert(!"unreachable");
    *pCount = 0;
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL
vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                     const char *pLayerName, uint32_t *pCount,
                                     VkExtensionProperties *pProperties)
{
    assert(physicalDevice == VK_NULL_HANDLE && pLayerName);

    for (int i = 0; i < LAYER_COUNT; i++) {
        if (!strcmp(pLayerName, all_layer_props[i].layerName)) {
            return EnumerateProperties(all_extension_props[i].count,
                    all_extension_props[i].extensions, pCount, pProperties);
        }
    }

    assert(!"unreachable");
    *pCount = 0;
    return VK_SUCCESS;
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL VK_LAYER_LUNARG_multi1GetDeviceProcAddr(VkDevice device, const char *pName) {
    return multi1GetDeviceProcAddr(device, pName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL VK_LAYER_LUNARG_multi1GetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (!strcmp(pName, "vkEnumerateInstanceLayerProperties"))
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceLayerProperties);
    if (!strcmp(pName, "vkEnumerateDeviceLayerProperties"))
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateDeviceLayerProperties);
    if (!strcmp(pName, "vkEnumerateInstanceExtensionProperties"))
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceExtensionProperties);
    if (!strcmp(pName, "vkGetInstanceProcAddr"))
        return reinterpret_cast<PFN_vkVoidFunction>(VK_LAYER_LUNARG_multi1GetInstanceProcAddr);

    return multi1GetInstanceProcAddr(instance, pName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL VK_LAYER_LUNARG_multi2GetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (!strcmp(pName, "vkEnumerateInstanceLayerProperties"))
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceLayerProperties);
    if (!strcmp(pName, "vkEnumerateDeviceLayerProperties"))
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateDeviceLayerProperties);
    if (!strcmp(pName, "vkEnumerateInstanceExtensionProperties"))
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceExtensionProperties);
    if (!strcmp(pName, "vkGetInstanceProcAddr"))
        return reinterpret_cast<PFN_vkVoidFunction>(VK_LAYER_LUNARG_multi2GetInstanceProcAddr);

    return multi2GetInstanceProcAddr(instance, pName);
}

#ifdef __cplusplus
} // extern "C"
#endif
