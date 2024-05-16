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

#include "vulkan_util.h"
#include "configurator.h"

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/override.h"

#include <vulkan/vulkan.h>

#include <QLibrary>
#include <QtGlobal>

#include <cassert>

const char *GetVulkanLibrary() {
    static const char *TABLE[] = {
        "vulkan-1.dll",              // PLATFORM_WINDOWS
        "libvulkan",                 // PLATFORM_LINUX
        "/usr/local/lib/libvulkan",  // PLATFORM_MACOS
        "N/A",                       // PLATFORM_ANDROID
    };
    static_assert(countof(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
}

std::string GetUUIDString(const uint8_t deviceUUID[VK_UUID_SIZE]) {
    std::string result;

    for (std::size_t i = 0, n = VK_UUID_SIZE; i < n; ++i) {
        result += format("%02X", deviceUUID[i]);
    }

    return result;
}

Version GetVulkanLoaderVersion() {
    // Check loader version
    QLibrary library(GetVulkanLibrary());

    if (!library.load()) return Version::VERSION_NULL;

    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
    vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)library.resolve("vkEnumerateInstanceVersion");
    assert(vkEnumerateInstanceVersion);

    uint32_t version = 0;
    const VkResult result = vkEnumerateInstanceVersion(&version);
    assert(result == VK_SUCCESS);

    return Version(version);
}

static std::string GetUserDefinedLayersPathsLog(const char *label, UserDefinedLayersPaths custom_layer_path) {
    std::string log;

    const std::vector<std::string> &user_defined_layer_paths =
        Configurator::Get().environment.GetUserDefinedLayersPaths(custom_layer_path);
    if (!user_defined_layer_paths.empty()) {
        log += format("    - %s:\n", label);
        for (std::size_t i = 0, n = user_defined_layer_paths.size(); i < n; ++i)
            log += format("        - %s\n", user_defined_layer_paths[i].c_str());
    } else
        log += format("    - %s: None\n", label);

    return log;
}
