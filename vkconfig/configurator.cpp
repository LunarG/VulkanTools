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
 * - Lenny Komow <lenny@lunarg.com>
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configurator.h"
#include "vulkan.h"

#include "dialog_custom_paths.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/path.h"
#include "../vkconfig_core/override.h"

#include <Qt>
#include <QDir>
#include <QSettings>
#include <QLibrary>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>

#include <cassert>
#include <cstdio>
#include <algorithm>

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() : environment(path), layers(environment), request_vulkan_status(true) { CopyResourceFiles(); }

Configurator::~Configurator() {
    for (std::size_t i = 0, n = available_configurations.size(); i < n; ++i) {
        available_configurations[i].Save(layers.available_layers,
                                         path.GetFullPath(PATH_CONFIGURATION, available_configurations[i].key));
    }

    if (!environment.UsePersistentOverrideMode()) {
        SurrenderLayers(environment);
    }
}

void Configurator::CopyResourceFiles() {
    const QString dir_save(QString(GetPath(BUILTIN_PATH_VULKAN_LAYER_CONFIG).c_str()) + "/VK_LAYER_LUNARG_device_simulation");
    CheckPathsExist(dir_save.toStdString());

    const QFileInfoList &devsim_files = GetJSONFiles(":/resourcefiles/devsim/");
    for (int i = 0, n = devsim_files.size(); i < n; ++i) {
        const QString filename_exist(dir_save + "/" + devsim_files[i].fileName());
        QFile file_exist(filename_exist);
        const bool opened_exist = file_exist.open(QIODevice::ReadOnly | QIODevice::Text);
        if (opened_exist) {
            file_exist.close();
            continue;
        }

        const QString filename_load(devsim_files[i].absoluteFilePath());
        QFile file_load(filename_load);
        const bool opened_load = file_load.open(QIODevice::ReadOnly | QIODevice::Text);
        assert(opened_load);
        QString json_text = file_load.readAll();
        file_load.close();

        const QString filename_save(dir_save + "/" + devsim_files[i].fileName());
        QFile file_save(filename_save);
        const bool opened_save = file_save.open(QIODevice::WriteOnly | QIODevice::Text);
        assert(opened_save);
        file_save.write(json_text.toStdString().c_str(), json_text.size());
        file_save.close();
    }
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    layers.LoadAllInstalledLayers();

    const bool has_layers = !layers.Empty();

    // If no layers are found, give the user another chance to add some custom paths
    if (!has_layers) {
        QMessageBox alert;
        alert.setWindowTitle("No Vulkan Layers found");
        alert.setText(
            "No Vulkan Layers were found in standard paths or in the SDK path. Vulkan Layers are required in order to use Vulkan "
            "Configurator.");
        alert.setInformativeText("Please select the path where you have your layers located.");
        alert.setIcon(QMessageBox::Warning);
        alert.exec();

        CustomPathsDialog dlg;
        dlg.exec();

        // Give it one more chance... If there are still no layers, bail
        layers.LoadAllInstalledLayers();
    }

    if (!has_layers) {
        QMessageBox alert;
        alert.setWindowTitle(VKCONFIG_NAME);
        alert.setText("Could not initialize Vulkan Configurator.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        return false;
    }

    LoadAllConfigurations();

    if (available_configurations.empty()) {
        QMessageBox alert;
        alert.setWindowTitle("Vulkan Configurator couldn't find any layers configuration.");
        alert.setText(
            "A layers configuration is required to override Vulkan layers but none could be found during the initialization...");
        alert.setInformativeText("Restoring the default layers configurations.");
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Warning);
        if (alert.exec() == QMessageBox::Ok) {
            ResetDefaultsConfigurations();
        }
    }

    if (HasActiveConfiguration()) {
        if (HasMissingParameter(_active_configuration->parameters, layers.available_layers)) {
            QSettings settings;
            if (settings.value("VKCONFIG_WARN_MISSING_LAYERS_IGNORE").toBool() == false) {
                QMessageBox alert;
                alert.setWindowTitle("Vulkan Configurator couldn't find some Vulkan layers...");
                alert.setText(format("%s is missing layers", _active_configuration->key.toUtf8().constData()).c_str());
                alert.setInformativeText("Do you want to add a custom path to find the layers?");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setDefaultButton(QMessageBox::Yes);
                alert.setIcon(QMessageBox::Warning);
                alert.setCheckBox(new QCheckBox("Do not show again."));
                if (alert.exec() == QMessageBox::Yes) {
                    CustomPathsDialog dlg;
                    dlg.exec();

                    layers.LoadAllInstalledLayers();
                }
                if (alert.checkBox()->isChecked()) {
                    settings.setValue("VKCONFIG_WARN_MISSING_LAYERS_IGNORE", true);
                }
            }
        }
    }

    return true;
}

