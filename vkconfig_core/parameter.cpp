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

#include "setting_filesystem.h"
#include "parameter.h"
#include "platform.h"
#include "version.h"
#include "util.h"
#include "path.h"

#include <QJsonArray>

#include <cassert>
#include <algorithm>

static const char* VK_LAYER_KHRONOS_PROFILES_NAME = "VK_LAYER_KHRONOS_profiles";
static const char* VK_LAYER_KHRONOS_VALIDATION_NAME = "VK_LAYER_KHRONOS_validation";

bool Parameter::ApplyPresetSettings(const LayerPreset& preset) {
    for (std::size_t preset_index = 0, preset_count = preset.settings.size(); preset_index < preset_count; ++preset_index) {
        const SettingData* preset_setting = preset.settings[preset_index];

        for (std::size_t i = 0, n = this->settings.size(); i < n; ++i) {
            SettingData* current_setting = this->settings[i];

            if (current_setting->key != preset_setting->key) continue;
            if (current_setting->type != preset_setting->type) continue;

            this->settings[i]->Copy(preset_setting);
        }
    }

    return true;
}

ParameterRank GetParameterOrdering(const std::vector<Layer>& available_layers, const Parameter& parameter) {
    assert(!parameter.key.empty());

    const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
    if (layer == nullptr) {
        return PARAMETER_RANK_MISSING;
    } else if (parameter.state == LAYER_STATE_EXCLUDED) {
        return PARAMETER_RANK_EXCLUDED;
    } else if (parameter.state == LAYER_STATE_APPLICATION_CONTROLLED && layer->type == LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_IMPLICIT_AVAILABLE;
    } else if (parameter.state == LAYER_STATE_OVERRIDDEN && layer->type == LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_IMPLICIT_OVERRIDDEN;
    } else if (parameter.state == LAYER_STATE_OVERRIDDEN && layer->type != LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_EXPLICIT_OVERRIDDEN;
    } else if (parameter.state == LAYER_STATE_APPLICATION_CONTROLLED && layer->type != LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_EXPLICIT_AVAILABLE;
    } else {
        assert(0);  // Unknown ordering
        return PARAMETER_RANK_MISSING;
    }
}

Version ComputeMinApiVersion(const Version api_version, const std::vector<Parameter>& parameters,
                             const std::vector<Layer>& layers) {
    if (parameters.empty()) return Version::VERSION_NULL;

    Version min_version = api_version;

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Layer* layer = FindByKey(layers, parameters[i].key.c_str());
        if (layer == nullptr) continue;

        const ParameterRank state = GetParameterOrdering(layers, parameters[i]);

        if (state == PARAMETER_RANK_EXCLUDED) continue;
        if (state == PARAMETER_RANK_MISSING) continue;

        min_version = min_version < layer->api_version ? min_version : layer->api_version;
    }

    return min_version;
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
                else if (a.key == VK_LAYER_KHRONOS_PROFILES_NAME)
                    return false;
                else if (b.key == VK_LAYER_KHRONOS_PROFILES_NAME)
                    return true;
                else if (a.key == VK_LAYER_KHRONOS_VALIDATION_NAME && b.key == VK_LAYER_KHRONOS_PROFILES_NAME)
                    return true;
                else if (a.key == VK_LAYER_KHRONOS_VALIDATION_NAME)
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

bool HasMissingLayer(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers, std::string& missing_layer) {
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        if (it->state == LAYER_STATE_EXCLUDED) {
            continue;  // If excluded are missing, it doesn't matter
        }

        if (!(it->platform_flags & (1 << VKC_PLATFORM))) {
            continue;  // If unsupported are missing, it doesn't matter
        }

        if (!IsFound(layers, it->key.c_str())) {
            missing_layer = it->key;
            return true;
        }
    }
    return false;
}

std::size_t CountOverriddenLayers(const std::vector<Parameter>& parameters) {
    std::size_t count = 0;

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        if (!IsPlatformSupported(parameter.platform_flags)) continue;

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        ++count;
    }

    return count;
}

std::size_t CountExcludedLayers(const std::vector<Parameter>& parameters, const std::vector<Layer>& layers) {
    std::size_t count = 0;

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        if (!IsPlatformSupported(parameter.platform_flags)) continue;

        if (parameter.state != LAYER_STATE_EXCLUDED) continue;

        const Layer* layer = FindByKey(layers, parameter.key.c_str());
        if (layer == nullptr) continue;  // Do not display missing excluded layers

        ++count;
    }

    return count;
}

std::vector<Parameter> GatherParameters(const std::vector<Parameter>& parameters, const std::vector<Layer>& available_layers) {
    std::vector<Parameter> gathered_parameters;

    // Loop through the layers. They are expected to be in order
    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        assert(!parameter.key.empty());

        gathered_parameters.push_back(parameter);
    }

    for (std::size_t i = 0, n = available_layers.size(); i < n; ++i) {
        const Layer& layer = available_layers[i];

        // The layer is already in the layer tree
        if (IsFound(parameters, layer.key.c_str())) continue;

        Parameter parameter;
        parameter.key = layer.key;
        parameter.state = LAYER_STATE_APPLICATION_CONTROLLED;
        CollectDefaultSettingData(layer.settings, parameter.settings);

        gathered_parameters.push_back(parameter);
    }

    OrderParameter(gathered_parameters, available_layers);

    return gathered_parameters;
}
