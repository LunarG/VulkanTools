#ifndef CLAYERFILE_H
#define CLAYERFILE_H
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

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QVector>
#include <QVariant>

typedef enum { LAYER_TYPE_EXPLICIT = 0, LAYER_TYPE_IMPLICIT, LAYER_TYPE_CUSTOM } TLayerType;

typedef enum { LAYER_SETTINGS_STRING = 0, LAYER_SETTINGS_FILE, LAYER_SETTINGS_BOOL,
                LAYER_SETTINGS_EXCLUSIVE_LIST, LAYER_SETTINGS_INCLUSIVE_LIST, LAYER_SETTINGS_RANGE_INT } TLayerSettingsType;

// This structure is copied by assignment elsewhere, so do not add
// any pointers to it please...
struct TLayerSettings {
    QString                 settingsName;                   // Name of the setting the layer looks for (programatic variable name)
    QString                 settingsPrompt;                 // Short name to prompt end user
    QString                 settingsDesc;                   // Human version, describes the setting
    TLayerSettingsType      settingsType;                   // The data type
    QVariant                settingsMaxValue;               // For range based
    QVariant                settingsMinValue;               // For range based
    QStringList             settingsListExclusiveValue;     // List of exclusive items
    QStringList             settingsListExclusivePrompt;    // List of exclusive item prompts
    QStringList             settingsListInclusiveValue;     // List of non-exclusive items (more than one item can be selected)
    QStringList             settingsListInclusivePrompt;    // List of non-exclusive item prompts (more than one item can be selected)
    QString                 settingsValue;                  // Default value as a string
    bool                    commonKhronosEdit;              // This setting is displayed only on the front panel.
    };


class CLayerFile : public QObject
{
    Q_OBJECT
public:
    // Standard pieces of a layer
    QString     file_format_version;
    QString     name;
    QString     type;
    QString     library_path;
    QString     api_version;
    QString     implementation_version;
    QString     description;

    QString     qsCustomLayerPath; // Only used by custom layer paths

    TLayerType  layerType;

    // This layers settings. This will be used to build the editor
    // as well as create settings files. This CAN be empty if the
    // layer doens't have any settings.
    QVector<TLayerSettings *>layerSettings;


    bool        bActive;            // When used in a profile, is this one active?
    bool        bDisabled;          // When used in a profile, is this one disabled?
    int         nRank;              // When used in a profile, what is the rank? (0 being first layer)

public:
    CLayerFile();
    ~CLayerFile();

    // No.. I do not like operator overloading. It's a bad idea.
    // Inlined here just so I can see all the variables that need to be copied.
    void CopyLayer(CLayerFile* destinationLayer) const {
        destinationLayer->file_format_version = file_format_version;
        destinationLayer->name = name;
        destinationLayer->type = type;
        destinationLayer->library_path = library_path;
        destinationLayer->api_version = api_version;
        destinationLayer->implementation_version = implementation_version;
        destinationLayer->description = description;
        destinationLayer->layerType = layerType;
        destinationLayer->bActive = bActive;
        destinationLayer->nRank = nRank;
        destinationLayer->bDisabled = bDisabled;
        destinationLayer->qsCustomLayerPath = qsCustomLayerPath;

        for(int i = 0; i < layerSettings.length(); i++) {
            TLayerSettings *pSettings = new TLayerSettings();
            *pSettings = *layerSettings[i];
            destinationLayer->layerSettings.push_back(pSettings);
            }
        }

    // File based layers
    bool readLayerFile(QString qsFullPathToFile, TLayerType layerKind);

    // Utility, may move outside this class....
    static bool loadSettingsFromJson(QJsonObject& layerSettingsDescriptors, QVector<TLayerSettings *>& layers);

};

#endif // CLAYERFILE_H
