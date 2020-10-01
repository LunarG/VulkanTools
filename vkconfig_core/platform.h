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

#ifdef _WIN32
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 0
#define PLATFORM_MACOS 0

#elif defined(__linux__)
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_MACOS 0

#elif defined(__APPLE__)
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 0
#define PLATFORM_MACOS 1

#else
#error "Unknown platform"
#endif

#define HAS_SHADER_BASED (PLATFORM_WINDOWS || PLATFORM_LINUX)
#define HAS_GFXRECONSTRUCT (PLATFORM_WINDOWS || PLATFORM_LINUX)
