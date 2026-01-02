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
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_validation", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_profiles", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_timeline_semaphore", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_synchronization2", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_shader_object", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_memory_decompression", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    layers.available_layers[0].type = LAYER_TYPE_IMPLICIT;

    Parameter parameter_implicit("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_OFF);
    parameter_implicit.type = LAYER_TYPE_IMPLICIT;

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
    EXPECT_EQ(PARAMETER_RANK_IMPLICIT_LAYER, GetParameterOrdering(layers, parameter_implicit));
    EXPECT_EQ(PARAMETER_RANK_MISSING_LAYER, GetParameterOrdering(layers, Parameter("VK_LAYER_KHRONOS_missing", LAYER_CONTROL_OFF)));
}

TEST(test_parameter, has_missing_layer) {
    LayerManager layers;
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_missing", LAYER_CONTROL_AUTO));

    std::string missing_layer;
    EXPECT_EQ(::HasMissingLayer(parameters, layers, missing_layer), true);
    EXPECT_STREQ(missing_layer.c_str(), "VK_LAYER_KHRONOS_missing");
}

TEST(test_parameter, no_missing_layer) {
    LayerManager layers;
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_explicit", LAYER_CONTROL_AUTO));

    std::string missing_layer;
    EXPECT_EQ(::HasMissingLayer(parameters, layers, missing_layer), false);
    EXPECT_TRUE(missing_layer.empty());
}

TEST(test_parameter, order_parameter_automatic) {
    LayerManager layers;
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_validation", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_profiles", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_timeline_semaphore", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_synchronization2", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_shader_object", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_memory_decompression", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers[0].type = LAYER_TYPE_IMPLICIT;

    Parameter unordered_layers(::GetToken(LAYER_BUILTIN_UNORDERED), LAYER_CONTROL_AUTO);
    unordered_layers.builtin = LAYER_BUILTIN_UNORDERED;

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_AUTO));
    parameters.push_back(unordered_layers);
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_validation", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_profiles", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_timeline_semaphore", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_synchronization2", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_shader_object", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_memory_decompression", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_explicit", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_missing", LAYER_CONTROL_AUTO));

    parameters[0].type = LAYER_TYPE_IMPLICIT;

    ::OrderParameter(parameters, layers);

    EXPECT_STREQ(parameters[0].key.c_str(), "VK_LAYER_KHRONOS_missing");
    EXPECT_STREQ(parameters[1].key.c_str(), "VK_LAYER_KHRONOS_implicit");
    EXPECT_EQ(parameters[2].builtin, LAYER_BUILTIN_UNORDERED);
    EXPECT_STREQ(parameters[3].key.c_str(), "VK_LAYER_KHRONOS_explicit");
    EXPECT_STREQ(parameters[4].key.c_str(), "VK_LAYER_KHRONOS_validation");
    EXPECT_STREQ(parameters[5].key.c_str(), "VK_LAYER_KHRONOS_profiles");
    EXPECT_STREQ(parameters[6].key.c_str(), "VK_LAYER_KHRONOS_memory_decompression");
    EXPECT_STREQ(parameters[7].key.c_str(), "VK_LAYER_KHRONOS_shader_object");
    EXPECT_STREQ(parameters[8].key.c_str(), "VK_LAYER_KHRONOS_synchronization2");
    EXPECT_STREQ(parameters[9].key.c_str(), "VK_LAYER_KHRONOS_timeline_semaphore");

    ::OrderParameter(parameters, layers);

    EXPECT_STREQ(parameters[0].key.c_str(), "VK_LAYER_KHRONOS_missing");
    EXPECT_STREQ(parameters[1].key.c_str(), "VK_LAYER_KHRONOS_implicit");
    EXPECT_EQ(parameters[2].builtin, LAYER_BUILTIN_UNORDERED);
    EXPECT_STREQ(parameters[3].key.c_str(), "VK_LAYER_KHRONOS_explicit");
    EXPECT_STREQ(parameters[4].key.c_str(), "VK_LAYER_KHRONOS_validation");
    EXPECT_STREQ(parameters[5].key.c_str(), "VK_LAYER_KHRONOS_profiles");
    EXPECT_STREQ(parameters[6].key.c_str(), "VK_LAYER_KHRONOS_memory_decompression");
    EXPECT_STREQ(parameters[7].key.c_str(), "VK_LAYER_KHRONOS_shader_object");
    EXPECT_STREQ(parameters[8].key.c_str(), "VK_LAYER_KHRONOS_synchronization2");
    EXPECT_STREQ(parameters[9].key.c_str(), "VK_LAYER_KHRONOS_timeline_semaphore");
}

