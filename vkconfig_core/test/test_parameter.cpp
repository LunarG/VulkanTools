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
#include "../parameter.h"
#include "../util.h"
#include "../setting_string.h"

#include <gtest/gtest.h>

inline SettingMetaString* InstantiateString(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaString*>(layer.Instantiate(layer.settings, key, SETTING_STRING));
}

static std::vector<Layer> GenerateTestLayers() {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E1", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer I0", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer I1", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer I2", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer C0", LAYER_TYPE_USER_DEFINED, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer C1", LAYER_TYPE_USER_DEFINED, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer C2", LAYER_TYPE_USER_DEFINED, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    return layers;
}

static std::vector<Parameter> GenerateTestParametersExist() {
    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer C1", LAYER_STATE_APPLICATION_CONTROLLED));
    return parameters;
}

static std::vector<Parameter> GenerateTestParametersMissing() {
    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E3", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer C1", LAYER_STATE_APPLICATION_CONTROLLED));
    return parameters;
}

static std::vector<Parameter> GenerateTestParametersAll() {
    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_APPLICATION_CONTROLLED));
    parameters.push_back(Parameter("Layer E3", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E4", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer E5", LAYER_STATE_APPLICATION_CONTROLLED));

    parameters.push_back(Parameter("Layer I0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer I1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer I2", LAYER_STATE_APPLICATION_CONTROLLED));
    parameters.push_back(Parameter("Layer I3", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer I4", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer I5", LAYER_STATE_APPLICATION_CONTROLLED));

    parameters.push_back(Parameter("Layer C0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer C1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer C2", LAYER_STATE_APPLICATION_CONTROLLED));
    parameters.push_back(Parameter("Layer C3", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer C4", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer C5", LAYER_STATE_APPLICATION_CONTROLLED));
    return parameters;
}

