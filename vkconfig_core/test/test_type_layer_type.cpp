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

#include "../type_layer_type.h"

#include <gtest/gtest.h>

TEST(test_type_layer_type, convert_all) {
    for (int i = LAYER_TYPE_FIRST, n = LAYER_TYPE_COUNT; i < n; ++i) {
        const LayerType value = static_cast<LayerType>(i);
        const char* string = GetToken(value);
        EXPECT_EQ(value, GetLayerType(string));
    }
}