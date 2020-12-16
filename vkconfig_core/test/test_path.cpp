/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
TEST(test_util, get_path) { EXPECT_STRNE(GetPath(BUILTIN_PATH_HOME).c_str(), ""); }

TEST(test_util, replace_path_home) {
    const std::string replaced_path = ReplaceBuiltInVariable("${HOME}/test.txt");

    EXPECT_TRUE(replaced_path.find("${HOME}") > replaced_path.size());
}

TEST(test_util, replace_path_vulkan_sdk) {
    const std::string replaced_path = ReplaceBuiltInVariable("${VULKAN_SDK}/test.txt");

    EXPECT_TRUE(replaced_path.find("${VULKAN_SDK}") > replaced_path.size());
}

TEST(test_util, replace_path_unknown) {
    const std::string replaced_path = ReplaceBuiltInVariable("${UNKNOWN}/test.txt");

    EXPECT_STREQ("${UNKNOWN}/test.txt", replaced_path.c_str());
}

TEST(test_util, convert_native_separator_empty) {
    const std::string replaced_path = ReplaceBuiltInVariable("");

    EXPECT_STREQ("", replaced_path.c_str());
}

TEST(test_util, native_path) {
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

TEST(test_util, native_path_with_file) {
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

TEST(test_util, is_portable_filename_valid) {
    EXPECT_TRUE(IsPortableFilename("VUlkanConfigurator2"));
    EXPECT_TRUE(IsPortableFilename("VUlkanConfigurator2.json"));
    EXPECT_TRUE(IsPortableFilename(".VUlkanConfigurator2.json"));
}

TEST(test_util, is_portable_filename_invalid) {
    EXPECT_TRUE(!IsPortableFilename("."));
    EXPECT_TRUE(!IsPortableFilename(".."));
    EXPECT_TRUE(!IsPortableFilename("../gni"));
    EXPECT_TRUE(!IsPortableFilename("..gni.json"));
    EXPECT_TRUE(!IsPortableFilename("VulkanConfigurator2\\.json"));
    EXPECT_TRUE(!IsPortableFilename("VulkanConfigurator*"));
    EXPECT_TRUE(!IsPortableFilename("Vulkan:Configurator"));
}
