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

#include "../type_layers_mode.h"

#include <gtest/gtest.h>

TEST(test_type_layers_mode, convert_all) {
    for (int i = LAYERS_MODE_FIRST, n = LAYERS_MODE_COUNT; i < n; ++i) {
        const LayersMode value = static_cast<LayersMode>(i);
        const char* string = GetToken(value);
        EXPECT_EQ(value, GetLayersMode(string));
    }
}
