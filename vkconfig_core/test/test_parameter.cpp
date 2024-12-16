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
#include "../parameter.h"
#include "../util.h"
#include "../setting_string.h"

#include <gtest/gtest.h>

inline SettingMetaString* InstantiateString(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaString*>(layer.Instantiate(layer.settings, key, SETTING_STRING));
}

TEST(test_parameter, apply_settings) {
    Layer layer;

    SettingMetaString* metaA = InstantiateString(layer, "A");
    SettingMetaString* metaB = InstantiateString(layer, "B");

    SettingDataString* preset_setting = Instantiate<SettingDataString>(metaA);
    preset_setting->value = "preset value";

    LayerPreset preset;
    preset.settings.push_back(preset_setting);

    SettingDataString* layer_setting_a = Instantiate<SettingDataString>(metaA);
    layer_setting_a->value = "setting value";

    SettingDataString* layer_setting_b = Instantiate<SettingDataString>(metaB);
    layer_setting_b->value = "setting value";

    Parameter parameter;
    parameter.settings.push_back(layer_setting_a);
    parameter.settings.push_back(layer_setting_b);

    EXPECT_EQ(1, preset.settings.size());
    EXPECT_EQ(2, parameter.settings.size());

    parameter.ApplyPresetSettings(preset);

    EXPECT_EQ(1, preset.settings.size());
    EXPECT_EQ(2, parameter.settings.size());

    EXPECT_STREQ("preset value", static_cast<SettingDataString*>(FindSetting(parameter.settings, "A"))->value.c_str());
    EXPECT_STREQ("setting value", static_cast<SettingDataString*>(FindSetting(parameter.settings, "B"))->value.c_str());
}

TEST(test_parameter, ordering_found_parameter_rank) {
    LayerManager layers;
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_validation", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_profiles", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(
        Layer("VK_LAYER_KHRONOS_timeline_semaphore", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(
        Layer("VK_LAYER_KHRONOS_synchronization2", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(
        Layer("VK_LAYER_KHRONOS_shader_object", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(
        Layer("VK_LAYER_KHRONOS_memory_decompression", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    layers.selected_layers[0].type = LAYER_TYPE_IMPLICIT;

    EXPECT_EQ(PARAMETER_RANK_VALIDATION_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_validation", LAYER_CONTROL_AUTO)));
    EXPECT_EQ(PARAMETER_RANK_PROFILES_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_profiles", LAYER_CONTROL_ON)));
    EXPECT_EQ(PARAMETER_RANK_EXTENSION_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_timeline_semaphore", LAYER_CONTROL_OFF)));
    EXPECT_EQ(PARAMETER_RANK_EXTENSION_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_synchronization2", LAYER_CONTROL_AUTO)));
    EXPECT_EQ(PARAMETER_RANK_EXTENSION_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_shader_object", LAYER_CONTROL_ON)));
    EXPECT_EQ(PARAMETER_RANK_EXTENSION_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_memory_decompression", LAYER_CONTROL_OFF)));
    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_explicit", LAYER_CONTROL_ON)));
    EXPECT_EQ(PARAMETER_RANK_IMPLICIT_LAYER,
              GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_OFF)));
    EXPECT_EQ(PARAMETER_RANK_MISSING_LAYER, GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_missing", LAYER_CONTROL_OFF)));
}

TEST(test_parameter, has_missing_layer) {
    LayerManager layers;
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_missing", LAYER_CONTROL_AUTO));

    std::string missing_layer;
    EXPECT_EQ(::HasMissingLayer(parameters, layers, missing_layer), true);
    EXPECT_STREQ(missing_layer.c_str(), "VK_LAYER_KHRONOS_missing");
}

TEST(test_parameter, no_missing_layer) {
    LayerManager layers;
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.selected_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_explicit", LAYER_CONTROL_AUTO));

    std::string missing_layer;
    EXPECT_EQ(::HasMissingLayer(parameters, layers, missing_layer), false);
    EXPECT_TRUE(missing_layer.empty());
}
