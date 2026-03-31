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
#include "json_validator.h"

#include <QJsonArray>
#include <QDesktopServices>
#include <QFileDialog>

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

                LayerDisplay layer;
                layer.id.manifest_path = json_layers_found_keys[i].toStdString();
                layer.descriptor.enabled = !json_status_object.value("disabled").toBool();
                layer.descriptor.validated = json_status_object.value("validated").toBool();

                std::vector<LayerDisplay> layers;
                layers.push_back(layer);
                this->AppendInit(layer.id.manifest_path, layers);
            }
        }
    }

    this->LoadAllInstalledLayers(configurator_mode);

    return true;
}

bool LayerManager::Save(QJsonObject &json_root_object) const {
    QJsonObject json_layers_object;
    json_layers_object.insert("validate_manifests", this->validate_manifests);
    json_layers_object.insert("last_layers_dir", this->last_layers_dir.RelativePath().c_str());
    // json_layers_object.insert("found", json_layers_found_object);
    // json_layers_object.insert("removed", json_layers_removed_array);
    // json_root_object.insert("layers", json_layers_object);

    return true;
}

std::string LayerManager::Log() const {
    std::string log;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        const Layer *layer = &this->available_layers[i];

        if (layer->type == LAYER_TYPE_IMPLICIT) {
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

        if (layer->type == LAYER_TYPE_IMPLICIT) {
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
        if (!this->available_layers[i].descriptor.enabled) {
            continue;
        }

        if (this->available_layers[i].key != layer_key) {
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
        if (!this->available_layers[i].descriptor.enabled) {
            continue;
        }

        if (this->available_layers[i].key != layer_key) {
            continue;
        }

        result.push_back(this->available_layers[i].api_version);
    }

    std::sort(result.rbegin(), result.rend());

    return result;
}

const Layer *LayerManager::Find(LayerId id, bool enable_only) const {
    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        const Layer &layer = this->available_layers[i];

        if (!layer.descriptor.enabled && enable_only) {
            continue;
        }

        if (layer.manifest_path != id.manifest_path) {
            continue;
        }

        if (layer.key != id.key) {
            continue;
        }

        if (layer.api_version != Version::LATEST) {
            if (layer.api_version != id.api_version) {
                continue;
            }
        }

        return &this->available_layers[i];
    }

    return nullptr;
}

Layer *LayerManager::Find(LayerId id, bool enable_only) {
    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        Layer &layer = this->available_layers[i];

        if (!layer.descriptor.enabled && enable_only) {
            continue;
        }

        if (layer.manifest_path != id.manifest_path) {
            continue;
        }

        if (layer.key != id.key) {
            continue;
        }

        if (layer.api_version != Version::LATEST) {
            if (layer.api_version != id.api_version) {
                continue;
            }
        }

        return &this->available_layers[i];
    }

    return nullptr;
}

const Layer *LayerManager::Find(const std::string &layer_name, const Version &layer_version) const {
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
        if (!this->available_layers[i].descriptor.enabled) {
            continue;
        }
        if (this->available_layers[i].key != layer_name) {
            continue;
        }
        if (this->available_layers[i].api_version != version) {
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
            if (!this->available_layers[i].descriptor.enabled) {
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
            if (!this->available_layers[i].descriptor.enabled) {
                continue;
            }
        }

        if (this->available_layers[i].manifest_path == manifest_path) {
            return &this->available_layers[i];
        }
    }
    return nullptr;
}

void LayerManager::ApplyLayerDescriptor() {
    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        Layer &layer = this->available_layers[i];

        auto it = this->layer_init.find(layer.manifest_path);
        if (it == this->layer_init.end()) {
            continue;
        }

        for (std::size_t j = 0, o = it->second.size(); j < o; ++j) {
            const LayerDisplay &init = it->second[j];

            if (!init.id.key.empty()) {
                if (init.id.key != layer.key) {
                    continue;
                }
            }

            layer.descriptor = init.descriptor;
        }
    }
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

    // LAYERS_PATHS_GUI
    std::vector<Path> added_paths = this->BuildLayerPaths();
    paths[LAYERS_PATHS_GUI].insert(paths[LAYERS_PATHS_GUI].begin(), added_paths.begin(), added_paths.end());

    for (std::size_t group_index = 0, group_count = paths.size(); group_index < group_count; ++group_index) {
        const LayersPaths layers_path = static_cast<LayersPaths>(group_index);

        const std::vector<Path> &paths_group = paths[group_index];
        for (std::size_t i = 0, n = paths_group.size(); i < n; ++i) {
            const std::vector<Path> &layers_paths = ::CollectLayersPaths(paths_group[i]);

            for (std::size_t p = 0, o = layers_paths.size(); p < o; ++p) {
                this->LoadLayers(layers_paths[p], ::GetLayerType(layers_path), configurator_mode);
            }
        }
    }

    this->ApplyLayerDescriptor();
}

