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

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QVector>
#include <QVariant>

typedef enum { LAYER_TYPE_EXPLICIT = 0, LAYER_TYPE_IMPLICIT, LAYER_TYPE_CUSTOM } LayerType;

typedef enum {
    LAYER_SETTINGS_STRING = 0,
    LAYER_SETTINGS_FILE,
    LAYER_SETTINGS_LOAD_FILE,
    LAYER_SETTINGS_SAVE_FOLDER,
    LAYER_SETTINGS_BOOL,
    LAYER_SETTINGS_BOOL_NUMERIC,
    LAYER_SETTINGS_EXCLUSIVE_LIST,
    LAYER_SETTINGS_INCLUSIVE_LIST,
    LAYER_SETTINGS_RANGE_INT,
    LAYER_SETTINGS_VUID_FILTER,
} LayerSettingsType;

// This structure is copied by assignment elsewhere, so do not add
// any pointers to it please...
struct LayerSettings {
    QString settings_name;                       // Name of the setting the layer looks for (programatic variable name)
    QString settings_prompt;                     // Short name to prompt end user
    QString settings_desc;                       // Human version, describes the setting
    LayerSettingsType settings_type;             // The data type
    QVariant settings_max_value;                 // For range based
    QVariant settings_min_value;                 // For range based
    QStringList settings_list_exclusive_value;   // List of exclusive items
    QStringList settings_list_exclusive_prompt;  // List of exclusive item prompts
    QStringList settings_list_inclusive_value;   // List of non-exclusive items (more than one item can be selected)
    QStringList settings_list_inclusive_prompt;  // List of non-exclusive item prompts (more than one item can be selected)
    QString settings_value;                      // Default value as a string
};

void RemoveString(QString& delimitedString, QString value);
void AddString(QString& delimitedString, QString value);

class LayerFile : public QObject {
    Q_OBJECT
   public:
    // Standard pieces of a layer
    QString _file_format_version;
    QString _name;
    QString _type;
    QString _library_path;  // This is a relative path, straight out of the json
    QString _api_version;
    QString _implementation_version;
    QString _description;

    QString _layer_path;  // Actual path to the folder that contains the layer (this is important!)
    LayerType _layer_type;

    // This layers settings. This will be used to build the editor
    // as well as create settings files. This CAN be empty if the
    // layer doens't have any settings.
    QVector<LayerSettings*> _layer_settings;

    bool _enabled;   // When used in a profile, is this one active?
    bool _disabled;  // When used in a profile, is this one disabled?
    int _rank;       // When used in a profile, what is the rank? (0 being first layer)

   public:
    LayerFile();
    ~LayerFile();

    // No.. I do not like operator overloading. It's a bad idea.
    // Inlined here just so I can see all the variables that need to be copied.
    void CopyLayer(LayerFile* destination_layer_file) const {
        destination_layer_file->_file_format_version = _file_format_version;
        destination_layer_file->_name = _name;
        destination_layer_file->_type = _type;
        destination_layer_file->_library_path = _library_path;
        destination_layer_file->_api_version = _api_version;
        destination_layer_file->_implementation_version = _implementation_version;
        destination_layer_file->_description = _description;
        destination_layer_file->_layer_type = _layer_type;
        destination_layer_file->_enabled = _enabled;
        destination_layer_file->_rank = _rank;
        destination_layer_file->_disabled = _disabled;
        destination_layer_file->_layer_path = _layer_path;

        for (int i = 0; i < _layer_settings.length(); i++) {
            LayerSettings* pSettings = new LayerSettings();
            *pSettings = *_layer_settings[i];
            destination_layer_file->_layer_settings.push_back(pSettings);
        }
    }

    // File based layers
    bool ReadLayerFile(QString full_path_to_file, LayerType layer_type);

    // Utility, may move outside this class....
    static void LoadSettingsFromJson(QJsonObject& layer_settings_descriptors, QVector<LayerSettings*>& layers);
};
