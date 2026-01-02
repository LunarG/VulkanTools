/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include "util.h"
#include "version.h"
#include "type_platform.h"

#include <map>

static std::map<std::string, int> const translation = {std::pair("WINDOWS_X86", PLATFORM_WINDOWS_X86_BIT),
                                                       std::pair("WINDOWS_ARM", PLATFORM_WINDOWS_ARM_BIT),
                                                       std::pair("WINDOWS", PLATFORM_WINDOWS_X86_BIT | PLATFORM_WINDOWS_ARM_BIT),
                                                       std::pair("LINUX", PLATFORM_LINUX_BIT),
                                                       std::pair("MACOS", PLATFORM_MACOS_BIT),
                                                       std::pair("ANDROID", PLATFORM_ANDROID_BIT),
                                                       std::pair("IOS", PLATFORM_IOS_BIT)};

int GetPlatformFlags(const std::vector<std::string>& platform_strings) {
    int result = 0;

    for (std::size_t i = 0, n = platform_strings.size(); i < n; ++i) {
        auto it = translation.find(platform_strings[i]);
        if (it != translation.end()) {
            result |= it->second;
        }
    }

    return result;
}

std::vector<std::string> GetPlatformTokens(int platform_flags) {
    std::vector<std::string> result;

    for (std::size_t i = 0, n = PLATFORM_COUNT; i < n; ++i) {
        const int flag = platform_flags & (1 << i);
        if (!flag) {
            continue;
        }

        for (auto it = translation.begin(), end = translation.end(); it != end; ++it) {
            if (it->second != flag) {
                continue;
            }

            result.push_back(it->first);
        }
    }

    return result;
}

bool IsDesktop(PlatformType type) { return type >= PLATFORM_DESKTOP_FIRST && type <= PLATFORM_DESKTOP_LAST; }

bool IsPlatformSupported(int platform_flags) { return platform_flags & (1 << VKC_PLATFORM); }

const char* GetLabel(PlatformType type) {
    static const char* TABLE[] = {
#ifdef _WIN64
        "Windows x86-64",  // PLATFORM_WINDOWS_X86
#else
        "Windows x86-32",  // PLATFORM_WINDOWS_X86
#endif
        "Windows ARM",  // PLATFORM_WINDOWS_ARM
        "Linux",        // PLATFORM_LINUX
        "macOS",        // PLATFORM_MACOS
        "Android",      // PLATFORM_ANDROID
        "iOS",          // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type];
}

const char* GetLatestReleaseSDK(PlatformType type) {
    assert(IsDesktop(type));

    static const char* TABLE[] = {
        "https://vulkan.lunarg.com/sdk/latest/windows.txt",  // PLATFORM_WINDOWS_X86
        "https://vulkan.lunarg.com/sdk/latest/warm.txt",     // PLATFORM_WINDOWS_ARM
        "https://vulkan.lunarg.com/sdk/latest/linux.txt",    // PLATFORM_LINUX
        "https://vulkan.lunarg.com/sdk/latest/mac.txt",      // PLATFORM_MACOS
        "N/A",                                               // PLATFORM_ANDROID
        "N/A",                                               // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type];
}

const char* GetInstallerFilename(PlatformType type) {
    assert(IsDesktop(type));

    static const char* TABLE[] = {
        "vulkan_sdk.exe",     // PLATFORM_WINDOWS_X86
        "vulkan_sdk.exe",     // PLATFORM_WINDOWS_ARM
        "vulkan_sdk.tar.xz",  // PLATFORM_LINUX
        "vulkan_sdk.zip",     // PLATFORM_MACOS
        "N/A",                // PLATFORM_ANDROID
        "N/A",                // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type];
}

const char* GetVersionedFilename(PlatformType type) {
    assert(IsDesktop(type));

    static const char* TABLE[] = {
        "vulkan_sdk-%s.exe",     // PLATFORM_WINDOWS_X86
        "vulkan_sdk-%s.exe",     // PLATFORM_WINDOWS_ARM
        "vulkan_sdk-%s.tar.xz",  // PLATFORM_LINUX
        "vulkan_sdk-%s.zip",     // PLATFORM_MACOS
        "N/A",                   // PLATFORM_ANDROID
        "N/A",                   // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type];
}

const char* GetLatestPackageSDK(PlatformType type) {
    assert(IsDesktop(type));

    static const char* TABLE[] = {
        "https://sdk.lunarg.com/sdk/download/latest/windows/vulkan_sdk.exe",   // PLATFORM_WINDOWS_X86
        "https://sdk.lunarg.com/sdk/download/latest/warm/vulkan_sdk.exe",      // PLATFORM_WINDOWS_ARM
        "https://sdk.lunarg.com/sdk/download/latest/linux/vulkan_sdk.tar.xz",  // PLATFORM_LINUX
        "https://sdk.lunarg.com/sdk/download/latest/mac/vulkan_sdk.zip",       // PLATFORM_MACOS
        "N/A",                                                                 // PLATFORM_ANDROID
        "N/A",                                                                 // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type];
}

std::string GetLunarGUrl(PlatformType type) {
    assert(IsDesktop(type));

    const char* TABLE[] = {
        "windows",  // PLATFORM_WINDOWS_X86
        "windows",  // PLATFORM_WINDOWS_ARM
        "linux",    // PLATFORM_LINUX
        "mac",      // PLATFORM_MACOS
        "N/A",      // PLATFORM_ANDROID
        "N/A",      // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT);

    return format("https://vulkan.lunarg.com/doc/sdk/%s.0/%s", Version::VKHEADER.str().c_str(), TABLE[type]);
}
