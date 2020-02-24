#ifndef CLAYERFILE_H
#define CLAYERFILE_H
/*
 * Copyright (c) 2018-2020 Valve Corporation
 * Copyright (c) 2018-2020 LunarG, Inc.
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
#include <QString>


typedef enum { LAYER_TYPE_EXPLICIT = 0, LAYER_TYPE_IMPLICIT } TLayerType;

class CLayerFile : public QObject, QJsonDocument
{
    Q_OBJECT
public:
    QString     file_format_version;
    QString     name;
    QString     type;
    QString     library_path;
    QString     api_version;
    QString     implementation_version;
    QString     description;

    TLayerType  layerType;

public:
    CLayerFile();

    // File based layers
    bool readLayerFile(QString qsFullPathToFile, TLayerType layerKind);

};

#endif // CLAYERFILE_H
