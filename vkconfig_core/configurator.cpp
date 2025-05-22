/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include "date.h"

#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonArray>
#include <QSettings>
#include <QApplication>
#include <QProcess>
#include <QLibraryInfo>
#include <QSysInfo>
#include <QOperatingSystemVersion>

#include <cassert>
#include <cstdio>
#include <algorithm>
#include <thread>

const char* MAINWINDOW_GEOMETRY = "vkconfig/mainwindow_geometry";
const char* MAINWINDOW_STATE = "vkconfig/mainwindow_state";

ConfiguratorGuard::ConfiguratorGuard(ConfiguratorMode mode) {
    Configurator& configurator = Configurator::Get();
    configurator.Init(mode);
}

ConfiguratorGuard::~ConfiguratorGuard() { Configurator::Release(); }

Configurator& ConfiguratorGuard::Get() { return Configurator::Get(); }

static std::unique_ptr<Configurator> configurator = nullptr;

Configurator& Configurator::Get() {
    if (::configurator == nullptr) {
        ::configurator.reset(new Configurator);
    }

    return *::configurator;
}

void Configurator::Release() { ::configurator.reset(); }

Configurator::Configurator() : mode(init_mode) {}

Configurator::~Configurator() {
    if (this->reset_hard) {
        return;
    }

    this->has_crashed = false;
    this->Save();
}