LayerLoadStatus LayerManager::LoadLayers(const Path &layer_path, LayerType type, ConfiguratorMode configurator_mode) {
    const std::string &last_modified = layer_path.LastModified();
    LayerDescriptor descriptor;

    QFile file(layer_path.AbsolutePath().c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        assert(0);
        return LAYER_LOAD_INVALID;
    }

    QString json_text = file.readAll();
    file.close();

    if (this->validate_manifests) {
        JsonValidator validator;
        descriptor.validated = validator.Check(json_text);

        if (descriptor.validated) {
            switch (configurator_mode) {
                default: {
                } break;
                case CONFIGURATOR_MODE_GUI: {
                    QMessageBox alert;
                    alert.setWindowTitle("Failed to load a layer manifest...");
                    alert.setText(format("%s is not a valid layer file", layer_path.AbsolutePath().c_str()).c_str());
                    alert.setInformativeText("Do you want to save the validation log?");
                    alert.setIcon(QMessageBox::Critical);
                    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    alert.setDefaultButton(QMessageBox::Yes);
                    int result = alert.exec();
                    if (result == QMessageBox::Yes) {
                        const QString &selected_path = QFileDialog::getSaveFileName(
                            nullptr, format("Export %s validation log", layer_path.AbsolutePath().c_str()).c_str(),
                            (AbsolutePath(Path::HOME) + "/" + layer_path.Basename() + "_log.txt").c_str(), "Log(*.txt)");
                        QFile log_file(selected_path);
                        const bool result = log_file.open(QIODevice::WriteOnly | QIODevice::Text);
                        if (result) {
                            QDesktopServices::openUrl(QUrl::fromLocalFile(selected_path));
                            log_file.write(validator.message.toStdString().c_str());
                            log_file.close();
                        } else {
                            QMessageBox alert;
                            alert.setWindowTitle("Failed to save layer manifest log...");
                            alert.setText(format("Couldn't not open %s file...", selected_path.toStdString().c_str()).c_str());
                            alert.setIcon(QMessageBox::Critical);
                            alert.exec();
                        }
                    }
                } break;
                case CONFIGURATOR_MODE_CMD: {
                    fprintf(stderr, "vkconfig: [ERROR] Couldn't validate layer file: %s\n", layer_path.AbsolutePath().c_str());
                    fprintf(stderr, "\n%s\n)", validator.message.toStdString().c_str());
                } break;
            }
        }

        return LAYER_LOAD_INVALID;
    }

    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonParseError json_parse_error;
    const QJsonDocument &json_document = QJsonDocument::fromJson(json_text.toUtf8(), &json_parse_error);
    if (json_parse_error.error != QJsonParseError::NoError) {
        return LAYER_LOAD_INVALID;
    }

    // Make sure it's not empty
    if (json_document.isNull() || json_document.isEmpty()) {
        return LAYER_LOAD_INVALID;
    }

    Version file_format_version;
    const QJsonObject &json_root_object = json_document.object();
    if (json_root_object.value("file_format_version") != QJsonValue::Undefined) {
        file_format_version = Version(json_root_object.value("file_format_version").toString().toStdString().c_str());
    }

    LayerLoadStatus status = LAYER_LOAD_ADDED;

    if (json_root_object.value("layers") != QJsonValue::Undefined) {
        const QJsonArray &json_layers_array = json_root_object.value("layers").toArray();
        for (int i = 0, n = json_layers_array.size(); i < n; ++i) {
            const QJsonObject &json_layer_object = json_layers_array[i].toObject();
            status = this->LoadLayer(json_layer_object, layer_path, type, last_modified, file_format_version, descriptor);
        }
    } else if (json_root_object.value("layer") != QJsonValue::Undefined) {
        const QJsonObject &json_layer_object = json_root_object.value("layer").toObject();
        status = this->LoadLayer(json_layer_object, layer_path, type, last_modified, file_format_version, descriptor);
    } else {
        assert(0);
    }

    return status;
}

