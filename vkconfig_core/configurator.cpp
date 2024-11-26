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
 * - Lenny Komow <lenny@lunarg.com>
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configurator.h"
#include "registry.h"
#include "util.h"
#include "path.h"
#include "alert.h"
#include "date.h"

#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonArray>

#include <cassert>
#include <cstdio>
#include <algorithm>

Configurator& Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() {}

Configurator::~Configurator() {
    if (this->mode == GUI) {
        QJsonObject json_root_object;

        this->Save(json_root_object);
        this->configurations.Save(json_root_object);
        this->layers.Save(json_root_object);
        this->executables.Save(json_root_object);

        QJsonDocument json_doc(json_root_object);

        const Path& vkconfig_init_path = ::Get(Path::INIT);
        QFile file(vkconfig_init_path.AbsolutePath().c_str());
        const bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
        assert(result);

        file.write(json_doc.toJson());
        file.close();

        this->Surrender(OVERRIDE_AREA_ALL);
    }
}

bool Configurator::Init(Mode mode) {
    this->mode = mode;

    const Path& vkconfig_init_path = ::Get(Path::INIT);

    QString init_data;

    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    const bool has_init_file = file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (has_init_file) {
        init_data = file.readAll();
        file.close();

        const QJsonDocument& json_doc = QJsonDocument::fromJson(init_data.toLocal8Bit());
        const QJsonObject& json_root_object = json_doc.object();

        this->Load(json_root_object);

        bool request_reset = false;
        if (this->has_crashed) {
            this->has_crashed = false;

            if (Alert::ConfiguratorCrashed() == QMessageBox::Yes) {
                request_reset = true;
            }
        }

        if (request_reset) {
            this->Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

            this->Reset(true);
        } else {
            this->executables.Load(json_root_object);
            this->layers.Load(json_root_object);
            this->configurations.Load(json_root_object);
        }

        this->Override(OVERRIDE_AREA_ALL);
    } else {
        this->Reset(false);
    }

    return true;
}

static QJsonObject CreateJsonSettingObject(const Configurator::LoaderSettings& loader_settings) {
    QJsonArray json_layers;
    for (std::size_t j = 0, o = loader_settings.layers.size(); j < o; ++j) {
        const Configurator::LoaderLayerSettings& layer = loader_settings.layers[j];

        QJsonObject json_layer;
        if (layer.builtin == LAYER_BUILTIN_NONE) {
            json_layer.insert("name", layer.key.c_str());
            json_layer.insert("path", layer.path.c_str());
            json_layer.insert("treat_as_implicit_manifest", layer.implicit);
            json_layer.insert("control", ToLowerCase(::GetToken(layer.control)).c_str());
        } else {
            json_layer.insert("control", ::GetToken(layer.builtin));
        }

        json_layers.append(json_layer);
    }

    QJsonArray json_stderr_log;
    const std::vector<std::string>& stderr_log = GetLogTokens(loader_settings.stderr_log_flags);
    for (std::size_t i = 0, n = stderr_log.size(); i < n; ++i) {
        json_stderr_log.append(stderr_log[i].c_str());
    }

    QJsonObject json_settings;

    if (!loader_settings.executable_path.empty()) {
        QJsonArray json_app_keys;
        json_app_keys.append(loader_settings.executable_path.c_str());
        json_settings.insert("app_keys", json_app_keys);
    }

    if (loader_settings.override_layers) {
        json_settings.insert("layers", json_layers);
    }
    if (loader_settings.override_loader) {
        json_settings.insert("stderr_log", json_stderr_log);
    }
    return json_settings;
}

