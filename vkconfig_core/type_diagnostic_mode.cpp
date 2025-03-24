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

#include "type_diagnostic_mode.h"

#include <array>
#include <cstring>

const char* GetToken(DiagnosticMode mode) {
    static const char* TABLE[] = {
        "Vulkan Development Status",           // DIAGNOSTIC_VULKAN_STATUS
        "Vulkan Info Summary",                 // DIAGNOSTIC_VULKAN_INFO_SUMMARY
        "Vulkan Info Text",                    // DIAGNOSTIC_VULKAN_INFO_TEXT
        "Vulkan Profile",                      // DIAGNOSTIC_VULKAN_PROFILE
        "Vulkan Loader Log",                   // DIAGNOSTIC_VULKAN_LOADER_LOG
        "Vulkan Active Loader Configuration",  // DIAGNOSTIC_ACTIVE_LOADER_CONFIGURATION
        "Vulkan Loader Settings",              // DIAGNOSTIC_VULKAN_LOADER_SETTINGS
        "Vulkan Layers Settings",              // DIAGNOSTIC_VULKAN_LAYERS_SETTINGS
    };
    static_assert(std::size(TABLE) == DIAGNOSTIC_MODE_COUNT);

    return TABLE[mode];
}

DiagnosticMode GetDiagnosticMode(const char* token) {
    for (int i = DIAGNOSTIC_MODE_FIRST, l = DIAGNOSTIC_MODE_LAST; i <= l; ++i) {
        const DiagnosticMode mode = static_cast<DiagnosticMode>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return DIAGNOSTIC_VULKAN_STATUS;
}
