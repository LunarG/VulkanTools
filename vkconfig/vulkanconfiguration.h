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

#include <QString>
#include <QVector>

#include <layerfile.h>
#include "profiledef.h"


// Saved settings for the application
#define VKCONFIG_KEY_LAUNCHAPP      "launchApp"
#define VKCONFIG_KEY_LAUNCHAPP_CWD  "launchAppCWD"
#define VKCONFIG_KEY_LAUNCHAPP_ARGS "launchAppARGS"
#define VKCONFIG_KEY_LOGFILE        "logFileName"
#define VKCONFIG_KEY_LOGSTDOUT      "logStdout"

#define VKCONFIG_CUSTOM_LAYER_PATHS "/.local/share/vulkan/CustomPaths.txt"
#define VKCONIFG_CUSTOM_APP_LIST    "/.local/share/vulkan/AppList.txt"
#define VKCONFIG_PROFILE_LIST       "/.local/share/vulkan/ProfileList.json"

// This is a master list of layer settings. All the settings
// for what layers can have user modified settings. It contains
// the names of the layers and the properties of the settings.
// THIS IS TO BE READ ONLY, as it is copied from frequently
// to reset or initialize the a full layer definition for the
// profiles.
struct LayerSettingsDefaults {
    QString                 layerName;                  // Name of layer
    QVector<TLayerSettings *>defaultSettings;           // Default settings for this layer
};




class CVulkanConfiguration
{
public:
    static CVulkanConfiguration* getVulkanConfig(void) {
        if(pMe == nullptr)
            pMe = new CVulkanConfiguration();

        return pMe;
        }

    ~CVulkanConfiguration();

    // Just local app settings
    void loadAppSettings(void);
    void saveAppSettings(void);
    QString qsLaunchApplicationWPath;
    QString qsLaunchApplicatinArgs;
    QString qsLaunchApplicationWorkingDir;
    QString qsLogFileWPath;
    bool    bLogStdout;

    /////////////////////////////////////////////////////////////////////////
    // Additional places to look for layers
    void loadAdditionalSearchPaths(void);
    void saveAdditionalSearchPaths(void);
    uint32_t nAdditionalSearchPathCount;
    QStringList additionalSearchPaths;

    /////////////////////////////////////////////////////////////////////////
    // The list of applications affected
    QStringList appList;
    void loadAppList(void);
    void saveAppList(void);


    ////////////////////////////////////////////////////////////////////////
    // A readonly list of layer names with the associated settings
    // and their default values. This is for reference by individual profile
    // objects.
    QVector <LayerSettingsDefaults *> defaultLayerSettings;
    void loadDefaultLayerSettings(void);
    const LayerSettingsDefaults* findSettingsFor(QString layerName);

    ////////////////////////////////////////////////////////////////////////
    // Look for all installed layers. This contains their path, version info, etc.
    // but does not contain settings information. The default settings are stored
    // in the above (defaultLayerSettings). The binding of a layer with it's
    // particular settings is done in the profile (CProfileDef - in profile list).
    QVector <CLayerFile*>   implicitLayers;     // All detected implicit layers
    QVector <CLayerFile*>   explicitLayers;     // All detected explicit layers
    QVector <CLayerFile*>   customLayers;       // Layers found in custom search paths
    void findAllInstalledLayers(void);
    void loadLayersFromPath(const QString &qsPath, QVector<CLayerFile *>& layerList, TLayerType type);
    const CLayerFile* findLayerNamed(QString qsLayerName);



    QVector <CProfileDef *>  profileList;       // List and details about current profiles
    CProfileDef*             pActiveProfile;
    CProfileDef* FindProfile(QString profileName);
    CProfileDef* LoadProfile(QString pathToProfile);        // Load .profile descriptor
    void loadAllProfiles(void);                             // Load all the .profile files found
    void SaveProfile(CProfileDef *pProfile, QString pathToProfile);     // Write .profile descriptor


    // Set this as the current override profile
    void SetCurrentProfile(CProfileDef *pProfile);



protected:
    CVulkanConfiguration();
    static CVulkanConfiguration*   pMe;


    void clearLayerLists(void);



};

#endif // CVULKANCONFIGURATION_H
