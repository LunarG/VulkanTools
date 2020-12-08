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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../layer_preset.h"
#include "../util.h"

#include <gtest/gtest.h>

TEST(test_layer_preset, get_preset) {
    LayerPreset layer_preset_a;
    layer_preset_a.preset_index = 1;

    LayerPreset layer_preset_b;
    layer_preset_b.preset_index = 2;

    std::vector<LayerPreset> presets;
    presets.push_back(layer_preset_a);
    presets.push_back(layer_preset_b);

    EXPECT_EQ(1, GetPreset(presets, 1)->preset_index);
    EXPECT_EQ(nullptr, GetPreset(presets, 3));
}
