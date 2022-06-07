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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "version.h"
#include "util.h"

#include <vulkan/vulkan.h>

#include <cstdio>
#include <cassert>
#include <cstring>

const Version Version::VKCONFIG(2, 5, 0);
const Version Version::LAYER_CONFIG(2, 2, 3);
#ifdef VK_HEADER_VERSION_COMPLETE
const Version Version::VKHEADER(VK_HEADER_VERSION_COMPLETE);
#elif defined(VK_VERSION_1_2)
const Version Version::VKHEADER(1, 2, VK_HEADER_VERSION);
#elif defined(VK_VERSION_1_1)
const Version Version::VKHEADER(1, 1, VK_HEADER_VERSION);
#elif defined(VK_VERSION_1_0)
const Version Version::VKHEADER(1, 0, VK_HEADER_VERSION);
#endif
const Version Version::VERSION_NULL(0u);
const char *VKCONFIG_NAME = "Vulkan Configurator";
const char *VKCONFIG_SHORT_NAME = "vkconfig";

static Version GetVersionData(const char *version) {
    uint32_t version_major = 0;
    uint32_t version_minor = 0;
    uint32_t version_patch = 0;

    std::sscanf(version, "%d.%d.%d", &version_major, &version_minor, &version_patch);

    return Version(version_major, version_minor, version_patch);
}

Version::Version(uint32_t version_complete)
    : _major(VK_VERSION_MAJOR(version_complete)),
      _minor(VK_VERSION_MINOR(version_complete)),
      _patch(VK_VERSION_PATCH(version_complete)) {}

Version::Version(uint32_t version_major, uint32_t version_minor, uint32_t version_patch)
    : _major(version_major), _minor(version_minor), _patch(version_patch) {}

Version::Version(const char *version) : Version(GetVersionData(version)) {}

Version::Version(const std::string &version) : Version(version.c_str()) {}

std::string Version::str() const { return format("%d.%d.%d", _major, _minor, _patch); }

bool Version::operator!=(const Version &other_version) const {
    return VK_MAKE_VERSION(_major, _minor, _patch) !=
           VK_MAKE_VERSION(other_version._major, other_version._minor, other_version._patch);
}

bool Version::operator==(const Version &other_version) const {
    return VK_MAKE_VERSION(_major, _minor, _patch) ==
           VK_MAKE_VERSION(other_version._major, other_version._minor, other_version._patch);
}

bool Version::operator<(const Version &other_version) const {
    return VK_MAKE_VERSION(_major, _minor, _patch) <
           VK_MAKE_VERSION(other_version._major, other_version._minor, other_version._patch);
}

bool Version::operator>=(const Version &other_version) const {
    return VK_MAKE_VERSION(_major, _minor, _patch) >=
           VK_MAKE_VERSION(other_version._major, other_version._minor, other_version._patch);
}

bool Version::operator>(const Version &other_version) const {
    return VK_MAKE_VERSION(_major, _minor, _patch) >
           VK_MAKE_VERSION(other_version._major, other_version._minor, other_version._patch);
}

bool Version::operator<=(const Version &other_version) const {
    return VK_MAKE_VERSION(_major, _minor, _patch) <=
           VK_MAKE_VERSION(other_version._major, other_version._minor, other_version._patch);
}
