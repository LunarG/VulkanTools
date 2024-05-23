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

    if (a.control != b.control) return false;

    if (a.settings.size() != b.settings.size()) return false;

    for (std::size_t i = 0, n = a.settings.size(); i < n; ++i) {
        if (*a.settings[i] != *b.settings[i]) return false;
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

struct TestBuilin {
    TestBuilin()
        : environment(),
          layer_manager(environment) {
        this->layer_manager.LoadLayersFromPath(":/sdk");
        EXPECT_TRUE(!this->layer_manager.selected_layers.empty());
    }

    ~TestBuilin() {
        environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
    }

    Configuration Load(const char* configuration_name) {
        Configuration configuration_loaded;
        const bool result = configuration_loaded.Load(
            layer_manager.selected_layers,
            format(":/configurations/%s.json", configuration_name).c_str());
        return result ? configuration_loaded : Configuration();
    }

    Configuration Restore(const Configuration& configuration_loaded) {
        const std::string filename = format("test_%s_layers.json", configuration_loaded.key.c_str());
        const bool saved = configuration_loaded.Save(this->layer_manager.selected_layers, filename.c_str());
        EXPECT_TRUE(saved);

        Configuration configuration_saved;
        EXPECT_TRUE(configuration_saved.Load(this->layer_manager.selected_layers, filename.c_str()));
        return configuration_saved;
    }

    std::string layers_version;
    std::string configurations_version;
    Environment environment;
    LayerManager layer_manager;
};

TEST(test_built_in_load, sdk_layers_with_configuration) {
    TestBuilin test;
    EXPECT_EQ(8, test.layer_manager.selected_layers.size());

    {
        Configuration load_api_dump = test.Load("API dump");
        EXPECT_EQ(1, load_api_dump.Size());
        Configuration save_api_dump = test.Restore(load_api_dump);
        EXPECT_EQ(save_api_dump, load_api_dump);
    }

    {
        Configuration load_frame_capture = test.Load("Frame Capture");
        EXPECT_EQ(1, load_frame_capture.Size());
        Configuration save_frame_capture = test.Restore(load_frame_capture);
        EXPECT_EQ(save_frame_capture, load_frame_capture);
    }

    {
        Configuration load_portability = test.Load("Portability");
        EXPECT_EQ(7, load_portability.Size());
        Configuration save_portability = test.Restore(load_portability);
        EXPECT_EQ(save_portability, load_portability);
    }

    {
        Configuration load_synchronization = test.Load("Synchronization");
        EXPECT_EQ(2, load_synchronization.Size());
        Configuration save_synchronization = test.Restore(load_synchronization);
        EXPECT_EQ(save_synchronization, load_synchronization);
    }

    {
        Configuration load_validation = test.Load("Validation");
        EXPECT_EQ(1, load_validation.Size());
        Configuration save_validation = test.Restore(load_validation);
        EXPECT_EQ(save_validation, load_validation);
    }
}
