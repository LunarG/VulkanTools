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

#include <cstddef>
#include <cstdio>
#include <string>
#include <array>
#include <vector>

#if defined(_WIN32) && defined(_DEBUG)
#include <windows.h>  // For OutputDebugString
#endif

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
