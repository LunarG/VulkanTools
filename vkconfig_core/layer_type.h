/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#pragma once

enum LayerType {  // Enum value can't be changed
    LAYER_TYPE_EXPLICIT = 0,
    LAYER_TYPE_IMPLICIT = 1,
    LAYER_TYPE_CUSTOM = 2,

    LAYER_TYPE_FIRST = LAYER_TYPE_EXPLICIT,
    LAYER_TYPE_LAST = LAYER_TYPE_CUSTOM
};

enum { LAYER_TYPE_COUNT = LAYER_TYPE_LAST - LAYER_TYPE_FIRST + 1 };

const char* GetLayerTypeLabel(LayerType type);
