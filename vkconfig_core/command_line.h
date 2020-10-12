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

#pragma once

#include <string>
#include <vector>

enum CommandType { COMMAND_NONE = 0, COMMAND_SHOW_USAGE, COMMAND_VERSION, COMMAND_GUI, COMMAND_LAYERS };
enum CommandLayersArg {
    COMMAND_LAYERS_NONE = 0,
    COMMAND_LAYERS_OVERRIDE,
    COMMAND_LAYERS_SURRENDER,
    COMMAND_LAYERS_LIST,
    COMMAND_LAYERS_VERBOSE
};
enum CommandError {
    ERROR_NONE = 0,
    ERROR_INVALID_COMMAND_USAGE,
    ERROR_INVALID_COMMAND_ARGUMENT,
    ERROR_TOO_MANY_COMMAND_ARGUMENTS,
    ERROR_MISSING_COMMAND_ARGUMENT,
    ERROR_UNKNOWN_ARGUMENT,
    ERROR_FILE_NOTFOUND
};

enum HelpType { HELP_NONE, HELP_DEFAULT, HELP_HELP, HELP_VERSION, HELP_GUI, HELP_LAYERS };

class CommandLine {
   public:
    CommandLine(int argc, char* argv[]);
    void log() const;
    void usage() const;
    void version() const;

    const CommandType& command;
    const CommandLayersArg& command_layers_arg;
    const std::string& layers_configuration_path;

    const CommandError& error;
    const std::vector<std::string>& error_args;

   private:
    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;

    CommandType _command;
    CommandLayersArg _command_layers_arg;
    std::string _layers_configuration_path;

    CommandError _error;
    std::vector<std::string> _error_args;

    HelpType _help;
};
