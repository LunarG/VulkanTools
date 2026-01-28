/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
#include "../version.h"
#include "../type_platform.h"

#include <array>

#include <QDir>

#include <gtest/gtest.h>

TEST(test_path, native_path) {
    static const char* table[] = {"/vkconfig/test\\path/format/", "/vkconfig/test\\path/format\\",
                                  "/vkconfig\\test/path/format/", "/vkconfig\\test/path\\format/",
                                  "/vkconfig/test/path/format/",  "\\vkconfig\\test/path\\format\\"};

    for (std::size_t i = 0, n = std::size(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_ENV == VKC_ENV_WIN32) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format\\", Path(test_case).RelativePath().c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format/", Path(test_case).RelativePath().c_str());
        }
    }
}

TEST(test_path, native_compare) {
    static const Path table[] = {"/vkconfig/test\\path/format/",
                                 "/vkconfig/test\\path/format\\",
                                 "/vkconfig\\test/path/format/",
                                 "/vkconfig\\test/path\\format/",
                                 "/vkconfig/test/path/format/",
                                 "\\vkconfig\\test/path\\format\\",
                                 "/vkconfig/test\\path/format",
                                 "/vkconfig/test\\path/format",
                                 "/vkconfig\\test/path/format",
                                 "/vkconfig\\test/path\\format",
                                 "/vkconfig/test/path/format",
                                 "\\vkconfig\\test/path\\format",
#if (VKC_ENV == VKC_ENV_WIN32)  // Only on Windows the case doesn't matter for paths
                                 "/vkConfig/test\\path/format/",
                                 "/vkConfig/Test\\Path/Format\\",
                                 "/vkconfig\\test/path/Format/",
                                 "/vkConfig\\test/path\\format/",
                                 "/vkConfig/Test/Path/Format/",
                                 "\\vkconfig\\test/path\\Format\\",
                                 "/vkConfig/test\\path/format",
                                 "/vkConfig/Test\\Path/Format",
                                 "/vkconfig\\test/path/Format",
                                 "/vkConfig\\test/path\\format",
                                 "/vkConfig/Test/Path/Format",
                                 "\\vkconfig\\test/path\\Format"
#endif
    };

    for (std::size_t j = 0, n = std::size(table); j < n; ++j) {
        const Path testJ = table[j];
        for (std::size_t i = 0, n = std::size(table); i < n; ++i) {
            const Path testI = table[i];

            EXPECT_TRUE(testI == testJ);
            EXPECT_FALSE(testI < testJ);
        }
    }
}

TEST(test_path, native_path_without_end_separator) {
    static const char* table[] = {"/vkconfig\\test/path/format", "/vkconfig/test/path/format", "\\vkconfig\\test/path\\format"};

    for (std::size_t i = 0, n = std::size(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_ENV == VKC_ENV_WIN32) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format", Path(test_case).RelativePath().c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format", Path(test_case).RelativePath().c_str());
        }
    }
}

TEST(test_path, native_path_with_file) {
    static const char* table[] = {"/vkconfig/test/path/format/file.txt", "\\vkconfig\\test/path\\format\\file.txt"};

    for (std::size_t i = 0, n = std::size(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_ENV == VKC_ENV_WIN32) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format\\file.txt", Path(test_case).RelativePath().c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format/file.txt", Path(test_case).RelativePath().c_str());
        }
    }
}

TEST(test_path, path_addition) {
    Path pathR("testA/test");
    Path pathA("testA\\test");
    Path pathB = std::string("testA") + "\\test";
    Path pathC = std::string("testA") + "/test";

    EXPECT_TRUE(pathR == pathA);
    EXPECT_TRUE(pathR == pathB);
    EXPECT_TRUE(pathR == pathC);
    EXPECT_STREQ(pathR.RelativePath().c_str(), pathA.RelativePath().c_str());
    EXPECT_STREQ(pathR.RelativePath().c_str(), pathB.RelativePath().c_str());
    EXPECT_STREQ(pathR.RelativePath().c_str(), pathC.RelativePath().c_str());
    EXPECT_STREQ(pathR.AbsolutePath().c_str(), pathA.AbsolutePath().c_str());
    EXPECT_STREQ(pathR.AbsolutePath().c_str(), pathB.AbsolutePath().c_str());
    EXPECT_STREQ(pathR.AbsolutePath().c_str(), pathC.AbsolutePath().c_str());
}

