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

#include "command_line.h"
#include "util.h"
#include "version.h"
#include "help.h"

#include <QFile>

#include <string>
#include <cstdio>
#include <cstring>
#include <cassert>

struct CommandHelpDesc {
    HelpType arguments;
    const char* token;
};

static const CommandHelpDesc command_help_desc[] = {
    {HELP_HELP, "help"},
    {HELP_VERSION, "version"},
    {HELP_GUI, "gui"},
    {HELP_LAYERS, "layers"},
};

static HelpType GetCommandHelpId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = countof(command_help_desc); i < n; ++i) {
        if (std::strcmp(command_help_desc[i].token, token) == 0) return command_help_desc[i].arguments;
    }

    return HELP_NONE;
}

struct ModeDesc {
    CommandType mode;
    const char* token;
    HelpType help;
};

static const ModeDesc mode_desc[] = {
    {COMMAND_NONE, "", HELP_DEFAULT},              // COMMAND_NONE
    {COMMAND_SHOW_USAGE, "-h", HELP_HELP},         // COMMAND_SHOW_USAGE
    {COMMAND_SHOW_USAGE, "--help", HELP_HELP},     // COMMAND_SHOW_USAGE
    {COMMAND_SHOW_USAGE, "help", HELP_HELP},       // COMMAND_SHOW_USAGE
    {COMMAND_VERSION, "-v", HELP_VERSION},         // COMMAND_VERSION
    {COMMAND_VERSION, "--version", HELP_VERSION},  // COMMAND_VERSION
    {COMMAND_VERSION, "version", HELP_VERSION},    // COMMAND_VERSION
    {COMMAND_GUI, "gui", HELP_GUI},                // COMMAND_GUI
    {COMMAND_LAYERS, "layers", HELP_LAYERS},       // COMMAND_LAYERS
};

static CommandType GetModeId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = countof(mode_desc); i < n; ++i) {
        if (std::strcmp(mode_desc[i].token, token) == 0) return mode_desc[i].mode;
    }

    return COMMAND_NONE;
}

static const ModeDesc& GetModeDesc(CommandType command_type) {
    for (std::size_t i = 0, n = countof(mode_desc); i < n; ++i) {
        if (mode_desc[i].mode == command_type) return mode_desc[i];
    }

    assert(0);
    return mode_desc[0];  // COMMAND_NONE
}

struct CommandLayersDesc {
    CommandLayersArg arguments;
    const char* token;
    int required_arguments;
};

static const CommandLayersDesc command_layers_desc[] = {
    {COMMAND_LAYERS_LIST, "--list", 2},
    {COMMAND_LAYERS_LIST, "-l", 2},
    {COMMAND_LAYERS_VERBOSE, "--list-verbose", 2},
    {COMMAND_LAYERS_VERBOSE, "-lv", 2},
    {COMMAND_LAYERS_OVERRIDE, "--override", 3},
    {COMMAND_LAYERS_OVERRIDE, "-o", 3},
    {COMMAND_LAYERS_SURRENDER, "--surrender", 2},
    {COMMAND_LAYERS_SURRENDER, "-s", 2},
};

static CommandLayersArg GetCommandLayersId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = countof(command_layers_desc); i < n; ++i) {
        if (std::strcmp(command_layers_desc[i].token, token) == 0) return command_layers_desc[i].arguments;
    }

    return COMMAND_LAYERS_NONE;
}

static const CommandLayersDesc& GetCommandLayers(CommandLayersArg layers_arg) {
    assert(layers_arg != COMMAND_LAYERS_NONE);

    for (std::size_t i = 0, n = countof(command_layers_desc); i < n; ++i) {
        if (command_layers_desc[i].arguments == layers_arg) return command_layers_desc[i];
    }

    assert(0);
    return command_layers_desc[0];
}

CommandLine::CommandLine(int argc, char* argv[])
    : command(_command),
      command_layers_arg(_command_layers_arg),
      layers_configuration_path(_layers_configuration_path),
      error(_error),
      error_args(_error_args),
      _command(COMMAND_GUI),
      _command_layers_arg(COMMAND_LAYERS_NONE),
      _error(ERROR_NONE),
      _help(HELP_DEFAULT) {
    assert(argc >= 1);

    if (argc <= 1) return;
    int arg_offset = 1;

    switch (_command = GetModeId(argv[arg_offset + 0])) {
        case COMMAND_LAYERS: {
            if (argc <= arg_offset + 1) {
                _error = ERROR_MISSING_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                break;
            }

            _command_layers_arg = GetCommandLayersId(argv[arg_offset + 1]);
            if (_command_layers_arg == COMMAND_LAYERS_NONE) {
                _error = ERROR_INVALID_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                _error_args.push_back(argv[arg_offset + 1]);
                break;
            }

            const CommandLayersDesc& desc = GetCommandLayers(_command_layers_arg);
            if (argc < arg_offset + desc.required_arguments) {
                _error = ERROR_MISSING_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                break;
            } else if (argc > arg_offset + desc.required_arguments) {
                _error = ERROR_TOO_MANY_COMMAND_ARGUMENTS;
                _error_args.push_back(argv[arg_offset + 0]);
                break;
            }

            if (_command_layers_arg == COMMAND_LAYERS_OVERRIDE) {
                _layers_configuration_path = argv[arg_offset + 2];
                QFile file(_layers_configuration_path.c_str());
                const bool result = file.open(QFile::ReadOnly);
                if (!result) {
                    _error = ERROR_FILE_NOTFOUND;
                    _error_args.push_back(argv[arg_offset + 2]);
                }
                break;
            }
        } break;
        case COMMAND_SHOW_USAGE: {
            if (argc <= arg_offset + 1) {
                _help = HELP_DEFAULT;
                break;
            }

            _help = GetCommandHelpId(argv[arg_offset + 1]);
            if (_help == HELP_NONE) {
                _error = ERROR_INVALID_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                _error_args.push_back(argv[arg_offset + 1]);
            }
            break;
        }
        case COMMAND_NONE: {
            _error = ERROR_UNKNOWN_ARGUMENT;
            _error_args.push_back(argv[arg_offset + 0]);
            break;
        }
        case COMMAND_GUI:
        case COMMAND_VERSION: {
            break;
        }
    }

    if (_error != ERROR_NONE) {
        _help = GetModeDesc(_command).help;
    }
}

