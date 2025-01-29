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

#include "util.h"
#include "configuration_manager.h"
#include "configurator.h"
#include "alert.h"

ConfigurationManager::ConfigurationManager() {}

ConfigurationManager::~ConfigurationManager() {}

bool ConfigurationManager::Load(const QJsonObject &json_root_object) {
    // configurations json object
    if (json_root_object.value("configurations") != QJsonValue::Undefined) {
        const QJsonObject &json_configurations_object = json_root_object.value("configurations").toObject();

        if (json_configurations_object.value("last_path_export_settings") != QJsonValue::Undefined) {
            this->last_path_export_settings =
                json_configurations_object.value("last_path_export_settings").toString().toStdString();
        }
        if (json_configurations_object.value("last_path_export_config") != QJsonValue::Undefined) {
            this->last_path_export_config = json_configurations_object.value("last_path_export_config").toString().toStdString();
        }
        if (json_configurations_object.value("last_path_import") != QJsonValue::Undefined) {
            this->last_path_import_config = json_configurations_object.value("last_path_import").toString().toStdString();
        }

        if (json_configurations_object.value("removed") != QJsonValue::Undefined) {
            const QJsonObject &json_removed_builtin_object = json_configurations_object.value("removed").toObject();
            const QStringList &json_removed_builtin_keys = json_removed_builtin_object.keys();

            for (int i = 0, n = json_removed_builtin_keys.length(); i < n; ++i) {
                const std::string &key = json_removed_builtin_keys[i].toStdString();
                const int version = json_removed_builtin_object.value(key.c_str()).toInt();
                this->removed_built_in_configuration.insert(std::make_pair(key, version));
            }
        }
    }

    return true;
}

bool ConfigurationManager::Save(QJsonObject &json_root_object) const {
    this->SaveAllConfigurations();

    QJsonObject json_removed_builtin_configurations_object;
    for (auto it = this->removed_built_in_configuration.begin(), end = this->removed_built_in_configuration.end(); it != end;
         ++it) {
        json_removed_builtin_configurations_object.insert(it->first.c_str(), it->second);
    }

    QJsonObject json_configurations_object;
    json_configurations_object.insert("last_path_export", this->last_path_export_config.RelativePath().c_str());
    json_configurations_object.insert("last_path_import", this->last_path_import_config.RelativePath().c_str());
    json_configurations_object.insert("removed", json_removed_builtin_configurations_object);

    json_root_object.insert("configurations", json_configurations_object);

    return true;
}

std::string ConfigurationManager::Log() const {
    std::string log;
    return log;
}

void ConfigurationManager::Clear() { this->available_configurations.clear(); }

bool ConfigurationManager::Empty() const { return this->available_configurations.empty(); }

std::size_t ConfigurationManager::Size() const { return this->available_configurations.size(); }

void ConfigurationManager::LoadAllConfigurations(const LayerManager &layers) {
    this->available_configurations.clear();

    this->LoadConfigurationsPath(layers);

    // If built-in configurations were updated, replace the stored configuration
    this->LoadDefaultConfigurations(layers);

    this->SortConfigurations();
}

void ConfigurationManager::LoadDefaultConfigurations(const LayerManager &layers) {
    const std::vector<Path> &configuration_files = CollectFilePaths(":/configurations/");

    for (std::size_t i = 0, n = configuration_files.size(); i < n; ++i) {
        Configuration configuration;
        const bool result = configuration.Load(configuration_files[i], layers);
        assert(result);

        if (!IsPlatformSupported(configuration.platform_flags)) {
            continue;
        }

        if (VKC_PLATFORM == PLATFORM_WINDOWS_ARM) {
            if (configuration.key == "Frame Capture") {
                continue;
            } else if (configuration.key == "Crash Diagnostic") {
                continue;
            }
        }

        OrderParameter(configuration.parameters, layers);

        Configuration *found_configuration = this->FindConfiguration(configuration.key);
        if (found_configuration == nullptr) {
            auto iter = this->removed_built_in_configuration.find(configuration.key);
            // If the removed built-in configuration is a version older than the current built-in configuration, we are it back.
            if (iter == this->removed_built_in_configuration.end()) {
                this->available_configurations.push_back(configuration);
            } else if (iter->second < configuration.version) {
                this->available_configurations.push_back(configuration);
            }
        } else if (found_configuration->version < configuration.version) {
            // Replaced the old configuration by the new one
            this->RemoveConfiguration(found_configuration->key);
            this->available_configurations.push_back(configuration);
        }
    }
}

