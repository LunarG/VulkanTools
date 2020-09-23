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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/path_manager.h"
#include "../vkconfig_core/environment.h"
#include "../vkconfig_core/configuration.h"

#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#include <Cfgmgr32.h>
#define WIN_BUFFER_SIZE 1024
#endif
#include <QString>
#include <QVector>
#include <QDir>
#include <QTreeWidget>

#include <vulkan/vulkan.h>

#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// Going back and forth between the Windows registry and looking for files
/// in specific folders is just a mess. This class consolidates all that into
/// one single abstraction that knows whether to look in the registry or in
/// a folder with QDir.
/// This is a little weird because generally QSettings is for going back
/// and forth between the Registry or .ini files. Here, I'm going from
/// the registry to directory entries.
class PathFinder {
   public:
#ifdef _WIN32
    PathFinder(const QString& path, bool force_file_system = false);
#else
    PathFinder(const QString& path, bool force_file_system = true);
#endif
    int FileCount() { return file_list_.size(); }
    QString GetFileName(int i) { return file_list_[i]; }

   private:
    QStringList file_list_;
};

// This is a master list of layer settings. All the settings
// for what layers can have user modified settings. It contains
// the names of the layers and the properties of the settings.
// THIS IS TO BE READ ONLY, as it is copied from frequently
// to reset or initialize the a full layer definition for the
// profiles.
struct LayerSettingsDefaults {
    QString layer_name;                  // Name of layer
    std::vector<LayerSetting> settings;  // Default settings for this layer
};

class Configurator {
   public:
    static Configurator& Get();
    bool Init();

   private:
    const bool _running_as_administrator;  // Are we being "Run as Administrator"

    /////////////////////////////////////////////////////////////////////////
    // Validation Preset
   public:
    const char* GetValidationPresetName(ValidationPreset preset) const;
    const char* GetValidationPresetLabel(ValidationPreset preset) const;

    /////////////////////////////////////////////////////////////////////////
    // Additional places to look for layers
   public:
    void BuildCustomLayerTree(QTreeWidget* tree_widget);

    QStringList VK_LAYER_PATH;  // If this environment variable is set, this contains
                                // a list of paths that should be searched first for
                                // Vulkan layers. (Named as environment variable for
                                // clarity as to where this comes from).

    /////////////////////////////////////////////////////////////////////////
    // The list of applications affected
   public:
    // If return_loader_version is not null, the function will return the loader version
    // If quiet is false, message box will be generate
    bool SupportApplicationList(bool quiet = true, Version* return_loader_version = nullptr) const;

    bool HasActiveOverrideOnApplicationListOnly() const {
        return SupportApplicationList() && environment.UseApplicationListOverrideMode();
    }

    ////////////////////////////////////////////////////////////////////////
    // A readonly list of layer names with the associated settings
    // and their default values. This is for reference by individual profile
    // objects.
    std::vector<LayerSettingsDefaults> _default_layers_settings;
    void LoadDefaultLayerSettings();
    const LayerSettingsDefaults* FindLayerSettings(const QString& layer_name) const;

    ////////////////////////////////////////////////////////////////////////
    // Look for all installed layers. This contains their path, version info, etc.
    // but does not contain settings information. The default settings are stored
    // in the above (defaultLayerSettings). The binding of a layer with it's
    // particular settings is done in the profile (Configuration - in configuration list).
    // This includes all found implicit, explicit, or layers found in custom folders
    std::vector<Layer> available_layers;  // All the found layers, lumped together
    void LoadAllInstalledLayers();
    Layer* FindLayerNamed(const QString& layer_name);
    void LoadLayersFromPath(const QString& path, std::vector<Layer>& layers);

    std::vector<Configuration> available_configurations;

    void LoadAllConfigurations();  // Load all the .profile files found
    void ImportConfiguration(const QString& full_import_path);
    void ExportConfiguration(const QString& source_file, const QString& full_export_path);
    bool HasMissingLayers(const Configuration& configuration) const;
    void ResetDefaultsConfigurations();

    bool HasLayers() const;

    // Set this as the current override configuration
    void SetActiveConfiguration(std::vector<Configuration>::iterator active_configuration);
    void SetActiveConfiguration(const QString& configuration_name);
    std::vector<Configuration>::iterator GetActiveConfiguration() const { return _active_configuration; }
    void RefreshConfiguration();
    bool HasActiveConfiguration() const;

   private:
    Configurator();
    ~Configurator();

    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

    std::vector<Configuration>::iterator _active_configuration;

    void ClearLayerLists();

#ifdef _WIN32
    void LoadDeviceRegistry(DEVINST id, const QString& entry, std::vector<Layer>& layers, LayerType type);
    void LoadRegistryLayers(const QString& path, std::vector<Layer>& layers, LayerType type);

    void AddRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
    void RemoveRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
#endif

   public:
    PathManager path;
    Environment environment;
};

ValidationPreset GetValidationPreset(const QString& configuration_name);
