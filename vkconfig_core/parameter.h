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
#include "layer_state.h"
#include "layer_setting_data.h"

#include <QString>

#include <vector>

enum ParameterRank {
    PARAMETER_RANK_MISSING = 0,
    PARAMETER_RANK_EXCLUDED,
    PARAMETER_RANK_IMPLICIT_AVAILABLE,
    PARAMETER_RANK_IMPLICIT_OVERRIDDEN,
    PARAMETER_RANK_EXPLICIT_OVERRIDDEN,
    PARAMETER_RANK_EXPLICIT_AVAILABLE
};

struct Parameter {
    static const int NO_RANK = -1;
    static const int NO_PRESET = 0;

    Parameter() : state(LAYER_STATE_APPLICATION_CONTROLLED), overridden_rank(NO_RANK), preset_index(NO_PRESET) {}
    Parameter(const std::string& key, const LayerState state)
        : key(key), state(state), overridden_rank(NO_RANK), preset_index(NO_PRESET) {}

    std::string key;
    LayerState state;
    std::vector<LayerSettingData> settings;
    int overridden_rank;
    int preset_index;  // Settings preset, 0 = none or user defined
};

ParameterRank GetParameterOrdering(const std::vector<Layer>& available_layers, const Parameter& parameter);
void OrderParameter(std::vector<Parameter>& parameters, const std::vector<Layer>& layers);
void FilterParameters(std::vector<Parameter>& parameters, const LayerState state);

bool HasMissingParameter(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers);

std::vector<LayerSettingData> BuildSettings(const std::vector<LayerSettingMeta>& layer_settings);

bool ApplySettings(Parameter& parameter, const LayerPreset& preset);
