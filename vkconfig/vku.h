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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <vulkan/vulkan.h>
#include <cstddef>
#include <string>

namespace vku {

std::string format(const char *message, ...);

template <typename T, std::size_t N>
inline constexpr std::size_t countof(T const (&)[N]) noexcept {
    return N;
}

struct version {
    static const version header_version;

    version(int major_version, int minor_version, int patch_version)
        : major(major_version), minor(minor_version), patch(patch_version) {}
    version(const char *version);

    std::string str() const;

    bool operator!=(const version &other_version) const { return !(*this == other_version); }

    bool operator==(const version &other_version) const {
        if (major != other_version.major) return false;
        if (minor != other_version.minor) return false;
        if (patch != other_version.patch) return false;
        return true;
    }

    bool operator<(const version &other_version) const {
        if (major < other_version.major) return true;
        if (minor < other_version.minor) return true;
        if (patch < other_version.patch) return true;
        return false;
    }

    bool operator>=(const version &other_version) const { return !(*this < other_version); }

    bool operator>(const version &other_version) const {
        if (major > other_version.major) return true;
        if (minor > other_version.minor) return true;
        if (patch > other_version.patch) return true;
        return false;
    }

    bool operator<=(const version &other_version) const { return !(*this > other_version); }

    int major, minor, patch;
};

}  // namespace vku

///////////////////////////////////
// Tests

int test_vku();
