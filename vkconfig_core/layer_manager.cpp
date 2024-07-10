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

LayerManager::LayerManager(const std::vector<Path> &user_defined_paths) {
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

    this->paths[LAYERS_PATHS_GUI].insert(this->paths[LAYERS_PATHS_GUI].begin(), user_defined_paths.begin(),
                                         user_defined_paths.end());

    this->paths[LAYERS_PATHS_SDK].push_back(::Get(Path::SDK_BIN));

    this->LoadAllInstalledLayers();
}

void LayerManager::Clear() { this->selected_layers.clear(); }

bool LayerManager::Empty() const { return this->selected_layers.empty(); }

std::size_t LayerManager::Size() const { return this->selected_layers.size(); }

Layer *LayerManager::Find(const std::string &layer_name) {
    assert(!layer_name.empty());

    return FindByKey(this->selected_layers, layer_name.c_str());
}

const Layer *LayerManager::Find(const std::string &layer_name) const {
    assert(!layer_name.empty());

    return FindByKey(this->selected_layers, layer_name.c_str());
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
        Layer layer;
        if (layer.Load(layers_paths[i], type)) {
            if (this->IsAvailable(layer)) {
                continue;
            }

            this->selected_layers.push_back(layer);
        }
    }
}

bool LayerManager::IsAvailable(const Layer &layer) const {
    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (this->selected_layers[i].key == layer.key && this->selected_layers[i].api_version == layer.api_version) {
            return true;
        }
    }

    return false;
}
