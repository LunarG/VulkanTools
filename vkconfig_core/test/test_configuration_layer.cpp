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

static std::vector<ConfigurationLayer> GenerateTestParametersExist() {
    std::vector<ConfigurationLayer> layers;
    layers.push_back(ConfigurationLayer("Layer E0", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer E1", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer C1", LAYER_STATE_APPLICATION_CONTROLLED));
    return layers;
}

static std::vector<ConfigurationLayer> GenerateTestParametersMissing() {
    std::vector<ConfigurationLayer> layers;
    layers.push_back(ConfigurationLayer("Layer E0", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer E3", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer C1", LAYER_STATE_APPLICATION_CONTROLLED));
    return layers;
}

static std::vector<ConfigurationLayer> GenerateTestParametersAll() {
    std::vector<ConfigurationLayer> layers;
    layers.push_back(ConfigurationLayer("Layer E0", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer E1", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer E2", LAYER_STATE_APPLICATION_CONTROLLED));
    layers.push_back(ConfigurationLayer("Layer E3", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer E4", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer E5", LAYER_STATE_APPLICATION_CONTROLLED));

    layers.push_back(ConfigurationLayer("Layer I0", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer I1", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer I2", LAYER_STATE_APPLICATION_CONTROLLED));
    layers.push_back(ConfigurationLayer("Layer I3", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer I4", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer I5", LAYER_STATE_APPLICATION_CONTROLLED));

    layers.push_back(ConfigurationLayer("Layer C0", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer C1", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer C2", LAYER_STATE_APPLICATION_CONTROLLED));
    layers.push_back(ConfigurationLayer("Layer C3", LAYER_STATE_OVERRIDDEN));
    layers.push_back(ConfigurationLayer("Layer C4", LAYER_STATE_EXCLUDED));
    layers.push_back(ConfigurationLayer("Layer C5", LAYER_STATE_APPLICATION_CONTROLLED));
    return layers;
}

