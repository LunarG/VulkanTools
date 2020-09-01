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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#pragma once

#include <vulkan/vulkan.h>

#include <cstddef>
#include <string>

static const int VKCONFIG_BUILD = 1003;

struct Version {
    static const Version header_version;

    Version(int major_version, int minor_version, int patch_version)
        : _vku_major(major_version), _vku_minor(minor_version), _vku_patch(patch_version) {}
    Version(const char *version);

    std::string str() const;

    bool operator!=(const Version &other_version) const { return !(*this == other_version); }

    bool operator==(const Version &other_version) const {
        if (_vku_major != other_version._vku_major) return false;
        if (_vku_minor != other_version._vku_minor) return false;
        if (_vku_patch != other_version._vku_patch) return false;
        return true;
    }

    bool operator<(const Version &other_version) const {
        if (_vku_major < other_version._vku_major) return true;
        if (_vku_minor < other_version._vku_minor) return true;
        if (_vku_patch < other_version._vku_patch) return true;
        return false;
    }

    bool operator>=(const Version &other_version) const { return !(*this < other_version); }

    bool operator>(const Version &other_version) const {
        if (_vku_major > other_version._vku_major) return true;
        if (_vku_minor > other_version._vku_minor) return true;
        if (_vku_patch > other_version._vku_patch) return true;
        return false;
    }

    bool operator<=(const Version &other_version) const { return !(*this > other_version); }

    int _vku_major, _vku_minor, _vku_patch;
};
