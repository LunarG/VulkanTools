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

#include "setting_list.h"
#include "json.h"
#include "layer.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

static QString ReadAll(const std::string& path) {
    QString json_text;

    QFile file(path.c_str());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        json_text = file.readAll();
        file.close();
    }

    return json_text;
}

void LoadVUIDs(std::vector<NumberOrString>& value) {
    const std::string vulkan_sdk_path(qgetenv("VULKAN_SDK").toStdString());
    const std::string path = vulkan_sdk_path.empty() ? "/usr" : vulkan_sdk_path;

    QString json_text;

    if (!vulkan_sdk_path.empty() && json_text.isEmpty()) {
        json_text = ReadAll(vulkan_sdk_path + "/share/vulkan/registry/validusage.json");
    }

    if (json_text.isEmpty()) {
        json_text = ReadAll(":/layers/validusage.json");
    }

    if (json_text.isEmpty()) {
        return;
    }

    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonParseError json_parse_error;
    const QJsonDocument& json_document = QJsonDocument::fromJson(json_text.toUtf8(), &json_parse_error);

    const QJsonObject& json_root_object = json_document.object();
    if (json_root_object.value("validation") == QJsonValue::Undefined) {
        return;
    }

    const QJsonObject& json_validation_object = json_root_object.value("validation").toObject();

    const QStringList& keys_depth0 = json_validation_object.keys();
    for (int i = 0, n = keys_depth0.size(); i < n; ++i) {
        const QJsonValue& value_depth1 = json_validation_object.value(keys_depth0[i]);
        assert(value_depth1 != QJsonValue::Undefined);

        const QJsonObject& object_depth1 = value_depth1.toObject();
        const QStringList& keys_depth1 = object_depth1.keys();

        for (int j = 0, o = keys_depth1.size(); j < o; ++j) {
            const QJsonValue& value_depth2 = object_depth1.value(keys_depth1[j]);
            assert(value_depth2 != QJsonValue::Undefined);

            const QJsonArray& json_array = value_depth2.toArray();
            for (int l = 0, q = json_array.size(); l < q; ++l) {
                const QString vuid_value = json_array[l].toObject().value("vuid").toString();

                NumberOrString data;
                data.key = vuid_value.toStdString();
                value.push_back(data);
            }
        }
    }
}

// SettingMetaList

const SettingType SettingMetaList::TYPE(SETTING_LIST);

SettingMetaList::SettingMetaList(Layer& layer, const std::string& key) : SettingMeta(layer, key, TYPE), list_only(false) {}

SettingData* SettingMetaList::Instantiate() {
    SettingData* setting_data = new SettingDataList(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaList::Load(const QJsonObject& json_setting) {
    if (json_setting.value("list") != QJsonValue::Undefined) {
        const QJsonArray& json_list_array = ReadArray(json_setting, "list");
        for (int i = 0, n = json_list_array.size(); i < n; ++i) {
            NumberOrString key;

            if (json_list_array[i].isString()) {
                key.key = json_list_array[i].toString().toStdString();
            } else {
                key.number = json_list_array[i].toInt();
            }

            this->list.push_back(key);
        }
    }

    if (this->layer.key == "VK_LAYER_KHRONOS_validation") {
        ::LoadVUIDs(this->list);
    }

    std::sort(this->list.begin(), this->list.end());

    if (json_setting.value("list_only") != QJsonValue::Undefined) {
        this->list_only = ReadBoolValue(json_setting, "list_only");
    }

    const QJsonArray& json_default = ReadArray(json_setting, "default");
    for (int i = 0, n = json_default.size(); i < n; ++i) {
        const QJsonObject& json_object = json_default[i].toObject();

        const NumberOrString& number_or_string = ReadNumberOrStringValue(json_object, "key");

        EnabledNumberOrString enabled_string;
        enabled_string.key = number_or_string.key;
        enabled_string.number = number_or_string.number;
        enabled_string.enabled = ReadBoolValue(json_object, "enabled");
        this->default_value.push_back(enabled_string);
    }

    return true;
}

std::string SettingMetaList::Export(ExportMode export_mode) const {
    (void)export_mode;

    std::string result;

    for (std::size_t i = 0, n = this->default_value.size(); i < n; ++i) {
        if (!this->default_value[i].enabled) continue;

        if (i != 0) {
            result += ",";
        }
        if (this->default_value[i].key.empty()) {
            result += this->default_value[i].number;
        } else {
            result += this->default_value[i].key.c_str();
        }
    }

    return result;
}

bool SettingMetaList::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaList& meta = static_cast<const SettingMetaList&>(other);

    if (this->list != meta.list) {
        return false;
    }

    if (this->default_value != meta.default_value) {
        return false;
    }

    return true;
}

// SettingDataList

SettingDataList::SettingDataList(const SettingMetaList* meta) : SettingData(meta->key, meta->type), meta(meta) {}

void SettingDataList::Reset() { this->value = this->meta->default_value; }

void SettingDataList::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataList* setting_data = static_cast<const SettingDataList*>(data);
    this->value = setting_data->value;
}

bool SettingDataList::Load(const QJsonObject& json_setting) {
    this->value.clear();

    const QJsonArray& values = ReadArray(json_setting, "value");
    for (int i = 0, n = values.size(); i < n; ++i) {
        EnabledNumberOrString value;

        if (values[i].isObject()) {
            const QJsonObject& object = values[i].toObject();

            const NumberOrString& number_or_string = ReadNumberOrStringValue(object, "key");

            value.key = number_or_string.key;
            value.number = number_or_string.number;
            value.enabled = ReadBoolValue(object, "enabled");
        } else {
            value.key = values[i].toString().toStdString();
            value.enabled = true;
        }
        this->value.push_back(value);
    }

    return true;
}

bool SettingDataList::Save(QJsonObject& json_setting) const {
    QJsonArray json_array;

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        QJsonObject object;
        if (this->value[i].key.empty()) {
            object.insert("key", this->value[i].number);
        } else {
            object.insert("key", this->value[i].key.c_str());
        }
        object.insert("enabled", this->value[i].enabled);
        json_array.append(object);
    }

    json_setting.insert("value", json_array);

    return true;
}

std::string SettingDataList::Export(ExportMode export_mode) const {
    (void)export_mode;

    std::string result;

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        if (!this->value[i].enabled) continue;
        if (i != 0) {
            result += ",";
        }
        if (this->value[i].key.empty()) {
            result += format("%d", this->value[i].number);
        } else {
            result += this->value[i].key.c_str();
        }
    }
    return result;
}

bool SettingDataList::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    const SettingDataList& data = static_cast<const SettingDataList&>(other);

    if (this->value.size() != data.value.size()) {
        return false;
    }

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        const EnabledNumberOrString& value = this->value[i];

        auto it = std::find(data.value.begin(), data.value.end(), value);
        if (it == data.value.end()) {
            return false;
        }
    }

    return true;
}
