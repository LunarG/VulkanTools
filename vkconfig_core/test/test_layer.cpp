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

#include <regex>

TEST(test_layer, collect_settings) {
    SettingMetaSet meta;

    EXPECT_TRUE(CollectDefaultSettingData(meta).Empty());

    SettingMetaString& meta0 = static_cast<SettingMetaString&>(meta.Create("key0", SETTING_STRING));
    meta0.default_value = "value0";

    SettingDataSet data = CollectDefaultSettingData(meta);

    SettingDataString& data0 = *data.Get<SettingDataString>("key0");
    EXPECT_STREQ("value0", data0.value.c_str());
}

TEST(test_layer, load_1_1_0_header) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_1_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 1, 0), layer.file_format_version);
    EXPECT_STREQ("VK_LAYER_LUNARG_reference_1_1_0", layer.key.c_str());
    EXPECT_STREQ(".\\VkLayer_reference.dll", layer.library_path.c_str());
    EXPECT_EQ(Version(1, 2, 162), layer.api_version);
    EXPECT_STREQ("Build 75", layer.implementation_version.c_str());
    EXPECT_STREQ("reference layer", layer.description.c_str());
    EXPECT_EQ(STATUS_STABLE, layer.status);
    EXPECT_TRUE(layer.url.empty());
    EXPECT_TRUE(layer.settings.Empty());
    EXPECT_TRUE(layer.presets.empty());
}

TEST(test_layer, load_1_4_0_header) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_STREQ("VK_LAYER_LUNARG_reference_1_2_0", layer.key.c_str());
    EXPECT_STREQ(".\\VkLayer_reference.dll", layer.library_path.c_str());
    EXPECT_EQ(Version(1, 2, 170), layer.api_version);
    EXPECT_STREQ("Build 76", layer.implementation_version.c_str());
    EXPECT_STREQ("reference layer", layer.description.c_str());
    EXPECT_EQ(STATUS_BETA, layer.status);
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html", layer.url.c_str());
    EXPECT_TRUE(!layer.settings.Empty());
    EXPECT_TRUE(!layer.presets.empty());
}

TEST(test_layer, load_1_4_0_preset_enum) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_STREQ("Preset Enum", layer.presets[0].label.c_str());
    EXPECT_STREQ("Description Enum", layer.presets[0].description.c_str());
    EXPECT_EQ(PLATFORM_ALL_BIT, layer.presets[0].platform_flags);
    EXPECT_EQ(STATUS_STABLE, layer.presets[0].status);
    EXPECT_STREQ("value2", layer.presets[0].settings.Get<SettingDataEnum>("enum_required_only")->value.c_str());
    EXPECT_STREQ("value2", layer.presets[0].settings.Get<SettingDataEnum>("enum_with_optional")->value.c_str());
}

TEST(test_layer, load_1_4_0_preset_flags) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_STREQ("Preset Flags", layer.presets[1].label.c_str());
    EXPECT_STREQ("Description Flags", layer.presets[1].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, layer.presets[1].platform_flags);
    EXPECT_EQ(STATUS_BETA, layer.presets[1].status);
    EXPECT_STREQ("flag0", layer.presets[1].settings.Get<SettingDataFlags>("flags_required_only")->value[0].c_str());
    EXPECT_STREQ("flag2", layer.presets[1].settings.Get<SettingDataFlags>("flags_required_only")->value[1].c_str());
    EXPECT_STREQ("flag0", layer.presets[1].settings.Get<SettingDataFlags>("flags_with_optional")->value[0].c_str());
    EXPECT_STREQ("flag2", layer.presets[1].settings.Get<SettingDataFlags>("flags_with_optional")->value[1].c_str());
}

TEST(test_layer, load_1_4_0_preset_string) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_STREQ("Preset String", layer.presets[2].label.c_str());
    EXPECT_STREQ("Description String", layer.presets[2].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.presets[2].platform_flags);
    EXPECT_EQ(STATUS_ALPHA, layer.presets[2].status);
    EXPECT_STREQ("Required Only", layer.presets[2].settings.Get<SettingDataString>("string_required_only")->value.c_str());
    EXPECT_STREQ("With Optional", layer.presets[2].settings.Get<SettingDataString>("string_with_optional")->value.c_str());
}

TEST(test_layer, load_1_4_0_preset_bool) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 3;

    EXPECT_STREQ("Preset Bool", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description Bool", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_EQ(true, layer.presets[index].settings.Get<SettingDataBool>("bool_required_only")->value);
    EXPECT_EQ(false, layer.presets[index].settings.Get<SettingDataBool>("bool_with_optional")->value);
}

