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

#pragma once

#include "path.h"

#include <string>
#include <vector>

enum LayersMode {
    LAYERS_CONTROLLED_BY_APPLICATIONS = 0,
    LAYERS_CONTROLLED_BY_CONFIGURATOR,
    LAYERS_DISABLED_BY_CONFIGURATOR,

    LAYERS_MODE_FIRST = LAYERS_CONTROLLED_BY_APPLICATIONS,
    LAYERS_MODE_LAST = LAYERS_DISABLED_BY_CONFIGURATOR
};

enum { LAYERS_MODE_COUNT = LAYERS_MODE_LAST - LAYERS_MODE_FIRST + 1 };

struct ApplicationOptions {
    std::string label;
    std::string working_folder;
    std::vector<std::string> arguments;
    std::vector<std::string> environment_variables;
    std::string log_file;
};

struct Application {
    Application() : layers_mode(LAYERS_CONTROLLED_BY_APPLICATIONS) {}

    Path executable_path;
    LayersMode layers_mode;
    std::string layers_configuration;
    int active_option_index;
    std::vector<ApplicationOptions> options;

    ApplicationOptions& GetActiveOptions() { return this->options[active_option_index]; }

    const ApplicationOptions& GetActiveOptions() const { return this->options[active_option_index]; }
};
