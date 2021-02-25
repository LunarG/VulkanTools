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

#include "../override.h"
#include "../environment.h"
#include "../layer.h"
#include "../layer_manager.h"

#include <gtest/gtest.h>

extern bool WriteLayersOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                                const Configuration& configuration, const std::string& layers_path);

extern bool WriteSettingsOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                                  const Configuration& configuration, const std::string& settings_path);

extern bool EraseLayersOverride(const std::string& layers_path);

extern bool EraseSettingsOverride(const std::string& settings_path);

TEST(test_override, write_erase) {
    PathManager paths;
    Environment env(paths, Version(1, 2, 162));
    env.Reset(Environment::DEFAULT);

    LayerManager layer_manager(env);
    layer_manager.LoadLayersFromPath(":/");

    Configuration configuration;
    const bool load = configuration.Load(layer_manager.available_layers, ":/Configuration 2.2.0 - Layer 1.4.0.json");
    EXPECT_TRUE(load);
    EXPECT_TRUE(!configuration.parameters.empty());

    EXPECT_EQ(true, WriteLayersOverride(env, layer_manager.available_layers, configuration, "./override_layers_1_4_0.json"));
    EXPECT_EQ(true, WriteSettingsOverride(env, layer_manager.available_layers, configuration, "./override_settings_1_4_0.txt"));

    QFile file_layers_override_ref(":/override_layers_1_4_0.json");
    const bool result_layers_override_ref = file_layers_override_ref.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result_layers_override_ref);
    QString text_layers_override_ref = file_layers_override_ref.readAll();
    file_layers_override_ref.close();

    QFile file_layers_override_gen("./override_layers_1_4_0.json");
    const bool result_layers_override_gen = file_layers_override_gen.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result_layers_override_gen);
    QString text_layers_override_gen = file_layers_override_gen.readAll();
    file_layers_override_gen.close();

    EXPECT_STREQ(text_layers_override_ref.toStdString().c_str(), text_layers_override_gen.toStdString().c_str());

    QFile file_settings_override_ref(":/override_settings_1_4_0.txt");
    const bool result_settings_override_ref = file_settings_override_ref.open(QFile::ReadOnly);
    assert(result_settings_override_ref);
    QString text_settings_override_ref = file_settings_override_ref.readAll();
    file_settings_override_ref.close();

    text_settings_override_ref.replace("\r\n", "\n");  // Using UNIX EOL

    QFile file_settings_override_gen("./override_settings_1_4_0.txt");
    const bool result_settings_override_gen = file_settings_override_gen.open(QFile::ReadOnly);
    assert(result_settings_override_gen);
    QString text_settings_override_gen = file_settings_override_gen.readAll();
    file_settings_override_gen.close();

    text_settings_override_gen.replace("\r\n", "\n");  // Using UNIX EOL

    EXPECT_EQ(text_settings_override_ref.size(), text_settings_override_gen.size());
    EXPECT_STREQ(text_settings_override_ref.toStdString().c_str(), text_settings_override_gen.toStdString().c_str());

    EXPECT_EQ(true, EraseLayersOverride("./override_layers_1_4_0.json"));
    EXPECT_EQ(true, EraseSettingsOverride("./override_settings_1_4_0.txt"));

    env.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
