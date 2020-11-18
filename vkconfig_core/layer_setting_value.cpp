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

#include "layer_setting_value.h"
#include "util.h"
#include "path.h"

#include <QJsonArray>
#include <QString>

#include <cassert>

SettingValue::SettingValue(const char* key, const QVariant& value) : setting_key(key) { setting_value.push_back(value); }

SettingValue::SettingValue(const char* key, const std::vector<QVariant>& value) : setting_key(key), setting_value(value) {}

const char* SettingValue::Key() const { return setting_key.c_str(); }

bool SettingValue::Empty() const { return this->setting_value.empty(); }

std::size_t SettingValue::Size() const { return this->setting_value.size(); }

bool SettingValue::Load(const char* key, const SettingType type, const QJsonObject& json_object) {
    assert(key != nullptr);
    assert(!json_object.isEmpty());

    setting_key = key;

    const QJsonValue& json_value = json_object.value(key);
    assert(json_value != QJsonValue::Undefined);

    switch (type) {
        case SETTING_VUID_FILTER:
        case SETTING_INCLUSIVE_LIST: {
            assert(json_value.isArray());
            const QJsonArray& json_array = json_value.toArray();
            for (int i = 0, n = json_array.size(); i < n; ++i) {
                setting_value.push_back(QVariant(json_array[i].toString()));
            }
            break;
        }
        case SETTING_EXCLUSIVE_LIST:
        case SETTING_STRING:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FOLDER: {
            assert(json_value.isString());
            setting_value.push_back(QVariant(json_value.toString()));
            break;
        }
        case SETTING_SAVE_FILE: {
            assert(json_value.isString());
            const QString& value = json_value.toString();
            const QString path(ReplacePathBuiltInVariables(ValidatePath(value.toStdString())).c_str());
            setting_value.push_back(path);
            break;
        }
        case SETTING_BOOL_NUMERIC:
        case SETTING_INT: {
            assert(!json_value.isArray());
            setting_value.push_back(QVariant(json_value.toInt()));
            break;
        }
        case SETTING_RANGE_INT: {
            assert(json_value.isArray());
            const QJsonArray& json_array = json_value.toArray();
            assert(json_array.size() == 2);
            for (int i = 0, n = 2; i < n; ++i) {
                setting_value.push_back(QVariant(json_array[i].toInt()));
            }
            break;
        }
        case SETTING_BOOL: {
            assert(json_value.isBool());
            setting_value.push_back(QVariant(json_value.toBool()));
            break;
        }
        default: {
            assert(0);
            return false;
        }
    }

    return true;
}

bool SettingValue::Save(const char* key, const SettingType type, QJsonObject& json_object) const {
    assert(key != nullptr);

    switch (type) {
        case SETTING_VUID_FILTER:
        case SETTING_INCLUSIVE_LIST: {
            QJsonArray json_array;
            for (std::size_t i = 0, n = this->setting_value.size(); i < n; ++i) {
                json_array.append(this->setting_value[i].toString());
            }
            json_object.insert(key, json_array);
            break;
        }
        case SETTING_EXCLUSIVE_LIST:
        case SETTING_STRING:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FILE:
        case SETTING_SAVE_FOLDER: {
            assert(this->setting_value.size() == 1);
            json_object.insert(key, this->setting_value[0].toString());
            break;
        }
        case SETTING_BOOL_NUMERIC:
        case SETTING_INT: {
            assert(this->setting_value.size() == 1);
            json_object.insert(key, this->setting_value[0].toInt());
            break;
        }
        case SETTING_RANGE_INT: {
            assert(this->setting_value.size() == 2);
            QJsonArray json_array;
            for (std::size_t i = 0, n = this->setting_value.size(); i < n; ++i) {
                json_array.append(this->setting_value[i].toInt());
            }
            json_object.insert(key, json_array);
            break;
        }
        case SETTING_BOOL: {
            assert(this->setting_value.size() == 1);
            json_object.insert(key, this->setting_value[0].toBool());
            break;
        }
        default: {
            assert(0);
            break;
        }
    }

    return true;
}

bool operator==(const SettingValue& l, const SettingValue& r) {
    if (l.Size() != r.Size()) return false;

    for (std::size_t i = 0, n = l.Size(); i < n; ++i) {
        if (l[i] != r[i]) return false;
    }

    return true;
}

bool operator!=(const SettingValue& l, const SettingValue& r) { return !(l == r); }
