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

enum LayersPathViewType {
    LAYERS_PATH_ONLY_USER_DEFINED = 0,
    LAYERS_PATH_ONLY_ACTIVE_SDK,
    LAYERS_PATH_ALL,

    LAYERS_PATH_FIRST = LAYERS_PATH_ONLY_USER_DEFINED,
    LAYERS_PATH_LAST = LAYERS_PATH_ALL,
};
enum { LAYERS_PATH_COUNT = LAYERS_PATH_LAST - LAYERS_PATH_FIRST + 1 };
