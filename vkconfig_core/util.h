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

#pragma once

#if defined(_WIN32) && defined(_DEBUG)
#include <windows.h>  // For OutputDebugString
#endif

#include <QStringList>

#include <cstddef>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
#include <array>

enum ParseSource { PARSE_SETTING = 0, PARSE_ENV_VAR };

const char* GetToken(ParseSource type);

std::string format(const char* message, ...);

std::string FormatNvidia(uint32_t driverVersion);

std::string FormatIntelWindows(uint32_t driverVersion);

bool IsFrames(const std::string& s);

bool IsIndex(const std::string& s);

bool IsNumber(const std::string& s);

bool IsFloat(const std::string& s);

std::size_t CountChar(const std::string& value, char c);

// Remove a value if it's present
void RemoveString(std::vector<std::string>& list, const std::string& value);

// Add a value with no duplicate
void AppendString(std::vector<std::string>& list, const std::string& value);

bool IsStringFound(const std::vector<std::string>& list, const std::string& value);

std::string TrimString(const std::string& str, const std::string& whitespaces = " \t");

std::string TrimSurroundingWhitespace(const std::string& str, const std::string& whitespaces = " \"\t\n\r");

std::vector<std::string> Split(const std::string& value, const std::string& delimiter);

std::vector<std::string> SplitSpace(const std::string& value);

std::string Merge(const std::vector<std::string>& value, const std::string& delimiter);

std::vector<std::string> ConvertString(const QStringList& string_list);

QStringList ConvertString(const std::vector<std::string>& strings);

std::vector<std::string> UniqueStrings(const std::vector<std::string>& strings);

std::string ToLowerCase(const std::string& value);

std::string ToUpperCase(const std::string& value);

inline bool EndsWith(std::string const& value, std::string const& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

struct NumberOrString {
    NumberOrString() : number(0) {}
    NumberOrString(int value) : number(value) {}
    NumberOrString(const std::string& value)
        : key(IsNumber(value) ? "" : value), number(IsNumber(value) ? std::atoi(value.c_str()) : 0) {}

    std::string key;
    int number;
};

struct EnabledNumberOrString : public NumberOrString {
    EnabledNumberOrString() : enabled(true) {}
    EnabledNumberOrString(const std::string& value) : NumberOrString(value), enabled(true) {}
    EnabledNumberOrString(const NumberOrString& value) : NumberOrString(value), enabled(true) {}

    bool enabled;
};

inline bool operator==(const NumberOrString& a, const NumberOrString& b) { return a.key == b.key && a.number == b.number; }

inline bool operator!=(const NumberOrString& a, const NumberOrString& b) { return !(a == b); }

inline bool operator<(const NumberOrString& a, const NumberOrString& b) {
    if (a.key.empty() && b.key.empty())
        return a.number < b.number;
    else if (a.key.empty() && !b.key.empty())
        return true;
    else if (!a.key.empty() && b.key.empty())
        return false;
    return a.key < b.key;
}

// Remove a value if it's present
void RemoveValue(std::vector<NumberOrString>& list, const NumberOrString& value);

void RemoveValue(std::vector<EnabledNumberOrString>& list, const EnabledNumberOrString& value);

// Add a value with no duplicate
void AppendValue(std::vector<NumberOrString>& list, const NumberOrString& value);

bool IsValueFound(const std::vector<NumberOrString>& list, const NumberOrString& value);

bool IsValueFound(const std::vector<EnabledNumberOrString>& list, const NumberOrString& value);

QStringList ConvertValues(const std::vector<NumberOrString>& values);

std::string GetLayerSettingPrefix(const std::string& key);

template <typename T>
std::vector<T> GetVector(const T& value) {
    std::vector<T> result;
    result.push_back(value);
    return result;
}

std::size_t ExtractDuplicateNumber(const std::string& name);

std::string ExtractDuplicateBaseName(const std::string& name);
