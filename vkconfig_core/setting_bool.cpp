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

SettingMetaBool::SettingMetaBool(Layer& layer, const std::string& key) : SettingMetaBool(layer, key, TYPE) {}

SettingMetaBool::SettingMetaBool(Layer& layer, const std::string& key, const SettingType type)
    : SettingMeta(layer, key, type), default_value(false) {}

SettingData* SettingMetaBool::Instantiate() {
    SettingData* setting_data = new SettingDataBool(this);
    setting_data->Reset();
    return setting_data;
}

bool SettingMetaBool::Load(const QJsonObject& json_setting) {
    this->default_value = ReadBoolValue(json_setting, "default");
    return true;
}

std::string SettingMetaBool::Export(ExportMode export_mode) const { return this->default_value ? "TRUE" : "FALSE"; }

bool SettingMetaBool::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    return this->default_value == static_cast<const SettingMetaBool&>(other).default_value;
}

// SettingDataBool

SettingDataBool::SettingDataBool(const SettingMetaBool* meta) : SettingData(meta->key, meta->type), value(false), meta(meta) {}

SettingDataBool::SettingDataBool(const std::string& key, const SettingType& type)
    : SettingData(key, type), value(false), meta(nullptr) {}

void SettingDataBool::Reset() { this->value = this->meta->default_value; }

bool SettingDataBool::Load(const QJsonObject& json_setting) {
    this->value = ReadBoolValue(json_setting, "value");
    return true;
}

bool SettingDataBool::Save(QJsonObject& json_setting) const {
    json_setting.insert("value", this->value);
    return true;
}

std::string SettingDataBool::Export(ExportMode export_mode) const { return this->value ? "TRUE" : "FALSE"; }

bool SettingDataBool::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataBool&>(other).value;
}

SettingData& SettingDataBool::Assign(const SettingData& other) {
    assert(this->type == other.type);

    this->value = static_cast<const SettingDataBool&>(other).value;
    return *this;
}

// SettingMetaBoolNumeric

SettingMetaBoolNumeric::SettingMetaBoolNumeric(Layer& layer, const std::string& key) : SettingMetaBool(layer, key, TYPE) {}

SettingData* SettingMetaBoolNumeric::Instantiate() { return new SettingDataBoolNumeric(this); }

std::string SettingMetaBoolNumeric::Export(ExportMode export_mode) const { return this->default_value ? "1" : "0"; }

// SettingDataBoolNumeric

SettingDataBoolNumeric::SettingDataBoolNumeric(const SettingMetaBoolNumeric* meta)
    : SettingDataBool(meta->key, meta->type), meta(meta) {}

std::string SettingDataBoolNumeric::Export(ExportMode export_mode) const { return this->value ? "1" : "0"; }
