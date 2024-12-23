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

#include "../layer.h"
#include "../layer_preset.h"
#include "../setting_string.h"
#include "../setting_int.h"
#include "../util.h"

#include <gtest/gtest.h>

static std::map<Path, std::string> Dummy() { return std::map<Path, std::string>(); }

static SettingMetaString* InstantiateString(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaString*>(layer.Instantiate(layer.settings, key, SETTING_STRING));
}

TEST(test_layer_preset, has_preset) {
    Layer layer;

    SettingDataSetConst preset_settings;
    SettingDataSet layer_settings;

    SettingMetaString* metaA = ::InstantiateString(layer, "KeyA");
    SettingMetaString* metaB = ::InstantiateString(layer, "KeyB");
    SettingMetaString* metaC = ::InstantiateString(layer, "KeyC");

    EXPECT_EQ(false, ::HasPreset(layer_settings, preset_settings));

    SettingDataString* presetA = Instantiate<SettingDataString>(metaA);
    presetA->value = "ValueA";
    preset_settings.push_back(presetA);
    EXPECT_EQ(false, ::HasPreset(layer_settings, preset_settings));

    SettingDataString* layerA = Instantiate<SettingDataString>(metaA);
    layerA->value = "ValueA";
    layer_settings.push_back(layerA);
    EXPECT_EQ(true, ::HasPreset(layer_settings, preset_settings));

    SettingDataString* layerB = Instantiate<SettingDataString>(metaB);
    layerB->value = "ValueB";
    layer_settings.push_back(layerB);
    EXPECT_EQ(true, ::HasPreset(layer_settings, preset_settings));

    SettingDataString* presetC = Instantiate<SettingDataString>(metaC);
    presetA->value = "ValueC";
    preset_settings.push_back(presetC);
    EXPECT_EQ(false, ::HasPreset(layer_settings, preset_settings));
}

TEST(test_layer_preset, find_preset_index_no_preset) {
    Layer layer;
    const LayerLoadStatus load_loaded = layer.Load(":/layers/VK_LAYER_LUNARG_test_03.json", LAYER_TYPE_EXPLICIT, false, Dummy());
    EXPECT_EQ(load_loaded, LAYER_LOAD_ADDED);

    SettingDataSet layer_settings;
    ::CollectDefaultSettingData(layer.settings, layer_settings);

    int index = layer.FindPresetIndex(layer_settings);
    EXPECT_EQ(index, Layer::NO_PRESET);
}

TEST(test_layer_preset, find_preset_index_empty) {
    Layer layer;
    const LayerLoadStatus load_loaded = layer.Load(":/layers/VK_LAYER_LUNARG_test_04.json", LAYER_TYPE_EXPLICIT, false, Dummy());
    EXPECT_EQ(load_loaded, LAYER_LOAD_ADDED);

    SettingDataSet layer_settings;
    int index = layer.FindPresetIndex(layer_settings);
    EXPECT_EQ(index, Layer::NO_PRESET);
}

TEST(test_layer_preset, find_preset_index_found) {
    Layer layer;
    const LayerLoadStatus load_loaded = layer.Load(":/layers/VK_LAYER_LUNARG_test_04.json", LAYER_TYPE_EXPLICIT, false, Dummy());
    EXPECT_EQ(load_loaded, LAYER_LOAD_ADDED);

    SettingDataSet layer_settings;
    ::CollectDefaultSettingData(layer.settings, layer_settings);
    SettingDataInt& setting = static_cast<SettingDataInt&>(*layer_settings[0]);

    int indexDefault = layer.FindPresetIndex(layer_settings);
    EXPECT_EQ(indexDefault, Layer::NO_PRESET);

    setting.value = 75;
    int index75 = layer.FindPresetIndex(layer_settings);
    EXPECT_EQ(index75, 0);

    setting.value = 76;
    int index76 = layer.FindPresetIndex(layer_settings);
    EXPECT_EQ(index76, 1);

    setting.value = 99;
    int index99 = layer.FindPresetIndex(layer_settings);
    EXPECT_EQ(index99, Layer::NO_PRESET);
}

TEST(test_layer_preset, find_preset_index_missing_value) {
    Layer layer;
    const LayerLoadStatus load_loaded = layer.Load(":/layers/VK_LAYER_LUNARG_test_07.json", LAYER_TYPE_EXPLICIT, false, Dummy());
    EXPECT_EQ(load_loaded, LAYER_LOAD_ADDED);

    SettingDataSet layer_settings;
    ::CollectDefaultSettingData(layer.settings, layer_settings);

    int index = layer.FindPresetIndex(layer_settings);  // missing preset values are ignored
    EXPECT_EQ(index, 0);
}
