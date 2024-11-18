/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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
    EXPECT_TRUE(!executable.GetActiveOptionsName().empty());
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
