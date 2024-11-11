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

std::vector<LayersPathInfo> GetEnvVariablePaths(const char *variable_name, LayerType type) {
    std::vector<LayersPathInfo> result;

    const char *SEPARATOR = GetToken(PARSE_ENV_VAR);

    const std::vector<std::string> &paths = UniqueStrings(Split(qgetenv(variable_name).toStdString(), SEPARATOR));
    result.resize(paths.size());
    for (std::size_t i = 0, n = paths.size(); i < n; ++i) {
        result[i].path = paths[i];
        result[i].enabled = true;
        result[i].type = type;
    }

    return result;
}

std::vector<LayersPathInfo> GetImplicitLayerPaths() {
    std::vector<LayersPathInfo> result;

#if VKC_ENV == VKC_ENV_WIN32
    const std::vector<LayersPathInfo> &admin_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers", LAYER_TYPE_IMPLICIT);
    result.insert(result.begin(), admin_registry_paths.begin(), admin_registry_paths.end());

    const std::vector<LayersPathInfo> &user_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers", LAYER_TYPE_IMPLICIT);
    result.insert(result.begin(), user_registry_paths.begin(), user_registry_paths.end());

    // Search for drivers specific layers
    const std::vector<LayersPathInfo> &drivers_registry_paths =
        LoadRegistrySystemLayers("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers");
    result.insert(result.begin(), drivers_registry_paths.begin(), drivers_registry_paths.end());
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
        LayersPathInfo info;
        info.type = LAYER_TYPE_IMPLICIT;
        info.path = LAYERS_PATHS[i];
        result.push_back(info);
    }
#endif

    return result;
}

std::vector<LayersPathInfo> GetExplicitLayerPaths() {
    std::vector<LayersPathInfo> result;

#if VKC_ENV == VKC_ENV_WIN32
    const std::vector<LayersPathInfo> &admin_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers", LAYER_TYPE_EXPLICIT);
    result.insert(result.begin(), admin_registry_paths.begin(), admin_registry_paths.end());

    const std::vector<LayersPathInfo> &user_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers", LAYER_TYPE_EXPLICIT);
    result.insert(result.begin(), user_registry_paths.begin(), user_registry_paths.end());

    // Search for drivers specific layers
    const std::vector<LayersPathInfo> &drivers_registry_paths =
        LoadRegistrySystemLayers("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers");
    result.insert(result.begin(), drivers_registry_paths.begin(), drivers_registry_paths.end());
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
        LayersPathInfo info;
        info.type = LAYER_TYPE_EXPLICIT;
        info.path = LAYERS_PATHS[i];
        result.push_back(info);
    }
#endif

    return result;
}

static LayersPathInfo *FindPathInfo(std::array<std::vector<LayersPathInfo>, LAYERS_PATHS_COUNT> &paths, const std::string &path) {
    for (int paths_type_index = LAYERS_PATHS_FIRST; paths_type_index <= LAYERS_PATHS_LAST; ++paths_type_index) {
        for (std::size_t i = 0, n = paths[paths_type_index].size(); i < n; ++i) {
            if (paths[paths_type_index][i].path == path) {
                return &paths[paths_type_index][i];
            }
        }
    }

    return nullptr;
}

