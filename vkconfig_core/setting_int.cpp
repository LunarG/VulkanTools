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

#include "setting_int.h"
#include "json.h"

// SettingMetaInt

const SettingType SettingMetaInt::TYPE(SETTING_INT);

SettingMetaInt::SettingMetaInt(Layer& layer, const std::string& key)
    : SettingMeta(layer, key, TYPE),
      default_value(0),
      min_value(std::numeric_limits<int>::min()),
      max_value(std::numeric_limits<int>::max()) {}

SettingData* SettingMetaInt::Instantiate() {
    SettingData* setting_data = new SettingDataInt(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaInt::Load(const QJsonObject& json_setting) {
    this->default_value = ReadIntValue(json_setting, "default");
    if (json_setting.value("range") != QJsonValue::Undefined) {
        const QJsonObject& json_setting_range = ReadObject(json_setting, "range");
        if (json_setting_range.value("min") != QJsonValue::Undefined) {
            this->min_value = ReadIntValue(json_setting_range, "min");
        }
        if (json_setting_range.value("max") != QJsonValue::Undefined) {
            this->max_value = ReadIntValue(json_setting_range, "max");
        }
    }
    if (json_setting.value("unit") != QJsonValue::Undefined) {
        this->unit = ReadStringValue(json_setting, "unit");
    }

    return true;
}

std::string SettingMetaInt::Export(ExportMode export_mode) const { return format("%d", this->default_value); }

bool SettingMetaInt::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaInt& meta = static_cast<const SettingMetaInt&>(other);

    if (this->min_value != meta.min_value) return false;
    if (this->max_value != meta.max_value) return false;
    if (this->unit != meta.unit) return false;

    return this->default_value == meta.default_value;
}

// SettingDataInt

SettingDataInt::SettingDataInt(const SettingMetaInt* meta)
    : SettingData(meta->key, meta->type), value(meta->default_value), meta(meta) {}

void SettingDataInt::Reset() { this->value = this->meta->default_value; }

void SettingDataInt::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataInt* setting_data = static_cast<const SettingDataInt*>(data);
    this->value = setting_data->value;
}

bool SettingDataInt::Load(const QJsonObject& json_setting) {
    this->value = ReadIntValue(json_setting, "value");
    return true;
}

bool SettingDataInt::Save(QJsonObject& json_setting) const {
    json_setting.insert("value", this->value);
    return true;
}

std::string SettingDataInt::Export(ExportMode export_mode) const {
    (void)export_mode;

    if (this->IsValid()) {
        return format("%d", this->value);
    } else {
        return format("%d", this->meta->default_value);
    }
}

bool SettingDataInt::IsValid() const { return this->value >= this->meta->min_value && this->value <= this->meta->max_value; }

SettingInputError SettingDataInt::ProcessInput(const std::string& value) {
    if (value.empty()) return SETTING_INPUT_ERROR_EMPTY;

    if (!IsNumber(value)) return SETTING_INPUT_ERROR_SYNTAX;

    int saved_data = this->value;
    this->value = std::atoi(value.c_str());

    if (!IsValid()) {
        this->value = saved_data;
        return SETTING_INPUT_ERROR_SEMENTICS;
    }

    return SETTING_INPUT_NO_ERROR;
}

bool SettingDataInt::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return this->value == static_cast<const SettingDataInt&>(other).value;
}