void Configurator::BuildLoaderSettings(const std::string& configuration_key, const std::string& executable_path,
                                       std::vector<LoaderSettings>& loader_settings_array) const {
    LoaderSettings result;
    result.executable_path = executable_path;

    const Configuration* configuration = this->configurations.FindConfiguration(configuration_key);
    assert(configuration != nullptr);

    result.override_loader = configuration->override_loader;
    result.override_layers = configuration->override_layers;
    result.stderr_log_flags = configuration->loader_log_messages_flags;

    for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
        LoaderLayerSettings loader_layer_settings;

        const Parameter& parameter = configuration->parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.control == LAYER_CONTROL_DISCARD) {
            continue;
        }

        loader_layer_settings.key = parameter.key;
        loader_layer_settings.builtin = parameter.builtin;
        loader_layer_settings.control = parameter.control;

        if (parameter.builtin == LAYER_BUILTIN_NONE) {
            const Layer* layer = this->layers.FindFromManifest(parameter.manifest);
            if (layer == nullptr) {
                continue;
            }

            // Extract just the path
            loader_layer_settings.key = parameter.key;
            if (layer != nullptr) {
                loader_layer_settings.path = layer->manifest_path.AbsolutePath();
            }
            loader_layer_settings.control = parameter.control;
            loader_layer_settings.implicit = layer != nullptr ? layer->type == LAYER_TYPE_IMPLICIT : false;
        }

        result.layers.push_back(loader_layer_settings);
    }

    loader_settings_array.push_back(result);
}

// Create and write vk_loader_settings.json file
bool Configurator::WriteLoaderSettings(OverrideArea override_area, const Path& loader_settings_path) {
    assert(!loader_settings_path.Empty());

    if (override_area & OVERRIDE_AREA_LOADER_SETTINGS_BIT) {
        std::vector<LoaderSettings> loader_settings_array;

        std::string configuration = this->selected_global_configuration;

        switch (this->executable_scope) {
            case EXECUTABLE_ANY:
                this->BuildLoaderSettings(configuration, "", loader_settings_array);
                break;
            case EXECUTABLE_ALL:
            case EXECUTABLE_PER: {
                const std::vector<Executable>& executables = this->executables.GetExecutables();
                for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                    if (!executables[i].enabled) {
                        continue;
                    }

                    if (this->executable_scope == EXECUTABLE_PER) {
                        configuration = executables[i].configuration;
                    }

                    this->BuildLoaderSettings(configuration, executables[i].path.AbsolutePath(), loader_settings_array);
                }
                break;
            }
            default:
            case EXECUTABLE_NONE:
                break;
        }

        if (this->executable_scope != EXECUTABLE_NONE) {
            QJsonObject root;
            root.insert("file_format_version", "1.0.0");
            if (::EnabledExecutables(this->executable_scope)) {
                QJsonArray json_settings_array;
                for (std::size_t i = 0, n = loader_settings_array.size(); i < n; ++i) {
                    json_settings_array.append(CreateJsonSettingObject(loader_settings_array[i]));
                }
                root.insert("settings_array", json_settings_array);
            } else if (!loader_settings_array.empty()) {
                root.insert("settings", CreateJsonSettingObject(loader_settings_array[0]));
            }
            QJsonDocument doc(root);

            QFile json_file(loader_settings_path.AbsolutePath().c_str());
            const bool result_layers_file = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
            assert(result_layers_file);
            json_file.write(doc.toJson());
            json_file.close();

            return result_layers_file;
        }
        return true;
    } else {
        return true;
    }
}

