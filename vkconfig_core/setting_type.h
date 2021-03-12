/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include <cassert>

enum SettingType {  // Enum value can't be changed
    SETTING_STRING = 0,
    SETTING_INT,
    SETTING_SAVE_FILE,
    SETTING_LOAD_FILE,
    SETTING_SAVE_FOLDER,
    SETTING_BOOL,
    SETTING_BOOL_NUMERIC_DEPRECATED,  // Deprecated
    SETTING_ENUM,
    SETTING_FLAGS,
    SETTING_FRAMES,
    SETTING_LIST,

    SETTING_FIRST = SETTING_STRING,
    SETTING_LAST = SETTING_LIST
};

enum { SETTING_COUNT = SETTING_LAST - SETTING_FIRST + 1 };

SettingType GetSettingType(const char* token);
const char* GetSettingToken(SettingType type);

inline bool IsEnum(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    return type == SETTING_ENUM || type == SETTING_FLAGS;
}

inline bool IsPath(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    return type == SETTING_SAVE_FILE || type == SETTING_LOAD_FILE || type == SETTING_SAVE_FOLDER;
}
