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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "main_loader.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/configuration.h"
#include "../vkconfig_core/layer_manager.h"

static int RunLoaderList(Configurator& configurator, const CommandLine& command_line) {
    const auto& configurations = configurator.configurations.available_configurations;

    if (configurator.configurations.available_configurations.empty()) {
        printf("vkconfig: No Vulkan Loader configuration found\n");
    } else {
        printf("vkconfig: Vulkan Loader Configurations:\n");
        for (std::size_t i = 0, n = configurations.size(); i < n; ++i) {
            printf(" - [%zd] %s\n", i, configurations[i].key.c_str());
        }
        printf("\n  (Use 'vkconfig loader --override <index>' to configure the system Vulkan Loader.)\n");
    }

    return 0;
}

static int GetConfigurationName(Configurator& configurator, const CommandLine& command_line, bool allow_import,
                                std::string& configuration_name) {
    if (allow_import && !command_line.layers_configuration_path.Empty()) {
        const bool load_result = configurator.configurations.ImportConfiguration(
            configurator.layers, command_line.layers_configuration_path, configuration_name);
        if (!load_result) {
            fprintf(stderr, "vkconfig: Failed to load `%s` loader configuration file...\n\n",
                    command_line.layers_configuration_path.AbsolutePath().c_str());

            ::RunLoaderList(configurator, command_line);
            return -1;
        }
    } else {
        const bool is_index = IsIndex(command_line.layers_configuration_name);

        if (is_index) {
            std::size_t index = std::strtoul(command_line.layers_configuration_name.c_str(), nullptr, 10);
            if (index >= configurator.configurations.available_configurations.size()) {
                fprintf(stderr, "vkconfig: Invalid `%zd` loader configuration index...\n\n", index);

                ::RunLoaderList(configurator, command_line);
                return -1;
            }

            configuration_name = configurator.configurations.available_configurations[index].key;
        } else {
            configuration_name = command_line.layers_configuration_name;
        }
    }

    if (configurator.configurations.FindConfiguration(configuration_name) == nullptr) {
        fprintf(stderr, "vkconfig: `%s` loader configuration not found...\n\n", configuration_name.c_str());

        ::RunLoaderList(configurator, command_line);
        return -1;
    }

    return 0;
}

static int RunLoaderOverride(Configurator& configurator, const CommandLine& command_line) {
    std::string configuration_name;
    int result = ::GetConfigurationName(configurator, command_line, true, configuration_name);
    if (result != 0) {
        return -1;
    }

    configurator.SetActiveConfigurationName(configuration_name);
    const bool override_result = configurator.Override(OVERRIDE_AREA_ALL);
    if (override_result) {
        printf("vkconfig: \"%s\" loader configuration applied to all Vulkan Applications.\n", configuration_name.c_str());
        const Configuration* configuration = configurator.GetActiveConfiguration();

        if (configuration->override_layers) {
            printf("\n");
            printf(" Including Vulkan layers:\n");
            printf(" (Execute Closer to the Vulkan Application)\n");
            for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
                const Parameter& parameter = configuration->parameters[i];
                printf("- %s (%s)\n", parameter.key.c_str(), GetToken(parameter.control));
            }
            printf(" (Execute Closer to the Vulkan Driver)\n");
        }

        if (configuration->override_loader) {
            printf("\n");
            if (configuration->loader_log_messages_flags == 0) {
                printf(" Including Vulkan loader messages: None\n");
            } else {
                printf(" Including Vulkan loader messages:\n");
                for (int i = 0, n = LOG_COUNT; i < n; ++i) {
                    LogBit bit = static_cast<LogBit>(1 << i);
                    if (configuration->loader_log_messages_flags & bit) {
                        printf("- %s\n", ::GetToken(bit));
                    }
                }
            }
        }

        printf("\n  (Run \"vkconfig loader --surrender\" to return Vulkan Loader control to Vulkan applications)\n");
    } else {
        fprintf(stderr, "vkconfig: Failed to activate `%s` Vulkan Loader configuration...\n", configuration_name.c_str());
    }

    if (!command_line.layers_configuration_path.Empty()) {
        configurator.configurations.RemoveConfiguration(configuration_name);
    }

    return override_result ? 0 : -1;
}

