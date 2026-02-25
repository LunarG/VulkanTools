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

    EXPECT_EQ(COMMAND_SHOW_USAGE, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--dfhsjfgasjkgf"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_NONE, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_INVALID_COMMAND, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_help) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--help"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SHOW_USAGE, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_version) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--version"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_VERSION, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_reset_default) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_reset_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset", (char*)"--fdlkhfdklhdfkl"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(command_line.error_args.size(), 2);
    EXPECT_EQ(HELP_RESET, command_line.help);
}

TEST(test_command_line, usage_mode_reset_soft) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset", (char*)"--soft"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_reset_hard) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset", (char*)"--hard"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_HARD, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_layers_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(HELP_LAYERS, command_line.help);
}

TEST(test_command_line, usage_mode_layers_invalid_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--pouet"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LAYERS, command_line.help);
}

TEST(test_command_line, usage_mode_layers_list) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--list"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_LIST, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_layers_surrender) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--surrender"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_SURRENDER, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_layers_surrender_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--surrender", (char*)"--hsdghdskjgdskjl"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_SURRENDER, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LAYERS, command_line.help);
}

TEST(test_command_line, usage_mode_layers_override) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override", (char*)":/test/reference_configuration.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_STREQ(":/test/reference_configuration.json",
                 ConvertStandardSeparators(command_line.GetInputPath().RelativePath()).c_str());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_layers_override_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LAYERS, command_line.help);
}

TEST(test_command_line, usage_mode_layers_override_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override", (char*)"bla", (char*)"blo"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("bla", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_FALSE(command_line.dry_run);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LAYERS, command_line.help);
}

TEST(test_command_line, usage_mode_loader_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(HELP_LOADER, command_line.help);
}

TEST(test_command_line, usage_mode_loader_invalid_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--dfgklhsgdklh"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LOADER, command_line.help);
}

TEST(test_command_line, usage_mode_loader_list) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--list"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_LIST, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_loader_surrender) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--surrender"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_SURRENDER, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_loader_surrender_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--surrender", (char*)"hgflksghflksdh"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_SURRENDER, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LOADER, command_line.help);
}

TEST(test_command_line, usage_mode_loader_override) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--override", (char*)":/test/reference_configuration.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_OVERRIDE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_STREQ(ConvertStandardSeparators(command_line.GetInputPath().RelativePath()).c_str(),
                 ":/test/reference_configuration.json");
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_loader_override_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--override"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_OVERRIDE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LOADER, command_line.help);
}

TEST(test_command_line, usage_mode_loader_override_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--override", (char*)"bla", (char*)"blo"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_OVERRIDE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("bla", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_LOADER, command_line.help);
}

TEST(test_command_line, usage_mode_settings_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(HELP_SETTINGS, command_line.help);
}

