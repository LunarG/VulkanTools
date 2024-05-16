/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

static int RunLayersOverride(const CommandLine& command_line) {
    Configurator& configurator = Configurator::Get();

    Configuration configuration;
    const bool load_result =
        configuration.Load(configurator.layers.selected_layers, command_line.layers_configuration_path.c_str());
    if (!load_result) {
        printf("\nFailed to load the layers configuration file...\n");
        return -1;
    }

    const bool override_result = configurator.Override();

    configurator.environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit

    if (override_result) {
        printf("\nLayers configuration \"%s\" applied to all Vulkan Applications, including Vulkan layers:\n",
               command_line.layers_configuration_path.c_str());
        for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration.parameters[i];
            if (parameter.control == LAYER_CONTROL_AUTO) {
                continue;
            }

            printf("\t%s (%s)\n", parameter.key.c_str(), parameter.control == LAYER_CONTROL_ON ? "Overridden" : "Excluded");
        }
        printf("\n  (Use \"vkconfig layers --surrender\" to return Vulkan layers control to Vulkan applications.)\n");
    } else {
        printf("\nFailed to override Vulkan applications layers...\n");
    }

    return override_result ? 0 : -1;
}

static int RunLayersSurrender(const CommandLine& command_line) {
    Configurator& configurator = Configurator::Get();

    const bool has_overridden_layers = configurator.HasOverride();
    const bool surrender_result = configurator.Surrender();

    configurator.environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit

    if (has_overridden_layers) {
        if (surrender_result) {
            printf("\nFull Vulkan layers control returned to Vulkan applications.\n");
            printf("\n  (Use \"vkconfig layers --override <layers_configuration_file> to take control of Vulkan layers.)\n");
        } else {
            printf("\nFailed to surrender Vulkan applications layers...\n");
        }
    } else {
        printf("\nNo overridden Vulkan layers, nothing to surrender.\n");
    }

    return surrender_result ? 0 : -1;
}

static int RunLayersList(const CommandLine& command_line) {
    LayerManager layers;

    if (layers.selected_layers.empty()) {
        printf("No Vulkan layer found\n");
    } else {
        for (std::size_t i = 0, n = layers.selected_layers.size(); i < n; ++i) {
            const Layer& layer = layers.selected_layers[i];

            printf("%s\n", layer.key.c_str());
        }
    }

    return 0;
}

static int RunLayersVerbose(const CommandLine& command_line) {
    LayerManager layers;

    for (std::size_t i = 0, n = layers.selected_layers.size(); i < n; ++i) {
        const Layer& layer = layers.selected_layers[i];

        printf("%s (%s) %s-%s\n", layer.key.c_str(), GetToken(layer.type), layer.api_version.str().c_str(),
               layer.implementation_version.c_str());
        printf("- %s\n", layer.description.c_str());
        printf("- %s\n", layer.manifest_path.AbsolutePath().c_str());
        printf("- %s\n", layer.binary_path.AbsolutePath().c_str());
    }

    return 0;
}

int run_layers(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_LAYERS);
    assert(command_line.error == ERROR_NONE);

    switch (command_line.command_layers_arg) {
        case COMMAND_LAYERS_OVERRIDE: {
            return RunLayersOverride(command_line);
        }
        case COMMAND_LAYERS_SURRENDER: {
            return RunLayersSurrender(command_line);
        }
        case COMMAND_LAYERS_LIST: {
            return RunLayersList(command_line);
        }
        case COMMAND_LAYERS_VERBOSE: {
            return RunLayersVerbose(command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