TEST(test_layer, load_1_4_0_preset_load_file) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 4;

    EXPECT_STREQ("Preset Load File", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description Load File", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_STREQ("./text.log", layer.presets[index].settings.Get<SettingDataString>("load_file_required_only")->value.c_str());
    EXPECT_STREQ("./text.log", layer.presets[index].settings.Get<SettingDataString>("load_file_with_optional")->value.c_str());
}

TEST(test_layer, load_1_4_0_preset_save_file) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 5;

    EXPECT_STREQ("Preset Save File", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description Save File", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_STREQ("./text.log", layer.presets[index].settings.Get<SettingDataFileSave>("save_file_required_only")->value.c_str());
    EXPECT_STREQ("./text.log", layer.presets[index].settings.Get<SettingDataFileSave>("save_file_with_optional")->value.c_str());
}

TEST(test_layer, load_1_4_0_preset_save_folder) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 6;

    EXPECT_STREQ("Preset Save Folder", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description Save Folder", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_STREQ("./text.log", layer.presets[index].settings.Get<SettingDataFileSave>("save_folder_required_only")->value.c_str());
    EXPECT_STREQ("./text.log", layer.presets[index].settings.Get<SettingDataFileSave>("save_folder_with_optional")->value.c_str());
}

TEST(test_layer, load_1_4_0_preset_int) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 7;

    EXPECT_STREQ("Preset Int", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description Int", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_EQ(75, layer.presets[index].settings.Get<SettingDataInt>("int_required_only")->value);
    EXPECT_EQ(77, layer.presets[index].settings.Get<SettingDataInt>("int_with_optional")->value);
}

TEST(test_layer, load_1_4_0_preset_frames) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 8;

    EXPECT_STREQ("Preset Frames", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description Frames", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_STREQ("13-17,24-32", layer.presets[index].settings.Get<SettingDataFrames>("frames_required_only")->value.c_str());
    EXPECT_STREQ("13-17,24,32", layer.presets[index].settings.Get<SettingDataFrames>("frames_with_optional")->value.c_str());
}

TEST(test_layer, load_1_4_0_preset_list) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    const std::size_t index = 9;

    EXPECT_STREQ("Preset List", layer.presets[index].label.c_str());
    EXPECT_STREQ("Description List", layer.presets[index].description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
    EXPECT_STREQ("stringA", layer.presets[index].settings.Get<SettingDataList>("list_required_only")->value[0].key.c_str());
    EXPECT_STREQ("stringB", layer.presets[index].settings.Get<SettingDataList>("list_required_only")->value[1].key.c_str());
    EXPECT_EQ(true, layer.presets[index].settings.Get<SettingDataList>("list_required_only")->value[0].enabled);
    EXPECT_EQ(false, layer.presets[index].settings.Get<SettingDataList>("list_required_only")->value[1].enabled);
    EXPECT_STREQ("stringA", layer.presets[index].settings.Get<SettingDataList>("list_with_optional")->value[0].key.c_str());
    EXPECT_STREQ("stringB", layer.presets[index].settings.Get<SettingDataList>("list_with_optional")->value[1].key.c_str());
    EXPECT_EQ(true, layer.presets[index].settings.Get<SettingDataList>("list_with_optional")->value[0].enabled);
    EXPECT_EQ(false, layer.presets[index].settings.Get<SettingDataList>("list_with_optional")->value[1].enabled);
}

