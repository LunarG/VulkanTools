/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "setting_flags.h"
#include "json.h"
#include "layer.h"
#include "setting.h"

#include <QJsonArray>

// SettingMetaEnumeration

SettingMetaEnumeration::SettingMetaEnumeration(Layer& layer, const std::string& key, const SettingType& setting_type)
    : SettingMeta(layer, key, setting_type) {}

bool SettingMetaEnumeration::Load(const QJsonObject& json_setting) {
    const QJsonArray& json_array_flags = ReadArray(json_setting, "flags");
    for (int i = 0, n = json_array_flags.size(); i < n; ++i) {
        const QJsonObject& json_object = json_array_flags[i].toObject();

        SettingEnumValue setting_enum_value;
        setting_enum_value.key = ReadStringValue(json_object, "key");
        setting_enum_value.platform_flags = this->platform_flags;
        setting_enum_value.status = this->status;
        setting_enum_value.view = this->view;
        LoadMetaHeader(setting_enum_value, json_object);

        this->enum_values.push_back(setting_enum_value);

        for (std::size_t i = 0, n = this->enum_values.size(); i < n; ++i) {
            if (this->enum_values[i].key != setting_enum_value.key) {
                continue;
            }

            if (json_object.value("settings") != QJsonValue::Undefined) {
                this->layer.AddSettingsSet(this->enum_values[i].settings, this, json_object.value("settings"));
            }
        }
    }
    return true;
}

bool SettingMetaEnumeration::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->enum_values == static_cast<const SettingMetaEnumeration&>(other).enum_values;
}

// SettingMetaEnum

const SettingType SettingMetaEnum::TYPE = SETTING_ENUM;

SettingMetaEnum::SettingMetaEnum(Layer& layer, const std::string& key) : SettingMetaEnumeration(layer, key, TYPE) {}

SettingData* SettingMetaEnum::Instantiate() {
    SettingData* setting_data = new SettingDataEnum(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaEnum::Load(const QJsonObject& json_setting) {
    this->SettingMetaEnumeration::Load(json_setting);

    this->default_value = ReadStringValue(json_setting, "default");
    return true;
}

std::string SettingMetaEnum::Export(ExportMode export_mode) const {
    (void)export_mode;
    return this->default_value;
}

bool SettingMetaEnum::Equal(const SettingMeta& other) const {
    if (!SettingMetaEnumeration::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaEnum&>(other).default_value;
}

// SettingDataEnum

SettingDataEnum::SettingDataEnum(const SettingMetaEnum* meta) : SettingDataString(meta->key, meta->type), meta(meta) {
    assert(meta != nullptr);
}

void SettingDataEnum::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataEnum* setting_data = static_cast<const SettingDataEnum*>(data);
    this->value = setting_data->value;
}

void SettingDataEnum::Reset() { this->value = this->meta->default_value; }

// SettingMetaFlags

const SettingType SettingMetaFlags::TYPE(SETTING_FLAGS);

SettingMetaFlags::SettingMetaFlags(Layer& layer, const std::string& key) : SettingMetaEnumeration(layer, key, TYPE) {}

SettingData* SettingMetaFlags::Instantiate() {
    SettingData* setting_data = new SettingDataFlags(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaFlags::Load(const QJsonObject& json_setting) {
    this->SettingMetaEnumeration::Load(json_setting);

    this->default_value = ReadStringArray(json_setting, "default");
    return true;
}

std::string SettingMetaFlags::Export(ExportMode export_mode) const {
    (void)export_mode;

    std::string result;

    for (std::size_t i = 0, n = this->default_value.size(); i < n; ++i) {
        result += this->default_value[i].c_str();
        if (i < n - 1) {
            result += ",";
        }
    }

    return result;
}

bool SettingMetaFlags::Equal(const SettingMeta& other) const {
    if (!SettingMetaEnumeration::Equal(other)) return false;

    const SettingMetaFlags& flags = static_cast<const SettingMetaFlags&>(other);

    if (this->default_value.size() != flags.default_value.size()) return false;

    for (std::size_t i = 0, n = flags.default_value.size(); i < n; ++i) {
        const char* flag = flags.default_value[i].c_str();

        if (!IsStringFound(this->default_value, flag)) {
            return false;
        }
    }

    return true;
}

// SettingDataFlags

SettingDataFlags::SettingDataFlags(const SettingMetaFlags* meta) : SettingData(meta->key, meta->type), meta(meta) {
    assert(meta != nullptr);

    for (std::size_t i = 0, n = meta->enum_values.size(); i < n; ++i) {
        this->expanded_flags.insert(std::make_pair(meta->enum_values[i].key, true));
    }
}

void SettingDataFlags::Reset() {
    assert(this->meta != nullptr);
    this->value = this->meta->default_value;
}

void SettingDataFlags::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFlags* setting_data = static_cast<const SettingDataFlags*>(data);
    this->value = setting_data->value;
}

void SettingDataFlags::Append(const SettingDataFlags* data) {
    for (std::size_t i = 0, n = data->value.size(); i < n; ++i) {
        if (std::find(this->value.begin(), this->value.end(), data->value[i]) == this->value.end()) {
            this->value.push_back(data->value[i]);
        }
    }
}

void SettingDataFlags::Remove(const SettingDataFlags* data) {
    for (std::size_t i = 0, n = data->value.size(); i < n; ++i) {
        auto it = std::find(this->value.begin(), this->value.end(), data->value[i]);
        if (it != this->value.end()) {
            this->value.erase(it);
        }
    }
}

bool SettingDataFlags::Load(const QJsonObject& json_setting) {
    this->value = ReadStringArray(json_setting, "value");
    if (json_setting.value("expanded") != QJsonValue::Undefined) {
        this->expanded = ReadBoolValue(json_setting, "expanded");
    }
    if (json_setting.value("expanded_flags") != QJsonValue::Undefined) {
        const QJsonObject& json_object = json_setting.value("expanded_flags").toObject();
        for (auto it = this->expanded_flags.begin(); it != this->expanded_flags.end(); ++it) {
            it->second = json_object.value(it->first.c_str()).toBool();
        }
    }

    return true;
}

bool SettingDataFlags::Save(QJsonObject& json_setting) const {
    QJsonArray json_array;

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        json_array.append(this->value[i].c_str());
    }

    json_setting.insert("value", json_array);
    json_setting.insert("expanded", this->expanded);

    return true;
}

std::string SettingDataFlags::Export(ExportMode export_mode) const {
    (void)export_mode;

    std::string result;

    for (std::size_t i = 0, n = this->value.size(); i < n; ++i) {
        result += this->value[i].c_str();
        if (i < n - 1) {
            result += ",";
        }
    }

    return result;
}

bool SettingDataFlags::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) {
        return false;
    }

    const SettingDataFlags& flags = static_cast<const SettingDataFlags&>(other);

    if (this->type == SETTING_FLAGS) {
        if (this->value.empty() && flags.value.empty()) {
            return true;
        }

        bool result = this->value.size() == flags.value.size();
        for (std::size_t i = 0, n = flags.value.size(); i < n; ++i) {
            const char* flag = flags.value[i].c_str();

            result = result && IsStringFound(this->value, flag);
            if (!result) {
                break;
            }
        }

        return result;
    } else {
        if (this->value.size() != flags.value.size()) {
            return false;
        }

        for (std::size_t i = 0, n = flags.value.size(); i < n; ++i) {
            const char* flag = flags.value[i].c_str();

            if (!IsStringFound(this->value, flag)) {
                return false;
            }
        }

        return true;
    }
}
