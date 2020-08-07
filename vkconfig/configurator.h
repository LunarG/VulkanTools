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

#include "layerfile.h"
#include "configuration.h"

#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#include <Cfgmgr32.h>
#define WIN_BUFFER_SIZE 1024
#endif
#include <QString>
#include <QVector>
#include <QSettings>
#include <QDir>

#include <vulkan/vulkan.h>

#define DONT_SHOW_AGAIN_MESSAGE "Do not show again"
#define APP_SHORT_NAME "vkconfig"

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

#define VKCONFIG_NAME "Vulkan Configurator"

// Saved settings for the application
#define VKCONFIG_KEY_LAUNCHAPP "launchApp"
#define VKCONFIG_KEY_ACTIVEPROFILE "activeProfile"
#define VKCONFIG_KEY_CUSTOM_PATHS "customPaths"
#define VKCONFIG_KEY_OVERRIDE_ACTIVE "overrideActive"
#define VKCONFIG_KEY_APPLY_ONLY_TO_LIST "applyOnlyToList"
#define VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT "keepActiveOnExit"
#define VKCONFIG_KEY_INITIALIZE_FILES "FirstTimeRun"
#define VKCONFIG_HIDE_RESTART_WARNING "restartWarning"
#define VKCONFIG_KEY_LAST_EXPORT_PATH "lastExportPath"
#define VKCONFIG_KEY_LAST_IMPORT_PATH "lastImportPath"
#define VKCONFIG_KEY_LAST_EXECUTABLE_PATH "lastExecutablePath"
#define VKCONFIG_KEY_VKCONFIG_VERSION "vkConfigVersion"
#define VKCONFIG_KEY_RESTORE_GEOMETRY "restaureGeometry"
#define VKCONFIG_WARN_SHUTDOWNSTATE "warnAboutShutdownState"

// This is a master list of layer settings. All the settings
// for what layers can have user modified settings. It contains
// the names of the layers and the properties of the settings.
// THIS IS TO BE READ ONLY, as it is copied from frequently
// to reset or initialize the a full layer definition for the
// profiles.
struct LayerSettingsDefaults {
    QString layer_name;                        // Name of layer
    QVector<LayerSettings*> default_settings;  // Default settings for this layer
};

//////////////////////////////////////////////////////////
// We will maintain a list of applicitons, each can have
// it's own working folder (when run in test mode), and
// it's own set of command line arguments
struct Application {
    Application() {}
    Application(const QString& executable_full_path, const QString& arguments);

    QString executable_path;
    QString working_folder;
    QString arguments;
    QString log_file;
    bool override_layers;
};

class Configurator {
   public:
    enum Path {
        ConfigurationPath = 0,     // Where config working files live
        OverrideSettingsPath = 1,  // Where settings go when profile is active
        OverrideLayersPath = 2,    // Where json goes when profile is active
        LastImportPath = 3,        // The last path used by the user to import a configuration
        LastExportPath = 4,        // The last path used by the user to export a configuration
        LastExecutablePath = 5,    // The last path used by the user when adding an executable to the application list

        FirstPath = ConfigurationPath,
        LastPath = LastExecutablePath
    };
    enum { PathCount = LastPath - FirstPath + 1 };

    static Configurator& Get();
    bool InitializeConfigurator(void);

    // Need this to check vulkan loader version
    uint32_t _vulkan_instance_version;
    bool _has_old_loader;  // Older loader does not support per-application overrides

   private:
    bool _running_as_administrator;  // Are we being "Run as Administrator"
    bool _first_run;                 // This is used for populating the initial set of configurations

    /////////////////////////////////////////////////////////////////////////
    // Just Vulkan Configurator settings
   public:
    void LoadSettings();
    void SaveSettings();
    void ResetToDefaultSettings();
    bool HasActiveOverrideOnApplicationListOnly() const { return !_has_old_loader && _overridden_application_list_only; }
    QString GetPath(Path requested_path) const;
    void SetPath(Path requested_path, QString path);

    bool _override_active;                    // Do we have active layers override?
    bool _overridden_application_list_only;   // Apply the override only to the application list
    bool _override_permanent;                 // The override remains active when Vulkan Configurator closes
    bool _override_application_list_updated;  // The list of applications to override has possibly been updated

