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

#include "type_executable_all_enabled_behavior.h"

#include <array>
#include <cstring>

const char* GetToken(ExecutableAllEnabledBehavior behavior) {
    static const char* TABLE[]{
        "GLOBAL",       // EXECUTABLE_ALL_ENABLED_GLOBAL
        "WORKING_DIR",  // EXECUTABLE_ALL_ENABLED_WORKING_DIR
    };
    static_assert(std::size(TABLE) == EXECUTABLE_ALL_ENABLED_COUNT);

    return TABLE[behavior - EXECUTABLE_ALL_ENABLED_FIRST];
}

ExecutableAllEnabledBehavior GetExecutableAllEnabledBehavior(const char* token) {
    for (int i = EXECUTABLE_ALL_ENABLED_FIRST, n = EXECUTABLE_ALL_ENABLED_LAST; i <= n; ++i) {
        const ExecutableAllEnabledBehavior behavior = static_cast<ExecutableAllEnabledBehavior>(i);
        if (std::strcmp(::GetToken(behavior), token) == 0) {
            return behavior;
        }
    }

    return EXECUTABLE_ALL_ENABLED_GLOBAL;
}
