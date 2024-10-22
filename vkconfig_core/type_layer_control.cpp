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
    static const char* TOKENS[] = {
        "Auto",                        // LAYER_CONTROL_AUTO
        "Off",                         // LAYER_CONTROL_OFF
        "On",                          // LAYER_CONTROL_ON
        "application_enabled_layers",  // LAYER_CONTROL_APPLICATIONS_API
        "unordered_layer_location"     // LAYER_CONTROL_APPLICATIONS_ENV
    };
    static_assert(std::size(TOKENS) == LAYER_CONTROL_COUNT);

    return TOKENS[control];
}

const char* GetLabel(LayerControl control) {
    static const char* TOKENS[] = {
        "N/A",                                                      // LAYER_CONTROL_AUTO
        "N/A",                                                      // LAYER_CONTROL_OFF
        "N/A",                                                      // LAYER_CONTROL_ON
        "Vulkan Layers from the Application Vulkan API",            // LAYER_CONTROL_APPLICATIONS_API
        "Vulkan Layers from the Application Environment Variables"  // LAYER_CONTROL_APPLICATIONS_ENV
    };
    static_assert(std::size(TOKENS) == LAYER_CONTROL_COUNT);

    return TOKENS[control];
}

const char* GetDescription(LayerControl control) {
    static const char*
        TOKENS[] =
            {
                "Auto, let Vulkan applications or environment variables to enable or disable the layer",  // LAYER_CONTROL_AUTO
                "Force Off the layer, preventing its execution",                                          // LAYER_CONTROL_OFF
                "Force On the layer, insuring its execution",                                             // LAYER_CONTROL_ON
                "Located and Enabled Layers using 'vkCreateInstance' by the Vulkan Application at launch",  // LAYER_CONTROL_APPLICATIONS_API
                "Located and Enabled Layers using 'VK_LOADER_LAYERS_ENABLE' Environment Variable by the Vulkan Application at "
                "launch"  // LAYER_CONTROL_APPLICATIONS_ENV
            };
    static_assert(std::size(TOKENS) == LAYER_CONTROL_COUNT);

    return TOKENS[control];
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

bool IsVisibleLayer(LayerControl control) {
    return control >= LAYER_CONTROL_EXPLICIT_FIRST && control <= LAYER_CONTROL_EXPLICIT_LAST;
}
