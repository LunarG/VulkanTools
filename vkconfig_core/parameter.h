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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "layer.h"
#include "layer_setting.h"

#include <QString>

#include <vector>

// The value of this enum can't be changed
enum LayerState {
    LAYER_STATE_APPLICATION_CONTROLLED = 0,  // The Vulkan application configured this layer at will
    LAYER_STATE_OVERRIDDEN = 1,              // Force on/override this layer and configure it regarless of the Vulkan application
    LAYER_STATE_EXCLUDED = 2,                // Force off/exclude this layer regarless of the Vulkan application

    LAYER_STATE_FIRST = LAYER_STATE_APPLICATION_CONTROLLED,
    LAYER_STATE_LAST = LAYER_STATE_EXCLUDED
};

enum { LAYER_STATE_COUNT = LAYER_STATE_LAST - LAYER_STATE_FIRST + 1 };

enum ParameterRank {
    PARAMETER_RANK_MISSING = 0,
    PARAMETER_RANK_EXCLUDED,
    PARAMETER_RANK_IMPLICIT_AVAILABLE,
    PARAMETER_RANK_IMPLICIT_OVERRIDDEN,
    PARAMETER_RANK_EXPLICIT_OVERRIDDEN,
    PARAMETER_RANK_EXPLICIT_AVAILABLE
};

struct Parameter {
    static const int UNRANKED = -1;

    Parameter() : state(LAYER_STATE_APPLICATION_CONTROLLED), preset_index(PRESET_INDEX_USER_DEFINED), overridden_rank(UNRANKED) {}
    Parameter(const QString& name, const LayerState state)
        : name(name), state(state), preset_index(PRESET_INDEX_USER_DEFINED), overridden_rank(UNRANKED) {}

    QString name;
    LayerState state;
    std::vector<LayerSetting> settings;
    int preset_index;
    int overridden_rank;
};

ParameterRank GetParameterOrdering(const std::vector<Layer>& available_layers, const Parameter& parameter);
void OrderParameter(std::vector<Parameter>& parameters, const std::vector<Layer>& layers);
void FilterParameters(std::vector<Parameter>& parameters, const LayerState state);
std::vector<Parameter>::iterator FindParameter(std::vector<Parameter>& parameters, const QString& layer_name);

bool HasMissingParameter(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers);

bool LoadConfigurationSettings(const QJsonObject& layer_settings_descriptors, Parameter& parameter);
bool SaveConfigurationSettings(const Parameter& parameter, QJsonObject& layer_settings_descriptors);
