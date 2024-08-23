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

#include "type_layers_mode.h"
#include "path.h"

#include <string>
#include <vector>

struct ApplicationOptions {
    std::string label;
    Path working_folder;
    std::vector<std::string> arguments;
    std::vector<std::string> environment_variables;
    Path log_file;
};

struct Application {
    Path executable_path;
    int active_option_index = 0;
    std::vector<ApplicationOptions> options;

    ApplicationOptions& GetActiveOptions() { return this->options[active_option_index]; }

    const ApplicationOptions& GetActiveOptions() const { return this->options[active_option_index]; }
};
