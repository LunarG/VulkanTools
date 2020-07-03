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

#include <QString>
#include <QStringList>

#include "layerfile.h"

class Configuration {
   public:
    Configuration();
    ~Configuration();

    QString name;                 // User readable display of the profile name (may contain spaces)
                                  // This is the same as the filename, but with the .json stripped off.
    QString file;                 // Root file name without path (by convention, no spaces and .profile suffix)
    QString description;          // A friendly description of what this profile does
    QByteArray settingTreeState;  // Recall editor tree state
    int preset_index;             // Khronos layer presets. 0 = none or user defined

    // A profile is nothing but a list of layers and their settings in truth
    QVector<LayerFile *> layers;

    QStringList excluded_layers;  // Just the names of blacklisted layers

    LayerFile *FindLayer(const QString &qsLayerName, const QString &qsFullPath) const;  // Find the layer if it exists
    LayerFile *FindLayerNamed(const QString &qsLayerName) const;  // Find the layer if it exists, only care about the name

    Configuration *DuplicateProfile();  // Copy a profile so we can mess with it

    void CollapseProfile();  // Remove unused layers and settings, set blacklist

    LayerFile *GetKhronosLayer();  // Retrieve the Khronos validation layer if it is included

    bool IsValid() { return all_layers_available; }

    bool all_layers_available;
};
