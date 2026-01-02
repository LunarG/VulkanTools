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

#include "command_line.h"
#include "util.h"
#include "version.h"

#include <QFile>

#include <array>
#include <string>
#include <cstdio>
#include <cstring>
#include <cassert>

struct CommandHelpDesc {
    HelpType arguments;
    const char* token;
};

static const CommandHelpDesc command_help_desc[] = {{HELP_HELP, "help"},     {HELP_VERSION, "version"}, {HELP_LAYERS, "layers"},
                                                    {HELP_LOADER, "loader"}, {HELP_DOC, "doc"},         {HELP_SETTINGS, "settings"},
                                                    {HELP_RESET, "reset"}};

static HelpType GetCommandHelpId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_help_desc); i < n; ++i) {
        if (std::strcmp(command_help_desc[i].token, token) == 0) {
            return command_help_desc[i].arguments;
        }
    }

    return HELP_NONE;
}

struct ModeDesc {
    CommandType mode;
    const char* token;
    HelpType help;
};

static const ModeDesc mode_desc[] = {
    {COMMAND_NONE, "", HELP_DEFAULT},                 // COMMAND_NONE
    {COMMAND_SHOW_USAGE, "--help", HELP_HELP},        // COMMAND_SHOW_USAGE
    {COMMAND_SHOW_USAGE, "help", HELP_HELP},          // COMMAND_SHOW_USAGE
    {COMMAND_VERSION, "-v", HELP_VERSION},            // COMMAND_VERSION
    {COMMAND_VERSION, "--version", HELP_VERSION},     // COMMAND_VERSION
    {COMMAND_VERSION, "version", HELP_VERSION},       // COMMAND_VERSION
    {COMMAND_LAYERS, "--layers", HELP_LAYERS},        // COMMAND_LAYERS
    {COMMAND_LAYERS, "layers", HELP_LAYERS},          // COMMAND_LAYERS
    {COMMAND_LOADER, "--loader", HELP_LOADER},        // COMMAND_LOADER
    {COMMAND_LOADER, "loader", HELP_LOADER},          // COMMAND_LOADER
    {COMMAND_DOC, "--doc", HELP_DOC},                 // COMMAND_DOC
    {COMMAND_DOC, "doc", HELP_DOC},                   // COMMAND_DOC
    {COMMAND_SETTINGS, "--settings", HELP_SETTINGS},  // COMMAND_EXPORT
    {COMMAND_SETTINGS, "settings", HELP_SETTINGS},    // COMMAND_EXPORT
    {COMMAND_GUI, "--gui", HELP_GUI},                 // COMMAND_GUI
    {COMMAND_GUI, "gui", HELP_GUI},                   // COMMAND_GUI
    {COMMAND_RESET, "--reset", HELP_RESET},           // COMMAND_RESET
    {COMMAND_RESET, "reset", HELP_RESET}              // COMMAND_RESET
};

static CommandType GetModeId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(mode_desc); i < n; ++i) {
        if (std::strcmp(mode_desc[i].token, token) == 0) {
            return mode_desc[i].mode;
        }
    }

    return COMMAND_NONE;
}

static const ModeDesc& GetModeDesc(CommandType command_type) {
    for (std::size_t i = 0, n = std::size(mode_desc); i < n; ++i) {
        if (mode_desc[i].mode == command_type) {
            return mode_desc[i];
        }
    }

    assert(0);
    return mode_desc[0];  // COMMAND_NONE
}

struct CommandResetDesc {
    CommandResetArg arguments;
    const char* token;
};

static const CommandResetDesc command_reset_desc[] = {
    {COMMAND_RESET_SOFT, "--soft"}, {COMMAND_RESET_SOFT, "-s"}, {COMMAND_RESET_HARD, "--hard"}, {COMMAND_RESET_HARD, "-h"}};

static CommandResetArg GetCommandResetId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_reset_desc); i < n; ++i) {
        if (std::strcmp(command_reset_desc[i].token, token) == 0) {
            return command_reset_desc[i].arguments;
        }
    }

    return COMMAND_RESET_NONE;
}

struct CommandLayersDesc {
    CommandLayersArg arguments;
    const char* token;
};

