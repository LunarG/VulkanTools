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

#include <QString>
#include <QVariant>
#include <QJsonObject>

#include <vector>

enum SettingType {  // Enum value can't be changed
    SETTING_STRING = 0,
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

// The value of this enum can't be changed
enum LayerState {
    LAYER_STATE_APPLICATION_CONTROLLED = 0,  // The Vulkan application configured this layer at will
    LAYER_STATE_OVERRIDDEN = 1,              // Force on/override this layer and configure it regarless of the Vulkan application
    LAYER_STATE_EXCLUDED = 2,                // Force off/exclude this layer regarless of the Vulkan application

    LAYER_STATE_FIRST = LAYER_STATE_APPLICATION_CONTROLLED,
    LAYER_STATE_LAST = LAYER_STATE_EXCLUDED
};

enum { LAYER_STATE_COUNT = LAYER_STATE_LAST - LAYER_STATE_FIRST + 1 };

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

bool operator==(const LayerSetting& a, const LayerSetting& b);
bool operator!=(const LayerSetting& a, const LayerSetting& b);

LayerSetting* FindSetting(std::vector<LayerSetting>& settings, const char* key);

SettingType GetSettingType(const char* token);
const char* GetSettingTypeToken(SettingType type);

struct Parameter {
    static const int UNRANKED = -1;

    Parameter() : state(LAYER_STATE_APPLICATION_CONTROLLED), overridden_rank(UNRANKED) {}

    QString name;
    LayerState state;
    std::vector<LayerSetting> settings;
    int overridden_rank;
};

bool LoadSettings(const QJsonObject& layer_settings_descriptors, Parameter& parameter);
bool SaveSettings(const Parameter& parameter, QJsonObject& layer_settings_descriptors);

// Order of layers matters and we handle this internally
void SortLayers(std::vector<Parameter>& parameters);

bool operator==(const std::vector<Parameter>& a, const std::vector<Parameter>& b);
bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b);
