/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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

#pragma once

#include "path.h"
#include "type_generate_settings.h"

#include <string>
#include <vector>

enum CommandType {
    COMMAND_NONE = 0,
    COMMAND_SHOW_USAGE,
    COMMAND_VERSION,
    COMMAND_GUI,
    COMMAND_RESET,
    COMMAND_LAYERS,
    COMMAND_LOADER,
    COMMAND_SETTINGS,
    COMMAND_DOC
};

enum CommandLayersArg {
    COMMAND_LAYERS_NONE = 0,
    COMMAND_LAYERS_OVERRIDE,
    COMMAND_LAYERS_SURRENDER,
    COMMAND_LAYERS_PATH,
    COMMAND_LAYERS_LIST,
    COMMAND_LAYERS_VERBOSE
};

enum CommandLoaderArg {
    COMMAND_LOADER_NONE = 0,
    COMMAND_LOADER_OVERRIDE,
    COMMAND_LOADER_SURRENDER,
    COMMAND_LOADER_LIST,
    COMMAND_LOADER_IMPORT,
    COMMAND_LOADER_EXPORT,
    COMMAND_LOADER_DELETE
};

enum CommandDocArg { COMMAND_DOC_NONE = 0, COMMAND_DOC_HTML, COMMAND_DOC_MARKDOWN, COMMAND_DOC_SETTINGS };

enum CommandResetArg { COMMAND_RESET_NONE = 0, COMMAND_RESET_SOFT, COMMAND_RESET_HARD };

enum CommandSettingsArg {
    COMMAND_SETTINGS_NONE = 0,
    COMMAND_SETTINGS_GENERATE,
    COMMAND_SETTINGS_CONFIG,
    COMMAND_SETTINGS_LAYER,
    COMMAND_SETTINGS_OUTPUT,
    COMMAND_SETTINGS_OUTPUT_DIR,
    COMMAND_SETTINGS_DRY_RUN,

    COMMAND_SETTINGS_FIRST = COMMAND_SETTINGS_NONE,
    COMMAND_SETTINGS_LAST = COMMAND_SETTINGS_DRY_RUN,
};

enum { COMMAND_SETTINGS_COUNT = COMMAND_SETTINGS_LAST - COMMAND_SETTINGS_FIRST + 1 };

enum CommandError {
    ERROR_NONE = 0,
    ERROR_INVALID_COMMAND,
    ERROR_INVALID_COMMAND_ARGUMENT,
    ERROR_TOO_MANY_COMMAND_ARGUMENTS,
    ERROR_MISSING_COMMAND_ARGUMENT,
    ERROR_FILE_NOTFOUND
};

enum HelpType {
    HELP_NONE = 0,
    HELP_DEFAULT,
    HELP_HELP,
    HELP_VERSION,
    HELP_LAYERS,
    HELP_LOADER,
    HELP_SETTINGS,
    HELP_GUI,
    HELP_DOC,
    HELP_RESET
};

class CommandLine {
   public:
    CommandLine(int argc, char* argv[]);
    void log() const;
    void usage() const;
    void version() const;

    Path GetInputPath() const;
    Path GetOutputPath() const;

    const CommandType& command;
    const CommandResetArg& command_reset_arg;
    const CommandLayersArg& command_layers_arg;
    const CommandLoaderArg& command_loader_arg;
    const CommandDocArg& command_doc_arg;
    const GenerateSettingsMode& generate_settings_mode;
    const std::string& selected_layer_name;
    const std::string& selected_configuration_name;
    const bool& dry_run;
    const HelpType& help;

    const CommandError& error;
    const std::vector<std::string>& error_args;

   private:
    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;

    CommandType _command = COMMAND_SHOW_USAGE;
    CommandResetArg _command_reset_arg = COMMAND_RESET_SOFT;
    CommandLayersArg _command_layers_arg = COMMAND_LAYERS_NONE;
    CommandLoaderArg _command_loader_arg = COMMAND_LOADER_NONE;
    CommandDocArg _command_doc_arg = COMMAND_DOC_NONE;
    GenerateSettingsMode _generate_settings_mode = GENERATE_SETTINGS_TXT;
    std::string _selected_layer_name = "default";
    std::string _selected_configuration_name = "default";
    Path _input_path;
    Path _output_path;
    Path _output_dir;
    bool _dry_run = false;

    CommandError _error = ERROR_NONE;
    std::vector<std::string> _error_args;

    HelpType _help = HELP_DEFAULT;
};
