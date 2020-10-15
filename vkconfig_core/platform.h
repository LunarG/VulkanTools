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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#define APP_SHORT_NAME "vkconfig"

#define VKC_PLATFORM_WINDOWS 0
#define VKC_PLATFORM_LINUX 1
#define VKC_PLATFORM_MACOS 2

enum PlatformType {
    PLATFORM_WINDOWS = VKC_PLATFORM_WINDOWS,
    PLATFORM_LINUX = VKC_PLATFORM_LINUX,
    PLATFORM_MACOS = VKC_PLATFORM_MACOS,

    PLATFORM_FIRST = PLATFORM_WINDOWS,
    PLATFORM_LAST = PLATFORM_MACOS
};

enum { PLATFORM_COUNT = PLATFORM_LAST - PLATFORM_FIRST + 1 };

enum PlatformFlags {
    PLATFORM_WINDOWS_BIT = (1 << PLATFORM_WINDOWS),
    PLATFORM_LINUX_BIT = (1 << PLATFORM_LINUX),
    PLATFORM_MACOS_BIT = (1 << PLATFORM_MACOS)
};

enum StatusType {
    STATUS_STABLE = 0,
    STATUS_BETA,
    STATUS_ALPHA,

    STATUS_FIRST = STATUS_STABLE,
    STATUS_LAST = STATUS_ALPHA
};

enum { STATUS_COUNT = STATUS_LAST - STATUS_FIRST + 1 };

const char* GetToken(StatusType type);
StatusType GetStatusType(const char* token);

#ifdef _WIN32
#define VKC_PLATFORM VKC_PLATFORM_WINDOWS

#elif defined(__linux__)
#define VKC_PLATFORM VKC_PLATFORM_LINUX

#elif defined(__APPLE__)

#define VKC_PLATFORM VKC_PLATFORM_MACOS

#else
#error "Unknown platform"
#endif

enum PlatformString {
    PLATFORM_STRING_OS,
    PLATFORM_STRING_APP_SUFFIX,
    PLATFORM_STRING_EXE_SUFFIX,
    PLATFORM_STRING_SEPARATOR,
    PLATFORM_STRING_FILTER,
    PLATFORM_STRING_VULKAN_INFO,
    PLATFORM_STRING_VULKAN_LIBRARY,
    PLATFORM_STRING_PATH_CONFIGURATION,
    PLATFORM_STRING_PATH_OVERRIDE_LAYERS,
    PLATFORM_STRING_PATH_OVERRIDE_SETTINGS,

    PLATFORM_STRING_FIRST = PLATFORM_STRING_OS,
    PLATFORM_STRING_LAST = PLATFORM_STRING_PATH_OVERRIDE_SETTINGS
};

enum { PLATFORM_STRING_COUNT = PLATFORM_STRING_LAST - PLATFORM_STRING_FIRST + 1 };

const char* GetPlatformString(PlatformString platform_string);
