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
#include "../command_line.h"
#include "../generate_layers_settings.h"
#include "./generated/vulkan_layer_settings.code"

#if VKC_ENV == VKC_ENV_UNIX
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwrite-strings"
#endif

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

TEST(test_settings, config_generate_html) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",  // Generate settings files
                           (char*)"--generate",   (char*)"html",      // Type of settings file
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vk_layer_settings.html");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.html");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, layer_generate_html) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",                         // Generate settings files
                           (char*)"--generate",   (char*)"html",                             // Type of settings file
                           (char*)"--layer",      (char*)"VK_LAYER_LUNARG_reference_1_2_1",  // Only for a specific layer
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/VK_LAYER_LUNARG_reference_1_2_1.html");
    std::string data_reference = ::Read(":/test/generated/VK_LAYER_LUNARG_reference_1_2_1.html");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, config_generate_markdown) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",  // Generate settings files
                           (char*)"--generate",   (char*)"markdown",  // Type of settings file
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vk_layer_settings.md");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.md");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, layer_generate_markdown) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",                         // Generate settings files
                           (char*)"--generate",   (char*)"markdown",                         // Type of settings file
                           (char*)"--layer",      (char*)"VK_LAYER_LUNARG_reference_1_2_1",  // Only for a specific layer
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/VK_LAYER_LUNARG_reference_1_2_1.md");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.md");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, config_generate_txt) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",  // Generate settings files
                           (char*)"--generate",   (char*)"txt",       // Type of settings file
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vk_layer_settings.txt");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.txt");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, layer_generate_txt) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",                         // Generate settings files
                           (char*)"--generate",   (char*)"txt",                              // Type of settings file
                           (char*)"--layer",      (char*)"VK_LAYER_LUNARG_reference_1_2_1",  // Only for a specific layer
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vk_layer_settings.txt");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.txt");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, config_generate_bash) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",  // Generate settings files
                           (char*)"--generate",   (char*)"bash",      // Type of settings file
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vk_layer_settings.sh");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.sh");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, layer_generate_bash) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",                         // Generate settings files
                           (char*)"--generate",   (char*)"bash",                             // Type of settings file
                           (char*)"--layer",      (char*)"VK_LAYER_LUNARG_reference_1_2_1",  // Only for a specific layer
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/VK_LAYER_LUNARG_reference_1_2_1.sh");
    std::string data_reference = ::Read(":/test/generated/VK_LAYER_LUNARG_reference_1_2_1.sh");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, config_generate_cmd) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",  // Generate settings files
                           (char*)"--generate",   (char*)"bat",       // Type of settings file
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vk_layer_settings.bat");
    std::string data_reference = ::Read(":/test/generated/vk_layer_settings.bat");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, layer_generate_cmd) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",                         // Generate a settings file
                           (char*)"--generate",   (char*)"bat",                              // Type of settings file
                           (char*)"--layer",      (char*)"VK_LAYER_LUNARG_reference_1_2_1",  // Only for a specific layer
                           (char*)"--output-dir", (char*)path.c_str()};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/VK_LAYER_LUNARG_reference_1_2_1.bat");
    std::string data_reference = ::Read(":/test/generated/VK_LAYER_LUNARG_reference_1_2_1.bat");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, config_generate_hpp) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",  // Generate a settings file
                           (char*)"--generate",   (char*)"hpp",       // Type of settings file
                           (char*)"--output-dir", (char*)path.c_str(), (char*)"--output", (char*)"vulkan_layer_settings.code"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/vulkan_layer_settings.code");
    std::string data_reference = ::Read(":/test/generated/vulkan_layer_settings.hpp");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

TEST(test_settings, layer_generate_hpp) {
    const std::string path(std::string(CMAKE_CURRENT_SOURCE_DIR) + "/tmp");

    static char* argv[] = {(char*)"vkconfig",     (char*)"settings",                         // Generate a settings file
                           (char*)"--generate",   (char*)"hpp",                              // Type of settings file
                           (char*)"--layer",      (char*)"VK_LAYER_LUNARG_reference_1_2_1",  // Only for a specific layer
                           (char*)"--output-dir", (char*)path.c_str(),
                           (char*)"--output",     (char*)"VK_LAYER_LUNARG_reference_1_2_1.code"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);
    Configurator& configurator = GetTestConfigurator();

    int result = ::generate_settings(configurator, command_line);
    EXPECT_EQ(result, 0);

    std::string data_generated = ::Read(path + "/VK_LAYER_LUNARG_reference_1_2_1.code");
    std::string data_reference = ::Read(":/test/generated/VK_LAYER_LUNARG_reference_1_2_1.hpp");
    EXPECT_STREQ(data_reference.c_str(), data_generated.c_str());
}

#if VKC_ENV == VKC_ENV_UNIX
#pragma GCC diagnostic pop
#pragma clang diagnostic pop
#endif
