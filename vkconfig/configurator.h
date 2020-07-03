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

#include "layerfile.h"
#include "configuration.h"

#define DONT_SHOW_AGAIN_MESSAGE "Do not show again"
#define APP_SHORT_NAME "vkconfig"

////////////////////////////////////////////////////////////////////////////////
/// \brief The CPathFinder class
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
    PathFinder(const QString& qsPath, bool bForceFileSystem = false);
#else
    PathFinder(const QString& qsPath, bool bForceFileSystem = true);
#endif
    int FileCount() { return fileList.size(); }
    QString GetFileName(int iIndex) { return fileList[iIndex]; }

   protected:
    QStringList fileList;
};

#define VKCONFIG_NAME "Vulkan Configurator"

// Saved settings for the application
#define VKCONFIG_KEY_LAUNCHAPP "launchApp"
#define VKCONFIG_KEY_ACTIVEPROFILE "activeProfile"
#define VKCONFIG_KEY_CUSTOM_PATHS "customPaths"
#define VKCONFIG_KEY_OVERRIDE_ACTIVE "overrideActive"
#define VKCONFIG_KEY_APPLY_ONLY_TO_LIST "applyOnlyToList"
#define VKCONFIG_KEY_KEEP_ACTIVE_ON_EXIT "keepActiveOnExit"
#define VKCONFIG_KEY_FIRST_RUN "firstRun"
#define VKCONFIG_KEY_APPLY_ALL "applytoall"
#define VKCONFIG_HIDE_RESTART_WARNING "restartWarning"

// This is a master list of layer settings. All the settings
// for what layers can have user modified settings. It contains
// the names of the layers and the properties of the settings.
// THIS IS TO BE READ ONLY, as it is copied from frequently
// to reset or initialize the a full layer definition for the
// profiles.
class LayerSettingsDefaults {
   public:
    QString layerName;                         // Name of layer
    QVector<LayerSettings*> defaultSettings;  // Default settings for this layer
};

//////////////////////////////////////////////////////////
// We will maintain a list of applicitons, each can have
// it's own working folder (when run in test mode), and
// it's own set of command line arguments
struct AppListEntry {
    QString qsAppNameWithPath;
    QString qsWorkingFolder;
    QString qsArguments;
    QString qsLogFile;
    bool bExcludeFromGlobalList;
};

class Configurator {
   public:
    static Configurator& Get();

    ~Configurator();

    static int nNumCannedProfiles;
    static const char* szCannedProfiles[8];

    // Need this to check vulkan loader version
    uint32_t vulkanInstanceVersion;

    /////////////////////////////////////////////////////////////////////////
    // Just local app settings
    void LoadAppSettings();
    void SaveAppSettings();
    void ResetToDefaultAppSettings();
    QString qsLastLaunchApplicationWPath;  // This is to match up with the application list
    bool bOverrideActive;                  // Do we have an active override?
    bool bApplyOnlyToList;                 // Apply the overide only to the application list
    bool bKeepActiveOnExit;                // Stay active when app closes
    bool bHasOldLoader;                    // Older loader does not support per-application overrides
    bool bApplyToAllUsers;                 // Apply to all users, not just the current user

    QString qsProfileFilesPath;      // Where config working files live
    QString qsOverrideSettingsPath;  // Where settings go when profile is active
    QString qsOverrideJsonPath;      // Where json goes when profile is active

    /////////////////////////////////////////////////////////////////////////
    // Additional places to look for layers
    void LoadAdditionalSearchPaths();
    void SaveAdditionalSearchPaths();
    QStringList additionalSearchPaths;

    /////////////////////////////////////////////////////////////////////////
    // The list of applications affected
    QVector<AppListEntry*> appList;
    void LoadAppList();
    void SaveAppList();

    ////////////////////////////////////////////////////////////////////////
    // A readonly list of layer names with the associated settings
    // and their default values. This is for reference by individual profile
    // objects.
    QVector<LayerSettingsDefaults*> defaultLayerSettings;
    void LoadDefaultLayerSettings();
    const LayerSettingsDefaults* FindSettingsFor(QString layerName);
    void LoadDefaultSettings(LayerFile* pBlankLayer);

    ////////////////////////////////////////////////////////////////////////
    // Look for all installed layers. This contains their path, version info, etc.
    // but does not contain settings information. The default settings are stored
    // in the above (defaultLayerSettings). The binding of a layer with it's
    // particular settings is done in the profile (CProfileDef - in profile list).
    // This includes all found implicit, explicit, or layers found in custom folders
    QVector<LayerFile*> allLayers;  // All the found layers, lumped together
    void FindAllInstalledLayers();
    void LoadLayersFromPath(const QString& qsPath, QVector<LayerFile*>& layerList, LayerType type);
    const LayerFile* FindLayerNamed(QString qsLayerName, const char* location = nullptr);

    QVector<Configuration*> profileList;  // List and details about current profiles

    // We need to push and pop a temporary environment.
    // The stack is only one deep...
    Configuration* pSavedProfile;

    void pushProfile(Configuration* pNew);
    void popProfile();

    Configuration* CreateEmptyProfile();
    Configuration* FindProfile(QString profileName);
    Configuration* LoadProfile(QString pathToProfile);  // Load .profile descriptor
    void LoadAllProfiles();                       // Load all the .profile files found
    bool SaveProfile(Configuration* pProfile);          // Write .profile descriptor
    void ImportProfile(QString qsFullPathToSource);
    void ExportProfile(QString qsFullPathToSource, QString qsFullPathToDest);

    void FindVkCube();

    bool HasLayers() const;
    bool IsRunningAsAdministrator() { return bRunningAsAdministrator; }

    // Set this as the current override profile
    void SetCurrentActiveProfile(Configuration* pProfile);
    Configuration* GetCurrentActiveProfile() { return pActiveProfile; }
    void RefreshProfile() {
        if (pActiveProfile) SetCurrentActiveProfile(pActiveProfile);
    }

    QString GetProfilePath() { return qsProfileFilesPath; }

    QString CheckVulkanSetup();
    void CheckApplicationRestart();

   protected:
    Configurator();

    // Currently active profile
    Configuration* pActiveProfile;

    bool bRunningAsAdministrator;  // Are we being "Run as Administrator"
    bool bFirstRun;                // This is used for populating the initial set of profiles/configurations
    void ClearLayerLists();

#ifdef WIN32
    void LoadDeviceRegistry(DEVINST id, const QString& entry, QVector<LayerFile*>& layerList, LayerType type);
    void LoadRegistryLayers(const QString& path, QVector<LayerFile*>& layerList, LayerType type);

    void AddRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
    void RemoveRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
#endif

   private:
    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;
};
