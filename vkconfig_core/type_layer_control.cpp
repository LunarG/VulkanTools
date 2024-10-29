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

#include "type_layer_control.h"
#include "util.h"

#include <array>
#include <cstring>

// These names are requied by the Vulkan Loader settings file
const char* GetToken(LayerControl control) {
    static const char* TABLE[] = {
        "auto",     // LAYER_CONTROL_AUTO
        "on",       // LAYER_CONTROL_ON
        "off",      // LAYER_CONTROL_OFF
        "discard",  // LAYER_CONTROL_DISCARD
    };
    static_assert(std::size(TABLE) == LAYER_CONTROL_COUNT);

    return TABLE[control];
}

LayerControl GetLayerControl(const char* token) {
    for (int i = LAYER_CONTROL_FIRST, l = LAYER_CONTROL_LAST; i <= l; ++i) {
        const LayerControl type = static_cast<LayerControl>(i);
        if (ToLowerCase(::GetToken(type)) == ToLowerCase(token)) {
            return type;
        }
    }

    return LAYER_CONTROL_AUTO;
}

// These names are requied by the Vulkan Loader settings file
const char* GetLabel(LayerControl control) {
    static const char* TABLE[] = {
        "Auto",     // LAYER_CONTROL_AUTO
        "Enable",   // LAYER_CONTROL_ON
        "Disable",  // LAYER_CONTROL_OFF
        "Discard",  // LAYER_CONTROL_DISCARD
    };
    static_assert(std::size(TABLE) == LAYER_CONTROL_COUNT);

    return TABLE[control];
}

const char* GetDescription(LayerControl control) {
    static const char* TOKENS[] = {
        "Explicit layers are disabled by default and implicit layers are enabled by default.",  // LAYER_CONTROL_AUTO
        "Enable the layer, insuring its execution.",                                            // LAYER_CONTROL_ON
        "Disable the layer, preventing its execution.",                                         // LAYER_CONTROL_OFF
        "Discard the layer, don't notify the Vulkan Loader this layer exist.",                  // LAYER_CONTROL_DISCARD
    };
    static_assert(std::size(TOKENS) == LAYER_CONTROL_COUNT);

    return TOKENS[control];
}
