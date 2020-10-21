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

#include "configuration_layer.h"
#include "layer.h"

enum LayerRank {
    LAYER_RANK_MISSING = 0,
    LAYER_RANK_EXCLUDED,
    LAYER_RANK_IMPLICIT_AVAILABLE,
    LAYER_RANK_IMPLICIT_OVERRIDDEN,
    LAYER_RANK_EXPLICIT_OVERRIDDEN,
    LAYER_RANK_EXPLICIT_AVAILABLE
};

LayerRank GetLayerRank(const std::vector<Layer>& available_layers, const ConfigurationLayer& configuration_layer);

void SortConfigurationLayers(std::vector<ConfigurationLayer>& configuration_layers, const std::vector<Layer>& layers);

void FilterConfiguratorLayers(std::vector<ConfigurationLayer>& configuration_layers, const LayerState state);

std::vector<ConfigurationLayer>::iterator FindConfigurationLayer(std::vector<ConfigurationLayer>& configuration_layers,
                                                                 const QString& layer_name);

bool HasMissingLayer(const std::vector<ConfigurationLayer>& configuration_layers, const std::vector<Layer>& layers);
