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

#include "configuration_setting.h"
#include "layer_setting.h"

#include <QJsonArray>

#include <cassert>

ConfigurationSetting::ConfigurationSetting(const LayerSetting& layer_setting)
    : key(layer_setting.key), value(layer_setting.default_value) {}

bool ConfigurationSetting::Load(const SettingType type, const QJsonObject& json_setting) {
    const QJsonValue& json_key = json_setting.value("key");
    assert(json_key != QJsonValue::Undefined);
    this->key = json_key.toString();

    const QJsonValue& json_value = json_setting.value("value");
    assert(json_value != QJsonValue::Undefined);

    switch (type) {
        case SETTING_VUID_FILTER:
        case SETTING_INCLUSIVE_LIST: {
            assert(json_value.isArray());
            const QJsonArray& json_array = json_value.toArray();
            for (int i = 0, n = json_array.size(); i < n; ++i) {
                this->value.push_back(QVariant(json_array[i].toString()));
            }
            break;
        }
        case SETTING_EXCLUSIVE_LIST:
        case SETTING_STRING:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FILE:
        case SETTING_SAVE_FOLDER: {
            assert(json_value.isString());
            this->value.push_back(QVariant(json_value.toString()));
            break;
        }
        case SETTING_BOOL_NUMERIC:
        case SETTING_INT: {
            assert(!json_value.isArray());
            this->value.push_back(QVariant(json_value.toInt()));
            break;
        }
        case SETTING_RANGE_INT: {
            assert(json_value.isArray());
            const QJsonArray& json_array = json_value.toArray();
            assert(json_array.size() == 2);
            for (int i = 0, n = 2; i < n; ++i) {
                this->value.push_back(QVariant(json_array[i].toInt()));
            }
            break;
        }
        case SETTING_BOOL: {
            assert(json_value.isBool());
            this->value.push_back(QVariant(json_value.toBool()));
            break;
        }
        default: {
            assert(0);
            break;
        }
    }

    return true;
}

bool ConfigurationSetting::Save(const SettingType type, QJsonObject& json_setting) const {
    json_setting.insert("key", this->key);

    switch (type) {
        case SETTING_VUID_FILTER:
        case SETTING_INCLUSIVE_LIST: {
            QJsonArray json_array;
            for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
                json_array.append(this->value[i].toString());
            }
            json_setting.insert("value", json_array);
            break;
        }
        case SETTING_EXCLUSIVE_LIST:
        case SETTING_STRING:
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FILE:
        case SETTING_SAVE_FOLDER: {
            assert(this->value.size() == 1);
            json_setting.insert("value", this->value[0].toString());
            break;
        }
        case SETTING_BOOL_NUMERIC:
        case SETTING_INT: {
            assert(this->value.size() == 1);
            json_setting.insert("value", this->value[0].toInt());
            break;
        }
        case SETTING_RANGE_INT: {
            assert(this->value.size() == 2);
            QJsonArray json_array;
            for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
                json_array.append(this->value[i].toInt());
            }
            json_setting.insert("value", json_array);
            break;
        }
        case SETTING_BOOL: {
            assert(this->value.size() == 1);
            json_setting.insert("value", this->value[0].toBool());
            break;
        }
        default: {
            assert(0);
            break;
        }
    }

    return true;
}

bool operator==(const ConfigurationSetting& l, const ConfigurationSetting& r) {
    if (l.key != r.key) return false;
    if (l.value.size() != r.value.size()) return false;
    for (std::size_t i = 0, n = l.value.size(); i < n; ++i) {
        if (l.value[i] != r.value[i]) return false;
    }

    return true;
}

bool operator!=(const ConfigurationSetting& l, const ConfigurationSetting& r) { return !(l == r); }
