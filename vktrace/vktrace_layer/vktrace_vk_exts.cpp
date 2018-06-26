/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * All Rights Reserved.
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
 */
#include "vktrace_lib_helpers.h"

#include "vulkan/vk_layer.h"

void ext_init_create_instance(layer_instance_data* instData, VkInstance inst, uint32_t extension_count,
                              const char* const* ppEnabledExtensions)  // extension names to be enabled
{
    PFN_vkGetInstanceProcAddr gpa = instData->instTable.GetInstanceProcAddr;

    instData->instTable.CreateDebugReportCallbackEXT =
        (PFN_vkCreateDebugReportCallbackEXT)gpa(inst, "vkCreateDebugReportCallbackEXT");
    instData->instTable.DestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT)gpa(inst, "vkDestroyDebugReportCallbackEXT");
    instData->instTable.DebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)gpa(inst, "vkDebugReportMessageEXT");
    instData->instTable.GetPhysicalDeviceSurfaceSupportKHR =
        (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)gpa(inst, "vkGetPhysicalDeviceSurfaceSupportKHR");
    instData->instTable.DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)gpa(inst, "vkDestroySurfaceKHR");
    instData->instTable.GetPhysicalDeviceSurfaceCapabilitiesKHR =
        (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)gpa(inst, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    instData->instTable.GetPhysicalDeviceSurfaceFormatsKHR =
        (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)gpa(inst, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    instData->instTable.GetPhysicalDeviceSurfacePresentModesKHR =
        (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)gpa(inst, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    instData->instTable.CreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)gpa(inst, "vkCreateXlibSurfaceKHR");
    instData->instTable.GetPhysicalDeviceXlibPresentationSupportKHR =
        (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)gpa(inst, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
    instData->instTable.CreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)gpa(inst, "vkCreateXcbSurfaceKHR");
    instData->instTable.GetPhysicalDeviceXcbPresentationSupportKHR =
        (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)gpa(inst, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_MIR_KHR)
    instData->instTable.CreateMirSurfaceKHR = (PFN_vkCreateMirSurfaceKHR)gpa(inst, "vkCreateMirSurfaceKHR");
    instData->instTable.GetPhysicalDeviceMirPresentationSupportKHR =
        (PFN_vkGetPhysicalDeviceMirPresentationSupportKHR)gpa(inst, "vkGetPhysicalDeviceMirPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    instData->instTable.CreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR)gpa(inst, "vkCreateWaylandSurfaceKHR");
    instData->instTable.GetPhysicalDeviceWaylandPresentationSupportKHR =
        (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)gpa(inst, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    instData->instTable.CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)gpa(inst, "vkCreateWin32SurfaceKHR");
    instData->instTable.GetPhysicalDeviceWin32PresentationSupportKHR =
        (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)gpa(inst, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    instData->instTable.CreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)gpa(inst, "vkCreateAndroidSurfaceKHR");
#endif
    instData->LunargDebugReportEnabled = false;
    instData->KHRSurfaceEnabled = false;
    instData->KHRXlibSurfaceEnabled = false;
    instData->KHRXcbSurfaceEnabled = false;
    instData->KHRWaylandSurfaceEnabled = false;
    instData->KHRMirSurfaceEnabled = false;
    instData->KHRWin32SurfaceEnabled = false;
    instData->KHRAndroidSurfaceEnabled = false;
    for (uint32_t i = 0; i < extension_count; i++) {
        if (strcmp(ppEnabledExtensions[i], VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0) {
            instData->LunargDebugReportEnabled = true;
        }
        if (strcmp(ppEnabledExtensions[i], VK_KHR_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRSurfaceEnabled = true;
        }
#if defined(VK_USE_PLATFORM_XLIB_KHR)
        if (strcmp(ppEnabledExtensions[i], VK_KHR_XLIB_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRXlibSurfaceEnabled = true;
        }
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
        if (strcmp(ppEnabledExtensions[i], VK_KHR_XCB_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRXcbSurfaceEnabled = true;
        }
#endif
#if defined(VK_USE_PLATFORM_MIR_KHR)
        if (strcmp(ppEnabledExtensions[i], VK_KHR_MIR_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRMirSurfaceEnabled = true;
        }
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
        if (strcmp(ppEnabledExtensions[i], VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRWaylandSurfaceEnabled = true;
        }
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
        if (strcmp(ppEnabledExtensions[i], VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRWin32SurfaceEnabled = true;
        }
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
        if (strcmp(ppEnabledExtensions[i], VK_KHR_ANDROID_SURFACE_EXTENSION_NAME) == 0) {
            instData->KHRAndroidSurfaceEnabled = true;
        }
#endif
    }
}

void ext_init_create_device(layer_device_data* devData, VkDevice dev, PFN_vkGetDeviceProcAddr gpa, uint32_t extension_count,
                            const char* const* ppEnabledExtensions)  // extension names to be enabled
{
    devData->devTable.CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)gpa(dev, "vkCreateSwapchainKHR");
    devData->devTable.DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)gpa(dev, "vkDestroySwapchainKHR");
    devData->devTable.GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)gpa(dev, "vkGetSwapchainImagesKHR");
    devData->devTable.AcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)gpa(dev, "vkAcquireNextImageKHR");
    devData->devTable.QueuePresentKHR = (PFN_vkQueuePresentKHR)gpa(dev, "vkQueuePresentKHR");

    devData->KHRDeviceSwapchainEnabled = false;
    for (uint32_t i = 0; i < extension_count; i++) {
        if (strcmp(ppEnabledExtensions[i], VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            devData->KHRDeviceSwapchainEnabled = true;
        }
    }
}