LayerLoadStatus LayerManager::LoadLayer(const QJsonObject &json_layer_object, const Path &layer_path, LayerType type,
                                        const std::string &last_modified, Version file_format_version, LayerDescriptor descriptor) {
    Layer layer;
    layer.type = type;
    layer.manifest_path = layer_path;
    layer.last_modified = last_modified;
    layer.file_format_version = file_format_version;
    layer.descriptor = descriptor;

    LayerLoadStatus status = layer.Load(json_layer_object);
    if (status == LAYER_LOAD_INVALID || status == LAYER_LOAD_IGNORED) {
        return status;
    }

    Layer *duplicated_layer = this->Find(layer.GetId(), false);
    if (duplicated_layer != nullptr) {
        if (duplicated_layer->descriptor.removed) {
            duplicated_layer->descriptor.removed = false;
            duplicated_layer->descriptor.enabled = true;
        } else if (duplicated_layer->last_modified != layer.last_modified) {
            // Reload when the manifest was updated
            LayerLoadStatus reloaded_status = duplicated_layer->Load(json_layer_object);
            status = reloaded_status == LAYER_LOAD_ADDED ? LAYER_LOAD_RELOADED : reloaded_status;
        } else {
            status = LAYER_LOAD_UNMODIFIED;
        }
    } else {
        this->available_layers.push_back(layer);
    }

    return status;
}

void LayerManager::RemoveLayer(LayerId id) {
    Layer *layer = this->Find(id, false);
    assert(layer);

    layer->descriptor.enabled = false;
    layer->descriptor.removed = true;
}

void LayerManager::EnableLayer(LayerId id, bool enable) {
    Layer *layer = this->Find(id, false);
    assert(layer);

    layer->descriptor.enabled = enable;
}

std::vector<std::string> LayerManager::GatherLayerNames() const {
    std::vector<std::string> result;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        if (!this->available_layers[i].descriptor.enabled) {
            continue;
        }

        if (std::find(result.begin(), result.end(), this->available_layers[i].key) != result.end()) {
            continue;
        }

        result.push_back(this->available_layers[i].key);
    }

    return result;
}

bool operator<(const LayerDisplay &a, const LayerDisplay &b) {
    if (a.id.key == b.id.key) {
        return a.id.api_version < b.id.api_version;
    } else {
        return a.id.key < b.id.key;
    }
}

std::map<Path, std::map<std::string, LayerDisplay>> LayerManager::BuildLayerStoreList() const {
    std::map<Path, std::map<std::string, LayerDisplay>> result;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        const Layer *layer = &this->available_layers[i];
        if (layer == nullptr) {
            continue;
        }
        if (layer->descriptor.removed) {
            continue;
        }

        LayerDisplay layer_display;
        layer_display.id.manifest_path = layer->manifest_path;
        layer_display.id.key = layer->key;
        layer_display.id.api_version = layer->api_version;
        layer_display.descriptor = layer->descriptor;

        auto it = result.find(layer->manifest_path);
        if (it == result.end()) {
            std::map<std::string, LayerDisplay> entry;
            entry.insert(std::make_pair(layer->key, layer_display));
            result.insert(std::make_pair(layer->manifest_path, entry));
        } else {
            auto jt = it->second.find(layer->key);
            if (jt == it->second.end()) {
                it->second.insert(std::make_pair(layer->key, layer_display));
            } else {
                assert(0);
            }
        }
    }

    return result;
}

std::set<LayerDisplay> LayerManager::BuildLayerDisplayList() const {
    std::set<LayerDisplay> result;

    for (std::size_t i = 0, n = this->available_layers.size(); i < n; ++i) {
        const Layer *layer = &this->available_layers[i];
        if (layer == nullptr) {
            continue;
        }
        if (layer->descriptor.removed) {
            continue;
        }

        LayerDisplay layer_display;
        layer_display.id.manifest_path = layer->manifest_path;
        layer_display.id.key = layer->key;
        layer_display.id.api_version = layer->api_version;
        layer_display.descriptor = layer->descriptor;
        result.insert(layer_display);
    }

    return result;
}

std::vector<Path> LayerManager::BuildLayerPaths() const {
    std::vector<Path> result;

    for (auto it = this->layer_init.begin(); it != this->layer_init.end(); ++it) {
        result.push_back(it->first);
    }

    return result;
}
