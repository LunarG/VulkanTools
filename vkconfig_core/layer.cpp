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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#include "../vkconfig_core/layer.h"

#include <QFile>
#include <QMessageBox>
#include <QJsonArray>

////////// A couple of utility functions for building/modifying enable/disable lists

///////////////////////////////////////////////////////////////////////////////
// delimted string is a comma delimited string. If value is found remove it
void RemoveString(QString& delimitedString, QString value) {
    // Well, it's not there now is it...
    if (!delimitedString.contains(value)) return;

    QStringList list = delimitedString.split(",");
    for (int i = 0; i < list.size(); i++)
        if (list[i] == value) {
            list.removeAt(i);
            break;
        }

    delimitedString = list.join(",");
}

/////////////////////////////////////////////////////////////////////////////////
// Pretty simple, add to list if it's not already in it
void AddString(QString& delimitedString, QString value) {
    // Do I have anything to do?
    if (delimitedString.contains(value))  // Nope
        return;

    if (!delimitedString.isEmpty()) delimitedString += ",";

    delimitedString += value;
}

Layer::Layer() : _state(LAYER_STATE_APPLICATION_CONTROLLED), _rank(0) {}

Layer::~Layer() {
    qDeleteAll(_layer_settings.begin(), _layer_settings.end());
    _layer_settings.clear();
}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return !_name.isEmpty() && !_type.isEmpty() && !_library_path.isEmpty() && !_api_version.isEmpty() &&
           !_implementation_version.isEmpty();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CLayerFile::readLayerFile
/// \param qsFullPathToFile - Fully qualified path to the layer json file.
/// \return true on success, false on failure.
/// Reports errors via a message box. This might be a bad idea?
/// //////////////////////////////////////////////////////////////////////////
bool Layer::ReadLayerFile(QString full_path_to_file, LayerType layer_type) {
    _layer_type = layer_type;  // Set layer type, no way to know this from the json file

    // Open the file, should be text. Read it into a
    // temporary string.
    if (full_path_to_file.isEmpty()) return false;

    QFile file(full_path_to_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
        QMessageBox message_box;
        message_box.setText("Could not open layer file");
        message_box.exec();
        return false;
    }

    QString json_text = file.readAll();
    file.close();

    _layer_path = full_path_to_file;

    //////////////////////////////////////////////////////
    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(json_text.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox message_box;
        message_box.setText(parseError.errorString());
        message_box.exec();
        return false;
    }

    // Make sure it's not empty
    if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
        QMessageBox message_box;
        message_box.setText("Json document is empty!");
        message_box.exec();
        return false;
    }

    // Populate key items about the layer
    QJsonObject json_object = jsonDoc.object();
    QJsonValue json_value = json_object.value("file_format_version");
    _file_format_version = json_value.toString();

    QJsonValue layer_value = json_object.value("layer");
    QJsonObject layer_object = layer_value.toObject();

    json_value = layer_object.value("name");
    _name = json_value.toString();

    json_value = layer_object.value("type");
    _type = json_value.toString();

    json_value = layer_object.value("library_path");
    _library_path = json_value.toString();

    json_value = layer_object.value("api_version");
    _api_version = json_value.toString();

    json_value = layer_object.value("implementation_version");
    _implementation_version = json_value.toString();

    json_value = layer_object.value("description");
    _description = json_value.toString();

    // The layer file is loaded
    return IsValid();  // Not all JSON file are layer JSON valid
}

////////////////////////////////////////////////////////////////////////////
void Layer::LoadSettingsFromJson(QJsonObject& json_layer_settings, QVector<LayerSetting*>& settings) {
    // Okay, how many settings do we have?
    QStringList settings_names = json_layer_settings.keys();

    for (int setting_index = 0, setting_count = settings_names.size(); setting_index < setting_count; setting_index++) {
        // The layer rank may or may not be here, but it is not a
        // user setting.
        if (settings_names[setting_index] == QString("layer_rank")) continue;

        LayerSetting* setting = new LayerSetting;
        setting->name = settings_names[setting_index];

        QJsonValue json_value = json_layer_settings.value(settings_names[setting_index]);
        QJsonObject json_object = json_value.toObject();

        // The easy stuff...
        QJsonValue value = json_object.value("description");
        setting->description = value.toString();

        value = json_object.value("name");
        setting->label = value.toString();

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        value = json_object.value("default");
        if (value.isArray()) {
            QJsonArray array = value.toArray();
            for (int a = 0; a < array.size(); a++) {
                setting->value += array[a].toString();
                if (a != array.size() - 1) setting->value += ",";
            }

        } else
            setting->value = value.toString();

        ///////////////////////////////////////////////////////////////////////
        // Everything from here down revolves around the data type
        // Data types and values start getting a little more involved.
        value = json_object.value("type");

        setting->type = GetSettingType(value.toString().toUtf8().constData());

        switch (setting->type) {
            case SETTING_EXCLUSIVE_LIST: {
                // Now we have a list of options, both the enum for the settings file, and the prompts
                value = json_object.value("options");
                QJsonObject object = value.toObject();
                QStringList keys, values;
                keys = object.keys();
                for (int v = 0; v < keys.size(); v++) {
                    setting->exclusive_values << keys[v];
                    setting->exclusive_labels << object.value(keys[v]).toString();
                }
            } break;
            case SETTING_INCLUSIVE_LIST: {
                // Now we have a list of options, both the enum for the settings file, and the prompts
                value = json_object.value("options");
                QJsonObject object = value.toObject();
                QStringList keys, values;
                keys = object.keys();
                for (int v = 0; v < keys.size(); v++) {
                    setting->inclusive_values << keys[v];
                    setting->inclusive_labels << object.value(keys[v]).toString();
                }
            } break;
            case SETTING_SAVE_FILE:
            case SETTING_LOAD_FILE:
            case SETTING_SAVE_FOLDER:
            case SETTING_BOOL:
            case SETTING_BOOL_NUMERIC:
            case SETTING_VUID_FILTER:
            case SETTING_STRING:
                break;
            default:
                assert(0);
                break;
        }

        settings.push_back(setting);
    }
}
