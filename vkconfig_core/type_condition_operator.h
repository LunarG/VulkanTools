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

enum ConditionOperatorType {
    CONDITION_OPERATOR_NONE = 0,
    CONDITION_OPERATOR_NOT,

    CONDITION_OPERATOR_FIRST = CONDITION_OPERATOR_NONE,
    CONDITION_OPERATOR_LAST = CONDITION_OPERATOR_NOT,
};

enum { CONDITION_OPERATOR_COUNT = CONDITION_OPERATOR_LAST - CONDITION_OPERATOR_FIRST + 1 };

const char* GetToken(ConditionOperatorType type);

ConditionOperatorType GetConditionOperatorType(const char* token);
