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

#include "../parameter.h"
#include "../util.h"

#include <gtest/gtest.h>

static std::vector<Layer> GenerateTestLayers() {
    std::vector<Layer> layers;
    layers.push_back(Layer("Layer E0", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer E1", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer E2", LAYER_TYPE_EXPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer I0", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer I1", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer I2", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer C0", LAYER_TYPE_CUSTOM, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer C1", LAYER_TYPE_CUSTOM, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer C2", LAYER_TYPE_CUSTOM, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
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
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E3", LAYER_STATE_EXCLUDED));
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

    std::vector<Parameter> parameters_exist = GenerateTestParametersExist();
    ;
    EXPECT_EQ(true, HasMissingParameter(parameters_exist, layers_empty));
    EXPECT_EQ(false, HasMissingParameter(parameters_exist, layers));

    std::vector<Parameter> parameters_missing = GenerateTestParametersMissing();
    EXPECT_EQ(true, HasMissingParameter(parameters_missing, layers_empty));
    EXPECT_EQ(true, HasMissingParameter(parameters_missing, layers));
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

TEST(test_parameter, find) {
    std::vector<Parameter> parameters = GenerateTestParametersExist();

    {
        auto parameter = FindParameter(parameters, "Layer E1");
        EXPECT_STREQ("Layer E1", parameter->name.toUtf8().constData());
    }

    {
        auto parameter = FindParameter(parameters, "Layer E4");
        EXPECT_EQ(parameters.end(), parameter);
    }
}

TEST(test_parameter, order_automatic) {
    std::vector<Layer> layers = GenerateTestLayers();
    std::vector<Parameter> parameters = GenerateTestParametersAll();

    OrderParameter(parameters, layers);

    // Missing
    EXPECT_STREQ("Layer C3", parameters[0].name.toUtf8().constData());
    EXPECT_STREQ("Layer C4", parameters[1].name.toUtf8().constData());
    EXPECT_STREQ("Layer C5", parameters[2].name.toUtf8().constData());
    EXPECT_STREQ("Layer E3", parameters[3].name.toUtf8().constData());
    EXPECT_STREQ("Layer E4", parameters[4].name.toUtf8().constData());
    EXPECT_STREQ("Layer E5", parameters[5].name.toUtf8().constData());
    EXPECT_STREQ("Layer I3", parameters[6].name.toUtf8().constData());
    EXPECT_STREQ("Layer I4", parameters[7].name.toUtf8().constData());
    EXPECT_STREQ("Layer I5", parameters[8].name.toUtf8().constData());

    // Exclude
    EXPECT_STREQ("Layer C1", parameters[9].name.toUtf8().constData());
    EXPECT_STREQ("Layer E1", parameters[10].name.toUtf8().constData());
    EXPECT_STREQ("Layer I1", parameters[11].name.toUtf8().constData());

    // Implicit application controlled
    EXPECT_STREQ("Layer I2", parameters[12].name.toUtf8().constData());

    // Implicit overriden
    EXPECT_STREQ("Layer I0", parameters[13].name.toUtf8().constData());

    // Explicit overriden
    EXPECT_STREQ("Layer C0", parameters[14].name.toUtf8().constData());
    EXPECT_STREQ("Layer E0", parameters[15].name.toUtf8().constData());

    // Explicit application controlled
    EXPECT_STREQ("Layer C2", parameters[16].name.toUtf8().constData());
    EXPECT_STREQ("Layer E2", parameters[17].name.toUtf8().constData());
}

TEST(test_parameter, order_manual) {
    std::vector<Layer> layers = GenerateTestLayers();
    std::vector<Parameter> parameters = GenerateTestParametersAll();

    auto layer_e0 = FindParameter(parameters, "Layer E0");
    layer_e0->overridden_rank = 14;

    auto layer_c0 = FindParameter(parameters, "Layer C0");
    layer_c0->overridden_rank = 15;

    OrderParameter(parameters, layers);

    // Missing
    EXPECT_STREQ("Layer C3", parameters[0].name.toUtf8().constData());
    EXPECT_STREQ("Layer C4", parameters[1].name.toUtf8().constData());
    EXPECT_STREQ("Layer C5", parameters[2].name.toUtf8().constData());
    EXPECT_STREQ("Layer E3", parameters[3].name.toUtf8().constData());
    EXPECT_STREQ("Layer E4", parameters[4].name.toUtf8().constData());
    EXPECT_STREQ("Layer E5", parameters[5].name.toUtf8().constData());
    EXPECT_STREQ("Layer I3", parameters[6].name.toUtf8().constData());
    EXPECT_STREQ("Layer I4", parameters[7].name.toUtf8().constData());
    EXPECT_STREQ("Layer I5", parameters[8].name.toUtf8().constData());

    // Exclude
    EXPECT_STREQ("Layer C1", parameters[9].name.toUtf8().constData());
    EXPECT_STREQ("Layer E1", parameters[10].name.toUtf8().constData());
    EXPECT_STREQ("Layer I1", parameters[11].name.toUtf8().constData());

    // Implicit application controlled
    EXPECT_STREQ("Layer I2", parameters[12].name.toUtf8().constData());

    // Implicit overriden
    EXPECT_STREQ("Layer I0", parameters[13].name.toUtf8().constData());

    // Explicit overriden with manual override!
    EXPECT_STREQ("Layer E0", parameters[14].name.toUtf8().constData());
    EXPECT_STREQ("Layer C0", parameters[15].name.toUtf8().constData());

    // Explicit application controlled
    EXPECT_STREQ("Layer C2", parameters[16].name.toUtf8().constData());
    EXPECT_STREQ("Layer E2", parameters[17].name.toUtf8().constData());
}
