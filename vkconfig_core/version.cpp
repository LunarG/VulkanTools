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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "version.h"
#include "util.h"

#include <vulkan/vulkan.h>

#include <cstdio>
#include <cassert>
#include <cstring>

const Version Version::VKCONFIG(3, 4, 3);
const Version Version::VKHEADER(VK_HEADER_VERSION_COMPLETE);
const Version Version::NONE(0, 0, 0);
const Version Version::LATEST(~0, ~0, ~0);
const Version Version::REQUIRED_LOADER_VERSION(1, 3, 301);

const char *VKCONFIG_NAME = "Vulkan Configurator";
const char *VKCONFIG_SHORT_NAME = "vkconfig";

static Version GetVersionData(const char *version) {
    uint32_t version_major = 0;
    uint32_t version_minor = 0;
    uint32_t version_patch = 0;
    uint32_t version_revision = 0;

    std::size_t count = CountChar(version, '.');

    switch (count) {
        default:
            assert(0);
            return Version::NONE;
        case 0:
            std::sscanf(version, "%d", &version_major);
            return Version(version_major, version_minor);
        case 1:
            std::sscanf(version, "%d.%d", &version_major, &version_minor);
            return Version(version_major, version_minor);
        case 2:
            std::sscanf(version, "%d.%d.%d", &version_major, &version_minor, &version_patch);
            return Version(version_major, version_minor, version_patch);
        case 3:
            std::sscanf(version, "%d.%d.%d.%d", &version_major, &version_minor, &version_patch, &version_revision);
            return Version(version_major, version_minor, version_patch, version_revision);
    }
}

Version::Version(uint32_t version_complete)
    : _major(VK_VERSION_MAJOR(version_complete)),
      _minor(VK_VERSION_MINOR(version_complete)),
      _patch(VK_VERSION_PATCH(version_complete)),
      _revision(VK_API_VERSION_VARIANT(version_complete)) {}

Version::Version(uint32_t version_major, uint32_t version_minor) : _major(version_major), _minor(version_minor), type(WITH_MINOR) {}

Version::Version(uint32_t version_major, uint32_t version_minor, uint32_t version_patch)
    : _major(version_major), _minor(version_minor), _patch(version_patch), type(WITH_PATCH) {}

Version::Version(uint32_t version_major, uint32_t version_minor, uint32_t version_patch, uint32_t version_revision)
    : _major(version_major), _minor(version_minor), _patch(version_patch), _revision(version_revision), type(WITH_REVISION) {}

Version::Version(const char *version) : Version(GetVersionData(version)) {}

Version::Version(const std::string &version) : Version(version.c_str()) {}

std::string Version::str() const {
    if (*this == LATEST) {
        return "Latest";
    } else {
        switch (this->type) {
            case WITH_MAJOR:
                return format("%d", _major);
            case WITH_MINOR:
                return format("%d.%d", _major, _minor);
            default:
            case WITH_PATCH:
                return format("%d.%d.%d", _major, _minor, _patch);
            case WITH_REVISION:
                return format("%d.%d.%d.%d", _major, _minor, _patch, _revision);
        }
    }
}

bool Version::operator!=(const Version &other_version) const {
    return VK_MAKE_API_VERSION(this->_major, this->_minor, this->_patch, this->_revision) !=
           VK_MAKE_API_VERSION(other_version._major, other_version._minor, other_version._patch, other_version._revision);
}

bool Version::operator==(const Version &other_version) const {
    return VK_MAKE_API_VERSION(this->_major, this->_minor, this->_patch, this->_revision) ==
           VK_MAKE_API_VERSION(other_version._major, other_version._minor, other_version._patch, other_version._revision);
}

bool Version::operator<(const Version &other_version) const {
    return VK_MAKE_API_VERSION(this->_major, this->_minor, this->_patch, this->_revision) <
           VK_MAKE_API_VERSION(other_version._major, other_version._minor, other_version._patch, other_version._revision);
}

bool Version::operator>=(const Version &other_version) const {
    return VK_MAKE_API_VERSION(this->_major, this->_minor, this->_patch, this->_revision) >=
           VK_MAKE_API_VERSION(other_version._major, other_version._minor, other_version._patch, other_version._revision);
}

bool Version::operator>(const Version &other_version) const {
    return VK_MAKE_API_VERSION(this->_major, this->_minor, this->_patch, this->_revision) >
           VK_MAKE_API_VERSION(other_version._major, other_version._minor, other_version._patch, other_version._revision);
}

bool Version::operator<=(const Version &other_version) const {
    return VK_MAKE_API_VERSION(this->_major, this->_minor, this->_patch, this->_revision) <=
           VK_MAKE_API_VERSION(other_version._major, other_version._minor, other_version._patch, other_version._revision);
}
