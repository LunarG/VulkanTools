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

#include <cctype>

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

void RemoveString(std::vector<std::string>& list, const std::string& value) {
    std::vector<std::string> new_list;
    new_list.reserve(list.size());

    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        if (list[i] != value) {
            new_list.push_back(list[i]);
        }
    }

    std::swap(list, new_list);
}

void AppendString(std::vector<std::string>& list, const std::string& value) {
    if (std::find(list.begin(), list.end(), value) == list.end()) {
        list.push_back(value);
    }
}

bool IsStringFound(const std::vector<std::string>& list, const std::string& value) {
    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        if (list[i] == value) return true;
    }

    return false;
}

std::vector<std::string> ConvertString(const QStringList& string_list) {
    std::vector<std::string> strings;

    for (int i = 0, n = string_list.length(); i < n; ++i) {
        strings.push_back(string_list[i].toStdString());
    }

    return strings;
}

QStringList ConvertString(const std::vector<std::string>& strings) {
    QStringList string_list;

    for (std::size_t i = 0, n = strings.size(); i < n; ++i) {
        string_list.append(strings[i].c_str());
    }

    return string_list;
}

std::string ToLowerCase(const std::string& value) {
    std::string result = value;

    for (std::size_t i = 0, n = result.size(); i < n; ++i) {
        result[i] = std::tolower(result[i]);
    }

    return result;
}

std::string ToUpperCase(const std::string& value) {
    std::string result = value;

    for (std::size_t i = 0, n = result.size(); i < n; ++i) {
        result[i] = std::toupper(result[i]);
    }

    return result;
}
