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

#include <string>

enum GenerateSettingsMode {
    GENERATE_SETTINGS_NONE = 0,

    GENERATE_SETTINGS_HTML,
    GENERATE_SETTINGS_MARKDOWN,
    GENERATE_SETTINGS_TXT,
    GENERATE_SETTINGS_BASH,
    GENERATE_SETTINGS_CMD,
    GENERATE_SETTINGS_HPP,

    GENERATE_SETTINGS_FIRST = GENERATE_SETTINGS_NONE,
    GENERATE_SETTINGS_LAST = GENERATE_SETTINGS_HPP,
};

enum { GENERATE_SETTINGS_COUNT = GENERATE_SETTINGS_LAST - GENERATE_SETTINGS_FIRST + 1 };

const char* GetLabel(GenerateSettingsMode mode);
const char* GetDefaultFileExt(GenerateSettingsMode mode);
std::string GetDefaultFilename(GenerateSettingsMode mode);
const char* GetToken(GenerateSettingsMode mode);
GenerateSettingsMode GetGenerateSettingsMode(const char* token);
