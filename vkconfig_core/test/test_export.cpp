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

#include <gtest/gtest.h>

#include <vector>
#include <array>

// #include "Validation.hpp"
// LayerSettings layer_settings;

Configurator& GetTestConfigurator() {
    Path path("./generated");
    path.Create();

    Configurator& configurator = Configurator::Get();
    configurator.layers.LoadLayersFromPath(":/layers/VK_LAYER_LUNARG_reference_1_2_1.json", LAYER_TYPE_EXPLICIT,
                                           CONFIGURATOR_MODE_CMD);
    configurator.configurations.CreateConfiguration(configurator.layers, "configuration");
    configurator.SetActiveConfigurationName("configuration");
    return configurator;
}

std::string Read(const std::string& path) {
    QFile file(path.c_str());
    bool open = file.open(QIODevice::ReadOnly | QIODevice::Text);
    EXPECT_TRUE(open);
    QString data = file.readAll();
    file.close();
    return data.toStdString();
}

TEST(test_export, extension_code) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/export/vulkan_layer_settings.hpp");

    configurator.WriteExtensionCode(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/vulkan_layer_settings.txt");
    std::string data_generated = ::Read(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/vulkan_layer_settings.txt");

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_export, vk_layer_settings_txt) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/export/vk_layer_settings.txt");

    configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT,
                                     std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/vk_layer_settings.txt");
    std::string data_generated = ::Read(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/vk_layer_settings.txt");

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_export, environment_variables_bash) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/export/environment_variables.sh");

    configurator.Export(EXPORT_ENV_BASH, std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/environment_variables.sh");
    std::string data_generated = ::Read(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/environment_variables.sh");

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_export, environment_variables_cmd) {
    Configurator& configurator = GetTestConfigurator();
    std::string data_reference = ::Read(":/test/export/environment_variables.bat");

    configurator.Export(EXPORT_ENV_CMD, std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/environment_variables.bat");
    std::string data_generated = ::Read(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/export/environment_variables.bat");

    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}
