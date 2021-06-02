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

struct SettingMetaFrames : public SettingMetaString {
    static const SettingType TYPE;

   private:
    SettingMetaFrames(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataFrames : public SettingDataString {
    SettingDataFrames(const SettingMetaFrames* meta);

    bool Load(const QJsonObject& json_setting) override;

    bool IsValid() const override;

    SettingInputError ProcessInput(const std::string& value);

    const SettingMetaFrames* meta;
};
