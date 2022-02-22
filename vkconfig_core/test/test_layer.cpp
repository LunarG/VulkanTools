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
#include "../setting_string.h"
#include "../setting_filesystem.h"
#include "../setting_bool.h"
#include "../setting_frames.h"
#include "../setting_flags.h"
#include "../setting_int.h"
#include "../setting_list.h"

#include <gtest/gtest.h>

#include <regex>

inline SettingMetaString* InstantiateString(Layer& layer, const std::string& key) {
    return static_cast<SettingMetaString*>(layer.Instantiate(layer.settings, key, SETTING_STRING));
}

TEST(test_layer, collect_settings) {
    Layer layer;

    SettingDataSet data0;
    SettingMetaSet meta;

    CollectDefaultSettingData(meta, data0);
    EXPECT_TRUE(data0.empty());

    SettingMetaString* meta0 = InstantiateString(layer, "key0");
    meta0->default_value = "value0";
    meta.push_back(meta0);

    SettingDataSet data1;
    CollectDefaultSettingData(meta, data1);

    SettingDataString* data_string = static_cast<SettingDataString*>(FindSetting(data1, "key0"));
    EXPECT_STREQ("value0", data_string->value.c_str());
}

TEST(test_layer, load_header_overridden) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_00.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_STREQ("VK_LAYER_LUNARG_test_00", layer.key.c_str());
    EXPECT_STREQ(".\\VkLayer_test.dll", layer.binary_path.c_str());
    EXPECT_EQ(Version(1, 2, 170), layer.api_version);
    EXPECT_STREQ("Build 76", layer.implementation_version.c_str());
    EXPECT_STREQ("test layer", layer.description.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.platforms);
    EXPECT_EQ(STATUS_BETA, layer.status);
    EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html", layer.url.c_str());
    EXPECT_TRUE(layer.settings.empty());
    EXPECT_TRUE(layer.presets.empty());
}

TEST(test_layer, load_header_default) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_01.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_STREQ("VK_LAYER_LUNARG_test_01", layer.key.c_str());
    EXPECT_STREQ(".\\VkLayer_test.dll", layer.binary_path.c_str());
    EXPECT_EQ(Version(1, 2, 170), layer.api_version);
    EXPECT_STREQ("Build 76", layer.implementation_version.c_str());
    EXPECT_STREQ("test layer", layer.description.c_str());
    EXPECT_EQ(PLATFORM_DESKTOP_BIT, layer.platforms);
    EXPECT_EQ(STATUS_STABLE, layer.status);
    EXPECT_TRUE(layer.url.empty());
    EXPECT_EQ(0, layer.settings.size());
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer, load_header_override) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_02.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.platforms);
    EXPECT_EQ(STATUS_BETA, layer.status);
    EXPECT_EQ(0, layer.settings.size());
    EXPECT_EQ(0, layer.presets.size());
}

TEST(test_layer, load_setting_interit) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_03.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.platforms);
    EXPECT_EQ(STATUS_BETA, layer.status);
    EXPECT_EQ(2, layer.settings.size());
    EXPECT_EQ(0, layer.presets.size());

    EXPECT_STREQ("int_inherit", layer.settings[0]->key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.settings[0]->platform_flags);
    EXPECT_EQ(STATUS_BETA, layer.settings[0]->status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, layer.settings[0]->view);

    EXPECT_STREQ("int_override", layer.settings[1]->key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.settings[1]->platform_flags);
    EXPECT_EQ(STATUS_ALPHA, layer.settings[1]->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, layer.settings[1]->view);
}

TEST(test_layer, load_preset_interit) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_04.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.platforms);
    EXPECT_EQ(STATUS_BETA, layer.status);
    EXPECT_EQ(1, layer.settings.size());
    EXPECT_EQ(2, layer.presets.size());

    EXPECT_STREQ("Preset Inherit", layer.presets[0].label.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.presets[0].platform_flags);
    EXPECT_EQ(STATUS_BETA, layer.presets[0].status);

    EXPECT_STREQ("Preset Override", layer.presets[1].label.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[1].platform_flags);
    EXPECT_EQ(STATUS_ALPHA, layer.presets[1].status);
}

