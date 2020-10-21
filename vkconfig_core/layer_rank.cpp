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

#include "layer_rank.h"
#include "util.h"

#include <cassert>
#include <algorithm>

LayerRank GetLayerRank(const std::vector<Layer>& available_layers, const ConfigurationLayer& configuration_layer) {
    assert(!configuration_layer.name.isEmpty());

    const std::vector<Layer>::const_iterator layer = Find(available_layers, configuration_layer.name);
    if (layer == available_layers.end()) {
        return LAYER_RANK_MISSING;
    } else if (configuration_layer.state == LAYER_STATE_EXCLUDED) {
        return LAYER_RANK_EXCLUDED;
    } else if (configuration_layer.state == LAYER_STATE_APPLICATION_CONTROLLED && layer->_layer_type == LAYER_TYPE_IMPLICIT) {
        return LAYER_RANK_IMPLICIT_AVAILABLE;
    } else if (configuration_layer.state == LAYER_STATE_OVERRIDDEN && layer->_layer_type == LAYER_TYPE_IMPLICIT) {
        return LAYER_RANK_IMPLICIT_OVERRIDDEN;
    } else if (configuration_layer.state == LAYER_STATE_OVERRIDDEN && layer->_layer_type != LAYER_TYPE_IMPLICIT) {
        return LAYER_RANK_EXPLICIT_OVERRIDDEN;
    } else if (configuration_layer.state == LAYER_STATE_APPLICATION_CONTROLLED && layer->_layer_type != LAYER_TYPE_IMPLICIT) {
        return LAYER_RANK_EXPLICIT_AVAILABLE;
    } else {
        assert(0);  // Unknown ordering
        return LAYER_RANK_MISSING;
    }
}

void SortConfigurationLayers(std::vector<ConfigurationLayer>& configuration_layers, const std::vector<Layer>& layers) {
    struct ConfigurationLayerCompare {
        ConfigurationLayerCompare(const std::vector<Layer>& layers) : layers(layers) {}

        bool operator()(const ConfigurationLayer& a, const ConfigurationLayer& b) const {
            const LayerRank rankA = GetLayerRank(layers, a);
            const LayerRank rankB = GetLayerRank(layers, b);
            if (rankA == rankB && a.state == LAYER_STATE_OVERRIDDEN) {
                if (a.overridden_rank != ConfigurationLayer::UNRANKED && b.overridden_rank != ConfigurationLayer::UNRANKED)
                    return a.overridden_rank < b.overridden_rank;
                else if (a.name == "VK_LAYER_LUNARG_device_simulation")
                    return false;
                else if (b.name == "VK_LAYER_LUNARG_device_simulation")
                    return true;
                else if (a.name == "VK_LAYER_KHRONOS_validation" && b.name == "VK_LAYER_LUNARG_device_simulation")
                    return true;
                else if (a.name == "VK_LAYER_KHRONOS_validation")
                    return false;
                else
                    return a.name < b.name;
            } else if (rankA == rankB && a.state != LAYER_STATE_OVERRIDDEN)
                return a.name < b.name;
            else
                return rankA < rankB;
        }

        const std::vector<Layer>& layers;
    };

    std::sort(configuration_layers.begin(), configuration_layers.end(), ConfigurationLayerCompare(layers));

    for (std::size_t i = 0, n = layers.size(); i < n; ++i) {
        configuration_layers[i].overridden_rank = static_cast<int>(i);
    }
}

void FilterConfiguratorLayers(std::vector<ConfigurationLayer>& layers, const LayerState state) {
    std::vector<ConfigurationLayer> filtered_parameters;

    for (std::size_t i = 0, n = layers.size(); i < n; ++i) {
        if (layers[i].state == state) continue;

        filtered_parameters.push_back(layers[i]);
    }

    layers = filtered_parameters;
}
