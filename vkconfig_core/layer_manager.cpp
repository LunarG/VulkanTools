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

#include "layer_manager.h"
#include "util.h"
#include "type_platform.h"
#include "registry.h"

#include <QJsonArray>

static std::vector<Path> GetEnvVariablePaths(const char *variable_name, LayerType type) {
    std::vector<Path> result;

    const char *SEPARATOR = GetToken(PARSE_ENV_VAR);

    const std::vector<std::string> &paths = UniqueStrings(Split(qgetenv(variable_name).toStdString(), SEPARATOR));
    result.resize(paths.size());
    for (std::size_t i = 0, n = paths.size(); i < n; ++i) {
        result[i] = paths[i];
    }

    return result;
}

static std::vector<Path> GetImplicitLayerPaths() {
    std::vector<Path> result;

#if VKC_ENV == VKC_ENV_WIN32
    const std::vector<Path> &admin_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers", LAYER_TYPE_IMPLICIT);
    result.insert(result.begin(), admin_registry_paths.begin(), admin_registry_paths.end());

    const std::vector<Path> &user_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers", LAYER_TYPE_IMPLICIT);
    result.insert(result.begin(), user_registry_paths.begin(), user_registry_paths.end());

    // Search for drivers specific layers
    const std::vector<Path> &drivers_registry_paths =
        LoadRegistrySystemLayers("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers");
    result.insert(result.begin(), drivers_registry_paths.begin(), drivers_registry_paths.end());
#else
    std::vector<std::string> paths;
    if (VKC_PLATFORM == PLATFORM_MACOS) {
        static const char *LAYERS_PATHS[] = {
            "/usr/local/share/vulkan/implicit_layer.d",
            ".local/share/vulkan/implicit_layer.d",
        };

        for (std::size_t i = 0, n = std::size(LAYERS_PATHS); i < n; ++i) {
            paths.push_back(LAYERS_PATHS[i]);
        }
    } else {
        static const char *LAYERS_PATHS[] = {
            "/usr/local/etc/vulkan/implicit_layer.d",
            "/usr/local/share/vulkan/implicit_layer.d",
            "/etc/vulkan/implicit_layer.d",
            "/usr/share/vulkan/implicit_layer.d",
            ".local/share/vulkan/implicit_layer.d",
#ifdef _DEBUG
#ifdef INSTALL_FULL_DATAROOTDIR
            INSTALL_FULL_DATAROOTDIR "/vulkan/implicit_layer.d",
#endif
#ifdef INSTALL_FULL_SYSCONFDIR
            INSTALL_FULL_SYSCONFDIR "/vulkan/implicit_layer.d",
#endif
#endif  //_DEBUG
        };

        for (std::size_t i = 0, n = std::size(LAYERS_PATHS); i < n; ++i) {
            paths.push_back(LAYERS_PATHS[i]);
        }
    }
#endif

    return result;
}

