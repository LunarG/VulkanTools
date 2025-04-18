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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "configuration.h"
#include "layer_manager.h"
#include "util.h"
#include "path.h"
#include "json.h"
#include "type_platform.h"
#include "type_log.h"
#include "version.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QMessageBox>

#include <cassert>
#include <cstdio>
#include <string>
#include <algorithm>

static void AddApplicationEnabledParameters(std::vector<Parameter>& parameters, LayerControl default_control) {
    for (auto paramater : parameters) {
        if (paramater.builtin != LAYER_BUILTIN_UNORDERED) {
            continue;
        }

        return;  // No need to add LAYER_BUILTIN_UNORDERED, it's in the list already
    }

    Parameter applications_enabled_layers;
    applications_enabled_layers.key = ::GetLabel(LAYER_BUILTIN_UNORDERED);
    applications_enabled_layers.builtin = LAYER_BUILTIN_UNORDERED;
    applications_enabled_layers.control = ::MapToUnordered(default_control);
    applications_enabled_layers.overridden_rank = Parameter::NO_RANK;
    parameters.push_back(applications_enabled_layers);
}

Configuration Configuration::Create(const LayerManager& layers, const std::string& configuration_key) {
    Configuration result;

    result.key = configuration_key;
    result.GatherParameters(layers);

    return result;
}

Configuration Configuration::CreateDisabled(const LayerManager& layers) {
    Configuration result;
    result.key = "_DisablingConfiguration";
    result.GatherParameters(layers);

    for (std::size_t i = 0, n = result.parameters.size(); i < n; ++i) {
        result.parameters[i].control = LAYER_CONTROL_OFF;
    }

    return result;
}

