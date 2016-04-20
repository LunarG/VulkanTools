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
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */

#ifndef GENERIC_H
#define GENERIC_H
#include "vulkan/vk_layer.h"

/*
 * This file contains static functions for the generated layer generic
 */

// The following is for logging error messages:
struct layer_data {
    debug_report_data *report_data;
    std::vector<VkDebugReportCallbackEXT> logging_callback;

    layer_data() : report_data(nullptr), logging_callback(VK_NULL_HANDLE){};
};

static const VkLayerProperties globalLayerProps[] = {{
    "VK_LAYER_LUNARG_generic",
    VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
    1, "layer: generic",
}};

static const VkLayerProperties deviceLayerProps[] = {{
    "VK_LAYER_LUNARG_generic",
    VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
    1, "layer: generic",
}};

struct devExts {
    bool wsi_enabled;
};
struct instExts {
    bool wsi_enabled;
};
static std::unordered_map<void *, struct devExts> deviceExtMap;
static std::unordered_map<void *, struct instExts> instanceExtMap;

static void createDeviceRegisterExtensions(const VkDeviceCreateInfo *pCreateInfo, VkDevice device) {
    uint32_t i;
    VkLayerDispatchTable *pDisp = device_dispatch_table(device);
    PFN_vkGetDeviceProcAddr gpa = pDisp->GetDeviceProcAddr;
    pDisp->CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)gpa(device, "vkCreateSwapchainKHR");
    pDisp->DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)gpa(device, "vkDestroySwapchainKHR");
    pDisp->GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)gpa(device, "vkGetSwapchainImagesKHR");
    pDisp->AcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)gpa(device, "vkAcquireNextImageKHR");
    pDisp->QueuePresentKHR = (PFN_vkQueuePresentKHR)gpa(device, "vkQueuePresentKHR");

    deviceExtMap[pDisp].wsi_enabled = false;
    for (i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
        if (strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
            deviceExtMap[pDisp].wsi_enabled = true;
    }
}

static void createInstanceRegisterExtensions(const VkInstanceCreateInfo *pCreateInfo, VkInstance instance) {
    uint32_t i;
    VkLayerInstanceDispatchTable *pDisp = instance_dispatch_table(instance);
    PFN_vkGetInstanceProcAddr gpa = pDisp->GetInstanceProcAddr;

    pDisp->DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)gpa(instance, "vkDestroySurfaceKHR");
    pDisp->GetPhysicalDeviceSurfaceSupportKHR =
        (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)gpa(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    pDisp->GetPhysicalDeviceSurfaceCapabilitiesKHR =
        (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)gpa(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    pDisp->GetPhysicalDeviceSurfaceFormatsKHR =
        (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)gpa(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    pDisp->GetPhysicalDeviceSurfacePresentModesKHR =
        (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)gpa(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
    instanceExtMap[pDisp].wsi_enabled = false;
    for (i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
        if (strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_SURFACE_EXTENSION_NAME) == 0)
            instanceExtMap[pDisp].wsi_enabled = true;
    }
}
#endif // GENERIC_H
