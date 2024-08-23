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

#include "../ui.h"

#include <gtest/gtest.h>

TEST(test_ui, ExtractLayerName_no_layer_loaded) {
    LayerManager layer_manager;

    const std::string& not_found = ExtractLayerName(layer_manager, " = VK_LAYER_KHRONOS_validation");
    EXPECT_TRUE(not_found.empty());
}

TEST(test_ui, ExtractLayerName_no_found) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/sdk");

    const std::string& not_found = ExtractLayerName(layer_manager, " = VK_LAYER_KHRONOS_unkown");
    EXPECT_TRUE(not_found.empty());
}

TEST(test_ui, ExtractLayerName_found) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/sdk");

    const std::string& layer_found = ExtractLayerName(layer_manager, " = VK_LAYER_KHRONOS_validation");
    EXPECT_STREQ("VK_LAYER_KHRONOS_validation", layer_found.c_str());
}

TEST(test_ui, GetMainWindowTitle) {
    std::string title = GetMainWindowTitle(false, false);

    EXPECT_STREQ(("Vulkan Configurator " + Version::VKCONFIG.str()).c_str(), title.c_str());
}

TEST(test_ui, GetMainWindowTitle_active) {
    std::string title = GetMainWindowTitle(true, false);

    EXPECT_STREQ(("Vulkan Configurator " + Version::VKCONFIG.str() + " <ACTIVE>").c_str(), title.c_str());
}
