/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
        this->layer_manager.LoadLayersFromPath(":/sdk");
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

    {
        Configuration load_api_dump = test.Load("API dump");
        Configuration save_api_dump = test.Restore(load_api_dump);
        EXPECT_STREQ(save_api_dump.key.c_str(), load_api_dump.key.c_str());
        EXPECT_EQ(save_api_dump.version, load_api_dump.version);
        EXPECT_EQ(save_api_dump.platform_flags, load_api_dump.platform_flags);
        EXPECT_EQ(save_api_dump.parameters.size(), load_api_dump.parameters.size());
        for (std::size_t i = 0, n = save_api_dump.parameters.size(); i < n; ++i) {
            EXPECT_EQ(save_api_dump.parameters[i], load_api_dump.parameters[i]);
        }
    }

    {
        Configuration load_frame_capture = test.Load("Frame Capture");
        Configuration save_frame_capture = test.Restore(load_frame_capture);
        EXPECT_EQ(save_frame_capture, load_frame_capture);
    }

    {
        Configuration load_portability = test.Load("Portability");
        Configuration save_portability = test.Restore(load_portability);
        EXPECT_EQ(save_portability, load_portability);
    }

    {
        Configuration load_synchronization = test.Load("Synchronization");
        Configuration save_synchronization = test.Restore(load_synchronization);
        EXPECT_EQ(save_synchronization, load_synchronization);
    }

    {
        Configuration load_validation = test.Load("Validation");
        Configuration save_validation = test.Restore(load_validation);
        EXPECT_EQ(save_validation, load_validation);
    }
}
