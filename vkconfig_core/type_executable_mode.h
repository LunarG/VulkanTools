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

enum ExecutableScope {  // Enum value can't be changed
    EXECUTABLE_NONE = 0,
    EXECUTABLE_ANY,
    EXECUTABLE_ALL,
    EXECUTABLE_PER,

    EXECUTABLE_SCOPE_FIRST = EXECUTABLE_NONE,
    EXECUTABLE_SCOPE_LAST = EXECUTABLE_PER
};

enum { EXECUTABLE_SCOPE_COUNT = EXECUTABLE_SCOPE_LAST - EXECUTABLE_SCOPE_FIRST + 1 };

const char* GetLabel(ExecutableScope scope);
const char* GetTooltip(ExecutableScope scope);
const char* GetToken(ExecutableScope scope);

ExecutableScope GetExecutableScope(const char* token);

bool EnabledExecutables(ExecutableScope scope);
