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
#include <regex>

std::string format(const char* message, ...) {
    std::size_t const STRING_BUFFER(4096);

    assert(message != nullptr);
    assert(strlen(message) < STRING_BUFFER);

    char buffer[STRING_BUFFER];
    va_list list;

    va_start(list, message);
    vsprintf(buffer, message, list);
    va_end(list);

    return buffer;
}

bool IsFrames(const std::string& s) {
    static const std::regex FRAME_REGEX("^([0-9]+([-][0-9]+){0,2})(,([0-9]+([-][0-9]+){0,2}))*$");

    return std::regex_search(s, FRAME_REGEX);
}

bool IsNumber(const std::string& s) {
    static const std::regex FRAME_REGEX("^-?([0-9]*|0x[0-9|a-z|A-Z]*)$");

    return std::regex_search(s, FRAME_REGEX);
}

bool IsFloat(const std::string& s) {
    static const std::regex FRAME_REGEX("^-?[0-9]*([.][0-9]*)?$");

    return std::regex_search(s, FRAME_REGEX);
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

std::vector<std::string> Split(const std::string& value, const std::string& delimiter) {
    std::vector<std::string> result;

    std::string parse = value;

    std::size_t start = 0;
    std::size_t end = parse.find(delimiter);
    while (end != std::string::npos) {
        result.push_back(parse.substr(start, end - start));
        start = end + delimiter.length();
        end = parse.find(delimiter, start);
    }

    const std::string last = parse.substr(start, end);
    if (!last.empty()) {
        result.push_back(last);
    }

    return result;
}

std::string Merge(const std::vector<std::string>& value, const std::string& delimiter) {
    std::string result;

    for (std::size_t i = 0, n = value.size(); i < n; ++i) {
        result += value[i];
        if (i < n - 1) {
            result += delimiter;
        }
    }

    return result;
}

void RemoveValue(std::vector<NumberOrString>& list, const NumberOrString& value) {
    std::vector<NumberOrString> new_list;
    new_list.reserve(list.size());

    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        if (list[i] != value) {
            new_list.push_back(list[i]);
        }
    }

    std::swap(list, new_list);
}

void RemoveValue(std::vector<EnabledNumberOrString>& list, const EnabledNumberOrString& value) {
    std::vector<EnabledNumberOrString> new_list;
    new_list.reserve(list.size());

    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        if (list[i] != value) {
            new_list.push_back(list[i]);
        }
    }

    std::swap(list, new_list);
}

void AppendValue(std::vector<NumberOrString>& list, const NumberOrString& value) {
    if (std::find(list.begin(), list.end(), value) == list.end()) {
        list.push_back(value);
    }
}

bool IsValueFound(const std::vector<NumberOrString>& list, const NumberOrString& value) {
    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        if (list[i] == value) return true;
    }

    return false;
}

bool IsValueFound(const std::vector<EnabledNumberOrString>& list, const NumberOrString& value) {
    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        if (list[i] == value) return true;
    }

    return false;
}

QStringList ConvertValues(const std::vector<NumberOrString>& values) {
    QStringList string_list;

    for (std::size_t i = 0, n = values.size(); i < n; ++i) {
        if (values[i].key.empty()) {
            string_list.append(format("%d", values[i].number).c_str());
        } else {
            string_list.append(values[i].key.c_str());
        }
    }

    return string_list;
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

std::string GetLayerSettingPrefix(const std::string& key) {
    QString result(key.c_str());
    result.remove("VK_LAYER_");
    return ToLowerCase(result.toStdString()) + ".";
}