bool Configuration::Load(const Path& full_path, const LayerManager& layers) {
    assert(!full_path.Empty());

    this->parameters.clear();

    QFile file(full_path.AbsolutePath().c_str());
    const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    std::string json_text = file.readAll().toStdString();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_text.c_str(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
        return false;
    }

    Version version(json_doc.object().value("file_format_version").toString().toStdString());
    if (version < (Version(3, 0, 0))) {
        return false;  // Unsupported version
    }

    const QJsonObject& json_root_object = json_doc.object();
    const QJsonValue& json_configuration_value = json_root_object.value("configuration");
    if (json_configuration_value == QJsonValue::Undefined) {
        return false;  // Not a configuration file
    }

    const QJsonObject& json_configuration_object = json_configuration_value.toObject();

    // Required configuration values
    this->key = ReadString(json_configuration_object, "name").c_str();

    // Optional configuration values
    if (json_configuration_object.value("version") != QJsonValue::Undefined) {
        this->version = json_configuration_object.value("version").toVariant().toInt();
    }

    if (json_configuration_object.value("platforms") != QJsonValue::Undefined) {
        this->platform_flags = GetPlatformFlags(ReadStringArray(json_configuration_object, "platforms"));
    }

    if (json_configuration_object.value("default_layer_control") != QJsonValue::Undefined) {
        this->default_control = ::GetLayerControl(ReadString(json_configuration_object, "default_layer_control").c_str());
    }

    if (json_configuration_object.value("override_layers") != QJsonValue::Undefined) {
        this->override_layers = ReadBoolValue(json_configuration_object, "override_layers");
    }

    if (json_configuration_object.value("override_loader") != QJsonValue::Undefined) {
        this->override_loader = ReadBoolValue(json_configuration_object, "override_loader");
    }

    if (json_configuration_object.value("loader_message_types") != QJsonValue::Undefined) {
        const std::vector<std::string>& loader_messsage_types = ReadStringArray(json_configuration_object, "loader_message_types");
        this->loader_log_messages_flags = GetLogFlags(loader_messsage_types);
    }

    if (json_configuration_object.value("selected_layer_name") != QJsonValue::Undefined) {
        this->selected_layer_name = ReadString(json_configuration_object, "selected_layer_name");
    }

    // Required configuration layers values
    if (json_configuration_object.value("layers") != QJsonValue::Undefined) {
        const QJsonArray& json_layers_array = ReadArray(json_configuration_object, "layers");
        for (int layer_index = 0, layer_count = json_layers_array.size(); layer_index < layer_count; ++layer_index) {
            const QJsonObject& json_layer_object = json_layers_array[layer_index].toObject();

            Parameter parameter;
            if (json_layer_object.value("name") != QJsonValue::Undefined) {
                parameter.key = json_layer_object.value("name").toString().toStdString();
            }
            if (json_layer_object.value("builtin") != QJsonValue::Undefined) {
                parameter.builtin = ::GetLayerBuiltin(json_layer_object.value("builtin").toString().toStdString().c_str());
                parameter.key = ::GetLabel(parameter.builtin);
            }
            if (json_layer_object.value("control") != QJsonValue::Undefined) {
                parameter.control = ::GetLayerControl(json_layer_object.value("control").toString().toStdString().c_str());
            }
            if (json_layer_object.value("rank") != QJsonValue::Undefined) {
                parameter.overridden_rank = json_layer_object.value("rank").toInt();
            }
            if (json_layer_object.value("version") != QJsonValue::Undefined) {
                const std::string& version = ReadStringValue(json_layer_object, "version");
                parameter.api_version = version == "latest" ? Version::LATEST : Version(version.c_str());
            }
            if (json_layer_object.value("type") != QJsonValue::Undefined) {
                parameter.type = GetLayerType(json_layer_object.value("type").toString().toStdString().c_str());
            }
            if (json_layer_object.value("override_settings") != QJsonValue::Undefined) {
                parameter.override_settings = json_layer_object.value("override_settings").toBool();
            }

            const Layer* layer = layers.Find(parameter.key, parameter.api_version);

            if (layer != nullptr) {
                parameter.manifest = layer->manifest_path;
                parameter.type = layer->type;
            }

            if (parameter.api_version != Version::LATEST) {
                if (json_layer_object.value("manifest") != QJsonValue::Undefined) {
                    std::string manifest_path = ReadString(json_layer_object, "manifest");

                    const Layer* layer = layers.FindFromManifest(manifest_path);
                    if (layer != nullptr) {
                        parameter.manifest = manifest_path;
                        parameter.api_version = layer->api_version;
                    }
                }
            } else if (layer != nullptr) {
                parameter.manifest = layer->manifest_path;
            }

            const QJsonValue& json_platform_value = json_layer_object.value("platforms");
            if (json_platform_value != QJsonValue::Undefined) {
                parameter.platform_flags = GetPlatformFlags(ReadStringArray(json_layer_object, "platforms"));
            }

            if (layer != nullptr) {
                CollectDefaultSettingData(layer->settings, parameter.settings);
            }

            const QJsonArray& json_settings = ReadArray(json_layer_object, "settings");
            for (int i = 0, n = json_settings.size(); i < n; ++i) {
                const QJsonObject& json_setting_object = json_settings[i].toObject();

                const std::string setting_key = ReadStringValue(json_setting_object, "key");
                const SettingType setting_type = GetSettingType(ReadStringValue(json_setting_object, "type").c_str());

                SettingData* setting_data = FindSetting(parameter.settings, setting_key.c_str());
                if (setting_data == nullptr) {
                    continue;
                }

                // Configuration type and layer type are differents, use layer default value
                if (setting_data->type != setting_type) {
                    continue;
                }

                const bool result = setting_data->Load(json_setting_object);
                assert(result);
            }

            this->parameters.push_back(parameter);
        }
    }

    this->GatherParameters(layers);

    return true;
}

