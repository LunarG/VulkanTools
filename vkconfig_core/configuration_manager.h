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

#include "configuration.h"
#include "configuration_info.h"
#include "path.h"
#include "serialization.h"

#include <string>
#include <vector>

extern const char* GLOBAL_CONFIGURATION_TOKEN;

class ConfigurationManager : public Serialize {
   public:
    ConfigurationManager();
    ~ConfigurationManager();

    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;

    void LoadAllConfigurations(const LayerManager& layers);
    void SaveAllConfigurations() const;

    const ConfigurationInfo* GetActiveConfigurationInfo() const;
    ConfigurationInfo* GetActiveConfigurationInfo();
    const ConfigurationInfo* FindConfigurationInfo(const std::string& key) const;
    const std::map<std::string, ConfigurationInfo>& GetConfigurationInfos() const;
    bool HasActiveConfiguration() const;

    Configuration& CreateConfiguration(const LayerManager& layers, const std::string& configuration_name);
    Configuration& DuplicateConfiguration(const LayerManager& layers, const std::string& configuration_name);
    void RemoveConfiguration(const std::string& configuration_name);
    int GetConfigurationIndex(const std::string& configuration_name) const;

    void ImportConfiguration(const LayerManager& layers, const Path& full_import_path);
    void ExportConfiguration(const LayerManager& layers, const Path& full_export_path, const std::string& configuration_name);

    const Configuration* FindConfiguration(const std::string& configuration_name) const;
    Configuration* FindConfiguration(const std::string& configuration_name);

    void SortConfigurations();

    bool Empty() const { return available_configurations.empty(); }

    bool HasFile(const Configuration& configuration) const;
    void RemoveConfigurationFile(const std::string& key);
    void RemoveConfigurationFiles();

    bool GetPerExecutableConfig() const;
    void SetPerExecutableConfig(bool enabled);

    bool GetUseSystemTray() const;
    void SetUseSystemTray(bool enabled);

    std::vector<Configuration> available_configurations;
    Path last_path_import;
    Path last_path_export;

   private:
    void LoadConfigurationsPath(const LayerManager& layers);
    void LoadDefaultConfigurations(const LayerManager& layers);

    std::map<std::string, int> removed_built_in_configuration;
    bool use_system_tray = false;
    bool use_per_executable_configuration = false;
    std::string active_executable;
    std::map<std::string, ConfigurationInfo> configuration_infos;
};
