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

#include "layerfile.h"

#include <QFile>
#include <QMessageBox>


CLayerFile::CLayerFile()
    {
    bActive = false;
    bDisabled = false;
    nRank = 0;
    }

CLayerFile::~CLayerFile()
    {
    qDeleteAll(layerSettings.begin(), layerSettings.end());
    layerSettings.clear();
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief CLayerFile::readLayerFile
/// \param qsFullPathToFile - Fully qualified path to the layer json file.
/// \return true on success, false on failure.
/// Reports errors via a message box. This might be a bad idea?
/// //////////////////////////////////////////////////////////////////////////
bool CLayerFile::readLayerFile(QString qsFullPathToFile, TLayerType layerKind)
{
    layerType = layerKind;           // Set layer type, no way to know this from the json file

    // Open the file, should be text. Read it into a
    // temporary string.
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

     qsLayerPath = qsFullPathToFile;

     //////////////////////////////////////////////////////
     // Convert the text to a JSON document & validate it.
     // It does need to be a valid json formatted file.
     QJsonDocument jsonDoc;
     QJsonParseError parseError;
     jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);
    if(parseError.error != QJsonParseError::NoError) {
         QMessageBox msgBox;
         msgBox.setText(parseError.errorString());
         msgBox.exec();
        return false;
        }

    // Make sure it's not empty
    if(jsonDoc.isNull() || jsonDoc.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(tr("Json document is empty!"));
        msgBox.exec();
        return false;
        }

    // Populate key items about the layer
    QJsonObject jsonObject = jsonDoc.object();
    QJsonValue  jsonValue = jsonObject.value("file_format_version");
    file_format_version = jsonValue.toString();

    QJsonValue layerValue = jsonObject.value("layer");
    QJsonObject layerObject = layerValue.toObject();

    jsonValue = layerObject.value("name");
    name = jsonValue.toString();

    jsonValue = layerObject.value("type");
    type = jsonValue.toString();

    jsonValue = layerObject.value("library_path");
    library_path = jsonValue.toString();

    jsonValue = layerObject.value("api_version");
    api_version = jsonValue.toString();

    jsonValue = layerObject.value("implementation_version");
    implementation_version = jsonValue.toString();

    jsonValue = layerObject.value("description");
    description = jsonValue.toString();

    // The layer file is loaded
    return true;
}



////////////////////////////////////////////////////////////////////////////
bool CLayerFile::loadSettingsFromJson(QJsonObject& layerSettingsDescriptors, QVector<TLayerSettings *>& layers)
    {
    bool bHasKhronos = false;

    // Okay, how many settings do we have?
    QStringList settingsNames = layerSettingsDescriptors.keys();

    for(int iSetting = 0; iSetting < settingsNames.size(); iSetting++) {
        if(settingsNames[iSetting] == QString("layer_rank"))
            continue;

        TLayerSettings *pLayerSettings = new TLayerSettings;
        pLayerSettings->settingsName = settingsNames[iSetting];

        QJsonValue settingValue = layerSettingsDescriptors.value(settingsNames[iSetting]);
        QJsonObject settingObject = settingValue.toObject();

        // This may or may not be present, it is false by default
        QJsonValue commonEdit = settingObject.value("common_edit");
        if(commonEdit.isUndefined())  // Default if no key is present
            pLayerSettings->commonKhronosEdit = false; // DNE - force to false
        else
             pLayerSettings->commonKhronosEdit = commonEdit.toBool();

        if(pLayerSettings->commonKhronosEdit)
            bHasKhronos = true;

        // The easy stuff...
        QJsonValue value = settingObject.value("description");
        pLayerSettings->settingsDesc = value.toString();

        value = settingObject.value("name");
        pLayerSettings->settingsPrompt = value.toString();

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        value = settingObject.value("default");
        if(value.isArray()) {
            QJsonArray array = value.toArray();
            for(int a = 0; a < array.size(); a++) {
                pLayerSettings->settingsValue += array[a].toString();
                if(a != array.size() -1)
                    pLayerSettings->settingsValue += ",";
            }

        } else
            pLayerSettings->settingsValue = value.toString();

        ///////////////////////////////////////////////////////////////////////
        // Everything from here down revolves around the data type
        // Data types and values start getting a little more involved.
        value = settingObject.value("type");

        QString typeString = value.toString();
        ///////////////////////////////////////////////// Exclusive Enums
        if(typeString == QString("enum")) {
            pLayerSettings->settingsType = LAYER_SETTINGS_EXCLUSIVE_LIST;

            // Now we have a list of options, both the enum for the settings file, and the prompts
            value = settingObject.value("options");
            QJsonObject object = value.toObject();
            QStringList keys, values;
            keys = object.keys();
            for(int v = 0; v < keys.size(); v++) {
                pLayerSettings->settingsListExclusiveValue << keys[v];
                pLayerSettings->settingsListExclusivePrompt << object.value(keys[v]).toString();
                }

            layers.push_back(pLayerSettings);
            continue;
            }

        /////////////////////////////////////////////////// Pick one or more from a list
        if(typeString == QString("multi_enum")) {
            pLayerSettings->settingsType = LAYER_SETTINGS_INCLUSIVE_LIST;

            // Now we have a list of options, both the enum for the settings file, and the prompts
            value = settingObject.value("options");
            QJsonObject object = value.toObject();
            QStringList keys, values;
            keys = object.keys();
            for(int v = 0; v < keys.size(); v++) {
                pLayerSettings->settingsListInclusiveValue << keys[v];
                pLayerSettings->settingsListInclusivePrompt << object.value(keys[v]).toString();
                }

            layers.push_back(pLayerSettings);
            continue;
            }

        ////////////////////////////////////////////////////// Select a file. Nice and simple
        if(typeString == QString("save_file")) {
            pLayerSettings->settingsType = LAYER_SETTINGS_FILE;
            layers.push_back(pLayerSettings);
            continue;
            }

        ////////////////////////////////////////////////////// Bool, also nice and simple
        if(typeString == QString("bool")) {
            pLayerSettings->settingsType = LAYER_SETTINGS_BOOL;
            layers.push_back(pLayerSettings);
            continue;
            }

        ////////////////////////////////////////////////////// Just a string please
        if(typeString == QString("string"))
            pLayerSettings->settingsType = LAYER_SETTINGS_STRING;


        layers.push_back(pLayerSettings);
        }

    return bHasKhronos;
    }

