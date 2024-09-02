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

#include "layer_manager.h"
#include "util.h"
#include "type_platform.h"
#include "registry.h"

#include <QJsonArray>

LayerType GetLayerType(LayersPaths Layers_paths_type) {
    if (Layers_paths_type == LAYERS_PATHS_IMPLICIT) {
        return LAYER_TYPE_IMPLICIT;
    } else {
        return LAYER_TYPE_EXPLICIT;
    }
}

std::vector<Path> GetImplicitLayerPaths() {
    std::vector<Path> result;

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    static const char *REGISTRY_PATHS[] = {
        "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers",
        "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers",
        "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers"};

    for (std::size_t i = 0, n = std::size(REGISTRY_PATHS); i < n; ++i) {
        const std::vector<Path> &registry_paths = LoadRegistryLayers(REGISTRY_PATHS[i]);
        result.insert(result.begin(), registry_paths.begin(), registry_paths.end());
    }
#else
    static const char *LAYERS_PATHS[] = {
        "/usr/local/etc/vulkan/implicit_layer.d",  // Not used on macOS, okay to just ignore
        "/usr/local/share/vulkan/implicit_layer.d",
        "/etc/vulkan/implicit_layer.d",
        "/usr/share/vulkan/implicit_layer.d",
        ".local/share/vulkan/implicit_layer.d",
#ifdef INSTALL_FULL_DATAROOTDIR
        INSTALL_FULL_DATAROOTDIR "/vulkan/implicit_layer.d",
#endif
#ifdef INSTALL_FULL_SYSCONFDIR
        INSTALL_FULL_SYSCONFDIR "/vulkan/implicit_layer.d",
#endif
    };

    for (std::size_t i = 0, n = std::size(LAYERS_PATHS); i < n; ++i) {
        result.push_back(LAYERS_PATHS[i]);
    }
#endif

    return result;
}

std::vector<Path> GetExplicitLayerPaths() {
    std::vector<Path> result;

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    static const char *REGISTRY_PATHS[] = {
        "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers",
        "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers",
        "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers"};

    for (std::size_t i = 0, n = std::size(REGISTRY_PATHS); i < n; ++i) {
        const std::vector<Path> &registry_paths = LoadRegistryLayers(REGISTRY_PATHS[i]);
        result.insert(result.begin(), registry_paths.begin(), registry_paths.end());
    }
#else
    static const char *LAYERS_PATHS[] = {
        "/usr/local/etc/vulkan/explicit_layer.d",  // Not used on macOS, okay to just ignore
        "/usr/local/share/vulkan/explicit_layer.d",
        "/etc/vulkan/explicit_layer.d",
        "/usr/share/vulkan/explicit_layer.d",
        ".local/share/vulkan/explicit_layer.d",
#ifdef INSTALL_FULL_DATAROOTDIR
        INSTALL_FULL_DATAROOTDIR "/vulkan/explicit_layer.d",
#endif
#ifdef INSTALL_FULL_SYSCONFDIR
        INSTALL_FULL_SYSCONFDIR "/vulkan/explicit_layer.d",
#endif
    };

    for (std::size_t i = 0, n = std::size(LAYERS_PATHS); i < n; ++i) {
        result.push_back(LAYERS_PATHS[i]);
    }
#endif

    return result;
}

LayerManager::LayerManager() {}

LayerManager::~LayerManager() {}

bool LayerManager::Load(const QJsonObject &json_root_object) {
    this->paths[LAYERS_PATHS_IMPLICIT] = GetImplicitLayerPaths();

    this->paths[LAYERS_PATHS_EXPLICIT] = GetExplicitLayerPaths();

    const char *SEPARATOR = GetToken(PARSE_ENV_VAR);

    const std::vector<std::string> &VK_LAYER_PATH = UniqueStrings(Split(qgetenv("VK_LAYER_PATH").toStdString(), SEPARATOR));
    for (std::size_t i = 0, n = VK_LAYER_PATH.size(); i < n; ++i) {
        this->paths[LAYERS_PATHS_ENV_SET].push_back(VK_LAYER_PATH[i]);
    }

    const std::vector<std::string> &VK_ADD_LAYER_PATH = UniqueStrings(Split(qgetenv("VK_ADD_LAYER_PATH").toStdString(), SEPARATOR));
    for (std::size_t i = 0, n = VK_ADD_LAYER_PATH.size(); i < n; ++i) {
        this->paths[LAYERS_PATHS_ENV_SET].push_back(VK_ADD_LAYER_PATH[i]);
    }

    this->paths[LAYERS_PATHS_SDK].push_back(::Get(Path::SDK_BIN));

    if (json_root_object.value("layers") != QJsonValue::Undefined) {
        const QJsonObject &json_layers_object = json_root_object.value("layers").toObject();

        if (json_layers_object.value("validated") != QJsonValue::Undefined) {
            const QJsonObject &json_layers_validated_object = json_layers_object.value("validated").toObject();

            const QStringList &json_layers_validated_keys = json_layers_validated_object.keys();

            for (int i = 0, n = json_layers_validated_keys.length(); i < n; ++i) {
                const std::string &manifest_path = json_layers_validated_keys[i].toStdString();
                const std::string &last_modified =
                    json_layers_validated_object.value(manifest_path.c_str()).toString().toStdString();
                this->layers_validated.insert(std::make_pair(manifest_path, last_modified));
            }
        }

        if (json_layers_object.value("user_defined_paths") != QJsonValue::Undefined) {
            const QJsonArray &json_array = json_layers_object.value("user_defined_paths").toArray();

            for (int i = 0, n = json_array.size(); i < n; ++i) {
                this->paths[LAYERS_PATHS_GUI].push_back(json_array[i].toString().toStdString());
            }
        }
    }

    this->LoadAllInstalledLayers();

    return true;
}

