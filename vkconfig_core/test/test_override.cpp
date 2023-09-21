/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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

#include "../override.h"
#include "../environment.h"
#include "../layer.h"
#include "../layer_manager.h"
#include <vulkan/layer/vk_layer_settings.hpp>

#include <gtest/gtest.h>

#include <QtGlobal>

#include <cstdlib>

extern bool WriteLayersOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                                const Configuration& configuration, const std::string& layers_path);

extern bool WriteSettingsOverride(const std::vector<Layer>& available_layers, const Configuration& configuration,
                                  const std::string& settings_path);

extern bool EraseLayersOverride(const std::string& layers_path);

extern bool EraseSettingsOverride(const std::string& settings_path);

TEST(test_override, write_erase_2_2_2) {
    const std::string LAYERS("/override_layers_2_2_2_schema_1_2_1.json");
    const std::string SETTINGS("/override_settings_2_2_2_schema_1_2_1.txt");

    PathManager paths("");
    Environment env(paths, Version(1, 2, 170));
    env.Reset(Environment::DEFAULT);

    LayerManager layer_manager(env);
    layer_manager.LoadLayersFromPath(":/");

    Configuration configuration;
    const bool load = configuration.Load(layer_manager.available_layers, ":/Configuration 2.2.2.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    EXPECT_EQ(true, WriteLayersOverride(env, layer_manager.available_layers, configuration, "." + LAYERS));
    EXPECT_EQ(true, WriteSettingsOverride(layer_manager.available_layers, configuration, "." + SETTINGS));

    QFile file_layers_override_ref((":" + LAYERS).c_str());
    const bool result_layers_override_ref = file_layers_override_ref.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result_layers_override_ref);
    QString text_layers_override_ref = file_layers_override_ref.readAll();
    file_layers_override_ref.close();

    QFile file_layers_override_gen(("." + LAYERS).c_str());
    const bool result_layers_override_gen = file_layers_override_gen.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result_layers_override_gen);
    QString text_layers_override_gen = file_layers_override_gen.readAll();
    file_layers_override_gen.close();

    EXPECT_STREQ(text_layers_override_ref.toStdString().c_str(), text_layers_override_gen.toStdString().c_str());

    QFile file_settings_override_ref((":" + SETTINGS).c_str());
    const bool result_settings_override_ref = file_settings_override_ref.open(QFile::ReadOnly);
    assert(result_settings_override_ref);
    QString text_settings_override_ref = file_settings_override_ref.readAll();
    file_settings_override_ref.close();

    text_settings_override_ref.replace("\r\n", "\n");  // Using UNIX EOL

    QFile file_settings_override_gen(("." + SETTINGS).c_str());
    const bool result_settings_override_gen = file_settings_override_gen.open(QFile::ReadOnly);
    assert(result_settings_override_gen);
    QString text_settings_override_gen = file_settings_override_gen.readAll();
    file_settings_override_gen.close();

    text_settings_override_gen.replace("\r\n", "\n");  // Using UNIX EOL

    EXPECT_EQ(text_settings_override_ref.size(), text_settings_override_gen.size());
    EXPECT_STREQ(text_settings_override_ref.toStdString().c_str(), text_settings_override_gen.toStdString().c_str());

    EXPECT_EQ(true, EraseLayersOverride("." + LAYERS));
    EXPECT_EQ(true, EraseSettingsOverride("." + SETTINGS));

    env.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}