bool LayerManager::Load(const QJsonObject &json_root_object) {
    this->InitSystemPaths();

    // LAYERS_PATHS_GUI
    if (json_root_object.value("layers") != QJsonValue::Undefined) {
        const QJsonObject &json_layers_object = json_root_object.value("layers").toObject();

        if (json_layers_object.value("last_layers_path") != QJsonValue::Undefined) {
            this->last_layers_path = json_layers_object.value("last_layers_path").toString().toStdString();
        }

        if (json_layers_object.value("validate_manifests") != QJsonValue::Undefined) {
            this->validate_manifests = json_layers_object.value("validate_manifests").toBool();
        }

        if (json_layers_object.value("validated") != QJsonValue::Undefined) {
            const QJsonObject &json_layers_validated_object = json_layers_object.value("validated").toObject();
            const QStringList &json_layers_validated_keys = json_layers_validated_object.keys();

            for (int i = 0, n = json_layers_validated_keys.length(); i < n; ++i) {
                const Path &manifest_path = json_layers_validated_keys[i].toStdString();
                const std::string &last_modified =
                    json_layers_validated_object.value(json_layers_validated_keys[i]).toString().toStdString();
                this->layers_validated.insert(std::make_pair(manifest_path, last_modified));
            }
        }

        if (json_layers_object.value("paths") != QJsonValue::Undefined) {
            const QJsonObject &json_paths_object = json_layers_object.value("paths").toObject();
            const QStringList &json_paths_keys = json_paths_object.keys();

            for (int i = 0, n = json_paths_keys.length(); i < n; ++i) {
                LayersPathInfo info;
                info.path = json_paths_keys[i].toStdString();
                info.enabled = json_paths_object.value(json_paths_keys[i].toStdString().c_str()).toBool();
                this->AppendPath(info);
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

    QJsonObject json_paths_object;
    for (int paths_type_index = LAYERS_PATHS_FIRST; paths_type_index <= LAYERS_PATHS_LAST; ++paths_type_index) {
        const std::vector<LayersPathInfo> &path_infos = this->paths[paths_type_index];

        for (std::size_t i = 0, n = path_infos.size(); i < n; ++i) {
            json_paths_object.insert(path_infos[i].path.RelativePath().c_str(), path_infos[i].enabled);
        }
    }

    QJsonObject json_layers_object;
    json_layers_object.insert("validate_manifests", this->validate_manifests);
    json_layers_object.insert("last_layers_path", this->last_layers_path.RelativePath().c_str());
    json_layers_object.insert("validated", json_layers_paths_object);
    json_layers_object.insert("paths", json_paths_object);

    json_root_object.insert("layers", json_layers_object);

    return true;
}

void LayerManager::Reset() {
    this->InitSystemPaths();
    this->LoadAllInstalledLayers();
    this->last_layers_path = Get(Path::HOME);
    this->validate_manifests = false;
}

std::string LayerManager::Log() const {
    std::string log;

    log += "Vulkan Layers Locations\n";

    for (std::size_t group_index = 0, group_count = this->paths.size(); group_index < group_count; ++group_index) {
        const std::vector<LayersPathInfo> &paths_group = this->paths[group_index];
        if (paths_group.empty()) {
            log += format(" %d. %s paths:\n", group_index + 1, ::GetLabel(static_cast<LayersPaths>(group_index)));
            log += format(" - None\n");
        } else {
            log += format(" %d. %s paths:\n", group_index + 1, ::GetLabel(static_cast<LayersPaths>(group_index)));
        }

        for (std::size_t path_index = 0, path_count = paths_group.size(); path_index < path_count; ++path_index) {
            log += format(" - %s (%s)\n", paths_group[path_index].path.AbsolutePath().c_str(),
                          paths_group[path_index].enabled ? "Enabled" : "Disabled");

            const std::vector<const Layer *> layers = this->GatherLayers(paths_group[path_index]);

            for (std::size_t i = 0, n = layers.size(); i < n; ++i) {
                if (layers[i]->type == LAYER_TYPE_IMPLICIT) {
                    log += format("   * %s - %s (Auto: %s)", layers[i]->key.c_str(), layers[i]->api_version.str().c_str(),
                                  GetLabel(layers[i]->GetActualControl()));
                } else {
                    log += format("   * %s - %s", layers[i]->key.c_str(), layers[i]->api_version.str().c_str());
                }

                if (layers[i]->status != STATUS_STABLE) {
                    log += format(" (%s)", GetToken(layers[i]->status));
                }
                log += "\n";

                if (layers[i]->type == LAYER_TYPE_IMPLICIT) {
                    if (!layers[i]->disable_env.empty()) {
                        const std::string &value = qEnvironmentVariableIsSet(layers[i]->disable_env.c_str()) ? "set" : "not set";
                        log += format("     '%s' is %s\n", layers[i]->disable_env.c_str(), value.c_str());
                    }
                    if (!layers[i]->enable_env.empty()) {
                        if (qEnvironmentVariableIsSet(layers[i]->enable_env.c_str())) {
                            const std::string &value = qgetenv(layers[i]->enable_env.c_str()).toStdString();
                            log += format("     '%s' is set to '%s'\n", layers[i]->enable_env.c_str(), value.c_str());
                        } else {
                            log += format("     '%s' is not set to '%s'\n", layers[i]->enable_env.c_str(),
                                          layers[i]->enable_value.c_str());
                        }
                    }
                }
            }
        }
    }

    log += "\n";

    return log;
}

void LayerManager::InitSystemPaths() {
    this->selected_layers.clear();
    this->layers_validated.clear();

    this->paths[LAYERS_PATHS_IMPLICIT_SYSTEM] = GetImplicitLayerPaths();

    // LAYERS_PATHS_IMPLICIT_ENV_SET: VK_IMPLICIT_LAYER_PATH env variables
    this->paths[LAYERS_PATHS_IMPLICIT_ENV_SET] = GetEnvVariablePaths("VK_IMPLICIT_LAYER_PATH", LAYER_TYPE_IMPLICIT);

    // LAYERS_PATHS_IMPLICIT_ENV_ADD: VK_ADD_IMPLICIT_LAYER_PATH env variables
    this->paths[LAYERS_PATHS_IMPLICIT_ENV_ADD] = GetEnvVariablePaths("VK_ADD_IMPLICIT_LAYER_PATH", LAYER_TYPE_IMPLICIT);

    // LAYERS_PATHS_EXPLICIT_SYSTEM
    this->paths[LAYERS_PATHS_EXPLICIT_SYSTEM] = GetExplicitLayerPaths();

    // LAYERS_PATHS_EXPLICIT_ENV_SET: VK_LAYER_PATH env variables
    this->paths[LAYERS_PATHS_EXPLICIT_ENV_SET] = GetEnvVariablePaths("VK_LAYER_PATH", LAYER_TYPE_EXPLICIT);

    // LAYERS_PATHS_EXPLICIT_ENV_ADD: VK_ADD_LAYER_PATH env variables
    this->paths[LAYERS_PATHS_EXPLICIT_ENV_ADD] = GetEnvVariablePaths("VK_ADD_LAYER_PATH", LAYER_TYPE_EXPLICIT);

    // LAYERS_PATHS_SDK
    this->paths[LAYERS_PATHS_SDK].clear();
    {
        LayersPathInfo info;
        info.path = ::Get(Path::SDK_BIN);
        info.enabled = true;
        this->paths[LAYERS_PATHS_SDK].push_back(info);
    }
}

void LayerManager::Clear() { this->selected_layers.clear(); }

bool LayerManager::Empty() const { return this->selected_layers.empty(); }

std::size_t LayerManager::Size() const { return this->selected_layers.size(); }

std::vector<Path> LayerManager::GatherManifests(const std::string &layer_name) const {
    std::vector<Path> result;

    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (!this->selected_layers[i].enabled) {
            continue;
        }

        if (this->selected_layers[i].key == layer_name) {
            result.push_back(this->selected_layers[i].manifest_path);
        }
    }

    std::sort(result.rbegin(), result.rend());

    return result;
}

std::vector<Version> LayerManager::GatherVersions(const std::string &layer_name) const {
    std::vector<Version> result;

    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (!this->selected_layers[i].enabled) {
            continue;
        }

        if (this->selected_layers[i].key == layer_name) {
            result.push_back(this->selected_layers[i].api_version);
        }
    }

    std::sort(result.rbegin(), result.rend());

    return result;
}

const Layer *LayerManager::Find(const std::string &layer_name, const Version &layer_version) const {
    // Version::VERSION_NULL refer to latest version
    if (layer_version == Version::LATEST) {
        const std::vector<Version> &version = this->GatherVersions(layer_name);
        if (version.empty()) {
            return nullptr;
        }

        Version latest = Version::NONE;
        for (std::size_t i = 0, n = version.size(); i < n; ++i) {
            if (latest == Version::NONE) {
                latest = version[i];
            } else if (version[i] > latest) {
                latest = version[i];
            }
        }
        assert(latest != Version::NONE);

        return this->Find(layer_name, latest);
    } else {
        const Layer *newest = this->FindLastModified(layer_name, layer_version);

        if (newest != nullptr) {
            return newest;
        } else {
            // Version not found, search for the latest available
            return this->Find(layer_name, Version::LATEST);
        }
    }

    return nullptr;
}

const Layer *LayerManager::FindLastModified(const std::string &layer_name, const Version &version) const {
    const Layer *result = nullptr;

    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (this->selected_layers[i].enabled == false) {
            continue;
        }
        if (this->selected_layers[i].key != layer_name) {
            continue;
        }
        if (this->selected_layers[i].api_version != version) {
            continue;
        }
        if (result != nullptr) {
            if (result->validated_last_modified > this->selected_layers[i].validated_last_modified) {
                continue;
            }
        }

        result = &this->selected_layers[i];
    }

    return result;
}

