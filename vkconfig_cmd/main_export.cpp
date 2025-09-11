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

#include "main_export.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/layer_manager.h"
#include "../vkconfig_core/configuration_manager.h"

static int run_export_txt_settings(Configurator& configurator, const CommandLine& command_line) {
    return configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT, command_line.export_filename) ? 0 : -1;
}

static int run_export_ext_code(Configurator& configurator, const CommandLine& command_line) {
    return configurator.WriteExtensionCode(command_line.export_filename) ? 0 : -1;
}

static int run_export_env_bash(Configurator& configurator, const CommandLine& command_line) {
    return configurator.Export(EXPORT_ENV_BASH, command_line.export_filename) ? 0 : -1;
}

static int run_export_env_cmd(Configurator& configurator, const CommandLine& command_line) {
    return configurator.Export(EXPORT_ENV_CMD, command_line.export_filename) ? 0 : -1;
}

int run_export(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_EXPORT);
    assert(command_line.error == ERROR_NONE);

    ConfiguratorGuard configurator_guard(CONFIGURATOR_MODE_CMD);

    Configurator& configurator = configurator_guard.Get();
    std::string configuration_key;
    if (configurator.GetActiveConfiguration() != nullptr) {
        configuration_key = configurator.GetActiveConfiguration()->key;
    }

    if (command_line.export_configuration_name == "default") {
        Configuration& configuration = configurator.configurations.CreateConfiguration(configurator.layers, "default");
        configurator.SetActiveConfigurationName(configuration.key);
    } else {
        configurator.SetActiveConfigurationName(command_line.export_configuration_name);
    }

    int result = -1;

    switch (command_line.command_export_arg) {
        case COMMAND_EXPORT_SETTINGS_TXT: {
            result = run_export_txt_settings(configurator, command_line);
            break;
        }
        case COMMAND_EXPORT_EXT_CODE: {
            result = run_export_ext_code(configurator, command_line);
            break;
        }
        case COMMAND_EXPORT_ENV_BASH: {
            result = run_export_env_bash(configurator, command_line);
            break;
        }
        case COMMAND_EXPORT_ENV_CMD: {
            result = run_export_env_cmd(configurator, command_line);
            break;
        }
        default: {
            assert(0);
        }
    }

    if (!configuration_key.empty()) {
        configurator.configurations.RemoveConfiguration(configuration_key);
    }

    if (command_line.export_configuration_name == "default") {
        configurator.SetActiveConfigurationName(configuration_key);
    }

    return result;
}
