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

enum LayersMode { LAYERS_MODE_BY_APPLICATIONS = 0, LAYERS_MODE_BY_CONFIGURATOR_RUNNING, LAYERS_MODE_BY_CONFIGURATOR_ALL_DISABLED };

struct Application {
    Application() {}
    Application(const std::string& name, const std::string& executable_full_path, const std::string& arguments);

    std::string app_name;
    Path executable_path;
    Path working_folder;
    std::string arguments;
    std::string env;
    Path log_file;
    LayersMode layers_mode;
    std::string layers_configuration;
};
