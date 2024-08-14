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
#include "vulkan_info.h"
#include "util.h"
#include "path.h"
#include "alert.h"

#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonArray>

#include <cassert>
#include <cstdio>
#include <algorithm>

static QJsonObject CreateJsonSettingObject(const Configurator::LoaderSettings& loader_settings) {
    QJsonArray json_layers;
    for (std::size_t j = 0, o = loader_settings.layers.size(); j < o; ++j) {
        const Configurator::LoaderLayerSettings& layer = loader_settings.layers[j];

        QJsonObject json_layer;
        if (layer.control != LAYER_CONTROL_APPLICATIONS_API && layer.control != LAYER_CONTROL_APPLICATIONS_ENV) {
            json_layer.insert("name", layer.key.c_str());
            json_layer.insert("path", layer.path.c_str());
            json_layer.insert("treat_as_implicit_manifest", layer.implicit);
        }

        json_layer.insert("control", ToLowerCase(::GetToken(layer.control)).c_str());
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

    json_settings.insert("layers", json_layers);
    json_settings.insert("stderr_log", json_stderr_log);
    return json_settings;
}

void Configurator::BuildLoaderSettings(const ConfigurationInfo& info, const std::string& executable_path,
                                       std::vector<LoaderSettings>& loader_settings_array) const {
    LoaderSettings result;
    result.executable_path = executable_path;

    static Configuration disbled_configuration = Configuration::CreateDisabled(this->layers.selected_layers);
    const Configuration* configuration = nullptr;

    switch (info.GetMode()) {
        case LAYERS_CONTROLLED_BY_APPLICATIONS:
            return;
        case LAYERS_CONTROLLED_BY_CONFIGURATOR: {
            configuration = this->configurations.FindConfiguration(info.GetName());
            if (configuration == nullptr) {
                return;
            }
        } break;
        case LAYERS_DISABLED_BY_CONFIGURATOR: {
            configuration = &disbled_configuration;
        } break;
    }
    assert(configuration != nullptr);

    result.stderr_log_flags = configuration->loader_log_messages_flags;

    for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
        LoaderLayerSettings loader_layer_settings;

        const Parameter& parameter = configuration->parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.control == LAYER_CONTROL_APPLICATIONS_API || parameter.control == LAYER_CONTROL_APPLICATIONS_ENV) {
            loader_layer_settings.control = parameter.control;
        } else {
            const Layer* layer = FindByKey(this->layers.selected_layers, parameter.key.c_str());
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

        if (this->environment.GetPerApplicationConfig()) {
            for (std::size_t i = 0, n = this->environment.GetApplications().size(); i < n; ++i) {
                const Application& application = this->environment.GetApplication(i);

                this->BuildLoaderSettings(application.configuration, application.executable_path.AbsolutePath(),
                                          loader_settings_array);
            }
        } else {
            BuildLoaderSettings(this->environment.global_configuration, "", loader_settings_array);
        }

        if (!loader_settings_array.empty()) {
            QJsonObject root;
            root.insert("file_format_version", "1.0.0");
            if (loader_settings_array.size() > 1) {
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

        if (this->environment.GetPerApplicationConfig()) {
            const std::vector<Application>& applications = this->environment.GetApplications();

            for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
                LayersSettings settings;
                settings.configuration_name = applications[i].configuration.GetName();
                settings.executable_path = applications[i].executable_path;
                settings.settings_path = applications[i].GetActiveOptions().working_folder;
                layers_settings_array.push_back(settings);
            }
        } else {
            LayersSettings settings;
            settings.configuration_name = this->environment.global_configuration.GetName();
            settings.settings_path = layers_settings_path;
            layers_settings_array.push_back(settings);
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
                if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
                    continue;
                }

                const Layer* layer = this->layers.Find(parameter.key.c_str());
                if (layer == nullptr) {
                    has_missing_layers = true;
                    continue;
                }

                if (parameter.control != LAYER_CONTROL_ON) {
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

Configurator& Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() : environment(), layers(), configurations() {}

Configurator::~Configurator() {
    this->UpdateLayersValidationCache();

    this->configurations.SaveAllConfigurations();

    this->Surrender(OVERRIDE_AREA_ALL);
}

void Configurator::UpdateLayersValidationCache() {
    this->environment.layers_validated.clear();

    for (std::size_t i = 0, n = this->layers.Size(); i < n; ++i) {
        const Layer& layer = this->layers.selected_layers[i];

        this->environment.layers_validated.insert(
            std::make_pair(layer.manifest_path.AbsolutePath(), layer.validated_last_modified));
    }
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    this->layers.LoadAllInstalledLayers(environment.layers_validated);

    if (this->environment.has_crashed) {
        this->environment.has_crashed = false;

        if (Alert::ConfiguratorCrashed() == QMessageBox::No) {
            this->configurations.LoadAllConfigurations(this->layers.selected_layers);
        }
    } else {
        this->configurations.LoadAllConfigurations(this->layers.selected_layers);
    }

    this->Override(OVERRIDE_AREA_ALL);

    return true;
}

bool Configurator::Override(OverrideArea override_area) {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    // Clean up
    this->Surrender(override_area);

    // vk_loader_settings.json
    bool result_loader_settings = this->WriteLoaderSettings(override_area, loader_settings_path);

    // TODO, Add per application layer_settings file
    // vk_layer_settings.txt
    bool result_layers_settings = this->WriteLayersSettings(override_area, layers_settings_path);

    // On Windows only, we need to write these values to the registry
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
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

    // TODO, Remove per application layer_settings file
    bool result_layers_settings = true;
    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        result_layers_settings = layers_settings_path.Remove();
    }

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    RemoveRegistryEntriesForLayers(loader_settings_path.AbsolutePath().c_str(), layers_settings_path.AbsolutePath().c_str());
#endif

    return result_loader_settings && result_layers_settings;
}

bool Configurator::HasOverride() const {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    return loader_settings_path.Exists() || layers_settings_path.Exists();
}

void Configurator::ResetToDefault(bool hard) {
    this->environment.Reset(Environment::CLEAR);
    this->layers.LoadAllInstalledLayers(this->environment.layers_validated);
    this->configurations.Reset(this->layers.selected_layers);
}

Configuration* Configurator::GetActiveConfiguration() {
    const ConfigurationInfo& info = this->environment.GetActiveConfigurationInfo();
    return this->configurations.FindConfiguration(info.GetName());
}

const Configuration* Configurator::GetActiveConfiguration() const {
    const ConfigurationInfo& info = this->environment.GetActiveConfigurationInfo();
    return this->configurations.FindConfiguration(info.GetName());
}

bool Configurator::HasActiveConfiguration() const {
    if (this->environment.GetPerApplicationConfig()) {
        return this->environment.HasOverriddenApplications();
    } else {
        return this->environment.GetActiveConfigurationInfo().GetMode() != LAYERS_CONTROLLED_BY_APPLICATIONS;
    }
}
