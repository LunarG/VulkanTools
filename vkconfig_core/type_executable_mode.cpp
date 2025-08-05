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

#include "type_executable_mode.h"

#include <array>
#include <cstring>

const char* GetLabel(ExecutableScope scope) {
    static const char* TABLE[]{
        "No Vulkan Executable",            // EXECUTABLE_NONE
        "Any Running Vulkan Executable",   // EXECUTABLE_ANY
        "All Enabled Vulkan Executables",  // EXECUTABLE_ALL
        "Per Vulkan Working Directory",    // EXECUTABLE_PER
    };

    static_assert(std::size(TABLE) == EXECUTABLE_SCOPE_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    return TABLE[scope - EXECUTABLE_SCOPE_FIRST];
}

const char* GetTooltip(ExecutableScope scope) {
    static const char* TABLE[]{
        "Vulkan Configurator won't affect the Vulkan layers for any executable running on the system.",  // EXECUTABLE_NONE
        "Apply the active layers configuration to any executable running.",                              // EXECUTABLE_ANY
        "Apply the active layers configuration to all enabled executables.",                             // EXECUTABLE_ALL
        "Apply a dedicated layers configuration for each Vulkan executable.",                            // EXECUTABLE_PER
    };

    static_assert(std::size(TABLE) == EXECUTABLE_SCOPE_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    return TABLE[scope - EXECUTABLE_SCOPE_FIRST];
}

const char* GetToken(ExecutableScope scope) {
    static const char* TABLE[]{
        "NONE",  // EXECUTABLE_NONE
        "ANY",   // EXECUTABLE_ANY
        "ALL",   // EXECUTABLE_ALL
        "PER",   // EXECUTABLE_PER
    };

    static_assert(std::size(TABLE) == EXECUTABLE_SCOPE_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    return TABLE[scope - EXECUTABLE_SCOPE_FIRST];
}

ExecutableScope GetExecutableScope(const char* token) {
    for (int i = EXECUTABLE_SCOPE_FIRST, n = EXECUTABLE_SCOPE_LAST; i <= n; ++i) {
        const ExecutableScope mode = static_cast<ExecutableScope>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return EXECUTABLE_NONE;
}

bool EnabledExecutables(ExecutableScope scope) { return scope == EXECUTABLE_ALL || scope == EXECUTABLE_PER; }