TEST(test_path, path_addition_clear_separator) {
    Path pathA = Path("testA").RelativePath() + "\\" + "test";
    EXPECT_STREQ(pathA.RelativePath().c_str(), Path("testA/test").RelativePath().c_str());

    Path pathB = Path("testB").RelativePath() + "/" + "test";
    EXPECT_STREQ(pathB.RelativePath().c_str(), Path("testB/test").RelativePath().c_str());
}

TEST(test_path, path_operation_clear) {
    static const char* table[] = {"/vkconfig/test/path/format/file.txt", "\\vkconfig\\test/path\\format\\file.txt",
                                  "/vkconfig\\test/path/format/", ""};

    for (std::size_t i = 0, n = std::size(table); i < n; ++i) {
        Path test_case = Path(table[i]);
        test_case.Clear();

        EXPECT_TRUE(test_case.Empty());
    }
}

TEST(test_path, path_operations_valid) {
    struct test_case {
        std::string path;
        bool is_file;
    };

    static const test_case CASES[] = {{"./fileA.pouet", true}, {"./testA/", false}};

    for (std::size_t i = 0, n = std::size(CASES); i < n; ++i) {
        Path test_case = Path(CASES[i].path.c_str());
        EXPECT_FALSE(test_case.Exists());
        EXPECT_TRUE(test_case.Create(CASES[i].is_file));
        EXPECT_TRUE(test_case.Exists());
        EXPECT_EQ(test_case.IsFile(), CASES[i].is_file);
        EXPECT_TRUE(test_case.Create(CASES[i].is_file));
        EXPECT_TRUE(test_case.Exists());
        EXPECT_TRUE(test_case.Remove());
        EXPECT_FALSE(test_case.Exists());
        EXPECT_FALSE(test_case.Remove());
        EXPECT_FALSE(test_case.Exists());

        test_case.Clear();
        EXPECT_TRUE(test_case.Empty());
    }
}

TEST(test_path, path_operations_invalid) {
    Path test_case("");
    EXPECT_TRUE(test_case.Empty());
    EXPECT_FALSE(test_case.Exists());
    EXPECT_FALSE(test_case.Create(true));
    EXPECT_FALSE(test_case.Create(false));
    EXPECT_FALSE(test_case.IsFile());
    EXPECT_FALSE(test_case.IsDir());
    EXPECT_FALSE(test_case.Remove());
};

// Test that GetPath return the home directory when the stored path is empty
TEST(test_path, get_path) { EXPECT_STRNE(Path(Path::HOME).RelativePath().c_str(), ""); }

TEST(test_path, replace_path_vk_home) {
    const std::string replaced_path = Path("${VULKAN_HOME}/test.txt").AbsolutePath();

    EXPECT_TRUE(replaced_path.find("${VULKAN_HOME}") > replaced_path.size());
}

TEST(test_path, replace_path_vk_appdata) {
    const std::string replaced_path = Path("${VULKAN_APPDATA}/test.txt").AbsolutePath();

    EXPECT_TRUE(replaced_path.find("${VULKAN_APPDATA}") > replaced_path.size());
}

TEST(test_path, replace_path_unknown) {
    const std::string replaced_path = Path("${UNKNOWN}/test.txt").AbsolutePath();

    // unknown variables are not replaced or modified
    EXPECT_TRUE(replaced_path.find("${UNKNOWN}") == 0);
}

