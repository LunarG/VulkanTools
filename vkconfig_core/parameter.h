/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "layer_manager.h"
#include "setting.h"
#include "type_layer_control.h"
#include "type_layer_builtin.h"

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

    Parameter() : control(LAYER_CONTROL_AUTO) {}

    Parameter(const std::string& key, const LayerControl control) : key(key), control(control) {}

    bool ApplyPresetSettings(const LayerPreset& preset);

    std::string key;
    LayerControl control = LAYER_CONTROL_AUTO;
    LayerBuiltin builtin = LAYER_BUILTIN_NONE;
    int platform_flags = PLATFORM_DESKTOP_BIT;
    SettingDataSet settings;
    int overridden_rank = NO_RANK;
    Version api_version = Version::LATEST;
    Path manifest;
    QByteArray setting_tree_state;  // Recall editor tree state
    bool override_settings = true;
};

ParameterRank GetParameterOrdering(const LayerManager& layers, const Parameter& parameter);
void OrderParameter(std::vector<Parameter>& parameters, const LayerManager& layers);

bool HasMissingLayer(const std::vector<Parameter>& parameters, const LayerManager& layers, std::string& missing_layer);
