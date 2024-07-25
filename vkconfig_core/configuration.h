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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "parameter.h"

#include <QByteArray>

#include <vector>
#include <string>

class Configuration {
   public:
    static Configuration CreateDisabled(const std::vector<Layer>& available_layers);
    static Configuration Create(const std::vector<Layer>& available_layers, const std::string& key);

    bool Load(const Path& full_path, const std::vector<Layer>& available_layers);
    bool Save(const Path& full_path, bool exporter = false) const;
    bool HasOverride() const;
    Parameter* Find(std::string parameter_key);

    void Reset(const std::vector<Layer>& available_layers);

    std::size_t Size() const { return this->parameters.size(); };

    std::string key = "New Configuration";  // User readable display of the configuration name (may contain spaces)
    int version = 1;
    int platform_flags = PLATFORM_DESKTOP_BIT;
    std::string description;        // A friendly description of what this profile does
    QByteArray setting_tree_state;  // Recall editor tree state
    bool view_advanced_settings = false;
    bool view_advanced_layers = false;

    std::vector<Parameter> parameters;
    std::vector<Path> user_defined_paths;

    bool IsBuiltIn() const;
};

std::string MakeConfigurationName(const std::vector<Configuration>& configurations, const std::string& configuration_name);
