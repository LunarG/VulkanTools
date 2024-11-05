/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "version.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

extern const Version REQUIRED_LOADER_VERSION;

enum VendorID {
    VENDOR_ID_KHRONOS = VK_VENDOR_ID_KHRONOS,
    VENDOR_ID_MESA = VK_VENDOR_ID_MESA,
    VENDOR_ID_AMD = 0x1022,
    VENDOR_ID_APPLE = 0x106B,
    VENDOR_ID_ARM = 0x13B5,
    VENDOR_ID_IMGTEC = 0x1010,
    VENDOR_ID_INTEL = 0x8086,
    VENDOR_ID_MICROSOFT = 0x1414,
    VENDOR_ID_NVIDIA = 0x10DE,
    VENDOR_ID_QUALCOMM = 0x17CB,
};

struct VulkanPhysicalDeviceInfo {
    std::string deviceName;
    std::string deviceUUID;
    std::string driverUUID;
    std::string deviceLUID;
    Version apiVersion;
    uint32_t driverVersion;
    VendorID vendorID;
    uint32_t deviceID;
    VkPhysicalDeviceType deviceType;
};

struct VulkanSystemInfo {
    Version loaderVersion = Version::NONE;
    std::vector<VkLayerProperties> instanceLayerProperties;
    std::vector<VkExtensionProperties> instanceExtensionPropertie;
    std::vector<VulkanPhysicalDeviceInfo> physicalDevices;
};

VulkanSystemInfo BuildVulkanSystemInfo();

std::vector<std::string> BuildEnvVariablesList(const char *layer_key, const char *setting_key);

const char *GetLabel(VkPhysicalDeviceType deviceType);

std::string GetLabel(VendorID vendorID);
