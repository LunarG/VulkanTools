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

// delimited string is a comma delimited string. If value is found remove it
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

// Pretty simple, add to list if it's not already in it
void AppendString(QString& delimitedString, QString value) {
    // Do I have anything to do?
    if (delimitedString.contains(value))  // Nope
        return;

    if (!delimitedString.isEmpty()) delimitedString += ",";

    delimitedString += value;
}

Layer::Layer() {}

Layer::Layer(const QString& name, const LayerType layer_type) : name(name), _layer_type(layer_type) {}

Layer::Layer(const QString& name, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
             const QString& implementation_version, const QString& library_path, const QString& type)
    : name(name),
      _layer_type(layer_type),
      _file_format_version(file_format_version),
      _api_version(api_version),
      _implementation_version(implementation_version),
      _library_path(library_path),
      _type(type) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return _file_format_version != Version::VERSION_NULL && !name.isEmpty() && !_type.isEmpty() && !_library_path.isEmpty() &&
           _api_version != Version::VERSION_NULL && !_implementation_version.isEmpty();
}

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
    name = json_value.toString();

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

    json_value = layer_object.value("default_preset_index");
    _default_preset_index = json_value.toInt();

    // The layer file is loaded
    return IsValid();  // Not all JSON file are layer JSON valid
}
