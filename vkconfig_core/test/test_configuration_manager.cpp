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

#include "../configuration_manager.h"

#include <gtest/gtest.h>

TEST(test_configuration_manager, create_remove) {
    PathManager path_manager("");
    Environment environment(path_manager);
    environment.Reset(Environment::DEFAULT);

    std::vector<Layer> available_layers;
    Layer layer;
    layer.key = "VK_LAYER_KHRONOS_validation";
    layer.manifest_path = "VK_LAYER_KHRONOS_validation.dummy_path";
    available_layers.push_back(layer);

    ConfigurationManager configuration_manager(environment);

    // No active configuration by default
    EXPECT_EQ(nullptr, configuration_manager.GetActiveConfiguration());
    EXPECT_EQ(false, configuration_manager.HasActiveConfiguration(available_layers));

    // Create configuration
    configuration_manager.CreateConfiguration(available_layers, "Configuration A");
    configuration_manager.CreateConfiguration(available_layers, "Configuration B");
    EXPECT_EQ(2, configuration_manager.available_configurations.size());
    EXPECT_EQ(false, configuration_manager.HasActiveConfiguration(available_layers));

    // Create configuration with the same name, updating the name
    const std::string configuration_duplicate_key =
        configuration_manager.CreateConfiguration(available_layers, "Configuration A").key;
    EXPECT_EQ(3, configuration_manager.available_configurations.size());
    EXPECT_STREQ("Configuration A (2)", configuration_duplicate_key.c_str());

    // Remove configurations
    configuration_manager.RemoveConfiguration(available_layers, "Configuration A");
    EXPECT_EQ(2, configuration_manager.available_configurations.size());

    configuration_manager.RemoveConfiguration(available_layers, "Configuration B");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());

    // Already deleted
    configuration_manager.RemoveConfiguration(available_layers, "Configuration A");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());
    EXPECT_EQ(false, configuration_manager.HasActiveConfiguration(available_layers));

    // Remove configuration
    configuration_manager.RemoveConfiguration(available_layers, configuration_duplicate_key.c_str());
    EXPECT_EQ(true, configuration_manager.Empty());
    EXPECT_EQ(false, configuration_manager.HasActiveConfiguration(available_layers));

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
