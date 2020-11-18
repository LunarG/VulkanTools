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

#include "parameter.h"
#include "platform.h"
#include "version.h"
#include "util.h"
#include "path.h"

#include <QJsonArray>

#include <cassert>
#include <algorithm>

bool ApplySettings(Parameter& parameter, const LayerPreset& preset) {
    for (std::size_t preset_index = 0, preset_count = preset.settings.size(); preset_index < preset_count; ++preset_index) {
        const LayerSettingData& preset_setting = preset.settings[preset_index];

        LayerSettingData* layer_setting = FindByKey(parameter.settings, preset_setting.key.c_str());

        if (layer_setting)
            layer_setting->value = preset_setting.value;
        else
            parameter.settings.push_back(preset_setting);
    }

    return true;
}

ParameterRank GetParameterOrdering(const std::vector<Layer>& available_layers, const Parameter& parameter) {
    assert(!parameter.key.empty());

    const std::vector<Layer>::const_iterator layer = FindItByKey(available_layers, parameter.key.c_str());
    if (layer == available_layers.end()) {
        return PARAMETER_RANK_MISSING;
    } else if (parameter.state == LAYER_STATE_EXCLUDED) {
        return PARAMETER_RANK_EXCLUDED;
    } else if (parameter.state == LAYER_STATE_APPLICATION_CONTROLLED && layer->_layer_type == LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_IMPLICIT_AVAILABLE;
    } else if (parameter.state == LAYER_STATE_OVERRIDDEN && layer->_layer_type == LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_IMPLICIT_OVERRIDDEN;
    } else if (parameter.state == LAYER_STATE_OVERRIDDEN && layer->_layer_type != LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_EXPLICIT_OVERRIDDEN;
    } else if (parameter.state == LAYER_STATE_APPLICATION_CONTROLLED && layer->_layer_type != LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_EXPLICIT_AVAILABLE;
    } else {
        assert(0);  // Unknown ordering
        return PARAMETER_RANK_MISSING;
    }
}

void OrderParameter(std::vector<Parameter>& parameters, const std::vector<Layer>& layers) {
    struct ParameterCompare {
        ParameterCompare(const std::vector<Layer>& layers) : layers(layers) {}

        bool operator()(const Parameter& a, const Parameter& b) const {
            const ParameterRank rankA = GetParameterOrdering(layers, a);
            const ParameterRank rankB = GetParameterOrdering(layers, b);
            if (rankA == rankB && a.state == LAYER_STATE_OVERRIDDEN) {
                if (a.overridden_rank != Parameter::NO_RANK && b.overridden_rank != Parameter::NO_RANK)
                    return a.overridden_rank < b.overridden_rank;
                else if (a.key == "VK_LAYER_LUNARG_device_simulation")
                    return false;
                else if (b.key == "VK_LAYER_LUNARG_device_simulation")
                    return true;
                else if (a.key == "VK_LAYER_KHRONOS_validation" && b.key == "VK_LAYER_LUNARG_device_simulation")
                    return true;
                else if (a.key == "VK_LAYER_KHRONOS_validation")
                    return false;
                else
                    return a.key < b.key;
            } else if (rankA == rankB && a.state != LAYER_STATE_OVERRIDDEN)
                return a.key < b.key;
            else
                return rankA < rankB;
        }

        const std::vector<Layer>& layers;
    };

    std::sort(parameters.begin(), parameters.end(), ParameterCompare(layers));

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].state == LAYER_STATE_OVERRIDDEN)
            parameters[i].overridden_rank = static_cast<int>(i);
        else
            parameters[i].overridden_rank = Parameter::NO_RANK;
    }
}

void FilterParameters(std::vector<Parameter>& parameters, const LayerState state) {
    std::vector<Parameter> filtered_parameters;

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].state == state) continue;

        filtered_parameters.push_back(parameters[i]);
    }

    parameters = filtered_parameters;
}

bool HasMissingParameter(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers) {
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        if (!IsFound(layers, it->key.c_str())) return true;
    }
    return false;
}

std::vector<LayerSettingData> BuildSettings(const std::vector<LayerSettingMeta>& layer_settings) {
    std::vector<LayerSettingData> result;

    for (std::size_t i = 0, n = layer_settings.size(); i < n; ++i) {
        LayerSettingData setting_storage;
        setting_storage.key = layer_settings[i].key;
        setting_storage.value = layer_settings[i].default_value.toStdString();
        result.push_back(setting_storage);
    }

    return result;
}
