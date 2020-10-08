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

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/override.h"

#include <Qt>
#include <QDir>
#include <QSettings>
#include <QLibrary>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonArray>

#include <cassert>
#include <cstdio>
#include <algorithm>

struct DefaultConfiguration {
    const char *name;
    const char *file;
    const char *required_layer;
};

static const DefaultConfiguration default_configurations[] = {
    {"Validation", ":/resourcefiles/configurations/validation.json", "VK_LAYER_KHRONOS_validation"},
    {"Frames Capture", ":/resourcefiles/configurations/frames_capture.json", "VK_LAYER_LUNARG_gfxreconstruct"},
    {"API dump", ":/resourcefiles/configurations/api_dump.json", "VK_LAYER_LUNARG_api_dump"}};

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() : environment(path), layers(environment) {}

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
        alert.setInformativeText("Do you want to restore the default layers configurations?");
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);
        alert.setIcon(QMessageBox::Warning);
        if (alert.exec() == QMessageBox::Yes) {
            ResetDefaultsConfigurations();
        }
    }

    // This will reset or clear the current profile if the files have been
    // manually manipulated
    SetActiveConfiguration(_active_configuration);

    if (_active_configuration != available_configurations.end()) {
        if (HasMissingParameter(_active_configuration->parameters, layers.available_layers)) {
            QSettings settings;
            if (settings.value("VKCONFIG_WARN_MISSING_LAYERS_IGNORE").toBool() == false) {
                QMessageBox alert;
                alert.setWindowTitle("Vulkan Configurator couldn't find some Vulkan layers...");
                alert.setText(format("%s is missing layers", _active_configuration->name.toUtf8().constData()).c_str());
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

Configurator::~Configurator() {
    for (std::size_t i = 0, n = available_configurations.size(); i < n; ++i) {
        available_configurations[i].Save(path.GetFullPath(PATH_CONFIGURATION, available_configurations[i].name));
    }

    layers.Clear();
    available_configurations.clear();
}

bool Configurator::HasLayers() const { return !layers.Empty(); }

bool Configurator::SupportApplicationList(bool quiet, Version *return_loader_version) const {
    // Check loader version
    const Version version = GetVulkanLoaderVersion();
    assert(version != Version::VERSION_NULL);

    if (return_loader_version) {
        *return_loader_version = version;
    }

    // This is the minimum version that supports the application list
    if (version < Version("1.2.141") && !quiet) {
        const QString message = QString().asprintf(
            "The detected Vulkan Loader version is %s but version 1.2.141 or newer is required in order to apply layers "
            "override to only a selected list of Vulkan applications.\n\n<br><br>"
            "Get the latest Vulkan Runtime from <a href='https://vulkan.lunarg.com/sdk/home'>HERE.</a> to use this feature.",
            version.str().c_str());

        QMessageBox alert(NULL);
        alert.setWindowTitle("Layers override of a selected list of Vulkan Applications is not available");
        alert.setText(message);
        alert.setTextFormat(Qt::RichText);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    }

    return true;
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
        const QString &custom_path = QDir::toNativeSeparators(custom_layer_paths[custom_path_index]);

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, custom_path);
        tree_widget->addTopLevelItem(item);

        // Look for layers that are loaded that are also from this folder
        for (std::size_t i = 0, n = layers.available_layers.size(); i < n; i++) {
            const Layer &layer = layers.available_layers[i];

            QFileInfo fileInfo = layer._layer_path;
            QString path = QDir::toNativeSeparators(fileInfo.path());
            if (path != custom_path) continue;

            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, layer.name);
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
        // Delete all the *.json files in the storage folder
        QDir dir(path.GetPath(PATH_CONFIGURATION));
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setNameFilters(QStringList() << "*.json");
        QFileInfoList configuration_files = dir.entryInfoList();

        // Loop through all the configurations found and remove them
        for (int i = 0, n = configuration_files.size(); i < n; i++) {
            QFileInfo info = configuration_files.at(i);
            if (info.absoluteFilePath().contains("applist.json")) continue;
            remove(info.filePath().toUtf8().constData());
        }

        for (std::size_t i = 0, n = countof(default_configurations); i < n; ++i) {
            // Search the list of loaded configurations
            const QString file = default_configurations[i].file;

            Configuration configuration;
            const bool result = configuration.Load(file);
            if (result) {
                const bool result = configuration.Save(path.GetFullPath(PATH_CONFIGURATION, configuration.name));
                assert(result);
            }
        }

        environment.first_run = false;
    }

    // Get a list of all files that end in .json in the folder where
    // we store them. TBD... don't hard code this here.
    QDir dir(path.GetPath(PATH_CONFIGURATION));
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList configuration_files = dir.entryInfoList();

    // Loop through all the configurations found and load them
    for (int i = 0, n = configuration_files.size(); i < n; i++) {
        QFileInfo info = configuration_files.at(i);
        if (info.absoluteFilePath().contains("applist.json")) continue;

        Configuration configuration;
        const bool result = configuration.Load(info.absoluteFilePath());
        if (result) {
            available_configurations.push_back(configuration);
        }
    }

    // Cache the active configuration
    const QString &active_configuration_name = environment.Get(ACTIVE_CONFIGURATION);
    if (!active_configuration_name.isEmpty()) {
        _active_configuration = Find(available_configurations, active_configuration_name);
        if (_active_configuration == available_configurations.end()) {
            environment.Set(ACTIVE_CONFIGURATION, "");  // The configuration no longer exist
        }
    } else {
        _active_configuration = available_configurations.end();
    }
}

void Configurator::SetActiveConfiguration(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    auto configuration = Find(available_configurations, configuration_name);
    assert(configuration != available_configurations.end());

    SetActiveConfiguration(configuration);
}

// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void Configurator::SetActiveConfiguration(std::vector<Configuration>::iterator active_configuration) {
    _active_configuration = active_configuration;

    bool surrender = false;
    if (_active_configuration != available_configurations.end()) {
        assert(!_active_configuration->name.isEmpty());
        environment.Set(ACTIVE_CONFIGURATION, _active_configuration->name);
        surrender = _active_configuration->IsEmpty();
    } else {
        environment.Set(ACTIVE_CONFIGURATION, "");
        surrender = true;
    }

    if (surrender) {
        SurrenderLayers(environment);
    } else {
        OverrideLayers(environment, layers.available_layers, *active_configuration);
    }
}

