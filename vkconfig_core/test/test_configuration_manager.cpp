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

#include "../configuration_manager.h"

#include <gtest/gtest.h>

TEST(test_configuration_manager, create_remove) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    ConfigurationManager configuration_manager;

    // Create configuration
    configuration_manager.CreateConfiguration(layer_manager, "Configuration A");
    configuration_manager.CreateConfiguration(layer_manager, "Configuration B");
    EXPECT_EQ(2, configuration_manager.available_configurations.size());

    // Create configuration with the same name, updating the name
    const std::string configuration_duplicate_key = configuration_manager.CreateConfiguration(layer_manager, "Configuration A").key;
    EXPECT_EQ(3, configuration_manager.available_configurations.size());
    EXPECT_STREQ("Configuration A (2)", configuration_duplicate_key.c_str());

    // Remove configurations
    configuration_manager.RemoveConfiguration("Configuration A");
    EXPECT_EQ(2, configuration_manager.available_configurations.size());

    configuration_manager.RemoveConfiguration("Configuration B");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());

    // Already deleted
    configuration_manager.RemoveConfiguration("Configuration A");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());

    // Remove configuration
    configuration_manager.RemoveConfiguration(configuration_duplicate_key.c_str());
    EXPECT_EQ(true, configuration_manager.Empty());
}
