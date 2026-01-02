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

#include "type_theme_mode.h"

#include <array>
#include <cstring>

const char* GetToken(ThemeMode mode) {
    static const char* TOKENS[] = {
        "Auto",   // THEME_MODE_USE_DEVICE
        "Light",  // THEME_MODE_FORCE_LIGHT
        "Dark",   // THEME_MODE_FORCE_DARK
    };
    static_assert(std::size(TOKENS) == THEME_MODE_COUNT);

    return TOKENS[mode];
}

ThemeMode GetThemeMode(const char* token) {
    for (int i = THEME_MODE_FIRST, l = THEME_MODE_LAST; i <= l; ++i) {
        const ThemeMode mode = static_cast<ThemeMode>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return THEME_MODE_AUTO;
}