void Configurator::RefreshConfiguration() {
    if (_active_configuration != available_configurations.end()) SetActiveConfiguration(_active_configuration);
}

bool Configurator::HasActiveConfiguration() const {
    return _active_configuration != available_configurations.end()
               ? !HasMissingParameter(_active_configuration->parameters, layers.available_layers)
               : false;
}

void Configurator::ImportConfiguration(const QString &full_import_path) {
    assert(!full_import_path.isEmpty());

    Configuration configuration;

    if (!configuration.Load(full_import_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(full_import_path);
        msg.exec();
        return;
    }

    configuration.name += " (Imported)";

    if (!configuration.Save(path.GetFullPath(PATH_CONFIGURATION, configuration.name))) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Import of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(format("%s.json", configuration.name.toUtf8().constData()).c_str());
        msg.exec();
        return;
    }

    LoadAllConfigurations();
}

void Configurator::ExportConfiguration(const QString &source_file, const QString &full_export_path) {
    assert(!source_file.isEmpty());
    assert(!full_export_path.isEmpty());

    Configuration configuration;

    const QString source_path = path.GetFullPath(PATH_CONFIGURATION, source_file);

    if (!configuration.Load(source_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot access the source configuration file.");
        msg.setInformativeText(source_path);
        msg.exec();
        return;
    }

    if (!configuration.Save(full_export_path)) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Export of Layers Configuration error");
        msg.setText("Cannot create the destination configuration file.");
        msg.setInformativeText(full_export_path);
        msg.exec();
        return;
    }
}

void Configurator::ResetDefaultsConfigurations() {
    // Clear the current configuration as we may be about to remove it.
    SetActiveConfiguration(available_configurations.end());

    environment.Reset(Environment::DEFAULT);

    // Delete all the *.json files in the storage folder
    QDir dir(path.GetPath(PATH_CONFIGURATION));
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList configuration_files = dir.entryInfoList();

    // Loop through all the profiles found and remove them
    for (int i = 0; i < configuration_files.size(); i++) {
        QFileInfo info = configuration_files.at(i);
        if (info.absoluteFilePath().contains("applist.json")) continue;
        remove(info.filePath().toUtf8().constData());
    }

    // Now we need to kind of restart everything
    LoadAllConfigurations();

    // Find the "Validation - Standard" configuration and make it current if we are active
    auto active_configuration = Find(available_configurations, environment.Get(ACTIVE_CONFIGURATION));
    if (environment.UseOverride()) {
        SetActiveConfiguration(active_configuration);
    }
}