static int RunLoaderSurrender(Configurator& configurator, const CommandLine& command_line) {
    const bool has_overridden_layers = configurator.HasOverride();
    const bool surrender_result = configurator.Surrender(OVERRIDE_AREA_ALL);

    if (has_overridden_layers) {
        if (surrender_result) {
            printf("vkconfig: Full Vulkan Loader control returned to Vulkan applications.\n");
            printf(
                "\n  (Run \"vkconfig loader --override (<layers_configuration_file>|<layers_configuration_index>) to control the "
                "Vulkan Loader)\n");
        } else {
            fprintf(stderr, "vkconfig: Failed to surrender Vulkan Loader configuration...\n");
        }
    } else {
        fprintf(stderr, "vkconfig: No active Vulkan Loader configuration, nothing to surrender\n");
    }

    return surrender_result ? 0 : -1;
}

static int RunLoaderImport(Configurator& configurator, const CommandLine& command_line) {
    std::string configuration_name;
    const bool imported = configurator.configurations.ImportConfiguration(
        configurator.layers, command_line.layers_configuration_path, configuration_name);
    if (imported) {
        fprintf(stdout, "vkconfig: `%s` loader configuration imported from `%s` file.\n", configuration_name.c_str(),
                command_line.layers_configuration_path.AbsolutePath().c_str());
    } else {
        fprintf(stderr, "vkconfig: Failed to import `%s` loader configuration...\n",
                command_line.layers_configuration_path.AbsolutePath().c_str());
        return -1;
    }

    return ::RunLoaderList(configurator, command_line);
}

static int RunLoaderExport(Configurator& configurator, const CommandLine& command_line) {
    std::string configuration_name;
    int result = ::GetConfigurationName(configurator, command_line, false, configuration_name);
    if (result != 0) {
        return -1;
    }

    const bool exported = configurator.configurations.ExportConfiguration(
        configurator.layers, command_line.layers_configuration_path, configuration_name);
    if (exported) {
        fprintf(stdout, "vkconfig: `%s` loader configuration exported to:\n `%s`\n\n", configuration_name.c_str(),
                command_line.layers_configuration_path.AbsolutePath().c_str());
    } else {
        fprintf(stderr, "vkconfig: Failed to export `%s` loader configuration...\n",
                command_line.layers_configuration_path.AbsolutePath().c_str());
        return -1;
    }

    return ::RunLoaderList(configurator, command_line);
}

static int RunLoaderDelete(Configurator& configurator, const CommandLine& command_line) {
    std::string configuration_name;
    int result = ::GetConfigurationName(configurator, command_line, false, configuration_name);
    if (result != 0) {
        return -1;
    }

    fprintf(stdout, "vkconfig: `%s` loader configuration deleted.\n\n", configuration_name.c_str());

    configurator.configurations.RemoveConfiguration(configuration_name);

    return ::RunLoaderList(configurator, command_line);
}

int run_loader(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_LOADER);
    assert(command_line.error == ERROR_NONE);

    Configurator& configurator = Configurator::Get();
    configurator.Init();

    switch (command_line.command_loader_arg) {
        case COMMAND_LOADER_OVERRIDE: {
            return RunLoaderOverride(configurator, command_line);
        }
        case COMMAND_LOADER_SURRENDER: {
            return RunLoaderSurrender(configurator, command_line);
        }
        case COMMAND_LOADER_LIST: {
            return RunLoaderList(configurator, command_line);
        }
        case COMMAND_LOADER_IMPORT: {
            return RunLoaderImport(configurator, command_line);
        }
        case COMMAND_LOADER_EXPORT: {
            return RunLoaderExport(configurator, command_line);
        }
        case COMMAND_LOADER_DELETE: {
            return RunLoaderDelete(configurator, command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
