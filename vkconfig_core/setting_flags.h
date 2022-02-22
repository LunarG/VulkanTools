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

#include "setting_string.h"

struct SettingEnumValue : public Header {
    std::string key;
    SettingMetaSet settings;
};

bool IsSupported(const SettingEnumValue* value);

bool operator==(const SettingEnumValue& a, const SettingEnumValue& b);
inline bool operator!=(const SettingEnumValue& a, const SettingEnumValue& b) { return !(a == b); }

struct SettingMetaEnumeration : public SettingMeta {
    bool Load(const QJsonObject& json_setting) override;

    std::vector<SettingEnumValue> enum_values;

   protected:
    SettingMetaEnumeration(Layer& layer, const std::string& key, const SettingType& setting_type);

    bool Equal(const SettingMeta& other) const override;
};

struct SettingMetaEnum : public SettingMetaEnumeration {
    static const SettingType TYPE;

    SettingData* Instantiate() override;
    bool Load(const QJsonObject& json_setting) override;
    std::string Export(ExportMode export_mode) const override;

    std::string default_value;

   protected:
    bool Equal(const SettingMeta& other) const override;

   private:
    SettingMetaEnum(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataEnum : public SettingDataString {
    SettingDataEnum(const SettingMetaEnum* meta);

    void Copy(const SettingData* data) override;
    void Reset() override;

    const SettingMetaEnum* meta;
};

struct SettingMetaFlags : public SettingMetaEnumeration {
    static const SettingType TYPE;

    SettingData* Instantiate() override;
    bool Load(const QJsonObject& json_setting) override;
    std::string Export(ExportMode export_mode) const override;

    std::vector<std::string> default_value;

   protected:
    bool Equal(const SettingMeta& other) const override;

   private:
    SettingMetaFlags(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataFlags : public SettingData {
    SettingDataFlags(const SettingMetaFlags* meta);

    void Copy(const SettingData* data) override;
    bool Load(const QJsonObject& json_setting) override;
    bool Save(QJsonObject& json_setting) const override;
    std::string Export(ExportMode export_mode) const override;
    void Reset() override;

    std::vector<std::string> value;

   protected:
    bool Equal(const SettingData& other) const override;

    const SettingMetaFlags* meta;
};
