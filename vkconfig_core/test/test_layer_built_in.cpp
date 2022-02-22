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

#include "../layer.h"
#include "../util.h"
#include "../setting_flags.h"

#include <gtest/gtest.h>

// Layers 130

TEST(test_layer_built_in, layer_130_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/130/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(5, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_130_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/130/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_130_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/130/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_130_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/130/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

// Layers 135

TEST(test_layer_built_in, layer_135_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/135/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(5, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_135_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/135/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_135_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/135/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_135_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/135/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

// Layers 141

TEST(test_layer_built_in, layer_141_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/141/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(5, CountSettings(layer.settings));
    EXPECT_EQ(5, layer.presets.size());
}

TEST(test_layer_built_in, layer_141_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/141/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_141_gfxreconstruct) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/141/VK_LAYER_LUNARG_gfxreconstruct.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(20, CountSettings(layer.settings));
    EXPECT_EQ(2, layer.presets.size());
}

TEST(test_layer_built_in, layer_141_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/141/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_141_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/141/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

// Layers 148

TEST(test_layer_built_in, layer_148_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/148/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(8, CountSettings(layer.settings));
    EXPECT_EQ(5, layer.presets.size());
}

TEST(test_layer_built_in, layer_148_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/148/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_148_gfxreconstruct) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/148/VK_LAYER_LUNARG_gfxreconstruct.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(20, CountSettings(layer.settings));
    EXPECT_EQ(2, layer.presets.size());
}

TEST(test_layer_built_in, layer_148_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/148/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_148_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/148/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

// Layers 154

TEST(test_layer_built_in, layer_154_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/154/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(8, CountSettings(layer.settings));
    EXPECT_EQ(6, layer.presets.size());
}

TEST(test_layer_built_in, layer_154_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/154/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_154_gfxreconstruct) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/154/VK_LAYER_LUNARG_gfxreconstruct.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(20, CountSettings(layer.settings));
    EXPECT_EQ(2, layer.presets.size());
}

TEST(test_layer_built_in, layer_154_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/154/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_154_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/154/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

// Layers 162

TEST(test_layer_built_in, layer_162_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/162/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(12, CountSettings(layer.settings));
    EXPECT_EQ(6, layer.presets.size());
}

TEST(test_layer_built_in, layer_162_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/162/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_162_gfxreconstruct) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/162/VK_LAYER_LUNARG_gfxreconstruct.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(21, CountSettings(layer.settings));
    EXPECT_EQ(2, layer.presets.size());
}

TEST(test_layer_built_in, layer_162_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/162/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_162_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/162/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

// Layers 170

TEST(test_layer_built_in, layer_170_validation) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/170/VK_LAYER_KHRONOS_validation.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(12, CountSettings(layer.settings));
    EXPECT_EQ(6, layer.presets.size());
    EXPECT_TRUE(static_cast<const SettingDataFlags*>(FindSetting(layer.presets[0].settings, "enables"))->value.empty());
    EXPECT_STREQ("VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT",
                 static_cast<const SettingDataFlags*>(FindSetting(layer.presets[0].settings, "disables"))->value[0].c_str());
}

TEST(test_layer_built_in, layer_170_synchronization2) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/170/VK_LAYER_KHRONOS_synchronization2.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(1, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_170_api_dump) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/170/VK_LAYER_LUNARG_api_dump.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(14, CountSettings(layer.settings));
    EXPECT_EQ(4, layer.presets.size());
}

TEST(test_layer_built_in, layer_170_gfxreconstruct) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/170/VK_LAYER_LUNARG_gfxreconstruct.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(21, CountSettings(layer.settings));
    EXPECT_EQ(2, layer.presets.size());
}

TEST(test_layer_built_in, layer_170_monitor) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/170/VK_LAYER_LUNARG_monitor.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(0, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer_built_in, layer_170_screenshot) {
    Layer layer;
    EXPECT_TRUE(layer.Load(std::vector<Layer>(), ":/layers/170/VK_LAYER_LUNARG_screenshot.json", LAYER_TYPE_EXPLICIT));
    EXPECT_EQ(3, CountSettings(layer.settings));
    EXPECT_EQ(0, layer.presets.size());
}
