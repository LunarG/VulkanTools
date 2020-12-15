/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include "platform.h"
#include "util.h"

#include <cstring>
#include <cassert>
#include <vector>
#include <string>

int GetPlatformFlags(const std::vector<std::string>& platform_strings) {
    int result = 0;

    for (std::size_t i = 0, n = platform_strings.size(); i < n; ++i) {
        result |= (1 << GetPlatformType(platform_strings[i].c_str()));
    }

    return result;
}

std::vector<std::string> GetPlatformTokens(int platform_flags) {
    std::vector<std::string> result;

    for (std::size_t i = 0, n = PLATFORM_COUNT; i < n; ++i) {
        if (platform_flags & (1 << i)) {
            result.push_back(GetToken(static_cast<PlatformType>(i)));
        }
    }

    return result;
}

const char* GetToken(PlatformType type) {
    static const char* table[] = {
        "WINDOWS",  // PLATFORM_WINDOWS
        "LINUX",    // PLATFORM_LINUX
        "MACOS"     // PLATFORM_MACOS
    };
    static_assert(countof(table) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[type];
}

PlatformType GetPlatformType(const char* token) {
    for (std::size_t i = 0, n = PLATFORM_COUNT; i < n; ++i) {
        const PlatformType platform_type = static_cast<PlatformType>(i);
        if (std::strcmp(GetToken(platform_type), token) == 0) return platform_type;
    }

    assert(0);
    return static_cast<PlatformType>(-1);
}

const char* GetToken(StatusType type) {
    static const char* table[] = {
        "STABLE",  // STATUS_STABLE
        "BETA",    // STATUS_BETA
        "ALPHA"    // STATUS_ALPHA
    };
    static_assert(countof(table) == STATUS_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[type];
}

StatusType GetStatusType(const char* token) {
    for (std::size_t i = 0, n = STATUS_COUNT; i < n; ++i) {
        const StatusType status_type = static_cast<StatusType>(i);
        if (std::strcmp(GetToken(status_type), token) == 0) return status_type;
    }

    assert(0);
    return static_cast<StatusType>(-1);
}

const char* GetPlatformString(PlatformString platform_string) {
    static const char* table[][PLATFORM_COUNT] = {
        {
            // PLATFORM_STRING_OS
            GetToken(PLATFORM_WINDOWS),  // PLATFORM_WINDOWS
            GetToken(PLATFORM_LINUX),    // PLATFORM_LINUX
            GetToken(PLATFORM_MACOS)     // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_APP_SUFFIX
            ".exe",  // PLATFORM_WINDOWS
            "",      // PLATFORM_LINUX
            ".app"   // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_EXE_SUFFIX
            ".exe",  // PLATFORM_WINDOWS
            "",      // PLATFORM_LINUX
            ""       // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_SDK_BIN_DIR
            "/Bin",  // PLATFORM_WINDOWS
            "/bin",  // PLATFORM_LINUX
            "/bin"   // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_SEPARATOR
            ";",  // PLATFORM_WINDOWS
            ":",  // PLATFORM_LINUX
            ":"   // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_FILTER
            "Applications (*.exe)",    // PLATFORM_WINDOWS
            "Applications (*)",        // PLATFORM_LINUX
            "Applications (*.app, *)"  // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_VULKAN_INFO
            "vulkaninfoSDK",              // PLATFORM_WINDOWS
            "vulkaninfo",                 // PLATFORM_LINUX
            "/usr/local/bin/vulkaninfo",  // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_VULKAN_LIBRARY
            "vulkan-1.dll",              // PLATFORM_WINDOWS
            "libvulkan",                 // PLATFORM_LINUX
            "/usr/local/lib/libvulkan",  // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_VULKAN_CONTENT
            "/Content",               // PLATFORM_WINDOWS
            "/share/vulkan/content",  // PLATFORM_LINUX
            "/share/vulkan/content",  // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_EXPLICIT_LAYERS
            "/Bin",                          // PLATFORM_WINDOWS
            "/etc/vulkan/explicit_layer.d",  // PLATFORM_LINUX
            "/etc/vulkan/explicit_layer.d",  // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_PATH_VKC_USER_DIR
            "/AppData/Local/LunarG/vkconfig/",        // PLATFORM_WINDOWS
            "/.local/share/vulkan/lunarg-vkconfig/",  // PLATFORM_LINUX
            "/.local/share/vulkan/lunarg-vkconfig/"   // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_PATH_CONFIGURATION
            "/AppData/Local/LunarG/vkconfig/configurations/",        // PLATFORM_WINDOWS
            "/.local/share/vulkan/lunarg-vkconfig/configurations/",  // PLATFORM_LINUX
            "/.local/share/vulkan/lunarg-vkconfig/configurations/"   // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_PATH_OVERRIDE_LAYERS
            "/AppData/Local/LunarG/vkconfig/override",  // PLATFORM_WINDOWS
            "/.local/share/vulkan/implicit_layer.d",    // PLATFORM_LINUX
            "/.local/share/vulkan/implicit_layer.d"     // PLATFORM_MACOS
        },
        {
            // PLATFORM_STRING_PATH_OVERRIDE_SETTINGS
            "/AppData/Local/LunarG/vkconfig/override",  // PLATFORM_WINDOWS
            "/.local/share/vulkan/settings.d",          // PLATFORM_LINUX
            "/.local/share/vulkan/settings.d"           // PLATFORM_MACOS
        },
    };
    static_assert(countof(table) == PLATFORM_STRING_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[platform_string][VKC_PLATFORM];
}
