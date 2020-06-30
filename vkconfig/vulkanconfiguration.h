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
 * The vkConfig2 program monitors and adjusts the Vulkan configuration
 * environment. These settings are wrapped in this class, which serves
 * as the "model" of the system.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */
#ifndef CVULKANCONFIGURATION_H
#define CVULKANCONFIGURATION_H

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
#include "profiledef.h"

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
class CPathFinder {
   public:
#ifdef _WIN32
    CPathFinder(const QString& qsPath, bool bForceFileSystem = false);
#else
    CPathFinder(const QString& qsPath, bool bForceFileSystem = true);
#endif
    int FileCount(void) { return fileList.size(); }
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
    QVector<TLayerSettings*> defaultSettings;  // Default settings for this layer
};

//////////////////////////////////////////////////////////
// We will maintain a list of applicitons, each can have
// it's own working folder (when run in test mode), and
// it's own set of command line arguments
struct TAppListEntry {
    QString qsAppNameWithPath;
    QString qsWorkingFolder;
    QString qsArguments;
    QString qsLogFile;
    bool bExcludeFromGlobalList;
};

class CVulkanConfiguration {
   public:
    static CVulkanConfiguration* getVulkanConfig(void) {
        // Just a note... pMe is also set inside the constructor
        // to prevent recusion when no layers are found.
        if (pMe == nullptr) pMe = new CVulkanConfiguration();

        return pMe;
    }

    ~CVulkanConfiguration();

    static int nNumCannedProfiles;
    static const char* szCannedProfiles[8];

    // Need this to check vulkan loader version
    uint32_t vulkanInstanceVersion;

    /////////////////////////////////////////////////////////////////////////
    // Just local app settings
    void LoadAppSettings(void);
    void SaveAppSettings(void);
    void ResetToDefaultAppSettings(void);
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
    void LoadAdditionalSearchPaths(void);
    void SaveAdditionalSearchPaths(void);
    QStringList additionalSearchPaths;

    /////////////////////////////////////////////////////////////////////////
    // The list of applications affected
    QVector<TAppListEntry*> appList;
    void LoadAppList(void);
    void SaveAppList(void);

    ////////////////////////////////////////////////////////////////////////
    // A readonly list of layer names with the associated settings
    // and their default values. This is for reference by individual profile
    // objects.
    QVector<LayerSettingsDefaults*> defaultLayerSettings;
    void LoadDefaultLayerSettings(void);
    const LayerSettingsDefaults* FindSettingsFor(QString layerName);
    void LoadDefaultSettings(CLayerFile* pBlankLayer);

    ////////////////////////////////////////////////////////////////////////
    // Look for all installed layers. This contains their path, version info, etc.
    // but does not contain settings information. The default settings are stored
    // in the above (defaultLayerSettings). The binding of a layer with it's
    // particular settings is done in the profile (CProfileDef - in profile list).
    // This includes all found implicit, explicit, or layers found in custom folders
    QVector<CLayerFile*> allLayers;  // All the found layers, lumped together
    void FindAllInstalledLayers(void);
    void LoadLayersFromPath(const QString& qsPath, QVector<CLayerFile*>& layerList, TLayerType type);
    const CLayerFile* FindLayerNamed(QString qsLayerName, const char* location = nullptr);

    QVector<CProfileDef*> profileList;  // List and details about current profiles

    // We need to push and pop a temporary environment.
    // The stack is only one deep...
    CProfileDef* pSavedProfile;

    void pushProfile(CProfileDef* pNew);
    void popProfile(void);

    CProfileDef* CreateEmptyProfile(void);
    CProfileDef* FindProfile(QString profileName);
    CProfileDef* LoadProfile(QString pathToProfile);  // Load .profile descriptor
    void LoadAllProfiles(void);                       // Load all the .profile files found
    bool SaveProfile(CProfileDef* pProfile);          // Write .profile descriptor
    void ImportProfile(QString qsFullPathToSource);

    void FindVkCube(void);

    bool IsRunningAsAdministrator(void) { return bRunningAsAdministrator; }

    // Set this as the current override profile
    void SetCurrentActiveProfile(CProfileDef* pProfile);
    CProfileDef* GetCurrentActiveProfile(void) { return pActiveProfile; }
    void RefreshProfile(void) {
        if (pActiveProfile) SetCurrentActiveProfile(pActiveProfile);
    }

    QString GetProfilePath(void) { return qsProfileFilesPath; }

    QString CheckVulkanSetup(void);
    void CheckApplicationRestart(void);

   protected:
    CVulkanConfiguration();
    static CVulkanConfiguration* pMe;

    // Currently active profile
    CProfileDef* pActiveProfile;

    bool bRunningAsAdministrator;  // Are we being "Run as Administrator"
    bool bFirstRun;                // This is used for populating the initial set of profiles/configurations
    void ClearLayerLists(void);

#ifdef WIN32
    void LoadDeviceRegistry(DEVINST id, const QString& entry, QVector<CLayerFile*>& layerList, TLayerType type);
    void LoadRegistryLayers(const QString& path, QVector<CLayerFile*>& layerList, TLayerType type);

    void AddRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);
    void RemoveRegistryEntriesForLayers(QString qsJSONFile, QString qsSettingsFile);

#endif
};

#endif  // CVULKANCONFIGURATION_H
