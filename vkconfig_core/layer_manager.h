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
#include "serialization.h"
#include "type_layers_paths.h"

#include <string>
#include <vector>
#include <memory>

class LayerManager : public Serialize {
   public:
    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;
    std::string Log() const override;

    void Clear();
    bool Empty() const;
    std::size_t Size() const;

    std::vector<Path> GatherManifests(const std::string& layer_name) const;
    std::vector<Version> GatherVersions(const std::string& layer_name) const;
    const Layer* Find(const std::string& layer_name, const Version& version = Version::LATEST) const;
    const Layer* FindLastModified(const std::string& layer_name, const Version& version) const;
    const Layer* FindFromManifest(const Path& manifest_path, bool find_disabled_layers = false) const;
    Layer* FindFromManifest(const Path& manifest_path, bool find_disabled_layers = false);

    void LoadAllInstalledLayers();
    void LoadLayersFromPath(const Path& layers_path, LayerType type = LAYER_TYPE_EXPLICIT);
    bool LoadLayer(const Path& layer_path, LayerType type = LAYER_TYPE_EXPLICIT);

    void AppendPath(const LayersPathInfo& path_info);
    void RemovePath(const LayersPathInfo& path_info);
    void UpdatePathEnabled(const LayersPathInfo& path_info);

    std::vector<std::string> BuildLayerNameList() const;

    std::vector<Layer> selected_layers;
    std::array<std::vector<LayersPathInfo>, LAYERS_PATHS_COUNT> paths;
    Path last_layers_path = Get(Path::HOME);
    bool validate_manifests = true;

   private:
    void InitSystemPaths();

    std::map<Path, std::string> layers_validated;
};
