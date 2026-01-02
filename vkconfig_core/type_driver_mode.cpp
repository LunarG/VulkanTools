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

#include "type_driver_mode.h"

#include <array>
#include <cstring>

const char* GetToken(DriverMode mode) {
    static const char* TOKENS[] = {
        "Single",  // DRIVER_MODE_SINGLE
        "Sorted",  // DRIVER_MODE_SORTED
    };
    static_assert(std::size(TOKENS) == DRIVER_MODE_COUNT);

    return TOKENS[mode];
}

DriverMode GetDriverMode(const char* token) {
    for (int i = DRIVER_MODE_FIRST, l = DRIVER_MODE_LAST; i <= l; ++i) {
        const DriverMode mode = static_cast<DriverMode>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return DRIVER_MODE_FIRST;
}
