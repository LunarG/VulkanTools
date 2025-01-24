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

#include "../layer_manager.h"

#include <gtest/gtest.h>

TEST(test_layer_manager, clear) {
    LayerManager layer_manager;

    EXPECT_TRUE(layer_manager.validate_manifests == false);
    EXPECT_TRUE(layer_manager.Empty());
    EXPECT_TRUE(layer_manager.Size() == 0);
}

TEST(test_layer_manager, load_json) {
    qunsetenv("VULKAN_SDK");

    QJsonObject json_object;

    LayerManager layer_manager;
    bool result = layer_manager.Load(json_object);
    EXPECT_TRUE(result);
}

TEST(test_layer_manager, save_json) {
    QJsonObject json_root_object;

    LayerManager layer_manager;
    bool result = layer_manager.Save(json_root_object);
    EXPECT_TRUE(result);

    EXPECT_TRUE(json_root_object.value("layers") != QJsonValue::Undefined);

    if (json_root_object.value("layers") != QJsonValue::Undefined) {
        const QJsonObject& json_layers_object = json_root_object.value("layers").toObject();

        EXPECT_TRUE(json_layers_object.value("validated") != QJsonValue::Undefined);
        EXPECT_TRUE(json_layers_object.value("paths") != QJsonValue::Undefined);
    }
}

TEST(test_layer_manager, load_all) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    EXPECT_EQ(16, layer_manager.Size());
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
    qunsetenv("VULKAN_SDK");

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

    layer_manager.Clear();
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

TEST(test_layer_manager, reload) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    EXPECT_TRUE(layer_manager.Size() > 1);

    const std::size_t initial_size = layer_manager.Size();

    LayerLoadStatus status1 = layer_manager.LoadLayer(":/layers/VK_LAYER_LUNARG_test_04.json");
    EXPECT_EQ(status1, LAYER_LOAD_RELOADED);

    Layer* layer1 = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_test_04.json", false);
    layer1->enabled = false;

    const std::size_t reloaded_size1 = layer_manager.Size();
    EXPECT_EQ(initial_size, reloaded_size1);

    LayerLoadStatus status2 = layer_manager.LoadLayer(":/layers/VK_LAYER_LUNARG_test_04.json");
    EXPECT_EQ(status2, LAYER_LOAD_RELOADED);

    const std::size_t reloaded_size2 = layer_manager.Size();
    EXPECT_EQ(initial_size, reloaded_size2);
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

TEST(test_layer_manager, FindLastModified) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");
    const std::size_t initial_count = layer_manager.Size();

    Layer* layer204_modified = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", false);
    layer204_modified->validated_last_modified = "1";
    Path modified_path = layer204_modified->manifest_path;
    layer204_modified->manifest_path = ":/layers/VK_LAYER_LUNARG_version_204_copy.json";

    layer_manager.LoadLayersFromPath(":/layers");
    const std::size_t reloaded_count = layer_manager.Size();
    EXPECT_EQ(initial_count + 1, reloaded_count);

    Layer* layer204 = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", false);
    layer204->validated_last_modified = "0";

    Layer* layer204_first = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204_copy.json", false);
    Layer* layer204_copy = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", false);

    const Layer* last0 = layer_manager.FindLastModified("VK_LAYER_LUNARG_version", Version(1, 3, 204));
    EXPECT_EQ(layer204_first, last0);

    layer204_copy->validated_last_modified = "2";
    const Layer* last1 = layer_manager.FindLastModified("VK_LAYER_LUNARG_version", Version(1, 3, 204));
    EXPECT_EQ(layer204_copy, last1);
}

TEST(test_layer_manager, FindFromManifest) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");
    const std::size_t initial_count = layer_manager.Size();

    Layer* layer204 = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", false);
    EXPECT_TRUE(layer204 != nullptr);
    EXPECT_STREQ(layer204->key.c_str(), "VK_LAYER_LUNARG_version");
    EXPECT_EQ(layer204->api_version, Version(1, 3, 204));
    EXPECT_TRUE(layer204->enabled);

    Layer* layer208 = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_208.json", false);
    EXPECT_TRUE(layer208 == nullptr);

    layer204->enabled = false;
    Layer* layer204_disabledA = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", false);
    EXPECT_TRUE(layer204_disabledA == nullptr);
    Layer* layer204_disabledB =
        layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", true);  // find disabled layers anyway
    EXPECT_TRUE(layer204_disabledB != nullptr);
    EXPECT_FALSE(layer204_disabledB->enabled);

    layer204_disabledB->enabled = true;
    Layer* layer204_disabledC = layer_manager.FindFromManifest(":/layers/VK_LAYER_LUNARG_version_204.json", false);
    EXPECT_TRUE(layer204_disabledC != nullptr);
    EXPECT_TRUE(layer204_disabledC->enabled);

    layer_manager.LoadLayersFromPath(":/layers");
    const std::size_t enabled_count = layer_manager.Size();
    EXPECT_EQ(initial_count, enabled_count);

    layer204->enabled = false;
    layer_manager.LoadLayersFromPath(":/layers");
    const std::size_t disabled_count = layer_manager.Size();
    EXPECT_EQ(initial_count, disabled_count);
}