std::vector<Path> GetExplicitLayerPaths() {
    std::vector<Path> result;

#if VKC_ENV == VKC_ENV_WIN32
    const std::vector<Path> &admin_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers", LAYER_TYPE_EXPLICIT);
    result.insert(result.begin(), admin_registry_paths.begin(), admin_registry_paths.end());

    const std::vector<Path> &user_registry_paths =
        LoadRegistrySoftwareLayers("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers", LAYER_TYPE_EXPLICIT);
    result.insert(result.begin(), user_registry_paths.begin(), user_registry_paths.end());

    // Search for drivers specific layers
    const std::vector<Path> &drivers_registry_paths =
        LoadRegistrySystemLayers("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers");
    result.insert(result.begin(), drivers_registry_paths.begin(), drivers_registry_paths.end());
#else
    std::vector<std::string> paths;
    if (VKC_PLATFORM == PLATFORM_MACOS) {
        static const char *LAYERS_PATHS[] = {
            "/usr/local/share/vulkan/explicit_layer.d",
            ".local/share/vulkan/explicit_layer.d",
        };

        for (std::size_t i = 0, n = std::size(LAYERS_PATHS); i < n; ++i) {
            paths.push_back(LAYERS_PATHS[i]);
        }
    } else {
        static const char *LAYERS_PATHS[] = {
            "/usr/local/etc/vulkan/explicit_layer.d",
            "/usr/local/share/vulkan/explicit_layer.d",
            "/etc/vulkan/explicit_layer.d",
            "/usr/share/vulkan/explicit_layer.d",
            ".local/share/vulkan/explicit_layer.d",
#ifdef _DEBUG
#ifdef INSTALL_FULL_DATAROOTDIR
            INSTALL_FULL_DATAROOTDIR "/vulkan/explicit_layer.d",
#endif
#ifdef INSTALL_FULL_SYSCONFDIR
            INSTALL_FULL_SYSCONFDIR "/vulkan/explicit_layer.d",
#endif
#endif  //_DEBUG
        };

        for (std::size_t i = 0, n = std::size(LAYERS_PATHS); i < n; ++i) {
            paths.push_back(LAYERS_PATHS[i]);
        }
    }
#endif

    return result;
}

LayerManager::LayerManager() {}

bool LayerManager::Load(const QJsonObject &json_root_object, ConfiguratorMode configurator_mode) {
    this->available_layers.clear();
    this->layers_found.clear();

    // LAYERS_PATHS_GUI
    if (json_root_object.value("layers") != QJsonValue::Undefined) {
        const QJsonObject &json_layers_object = json_root_object.value("layers").toObject();

        if (json_layers_object.value("last_layers_dir") != QJsonValue::Undefined) {
            this->last_layers_dir = json_layers_object.value("last_layers_dir").toString().toStdString();
        }

        if (json_layers_object.value("validate_manifests") != QJsonValue::Undefined) {
            this->validate_manifests = json_layers_object.value("validate_manifests").toBool();
        }

        if (json_layers_object.value("found") != QJsonValue::Undefined) {
            const QJsonObject &json_layers_found_object = json_layers_object.value("found").toObject();
            const QStringList &json_layers_found_keys = json_layers_found_object.keys();

            for (int i = 0, n = json_layers_found_keys.length(); i < n; ++i) {
                const QJsonObject &json_status_object = json_layers_found_object.value(json_layers_found_keys[i]).toObject();

                LayerDescriptor descriptor;
                if (json_status_object.value("type") != QJsonValue::Undefined) {
                    descriptor.type = ::GetLayerType(json_status_object.value("type").toString().toStdString().c_str());
                }
                descriptor.last_modified = json_status_object.value("last_modified").toString().toStdString();
                descriptor.validated = json_status_object.value("validated").toBool();
                if (json_status_object.value("enabled") != QJsonValue::Undefined) {
                    descriptor.enabled = json_status_object.value("enabled").toBool();
                }

                const Path &manifest_path = json_layers_found_keys[i].toStdString();

                this->layers_found.insert(std::make_pair(manifest_path, descriptor));
            }
        }

        if (json_layers_object.value("removed") != QJsonValue::Undefined) {
            const QJsonArray &array = json_layers_object.value("removed").toArray();
            for (int i = 0, n = array.size(); i < n; ++i) {
                this->layers_removed.insert(array[i].toString().toStdString());
            }
        }
    }

    this->LoadAllInstalledLayers(configurator_mode);

    return true;
}

