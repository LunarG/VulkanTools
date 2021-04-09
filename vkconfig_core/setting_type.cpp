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

#include "setting_type.h"
#include "util.h"
#include "version.h"

#include <cassert>

SettingType GetSettingType(const char* token) {
    assert(token != nullptr);
    assert(std::strcmp(token, "") != 0);

    if (SUPPORT_LAYER_CONFIG_2_1_0 && ToUpperCase(token) == "MULTI_ENUM") return SETTING_FLAGS;
    if (SUPPORT_LAYER_CONFIG_2_2_0 && ToUpperCase(token) == "VUID_EXCLUDE") return SETTING_LIST;
    if (SUPPORT_LAYER_CONFIG_2_2_0 && ToUpperCase(token) == "INT_RANGES") return SETTING_FRAMES;

    for (int i = SETTING_FIRST; i <= SETTING_LAST; ++i) {
        const SettingType type = static_cast<SettingType>(i);
        if (ToUpperCase(token) == GetSettingTypeToken(type)) return type;
    }

    assert(0);  // Unknown token
    return static_cast<SettingType>(-1);
}

const char* GetSettingTypeToken(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    static const char* table[] = {
        "STRING",        // SETTING_STRING
        "INT",           // SETTING_INT
        "FLOAT",         // SETTING_FLOAT
        "GROUP",         // SETTING_GROUP
        "SAVE_FILE",     // SETTING_SAVE_FILE
        "LOAD_FILE",     // SETTING_LOAD_FILE
        "SAVE_FOLDER",   // SETTING_SAVE_FOLDER
        "BOOL",          // SETTING_BOOL
        "BOOL_NUMERIC",  // SETTING_BOOL_NUMERIC_DEPRECATED
        "ENUM",          // SETTING_ENUM
        "FLAGS",         // SETTING_FLAGS
        "FRAMES",        // SETTING_FRAMES
        "LIST"           // SETTING_LIST
    };
    static_assert(countof(table) == SETTING_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[type];
}

SettingView GetSettingView(const char* token) {
    assert(token != nullptr);
    assert(std::strcmp(token, "") != 0);

    for (int i = SETTING_VIEW_FIRST; i <= SETTING_VIEW_LAST; ++i) {
        const SettingView state = static_cast<SettingView>(i);
        if (ToUpperCase(token) == GetSettingViewToken(state)) return state;
    }

    assert(0);  // Unknown token
    return static_cast<SettingView>(-1);
}

const char* GetSettingViewToken(SettingView state) {
    assert(state >= SETTING_VIEW_FIRST && state <= SETTING_VIEW_LAST);

    static const char* table[] = {
        "STANDARD",  // SETTING_VIEW_STANDARD
        "ADVANCED",  // SETTING_VIEW_ADVANCED
        "HIDDEN",    // SETTING_VIEW_HIDDEN
    };
    static_assert(countof(table) == SETTING_VIEW_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[state];
}
