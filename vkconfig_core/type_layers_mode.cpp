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

#include "type_layers_mode.h"

#include <array>
#include <cstring>

const char* GetToken(LayersMode mode) {
    static const char* TOKENS[] = {
        "CONTROLLED_BY_APPLICATIONS",  // LAYERS_MODE_BY_APPLICATIONS
        "CONTROLLED_BY_CONFIGURATOR",  // LAYERS_MODE_BY_CONFIGURATOR_RUNNING
        "DISABLED_BY_CONFIGURATOR"     // LAYERS_MODE_BY_CONFIGURATOR_ALL_DISABLED
    };
    static_assert(std::size(TOKENS) == LAYERS_MODE_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TOKENS[mode];
}

LayersMode GetLayersMode(const char* token) {
    for (int i = LAYERS_MODE_FIRST, l = LAYERS_MODE_LAST; i <= l; ++i) {
        const LayersMode type = static_cast<LayersMode>(i);
        if (std::strcmp(GetToken(type), token) == 0) {
            return type;
        }
    }

    return LAYERS_CONTROLLED_BY_APPLICATIONS;
}
