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

#include "setting_float.h"
#include "util.h"
#include "json.h"

// SettingMetaFloat

const SettingType SettingMetaFloat::TYPE(SETTING_FLOAT);

SettingMetaFloat::SettingMetaFloat(Layer& layer, const std::string& key)
    : SettingMeta(layer, key, TYPE), default_value(0.0f), min_value(0.0f), max_value(0.0f), precision(0), width(0) {}

SettingData* SettingMetaFloat::Instantiate() {
    SettingData* setting_data = new SettingDataFloat(this);
    setting_data->Reset();
    this->instances.push_back(setting_data);
    return setting_data;
}

bool SettingMetaFloat::Load(const QJsonObject& json_setting) {
    this->default_value = ReadFloatValue(json_setting, "default");
    if (json_setting.value("range") != QJsonValue::Undefined) {
        const QJsonObject& json_setting_range = ReadObject(json_setting, "range");
        if (json_setting_range.value("min") != QJsonValue::Undefined) {
            this->min_value = ReadFloatValue(json_setting_range, "min");
        }
        if (json_setting_range.value("max") != QJsonValue::Undefined) {
            this->max_value = ReadFloatValue(json_setting_range, "max");
        }
        if (json_setting_range.value("precision") != QJsonValue::Undefined) {
            this->precision = ReadIntValue(json_setting_range, "precision");
        }
        if (json_setting_range.value("width") != QJsonValue::Undefined) {
            this->width = ReadIntValue(json_setting_range, "width");
        }
    }
    if (json_setting.value("unit") != QJsonValue::Undefined) {
        this->unit = ReadStringValue(json_setting, "unit");
    }

    return true;
}

std::string SettingMetaFloat::Export(ExportMode export_mode) const {
    (void)export_mode;

    const std::string float_format = this->GetFloatFormat();

    return format(float_format.c_str(), this->default_value);
}

bool SettingMetaFloat::Equal(const SettingMeta& other) const {
    if (!SettingMeta::Equal(other)) return false;

    const SettingMetaFloat& meta = static_cast<const SettingMetaFloat&>(other);

    if (std::abs(this->min_value - meta.min_value) > std::numeric_limits<float>::epsilon()) return false;
    if (std::abs(this->max_value - meta.max_value) > std::numeric_limits<float>::epsilon()) return false;
    if (this->precision != meta.precision) return false;
    if (this->width != meta.width) return false;
    if (this->unit != meta.unit) return false;

    return std::abs(this->default_value - meta.default_value) <= std::numeric_limits<float>::epsilon();
}

// SettingDataFloat

SettingDataFloat::SettingDataFloat(const SettingMetaFloat* meta) : SettingData(meta->key, meta->type), value(0.0f), meta(meta) {}

void SettingDataFloat::Reset() {
    assert(this->meta != nullptr);
    this->value = this->meta->default_value;
}

void SettingDataFloat::Copy(const SettingData* data) {
    if (data->type != this->type) return;

    const SettingDataFloat* setting_data = static_cast<const SettingDataFloat*>(data);
    this->value = setting_data->value;
}

bool SettingDataFloat::Load(const QJsonObject& json_setting) {
    this->value = ReadFloatValue(json_setting, "value");
    return true;
}

bool SettingDataFloat::Save(QJsonObject& json_setting) const {
    json_setting.insert("value", this->value);
    return true;
}

std::string SettingDataFloat::Export(ExportMode export_mode) const {
    (void)export_mode;

    const std::string float_format = this->meta->GetFloatFormat();

    if (this->IsValid()) {
        return format(float_format.c_str(), this->value);
    } else {
        return format(float_format.c_str(), this->meta->default_value);
    }
}

bool SettingDataFloat::IsValid() const {
    if (meta->HasRange()) {
        return this->value >= this->meta->min_value && this->value <= this->meta->max_value;
    } else {
        return true;
    }
}

SettingInputError SettingDataFloat::ProcessInput(const std::string& input_value) {
    if (input_value.empty()) return SETTING_INPUT_ERROR_EMPTY;

    if (!IsFloat(input_value)) return SETTING_INPUT_ERROR_SYNTAX;

    int saved_data = this->value;
    this->value = std::atof(input_value.c_str());

    if (!IsValid()) {
        this->value = saved_data;
        return SETTING_INPUT_ERROR_SEMENTICS;
    }

    return SETTING_INPUT_NO_ERROR;
}

bool SettingDataFloat::Equal(const SettingData& other) const {
    if (!SettingData::Equal(other)) return false;

    return std::abs(this->value - static_cast<const SettingDataFloat&>(other).value) <= std::numeric_limits<float>::epsilon();
}
