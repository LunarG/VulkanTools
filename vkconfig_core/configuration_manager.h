/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "configuration.h"
#include "environment.h"
#include "path_manager.h"

#include <string>
#include <vector>

class ConfigurationManager {
   public:
    ConfigurationManager(const PathManager& path_manager, Environment& environment);
    ~ConfigurationManager();

    void LoadAllConfigurations(const std::vector<Layer>& available_layers);

    void SaveAllConfigurations(const std::vector<Layer>& available_layers);

    Configuration& CreateConfiguration(const std::vector<Layer>& available_layers, const std::string& configuration_name);

    void RemoveConfiguration(const std::vector<Layer>& available_layers, const std::string& configuration_name);

    void ImportConfiguration(const std::vector<Layer>& available_layers, const std::string& full_import_path);
    void ExportConfiguration(const std::vector<Layer>& available_layers, const std::string& full_export_path,
                             const std::string& configuration_name);

    Configuration* GetActiveConfiguration() const { return active_configuration; }
    void SetActiveConfiguration(const std::vector<Layer>& available_layers, Configuration* active_configuration);
    void SetActiveConfiguration(const std::vector<Layer>& available_layers, const std::string& configuration_name);
    bool HasActiveConfiguration(const std::vector<Layer>& available_layers) const;

    void RefreshConfiguration(const std::vector<Layer>& available_layers);

    void ResetDefaultsConfigurations(const std::vector<Layer>& available_layers);

    void ReloadDefaultsConfigurations(const std::vector<Layer>& available_layers);

    void FirstDefaultsConfigurations(const std::vector<Layer>& available_layers);

    void SortConfigurations();

    bool Empty() const { return available_configurations.empty(); }

    std::vector<Configuration> available_configurations;

   private:
    void RemoveConfigurationFiles();

    void LoadConfigurationsPath(const std::vector<Layer>& available_layers, PathType path_type);
    void LoadDefaultConfigurations(const std::vector<Layer>& available_layers);

    Configuration* active_configuration;

    const PathManager& path_manager;
    Environment& environment;
};
