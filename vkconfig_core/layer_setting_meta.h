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

#include "layer_setting_type.h"

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QJsonObject>

#include <string>

struct LayerSettingMeta {
    std::string key;      // Name of the setting the layer looks for (programatic variable name)
    QString label;        // Short name to prompt end user
    QString description;  // Human version, describes the setting
    SettingType type;     // The data type
    QVariant max_value;   // For range based
    QVariant min_value;   // For range based
    QStringList enum_values;
    QStringList enum_labels;
    QString default_value;  // Default value as a string
};

bool HasValue(const QStringList& values, const char* searched_value);
