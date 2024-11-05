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

#include "util.h"
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
