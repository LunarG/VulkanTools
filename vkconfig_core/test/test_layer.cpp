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

#include "../layer.h"
#include "../util.h"

#include <gtest/gtest.h>

TEST(test_layer, collect_settings) {
    std::vector<LayerSettingMeta> meta;

    EXPECT_TRUE(CollectDefaultSettingData(std::vector<LayerSettingMeta>()).empty());

    LayerSettingMeta meta0;
    meta0.key = "key0";
    meta0.default_value = "value0";
    meta.push_back(meta0);

    std::vector<LayerSettingData> data = CollectDefaultSettingData(meta);

    LayerSettingData* data0 = FindByKey(data, "key0");
    EXPECT_STREQ("value0", data0->value.c_str());
}

TEST(test_layer, load_v1_X_0_layer_header) {
    static const char* SOURCES[] = {":/Layer 1.3.0.json", ":/Layer 1.4.0 - setting platforms.json"};

    static const char* FILE_FORMAT_VERSIONS[] = {"1.3.0", "1.4.0"};

    for (std::size_t i = 0, n = countof(SOURCES); i < n; ++i) {
        Layer layer;
        const bool load_loaded = layer.Load(SOURCES[i], LAYER_TYPE_EXPLICIT);
        ASSERT_TRUE(load_loaded);

        EXPECT_STREQ(FILE_FORMAT_VERSIONS[i], layer.file_format_version.str().c_str());

        EXPECT_STREQ("VK_LAYER_LUNARG_test", layer.key.c_str());
        EXPECT_STREQ("GLOBAL", layer._type.c_str());
        EXPECT_STREQ(".\\VkLayer_test.dll", layer._library_path.c_str());
        EXPECT_STREQ("1.2.162", layer._api_version.str().c_str());
        EXPECT_STREQ("2", layer._implementation_version.c_str());
        EXPECT_STREQ("test layer", layer.description.c_str());

        EXPECT_EQ(0, layer.presets.size());
    }
}

TEST(test_layer, load_v1_4_0_layer_setting_platform) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting platforms.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(PLATFORM_WINDOWS_BIT, FindByKey(layer.settings, "case0")->platform_flags);
    EXPECT_EQ(PLATFORM_LINUX_BIT, FindByKey(layer.settings, "case1")->platform_flags);
    EXPECT_EQ(PLATFORM_MACOS_BIT, FindByKey(layer.settings, "case2")->platform_flags);
    EXPECT_EQ(PLATFORM_ALL_BIT, FindByKey(layer.settings, "case3")->platform_flags);
    EXPECT_EQ(PLATFORM_ALL_BIT, FindByKey(layer.settings, "case4")->platform_flags);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, FindByKey(layer.settings, "case5")->platform_flags);
}

TEST(test_layer, load_v1_4_0_layer_setting_default_value) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(SETTING_COUNT, layer.settings.size());

    EXPECT_STREQ("value1", FindByKey(layer.settings, "ENUM")->default_value.c_str());
    EXPECT_STREQ("flag0,flag1", FindByKey(layer.settings, "FLAGS")->default_value.c_str());
    EXPECT_STREQ("A string", FindByKey(layer.settings, "STRING")->default_value.c_str());
    EXPECT_STREQ("TRUE", FindByKey(layer.settings, "BOOL")->default_value.c_str());
    EXPECT_STREQ("1", FindByKey(layer.settings, "BOOL_NUMERIC")->default_value.c_str());
    EXPECT_STREQ("./test.txt", FindByKey(layer.settings, "SAVE_FILE")->default_value.c_str());
    EXPECT_STREQ("./test", FindByKey(layer.settings, "SAVE_FOLDER")->default_value.c_str());
    EXPECT_STREQ("./test.txt", FindByKey(layer.settings, "LOAD_FILE")->default_value.c_str());
    EXPECT_STREQ("76", FindByKey(layer.settings, "INT")->default_value.c_str());
    EXPECT_STREQ("76-82", FindByKey(layer.settings, "INT_RANGE")->default_value.c_str());
    EXPECT_STREQ("stringB,stringC", FindByKey(layer.settings, "VUID_EXCLUDE")->default_value.c_str());
}

TEST(test_layer, load_v1_4_0_layer_preset) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - presets.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(2, layer.presets.size());

    EXPECT_STREQ("Preset0", layer.presets[0].label.c_str());
    EXPECT_STREQ("Description0", layer.presets[0].description.c_str());
    EXPECT_EQ(PLATFORM_ALL_BIT, layer.presets[0].platform_flags);
    EXPECT_EQ(STATUS_STABLE, layer.presets[0].status_type);
    EXPECT_STREQ("A string0", FindByKey(layer.presets[0].settings, "stringA")->value.c_str());

    EXPECT_STREQ("Preset1", layer.presets[1].label.c_str());
    EXPECT_STREQ("Description1", layer.presets[1].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, layer.presets[1].platform_flags);
    EXPECT_EQ(STATUS_BETA, layer.presets[1].status_type);
    EXPECT_STREQ("A string1", FindByKey(layer.presets[1].settings, "stringA")->value.c_str());
}