bool Configuration::Save(const Path& full_path, bool export_mode) const {
    assert(!full_path.Empty());

    QJsonObject root;
    root.insert("file_format_version", Version::VKCONFIG.str().c_str());

    // Build the json document
    QJsonArray excluded_list;
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        if (this->parameters[i].control != LAYER_CONTROL_OFF) {
            continue;
        }
        excluded_list.append(this->parameters[i].key.c_str());
    }

    QJsonArray json_layers;  // This list of layers

    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        const Parameter& parameter = this->parameters[i];

        if (parameter.type == LAYER_TYPE_IMPLICIT && parameter.control == LAYER_CONTROL_AUTO) {
            continue;
        }

        QJsonObject json_layer;
        json_layer.insert("name", parameter.key.c_str());
        json_layer.insert("type", ::GetToken(parameter.type));
        if (parameter.builtin != LAYER_BUILTIN_NONE) {
            json_layer.insert("builtin", GetToken(parameter.builtin));
        }
        json_layer.insert("control", GetToken(parameter.control));
        json_layer.insert("rank", parameter.overridden_rank);
        json_layer.insert("version", parameter.api_version == Version::LATEST ? "latest" : parameter.api_version.str().c_str());
        if (parameter.builtin == LAYER_BUILTIN_NONE && !export_mode) {
            json_layer.insert("manifest", parameter.manifest.RelativePath().c_str());
        }
        SaveStringArray(json_layer, "platforms", GetPlatformTokens(parameter.platform_flags));
        json_layer.insert("override_settings", parameter.override_settings);

        QJsonArray json_settings;
        for (std::size_t j = 0, m = parameter.settings.size(); j < m; ++j) {
            const SettingData* setting_data = parameter.settings[j];

            if (setting_data->type == SETTING_GROUP) {
                continue;
            }

            QJsonObject json_setting;
            json_setting.insert("key", parameter.settings[j]->key.c_str());
            json_setting.insert("type", GetToken(setting_data->type));

            setting_data->Save(json_setting);

            json_settings.append(json_setting);
        }

        json_layer.insert("settings", json_settings);

        json_layers.append(json_layer);
    }

    QJsonObject json_configuration;
    json_configuration.insert("name", this->key.c_str());
    json_configuration.insert("version", this->version);
    SaveStringArray(json_configuration, "platforms", GetPlatformTokens(this->platform_flags));
    json_configuration.insert("default_layer_control", ::GetToken(this->default_control));
    json_configuration.insert("override_layers", this->override_layers);
    json_configuration.insert("override_loader", this->override_loader);
    SaveStringArray(json_configuration, "loader_message_types", GetLogTokens(this->loader_log_messages_flags));
    json_configuration.insert("selected_layer_name", this->selected_layer_name.c_str());
    json_configuration.insert("layers", json_layers);

    root.insert("configuration", json_configuration);

    QJsonDocument doc(root);

    const std::string absolute_path = full_path.AbsolutePath();

    QFile json_file(absolute_path.c_str());
    const bool result = json_file.open(QIODevice::WriteOnly | QIODevice::Text);

    if (!result) {
        QMessageBox alert;
        alert.setText("Could not save the configuration file!");
        alert.setWindowTitle(this->key.c_str());
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    } else {
        json_file.write(doc.toJson());
        json_file.close();
        return true;
    }
}

Parameter* Configuration::Find(const std::string& layer_key) {
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        if (this->parameters[i].key == layer_key) {
            return &this->parameters[i];
        }
    }

    return nullptr;
}

const Parameter* Configuration::Find(const std::string& layer_key) const {
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        if (this->parameters[i].key == layer_key) {
            return &this->parameters[i];
        }
    }

    return nullptr;
}

Parameter* Configuration::GetActiveParameter() {
    if (!this->override_layers) {
        return nullptr;
    }

    if (this->selected_layer_name.empty()) {
        return nullptr;
    }

    return this->Find(this->selected_layer_name);
}

const Parameter* Configuration::GetActiveParameter() const { return this->Find(this->selected_layer_name); }

