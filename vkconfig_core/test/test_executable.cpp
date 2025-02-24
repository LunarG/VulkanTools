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

#include "../executable.h"

#include <gtest/gtest.h>

TEST(test_executable, ctor_default) {
    Executable executable;

    EXPECT_TRUE(executable.path.Empty());
    EXPECT_TRUE(executable.enabled);
    EXPECT_STREQ("Validation", executable.configuration.c_str());
    EXPECT_TRUE(executable.GetActiveOptionsName().empty());
    EXPECT_EQ(0, executable.GetOptions().size());

    EXPECT_EQ(Executable::INVALID_OPTIONS, executable.GetActiveOptionsIndex());
    EXPECT_EQ(false, executable.RemoveActiveOptions());
    EXPECT_EQ(false, executable.DuplicateActiveOptions());
    EXPECT_EQ(nullptr, executable.GetActiveOptions());
    EXPECT_EQ(false, executable.HasActiveOptions());
}

TEST(test_executable, ctor_default_exe) {}

TEST(test_executable, ctor_path) {
    Executable executable("./vkcube");

    EXPECT_STREQ(Path("./vkcube").AbsolutePath().c_str(), executable.path.AbsolutePath().c_str());
    EXPECT_TRUE(executable.enabled);
    EXPECT_STREQ("Default Options", executable.GetActiveOptionsName().c_str());
    EXPECT_EQ(1, executable.GetOptions().size());

    EXPECT_EQ(0, executable.GetActiveOptionsIndex());
    EXPECT_EQ(true, executable.HasActiveOptions());

    const ExecutableOptions* options = executable.GetActiveOptions();
    EXPECT_STREQ("Default Options", options->label.c_str());
    EXPECT_TRUE(!options->working_folder.Empty());
    EXPECT_EQ(true, options->args.empty());
    EXPECT_EQ(true, options->envs.empty());
    EXPECT_STREQ(Path("${VK_HOME}/vkcube.txt").RelativePath().c_str(), options->log_file.RelativePath().c_str());
}

TEST(test_executable, remove_last) {
    Executable executable("./vkcube");
    EXPECT_EQ(1, executable.GetOptions().size());
    EXPECT_EQ(0, executable.GetActiveOptionsIndex());

    EXPECT_EQ(true, executable.RemoveActiveOptions());
    EXPECT_EQ(0, executable.GetOptions().size());
    EXPECT_EQ(Executable::INVALID_OPTIONS, executable.GetActiveOptionsIndex());
}

TEST(test_executable, duplicate) {
    Executable executable("./vkcube");
    EXPECT_EQ(1, executable.GetOptions().size());
    EXPECT_EQ(0, executable.GetActiveOptionsIndex());

    EXPECT_EQ(true, executable.DuplicateActiveOptions());
    EXPECT_EQ(2, executable.GetOptions().size());
    EXPECT_EQ(1, executable.GetActiveOptionsIndex());

    EXPECT_EQ(true, executable.RemoveActiveOptions());
    EXPECT_EQ(1, executable.GetOptions().size());
    EXPECT_EQ(0, executable.GetActiveOptionsIndex());

    EXPECT_EQ(true, executable.RemoveActiveOptions());
    EXPECT_EQ(0, executable.GetOptions().size());
    EXPECT_EQ(Executable::INVALID_OPTIONS, executable.GetActiveOptionsIndex());

    EXPECT_EQ(false, executable.DuplicateActiveOptions());
    EXPECT_EQ(0, executable.GetOptions().size());
    EXPECT_EQ(Executable::INVALID_OPTIONS, executable.GetActiveOptionsIndex());
}

TEST(test_executable, rename) {
    Executable executable("./vkcube");
    EXPECT_EQ(1, executable.GetOptions().size());
    EXPECT_STREQ("Default Options", executable.GetActiveOptionsName().c_str());

    executable.RenameActiveOptions("Pouet");
    EXPECT_STREQ("Pouet", executable.GetActiveOptionsName().c_str());
}

TEST(test_executable, add) {
    Executable executable("./vkcube");
    EXPECT_EQ(1, executable.GetOptions().size());
    EXPECT_STREQ("Default Options", executable.GetActiveOptionsName().c_str());

    ExecutableOptions executable_options;
    executable_options.label = "Pouet Options";
    executable_options.working_folder =
        ".";  // set it otherwise GetLocalLayersSettingsPath will use the executable absolute directory
    executable.AddOptions(executable_options);
    EXPECT_EQ(2, executable.GetOptions().size());

    EXPECT_STREQ("Default Options", executable.GetActiveOptionsName().c_str());

    executable.SetActiveOptions("Pouet Options");
    EXPECT_STREQ("Pouet Options", executable.GetActiveOptionsName().c_str());

    Path path = executable.GetLocalLayersSettingsPath();
    EXPECT_STREQ(path.AbsolutePath().c_str(), Path("./vk_layer_settings.txt").AbsolutePath().c_str());

    executable.GetActiveOptions()->working_folder.Clear();
    Path path2 = executable.GetLocalLayersSettingsPath();
    // When working_folder is not set, use the executable absolute directory
    EXPECT_STREQ(path.AbsoluteDir().c_str(), executable.path.AbsoluteDir().c_str());
}