TEST(test_layer, load_setting_children_interit) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_05.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_EQ(0, layer.presets.size());
    EXPECT_EQ(1, layer.settings.size());
    EXPECT_EQ(2, layer.settings[0]->children.size());
    EXPECT_EQ(3, CountSettings(layer.settings));

    EXPECT_STREQ("int_inherit", layer.settings[0]->children[0]->key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.settings[0]->children[0]->platform_flags);
    EXPECT_EQ(STATUS_BETA, layer.settings[0]->children[0]->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, layer.settings[0]->children[0]->view);

    EXPECT_STREQ("int_override", layer.settings[0]->children[1]->key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, layer.settings[0]->children[1]->platform_flags);
    EXPECT_EQ(STATUS_ALPHA, layer.settings[0]->children[1]->status);
    EXPECT_EQ(SETTING_VIEW_HIDDEN, layer.settings[0]->children[1]->view);
}

TEST(test_layer, load_setting_enum_interit) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_test_06.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 2, 0), layer.file_format_version);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT | PLATFORM_ANDROID_BIT, layer.platforms);
    EXPECT_EQ(STATUS_BETA, layer.status);
    EXPECT_EQ(2, layer.settings.size());
    EXPECT_EQ(0, layer.presets.size());

    SettingMetaEnum* setting_inherit = static_cast<SettingMetaEnum*>(layer.settings[0]);
    EXPECT_EQ(2, setting_inherit->enum_values.size());

    EXPECT_STREQ("value0", setting_inherit->enum_values[0].key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT | PLATFORM_ANDROID_BIT,
              setting_inherit->enum_values[0].platform_flags);
    EXPECT_EQ(STATUS_BETA, setting_inherit->enum_values[0].status);
    EXPECT_EQ(SETTING_VIEW_STANDARD, setting_inherit->enum_values[0].view);

    EXPECT_STREQ("value1", setting_inherit->enum_values[1].key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, setting_inherit->enum_values[1].platform_flags);
    EXPECT_EQ(STATUS_ALPHA, setting_inherit->enum_values[1].status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_inherit->enum_values[1].view);

    SettingMetaEnum* setting_override = static_cast<SettingMetaEnum*>(layer.settings[1]);
    EXPECT_EQ(2, setting_override->enum_values.size());
    EXPECT_EQ(STATUS_ALPHA, setting_override->status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_override->view);
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, setting_override->platform_flags);

    EXPECT_STREQ("value0", setting_override->enum_values[0].key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, setting_override->enum_values[0].platform_flags);
    EXPECT_EQ(STATUS_ALPHA, setting_override->enum_values[0].status);
    EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_override->enum_values[0].view);

    EXPECT_STREQ("value1", setting_override->enum_values[1].key.c_str());
    EXPECT_EQ(PLATFORM_WINDOWS_BIT, setting_override->enum_values[1].platform_flags);
    EXPECT_EQ(STATUS_DEPRECATED, setting_override->enum_values[1].status);
    EXPECT_EQ(SETTING_VIEW_HIDDEN, setting_override->enum_values[1].view);
}

TEST(test_layer, load_1_1_0_header) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_reference_1_1_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    EXPECT_EQ(Version(1, 1, 0), layer.file_format_version);
    EXPECT_STREQ("VK_LAYER_LUNARG_reference_1_1_0", layer.key.c_str());
    EXPECT_STREQ(".\\VkLayer_reference.dll", layer.binary_path.c_str());
    EXPECT_EQ(Version(1, 2, 162), layer.api_version);
    EXPECT_STREQ("Build 75", layer.implementation_version.c_str());
    EXPECT_STREQ("reference layer", layer.description.c_str());
    EXPECT_EQ(PLATFORM_DESKTOP_BIT, layer.platforms);
    EXPECT_EQ(STATUS_STABLE, layer.status);
    EXPECT_TRUE(layer.url.empty());
    EXPECT_TRUE(layer.settings.empty());
    EXPECT_TRUE(layer.presets.empty());
}

