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

#include "layerfile.h"

// Saved settings for the application
#define VKCONFIG_KEY_LAUNCHAPP      "launchApp"
#define VKCONFIG_KEY_LAUNCHAPP_CWD  "launchAppCWD"
#define VKCONFIG_KEY_LAUNCHAPP_ARGS "launchAppARGS"
#define VKCONFIG_KEY_LOGFILE        "logFileName"
#define VKCONFIG_KEY_LOGSTDOUT      "logStdout"

#define VKCONFIG_CUSTOM_LAYER_PATHS "/.local/share/vulkan/CustomPaths.txt"
#define VKCONIFG_CUSTOM_APP_LIST    "/.local/share/vulkan/AppList.txt"
#define VKCONFIG_PROFILE_LIST       "/.local/share/vulkan/ProfileList.json"

struct CProfileDef {
    QString                 profileName;        // Name of the profile
    QVector<CLayerFile*>    layers;             // List of layers and their settings.
    bool                    readOnly;           // This profile cannot be changed
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


    // Look for all installed layers
    void reLoadLayerConfiguration(void);
    QVector <CLayerFile*>   implicitLayers;     // All detected implicit layers
    QVector <CLayerFile*>   explicitLayers;     // All detected explicit layers
    QVector <CLayerFile*>   customLayers;       // Layers found in custom search paths
    void loadLayersFromPath(const QString &qsPath, QVector<CLayerFile *>& layerList, TLayerType type);

    QVector <CProfileDef *>  profileList;       // List and details about current profiles
    int nActiveProfile;
    CProfileDef* getActiveProfile(void) { if(nActiveProfile < 0) return nullptr;
                                          return profileList[nActiveProfile]; }

    void loadAdditionalSearchPaths(void);
    void saveAdditionalSearchPaths(void);
    uint32_t nAdditionalSearchPathCount;
    QStringList additionalSearchPaths;

    // This is really only done once, to initialize the list
    // of canned profiles.
    void loadDefaultProfiles(void);

    // This is our current list of profiles, both canned (with tweaks),
    // and cloned, edited.
    bool loadSavedProfiles(void);
    void saveProfiles(void);

    // The list of applications affected
    QStringList appList;
    void loadAppList(void);
    void saveAppList(void);


protected:
    CVulkanConfiguration();
    static CVulkanConfiguration*   pMe;

    QStringList         layersWithSettings;

    void loadLayerSettingsFromJson(void);
    void clearLayerLists(void);

};

#endif // CVULKANCONFIGURATION_H