TEST(test_path, convert_to_variable) {
    const std::string replaced_path = Path(Path::SDK).AbsolutePath();

    Path recovered_path(replaced_path);

    EXPECT_EQ(Path(Path::SDK), recovered_path);
}

TEST(test_path, recover) {
    Path reference = Path("${VULKAN_HOME}/test.txt");
    Path absolute_path = reference.AbsolutePath();
    EXPECT_EQ(reference, absolute_path);

    std::string absolute_string = reference.AbsolutePath();
    Path recover = absolute_string;

    EXPECT_EQ(reference, recover);
}

TEST(test_path, convert_to_variable_postfix) {
    const Path base_path(Path(Path::SDK).AbsolutePath());
    const Path reference_path = base_path.RelativePath() + "/TestA";

    const std::string replaced_path = reference_path.AbsolutePath();
    Path recovered_path(replaced_path);

    EXPECT_EQ(reference_path, recovered_path);
}

TEST(test_path, convert_native_separator_empty) {
    const std::string replaced_path = Path("").AbsolutePath();

    EXPECT_STREQ("", replaced_path.c_str());
}

TEST(test_path, get_path_home) {
    const std::string value_default(AbsolutePath(Path::HOME).c_str());
    qputenv("VULKAN_HOME", ":/MyVulkanSDKLocalDir");

    const std::string value_env(AbsolutePath(Path::HOME).c_str());
    EXPECT_TRUE(EndsWith(value_env, "MyVulkanSDKLocalDir"));

    Path convert(":/MyVulkanSDKLocalDir");
    EXPECT_STREQ(convert.RelativePath().c_str(), "${VULKAN_HOME}");
}

TEST(test_path, get_path_appdata) {
    const std::string data(AbsolutePath(Path::APPDATA).c_str());
    const std::string home(Path(QDir().homePath().toStdString()).AbsolutePath());

    EXPECT_TRUE(data.find(home.c_str()) == 0);
}

TEST(test_path, get_path_init) {
    const std::string init(AbsolutePath(Path::INIT).c_str());
    const std::string data(AbsolutePath(Path::APPDATA).c_str());

    EXPECT_TRUE(init.find(data.c_str()) != std::string::npos);
    EXPECT_TRUE(init.find("vkconfig.json") != std::string::npos);
}

TEST(test_path, get_path_config) {
    const std::string path = AbsolutePath(Path::CONFIGS);
    const QString value(path.c_str());

    EXPECT_TRUE(value.startsWith(path.c_str()));
    EXPECT_TRUE(value.endsWith("configurations"));
}

TEST(test_path, get_path_layers_settings) {
    const std::string value(AbsolutePath(Path::LAYERS_SETTINGS).c_str());

    EXPECT_TRUE(value.find("vk_layer_settings.txt") != std::string::npos);
    EXPECT_TRUE(value.find(AbsolutePath(Path::APPDATA).c_str()) != std::string::npos);
}

TEST(test_path, get_path_loader_settings) {
    const std::string value(AbsolutePath(Path::LOADER_SETTINGS).c_str());

    EXPECT_TRUE(value.find("vk_loader_settings.json") != std::string::npos);
    EXPECT_TRUE(value.find(AbsolutePath(Path::APPDATA).c_str()) != std::string::npos);
}

TEST(test_path, get_path_override_settings) {
    Path path("~/TestVulkanDirectory");
    qputenv("VULKAN_SDK", path.AbsolutePath().c_str());

    {
        const QString value(AbsolutePath(Path::LAYERS_SETTINGS).c_str());
        EXPECT_TRUE(value.endsWith("vk_layer_settings.txt"));
    }

    {
        qputenv("VK_LAYER_SETTINGS_PATH", "~/TestVulkanDirectory/vk_layer_settings.txt");
        const std::string value = AbsolutePath(Path::LAYERS_SETTINGS).c_str();
        EXPECT_STREQ(Path("~/TestVulkanDirectory/vk_layer_settings.txt").AbsolutePath().c_str(), value.c_str());
    }

    {
        qputenv("VK_LAYER_SETTINGS_PATH", "~/TestVulkanDirectory");
        const std::string value = AbsolutePath(Path::LAYERS_SETTINGS).c_str();
        EXPECT_STREQ(Path("~/TestVulkanDirectory/vk_layer_settings.txt").AbsolutePath().c_str(), value.c_str());
    }
}