TEST(test_layer, load_1_2_0_preset_and_setting_type) {
    Layer layer;
    const bool load_loaded = layer.Load(std::vector<Layer>(), ":/VK_LAYER_LUNARG_reference_1_2_0.json", LAYER_TYPE_EXPLICIT);
    ASSERT_TRUE(load_loaded);

    // Preset Enum
    {
        const std::size_t index = 0;

        const SettingDataEnum* setting_only =
            static_cast<const SettingDataEnum*>(FindSetting(layer.presets[index].settings, "enum_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataEnum* setting_opt =
            static_cast<const SettingDataEnum*>(FindSetting(layer.presets[index].settings, "enum_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset Enum", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Enum", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_STABLE, layer.presets[index].status);
        EXPECT_STREQ("value2", setting_only->value.c_str());
        EXPECT_STREQ("value2", setting_opt->value.c_str());
    }

    // Preset Flags
    {
        const std::size_t index = 1;

        const SettingDataFlags* setting_only =
            static_cast<const SettingDataFlags*>(FindSetting(layer.presets[index].settings, "flags_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataFlags* setting_opt =
            static_cast<const SettingDataFlags*>(FindSetting(layer.presets[index].settings, "flags_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset Flags", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Flags", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_BETA, layer.presets[index].status);
        EXPECT_STREQ("flag0", setting_only->value[0].c_str());
        EXPECT_STREQ("flag2", setting_only->value[1].c_str());
        EXPECT_STREQ("flag0", setting_opt->value[0].c_str());
        EXPECT_STREQ("flag2", setting_opt->value[1].c_str());
    }

    // Preset String
    {
        const std::size_t index = 2;

        const SettingDataFileLoad* setting_only =
            static_cast<const SettingDataFileLoad*>(FindSetting(layer.presets[index].settings, "string_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataFileLoad* setting_opt =
            static_cast<const SettingDataFileLoad*>(FindSetting(layer.presets[index].settings, "string_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset String", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description String", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_ALPHA, layer.presets[index].status);
        EXPECT_STREQ("Required Only", setting_only->value.c_str());
        EXPECT_STREQ("With Optional", setting_opt->value.c_str());
    }

    // Preset Bool
    {
        const std::size_t index = 3;

        EXPECT_STREQ("Preset Bool", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Bool", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_EQ(true,
                  static_cast<const SettingDataBool*>(FindSetting(layer.presets[index].settings, "bool_required_only"))->value);
        EXPECT_EQ(false,
                  static_cast<const SettingDataBool*>(FindSetting(layer.presets[index].settings, "bool_with_optional"))->value);
    }

    // Preset Load File
    {
        const std::size_t index = 4;

        const SettingDataFileLoad* setting_only =
            static_cast<const SettingDataFileLoad*>(FindSetting(layer.presets[index].settings, "load_file_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataFileLoad* setting_opt =
            static_cast<const SettingDataFileLoad*>(FindSetting(layer.presets[index].settings, "load_file_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset Load File", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Load File", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_STREQ("./text.log", setting_only->value.c_str());
        EXPECT_STREQ("./text.log", setting_opt->value.c_str());
    }

    // Preset Save File
    {
        const std::size_t index = 5;

        const SettingDataFileSave* setting_only =
            static_cast<const SettingDataFileSave*>(FindSetting(layer.presets[index].settings, "save_file_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataFileSave* setting_opt =
            static_cast<const SettingDataFileSave*>(FindSetting(layer.presets[index].settings, "save_file_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset Save File", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Save File", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_STREQ("./text.log", setting_only->value.c_str());
        EXPECT_STREQ("./text.log", setting_opt->value.c_str());
    }

    // Preset Save Folder
    {
        const std::size_t index = 6;

        const SettingDataFolderSave* setting_only =
            static_cast<const SettingDataFolderSave*>(FindSetting(layer.presets[index].settings, "save_folder_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataFolderSave* setting_opt =
            static_cast<const SettingDataFolderSave*>(FindSetting(layer.presets[index].settings, "save_folder_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset Save Folder", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Save Folder", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_STREQ("./text.log", setting_only->value.c_str());
        EXPECT_STREQ("./text.log", setting_opt->value.c_str());
    }

    // Preset int
    {
        const std::size_t index = 7;

        const SettingDataInt* setting_only =
            static_cast<const SettingDataInt*>(FindSetting(layer.presets[index].settings, "int_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataInt* setting_opt =
            static_cast<const SettingDataInt*>(FindSetting(layer.presets[index].settings, "int_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset Int", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Int", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_EQ(75, setting_only->value);
        EXPECT_EQ(77, setting_opt->value);
    }

    // Preset Frames
    {
        const std::size_t index = 8;

        EXPECT_STREQ("Preset Frames", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description Frames", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_STREQ("13-17,24-32",
                     static_cast<const SettingDataFrames*>(FindSetting(layer.presets[index].settings, "frames_required_only"))
                         ->value.c_str());
        EXPECT_STREQ("13-17,24,32",
                     static_cast<const SettingDataFrames*>(FindSetting(layer.presets[index].settings, "frames_with_optional"))
                         ->value.c_str());
    }

    // Preset List
    {
        const std::size_t index = 9;

        const SettingDataList* setting_only =
            static_cast<const SettingDataList*>(FindSetting(layer.presets[index].settings, "list_required_only"));
        ASSERT_TRUE(setting_only);
        const SettingDataList* setting_opt =
            static_cast<const SettingDataList*>(FindSetting(layer.presets[index].settings, "list_with_optional"));
        ASSERT_TRUE(setting_opt);

        EXPECT_STREQ("Preset List", layer.presets[index].label.c_str());
        EXPECT_STREQ("Description List", layer.presets[index].description.c_str());
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_MACOS_BIT, layer.presets[index].platform_flags);
        EXPECT_EQ(STATUS_DEPRECATED, layer.presets[index].status);
        EXPECT_STREQ("stringA", setting_only->value[0].key.c_str());
        EXPECT_STREQ("stringB", setting_only->value[1].key.c_str());
        EXPECT_EQ(true, setting_only->value[0].enabled);
        EXPECT_EQ(false, setting_only->value[1].enabled);
        EXPECT_STREQ("stringA", setting_opt->value[0].key.c_str());
        EXPECT_STREQ("stringB", setting_opt->value[1].key.c_str());
        EXPECT_EQ(true, setting_opt->value[0].enabled);
        EXPECT_EQ(false, setting_opt->value[1].enabled);
    }

    // Enum Required Only
    {
        SettingMetaEnum* setting_meta = static_cast<SettingMetaEnum*>(FindSetting(layer.settings, "enum_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("enum_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_ENUM, setting_meta->type);
        EXPECT_STREQ("enum", setting_meta->label.c_str());
        EXPECT_STREQ("enum case", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);
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

    // Enum With Optional
    {
        SettingMetaEnum* setting_meta = static_cast<SettingMetaEnum*>(FindSetting(layer.settings, "enum_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("enum_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_ENUM", setting_meta->env.c_str());
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
        value0.platform_flags = PLATFORM_DESKTOP_BIT;
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

    // Flags Required Only
    {
        const SettingMetaFlags* setting_meta = static_cast<SettingMetaFlags*>(FindSetting(layer.settings, "flags_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("flags_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_FLAGS, setting_meta->type);
        EXPECT_STREQ("flags", setting_meta->label.c_str());
        EXPECT_STREQ("flags case", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);
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

    // Flags With Optional
    {
        SettingMetaFlags* setting_meta = static_cast<SettingMetaFlags*>(FindSetting(layer.settings, "flags_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("flags_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_FLAGS", setting_meta->env.c_str());
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
        value0.platform_flags = PLATFORM_DESKTOP_BIT;
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

    // String Required Only
    {
        SettingMetaString* setting_meta = static_cast<SettingMetaString*>(FindSetting(layer.settings, "string_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("string_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_STRING, setting_meta->type);
        EXPECT_STREQ("String", setting_meta->label.c_str());
        EXPECT_STREQ("string", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);

        EXPECT_STREQ("A string", setting_meta->default_value.c_str());
    }

    // String With Optional
    {
        SettingMetaString* setting_meta = static_cast<SettingMetaString*>(FindSetting(layer.settings, "string_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("string_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_STRING", setting_meta->env.c_str());
        EXPECT_EQ(SETTING_STRING, setting_meta->type);
        EXPECT_STREQ("String", setting_meta->label.c_str());
        EXPECT_STREQ("string", setting_meta->description.c_str());
        EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#string", setting_meta->url.c_str());
        EXPECT_EQ(STATUS_BETA, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

        EXPECT_STREQ("A string", setting_meta->default_value.c_str());
    }

    // Bool Required Only
    {
        SettingMetaBool* setting_meta = static_cast<SettingMetaBool*>(FindSetting(layer.settings, "bool_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("bool_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_BOOL, setting_meta->type);
        EXPECT_STREQ("bool", setting_meta->label.c_str());
        EXPECT_STREQ("true or false", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);

        EXPECT_EQ(true, setting_meta->default_value);
    }

    // Bool With Optional
    {
        SettingMetaBool* setting_meta = static_cast<SettingMetaBool*>(FindSetting(layer.settings, "bool_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("bool_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_BOOL", setting_meta->env.c_str());
        EXPECT_EQ(SETTING_BOOL, setting_meta->type);
        EXPECT_STREQ("bool", setting_meta->label.c_str());
        EXPECT_STREQ("true or false", setting_meta->description.c_str());
        EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#bool", setting_meta->url.c_str());
        EXPECT_EQ(STATUS_BETA, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

        EXPECT_EQ(true, setting_meta->default_value);
    }

    // Load File Required Only
    {
        SettingMetaFileLoad* setting_meta =
            static_cast<SettingMetaFileLoad*>(FindSetting(layer.settings, "load_file_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("load_file_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_LOAD_FILE, setting_meta->type);
        EXPECT_STREQ("Load file", setting_meta->label.c_str());
        EXPECT_STREQ("Load file path", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);
        EXPECT_TRUE(setting_meta->filter.empty());

        EXPECT_STREQ("./test.txt", setting_meta->default_value.c_str());
    }

    // Load File With Optional
    {
        SettingMetaFileLoad* setting_meta =
            static_cast<SettingMetaFileLoad*>(FindSetting(layer.settings, "load_file_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("load_file_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_LOAD_FILE", setting_meta->env.c_str());
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

    // Save File Required Only
    {
        SettingMetaFileSave* setting_meta =
            static_cast<SettingMetaFileSave*>(FindSetting(layer.settings, "save_file_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("save_file_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_SAVE_FILE, setting_meta->type);
        EXPECT_STREQ("Save file", setting_meta->label.c_str());
        EXPECT_STREQ("Save file path", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);
        EXPECT_TRUE(setting_meta->filter.empty());

        EXPECT_STREQ("./test.json", setting_meta->default_value.c_str());
    }

    // Save File With Optional
    {
        SettingMetaFileSave* setting_meta =
            static_cast<SettingMetaFileSave*>(FindSetting(layer.settings, "save_file_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("save_file_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_SAVE_FILE", setting_meta->env.c_str());
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

    // Save Folder Required Only
    {
        SettingMetaFolderSave* setting_meta =
            static_cast<SettingMetaFolderSave*>(FindSetting(layer.settings, "save_folder_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("save_folder_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_SAVE_FOLDER, setting_meta->type);
        EXPECT_STREQ("Save folder", setting_meta->label.c_str());
        EXPECT_STREQ("Save folder path", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);

        EXPECT_STREQ("./test", setting_meta->default_value.c_str());
    }

    // Save Folder With Optional
    {
        SettingMetaFolderSave* setting_meta =
            static_cast<SettingMetaFolderSave*>(FindSetting(layer.settings, "save_folder_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("save_folder_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_SAVE_FOLDER", setting_meta->env.c_str());
        EXPECT_EQ(SETTING_SAVE_FOLDER, setting_meta->type);
        EXPECT_STREQ("Save folder", setting_meta->label.c_str());
        EXPECT_STREQ("Save folder path", setting_meta->description.c_str());
        EXPECT_STREQ("https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_dummy.html#save_folder", setting_meta->url.c_str());
        EXPECT_EQ(STATUS_BETA, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_ADVANCED, setting_meta->view);
        EXPECT_EQ(PLATFORM_WINDOWS_BIT | PLATFORM_LINUX_BIT, setting_meta->platform_flags);

        EXPECT_STREQ("./test", setting_meta->default_value.c_str());
    }

    // Int Required Only
    {
        SettingMetaInt* setting_meta = static_cast<SettingMetaInt*>(FindSetting(layer.settings, "int_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("int_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_INT, setting_meta->type);
        EXPECT_STREQ("Integer", setting_meta->label.c_str());
        EXPECT_STREQ("Integer Description", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);
        EXPECT_EQ(std::numeric_limits<int>::min(), setting_meta->min_value);
        EXPECT_EQ(std::numeric_limits<int>::max(), setting_meta->max_value);
        EXPECT_TRUE(setting_meta->unit.empty());

        EXPECT_EQ(76, setting_meta->default_value);
    }

    // Int With Optional
    {
        SettingMetaInt* setting_meta = static_cast<SettingMetaInt*>(FindSetting(layer.settings, "int_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("int_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_INT", setting_meta->env.c_str());
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

    // Frames Required Only
    {
        SettingMetaFrames* setting_meta = static_cast<SettingMetaFrames*>(FindSetting(layer.settings, "frames_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("frames_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_FRAMES, setting_meta->type);
        EXPECT_STREQ("Frames", setting_meta->label.c_str());
        EXPECT_STREQ("Frames Description", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);

        EXPECT_STREQ("76-82,75", setting_meta->default_value.c_str());
    }

    // Frames With Optional
    {
        SettingMetaFrames* setting_meta = static_cast<SettingMetaFrames*>(FindSetting(layer.settings, "frames_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("frames_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_FRAMES", setting_meta->env.c_str());
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

    // List Required Only
    {
        SettingMetaList* setting_meta = static_cast<SettingMetaList*>(FindSetting(layer.settings, "list_required_only"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("list_required_only", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
        EXPECT_EQ(SETTING_LIST, setting_meta->type);
        EXPECT_STREQ("List", setting_meta->label.c_str());
        EXPECT_STREQ("List description", setting_meta->description.c_str());
        EXPECT_TRUE(setting_meta->url.empty());
        EXPECT_EQ(STATUS_STABLE, setting_meta->status);
        EXPECT_EQ(SETTING_VIEW_STANDARD, setting_meta->view);
        EXPECT_EQ(PLATFORM_DESKTOP_BIT, setting_meta->platform_flags);

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

    // List With Optional
    {
        SettingMetaList* setting_meta = static_cast<SettingMetaList*>(FindSetting(layer.settings, "list_with_optional"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("list_with_optional", setting_meta->key.c_str());
        EXPECT_STREQ("VK_REF_LIST", setting_meta->env.c_str());
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

    // List Empty
    {
        SettingMetaList* setting_meta = static_cast<SettingMetaList*>(FindSetting(layer.settings, "list_empty"));
        ASSERT_TRUE(setting_meta != nullptr);

        EXPECT_STREQ("list_empty", setting_meta->key.c_str());
        EXPECT_TRUE(setting_meta->env.empty());
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
}
