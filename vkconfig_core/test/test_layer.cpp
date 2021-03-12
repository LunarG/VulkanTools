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

#include <gtest/gtest.h>

TEST(test_layer, collect_settings) {
    SettingMetaSet meta;

    EXPECT_TRUE(CollectDefaultSettingData(meta).Empty());

    SettingMetaString& meta0 = static_cast<SettingMetaString&>(meta.Create("key0", SETTING_STRING));
    meta0.default_value = "value0";

    SettingDataSet data = CollectDefaultSettingData(meta);

    SettingDataString& data0 = *data.Get<SettingDataString>("key0");
    EXPECT_STREQ("value0", data0.value.c_str());
}

TEST(test_layer, load_1_X_0_layer_header) {
    static const char* SOURCES[] = {":/Layer 1.3.0.json", ":/Layer 1.4.0 - setting platforms.json"};

    static const char* FILE_FORMAT_VERSIONS[] = {"1.3.0", "1.4.0"};
    static const char* KEYS[] = {"VK_LAYER_LUNARG_test_1_3_0", "VK_LAYER_LUNARG_test_1_4_0_setting_platforms"};

    for (std::size_t i = 0, n = countof(SOURCES); i < n; ++i) {
        Layer layer;
        const bool load_loaded = layer.Load(SOURCES[i], LAYER_TYPE_EXPLICIT);
        ASSERT_TRUE(load_loaded);

        EXPECT_STREQ(FILE_FORMAT_VERSIONS[i], layer.file_format_version.str().c_str());

        EXPECT_STREQ(KEYS[i], layer.key.c_str());
        EXPECT_STREQ(".\\VkLayer_test.dll", layer._library_path.c_str());
        EXPECT_STREQ("1.2.162", layer._api_version.str().c_str());
        EXPECT_STREQ("2", layer._implementation_version.c_str());
        EXPECT_STREQ("test layer", layer.description.c_str());
        if (layer._api_version > Version(1, 3, 0)) {
            EXPECT_EQ(STATUS_BETA, layer.status);
            EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html", layer.url.c_str());
        }

        EXPECT_EQ(0, layer.presets.size());
    }
}

TEST(test_layer, load_1_4_0_layer_setting_platform) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting platforms.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(PLATFORM_WINDOWS_BIT, layer.settings.Get("case0")->platform_flags);
    EXPECT_EQ(PLATFORM_LINUX_BIT, layer.settings.Get("case1")->platform_flags);
    EXPECT_EQ(PLATFORM_MACOS_BIT, layer.settings.Get("case2")->platform_flags);
    EXPECT_EQ(PLATFORM_ALL_BIT, layer.settings.Get("case3")->platform_flags);
    EXPECT_EQ(PLATFORM_ALL_BIT, layer.settings.Get("case4")->platform_flags);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.settings.Get("case5")->platform_flags);
}

TEST(test_layer, load_1_4_0_layer_preset) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - presets.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(2, layer.presets.size());

    EXPECT_STREQ("Preset0", layer.presets[0].label.c_str());
    EXPECT_STREQ("Description0", layer.presets[0].description.c_str());
    EXPECT_EQ(PLATFORM_ALL_BIT, layer.presets[0].platform_flags);
    EXPECT_EQ(STATUS_STABLE, layer.presets[0].status);
    EXPECT_STREQ("A string0", layer.presets[0].settings.Get<SettingDataString>("stringA")->value.c_str());

    EXPECT_STREQ("Preset1", layer.presets[1].label.c_str());
    EXPECT_STREQ("Description1", layer.presets[1].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, layer.presets[1].platform_flags);
    EXPECT_EQ(STATUS_BETA, layer.presets[1].status);
    EXPECT_STREQ("A string1", layer.presets[1].settings.Get<SettingDataString>("stringA")->value.c_str());
}

