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

#pragma once

#include "setting.h"

struct SettingMetaFloat : public SettingMeta {
    static const SettingType TYPE;

    SettingData* Instantiate() override;
    bool Load(const QJsonObject& json_setting) override;
    std::string Export(ExportMode export_mode) const override;

    float default_value;
    float min_value;
    float max_value;
    int precision;
    int width;
    std::string unit;

    bool HasRange() const { return std::abs(this->max_value - this->min_value) > std::numeric_limits<float>::epsilon(); }

    bool HasPrecision() const { return this->precision != 0 || this->width != 0; }

    std::string GetFloatFormat() const {
        if (this->HasPrecision()) {
            return "%" + format("%d.%df", this->width, this->precision);
        } else {
            return "%f";
        }
    }

   protected:
    bool Equal(const SettingMeta& other) const override;

   private:
    SettingMetaFloat(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataFloat : public SettingData {
    SettingDataFloat(const SettingMetaFloat* meta);

    void Reset() override;
    void Copy(const SettingData* data) override;
    bool Load(const QJsonObject& json_setting) override;
    bool Save(QJsonObject& json_setting) const override;
    std::string Export(ExportMode export_mode) const override;
    bool IsValid() const override;

    SettingInputError ProcessInput(const std::string& value);

    float value;

   protected:
    bool Equal(const SettingData& other) const override;

    const SettingMetaFloat* meta;
};
