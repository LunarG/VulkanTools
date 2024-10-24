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

#include "../configuration.h"
#include "../layer_manager.h"
#include "../util.h"

#include <array>
#include <string>

#include <gtest/gtest.h>

static bool operator==(const Configuration& a, const Configuration& b) {
    if (a.key != b.key)
        return false;
    else if (a.platform_flags != b.platform_flags)
        return false;
    else if (a.parameters != b.parameters)
        return false;
    return true;
}

static bool operator!=(const Configuration& a, const Configuration& b) { return !(a == b); }

static bool operator==(const Parameter& a, const Parameter& b) {
    if (a.key != b.key) return false;

    if (a.control != b.control) return false;

    if (a.settings.size() != b.settings.size()) return false;

    if (a.setting_tree_state != b.setting_tree_state) return false;

    for (std::size_t i = 0, n = a.settings.size(); i < n; ++i) {
        if (a.settings[i] != b.settings[i]) return false;
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

TEST(test_configuration, load_and_save) {
    LayerManager layers;

    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/test/Configuration 3.0.0.json", layers);
    EXPECT_TRUE(load_loaded);
    EXPECT_EQ(3, configuration_loaded.parameters.size());

    Parameter* parameter = configuration_loaded.Find("VK_LAYER_LUNARG_reference_1_2_1");
    EXPECT_TRUE(parameter != nullptr);
    EXPECT_EQ(LAYER_CONTROL_ON, parameter->control);

    configuration_loaded.Save("test_layer_1_2_1.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_layer_1_2_1.json", layers);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

static std::vector<Configuration> GenerateConfigurations() {
    std::vector<Configuration> configurations;

    {
        Configuration configuration;
        configuration.key = "Duplicate Configuration";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "Duplicate Configuration (2)";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "My Configuration";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "Old Configuration (3)";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "Old Configuration (1)";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "Tag Configuration (tag) (3)";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "Tag Configuration (tag)";
        configurations.push_back(configuration);
    }

    {
        Configuration configuration;
        configuration.key = "Tag Configuration Bla";
        configurations.push_back(configuration);
    }

    return configurations;
}

TEST(test_configuration, make_first_duplicate_name) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "My Configuration");

    EXPECT_STREQ("My Configuration (2)", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, make_additional_duplicate_name) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "Duplicate Configuration");

    EXPECT_STREQ("Duplicate Configuration (3)", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, make_extra_duplicate_name) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "Old Configuration (2)");

    EXPECT_STREQ("Old Configuration (4)", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, make_new_name) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "New Configuration");

    EXPECT_STREQ("New Configuration", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, make_new_name_tagged) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "New Configuration (tag)");

    EXPECT_STREQ("New Configuration (tag)", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, make_duplicate_tagged_name) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "Tag Configuration Bla (tag)");

    EXPECT_STREQ("Tag Configuration Bla (tag)", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, make_duplicate_tagged_name_mix) {
    const std::vector<Configuration>& configurations = GenerateConfigurations();

    const std::string& new_name = MakeConfigurationName(configurations, "Tag Configuration (tag) Bla");

    EXPECT_STREQ("Tag Configuration (tag) Bla", new_name.c_str());
    EXPECT_TRUE(!new_name.empty());
}

TEST(test_configuration, create) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::Create(layer_manager, "New Configuration");

    bool has_app_api = false;
    bool has_app_env = false;

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        if (configuration.parameters[i].control == LAYER_CONTROL_APPLICATIONS_API) {
            has_app_api = true;
            continue;
        }

        if (configuration.parameters[i].control == LAYER_CONTROL_APPLICATIONS_ENV) {
            has_app_env = true;
            continue;
        }

        EXPECT_EQ(configuration.parameters[i].control, LAYER_CONTROL_AUTO);
    }

    EXPECT_EQ(has_app_api, true);
    EXPECT_EQ(has_app_env, true);
}

TEST(test_configuration, create_disabled) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::CreateDisabled(layer_manager);

    bool has_app_api = false;
    bool has_app_env = false;

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        if (configuration.parameters[i].control == LAYER_CONTROL_APPLICATIONS_API) {
            has_app_api = true;
            continue;
        }

        if (configuration.parameters[i].control == LAYER_CONTROL_APPLICATIONS_ENV) {
            has_app_env = true;
            continue;
        }

        EXPECT_EQ(configuration.parameters[i].control, LAYER_CONTROL_OFF);
    }

    EXPECT_EQ(has_app_api, true);
    EXPECT_EQ(has_app_env, true);
}