TEST(test_layer, load_1_4_0_setting_enum_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaEnum* setting_meta = layer.settings.Get<SettingMetaEnum>("enum_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("enum_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_ENUM, setting_meta->type);
    EXPECT_STREQ("enum", setting_meta->label.c_str());
    EXPECT_STREQ("enum case", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_EQ(3, setting_meta->enum_values.size());

    SettingEnumValue value0;
    value0.key = "value0";
    value0.label = "Value0";
    value0.description = "My value0";
    EXPECT_EQ(value0, setting_meta->enum_values[0]);

    SettingEnumValue value1;
    value1.key = "value1";
    value1.label = "Value1";
    value1.description = "My value1";
    EXPECT_EQ(value1, setting_meta->enum_values[1]);

    SettingEnumValue value2;
    value2.key = "value2";
    value2.label = "Value2";
    value2.description = "My value2";
    EXPECT_EQ(value2, setting_meta->enum_values[2]);

    EXPECT_STREQ("value1", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_enum_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaEnum* setting_meta = layer.settings.Get<SettingMetaEnum>("enum_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("enum_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_ENUM, setting_meta->type);
    EXPECT_STREQ("enum", setting_meta->label.c_str());
    EXPECT_STREQ("enum case", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#enum", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, setting_meta->platform_flags);
    EXPECT_EQ(3, setting_meta->enum_values.size());

    SettingEnumValue value0;
    value0.key = "value0";
    value0.label = "Value0";
    value0.description = "My value0";
    value0.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#value0";
    value0.status = STATUS_STABLE;
    value0.platform_flags = PLATFORM_ALL_BIT;
    EXPECT_EQ(value0, setting_meta->enum_values[0]);

    SettingEnumValue value1;
    value1.key = "value1";
    value1.label = "Value1";
    value1.description = "My value1";
    value1.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#value1";
    value1.status = STATUS_BETA;
    value1.platform_flags = PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT;
    EXPECT_EQ(value1, setting_meta->enum_values[1]);

    SettingEnumValue value2;
    value2.key = "value2";
    value2.label = "Value2";
    value2.description = "My value2";
    value2.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#value2";
    value2.status = STATUS_ALPHA;
    value2.platform_flags = PLATFORM_WINDOWS_BIT;
    EXPECT_EQ(value2, setting_meta->enum_values[2]);

    EXPECT_STREQ("value1", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_flags_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFlags* setting_meta = layer.settings.Get<SettingMetaFlags>("flags_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("flags_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_FLAGS, setting_meta->type);
    EXPECT_STREQ("flags", setting_meta->label.c_str());
    EXPECT_STREQ("flags case", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_EQ(3, setting_meta->enum_values.size());

    SettingEnumValue value0;
    value0.key = "flag0";
    value0.label = "Flag0";
    value0.description = "My flag0";
    EXPECT_EQ(value0, setting_meta->enum_values[0]);

    SettingEnumValue value1;
    value1.key = "flag1";
    value1.label = "Flag1";
    value1.description = "My flag1";
    EXPECT_EQ(value1, setting_meta->enum_values[1]);

    SettingEnumValue value2;
    value2.key = "flag2";
    value2.label = "Flag2";
    value2.description = "My flag2";
    EXPECT_EQ(value2, setting_meta->enum_values[2]);

    EXPECT_STREQ("flag0", setting_meta->default_value[0].c_str());
    EXPECT_STREQ("flag1", setting_meta->default_value[1].c_str());
}

TEST(test_layer, load_1_4_0_setting_flags_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFlags* setting_meta = layer.settings.Get<SettingMetaFlags>("flags_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("flags_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_FLAGS, setting_meta->type);
    EXPECT_STREQ("flags", setting_meta->label.c_str());
    EXPECT_STREQ("flags case", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flags", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_EQ(3, setting_meta->enum_values.size());

    SettingEnumValue value0;
    value0.key = "flag0";
    value0.label = "Flag0";
    value0.description = "My flag0";
    value0.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flag0";
    value0.status = STATUS_STABLE;
    value0.platform_flags = PLATFORM_ALL_BIT;
    EXPECT_EQ(value0, setting_meta->enum_values[0]);

    SettingEnumValue value1;
    value1.key = "flag1";
    value1.label = "Flag1";
    value1.description = "My flag1";
    value1.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flag1";
    value1.status = STATUS_BETA;
    value1.platform_flags = PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT;
    EXPECT_EQ(value1, setting_meta->enum_values[1]);

    SettingEnumValue value2;
    value2.key = "flag2";
    value2.label = "Flag2";
    value2.description = "My flag2";
    value2.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flag2";
    value2.status = STATUS_ALPHA;
    value2.platform_flags = PLATFORM_WINDOWS_BIT;
    EXPECT_EQ(value2, setting_meta->enum_values[2]);

    EXPECT_STREQ("flag0", setting_meta->default_value[0].c_str());
    EXPECT_STREQ("flag1", setting_meta->default_value[1].c_str());
}

TEST(test_layer, load_1_4_0_setting_string_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaString* setting_meta = layer.settings.Get<SettingMetaString>("string_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("string_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_STRING, setting_meta->type);
    EXPECT_STREQ("String", setting_meta->label.c_str());
    EXPECT_STREQ("string", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("A string", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_string_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaString* setting_meta = layer.settings.Get<SettingMetaString>("string_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("string_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_STRING, setting_meta->type);
    EXPECT_STREQ("String", setting_meta->label.c_str());
    EXPECT_STREQ("string", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#string", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("A string", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_bool_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaBool* setting_meta = layer.settings.Get<SettingMetaBool>("bool_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("bool_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_BOOL, setting_meta->type);
    EXPECT_STREQ("bool", setting_meta->label.c_str());
    EXPECT_STREQ("true or false", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_EQ(true, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_bool_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaBool* setting_meta = layer.settings.Get<SettingMetaBool>("bool_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("bool_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_BOOL, setting_meta->type);
    EXPECT_STREQ("bool", setting_meta->label.c_str());
    EXPECT_STREQ("true or false", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#bool", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_EQ(true, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_load_file_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileLoad* setting_meta = layer.settings.Get<SettingMetaFileLoad>("load_file_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("load_file_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_LOAD_FILE, setting_meta->type);
    EXPECT_STREQ("Load file", setting_meta->label.c_str());
    EXPECT_STREQ("Load file path", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_TRUE(setting_meta->filter.empty());

    EXPECT_STREQ("./test.txt", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_load_file_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileLoad* setting_meta = layer.settings.Get<SettingMetaFileLoad>("load_file_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("load_file_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_LOAD_FILE, setting_meta->type);
    EXPECT_STREQ("Load file", setting_meta->label.c_str());
    EXPECT_STREQ("Load file path", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#load_file", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_STREQ("*.txt", setting_meta->filter.c_str());

    EXPECT_STREQ("./test.txt", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_file_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileSave* setting_meta = layer.settings.Get<SettingMetaFileSave>("save_file_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_file_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_SAVE_FILE, setting_meta->type);
    EXPECT_STREQ("Save file", setting_meta->label.c_str());
    EXPECT_STREQ("Save file path", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_TRUE(setting_meta->filter.empty());

    EXPECT_STREQ("./test.json", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_file_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileSave* setting_meta = layer.settings.Get<SettingMetaFileSave>("save_file_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_file_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_SAVE_FILE, setting_meta->type);
    EXPECT_STREQ("Save file", setting_meta->label.c_str());
    EXPECT_STREQ("Save file path", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_file", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_STREQ("*.json", setting_meta->filter.c_str());

    EXPECT_STREQ("./test.json", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_folder_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFolderSave* setting_meta = layer.settings.Get<SettingMetaFolderSave>("save_folder_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_folder_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_SAVE_FOLDER, setting_meta->type);
    EXPECT_STREQ("Save folder", setting_meta->label.c_str());
    EXPECT_STREQ("Save folder path", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("./test", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_folder_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFolderSave* setting_meta = layer.settings.Get<SettingMetaFolderSave>("save_folder_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_folder_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_SAVE_FOLDER, setting_meta->type);
    EXPECT_STREQ("Save folder", setting_meta->label.c_str());
    EXPECT_STREQ("Save folder path", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_folder", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("./test", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_int_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaInt* setting_meta = layer.settings.Get<SettingMetaInt>("int_required_only");
    ASSERT_TRUE(setting_meta != nullptr);

    EXPECT_STREQ("int_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_INT, setting_meta->type);
    EXPECT_STREQ("Integer", setting_meta->label.c_str());
    EXPECT_STREQ("Integer Description", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_EQ(std::numeric_limits<int>::min(), setting_meta->min_value);
    EXPECT_EQ(std::numeric_limits<int>::max(), setting_meta->max_value);
    EXPECT_TRUE(setting_meta->unit.empty());

    EXPECT_EQ(76, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_int_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaInt* setting_meta = layer.settings.Get<SettingMetaInt>("int_with_optional");
    ASSERT_TRUE(setting_meta != nullptr);

    EXPECT_STREQ("int_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_INT, setting_meta->type);
    EXPECT_STREQ("Integer", setting_meta->label.c_str());
    EXPECT_STREQ("Integer Description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#int", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_EQ(75, setting_meta->min_value);
    EXPECT_EQ(82, setting_meta->max_value);
    EXPECT_STREQ("byte", setting_meta->unit.c_str());

    EXPECT_EQ(76, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_int_range_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaIntRanges* setting_meta = layer.settings.Get<SettingMetaIntRanges>("int_range_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("int_range_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_INT_RANGES, setting_meta->type);
    EXPECT_STREQ("Integer Range", setting_meta->label.c_str());
    EXPECT_STREQ("Integer Range Description", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("76-82", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_int_range_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaIntRanges* setting_meta = layer.settings.Get<SettingMetaIntRanges>("int_range_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("int_range_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_INT_RANGES, setting_meta->type);
    EXPECT_STREQ("Integer Range", setting_meta->label.c_str());
    EXPECT_STREQ("Integer Range Description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#int_range", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("76-82", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_list_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaList* setting_meta = layer.settings.Get<SettingMetaList>("list_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("list_required_only", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_LIST, setting_meta->type);
    EXPECT_STREQ("List", setting_meta->label.c_str());
    EXPECT_STREQ("List description", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("stringA", setting_meta->list[0].c_str());
    EXPECT_STREQ("stringB", setting_meta->list[1].c_str());
    EXPECT_STREQ("stringC", setting_meta->list[2].c_str());

    EXPECT_STREQ("stringB", setting_meta->default_value[0].c_str());
    EXPECT_STREQ("stringC", setting_meta->default_value[1].c_str());
}

TEST(test_layer, load_1_4_0_setting_list_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/Layer 1.4.0 - setting types.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaList* setting_meta = layer.settings.Get<SettingMetaList>("list_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("list_with_optional", setting_meta->key.c_str());
    EXPECT_EQ(SETTING_LIST, setting_meta->type);
    EXPECT_STREQ("List", setting_meta->label.c_str());
    EXPECT_STREQ("List description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#list", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("stringA", setting_meta->list[0].c_str());
    EXPECT_STREQ("stringB", setting_meta->list[1].c_str());
    EXPECT_STREQ("stringC", setting_meta->list[2].c_str());

    EXPECT_STREQ("stringB", setting_meta->default_value[0].c_str());
    EXPECT_STREQ("stringC", setting_meta->default_value[1].c_str());
}
