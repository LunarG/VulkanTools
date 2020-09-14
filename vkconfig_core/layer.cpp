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

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/platform.h"

#include <QFile>
#include <QMessageBox>
#include <QJsonArray>

#include <cassert>

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

Layer::Layer() {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return _file_format_version != Version::VERSION_NULL && !_name.isEmpty() && !_type.isEmpty() && !_library_path.isEmpty() &&
           _api_version != Version::VERSION_NULL && !_implementation_version.isEmpty();
}

///////////////////////////////////////////////////////////////////////////////
/// Reports errors via a message box. This might be a bad idea?
/// //////////////////////////////////////////////////////////////////////////
bool Layer::Load(QString full_path_to_file, LayerType layer_type) {
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
    QJsonParseError parseError;
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(json_text.toUtf8(), &parseError);
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
    _file_format_version = Version(json_value.toString());

    QJsonValue layer_value = json_object.value("layer");
    QJsonObject layer_object = layer_value.toObject();

    json_value = layer_object.value("name");
    _name = json_value.toString();

    json_value = layer_object.value("type");
    _type = json_value.toString();

    json_value = layer_object.value("library_path");
    _library_path = json_value.toString();

    json_value = layer_object.value("api_version");
    _api_version = Version(json_value.toString());

    json_value = layer_object.value("implementation_version");
    _implementation_version = json_value.toString();

    json_value = layer_object.value("description");
    _description = json_value.toString();

    // The layer file is loaded
    return IsValid();  // Not all JSON file are layer JSON valid
}

bool Layer::operator==(const Layer& layer) const {
    if (_file_format_version != layer._file_format_version)
        return false;
    else if (_name != layer._name)
        return false;
    else if (_type != layer._type)
        return false;
    else if (_library_path != layer._library_path)
        return false;
    else if (_api_version != layer._api_version)
        return false;
    else if (_implementation_version != layer._implementation_version)
        return false;
    else if (_description != layer._description)
        return false;
    else if (_layer_path != layer._layer_path)
        return false;
    else if (_layer_type != layer._layer_type)
        return false;

    //    for (std::size_t i = 0, n = _layer_settings.size(); i < n; ++i)
    //        if (_layer_settings[i] != layer._layer_settings[i]) return false;

    return true;
}

bool Layer::operator!=(const Layer& layer) const { return !(*this == layer); }

const Layer* FindLayer(const QVector<Layer*>& layers, QString layer_name) {
    assert(!layer_name.isEmpty());

    for (int i = 0; i < layers.size(); ++i) {
        const Layer* layer = layers[i];

        if (layer_name != layer->_name) continue;
        return layer;
    }

    return nullptr;
}
