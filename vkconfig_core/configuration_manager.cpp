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

#include "util.h"
#include "configuration_manager.h"
#include "configurator.h"
#include "alert.h"

ConfigurationManager::ConfigurationManager() {}

ConfigurationManager::~ConfigurationManager() {}

void ConfigurationManager::LoadAllConfigurations(const std::vector<Layer> &available_layers) {
    this->available_configurations.clear();

    this->LoadConfigurationsPath(available_layers);

    // If built-in configurations were updated, replace the stored configuration
    this->LoadDefaultConfigurations(available_layers);

    this->SortConfigurations();
}

void ConfigurationManager::LoadDefaultConfigurations(const std::vector<Layer> &available_layers) {
    const std::vector<Path> &configuration_files = CollectFilePaths(":/configurations/");

    for (int i = 0, n = configuration_files.size(); i < n; ++i) {
        Configuration configuration;
        const bool result = configuration.Load(configuration_files[i], available_layers);
        assert(result);

        if (!IsPlatformSupported(configuration.platform_flags)) {
            continue;
        }

#ifdef _M_ARM64
        if (configuration.key == "Frame Capture") {
            continue;
        } else if (configuration.key == "Crash Diagnostic") {
            continue;
        }
#endif

        OrderParameter(configuration.parameters, available_layers);

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
            this->RemoveConfiguration(available_layers, found_configuration->key);
            this->available_configurations.push_back(configuration);
        }
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

void ConfigurationManager::LoadConfigurationsPath(const std::vector<Layer> &available_layers) {
    const std::vector<Path> &configuration_files = CollectFilePaths(Get(Path::CONFIGS));
    for (int i = 0, n = configuration_files.size(); i < n; ++i) {
        Configuration configuration;
        const bool result = configuration.Load(configuration_files[i], available_layers);
        if (!result) {
            continue;
        }

        if (FindByKey(available_configurations, configuration.key.c_str()) != nullptr) {
            continue;
        }

        std::string missing_layer;
        if (!HasMissingLayer(configuration.parameters, available_layers, missing_layer)) {
            OrderParameter(configuration.parameters, available_layers);
        }

        available_configurations.push_back(configuration);
    }
}

static Path MakeConfigurationPath(const std::string &key) {
    const Path &path = Get(Path::CONFIGS) + "/" + key + ".json";
    return path.AbsolutePath();
}

void ConfigurationManager::SaveAllConfigurations() {
    for (std::size_t i = 0, n = available_configurations.size(); i < n; ++i) {
        const Path &path(MakeConfigurationPath(available_configurations[i].key));
        available_configurations[i].Save(path);
    }
}

Configuration &ConfigurationManager::CreateConfiguration(const std::vector<Layer> &available_layers,
                                                         const std::string &configuration_name, bool duplicate) {
    Configuration *duplicate_configuration = FindByKey(available_configurations, configuration_name.c_str());

    Configuration new_configuration = duplicate_configuration != nullptr && duplicate ? *duplicate_configuration : Configuration();
    new_configuration.key = MakeConfigurationName(available_configurations, configuration_name);

    const Path &path(MakeConfigurationPath(new_configuration.key));
    new_configuration.Save(path);

    // Reload from file to workaround the lack of SettingSet copy support
    Configuration configuration;
    const bool result = configuration.Load(path, available_layers);
    assert(result);

    this->available_configurations.push_back(configuration);
    this->SortConfigurations();

    return *FindByKey(this->available_configurations, configuration.key.c_str());
}

bool ConfigurationManager::HasFile(const Configuration &configuration) const {
    const std::string base_path = AbsolutePath(Path::CONFIGS);

    const std::string path = base_path + "/" + configuration.key + ".json";

    std::FILE *file = std::fopen(path.c_str(), "r");
    if (file) {
        std::fclose(file);
        return true;
    }

    return false;
}

void ConfigurationManager::RemoveConfigurationFiles() {
    const std::vector<Path> &configuration_files = CollectFilePaths(Get(Path::CONFIGS));
    for (int i = 0, n = configuration_files.size(); i < n; ++i) {
        configuration_files[i].Remove();
    }
}

void ConfigurationManager::RemoveConfigurationFile(const std::string &key) {
    const std::vector<Path> &configuration_files = CollectFilePaths(Get(Path::CONFIGS));
    for (int j = 0, o = configuration_files.size(); j < o; ++j) {
        if (configuration_files[j].Basename() == key) {
            configuration_files[j].Remove();
        }
    }
}

void ConfigurationManager::RemoveConfiguration(const std::vector<Layer> &available_layers, const std::string &configuration_name) {
    assert(!configuration_name.empty());

    RemoveConfigurationFile(configuration_name.c_str());

    // Update the configuration in the list
    std::vector<Configuration> updated_configurations;

    for (std::size_t i = 0, n = this->available_configurations.size(); i < n; ++i) {
        if (this->available_configurations[i].key == configuration_name) {
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

    return FindByKey(this->available_configurations, configuration_name.c_str());
}

const Configuration *ConfigurationManager::FindConfiguration(const std::string &configuration_name) const {
    if (configuration_name.empty()) {
        return nullptr;
    }

    if (this->available_configurations.empty()) {
        return nullptr;
    }

    return FindByKey(this->available_configurations, configuration_name.c_str());
}

std::string ConfigurationManager::ImportConfiguration(const std::vector<Layer> &available_layers, const Path &full_import_path) {
    assert(!full_import_path.Empty());

    Configuration configuration;
    if (!configuration.Load(full_import_path, available_layers)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(full_import_path.AbsolutePath().c_str());
        msg.exec();
        return "";
    }

    configuration.key = MakeConfigurationName(this->available_configurations, configuration.key + " (Imported)");
    this->available_configurations.push_back(configuration);
    this->SortConfigurations();

    return configuration.key;
}

void ConfigurationManager::ExportConfiguration(const std::vector<Layer> &available_layers, const Path &full_export_path,
                                               const std::string &configuration_name) {
    assert(!configuration_name.empty());
    assert(!full_export_path.Empty());

    Configuration *configuration = FindByKey(available_configurations, configuration_name.c_str());
    assert(configuration);

    if (!configuration->Save(full_export_path, true)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(full_export_path.AbsolutePath().c_str());
        msg.exec();
    }
}

void ConfigurationManager::Reset(const std::vector<Layer> &available_layers) {
    // Now we need to kind of restart everything
    this->LoadDefaultConfigurations(available_layers);

    this->SortConfigurations();
}

bool ConfigurationManager::CheckApiVersions(const std::vector<Layer> &available_layers, Configuration *selected_configuration,
                                            std::string &log_versions) const {
    return this->CompareLayersVersions(available_layers, selected_configuration, Version::VKCONFIG, log_versions, true);
}

bool ConfigurationManager::CheckLayersVersions(const std::vector<Layer> &available_layers, Configuration *selected_configuration,
                                               std::string &log_versions) const {
    return this->CompareLayersVersions(available_layers, selected_configuration, Version::VERSION_NULL, log_versions, false);
}

bool ConfigurationManager::CompareLayersVersions(const std::vector<Layer> &available_layers, Configuration *selected_configuration,
                                                 const Version &version, std::string &log_versions, bool is_less) const {
    assert(selected_configuration != nullptr);

    Version current_version = version;

    bool result = true;

    for (std::size_t param_index = 0, param_count = selected_configuration->parameters.size(); param_index < param_count;
         ++param_index) {
        const Parameter &parameter = selected_configuration->parameters[param_index];

        if (parameter.control != LAYER_CONTROL_ON) {
            continue;
        }

        for (std::size_t layer_index = 0, layer_count = available_layers.size(); layer_index < layer_count; ++layer_index) {
            const Layer &layer = available_layers[layer_index];

            if (layer.key == parameter.key) {
                if (current_version == Version::VERSION_NULL) {
                    current_version = layer.api_version;
                }

                if (is_less) {
                    if (layer.api_version.GetMinor() > version.GetMinor()) result = false;
                } else {
                    if (layer.api_version.GetMinor() != current_version.GetMinor()) result = false;
                }

                log_versions += format("%s - %s\n", layer.key.c_str(), layer.api_version.str().c_str());
            }
        }
    }

    return result;
}
