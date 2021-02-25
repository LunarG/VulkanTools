/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "layer_state.h"

#include "util.h"

#include <cstring>
#include <cassert>

const char* GetToken(LayerState state) {
    static const char* table[] = {
        "APPLICATION_CONTROLLED",  // LAYER_STATE_APPLICATION_CONTROLLED
        "OVERRIDDEN",              // LAYER_STATE_OVERRIDDEN
        "EXCLUDED"                 // LAYER_STATE_EXCLUDED
    };
    static_assert(countof(table) == LAYER_STATE_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[state];
}

LayerState GetLayerState(const char* token) {
    for (std::size_t i = 0, n = LAYER_STATE_COUNT; i < n; ++i) {
        const LayerState layer_state = static_cast<LayerState>(i);
        if (std::strcmp(GetToken(layer_state), token) == 0) return layer_state;
    }

    assert(0);
    return static_cast<LayerState>(-1);
}
