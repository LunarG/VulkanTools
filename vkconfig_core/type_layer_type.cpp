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

#include "type_layer_type.h"

#include <array>
#include <cstring>

const char* GetToken(LayerType type) {
    static const char* TOKENS[] = {
        "Implicit",  // LAYER_TYPE_IMPLICIT
        "Explicit",  // LAYER_TYPE_EXPLICIT
    };
    static_assert(std::size(TOKENS) == LAYER_TYPE_COUNT);

    return TOKENS[type];
}

LayerType GetLayerType(const char* token) {
    for (int i = LAYER_TYPE_FIRST, l = LAYER_TYPE_LAST; i <= l; ++i) {
        const LayerType type = static_cast<LayerType>(i);
        if (std::strcmp(::GetToken(type), token) == 0) {
            return type;
        }
    }

    return LAYER_TYPE_EXPLICIT;
}