TEST(test_configuration, SwitchLayerVersion) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::Create(layer_manager, "New Configuration");
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        EXPECT_EQ(configuration.parameters[i].api_version, Version::LATEST);
    }
    Parameter* parameter_latest = configuration.Find("VK_LAYER_LUNARG_version");
    EXPECT_EQ(parameter_latest->api_version, Version::LATEST);

    configuration.SwitchLayerVersion(layer_manager, "VK_LAYER_LUNARG_version", Version(1, 3, 204));
    Parameter* parameter_204 = configuration.Find("VK_LAYER_LUNARG_version");
    EXPECT_EQ(parameter_204->api_version, Version(1, 3, 204));

    configuration.SwitchLayerVersion(layer_manager, "VK_LAYER_LUNARG_version", Version::LATEST);
    Parameter* parameter_restore = configuration.Find("VK_LAYER_LUNARG_version");
    EXPECT_EQ(parameter_restore->api_version, Version::LATEST);

    configuration.SwitchLayerVersion(layer_manager, "VK_LAYER_LUNARG_version", Version(1, 3, 205));
    Parameter* parameter_version_not_found = configuration.Find("VK_LAYER_LUNARG_version");
    EXPECT_EQ(parameter_version_not_found->api_version, Version::LATEST);
}

TEST(test_configuration, gather_parameters_exist) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration;
    configuration.key = "New Configuration";

    configuration.GatherParameters(layer_manager);

    EXPECT_STREQ(configuration.parameters[0].key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
    EXPECT_STREQ(configuration.parameters[1].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_0");
    EXPECT_STREQ(configuration.parameters[2].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_1");

    EXPECT_STREQ(configuration.parameters[3].key.c_str(), "VK_LAYER_LUNARG_test_00");
    EXPECT_STREQ(configuration.parameters[4].key.c_str(), "VK_LAYER_LUNARG_test_01");
    EXPECT_STREQ(configuration.parameters[5].key.c_str(), "VK_LAYER_LUNARG_test_02");
    EXPECT_STREQ(configuration.parameters[6].key.c_str(), "VK_LAYER_LUNARG_test_03");
    EXPECT_STREQ(configuration.parameters[7].key.c_str(), "VK_LAYER_LUNARG_test_04");
    EXPECT_STREQ(configuration.parameters[8].key.c_str(), "VK_LAYER_LUNARG_test_05");
    EXPECT_STREQ(configuration.parameters[9].key.c_str(), "VK_LAYER_LUNARG_test_06");

    EXPECT_STREQ(configuration.parameters[10].key.c_str(), "VK_LAYER_LUNARG_version");

    std::string missing_layer;
    EXPECT_FALSE(HasMissingLayer(configuration.parameters, layer_manager, missing_layer));
    EXPECT_TRUE(missing_layer.empty());
}

TEST(test_configuration, gather_parameters_repeat) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration;
    configuration.key = "New Configuration";

    configuration.GatherParameters(layer_manager);
    configuration.GatherParameters(layer_manager);  // Again, check for no duplication!

    EXPECT_STREQ(configuration.parameters[0].key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
    EXPECT_STREQ(configuration.parameters[1].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_0");
    EXPECT_STREQ(configuration.parameters[2].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_1");

    EXPECT_STREQ(configuration.parameters[3].key.c_str(), "VK_LAYER_LUNARG_test_00");
    EXPECT_STREQ(configuration.parameters[4].key.c_str(), "VK_LAYER_LUNARG_test_01");
    EXPECT_STREQ(configuration.parameters[5].key.c_str(), "VK_LAYER_LUNARG_test_02");
    EXPECT_STREQ(configuration.parameters[6].key.c_str(), "VK_LAYER_LUNARG_test_03");
    EXPECT_STREQ(configuration.parameters[7].key.c_str(), "VK_LAYER_LUNARG_test_04");
    EXPECT_STREQ(configuration.parameters[8].key.c_str(), "VK_LAYER_LUNARG_test_05");
    EXPECT_STREQ(configuration.parameters[9].key.c_str(), "VK_LAYER_LUNARG_test_06");

    EXPECT_STREQ(configuration.parameters[10].key.c_str(), "VK_LAYER_LUNARG_version");
}

TEST(test_configuration, gather_parameters_missing) {
    LayerManager layer_manager;

    Configuration configuration;
    configuration.key = "New Configuration";

    layer_manager.LoadLayersFromPath(":/layers");
    configuration.GatherParameters(layer_manager);

    layer_manager.Clear();
    configuration.GatherParameters(layer_manager);

    EXPECT_STREQ(configuration.parameters[0].key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
    EXPECT_STREQ(configuration.parameters[1].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_0");
    EXPECT_STREQ(configuration.parameters[2].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_1");

    EXPECT_STREQ(configuration.parameters[3].key.c_str(), "VK_LAYER_LUNARG_test_00");
    EXPECT_STREQ(configuration.parameters[4].key.c_str(), "VK_LAYER_LUNARG_test_01");
    EXPECT_STREQ(configuration.parameters[5].key.c_str(), "VK_LAYER_LUNARG_test_02");
    EXPECT_STREQ(configuration.parameters[6].key.c_str(), "VK_LAYER_LUNARG_test_03");
    EXPECT_STREQ(configuration.parameters[7].key.c_str(), "VK_LAYER_LUNARG_test_04");
    EXPECT_STREQ(configuration.parameters[8].key.c_str(), "VK_LAYER_LUNARG_test_05");
    EXPECT_STREQ(configuration.parameters[9].key.c_str(), "VK_LAYER_LUNARG_test_06");

    EXPECT_STREQ(configuration.parameters[10].key.c_str(), "VK_LAYER_LUNARG_version");

    std::string missing_layer;
    EXPECT_TRUE(HasMissingLayer(configuration.parameters, layer_manager, missing_layer));
    EXPECT_STREQ(missing_layer.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
}

TEST(test_configuration, HasMissingLayer_UnsupportPlatform) {
    LayerManager layer_manager;

    Configuration configuration;
    configuration.key = "New Configuration";

    layer_manager.LoadLayersFromPath(":/layers");
    configuration.GatherParameters(layer_manager);

    layer_manager.Clear();

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        configuration.parameters[i].platform_flags = 0;
    }

    std::string missing_layer;
    EXPECT_FALSE(HasMissingLayer(configuration.parameters, layer_manager, missing_layer));
    EXPECT_TRUE(missing_layer.empty());
}

TEST(test_configuration, HasMissingLayer_Off) {
    LayerManager layer_manager;

    Configuration configuration;
    configuration.key = "New Configuration";

    layer_manager.LoadLayersFromPath(":/layers");
    configuration.GatherParameters(layer_manager);

    layer_manager.Clear();

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        configuration.parameters[i].control = LAYER_CONTROL_OFF;
    }

    std::string missing_layer;
    EXPECT_FALSE(HasMissingLayer(configuration.parameters, layer_manager, missing_layer));
    EXPECT_TRUE(missing_layer.empty());
}

TEST(test_configuration, gather_parameters_missing_but_unsupported_platform) {
    LayerManager layer_manager;

    Configuration configuration;
    configuration.key = "New Configuration";

    layer_manager.LoadLayersFromPath(":/layers");
    configuration.GatherParameters(layer_manager);

    layer_manager.Clear();

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        configuration.parameters[i].platform_flags = 0;
    }

    std::string missing_layer;
    EXPECT_FALSE(HasMissingLayer(configuration.parameters, layer_manager, missing_layer));
    EXPECT_TRUE(missing_layer.empty());
}

TEST(test_configuration, Reorder_full) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::Create(layer_manager, "New Configuration");

    std::vector<std::string> layer_names;
    layer_names.push_back("VK_LAYER_LUNARG_test_04");
    layer_names.push_back("VK_LAYER_LUNARG_test_01");
    layer_names.push_back("VK_LAYER_LUNARG_test_02");
    layer_names.push_back("Vulkan Layers from the Application Vulkan API");
    layer_names.push_back("Vulkan Layers from the Application Environment Variables");
    layer_names.push_back("VK_LAYER_LUNARG_version");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_2_1");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_2_0");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_1_0");
    layer_names.push_back("VK_LAYER_LUNARG_test_00");
    layer_names.push_back("VK_LAYER_LUNARG_test_05");
    layer_names.push_back("VK_LAYER_LUNARG_test_06");
    layer_names.push_back("VK_LAYER_LUNARG_test_03");

    configuration.Reorder(layer_names);

    EXPECT_STREQ(configuration.parameters[0].key.c_str(), "VK_LAYER_LUNARG_test_04");
    EXPECT_STREQ(configuration.parameters[1].key.c_str(), "VK_LAYER_LUNARG_test_01");
    EXPECT_STREQ(configuration.parameters[2].key.c_str(), "VK_LAYER_LUNARG_test_02");
    EXPECT_STREQ(configuration.parameters[3].key.c_str(), "Vulkan Layers from the Application Vulkan API");
    EXPECT_STREQ(configuration.parameters[4].key.c_str(), "Vulkan Layers from the Application Environment Variables");
    EXPECT_STREQ(configuration.parameters[5].key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_STREQ(configuration.parameters[6].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_1");
    EXPECT_STREQ(configuration.parameters[7].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_0");
    EXPECT_STREQ(configuration.parameters[8].key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
    EXPECT_STREQ(configuration.parameters[9].key.c_str(), "VK_LAYER_LUNARG_test_00");
    EXPECT_STREQ(configuration.parameters[10].key.c_str(), "VK_LAYER_LUNARG_test_05");
    EXPECT_STREQ(configuration.parameters[11].key.c_str(), "VK_LAYER_LUNARG_test_06");
    EXPECT_STREQ(configuration.parameters[12].key.c_str(), "VK_LAYER_LUNARG_test_03");
}

TEST(test_configuration, Reorder_partial) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::Create(layer_manager, "New Configuration");

    std::vector<std::string> layer_names;
    layer_names.push_back("VK_LAYER_LUNARG_test_04");
    layer_names.push_back("Vulkan Layers from the Application Vulkan API");
    layer_names.push_back("VK_LAYER_LUNARG_version");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_2_1");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_2_0");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_1_0");
    layer_names.push_back("VK_LAYER_LUNARG_test_06");
    layer_names.push_back("VK_LAYER_LUNARG_test_03");

    configuration.Reorder(layer_names);

    EXPECT_STREQ(configuration.parameters[0].key.c_str(), "VK_LAYER_LUNARG_test_04");
    EXPECT_STREQ(configuration.parameters[1].key.c_str(), "Vulkan Layers from the Application Vulkan API");
    EXPECT_STREQ(configuration.parameters[2].key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_STREQ(configuration.parameters[3].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_1");
    EXPECT_STREQ(configuration.parameters[4].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_0");
    EXPECT_STREQ(configuration.parameters[5].key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
    EXPECT_STREQ(configuration.parameters[6].key.c_str(), "VK_LAYER_LUNARG_test_06");
    EXPECT_STREQ(configuration.parameters[7].key.c_str(), "VK_LAYER_LUNARG_test_03");
}

TEST(test_configuration, Reorder_missing) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::Create(layer_manager, "New Configuration");

    std::vector<std::string> layer_names;
    layer_names.push_back("VK_LAYER_LUNARG_test_04");
    layer_names.push_back("Vulkan Layers from the Application Vulkan API");
    layer_names.push_back("VK_LAYER_LUNARG_missing_0");
    layer_names.push_back("VK_LAYER_LUNARG_version");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_2_1");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_2_0");
    layer_names.push_back("VK_LAYER_LUNARG_reference_1_1_0");
    layer_names.push_back("VK_LAYER_LUNARG_test_06");
    layer_names.push_back("VK_LAYER_LUNARG_test_03");
    layer_names.push_back("VK_LAYER_LUNARG_missing_1");

    configuration.Reorder(layer_names);

    EXPECT_STREQ(configuration.parameters[0].key.c_str(), "VK_LAYER_LUNARG_test_04");
    EXPECT_STREQ(configuration.parameters[1].key.c_str(), "Vulkan Layers from the Application Vulkan API");
    EXPECT_STREQ(configuration.parameters[2].key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_STREQ(configuration.parameters[3].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_1");
    EXPECT_STREQ(configuration.parameters[4].key.c_str(), "VK_LAYER_LUNARG_reference_1_2_0");
    EXPECT_STREQ(configuration.parameters[5].key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
    EXPECT_STREQ(configuration.parameters[6].key.c_str(), "VK_LAYER_LUNARG_test_06");
    EXPECT_STREQ(configuration.parameters[7].key.c_str(), "VK_LAYER_LUNARG_test_03");
}

TEST(test_configuration, Reorder_empty) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    Configuration configuration = Configuration::Create(layer_manager, "New Configuration");
    std::size_t size = configuration.parameters.size();

    std::vector<std::string> layer_names;
    configuration.Reorder(layer_names);

    EXPECT_EQ(configuration.parameters.size(), size);
}
