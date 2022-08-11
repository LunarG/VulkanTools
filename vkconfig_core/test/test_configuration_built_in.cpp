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

    if (a.state != b.state) return false;

    if (a.settings.size() != b.settings.size()) return false;

    for (std::size_t i = 0, n = a.settings.size(); i < n; ++i) {
        if (*a.settings[i] != *b.settings[i]) return false;
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

struct TestBuilin {
    TestBuilin(const char* layers_version, const char* configurations_version)
        : paths(""),
          layers_version(layers_version),
          configurations_version(configurations_version),
          environment(paths),
          layer_manager(environment) {
        this->layer_manager.LoadLayersFromPath(format(":/layers/%s", layers_version).c_str());
        EXPECT_TRUE(!this->layer_manager.available_layers.empty());
    }

    ~TestBuilin() {
        environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
    }

    Configuration Load(const char* configuration_name) {
        Configuration configuration_loaded;
        const bool result = configuration_loaded.Load(
            layer_manager.available_layers,
            format(":/configurations/%s/%s.json", configurations_version.c_str(), configuration_name).c_str());
        return result ? configuration_loaded : Configuration();
    }

    Configuration Restore(const Configuration& configuration_loaded) {
        const std::string filename = format("test_%s_layers_%s.json", configuration_loaded.key.c_str(), layers_version.c_str());
        const bool saved = configuration_loaded.Save(this->layer_manager.available_layers, filename.c_str());
        EXPECT_TRUE(saved);

        Configuration configuration_saved;
        EXPECT_TRUE(configuration_saved.Load(this->layer_manager.available_layers, filename.c_str()));
        return configuration_saved;
    }

    std::string layers_version;
    std::string configurations_version;
    PathManager paths;
    Environment environment;
    LayerManager layer_manager;
};

static const char* CONFIGURATION_VERSION = "2.2.2";

// Layers 130

TEST(test_built_in_load, layers_130_with_configuration) {
    TestBuilin test("130", CONFIGURATION_VERSION);
    EXPECT_EQ(4, test.layer_manager.available_layers.size());

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

// Layers 135

TEST(test_built_in_load, layers_135_with_configuration) {
    TestBuilin test("135", CONFIGURATION_VERSION);
    EXPECT_EQ(4, test.layer_manager.available_layers.size());

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

// Layers 141

TEST(test_built_in_load, layers_141_with_configuration) {
    TestBuilin test("141", CONFIGURATION_VERSION);
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

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

// Layers 148

TEST(test_built_in_load, layers_148_with_configuration) {
    TestBuilin test("148", CONFIGURATION_VERSION);
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

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

// Layers 154

TEST(test_built_in_load, layers_154_with_configuration) {
    TestBuilin test("154", CONFIGURATION_VERSION);
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

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

// Layers 162

TEST(test_built_in_load, layers_162_with_configuration) {
    TestBuilin test("162", CONFIGURATION_VERSION);
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

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

// Layers 170

TEST(test_built_in_load, layers_170_with_configuration) {
    TestBuilin test("170", CONFIGURATION_VERSION);
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

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