static const CommandLayersDesc command_layers_desc[] = {
    {COMMAND_LAYERS_LIST, "--list"},
    {COMMAND_LAYERS_LIST, "-l"},
    {COMMAND_LAYERS_PATH, "--path"},
    {COMMAND_LAYERS_PATH, "-p"},
    {COMMAND_LAYERS_VERBOSE, "--list-verbose"},
    {COMMAND_LAYERS_VERBOSE, "-lv"},
    {COMMAND_LAYERS_OVERRIDE, "--override"},
    {COMMAND_LAYERS_OVERRIDE, "-o"},
    {COMMAND_LAYERS_SURRENDER, "--surrender"},
    {COMMAND_LAYERS_SURRENDER, "-s"},
};

struct CommandLoaderDesc {
    CommandLoaderArg arguments;
    const char* token;
};

static const CommandLoaderDesc command_loader_desc[] = {
    {COMMAND_LOADER_LIST, "--list"},           {COMMAND_LOADER_LIST, "-l"},
    {COMMAND_LOADER_OVERRIDE, "--override"},   {COMMAND_LOADER_OVERRIDE, "-o"},
    {COMMAND_LOADER_SURRENDER, "--surrender"}, {COMMAND_LOADER_SURRENDER, "-s"},
    {COMMAND_LOADER_IMPORT, "--import"},       {COMMAND_LOADER_IMPORT, "-i"},
    {COMMAND_LOADER_EXPORT, "--export"},       {COMMAND_LOADER_EXPORT, "-e"},
    {COMMAND_LOADER_DELETE, "--delete"},       {COMMAND_LOADER_DELETE, "-d"},
};

struct CommandDocDesc {
    CommandDocArg arguments;
    const char* token;
};

static const CommandDocDesc command_doc_desc[] = {
    {COMMAND_DOC_HTML, "--html"},
    {COMMAND_DOC_MARKDOWN, "--markdown"},
    {COMMAND_DOC_SETTINGS, "--settings"},
};

struct CommandSettingsDesc {
    CommandSettingsArg arguments;
    const char* token;
};

static const CommandSettingsDesc command_settings_desc[] = {
    {COMMAND_SETTINGS_GENERATE, "--generate"},     {COMMAND_SETTINGS_GENERATE, "-g"},    // COMMAND_SETTINGS_GENERATE
    {COMMAND_SETTINGS_CONFIG, "--configuration"},  {COMMAND_SETTINGS_CONFIG, "-c"},      // COMMAND_SETTINGS_CONFIG
    {COMMAND_SETTINGS_LAYERS, "--layers"},         {COMMAND_SETTINGS_LAYERS, "-l"},      // COMMAND_SETTINGS_LAYERS
    {COMMAND_SETTINGS_LAYER, "--layer"},                                                 // COMMAND_SETTINGS_LAYER
    {COMMAND_SETTINGS_OUTPUT, "--output"},         {COMMAND_SETTINGS_OUTPUT, "-o"},      // COMMAND_SETTINGS_OUTPUT
    {COMMAND_SETTINGS_OUTPUT_DIR, "--output-dir"}, {COMMAND_SETTINGS_OUTPUT_DIR, "-d"},  // COMMAND_SETTINGS_OUTPUT_DIR
    {COMMAND_SETTINGS_DRY_RUN, "--dry-run"}};

static CommandLayersArg GetCommandLayersId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_layers_desc); i < n; ++i) {
        if (std::strcmp(command_layers_desc[i].token, token) == 0) {
            return command_layers_desc[i].arguments;
        }
    }

    return COMMAND_LAYERS_NONE;
}

static CommandLoaderArg GetCommandLoaderId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_loader_desc); i < n; ++i) {
        if (std::strcmp(command_loader_desc[i].token, token) == 0) {
            return command_loader_desc[i].arguments;
        }
    }

    return COMMAND_LOADER_NONE;
}

static CommandDocArg GetCommandDocId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_doc_desc); i < n; ++i) {
        if (std::strcmp(command_doc_desc[i].token, token) == 0) {
            return command_doc_desc[i].arguments;
        }
    }

    return COMMAND_DOC_NONE;
}