bool LayerManager::Save(QJsonObject &json_root_object) const {
    QJsonObject json_layers_found_object;
    for (auto it = this->layers_found.begin(); it != this->layers_found.end(); ++it) {
        QJsonObject object;
        object.insert("last_modified", it->second.last_modified.c_str());
        object.insert("type", ::GetToken(it->second.type));
        object.insert("validated", it->second.validated);
        object.insert("enabled", it->second.enabled);
        json_layers_found_object.insert(it->first.AbsolutePath().c_str(), object);
    }

    QJsonArray json_layers_removed_array;
    for (auto it = this->layers_removed.begin(); it != this->layers_removed.end(); ++it) {
        json_layers_removed_array.append(it->AbsolutePath().c_str());
    }

    QJsonObject json_layers_object;
    json_layers_object.insert("validate_manifests", this->validate_manifests);
    json_layers_object.insert("last_layers_dir", this->last_layers_dir.RelativePath().c_str());
    json_layers_object.insert("found", json_layers_found_object);
    json_layers_object.insert("removed", json_layers_removed_array);
    json_root_object.insert("layers", json_layers_object);

    return true;
}

std::string LayerManager::Log() const {
    std::string log;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        const Layer *layer = &this->available_layers[i];

        auto it = this->layers_found.find(layer->manifest_path);
        if (it == this->layers_found.end()) {
            assert(0);
            continue;
        }

        const LayerDescriptor &descriptor = it->second;

        if (descriptor.type == LAYER_TYPE_IMPLICIT) {
            log += format("   * %s - %s (Auto: %s)", layer->key.c_str(), layer->api_version.str().c_str(),
                          GetLabel(layer->GetActualControl()));
        } else {
            log += format("   * %s - %s", layer->key.c_str(), layer->api_version.str().c_str());
        }

        if (layer->status != STATUS_STABLE) {
            log += format(" (%s)", GetToken(layer->status));
        }
        log += "\n";

        log += format("     %s\n", layer->manifest_path.AbsolutePath().c_str());
        log += "\n";

        if (descriptor.type == LAYER_TYPE_IMPLICIT) {
            if (!layer->disable_env.empty()) {
                const std::string &value = qEnvironmentVariableIsSet(layer->disable_env.c_str()) ? "set" : "not set";
                log += format("     '%s' is %s\n", layer->disable_env.c_str(), value.c_str());
            }
            if (!layer->enable_env.empty()) {
                if (qEnvironmentVariableIsSet(layer->enable_env.c_str())) {
                    const std::string &value = qgetenv(layer->enable_env.c_str()).toStdString();
                    log += format("     '%s' is set to '%s'\n", layer->enable_env.c_str(), value.c_str());
                } else {
                    log += format("     '%s' is not set to '%s'\n", layer->enable_env.c_str(), layer->enable_value.c_str());
                }
            }
        }
    }

    log += "\n";

    return log;
}

void LayerManager::Clear() { this->available_layers.clear(); }

bool LayerManager::Empty() const { return this->available_layers.empty(); }

std::size_t LayerManager::Size() const { return this->available_layers.size(); }

std::vector<Path> LayerManager::GatherManifests(const std::string &layer_key) const {
    std::vector<Path> result;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        if (this->available_layers[i].key != layer_key) {
            continue;
        }

        if (!this->IsEnabled(this->available_layers[i].manifest_path)) {
            continue;
        }

        result.push_back(this->available_layers[i].manifest_path);
    }

    std::sort(result.rbegin(), result.rend());

    return result;
}

std::vector<Version> LayerManager::GatherVersions(const std::string &layer_key) const {
    std::vector<Version> result;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        if (this->available_layers[i].key != layer_key) {
            continue;
        }

        if (!this->IsEnabled(this->available_layers[i].manifest_path)) {
            continue;
        }

        result.push_back(this->available_layers[i].api_version);
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

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        if (this->available_layers[i].key != layer_name) {
            continue;
        }
        if (this->available_layers[i].api_version != version) {
            continue;
        }

        if (!this->IsEnabled(this->available_layers[i].manifest_path)) {
            continue;
        }

        if (result != nullptr) {
            if (result->last_modified > this->available_layers[i].last_modified) {
                continue;
            }
        }

        result = &this->available_layers[i];
    }

    return result;
}