void ConfigurationManager::GatherConfigurationsParameters(const LayerManager &layers) {
    for (std::size_t i = 0, n = this->available_configurations.size(); i < n; ++i) {
        this->available_configurations[i].GatherParameters(layers);
    }
}

void ConfigurationManager::SortConfigurations() {
    struct Compare {
        bool operator()(const Configuration &a, const Configuration &b) const {
            const bool result = a.key < b.key;
            return result;
        }
    };

    std::sort(this->available_configurations.begin(), this->available_configurations.end(), Compare());
}

void ConfigurationManager::LoadConfigurationsPath(const LayerManager &layers) {
    const std::vector<Path> &configuration_files = ::CollectFilePaths(Path(Path::CONFIGS));
    for (std::size_t i = 0, n = configuration_files.size(); i < n; ++i) {
        Configuration configuration;
        const bool result = configuration.Load(configuration_files[i], layers);
        if (!result) {
            continue;
        }

        if (this->FindConfiguration(configuration.key) != nullptr) {
            continue;
        }

        /*
        std::string missing_layer;
        if (!HasMissingLayer(configuration.parameters, layers, missing_layer)) {
            //OrderParameter(configuration.parameters, layers);
        }
        */

        available_configurations.push_back(configuration);
    }
}

static Path MakeConfigurationPath(const std::string &key) { return RelativePath(Path::CONFIGS) + "/" + key + ".json"; }

void ConfigurationManager::SaveAllConfigurations() const {
    for (std::size_t i = 0, n = available_configurations.size(); i < n; ++i) {
        const Path &path(MakeConfigurationPath(available_configurations[i].key));
        available_configurations[i].Save(path);
    }
}

Configuration &ConfigurationManager::CreateConfiguration(const LayerManager &layers, const std::string &configuration_name) {
    std::string configuration_key = MakeConfigurationName(available_configurations, configuration_name);

    Configuration new_configuration = Configuration::Create(layers, configuration_key);

    this->available_configurations.push_back(new_configuration);
    this->SortConfigurations();

    return *this->FindConfiguration(new_configuration.key);
}

Configuration &ConfigurationManager::DuplicateConfiguration(const LayerManager &layers, const std::string &configuration_name) {
    Configuration *source_configuration = this->FindConfiguration(configuration_name);
    assert(source_configuration != nullptr);

    Configuration duplicated_configuration = *source_configuration;
    duplicated_configuration.key = MakeConfigurationName(available_configurations, configuration_name);

    const Path &path = MakeConfigurationPath(duplicated_configuration.key);
    duplicated_configuration.Save(path);

    // Reload from file to workaround the lack of SettingSet copy support
    Configuration reloaded_configuration;
    const bool result = reloaded_configuration.Load(path, layers);
    assert(result);

    this->available_configurations.push_back(reloaded_configuration);
    this->SortConfigurations();  // invalidated all pointers to configuration object

    return *this->FindConfiguration(reloaded_configuration.key);
}

bool ConfigurationManager::HasFile(const Configuration &configuration) const {
    const std::string base_path = ::AbsolutePath(Path::CONFIGS);

    const std::string path = base_path + "/" + configuration.key + ".json";

    std::FILE *file = std::fopen(path.c_str(), "r");
    if (file) {
        std::fclose(file);
        return true;
    }

    return false;
}

void ConfigurationManager::RemoveConfigurationFiles() {
    const std::vector<Path> &configuration_files = ::CollectFilePaths(Path(Path::CONFIGS));
    for (std::size_t i = 0, n = configuration_files.size(); i < n; ++i) {
        configuration_files[i].Remove();
    }
}