bool LayerManager::Save(QJsonObject &json_root_object) const {
    QJsonObject json_layers_paths_object;
    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        const Layer &layer = this->selected_layers[i];

        json_layers_paths_object.insert(layer.manifest_path.AbsolutePath().c_str(), layer.validated_last_modified.c_str());
    }

    QJsonArray json_user_defined_paths_array;
    for (std::size_t i = 0, n = this->paths[LAYERS_PATHS_GUI].size(); i < n; ++i) {
        json_user_defined_paths_array.append(this->paths[LAYERS_PATHS_GUI][i].RelativePath().c_str());
    }

    QJsonObject json_layers_object;
    json_layers_object.insert("validated", json_layers_paths_object);
    json_layers_object.insert("user_defined_paths", json_user_defined_paths_array);

    json_root_object.insert("layers", json_layers_object);

    return true;
}

void LayerManager::Reset() {
    this->selected_layers.clear();
    this->layers_validated.clear();
    this->paths[LAYERS_PATHS_SDK].clear();
}

void LayerManager::Clear() { this->selected_layers.clear(); }

bool LayerManager::Empty() const { return this->selected_layers.empty(); }

std::size_t LayerManager::Size() const { return this->selected_layers.size(); }

std::vector<Version> LayerManager::GatherVersions(const std::string &layer_name) const {
    std::vector<Version> result;

    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (this->selected_layers[i].key == layer_name) {
            result.push_back(this->selected_layers[i].api_version);
        }
    }

    return result;
}

const Layer *LayerManager::FindFromVersion(const std::string &layer_name, const Version &layer_version) const {
    // Version::VERSION_NULL refer to latest version
    if (layer_version == Version::VERSION_NULL) {
        const std::vector<Version> &version = this->GatherVersions(layer_name);
        if (version.empty()) {
            return nullptr;
        }

        Version latest = Version::VERSION_NULL;
        for (std::size_t i = 0, n = version.size(); i < n; ++i) {
            if (latest == Version::VERSION_NULL) {
                latest = version[i];
            } else if (version[i] > latest) {
                latest = version[i];
            }
        }
        assert(latest != Version::VERSION_NULL);

        return FindFromVersion(layer_name, latest);
    } else {
        for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
            if (this->selected_layers[i].key != layer_name) {
                continue;
            }
            if (this->selected_layers[i].api_version != layer_version) {
                continue;
            }

            return &this->selected_layers[i];
        }
    }

    return nullptr;
}

const Layer *LayerManager::FindFromManifest(const Path &manifest_path) const {
    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (this->selected_layers[i].manifest_path == manifest_path) {
            return &this->selected_layers[i];
        }
    }
    return nullptr;
}

Layer *LayerManager::FindFromManifest(const Path &manifest_path) {
    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (this->selected_layers[i].manifest_path == manifest_path) {
            return &this->selected_layers[i];
        }
    }
    return nullptr;
}

// Find all installed layers on the system.
void LayerManager::LoadAllInstalledLayers() {
    this->selected_layers.clear();

    for (std::size_t group_index = 0, group_count = this->paths.size(); group_index < group_count; ++group_index) {
        const LayerType layer_type = ::GetLayerType(static_cast<LayersPaths>(group_index));

        const std::vector<Path> &paths_group = this->paths[group_index];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            this->LoadLayersFromPath(paths_group[i], layer_type);
        }
    }
}

void LayerManager::LoadLayersFromPath(const Path &layers_path, LayerType type) {
    const std::vector<Path> &layers_paths = layers_path.IsDir() ? CollectFilePaths(layers_path) : GetVector(layers_path);

    for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
        const std::string &last_modified = layers_paths[i].LastModified();

        Layer *already_loaded_layer = this->FindFromManifest(layers_paths[i]);
        if (already_loaded_layer != nullptr) {
            // Already loaded
            auto it = this->layers_validated.find(layers_paths[i].AbsolutePath());
            if (it != layers_validated.end()) {
                if (last_modified == it->second) {
                    continue;  // Already loaded and up to date
                }
            }

            // Reload
            already_loaded_layer->Load(layers_paths[i], this->layers_validated, type);
        } else {
            Layer layer;
            if (layer.Load(layers_paths[i], this->layers_validated, type)) {
                this->selected_layers.push_back(layer);
            }
        }
    }
}
