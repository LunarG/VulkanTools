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

#include "setting_string.h"
#include "json.h"

// SettingMetaString

const SettingType SettingMetaString::TYPE = SETTING_STRING;

SettingMetaString::SettingMetaString(Layer& layer, const std::string& key) : SettingMetaString(layer, key, TYPE) {}

SettingMetaString::SettingMetaString(Layer& layer, const std::string& key, const SettingType& setting_type)
    : SettingMeta(layer, key, setting_type) {}

SettingData* SettingMetaString::Instantiate() {
    SettingData* setting_data = new SettingDataString(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaString::Load(const QJsonObject& json_setting) {
    this->default_value = ReadStringValue(json_setting, "default");

    return true;
}

std::string SettingMetaString::Export(ExportMode export_mode) const {
    (void)export_mode;
    return this->default_value;
}

bool SettingMetaString::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaString&>(other).default_value;
}

// SettingDataString

SettingDataString::SettingDataString(const SettingMetaString* meta) : SettingData(meta->key, meta->type), meta(meta) {
    assert(meta != nullptr);
    this->Reset();
}

SettingDataString::SettingDataString(const std::string& key, const SettingType& type) : SettingData(key, type), meta(nullptr) {}

void SettingDataString::Reset() {
    assert(meta != nullptr);
    this->value = this->meta->default_value;
}

void SettingDataString::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataString* setting_data = static_cast<const SettingDataString*>(data);
    this->value = setting_data->value;
}

bool SettingDataString::Load(const QJsonObject& json_setting) {
    this->value = ReadStringValue(json_setting, "value");
    return true;
}

bool SettingDataString::Save(QJsonObject& json_setting) const {
    json_setting.insert("value", this->value.c_str());
    return true;
}

std::string SettingDataString::Export(ExportMode export_mode) const {
    (void)export_mode;

    if (this->IsValid()) {
        return this->value;
    } else {
        return this->meta->default_value;
    }
}

bool SettingDataString::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataString&>(other).value;
}
