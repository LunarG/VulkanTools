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

#include "type_layer_type.h"

enum LayersPaths {
    LAYERS_PATHS_IMPLICIT_SYSTEM = 0,
    LAYERS_PATHS_IMPLICIT_ENV_SET,  // From VK_IMPLICIT_LAYER_PATH
    LAYERS_PATHS_IMPLICIT_ENV_ADD,  // from VK_ADD_IMPLICIT_LAYER_PATH
    LAYERS_PATHS_EXPLICIT_SYSTEM,
    LAYERS_PATHS_EXPLICIT_ENV_SET,  // From $VK_LAYER_PATH
    LAYERS_PATHS_EXPLICIT_ENV_ADD,  // from $VK_ADD_LAYER_PATH
    LAYERS_PATHS_GUI,
    LAYERS_PATHS_SDK,

    LAYERS_PATHS_FIRST = LAYERS_PATHS_IMPLICIT_SYSTEM,
    LAYERS_PATHS_LAST = LAYERS_PATHS_SDK,
};

enum { LAYERS_PATHS_COUNT = LAYERS_PATHS_LAST - LAYERS_PATHS_FIRST + 1 };

const char* GetLabel(LayersPaths Layers_paths_type);