const Layer *LayerManager::FindFromManifest(const Path &manifest_path, bool find_disabled_layers) const {
    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (!find_disabled_layers && this->selected_layers[i].enabled == false) {
            continue;
        }

        if (this->selected_layers[i].manifest_path == manifest_path) {
            return &this->selected_layers[i];
        }
    }
    return nullptr;
}

Layer *LayerManager::FindFromManifest(const Path &manifest_path, bool find_disabled_layers) {
    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (!find_disabled_layers && this->selected_layers[i].enabled == false) {
            continue;
        }

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
        const std::vector<LayersPathInfo> &paths_group = this->paths[group_index];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            this->LoadLayersFromPath(paths_group[i].path, paths_group[i].type);
            this->UpdatePathEnabled(paths_group[i]);
        }
    }
}

void LayerManager::LoadLayersFromPath(const Path &layers_path, LayerType type) {
    const std::vector<Path> &layers_paths = CollectFilePaths(layers_path);

    for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
        this->LoadLayer(layers_paths[i], type);
    }
}

LayerLoadStatus LayerManager::LoadLayer(const Path &layer_path, LayerType type) {
    const std::string &last_modified = layer_path.LastModified();

    Layer *already_loaded_layer = this->FindFromManifest(layer_path);
    if (already_loaded_layer != nullptr) {
        // Already loaded
        auto it = this->layers_validated.find(layer_path);
        if (it != layers_validated.end()) {
            if (last_modified == it->second) {
                return LAYER_LOAD_UNMODIFIED;
            }
        }

        // Modified to reload
        LayerLoadStatus status = already_loaded_layer->Load(layer_path, type, this->validate_manifests, this->layers_validated);
        if (status == LAYER_LOAD_ADDED) {
            it->second = already_loaded_layer->validated_last_modified;
            return LAYER_LOAD_RELOADED;
        } else {
            return status;
        }
    } else {
        Layer layer;
        LayerLoadStatus status = layer.Load(layer_path, type, this->validate_manifests, this->layers_validated);
        if (status == LAYER_LOAD_ADDED) {
            this->selected_layers.push_back(layer);
            this->layers_validated.insert(std::make_pair(layer.manifest_path, layer.validated_last_modified));
        }

        return status;
    }
}

