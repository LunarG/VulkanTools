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
    const bool has_init_file = this->mode == GUI ? file.open(QIODevice::ReadOnly | QIODevice::Text) : false;

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
            const Layer* layer = this->layers.Find(parameter.key, parameter.api_version);
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

    if (!this->HasActiveConfiguration()) {
        return false;
    }

    if (override_area & OVERRIDE_AREA_LOADER_SETTINGS_BIT) {
        std::vector<LoaderSettings> loader_settings_array;

        std::string configuration = this->selected_global_configuration;

        switch (this->executable_scope) {
            case EXECUTABLE_ANY:
                this->BuildLoaderSettings(configuration, "", loader_settings_array);
                break;
            case EXECUTABLE_ALL:
            case EXECUTABLE_PER: {
                const std::vector<Executable>& collection = this->executables.GetExecutables();
                for (std::size_t i = 0, n = collection.size(); i < n; ++i) {
                    if (this->executable_scope == EXECUTABLE_PER) {
                        configuration = collection[i].GetActiveOptions()->configuration;
                    }

                    this->BuildLoaderSettings(configuration, collection[i].path.AbsolutePath(), loader_settings_array);
                }
                break;
            }
            default:
            case EXECUTABLE_NONE:
                break;
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
    if (!this->HasActiveConfiguration()) {
        return false;
    }

    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        std::vector<LayersSettings> layers_settings_array;

        switch (this->executable_scope) {
            case EXECUTABLE_ANY: {
                LayersSettings settings;
                settings.configuration_name = this->selected_global_configuration;
                settings.settings_path = layers_settings_path;
                layers_settings_array.push_back(settings);
                break;
            }
            case EXECUTABLE_ALL:
            case EXECUTABLE_PER: {
                const std::vector<Executable>& executables = this->executables.GetExecutables();

                std::string configuration_name = this->selected_global_configuration;

                for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                    if (this->executable_scope == EXECUTABLE_PER) {
                        configuration_name = executables[i].GetActiveOptions()->configuration;
                    }

                    LayersSettings settings;
                    settings.executable_path = executables[i].path;
                    settings.configuration_name = configuration_name;
                    settings.settings_path = executables[i].GetActiveOptions()->working_folder;
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

    // TODO, Add per application layer_settings file
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

    // TODO, Remove per application layer_settings file
    bool result_layers_settings = true;
    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        result_layers_settings = layers_settings_path.Remove();
    }

#if VKC_ENV == VKC_ENV_WIN32
    RemoveRegistryEntriesForLayers(loader_settings_path.AbsolutePath().c_str(), layers_settings_path.AbsolutePath().c_str());
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
        executable->GetActiveOptions()->configuration = configuration_name;
    } else {
        this->selected_global_configuration = configuration_name;
    }
}

std::string Configurator::GetActionConfigurationName() const {
    if (this->executable_scope == EXECUTABLE_PER) {
        const Executable* executable = this->executables.GetActiveExecutable();
        return executable->GetActiveOptions()->configuration;
    } else {
        return this->selected_global_configuration;
    }
}

Configuration* Configurator::GetActiveConfiguration() {
    if (this->executable_scope == EXECUTABLE_PER) {
        const Executable* executable = this->executables.GetActiveExecutable();
        return this->configurations.FindConfiguration(executable->GetActiveOptions()->configuration);
    } else {
        return this->configurations.FindConfiguration(this->selected_global_configuration);
    }
}

const Configuration* Configurator::GetActiveConfiguration() const {
    if (this->executable_scope == EXECUTABLE_PER) {
        const Executable* executable = this->executables.GetActiveExecutable();
        return this->configurations.FindConfiguration(executable->GetActiveOptions()->configuration);
    } else {
        return this->configurations.FindConfiguration(this->selected_global_configuration);
    }
}

bool Configurator::HasActiveConfiguration() const {
    switch (this->executable_scope) {
        default:
        case EXECUTABLE_NONE:
            return false;
        case EXECUTABLE_ANY:
            return this->configurations.FindConfiguration(this->selected_global_configuration) != nullptr;
        case EXECUTABLE_ALL: {
            const Configuration* configuration = this->configurations.FindConfiguration(this->selected_global_configuration);
            const std::vector<Executable>& data = this->executables.GetExecutables();
            for (std::size_t i = 0, n = data.size(); i < n; ++i) {
                if (data[i].enabled) {
                    return true;
                }
            }
            return false;
        } break;
        case EXECUTABLE_PER: {
            const std::vector<Executable>& data = this->executables.GetExecutables();
            for (std::size_t i = 0, n = data.size(); i < n; ++i) {
                if (data[i].enabled) {
                    const ExecutableOptions* options = data[i].GetActiveOptions();
                    if (options != nullptr) {
                        return this->configurations.FindConfiguration(options->configuration) != nullptr;
                    } else {
                        return false;
                    }
                }
            }
            return false;
        }
    }
}

Parameter* Configurator::GetActiveParameter() {
    Configuration* configuration = this->GetActiveConfiguration();
    if (configuration != nullptr) {
        return configuration->GetActiveParameter();
    } else {
        return nullptr;
    }
}

const Parameter* Configurator::GetActiveParameter() const {
    const Configuration* configuration = this->GetActiveConfiguration();
    if (configuration != nullptr) {
        return configuration->GetActiveParameter();
    } else {
        return nullptr;
    }
}

bool Configurator::HasActiveParameter() const { return this->GetActiveParameter() != nullptr; }

Executable* Configurator::GetActiveExecutable() { return this->executables.GetActiveExecutable(); }

const Executable* Configurator::GetActiveExecutable() const { return this->executables.GetActiveExecutable(); }

void Configurator::Reset() {
    this->has_crashed = false;
    this->use_system_tray = false;
    this->executable_scope = EXECUTABLE_ANY;
    this->selected_global_configuration = "Validation";
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

        this->home_sdk_path = json_object.value("VK_HOME").toString().toStdString();
        if (this->home_sdk_path.Empty()) {
            this->home_sdk_path = ::Get(Path::HOME);
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
        json_object.insert("VK_HOME", this->home_sdk_path.RelativePath().c_str());
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
