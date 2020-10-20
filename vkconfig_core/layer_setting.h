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
#include "platform.h"

#include <QString>
#include <QVariant>
#include <QJsonObject>

enum SettingType {  // Enum value can't be changed
    SETTING_STRING = 0,
    SETTING_INT,
    SETTING_SAVE_FILE,
    SETTING_LOAD_FILE,
    SETTING_SAVE_FOLDER,
    SETTING_BOOL,
    SETTING_BOOL_NUMERIC,
    SETTING_EXCLUSIVE_LIST,
    SETTING_INCLUSIVE_LIST,
    SETTING_RANGE_INT,
    SETTING_VUID_FILTER,

    SETTING_FIRST = SETTING_STRING,
    SETTING_LAST = SETTING_VUID_FILTER
};

enum { SETTING_COUNT = SETTING_LAST - SETTING_FIRST + 1 };

struct LayerSetting {
    QString key;                    // Name of the setting the layer looks for (programatic variable name)
    QString label;                  // Short name to prompt end user
    StatusType status;              // Is the setting qualified as "stable", "beta" or "alpha"
    int platform_flags;             // Platforms on which the setting is supported
    QString description;            // Human version, describes the setting
    SettingType type;               // The data type
    QVariant max_value;             // For range based
    QVariant min_value;             // For range based
    std::vector<QString> labels;    // List of labels
    std::vector<QString> values;    // List of values
    std::vector<QString> defaults;  // List of default values
};

bool LoadLayerSettings(const QJsonValue& json_layer_settings, std::vector<LayerSetting>& settings);
bool SaveLayerSettings(const std::vector<LayerSetting>& settings, QJsonArray& json_settings);

LayerSetting* FindSetting(std::vector<LayerSetting>& settings, const char* key);

SettingType GetSettingType(const char* token);
const char* GetSettingTypeToken(SettingType type);

bool IsStringFound(const std::vector<QString>& data, const QString& token);