const Layer *LayerManager::FindFromManifest(const Path &manifest_path, bool find_disabled_layers) const {
    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        if (!find_disabled_layers) {
            if (!this->IsEnabled(this->available_layers[i].manifest_path)) {
                continue;
            }
        }

        if (this->available_layers[i].manifest_path == manifest_path) {
            return &this->available_layers[i];
        }
    }
    return nullptr;
}

Layer *LayerManager::FindFromManifest(const Path &manifest_path, bool find_disabled_layers) {
    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        if (!find_disabled_layers) {
            if (!this->IsEnabled(this->available_layers[i].manifest_path)) {
                continue;
            }
        }

        if (this->available_layers[i].manifest_path == manifest_path) {
            return &this->available_layers[i];
        }
    }
    return nullptr;
}

// Find all installed layers on the system.
void LayerManager::LoadAllInstalledLayers(ConfiguratorMode configurator_mode) {
    std::array<std::vector<Path>, LAYERS_PATHS_COUNT> paths;

    // Search new layers
    paths[LAYERS_PATHS_IMPLICIT_SYSTEM] = GetImplicitLayerPaths();

    // LAYERS_PATHS_IMPLICIT_ENV_SET: VK_IMPLICIT_LAYER_PATH env variables
    paths[LAYERS_PATHS_IMPLICIT_ENV_SET] = GetEnvVariablePaths("VK_IMPLICIT_LAYER_PATH", LAYER_TYPE_IMPLICIT);

    // LAYERS_PATHS_IMPLICIT_ENV_ADD: VK_ADD_IMPLICIT_LAYER_PATH env variables
    paths[LAYERS_PATHS_IMPLICIT_ENV_ADD] = GetEnvVariablePaths("VK_ADD_IMPLICIT_LAYER_PATH", LAYER_TYPE_IMPLICIT);

    // LAYERS_PATHS_EXPLICIT_SYSTEM
    paths[LAYERS_PATHS_EXPLICIT_SYSTEM] = GetExplicitLayerPaths();

    // LAYERS_PATHS_EXPLICIT_ENV_SET: VK_LAYER_PATH env variables
    paths[LAYERS_PATHS_EXPLICIT_ENV_SET] = GetEnvVariablePaths("VK_LAYER_PATH", LAYER_TYPE_EXPLICIT);

    // LAYERS_PATHS_EXPLICIT_ENV_ADD: VK_ADD_LAYER_PATH env variables
    paths[LAYERS_PATHS_EXPLICIT_ENV_ADD] = GetEnvVariablePaths("VK_ADD_LAYER_PATH", LAYER_TYPE_EXPLICIT);

    // LAYERS_PATHS_SDK
    paths[LAYERS_PATHS_SDK].push_back(Path(Path::SDK_EXPLICIT_LAYERS));

    for (std::size_t group_index = 0, group_count = paths.size(); group_index < group_count; ++group_index) {
        const LayersPaths layers_path = static_cast<LayersPaths>(group_index);

        const std::vector<Path> &paths_group = paths[group_index];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            const std::vector<Path> &layers_paths = ::CollectFilePaths(paths_group[i]);

            for (std::size_t p = 0, o = layers_paths.size(); p < o; ++p) {
                this->AppendPath(layers_paths[p], ::GetLayerType(layers_path));
            }
        }
    }

    for (auto it = this->layers_found.begin(); it != this->layers_found.end(); ++it) {
        this->LoadLayer(it->first, it->second.type, configurator_mode);
    }
}