// Create and write vk_layer_settings.txt file
bool Configurator::WriteLayersSettings(OverrideArea override_area, const Path& layers_settings_path) {
    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        std::vector<LayersSettings> layers_settings_array;

        switch (this->executable_scope) {
            case EXECUTABLE_ALL:
            case EXECUTABLE_ANY: {
                LayersSettings settings;
                settings.configuration_name = this->selected_global_configuration;
                settings.settings_path = layers_settings_path;
                layers_settings_array.push_back(settings);
                break;
            }
            case EXECUTABLE_PER: {
                const std::vector<Executable>& executables = this->executables.GetExecutables();

                std::string configuration_name = this->selected_global_configuration;

                for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                    if (!executables[i].enabled) {
                        continue;
                    }

                    if (this->executable_scope == EXECUTABLE_PER) {
                        configuration_name = executables[i].configuration;
                    }

                    LayersSettings settings;
                    settings.executable_path = executables[i].path;
                    settings.configuration_name = configuration_name;
                    settings.settings_path = executables[i].GetLocalLayersSettingsPath();
                    layers_settings_array.push_back(settings);
                }
                break;
            }
            default:
            case EXECUTABLE_NONE:
                break;
        }

        bool has_missing_layers = false;
        bool result_settings_file = true;

        for (std::size_t i = 0, n = layers_settings_array.size(); i < n; ++i) {
            const LayersSettings& layers_settings = layers_settings_array[i];

            Configuration* configuration = this->configurations.FindConfiguration(layers_settings.configuration_name);
            if (configuration == nullptr) {
                if (layers_settings.executable_path.Empty()) {
                    fprintf(stderr, "Fail to apply unknown '%s' global configuration\n",
                            layers_settings.configuration_name.c_str());
                } else {
                    fprintf(stderr, "Fail to apply unknown '%s' configuration to '%s'\n",
                            layers_settings.configuration_name.c_str(), layers_settings.executable_path.AbsolutePath().c_str());
                }

                result_settings_file = false;
                continue;
            }

            if (layers_settings.settings_path.Exists()) {
                if (this->GetActiveConfiguration()->key != configuration->key) {
                    continue;
                }
            }

            QFile file(layers_settings.settings_path.AbsolutePath().c_str());
            result_settings_file = result_settings_file && file.open(QIODevice::WriteOnly | QIODevice::Text);
            if (!result_settings_file) {
                fprintf(stderr, "Cannot open file %s\n", layers_settings.settings_path.AbsolutePath().c_str());
                continue;
            }
            QTextStream stream(&file);

            // Loop through all the layers
            for (std::size_t j = 0, n = configuration->parameters.size(); j < n; ++j) {
                const Parameter& parameter = configuration->parameters[j];
                if (!parameter.override_settings) {
                    continue;
                }

                if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
                    continue;
                }

                if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
                    continue;
                }

                if (parameter.control == LAYER_CONTROL_DISCARD || parameter.control == LAYER_CONTROL_OFF) {
                    continue;
                }

                const Layer* layer = this->layers.Find(parameter.key.c_str(), parameter.api_version);
                if (layer == nullptr) {
                    has_missing_layers = true;
                    continue;
                }

                stream << "\n";
                stream << "# " << layer->key.c_str() << "\n\n";

                std::string lc_layer_name = GetLayerSettingPrefix(layer->key);

                for (std::size_t i = 0, m = parameter.settings.size(); i < m; ++i) {
                    const SettingData* setting_data = parameter.settings[i];

                    // Skip groups - they aren't settings, so not relevant in this output
                    if (setting_data->type == SETTING_GROUP) {
                        continue;
                    }

                    // Skip missing settings
                    const SettingMeta* meta = FindSetting(layer->settings, setting_data->key.c_str());
                    if (meta == nullptr) {
                        continue;
                    }

                    // Skip overriden settings
                    if (::CheckSettingOverridden(*meta)) {
                        continue;
                    }

                    stream << "# ";
                    stream << meta->label.c_str();
                    stream << "\n# =====================\n# <LayerIdentifier>.";
                    stream << meta->key.c_str() << "\n";

                    // Break up description into smaller words
                    std::string description = meta->description;
                    std::vector<std::string> words;
                    std::size_t pos;
                    while ((pos = description.find(" ")) != std::string::npos) {
                        words.push_back(description.substr(0, pos));
                        description.erase(0, pos + 1);
                    }
                    if (description.size() > 0) words.push_back(description);
                    if (words.size() > 0) {
                        stream << "#";
                        std::size_t nchars = 2;
                        for (auto word : words) {
                            if (word.size() + nchars > 80) {
                                stream << "\n#";
                                nchars = 2;
                            }
                            stream << " " << word.c_str();
                            nchars += (word.size() + 1);
                        }
                    }
                    stream << "\n";

                    // If feature has unmet dependency, output it but comment it out
                    if (::CheckDependence(*meta, parameter.settings) != SETTING_DEPENDENCE_ENABLE) {
                        stream << "#";
                    }

                    stream << lc_layer_name.c_str() << setting_data->key.c_str() << " = ";
                    stream << setting_data->Export(EXPORT_MODE_OVERRIDE).c_str();
                    stream << "\n\n";
                }
            }
            file.close();
        }

        return result_settings_file && !has_missing_layers;
    } else {
        return true;
    }
}

