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

#include "../layer_manager.h"
#include "../environment.h"

#include <gtest/gtest.h>

TEST(test_layer_manager, clear) {
    LayerManager layer_manager;

    EXPECT_TRUE(layer_manager.Empty());
    EXPECT_TRUE(layer_manager.Size() == 0);
}

TEST(test_layer_manager, load_all) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");
    layer_manager.LoadAllInstalledLayers();

    EXPECT_TRUE(layer_manager.Size() >= 10);
    EXPECT_TRUE(!layer_manager.Empty());

    layer_manager.Clear();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, load_dir) {
    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers");

    EXPECT_TRUE(!layer_manager.Empty());
    EXPECT_EQ(10, layer_manager.Size());

    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_reference_1_1_0") != nullptr);

    layer_manager.Clear();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, load_file) {
    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers/VK_LAYER_LUNARG_reference_1_1_0.json");

    EXPECT_TRUE(!layer_manager.Empty());
    EXPECT_EQ(1, layer_manager.Size());

    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_reference_1_1_0") != nullptr);

    layer_manager.Clear();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, find) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");
    EXPECT_TRUE(layer_manager.Empty());

    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_reference_1_1_0") != nullptr);
    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_test_03") != nullptr);
}

TEST(test_layer_manager, avoid_duplicate) {
    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers");
    EXPECT_EQ(10, layer_manager.Size());

    layer_manager.LoadLayersFromPath(":/layers");
    EXPECT_EQ(10, layer_manager.Size());
}
