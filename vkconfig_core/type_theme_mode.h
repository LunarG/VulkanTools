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

enum ThemeMode {
    THEME_MODE_AUTO = 0,
    THEME_MODE_FORCE_LIGHT,
    THEME_MODE_FORCE_DARK,

    THEME_MODE_FIRST = THEME_MODE_AUTO,
    THEME_MODE_LAST = THEME_MODE_FORCE_DARK,
};

enum { THEME_MODE_COUNT = THEME_MODE_LAST - THEME_MODE_FIRST + 1 };

const char* GetToken(ThemeMode mode);

ThemeMode GetThemeMode(const char* token);
