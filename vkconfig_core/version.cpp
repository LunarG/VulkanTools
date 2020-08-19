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

#include "version.h"
#include "util.h"

#include <cstdio>
#include <cassert>
#include <cstring>

const Version Version::header_version(VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE), VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
                                      VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE));

Version::Version(const char* version) { sscanf(version, "%d.%d.%d", &_vku_major, &_vku_minor, &_vku_patch); }

std::string Version::str() const { return format("%d.%d.%d", _vku_major, _vku_minor, _vku_patch); }
