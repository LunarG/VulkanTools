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

#include "../configurator.h"
#include "./generated/vulkan_layer_settings.txt"

#include <gtest/gtest.h>

#include <vector>
#include <array>

LayerSettings layer_settings;

Configurator& GetTestConfigurator() {
    Path path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");
    path.Create();

    Configurator& configurator = Configurator::Get();
    configurator.layers.LoadLayersFromPath(":/layers/VK_LAYER_LUNARG_reference_1_2_1.json", LAYER_TYPE_EXPLICIT,
                                           CONFIGURATOR_MODE_CMD);
    configurator.configurations.CreateConfiguration(configurator.layers, "configuration");
    configurator.SetActiveConfigurationName("configuration");
    return configurator;
}

std::string Read(const Path& path) {
    QFile file(path.AbsolutePath().c_str());
    bool open = file.open(QIODevice::ReadOnly | QIODevice::Text);
    EXPECT_TRUE(open);
    QString data = file.readAll();
    file.close();
    return data.toStdString();
}

TEST(test_export, extension_code) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/generated/vulkan_layer_settings.hpp");

    const Path path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp/vulkan_layer_settings.txt");
    configurator.WriteExtensionCode(path);
    std::string data_generated = ::Read(path);

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_export, vk_layer_settings_txt) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.txt");

    const Path path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp/vk_layer_settings.txt");
    configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT, path);
    std::string data_generated = ::Read(path);

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_export, environment_variables_bash) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.sh");

    const Path path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp/vk_layer_settings.sh");
    configurator.Export(EXPORT_ENV_BASH, path);
    std::string data_generated = ::Read(path);

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_export, environment_variables_cmd) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.bat");

    const Path path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp/vk_layer_settings.bat");
    configurator.Export(EXPORT_ENV_CMD, path);
    std::string data_generated = ::Read(path);

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}
