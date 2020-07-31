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

#include "layerfile.h"

#include <QFile>
#include <QMessageBox>

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

LayerFile::LayerFile() {
    _enabled = false;
    _disabled = false;
    _rank = 0;
}

LayerFile::~LayerFile() {
    qDeleteAll(_layer_settings.begin(), _layer_settings.end());
    _layer_settings.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief CLayerFile::readLayerFile
/// \param qsFullPathToFile - Fully qualified path to the layer json file.
/// \return true on success, false on failure.
/// Reports errors via a message box. This might be a bad idea?
/// //////////////////////////////////////////////////////////////////////////
bool LayerFile::ReadLayerFile(QString qsFullPathToFile, LayerType layerKind) {
    _layer_type = layerKind;  // Set layer type, no way to know this from the json file

    // Open the file, should be text. Read it into a
    // temporary string.
    if (qsFullPathToFile.isEmpty()) return false;

    QFile file(qsFullPathToFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
        QMessageBox msgBox;
        msgBox.setText(tr("Could not open layer file"));
        msgBox.exec();
        return false;
    }

    QString jsonText = file.readAll();
    file.close();

    _layer_path = qsFullPathToFile;

    //////////////////////////////////////////////////////
    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox msgBox;
        msgBox.setText(parseError.errorString());
        msgBox.exec();
        return false;
    }

    // Make sure it's not empty
    if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(tr("Json document is empty!"));
        msgBox.exec();
        return false;
    }

    // Populate key items about the layer
    QJsonObject jsonObject = jsonDoc.object();
    QJsonValue jsonValue = jsonObject.value("file_format_version");
    _file_format_version = jsonValue.toString();

    QJsonValue layerValue = jsonObject.value("layer");
    QJsonObject layerObject = layerValue.toObject();

    jsonValue = layerObject.value("name");
    _name = jsonValue.toString();

    jsonValue = layerObject.value("type");
    _type = jsonValue.toString();

    jsonValue = layerObject.value("library_path");
    _library_path = jsonValue.toString();

    jsonValue = layerObject.value("api_version");
    _api_version = jsonValue.toString();

    jsonValue = layerObject.value("implementation_version");
    _implementation_version = jsonValue.toString();

    jsonValue = layerObject.value("description");
    _description = jsonValue.toString();

    // The layer file is loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////
void LayerFile::LoadSettingsFromJson(QJsonObject& layerSettingsDescriptors, QVector<LayerSettings*>& layers) {
    // Okay, how many settings do we have?
    QStringList settingsNames = layerSettingsDescriptors.keys();

    for (int iSetting = 0; iSetting < settingsNames.size(); iSetting++) {
        // The layer rank may or may not be here, but it is not a
        // user setting.
        if (settingsNames[iSetting] == QString("layer_rank")) continue;

        LayerSettings* pLayerSettings = new LayerSettings;
        pLayerSettings->settings_name = settingsNames[iSetting];

        QJsonValue settingValue = layerSettingsDescriptors.value(settingsNames[iSetting]);
        QJsonObject settingObject = settingValue.toObject();

        // The easy stuff...
        QJsonValue value = settingObject.value("description");
        pLayerSettings->settings_desc = value.toString();

        value = settingObject.value("name");
        pLayerSettings->settings_prompt = value.toString();

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        value = settingObject.value("default");
        if (value.isArray()) {
            QJsonArray array = value.toArray();
            for (int a = 0; a < array.size(); a++) {
                pLayerSettings->settings_value += array[a].toString();
                if (a != array.size() - 1) pLayerSettings->settings_value += ",";
            }

        } else
            pLayerSettings->settings_value = value.toString();

        ///////////////////////////////////////////////////////////////////////
        // Everything from here down revolves around the data type
        // Data types and values start getting a little more involved.
        value = settingObject.value("type");

        QString typeString = value.toString();
        ///////////////////////////////////////////////// Exclusive Enums
        if (typeString == QString("enum")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_EXCLUSIVE_LIST;

            // Now we have a list of options, both the enum for the settings file, and the prompts
            value = settingObject.value("options");
            QJsonObject object = value.toObject();
            QStringList keys, values;
            keys = object.keys();
            for (int v = 0; v < keys.size(); v++) {
                pLayerSettings->settings_list_exclusive_value << keys[v];
                pLayerSettings->settings_list_exclusive_prompt << object.value(keys[v]).toString();
            }

            layers.push_back(pLayerSettings);
            continue;
        }

        /////////////////////////////////////////////////// Pick one or more from a list
        if (typeString == QString("multi_enum")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_INCLUSIVE_LIST;

            // Now we have a list of options, both the enum for the settings file, and the prompts
            value = settingObject.value("options");
            QJsonObject object = value.toObject();
            QStringList keys, values;
            keys = object.keys();
            for (int v = 0; v < keys.size(); v++) {
                pLayerSettings->settings_list_inclusive_value << keys[v];
                pLayerSettings->settings_list_inclusive_prompt << object.value(keys[v]).toString();
            }

            layers.push_back(pLayerSettings);
            continue;
        }

        ////////////////////////////////////////////////////// Select a file. Nice and simple
        if (typeString == QString("save_file")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_FILE;
            layers.push_back(pLayerSettings);
            continue;
        }

        ////////////////////////////////////////////////////// Load a file.
        if (typeString == QString("load_file")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_LOAD_FILE;
            layers.push_back(pLayerSettings);
            continue;
        }

        ////////////////////////////////////////////////////// Folder to put screen shots in
        if (typeString == QString("save_folder")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_SAVE_FOLDER;
            layers.push_back(pLayerSettings);
            continue;
        }

        ////////////////////////////////////////////////////// Bool, also nice and simple ("true"/"false")
        if (typeString == QString("bool")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_BOOL;
            layers.push_back(pLayerSettings);
            continue;
        }

        //////////////////////////////////////////////////// Bool, but written out as 0 or 1
        if (typeString == QString("bool_numeric")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_BOOL_NUMERIC;
            layers.push_back(pLayerSettings);
            continue;
        }

        //////////////////////////////////////////////////// VUID Filter List
        if (typeString == QString("vuid_exclude")) {
            pLayerSettings->settings_type = LAYER_SETTINGS_VUID_FILTER;
            layers.push_back(pLayerSettings);
            continue;
        }

        ////////////////////////////////////////////////////// Just a string please
        if (typeString == QString("string")) pLayerSettings->settings_type = LAYER_SETTINGS_STRING;

        layers.push_back(pLayerSettings);
    }

    return;
}
