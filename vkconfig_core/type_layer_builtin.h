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

enum LayerBuiltin {
    LAYER_BUILTIN_NONE = 0,
    LAYER_BUILTIN_API,
    LAYER_BUILTIN_ENV,

    LAYER_BUILTIN_FIRST = LAYER_BUILTIN_NONE,
    LAYER_BUILTIN_LAST = LAYER_BUILTIN_ENV,
};

enum { LAYER_BUILTIN_COUNT = LAYER_BUILTIN_LAST - LAYER_BUILTIN_FIRST + 1 };

LayerBuiltin GetLayerBuiltin(const char* token);
const char* GetToken(LayerBuiltin builtin);
const char* GetLabel(LayerBuiltin builtin);
const char* GetDescription(LayerBuiltin builtin);
