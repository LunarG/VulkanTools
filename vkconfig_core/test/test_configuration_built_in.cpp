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

#include "../configuration.h"
#include "../util.h"
#include "../layer_manager.h"

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

    for (std::size_t i = 0, n = a.settings.size(); i < n; ++i) {
        if (*a.settings[i] != *b.settings[i]) return false;
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

std::map<std::string, std::string> Dummy() { return std::map<std::string, std::string>(); }

struct TestBuilin {
    TestBuilin() : layer_manager() {
        this->layer_manager.LoadLayersFromPath(":/sdk", LAYER_TYPE_EXPLICIT, CONFIGURATOR_MODE_CMD);
        EXPECT_TRUE(!this->layer_manager.available_layers.empty());
    }

    ~TestBuilin() {}

    Configuration Load(const char* configuration_name) {
        const std::string& path = format(":/configurations/%s.json", configuration_name);

        Configuration configuration_loaded;
        const bool result = configuration_loaded.Load(path.c_str(), this->layer_manager);
        return result ? configuration_loaded : Configuration();
    }

    Configuration Restore(const Configuration& configuration_loaded) {
        const std::string filename = format("test_%s_layers.json", configuration_loaded.key.c_str());
        const bool saved = configuration_loaded.Save(filename.c_str());
        EXPECT_TRUE(saved);

        Configuration configuration_saved;
        EXPECT_TRUE(configuration_saved.Load(filename.c_str(), this->layer_manager));
        return configuration_saved;
    }

    std::string layers_version;
    std::string configurations_version;
    LayerManager layer_manager;
};

TEST(test_built_in_load, sdk_layers_with_configuration) {
    TestBuilin test;
    EXPECT_TRUE(test.layer_manager.available_layers.size() >= 8);

    const std::string configurations[] = {"API dump", "Frame Capture", "Portability", "Validation"};

    for (std::size_t i = 0, n = std::size(configurations); i < n; ++i) {
        Configuration load_config = test.Load(configurations[i].c_str());
        Configuration save_config = test.Restore(load_config);
        EXPECT_STREQ(save_config.key.c_str(), load_config.key.c_str());
        EXPECT_EQ(save_config.version, load_config.version);
        EXPECT_EQ(save_config.platform_flags, load_config.platform_flags);
        EXPECT_EQ(save_config.parameters.size(), load_config.parameters.size());
        for (std::size_t i = 0, n = save_config.parameters.size(); i < n; ++i) {
            EXPECT_EQ(save_config.parameters[i].type, load_config.parameters[i].type);
            EXPECT_EQ(save_config.parameters[i].control, load_config.parameters[i].control);
            EXPECT_EQ(save_config.parameters[i].builtin, load_config.parameters[i].builtin);
            EXPECT_EQ(save_config.parameters[i].platform_flags, load_config.parameters[i].platform_flags);
            EXPECT_EQ(save_config.parameters[i].settings.size(), load_config.parameters[i].settings.size());
            if (save_config.parameters[i].settings.size() == load_config.parameters[i].settings.size()) {
                for (std::size_t j = 0, m = save_config.parameters[i].settings.size(); j < m; ++j) {
                    EXPECT_EQ(*save_config.parameters[i].settings[j], *load_config.parameters[i].settings[j]);
                }
            }
            EXPECT_EQ(save_config.parameters[i].overridden_rank, load_config.parameters[i].overridden_rank);
            EXPECT_EQ(save_config.parameters[i].api_version, load_config.parameters[i].api_version);
            EXPECT_STREQ(save_config.parameters[i].manifest.RelativePath().c_str(),
                         load_config.parameters[i].manifest.RelativePath().c_str());
            EXPECT_EQ(save_config.parameters[i].override_settings, load_config.parameters[i].override_settings);
        }
    }
}