TEST(test_layer_manager, GatherManifests) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    const std::vector<Path>& versions_notfound = layer_manager.GatherManifests("VK_LAYER_LUNARG_version_not_found");
    EXPECT_TRUE(versions_notfound.empty());

    const std::vector<Path>& versions_found = layer_manager.GatherManifests("VK_LAYER_LUNARG_version");
    EXPECT_FALSE(versions_found.empty());
    EXPECT_EQ(versions_found.size(), 4);

    const Layer* layer0 = layer_manager.FindFromManifest(versions_found[0]);
    EXPECT_EQ(layer0->api_version, Version(1, 3, 290));
    const Layer* layer1 = layer_manager.FindFromManifest(versions_found[1]);
    EXPECT_EQ(layer1->api_version, Version(1, 3, 204));
    const Layer* layer2 = layer_manager.FindFromManifest(versions_found[2]);
    EXPECT_EQ(layer2->api_version, Version(1, 2, 193));
    const Layer* layer3 = layer_manager.FindFromManifest(versions_found[3]);
    EXPECT_EQ(layer3->api_version, Version(1, 1, 135));

    const Layer* layer193 = layer_manager.Find("VK_LAYER_LUNARG_version", Version(1, 2, 193));
    EXPECT_TRUE(layer193 != nullptr);

    Layer* layer_edit = layer_manager.FindFromManifest(layer193->manifest_path);
    layer_edit->enabled = false;

    const std::vector<Path>& versions_found_b = layer_manager.GatherManifests("VK_LAYER_LUNARG_version");
    EXPECT_FALSE(versions_found_b.empty());
    EXPECT_EQ(versions_found_b.size(), 3);
    const Layer* layerB0 = layer_manager.FindFromManifest(versions_found_b[0]);
    EXPECT_EQ(layerB0->api_version, Version(1, 3, 290));
    const Layer* layerB1 = layer_manager.FindFromManifest(versions_found_b[1]);
    EXPECT_EQ(layerB1->api_version, Version(1, 3, 204));
    const Layer* layerB2 = layer_manager.FindFromManifest(versions_found_b[2]);
    EXPECT_EQ(layerB2->api_version, Version(1, 1, 135));
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
    layer_edit->enabled = false;

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

    EXPECT_EQ(layer_manager.GatherLayerNames().size(), 13);
}

TEST(test_layer_manager, avoid_duplicate) {
    qunsetenv("VULKAN_SDK");

    LayerManager layer_manager;
    EXPECT_TRUE(layer_manager.Empty());

    layer_manager.LoadLayersFromPath(":/layers");
    std::size_t first_load_count = layer_manager.Size();

    layer_manager.LoadLayersFromPath(":/layers");
    std::size_t second_load_count = layer_manager.Size();

    EXPECT_EQ(first_load_count, second_load_count);
}

TEST(test_layer_manager, custom_path_append_remove) {
    LayerManager layer_manager;

    LayersPathInfo infoA;
    infoA.path = ":/layers";
    LayersPathInfo infoB;
    infoB.path = ":/layersB";
    LayersPathInfo infoC;
    infoC.path = ":/layersC";

    layer_manager.AppendPath(infoA);
    layer_manager.AppendPath(infoB);
    layer_manager.AppendPath(infoC);

    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 3);

    layer_manager.AppendPath(infoA);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 3);

    layer_manager.RemovePath(infoA);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 2);

    layer_manager.AppendPath(infoA);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 3);

    layer_manager.RemovePath(infoA);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 2);

    layer_manager.RemovePath(infoA);  // Check that removing an already removed path doesn't cause any issue
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 2);

    layer_manager.RemovePath(infoB);
    layer_manager.RemovePath(infoC);

    EXPECT_TRUE(layer_manager.paths[LAYERS_PATHS_GUI].empty());
}

TEST(test_layer_manager, custom_path_update_layers) {
    LayerManager layer_manager;
    layer_manager.LoadLayersFromPath(":/layers");

    LayersPathInfo info;
    info.path = ":/layers";
    // info.enabled = true; default value

    layer_manager.AppendPath(info);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 1);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI][0].enabled, true);
    for (std::size_t i = 0, n = layer_manager.available_layers.size(); i < n; ++i) {
        EXPECT_TRUE(layer_manager.available_layers[i].enabled);
    }

    info.enabled = false;
    layer_manager.UpdatePathEnabled(info, LAYERS_PATHS_GUI);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI][0].enabled, false);
    for (std::size_t i = 0, n = layer_manager.available_layers.size(); i < n; ++i) {
        EXPECT_FALSE(layer_manager.available_layers[i].enabled);
    }

    info.enabled = true;
    layer_manager.UpdatePathEnabled(info, LAYERS_PATHS_GUI);
    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI][0].enabled, true);
    for (std::size_t i = 0, n = layer_manager.available_layers.size(); i < n; ++i) {
        EXPECT_TRUE(layer_manager.available_layers[i].enabled);
    }

    EXPECT_EQ(layer_manager.paths[LAYERS_PATHS_GUI].size(), 1);
}
