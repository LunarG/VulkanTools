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

#include "type_layers_view.h"

#include <array>
#include <cstring>

const char* GetToken(LayersView view) {
    static const char* TOKENS[]{
        "Overridden Layers Only",  // LAYERS_VIEW_OVERRIDDEN_ONLY
        "All Available LAyers",    // LAYERS_VIEW_ALL_AVAILABLE
    };

    static_assert(std::size(TOKENS) == LAYERS_PATH_VIEW_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    return TOKENS[view - LAYERS_PATH_VIEW_FIRST];
}

LayersView GetLayersView(const char* token) {
    for (int i = LAYERS_PATH_VIEW_FIRST, n = LAYERS_PATH_VIEW_LAST; i <= n; ++i) {
        const LayersView view = static_cast<LayersView>(i);
        if (std::strcmp(::GetToken(view), token) == 0) {
            return view;
        }
    }

    return LAYERS_VIEW_OVERRIDDEN_ONLY;
}