bool Configurator::Override(OverrideArea override_area) {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    // Clean up
    this->Surrender(override_area);

    // vk_loader_settings.json
    bool result_loader_settings = this->WriteLoaderSettings(override_area, loader_settings_path);

    // vk_layer_settings.txt
    bool result_layers_settings = this->WriteLayersSettings(override_area, layers_settings_path);

    // On Windows only, we need to write these values to the registry
#if VKC_ENV == VKC_ENV_WIN32
    AppendRegistryEntriesForLayers(loader_settings_path.AbsolutePath().c_str(), layers_settings_path.AbsolutePath().c_str());
#endif

    return result_loader_settings && result_layers_settings;
}

bool Configurator::Surrender(OverrideArea override_area) {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    bool result_loader_settings = true;
    if (override_area & OVERRIDE_AREA_LOADER_SETTINGS_BIT) {
        result_loader_settings = loader_settings_path.Remove();
    }

    bool result_layers_settings = true;
    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        bool global_removed = layers_settings_path.Remove();
        if (this->executable_scope == EXECUTABLE_ALL || this->executable_scope == EXECUTABLE_ANY) {
            result_layers_settings = global_removed;
        }

        const std::vector<Executable>& executables = this->executables.GetExecutables();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            Path path(executables[i].GetActiveOptions()->working_folder + "/vk_layer_settings.txt");
            if (path.Exists()) {
                bool local_removed = path.Remove();
                if (::EnabledExecutables(this->executable_scope)) {
                    result_layers_settings = result_layers_settings && local_removed;
                }
            }
        }
    }

#if VKC_ENV == VKC_ENV_WIN32
    RemoveRegistryEntriesForLayers();
#endif

    return result_loader_settings && result_layers_settings;
}

bool Configurator::HasOverride() const {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    return loader_settings_path.Exists() || layers_settings_path.Exists();
}

void Configurator::Reset(bool hard) {
    this->Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->Reset();
    this->executables.Reset();
    this->layers.Reset();

    if (hard) {
        this->configurations.RemoveConfigurationFiles();
    }

    this->configurations.Reset();
}

void Configurator::SetActiveConfigurationName(const std::string& configuration_name) {
    if (this->executable_scope == EXECUTABLE_PER) {
        Executable* executable = this->executables.GetActiveExecutable();
        executable->configuration = configuration_name;
    } else {
        this->selected_global_configuration = configuration_name;
    }
}

Configuration* Configurator::GetActiveConfiguration() {
    if (this->executable_scope == EXECUTABLE_PER) {
        const Executable* executable = this->executables.GetActiveExecutable();
        return this->configurations.FindConfiguration(executable->configuration);
    } else {
        return this->configurations.FindConfiguration(this->selected_global_configuration);
    }
}

const Configuration* Configurator::GetActiveConfiguration() const {
    return const_cast<Configurator*>(this)->GetActiveConfiguration();
}

Parameter* Configurator::GetActiveParameter() {
    Configuration* configuration = this->GetActiveConfiguration();
    if (configuration != nullptr) {
        return configuration->GetActiveParameter();
    } else {
        return nullptr;
    }
}

const Parameter* Configurator::GetActiveParameter() const { return const_cast<Configurator*>(this)->GetActiveParameter(); }

bool Configurator::HasActiveParameter() const { return this->GetActiveParameter() != nullptr; }

Executable* Configurator::GetActiveExecutable() { return this->executables.GetActiveExecutable(); }

const Executable* Configurator::GetActiveExecutable() const { return this->executables.GetActiveExecutable(); }

void Configurator::Reset() {
    this->has_crashed = false;
    this->use_system_tray = false;
    this->executable_scope = EXECUTABLE_ANY;
    this->selected_global_configuration = "Validation";
}

std::string Configurator::LogConfiguration(const std::string& configuration_key) const {
    const Configuration* configuration = this->configurations.FindConfiguration(configuration_key);
    assert(configuration != nullptr);

    std::string log = format("'%s' Loader Configuration:\n", configuration->key.c_str());

    if (configuration->override_layers) {
        log += " - Vulkan Layers Selection and Execution Order:\n";
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration->parameters[i];

            if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
                log += format("   * %s: %s\n", parameter.key.c_str(), ::GetToken(parameter.control));
            } else {
                if (this->layers.Find(parameter.key, parameter.api_version) == nullptr) {
                    log += format("   * %s - Missing: %s\n", parameter.key.c_str(), ::GetToken(parameter.control));
                } else {
                    log += format("   * %s - %s: %s\n", parameter.key.c_str(), parameter.api_version.str().c_str(),
                                  ::GetToken(parameter.control));
                }
                log += format("     Layer manifest path: %s\n", parameter.manifest.AbsolutePath().c_str());
                log += format("     Layer settings export: %s\n", parameter.override_settings ? "enabled" : "disabled");
            }
        }
    } else {
        log += " - Vulkan Layers Selection and Execution Order: disabled\n";
    }

    if (configuration->override_loader) {
        log += format(" - Vulkan Loader Messages: %s\n", GetLogString(configuration->loader_log_messages_flags).c_str());
    } else {
        log += " - Vulkan Loader Messages: disabled\n";
    }

    log += "\n";

    return log;
}