bool Configurator::HasLayers() const { return !layers.Empty(); }

bool Configurator::SupportApplicationList(Version *return_loader_version) const {
    // Check loader version
    const Version version = GetVulkanLoaderVersion();
    assert(version != Version::VERSION_NULL);

    if (return_loader_version) {
        *return_loader_version = version;
    }

    return version >= Version("1.2.141");
}

// Populate a tree widget with the custom layer paths and the layers that
// are being used in them.
void Configurator::BuildCustomLayerTree(QTreeWidget *tree_widget) {
    // Populate the tree
    tree_widget->clear();

    // Building the list is not obvious. Each custom path may have multiple layers and there
    // could be duplicates, which are not allowed. The layer paths are traversed in order, and
    // layers are used on a first occurance basis. So we can't just show the layers that are
    // present in the folder (because they may not be used). We have to list the custom layer paths
    // and then look for layers that are already loaded that are from that path.
    const QStringList &custom_layer_paths = environment.GetCustomLayerPaths();
    for (int custom_path_index = 0, n = custom_layer_paths.size(); custom_path_index < n; ++custom_path_index) {
        // Custom path is the parent tree item
        const QString custom_path(ConvertNativeSeparators(custom_layer_paths[custom_path_index].toStdString()).c_str());

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path);
        tree_widget->addTopLevelItem(item);

        // Look for layers that are loaded that are also from this folder
        for (std::size_t i = 0, n = layers.available_layers.size(); i < n; i++) {
            const Layer &layer = layers.available_layers[i];

            const QFileInfo file_info(layer._layer_path);
            const QString path(ConvertNativeSeparators(file_info.path().toStdString()).c_str());
            if (path != custom_path) continue;

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, layer.key.c_str());
            item->addChild(child);
        }
        item->setExpanded(true);
    }
}

/// Load all the configurations. If the built-in configurations don't exist,
/// they are created from the embedded json files
void Configurator::LoadAllConfigurations() {
    available_configurations.clear();
    _active_configuration = available_configurations.end();

    // If this is the first time, we need to create the initial set of
    // configuration files.
    if (environment.first_run) {
        RemoveConfigurationFiles();

        const QFileInfoList &configuration_files = GetJSONFiles(":/resourcefiles/configurations/");

        for (int i = 0, n = configuration_files.size(); i < n; ++i) {
            Configuration configuration;
            const bool result = configuration.Load(layers.available_layers, configuration_files[i].absoluteFilePath());

            if (!configuration.IsAvailableOnThisPlatform()) continue;

            OrderParameter(configuration.parameters, layers.available_layers);
            if (result) {
                const bool result =
                    configuration.Save(layers.available_layers, path.GetFullPath(PATH_CONFIGURATION, configuration.key));
                assert(result);
            }
        }

        environment.first_run = false;
    }

    const QFileInfoList &configuration_files = GetJSONFiles(path.GetPath(PATH_CONFIGURATION).c_str());
    for (int i = 0, n = configuration_files.size(); i < n; ++i) {
        const QFileInfo &info = configuration_files.at(i);

        Configuration configuration;
        const bool result = configuration.Load(layers.available_layers, info.absoluteFilePath());
        OrderParameter(configuration.parameters, layers.available_layers);
        if (result) {
            available_configurations.push_back(configuration);
        }
    }

    RefreshConfiguration();
}

