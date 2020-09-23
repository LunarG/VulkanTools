/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include <gtest/gtest.h>

static bool operator==(const Configuration& a, const Configuration& b) {
    if (a.name != b.name)
        return false;
    else if (a._description != b._description)
        return false;
    else if (a._setting_tree_state != b._setting_tree_state)
        return false;
    else if (a._preset != b._preset)
        return false;
    else if (a.parameters != b.parameters)
        return false;
    return true;
}

static bool operator!=(const Configuration& a, const Configuration& b) { return !(a == b); }

static bool operator==(const LayerSetting& a, const LayerSetting& b) {
    if (a.key != b.key)
        return false;
    else if (a.label != b.label)
        return false;
    else if (a.description != b.description)
        return false;
    else if (a.type != b.type)
        return false;
    else if (a.max_value != b.max_value)
        return false;
    else if (a.min_value != b.min_value)
        return false;
    else if (a.exclusive_values != b.exclusive_values)
        return false;
    else if (a.exclusive_labels != b.exclusive_labels)
        return false;
    else if (a.inclusive_values != b.inclusive_values)
        return false;
    else if (a.inclusive_labels != b.inclusive_labels)
        return false;
    else if (a.value != b.value)
        return false;
    return true;
}

static bool operator!=(const LayerSetting& a, const LayerSetting& b) { return !(a == b); }

static bool operator==(const std::vector<Parameter>& a, const std::vector<Parameter>& b) {
    if (a.size() != b.size()) return false;

    for (std::size_t i = 0, n = a.size(); i < n; ++i) {
        if (a[i].name != b[i].name) return false;

        if (a[i].state != b[i].state) return false;

        if (a[i].settings.size() != b[i].settings.size()) return false;

        for (std::size_t j = 0, o = a[i].settings.size(); j < o; ++j) {
            if (a[i].settings[j] != b[i].settings[j]) return false;
        }
    }

    return true;
}

static bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }

TEST(test_configuration, ctor) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.1 - API dump.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());

    Configuration configuration_copy(configuration_loaded);

    EXPECT_EQ(configuration_loaded, configuration_copy);
}

