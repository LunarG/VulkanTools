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
    {HELP_HELP, "help"},     {HELP_VERSION, "version"}, {HELP_GUI, "gui"},
    {HELP_LAYERS, "layers"}, {HELP_DOC, "doc"},         {HELP_RESET, "reset"},
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
    {COMMAND_NONE, "", HELP_DEFAULT},               // COMMAND_NONE
    {COMMAND_SHOW_USAGE, "-h", HELP_HELP},          // COMMAND_SHOW_USAGE
    {COMMAND_SHOW_USAGE, "--help", HELP_HELP},      // COMMAND_SHOW_USAGE
    {COMMAND_SHOW_USAGE, "help", HELP_HELP},        // COMMAND_SHOW_USAGE
    {COMMAND_VERSION, "-v", HELP_VERSION},          // COMMAND_VERSION
    {COMMAND_VERSION, "--version", HELP_VERSION},   // COMMAND_VERSION
    {COMMAND_VERSION, "version", HELP_VERSION},     // COMMAND_VERSION
    {COMMAND_GUI, "gui", HELP_GUI},                 // COMMAND_GUI
    {COMMAND_LAYERS, "layers", HELP_LAYERS},        // COMMAND_LAYERS
    {COMMAND_DOC, "doc", HELP_DOC},                 // COMMAND_DOC
    {COMMAND_RESET, "reset", HELP_RESET},           // COMMAND_RESET
    {COMMAND_VULKAN_SDK, "VULKAN_SDK", HELP_RESET}  // COMMAND_VULKAN_SDK
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

struct CommandResetDesc {
    CommandResetArg arguments;
    const char* token;
    int required_arguments;
};

static const CommandResetDesc command_reset_desc[] = {{COMMAND_RESET_SOFT, "--soft", 2},
                                                      {COMMAND_RESET_SOFT, "-s", 2},
                                                      {COMMAND_RESET_HARD, "--hard", 2},
                                                      {COMMAND_RESET_HARD, "-h", 2}};

static CommandResetArg GetCommandResetId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = countof(command_reset_desc); i < n; ++i) {
        if (std::strcmp(command_reset_desc[i].token, token) == 0) return command_reset_desc[i].arguments;
    }

    return COMMAND_RESET_NONE;
}

static const CommandResetDesc& GetCommandReset(CommandResetArg reset_arg) {
    assert(reset_arg != COMMAND_RESET_NONE);

    for (std::size_t i = 0, n = countof(command_reset_desc); i < n; ++i) {
        if (command_reset_desc[i].arguments == reset_arg) return command_reset_desc[i];
    }

    assert(0);
    return command_reset_desc[0];
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

struct CommandDocDesc {
    CommandDocArg arguments;
    const char* token;
    int required_arguments;
};

static const CommandDocDesc command_doc_desc[] = {
    {COMMAND_DOC_HTML, "--html", 3},
    {COMMAND_DOC_MARKDOWN, "--markdown", 3},
    {COMMAND_DOC_SETTINGS, "--settings", 3},
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

static CommandDocArg GetCommandDocId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = countof(command_doc_desc); i < n; ++i) {
        if (std::strcmp(command_doc_desc[i].token, token) == 0) return command_doc_desc[i].arguments;
    }

    return COMMAND_DOC_NONE;
}

static const CommandDocDesc& GetCommandDoc(CommandDocArg doc_arg) {
    assert(doc_arg != COMMAND_DOC_NONE);

    for (std::size_t i = 0, n = countof(command_doc_desc); i < n; ++i) {
        if (command_doc_desc[i].arguments == doc_arg) return command_doc_desc[i];
    }

    assert(0);
    return command_doc_desc[0];
}