void Configuration::Reset(const LayerManager& layers) {
    // Case 1: reset using built-in configuration files
    const std::vector<Path>& builtin_configuration_files = CollectFilePaths(":/configurations/");
    for (std::size_t i = 0, n = builtin_configuration_files.size(); i < n; ++i) {
        const std::string& basename = builtin_configuration_files[i].Basename();

        if (this->key == basename) {
            const bool result = this->Load(builtin_configuration_files[i], layers);
            assert(result);

            // OrderParameter(this->parameters, layers);
            return;
        }
    }

    // Case 2: reset using configuration files using saved configurations
    const Path full_path = RelativePath(Path::CONFIGS) + "/" + this->key + ".json";

    std::FILE* file = std::fopen(full_path.AbsolutePath().c_str(), "r");
    if (file) {
        std::fclose(file);
        const bool result = this->Load(full_path, layers);
        assert(result);

        // OrderParameter(this->parameters, layers);
        return;
    }

    // Case 3: reset to default values
    {
        for (auto it = this->parameters.begin(); it != this->parameters.end(); ++it) {
            it->control = LAYER_CONTROL_AUTO;
            it->overridden_rank = Parameter::NO_RANK;
            for (std::size_t i = 0, n = it->settings.size(); i < n; ++i) {
                it->settings[i]->Reset();
            }
        }

        // OrderParameter(this->parameters, layers);
    }
}

bool Configuration::HasMissingLayer(const LayerManager& layers, std::vector<std::string>& missing_layers) const {
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        if (!(it->platform_flags & (1 << VKC_PLATFORM))) {
            continue;  // If unsupported are missing, it doesn't matter
        }

        if (it->builtin != LAYER_BUILTIN_NONE) {
            continue;
        }

        if (it->control != LAYER_CONTROL_ON) {
            continue;  // If not on, let's say it doesn't matter so it's not notified to the users
        }

        /*
                if (layers.Find(it->key, it->api_version) == nullptr) {
                    missing_layers.push_back(it->key);
                    continue;
                }
        */

        if (layers.FindFromManifest(it->manifest) != nullptr) {
            continue;
        }

        if (layers.Find(it->key) != nullptr) {
            continue;
        }

        missing_layers.push_back(it->key);
    }

    return !missing_layers.empty();
}

void Configuration::RemoveParameter(const std::string& layer_key) {
    std::vector<Parameter> updated_parameters;

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        assert(!parameter.key.empty());

        if (parameter.key == layer_key) {
            continue;
        }

        updated_parameters.push_back(parameter);
    }

    std::swap(this->parameters, updated_parameters);
}

void Configuration::SwitchLayerVersion(const LayerManager& layers, const std::string& layer_key, const Path& manifest_path) {
    if (manifest_path.Empty()) {
        this->SwitchLayerLatest(layers, layer_key);
        return;
    }

    Parameter* parameter = this->Find(layer_key);
    assert(parameter != nullptr);

    const Layer* new_layer = layers.FindFromManifest(manifest_path);
    parameter->api_version = new_layer->api_version;
    parameter->type = new_layer->type;
    parameter->manifest = new_layer->manifest_path;
    ::CollectDefaultSettingData(new_layer->settings, parameter->settings);
}

void Configuration::SwitchLayerLatest(const LayerManager& layers, const std::string& layer_key) {
    Parameter* parameter = this->Find(layer_key);
    assert(parameter != nullptr);

    const Layer* new_layer = layers.Find(layer_key, Version::LATEST);
    parameter->api_version = Version::LATEST;
    parameter->type = new_layer->type;
    parameter->manifest = new_layer->manifest_path;
    ::CollectDefaultSettingData(new_layer->settings, parameter->settings);
}

