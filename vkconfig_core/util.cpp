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

#include "util.h"

#include <cstddef>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <cctype>

#include <QDir>

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

bool IsNumber(const std::string& s) {
    if (s.empty()) return false;

    for (std::size_t i = 0, n = s.length(); i < n; ++i) {
        if (std::isdigit(s[i])) continue;

        return false;
    }

    return true;
}

std::string ReplacePathBuiltInVariables(const std::string& path) {
    static const std::string HOME("$HOME");

    const std::size_t found = path.find_first_of(HOME);
    if (found < path.size()) {
        const std::size_t offset = found + HOME.size();
        return QDir().homePath().toStdString() + path.substr(found + offset, path.size() - offset);
    }

    // No built-in variable found, return unchanged
    return path;
}
