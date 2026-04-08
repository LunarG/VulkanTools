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

#pragma once

enum LayersDisplayMode {  // Enum value can't be changed
    LAYERS_DISPLAY_ALL = 0,
    LAYERS_DISPLAY_EXPLICIT_ONLY,
    LAYERS_DISPLAY_ENABLED_ONLY,

    LAYERS_DISPLAY_FIRST = LAYERS_DISPLAY_ALL,
    LAYERS_DISPLAY_LAST = LAYERS_DISPLAY_ENABLED_ONLY
};

enum { LAYERS_DISPLAY_COUNT = LAYERS_DISPLAY_LAST - LAYERS_DISPLAY_FIRST + 1 };

const char* GetToken(LayersDisplayMode mode);

const char* GetLabel(LayersDisplayMode mode);

LayersDisplayMode GetLayersDisplayMode(const char* token);