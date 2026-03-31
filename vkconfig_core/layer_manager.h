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
    LayerId id;
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

    std::vector<Path> GatherManifests(const std::string& layer_key) const;
    std::vector<Version> GatherVersions(const std::string& layer_key) const;

    const Layer* Find(LayerId id, bool enable_only = true) const;
    Layer* Find(LayerId id, bool enable_only = true);

    const Layer* Find(const std::string& layer_key, const Version& version = Version::LATEST) const;
    const Layer* FindLastModified(const std::string& layer_key, const Version& version) const;
    const Layer* FindFromManifest(const Path& manifest_path, bool find_disabled_layers = false) const;
    Layer* FindFromManifest(const Path& manifest_path, bool find_disabled_layers = false);

    void LoadAllInstalledLayers(ConfiguratorMode configurator_mode);
    LayerLoadStatus LoadLayers(const Path& layer_path, LayerType type, ConfiguratorMode configurator_mode);

    void RemoveLayer(LayerId id);
    void EnableLayer(LayerId id, bool enable);

    std::set<LayerDisplay> BuildLayerDisplayList() const;
    std::map<Path, std::map<std::string, LayerDisplay>> BuildLayerStoreList() const;
    std::vector<Path> BuildLayerPaths() const;

    std::vector<std::string> GatherLayerNames() const;

    std::set<Path> gui_added_layers_paths;

    std::vector<Layer> available_layers;
    Path last_layers_dir = Path(Path::HOME);
    bool validate_manifests = false;

    void AppendInit(const Path& path, const std::vector<LayerDisplay>& layers) {
        this->layer_init.insert(std::make_pair(path, layers));
    }

   private:
    LayerLoadStatus LoadLayer(const QJsonObject& json_layer_object, const Path& layer_path, LayerType type,
                              const std::string& last_modified, Version file_format_version, LayerDescriptor descriptor);

    void ApplyLayerDescriptor();

    std::map<Path, std::vector<LayerDisplay>> layer_init;
};
