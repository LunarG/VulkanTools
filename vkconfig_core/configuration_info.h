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

#include <string>

class ConfigurationInfo {
   private:
    std::string name = "Validation";
    LayersMode mode = LAYERS_CONTROLLED_BY_CONFIGURATOR;

   public:
    const char* GetName() const { return this->name.c_str(); }

    void SetName(const std::string& new_configuration_name) {
        if (this->name != new_configuration_name) {
            this->name = new_configuration_name;
        }
    }

    LayersMode GetMode() const { return this->mode; }

    void SetMode(LayersMode new_configuration_mode) {
        if (this->mode != new_configuration_mode) {
            this->mode = new_configuration_mode;
        }
    }
};
