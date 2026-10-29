/*
 * Copyright (c) 2026 LunarG, Inc.
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
 */

#pragma once

#include <cstdint>
#include <string_view>

#include <vulkan/vulkan.h>

// The layer's Vulkan functions. The layer library only exports the entry points below; everything else is reached
// through them, and lives here so it can be unit tested.
namespace monitor {

inline constexpr std::string_view kLayerName = "VK_LAYER_LUNARG_monitor";

// The layer's own function for `name`, or null if the layer doesn't hook it.
[[nodiscard]] PFN_vkVoidFunction FindInstanceHook(std::string_view name);

// The same for device functions. Functions from extensions or newer versions are only hooked when `next_supported`,
// meaning the next layer or the driver has the function for this device.
[[nodiscard]] PFN_vkVoidFunction FindDeviceHook(std::string_view name, bool next_supported);

// vkGetPhysicalDeviceToolPropertiesEXT for a layer that adds `tool` in front of the tools of `next`, which may be null.
[[nodiscard]] VkResult PrependToolProperties(const VkPhysicalDeviceToolPropertiesEXT &tool, VkPhysicalDevice physical_device,
                                             PFN_vkGetPhysicalDeviceToolPropertiesEXT next, uint32_t *pToolCount,
                                             VkPhysicalDeviceToolPropertiesEXT *pToolProperties);

// The exported entry points
[[nodiscard]] VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(VkInstance instance, const char *pName);
[[nodiscard]] VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(VkDevice device, const char *pName);
[[nodiscard]] VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceLayerProperties(uint32_t *pPropertyCount,
                                                                              VkLayerProperties *pProperties);
[[nodiscard]] VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice,
                                                                            uint32_t *pPropertyCount,
                                                                            VkLayerProperties *pProperties);
[[nodiscard]] VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount,
                                                                                  VkExtensionProperties *pProperties);
[[nodiscard]] VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                                                                const char *pLayerName, uint32_t *pPropertyCount,
                                                                                VkExtensionProperties *pProperties);

}  // namespace monitor