void Configurator::RemoveConfigurationFiles() {
    const QFileInfoList &configuration_files = GetJSONFiles(path.GetPath(PATH_CONFIGURATION).c_str());

    for (int i = 0, n = configuration_files.size(); i < n; i++) {
        remove(configuration_files[i].filePath().toStdString().c_str());
    }
}

void Configurator::RemoveConfiguration(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    // Not the active configuration
    if (GetActiveConfiguration()->key == configuration_name) {
        SetActiveConfiguration(available_configurations.end());
    }

    // Delete the configuration file
    const QString full_path(path.GetFullPath(PATH_CONFIGURATION, configuration_name));
    const bool result = std::remove(full_path.toStdString().c_str()) == 0;
    assert(result);

    // Reload to remove the configuration in the UI
    LoadAllConfigurations();
}

void Configurator::SetActiveConfiguration(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    auto configuration = FindItByKey(available_configurations, configuration_name.toStdString().c_str());
    assert(configuration != available_configurations.end());

    SetActiveConfiguration(configuration);
}

void Configurator::SetActiveConfiguration(std::vector<Configuration>::iterator active_configuration) {
    _active_configuration = active_configuration;

    bool surrender = false;
    if (_active_configuration != available_configurations.end()) {
        assert(!_active_configuration->key.isEmpty());
        environment.Set(ACTIVE_CONFIGURATION, _active_configuration->key);
        surrender = _active_configuration->IsEmpty();
    } else {
        _active_configuration = available_configurations.end();
        surrender = true;
    }

    if (surrender) {
        SurrenderLayers(environment);
    } else {
        assert(_active_configuration != available_configurations.end());
        OverrideLayers(environment, layers.available_layers, *active_configuration);
    }
}

void Configurator::RefreshConfiguration() {
    const QString active_configuration_name = environment.Get(ACTIVE_CONFIGURATION);

    if (!active_configuration_name.isEmpty() && environment.UseOverride()) {
        auto active_configuration = FindItByKey(available_configurations, active_configuration_name.toStdString().c_str());
        if (active_configuration == available_configurations.end()) {
            environment.Set(ACTIVE_CONFIGURATION, "");
        }
        SetActiveConfiguration(active_configuration);
    } else {
        SetActiveConfiguration(available_configurations.end());
    }
}

bool Configurator::HasActiveConfiguration() const {
    if (_active_configuration != available_configurations.end())
        return !HasMissingParameter(_active_configuration->parameters, layers.available_layers) &&
               !_active_configuration->IsEmpty();
    else
        return false;
}

void Configurator::ImportConfiguration(const QString &full_import_path) {
    assert(!full_import_path.isEmpty());

    Configuration configuration;

    if (!configuration.Load(layers.available_layers, full_import_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(full_import_path);
        msg.exec();
        return;
    }

    configuration.key += " (Imported)";

    if (!configuration.Save(layers.available_layers, path.GetFullPath(PATH_CONFIGURATION, configuration.key))) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(format("%s.json", configuration.key.toUtf8().constData()).c_str());
        msg.exec();
        return;
    }

    LoadAllConfigurations();
}

void Configurator::ExportConfiguration(const QString &full_export_path, const QString &ConfigurationName) {
    assert(!ConfigurationName.isEmpty());
    assert(!full_export_path.isEmpty());

    Configuration *configuration = FindByKey(available_configurations, ConfigurationName.toStdString().c_str());
    assert(configuration);

    if (!configuration->Save(layers.available_layers, full_export_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(full_export_path);
        msg.exec();
    }
}

void Configurator::ResetDefaultsConfigurations() {
    // Clear the current configuration as we may be about to remove it.
    SetActiveConfiguration(available_configurations.end());

    environment.Reset(Environment::DEFAULT);

    // Now we need to kind of restart everything
    LoadAllConfigurations();
}
