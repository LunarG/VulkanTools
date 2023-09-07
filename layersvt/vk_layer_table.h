/* Copyright (c) 2015-2021 The Khronos Group Inc.
 * Copyright (c) 2015-2021 Valve Corporation
 * Copyright (c) 2015-2021 LunarG, Inc.
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
 * Author: Tobin Ehlis <tobin@lunarg.com>
 */

#pragma once

#include <vulkan/utility/vk_dispatch_table.h>
#include "vulkan/vk_layer.h"
#include "vulkan/vulkan.h"
#include <unordered_map>
#include <cstring>

typedef std::unordered_map<void *, VulDeviceDispatchTable *> device_table_map;
typedef std::unordered_map<void *, VulInstanceDispatchTable *> instance_table_map;
VulDeviceDispatchTable *initDeviceTable(VkDevice device, const PFN_vkGetDeviceProcAddr gpa, device_table_map &map);
VulDeviceDispatchTable *initDeviceTable(VkDevice device, const PFN_vkGetDeviceProcAddr gpa);
VulInstanceDispatchTable *initInstanceTable(VkInstance instance, const PFN_vkGetInstanceProcAddr gpa, instance_table_map &map);
VulInstanceDispatchTable *initInstanceTable(VkInstance instance, const PFN_vkGetInstanceProcAddr gpa);

typedef void *dispatch_key;

dispatch_key get_dispatch_key(const void *object);

VulDeviceDispatchTable *device_dispatch_table(void *object);

VulInstanceDispatchTable *instance_dispatch_table(void *object);

VulDeviceDispatchTable *get_dispatch_table(device_table_map &map, void *object);

VulInstanceDispatchTable *get_dispatch_table(instance_table_map &map, void *object);

VkLayerInstanceCreateInfo *get_chain_info(const VkInstanceCreateInfo *pCreateInfo, VkLayerFunction func);
VkLayerDeviceCreateInfo *get_chain_info(const VkDeviceCreateInfo *pCreateInfo, VkLayerFunction func);

void destroy_device_dispatch_table(dispatch_key key);
void destroy_instance_dispatch_table(dispatch_key key);
void destroy_dispatch_table(device_table_map &map, dispatch_key key);
void destroy_dispatch_table(instance_table_map &map, dispatch_key key);

// For the given data key, look up the layer_data instance from given layer_data_map
template <typename DATA_T>
DATA_T *GetLayerDataPtr(void *data_key, std::unordered_map<void *, DATA_T *> &layer_data_map) {
    DATA_T *debug_data;
    /* TODO: We probably should lock here, or have caller lock */
    auto got = layer_data_map.find(data_key);

    if (got == layer_data_map.end()) {
        debug_data = new DATA_T;
        layer_data_map[(void *)data_key] = debug_data;
    } else {
        debug_data = got->second;
    }

    return debug_data;
}

inline VkResult util_GetExtensionProperties(const uint32_t count, const VkExtensionProperties *layer_extensions, uint32_t *pCount,
                                            VkExtensionProperties *pProperties) {
    if (pProperties == nullptr || layer_extensions == nullptr) {
        *pCount = count;
        return VK_SUCCESS;
    }

    const uint32_t copy_size = *pCount < count ? *pCount : count;
    std::memcpy(pProperties, layer_extensions, copy_size * sizeof(VkExtensionProperties));
    *pCount = copy_size;
    if (copy_size < count) {
        return VK_INCOMPLETE;
    }

    return VK_SUCCESS;
}

inline VkResult util_GetLayerProperties(const uint32_t count, const VkLayerProperties *layer_properties, uint32_t *pCount,
                                        VkLayerProperties *pProperties) {
    if (pProperties == nullptr || layer_properties == nullptr) {
        *pCount = count;
        return VK_SUCCESS;
    }

    const uint32_t copy_size = *pCount < count ? *pCount : count;
    std::memcpy(pProperties, layer_properties, copy_size * sizeof(VkLayerProperties));
    *pCount = copy_size;
    if (copy_size < count) {
        return VK_INCOMPLETE;
    }

    return VK_SUCCESS;
}
