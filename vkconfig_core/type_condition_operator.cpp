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

#include "type_condition_operator.h"

#include <array>
#include <cstring>

const char* GetToken(ConditionOperatorType type) {
    static const char* TABLE[] = {
        "NONE",  // CONDITION_OPERATOR_NONE
        "NOT",   // CONDITION_OPERATOR_NOT
    };
    static_assert(std::size(TABLE) == CONDITION_OPERATOR_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    if (type >= CONDITION_OPERATOR_FIRST && type <= CONDITION_OPERATOR_LAST) {
        return TABLE[type - CONDITION_OPERATOR_FIRST];
    } else {
        return nullptr;
    }
}

ConditionOperatorType GetConditionOperatorType(const char* token) {
    for (std::size_t i = 0, n = CONDITION_OPERATOR_COUNT; i < n; ++i) {
        const ConditionOperatorType value = static_cast<ConditionOperatorType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return CONDITION_OPERATOR_NONE;
}