TEST(test_path, get_path_override_layers) {
    const QString value(AbsolutePath(Path::LOADER_SETTINGS).c_str());

    EXPECT_TRUE(value.endsWith("vk_loader_settings.json"));
}

TEST(test_path, get_path_vulkan_sdk) {
    qputenv("VULKAN_SDK", "~/TestVulkanDirectory");
    const std::string value = AbsolutePath(Path::SDK);
    EXPECT_STREQ(Path("~/TestVulkanDirectory").AbsolutePath().c_str(), value.c_str());
}

TEST(test_path, get_path_vulkan_content) {
    const QString value(AbsolutePath(Path::CONTENT).c_str());

    EXPECT_TRUE(!value.isEmpty());
    EXPECT_TRUE(value.startsWith(AbsolutePath(Path::SDK).c_str()));
    EXPECT_TRUE(value.toLower().endsWith("config"));
}

TEST(test_path, get_path_home_sdk) {
    Path home_default = Path(Path::DEFAULT_HOME);
    Path home_current = Path(Path::HOME);
    EXPECT_STREQ(home_current.AbsolutePath().c_str(), home_default.AbsolutePath().c_str());

    qputenv("VULKAN_SDK", "~/TestVulkanDirectory");
    const std::string value = AbsolutePath(Path::SDK);
    EXPECT_STREQ(Path("~/TestVulkanDirectory").AbsolutePath().c_str(), value.c_str());
}

TEST(test_path, get_path_url_sdk) {
    const std::string value = AbsolutePath(Path::URL_SDK, false);
    const std::string expected = format("https://vulkan.lunarg.com/doc/sdk/%s.0/", Version::VKHEADER.str().c_str());

    EXPECT_TRUE(value.find(expected) != std::string::npos);
}

TEST(test_path, collect_file_paths_success_set2) {
    const std::vector<Path>& paths = CollectFilePaths(":/layers/");

    EXPECT_EQ(paths.size(), 17);
    EXPECT_STREQ(Path(":/layers/VK_LAYER_LUNARG_reference_1_1_0.json").AbsolutePath().c_str(), paths[0].AbsolutePath().c_str());
}

TEST(test_path, collect_file_paths_success_set3) {
    const std::vector<Path>& paths = ::CollectFilePaths(":/profiles/");

    EXPECT_EQ(paths.size(), 4);
    EXPECT_STREQ(Path(":/profiles/VP_KHR_roadmap.json").AbsolutePath().c_str(), paths[0].AbsolutePath().c_str());
}

TEST(test_path, collect_file_paths_not_found) {
    const std::vector<Path>& result = ::CollectFilePaths(":/configurations_not_found/");

    EXPECT_EQ(result.empty(), true);
}

TEST(test_path, collect_profiles_from_file) {
    const std::vector<std::string>& result = ::CollectProfileNamesFromFile(":/profiles/VP_KHR_roadmap.json");

    EXPECT_EQ(result.size(), 2);
    EXPECT_STREQ("VP_KHR_roadmap_2022", result[0].c_str());
    EXPECT_STREQ("VP_KHR_roadmap_2024", result[1].c_str());
}

TEST(test_path, collect_profiles_from_dir) {
    const std::vector<std::string>& result = ::CollectProfileNamesFromDir(":/profiles/");

    EXPECT_EQ(result.size(), 9);
    EXPECT_STREQ("VP_KHR_roadmap_2022", result[0].c_str());
    EXPECT_STREQ("VP_KHR_roadmap_2024", result[1].c_str());
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
