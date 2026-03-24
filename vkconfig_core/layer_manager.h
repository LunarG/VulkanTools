/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
#include <set>

struct LayerDisplay {
    std::string key;
    Path manifest_path;
    Version api_version;
    LayerDescriptor descriptor;
};

bool operator<(const LayerDisplay& a, const LayerDisplay& b);

class LayerManager : public Serialize {
   public:
    LayerManager();

    bool Load(const QJsonObject& json_root_object, ConfiguratorMode configurator_mode) override;
    bool Save(QJsonObject& json_root_object) const override;
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

    void LoadAllInstalledLayers(ConfiguratorMode configurator_mode);
    LayerLoadStatus LoadLayer(const Path& layer_path, LayerType type, ConfiguratorMode configurator_mode);

    void AppendPath(const Path& path, LayerType type, bool added = false);
    void RemovePath(const Path& path);

    bool IsEnabled(const Path& manifest_path) const;
    void Enable(const Path& manifest_path, bool enabled);

    std::set<LayerDisplay> BuildLayerDisplayList() const;

    std::vector<std::string> GatherLayerNames() const;

    std::vector<Layer> available_layers;
    Path last_layers_dir = Path(Path::HOME);
    bool validate_manifests = false;
    std::map<Path, LayerDescriptor> layers_found;

   private:
    std::set<Path> layers_removed;
};
