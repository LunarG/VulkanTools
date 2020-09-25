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
    layers.push_back(Layer("Layer I0", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer I1", LAYER_TYPE_IMPLICIT, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer C0", LAYER_TYPE_CUSTOM, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    layers.push_back(Layer("Layer C1", LAYER_TYPE_CUSTOM, Version(1, 0, 0), Version(1, 2, 148), "1", "layer.json", "GLOBAL"));
    return layers;
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
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer C2", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer C3", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_explicit_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_AVAILABLE,
              GetParameterOrdering(layers, Parameter("Layer E0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(PARAMETER_RANK_EXPLICIT_OVERRIDDEN, GetParameterOrdering(layers, Parameter("Layer E1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_EXCLUDED, GetParameterOrdering(layers, Parameter("Layer E1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer E2", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer E3", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_implicit_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(PARAMETER_RANK_IMPLICIT_AVAILABLE,
              GetParameterOrdering(layers, Parameter("Layer I0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(PARAMETER_RANK_IMPLICIT_OVERRIDDEN, GetParameterOrdering(layers, Parameter("Layer I1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_EXCLUDED, GetParameterOrdering(layers, Parameter("Layer I1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer I2", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(PARAMETER_RANK_MISSING, GetParameterOrdering(layers, Parameter("Layer I3", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_missing_layers) {
    std::vector<Layer> layers_empty;
    std::vector<Layer> layers = GenerateTestLayers();

    std::vector<Parameter> parameters;
    parameters.push_back(Parameter("Layer E0", LAYER_STATE_OVERRIDDEN));
    parameters.push_back(Parameter("Layer E1", LAYER_STATE_EXCLUDED));

    EXPECT_EQ(true, HasMissingParameter(parameters, layers_empty));
    EXPECT_EQ(false, HasMissingParameter(parameters, layers));
}
