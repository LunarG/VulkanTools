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

struct SettingMetaInt : public SettingMeta {
    static const SettingType TYPE;

    SettingData* Instantiate() override;
    bool Load(const QJsonObject& json_setting) override;
    std::string Export(ExportMode export_mode) const override;

    int default_value;
    int min_value;
    int max_value;
    std::string unit;

   protected:
    bool Equal(const SettingMeta& other) const override;

   private:
    SettingMetaInt(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataInt : public SettingData {
    SettingDataInt(const SettingMetaInt* meta);

    void Reset() override;
    void Copy(const SettingData* data) override;
    bool Load(const QJsonObject& json_setting) override;
    bool Save(QJsonObject& json_setting) const override;
    std::string Export(ExportMode export_mode) const override;
    bool IsValid() const override;

    SettingInputError ProcessInput(const std::string& value);

    int value;

   protected:
    bool Equal(const SettingData& other) const override;

    const SettingMetaInt* meta;
};
