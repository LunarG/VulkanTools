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
#include "../../vku/vk_layer_settings.h"

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

    EXPECT_EQ(false, vku::IsLayerSetting(LAYER, "not_found"));

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "toogle"));
    EXPECT_EQ(true, vku::GetLayerSettingBool(LAYER, "toogle"));

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "enum_required_only"));
    EXPECT_STREQ("value2", vku::GetLayerSettingString(LAYER, "enum_required_only").c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "enum_with_optional"));
    EXPECT_STREQ("value1", vku::GetLayerSettingString(LAYER, "enum_with_optional").c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "flags_required_only"));
    std::vector<std::string> flags_required_only = vku::GetLayerSettingStrings(LAYER, "flags_required_only");
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "flags_with_optional"));
    std::vector<std::string> flags_with_optional = vku::GetLayerSettingStrings(LAYER, "flags_with_optional");

    EXPECT_STREQ("flag0", flags_required_only[0].c_str());
    EXPECT_STREQ("flag2", flags_required_only[1].c_str());
    EXPECT_STREQ("flag0", flags_with_optional[0].c_str());
    EXPECT_STREQ("flag2", flags_with_optional[1].c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "string_required_only"));
    EXPECT_STREQ("My string", vku::GetLayerSettingString(LAYER, "string_required_only").c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "string_with_optional"));
    EXPECT_STREQ("My string", vku::GetLayerSettingString(LAYER, "string_with_optional").c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "bool_required_only"));
    EXPECT_EQ(true, vku::GetLayerSettingBool(LAYER, "bool_required_only"));

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "bool_with_optional"));
    EXPECT_EQ(true, vku::GetLayerSettingBool(LAYER, "bool_with_optional"));

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "load_file_required_only"));
    EXPECT_STREQ("./my_test.txt", vku::GetLayerSettingString(LAYER, "load_file_required_only").c_str());
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "load_file_with_optional"));
    EXPECT_STREQ("./my_test.txt", vku::GetLayerSettingString(LAYER, "load_file_with_optional").c_str());
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "save_file_required_only"));
    EXPECT_STREQ("./my_test.txt", vku::GetLayerSettingString(LAYER, "save_file_required_only").c_str());
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "save_file_with_optional"));
    EXPECT_STREQ("./my_test.txt", vku::GetLayerSettingString(LAYER, "save_file_with_optional").c_str());
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "save_folder_required_only"));
    EXPECT_STREQ("./my_test", vku::GetLayerSettingString(LAYER, "save_folder_required_only").c_str());
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "save_folder_with_optional"));
    EXPECT_STREQ("./my_test", vku::GetLayerSettingString(LAYER, "save_folder_with_optional").c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "int_required_only"));
    EXPECT_EQ(76, vku::GetLayerSettingInt(LAYER, "int_required_only"));
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "int_with_optional"));
    EXPECT_EQ(82, vku::GetLayerSettingInt(LAYER, "int_with_optional"));

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "float_required_only"));
    EXPECT_EQ(76.500000, vku::GetLayerSettingFloat(LAYER, "float_required_only"));
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "float_with_optional"));
    EXPECT_EQ(76.500000, vku::GetLayerSettingFloat(LAYER, "float_with_optional"));

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "frames_required_only"));
    EXPECT_STREQ("76-82,75", vku::GetLayerSettingFrames(LAYER, "frames_required_only").c_str());
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "frames_with_optional"));
    EXPECT_STREQ("79-82,75", vku::GetLayerSettingFrames(LAYER, "frames_with_optional").c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "list_required_only"));
    std::vector<std::pair<std::string, int>> list_required_only = vku::GetLayerSettingList(LAYER, "list_required_only");

    EXPECT_EQ(76, list_required_only[0].second);
    EXPECT_EQ(82, list_required_only[1].second);
    EXPECT_STREQ("stringB", list_required_only[2].first.c_str());
    EXPECT_STREQ("stringD", list_required_only[3].first.c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "list_with_optional"));
    std::vector<std::pair<std::string, int>> list_with_optional = vku::GetLayerSettingList(LAYER, "list_with_optional");

    EXPECT_EQ(76, list_with_optional[0].second);
    EXPECT_STREQ("stringA", list_with_optional[1].first.c_str());

    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "list_empty"));
    std::vector<std::pair<std::string, int>> list_empty = vku::GetLayerSettingList(LAYER, "list_empty");
    EXPECT_EQ(true, list_empty.empty());

    EXPECT_EQ(true, SurrenderConfiguration(env));

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

    EXPECT_EQ(false, vku::IsLayerSetting(LAYER, "env_o"));

    qputenv("VK_LUNARG_REFERENCE_1_2_1_ENV_A", "pouet");
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "env_a"));
    EXPECT_STREQ("pouet", vku::GetLayerSettingString(LAYER, "env_a").c_str());

    // Check support of environment variable without vendor namespace
    qputenv("VK_REFERENCE_1_2_1_ENV_B", "pouet");
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "env_b"));
    EXPECT_STREQ("pouet", vku::GetLayerSettingString(LAYER, "env_b").c_str());

    // Check support of environment variable without full namespace
    qputenv("VK_ENV_C", "pouet");
    EXPECT_EQ(true, vku::IsLayerSetting(LAYER, "env_c"));
    EXPECT_STREQ("pouet", vku::GetLayerSettingString(LAYER, "env_c").c_str());

    EXPECT_EQ(true, SurrenderConfiguration(env));

    env.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
