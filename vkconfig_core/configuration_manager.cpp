/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
#include "path_manager.h"
#include "configuration_manager.h"
#include "override.h"

#include <QMessageBox>
#include <QFileInfoList>

ConfigurationManager::ConfigurationManager() {}

ConfigurationManager::~ConfigurationManager() {}

void ConfigurationManager::LoadAllConfigurations(const std::vector<Layer> &available_layers, const PathManager &path_manager,
                                                 Environment &environment) {
    available_configurations.clear();
    active_configuration = available_configurations.end();

    // If this is the first time, we need to create the initial set of
    // configuration files.
    if (environment.first_run) {
        RemoveConfigurationFiles(path_manager);

        const QFileInfoList &configuration_files = GetJSONFiles(":/resourcefiles/configurations/");

        for (int i = 0, n = configuration_files.size(); i < n; ++i) {
            Configuration configuration;
            const bool result = configuration.Load(available_layers, configuration_files[i].absoluteFilePath().toStdString());

            if (!configuration.IsAvailableOnThisPlatform()) continue;

            OrderParameter(configuration.parameters, available_layers);
            if (result) {
                const std::string &path = path_manager.GetFullPath(PATH_CONFIGURATION, configuration.key.c_str());
                const bool result = configuration.Save(available_layers, path);
                assert(result);
            }
        }

        environment.first_run = false;
    }

    LoadConfigurationsPath(available_layers, path_manager, PATH_CONFIGURATION);
    if (SUPPORT_VKCONFIG_2_0_3) LoadConfigurationsPath(available_layers, path_manager, PATH_CONFIGURATION_LEGACY);

    RefreshConfiguration(available_layers, environment);
}

static bool IsConfigurationExcluded(const char *filename) {
    static const char *EXCLUDED_FILENAMES[] = {"Validation - Synchronization (Alpha).json",
                                               "Validation - Standard.json",
                                               "Validation - Reduced-Overhead.json",
                                               "Validation - GPU-Assisted.json",
                                               "Validation - Debug Printf.json",
                                               "Validation - Shader Printf.json",
                                               "Validation - Best Practices.json",
                                               "Frame Capture - Range (F5 to start and to stop).json",
                                               "Frame Capture - Range (F10 to start and to stop).json",
                                               "Frame Capture - First two frames.json",
                                               "applist.json"};

    for (std::size_t i = 0, n = countof(EXCLUDED_FILENAMES); i < n; ++i) {
        if (std::strcmp(EXCLUDED_FILENAMES[i], filename) == 0) return true;
    }

    return false;
}

void ConfigurationManager::LoadConfigurationsPath(const std::vector<Layer> &available_layers, const PathManager &path_manager,
                                                  PathType path_type) {
    const QFileInfoList &configuration_files = GetJSONFiles(path_manager.GetPath(path_type).c_str());
    for (int i = 0, n = configuration_files.size(); i < n; ++i) {
        const QFileInfo &info = configuration_files[i];
        if (SUPPORT_VKCONFIG_2_0_3 && IsConfigurationExcluded(info.fileName().toStdString().c_str())) continue;

        Configuration configuration;
        const bool result = configuration.Load(available_layers, info.absoluteFilePath().toStdString());

        if (FindByKey(available_configurations, configuration.key.c_str()) != nullptr) continue;

        OrderParameter(configuration.parameters, available_layers);
        if (result) {
            available_configurations.push_back(configuration);
        }
    }
}

void ConfigurationManager::SaveAllConfigurations(const std::vector<Layer> &available_layers, const PathManager &path_manager) {
    for (std::size_t i = 0, n = available_configurations.size(); i < n; ++i) {
        const std::string path = path_manager.GetFullPath(PATH_CONFIGURATION, available_configurations[i].key.c_str());
        available_configurations[i].Save(available_layers, path.c_str());
    }
}

void ConfigurationManager::RemoveConfigurationFiles(const PathManager &path_manager) {
    const QFileInfoList &configuration_files = GetJSONFiles(path_manager.GetPath(PATH_CONFIGURATION).c_str());

    for (int i = 0, n = configuration_files.size(); i < n; i++) {
        remove(configuration_files[i].filePath().toStdString().c_str());
    }
}

