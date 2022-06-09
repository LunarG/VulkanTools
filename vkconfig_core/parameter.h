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

#include "layer.h"
#include "layer_state.h"
#include "setting.h"

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

    Parameter() : state(LAYER_STATE_APPLICATION_CONTROLLED), platform_flags(PLATFORM_DESKTOP_BIT), overridden_rank(NO_RANK) {}
    Parameter(const std::string& key, const LayerState state)
        : key(key), state(state), platform_flags(PLATFORM_DESKTOP_BIT), overridden_rank(NO_RANK) {}

    bool ApplyPresetSettings(const LayerPreset& preset);

    std::string key;
    LayerState state;
    int platform_flags;
    SettingDataSet settings;
    int overridden_rank;
};

ParameterRank GetParameterOrdering(const std::vector<Layer>& available_layers, const Parameter& parameter);
Version ComputeMinApiVersion(const Version api_version, const std::vector<Parameter>& parameters, const std::vector<Layer>& layers);
void OrderParameter(std::vector<Parameter>& parameters, const std::vector<Layer>& layers);
void FilterParameters(std::vector<Parameter>& parameters, const LayerState state);
std::vector<Parameter> GatherParameters(const std::vector<Parameter>& parameters, const std::vector<Layer>& available_layers);

bool HasMissingLayer(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers, std::string& missing_layer);

std::size_t CountOverriddenLayers(const std::vector<Parameter>& parameters);
std::size_t CountExcludedLayers(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers);
