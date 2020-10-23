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

#include "layer_setting.h"

#include "util.h"
#include "version.h"

#include <QJsonArray>

#include <cassert>

bool LayerSetting::Load(const QJsonObject& json_setting) {
    const QJsonValue& json_key = json_setting.value("key");
    assert(json_key != QJsonValue::Undefined);
    this->key = json_key.toString();

    const QJsonValue& json_label = json_setting.value("label");
    assert(json_label != QJsonValue::Undefined);
    this->label = json_label.toString();

    const QJsonValue& json_description = json_setting.value("description");
    assert(json_description != QJsonValue::Undefined);
    this->description = json_description.toString();

    const QJsonValue& json_type = json_setting.value("type");
    assert(json_type != QJsonValue::Undefined);
    this->type = GetSettingType(json_type.toString().toStdString().c_str());

    this->default_value.Load("default", this->type, json_setting);

    const QJsonValue& json_enum_values = json_setting.value("enum_string");
    assert((json_enum_values != QJsonValue::Undefined && IsEnum(this->type)) ||
           (json_enum_values == QJsonValue::Undefined && !IsEnum(this->type)));

    if (json_enum_values != QJsonValue::Undefined) {
        const QJsonArray& json_array = json_enum_values.toArray();
        for (int i = 0, n = json_array.size(); i < n; ++i) {
            const QJsonObject& json_object = json_array[i].toObject();

            EnumValue enum_value;
            enum_value.key = json_object.value("key").toString();
            enum_value.label = json_object.value("label").toString();
            this->enums.push_back(enum_value);
        }
    }

    return true;
}

bool LayerSetting::Save(QJsonObject& json_setting) const {
    json_setting.insert("key", this->key);
    json_setting.insert("name", this->label);
    json_setting.insert("description", this->description);
    json_setting.insert("type", GetSettingToken(this->type));
    if (IsEnum(this->type)) {
        QJsonArray enum_values;
        for (std::size_t i = 0, n = enums.size(); i < n; ++i) {
            QJsonObject enum_value;
            enum_value.insert("key", enums[i].key);
            enum_value.insert("label", enums[i].label);
            enum_values.append(enum_value);
        }
        json_setting.insert("enum_string", enum_values);
    }
    return this->default_value.Save("default", this->type, json_setting);
}

bool operator==(const LayerSetting& l, const LayerSetting& r) {
    if (l.key != r.key) return false;

    if (l.label != r.label) return false;

    if (l.status != r.status) return false;

    if (l.platform_flags != r.platform_flags) return false;

    if (l.description != r.description) return false;

    if (l.type != r.type) return false;

    if (l.enums.size() != r.enums.size()) return false;

    for (std::size_t i = 0, n = l.enums.size(); i < n; ++i) {
        if (l.enums[i].key != r.enums[i].key) return false;
        if (l.enums[i].label != r.enums[i].label) return false;
    }

    if (l.default_value != r.default_value) return false;

    return true;
}

bool operator!=(const LayerSetting& l, const LayerSetting& r) { return !(l == r); }

LayerSetting* Find(std::vector<LayerSetting>& settings, const char* key) {
    for (std::size_t i = 0, n = settings.size(); i < n; i++) {
        if (settings[i].key == key) {
            return &settings[i];
        }
    }

    return nullptr;
}

void Sort(std::vector<LayerSetting>& settings) {
    struct LayerCompare {
        bool operator()(const LayerSetting& a, const LayerSetting& b) const { return a.key < b.key; }
    };

    std::sort(settings.begin(), settings.end(), LayerCompare());
}

bool IsStringFound(const std::vector<QString>& data, const QString& token) {
    for (std::size_t i = 0, n = data.size(); i < n; ++i) {
        if (data[i] == token) return true;
    }

    return false;
}
