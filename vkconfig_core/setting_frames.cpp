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

#include "setting_frames.h"
#include "json.h"

// SettingMetaFrames

const SettingType SettingMetaFrames::TYPE(SETTING_FRAMES);

SettingMetaFrames::SettingMetaFrames(Layer& layer, const std::string& key) : SettingMetaString(layer, key, TYPE) {}

// SettingDataFrames

SettingDataFrames::SettingDataFrames(const SettingMetaFrames* meta) : SettingDataString(meta), meta(meta) {}

bool SettingDataFrames::Load(const QJsonObject& json_setting) {
    this->value = ReadStringValue(json_setting, "value");
    return true;
}

bool SettingDataFrames::IsValid() const { return IsFrames(this->value) || this->value.empty(); }

SettingInputError SettingDataFrames::ProcessInput(const std::string& input_value) {
    if (input_value.empty()) return SETTING_INPUT_ERROR_EMPTY;

    if (!IsFrames(input_value)) return SETTING_INPUT_ERROR_SYNTAX;

    std::string saved_data = this->value;
    this->value = input_value;

    if (!IsValid()) {
        this->value = saved_data;
        return SETTING_INPUT_ERROR_SEMENTICS;
    }

    return SETTING_INPUT_NO_ERROR;
}