TEST(test_parameter, ordering_no_layers) {
    std::vector<Layer> layers;

    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_custom_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_AVAILABLE,
              GetParameterOrdering(layers, Parameter("Layer C0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_OVERRIDDEN, GetParameterOrdering(layers, Parameter("Layer C1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_EXCLUDED, GetParameterOrdering(layers, Parameter("Layer C1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer C3", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer C4", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_explicit_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_AVAILABLE,
              GetParameterOrdering(layers, Parameter("Layer E0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_OVERRIDDEN, GetParameterOrdering(layers, Parameter("Layer E1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_EXCLUDED, GetParameterOrdering(layers, Parameter("Layer E1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer E3", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer E4", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_implicit_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(PARAMETER_RANK_IMPLICIT_AVAILABLE,
              GetParameterOrdering(layers, Parameter("Layer I0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(PARAMETER_RANK_IMPLICIT_OVERRIDDEN, GetParameterOrdering(layers, Parameter("Layer I1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_EXCLUDED, GetParameterOrdering(layers, Parameter("Layer I1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer I3", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer I4", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, missing_layers) {
    std::vector<Layer> layers_empty;
    std::vector<Layer> layers = GenerateTestLayers();

    std::string missing_layer;

    std::vector<Parameter> parameters_exist = GenerateTestParametersExist();
    EXPECT_EQ(true, HasMissingLayer(parameters_exist, layers_empty, missing_layer));
    EXPECT_EQ(false, HasMissingLayer(parameters_exist, layers, missing_layer));

    std::vector<Parameter> parameters_missing = GenerateTestParametersMissing();
    EXPECT_EQ(true, HasMissingLayer(parameters_missing, layers_empty, missing_layer));
    EXPECT_EQ(true, HasMissingLayer(parameters_missing, layers, missing_layer));
}

TEST(test_parameter, filter) {
    std::vector<Parameter> parameters_app_controlled = GenerateTestParametersExist();
    FilterParameters(parameters_app_controlled, LAYER_STATE_APPLICATION_CONTROLLED);
    EXPECT_EQ(2, parameters_app_controlled.size());

    std::vector<Parameter> parameters_overridden = GenerateTestParametersExist();
    FilterParameters(parameters_overridden, LAYER_STATE_OVERRIDDEN);
    EXPECT_EQ(2, parameters_overridden.size());

    std::vector<Parameter> parameters_excluded = GenerateTestParametersExist();
    FilterParameters(parameters_excluded, LAYER_STATE_EXCLUDED);
    EXPECT_EQ(2, parameters_excluded.size());
}

TEST(test_parameter, order_automatic) {
    std::vector<Layer> layers = GenerateTestLayers();
    std::vector<Parameter> parameters = GenerateTestParametersAll();

    OrderParameter(parameters, layers);

    // Missing
    EXPECT_STREQ("Layer C3", parameters[0].key.c_str());
    EXPECT_STREQ("Layer C4", parameters[1].key.c_str());
    EXPECT_STREQ("Layer C5", parameters[2].key.c_str());
    EXPECT_STREQ("Layer E3", parameters[3].key.c_str());
    EXPECT_STREQ("Layer E4", parameters[4].key.c_str());
    EXPECT_STREQ("Layer E5", parameters[5].key.c_str());
    EXPECT_STREQ("Layer I3", parameters[6].key.c_str());
    EXPECT_STREQ("Layer I4", parameters[7].key.c_str());
    EXPECT_STREQ("Layer I5", parameters[8].key.c_str());

    // Exclude
    EXPECT_STREQ("Layer C1", parameters[9].key.c_str());
    EXPECT_STREQ("Layer E1", parameters[10].key.c_str());
    EXPECT_STREQ("Layer I1", parameters[11].key.c_str());

    // Implicit application controlled
    EXPECT_STREQ("Layer I2", parameters[12].key.c_str());

    // Implicit overriden
    EXPECT_STREQ("Layer I0", parameters[13].key.c_str());

    // Explicit overriden
    EXPECT_STREQ("Layer C0", parameters[14].key.c_str());
    EXPECT_STREQ("Layer E0", parameters[15].key.c_str());

    // Explicit application controlled
    EXPECT_STREQ("Layer C2", parameters[16].key.c_str());
    EXPECT_STREQ("Layer E2", parameters[17].key.c_str());
}

TEST(test_parameter, order_manual) {
    std::vector<Layer> layers = GenerateTestLayers();
    std::vector<Parameter> parameters = GenerateTestParametersAll();

    Parameter* layer_e0 = FindByKey(parameters, "Layer E0");
    layer_e0->overridden_rank = 14;

    Parameter* layer_c0 = FindByKey(parameters, "Layer C0");
    layer_c0->overridden_rank = 15;

    OrderParameter(parameters, layers);

    // Missing
    EXPECT_STREQ("Layer C3", parameters[0].key.c_str());
    EXPECT_STREQ("Layer C4", parameters[1].key.c_str());
    EXPECT_STREQ("Layer C5", parameters[2].key.c_str());
    EXPECT_STREQ("Layer E3", parameters[3].key.c_str());
    EXPECT_STREQ("Layer E4", parameters[4].key.c_str());
    EXPECT_STREQ("Layer E5", parameters[5].key.c_str());
    EXPECT_STREQ("Layer I3", parameters[6].key.c_str());
    EXPECT_STREQ("Layer I4", parameters[7].key.c_str());
    EXPECT_STREQ("Layer I5", parameters[8].key.c_str());

    // Exclude
    EXPECT_STREQ("Layer C1", parameters[9].key.c_str());
    EXPECT_STREQ("Layer E1", parameters[10].key.c_str());
    EXPECT_STREQ("Layer I1", parameters[11].key.c_str());

    // Implicit application controlled
    EXPECT_STREQ("Layer I2", parameters[12].key.c_str());

    // Implicit overriden
    EXPECT_STREQ("Layer I0", parameters[13].key.c_str());

    // Explicit overriden with manual override!
    EXPECT_STREQ("Layer E0", parameters[14].key.c_str());
    EXPECT_STREQ("Layer C0", parameters[15].key.c_str());

    // Explicit application controlled
    EXPECT_STREQ("Layer C2", parameters[16].key.c_str());
    EXPECT_STREQ("Layer E2", parameters[17].key.c_str());
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

TEST(test_parameter, gather_parameters_exist) {
    std::vector<Parameter> parameters = GatherParameters(GenerateTestParametersExist(), GenerateTestLayers());

    EXPECT_EQ(9, parameters.size());

    EXPECT_STREQ("Layer E1", parameters[0].key.c_str());
    EXPECT_STREQ("Layer I0", parameters[1].key.c_str());
    EXPECT_STREQ("Layer I1", parameters[2].key.c_str());
    EXPECT_STREQ("Layer I2", parameters[3].key.c_str());
    EXPECT_STREQ("Layer E0", parameters[4].key.c_str());
    EXPECT_STREQ("Layer C0", parameters[5].key.c_str());
    EXPECT_STREQ("Layer C1", parameters[6].key.c_str());
    EXPECT_STREQ("Layer C2", parameters[7].key.c_str());
    EXPECT_STREQ("Layer E2", parameters[8].key.c_str());
}

TEST(test_parameter, gather_parameters_missing) {
    std::vector<Parameter> parameters = GatherParameters(GenerateTestParametersMissing(), GenerateTestLayers());

    EXPECT_EQ(10, parameters.size());

    EXPECT_STREQ("Layer E3", parameters[0].key.c_str());
    EXPECT_STREQ("Layer E1", parameters[1].key.c_str());
    EXPECT_STREQ("Layer I0", parameters[2].key.c_str());
    EXPECT_STREQ("Layer I1", parameters[3].key.c_str());
    EXPECT_STREQ("Layer I2", parameters[4].key.c_str());
    EXPECT_STREQ("Layer C0", parameters[5].key.c_str());
    EXPECT_STREQ("Layer C1", parameters[6].key.c_str());
    EXPECT_STREQ("Layer C2", parameters[7].key.c_str());
    EXPECT_STREQ("Layer E0", parameters[8].key.c_str());
    EXPECT_STREQ("Layer E2", parameters[9].key.c_str());
}

TEST(test_parameter, gather_parameters_all) {
    std::vector<Parameter> parameters = GatherParameters(GenerateTestParametersAll(), GenerateTestLayers());

    EXPECT_EQ(18, parameters.size());

    EXPECT_STREQ("Layer C3", parameters[0].key.c_str());
    EXPECT_STREQ("Layer C4", parameters[1].key.c_str());
    EXPECT_STREQ("Layer C5", parameters[2].key.c_str());
    EXPECT_STREQ("Layer E3", parameters[3].key.c_str());
    EXPECT_STREQ("Layer E4", parameters[4].key.c_str());
    EXPECT_STREQ("Layer E5", parameters[5].key.c_str());
    EXPECT_STREQ("Layer I3", parameters[6].key.c_str());
    EXPECT_STREQ("Layer I4", parameters[7].key.c_str());
    EXPECT_STREQ("Layer I5", parameters[8].key.c_str());
    EXPECT_STREQ("Layer C1", parameters[9].key.c_str());
    EXPECT_STREQ("Layer E1", parameters[10].key.c_str());
    EXPECT_STREQ("Layer I1", parameters[11].key.c_str());
    EXPECT_STREQ("Layer I2", parameters[12].key.c_str());
    EXPECT_STREQ("Layer I0", parameters[13].key.c_str());
    EXPECT_STREQ("Layer C0", parameters[14].key.c_str());
    EXPECT_STREQ("Layer E0", parameters[15].key.c_str());
    EXPECT_STREQ("Layer C2", parameters[16].key.c_str());
    EXPECT_STREQ("Layer E2", parameters[17].key.c_str());
}

TEST(test_parameter, compute_min_api_version) {
    std::vector<Parameter> parameters = GenerateTestParametersExist();
    std::vector<Layer> layers = GenerateTestLayers();

    Version min_version = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 148), min_version);
}

TEST(test_parameter, compute_min_api_version_exclude_middle) {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E1", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 162), "1", "layer.json"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 176), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_APPLICATION_CONTROLLED));

    Version min_version_A = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 148), min_version_A);

    Version min_version_B = ComputeMinApiVersion(Version(1, 2, 140), parameters, layers);
    EXPECT_EQ(Version(1, 2, 140), min_version_B);
}

TEST(test_parameter, compute_min_api_version_exclude_older) {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E1", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 162), "1", "layer.json"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 176), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_APPLICATION_CONTROLLED));

    Version min_version_A = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 162), min_version_A);

    Version min_version_B = ComputeMinApiVersion(Version(1, 2, 160), parameters, layers);
    EXPECT_EQ(Version(1, 2, 160), min_version_B);
}

TEST(test_parameter, compute_min_api_version_exclude_newer) {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E1", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 162), "1", "layer.json"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 176), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_APPLICATION_CONTROLLED));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_EXCLUDED));

    Version min_version_A = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 148), min_version_A);

    Version min_version_B = ComputeMinApiVersion(Version(1, 2, 140), parameters, layers);
    EXPECT_EQ(Version(1, 2, 140), min_version_B);
}

TEST(test_parameter, compute_min_api_version_exclude_all) {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E1", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 162), "1", "layer.json"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 176), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_EXCLUDED));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_EXCLUDED));

    Version min_version_A = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 170), min_version_A);
}

TEST(test_parameter, compute_min_api_version_missing_one) {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 176), "1", "layer.json"));

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_APPLICATION_CONTROLLED));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_OVERRIDDEN));

    Version min_version_A = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 148), min_version_A);
}

TEST(test_parameter, compute_min_api_version_missing_all) {
    std::vector<Layer> layers;

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_APPLICATION_CONTROLLED));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E2", LAYER_STATE_OVERRIDDEN));

    Version min_version_A = ComputeMinApiVersion(Version(1, 2, 170), parameters, layers);
    EXPECT_EQ(Version(1, 2, 170), min_version_A);
}