TEST(test_command_line, usage_mode_settings_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--invalid"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_TXT, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.txt", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(HELP_SETTINGS, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings",  // settings
                           (char*)"--generate", (char*)"hpp"};    // generate
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vulkan_layer_settings.hpp", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit) {
    static char* argv[] = {(char*)"vkconfig",        (char*)"settings",  // settings
                           (char*)"--generate",      (char*)"hpp",       // generate
                           (char*)"--configuration", (char*)"default"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vulkan_layer_settings.hpp", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default_implicit_and_output) {
    static char* argv[] = {(char*)"vkconfig",   (char*)"settings",  // settings
                           (char*)"--generate", (char*)"hpp",       // generate
                           (char*)"--output",   (char*)"my_settings.hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_settings.hpp", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit_and_output) {
    static char* argv[] = {(char*)"vkconfig",        (char*)"settings",  // settings
                           (char*)"--generate",      (char*)"hpp",       // generate
                           (char*)"--configuration", (char*)"default",   // configuration
                           (char*)"--output",        (char*)"my_settings.hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_settings.hpp", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit_and_output_dir) {
    static char* argv[] = {(char*)"vkconfig",        (char*)"settings",  // settings
                           (char*)"--generate",      (char*)"hpp",       // generate
                           (char*)"--configuration", (char*)"default",   // configuration
                           (char*)"--output-dir",    (char*)"./tmp",    (char*)"--output", (char*)"my_settings.hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("./tmp/my_settings.hpp", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit_and_output_dir_and_path) {
    static char* argv[] = {(char*)"vkconfig",        (char*)"settings",  // settings
                           (char*)"--generate",      (char*)"hpp",       // generate
                           (char*)"--configuration", (char*)"default",   // configuration
                           (char*)"--output-dir",    (char*)"./tmp",    (char*)"--output", (char*)"./gni/my_settings.hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("./tmp/my_settings.hpp", ::ConvertStandardSeparators(command_line.GetOutputPath().RelativePath()).c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit_and_output_dryrun) {
    static char* argv[] = {(char*)"vkconfig",        (char*)"settings",  // settings
                           (char*)"--generate",      (char*)"hpp",       // generate
                           (char*)"--configuration", (char*)"default",   // configuration
                           (char*)"--output",        (char*)"my_settings.hpp",
                           (char*)"--dry-run"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HPP_VULKAN_H, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_settings.hpp", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(command_line.dry_run, true);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_html_default) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings",  // settings
                           (char*)"--generate", (char*)"html"};   // generate
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HTML, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.html", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(command_line.dry_run, false);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit) {
    static char* argv[] = {(char*)"vkconfig",   (char*)"settings",  // settings
                           (char*)"--generate", (char*)"html",      // generate
                           (char*)"--layers",   (char*)"my_layer_name"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HTML, command_line.generate_settings_mode);
    EXPECT_STREQ("my_layer_name", command_line.selected_layers_name[0].c_str());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_layer_name.html", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(false, command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit_multiple_layers) {
    static char* argv[] = {(char*)"vkconfig",   (char*)"settings",  // settings
                           (char*)"--generate", (char*)"html",      // generate
                           (char*)"--layers",   (char*)"layer0,layer1"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HTML, command_line.generate_settings_mode);
    EXPECT_STREQ("layer0", command_line.selected_layers_name[0].c_str());
    EXPECT_STREQ("layer1", command_line.selected_layers_name[1].c_str());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("vk_layer_settings.html", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(false, command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_html_default_implicit_and_output) {
    static char* argv[] = {(char*)"vkconfig",   (char*)"settings",  // settings
                           (char*)"--generate", (char*)"html",      // generate
                           (char*)"--output",   (char*)"my_settings.html"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HTML, command_line.generate_settings_mode);
    EXPECT_TRUE(command_line.selected_layers_name.empty());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_settings.html", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(false, command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit_and_output) {
    static char* argv[] = {(char*)"vkconfig",   (char*)"settings",       // settings
                           (char*)"--generate", (char*)"html",           // generate
                           (char*)"--layers",   (char*)"my_layer_name",  // layers
                           (char*)"--output",   (char*)"my_settings.html"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HTML, command_line.generate_settings_mode);
    EXPECT_STREQ("my_layer_name", command_line.selected_layers_name[0].c_str());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_settings.html", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(false, command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit_and_output_dryrun) {
    static char* argv[] = {(char*)"vkconfig",   (char*)"settings",       // settings
                           (char*)"--generate", (char*)"html",           // generate
                           (char*)"--layers",   (char*)"my_layer_name",  // layers
                           (char*)"--output",   (char*)"my_settings.html",
                           (char*)"--dry-run"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(GENERATE_SETTINGS_HTML, command_line.generate_settings_mode);
    EXPECT_STREQ("my_layer_name", command_line.selected_layers_name[0].c_str());
    EXPECT_STREQ("default", command_line.selected_configuration_name.c_str());
    EXPECT_TRUE(command_line.GetInputPath().Empty());
    EXPECT_STREQ("my_settings.html", command_line.GetOutputPath().RelativePath().c_str());
    EXPECT_EQ(true, command_line.dry_run);
    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(HELP_DEFAULT, command_line.help);
}

#if VKC_ENV == VKC_ENV_UNIX
#pragma GCC diagnostic pop
#pragma clang diagnostic pop
#endif
