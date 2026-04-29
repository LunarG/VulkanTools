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

#include "type_stdout_display.h"

#include <array>
#include <cstring>

const char* GetToken(StdoutDisplay mode) {
    static const char* TABLE[]{
        "ON_EXIT",  // STDOUT_DISPLAY_EXIT
        "STREAM",   // STDOUT_DISPLAY_STREAM
    };
    static_assert(std::size(TABLE) == STDOUT_DISPLAY_COUNT);

    return TABLE[mode - STDOUT_DISPLAY_FIRST];
}

StdoutDisplay GetStdoutDiaplay(const char* token) {
    for (int i = STDOUT_DISPLAY_FIRST, n = STDOUT_DISPLAY_LAST; i <= n; ++i) {
        const StdoutDisplay mode = static_cast<StdoutDisplay>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return STDOUT_DISPLAY_FIRST;
}
