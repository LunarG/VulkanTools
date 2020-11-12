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
    const char *required_layer;
    Version required_api_version;
    const char *preset_label;
    ValidationPreset preset;
};

static const DefaultConfiguration default_configurations[] = {
    {"Validation - Standard", "VK_LAYER_KHRONOS_validation", Version("1.0.0"), "Standard", ValidationPresetStandard},
    {"Validation - Reduced-Overhead", "VK_LAYER_KHRONOS_validation", Version("1.0.0"), "Reduced-Overhead",
     ValidationPresetReducedOverhead},
    {"Validation - Best Practices", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Best Practices",
     ValidationPresetBestPractices},
    {"Validation - Synchronization (Alpha)", "VK_LAYER_KHRONOS_validation", Version("1.2.147"), "Synchronization (Alpha)",
     ValidationPresetSynchronization},
#if VKC_PLATFORM != VKC_PLATFORM_MACOS
    {"Validation - GPU-Assisted", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "GPU-Assisted", ValidationPresetGPUAssisted},
    {"Validation - Shader Printf", "VK_LAYER_KHRONOS_validation", Version("1.1.126"), "Debug Printf", ValidationPresetDebugPrintf},
    {"Frame Capture - First two frames", "VK_LAYER_LUNARG_gfxreconstruct", Version("1.2.147"), "", ValidationPresetNone},
    {"Frame Capture - Range (F5 to start and to stop)", "VK_LAYER_LUNARG_gfxreconstruct", Version("1.2.147"), "",
     ValidationPresetNone},
#endif
    {"API dump", "VK_LAYER_LUNARG_api_dump", Version("1.1.126"), "", ValidationPresetNone}};

ValidationPreset GetValidationPreset(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    for (std::size_t i = 0, n = countof(default_configurations); i < n; ++i) {
        if (default_configurations[i].name != configuration_name) continue;
        return default_configurations[i].preset;
    }

    return ValidationPresetNone;  // Not found
}

static const DefaultConfiguration *FindDefaultConfiguration(ValidationPreset preset) {
    assert(preset >= ValidationPresetFirst && preset <= ValidationPresetLast);

    for (std::size_t i = 0, n = countof(default_configurations); i < n; ++i) {
        if (default_configurations[i].preset != preset) continue;
        return &default_configurations[i];
    }

    return nullptr;  // Not found
}

const char *Configurator::GetValidationPresetName(ValidationPreset preset) const {
    // 0 is user defined, there is no file for that
    assert(preset > ValidationPresetUserDefined);

    const DefaultConfiguration *configuration = FindDefaultConfiguration(preset);
    if (configuration) return configuration->name;

    assert(0);
    return nullptr;
}

const char *Configurator::GetValidationPresetLabel(ValidationPreset preset) const {
    const DefaultConfiguration *configuration = FindDefaultConfiguration(preset);
    if (configuration) return configuration->preset_label;

    assert(0);
    return nullptr;
}

Configurator &Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() : environment(path), layers(environment) {}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    layers.LoadAllInstalledLayers();
    LoadDefaultLayerSettings();

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

    if (!environment.UsePersistentOverrideMode()) {
        SurrenderLayers(environment);
    }
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

/// Find the settings for this named layer. If none found, return nullptr
const LayerSettingsDefaults *Configurator::FindLayerSettings(const QString &layer_name) const {
    for (std::size_t i = 0, n = _default_layers_settings.size(); i < n; ++i)
        if (layer_name == _default_layers_settings[i].layer_name) return &_default_layers_settings[i];

    return nullptr;
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
            const QString file = QString(":/resourcefiles/") + default_configurations[i].name + ".json";

            Configuration configuration;
            const bool result = configuration.Load(file);
            OrderParameter(configuration.parameters, layers.available_layers);
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
        OrderParameter(configuration.parameters, layers.available_layers);
        if (result) {
            available_configurations.push_back(configuration);
        }
    }

    RefreshConfiguration();
}

void Configurator::LoadDefaultLayerSettings() {
    assert(!layers.Empty());  // layers should be loaded before default settings

    // Load the main object into the json document
    QFile file(":/resourcefiles/layer_info.json");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument json_layer_info_doc;
    json_layer_info_doc = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!json_layer_info_doc.isObject()) return;

    // Isolate the Json object for each layer
    QJsonObject doc_object = json_layer_info_doc.object();
    QJsonValue layer_options_value = doc_object.value("layer_options");
    QJsonObject layers_options_object = layer_options_value.toObject();

    // This is a list of layers for which we have user editable settings.
    // there are nine as of this writing, but this code should accomodate
    // if more are added at a later time.
    // All the layers have been loaded, so we can look for matches
    // and let the layers parse the json data to create their own list
    // of settings.
    QStringList layers_with_settings = layers_options_object.keys();
    for (int i = 0; i < layers_with_settings.size(); i++) {  // For each setting
        LayerSettingsDefaults settings_defaults;
        settings_defaults.layer_name = layers_with_settings[i];

        // Save the name of the layer, and by default none are read only
        settings_defaults.layer_name = layers_with_settings[i];

        // Get the object for just this layer
        const QJsonValue &layer_value = layers_options_object.value(layers_with_settings[i]);
        const QJsonObject &layer_object = layer_value.toObject();

        Parameter parameter;
        parameter.name = settings_defaults.layer_name;
        parameter.state = LAYER_STATE_APPLICATION_CONTROLLED;
        parameter.settings = settings_defaults.settings;

        ::LoadSettings(layer_object, parameter);

        settings_defaults.settings = parameter.settings;

        // Add to my list of layer settings
        _default_layers_settings.push_back(settings_defaults);
    }
}

void Configurator::RemoveConfiguration(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    // Not the active configuration
    if (GetActiveConfiguration()->name == configuration_name) {
        SetActiveConfiguration(available_configurations.end());
    }

    // Delete the configuration file
    const QString full_path(path.GetFullPath(PATH_CONFIGURATION, configuration_name));
    const bool result = std::remove(full_path.toUtf8().constData()) == 0;
    assert(result);

    // Reload to remove the configuration in the UI
    LoadAllConfigurations();
}

void Configurator::SetActiveConfiguration(const QString &configuration_name) {
    assert(!configuration_name.isEmpty());

    auto configuration = Find(available_configurations, configuration_name);
    assert(configuration != available_configurations.end());

    SetActiveConfiguration(configuration);
}

void Configurator::SetActiveConfiguration(std::vector<Configuration>::iterator active_configuration) {
    _active_configuration = active_configuration;

    bool surrender = false;
    if (_active_configuration != available_configurations.end()) {
        assert(!_active_configuration->name.isEmpty());
        environment.Set(ACTIVE_CONFIGURATION, _active_configuration->name);
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
        auto active_configuration = Find(available_configurations, active_configuration_name);
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
}
