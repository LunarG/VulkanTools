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
    {COMMAND_SHOW_USAGE, "-h", HELP_HELP},            // COMMAND_SHOW_USAGE
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
    {COMMAND_SETTINGS_TXT, "--txt"},       {COMMAND_SETTINGS_BASH, "--bash"}, {COMMAND_SETTINGS_CMD, "--bat"},
    {COMMAND_SETTINGS_HPP, "--hpp"},       {COMMAND_SETTINGS_HTML, "--html"}, {COMMAND_SETTINGS_MARKDOWN, "--markdown"},
    {COMMAND_SETTINGS_OUTPUT, "--output"}, {COMMAND_SETTINGS_OUTPUT, "-o"},   {COMMAND_SETTINGS_DRY_RUN, "--dry-run"}};

static CommandLayersArg GetCommandLayersId(const char* token) {
    assert(token != nullptr);

    for (std::size_t i = 0, n = std::size(command_layers_desc); i < n; ++i) {
        if (std::strcmp(command_layers_desc[i].token, token) == 0) {
            return command_layers_desc[i].arguments;
        }
    }

    return COMMAND_LAYERS_NONE;
}

static const CommandLayersDesc& GetCommandLayers(CommandLayersArg layers_arg) {
    assert(layers_arg != COMMAND_LAYERS_NONE);

    for (std::size_t i = 0, n = std::size(command_layers_desc); i < n; ++i) {
        if (command_layers_desc[i].arguments == layers_arg) {
            return command_layers_desc[i];
        }
    }

    assert(0);
    return command_layers_desc[0];
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

static const CommandLoaderDesc& GetCommandLoader(CommandLoaderArg loader_arg) {
    assert(loader_arg != COMMAND_LOADER_NONE);

    for (std::size_t i = 0, n = std::size(command_loader_desc); i < n; ++i) {
        if (command_loader_desc[i].arguments == loader_arg) {
            return command_loader_desc[i];
        }
    }

    assert(0);
    return command_loader_desc[0];
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

static const CommandSettingsDesc& GetCommandSettings(CommandSettingsArg export_arg) {
    assert(export_arg != COMMAND_SETTINGS_NONE);

    for (std::size_t i = 0, n = std::size(command_settings_desc); i < n; ++i) {
        if (command_settings_desc[i].arguments == export_arg) {
            return command_settings_desc[i];
        }
    }

    assert(0);
    return command_settings_desc[0];
}

static const char* GetDefaultFilaname(SettingsFormat format) {
    static const char* TABLE[] = {
        "N/A",                        // SETTINGS_FORMAT_NONE
        "vk_layer_settings.html",     // SETTINGS_FORMAT_HTML
        "vk_layer_settings.mk",       // SETTINGS_FORMAT_MARKDOWN
        "vk_layer_settings.txt",      // SETTINGS_FORMAT_TXT
        "vk_layer_settings.sh",       // SETTINGS_FORMAT_BASH
        "vk_layer_settings.bat",      // SETTINGS_FORMAT_CMD
        "vulkan_layer_settings.hpp",  // SETTINGS_FORMAT_HPP
    };
    static_assert(std::size(TABLE) == SETTINGS_FORMAT_COUNT);

    return TABLE[format - SETTINGS_FORMAT_FIRST];
}

static SettingsFormat GetSettingsFormat(CommandSettingsArg command_settings_arg) {
    static const SettingsFormat TABLE[] = {
        SETTINGS_FORMAT_NONE,      // COMMAND_SETTINGS_NONE
        SETTINGS_FORMAT_TXT,       // COMMAND_SETTINGS_TXT
        SETTINGS_FORMAT_BASH,      // COMMAND_SETTINGS_BASH
        SETTINGS_FORMAT_CMD,       // COMMAND_SETTINGS_CMD
        SETTINGS_FORMAT_HPP,       // COMMAND_SETTINGS_HPP
        SETTINGS_FORMAT_HTML,      // COMMAND_SETTINGS_HTML
        SETTINGS_FORMAT_MARKDOWN,  // COMMAND_SETTINGS_MARKDOWN
        SETTINGS_FORMAT_NONE,      // COMMAND_SETTINGS_OUTPUT
        SETTINGS_FORMAT_NONE,      // COMMAND_SETTINGS_DRY_RUN
    };
    static_assert(std::size(TABLE) == COMMAND_SETTINGS_COUNT);

    return TABLE[command_settings_arg];
}

CommandLine::CommandLine(int argc, char* argv[])
    : command(_command),
      command_reset_arg(_command_reset_arg),
      command_layers_arg(_command_layers_arg),
      command_loader_arg(_command_loader_arg),
      command_doc_arg(_command_doc_arg),
      settings_format(_settings_format),
      selected_layer_name(_selected_layer_name),
      selected_configuration_name(_selected_configuration_name),
      input_path(_input_path),
      output_path(_output_path),
      dry_run(_dry_run),
      error(_error),
      error_args(_error_args),
      _command_reset_arg(COMMAND_RESET_NONE),
      _command_layers_arg(COMMAND_LAYERS_NONE),
      _command_loader_arg(COMMAND_LOADER_NONE),
      _command_doc_arg(COMMAND_DOC_NONE),
      _settings_format(SETTINGS_FORMAT_NONE),
      _error(ERROR_NONE),
      _help(HELP_DEFAULT) {
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
                this->_command_loader_arg = ::GetCommandLoaderId(command_argument.c_str());

                switch (this->_command_loader_arg) {
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
                this->_command_layers_arg = ::GetCommandLayersId(command_argument.c_str());

                switch (this->_command_layers_arg) {
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

            this->_selected_layer_name = argv[arg_offset + 2];
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
                    printf("\n  (Run \"vkconfig settings --html\" instead)\n");
                    break;
                case COMMAND_DOC_MARKDOWN:
                    printf("vkconfig: \"vkconfig doc --markdown\" is deprecated\n");
                    printf("\n  (Run \"vkconfig settings --markdown\" instead)\n");
                    break;
                case COMMAND_DOC_SETTINGS:
                    printf("vkconfig: \"vkconfig doc --settings\" is deprecated\n");
                    printf("\n  (Run \"vkconfig settings --txt\" instead)\n");
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
                    case COMMAND_SETTINGS_TXT:
                    case COMMAND_SETTINGS_BASH:
                    case COMMAND_SETTINGS_CMD:
                    case COMMAND_SETTINGS_HPP:
                    case COMMAND_SETTINGS_HTML:
                    case COMMAND_SETTINGS_MARKDOWN: {
                        this->_settings_format = ::GetSettingsFormat(command_settings_arg);

                        if (this->_settings_format == SETTINGS_FORMAT_HTML || this->_settings_format == SETTINGS_FORMAT_MARKDOWN) {
                            if (argc <= arg_offset + 1) {
                                this->_error = ERROR_MISSING_COMMAND_ARGUMENT;
                            } else {
                                const std::string layer_name = argv[arg_offset + 1];
                                if (layer_name[0] == '-') {  // Not a value but another argumment
                                    this->_error = ERROR_INVALID_COMMAND_ARGUMENT;
                                } else {
                                    this->_selected_layer_name = layer_name;
                                    arg_offset += 2;
                                }
                            }
                        } else {
                            if (argc <= arg_offset + 1) {
                                this->_selected_configuration_name = "default";
                                arg_offset += 1;
                            } else {
                                const std::string configuration_name = argv[arg_offset + 1];
                                if (configuration_name[0] == '-') {  // Not a value but another argumment
                                    this->_selected_configuration_name = "default";
                                    arg_offset += 1;
                                } else {
                                    this->_selected_configuration_name = configuration_name;
                                    arg_offset += 2;
                                }
                            }
                        }

                        if (this->_output_path.Empty()) {
                            this->_output_path = ::GetDefaultFilaname(this->_settings_format);
                        }
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
            printf("\tvkconfig [help] | [version] | [gui] | [reset <args>] | [doc <args>] |\n");
            printf("\t         [loader <args>] | [layers <args>] | [settings <args>]\n");
            printf("\n");
            printf("Command:\n");
            printf("\thelp          = Display usage and documentation.\n");
            printf("\tversion       = Display %s version.\n", VKCONFIG_NAME);
            printf("\tgui           = Launch the graphical interface.\n");
            printf("\treset         = Reset layers configurations.\n");
            printf("\tdoc           = Create documentation files for layer.\n");
            printf("\tloader        = Configure system the Vulkan Loader configuration.\n");
            printf("\tlayers        = List the Vulkan layers found on the system.\n");
            printf("\tsettings      = Create layer setting files for Vulkan developers.\n");
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
            printf("\t\tDEPRECATED: use `vkconfig loader --override` instead.\n");
            printf("\n");
            printf("\tvkconfig layers (--surrender | -s)\n");
            printf("\t\tSurrender the Vulkan layers control to Vulkan applications.\n");
            printf("\t\tDEPRECATED: use `vkconfig loader --surrender` instead.\n");
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
            printf("\n");
            printf("\tvkconfig loader (--delete | -d) (<configuration_index> | <configuration_name>)\n");
            printf("\t\tRemove a Vulkan Layers configuration stored by %s on the system.\n", VKCONFIG_NAME);
            printf("\t\t - <configuration_index> is an index enumerated with `vkconfig loader --list`.\n");
            printf("\t\t - <configuration_name> is the name of the stored configuration listed with `vkconfig loader --list`.\n");
            break;
        }
        case HELP_SETTINGS: {
            printf("Name\n");
            printf("\t'settings' - Command to generate layer settings files\n");
            printf("\n");
            printf("Synopsis\n");
            // printf("\tvkconfig settings --html <layer_name> [(--output | -o) <output_file>] [--dry-run]\n");
            // printf("\tvkconfig settings --markdown <layer_name> [(--output | -o) <output_file>] [--dry-run]\n");
            printf(
                "\tvkconfig settings --txt [<configuration_index> | <configuration_name> | default] [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf(
                "\tvkconfig settings --bash [<configuration_index> | <configuration_name> | default] [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf(
                "\tvkconfig settings --bat [<configuration_index> | <configuration_name> | default] [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf(
                "\tvkconfig settings --hpp [<configuration_index> | <configuration_name> | default] [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf("\n");
            printf("Description\n");
            printf("\n");
            /*
            printf("\tvkconfig settings --html <layer_name> [(--output | -o) <output_file>] [--dry-run]\n");
            printf("\t\tGenerate vk_layer_settings.txt file of a layers configuration.\n");
            printf("\t\tThe file is written to <output_file>, or current directory if not specified.\n");
            printf("\t\t `--output`: Specify the output file path.\n");
            printf("\t\t `--dry-run`: Run without affecting the system and Vulkan Configurator files.\n");
            printf("\n");
            printf("\tvkconfig settings --markdown <layer_name> [(--output | -o) <output_file>] [--dry-run]\n");
            printf("\t\tGenerate vk_layer_settings.txt file of a layers configuration.\n");
            printf("\t\tThe file is written to <output_file>, or current directory if not specified.\n");
            printf("\t\t `--output`: Specify the output file path.\n");
            printf("\t\t `--dry-run`: Run without affecting the system and Vulkan Configurator files.\n");
            printf("\n");
*/
            printf(
                "\tvkconfig settings --txt [<configuration_index> | <configuration_name> | default] [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf("\t\tGenerate vk_layer_settings.txt file of a layers configuration.\n");
            printf("\t\tThe file is written to <output_file>, or current directory if not specified.\n");
            printf("\t\t `--output`: Specify the output file path.\n");
            printf("\t\t `--dry-run`: Run without affecting the system and Vulkan Configurator files.\n");
            printf("\n");
            printf(
                "\tvkconfig settings --bash [<configuration_index> | <configuration_name> | default] [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf("\t\tGenerate the environment variables bash script of a layers configuration.\n");
            printf("\t\tThe file is written to <output_file>, or current directory if not specified.\n");
            printf("\t\t `--output`: Specify the output file path.\n");
            printf("\t\t `--dry-run`: Run without affecting the system and Vulkan Configurator files.\n");
            printf("\n");
            printf(
                "\tvkconfig settings --bat (<configuration_index> | <configuration_name> | default) [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf("\t\tGenerate the environment variables command prompt script of a layers configuration.\n");
            printf("\t\tThe file is written to <output_file>, or current directory if not specified.\n");
            printf("\t\t `--output`: Specify the output file path.\n");
            printf("\t\t `--dry-run`: Run without affecting the system and Vulkan Configurator files.\n");
            printf("\n");
            printf(
                "\tvkconfig settings --hpp (<configuration_index> | <configuration_name> | default) [(--output | -o) "
                "<output_file>] [--dry-run]\n");
            printf("\t\tGenerate VK_EXT_layer_settings code of a layers configuration.\n");
            printf("\t\tThe file is written to <output_file>, or current directory if not specified.\n");
            printf("\t\t `--output`: Specify the output file path.\n");
            printf("\t\t `--dry-run`: Run without affecting the system and Vulkan Configurator files.\n");
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

void CommandLine::version() const {
    const std::string& version = Version::VKCONFIG.str();

    printf("%s version %s\n", VKCONFIG_NAME, version.c_str());
}
