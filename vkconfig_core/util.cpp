/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include "type_platform.h"

#include <QString>
#include <QStringList>

#include <cstddef>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <cctype>

#include <cctype>
#include <regex>
#include <set>

const char* GetToken(ParseSource type) {
    static const char* table[] = {",", VKC_ENV == VKC_ENV_WIN32 ? ";" : ":"};

    return table[type];
}

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

std::string FormatNvidia(uint32_t driverVersion) {
    return format("%d.%d.%d.%d", (driverVersion >> 22) & 0x3ff, (driverVersion >> 14) & 0x0ff, (driverVersion >> 6) & 0x0ff,
                  driverVersion & 0x003f);
}

std::string FormatIntelWindows(uint32_t driverVersion) { return format("%d.%d", (driverVersion >> 14), (driverVersion)&0x3fff); }

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

std::string TrimString(const std::string& str, const std::string& whitespaces) {
    const auto strBegin = str.find_first_not_of(whitespaces);
    if (strBegin == std::string::npos) return "";  // no content

    const auto strEnd = str.find_last_not_of(whitespaces);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string TrimSurroundingWhitespace(const std::string& str, const std::string& whitespaces) {
    std::string s = str;
    s.erase(0, s.find_first_not_of(whitespaces));
    s.erase(s.find_last_not_of(whitespaces) + 1);
    return s;
}

std::vector<std::string> TrimEmpty(const std::vector<std::string>& values) {
    std::vector<std::string> results;

    for (std::size_t i = 0, n = values.size(); i < n; ++i) {
        if (values[i].empty()) {
            continue;
        }

        results.push_back(values[i]);
    }

    return results;
}

std::vector<std::string> Split(const std::string& value, const std::string& delimiter) {
    std::vector<std::string> split_result;

    std::string parse = value;

    std::size_t start = 0;
    std::size_t end = parse.find(delimiter);
    while (end != std::string::npos) {
        split_result.push_back(parse.substr(start, end - start));
        start = end + delimiter.length();
        end = parse.find(delimiter, start);
    }

    const std::string last = parse.substr(start, end);
    if (!last.empty()) {
        split_result.push_back(last);
    }

    return split_result;
}

static std::string AddMissingSpaces(const std::string& value) {
    std::string result;

    char prev_value = 0;
    char next_value = 0;
    bool found_open = false;
    for (std::size_t i = 0, n = value.size(); i < n; ++i) {
        next_value = i < n - 1 ? value[i + 1] : 0;
        if (!found_open && value[i] == '\"') {
            if (prev_value != ' ' && (i > 0)) {
                result += ' ';
            }
            result += value[i];
            found_open = true;
        } else if (found_open && value[i] == '\"') {
            result += value[i];
            if (next_value != ' ' && (i != n - 1)) {
                result += ' ';
            }
            found_open = false;
        } else {
            result += value[i];
        }
        prev_value = value[i];
    }

    return result;
}

std::vector<std::string> SplitSpace(const std::string& value) {
    const std::string& standardized_value = AddMissingSpaces(value);

    std::vector<std::string> split_result = TrimEmpty(Split(standardized_value, " "));

    std::vector<std::string> merge_result;

    bool found_open = false;
    bool require_push_back = false;
    for (std::size_t i = 0, n = split_result.size(); i < n; ++i) {
        const std::string& split_value = split_result[i];

        const std::string& trim_value = TrimString(split_value, "\"");

        if (split_value == "\"") {
            if (!found_open) {
                require_push_back = true;  // new element
            }
            found_open = !found_open;
        } else if (split_value.front() == '\"' && split_value.back() == '\"') {
            merge_result.push_back(trim_value);
        } else if (split_value.front() == '\"') {
            merge_result.push_back(trim_value);
            found_open = true;
        } else if (split_value.back() == '\"') {
            merge_result.back() += " " + trim_value;
            found_open = false;
        } else {
            if (found_open && !merge_result.empty() && !require_push_back) {
                merge_result.back() += " " + trim_value;
            } else {
                merge_result.push_back(trim_value);
                require_push_back = false;
            }
        }
    }

    std::vector<std::string> trim_result;
    trim_result.reserve(merge_result.size());

    for (std::size_t i = 0, n = merge_result.size(); i < n; ++i) {
        trim_result.push_back(TrimString(merge_result[i], "\""));
    }

    return trim_result;
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
        string_list.append(TrimSurroundingWhitespace(strings[i]).c_str());
    }

    return string_list;
}

std::vector<std::string> UniqueStrings(const std::vector<std::string>& strings) {
    std::set<std::string> uniques(strings.begin(), strings.end());
    std::vector<std::string> results(uniques.begin(), uniques.end());
    return results;
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

static const size_t NOT_FOUND = static_cast<size_t>(-1);

std::size_t ExtractDuplicateNumber(const std::string& name) {
    const std::size_t name_open = name.find_last_of("(");
    if (name_open == NOT_FOUND) {
        return NOT_FOUND;
    }

    const std::size_t name_close = name.find_last_of(")");
    if (name_close == NOT_FOUND) {
        return NOT_FOUND;
    }

    const std::string number = name.substr(name_open + 1, name_close - (name_open + 1));
    if (!IsNumber(number)) {
        return NOT_FOUND;
    }

    return std::stoi(number);
}

std::string ExtractDuplicateBaseName(const std::string& name) {
    assert(ExtractDuplicateNumber(name) != NOT_FOUND);
    const std::size_t found = name.find_last_of("(");
    assert(found != NOT_FOUND);
    return name.substr(0, found - 1);
}
