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

#ifdef __cplusplus
extern "C" {
#endif

static device_table_map multi1_device_table_map;
static instance_table_map multi1_instance_table_map;
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
    if (!strcmp(pName, "multi1GetInstanceProcAddr") || !strcmp(pName, "vkGetInsatnceProcAddr"))
        return (PFN_vkVoidFunction)multi1GetInstanceProcAddr;
    if (!strcmp(pName, "multi1GetDeviceProcAddr") || !strcmp(pName, "vkGetDeviceProcAddr"))
        return (PFN_vkVoidFunction)multi1GetDeviceProcAddr;
    if (!strcmp("vkCreateInstance", pName))
        return (PFN_vkVoidFunction)multi1CreateInstance;
    if (!strcmp("vkCreateDevice", pName))
        return (PFN_vkVoidFunction)multi1CreateDevice;
    if (!strcmp("vkDestroyDevice", pName))
        return (PFN_vkVoidFunction)multi1DestroyDevice;
    if (!strcmp("vkCreateSampler", pName))
        return (PFN_vkVoidFunction)multi1CreateSampler;
    if (!strcmp("vkCreateGraphicsPipelines", pName))
        return (PFN_vkVoidFunction)multi1CreateGraphicsPipelines;
    if (!strcmp("vkDestroyInstance", pName))
        return (PFN_vkVoidFunction)multi1DestroyInstance;

    PFN_vkVoidFunction proc = multi1GetDeviceProcAddr(VK_NULL_HANDLE, pName);
    if (proc)
        return proc;

    if (instance == NULL)
        return NULL;

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

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL multi2GetInstanceProcAddr(VkInstance inst, const char *pName) {
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

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL VK_LAYER_LUNARG_multi1GetDeviceProcAddr(VkDevice device, const char *pName) {
    return multi1GetDeviceProcAddr(device, pName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL VK_LAYER_LUNARG_multi1GetInstanceProcAddr(VkInstance instance, const char *pName) {
    return multi1GetInstanceProcAddr(instance, pName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL VK_LAYER_LUNARG_multi2GetInstanceProcAddr(VkInstance instance, const char *pName) {
    return multi2GetInstanceProcAddr(instance, pName);
}

#ifdef __cplusplus
} // extern "C"
#endif
