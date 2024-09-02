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

    EXPECT_EQ(14, layer_manager.Size());
    EXPECT_TRUE(!layer_manager.Empty());

    layer_manager.Clear();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, load_dir) {
    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers");
    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_reference_1_1_0", Version::LATEST) != nullptr);

    layer_manager.Clear();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, load_file) {
    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers/VK_LAYER_LUNARG_reference_1_1_0.json");

    EXPECT_TRUE(!layer_manager.Empty());
    EXPECT_EQ(1, layer_manager.Size());

    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_reference_1_1_0", Version::LATEST) != nullptr);

    layer_manager.Clear();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, reset) {
    LayerManager layer_manager;

    layer_manager.LoadLayersFromPath(":/layers");
    EXPECT_FALSE(layer_manager.Empty());

    layer_manager.Reset();
    EXPECT_TRUE(layer_manager.Empty());
}

TEST(test_layer_manager, find_single) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    EXPECT_TRUE(layer_manager.Find("VK_LAYER_LUNARG_test_03", Version::LATEST) != nullptr);

    const Layer* layer = layer_manager.Find("VK_LAYER_LUNARG_reference_1_1_0", Version::LATEST);
    EXPECT_TRUE(layer != nullptr);
    EXPECT_STREQ(layer->key.c_str(), "VK_LAYER_LUNARG_reference_1_1_0");
}

TEST(test_layer_manager, find_multiple) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    const Layer* layer135 = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 1, 135));
    EXPECT_TRUE(layer135 != nullptr);
    EXPECT_STREQ(layer135->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer135->api_version, Version(1, 1, 135));

    const Layer* layer193 = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 2, 193));
    EXPECT_TRUE(layer193 != nullptr);
    EXPECT_STREQ(layer193->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer193->api_version, Version(1, 2, 193));

    const Layer* layer204 = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 3, 204));
    EXPECT_TRUE(layer204 != nullptr);
    EXPECT_STREQ(layer204->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer204->api_version, Version(1, 3, 204));

    const Layer* layer290 = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 3, 290));
    EXPECT_TRUE(layer290 != nullptr);
    EXPECT_STREQ(layer290->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer290->api_version, Version(1, 3, 290));

    const Layer* layer_latest = layer_manager.Find("VK_LAYER_LUNARG_version", Version::LATEST);
    EXPECT_TRUE(layer_latest != nullptr);
    EXPECT_STREQ(layer_latest->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer_latest->api_version, Version(1, 3, 290));

    const Layer* layer_version_not_found = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 3, 208));
    EXPECT_TRUE(layer_version_not_found != nullptr);
    EXPECT_TRUE(layer_version_not_found == layer_latest);

    const Layer* layer_not_found_key = layer_manager.Find("VK_LAYER_LUNARG_version_not_found", Version(1, 3, 290));
    EXPECT_TRUE(layer_not_found_key == nullptr);
}

TEST(test_layer_manager, FindFromManifest) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    const Layer* layer204 = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json");
    EXPECT_TRUE(layer204 != nullptr);
    EXPECT_STREQ(layer204->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer204->api_version, Version(1, 3, 204));

    const Layer* layer208 = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_208.json");
    EXPECT_TRUE(layer208 == nullptr);
}

TEST(test_layer_manager, GatherVersions) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    const std::vector<Version>& versions_notfound = layer_manager.GatherVersions("VK_LAYER_LUNARG_version_not_found");
    EXPECT_TRUE(versions_notfound.empty());

    const std::vector<Version>& versions_found = layer_manager.GatherVersions("VK_LAYER_LUNARG_version");
    EXPECT_FALSE(versions_found.empty());
    EXPECT_EQ(versions_found.size(), 4);
    EXPECT_EQ(versions_found[0], Version(1, 3, 290));
    EXPECT_EQ(versions_found[1], Version(1, 3, 204));
    EXPECT_EQ(versions_found[2], Version(1, 2, 193));
    EXPECT_EQ(versions_found[3], Version(1, 1, 135));

    const Layer* layer193 = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 2, 193));
    EXPECT_TRUE(layer193 != nullptr);

    Layer* layer_edit = layer_manager.FindFromManifest(layer193->manifest_path);
    layer_edit->visible = false;

    const std::vector<Version>& versions_found_b = layer_manager.GatherVersions("VK_LAYER_LUNARG_version");
    EXPECT_FALSE(versions_found_b.empty());
    EXPECT_EQ(versions_found_b.size(), 3);
    EXPECT_EQ(versions_found_b[0], Version(1, 3, 290));
    EXPECT_EQ(versions_found_b[1], Version(1, 3, 204));
    EXPECT_EQ(versions_found_b[2], Version(1, 1, 135));
}

TEST(test_layer_manager, BuildLayerNameList) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    EXPECT_EQ(layer_manager.BuildLayerNameList().size(), 11);
}

TEST(test_layer_manager, avoid_duplicate) {
    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers");
    std::size_t first_load_count = layer_manager.Size();

    layer_manager.LoadLayersFromPath(":/layers");
    std::size_t second_load_count = layer_manager.Size();

    EXPECT_EQ(first_load_count, second_load_count);
}
