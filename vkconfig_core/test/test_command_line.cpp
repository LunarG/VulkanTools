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
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--dfhsjfgasjkgf"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_NONE, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_help) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--help"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SHOW_USAGE, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_h) {
    static char* argv[] = {(char*)"vkconfig", (char*)"-h"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SHOW_USAGE, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_version) {
    static char* argv[] = {(char*)"vkconfig", (char*)"--version"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_VERSION, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_reset_default) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_reset_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset", (char*)"--fdlkhfdklhdfkl"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(command_line.error_args.size(), 2);
    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_reset_soft) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset", (char*)"--soft"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_SOFT, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_reset_hard) {
    static char* argv[] = {(char*)"vkconfig", (char*)"reset", (char*)"--hard"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_RESET, command_line.command);
    EXPECT_EQ(COMMAND_RESET_HARD, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_layers_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_invalid_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--pouet"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_list) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--list"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_LIST, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_surrender) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--surrender"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_SURRENDER, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_surrender_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--surrender", (char*)"file.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_override) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override", (char*)":/test/reference_configuration.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_STREQ(ConvertStandardSeparators(command_line.input_path.RelativePath()).c_str(), ":/test/reference_configuration.json");
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_override_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_OVERRIDE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_layers_override_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"layers", (char*)"--override", (char*)"bla", (char*)"blo"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LAYERS, command_line.command);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
}

TEST(test_command_line, usage_mode_loader_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_invalid_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--pouet"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_list) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--list"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_LIST, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_surrender) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--surrender"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_SURRENDER, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_surrender_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--surrender", (char*)"file.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_override) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--override", (char*)":/test/reference_configuration.json"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_OVERRIDE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_STREQ(ConvertStandardSeparators(command_line.input_path.RelativePath()).c_str(), ":/test/reference_configuration.json");
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_override_invalid) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--override"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_OVERRIDE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_loader_override_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"loader", (char*)"--override", (char*)"bla", (char*)"blo"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_LOADER, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_STREQ(command_line.selected_configuration_name.c_str(), "bla");
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_missing_argv) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(1, command_line.error_args.size());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_invalid_args) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--invalid"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(2, command_line.error_args.size());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_NONE, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_TRUE(command_line.output_path.Empty());
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_hpp_default) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HPP, command_line.settings_format);
    EXPECT_STREQ(command_line.selected_configuration_name.c_str(), "default");
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "vulkan_layer_settings.hpp");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--hpp", (char*)"default"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HPP, command_line.settings_format);
    EXPECT_STREQ(command_line.selected_configuration_name.c_str(), "default");
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "vulkan_layer_settings.hpp");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_hpp_default_implicit_and_output) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--hpp", (char*)"--output", (char*)"my_settings.hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HPP, command_line.settings_format);
    EXPECT_STREQ(command_line.selected_configuration_name.c_str(), "default");
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "my_settings.hpp");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit_and_output) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--hpp",
                           (char*)"default",  (char*)"--output", (char*)"my_settings.hpp"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HPP, command_line.settings_format);
    EXPECT_STREQ(command_line.selected_configuration_name.c_str(), "default");
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "my_settings.hpp");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_hpp_default_explicit_and_output_dryrun) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings",        (char*)"--hpp",    (char*)"default",
                           (char*)"--output", (char*)"my_settings.hpp", (char*)"--dry-run"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HPP, command_line.settings_format);
    EXPECT_STREQ(command_line.selected_configuration_name.c_str(), "default");
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "my_settings.hpp");
    EXPECT_EQ(command_line.dry_run, true);
}

TEST(test_command_line, usage_mode_settings_html_default) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--html"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_MISSING_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(command_line.error_args.size(), 2);
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HTML, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "vk_layer_settings.html");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--html", (char*)"my_layer_name"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HTML, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_STREQ(command_line.selected_layer_name.c_str(), "my_layer_name");
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "vk_layer_settings.html");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_html_default_implicit_and_output) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings", (char*)"--html", (char*)"--output", (char*)"my_settings.html"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_INVALID_COMMAND_ARGUMENT, command_line.error);
    EXPECT_EQ(command_line.error_args.size(), 2);
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HTML, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_TRUE(command_line.selected_layer_name.empty());
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "vk_layer_settings.html");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit_and_output) {
    static char* argv[] = {(char*)"vkconfig",      (char*)"settings", (char*)"--html",
                           (char*)"my_layer_name", (char*)"--output", (char*)"my_settings.html"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HTML, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_STREQ(command_line.selected_layer_name.c_str(), "my_layer_name");
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "my_settings.html");
    EXPECT_EQ(command_line.dry_run, false);
}

TEST(test_command_line, usage_mode_settings_html_default_explicit_and_output_dryrun) {
    static char* argv[] = {(char*)"vkconfig", (char*)"settings",         (char*)"--html",   (char*)"my_layer_name",
                           (char*)"--output", (char*)"my_settings.html", (char*)"--dry-run"};
    int argc = static_cast<int>(std::size(argv));

    CommandLine command_line(argc, argv);

    EXPECT_EQ(ERROR_NONE, command_line.error);
    EXPECT_TRUE(command_line.error_args.empty());
    EXPECT_EQ(COMMAND_SETTINGS, command_line.command);
    EXPECT_EQ(COMMAND_RESET_NONE, command_line.command_reset_arg);
    EXPECT_EQ(COMMAND_LAYERS_NONE, command_line.command_layers_arg);
    EXPECT_EQ(COMMAND_LOADER_NONE, command_line.command_loader_arg);
    EXPECT_EQ(COMMAND_DOC_NONE, command_line.command_doc_arg);
    EXPECT_EQ(SETTINGS_FORMAT_HTML, command_line.settings_format);
    EXPECT_TRUE(command_line.selected_configuration_name.empty());
    EXPECT_STREQ(command_line.selected_layer_name.c_str(), "my_layer_name");
    EXPECT_TRUE(command_line.input_path.Empty());
    EXPECT_STREQ(command_line.output_path.RelativePath().c_str(), "my_settings.html");
    EXPECT_EQ(command_line.dry_run, true);
}

#if VKC_ENV == VKC_ENV_UNIX
#pragma GCC diagnostic pop
#pragma clang diagnostic pop
#endif
