/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "type_executable_mode.h"

#include <array>
#include <cstring>

const char* GetToken(ExecutableMode mode) {
    static const char* TABLE[]{
        "Any Executables",           // EXECUTABLE_ANY
        "All-Selected Executables",  // EXECUTABLE_ALL
        "Per-Selected Executable",   // EXECUTABLE_PER
    };

    static_assert(std::size(TABLE) == EXECUTABLE_MODE_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[mode - EXECUTABLE_MODE_FIRST];
}

ExecutableMode GetExecutableMode(const char* token) {
    for (int i = EXECUTABLE_MODE_FIRST, n = EXECUTABLE_MODE_LAST; i <= n; ++i) {
        const ExecutableMode mode = static_cast<ExecutableMode>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return EXECUTABLE_MODE_FIRST;
}

const char* GetLabel(ExecutableMode mode) {
    static const char* TABLE[]{
        "Apply a layers configuration to any Vulkan Executables running",   // EXECUTABLE_ANY
        "Apply a layers configuration to all selected Vulkan Executables",  // EXECUTABLE_ALL
        "Apply a layers configuration per selected Vulkan Executable",      // EXECUTABLE_PER
    };

    static_assert(std::size(TABLE) == EXECUTABLE_MODE_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[mode - EXECUTABLE_MODE_FIRST];
}

ExecutableMode GetExecutableModeFromLabel(const char* token) {
    for (int i = EXECUTABLE_MODE_FIRST, n = EXECUTABLE_MODE_LAST; i <= n; ++i) {
        const ExecutableMode mode = static_cast<ExecutableMode>(i);
        if (std::strcmp(::GetLabel(mode), token) == 0) {
            return mode;
        }
    }

    return EXECUTABLE_MODE_FIRST;
}
