/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "layer_type.h"
#include "setting_type.h"
#include "setting_value.h"
#include "platform.h"

#include <QString>
#include <QJsonObject>

struct EnumValue {
    QString key;
    QString label;
};

struct LayerSetting {
    LayerSetting() : status(STATUS_STABLE), platform_flags(0), type(SETTING_STRING) {}

    QString key;                   // Name of the setting the layer looks for (programatic variable name)
    QString label;                 // Short name to prompt end user
    StatusType status;             // Is the setting qualified as "stable", "beta" or "alpha"
    int platform_flags;            // Platforms on which the setting is supported
    QString description;           // Human version, describes the setting
    SettingType type;              // The data type
    std::vector<EnumValue> enums;  // List of enum values
    SettingValue default_value;    // List of default values

    bool Load(const QJsonObject& json_setting);
    bool Save(QJsonObject& json_setting) const;
};

bool operator==(const LayerSetting& l, const LayerSetting& r);
bool operator!=(const LayerSetting& l, const LayerSetting& r);

void Sort(std::vector<LayerSetting>& settings);
LayerSetting* FindSetting(std::vector<LayerSetting>& settings, const char* key);
