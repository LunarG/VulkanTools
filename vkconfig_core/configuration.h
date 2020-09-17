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

#include "../vkconfig_core/layer.h"

#include <QString>
#include <QStringList>

#include <vector>

// json file preset_index must match the preset enum values
enum ValidationPreset {
    ValidationPresetNone = 0,
    ValidationPresetUserDefined = ValidationPresetNone,
    ValidationPresetStandard = 1,
    ValidationPresetGPUAssisted = 2,
    ValidationPresetDebugPrintf = 3,
    ValidationPresetReducedOverhead = 4,
    ValidationPresetBestPractices = 5,
    ValidationPresetSynchronization = 6,

    ValidationPresetFirst = ValidationPresetUserDefined,
    ValidationPresetLast = ValidationPresetSynchronization
};

enum { ValidationPresetCount = ValidationPresetLast - ValidationPresetFirst + 1 };

class Configuration {
   public:
    Configuration();

    bool Load(const QString& full_path);
    bool Save(const QString& full_path) const;

    QString _name;                   // User readable display of the profile name (may contain spaces)
                                     // This is the same as the filename, but with the .json stripped off.
    QString _description;            // A friendly description of what this profile does
    QByteArray _setting_tree_state;  // Recall editor tree state
    ValidationPreset _preset;        // Khronos layer presets. 0 = none or user defined

    std::vector<Parameter> parameters;

    Parameter* FindParameter(const QString& layer_name);  // Find the layer if it exists

    bool IsEmpty() const;
};
