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

#include "../configuration_manager.h"

#include <gtest/gtest.h>

TEST(test_configuration_manager, init_default) {
    ConfigurationManager configuration_manager;

    const Configuration* info_unknown = configuration_manager.FindConfiguration("unknown");
    EXPECT_TRUE(info_unknown == nullptr);
    EXPECT_TRUE(configuration_manager.Empty());

    EXPECT_STREQ(configuration_manager.last_path_import_config.RelativePath().c_str(), Path(Path::HOME).RelativePath().c_str());
    EXPECT_STREQ(configuration_manager.last_path_export_config.RelativePath().c_str(), Path(Path::HOME).RelativePath().c_str());
}

TEST(test_configuration_manager, create_remove) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

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
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

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
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

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
    EXPECT_EQ(configuration_ori->selected_layer_name, configuration_dup->selected_layer_name);
    EXPECT_EQ(configuration_ori->parameters.size(), configuration_dup->parameters.size());
    EXPECT_EQ(configuration_ori->IsDefault(), configuration_dup->IsDefault());
    EXPECT_EQ(configuration_ori->Size(), configuration_dup->Size());
}

TEST(test_configuration_manager, sort) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

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

TEST(test_configuration_manager, default_configuration) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

    ConfigurationManager configuration_manager;
    configuration_manager.LoadDefaultConfigurations(layer_manager);

    EXPECT_EQ(configuration_manager.IsDefaultConfiguration("Validation"), true);
    EXPECT_EQ(configuration_manager.IsDefaultConfiguration("Unknown"), false);
}

TEST(test_configuration_manager, RenameConfiguration) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

    ConfigurationManager configuration_manager;
    configuration_manager.LoadDefaultConfigurations(layer_manager);

    const Configuration* configuration1 = configuration_manager.FindConfiguration("Validation");
    EXPECT_TRUE(configuration1 != nullptr);

    configuration_manager.RenameConfiguration("Validation", "ValidationAHKJDGKJ");
    const Configuration* configuration2 = configuration_manager.FindConfiguration("Validation");
    EXPECT_TRUE(configuration2 == nullptr);

    const Configuration* configuration3 = configuration_manager.FindConfiguration("ValidationAHKJDGKJ");
    EXPECT_TRUE(configuration3 != nullptr);
    EXPECT_FALSE(configuration_manager.HasFile(*configuration3));

    configuration_manager.RenameConfiguration("ValidationAHKJDGKJ", "Validation");
    const Configuration* configuration4 = configuration_manager.FindConfiguration("Validation");
    EXPECT_TRUE(configuration4 != nullptr);
}

TEST(test_configuration_manager, SaveConfiguration) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

    ConfigurationManager configuration_manager;
    EXPECT_TRUE(configuration_manager.Empty());

    configuration_manager.LoadDefaultConfigurations(layer_manager);
    EXPECT_FALSE(configuration_manager.Empty());

    configuration_manager.RenameConfiguration("Validation", "ValidationAHKJDGKJ");

    std::vector<Configuration> available_configurations;
    for (std::size_t i = 0, n = configuration_manager.available_configurations.size(); i < n; ++i) {
        if (configuration_manager.available_configurations[i].key != "ValidationAHKJDGKJ") {
            continue;
        }
        available_configurations.push_back(configuration_manager.available_configurations[i]);
    }
    std::swap(configuration_manager.available_configurations, available_configurations);

    const Configuration* configuration = configuration_manager.FindConfiguration("ValidationAHKJDGKJ");
    EXPECT_FALSE(configuration_manager.HasFile(*configuration));

    configuration_manager.SaveAllConfigurations();
    EXPECT_TRUE(configuration_manager.HasFile(*configuration));

    configuration_manager.RemoveConfiguration("ValidationAHKJDGKJ");
}

TEST(test_configuration_manager, ExportImport) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);

    ConfigurationManager configuration_manager;
    configuration_manager.LoadDefaultConfigurations(layer_manager);
    std::size_t count_init = configuration_manager.available_configurations.size();

    configuration_manager.ExportConfiguration(layer_manager, Path(Path::HOME).RelativePath() + "/ValidationExported.json",
                                              "Validation");

    std::string configuration_name;
    configuration_manager.ImportConfiguration(layer_manager, Path(Path::HOME).RelativePath() + "/ValidationExported.json",
                                              configuration_name);
    EXPECT_EQ(count_init + 1, configuration_manager.available_configurations.size());

    const Configuration* configuration = configuration_manager.FindConfiguration("Validation (Imported)");
    EXPECT_TRUE(configuration != nullptr);
}

TEST(test_configuration_manager, UpdateConfigurations) {
    LayerManager layer_manager;

    ConfigurationManager configuration_manager;
    configuration_manager.LoadDefaultConfigurations(layer_manager);

    std::size_t count_init = configuration_manager.available_configurations.size();
    EXPECT_TRUE(count_init > 0);

    const Configuration* configuration_init = configuration_manager.FindConfiguration("Validation");
    std::vector<std::string> missing_layers_init;
    const bool init = configuration_init->HasMissingLayer(layer_manager, missing_layers_init);
    EXPECT_TRUE(init);
    EXPECT_EQ(1, missing_layers_init.size());
    EXPECT_STREQ(missing_layers_init[0].c_str(), "VK_LAYER_KHRONOS_validation");
    EXPECT_EQ(2, configuration_init->parameters.size());

    layer_manager.LoadLayersFromPath(":/sdk", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);
    configuration_manager.UpdateConfigurations(layer_manager);

    std::size_t count_update = configuration_manager.available_configurations.size();
    EXPECT_TRUE(count_update > 0);

    const Configuration* configuration_update = configuration_manager.FindConfiguration("Validation");
    std::vector<std::string> missing_layers_update;
    const bool update = configuration_update->HasMissingLayer(layer_manager, missing_layers_update);
    EXPECT_FALSE(update);
    EXPECT_TRUE(missing_layers_update.empty());
    EXPECT_TRUE(configuration_init->parameters.size() > 2);

    EXPECT_EQ(count_init, count_update);
}
