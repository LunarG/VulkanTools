/*
 * Copyright (c) 2018 Valve Corporation
 * Copyright (c) 2018 LunarG, Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 */

#pragma once

#include <QHash>
#include <QList>
#include <QString>

#include "layer_manifest.h"

// Add a Paths() method?
class OverrideSettings
{
public:
    OverrideSettings();
    void ClearLayers();
    void ClearSettings();
    QList<QString> Layers() const;
    QHash<QString, QHash<QString, QString>> LayerSettings() const;
    bool SaveLayers(const QList<QPair<QString, LayerType>> &paths, const QList<LayerManifest> &layers, int expiration);
    bool SaveSettings(const QHash<QString, QHash<QString, LayerValue>> &settings);

private:
    QString LayerFile(bool create_path) const;
    QString LayerSettingsFile(bool create_path) const;

    QList<QString> layers;
    QHash<QString, QHash<QString, QString>> layer_settings;
};
