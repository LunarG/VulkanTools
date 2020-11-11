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

#include <QString>
#include <QVariant>
#include <QJsonObject>

struct LayerSetting {
    QString key;                   // Name of the setting the layer looks for (programatic variable name)
    QString label;                 // Short name to prompt end user
    QString description;           // Human version, describes the setting
    SettingType type;              // The data type
    QVariant max_value;            // For range based
    QVariant min_value;            // For range based
    QStringList exclusive_values;  // List of exclusive items
    QStringList exclusive_labels;  // List of exclusive item prompts
    QStringList inclusive_values;  // List of non-exclusive items (more than one item can be selected)
    QStringList inclusive_labels;  // List of non-exclusive item prompts (more than one item can be selected)
    QString value;                 // Default value as a string
};

LayerSetting* FindSetting(std::vector<LayerSetting>& settings, const char* key);
