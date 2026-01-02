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

#include "type_layer_builtin.h"
#include "util.h"

LayerBuiltin GetLayerBuiltin(const char* token) {
    for (int i = LAYER_BUILTIN_FIRST, l = LAYER_BUILTIN_LAST; i <= l; ++i) {
        const LayerBuiltin type = static_cast<LayerBuiltin>(i);
        if (ToLowerCase(::GetToken(type)) == ToLowerCase(token)) {
            return type;
        }
    }

    return LAYER_BUILTIN_NONE;
}

const char* GetToken(LayerBuiltin builtin) {
    static const char* TABLE[] = {
        "N/A",                       // LAYER_BUILTIN_NONE
        "unordered_layer_location",  // LAYER_BUILTIN_UNORDERED
    };
    static_assert(std::size(TABLE) == LAYER_BUILTIN_COUNT);

    return TABLE[builtin];
}

const char* GetLabel(LayerBuiltin builtin) {
    static const char* TABLE[] = {
        "N/A",                                              // LAYER_BUILTIN_NONE
        "Vulkan Layers Located by the Vulkan Application",  // LAYER_BUILTIN_UNORDERED
    };
    static_assert(std::size(TABLE) == LAYER_BUILTIN_COUNT);

    return TABLE[builtin];
}

const char* GetDescription(LayerBuiltin builtin) {
    static const char* TABLE[] = {
        "N/A",  // LAYER_BUILTIN_NONE
        "Vulkan Layers are located by the Vulkan Application by setting 'VK_ADD_LAYER_PATH' Application at launch",  // LAYER_BUILTIN_UNORDERED
    };
    static_assert(std::size(TABLE) == LAYER_BUILTIN_COUNT);

    return TABLE[builtin];
}
