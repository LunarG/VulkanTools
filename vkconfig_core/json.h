/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "version.h"
#include "util.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <string>
#include <vector>

QJsonDocument ParseJsonFile(const char* file);

// Read an object from the json_object
QJsonObject ReadObject(const QJsonObject& json_object, const char* key);

// Read an array from the json_object
QJsonArray ReadArray(const QJsonObject& json_object, const char* key);

// Read a string value from the json_object
std::string ReadStringValue(const QJsonObject& json_object, const char* key);

// Read a string value from the json_object
NumberOrString ReadNumberOrStringValue(const QJsonObject& json_object, const char* key);

// Read a string value from the json_object
std::vector<std::string> ReadStringArray(const QJsonObject& json_object, const char* key);

// Read a concatenated string from the json_object
std::string ReadString(const QJsonObject& json_object, const char* key);

// Read an int value from the json_object
int ReadIntValue(const QJsonObject& json_object, const char* key);

// Read a float value from the json_object
float ReadFloatValue(const QJsonObject& json_object, const char* key);

// Read a bool value from the json_object
bool ReadBoolValue(const QJsonObject& json_object, const char* key);

// Read a Version from the json_object
Version ReadVersionValue(const QJsonObject& json_object, const char* key);

// Save array of strings into a jsong object
void SaveStringArray(QJsonObject& json_object, const char* key, const std::vector<std::string>& tokens);
