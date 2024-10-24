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

static int RunLayersOverride(Configurator& configurator, const CommandLine& command_line) {
    const bool load_result =
        configurator.configurations.ImportConfiguration(configurator.layers, command_line.layers_configuration_path);
    if (!load_result) {
        fprintf(stderr, "vkconfig: Failed to load %s layers configuration file...\n",
                command_line.layers_configuration_path.c_str());
        return -1;
    }

    const bool override_result = configurator.Override(OVERRIDE_AREA_ALL);
    if (override_result) {
        printf("vkconfig: Layers configuration \"%s\" applied to all Vulkan Applications, including Vulkan layers:\n",
               command_line.layers_configuration_path.c_str());
        const Configuration* configuration = configurator.GetActiveConfiguration();

        printf(" (Execute Closer to the Vulkan Application)\n");
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration->parameters[i];
            printf("- %s (%s)\n", parameter.key.c_str(), GetToken(parameter.control));
        }
        printf(" (Execute Closer to the Vulkan Driver)\n");

        printf("\n  (Run \"vkconfig layers --surrender\" to return Vulkan layers control to Vulkan applications)\n");
    } else {
        fprintf(stderr, "vkconfig: Failed to override Vulkan applications layers...\n");
    }

    return override_result ? 0 : -1;
}

static int RunLayersSurrender(Configurator& configurator, const CommandLine& command_line) {
    const bool has_overridden_layers = configurator.HasOverride();
    const bool surrender_result = configurator.Surrender(OVERRIDE_AREA_ALL);

    if (has_overridden_layers) {
        if (surrender_result) {
            printf("vkconfig: Full Vulkan layers control returned to Vulkan applications.\n");
            printf("\n  (Run \"vkconfig layers --override <layers_configuration_file> to take control of Vulkan layers)\n");
        } else {
            fprintf(stderr, "vkconfig: Failed to surrender Vulkan applications layers...\n");
        }
    } else {
        fprintf(stderr, "vkconfig: No overridden Vulkan layers, nothing to surrender\n");
    }

    return surrender_result ? 0 : -1;
}

static int RunLayersList(Configurator& configurator, const CommandLine& command_line) {
    if (configurator.layers.selected_layers.empty()) {
        printf("vkconfig: No Vulkan layer found\n");
    } else {
        for (std::size_t i = 0, n = configurator.layers.selected_layers.size(); i < n; ++i) {
            const Layer& layer = configurator.layers.selected_layers[i];

            printf("%s\n", layer.key.c_str());
        }
    }

    return 0;
}

static int RunLayersVerbose(Configurator& configurator, const CommandLine& command_line) {
    for (std::size_t i = 0, n = configurator.layers.selected_layers.size(); i < n; ++i) {
        const Layer& layer = configurator.layers.selected_layers[i];

        printf("%s %s-%s\n", layer.key.c_str(), layer.api_version.str().c_str(), layer.implementation_version.c_str());
        printf("- %s\n", layer.description.c_str());
        printf("- %s\n", layer.manifest_path.AbsolutePath().c_str());
        printf("- %s\n", layer.binary_path.AbsolutePath().c_str());
        printf("- %s layer\n", GetToken(layer.type));

        if (i < (n - 1)) {
            printf("\n");
        }
    }

    return 0;
}

int run_layers(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_LAYERS);
    assert(command_line.error == ERROR_NONE);

    Configurator& configurator = Configurator::Get();
    configurator.Init(Configurator::CMD);

    switch (command_line.command_layers_arg) {
        case COMMAND_LAYERS_OVERRIDE: {
            return RunLayersOverride(configurator, command_line);
        }
        case COMMAND_LAYERS_SURRENDER: {
            return RunLayersSurrender(configurator, command_line);
        }
        case COMMAND_LAYERS_LIST: {
            return RunLayersList(configurator, command_line);
        }
        case COMMAND_LAYERS_VERBOSE: {
            return RunLayersVerbose(configurator, command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
