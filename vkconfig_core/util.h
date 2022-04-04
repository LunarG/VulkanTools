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

#pragma once

#if defined(_WIN32) && defined(_DEBUG)
#define NOMINMAX
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

// Based on https://www.g-truc.net/post-0708.html#menu
template <typename T, std::size_t N>
inline constexpr std::size_t countof(T const (&)[N]) noexcept {
    return N;
}

template <typename T, std::size_t N>
inline constexpr std::size_t countof(std::array<T, N> const& data) noexcept {
    return data.size();
}

// For C++ container
template <typename T, typename Alloc, template <typename, typename> class C>
inline constexpr std::size_t countof(C<T, Alloc> const& data) noexcept {
    return data.size();
}

std::string format(const char* message, ...);

bool IsFrames(const std::string& s);

bool IsNumber(const std::string& s);

bool IsFloat(const std::string& s);

template <typename T>
T* FindByKey(std::vector<T>& container, const char* key) {
    assert(key != nullptr);
    assert(std::strcmp(key, "") != 0);

    for (std::size_t i = 0, n = container.size(); i < n; ++i) {
        if (container[i].key == key) return &container[i];
    }

    return nullptr;
}

template <typename T>
const T* FindByKey(const std::vector<T>& container, const char* key) {
    assert(key != nullptr);
    assert(std::strcmp(key, "") != 0);

    for (std::size_t i = 0, n = container.size(); i < n; ++i) {
        if (container[i].key == key) return &container[i];
    }

    return nullptr;
}

template <typename T>
bool IsFound(const std::vector<T>& container, const char* key) {
    return FindByKey(container, key) != nullptr;
}

// Remove a value if it's present
void RemoveString(std::vector<std::string>& list, const std::string& value);

// Add a value with no duplicate
void AppendString(std::vector<std::string>& list, const std::string& value);

bool IsStringFound(const std::vector<std::string>& list, const std::string& value);

std::vector<std::string> Split(const std::string& value, const std::string& delimiter);

std::string Merge(const std::vector<std::string>& value, const std::string& delimiter);

std::vector<std::string> ConvertString(const QStringList& string_list);

QStringList ConvertString(const std::vector<std::string>& strings);

std::string ToLowerCase(const std::string& value);

std::string ToUpperCase(const std::string& value);

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
