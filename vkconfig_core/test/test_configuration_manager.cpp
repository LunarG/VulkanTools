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

TEST(test_configuration_manager, init_default) {
    ConfigurationManager configuration_manager;

    const ConfigurationInfo* info_global = configuration_manager.FindConfigurationInfo(GLOBAL_CONFIGURATION_TOKEN);
    EXPECT_TRUE(info_global != nullptr);
    const ConfigurationInfo* info_unknown = configuration_manager.FindConfigurationInfo("unknown");
    EXPECT_TRUE(info_unknown == nullptr);
    const ConfigurationInfo* info_active = configuration_manager.GetActiveConfigurationInfo();
    EXPECT_EQ(info_active, info_global);

    EXPECT_FALSE(configuration_manager.GetUseSystemTray());
    EXPECT_FALSE(configuration_manager.GetPerExecutableConfig());
    EXPECT_TRUE(configuration_manager.HasActiveConfiguration());
    EXPECT_TRUE(configuration_manager.Empty());
    EXPECT_STREQ(configuration_manager.last_path_import.RelativePath().c_str(), Get(Path::HOME).RelativePath().c_str());
    EXPECT_STREQ(configuration_manager.last_path_export.RelativePath().c_str(), Get(Path::HOME).RelativePath().c_str());
}

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

TEST(test_configuration_manager, duplicate_names) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    ConfigurationManager configuration_manager;

    // Create configuration
    configuration_manager.CreateConfiguration(layer_manager, "Configuration A");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());

    std::string dup2 = configuration_manager.DuplicateConfiguration(layer_manager, "Configuration A").key;
    EXPECT_EQ(2, configuration_manager.available_configurations.size());
    Configuration* configuration2 = configuration_manager.FindConfiguration("Configuration A (2)");
    EXPECT_STREQ(configuration2->key.c_str(), dup2.c_str());

    std::string dup3 = configuration_manager.DuplicateConfiguration(layer_manager, "Configuration A (2)").key;
    EXPECT_EQ(3, configuration_manager.available_configurations.size());
    Configuration* configuration3 = configuration_manager.FindConfiguration("Configuration A (3)");
    EXPECT_STREQ(configuration3->key.c_str(), dup3.c_str());

    std::string dup4 = configuration_manager.DuplicateConfiguration(layer_manager, "Configuration A").key;
    EXPECT_EQ(4, configuration_manager.available_configurations.size());
    Configuration* configuration4 = configuration_manager.FindConfiguration("Configuration A (4)");
    EXPECT_STREQ(configuration4->key.c_str(), dup4.c_str());
}

TEST(test_configuration_manager, duplicate_object) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    ConfigurationManager configuration_manager;

    // Create configuration
    configuration_manager.CreateConfiguration(layer_manager, "Configuration A");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());

    std::string dup2 = configuration_manager.DuplicateConfiguration(layer_manager, "Configuration A").key;
    EXPECT_EQ(2, configuration_manager.available_configurations.size());

    Configuration* configuration_ori = configuration_manager.FindConfiguration("Configuration A");
    Configuration* configuration_dup = configuration_manager.FindConfiguration("Configuration A (2)");

    EXPECT_EQ(configuration_ori->version, configuration_dup->version);
    EXPECT_EQ(configuration_ori->platform_flags, configuration_dup->platform_flags);
    EXPECT_EQ(configuration_ori->description, configuration_dup->description);
    EXPECT_EQ(configuration_ori->view_advanced_settings, configuration_dup->view_advanced_settings);
    EXPECT_EQ(configuration_ori->view_advanced_layers, configuration_dup->view_advanced_layers);
    EXPECT_EQ(configuration_ori->loader_log_messages_flags, configuration_dup->loader_log_messages_flags);
    EXPECT_EQ(configuration_ori->selected_layer_name, configuration_dup->selected_layer_name);
    EXPECT_EQ(configuration_ori->parameters.size(), configuration_dup->parameters.size());
    EXPECT_EQ(configuration_ori->user_defined_paths, configuration_dup->user_defined_paths);
    EXPECT_EQ(configuration_ori->IsBuiltIn(), configuration_dup->IsBuiltIn());
    EXPECT_EQ(configuration_ori->Size(), configuration_dup->Size());
}

TEST(test_configuration_manager, sort) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    ConfigurationManager configuration_manager;

    configuration_manager.CreateConfiguration(layer_manager, "Configuration A");
    configuration_manager.CreateConfiguration(layer_manager, "Configuration 2");
    configuration_manager.CreateConfiguration(layer_manager, "Configuration 1");
    configuration_manager.CreateConfiguration(layer_manager, "Configuration B");
    configuration_manager.CreateConfiguration(layer_manager, "Configuration 3");

    configuration_manager.SortConfigurations();

    EXPECT_STREQ("Configuration 1", configuration_manager.available_configurations[0].key.c_str());
    EXPECT_STREQ("Configuration 2", configuration_manager.available_configurations[1].key.c_str());
    EXPECT_STREQ("Configuration 3", configuration_manager.available_configurations[2].key.c_str());
    EXPECT_STREQ("Configuration A", configuration_manager.available_configurations[3].key.c_str());
    EXPECT_STREQ("Configuration B", configuration_manager.available_configurations[4].key.c_str());
}

TEST(test_configuration_manager, get_index) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    ConfigurationManager configuration_manager;

    configuration_manager.CreateConfiguration(layer_manager, "Configuration B");
    EXPECT_EQ(1, configuration_manager.available_configurations.size());
    EXPECT_EQ(0, configuration_manager.GetConfigurationIndex("Configuration B"));

    configuration_manager.CreateConfiguration(layer_manager, "Configuration A");
    EXPECT_EQ(2, configuration_manager.available_configurations.size());
    EXPECT_EQ(0, configuration_manager.GetConfigurationIndex("Configuration A"));
    EXPECT_EQ(1, configuration_manager.GetConfigurationIndex("Configuration B"));

    configuration_manager.CreateConfiguration(layer_manager, "Configuration C");
    EXPECT_EQ(3, configuration_manager.available_configurations.size());
    EXPECT_EQ(0, configuration_manager.GetConfigurationIndex("Configuration A"));
    EXPECT_EQ(1, configuration_manager.GetConfigurationIndex("Configuration B"));
    EXPECT_EQ(2, configuration_manager.GetConfigurationIndex("Configuration C"));

    std::string configuration_a2_key = configuration_manager.CreateConfiguration(layer_manager, "Configuration A").key;
    EXPECT_EQ(4, configuration_manager.available_configurations.size());
    EXPECT_EQ(0, configuration_manager.GetConfigurationIndex("Configuration A"));
    EXPECT_EQ(1, configuration_manager.GetConfigurationIndex("Configuration A (2)"));
    EXPECT_EQ(2, configuration_manager.GetConfigurationIndex("Configuration B"));
    EXPECT_EQ(3, configuration_manager.GetConfigurationIndex("Configuration C"));

    EXPECT_EQ(-1, configuration_manager.GetConfigurationIndex("Configuration D"));
}
