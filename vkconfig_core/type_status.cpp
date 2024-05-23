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

#include "type_status.h"

#include <array>
#include <cstring>

const char* GetToken(StatusType value) {
    static const char* TOKENS[] = {
        "STABLE",     // STATUS_STABLE
        "BETA",       // STATUS_BETA
        "ALPHA",      // STATUS_ALPHA
        "DEPRECATED"  // STATUS_ALPHA
    };
    static_assert(std::size(TOKENS) == STATUS_COUNT, "The tranlation table size doesn't match the enum number of elements");

    if (value >= STATUS_FIRST && value <= STATUS_LAST) {
        return TOKENS[value - STATUS_FIRST];
    } else {
        return nullptr;
    }
}

StatusType GetStatusType(const char* token) {
    for (std::size_t i = 0, n = STATUS_COUNT; i < n; ++i) {
        const StatusType value = static_cast<StatusType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return STATUS_INVALID;
}