void LayerManager::AppendPath(const LayersPathInfo &info) {
    LayersPathInfo *existing_info = FindPathInfo(this->paths, info.path.RelativePath());
    if (existing_info != nullptr) {
        existing_info->enabled = info.enabled;
    } else {
        this->paths[LAYERS_PATHS_GUI].push_back(info);
    }

    std::sort(this->paths[LAYERS_PATHS_GUI].begin(), this->paths[LAYERS_PATHS_GUI].end());
}

void LayerManager::RemovePath(const LayersPathInfo &path_info) {
    const std::vector<Path> &layers_paths = CollectFilePaths(path_info.path);

    for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
        Layer *layer = this->FindFromManifest(layers_paths[i]);
        if (layer == nullptr) {
            continue;
        }

        layer->enabled = false;
    }

    for (int paths_type_index = LAYERS_PATHS_FIRST; paths_type_index <= LAYERS_PATHS_LAST; ++paths_type_index) {
        std::vector<LayersPathInfo> new_path_list;
        for (std::size_t i = 0, n = this->paths[paths_type_index].size(); i < n; ++i) {
            if (path_info.path == this->paths[paths_type_index][i].path) {
                continue;
            }

            new_path_list.push_back(this->paths[paths_type_index][i]);
        }
        this->paths[paths_type_index] = new_path_list;
    }
}

void LayerManager::UpdatePathEnabled(const LayersPathInfo &path_info) {
    for (int paths_type_index = LAYERS_PATHS_FIRST; paths_type_index <= LAYERS_PATHS_LAST; ++paths_type_index) {
        for (std::size_t i = 0, n = this->paths[paths_type_index].size(); i < n; ++i) {
            if (path_info.path == this->paths[paths_type_index][i].path) {
                this->paths[paths_type_index][i].enabled = path_info.enabled;
                break;
            }
        }
    }

    const std::vector<Path> &layers_paths = CollectFilePaths(path_info.path);

    for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
        Layer *layer = this->FindFromManifest(layers_paths[i], true);
        if (layer == nullptr) {
            continue;
        }

        layer->enabled = path_info.enabled;
    }
}

std::vector<Path> LayerManager::CollectManifestPaths() const {
    std::vector<Path> results;

    for (int paths_type_index = LAYERS_PATHS_FIRST; paths_type_index <= LAYERS_PATHS_LAST; ++paths_type_index) {
        for (std::size_t i = 0, n = this->paths[paths_type_index].size(); i < n; ++i) {
            const std::vector<Path> &layers_paths = ::CollectFilePaths(this->paths[paths_type_index][i].path);
            results.insert(results.end(), layers_paths.begin(), layers_paths.end());
        }
    }

    return results;
}

std::vector<std::string> LayerManager::GatherLayerNames() const {
    std::vector<std::string> result;

    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        if (this->selected_layers[i].enabled == false) {
            continue;
        }

        if (std::find(result.begin(), result.end(), this->selected_layers[i].key) != result.end()) {
            continue;
        }

        result.push_back(this->selected_layers[i].key);
    }

    return result;
}

std::vector<const Layer *> LayerManager::GatherLayers(const LayersPathInfo &path_info) const {
    std::vector<const Layer *> result;

    for (std::size_t i = 0, n = this->selected_layers.size(); i < n; ++i) {
        const std::string &layer_path = path_info.path.AbsolutePath();
        const std::string &current_layer_path = this->selected_layers[i].manifest_path.AbsolutePath();
        if (current_layer_path.find(layer_path) == std::string::npos) {
            continue;
        }

        result.push_back(&this->selected_layers[i]);
    }

    return result;
}