TEST(test_parameter, ordering_no_layers) {
    std::vector<Layer> layers;

    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_custom_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(LAYER_RANK_EXPLICIT_AVAILABLE,
              GetLayerRank(layers, ConfigurationLayer("Layer C0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(LAYER_RANK_EXPLICIT_OVERRIDDEN, GetLayerRank(layers, ConfigurationLayer("Layer C1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_EXCLUDED, GetLayerRank(layers, ConfigurationLayer("Layer C1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer C3", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer C4", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_explicit_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(LAYER_RANK_EXPLICIT_AVAILABLE,
              GetLayerRank(layers, ConfigurationLayer("Layer E0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(LAYER_RANK_EXPLICIT_OVERRIDDEN, GetLayerRank(layers, ConfigurationLayer("Layer E1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_EXCLUDED, GetLayerRank(layers, ConfigurationLayer("Layer E1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer E3", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer E4", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, ordering_found_implicit_layers) {
    const std::vector<Layer>& layers = GenerateTestLayers();

    EXPECT_EQ(LAYER_RANK_IMPLICIT_AVAILABLE,
              GetLayerRank(layers, ConfigurationLayer("Layer I0", LAYER_STATE_APPLICATION_CONTROLLED)));
    EXPECT_EQ(LAYER_RANK_IMPLICIT_OVERRIDDEN, GetLayerRank(layers, ConfigurationLayer("Layer I1", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_EXCLUDED, GetLayerRank(layers, ConfigurationLayer("Layer I1", LAYER_STATE_EXCLUDED)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer I3", LAYER_STATE_OVERRIDDEN)));
    EXPECT_EQ(LAYER_RANK_MISSING, GetLayerRank(layers, ConfigurationLayer("Layer I4", LAYER_STATE_EXCLUDED)));
}

TEST(test_parameter, missing_layers) {
    std::vector<Layer> layers_empty;
    std::vector<Layer> layers = GenerateTestLayers();

    std::vector<ConfigurationLayer> parameters_exist = GenerateTestParametersExist();
    ;
    EXPECT_EQ(true, HasMissingLayer(parameters_exist, layers_empty));
    EXPECT_EQ(false, HasMissingLayer(parameters_exist, layers));

    std::vector<ConfigurationLayer> parameters_missing = GenerateTestParametersMissing();
    EXPECT_EQ(true, HasMissingLayer(parameters_missing, layers_empty));
    EXPECT_EQ(true, HasMissingLayer(parameters_missing, layers));
}

TEST(test_parameter, filter) {
    std::vector<ConfigurationLayer> parameters_app_controlled = GenerateTestParametersExist();
    FilterConfiguratorLayers(parameters_app_controlled, LAYER_STATE_APPLICATION_CONTROLLED);
    EXPECT_EQ(2, parameters_app_controlled.size());

    std::vector<ConfigurationLayer> parameters_overridden = GenerateTestParametersExist();
    FilterConfiguratorLayers(parameters_overridden, LAYER_STATE_OVERRIDDEN);
    EXPECT_EQ(2, parameters_overridden.size());

    std::vector<ConfigurationLayer> parameters_excluded = GenerateTestParametersExist();
    FilterConfiguratorLayers(parameters_excluded, LAYER_STATE_EXCLUDED);
    EXPECT_EQ(2, parameters_excluded.size());
}

TEST(test_parameter, find) {
    std::vector<ConfigurationLayer> layers = GenerateTestParametersExist();

    {
        auto parameter = FindConfigurationLayer(layers, "Layer E1");
        EXPECT_STREQ("Layer E1", parameter->name.toUtf8().constData());
    }

    {
        auto parameter = FindConfigurationLayer(layers, "Layer E4");
        EXPECT_EQ(layers.end(), parameter);
    }
}

TEST(test_parameter, order_automatic) {
    std::vector<Layer> layers = GenerateTestLayers();
    std::vector<ConfigurationLayer> layers = GenerateTestParametersAll();

    SortConfigurationLayers(layers, layers);

    // Missing
    EXPECT_STREQ("Layer C3", layers[0].name.toUtf8().constData());
    EXPECT_STREQ("Layer C4", layers[1].name.toUtf8().constData());
    EXPECT_STREQ("Layer C5", layers[2].name.toUtf8().constData());
    EXPECT_STREQ("Layer E3", layers[3].name.toUtf8().constData());
    EXPECT_STREQ("Layer E4", layers[4].name.toUtf8().constData());
    EXPECT_STREQ("Layer E5", layers[5].name.toUtf8().constData());
    EXPECT_STREQ("Layer I3", layers[6].name.toUtf8().constData());
    EXPECT_STREQ("Layer I4", layers[7].name.toUtf8().constData());
    EXPECT_STREQ("Layer I5", layers[8].name.toUtf8().constData());

    // Exclude
    EXPECT_STREQ("Layer C1", layers[9].name.toUtf8().constData());
    EXPECT_STREQ("Layer E1", layers[10].name.toUtf8().constData());
    EXPECT_STREQ("Layer I1", layers[11].name.toUtf8().constData());

    // Implicit application controlled
    EXPECT_STREQ("Layer I2", layers[12].name.toUtf8().constData());

    // Implicit overriden
    EXPECT_STREQ("Layer I0", layers[13].name.toUtf8().constData());

    // Explicit overriden
    EXPECT_STREQ("Layer C0", layers[14].name.toUtf8().constData());
    EXPECT_STREQ("Layer E0", layers[15].name.toUtf8().constData());

    // Explicit application controlled
    EXPECT_STREQ("Layer C2", layers[16].name.toUtf8().constData());
    EXPECT_STREQ("Layer E2", layers[17].name.toUtf8().constData());
}

TEST(test_parameter, order_manual) {
    std::vector<Layer> layers = GenerateTestLayers();
    std::vector<ConfigurationLayer> layers = GenerateTestParametersAll();

    auto layer_e0 = FindConfigurationLayer(layers, "Layer E0");
    layer_e0->overridden_rank = 14;

    auto layer_c0 = FindConfigurationLayer(layers, "Layer C0");
    layer_c0->overridden_rank = 15;

    SortConfigurationLayers(layers, layers);

    // Missing
    EXPECT_STREQ("Layer C3", layers[0].name.toUtf8().constData());
    EXPECT_STREQ("Layer C4", layers[1].name.toUtf8().constData());
    EXPECT_STREQ("Layer C5", layers[2].name.toUtf8().constData());
    EXPECT_STREQ("Layer E3", layers[3].name.toUtf8().constData());
    EXPECT_STREQ("Layer E4", layers[4].name.toUtf8().constData());
    EXPECT_STREQ("Layer E5", layers[5].name.toUtf8().constData());
    EXPECT_STREQ("Layer I3", layers[6].name.toUtf8().constData());
    EXPECT_STREQ("Layer I4", layers[7].name.toUtf8().constData());
    EXPECT_STREQ("Layer I5", layers[8].name.toUtf8().constData());

    // Exclude
    EXPECT_STREQ("Layer C1", layers[9].name.toUtf8().constData());
    EXPECT_STREQ("Layer E1", layers[10].name.toUtf8().constData());
    EXPECT_STREQ("Layer I1", layers[11].name.toUtf8().constData());

    // Implicit application controlled
    EXPECT_STREQ("Layer I2", layers[12].name.toUtf8().constData());

    // Implicit overriden
    EXPECT_STREQ("Layer I0", layers[13].name.toUtf8().constData());

    // Explicit overriden with manual override!
    EXPECT_STREQ("Layer E0", layers[14].name.toUtf8().constData());
    EXPECT_STREQ("Layer C0", layers[15].name.toUtf8().constData());

    // Explicit application controlled
    EXPECT_STREQ("Layer C2", layers[16].name.toUtf8().constData());
    EXPECT_STREQ("Layer E2", layers[17].name.toUtf8().constData());
}
