/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

enum DiagnosticMode {  // Enum value can't be changed
    DIAGNOSTIC_VULKAN_STATUS = 0,
    DIAGNOSTIC_VULKAN_INFO_SUMMARY,
    DIAGNOSTIC_VULKAN_INFO_TEXT,
    DIAGNOSTIC_VULKAN_PROFILE,
    DIAGNOSTIC_VULKAN_LOADER_LOG,
    DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION,
    DIAGNOSTIC_VULKAN_LAYERS_SETTINGS,
    DIAGNOSTIC_VULKAN_LOADER_SETTINGS,

    DIAGNOSTIC_MODE_FIRST = DIAGNOSTIC_VULKAN_STATUS,
    DIAGNOSTIC_MODE_LAST = DIAGNOSTIC_VULKAN_LOADER_SETTINGS
};

enum { DIAGNOSTIC_MODE_COUNT = DIAGNOSTIC_MODE_LAST - DIAGNOSTIC_MODE_FIRST + 1 };

const char* GetToken(DiagnosticMode mode);

DiagnosticMode GetDiagnosticMode(const char* token);

const char* GetFilename(DiagnosticMode mode);
