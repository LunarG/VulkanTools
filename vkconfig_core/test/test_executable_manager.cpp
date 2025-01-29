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

#include "../executable_manager.h"

#include <gtest/gtest.h>

#include <QFile>

TEST(test_executable_manager, reset_default_applications_sdk_found) {
    ExecutableManager executable_manager;
    executable_manager.Reset();

    const std::vector<Executable>& executables = executable_manager.GetExecutables();

    std::string result = qgetenv("VULKAN_SDK").toStdString();

    if (!result.empty()) {
        EXPECT_EQ(3, executables.size());

        // Make sure the variable are not replaced
        EXPECT_TRUE(executables[0].path.RelativePath().find("${VULKAN_BIN}") != std::string::npos);
        const std::vector<ExecutableOptions>& options0 = executables[0].GetOptions();
        EXPECT_TRUE(options0[0].working_folder.RelativePath().find("${VULKAN_BIN}") != std::string::npos);
        EXPECT_TRUE(options0[0].log_file.RelativePath().find("${VK_HOME}") != std::string::npos);

        EXPECT_TRUE(executables[1].path.RelativePath().find("${VULKAN_BIN}") != std::string::npos);
        const std::vector<ExecutableOptions>& options1 = executables[1].GetOptions();
        EXPECT_TRUE(options1[0].working_folder.RelativePath().find("${VULKAN_BIN}") != std::string::npos);
        EXPECT_TRUE(options1[0].log_file.RelativePath().find("${VK_HOME}") != std::string::npos);

        EXPECT_TRUE(executables[2].path.RelativePath().find("${VULKAN_BIN}") != std::string::npos);
        const std::vector<ExecutableOptions>& options2 = executables[2].GetOptions();
        EXPECT_TRUE(options2[0].working_folder.RelativePath().find("${VULKAN_BIN}") != std::string::npos);
        EXPECT_TRUE(options2[0].log_file.RelativePath().find("${VK_HOME}") != std::string::npos);
    }
}

TEST(test_executable_manager, reset_default_applications_no_sdk) {
    qunsetenv("VULKAN_SDK");

    ExecutableManager executable_manager;
    executable_manager.Reset();

    const std::vector<Executable>& executables = executable_manager.GetExecutables();

    EXPECT_EQ(3, executables.size());

    EXPECT_TRUE(executables[0].path.RelativePath().find("vkcube") != std::string::npos);
    const std::vector<ExecutableOptions>& options0 = executables[0].GetOptions();
    EXPECT_TRUE(options0[0].working_folder.RelativePath().find(".") != std::string::npos);
    EXPECT_TRUE(options0[0].log_file.RelativePath().find("${VK_HOME}") != std::string::npos);

    EXPECT_TRUE(executables[1].path.RelativePath().find("vkcubepp") != std::string::npos);
    const std::vector<ExecutableOptions>& options1 = executables[1].GetOptions();
    EXPECT_TRUE(options1[0].working_folder.RelativePath().find(".") != std::string::npos);
    EXPECT_TRUE(options1[0].log_file.RelativePath().find("${VK_HOME}") != std::string::npos);

    EXPECT_TRUE(executables[2].path.RelativePath().find("vulkaninfo") != std::string::npos);
    const std::vector<ExecutableOptions>& options2 = executables[2].GetOptions();
    EXPECT_TRUE(options2[0].working_folder.RelativePath().find(".") != std::string::npos);
    EXPECT_TRUE(options2[0].log_file.RelativePath().find("${VK_HOME}") != std::string::npos);
}

TEST(test_executable_manager, active_executable) {
    qunsetenv("VULKAN_SDK");

    ExecutableManager executable_manager;
    executable_manager.Reset();

    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), 0);
    EXPECT_EQ(executable_manager.GetExecutable(executable_manager.GetActiveExecutableIndex()),
              executable_manager.GetActiveExecutable());

    executable_manager.SetActiveExecutable(1);
    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), 1);
    EXPECT_EQ(executable_manager.GetExecutable(executable_manager.GetActiveExecutableIndex()),
              executable_manager.GetActiveExecutable());

    const Executable* executable = executable_manager.GetActiveExecutable();
    Path saved_path = executable->path;

    EXPECT_TRUE(executable_manager.RemoveExecutable());

    executable_manager.RenameActiveExecutable(saved_path);
    EXPECT_STREQ(saved_path.AbsolutePath().c_str(), executable_manager.GetActiveExecutable()->path.AbsolutePath().c_str());
}

TEST(test_executable_manager, remove_executable) {
    qunsetenv("VULKAN_SDK");

    ExecutableManager executable_manager;
    executable_manager.Reset();

    EXPECT_EQ(3, executable_manager.GetExecutables().size());

    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), 0);

    executable_manager.SetActiveExecutable(2);
    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), 2);

    EXPECT_TRUE(executable_manager.RemoveExecutable());
    EXPECT_EQ(2, executable_manager.GetExecutables().size());
    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), 0);

    EXPECT_TRUE(executable_manager.RemoveExecutable());
    EXPECT_EQ(1, executable_manager.GetExecutables().size());
    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), 0);

    EXPECT_TRUE(executable_manager.RemoveExecutable());
    EXPECT_EQ(0, executable_manager.GetExecutables().size());
    EXPECT_EQ(executable_manager.GetActiveExecutableIndex(), -1);

    EXPECT_FALSE(executable_manager.RemoveExecutable());
}

TEST(test_executable_manager, remove_missing_applications) {
    ExecutableManager executable_manager;

    const Path& path_missing = ::Path(Path::HOME).RelativePath() + "my_missing_executable";
    const Path& path_exciting = ::Path(Path::HOME).RelativePath() + "my_exciting_executable";

    QFile file(path_exciting.AbsolutePath().c_str());
    const bool result = file.open(QIODevice::WriteOnly);
    ASSERT_TRUE(result);

    std::vector<Executable> executables;
    Executable executable;

    executable.path = path_missing.AbsolutePath();
    executables.push_back(executable);

    executable.path = path_exciting.AbsolutePath();
    executables.push_back(executable);

    EXPECT_EQ(1, executable_manager.RemoveMissingExecutables(executables).size());
}