std::string Configurator::Log() const {
    std::string log;

#ifdef _DEBUG
    std::string build = "Debug";
#else
    std::string build = "Release";
#endif

    log += format("%s %s - %s:\n", VKCONFIG_NAME, Version::VKCONFIG.str().c_str(), GetBuildDate().c_str());
    log += format(" - Build: %s %s\n", GetLabel(VKC_PLATFORM), build.c_str());
    log += format(" - Vulkan API version: %s\n", Version::VKHEADER.str().c_str());
    log += format(" - ${VULKAN_SDK}: %s\n", ::Get(Path::SDK).AbsolutePath().c_str());
    log += "\n";

    log += format("%s Settings:\n", VKCONFIG_NAME);
    log += format(" - Vulkan Loader configuration scope: %s\n", ::GetLabel(this->GetExecutableScope()));
    log += format("   * Loader settings: %s\n", ::Get(Path::LOADER_SETTINGS).AbsolutePath().c_str());

    if (this->GetExecutableScope() == EXECUTABLE_ANY || this->GetExecutableScope() == EXECUTABLE_ALL) {
        log += format("   * Layers settings: %s\n", ::Get(Path::LAYERS_SETTINGS).AbsolutePath().c_str());
    }

    if (this->GetExecutableScope() == EXECUTABLE_ANY || this->GetExecutableScope() == EXECUTABLE_ALL) {
        const Configuration* configuration = this->GetActiveConfiguration();
        if (configuration != nullptr) {
            log += format(" - Active Vulkan Loader Configuration: '%s'\n", configuration->key.c_str());
        } else {
            log += " - No Active Vulkan Loader Configuration\n";
        }
    }
    if (this->GetExecutableScope() == EXECUTABLE_ALL) {
        log += " - Listed Executables:\n";
        const std::vector<Executable>& executables = this->executables.GetExecutables();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            log += format("   * %s: %s\n", executables[i].path.AbsolutePath().c_str(),
                          executables[i].enabled ? "enabled" : "disabled");
        }
    }

    if (this->GetExecutableScope() == EXECUTABLE_PER) {
        log += " - Listed Executables:\n";
        const std::vector<Executable>& executables = this->executables.GetExecutables();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            const Configuration* configuration = this->configurations.FindConfiguration(executables[i].configuration);

            log += format("   * %s: '%s'\n", executables[i].path.AbsolutePath().c_str(),
                          executables[i].enabled && configuration != nullptr ? executables[i].configuration.c_str() : "None");
            if (!executables[i].enabled) {
                continue;
            }

            log += format("     Vulkan Loader settings file: %s\n",
                          executables[i].GetLocalLayersSettingsPath().AbsolutePath().c_str());
        }
    }

    log += format(" - Use system tray: %s\n", this->use_system_tray ? "true" : "false");
    log += format(" - ${VK_HOME}: %s\n", ::Get(Path::HOME).AbsolutePath().c_str());
    log += "\n";

    if (this->GetExecutableScope() == EXECUTABLE_ANY || this->GetExecutableScope() == EXECUTABLE_ALL) {
        const Configuration* configuration = this->GetActiveConfiguration();

        if (configuration != nullptr) {
            log += LogConfiguration(configuration->key.c_str());
        }
    } else if (this->GetExecutableScope() == EXECUTABLE_PER) {
        const std::vector<Executable>& executables = this->executables.GetExecutables();

        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            if (!executables[i].enabled) {
                continue;
            }
            if (executables[i].configuration.empty()) {
                continue;
            }
            const Configuration* configuration = this->configurations.FindConfiguration(executables[i].configuration);
            if (configuration == nullptr) {
                continue;
            }
            log += LogConfiguration(configuration->key.c_str());
        }
    }

    log += "Vulkan Physical Devices:\n";
    for (std::size_t i = 0, n = this->vulkan_system_info.physicalDevices.size(); i < n; ++i) {
        const VulkanPhysicalDeviceInfo& info = this->vulkan_system_info.physicalDevices[i];
        log += format(" - %s with Vulkan %s (%s)\n", info.deviceName.c_str(), info.apiVersion.str().c_str(),
                      GetLabel(info.deviceType));
        if (info.vendorID == 0x10DE) {
            log += format("   * Driver: %s %s\n", GetLabel(info.vendorID).c_str(), FormatNvidia(info.driverVersion).c_str());
        } else if ((info.vendorID == 0x8086) && (VKC_PLATFORM & PLATFORM_WINDOWS_BIT)) {
            log += format("   * Driver: %s %s\n", GetLabel(info.vendorID).c_str(), FormatIntelWindows(info.driverVersion).c_str());
        } else {
            log += format("   * Driver: %s %s\n", GetLabel(info.vendorID).c_str(), Version(info.driverVersion).str().c_str());
        }
        log += format("   * deviceUUID: %s\n", info.deviceUUID.c_str());
        log += format("   * driverUUID: %s\n", info.driverUUID.c_str());
        log += format("   * deviceLUID: %s\n", info.deviceLUID.c_str());
    }
    log += "\n";

    log += "Vulkan Loader Settings:\n";
    if (this->vulkan_system_info.loaderVersion != Version::NONE) {
        log += format(" - API version: %s\n", this->vulkan_system_info.loaderVersion.str().c_str());
    } else {
        log += format(" - Couldn't Find a Vulkan Loader\n");
    }

    log += " - Vulkan Loader found Vulkan layers:\n";
    for (std::size_t i = 0, n = this->vulkan_system_info.instanceLayerProperties.size(); i < n; ++i) {
        log += format("   * %s\n", this->vulkan_system_info.instanceLayerProperties[i].layerName);
    }
    log += " - Vulkan implementation or by implicitly enabled layers extensions:\n";
    for (std::size_t i = 0, n = this->vulkan_system_info.instanceExtensionPropertie.size(); i < n; ++i) {
        log += format("   * %s\n", this->vulkan_system_info.instanceExtensionPropertie[i].extensionName);
    }

    if (qEnvironmentVariableIsSet("VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING")) {
        log += format(" - ${VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING}: %s\n",
                      qgetenv("VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING").toStdString().c_str());
    } else {
        log += " - ${VK_LOADER_DISABLE_DYNAMIC_LIBRARY_UNLOADING}: unset\n";
    }

    log += " - Vulkan Loader Layers environment variables:\n";
    if (qEnvironmentVariableIsSet("VK_LOADER_DEBUG")) {
        log += format("   * ${VK_LOADER_DEBUG}: %s\n", qgetenv("VK_LOADER_DEBUG").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DEBUG}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_LAYERS_ENABLE")) {
        log += format("   * ${VK_LOADER_LAYERS_ENABLE}: %s\n", qgetenv("VK_LOADER_LAYERS_ENABLE").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_LAYERS_ENABLE}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_LAYERS_DISABLE")) {
        log += format("   * ${VK_LOADER_LAYERS_DISABLE}: %s\n", qgetenv("VK_LOADER_LAYERS_DISABLE").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_LAYERS_DISABLE}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_LAYERS_ALLOW")) {
        log += format("   * ${VK_LOADER_LAYERS_ALLOW}: %s\n", qgetenv("VK_LOADER_LAYERS_ALLOW").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_LAYERS_ALLOW}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_INSTANCE_LAYERS")) {
        log += format("   * ${VK_INSTANCE_LAYERS}: %s\n", qgetenv("VK_INSTANCE_LAYERS").toStdString().c_str());
    }
    log += " - Vulkan Loader Drivers environment variables:\n";
    if (qEnvironmentVariableIsSet("VK_DRIVER_FILES")) {
        log += format("   * ${VK_DRIVER_FILES}: %s\n", qgetenv("VK_DRIVER_FILES").toStdString().c_str());
    } else {
        log += "   * ${VK_DRIVER_FILES}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_ADD_DRIVER_FILES")) {
        log += format("   * ${VK_ADD_DRIVER_FILES}: %s\n", qgetenv("VK_ADD_DRIVER_FILES").toStdString().c_str());
    } else {
        log += "   * ${VK_ADD_DRIVER_FILES}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_DRIVERS_SELECT")) {
        log += format("   * ${VK_LOADER_DRIVERS_SELECT}: %s\n", qgetenv("VK_LOADER_DRIVERS_SELECT").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DRIVERS_SELECT}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_DRIVERS_DISABLE")) {
        log += format("   * ${VK_LOADER_DRIVERS_DISABLE}: %s\n", qgetenv("VK_LOADER_DRIVERS_DISABLE").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DRIVERS_DISABLE}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_LOADER_DISABLE_INST_EXT_FILTER")) {
        log += format("   * ${VK_LOADER_DISABLE_INST_EXT_FILTER}: %s\n",
                      qgetenv("VK_LOADER_DISABLE_INST_EXT_FILTER").toStdString().c_str());
    } else {
        log += "   * ${VK_LOADER_DISABLE_INST_EXT_FILTER}: unset\n";
    }
    if (qEnvironmentVariableIsSet("VK_ICD_FILENAMES")) {
        log += format("   * ${VK_ICD_FILENAMES}: %s\n", qgetenv("VK_ICD_FILENAMES").toStdString().c_str());
    }
    log += "\n";

    return log;
}

