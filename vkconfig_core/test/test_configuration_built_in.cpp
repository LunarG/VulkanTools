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

    if (a.settings.Size() != b.settings.Size()) return false;

    for (std::size_t i = 0, n = a.settings.Size(); i < n; ++i) {
        if (a.settings[i] != b.settings[i]) return false;
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

struct TestBuilin {
    TestBuilin(const char* layers_version) : layers_version(layers_version), environment(paths), layer_manager(environment) {
        this->layer_manager.LoadLayersFromPath(format(":/layers/%s", layers_version).c_str());
        EXPECT_TRUE(!this->layer_manager.available_layers.empty());
    }

    ~TestBuilin() {
        environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
    }

    Configuration Load(const char* configuration_name) {
        Configuration configuration_loaded;
        EXPECT_TRUE(configuration_loaded.Load(layer_manager.available_layers,
                                              format(":/configurations/%s.json", configuration_name).c_str()));
        EXPECT_TRUE(!configuration_loaded.parameters.empty());
        return configuration_loaded;
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
    PathManager paths;
    Environment environment;
    LayerManager layer_manager;
};

// Layers 130

TEST(test_built_in_load, layers_130_with_api_dump) {
    TestBuilin test("130");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_130_with_frame_capture) {
    TestBuilin test("130");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_130_with_portability) {
    TestBuilin test("130");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_130_with_synchronization) {
    TestBuilin test("130");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_130_with_validation) {
    TestBuilin test("130");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

// Layers 135

TEST(test_built_in_load, layers_135_with_api_dump) {
    TestBuilin test("135");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_135_with_frame_capture) {
    TestBuilin test("135");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_135_with_portability) {
    TestBuilin test("135");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_135_with_synchronization) {
    TestBuilin test("135");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_135_with_validation) {
    TestBuilin test("135");
    EXPECT_EQ(5, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

// Layers 141

TEST(test_built_in_load, layers_141_with_api_dump) {
    TestBuilin test("141");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_141_with_frame_capture) {
    TestBuilin test("141");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_141_with_portability) {
    TestBuilin test("141");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_141_with_synchronization) {
    TestBuilin test("141");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_141_with_validation) {
    TestBuilin test("141");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

// Layers 148

TEST(test_built_in_load, layers_148_with_api_dump) {
    TestBuilin test("148");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_148_with_frame_capture) {
    TestBuilin test("148");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_148_with_portability) {
    TestBuilin test("148");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_148_with_synchronization) {
    TestBuilin test("148");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_148_with_validation) {
    TestBuilin test("148");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

// Layers 154

TEST(test_built_in_load, layers_154_with_api_dump) {
    TestBuilin test("154");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_154_with_frame_capture) {
    TestBuilin test("154");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_154_with_portability) {
    TestBuilin test("154");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_154_with_synchronization) {
    TestBuilin test("154");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_154_with_validation) {
    TestBuilin test("154");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

// Layers 162

TEST(test_built_in_load, layers_162_with_api_dump) {
    TestBuilin test("162");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_162_with_frame_capture) {
    TestBuilin test("162");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_162_with_portability) {
    TestBuilin test("162");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_162_with_synchronization) {
    TestBuilin test("162");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_162_with_validation) {
    TestBuilin test("162");
    EXPECT_EQ(6, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    Configuration configuration_saved = test.Restore(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

// Layers Latest

TEST(test_built_in_load, layers_latest_with_api_dump) {
    TestBuilin test("latest");
    EXPECT_EQ(7, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("API dump");
    EXPECT_EQ(1, configuration_loaded.parameters.size());

    Configuration configuration_saved = test.Restore(configuration_loaded);
    EXPECT_EQ(1, configuration_saved.parameters.size());

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_latest_with_frame_capture) {
    TestBuilin test("latest");
    EXPECT_EQ(7, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Frame Capture");
    EXPECT_EQ(1, configuration_loaded.parameters.size());

    Configuration configuration_saved = test.Restore(configuration_loaded);
    EXPECT_EQ(1, configuration_saved.parameters.size());

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_latest_with_portability) {
    TestBuilin test("latest");
    EXPECT_EQ(7, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Portability");
    EXPECT_EQ(7, configuration_loaded.parameters.size());

    Configuration configuration_saved = test.Restore(configuration_loaded);
    EXPECT_EQ(7, configuration_saved.parameters.size());

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_latest_with_synchronization) {
    TestBuilin test("latest");
    EXPECT_EQ(7, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Synchronization");
    EXPECT_EQ(2, configuration_loaded.parameters.size());

    Configuration configuration_saved = test.Restore(configuration_loaded);
    EXPECT_EQ(2, configuration_saved.parameters.size());

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_built_in_load, layers_latest_with_validation) {
    TestBuilin test("latest");
    EXPECT_EQ(7, test.layer_manager.available_layers.size());

    Configuration configuration_loaded = test.Load("Validation");
    EXPECT_EQ(1, configuration_loaded.parameters.size());

    Configuration configuration_saved = test.Restore(configuration_loaded);
    EXPECT_EQ(1, configuration_saved.parameters.size());

    EXPECT_EQ(configuration_loaded, configuration_saved);
}