TEST(test_override, vk_layer_settings_txt) {
    const char* LAYER = "VK_LAYER_LUNARG_reference_1_2_1";

    PathManager paths("");
    Environment env(paths, Version(1, 2, 162));
    env.Reset(Environment::DEFAULT);

    LayerManager layer_manager(env);
    layer_manager.LoadLayersFromPath(":/");

    Configuration configuration;
    const bool load = configuration.Load(layer_manager.available_layers, ":/Configuration 2.2.2.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    EXPECT_EQ(true, OverrideConfiguration(env, layer_manager.available_layers, configuration));

    VkuLayerSettingSet layerSettingSet = VK_NULL_HANDLE;
    vkuCreateLayerSettingSet(LAYER, nullptr, nullptr, nullptr, &layerSettingSet);

    EXPECT_EQ(false, vkuHasLayerSetting(layerSettingSet, "not_found"));

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "toogle"));
    bool setting_toogle = false;
    vkuGetLayerSettingValue(layerSettingSet, "toogle", setting_toogle);
    EXPECT_EQ(true, setting_toogle);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "enum_required_only"));
    std::string setting_enum_required_only;
    vkuGetLayerSettingValue(layerSettingSet, "enum_required_only", setting_enum_required_only);
    EXPECT_STREQ("value2", setting_enum_required_only.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "enum_with_optional"));
    std::string setting_enum_with_optional;
    vkuGetLayerSettingValue(layerSettingSet, "enum_with_optional", setting_enum_with_optional);
    EXPECT_STREQ("value1", setting_enum_with_optional.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "flags_required_only"));
    std::vector<std::string> setting_flags_required_only;
    vkuGetLayerSettingValues(layerSettingSet, "flags_required_only", setting_flags_required_only);
    EXPECT_STREQ("flag0", setting_flags_required_only[0].c_str());
    EXPECT_STREQ("flag2", setting_flags_required_only[1].c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "flags_with_optional"));
    std::vector<std::string> setting_flags_with_optional;
    vkuGetLayerSettingValues(layerSettingSet, "flags_with_optional", setting_flags_with_optional);
    EXPECT_STREQ("flag0", setting_flags_with_optional[0].c_str());
    EXPECT_STREQ("flag2", setting_flags_with_optional[1].c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "string_required_only"));
    std::string setting_string_required_only;
    vkuGetLayerSettingValue(layerSettingSet, "string_required_only", setting_string_required_only);
    EXPECT_STREQ("My string", setting_string_required_only.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "string_with_optional"));
    std::string setting_string_with_optional;
    vkuGetLayerSettingValue(layerSettingSet, "string_required_only", setting_string_with_optional);
    EXPECT_STREQ("My string", setting_string_with_optional.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "bool_required_only"));
    bool setting_bool_required_only = false;
    vkuGetLayerSettingValue(layerSettingSet, "bool_required_only", setting_bool_required_only);
    EXPECT_EQ(true, setting_bool_required_only);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "bool_with_optional"));
    bool setting_bool_with_optional = false;
    vkuGetLayerSettingValue(layerSettingSet, "bool_with_optional", setting_bool_with_optional);
    EXPECT_EQ(true, setting_bool_with_optional);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "load_file_required_only"));
    std::string setting_load_file_required_only;
    vkuGetLayerSettingValue(layerSettingSet, "load_file_required_only", setting_load_file_required_only);
    EXPECT_STREQ("./my_test.txt", setting_load_file_required_only.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "load_file_with_optional"));
    std::string setting_load_file_with_optional;
    vkuGetLayerSettingValue(layerSettingSet, "load_file_with_optional", setting_load_file_with_optional);
    EXPECT_STREQ("./my_test.txt", setting_load_file_with_optional.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "save_file_required_only"));
    std::string setting_save_file_required_only;
    vkuGetLayerSettingValue(layerSettingSet, "load_file_with_optional", setting_save_file_required_only);
    EXPECT_STREQ("./my_test.txt", setting_save_file_required_only.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "save_file_with_optional"));
    std::string setting_save_file_with_optional;
    vkuGetLayerSettingValue(layerSettingSet, "load_file_with_optional", setting_save_file_with_optional);
    EXPECT_STREQ("./my_test.txt", setting_save_file_with_optional.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "save_folder_required_only"));
    std::string setting_save_folder_required_only;
    vkuGetLayerSettingValue(layerSettingSet, "save_folder_required_only", setting_save_folder_required_only);
    EXPECT_STREQ("./my_test", setting_save_folder_required_only.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "save_folder_with_optional"));
    std::string setting_save_folder_with_optional;
    vkuGetLayerSettingValue(layerSettingSet, "save_folder_with_optional", setting_save_folder_with_optional);
    EXPECT_STREQ("./my_test", setting_save_folder_with_optional.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "int_required_only"));
    int setting_int_required_only = 0;
    vkuGetLayerSettingValue(layerSettingSet, "int_required_only", setting_int_required_only);
    EXPECT_EQ(76, setting_int_required_only);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "int_with_optional"));
    int setting_int_with_optional = 0;
    vkuGetLayerSettingValue(layerSettingSet, "int_with_optional", setting_int_with_optional);
    EXPECT_EQ(82, setting_int_with_optional);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "float_required_only"));
    float setting_float_required_only = 0.0f;
    vkuGetLayerSettingValue(layerSettingSet, "float_required_only", setting_float_required_only);
    EXPECT_EQ(76.500000, setting_float_required_only);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "float_with_optional"));
    float setting_float_with_optional = 0.0f;
    vkuGetLayerSettingValue(layerSettingSet, "float_with_optional", setting_float_with_optional);
    EXPECT_EQ(76.500000, setting_float_with_optional);

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "frames_required_only"));
    std::string setting_frames_required_only;
    vkuGetLayerSettingValue(layerSettingSet, "frames_required_only", setting_frames_required_only);
    EXPECT_STREQ("76-82,75", setting_frames_required_only.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "frames_with_optional"));
    std::string setting_frames_with_optional;
    vkuGetLayerSettingValue(layerSettingSet, "frames_with_optional", setting_frames_with_optional);
    EXPECT_STREQ("79-82,75", setting_frames_with_optional.c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "list_required_only"));
    std::vector<std::string> setting_list_required_only;
    vkuGetLayerSettingValues(layerSettingSet, "list_required_only", setting_list_required_only);
    EXPECT_STREQ("76", setting_list_required_only[0].c_str());
    EXPECT_STREQ("82", setting_list_required_only[1].c_str());
    EXPECT_STREQ("stringB", setting_list_required_only[2].c_str());
    EXPECT_STREQ("stringD", setting_list_required_only[3].c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "list_with_optional"));
    std::vector<std::string> setting_list_with_optional;
    vkuGetLayerSettingValues(layerSettingSet, "list_with_optional", setting_list_with_optional);
    EXPECT_STREQ("76", setting_list_with_optional[0].c_str());
    EXPECT_STREQ("stringA", setting_list_with_optional[1].c_str());

    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "list_empty"));
    std::vector<std::string> setting_list_empty;
    vkuGetLayerSettingValues(layerSettingSet, "list_empty", setting_list_empty);
    EXPECT_EQ(true, setting_list_empty.empty());

    EXPECT_EQ(true, SurrenderConfiguration(env));

    vkuDestroyLayerSettingSet(layerSettingSet, nullptr);

    env.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}

