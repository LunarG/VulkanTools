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

#include "type_action.h"

#include <array>
#include <cstring>

const char* GetToken(ActionType type) {
    static const char* TABLE[] = {
        "BUTTON0",  // ACTION0
        "BUTTON1"   // ACTION1
    };
    static_assert(std::size(TABLE) == ACTION_COUNT, "The tranlation table size doesn't match the enum number of elements");

    if (type >= ACTION_FIRST && type <= ACTION_LAST) {
        return TABLE[type - ACTION_FIRST];
    } else {
        return nullptr;
    }
}

ActionType GetActionType(const char* token) {
    for (std::size_t i = 0, n = ACTION_COUNT; i < n; ++i) {
        const ActionType type = static_cast<ActionType>(i);
        if (std::strcmp(::GetToken(type), token) == 0) {
            return type;
        }
    }

    return ACTION0;
}
