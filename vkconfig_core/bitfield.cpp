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

#include "bitfield.h"

std::size_t PopCount(int bitfield) {
    std::size_t count = 0;

    for (std::size_t i = 0, n = sizeof(bitfield) * 8; i < n; ++i) {
        if (bitfield & (1 << i)) {
            ++count;
        }
    }

    return count;
}

int FindPosition(int bit) {
    assert(PopCount(bit) == 1);

    for (int i = 0, n = sizeof(bit) * 8; i < n; ++i) {
        if (bit & (1 << i)) {
            return i;
        }
    }

    return -1;
}