TEST(test_override, env_var) {
    const char* LAYER = "VK_LAYER_LUNARG_reference_1_2_1";

    PathManager paths("");
    Environment env(paths, Version(1, 2, 162));
    env.Reset(Environment::DEFAULT);

    LayerManager layer_manager(env);
    layer_manager.LoadLayersFromPath(":/");

    Configuration configuration;
    const bool load = configuration.Load(layer_manager.available_layers, ":/Configuration 2.2.2.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    qputenv("VK_LAYER_SETTINGS_PATH", "./vk_layer_settings.txt");

    EXPECT_EQ(true, OverrideConfiguration(env, layer_manager.available_layers, configuration));

    VkuLayerSettingSet layerSettingSet = VK_NULL_HANDLE;
    vkuCreateLayerSettingSet(LAYER, nullptr, nullptr, nullptr, &layerSettingSet);

    EXPECT_EQ(false, vkuHasLayerSetting(layerSettingSet, "env_o"));

    qputenv("VK_LUNARG_REFERENCE_1_2_1_ENV_A", "pouet");
    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "env_a"));
    std::string setting_env_a;
    vkuGetLayerSettingValue(layerSettingSet, "env_a", setting_env_a);
    EXPECT_STREQ("pouet", setting_env_a.c_str());

    // Check support of environment variable without vendor namespace
    qputenv("VK_REFERENCE_1_2_1_ENV_B", "pouet");
    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "env_b"));
    std::string setting_env_b;
    vkuGetLayerSettingValue(layerSettingSet, "env_b", setting_env_b);
    EXPECT_STREQ("pouet", setting_env_b.c_str());

    // Check support of environment variable without full namespace
    qputenv("VK_ENV_C", "pouet");
    EXPECT_EQ(true, vkuHasLayerSetting(layerSettingSet, "env_c"));
    std::string setting_env_c;
    vkuGetLayerSettingValue(layerSettingSet, "env_c", setting_env_c);
    EXPECT_STREQ("pouet", setting_env_c.c_str());

    vkuDestroyLayerSettingSet(layerSettingSet, nullptr);

    EXPECT_EQ(true, SurrenderConfiguration(env));

    env.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
