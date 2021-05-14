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

#include "../path.h"
#include "../util.h"
#include "../platform.h"

#include <array>

#include <QDir>

#include <gtest/gtest.h>

// Test that GetPath return the home directory when the stored path is empty
TEST(test_path, get_path) { EXPECT_STRNE(GetPath(BUILTIN_PATH_HOME).c_str(), ""); }

TEST(test_path, replace_path_home) {
    const std::string replaced_path = ReplaceBuiltInVariable("${HOME}/test.txt");

    EXPECT_TRUE(replaced_path.find("${HOME}") > replaced_path.size());
}

TEST(test_path, replace_path_vk_local) {
    const std::string replaced_path = ReplaceBuiltInVariable("${VK_LOCAL}/test.txt");

    EXPECT_TRUE(replaced_path.find("${VK_LOCAL}") > replaced_path.size());
}

TEST(test_path, replace_path_vk_appdata) {
    const std::string replaced_path = ReplaceBuiltInVariable("${VK_APPDATA}/test.txt");

    EXPECT_TRUE(replaced_path.find("${VK_APPDATA}") > replaced_path.size());
}

TEST(test_path, replace_path_unknown) {
    const std::string replaced_path = ReplaceBuiltInVariable("${UNKNOWN}/test.txt");

    EXPECT_STREQ("${UNKNOWN}/test.txt", replaced_path.c_str());
}

TEST(test_path, convert_native_separator_empty) {
    const std::string replaced_path = ReplaceBuiltInVariable("");

    EXPECT_STREQ("", replaced_path.c_str());
}

TEST(test_path, native_path) {
    static const char* table[] = {
        "/vkconfig/test\\path/format/",  "/vkconfig/test\\path/format\\", "/vkconfig\\test/path/format/",
        "/vkconfig\\test/path\\format/", "/vkconfig\\test/path/format",   "/vkconfig/test/path/format",
        "\\vkconfig\\test/path\\format", "/vkconfig/test/path/format/",   "\\vkconfig\\test/path\\format\\"};

    for (std::size_t i = 0, n = countof(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format", ConvertNativeSeparators(test_case).c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format", ConvertNativeSeparators(test_case).c_str());
        }
    }
}

TEST(test_path, native_path_with_file) {
    static const char* table[] = {"/vkconfig/test/path/format/file.txt", "\\vkconfig\\test/path\\format\\file.txt"};

    for (std::size_t i = 0, n = countof(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format\\file.txt", ConvertNativeSeparators(test_case).c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format/file.txt", ConvertNativeSeparators(test_case).c_str());
        }
    }
}

TEST(test_path, is_portable_filename_valid) {
    EXPECT_TRUE(IsPortableFilename("VUlkanConfigurator2"));
    EXPECT_TRUE(IsPortableFilename("VUlkanConfigurator2.json"));
    EXPECT_TRUE(IsPortableFilename(".VUlkanConfigurator2.json"));
}

TEST(test_path, is_portable_filename_invalid) {
    EXPECT_TRUE(!IsPortableFilename("."));
    EXPECT_TRUE(!IsPortableFilename(".."));
    EXPECT_TRUE(!IsPortableFilename("../gni"));
    EXPECT_TRUE(!IsPortableFilename("..gni.json"));
    EXPECT_TRUE(!IsPortableFilename("VulkanConfigurator2\\.json"));
    EXPECT_TRUE(!IsPortableFilename("VulkanConfigurator*"));
    EXPECT_TRUE(!IsPortableFilename("Vulkan:Configurator"));
}

TEST(test_path, get_path_local) {
    const QString value(::GetPath(BUILTIN_PATH_LOCAL).c_str());

    EXPECT_TRUE(value.endsWith("VulkanSDK"));
}

TEST(test_path, get_path_appdata) {
    const QString value(::GetPath(BUILTIN_PATH_APPDATA).c_str());

    EXPECT_TRUE(value.startsWith(::GetPath(BUILTIN_PATH_HOME).c_str()));
}

TEST(test_path, get_path_config) {
    const QString value(::GetPath(BUILTIN_PATH_CONFIG_REF).c_str());

    EXPECT_TRUE(value.startsWith(::GetPath(BUILTIN_PATH_APPDATA).c_str()));
    EXPECT_TRUE(value.endsWith("configurations"));
}

TEST(test_path, get_path_override_settings) {
    {
        const QString value(::GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS).c_str());
        EXPECT_TRUE(value.endsWith("vk_layer_settings.txt"));
    }

    {
        qputenv("VK_LAYER_SETTINGS_PATH", "~/VulkanSDK/vk_layer_settings.txt");
        const std::string value = ::GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);
        EXPECT_STREQ(ConvertNativeSeparators("~/VulkanSDK/vk_layer_settings.txt").c_str(), value.c_str());
    }

    {
        qputenv("VK_LAYER_SETTINGS_PATH", "~/VulkanSDK");
        const std::string value = ::GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);
        EXPECT_STREQ(ConvertNativeSeparators("~/VulkanSDK/vk_layer_settings.txt").c_str(), value.c_str());
    }
}

TEST(test_path, get_path_override_layers) {
    const QString value(::GetPath(BUILTIN_PATH_OVERRIDE_LAYERS).c_str());

    EXPECT_TRUE(value.endsWith("VkLayer_override.json"));
}

TEST(test_path, get_path_vulkan_sdk) {
    {
        qputenv("VULKAN_SDK", "");
        const std::string value = ::GetPath(BUILTIN_PATH_VULKAN_SDK);
        EXPECT_TRUE(!value.empty());
    }

    {
        qputenv("VULKAN_SDK", "~/VulkanSDK");
        const std::string value = ::GetPath(BUILTIN_PATH_VULKAN_SDK);
        EXPECT_STREQ(ConvertNativeSeparators("~/VulkanSDK").c_str(), value.c_str());
    }
}

TEST(test_path, get_path_vulkan_content) {
    const QString value(::GetPath(BUILTIN_PATH_VULKAN_CONTENT).c_str());

    EXPECT_TRUE(!value.isEmpty());
    EXPECT_TRUE(value.startsWith(::GetPath(BUILTIN_PATH_VULKAN_SDK).c_str()));
    EXPECT_TRUE(value.toLower().endsWith("config"));
}
