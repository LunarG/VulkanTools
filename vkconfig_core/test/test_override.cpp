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

#include "../override.h"
#include "../environment.h"
#include "../layer.h"
#include "../layer_manager.h"

#include <gtest/gtest.h>

TEST(test_override, override_and_surrender) {
    PathManager paths;
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);

    std::vector<Layer> available_layers;
    Layer layer;
    layer.key = "VK_LAYER_KHRONOS_validation";
    layer._layer_path = "VK_LAYER_KHRONOS_validation.dummy_path";
    available_layers.push_back(layer);

    Configuration configuration;
    const bool load = configuration.Load(std::vector<Layer>(), ":/Configuration 2.0.2 - Standard.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    EXPECT_EQ(true, OverrideConfiguration(environment, available_layers, configuration));
    EXPECT_EQ(true, SurrenderConfiguration(environment));

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}

TEST(test_override, missing_layers) {
    PathManager paths;
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);

    Configuration configuration;
    const bool load = configuration.Load(std::vector<Layer>(), ":/Configuration 2.0.2 - Standard.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    EXPECT_EQ(false, OverrideConfiguration(environment, std::vector<Layer>(), configuration));
    EXPECT_EQ(true, SurrenderConfiguration(environment));

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}

extern bool WriteLayersOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                                const Configuration& configuration, const std::string& layers_path);

extern bool WriteSettingsOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                                  const Configuration& configuration, const std::string& settings_path);

extern bool EraseLayersOverride(const std::string& layers_path);

extern bool EraseSettingsOverride(const std::string& settings_path);

TEST(test_override, write_erase) {
    PathManager paths;
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);

    LayerManager layer_manager(environment);
    layer_manager.LoadLayersFromPath(":/");

    Configuration configuration;
    const bool load = configuration.Load(layer_manager.available_layers, ":/Configuration 2.2.0 - Layer 1.4.0.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    EXPECT_EQ(true, WriteLayersOverride(environment, layer_manager.available_layers, configuration, "./test_layers_override.json"));
    EXPECT_EQ(true,
              WriteSettingsOverride(environment, layer_manager.available_layers, configuration, "./test_settings_override.txt"));

    EXPECT_EQ(true, EraseLayersOverride("./test_layers_override.json"));
    EXPECT_EQ(true, EraseSettingsOverride("./test_settings_override.txt"));

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
