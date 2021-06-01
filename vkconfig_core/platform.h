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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <vector>
#include <string>

#define VKC_PLATFORM_WINDOWS 0
#define VKC_PLATFORM_LINUX 1
#define VKC_PLATFORM_MACOS 2
#define VKC_PLATFORM_ANDROID 3
#define VKC_PLATFORM_IOS 4

#define VKC_ENV_WIN32 0
#define VKC_ENV_UNIX 1

enum EnviromentType {
    ENVIRONMENT_WIN32 = VKC_ENV_WIN32,
    ENVIRONMENT_UNIX = VKC_ENV_UNIX,

    ENVIRONMENT_FIRST = ENVIRONMENT_WIN32,
    ENVIRONMENT_LAST = ENVIRONMENT_UNIX,
};

enum { ENVIRONMENT_COUNT = ENVIRONMENT_LAST - ENVIRONMENT_FIRST + 1 };

enum PlatformType {
    PLATFORM_WINDOWS = VKC_PLATFORM_WINDOWS,
    PLATFORM_LINUX = VKC_PLATFORM_LINUX,
    PLATFORM_MACOS = VKC_PLATFORM_MACOS,
    PLATFORM_ANDROID = VKC_PLATFORM_ANDROID,

    PLATFORM_FIRST = PLATFORM_WINDOWS,
    PLATFORM_LAST = PLATFORM_ANDROID
};

enum { PLATFORM_COUNT = PLATFORM_LAST - PLATFORM_FIRST + 1 };

enum PlatformFlags {
    PLATFORM_WINDOWS_BIT = (1 << PLATFORM_WINDOWS),
    PLATFORM_LINUX_BIT = (1 << PLATFORM_LINUX),
    PLATFORM_MACOS_BIT = (1 << PLATFORM_MACOS),
    PLATFORM_ANDROID_BIT = (1 << PLATFORM_ANDROID),

    PLATFORM_DESKTOP_BIT = PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT
};

int GetPlatformFlags(const std::vector<std::string>& platform_strings);
std::vector<std::string> GetPlatformTokens(int platform_flags);

PlatformType GetPlatformType(const char* token);
const char* GetToken(PlatformType type);

enum StatusType {
    STATUS_STABLE = 0,
    STATUS_BETA,
    STATUS_ALPHA,
    STATUS_DEPRECATED,

    STATUS_FIRST = STATUS_STABLE,
    STATUS_LAST = STATUS_DEPRECATED
};

enum { STATUS_COUNT = STATUS_LAST - STATUS_FIRST + 1 };

const char* GetToken(StatusType type);
StatusType GetStatusType(const char* token);

#ifdef _WIN32
#define VKC_PLATFORM VKC_PLATFORM_WINDOWS
#define VKC_ENV VKC_ENV_WIN32

#elif defined(__linux__)
#define VKC_PLATFORM VKC_PLATFORM_LINUX
#define VKC_ENV VKC_ENV_UNIX

#elif defined(__APPLE__)

#define VKC_PLATFORM VKC_PLATFORM_MACOS
#define VKC_ENV VKC_ENV_UNIX

#else
#error "Unknown platform"
#endif

bool IsPlatformSupported(int platform_flags);
