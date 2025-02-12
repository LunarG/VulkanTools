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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <vector>
#include <string>

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
    PLATFORM_WINDOWS_X86 = 0,
    PLATFORM_WINDOWS_ARM = 1,
    PLATFORM_LINUX = 2,
    PLATFORM_MACOS = 3,
    PLATFORM_ANDROID = 4,
    PLATFORM_IOS = 5,

    PLATFORM_FIRST = PLATFORM_WINDOWS_X86,
    PLATFORM_LAST = PLATFORM_IOS,

    PLATFORM_DESKTOP_FIRST = PLATFORM_WINDOWS_X86,
    PLATFORM_DESKTOP_LAST = PLATFORM_MACOS
};

enum { PLATFORM_COUNT = PLATFORM_LAST - PLATFORM_FIRST + 1 };

enum PlatformFlags {
    PLATFORM_WINDOWS_X86_BIT = (1 << PLATFORM_WINDOWS_X86),
    PLATFORM_WINDOWS_ARM_BIT = (1 << PLATFORM_WINDOWS_ARM),
    PLATFORM_LINUX_BIT = (1 << PLATFORM_LINUX),
    PLATFORM_MACOS_BIT = (1 << PLATFORM_MACOS),
    PLATFORM_ANDROID_BIT = (1 << PLATFORM_ANDROID),
    PLATFORM_IOS_BIT = (1 << PLATFORM_IOS),

    PLATFORM_WINDOWS_BIT = PLATFORM_WINDOWS_X86_BIT | PLATFORM_WINDOWS_ARM_BIT,

    PLATFORM_DESKTOP_BIT = PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT,
    PLATFORM_MOBILE_BIT = PLATFORM_ANDROID_BIT | PLATFORM_IOS_BIT
};

int GetPlatformFlags(const std::vector<std::string>& platform_strings);
std::vector<std::string> GetPlatformTokens(int platform_flags);

#ifdef _WIN32
#ifdef _M_ARM64
#define VKC_PLATFORM PLATFORM_WINDOWS_ARM
#else
#define VKC_PLATFORM PLATFORM_WINDOWS_X86
#endif
#define VKC_ENV VKC_ENV_WIN32

#elif defined(__linux__)
#define VKC_PLATFORM PLATFORM_LINUX
#define VKC_ENV VKC_ENV_UNIX

#elif defined(__APPLE__)

#define VKC_PLATFORM PLATFORM_MACOS
#define VKC_ENV VKC_ENV_UNIX

#else
#error "Unknown platform"
#endif

bool IsDesktop(PlatformType type);

bool IsPlatformSupported(int platform_flags);

const char* GetLabel(PlatformType type);

const char* GetLatestReleaseSDK(PlatformType type);

const char* GetLatestPackageSDK(PlatformType type);

const char* GetInstallerFilename(PlatformType type);

const char* GetVersionedFilename(PlatformType type);