static CommandSettingsArg GetCommandSettingsArgsId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_settings_desc); i < n; ++i) {
        if (std::strcmp(command_settings_desc[i].token, token) == 0) {
            return command_settings_desc[i].arguments;
        }
    }

    return COMMAND_SETTINGS_NONE;
}

CommandLine::CommandLine(int argc, char* argv[])
    : command(_command),
      command_reset_arg(_command_reset_arg),
      command_layers_arg(_command_layers_arg),
      command_loader_arg(_command_loader_arg),
      command_doc_arg(_command_doc_arg),
      generate_settings_mode(_generate_settings_mode),
      selected_layers_name(_selected_layers_name),
      selected_configuration_name(_selected_configuration_name),
      dry_run(_dry_run),
      help(_help),
      error(_error),
      error_args(_error_args) {
    assert(argc >= 1);

    if (argc <= 1) {
        return;
    }

    int arg_offset = 1;

    std::string command_mode_name = argv[arg_offset];

    switch (_command = GetModeId(command_mode_name.c_str())) {
        default:
        case COMMAND_NONE: {
            this->_error = ERROR_INVALID_COMMAND;
            this->_error_args.push_back(argv[arg_offset + 0]);
            break;
        }
        case COMMAND_LOADER: {
            if (argc <= arg_offset + 1) {
                this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                this->_error_args.push_back(command_mode_name);
                break;
            }

            arg_offset += 1;

            while (arg_offset < argc) {
                std::string command_argument = argv[arg_offset];
                CommandLoaderArg command_loader = ::GetCommandLoaderId(command_argument.c_str());

                switch (command_loader) {
                    default:
                    case COMMAND_LOADER_NONE: {
                        this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                    } break;
                    case COMMAND_LOADER_OVERRIDE: {
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                        } else {
                            const std::string configuration_name = argv[arg_offset + 1];

                            QFile file(Path(configuration_name).AbsolutePath().c_str());
                            const bool result = file.open(QFile::ReadOnly);
                            if (!result) {
                                this->_selected_configuration_name = configuration_name;
                            } else {
                                this->_input_path = configuration_name;
                            }
                        }
                        arg_offset += 2;
                    } break;
                    case COMMAND_LOADER_SURRENDER:
                    case COMMAND_LOADER_LIST: {
                        ++arg_offset;
                    } break;
                    case COMMAND_LOADER_IMPORT: {
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                        } else {
                            this->_output_path = argv[arg_offset + 1];

                            QFile file(this->_output_path.AbsolutePath().c_str());
                            const bool result = file.open(QFile::ReadOnly);
                            if (!result) {
                                this->_error = ERROR_FILE_NOTFOUND;
                            }
                        }
                        ++arg_offset;
                    } break;
                    case COMMAND_LOADER_EXPORT: {
                        if (argc <= arg_offset + 2) {
                            this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                        } else {
                            this->_selected_configuration_name = argv[arg_offset + 1];
                            this->_output_path = argv[arg_offset + 2];
                            Path ExportDir(_output_path.AbsoluteDir());
                            ExportDir.Create();
                            if (!ExportDir.Exists()) {
                                this->_error = ERROR_FILE_NOTFOUND;
                            }
                        }
                        arg_offset += 2;
                    } break;
                    case COMMAND_LOADER_DELETE: {
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                        } else {
                            this->_selected_configuration_name = argv[arg_offset + 1];
                        }
                        ++arg_offset;
                    } break;
                }

                if (this->_command_loader_arg == COMMAND_LOADER_NONE) {
                    this->_command_loader_arg = command_loader;
                }

                if (this->_error != ERROR_NONE) {
                    this->_error_args.push_back(command_mode_name);
                    this->_error_args.push_back(command_argument);
                    arg_offset = argc;  // End the loop
                }
            }
        } break;
        case COMMAND_LAYERS: {
            if (argc <= arg_offset + 1) {
                this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                this->_error_args.push_back(command_mode_name);
                arg_offset = argc;  // End the loop
                break;
            }

            arg_offset += 1;

            while (arg_offset < argc) {
                std::string command_argument = argv[arg_offset];
                CommandLayersArg command_layers = ::GetCommandLayersId(command_argument.c_str());

                switch (command_layers) {
                    default:
                    case COMMAND_LAYERS_NONE: {
                        this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                    } break;
                    case COMMAND_LAYERS_OVERRIDE: {
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                        } else {
                            const std::string configuration_name = argv[arg_offset + 1];

                            QFile file(Path(configuration_name).AbsolutePath().c_str());
                            const bool result = file.open(QFile::ReadOnly);
                            if (!result) {
                                this->_selected_configuration_name = configuration_name;
                            } else {
                                this->_input_path = configuration_name;
                            }
                        }
                        arg_offset += 2;
                    } break;
                    case COMMAND_LAYERS_SURRENDER:
                    case COMMAND_LAYERS_LIST: {
                        ++arg_offset;
                    } break;
                }

                if (this->_command_layers_arg == COMMAND_LAYERS_NONE) {
                    this->_command_layers_arg = command_layers;
                }

                if (this->_error != ERROR_NONE) {
                    this->_error_args.push_back(command_mode_name);
                    this->_error_args.push_back(command_argument);
                    arg_offset = argc;  // End the loop
                }
            }
        } break;
        case COMMAND_DOC: {
            if (argc <= arg_offset + 2) {
                this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                this->_error_args.push_back(argv[arg_offset + 0]);
                arg_offset = argc;  // End the loop
                break;
            }

            this->_command_doc_arg = GetCommandDocId(argv[arg_offset + 1]);
            if (this->_command_doc_arg == COMMAND_DOC_NONE) {
                this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                this->_error_args.push_back(argv[arg_offset + 0]);
                this->_error_args.push_back(argv[arg_offset + 1]);
                arg_offset = argc;  // End the loop
                break;
            }

            this->_selected_layers_name = ::Split(argv[arg_offset + 2], ",");
            if (argc == 5) {
                // Output dir arg was specified
                this->_output_path = argv[arg_offset + 3];
            } else {
                // Output dir arg was not specified
                this->_output_path = ".";
            }

            switch (this->_command_doc_arg) {
                default:
                    break;
                case COMMAND_DOC_HTML:
                    printf("vkconfig: \"vkconfig doc --html\" is deprecated\n");
                    printf("\n  (Run \"vkconfig settings --mode html\" instead)\n");
                    break;
                case COMMAND_DOC_MARKDOWN:
                    printf("vkconfig: \"vkconfig doc --markdown\" is deprecated\n");
                    printf("\n  (Run \"vkconfig settings --mode markdown\" instead)\n");
                    break;
                case COMMAND_DOC_SETTINGS:
                    printf("vkconfig: \"vkconfig doc --settings\" is deprecated\n");
                    printf("\n  (Run \"vkconfig settings --mode txt\" instead)\n");
                    break;
            }
        } break;
        case COMMAND_SETTINGS: {
            if (argc <= arg_offset + 1) {
                this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                this->_error_args.push_back(command_mode_name);
                arg_offset = argc;  // End the loop
                break;
            }

            arg_offset += 1;

            while (arg_offset < argc) {
                const std::string command_argument = argv[arg_offset];
                CommandSettingsArg command_settings_arg = GetCommandSettingsArgsId(command_argument.c_str());

                switch (command_settings_arg) {
                    default:
                    case COMMAND_SETTINGS_NONE: {
                        this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                    } break;
                    case COMMAND_SETTINGS_CONFIG: {
                        if (argc <= arg_offset + 1) {
                            arg_offset += 1;
                        } else {
                            const std::string configuration_name = argv[arg_offset + 1];
                            if (configuration_name[0] == '-') {  // Not a value but another argumment
                                arg_offset += 1;
                            } else {
                                this->_selected_configuration_name = configuration_name;
                                arg_offset += 2;
                            }
                        }
                    } break;
                    case COMMAND_SETTINGS_LAYER:
                    case COMMAND_SETTINGS_LAYERS: {
                        if (argc <= arg_offset + 1) {
                            arg_offset += 1;
                        } else {
                            const std::string layer_names = argv[arg_offset + 1];
                            if (layer_names[0] == '-') {  // Not a value but another argumment
                                this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                            } else {
                                this->_selected_layers_name = Split(layer_names, ",");
                                arg_offset += 2;
                            }
                        }
                    } break;
                    case COMMAND_SETTINGS_GENERATE: {
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                        } else {
                            const std::string mode_name = argv[arg_offset + 1];
                            if (mode_name[0] == '-') {  // Not a value but another argumment
                                this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                            } else {
                                this->_generate_settings_mode = ::GetGenerateSettingsMode(mode_name.c_str());
                                arg_offset += 2;
                            }
                        }
                    } break;
                    case COMMAND_SETTINGS_OUTPUT_DIR: {
                        // Missing --output_dir value
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                            break;
                        }

                        // <output_dir> arg was specified
                        this->_output_dir = argv[arg_offset + 1];
                        arg_offset += 2;
                    } break;
                    case COMMAND_SETTINGS_OUTPUT: {
                        // Missing --output value
                        if (argc <= arg_offset + 1) {
                            this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                            break;
                        }

                        // <output_file> arg was specified
                        this->_output_path = argv[arg_offset + 1];
                        arg_offset += 2;
                    } break;
                    case COMMAND_SETTINGS_DRY_RUN: {
                        this->_dry_run = true;
                        arg_offset += 1;
                    } break;
                }

                if (this->_error != ERROR_NONE) {
                    this->_error_args.push_back(command_mode_name);
                    this->_error_args.push_back(command_argument);
                    arg_offset = argc;  // End the loop
                }
            }
        } break;
        case COMMAND_RESET: {
            if (argc <= arg_offset + 1) {
                this->_command_reset_arg = COMMAND_RESET_SOFT;  // default for --reset
                arg_offset += 1;
                break;
            }

            this->_command_reset_arg = GetCommandResetId(argv[arg_offset + 1]);
            if (this->_command_reset_arg == COMMAND_RESET_NONE) {
                this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                this->_error_args.push_back(argv[arg_offset + 0]);
                this->_error_args.push_back(argv[arg_offset + 1]);
                arg_offset = argc;  // End the loop
                break;
            }
        } break;
        case COMMAND_SHOW_USAGE: {
            if (argc <= arg_offset + 1) {
                this->_help = HELP_DEFAULT;
                break;
            }

            this->_help = GetCommandHelpId(argv[arg_offset + 1]);
            if (this->_help == HELP_NONE) {
                this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                this->_error_args.push_back(argv[arg_offset + 0]);
                this->_error_args.push_back(argv[arg_offset + 1]);
            }
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
        case ERROR_INVALID_COMMAND: {
            assert(_error_args.size() >= 1);
            printf("vkconfig: Invalid '%s' command...\n\n", _error_args[0].c_str());
        } break;
        case ERROR_INVALID_COMMAND_ARGUMENT: {
            assert(_error_args.size() == 2);
            printf("vkconfig: Invalid '%s' command argument: %s\n\n", _error_args[0].c_str(), _error_args[1].c_str());
        } break;
        case ERROR_TOO_MANY_COMMAND_ARGUMENTS: {
            assert(this->_error_args.size() >= 1);
            if (this->error_args.size() > 1) {
                printf("vkconfig: Invalid '%s' command usage:", this->_error_args[0].c_str());
                std::string arguments;
                for (std::size_t i = 1, n = this->_error_args.size(); i < n; ++i) {
                    arguments += format(" %s", this->_error_args[i].c_str());
                }
                printf("%s\n\n", arguments.c_str());
            } else {
                printf("vkconfig: Invalid '%s' command usage...\n\n", _error_args[0].c_str());
            }
        } break;
        case ERROR_MISSING_COMMAND_ARGUMENT: {
            assert(this->_error_args.size() >= 1);
            if (this->error_args.size() > 1) {
                printf("vkconfig: Invalid '%s' command usage:", this->_error_args[0].c_str());
                std::string arguments;
                for (std::size_t i = 1, n = this->_error_args.size(); i < n; ++i) {
                    arguments += format(" %s", this->_error_args[i].c_str());
                }
                printf("%s\n\n", arguments.c_str());
            } else {
                printf("vkconfig: Invalid '%s' command usage...\n\n", _error_args[0].c_str());
            }
        } break;
        case ERROR_FILE_NOTFOUND: {
            assert(_error_args.size() == 1);
            printf("vkconfig: '%s' couldn't be found...\n\n", _error_args[0].c_str());
        } break;
        default: {
            assert(0);
            printf("vkconfig: Unknown error...\n\n");
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
            printf("\tvkconfig [help] | [version] | [gui] | [reset <args>] |\n");
            printf("\t         [loader <args>] | [layers <args>] | [settings <args>]\n");
            printf("\n");
            printf("Command:\n");
            printf("\thelp          = Display usage and documentation.\n");
            printf("\tversion       = Display %s version.\n", VKCONFIG_NAME);
            printf("\tgui           = Launch the graphical interface.\n");
            printf("\treset         = Reset layers configurations.\n");
            printf("\tloader        = Configure system the Vulkan Loader configuration.\n");
            printf("\tlayers        = List the Vulkan layers found on the system.\n");
            printf("\tsettings      = Create layer settings and documentation files for Vulkan developers.\n");
            printf("\n");
            printf("  (Run 'vkconfig help <command>' for detailed usage of %s commands.)\n", VKCONFIG_NAME);
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
            printf("\t'gui' - Launch %s GUI.\n", VKCONFIG_NAME);
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig gui\n");
            break;
        }
        case HELP_LAYERS: {
            printf("Name\n");
            printf("\t'layers' - Command to list the Vulkan layers found on the system.\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig layers (--list | -l)\n");
            printf("\tvkconfig layers (--list-verbose | -lv)\n");
            printf("\tvkconfig layers (--path | -p)\n");
            printf("\tvkconfig layers (--override | -o) <loader_configuration_file>\n");
            printf("\tvkconfig layers (--surrender | -s)\n");
            printf("\n");
            printf("Description\n");
            printf("\tvkconfig layers (--list | -l)\n");
            printf("\t\tList the Vulkan layers found by %s on the system.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig layers (--list-version | -lv)\n");
            printf("\t\tList the Vulkan layers found by %s on the system with locations and versions.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig layers (--path | -p)\n");
            printf("\t\tList the Vulkan layers locations found by %s on the system.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig layers (--override | -o) <loader_configuration_file>\n");
            printf("\t\tOverride the Vulkan layers using <loader_configuration_file> generated by %s.\n", VKCONFIG_NAME);
            printf("\t  (DEPRECATED: Use `vkconfig loader --override` instead.)\n");
            printf("\n");
            printf("\tvkconfig layers (--surrender | -s)\n");
            printf("\t\tSurrender the Vulkan layers control to Vulkan applications.\n");
            printf("\t  (DEPRECATED: Use `vkconfig loader --surrender` instead.)\n");
            break;
        }
        case HELP_LOADER: {
            printf("Name\n");
            printf(
                "\t'loader' - Command to configure system Vulkan Loader configuration, including layers, loader logging and "
                "drivers.\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig loader (--override | -o) (<configuration_index> | <configuration_name> | <configuration_file>)\n");
            printf("\tvkconfig loader (--surrender | -s)\n");
            printf("\tvkconfig loader (--list | -l)\n");
            printf("\tvkconfig loader (--import | -i) <configuration_file>\n");
            printf("\tvkconfig loader (--export | -e) (<configuration_index> | <configuration_name>) <configuration_file>\n");
            printf("\tvkconfig loader (--delete | -d) (<configuration_index> | <configuration_name>)\n");
            printf("\n");
            printf("Description\n");
            printf("\tvkconfig loader (--override | -o) (<configuration_index> | <configuration_name> | <configuration_file>)\n");
            printf("\t\tOverride the system Vulkan Layers configuration generated by %s.\n", VKCONFIG_NAME);
            printf("\t\t - <configuration_index> is an index enumerated with `vkconfig loader --list`.\n");
            printf("\t\t - <configuration_name> is the name of the stored configuration listed with `vkconfig loader --list`.\n");
            printf("\t  (Run 'vkconfig loader --list' to enumerate the available configurations.)\n");
            printf("\n");
            printf("\tvkconfig loader (--surrender | -s)\n");
            printf("\t\tSurrender the Vulkan Loader configuration to Vulkan applications.\n");
            printf("\n");
            printf("\tvkconfig loader (--list | -l)\n");
            printf("\t\tList the Vulkan Layers configurations found by %s on the system.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig loader (--import | -i) <configuration_file>\n");
            printf("\t\tImport a Vulkan Layers configuration stored by %s on the system.\n", VKCONFIG_NAME);
            printf("\n");
            printf("\tvkconfig loader (--export | -e) (<configuration_index> | <configuration_name>) <configuration_file>\n");
            printf("\t\tExport a Vulkan Layers configuration stored by %s on the system.\n", VKCONFIG_NAME);
            printf("\t\t - <configuration_index> is an index enumerated with `vkconfig loader --list`.\n");
            printf("\t\t - <configuration_name> is the name of the stored configuration listed with `vkconfig loader --list`.\n");
            printf("\t  (Run 'vkconfig loader --list' to enumerate the available configurations.)\n");
            printf("\n");
            printf("\tvkconfig loader (--delete | -d) (<configuration_index> | <configuration_name>)\n");
            printf("\t\tRemove a Vulkan Layers configuration stored by %s on the system.\n", VKCONFIG_NAME);
            printf("\t\t - <configuration_index> is an index enumerated with `vkconfig loader --list`.\n");
            printf("\t\t - <configuration_name> is the name of the stored configuration listed with `vkconfig loader --list`.\n");
            printf("\t  (Run 'vkconfig loader --list' to enumerate the available configurations.)\n");
            break;
        }
        case HELP_SETTINGS: {
            printf("Name\n");
            printf("\t'settings' - Command to generate layer settings files\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig settings\n");
            printf("\t                  [(--generate | -g) (html | markdown | txt | bash | bat | hpp)]\n");
            printf("\t                  [(--configuration | -c) [<configuration_index> | <configuration_name> | default]]\n");
            printf("\t                  [(--layers | -l) [<layer_names> | default]]\n");
            printf("\t                  [(--output-dir | -d) <output_dir>]\n");
            printf("\t                  [(--output | -o) <output_file>]\n");
            printf("\t                  [--dry-run]\n");
            printf("\n");
            printf("Description\n");
            printf("\n");
            printf("\tGenerate layer settings files either for system configuration or documentation of a layers configuration.\n");
            printf("\n");
            printf("Arguments\n");
            printf("\t`[--generate (html | markdown | txt | bash | bat | hpp)]`\n");
            printf("\t\tSpecify the layer settings generation mode, the default value is 'txt':\n");
            printf(
                "\t\t- 'html' to generate the HTML layer settings documentation, the default filename is "
                "'vk_layer_settings.html'\n");
            printf(
                "\t\t- 'markdown' to generate the Markdown layer settings documentation, the default filename is "
                "'vk_layer_settings.md'\n");
            printf(
                "\t\t- 'txt' to generate the `vk_layer_settings.txt` layer settings file, the default filename is "
                "'vk_layer_settings.txt'\n");
            printf(
                "\t\t- 'bash' to generate the environment variables layer settings script for 'Bash', the default filename is "
                "'vk_layer_settings.sh'\n");
            printf(
                "\t\t- 'bat' to generate the environment variables layer settings script for 'command prompt', the default "
                "filename is 'vk_layer_settings.bat'\n");
            printf("\t\t- 'hpp' to generate the C++ layer settings helper code, the default filename is 'vk_layer_settings.hpp'\n");
            printf("\t  (Run 'vkconfig layers --list' to enumerate the available layers.)\n");
            printf("\n");
            printf("\t`[--configuration [<configuration_index> | <configuration_name> | default]]`\n");
            printf(
                "\t\tSpecify the configuration name or index in the configuration list. If the argument is not set or set to "
                "'default', the default layer settings will be used.\n");
            printf("\t  (Run 'vkconfig loader --list' to enumerate the available configurations.)\n");
            printf("\n");
            printf("\t`[--layers <layer_names>]`\n");
            printf(
                "\t\tSpecify the layer names, if the argument is not set or set to 'default', all the found layers will be used. "
                "Multiple layer names can be listed using a comma (',') separator. \n");
            printf("\t  (Run 'vkconfig layers --list' to enumerate the available layers.)\n");
            printf("\n");
            printf("\t`[--output-dir | -d] <output_dir>`\n");
            printf(
                "\t\tSpecify the output directory path. The filename used will be the default filename if <output_file> is not "
                "set\n");
            printf("\t\t- If the 'generate' is set to 'html', the default filename is 'vk_layer_settings.html'\n");
            printf("\t\t- If the 'generate' is set to 'markdown', the default filename is 'vk_layer_settings.md'\n");
            printf("\t\t- If the 'generate' is set to 'txt', the default filename is 'vk_layer_settings.txt'\n");
            printf("\t\t- If the 'generate' is set to 'bash', the default filename is 'vk_layer_settings.sh'\n");
            printf("\t\t- If the 'generate' is set to 'bat', the default filename is 'vk_layer_settings.bat'\n");
            printf("\t\t- If the 'generate' is set to 'hpp', the default filename is 'vk_layer_settings.hpp'\n");
            printf("\n");
            printf("\t`[(--output | -o) <output_file>]`\n");
            printf("\t\tSpecify the output file path. If <output_dir> is set, then <output_file> must be the filename only.\n");
            printf("\t\t- If the 'generate' is set to 'html', the default filename is 'vk_layer_settings.html'\n");
            printf("\t\t- If the 'generate' is set to 'markdown', the default filename is 'vk_layer_settings.md'\n");
            printf("\t\t- If the 'generate' is set to 'txt', the default filename is 'vk_layer_settings.txt'\n");
            printf("\t\t- If the 'generate' is set to 'bash', the default filename is 'vk_layer_settings.sh'\n");
            printf("\t\t- If the 'generate' is set to 'bat', the default filename is 'vk_layer_settings.bat'\n");
            printf("\t\t- If the 'generate' is set to 'hpp', the default filename is 'vk_layer_settings.hpp'\n");
            printf("\n");
            printf("\t`[--dry-run]`\n");
            printf("\t\tRun without affecting the system and Vulkan Configurator files.\n");
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
            printf(
                "\t  (DEPRECATED: Run `vkconfig settings --generate html --layer <layer_name> [--output-dir <output_dir>]` "
                "instead.)\n");
            printf("\n");
            printf("\tvkconfig doc --markdown <layer_name> [<output_dir>]\n");
            printf("\t\tCreate the markdown documentation file for the given layer.\n");
            printf("\t\tThe file is written to <output_dir>, or current directory if not specified.\n");
            printf(
                "\t  (DEPRECATED: Run `vkconfig settings --generate markdown --layer <layer_name> [--output-dir <output_dir>]` "
                "instead.)\n");
            printf("\n");
            printf("\tvkconfig doc --settings <layer_name> [<output_dir>]\n");
            printf("\t\tCreate the vk_layers_settings.txt file for the given layer.\n");
            printf("\t\tThe file is written to <output_dir>, or current directory if not specified.\n");
            printf(
                "\t  (DEPRECATED: Run `vkconfig settings --generate txt --layer <layer_name> [--output-dir <output_dir>]` "
                "instead.)\n");
            break;
        }
        case HELP_RESET: {
            printf("Name\n");
            printf("\t'reset' - Reset layers configurations\n");
            printf("\n");
            printf("Synopsis\n");
            printf("\tvkconfig reset (--hard | -h)\n");
            printf("\tvkconfig reset [--soft | -s]\n");
            printf("\n");
            printf("Description\n");
            printf("\tvkconfig reset (--hard | -h)\n");
            printf("\t\tReset all layers configurations, all user-defined configurations will be lost.\n");
            printf("\n");
            printf("\tvkconfig reset [--soft | -s]\n");
            printf("\t\tReset all default configurations, all user-defined configuration will be preserved.\n");
            printf("\n");
            break;
        }
    }
}

void CommandLine::version() const {
    const std::string& version = Version::VKCONFIG.str();

    printf("%s version %s\n", VKCONFIG_NAME, version.c_str());
}

Path CommandLine::GetInputPath() const { return this->_input_path; }

Path CommandLine::GetOutputPath() const {
    Path output_path = this->_output_path;
    if (output_path.Empty()) {
        if (this->selected_layers_name.size() == 1) {
            output_path = this->selected_layers_name[0] + GetDefaultFileExt(this->_generate_settings_mode);
        } else {
            output_path = ::GetDefaultFilename(this->_generate_settings_mode);
        }
    }

    if (this->_output_dir.Empty()) {
        return output_path;
    } else {
        return this->_output_dir.AbsolutePath() + "/" + output_path.Filename().c_str();
    }
}