CommandLine::CommandLine(int argc, char* argv[])
    : command(_command),
      command_reset_arg(_command_reset_arg),
      command_layers_arg(_command_layers_arg),
      command_doc_arg(_command_doc_arg),
      layers_configuration_path(_layers_configuration_path),
      command_vulkan_sdk(_command_vulkan_sdk),
      doc_layer_name(_doc_layer_name),
      doc_out_dir(_doc_out_dir),
      error(_error),
      error_args(_error_args),
      _command(COMMAND_GUI),
      _command_reset_arg(COMMAND_RESET_NONE),
      _command_layers_arg(COMMAND_LAYERS_NONE),
      _command_doc_arg(COMMAND_DOC_NONE),
      _error(ERROR_NONE),
      _help(HELP_DEFAULT) {
    assert(argc >= 1);

    if (argc <= 1) return;
    int arg_offset = 1;

    switch (_command = GetModeId(argv[arg_offset + 0])) {
        case COMMAND_VULKAN_SDK: {
            _command_vulkan_sdk = argv[arg_offset + 1];
        } break;
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
        case COMMAND_DOC: {
            if (argc <= arg_offset + 2) {
                _error = ERROR_MISSING_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                break;
            }

            if (argc > 5) {
                _error = ERROR_TOO_MANY_COMMAND_ARGUMENTS;
                _error_args.push_back(argv[arg_offset + 0]);
                break;
            }

            _command_doc_arg = GetCommandDocId(argv[arg_offset + 1]);
            if (_command_doc_arg == COMMAND_DOC_NONE) {
                _error = ERROR_INVALID_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                _error_args.push_back(argv[arg_offset + 1]);
                break;
            }
            _doc_layer_name = argv[arg_offset + 2];
            if (argc == 5) {
                // Output dir arg was specified
                _doc_out_dir = argv[arg_offset + 3];
            } else
                // Output dir arg was not specified
                _doc_out_dir = ".";

        } break;
        case COMMAND_RESET: {
            if (argc <= arg_offset + 1) {
                _command_reset_arg = COMMAND_RESET_SOFT;
                break;
            }

            _command_reset_arg = GetCommandResetId(argv[arg_offset + 1]);
            if (_command_reset_arg == COMMAND_RESET_NONE) {
                _error = ERROR_INVALID_COMMAND_ARGUMENT;
                _error_args.push_back(argv[arg_offset + 0]);
                _error_args.push_back(argv[arg_offset + 1]);
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
            printf("\tdoc                       = Create doc files for layer.\n");
            printf("\treset                     = Reset layers configurations.\n");
            printf("\n");
            printf("  (Use 'vkconfig help <command>' for detailed usage of %s commands.)\n", VKCONFIG_NAME);
            break;
        }
        case HELP_HELP: {
            printf("Name\n");
            printf("\t'help' - Displays the %s command help pages:\n", VKCONFIG_NAME);
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
        case HELP_DOC: {
            printf("Name\n");
            printf("\t'doc' - Command to create Vulkan layer doc files\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig doc --html <layer_name> [<output_dir>]\n");
            printf("\tvkconfig doc --markdown <layer_name> [<output_dir>]\n");
            printf("\tvkconfig doc --settings <layer_name> [<output_dir>]\n");
            printf("\n");
            printf("Description\n");
            printf("\tvkconfig doc --html <layer_name> [<output_dir>]\n");
            printf("\t\tCreate the html documentation file for the given layer.\n");
            printf("\t\tThe file is written to <output_dir>, or current directory if not specified.\n");
            printf("\n");
            printf("\tvkconfig doc --markdown <layer_name> [<output_dir>]\n");
            printf("\t\tCreate the markdown documentation file for the given layer.\n");
            printf("\t\tThe file is written to <output_dir>, or current directory if not specified.\n");
            printf("\n");
            printf("\tvkconfig doc --settings <layer_name> [<output_dir>]\n");
            printf("\t\tCreate the vk_layers_settings.txt file for the given layer.\n");
            printf("\t\tThe file is written to <output_dir>, or current directory if not specified.\n");
            break;
        }
        case HELP_RESET: {
            printf("Name\n");
            printf("\t'reset' - Reset layers configurations\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig reset (--hard | -h)\n");
            printf("\tvkconfig reset <--soft | -s>\n");
            printf("\n");
            printf("Description\n");
            printf("\tvkconfig reset (--hard | -h)\n");
            printf("\t\tReset all layers configurations, all user-defined configurations will be lost.\n");
            printf("\n");
            printf("\tvkconfig reset <--soft | -s>\n");
            printf("\t\tReset all default configurations, all user-defined configuration will be preserved.\n");
            printf("\n");
            break;
        }
    }
}

void CommandLine::version() const { printf("%s version %s\n", VKCONFIG_NAME, Version::VKCONFIG.str().c_str()); }
