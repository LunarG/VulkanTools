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
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/layer_setting.h"
#include "../vkconfig_core/version.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include <vector>

void RemoveString(QString& delimitedString, QString value);
void AddString(QString& delimitedString, QString value);

class Layer {
   public:
    Layer();

    bool IsValid() const;
    bool operator==(const Layer& layer) const;
    bool operator!=(const Layer& layer) const;

   public:
    // Standard pieces of a layer
    Version _file_format_version;
    QString _name;
    QString _type;
    QString _library_path;  // This is a relative path, straight out of the json
    Version _api_version;
    QString _implementation_version;
    QString _description;

    QString _layer_path;  // Actual path to the folder that contains the layer (this is important!)
    LayerType _layer_type;

    // This layers settings. This will be used to build the editor
    // as well as create settings files. This CAN be empty if the
    // layer doens't have any settings.
    // std::vector<LayerSetting> _layer_settings;

    // File based layers
    bool Load(QString full_path_to_file, LayerType layer_type);
};

const Layer* FindLayer(const QVector<Layer*>& layers, QString layer_name);
