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

#include "type_view.h"
#include "util.h"

#include <cassert>

SettingView GetSettingView(const char* token) {
    assert(token != nullptr);
    assert(std::strcmp(token, "") != 0);

    for (int i = SETTING_VIEW_FIRST; i <= SETTING_VIEW_LAST; ++i) {
        const SettingView state = static_cast<SettingView>(i);
        if (ToUpperCase(token) == GetToken(state)) {
            return state;
        }
    }

    return SETTING_VIEW_STANDARD;  // Unknown token
}

const char* GetToken(SettingView state) {
    assert(state >= SETTING_VIEW_FIRST && state <= SETTING_VIEW_LAST);

    static const char* table[] = {
        "STANDARD",  // SETTING_VIEW_STANDARD
        "ADVANCED",  // SETTING_VIEW_ADVANCED
        "DEBUG",     // SETTING_VIEW_DEBUG
        "HIDDEN",    // SETTING_VIEW_HIDDEN
    };
    static_assert(std::size(table) == SETTING_VIEW_COUNT);

    return table[state];
}
