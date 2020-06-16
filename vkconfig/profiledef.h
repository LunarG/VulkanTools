#ifndef CPROFILEDEF_H
#define CPROFILEDEF_H

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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 *
 * Layer files are JSON documents, so a layer file object is derived
 * from QJsonDocument and is given several useful utility access methods
 * for querying and manipulating layer .json files.
 *
 */
#include <QString>
#include <QStringList>

#include <layerfile.h>

class CProfileDef
{
public:
    CProfileDef();
    ~CProfileDef();

    QString         qsProfileName;              // User readable display of the profile name (may contain spaces)
                                                // This is the same as the filename, but with the .json stripped off.
    QString         qsFileName;                 // Root file name without path (by convention, no spaces and .profile suffix)
    QString         qsDescription;              // A friendly description of what this profile does
    QByteArray      settingTreeState;           // Recall editor tree state
    int             nPresetIndex;               // Khronos layer presets. 0 = none or user defined

    // A profile is nothing but a list of layers and their settings in truth
    QVector<CLayerFile *>layers;

    QStringList     blacklistedLayers;                  // Just the names of blacklisted layers

    CLayerFile*     FindLayer(QString qsLayerName, QString qsFullPath);     // Find the layer if it exists
    CLayerFile*     FindLayerNamed(QString qsLayerName);                    // Find the layer if it exists, only care about the name

    CProfileDef     *DuplicateProfile(void);            // Copy a profile so we can mess with it

    void            CollapseProfile(void);              // Remove unused layers and settings, set blacklist

    CLayerFile      *GetKhronosLayer(void);             // Retrieve the Khronos validation layer if it is included
};

#endif // CPROFILEDEF_H