LayerLoadStatus LayerManager::LoadLayer(const Path &layer_path, LayerType type, ConfiguratorMode configurator_mode) {
    const std::string &last_modified = layer_path.LastModified();

    Layer *already_loaded_layer = this->FindFromManifest(layer_path, true);
    if (already_loaded_layer != nullptr) {
        // Already loaded
        auto it = this->layers_found.find(layer_path);
        if (it != layers_found.end()) {
            if (last_modified == it->second.last_modified) {
                return LAYER_LOAD_UNMODIFIED;
            }
        }

        // Modified to reload
        LayerLoadStatus status = already_loaded_layer->Load(layer_path, type, this->validate_manifests, configurator_mode);
        if (status == LAYER_LOAD_ADDED) {
            it->second.last_modified = already_loaded_layer->last_modified;
            return LAYER_LOAD_RELOADED;
        } else {
            // it->second.enabled = ::IsEnabled(status);
            return status;
        }
    } else {
        Layer layer;
        LayerLoadStatus status = layer.Load(layer_path, type, this->validate_manifests, configurator_mode);
        if (status == LAYER_LOAD_ADDED) {
            this->available_layers.push_back(layer);
        }

        auto it = this->layers_found.find(layer_path);
        // assert(it != this->layers_found.end());
        if (it != layers_found.end()) {
            // it->second.enabled = ::IsEnabled(status);
            // it->second.validated = this->validate_manifests && it->second.enabled;
            it->second.last_modified = layer.last_modified;
        } else {
            LayerDescriptor descriptor;
            descriptor.type = layer.type;
            descriptor.last_modified = layer.last_modified;
            descriptor.validated = this->validate_manifests;
            descriptor.enabled = true;
            descriptor.added = true;
            layers_found.insert(std::make_pair(layer.manifest_path, descriptor));
        }

        return status;
    }
}

void LayerManager::AppendPath(const Path &path, LayerType type, bool added) {
    if (added) {
        auto it_removed = this->layers_removed.find(path);
        if (it_removed != this->layers_removed.end()) {
            this->layers_removed.erase(it_removed);
        }
    }

    if (this->layers_removed.find(path) != this->layers_removed.end()) {
        return;
    }

    auto it = this->layers_found.find(path);
    if (it == this->layers_found.end()) {
        LayerDescriptor descriptor;
        descriptor.type = type;
        descriptor.added = added;
        descriptor.last_modified = path.LastModified();
        this->layers_found.insert(std::make_pair(path, descriptor));
    }
}

void LayerManager::RemovePath(const Path &path) {
    auto it_found = this->layers_found.find(path);
    if (it_found != this->layers_found.end()) {
        this->layers_found.erase(it_found);
    }

    this->layers_removed.insert(path);
}

std::vector<std::string> LayerManager::GatherLayerNames() const {
    std::vector<std::string> result;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        auto it = this->layers_found.find(this->available_layers[i].manifest_path);
        if (it != this->layers_found.end()) {
            if (!it->second.enabled) {
                continue;
            }
        }

        if (std::find(result.begin(), result.end(), this->available_layers[i].key) != result.end()) {
            continue;
        }

        result.push_back(this->available_layers[i].key);
    }

    return result;
}

bool LayerManager::IsEnabled(const Path &manifest_path) const {
    auto it = this->layers_found.find(manifest_path);
    if (it != this->layers_found.end()) {
        return it->second.enabled;
    }
    return false;
}

void LayerManager::Enable(const Path &manifest_path, bool enabled) {
    auto it = this->layers_found.find(manifest_path);
    if (it != this->layers_found.end()) {
        it->second.enabled = enabled;
    }
}

bool operator<(const LayerDisplay &a, const LayerDisplay &b) {
    if (a.key == b.key) {
        return a.api_version < b.api_version;
    } else {
        return a.key < b.key;
    }
}

std::set<LayerDisplay> LayerManager::BuildLayerDisplayList() const {
    std::set<LayerDisplay> result;

    for (auto it = this->layers_found.begin(), end = this->layers_found.end(); it != end; ++it) {
        const Layer *layer = this->FindFromManifest(it->first, true);
        if (layer == nullptr) {
            continue;
        }

        LayerDisplay layer_display;
        layer_display.key = layer->key;
        layer_display.manifest_path = it->first;
        layer_display.api_version = layer->api_version;
        layer_display.descriptor = it->second;

        result.insert(layer_display);
    }

    return result;
}