void ConfigurationManager::RemoveConfigurationFile(const std::string &key) {
    const std::vector<Path> &configuration_files = ::CollectFilePaths(Path(Path::CONFIGS));
    for (std::size_t j = 0, o = configuration_files.size(); j < o; ++j) {
        if (configuration_files[j].Basename() == key) {
            configuration_files[j].Remove();
        }
    }
}

void ConfigurationManager::RemoveConfiguration(const std::string &configuration_name) {
    assert(!configuration_name.empty());

    this->RemoveConfigurationFile(configuration_name.c_str());

    // Update the configuration in the list
    std::vector<Configuration> updated_configurations;

    const std::string key = ToLowerCase(std::string(configuration_name));

    for (std::size_t i = 0, n = this->available_configurations.size(); i < n; ++i) {
        if (ToLowerCase(this->available_configurations[i].key) == key) {
            int version = this->available_configurations[i].version;

            auto result = this->removed_built_in_configuration.insert(std::make_pair(configuration_name, version));
            if (!result.second) {
                this->removed_built_in_configuration[configuration_name] = version;
            }
            continue;
        }
        updated_configurations.push_back(this->available_configurations[i]);
    }

    std::swap(updated_configurations, this->available_configurations);
    this->SortConfigurations();
}

Configuration *ConfigurationManager::FindConfiguration(const std::string &configuration_name) {
    if (configuration_name.empty()) {
        return nullptr;
    }

    if (this->available_configurations.empty()) {
        return nullptr;
    }

    const std::string key = ToLowerCase(std::string(configuration_name));

    for (std::size_t i = 0, n = this->available_configurations.size(); i < n; ++i) {
        if (ToLowerCase(this->available_configurations[i].key) == key) {
            return &this->available_configurations[i];
        }
    }

    return nullptr;
}

const Configuration *ConfigurationManager::FindConfiguration(const std::string &configuration_name) const {
    return const_cast<ConfigurationManager *>(this)->FindConfiguration(configuration_name);
}

bool ConfigurationManager::IsDefaultConfiguration(const std::string &configuration_key) const {
    const Configuration *configuration = this->FindConfiguration(configuration_key);
    if (configuration == nullptr) {
        return false;
    }
    return configuration->IsDefault();
}

void ConfigurationManager::ResetDefaultConfigurations(const LayerManager &layers) {
    for (std::size_t i = 0, n = this->available_configurations.size(); i < n; ++i) {
        if (!this->available_configurations[i].IsDefault()) {
            continue;
        }

        this->available_configurations[i].Reset(layers);
    }
}

void ConfigurationManager::RenameConfiguration(const std::string &old_configuration_name,
                                               const std::string &new_configuration_name) {
    Configuration *configuration = this->FindConfiguration(old_configuration_name);
    this->RemoveConfigurationFile(old_configuration_name);
    configuration->key = new_configuration_name;

    this->SortConfigurations();
}

bool ConfigurationManager::ImportConfiguration(const LayerManager &layers, const Path &full_import_path,
                                               std::string &configuration_name) {
    assert(!full_import_path.Empty());

    this->last_path_import_config = full_import_path.AbsoluteDir();

    Configuration configuration;
    if (!configuration.Load(full_import_path, layers)) {
        return false;
    }

    configuration_name = configuration.key =
        MakeConfigurationName(this->available_configurations, configuration.key + " (Imported)");
    this->available_configurations.push_back(configuration);
    this->SortConfigurations();

    return true;
}

bool ConfigurationManager::ExportConfiguration(const LayerManager &layers, const Path &full_export_path,
                                               const std::string &configuration_name) {
    (void)layers;

    assert(!configuration_name.empty());
    assert(!full_export_path.Empty());

    this->last_path_export_config = full_export_path.AbsoluteDir();

    Configuration *configuration = this->FindConfiguration(configuration_name);
    assert(configuration);

    return configuration->Save(full_export_path);
}
