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

#include "main_layers.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/configuration.h"
#include "../vkconfig_core/layer_manager.h"

#include <cassert>

static int RunLayersOverride(Configurator& configurator, const CommandLine& command_line) {
    std::string configuration_name;
    const bool load_result = configurator.configurations.ImportConfiguration(
        configurator.layers, command_line.layers_configuration_path, configuration_name);
    if (!load_result) {
        fprintf(stderr, "vkconfig: [ERROR] Failed to load `%s` layers configuration file...\n",
                command_line.layers_configuration_path.AbsolutePath().c_str());
        return -1;
    }

    configurator.SetActiveConfigurationName(configuration_name);
    const bool override_result = configurator.Override(OVERRIDE_AREA_ALL);
    if (override_result) {
        printf("vkconfig: [INFO] \"%s\" applied to all Vulkan Applications, including Vulkan layers:\n",
               command_line.layers_configuration_path.AbsolutePath().c_str());
        const Configuration* configuration = configurator.GetActiveConfiguration();

        printf(" (Execute Closer to the Vulkan Application)\n");
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration->parameters[i];

            if (parameter.builtin == LAYER_BUILTIN_NONE) {
                const Layer* layer = configurator.layers.Find(parameter.key.c_str(), parameter.api_version);
                if (layer == nullptr) {
                    continue;
                }
            }

            printf("- %s (%s)\n", parameter.key.c_str(), GetToken(parameter.control));
        }
        printf(" (Execute Closer to the Vulkan Driver)\n");

        printf("\n  (Run \"vkconfig layers --surrender\" to return Vulkan layers control to Vulkan applications)\n");
    } else {
        fprintf(stderr, "vkconfig: [ERROR] Failed to override Vulkan applications layers...\n");
    }

    configurator.configurations.RemoveConfiguration(configuration_name);

    return override_result ? 0 : -1;
}

static int RunLayersSurrender(Configurator& configurator, const CommandLine& command_line) {
    const bool has_overridden_layers = configurator.HasOverride();
    const bool surrender_result = configurator.Surrender(OVERRIDE_AREA_ALL);

    if (has_overridden_layers) {
        if (surrender_result) {
            printf("vkconfig: [INFO] Full Vulkan layers control returned to Vulkan applications.\n");
            printf("\n  (Run \"vkconfig layers --override <layers_configuration_file> to take control of Vulkan layers)\n");
        } else {
            fprintf(stderr, "vkconfig: [ERROR] Failed to surrender Vulkan applications layers...\n");
        }
    } else {
        fprintf(stderr, "vkconfig: No overridden Vulkan layers, nothing to surrender\n");
    }

    return surrender_result ? 0 : -1;
}

static int RunLayersPath(Configurator& configurator, const CommandLine& command_line) {
    printf("vkconfig: [INFO] Paths to find Vulkan Layers\n");

    for (int layers_paths_index = 0, layers_paths_count = LAYERS_PATHS_COUNT; layers_paths_index < layers_paths_count;
         ++layers_paths_index) {
        const std::vector<LayersPathInfo>& paths = configurator.layers.paths[layers_paths_index];
        printf("\n%s:\n", GetLabel(static_cast<LayersPaths>(layers_paths_index)));

        if (paths.empty()) {
            printf(" - None\n");
        } else {
            for (std::size_t i = 0, n = paths.size(); i < n; ++i) {
                if (paths[i].enabled) {
                    printf(" - %s\n", paths[i].path.AbsolutePath().c_str());
                }
            }
        }
    }

    return 0;
}

static int RunLayersList(Configurator& configurator, const CommandLine& command_line) {
    if (configurator.layers.available_layers.empty()) {
        printf("vkconfig: [INFO] No Vulkan layer found\n");
    } else {
        const std::vector<std::string>& layer_names = configurator.layers.GatherLayerNames();

        for (std::size_t i = 0, n = layer_names.size(); i < n; ++i) {
            printf("%s\n", layer_names[i].c_str());
        }
    }

    return 0;
}

static int RunLayersVerbose(Configurator& configurator, const CommandLine& command_line) {
    printf("%s", configurator.layers.Log().c_str());
    return 0;
}

int run_layers(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_LAYERS);
    assert(command_line.error == ERROR_NONE);

    ConfiguratorGuard configurator_guard(CONFIGURATOR_MODE_CMD);

    switch (command_line.command_layers_arg) {
        case COMMAND_LAYERS_OVERRIDE: {
            return RunLayersOverride(configurator_guard.Get(), command_line);
        }
        case COMMAND_LAYERS_SURRENDER: {
            return RunLayersSurrender(configurator_guard.Get(), command_line);
        }
        case COMMAND_LAYERS_PATH: {
            return RunLayersPath(configurator_guard.Get(), command_line);
        }
        case COMMAND_LAYERS_LIST: {
            return RunLayersList(configurator_guard.Get(), command_line);
        }
        case COMMAND_LAYERS_VERBOSE: {
            return RunLayersVerbose(configurator_guard.Get(), command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