   private:
    QString _paths[PathCount];

    /////////////////////////////////////////////////////////////////////////
    // Validation Preset
   public:
    const char* GetValidationPresetName(ValidationPreset preset) const;
    const char* GetValidationPresetLabel(ValidationPreset preset) const;

    /////////////////////////////////////////////////////////////////////////
    // Application Launcher
   public:
    void SelectLaunchApplication(int application_index);
    int GetLaunchApplicationIndex() const;
    void FindVkCube();

   private:
    QString _active_launch_executable_path;  // This is to match up with the application list

    /////////////////////////////////////////////////////////////////////////
    // Additional places to look for layers
   public:
    void LoadCustomLayersPaths();
    void SaveCustomLayersPaths();
    void RemoveCustomLayersPath(int path_index);
    void RemoveCustomLayersPath(const QString& path);
    void AppendCustomLayersPath(const QString& path);
    int GetCustomLayersPathSize() const;
    const QString& GetCustomLayersPath(int path_index) const;

    QStringList VK_LAYER_PATH;  // If this environment variable is set, this contains
                                // a list of paths that should be searched first for
                                // Vulkan layers. (Named as environment variable for
                                // clarity as to where this comes from).

   private:
    QStringList _custom_layers_paths;

    /////////////////////////////////////////////////////////////////////////
    // The list of applications affected
   public:
    QVector<Application*> _overridden_application_list;
    void LoadOverriddenApplicationList();
    void SaveOverriddenApplicationList();
    bool HasOverriddenApplications() const;

    ////////////////////////////////////////////////////////////////////////
    // A readonly list of layer names with the associated settings
    // and their default values. This is for reference by individual profile
    // objects.
    QVector<LayerSettingsDefaults*> _default_layers_settings;
    void LoadDefaultLayerSettings();
    const LayerSettingsDefaults* FindLayerSettings(const QString& layer_name) const;
    void LoadDefaultSettings(LayerFile* empty_layer);

    ////////////////////////////////////////////////////////////////////////
    // Look for all installed layers. This contains their path, version info, etc.
    // but does not contain settings information. The default settings are stored
    // in the above (defaultLayerSettings). The binding of a layer with it's
    // particular settings is done in the profile (Configuration - in configuration list).
    // This includes all found implicit, explicit, or layers found in custom folders
    QVector<LayerFile*> _available_Layers;  // All the found layers, lumped together
    void LoadAllInstalledLayers();
    const LayerFile* FindLayerNamed(QString layer_name);
    void LoadLayersFromPath(const QString& path, QVector<LayerFile*>& layer_list);

    QVector<Configuration*> _available_configurations;

    // We need to push and pop a temporary environment.
    // The stack is only one deep...
    Configuration* _saved_configuration;

    void PushConfiguration(Configuration* configuration);
    void PopConfiguration();

    Configuration* CreateEmptyConfiguration();
    Configuration* FindConfiguration(const QString& configuration_name) const;
    Configuration* LoadConfiguration(const QString& path_configuration);  // Load .profile descriptor
    void LoadAllConfigurations();                                         // Load all the .profile files found
    bool SaveConfiguration(Configuration* configuration);                 // Write .profile descriptor
    void ImportConfiguration(const QString& full_import_path);
    void ExportConfiguration(const QString& source_file, const QString& full_export_path);

    bool HasLayers() const;
    bool IsRunningAsAdministrator() { return _running_as_administrator; }

    // Set this as the current override configuration
    void SetActiveConfiguration(Configuration* configuration);
    Configuration* GetActiveConfiguration() { return _active_configuration; }
    void RefreshConfiguration() {
        if (_active_configuration) SetActiveConfiguration(_active_configuration);
    }

    QString CheckVulkanSetup() const;
    void CheckApplicationRestart() const;

   private:
    Configurator();
    ~Configurator();
    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

    Configuration* _active_configuration;

    void ClearLayerLists();

#ifdef _WIN32
    void LoadDeviceRegistry(DEVINST id, const QString& entry, QVector<LayerFile*>& layerList, LayerType type);
    void LoadRegistryLayers(const QString& path, QVector<LayerFile*>& layerList, LayerType type);

    void AddRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
    void RemoveRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
#endif
};
