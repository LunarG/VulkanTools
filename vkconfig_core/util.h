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

#pragma once

#if defined(_WIN32) && defined(_DEBUG)
#include <windows.h>  // For OutputDebugString
#endif

#include <QString>

#include <cstddef>
#include <cstdio>
#include <cassert>
#include <string>
#include <array>
#include <vector>

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

bool IsNumber(const std::string& s);

// Create a directory from the home directory if it doesn't exist
void CheckHomePathsExist(const QString& path);

// Replace "$HOME" built-in variable by the actual system home directory
std::string ReplacePathBuiltInVariables(const std::string& path);

// Exact the filename and change the path to "$HOME" directory if necessary
std::string ValidatePath(const std::string& path);

template <typename T>
const T& Get(const std::vector<T>& container, const QString& key) {
    assert(key != nullptr);

    for (auto it = container.begin(), end = container.end(); it != end; ++it)
        if (it->key == key) return *it;

    assert(0);
    return container[0];
}

template <typename T>
typename std::vector<T>::iterator Find(std::vector<T>& container, const QString& name) {
    assert(!name.isEmpty());

    for (auto it = container.begin(), end = container.end(); it != end; ++it)
        if (it->name == name) return it;
    return container.end();
}

template <typename T>
typename std::vector<T>::const_iterator Find(const std::vector<T>& container, const QString& name) {
    assert(!name.isEmpty());

    for (auto it = container.begin(), end = container.end(); it != end; ++it) {
        if (it->name == name) return it;
    }

    return container.end();
}

template <typename T>
bool IsFound(const std::vector<T>& container, const QString& name) {
    assert(!name.isEmpty());

    for (auto it = container.begin(), end = container.end(); it != end; ++it) {
        if (name == it->name) return true;
    }

    return false;
}