void ConfigurationManager::RemoveConfiguration(const std::vector<Layer> &available_layers, const PathManager &path_manager,
                                               Environment &environment, const std::string &configuration_name) {
    assert(!configuration_name.empty());

    // Not the active configuration
    if (GetActiveConfiguration()->key == configuration_name.c_str()) {
        SetActiveConfiguration(available_layers, environment, available_configurations.end());
    }

    // Delete the configuration file
    const std::string full_path(path_manager.GetFullPath(PATH_CONFIGURATION, configuration_name.c_str()));
    const bool result = std::remove(full_path.c_str()) == 0;
    assert(result);

    // Reload to remove the configuration in the UI
    LoadAllConfigurations(available_layers, path_manager, environment);
}

void ConfigurationManager::SetActiveConfiguration(const std::vector<Layer> &available_layers, Environment &environment,
                                                  const std::string &configuration_name) {
    assert(!configuration_name.empty());

    auto configuration = FindItByKey(available_configurations, configuration_name.c_str());
    assert(configuration != available_configurations.end());

    SetActiveConfiguration(available_layers, environment, configuration);
}

void ConfigurationManager::SetActiveConfiguration(const std::vector<Layer> &available_layers, Environment &environment,
                                                  std::vector<Configuration>::iterator active_configuration) {
    this->active_configuration = active_configuration;

    bool surrender = false;
    if (this->active_configuration != available_configurations.end()) {
        assert(!this->active_configuration->key.empty());
        environment.Set(ACTIVE_CONFIGURATION, this->active_configuration->key.c_str());
        surrender = this->active_configuration->IsEmpty();
    } else {
        this->active_configuration = available_configurations.end();
        surrender = true;
    }

    if (surrender) {
        SurrenderLayers(environment);
    } else {
        assert(this->active_configuration != available_configurations.end());
        OverrideLayers(environment, available_layers, *active_configuration);
    }
}

void ConfigurationManager::RefreshConfiguration(const std::vector<Layer> &available_layers, Environment &environment) {
    const std::string active_configuration_name = environment.Get(ACTIVE_CONFIGURATION);

    if (!active_configuration_name.empty() && environment.UseOverride()) {
        auto active_configuration = FindItByKey(available_configurations, active_configuration_name.c_str());
        if (active_configuration == available_configurations.end()) {
            environment.Set(ACTIVE_CONFIGURATION, "");
        }
        SetActiveConfiguration(available_layers, environment, active_configuration);
    } else {
        SetActiveConfiguration(available_layers, environment, available_configurations.end());
    }
}

bool ConfigurationManager::HasActiveConfiguration(const std::vector<Layer> &available_layers) const {
    if (this->active_configuration != available_configurations.end())
        return !HasMissingLayer(this->active_configuration->parameters, available_layers) && !this->active_configuration->IsEmpty();
    else
        return false;
}

void ConfigurationManager::ImportConfiguration(const std::vector<Layer> &available_layers, const PathManager &path_manager,
                                               Environment &environment, const std::string &full_import_path) {
    assert(!full_import_path.empty());

    Configuration configuration;

    if (!configuration.Load(available_layers, full_import_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(full_import_path.c_str());
        msg.exec();
        return;
    }

    configuration.key += " (Imported)";

    if (!configuration.Save(available_layers, path_manager.GetFullPath(PATH_CONFIGURATION, configuration.key.c_str()))) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(format("%s.json", configuration.key.c_str()).c_str());
        msg.exec();
        return;
    }

    LoadAllConfigurations(available_layers, path_manager, environment);
}

void ConfigurationManager::ExportConfiguration(const std::vector<Layer> &available_layers, const std::string &full_export_path,
                                               const std::string &configuration_name) {
    assert(!configuration_name.empty());
    assert(!full_export_path.empty());

    Configuration *configuration = FindByKey(available_configurations, configuration_name.c_str());
    assert(configuration);

    if (!configuration->Save(available_layers, full_export_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(full_export_path.c_str());
        msg.exec();
    }
}

void ConfigurationManager::ResetDefaultsConfigurations(const std::vector<Layer> &available_layers, const PathManager &path_manager,
                                                       Environment &environment) {
    // Clear the current configuration as we may be about to remove it.
    SetActiveConfiguration(available_layers, environment, available_configurations.end());

    environment.Reset(Environment::DEFAULT);

    // Now we need to kind of restart everything
    LoadAllConfigurations(available_layers, path_manager, environment);
}