TEST(test_executable, GetDefaultExecutablePath) {
    std::string saved = qgetenv("VULKAN_SDK").toStdString();
    qunsetenv("VULKAN_SDK");

#if defined(_WIN32)
    DefaultExecutable default_executable{"/vkcube.exe", "--suppress_popups", "vkcube launcher options"};
    const DefaultPath& default_paths = ::GetDefaultExecutablePath(default_executable.key);
    EXPECT_STREQ(default_paths.executable_path.RelativePath().c_str(), Path(".\\vkcube.exe").RelativePath().c_str());
#elif defined(__linux__)
    DefaultExecutable default_executable{"/vkcube", "--suppress_popups", "vkcube launcher options"};
    const DefaultPath& default_paths = ::GetDefaultExecutablePath(default_executable.key);
    EXPECT_STREQ(default_paths.executable_path.RelativePath().c_str(), Path(".\\vkcube").RelativePath().c_str());
#elif defined(__APPLE__)
    DefaultExecutable default_executable{"/vkcube.app", "--suppress_popups", "vkcube launcher options"};
    const DefaultPath& default_paths = ::GetDefaultExecutablePath(default_executable.key);
    EXPECT_STREQ(default_paths.executable_path.RelativePath().c_str(),
                 Path("/Applications/vkcube.app/Contents/MacOS/vkcube").RelativePath().c_str());
#endif

#if defined(_WIN32) || defined(__linux__)
    EXPECT_STREQ(default_paths.working_folder.RelativePath().c_str(), Path(".").RelativePath().c_str());
#elif defined(__APPLE__)
    EXPECT_STREQ(default_paths.working_folder.RelativePath().c_str(),
                 Path("/Applications/vkcube.app/Contents/MacOS").RelativePath().c_str());
#endif

    qputenv("VULKAN_SDK", saved.c_str());
}

TEST(test_executable, DefaultExecutable) {
    std::string saved = qgetenv("VULKAN_SDK").toStdString();
    qunsetenv("VULKAN_SDK");

#if defined(_WIN32)
    DefaultExecutable default_executable{"/vkcube.exe", "vkcube", "--suppress_popups", "vkcube launcher options"};
    Executable executable(default_executable);
    EXPECT_STREQ(executable.path.RelativePath().c_str(), Path(".\\vkcube.exe").RelativePath().c_str());
#elif defined(__linux__)
    DefaultExecutable default_executable{"/vkcube", "vkcube", "--suppress_popups", "vkcube launcher options"};
    Executable executable(default_executable);
    EXPECT_STREQ(executable.path.RelativePath().c_str(), Path(".\\vkcube").RelativePath().c_str());
#elif defined(__APPLE__)
    DefaultExecutable default_executable{"/vkcube.app", "vkcube", "--suppress_popups", "vkcube launcher options"};
    Executable executable(default_executable);
    EXPECT_STREQ(executable.path.RelativePath().c_str(),
                 Path("/Applications/vkcube.app/Contents/MacOS/vkcube").RelativePath().c_str());
#endif

    EXPECT_EQ(1, executable.GetOptions().size());
    EXPECT_STREQ(executable.configuration.c_str(), "Validation");
    EXPECT_EQ(executable.enabled, true);
    EXPECT_STREQ(executable.GetActiveOptionsName().c_str(), "vkcube launcher options");
    const ExecutableOptions* options = executable.GetActiveOptions();
#if defined(_WIN32) || defined(__linux__)
    EXPECT_STREQ(options->working_folder.RelativePath().c_str(), Path(".").RelativePath().c_str());
#elif defined(__APPLE__)
    EXPECT_STREQ(options->working_folder.RelativePath().c_str(),
                 Path("/Applications/vkcube.app/Contents/MacOS").RelativePath().c_str());
#endif
    EXPECT_STREQ(options->args[0].c_str(), "--suppress_popups");
    EXPECT_TRUE(options->envs.empty());
    EXPECT_STREQ(options->log_file.RelativePath().c_str(), Path("${VK_HOME}/vkcube.txt").RelativePath().c_str());

    qputenv("VULKAN_SDK", saved.c_str());
}
