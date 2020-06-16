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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 *
 * Layer files are JSON documents, so a layer file object is derived
 * from QJsonDocument and is given several useful utility access methods
 * for querying and manipulating layer .json files.
 *
 */

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "profiledef.h"
#include "vulkanconfiguration.h"

CProfileDef::CProfileDef()
    {
    nPresetIndex = 0;
    }

CProfileDef::~CProfileDef()
    {
    qDeleteAll(layers.begin(), layers.end());
    layers.clear();
    }

///////////////////////////////////////////////////////////
// Find the layer if it exists.
CLayerFile* CProfileDef::FindLayer(QString qsLayerName, QString qsFullPath)
    {
    for(int i = 0; i < layers.size(); i++)
        if(layers[i]->name == qsLayerName && layers[i]->qsLayerPath == qsFullPath)
            return layers[i];

    return nullptr;
    }

///////////////////////////////////////////////////////////
// Find the layer if it exists. Only care about the name
CLayerFile* CProfileDef::FindLayerNamed(QString qsLayerName)
    {
    for(int i = 0; i < layers.size(); i++)
        if(layers[i]->name == qsLayerName)
            return layers[i];

    return nullptr;
    }



///////////////////////////////////////////////////////////
// Convienience function
// Retrieve the Khronos validation layer if it is included
CLayerFile* CProfileDef::GetKhronosLayer(void)
    {
    return FindLayerNamed("VK_LAYER_KHRONOS_validation");
    }


////////////////////////////////////////////////////////////
// Copy a profile so we can mess with it.
CProfileDef* CProfileDef::DuplicateProfile(void)
    {
    CProfileDef *pDuplicate = new CProfileDef;
    pDuplicate->qsProfileName = qsProfileName;
    pDuplicate->qsFileName = qsFileName;
    pDuplicate->qsDescription = qsDescription;
    pDuplicate->blacklistedLayers = blacklistedLayers;
    pDuplicate->nPresetIndex = nPresetIndex;
    // Do not copy ->bFixedProfile

    for(int i = 0; i < layers.size(); i++) {
        CLayerFile* pLayer = new CLayerFile;
        layers[i]->CopyLayer(pLayer);
        pDuplicate->layers.push_back(pLayer);
        }

    return pDuplicate;
    }


///////////////////////////////////////////////////////////
/// \brief CProfileDef::CollapseProfile
/// Remove unused layers and build the list of
/// black listed layers.
void CProfileDef::CollapseProfile()
    {
    blacklistedLayers.clear();

    // Look for black listed layers, add them to the
    // string list of names, but remove them from
    // the list of layers
    int iCurrent = 0;
    int nNewRank = 0;
    while(iCurrent < layers.size())
        {
        // Remove this layer?
        if(layers[iCurrent]->bDisabled) {
            blacklistedLayers << layers[iCurrent]->name;
            layers.removeAt(iCurrent);
            continue;
            }

        // If the layer is not active, also remove it
        // Important to do black list test FIRST
        if(!layers[iCurrent]->bActive) {
            layers.removeAt(iCurrent);
            continue;
            }

        // We are keeping this layer, reset it's rank
        layers[iCurrent]->nRank = nNewRank++;

        iCurrent++;
        }
    }