bool Configurator::Init(ConfiguratorMode configurator_mode) {
    this->init_mode = configurator_mode;

    const bool result = this->Load();

    if (result && this->has_crashed) {
        switch (this->mode) {
            default:
            case CONFIGURATOR_MODE_NONE: {
            } break;
            case CONFIGURATOR_MODE_GUI: {
                QMessageBox alert;
                alert.setWindowTitle(format("%s crashed during last run...", VKCONFIG_NAME).c_str());
                alert.setText("Do you want to reset to default resolve the issue?");
                alert.setInformativeText("All layers configurations will be lost...");
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setDefaultButton(QMessageBox::No);
                alert.setIcon(QMessageBox::Critical);
                int result = alert.exec();

                if (result == QMessageBox::Yes) {
                    this->Reset(true);
                    return false;
                }
            } break;
            case CONFIGURATOR_MODE_CMD: {
                fprintf(stderr, "%s: [ERROR] crashed during last run...\n", VKCONFIG_SHORT_NAME);
                fprintf(stderr, "\n  (Run \"%s reset --hard\" to reset to default %s if the problem continue)\n",
                        VKCONFIG_SHORT_NAME, VKCONFIG_NAME);
            } break;
        }
    }

    this->has_crashed = true;  // This is set to `false` when saving on exit.
    this->Save();

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
                                       std::vector<LoaderSettings>& loader_settings_array, bool full_loader_log) const {
    if (configuration_key.empty()) {
        return;
    }

    const Configuration* configuration = this->configurations.FindConfiguration(configuration_key);
    if (configuration == nullptr) {
        return;
    }

    LoaderSettings result;
    result.executable_path = executable_path;
    result.override_loader = configuration->override_loader;
    result.override_layers = configuration->override_layers;
    result.stderr_log_flags = full_loader_log ? ~0 : configuration->loader_log_messages_flags;

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
                this->BuildLoaderSettings(configuration, "", loader_settings_array, this->force_full_loader_log);
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

                    this->BuildLoaderSettings(configuration, executables[i].path.AbsolutePath(), loader_settings_array,
                                              this->force_full_loader_log);
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

bool Configurator::Export(const Path& export_path) const {
    QFile file(export_path.AbsolutePath().c_str());

    const bool result_layers_file = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!result_layers_file) {
        return false;
    }

    bool has_missing_layers = false;
    const Configuration* configuration = this->GetActiveConfiguration();

    QTextStream stream(&file);

    stream << "#! Loader Settings:\n";

    const std::vector<std::string>& stderr_log = GetLogTokens(configuration->loader_log_messages_flags);
    const std::string stderr_logs = Merge(stderr_log, ",");

    if (configuration->override_loader) {
        stream << "export VK_LOADER_DEBUG=" << stderr_logs.c_str() << "\n";
    }
    if (configuration->override_layers) {
        stream << "#! For now, the Vulkan Loader doesn't fully support the same behavior with environment variables than what's "
                  "supported with Vulkan Configurator...\n";
        stream << "#! The Vulkan Loader doesn't support fully ordering layers with environment variables.\n";
        stream << "export VK_LOADER_LAYERS_ENABLE=";

        std::vector<std::string> layer_list;
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration->parameters[i];
            if (parameter.control != LAYER_CONTROL_ON) {
                continue;
            }
            layer_list.push_back(parameter.key);
        }
        stream << Merge(layer_list, ",").c_str();
        stream << "\n";
    }

    stream << "\n";

    stream << "#! Layers Settings:\n";

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
            if (parameter.control == LAYER_CONTROL_ON) {
                has_missing_layers = true;
                fprintf(stderr,
                        "vkconfig: [ERROR] `%s` layer is set to `%s` in `%s` loader configuration but missing and being "
                        "ignored\n",
                        parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str());
            } else {
                fprintf(stderr,
                        "vkconfig: [WARNING] `%s` layer is set to `%s` in `%s` loader configuration but missing and being "
                        "ignored\n",
                        parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str());
            }
            continue;
        }

        stream << "\n";
        stream << "#! " << layer->key.c_str() << " " << layer->api_version.str().c_str() << "\n\n";

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

            stream << "#! ";
            stream << meta->label.c_str();
            stream << "\n#! =====================\n";
            stream << "#! " << meta->key.c_str();

            if (meta->status != STATUS_STABLE) {
                stream << format(" (%s)", GetToken(meta->status)).c_str();
            }

            stream << "\n";

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
                stream << "#!";
                std::size_t nchars = 2;
                for (auto word : words) {
                    if (word.size() + nchars > 80) {
                        stream << "\n#!";
                        nchars = 3;
                    }
                    stream << " " << word.c_str();
                    nchars += (word.size() + 1);
                }
            }
            stream << "\n";

            // If feature has unmet dependency, output it but comment it out
            if (::CheckDependence(*meta, parameter.settings) != SETTING_DEPENDENCE_ENABLE) {
                stream << "#!";
            }

            if (meta->status == STATUS_DEPRECATED && !meta->deprecated_by_key.empty()) {
                const SettingMeta* replaced_setting = FindSetting(layer->settings, meta->deprecated_by_key.c_str());

                stream << format("#! This setting was deprecated and replaced by '%s' (%s) setting.\n",
                                 replaced_setting->label.c_str(), replaced_setting->key.c_str())
                              .c_str();
            }

            std::vector<std::string> data = ::BuildEnvVariablesList(layer->key.c_str(), setting_data->key.c_str(), false);

            stream << "export " << data[0].c_str() << "=";
            stream << setting_data->Export(EXPORT_MODE_OVERRIDE).c_str();
            stream << "\n\n";
        }
    }

    file.close();

    return true;
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
                    fprintf(stderr, "vkconfig: [ERROR] Fail to apply unknown '%s' global configuration\n",
                            layers_settings.configuration_name.c_str());
                } else {
                    fprintf(stderr, "vkconfig: [ERROR] Fail to apply unknown '%s' configuration to '%s'\n",
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
                fprintf(stderr, "vkconfig: [ERROR] Cannot open file:\t%s\n", layers_settings.settings_path.AbsolutePath().c_str());
                continue;
            }
            QTextStream stream(&file);

            if (configuration->override_settings) {
                QFile original_file(configuration->override_settings_path.AbsolutePath().c_str());
                bool result_original_file = original_file.open(QIODevice::ReadOnly);
                if (!result_settings_file) {
                    fprintf(stderr, "vkconfig: [ERROR] Cannot open override settings file:\t%s\n",
                            configuration->override_settings_path.AbsolutePath().c_str());
                    continue;
                }

                QTextStream instream(&original_file);
                while (!instream.atEnd()) {
                    QString line = instream.readLine();
                    stream << line << '\n';
                }
            } else {
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
                        if (parameter.control == LAYER_CONTROL_ON) {
                            has_missing_layers = true;
                            fprintf(
                                stderr,
                                "vkconfig: [ERROR] `%s` layer is set to `%s` in `%s` loader configuration but missing and being "
                                "ignored\n",
                                parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str());
                        } else {
                            fprintf(
                                stderr,
                                "vkconfig: [WARNING] `%s` layer is set to `%s` in `%s` loader configuration but missing and being "
                                "ignored\n",
                                parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str());
                        }
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
                        stream << meta->key.c_str();

                        if (meta->status != STATUS_STABLE) {
                            stream << format(" (%s)", GetToken(meta->status)).c_str();
                        }

                        stream << "\n";

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

                        if (meta->status == STATUS_DEPRECATED && !meta->deprecated_by_key.empty()) {
                            const SettingMeta* replaced_setting = FindSetting(layer->settings, meta->deprecated_by_key.c_str());

                            stream << format("# This setting was deprecated and replaced by '%s' (%s) setting.\n",
                                             replaced_setting->label.c_str(), replaced_setting->key.c_str())
                                          .c_str();
                        }

                        stream << lc_layer_name.c_str() << setting_data->key.c_str() << " = ";
                        stream << setting_data->Export(EXPORT_MODE_OVERRIDE).c_str();
                        stream << "\n\n";
                    }
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
    const Path& loader_settings_path = ::Path(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Path(Path::LAYERS_SETTINGS);

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
    const Path& loader_settings_path = ::Path(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Path(Path::LAYERS_SETTINGS);

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
            Path path(executables[i].GetActiveOptions()->working_folder.RelativePath() + "/vk_layer_settings.txt");
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
    const Path& loader_settings_path = ::Path(Path::LOADER_SETTINGS);
    const Path& layers_settings_path = ::Path(Path::LAYERS_SETTINGS);

    return loader_settings_path.Exists() || layers_settings_path.Exists();
}

void Configurator::Reset(bool hard) {
    this->Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    const Path& vkconfig_init_path = ::Path(Path::INIT);
    vkconfig_init_path.Remove();

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    const QStringList& keys = settings.allKeys();
    for (int i = 0, n = keys.size(); i < n; ++i) {
        settings.remove(keys[i]);
    }

    if (hard) {
        this->configurations.RemoveConfigurationFiles();
    }

    this->use_system_tray = false;  // This prevents Vulkan Configurator to abort the restart
    this->reset_hard = true;
}

void Configurator::SetActiveConfigurationName(const std::string& configuration_name) {
    if (this->executable_scope == EXECUTABLE_PER) {
        Executable* executable = this->executables.GetActiveExecutable();
        if (executable != nullptr) {
            executable->configuration = configuration_name;
        }
    } else {
        this->selected_global_configuration = configuration_name;
    }
}

void Configurator::UpdateConfigurations() {
    this->configurations.UpdateConfigurations(this->layers);
    return;
}

Configuration* Configurator::GetActiveConfiguration() {
    if (this->executable_scope == EXECUTABLE_PER) {
        const Executable* executable = this->executables.GetActiveExecutable();
        if (executable != nullptr) {
            return this->configurations.FindConfiguration(executable->configuration);
        } else {
            return nullptr;
        }
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
                const Layer* layer = this->layers.Find(parameter.key, parameter.api_version);
                if (layer == nullptr) {
                    log += format("   * %s - Missing: %s\n", parameter.key.c_str(), ::GetToken(parameter.control));
                } else {
                    log += format("   * %s - %s (%s layer): %s\n", parameter.key.c_str(), layer->api_version.str().c_str(),
                                  ::GetToken(layer->type), ::GetToken(parameter.control));
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
    log += format(" - Vulkan API version: %s\n", Version::VKHEADER.str().c_str());
    log += format(" - Build: %s %s\n", GetLabel(VKC_PLATFORM), build.c_str());
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    log += format(" - Qt version: %s %s\n", QLibraryInfo::version().toString().toStdString().c_str(),
                  QLibraryInfo::isDebugBuild() ? "Debug" : "Release");
#else
    log += format(" - Qt version: %s %s-%s\n", QLibraryInfo::version().toString().toStdString().c_str(),
                  QLibraryInfo::isSharedBuild() ? "Shared" : "Static", QLibraryInfo::isDebugBuild() ? "Debug" : "Release");
#endif

    if (Path(Path::SDK).Empty()) {
        log += " - ${VULKAN_SDK}: unset\n";
    } else {
        log += format(" - ${VULKAN_SDK}: %s\n", Path(Path::SDK).AbsolutePath().c_str());
    }
    log += "\n";

    log += "System Information:\n";
#if VKC_PLATFORM == PLATFORM_LINUX || QT_VERSION < QT_VERSION_CHECK(6, 1, 0)
    log += format(" - %s\n", QSysInfo::prettyProductName().toStdString().c_str());
#else
    const QOperatingSystemVersion& current = QOperatingSystemVersion::current();
    log += format(" - %s %s\n", current.name().toStdString().c_str(), current.version().toString().toStdString().c_str());
#endif

    log += format(" - CPU architechture: %s\n", QSysInfo::currentCpuArchitecture().toStdString().c_str());
    log += format(" - Logical CPU core count: %d\n", std::thread::hardware_concurrency());
    log += "\n";

    log += format("%s Settings:\n", VKCONFIG_NAME);
    log += format(" - Vulkan Loader configuration scope: %s\n", ::GetLabel(this->GetExecutableScope()));
    log += format("   * Loader settings: %s\n", Path(Path::LOADER_SETTINGS).AbsolutePath().c_str());

    if (this->GetExecutableScope() == EXECUTABLE_ANY || this->GetExecutableScope() == EXECUTABLE_ALL) {
        log += format("   * Layers settings: %s\n", Path(Path::LAYERS_SETTINGS).AbsolutePath().c_str());
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
    log += format(" - Use layer developer mode: %s\n", this->use_layer_debug_mode ? "true" : "false");
    log += format(" - ${VULKAN_BIN}: %s\n", ::Path(Path::BIN).AbsolutePath().c_str());
    log += format(" - ${VULKAN_PROFILES}: %s\n", ::Path(Path::PROFILES).AbsolutePath().c_str());
    log += format(" - ${VULKAN_HOME}: %s\n", ::Path(Path::HOME).AbsolutePath().c_str());
    log += format(" - ${VULKAN_DOWNLOAD}: %s\n", ::Path(Path::DOWNLOAD).AbsolutePath().c_str());
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
    } else {
        log += "   * ${VK_INSTANCE_LAYERS}: unset\n";
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

bool Configurator::Load() {
    const Path& vkconfig_init_path = ::Path(Path::INIT);

    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    const bool has_init_file = file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (has_init_file) {
        QString init_data = file.readAll();
        file.close();

        QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
        this->window_geometry = settings.value(MAINWINDOW_GEOMETRY).toByteArray();
        this->window_state = settings.value(MAINWINDOW_STATE).toByteArray();

        const QJsonDocument& json_doc = QJsonDocument::fromJson(init_data.toLocal8Bit());
        const QJsonObject& json_root_object = json_doc.object();

        const Version file_format_version = Version(json_root_object.value("file_format_version").toString().toStdString());
        if (file_format_version > Version::VKCONFIG) {
            switch (this->mode) {
                default:
                case CONFIGURATOR_MODE_NONE:
                    break;
                case CONFIGURATOR_MODE_GUI: {
                    QMessageBox alert;
                    alert.setWindowTitle(format("Launching an older version of %s...", VKCONFIG_NAME).c_str());
                    alert.setText(format("Running a Vulkan Configurator %s but a newer %s version was previously launched.",
                                         Version::VKCONFIG.str().c_str(), file_format_version.str().c_str())
                                      .c_str());
                    alert.setInformativeText("Do you want to continue? This may cause crashes...");
                    alert.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
                    alert.setDefaultButton(QMessageBox::Cancel);
                    alert.setIcon(QMessageBox::Critical);
                    if (alert.exec() == QMessageBox::Cancel) {
                        return false;  // Vulkan Configurator is reset to default
                    }
                } break;
                case CONFIGURATOR_MODE_CMD: {
                    fprintf(stderr, "vkconfig: [WARNING] Launching an older version %s of %s...\n", Version::VKCONFIG.str().c_str(),
                            VKCONFIG_NAME);
                    fprintf(stderr, "\n  (Run \"%s reset --hard\" to reset to default %s if the problem continue)\n",
                            VKCONFIG_SHORT_NAME, VKCONFIG_NAME);
                } break;
            }
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

        const QJsonObject& json_hide_layer_message_boxes_object =
            json_interface_object.value("hide_layer_message_boxes").toObject();
        this->ignored_messages.clear();
        const QStringList json_hide_layer_message_boxes_keys = json_hide_layer_message_boxes_object.keys();
        for (int i = 0, n = json_hide_layer_message_boxes_keys.size(); i < n; ++i) {
            const std::string& key = json_hide_layer_message_boxes_keys[i].toStdString();
            const int version = json_hide_layer_message_boxes_object.value(key.c_str()).toInt();
            this->ignored_messages.insert(std::make_pair(key, version));
        }

        this->active_tab = GetTabType(json_interface_object.value("active_tab").toString().toStdString().c_str());

        // TAB_CONFIGURATIONS
        if (json_interface_object.value(GetToken(TAB_CONFIGURATIONS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_CONFIGURATIONS)).toObject();
            this->advanced = json_object.value("advanced").toBool();
            this->executable_scope = ::GetExecutableScope(json_object.value("executable_scope").toString().toStdString().c_str());
            this->selected_global_configuration = json_object.value("selected_global_configuration").toString().toStdString();
        }

        // TAB_LAYERS
        if (json_interface_object.value(GetToken(TAB_LAYERS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_LAYERS)).toObject();
            (void)json_object;
        }

        // TAB_APPLICATIONS
        if (json_interface_object.value(GetToken(TAB_APPLICATIONS)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_APPLICATIONS)).toObject();
            (void)json_object;
        }

        // TAB_PREFERENCES
        if (json_interface_object.value(GetToken(TAB_PREFERENCES)) != QJsonValue::Undefined) {
            const QJsonObject& json_object = json_interface_object.value(GetToken(TAB_PREFERENCES)).toObject();

            if (json_object.value("use_layer_debug_mode") != QJsonValue::Undefined) {
                this->use_layer_debug_mode = json_object.value("use_layer_debug_mode").toBool();
            }

            if (json_object.value("use_notify_releases") != QJsonValue::Undefined) {
                this->use_notify_releases = json_object.value("use_notify_releases").toBool();
            }

            if (json_object.value("latest_sdk_version") != QJsonValue::Undefined) {
                this->latest_sdk_version = Version(json_object.value("latest_sdk_version").toString().toStdString().c_str());
            }

            if (json_object.value("last_vkconfig_version") != QJsonValue::Undefined) {
                this->last_vkconfig_version = Version(json_object.value("last_vkconfig_version").toString().toStdString().c_str());
            }

            if (json_object.value("use_system_tray") != QJsonValue::Undefined) {
                this->use_system_tray = json_object.value("use_system_tray").toBool();
            }

            if (json_object.value("show_external_layers_settings") != QJsonValue::Undefined) {
                this->show_external_layers_settings = json_object.value("show_external_layers_settings").toBool();
            }

            if (json_object.value("VULKAN_HOME") != QJsonValue::Undefined) {
                ::SetHomePath(json_object.value("VULKAN_HOME").toString().toStdString());
            }
            if (json_object.value("VULKAN_DOWNLOAD") != QJsonValue::Undefined) {
                ::SetDownloadPath(json_object.value("VULKAN_DOWNLOAD").toString().toStdString());
            }
        }

        this->executables.Load(json_root_object, this->mode);
        this->layers.Load(json_root_object, this->mode);
        this->configurations.Load(json_root_object, this->mode);
    } else {
        this->executables.Reset();
        this->layers.LoadAllInstalledLayers(this->mode);
    }

    this->configurations.LoadAllConfigurations(this->layers);

    return has_init_file;
}

bool Configurator::Save() const {
    if (!this->window_geometry.isEmpty()) {
        QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
        settings.setValue(MAINWINDOW_GEOMETRY, this->window_geometry);
        settings.setValue(MAINWINDOW_STATE, this->window_state);
    }

    QJsonObject json_root_object;
    json_root_object.insert("file_format_version", Version::VKCONFIG.str().c_str());

    QJsonObject json_interface_object;

    // TAB_CONFIGURATIONS
    {
        QJsonObject json_object;
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

    // TAB_PREFERENCES
    {
        QJsonObject json_object;
        json_object.insert("use_system_tray", this->use_system_tray);
        json_object.insert("use_layer_debug_mode", this->use_layer_debug_mode);
        json_object.insert("use_notify_releases", this->use_notify_releases);
        json_object.insert("latest_sdk_version", this->latest_sdk_version.str().c_str());
        json_object.insert("last_vkconfig_version", Version::VKCONFIG.str().c_str());
        json_object.insert("show_external_layers_settings", this->show_external_layers_settings);
        json_object.insert("VULKAN_HOME", ::Path(Path::HOME).RelativePath().c_str());
        json_object.insert("VULKAN_DOWNLOAD", ::Path(Path::DOWNLOAD).RelativePath().c_str());
        json_interface_object.insert(GetToken(TAB_PREFERENCES), json_object);
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

        QJsonObject json_hide_layer_message_boxes_object;
        for (auto it = this->ignored_messages.begin(); it != this->ignored_messages.end(); ++it) {
            json_hide_layer_message_boxes_object.insert(it->first.c_str(), it->second);
        }
        json_interface_object.insert("hide_layer_message_boxes", json_hide_layer_message_boxes_object);

        json_interface_object.insert("active_tab", GetToken(this->active_tab));

        json_root_object.insert("interface", json_interface_object);
    }

    this->configurations.Save(json_root_object);
    this->layers.Save(json_root_object);
    this->executables.Save(json_root_object);

    QJsonDocument json_doc(json_root_object);

    const Path& vkconfig_init_path = ::Path(Path::INIT);
    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    const bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);

    file.write(json_doc.toJson());
    file.close();

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

bool Configurator::GetUseLayerDebugMode() const { return this->use_layer_debug_mode; }

void Configurator::SetUseLayerDebugMode(bool enabled) { this->use_layer_debug_mode = enabled; }

bool Configurator::GetUseNotifyReleases() const { return this->use_notify_releases; }

void Configurator::SetUseNotifyReleases(bool enabled) { this->use_notify_releases = enabled; }

bool Configurator::GetShowExternalLayersSettings() const { return this->show_external_layers_settings; }

void Configurator::SetShowExternalLayersSettings(bool enabled) { this->show_external_layers_settings = enabled; }

bool Configurator::IsExternalLayersSettingsUsed(bool icon_mode) const {
    const Configuration* configuration = this->GetActiveConfiguration();
    if (configuration == nullptr) {
        return this->GetShowExternalLayersSettings();
    }

    const bool in_use = configuration->override_settings || !configuration->override_settings_path.Empty();

    if (icon_mode) {
        return in_use;
    } else {
        return in_use || this->GetShowExternalLayersSettings();
    }
}

bool Configurator::ShouldNotify() const {
    // Notify if
    return this->latest_sdk_version < this->online_sdk_version  // There is an online SDK version newer than the latest SDK version
           && this->online_sdk_version != Version::NONE;        // We could query the online SDK version
    //        && Version::VKCONFIG < this->last_vkconfig_version // The Vulkan Configurator version
    //&& Version::VKHEADER < this->online_sdk_version;  // The Vulkan Header version used to build Vulkan Configurator is older
    // than the online version
}

bool Configurator::HasActiveSettings() const {
    switch (this->executable_scope) {
        default:
        case EXECUTABLE_NONE:
            return false;
        case EXECUTABLE_ALL:
        case EXECUTABLE_PER:
        case EXECUTABLE_ANY: {
            if (::EnabledExecutables(this->executable_scope)) {
                if (this->executables.GetActiveExecutable() == nullptr) {
                    return false;
                }
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
            }
            return false;
        }
    }
}

bool Configurator::HasEnabledUI(EnabledUI enabled_ui) const {
    switch (enabled_ui) {
        default:
            assert(false);
            return false;
        case ENABLE_UI_CONFIG: {
            switch (this->GetExecutableScope()) {
                default:
                case EXECUTABLE_NONE:
                    return false;
                case EXECUTABLE_ALL:
                case EXECUTABLE_PER:
                    return this->executables.GetActiveExecutable() != nullptr;
                case EXECUTABLE_ANY:
                    return true;
            }
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

    return log;
}
