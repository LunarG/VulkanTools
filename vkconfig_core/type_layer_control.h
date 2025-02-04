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

#pragma once

enum LayerControl {
    LAYER_CONTROL_AUTO = 0,
    LAYER_CONTROL_DISCARD,
    LAYER_CONTROL_ON,
    LAYER_CONTROL_OFF,

    LAYER_CONTROL_FIRST = LAYER_CONTROL_AUTO,
    LAYER_CONTROL_LAST = LAYER_CONTROL_OFF,

    LAYER_CONTROL_UNORDERED_FIRST = LAYER_CONTROL_AUTO,
    LAYER_CONTROL_UNORDERED_LAST = LAYER_CONTROL_DISCARD,
};

enum {
    LAYER_CONTROL_COUNT = LAYER_CONTROL_LAST - LAYER_CONTROL_FIRST + 1,
    LAYER_CONTROL_UNORDERED_COUNT = LAYER_CONTROL_UNORDERED_LAST - LAYER_CONTROL_UNORDERED_FIRST + 1
};

LayerControl MapToUnordered(LayerControl control);
LayerControl GetLayerControl(const char* token);
const char* GetToken(LayerControl control);
const char* GetLabel(LayerControl control);
const char* GetDescription(LayerControl control);
