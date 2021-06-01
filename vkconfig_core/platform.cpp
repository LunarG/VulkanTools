/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
#include "path.h"
#include "util.h"

#include <cstring>
#include <cassert>
#include <vector>
#include <string>

#include <QDir>

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
        "MACOS",    // PLATFORM_MACOS
        "ANDROID",  // PLATFORM_ANDROID
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
        "STABLE",     // STATUS_STABLE
        "BETA",       // STATUS_BETA
        "ALPHA",      // STATUS_ALPHA
        "DEPRECATED"  // STATUS_ALPHA
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

bool IsPlatformSupported(int platform_flags) { return platform_flags & (1 << VKC_PLATFORM); }
