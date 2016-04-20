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
 *
 */

#ifndef EXTENSION_INFO_H
#define EXTENSION_INFO_H

#include "intel.h"

enum intel_phy_dev_ext_type {
    INTEL_PHY_DEV_EXT_WSI_SWAPCHAIN,

    INTEL_PHY_DEV_EXT_COUNT,
    INTEL_PHY_DEV_EXT_INVALID = INTEL_PHY_DEV_EXT_COUNT,
};

enum intel_global_ext_type {
    INTEL_PHY_DEV_EXT_DEBUG_REPORT,
    INTEL_GLOBAL_EXT_WSI_SURFACE,
#ifdef VK_USE_PLATFORM_XCB_KHR
    INTEL_GLOBAL_EXT_WSI_XCB_SURFACE,
#endif // VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
    INTEL_GLOBAL_EXT_WSI_XLIB_SURFACE,
#endif // VK_USE_PLATFORM_XLIB_KHR
    INTEL_GLOBAL_EXT_COUNT,
    INTEL_GLOBAL_EXT_INVALID = INTEL_GLOBAL_EXT_COUNT,
};

extern const VkExtensionProperties intel_phy_dev_gpu_exts[INTEL_PHY_DEV_EXT_COUNT];
extern const VkExtensionProperties intel_global_exts[INTEL_GLOBAL_EXT_COUNT];

bool compare_vk_extension_properties(
        const VkExtensionProperties *op1,
        const char                  *extensionName);
#endif /* EXTENSION_INFO_H */
