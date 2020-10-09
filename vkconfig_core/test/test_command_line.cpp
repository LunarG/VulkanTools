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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../command_line.h"
#include "../util.h"

#include <array>

#include <gtest/gtest.h>

TEST(test_command_line, execute_mode) {
    static char* argv[] = {"vkconfig"};
    int argc = static_cast<int>(countof(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(CommandLine::ModeGUI, command_line.mode);
}

TEST(test_command_line, usage_mode_help) {
    static char* argv[] = {"vkconfig", "--help"};
    int argc = static_cast<int>(countof(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(CommandLine::ModeShowUsage, command_line.mode);
}

TEST(test_command_line, usage_mode_h) {
    static char* argv[] = {"vkconfig", "-h"};
    int argc = static_cast<int>(countof(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(CommandLine::ModeShowUsage, command_line.mode);
}

TEST(test_command_line, usage_mode_invalid) {
    static char* argv[] = {"vkconfig", "--dfhsjfgasjkgf"};
    int argc = static_cast<int>(countof(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(CommandLine::ModeShowUsage, command_line.mode);
}
