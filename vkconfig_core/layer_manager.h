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

#include "layer.h"
#include "environment.h"

#include <QStringList>

#include <string>
#include <vector>

class LayerManager {
   public:
    LayerManager(const Environment& environment);

    void Clear();
    bool Empty() const;

    void LoadAllInstalledLayers();
    void LoadLayersFromPath(const std::string& path, std::vector<Layer>& layers);

    std::vector<Layer> available_layers;

    const Environment& environment;
};
