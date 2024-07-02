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

#pragma once

enum LayerControl {
    LAYER_CONTROL_AUTO = 0,
    LAYER_CONTROL_ON,
    LAYER_CONTROL_OFF,
    LAYER_CONTROL_APPLICATIONS,
    LAYER_CONTROL_UNORDERED,

    LAYER_CONTROL_FIRST = LAYER_CONTROL_AUTO,
    LAYER_CONTROL_LAST = LAYER_CONTROL_UNORDERED
};

enum { LAYER_CONTROL_COUNT = LAYER_CONTROL_LAST - LAYER_CONTROL_FIRST + 1 };

const char* GetToken(LayerControl control);

LayerControl GetLayerControl(const char* token);