TEST(test_parameter, order_parameter_manual_partial) {
    LayerManager layers;
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_implicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_explicit", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_validation", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(Layer("VK_LAYER_KHRONOS_profiles", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_timeline_semaphore", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_synchronization2", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_memory_decompression", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.available_layers[0].type = LAYER_TYPE_IMPLICIT;

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_implicit", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter(::GetToken(LAYER_BUILTIN_UNORDERED), LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_missing", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_validation", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_profiles", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_timeline_semaphore", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_synchronization2", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_memory_decompression", LAYER_CONTROL_AUTO));
    parameters.push_back(Parameter("VK_LAYER_KHRONOS_explicit", LAYER_CONTROL_AUTO));

    parameters[0].type = LAYER_TYPE_IMPLICIT;
    parameters[1].builtin = LAYER_BUILTIN_UNORDERED;

    ::OrderParameter(parameters, layers);

    EXPECT_STREQ(parameters[0].key.c_str(), "VK_LAYER_KHRONOS_missing");
    EXPECT_STREQ(parameters[1].key.c_str(), "VK_LAYER_KHRONOS_implicit");
    EXPECT_EQ(parameters[2].builtin, LAYER_BUILTIN_UNORDERED);
    EXPECT_STREQ(parameters[3].key.c_str(), "VK_LAYER_KHRONOS_explicit");
    EXPECT_STREQ(parameters[4].key.c_str(), "VK_LAYER_KHRONOS_validation");
    EXPECT_STREQ(parameters[5].key.c_str(), "VK_LAYER_KHRONOS_profiles");
    EXPECT_STREQ(parameters[6].key.c_str(), "VK_LAYER_KHRONOS_memory_decompression");
    EXPECT_STREQ(parameters[7].key.c_str(), "VK_LAYER_KHRONOS_synchronization2");
    EXPECT_STREQ(parameters[8].key.c_str(), "VK_LAYER_KHRONOS_timeline_semaphore");

    // Check ordering again result in the same list
    ::OrderParameter(parameters, layers);

    EXPECT_STREQ(parameters[0].key.c_str(), "VK_LAYER_KHRONOS_missing");
    EXPECT_STREQ(parameters[1].key.c_str(), "VK_LAYER_KHRONOS_implicit");
    EXPECT_EQ(parameters[2].builtin, LAYER_BUILTIN_UNORDERED);
    EXPECT_STREQ(parameters[3].key.c_str(), "VK_LAYER_KHRONOS_explicit");
    EXPECT_STREQ(parameters[4].key.c_str(), "VK_LAYER_KHRONOS_validation");
    EXPECT_STREQ(parameters[5].key.c_str(), "VK_LAYER_KHRONOS_profiles");
    EXPECT_STREQ(parameters[6].key.c_str(), "VK_LAYER_KHRONOS_memory_decompression");
    EXPECT_STREQ(parameters[7].key.c_str(), "VK_LAYER_KHRONOS_synchronization2");
    EXPECT_STREQ(parameters[8].key.c_str(), "VK_LAYER_KHRONOS_timeline_semaphore");

    // Insert a new layer in the parameter list
    layers.available_layers.push_back(
        Layer("VK_LAYER_KHRONOS_shader_object", Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));

    Parameter parameter_implicit2("VK_LAYER_KHRONOS_shader_object", LAYER_CONTROL_AUTO);
    parameters.push_back(parameter_implicit2);

    ::OrderParameter(parameters, layers);

    EXPECT_STREQ(parameters[0].key.c_str(), "VK_LAYER_KHRONOS_missing");
    EXPECT_STREQ(parameters[1].key.c_str(), "VK_LAYER_KHRONOS_implicit");
    EXPECT_EQ(parameters[2].builtin, LAYER_BUILTIN_UNORDERED);
    EXPECT_STREQ(parameters[3].key.c_str(), "VK_LAYER_KHRONOS_explicit");
    EXPECT_STREQ(parameters[4].key.c_str(), "VK_LAYER_KHRONOS_validation");
    EXPECT_STREQ(parameters[5].key.c_str(), "VK_LAYER_KHRONOS_profiles");
    EXPECT_STREQ(parameters[6].key.c_str(), "VK_LAYER_KHRONOS_memory_decompression");
    EXPECT_STREQ(parameters[7].key.c_str(), "VK_LAYER_KHRONOS_synchronization2");
    EXPECT_STREQ(parameters[8].key.c_str(), "VK_LAYER_KHRONOS_timeline_semaphore");
    EXPECT_STREQ(parameters[9].key.c_str(), "VK_LAYER_KHRONOS_shader_object");
}
