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

#include "../configuration.h"
#include "../util.h"

#include <array>
#include <string>

#include <gtest/gtest.h>

static bool operator==(const Configuration& a, const Configuration& b) {
    if (a.key != b.key)
        return false;
    else if (a.platform_flags != b.platform_flags)
        return false;
    else if (a.description != b.description)
        return false;
    else if (a.setting_tree_state != b.setting_tree_state)
        return false;
    else if (a.parameters != b.parameters)
        return false;
    return true;
}

static bool operator!=(const Configuration& a, const Configuration& b) { return !(a == b); }

static bool operator==(const Parameter& a, const Parameter& b) {
    if (a.key != b.key) return false;

    if (a.state != b.state) return false;

    if (a.settings.size() != b.settings.size()) return false;

    for (std::size_t i = 0, n = a.settings.size(); i < n; ++i) {
        if (a.settings[i] != b.settings[i]) return false;
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

TEST(test_configuration, load_and_save_v2_2_2) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(std::vector<Layer>(), ":/Configuration 2.2.2.json");
    EXPECT_TRUE(load_loaded);
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_TRUE(!configuration_loaded.description.empty());

    Parameter* parameter = FindByKey(configuration_loaded.parameters, "VK_LAYER_LUNARG_reference_1_2_1");
    EXPECT_TRUE(parameter != nullptr);
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter->state);

    configuration_loaded.Save(std::vector<Layer>(), "test_v2_2_2_layer_1_2_1.json");

    Configuration configuration_saved;
    configuration_saved.Load(std::vector<Layer>(), "test_v2_2_2_layer_1_2_1.json");

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
