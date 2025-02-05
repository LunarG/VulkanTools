/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include "path.h"
#include "serialization.h"

#include <string>
#include <vector>

class ConfigurationManager : public Serialize {
   public:
    ConfigurationManager();
    ~ConfigurationManager();

    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    std::string Log() const override;

    void Clear();
    bool Empty() const;
    std::size_t Size() const;

    void LoadAllConfigurations(const LayerManager& layers);
    void SaveAllConfigurations() const;

    void UpdateConfigurations(const LayerManager& layers);

    Configuration& CreateConfiguration(const LayerManager& layers, const std::string& configuration_name);
    Configuration& DuplicateConfiguration(const LayerManager& layers, const std::string& configuration_name);
    void RemoveConfiguration(const std::string& configuration_name);

    void RenameConfiguration(const std::string& old_configuration_name, const std::string& new_configuration_name);
    bool ImportConfiguration(const LayerManager& layers, const Path& full_import_path, std::string& configuration_name);
    bool ExportConfiguration(const LayerManager& layers, const Path& full_export_path, const std::string& configuration_name);

    const Configuration* FindConfiguration(const std::string& configuration_name) const;
    Configuration* FindConfiguration(const std::string& configuration_name);

    void SortConfigurations();

    bool IsDefaultConfiguration(const std::string& configuration_key) const;
    void ResetDefaultConfigurations(const LayerManager& layers);

    bool HasFile(const Configuration& configuration) const;
    void RemoveConfigurationFile(const std::string& key);
    void RemoveConfigurationFiles();

    std::vector<Configuration> available_configurations;
    Path last_path_import_config = Path(Path::HOME);
    Path last_path_export_config = Path(Path::HOME);
    Path last_path_export_settings = Path(Path::HOME).RelativePath() + "/vk_layer_settings.txt";

    // Public for unit tests...
    void LoadDefaultConfigurations(const LayerManager& layers);

   private:
    void LoadConfigurationsPath(const LayerManager& layers);

    std::map<std::string, int> removed_built_in_configuration;
};
