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

#include "setting_type.h"
#include "util.h"

#include <cassert>

SettingType GetSettingType(const char* token) {
    for (int i = SETTING_FIRST; i <= SETTING_LAST; ++i) {
        const SettingType type = static_cast<SettingType>(i);
        if (std::strcmp(token, GetSettingToken(type)) == 0) return type;
    }

    assert(0);  // Unknown token
    return static_cast<SettingType>(-1);
}

const char* GetSettingToken(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    static const char* table[] = {
        "string",        // SETTING_STRING
        "int",           // SETTING_INT
        "save_file",     // SETTING_SAVE_FILE
        "load_file",     // SETTING_LOAD_FILE
        "save_folder",   // SETTING_SAVE_FOLDER
        "bool",          // SETTING_BOOL
        "bool_numeric",  // SETTING_BOOL_NUMERIC
        "enum",          // SETTING_EXCLUSIVE_LIST
        "multi_enum",    // SETTING_INCLUSIVE_LIST
        "range",         // SETTING_RANGE_INT
        "vuid_exclude"   // SETTING_VUID_FILTER
    };
    static_assert(countof(table) == SETTING_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[type];
}