bool Configurator::Load(const QJsonObject& json_root_object) {
    const Version file_format_version = Version(json_root_object.value("file_format_version").toString().toStdString());
    if (file_format_version > Version::VKCONFIG) {
        return false;  // Vulkan Configurator needs to be updated
    }

    // interface json object
    const QJsonObject& json_interface_object = json_root_object.value("interface").toObject();
    this->has_crashed = json_interface_object.value("has_crashed").toBool();

    const QJsonArray& json_hide_message_boxes_array = json_interface_object.value("hide_message_boxes").toArray();
    this->hide_message_boxes_flags = 0;
    for (int i = 0, n = json_hide_message_boxes_array.size(); i < n; ++i) {
        const std::string& token = json_hide_message_boxes_array[i].toString().toStdString();
        this->hide_message_boxes_flags |= GetHideMessageBit(token.c_str());
    }

    this->active_tab = GetTabType(json_interface_object.value("active_tab").toString().toStdString().c_str());

    // TAB_CONFIGURATIONS
    if (json_interface_object.value(GetToken(TAB_CONFIGURATIONS)) != QJsonValue::Undefined) {
        const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_CONFIGURATIONS)).toObject();
        this->use_system_tray = json_object.value("use_system_tray").toBool();
        this->advanced = json_object.value("advanced").toBool();
        this->executable_scope = ::GetExecutableScope(json_object.value("executable_scope").toString().toStdString().c_str());
        this->selected_global_configuration = json_object.value("selected_global_configuration").toString().toStdString();
    }

    // TAB_LAYERS
    if (json_interface_object.value(GetToken(TAB_LAYERS)) != QJsonValue::Undefined) {
        const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_LAYERS)).toObject();
    }

    // TAB_APPLICATIONS
    if (json_interface_object.value(GetToken(TAB_APPLICATIONS)) != QJsonValue::Undefined) {
        const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_APPLICATIONS)).toObject();
    }

    // TAB_DIAGNOSTIC
    if (json_interface_object.value(GetToken(TAB_DIAGNOSTIC)) != QJsonValue::Undefined) {
        const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_DIAGNOSTIC)).toObject();

        if (json_object.value("VK_HOME") != QJsonValue::Undefined) {
            ::SetHomePath(json_object.value("VK_HOME").toString().toStdString());
        }
    }

    return true;
}

