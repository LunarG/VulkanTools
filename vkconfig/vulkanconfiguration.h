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

#define VKCONFIG_CUSTOM_LAYER_PATHS "./CustomPaths.txt"


struct CProfileDef {
    QString                  profileName;        // Name of the profile
    QVector<CLayerFile*>     layers;             // List of layers and their settings.
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
    void LoadAppSettings(void);
    void SaveAppSettings(void);
    QString qsLaunchApplicationWPath;
    QString qsLaunchApplicatinArgs;
    QString qsLaunchApplicationWorkingDir;
    QString qsLogFileWPath;
    bool    bLogStdout;


    // Look for all installed layers
    void LoadLayerConfiguration(void);
    QVector <CLayerFile*>   implicitLayers;     // All detected implicit layers
    QVector <CLayerFile*>   explicitLayers;     // All detected explicit layers
    QVector <CLayerFile*>   customLayers;       // Layers found in custom search paths
    void loadLayersFromPath(const QString &qsPath, QVector<CLayerFile *>& layerList, TLayerType type);

    QVector <CProfileDef *>  profileList;       // List and details about current profiles
    int nActiveProfile;

    void loadAdditionalSearchPaths(void);
    void saveAdditionalSearchPaths(void);
    uint32_t nAdditionalSearchPathCount;
    QStringList additionalSearchPaths;

    void loadProfiles(void);
    void saveProfiles(void);


protected:
    CVulkanConfiguration();

    static CVulkanConfiguration*   pMe;



};

#endif // CVULKANCONFIGURATION_H