void Configuration::GatherParameters(const LayerManager& layers) {
    const std::vector<Parameter>& previous_parameters = this->parameters;

    std::vector<Parameter> gathered_parameters;

    // Loop through the layers. They are expected to be in order
    for (std::size_t i = 0, n = previous_parameters.size(); i < n; ++i) {
        const Parameter& parameter = previous_parameters[i];
        assert(!parameter.key.empty());

        gathered_parameters.push_back(parameter);
    }

    const std::vector<std::string>& list = layers.GatherLayerNames();

    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        const Layer* layer = layers.Find(list[i], Version::LATEST);

        // The layer is already in the layer tree
        Parameter* parameter = nullptr;

        for (std::size_t i = 0, n = gathered_parameters.size(); i < n; ++i) {
            if (gathered_parameters[i].key == layer->key) {
                parameter = &gathered_parameters[i];
                break;
            }
        }

        if (parameter != nullptr) {
            const Layer* manifest_layer = layers.FindFromManifest(parameter->manifest);
            const Layer* actual_layer = manifest_layer != nullptr ? manifest_layer : layer;
            ::CollectDefaultSettingData(actual_layer->settings, parameter->settings);
        } else {
            Parameter parameter;
            parameter.key = layer->key;
            parameter.type = layer->type;
            parameter.control = this->default_control;
            parameter.api_version = Version::LATEST;
            parameter.manifest = layer->manifest_path;
            ::CollectDefaultSettingData(layer->settings, parameter.settings);

            gathered_parameters.push_back(parameter);
        }
    }

    ::AddApplicationEnabledParameters(gathered_parameters, this->default_control);

    ::OrderParameter(gathered_parameters, layers);

    std::swap(this->parameters, gathered_parameters);
}

void Configuration::Reorder(const std::vector<std::string>& layer_names) {
    std::vector<Parameter> ordered_parameters;

    int rank = 0;

    for (std::size_t i = 0, n = layer_names.size(); i < n; ++i) {
        Parameter* parameter = this->Find(layer_names[i]);
        if (parameter == nullptr) {
            continue;
        }

        parameter->overridden_rank = rank;
        ++rank;
        ordered_parameters.push_back(*parameter);
    }

    // Add the remaining parameters not listed in `layer_names`
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        Parameter& parameter = this->parameters[i];

        bool found = false;
        for (std::size_t j = 0, o = ordered_parameters.size(); j < o; ++j) {
            if (ordered_parameters[j].key == parameter.key) {
                found = true;
            }
        }

        if (!found) {
            parameter.overridden_rank = rank;
            ++rank;
            ordered_parameters.push_back(parameter);
        }
    }

    this->parameters = ordered_parameters;
}

bool Configuration::HasMultipleActiveParameter() const {
    std::size_t count = 0;

    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        const LayerControl control = this->parameters[i].control;

        if (control == LAYER_CONTROL_ON || control == LAYER_CONTROL_OFF) {
            ++count;
        }
    }

    return count > 1;
}

bool Configuration::IsDefault() const {
    const std::vector<Path>& builtin_configuration_files = CollectFilePaths(":/configurations/");
    for (std::size_t i = 0, n = builtin_configuration_files.size(); i < n; ++i) {
        const std::string& basename = builtin_configuration_files[i].Basename();

        if (basename == this->key) {
            return true;
        }
    }

    return false;
}

static const size_t NOT_FOUND = static_cast<size_t>(-1);

std::string MakeConfigurationName(const std::vector<Configuration>& configurations, const std::string& configuration_name) {
    const std::string key = configuration_name;
    const std::string base_name = ExtractDuplicateNumber(key) != NOT_FOUND ? ExtractDuplicateBaseName(key) : key;

    std::size_t max_duplicate = 0;
    for (std::size_t i = 0, n = configurations.size(); i < n; ++i) {
        const std::string& search_name = configurations[i].key;

        if (search_name.compare(0, base_name.length(), base_name) != 0) continue;

        const std::size_t found_number = ExtractDuplicateNumber(search_name);
        max_duplicate = std::max<std::size_t>(max_duplicate, found_number != NOT_FOUND ? found_number : 1);
    }

    return base_name + (max_duplicate > 0 ? format(" (%d)", max_duplicate + 1).c_str() : "");
}
