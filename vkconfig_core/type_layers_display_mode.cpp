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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "type_layers_display_mode.h"

#include <array>
#include <cstring>

const char* GetToken(LayersDisplayMode mode) {
    static const char* TOKENS[] = {
        "all",       // LAYERS_DISPLAY_ALL
        "explicit",  // LAYERS_DISPLAY_EXPLICIT_ONLY
        "enabled",   // LAYERS_DISPLAY_ENABLED_ONLY
    };
    static_assert(std::size(TOKENS) == LAYERS_DISPLAY_COUNT);

    return TOKENS[mode];
}

const char* GetLabel(LayersDisplayMode mode) {
    static const char* TOKENS[] = {
        "Show all available layers",       // LAYERS_DISPLAY_ALL
        "Bundle default implicit layers",  // LAYERS_DISPLAY_EXPLICIT_ONLY
        "Show only enabled layers",        // LAYERS_DISPLAY_ENABLED_ONLY
    };
    static_assert(std::size(TOKENS) == LAYERS_DISPLAY_COUNT);

    return TOKENS[mode];
}

LayersDisplayMode GetLayersDisplayMode(const char* token) {
    for (int i = LAYERS_DISPLAY_FIRST, l = LAYERS_DISPLAY_LAST; i <= l; ++i) {
        const LayersDisplayMode mode = static_cast<LayersDisplayMode>(i);
        if (std::strcmp(::GetToken(mode), token) == 0) {
            return mode;
        }
    }

    return LAYERS_DISPLAY_ALL;
}
