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
#include "type_log.h"

#include <QByteArray>

#include <vector>
#include <string>

class LayerManager;

class Configuration {
   public:
    static Configuration CreateDisabled(const LayerManager& layers);
    static Configuration Create(const LayerManager& layers, const std::string& configuration_key);

    bool Load(const Path& full_path, const LayerManager& layers);
    bool Save(const Path& full_path, bool exporter = false) const;
    void Reset(const LayerManager& layers);

    bool HasOverride() const;
    Parameter* Find(const std::string& layer_key);
    std::size_t Size() const { return this->parameters.size(); };

    void SwitchLayerVersion(const LayerManager& layers, const std::string& layer_key, const Version& version);
    void GatherParameters(const LayerManager& layers);
    void Reorder(const std::vector<std::string>& layer_names);

    std::string key = "New Configuration";  // User readable display of the configuration name (may contain spaces)
    int version = 1;
    int platform_flags = PLATFORM_DESKTOP_BIT;
    std::string description;  // A friendly description of what this profile does
    bool view_advanced_settings = false;
    bool view_advanced_layers = true;
    int loader_log_messages_flags = GetBit(LOG_ERROR) | GetBit(LOG_WARN);
    std::string selected_layer_name;

    std::vector<Parameter> parameters;
    std::vector<Path> user_defined_paths;

    bool IsBuiltIn() const;
};

std::string MakeConfigurationName(const std::vector<Configuration>& configurations, const std::string& configuration_name);
