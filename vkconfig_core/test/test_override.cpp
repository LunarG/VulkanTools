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
    ASSERT_TRUE(load);
    ASSERT_TRUE(!configuration.IsEmpty());

    EXPECT_EQ(true, OverrideLayers(environment, available_layers, configuration));
    EXPECT_EQ(true, SurrenderLayers(environment));

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}

TEST(test_override, missing_layers) {
    PathManager paths;
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);

    Configuration configuration;
    const bool load = configuration.Load(std::vector<Layer>(), ":/Configuration 2.0.2 - Standard.json");
    ASSERT_TRUE(load);
    ASSERT_TRUE(!configuration.IsEmpty());

    EXPECT_EQ(false, OverrideLayers(environment, std::vector<Layer>(), configuration));
    EXPECT_EQ(true, SurrenderLayers(environment));

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
