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

#include "setting_filesystem.h"
#include "parameter.h"
#include "type_platform.h"
#include "version.h"
#include "util.h"
#include "path.h"

#include <QJsonArray>

#include <cassert>
#include <algorithm>

static bool IsExtensionLayer(const std::string& key) {
    return key == "VK_LAYER_KHRONOS_timeline_semaphore" || key == "VK_LAYER_KHRONOS_synchronization2" ||
           key == "VK_LAYER_KHRONOS_shader_object" || key == "VK_LAYER_KHRONOS_memory_decompression";
}

static bool IsValidationLayer(const std::string& key) { return key == "VK_LAYER_KHRONOS_validation"; }

static bool IsProfilesLayer(const std::string& key) { return key == "VK_LAYER_KHRONOS_profiles"; }

bool Parameter::ApplyPresetSettings(const LayerPreset& preset) {
    for (std::size_t preset_index = 0, preset_count = preset.settings.size(); preset_index < preset_count; ++preset_index) {
        const SettingData* preset_setting = preset.settings[preset_index];

        for (std::size_t i = 0, n = this->settings.size(); i < n; ++i) {
            SettingData* current_setting = this->settings[i];

            if (current_setting->key != preset_setting->key) {
                continue;
            }

            if (current_setting->type != preset_setting->type) {
                continue;
            }

            this->settings[i]->Copy(preset_setting);
        }
    }

    return true;
}

ParameterRank GetParameterOrdering(const LayerManager& layers, const Parameter& parameter) {
    assert(!parameter.key.empty());

    const Layer* layer = layers.Find(parameter.key, parameter.api_version);
    if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
        return PARAMETER_RANK_UNORDERED_LAYER;
    } else if (layer == nullptr) {
        return PARAMETER_RANK_MISSING_LAYER;
    } else if (IsValidationLayer(layer->key)) {
        return PARAMETER_RANK_VALIDATION_LAYER;
    } else if (IsProfilesLayer(layer->key)) {
        return PARAMETER_RANK_PROFILES_LAYER;
    } else if (IsExtensionLayer(layer->key)) {
        return PARAMETER_RANK_EXTENSION_LAYER;
    } else if (layer->type == LAYER_TYPE_IMPLICIT) {
        return PARAMETER_RANK_IMPLICIT_LAYER;
    } else if (layer->type == LAYER_TYPE_EXPLICIT) {
        return PARAMETER_RANK_EXPLICIT_LAYER;
    } else {
        assert(0);  // Unknown ordering
        return PARAMETER_RANK_EXPLICIT_LAYER;
    }
}

void OrderParameter(std::vector<Parameter>& parameters, const LayerManager& layers) {
    struct ParameterCompare {
        ParameterCompare(const LayerManager& layers) : layers(layers) {}

        bool operator()(const Parameter& a, const Parameter& b) const {
            const ParameterRank rankA = GetParameterOrdering(layers, a);
            const ParameterRank rankB = GetParameterOrdering(layers, b);

            const bool both_overridden = a.overridden_rank != Parameter::NO_RANK && b.overridden_rank != Parameter::NO_RANK;

            if (both_overridden)
                return a.overridden_rank < b.overridden_rank;
            else if (rankA == rankB)
                return a.key < b.key;
            else
                return rankA < rankB;
        }

        const LayerManager& layers;
    };

    std::sort(parameters.begin(), parameters.end(), ParameterCompare(layers));

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].control == LAYER_CONTROL_ON)
            parameters[i].overridden_rank = static_cast<int>(i);
        else
            parameters[i].overridden_rank = Parameter::NO_RANK;
    }
}

bool HasMissingLayer(const std::vector<Parameter>& parameters, const LayerManager& layers, std::string& missing_layer) {
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        if (it->builtin != LAYER_BUILTIN_NONE) {
            continue;  // If application API or ENV layers
        }

        if (it->control == LAYER_CONTROL_OFF) {
            continue;  // If excluded are missing, it doesn't matter
        }

        if (!(it->platform_flags & (1 << VKC_PLATFORM))) {
            continue;  // If unsupported are missing, it doesn't matter
        }

        if (layers.Find(it->key, it->api_version) == nullptr) {
            missing_layer = it->key;
            return true;
        }
    }
    return false;
}
