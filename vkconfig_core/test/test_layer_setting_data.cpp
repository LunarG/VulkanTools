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

#include "../layer_setting_data.h"

#include <gtest/gtest.h>

#include <vector>

TEST(test_layer_setting_data, has_preset) {
    std::vector<LayerSettingData> preset_settings;
    std::vector<LayerSettingData> layer_settings;

    EXPECT_EQ(false, HasPreset(layer_settings, preset_settings));

    preset_settings.push_back(LayerSettingData("KeyA", "ValueA"));
    EXPECT_EQ(false, HasPreset(layer_settings, preset_settings));

    layer_settings.push_back(LayerSettingData("KeyA", "ValueA"));
    EXPECT_EQ(true, HasPreset(layer_settings, preset_settings));

    layer_settings.push_back(LayerSettingData("KeyB", "ValueB"));
    EXPECT_EQ(true, HasPreset(layer_settings, preset_settings));

    preset_settings.push_back(LayerSettingData("KeyC", "ValueC"));
    EXPECT_EQ(false, HasPreset(layer_settings, preset_settings));
}
