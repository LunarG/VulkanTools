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

#include "version.h"
#include "util.h"

#include <vulkan/vulkan.h>

#include <cstdio>
#include <cassert>
#include <cstring>

const Version Version::VKCONFIG(2, 0, 3);
const Version Version::VKHEADER(VK_HEADER_VERSION_COMPLETE);
const Version Version::VERSION_NULL(0u);

static uint32_t GetVersionData(const char* version) {
    uint32_t version_major = 0;
    uint32_t version_minor = 0;
    uint32_t version_patch = 0;

    sscanf(version, "%d.%d.%d", &version_major, &version_minor, &version_patch);

    return VK_MAKE_VERSION(version_major, version_minor, version_patch);
}

Version::Version(uint32_t version_major, uint32_t version_minor, uint32_t version_patch)
    : _data(VK_MAKE_VERSION(version_major, version_minor, version_patch)) {}

Version::Version(const char* version) : _data(GetVersionData(version)) {}

Version::Version(const QString& version) : Version(version.toUtf8().constData()) {}

uint32_t Version::GetMajor() const { return VK_VERSION_MAJOR(_data); }

uint32_t Version::GetMinor() const { return VK_VERSION_MINOR(_data); }

uint32_t Version::GetPatch() const { return VK_VERSION_PATCH(_data); }

std::string Version::str() const {
    return format("%d.%d.%d", VK_VERSION_MAJOR(_data), VK_VERSION_MINOR(_data), VK_VERSION_PATCH(_data));
}
