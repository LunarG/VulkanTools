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

enum LayersView {
    LAYERS_VIEW_OVERRIDDEN_ONLY = 0,
    LAYERS_VIEW_ALL_AVAILABLE,

    LAYERS_PATH_VIEW_FIRST = LAYERS_VIEW_OVERRIDDEN_ONLY,
    LAYERS_PATH_VIEW_LAST = LAYERS_VIEW_ALL_AVAILABLE
};

enum { LAYERS_PATH_VIEW_COUNT = LAYERS_PATH_VIEW_LAST - LAYERS_PATH_VIEW_FIRST + 1 };

const char* GetToken(LayersView type);

LayersView GetLayersView(const char* token);
