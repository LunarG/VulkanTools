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

Configuration::Configuration() : preset_index(0), all_layers_available(true) {}

Configuration::~Configuration() {
    qDeleteAll(layers.begin(), layers.end());
    layers.clear();
}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
LayerFile* Configuration::FindLayer(const QString& layer_name, const QString& full_path) const {
    for (int i = 0; i < layers.size(); i++)
        if (layers[i]->name == layer_name && layers[i]->layer_path == full_path) return layers[i];

    return nullptr;
}

///////////////////////////////////////////////////////////
// Find the layer if it exists. Only care about the name
LayerFile* Configuration::FindLayerNamed(const QString& layer_name) const {
    for (int i = 0; i < layers.size(); i++)
        if (layers[i]->name == layer_name) return layers[i];

    return nullptr;
}

///////////////////////////////////////////////////////////
// Convienience function
// Retrieve the Khronos validation layer if it is included
LayerFile* Configuration::GetKhronosLayer() { return FindLayerNamed("VK_LAYER_KHRONOS_validation"); }

////////////////////////////////////////////////////////////
// Copy a profile so we can mess with it.
Configuration* Configuration::DuplicateConfiguration() {
    Configuration* duplicate = new Configuration;
    duplicate->name = name;
    duplicate->file = file;
    duplicate->description = description;
    duplicate->excluded_layers = excluded_layers;
    duplicate->preset_index = preset_index;
    duplicate->all_layers_available = all_layers_available;
    // Do not copy ->bFixedProfile

    for (int i = 0; i < layers.size(); i++) {
        LayerFile* layer_file = new LayerFile;
        layers[i]->CopyLayer(layer_file);
        duplicate->layers.push_back(layer_file);
    }

    return duplicate;
}

///////////////////////////////////////////////////////////
/// Remove unused layers and build the list of
/// black listed layers.
void Configuration::CollapseConfiguration() {
    excluded_layers.clear();

    // Look for black listed layers, add them to the
    // string list of names, but remove them from
    // the list of layers
    int layer_index = 0;
    int nNewRank = 0;
    while (layer_index < layers.size()) {
        // Remove this layer?
        if (layers[layer_index]->disabled) {
            excluded_layers << layers[layer_index]->name;
            layers.removeAt(layer_index);
            continue;
        }

        // If the layer is not active, also remove it
        // Important to do black list test FIRST
        if (!layers[layer_index]->enabled) {
            layers.removeAt(layer_index);
            continue;
        }

        // We are keeping this layer, reset it's rank
        layers[layer_index]->rank = nNewRank++;

        layer_index++;
    }
}
