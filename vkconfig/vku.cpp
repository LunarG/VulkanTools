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

#include "vku.h"
#include <cstdarg>
#include <cstdio>
#include <cassert>

namespace vku {

std::string format(const char* message, ...) {
    std::size_t const STRING_BUFFER(4096);

    assert(message != nullptr);
    assert(strlen(message) >= 0 && strlen(message) < STRING_BUFFER);

    char buffer[STRING_BUFFER];
    va_list list;

    va_start(list, message);
    vsprintf(buffer, message, list);
    va_end(list);

    return buffer;
}

const version version::header_version(VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE), VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
                                      VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE));

version::version(const char* version) { sscanf(version, "%d.%d.%d", &major, &minor, &patch); }

std::string version::str() const { return format("%d.%d.%d", major, minor, patch); }

}  // namespace vku
