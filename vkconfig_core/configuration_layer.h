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

#include "configuration_setting.h"
#include "layer.h"

#include <QString>
#include <QJsonObject>

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

struct ConfigurationLayer {
    static const int UNRANKED = -1;

    typedef std::vector<ConfigurationSetting> ConfigurationSettings;

    ConfigurationLayer() : state(LAYER_STATE_APPLICATION_CONTROLLED), overridden_rank(UNRANKED) {}
    ConfigurationLayer(const QString& name, const LayerState state) : name(name), state(state), overridden_rank(UNRANKED) {}

    QString name;
    LayerState state;
    ConfigurationSettings settings;
    int overridden_rank;
};

bool LoadConfigurationSettings(const QJsonObject& layer_settings_descriptors, ConfigurationLayer& configuration_layer);

void FilterConfiguratorLayers(std::vector<ConfigurationLayer>& layers, const LayerState state);

std::vector<ConfigurationLayer>::iterator FindConfigurationLayer(std::vector<ConfigurationLayer>& layers,
                                                                 const QString& layer_name);

bool HasMissingLayer(const std::vector<ConfigurationLayer>& configuration_layers, const std::vector<Layer>& layers);
