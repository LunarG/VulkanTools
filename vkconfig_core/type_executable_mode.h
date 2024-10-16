/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

enum ExecutableMode {  // Enum value can't be changed
    EXECUTABLE_MODE_ALL = 0,
    EXECUTABLE_MODE_PER,

    EXECUTABLE_MODE_FIRST = EXECUTABLE_MODE_ALL,
    EXECUTABLE_MODE_LAST = EXECUTABLE_MODE_PER
};

enum { EXECUTABLE_MODE_COUNT = EXECUTABLE_MODE_LAST - EXECUTABLE_MODE_FIRST + 1 };

const char* GetToken(ExecutableMode mode);

ExecutableMode GetExecutableMode(const char* token);
