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

struct LayerSettings {
    std::string key;
    std::string path;
    LayerControl control = LAYER_CONTROL_AUTO;
    bool implicit = false;
};

struct LoaderSettings {
    std::string executable_path;
    std::vector<LayerSettings> layers;
    LogFlags stderr_log_flags = LOG_ERROR;
};

static QJsonObject CreateJsonSettingObject(const LoaderSettings& loader_settings) {
    QJsonArray json_layers;
    for (std::size_t j = 0, o = loader_settings.layers.size(); j < o; ++j) {
        const LayerSettings& layer = loader_settings.layers[j];

        QJsonObject json_layer;
        json_layer.insert("name", layer.key.c_str());
        json_layer.insert("path", layer.path.c_str());
        json_layer.insert("control", ::GetToken(layer.control));
        json_layer.insert("treat_as_implicit_manifest", layer.implicit);
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

static LoaderSettings BuildLoaderSettings(const Configuration& configuration) {
    LoaderSettings result;

    Configurator& configurator = Configurator::Get();

    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        LayerSettings layer_settings;

        const Parameter& parameter = configuration.parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        const Layer* layer = FindByKey(configurator.layers.selected_layers, parameter.key.c_str());
        if (layer == nullptr) {
            continue;
        }

        // Extract just the path
        layer_settings.key = parameter.key;
        if (layer != nullptr) {
            layer_settings.path = layer->manifest_path.AbsolutePath();
        }
        layer_settings.control = parameter.control;
        layer_settings.implicit = layer != nullptr ? layer->type == LAYER_TYPE_IMPLICIT : false;

        result.layers.push_back(layer_settings);
    }

    return result;
}

// Create and write vk_loader_settings.json file
bool Configurator::WriteLoaderSettings(const Path& loader_settings_path) {
    assert(!loader_settings_path.Empty());

    std::vector<LoaderSettings> loader_settings_array;

    if (this->environment.GetPerApplicationConfig()) {
        for (std::size_t i = 0, n = this->environment.GetApplications().size(); i < n; ++i) {
            const Application& application = this->environment.GetApplication(i);
            LoaderSettings loader_settings;

            switch (application.layers_mode) {
                case LAYERS_CONTROLLED_BY_APPLICATIONS: {
                    continue;
                } break;
                case LAYERS_CONTROLLED_BY_CONFIGURATOR: {
                    Configuration* configuration = this->configurations.FindConfiguration(application.layers_configuration);
                    loader_settings = BuildLoaderSettings(*configuration);
                } break;
                case LAYERS_DISABLED_BY_CONFIGURATOR: {
                    Configuration disabled_configuration = Configuration::CreateDisabled(this->layers.selected_layers);
                    loader_settings = BuildLoaderSettings(disabled_configuration);
                } break;
            }

            loader_settings.executable_path = application.executable_path.AbsolutePath();
            loader_settings_array.push_back(loader_settings);
        }
    } else {
        Configuration* configuration = this->configurations.FindConfiguration(this->environment.GetSelectedConfiguration());

        if (configuration != nullptr) {
            loader_settings_array.push_back(BuildLoaderSettings(*configuration));
        }
    }

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

// Create and write vk_layer_settings.txt file
bool Configurator::WriteLayersSettings(const Path& settings_path) {
    QFile file(settings_path.AbsolutePath().c_str());
    const bool result_settings_file = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!result_settings_file) {
        fprintf(stderr, "Cannot open file %s\n", settings_path.AbsolutePath().c_str());
        exit(1);
    }
    QTextStream stream(&file);

    bool has_missing_layers = false;

    Configuration* configuration = this->configurations.FindConfiguration(this->environment.GetSelectedConfiguration());

    // Loop through all the layers
    for (std::size_t j = 0, n = configuration->parameters.size(); j < n; ++j) {
        const Parameter& parameter = configuration->parameters[j];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        const Layer* layer = FindByKey(this->layers.selected_layers, parameter.key.c_str());
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

    return result_settings_file && !has_missing_layers;
}

Configurator& Configurator::Get() {
    static Configurator configurator;
    return configurator;
}

Configurator::Configurator() : environment(), layers(environment), configurations(environment) {}

Configurator::~Configurator() {
    configurations.SaveAllConfigurations(layers.selected_layers);

    this->Surrender();
}

bool Configurator::Init() {
    // Load simple app settings, the additional search paths, and the
    // override app list.
    this->layers.LoadAllInstalledLayers();

    if (this->environment.has_crashed) {
        this->environment.has_crashed = false;

        if (Alert::ConfiguratorCrashed() == QMessageBox::No) {
            this->configurations.LoadAllConfigurations(this->layers.selected_layers);
        }
    } else {
        this->configurations.LoadAllConfigurations(this->layers.selected_layers);
    }

    if (this->configurations.Empty()) {
        this->configurations.ResetDefaultsConfigurations(layers.selected_layers);
    } else {
        this->configurations.FirstDefaultsConfigurations(layers.selected_layers);
    }

    this->ActivateConfiguration(this->environment.GetSelectedConfiguration());

    return true;
}

void Configurator::Configure(const std::vector<Layer>& available_layers) {
    const std::string active_configuration_name = this->environment.GetSelectedConfiguration();

    if (active_configuration_name.empty()) {
        this->environment.SetSelectedConfiguration("");
        this->Configure(available_layers, "");
    } else {
        Configuration* selected_configuration = this->configurations.FindConfiguration(active_configuration_name);
        if (selected_configuration == nullptr) {
            environment.SetSelectedConfiguration("");
        }
        this->Configure(available_layers, this->environment.GetSelectedConfiguration());
    }
}

void Configurator::Configure(const std::vector<Layer>& available_layers, const std::string& configuration_name) {
    switch (this->environment.GetMode()) {
        default:
        case LAYERS_CONTROLLED_BY_APPLICATIONS: {
            this->Surrender();
        } break;
        case LAYERS_CONTROLLED_BY_CONFIGURATOR: {
            if (configuration_name.empty()) {
                this->Surrender();
            } else {
                Configuration* selected_configuration = this->configurations.FindConfiguration(configuration_name);
                std::string missing_layer;
                if (::HasMissingLayer(selected_configuration->parameters, available_layers, missing_layer)) {
                    this->Surrender();
                } else {
                    this->Override(*selected_configuration);
                }
            }
        } break;
        case LAYERS_DISABLED_BY_CONFIGURATOR: {
            Configuration temp_configuration;
            temp_configuration.key = "_TempConfiguration";
            temp_configuration.parameters = GatherParameters(temp_configuration.parameters, available_layers);

            for (std::size_t i = 0, n = temp_configuration.parameters.size(); i < n; ++i) {
                temp_configuration.parameters[i].control = LAYER_CONTROL_OFF;
            }

            this->Override(temp_configuration);
        } break;
    }
}

bool Configurator::Override() {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    // Clean up
    this->Surrender();

    // vk_loader_settings.json
    const bool result_layers = this->WriteLoaderSettings(loader_settings_path);

    // vk_layer_settings.txt
    const bool result_settings = this->WriteLayersSettings(layers_settings_path);

    // On Windows only, we need to write these values to the registry
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    AppendRegistryEntriesForLayers(loader_settings_path.AbsolutePath().c_str(), layers_settings_path.AbsolutePath().c_str());
#endif

    return result_settings && result_layers;
}

bool Configurator::Surrender() {
    const Path& loader_settings_path = ::Get(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Get(Path::LAYERS_SETTINGS);

    const bool result_loader_settings = loader_settings_path.Remove();
    const bool result_layers_settings = layers_settings_path.Remove();

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

void Configurator::ActivateConfiguration(const std::string& configuration_name) {
    Configuration* configuration = nullptr;
    if (!configuration_name.empty()) {
        configuration = FindByKey(this->configurations.available_configurations, configuration_name.c_str());
    }

    if (configuration_name.empty()) {
        this->environment.SetSelectedConfiguration("");
        this->Configure(this->layers.selected_layers);
    } else if (configuration == nullptr) {
        QMessageBox alert;
        alert.QDialog::setWindowTitle("Vulkan layers configuration is missing...");
        const std::string text = format("%s couldn't find '%s' layers configuration.", VKCONFIG_NAME, configuration_name.c_str());
        alert.setText(text.c_str());
        alert.setInformativeText("Vulkan Configurator is switching to Layers Controlled by Vulkan Application mode");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        this->environment.SetSelectedConfiguration("");
        this->environment.SetMode(LAYERS_CONTROLLED_BY_APPLICATIONS);
        this->Configure(this->layers.selected_layers);
    } else {
        // If the layers paths are differents, we need to reload the layers and the configurations
        const std::vector<Path>& paths = this->environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
        if (configuration->user_defined_paths != paths) {
            this->configurations.SaveAllConfigurations(this->layers.selected_layers);
            this->environment.SetPerConfigUserDefinedLayersPaths(configuration->user_defined_paths);
            this->layers.LoadAllInstalledLayers();
            this->configurations.LoadAllConfigurations(this->layers.selected_layers);
        }

        std::string missing_layer;
        if (::HasMissingLayer(configuration->parameters, layers.selected_layers, missing_layer)) {
            QMessageBox alert;
            alert.QDialog::setWindowTitle("Vulkan layer missing...");
            alert.setText(format("%s couldn't find '%s' layer required by '%s' configuration:", VKCONFIG_NAME,
                                 missing_layer.c_str(), configuration->key.c_str())
                              .c_str());
            alert.setIcon(QMessageBox::Critical);
            alert.exec();
        } else {
            this->Configure(this->layers.selected_layers);
        }

        this->environment.SetSelectedConfiguration(configuration_name.c_str());
    }
}

void Configurator::ResetToDefault(bool hard) {
    if (hard) {
        this->environment.Reset(Environment::CLEAR);
        this->layers.LoadAllInstalledLayers();
        this->configurations.ResetDefaultsConfigurations(this->layers.selected_layers);

        this->ActivateConfiguration(this->environment.GetSelectedConfiguration());
    } else {
        this->configurations.ReloadDefaultsConfigurations(this->layers.selected_layers);
    }
}
