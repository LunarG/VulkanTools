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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configuration.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Configuration::Configuration() : _preset(ValidationPresetNone), _all_layers_available(true) {}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
bool Configuration::IsLayerAvailable(const QString& layer_name, const QString& full_path) const {
    for (std::size_t i = 0, n = _layers.size(); i < n; ++i)
        if (_layers[i]._name == layer_name && _layers[i]._layer_path == full_path) return true;

    return false;
}

///////////////////////////////////////////////////////////
// Find the layer if it exists. Only care about the name
Layer* Configuration::FindLayerNamed(const QString& layer_name) {
    for (std::size_t i = 0, n = _layers.size(); i < n; ++i)
        if (_layers[i]._name == layer_name) return &_layers[i];

    return nullptr;
}

////////////////////////////////////////////////////////////
// Copy a profile so we can mess with it.
Configuration* Configuration::DuplicateConfiguration() {
    Configuration* duplicate = new Configuration;
    duplicate->_name = _name;
    duplicate->_file = _file;
    duplicate->_description = _description;
    duplicate->_excluded_layers = _excluded_layers;
    duplicate->_preset = _preset;
    duplicate->_all_layers_available = _all_layers_available;
    // Do not copy ->bFixedProfile

    for (int i = 0; i < _layers.size(); i++) {
        duplicate->_layers.push_back(_layers[i]);
    }

    return duplicate;
}

///////////////////////////////////////////////////////////
/// Remove unused layers and build the list of
/// black listed layers.
void Configuration::CollapseConfiguration() {
    _excluded_layers.clear();

    std::vector<Layer> collapsed_layers;
    collapsed_layers.reserve(_layers.size()); // Do a single memory allocation

    int new_rank = 0;
    for (std::size_t i = 0, n = _layers.size(); i < n; ++i) {
        switch (_layers[i]._layer_state) {
            case LAYER_STATE_EXCLUDED:
                _excluded_layers << _layers[i]._name;
                break;
            case LAYER_STATE_OVERRIDDEN:
                _layers[i]._rank = new_rank++;
                collapsed_layers.push_back(_layers[i]);
                break;
            case LAYER_STATE_APPLICATION_CONTROLLED:
                break;
        }
    }

    _layers = collapsed_layers;
}
