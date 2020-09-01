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
#include "configurator.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <cassert>

Configuration::Configuration() : _preset(ValidationPresetNone) {}

Configuration::~Configuration() {
    qDeleteAll(_layers.begin(), _layers.end());
    _layers.clear();
}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
Layer* Configuration::FindLayer(const QString& layer_name, const QString& full_path) const {
    assert(!layer_name.isEmpty());
    assert(!full_path.isEmpty());

    for (int i = 0; i < _layers.size(); i++)
        if (_layers[i]->_name == layer_name && _layers[i]->_layer_path == full_path) return _layers[i];

    return nullptr;
}

///////////////////////////////////////////////////////////
// Find the layer if it exists. Only care about the name
Layer* Configuration::FindLayerNamed(const QString& layer_name) const {
    assert(!layer_name.isEmpty());

    for (int i = 0; i < _layers.size(); i++)
        if (_layers[i]->_name == layer_name) return _layers[i];

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
    // Do not copy ->bFixedProfile

    for (int i = 0; i < _layers.size(); i++) {
        Layer* layer_file = new Layer;
        _layers[i]->CopyLayer(layer_file);
        duplicate->_layers.push_back(layer_file);
    }

    return duplicate;
}

///////////////////////////////////////////////////////////
/// Remove unused layers and build the list of
/// black listed layers.
void Configuration::CollapseConfiguration() {
    _excluded_layers.clear();

    // Look for black listed layers, add them to the
    // string list of names, but remove them from
    // the list of layers
    int layer_index = 0;
    int new_rank = 0;
    while (layer_index < _layers.size()) {
        if (!_layers[layer_index]->IsValid()) continue;

        // Remove this layer?
        if (_layers[layer_index]->_state == LAYER_STATE_EXCLUDED) {
            _excluded_layers << _layers[layer_index]->_name;
            _layers.removeAt(layer_index);
            continue;
        }

        // If the layer is not active, also remove it
        // Important to do black list test FIRST
        if (!_layers[layer_index]->_state == LAYER_STATE_OVERRIDDEN) {
            _layers.removeAt(layer_index);
            continue;
        }

        // We are keeping this layer, reset it's rank
        _layers[layer_index]->_rank = new_rank++;

        layer_index++;
    }
}

bool Configuration::IsValid() const {
    Configurator& configurator = Configurator::Get();

    if (_excluded_layers.empty() && _layers.empty()) return false;

    for (int i = 0, n = _layers.size(); i < n; ++i) {
        if (configurator.FindLayerNamed(_layers[i]->_name) == nullptr) return false;
    }

    for (int i = 0, n = _excluded_layers.size(); i < n; ++i) {
        if (configurator.FindLayerNamed(_excluded_layers[i]) == nullptr) return false;
    }

    return true;
}
