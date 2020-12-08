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

#include "util.h"
#include "platform.h"

#include <cstddef>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <cctype>

#include <QString>
#include <QStringList>
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

// delimted string is a comma delimited string. If value is found remove it
void RemoveString(std::string& delimited_string, const std::string& value) {
    if (delimited_string.find(value) == std::string::npos) return;

    QStringList list = QString(delimited_string.c_str()).split(",");
    for (int i = 0, n = list.size(); i < n; ++i) {
        if (list[i] == value.c_str()) {
            list.removeAt(i);
            break;
        }
    }

    delimited_string = list.join(",").toStdString();
}

// Pretty simple, add to list if it's not already in it
void AppendString(std::string& delimited_string, const std::string& value) {
    if (delimited_string.find(value) != std::string::npos) return;

    if (!delimited_string.empty()) delimited_string += ",";

    delimited_string += value;
}
