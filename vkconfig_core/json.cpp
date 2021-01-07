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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "json.h"

#include <QJsonArray>

#include <cassert>

QJsonObject ReadObject(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);
    assert(json_value.isObject());
    return json_value.toObject();
}

QJsonArray ReadArray(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);
    assert(json_value.isArray());
    return json_value.toArray();
}

std::string ReadStringValue(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);
    assert(json_value.isString());
    return json_value.toString().toStdString();
}

std::vector<std::string> ReadStringArray(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);
    assert(json_value.isArray());

    std::vector<std::string> string_array;

    const QJsonArray& json_array = json_value.toArray();
    for (int i = 0, n = json_array.size(); i < n; ++i) {
        string_array.push_back(json_array[i].toString().toStdString());
    }

    return string_array;
}

std::string ReadString(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);

    if (json_value.isArray()) {
        std::string result;

        const QJsonArray& array = json_value.toArray();
        for (int a = 0, n = array.size(); a < n; ++a) {
            result += array[a].toString().toStdString();
            if (a != n - 1) {
                result += ",";
            }
        }

        return result;
    } else {
        return json_value.toString().toStdString();
    }
}

bool ReadBoolValue(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);
    assert(json_value.isBool());
    return json_value.toBool();
}

int ReadIntValue(const QJsonObject& json_object, const char* key) {
    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);
    assert(!json_value.isArray());
    return json_value.toInt();
}

Version ReadVersionValue(const QJsonObject& json_object, const char* key) {
    const std::string& version_string = ReadStringValue(json_object, key);
    assert(!version_string.empty());
    return Version(version_string.c_str());
}

void SaveStringArray(QJsonObject& json_object, const char* key, const std::vector<std::string>& tokens) {
    QJsonArray json_platforms;
    for (std::size_t i = 0, n = tokens.size(); i < n; ++i) {
        json_platforms.append(tokens[i].c_str());
    }
    json_object.insert(key, json_platforms);
}
