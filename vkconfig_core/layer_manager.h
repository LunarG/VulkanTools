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

#pragma once

#include "layer.h"
#include "path.h"

#include <string>
#include <vector>
#include <memory>

enum LayersPaths {
    LAYERS_PATHS_IMPLICIT = 0,
    LAYERS_PATHS_EXPLICIT,
    LAYERS_PATHS_ENV_SET,  // From $VK_LAYER_PATH
    LAYERS_PATHS_ENV_ADD,  // from $VK_ADD_LAYER_PATH
    LAYERS_PATHS_GUI,
    LAYERS_PATHS_SDK,

    LAYERS_PATHS_FIRST = LAYERS_PATHS_IMPLICIT,
    LAYERS_PATHS_LAST = LAYERS_PATHS_SDK,
};

enum { LAYERS_PATHS_COUNT = LAYERS_PATHS_LAST - LAYERS_PATHS_FIRST + 1 };

LayerType GetLayerType(LayersPaths Layers_paths_type);

class LayerManager {
   public:
    LayerManager(const std::vector<Path>& user_defined_paths = std::vector<Path>());

    void Clear();
    bool Empty() const;
    std::size_t Size() const;

    Layer* Find(const std::string& layer_name);
    const Layer* Find(const std::string& layer_name) const;

    void LoadAllInstalledLayers(const std::map<std::string, std::string>& layers_validated);

    void LoadLayersFromPath(const Path& layers_path, const std::map<std::string, std::string>& layers_validated,
                            LayerType type = LAYER_TYPE_EXPLICIT);

    std::vector<Layer> selected_layers;

   private:
    bool IsAvailable(const Layer& layer) const;

    std::array<std::vector<Path>, LAYERS_PATHS_COUNT> paths;
};
