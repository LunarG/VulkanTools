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

#include "type_action_operator.h"

#include <array>
#include <cstring>

const char* GetToken(ActionOperatorType type) {
    static const char* TABLE[] = {
        "SET",     // ACTION_OPERATOR_SET
        "APPEND",  // ACTION_OPERATOR_APPEND
        "REMOVE"   // ACTION_OPERATOR_REMOVE
    };
    static_assert(std::size(TABLE) == ACTION_OPERATOR_COUNT);

    if (type >= ACTION_OPERATOR_FIRST && type <= ACTION_OPERATOR_LAST) {
        return TABLE[type - ACTION_OPERATOR_FIRST];
    } else {
        return nullptr;
    }
}

ActionOperatorType GetActionOperatorType(const char* token) {
    for (std::size_t i = 0, n = ACTION_OPERATOR_COUNT; i < n; ++i) {
        const ActionOperatorType value = static_cast<ActionOperatorType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return ACTION_OPERATOR_SET;
}