TEST(test_configuration, load_and_save_v2_0_1_api_dump) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.1 - API dump.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetNone, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_api_dump");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    configuration_loaded.name = "Api Dump";
    configuration_loaded.Save("test_v2_0_1_api_dump.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_1_api_dump.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_1_frame_capture) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.1 - Frame Capture.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetNone, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_gfxreconstruct");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    configuration_loaded.name = "Frame Capture";
    configuration_loaded.Save("test_v2_0_1_frame_capture.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_1_frame_capture.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_2_frame_capture) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.2 - Frame Capture.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetNone, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_gfxreconstruct");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    EXPECT_STREQ("Frame Capture - Range (F5 to start and to stop)", configuration_loaded._name.toStdString().c_str());
    configuration_loaded.Save("test_v2_0_2_frame_capture.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_2_frame_capture.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_1_gpu_assisted) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.1 - GPU-Assisted.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetGPUAssisted, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    configuration_loaded.name = "GPU-Assisted";
    configuration_loaded.Save("test_v2_0_1_gpu_assisted.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_1_gpu_assisted.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_2_gpu_assisted) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.2 - GPU-Assisted.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetGPUAssisted, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    EXPECT_STREQ("Validation - GPU-Assisted", configuration_loaded.name.toStdString().c_str());
    configuration_loaded.Save("test_v2_0_2_gpu_assisted.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_2_gpu_assisted.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_1_shader_printf) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.1 - Shader Printf.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetDebugPrintf, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    configuration_loaded.name = "shader-printf";
    configuration_loaded.Save("test_v2_0_1_shader_printf.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_1_shader_printf.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_2_debug_printf) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.2 - Debug Printf.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetDebugPrintf, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    EXPECT_STREQ("Validation - Debug Printf", configuration_loaded.name.toStdString().c_str());
    configuration_loaded.Save("test_v2_0_2_shader_printf.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_2_shader_printf.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_1_best_practices) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.1 - Best Practices.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetBestPractices, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    configuration_loaded.name = "best-practices";
    configuration_loaded.Save("test_v2_0_1_best_practices.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_1_best_practices.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_2_best_practices) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.2 - Best Practices.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(1, configuration_loaded.parameters.size());
    EXPECT_EQ(ValidationPresetBestPractices, configuration_loaded._preset);
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_loaded.parameters.end());

    EXPECT_STREQ("Validation - Best Practices", configuration_loaded.name.toStdString().c_str());
    configuration_loaded.Save("test_v2_0_2_best_practices.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_2_best_practices.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_2_override_all_layers) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.2 - Override all layers.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(6, configuration_loaded.parameters.size());
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter_validation = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter_validation != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter_validation->state);

    auto parameter_api_dump = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_api_dump");
    ASSERT_TRUE(parameter_api_dump != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter_api_dump->state);

    auto parameter_device_simulation = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_device_simulation");
    ASSERT_TRUE(parameter_device_simulation != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter_device_simulation->state);

    auto parameter_gfxreconstruct = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_gfxreconstruct");
    ASSERT_TRUE(parameter_gfxreconstruct != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter_gfxreconstruct->state);

    auto parameter_monitor = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_monitor");
    ASSERT_TRUE(parameter_monitor != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter_monitor->state);

    auto parameter_screenshot = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_screenshot");
    ASSERT_TRUE(parameter_screenshot != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_OVERRIDDEN, parameter_screenshot->state);

    configuration_loaded.Save("test_v2_0_2_override_all_layers.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_2_override_all_layers.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_and_save_v2_0_2_exclude_all_layers) {
    Configuration configuration_loaded;
    const bool load_loaded = configuration_loaded.Load(":/Configuration 2.0.2 - Exclude all layers.json");
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!configuration_loaded.IsEmpty());
    EXPECT_EQ(6, configuration_loaded.parameters.size());
    EXPECT_TRUE(!configuration_loaded._description.isEmpty());

    auto parameter_validation = FindParameter(configuration_loaded.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter_validation != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_EXCLUDED, parameter_validation->state);

    auto parameter_api_dump = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_api_dump");
    ASSERT_TRUE(parameter_api_dump != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_EXCLUDED, parameter_api_dump->state);

    auto parameter_device_simulation = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_device_simulation");
    ASSERT_TRUE(parameter_device_simulation != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_EXCLUDED, parameter_device_simulation->state);

    auto parameter_gfxreconstruct = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_gfxreconstruct");
    ASSERT_TRUE(parameter_gfxreconstruct != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_EXCLUDED, parameter_gfxreconstruct->state);

    auto parameter_monitor = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_monitor");
    ASSERT_TRUE(parameter_monitor != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_EXCLUDED, parameter_monitor->state);

    auto parameter_screenshot = FindParameter(configuration_loaded.parameters, "VK_LAYER_LUNARG_screenshot");
    ASSERT_TRUE(parameter_screenshot != configuration_loaded.parameters.end());
    EXPECT_EQ(LAYER_STATE_EXCLUDED, parameter_screenshot->state);

    configuration_loaded.Save("test_v2_0_2_exclude_all_layers.json");

    Configuration configuration_saved;
    configuration_saved.Load("test_v2_0_2_exclude_all_layers.json");

    EXPECT_EQ(configuration_loaded, configuration_saved);
}

TEST(test_configuration, load_standard) {
    Configuration configuration;
    const bool load = configuration.Load(":/Configuration 2.0.2 - Standard.json");
    ASSERT_TRUE(load);
    ASSERT_TRUE(!configuration.IsEmpty());

    EXPECT_STREQ("Validation - Standard", configuration.name.toStdString().c_str());
    EXPECT_EQ(1, configuration.parameters.size());
    EXPECT_EQ(ValidationPresetStandard, configuration._preset);
    EXPECT_TRUE(!configuration._description.isEmpty());
}

TEST(test_configuration, compare_version_standard) {
    Configuration configuration_2_0_1;
    const bool load_2_0_1 = configuration_2_0_1.Load(":/Configuration 2.0.1 - Standard.json");
    ASSERT_TRUE(load_2_0_1);
    ASSERT_TRUE(!configuration_2_0_1.IsEmpty());
    EXPECT_STREQ("Configuration 2.0.1 - Standard", configuration_2_0_1.name.toStdString().c_str());

    Configuration configuration_2_0_2;
    const bool load_2_0_2 = configuration_2_0_2.Load(":/Configuration 2.0.2 - Standard.json");
    ASSERT_TRUE(load_2_0_2);
    ASSERT_TRUE(!configuration_2_0_2.IsEmpty());
    EXPECT_STREQ("Validation - Standard", configuration_2_0_2.name.toStdString().c_str());

    EXPECT_TRUE(configuration_2_0_1.parameters == configuration_2_0_2.parameters);
}

TEST(test_configuration, compare_version_debug_printf) {
    Configuration configuration_2_0_1;
    const bool load_2_0_1 = configuration_2_0_1.Load(":/Configuration 2.0.1 - Shader Printf.json");
    ASSERT_TRUE(load_2_0_1);
    ASSERT_TRUE(!configuration_2_0_1.IsEmpty());
    EXPECT_STREQ("Configuration 2.0.1 - Shader Printf", configuration_2_0_1.name.toStdString().c_str());

    Configuration configuration_2_0_2;
    const bool load_2_0_2 = configuration_2_0_2.Load(":/Configuration 2.0.2 - Debug Printf.json");
    ASSERT_TRUE(load_2_0_2);
    ASSERT_TRUE(!configuration_2_0_2.IsEmpty());
    EXPECT_STREQ("Validation - Debug Printf", configuration_2_0_2.name.toStdString().c_str());

    EXPECT_TRUE(configuration_2_0_1.parameters == configuration_2_0_2.parameters);
}

TEST(test_configuration, compare_settings) {
    Configuration configuration_standard;
    const bool load_standard = configuration_standard.Load(":/Configuration 2.0.2 - Standard.json");
    ASSERT_TRUE(load_standard);
    ASSERT_TRUE(!configuration_standard.IsEmpty());
    EXPECT_STREQ("Validation - Standard", configuration_standard.name.toStdString().c_str());

    Configuration configuration_best_practices;
    const bool load_best_practices = configuration_best_practices.Load(":/Configuration 2.0.2 - Best Practices.json");
    ASSERT_TRUE(load_best_practices);
    ASSERT_TRUE(!configuration_best_practices.IsEmpty());
    EXPECT_STREQ("Validation - Best Practices", configuration_best_practices.name.toStdString().c_str());

    EXPECT_TRUE(configuration_standard.parameters != configuration_best_practices.parameters);
}

TEST(test_configuration, find_layer_parameter) {
    Configuration configuration_2_0_1;
    const bool load_2_0_1 = configuration_2_0_1.Load(":/Configuration 2.0.1 - Standard.json");
    ASSERT_TRUE(load_2_0_1);

    auto parameter = FindParameter(configuration_2_0_1.parameters, "VK_LAYER_KHRONOS_validation");
    ASSERT_TRUE(parameter != configuration_2_0_1.parameters.end());

    EXPECT_STREQ(parameter->name.toStdString().c_str(), "VK_LAYER_KHRONOS_validation");
}
