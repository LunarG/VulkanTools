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
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 * Author: Ian Elliott <ian@lunarg.com>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 *
 */

#include "extension_info.h"

const VkExtensionProperties intel_global_exts[INTEL_GLOBAL_EXT_COUNT] = {
    {
        .extensionName = VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        .specVersion = VK_EXT_DEBUG_REPORT_SPEC_VERSION,
    },
    {
        .extensionName = VK_KHR_SURFACE_EXTENSION_NAME,
        .specVersion = VK_KHR_SURFACE_SPEC_VERSION,
    }
#ifdef VK_USE_PLATFORM_XCB_KHR
    ,{
        .extensionName = VK_KHR_XCB_SURFACE_EXTENSION_NAME,
        .specVersion = VK_KHR_XCB_SURFACE_SPEC_VERSION,
    }
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    ,{
        .extensionName = VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
        .specVersion = VK_KHR_XLIB_SURFACE_SPEC_VERSION,
    }
#endif
#if 0
#ifdef VK_USE_PLATFORM_MIR_KHR
    ,{
        .extensionName = VK_KHR_MIR_SURFACE_EXTENSION_NAME,
        .specVersion = VK_KHR_MIR_SURFACE_SPEC_VERSION,
    }
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    ,{
        .extensionName = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
        .specVersion = VK_KHR_WAYLAND_SURFACE_SPEC_VERSION,
    }
#endif
#endif
};

const VkExtensionProperties intel_phy_dev_gpu_exts[INTEL_PHY_DEV_EXT_COUNT] = {
    {
        .extensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        .specVersion = VK_KHR_SWAPCHAIN_SPEC_VERSION,
    }
};

bool compare_vk_extension_properties(
        const VkExtensionProperties *op1,
        const char *extensionName)
{
    return strcmp(op1->extensionName, extensionName) == 0 ? true : false;
}
