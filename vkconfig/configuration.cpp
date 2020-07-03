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

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "configuration.h"

Configuration::Configuration() {
    preset_index = 0;
    all_layers_available = true;
}

Configuration::~Configuration() {
    qDeleteAll(layers.begin(), layers.end());
    layers.clear();
}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
LayerFile* Configuration::FindLayer(const QString& layer_name, const QString& full_path) const {
    for (int i = 0; i < layers.size(); i++)
        if (layers[i]->name == layer_name && layers[i]->qsLayerPath == full_path) return layers[i];

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
Configuration* Configuration::DuplicateProfile() {
    Configuration* pDuplicate = new Configuration;
    pDuplicate->name = name;
    pDuplicate->file = file;
    pDuplicate->description = description;
    pDuplicate->excluded_layers = excluded_layers;
    pDuplicate->preset_index = preset_index;
    pDuplicate->all_layers_available = all_layers_available;
    // Do not copy ->bFixedProfile

    for (int i = 0; i < layers.size(); i++) {
        LayerFile* pLayer = new LayerFile;
        layers[i]->CopyLayer(pLayer);
        pDuplicate->layers.push_back(pLayer);
    }

    return pDuplicate;
}

///////////////////////////////////////////////////////////
/// \brief CProfileDef::CollapseProfile
/// Remove unused layers and build the list of
/// black listed layers.
void Configuration::CollapseProfile() {
    excluded_layers.clear();

    // Look for black listed layers, add them to the
    // string list of names, but remove them from
    // the list of layers
    int iCurrent = 0;
    int nNewRank = 0;
    while (iCurrent < layers.size()) {
        // Remove this layer?
        if (layers[iCurrent]->bDisabled) {
            excluded_layers << layers[iCurrent]->name;
            layers.removeAt(iCurrent);
            continue;
        }

        // If the layer is not active, also remove it
        // Important to do black list test FIRST
        if (!layers[iCurrent]->bActive) {
            layers.removeAt(iCurrent);
            continue;
        }

        // We are keeping this layer, reset it's rank
        layers[iCurrent]->nRank = nNewRank++;

        iCurrent++;
    }
}
