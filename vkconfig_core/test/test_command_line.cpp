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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../command_line.h"
#include "../util.h"
#include "../type_platform.h"

#if VKC_ENV == VKC_ENV_UNIX
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwrite-strings"
#endif

#include <array>

#include <gtest/gtest.h>

TEST(test_command_line, execute_mode) {
    static char* argv[] = {(char*)"vkconfig"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_help) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--help"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SHOW_USAGE, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_h) {
    static char* argv[] = {(char*)"vkconfig", (char*)"-h"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SHOW_USAGE, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_version) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--version"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_VERSION, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--dfhsjfgasjkgf"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_UNKNOWN_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_NONE, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_invalid_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--pouet"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_list) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--list"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_LIST, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_surrender) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--surrender"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_SURRENDER, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_surrender_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--surrender", (char*)"file.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_TOO_MANY_COMMAND_ARGUMENTS, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_SURRENDER, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_override) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override", (char*)":/test/Configuration 3.0.0.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_TRUE(!command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_override_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

TEST(test_command_line, usage_mode_layers_override_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override", (char*)"bla", (char*)"blo"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_TOO_MANY_COMMAND_ARGUMENTS, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.layers_configuration_path.Empty());
}

#if VKC_ENV == VKC_ENV_UNIX
#pragma GCC diagnostic pop
#pragma clang diagnostic pop
#endif
