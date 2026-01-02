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

#include "type_generate_settings.h"

#include <cstring>

const char* GetLabel(GenerateSettingsMode mode) {
    static const char* TABLE[] = {
        "N/A",                                          // GENERATE_SETTINGS_NONE
        "HTML documentation",                           // GENERATE_SETTINGS_HTML
        "Markdown documentation",                       // GENERATE_SETTINGS_MARKDOWN
        "vk_layer_settings.txt",                        // GENERATE_SETTINGS_TXT
        "Environment Variables bash script",            // GENERATE_SETTINGS_BASH
        "Environment Variables command prompt script",  // GENERATE_SETTINGS_CMD
        "VK_EXT_layer_settings C++ code",               // GENERATE_SETTINGS_HPP
    };
    static_assert(std::size(TABLE) == GENERATE_SETTINGS_COUNT);

    return TABLE[mode - GENERATE_SETTINGS_FIRST];
}

const char* GetDefaultFileExt(GenerateSettingsMode mode) {
    static const char* TABLE[] = {
        "N/A",    // GENERATE_SETTINGS_NONE
        ".html",  // GENERATE_SETTINGS_HTML
        ".md",    // GENERATE_SETTINGS_MARKDOWN
        ".txt",   // GENERATE_SETTINGS_TXT
        ".sh",    // GENERATE_SETTINGS_BASH
        ".bat",   // GENERATE_SETTINGS_CMD
        ".hpp",   // GENERATE_SETTINGS_HPP
    };
    static_assert(std::size(TABLE) == GENERATE_SETTINGS_COUNT);

    return TABLE[mode - GENERATE_SETTINGS_FIRST];
}

std::string GetDefaultFilename(GenerateSettingsMode mode) {
    static const char* TABLE[] = {
        "N/A",                    // GENERATE_SETTINGS_NONE
        "vk_layer_settings",      // GENERATE_SETTINGS_HTML
        "vk_layer_settings",      // GENERATE_SETTINGS_MARKDOWN
        "vk_layer_settings",      // GENERATE_SETTINGS_TXT
        "vk_layer_settings",      // GENERATE_SETTINGS_BASH
        "vk_layer_settings",      // GENERATE_SETTINGS_CMD
        "vulkan_layer_settings",  // GENERATE_SETTINGS_HPP
    };
    static_assert(std::size(TABLE) == GENERATE_SETTINGS_COUNT);

    return std::string(TABLE[mode - GENERATE_SETTINGS_FIRST]) + GetDefaultFileExt(mode);
}

const char* GetToken(GenerateSettingsMode mode) {
    static const char* TABLE[] = {
        "N/A",       // GENERATE_SETTINGS_NONE
        "html",      // GENERATE_SETTINGS_HTML
        "markdown",  // GENERATE_SETTINGS_MARKDOWN
        "txt",       // GENERATE_SETTINGS_TXT
        "bash",      // GENERATE_SETTINGS_BASH
        "bat",       // GENERATE_SETTINGS_CMD
        "hpp",       // GENERATE_SETTINGS_HPP
    };
    static_assert(std::size(TABLE) == GENERATE_SETTINGS_COUNT);

    return TABLE[mode - GENERATE_SETTINGS_FIRST];
}

GenerateSettingsMode GetGenerateSettingsMode(const char* token) {
    for (int i = GENERATE_SETTINGS_FIRST, n = GENERATE_SETTINGS_COUNT; i < n; ++i) {
        const GenerateSettingsMode mode = static_cast<GenerateSettingsMode>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return GENERATE_SETTINGS_NONE;
}