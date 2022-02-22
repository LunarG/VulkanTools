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

#include "setting_bool.h"
#include "json.h"

// SettingMetaBool

const SettingType SettingMetaBool::TYPE(SETTING_BOOL);

SettingMetaBool::SettingMetaBool(Layer& layer, const std::string& key) : SettingMetaBool(layer, key, TYPE) {}

SettingMetaBool::SettingMetaBool(Layer& layer, const std::string& key, const SettingType type)
    : SettingMeta(layer, key, type), default_value(false) {}

SettingData* SettingMetaBool::Instantiate() {
    SettingData* setting_data = new SettingDataBool(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaBool::Load(const QJsonObject& json_setting) {
    this->default_value = ReadBoolValue(json_setting, "default");
    return true;
}

std::string SettingMetaBool::Export(ExportMode export_mode) const {
    (void)export_mode;
    return this->default_value ? "true" : "false";
}

bool SettingMetaBool::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaBool&>(other).default_value;
}

// SettingDataBool

SettingDataBool::SettingDataBool(const SettingMetaBool* meta)
    : SettingData(meta->key, meta->type), value(meta->default_value), meta(meta) {}

void SettingDataBool::Reset() { this->value = this->meta->default_value; }

void SettingDataBool::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataBool* setting_data = static_cast<const SettingDataBool*>(data);
    this->value = setting_data->value;
}

bool SettingDataBool::Load(const QJsonObject& json_setting) {
    this->value = ReadBoolValue(json_setting, "value");
    return true;
}

bool SettingDataBool::Save(QJsonObject& json_setting) const {
    json_setting.insert("value", this->value);
    return true;
}

std::string SettingDataBool::Export(ExportMode export_mode) const {
    (void)export_mode;
    return this->value ? "true" : "false";
}

bool SettingDataBool::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataBool&>(other).value;
}

// SettingMetaBoolNumeric

const SettingType SettingMetaBoolNumeric::TYPE(SETTING_BOOL_NUMERIC_DEPRECATED);

SettingMetaBoolNumeric::SettingMetaBoolNumeric(Layer& layer, const std::string& key) : SettingMetaBool(layer, key, TYPE) {}

SettingData* SettingMetaBoolNumeric::Instantiate() { return new SettingDataBoolNumeric(this); }

std::string SettingMetaBoolNumeric::Export(ExportMode export_mode) const {
    (void)export_mode;
    return this->default_value ? "1" : "0";
}

// SettingDataBoolNumeric

SettingDataBoolNumeric::SettingDataBoolNumeric(const SettingMetaBoolNumeric* meta) : SettingDataBool(meta) {}

std::string SettingDataBoolNumeric::Export(ExportMode export_mode) const {
    (void)export_mode;
    return this->value ? "1" : "0";
}