void CommandLine::log() const {
    switch (error) {
        case ERROR_NONE: {
            assert(_error_args.empty());
        } break;
        case ERROR_INVALID_COMMAND_USAGE: {
            assert(_error_args.size() == 1);
            printf("Invalid '%s' command usage...\n\n", _error_args[0].c_str());
        } break;
        case ERROR_INVALID_COMMAND_ARGUMENT: {
            assert(_error_args.size() == 2);
            printf("Invalid '%s' command argument: '%s'...\n\n", _error_args[0].c_str(), _error_args[1].c_str());
        } break;
        case ERROR_MISSING_COMMAND_ARGUMENT: {
            assert(_error_args.size() == 1);
            printf("Missing '%s' command argument...\n\n", _error_args[0].c_str());
        } break;
        case ERROR_TOO_MANY_COMMAND_ARGUMENTS: {
            assert(_error_args.size() == 1);
            printf("Too many '%s' command arguments...\n\n", _error_args[0].c_str());
        }
        case ERROR_FILE_NOTFOUND: {
            assert(_error_args.size() == 1);
            printf("'%s' couldn't be found...\n\n", _error_args[0].c_str());
        } break;
        case ERROR_UNKNOWN_ARGUMENT: {
            assert(_error_args.size() == 1);
            printf("Unknown argument: '%s'...\n\n", _error_args[0].c_str());
        } break;
        default: {
            assert(0);
            printf("Unknown error...\n\n");
        } break;
    }
}

void CommandLine::usage() const {
    switch (_help) {
        default: {
            assert(0);
            break;
        }
        case HELP_DEFAULT: {
            printf("Usage\n");
            printf("\tvkconfig [[help] | [version] | [gui] | [layers <args>]]\n");
            printf("\n");
            printf("Command:\n");
            printf("\thelp                      = Display usage and documentation.\n");
            printf("\tversion                   = Display %s version.\n", VKCONFIG_NAME);
            printf("\tgui                       = Launch the %s GUI.\n", VKCONFIG_NAME);
            printf("\tlayers                    = Manage system Vulkan Layers.\n");
            printf("\n");
            printf("  (Use 'vkconfig help <commamd>' for detailed usage of %s commands.)\n", VKCONFIG_NAME);
            break;
        }
        case HELP_HELP: {
            printf("Name\n");
            printf("\t'help' - Displays of the %s commands help pages:\n", VKCONFIG_NAME);
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig help [command]\n");
            break;
        }
        case HELP_VERSION: {
            printf("Name\n");
            printf("\t'version' - Command to display %s version.\n", VKCONFIG_NAME);
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig version\n");
            break;
        }
        case HELP_GUI: {
            printf("Name\n");
            printf("\t'gui' - Command to launch %s GUI\n", VKCONFIG_NAME);
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig gui\n");
            break;
        }
        case HELP_LAYERS: {
            printf("Name\n");
            printf("\t'layers' - Command to manage system Vulkan Layers\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig layers (--override | -o) <layers_configuration_file>\n");
            printf("\tvkconfig layers (--surrender | -s)\n");
            printf("\tvkconfig layers (--list | -l)\n");
            printf("\tvkconfig layers (--list-verbose | -lv)\n");
            printf("\n");
            printf("Description\n");
            printf("\tvkconfig layers (--override | -o) <layers_configuration_file>\n");
            printf("\t\tOverride the Vulkan layers using <layers_configuration_file> generated by %s.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig layers (--surrender | -s)\n");
            printf("\t\tSurrender the Vulkan layers control to Vulkan applications.\n");
            printf("\n");
            printf("\tvkconfig layers (--list | -l)\n");
            printf("\t\tList the Vulkan layers found by %s on the system.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig layers (--list-version | -lv)\n");
            printf("\t\tList the Vulkan layers found by %s on the system with locations and versions.\n", VKCONFIG_NAME);
            break;
        }
    }
}

void CommandLine::version() const { printf("%s version %s\n", VKCONFIG_NAME, Version::VKCONFIG.str().c_str()); }
