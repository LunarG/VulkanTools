/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "../layer.h"
#include "../layer_preset.h"
#include "../setting_string.h"
#include "../util.h"

#include <gtest/gtest.h>

inline SettingMetaString* InstantiateString(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaString*>(layer.Instantiate(layer.settings, key, SETTING_STRING));
}

TEST(test_layer_preset, get_preset) {
    LayerPreset layer_preset_a;
    layer_preset_a.label = "1";

    LayerPreset layer_preset_b;
    layer_preset_b.label = "2";

    std::vector<LayerPreset> presets;
    presets.push_back(layer_preset_a);
    presets.push_back(layer_preset_b);

    EXPECT_STREQ("1", GetPreset(presets, "1")->label.c_str());
    EXPECT_EQ(nullptr, GetPreset(presets, "3"));
}

TEST(test_layer_preset, has_preset) {
    Layer layer;

    SettingDataSetConst preset_settings;
    SettingDataSet layer_settings;

    SettingMetaString* metaA = InstantiateString(layer, "KeyA");
    SettingMetaString* metaB = InstantiateString(layer, "KeyB");
    SettingMetaString* metaC = InstantiateString(layer, "KeyC");

    EXPECT_EQ(false, HasPreset(layer_settings, preset_settings));

    SettingDataString* presetA = Instantiate<SettingDataString>(metaA);
    presetA->value = "ValueA";
    preset_settings.push_back(presetA);
    EXPECT_EQ(false, HasPreset(layer_settings, preset_settings));

    SettingDataString* layerA = Instantiate<SettingDataString>(metaA);
    layerA->value = "ValueA";
    layer_settings.push_back(layerA);
    EXPECT_EQ(true, HasPreset(layer_settings, preset_settings));

    SettingDataString* layerB = Instantiate<SettingDataString>(metaB);
    layerB->value = "ValueB";
    layer_settings.push_back(layerB);
    EXPECT_EQ(true, HasPreset(layer_settings, preset_settings));

    SettingDataString* presetC = Instantiate<SettingDataString>(metaC);
    presetA->value = "ValueC";
    preset_settings.push_back(presetC);
    EXPECT_EQ(false, HasPreset(layer_settings, preset_settings));
}