bool Configurator::Save(QJsonObject& json_root_object) const {
    json_root_object.insert("file_format_version", Version::VKCONFIG.str().c_str());

    QJsonObject json_interface_object;

    // TAB_CONFIGURATIONS
    {
        QJsonObject json_object;
        json_object.insert("use_system_tray", this->use_system_tray);
        json_object.insert("advanced", this->advanced);
        json_object.insert("executable_scope", ::GetToken(this->executable_scope));
        json_object.insert("selected_global_configuration", this->selected_global_configuration.c_str());
        json_interface_object.insert(::GetToken(TAB_CONFIGURATIONS), json_object);
    }

    // TAB_LAYERS
    {
        QJsonObject json_object;
        json_interface_object.insert(GetToken(TAB_LAYERS), json_object);
    }

    // TAB_APPLICATIONS
    {
        QJsonObject json_object;
        json_interface_object.insert(GetToken(TAB_APPLICATIONS), json_object);
    }

    // TAB_DIAGNOSTIC
    {
        QJsonObject json_object;
        json_object.insert("VK_HOME", ::Get(Path::HOME).RelativePath().c_str());
        json_interface_object.insert(GetToken(TAB_DIAGNOSTIC), json_object);
    }

    // interface json object
    {
        json_interface_object.insert("has_crashed", this->has_crashed);

        QJsonArray json_hide_message_boxes_array;
        for (int i = HIDE_MESSAGE_FIRST, n = HIDE_MESSAGE_COUNT; i < n; ++i) {
            HideMessageType type = static_cast<HideMessageType>(i);
            if (this->hide_message_boxes_flags & (1 << i)) {
                json_hide_message_boxes_array.append(GetToken(type));
            }
        }
        json_interface_object.insert("hide_message_boxes", json_hide_message_boxes_array);
        json_interface_object.insert("active_tab", GetToken(this->active_tab));

        json_root_object.insert("interface", json_interface_object);
    }

    return true;
}