TEST(test_layer, load_1_4_0_setting_enum_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaEnum* setting_meta = layer.settings.Get<SettingMetaEnum>("enum_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("enum_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_ENUM, setting_meta->type);
    EXPECT_STREQ("enum", setting_meta->label.c_str());
    EXPECT_STREQ("enum case", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
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
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaEnum* setting_meta = layer.settings.Get<SettingMetaEnum>("enum_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("enum_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_ENUM", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_ENUM, setting_meta->type);
    EXPECT_STREQ("enum", setting_meta->label.c_str());
    EXPECT_STREQ("enum case", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#enum", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, setting_meta->platform_flags);
    EXPECT_EQ(3, setting_meta->enum_values.size());

    SettingEnumValue value0;
    value0.key = "value0";
    value0.label = "Value0";
    value0.description = "My value0";
    value0.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#value0";
    value0.status = STATUS_STABLE;
    value0.view = SETTING_VIEW_HIDDEN;
    value0.platform_flags = PLATFORM_ALL_BIT;
    EXPECT_EQ(value0, setting_meta->enum_values[0]);

    SettingEnumValue value1;
    value1.key = "value1";
    value1.label = "Value1";
    value1.description = "My value1";
    value1.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#value1";
    value1.status = STATUS_BETA;
    value1.view = SETTING_VIEW_ADVANCED;
    value1.platform_flags = PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT;
    EXPECT_EQ(value1, setting_meta->enum_values[1]);

    SettingEnumValue value2;
    value2.key = "value2";
    value2.label = "Value2";
    value2.description = "My value2";
    value2.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#value2";
    value2.status = STATUS_ALPHA;
    value2.view = SETTING_VIEW_STANDARD;
    value2.platform_flags = PLATFORM_WINDOWS_BIT;
    EXPECT_EQ(value2, setting_meta->enum_values[2]);

    EXPECT_STREQ("value1", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_flags_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFlags* setting_meta = layer.settings.Get<SettingMetaFlags>("flags_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("flags_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_FLAGS, setting_meta->type);
    EXPECT_STREQ("flags", setting_meta->label.c_str());
    EXPECT_STREQ("flags case", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
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
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFlags* setting_meta = layer.settings.Get<SettingMetaFlags>("flags_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("flags_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_FLAGS", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_FLAGS, setting_meta->type);
    EXPECT_STREQ("flags", setting_meta->label.c_str());
    EXPECT_STREQ("flags case", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flags", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_EQ(3, setting_meta->enum_values.size());

    SettingEnumValue value0;
    value0.key = "flag0";
    value0.label = "Flag0";
    value0.description = "My flag0";
    value0.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flag0";
    value0.status = STATUS_STABLE;
    value0.view = SETTING_VIEW_STANDARD;
    value0.platform_flags = PLATFORM_ALL_BIT;
    EXPECT_EQ(value0, setting_meta->enum_values[0]);

    SettingEnumValue value1;
    value1.key = "flag1";
    value1.label = "Flag1";
    value1.description = "My flag1";
    value1.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flag1";
    value1.status = STATUS_BETA;
    value1.view = SETTING_VIEW_ADVANCED;
    value1.platform_flags = PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT;
    EXPECT_EQ(value1, setting_meta->enum_values[1]);

    SettingEnumValue value2;
    value2.key = "flag2";
    value2.label = "Flag2";
    value2.description = "My flag2";
    value2.url = "https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#flag2";
    value2.status = STATUS_ALPHA;
    value2.view = SETTING_VIEW_HIDDEN;
    value2.platform_flags = PLATFORM_WINDOWS_BIT;
    EXPECT_EQ(value2, setting_meta->enum_values[2]);

    EXPECT_STREQ("flag0", setting_meta->default_value[0].c_str());
    EXPECT_STREQ("flag1", setting_meta->default_value[1].c_str());
}

TEST(test_layer, load_1_4_0_setting_string_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaString* setting_meta = layer.settings.Get<SettingMetaString>("string_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("string_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_STRING, setting_meta->type);
    EXPECT_STREQ("String", setting_meta->label.c_str());
    EXPECT_STREQ("string", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("A string", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_string_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaString* setting_meta = layer.settings.Get<SettingMetaString>("string_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("string_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_STRING", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_STRING, setting_meta->type);
    EXPECT_STREQ("String", setting_meta->label.c_str());
    EXPECT_STREQ("string", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#string", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("A string", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_bool_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaBool* setting_meta = layer.settings.Get<SettingMetaBool>("bool_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("bool_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_BOOL, setting_meta->type);
    EXPECT_STREQ("bool", setting_meta->label.c_str());
    EXPECT_STREQ("true or false", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_EQ(true, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_bool_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaBool* setting_meta = layer.settings.Get<SettingMetaBool>("bool_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("bool_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_BOOL", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_BOOL, setting_meta->type);
    EXPECT_STREQ("bool", setting_meta->label.c_str());
    EXPECT_STREQ("true or false", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#bool", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_EQ(true, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_load_file_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileLoad* setting_meta = layer.settings.Get<SettingMetaFileLoad>("load_file_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("load_file_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_LOAD_FILE, setting_meta->type);
    EXPECT_STREQ("Load file", setting_meta->label.c_str());
    EXPECT_STREQ("Load file path", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_TRUE(setting_meta->filter.empty());

    EXPECT_STREQ("./test.txt", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_load_file_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileLoad* setting_meta = layer.settings.Get<SettingMetaFileLoad>("load_file_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("load_file_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_LOAD_FILE", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_LOAD_FILE, setting_meta->type);
    EXPECT_STREQ("Load file", setting_meta->label.c_str());
    EXPECT_STREQ("Load file path", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#load_file", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_STREQ("*.txt", setting_meta->filter.c_str());

    EXPECT_STREQ("./test.txt", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_file_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileSave* setting_meta = layer.settings.Get<SettingMetaFileSave>("save_file_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_file_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_SAVE_FILE, setting_meta->type);
    EXPECT_STREQ("Save file", setting_meta->label.c_str());
    EXPECT_STREQ("Save file path", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_TRUE(setting_meta->filter.empty());

    EXPECT_STREQ("./test.json", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_file_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFileSave* setting_meta = layer.settings.Get<SettingMetaFileSave>("save_file_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_file_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_SAVE_FILE", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_SAVE_FILE, setting_meta->type);
    EXPECT_STREQ("Save file", setting_meta->label.c_str());
    EXPECT_STREQ("Save file path", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_file", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_STREQ("*.json", setting_meta->filter.c_str());

    EXPECT_STREQ("./test.json", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_folder_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFolderSave* setting_meta = layer.settings.Get<SettingMetaFolderSave>("save_folder_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_folder_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_SAVE_FOLDER, setting_meta->type);
    EXPECT_STREQ("Save folder", setting_meta->label.c_str());
    EXPECT_STREQ("Save folder path", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("./test", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_save_folder_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFolderSave* setting_meta = layer.settings.Get<SettingMetaFolderSave>("save_folder_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("save_folder_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_SAVE_FOLDER", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_SAVE_FOLDER, setting_meta->type);
    EXPECT_STREQ("Save folder", setting_meta->label.c_str());
    EXPECT_STREQ("Save folder path", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_folder", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("./test", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_int_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaInt* setting_meta = layer.settings.Get<SettingMetaInt>("int_required_only");
    ASSERT_TRUE(setting_meta != nullptr);

    EXPECT_STREQ("int_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_INT, setting_meta->type);
    EXPECT_STREQ("Integer", setting_meta->label.c_str());
    EXPECT_STREQ("Integer Description", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);
    EXPECT_EQ(std::numeric_limits<int>::min(), setting_meta->min_value);
    EXPECT_EQ(std::numeric_limits<int>::max(), setting_meta->max_value);
    EXPECT_TRUE(setting_meta->unit.empty());

    EXPECT_EQ(76, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_int_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaInt* setting_meta = layer.settings.Get<SettingMetaInt>("int_with_optional");
    ASSERT_TRUE(setting_meta != nullptr);

    EXPECT_STREQ("int_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_INT", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_INT, setting_meta->type);
    EXPECT_STREQ("Integer", setting_meta->label.c_str());
    EXPECT_STREQ("Integer Description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#int", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_EQ(75, setting_meta->min_value);
    EXPECT_EQ(82, setting_meta->max_value);
    EXPECT_STREQ("byte", setting_meta->unit.c_str());

    EXPECT_EQ(76, setting_meta->default_value);
}

TEST(test_layer, load_1_4_0_setting_frames_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFrames* setting_meta = layer.settings.Get<SettingMetaFrames>("frames_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("frames_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_FRAMES, setting_meta->type);
    EXPECT_STREQ("Frames", setting_meta->label.c_str());
    EXPECT_STREQ("Frames Description", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_STREQ("76-82,75", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_frames_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaFrames* setting_meta = layer.settings.Get<SettingMetaFrames>("frames_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("frames_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_FRAMES", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_FRAMES, setting_meta->type);
    EXPECT_STREQ("Frames", setting_meta->label.c_str());
    EXPECT_STREQ("Frames Description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#frames", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);
    EXPECT_TRUE(IsFrames(setting_meta->default_value));

    EXPECT_STREQ("76-82,75", setting_meta->default_value.c_str());
}

TEST(test_layer, load_1_4_0_setting_list_required_only) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaList* setting_meta = layer.settings.Get<SettingMetaList>("list_required_only");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("list_required_only", setting_meta->key.c_str());
    EXPECT_TRUE(setting_meta->env.empty());
    EXPECT_EQ(SETTING_LIST, setting_meta->type);
    EXPECT_STREQ("List", setting_meta->label.c_str());
    EXPECT_STREQ("List description", setting_meta->description.c_str());
    EXPECT_TRUE(setting_meta->url.empty());
    EXPECT_EQ(STATUS_STABLE, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
    EXPECT_EQ(PLATFORM_ALL_BIT, setting_meta->platform_flags);

    EXPECT_TRUE(!setting_meta->list_only);

    EXPECT_TRUE(setting_meta->list.empty());

    EXPECT_EQ(5, setting_meta->default_value.size());

    EXPECT_EQ(76, setting_meta->default_value[0].number);
    EXPECT_TRUE(setting_meta->default_value[0].key.empty());
    EXPECT_EQ(true, setting_meta->default_value[0].enabled);

    EXPECT_EQ(82, setting_meta->default_value[1].number);
    EXPECT_TRUE(setting_meta->default_value[1].key.empty());
    EXPECT_EQ(true, setting_meta->default_value[1].enabled);

    EXPECT_EQ(0, setting_meta->default_value[2].number);
    EXPECT_STREQ("stringB", setting_meta->default_value[2].key.c_str());
    EXPECT_EQ(true, setting_meta->default_value[2].enabled);

    EXPECT_EQ(0, setting_meta->default_value[3].number);
    EXPECT_STREQ("stringC", setting_meta->default_value[3].key.c_str());
    EXPECT_EQ(false, setting_meta->default_value[3].enabled);

    EXPECT_EQ(0, setting_meta->default_value[4].number);
    EXPECT_STREQ("stringD", setting_meta->default_value[4].key.c_str());
    EXPECT_EQ(true, setting_meta->default_value[4].enabled);
}

TEST(test_layer, load_1_4_0_setting_list_with_optional) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaList* setting_meta = layer.settings.Get<SettingMetaList>("list_with_optional");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("list_with_optional", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_LIST", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_LIST, setting_meta->type);
    EXPECT_STREQ("List", setting_meta->label.c_str());
    EXPECT_STREQ("List description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#list", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_TRUE(setting_meta->list_only);

    EXPECT_EQ(5, setting_meta->list.size());

    EXPECT_EQ(75, setting_meta->list[0].number);
    EXPECT_EQ(76, setting_meta->list[1].number);
    EXPECT_STREQ("stringA", setting_meta->list[2].key.c_str());
    EXPECT_STREQ("stringB", setting_meta->list[3].key.c_str());
    EXPECT_STREQ("stringC", setting_meta->list[4].key.c_str());

    EXPECT_EQ(5, setting_meta->default_value.size());

    EXPECT_EQ(76, setting_meta->default_value[0].number);
    EXPECT_TRUE(setting_meta->default_value[0].key.empty());
    EXPECT_EQ(true, setting_meta->default_value[0].enabled);

    EXPECT_EQ(82, setting_meta->default_value[1].number);
    EXPECT_TRUE(setting_meta->default_value[1].key.empty());
    EXPECT_EQ(true, setting_meta->default_value[1].enabled);

    EXPECT_EQ(0, setting_meta->default_value[2].number);
    EXPECT_STREQ("stringB", setting_meta->default_value[2].key.c_str());
    EXPECT_EQ(true, setting_meta->default_value[2].enabled);

    EXPECT_EQ(0, setting_meta->default_value[3].number);
    EXPECT_STREQ("stringC", setting_meta->default_value[3].key.c_str());
    EXPECT_EQ(false, setting_meta->default_value[3].enabled);

    EXPECT_EQ(0, setting_meta->default_value[4].number);
    EXPECT_STREQ("stringD", setting_meta->default_value[4].key.c_str());
    EXPECT_EQ(true, setting_meta->default_value[4].enabled);
}

TEST(test_layer, load_1_4_0_setting_list_empty) {
    Layer layer;
    const bool load_loaded = layer.Load(":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);
    ASSERT_TRUE(!layer.settings.Empty());

    SettingMetaList* setting_meta = layer.settings.Get<SettingMetaList>("list_empty");
    ASSERT_TRUE(setting_meta);

    EXPECT_STREQ("list_empty", setting_meta->key.c_str());
    EXPECT_STREQ("TEST_LIST", setting_meta->env.c_str());
    EXPECT_EQ(SETTING_LIST, setting_meta->type);
    EXPECT_STREQ("List", setting_meta->label.c_str());
    EXPECT_STREQ("List description", setting_meta->description.c_str());
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#list", setting_meta->url.c_str());
    EXPECT_EQ(STATUS_BETA, setting_meta->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

    EXPECT_FALSE(setting_meta->list_only);
    EXPECT_TRUE(setting_meta->list.empty());
}