void Configurator::Set(HideMessageType type) {
    assert(type >= HIDE_MESSAGE_FIRST && type <= HIDE_MESSAGE_LAST);
    this->hide_message_boxes_flags |= GetBit(type);
}

bool Configurator::Get(HideMessageType type) const { return this->hide_message_boxes_flags & GetBit(type); }

ExecutableScope Configurator::GetExecutableScope() const { return this->executable_scope; }

void Configurator::SetExecutableScope(ExecutableScope scope) {
    assert(scope >= EXECUTABLE_SCOPE_FIRST && scope <= EXECUTABLE_SCOPE_LAST);
    this->executable_scope = scope;
}

bool Configurator::GetUseSystemTray() const { return this->use_system_tray; }

void Configurator::SetUseSystemTray(bool enabled) { this->use_system_tray = enabled; }

bool Configurator::HasActiveSettings() const {
    if (this->executable_scope == EXECUTABLE_NONE) {
        return false;
    }

    const Configuration* configuration = this->GetActiveConfiguration();
    if (configuration != nullptr) {
        const Parameter* parameter = configuration->GetActiveParameter();
        if (parameter != nullptr && configuration->override_layers) {
            if (parameter->builtin != LAYER_BUILTIN_NONE) {
                return false;
            } else if (parameter->settings.empty()) {
                return false;
            } else {
                return this->layers.Find(parameter->key, parameter->api_version) != nullptr;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Configurator::HasEnabledUI(EnabledUI enabled_ui) const {
    switch (enabled_ui) {
        default:
            assert(false);
            return false;
        case ENABLE_UI_CONFIG: {
            return this->GetExecutableScope() != EXECUTABLE_NONE;
        }
        case ENABLE_UI_LOADER:
        case ENABLE_UI_LAYERS: {
            if (!this->HasEnabledUI(ENABLE_UI_CONFIG)) {
                return false;
            }
            if (::EnabledExecutables(this->GetExecutableScope())) {
                const Executable* executable = this->executables.GetActiveExecutable();
                if (executable == nullptr) {
                    return false;
                }
                if (!executable->enabled) {
                    return false;
                }
            }
            return this->GetActiveConfiguration() != nullptr;
        }
        case ENABLE_UI_SETTINGS: {
            if (!this->HasEnabledUI(ENABLE_UI_LAYERS)) {
                return false;
            }
            return this->HasActiveSettings();
        }
    }
}

std::string Configurator::GenerateVulkanStatus() const {
    std::string log;

    log += this->Log();

    log += "Vulkan Layers Locations\n";
    log += this->layers.Log();
    log += this->configurations.Log();
    log += this->executables.Log();
    // log += "Vulkan Loader Log:\n";
    // log += ::GenerateLoaderLog();

    return log;
}
