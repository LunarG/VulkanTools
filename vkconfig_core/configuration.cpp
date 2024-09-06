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

static void AddApplicationEnabledParameters(std::vector<Parameter>& parameters) {
    bool found_applications_api = false;
    for (auto paramater : parameters) {
        if (paramater.control != LAYER_CONTROL_APPLICATIONS_API) {
            continue;
        }

        found_applications_api = true;
        break;
    }

    if (!found_applications_api) {
        Parameter applications_enabled_layers_api;
        applications_enabled_layers_api.key = "Vulkan Layers from the Application Vulkan API";
        applications_enabled_layers_api.control = LAYER_CONTROL_APPLICATIONS_API;
        applications_enabled_layers_api.overridden_rank = 998;
        parameters.push_back(applications_enabled_layers_api);
    }

    bool found_applications_env = false;
    for (auto paramater : parameters) {
        if (paramater.control != LAYER_CONTROL_APPLICATIONS_ENV) {
            continue;
        }

        found_applications_env = true;
        break;
    }

    if (!found_applications_env) {
        Parameter applications_enabled_layers_env;
        applications_enabled_layers_env.key = "Vulkan Layers from Application Environment Variables";
        applications_enabled_layers_env.control = LAYER_CONTROL_APPLICATIONS_ENV;
        applications_enabled_layers_env.overridden_rank = 999;
        parameters.push_back(applications_enabled_layers_env);
    }
}

Configuration Configuration::Create(const LayerManager& layers, const std::string& key) {
    Configuration result;

    result.key = key;
    result.GatherParameters(layers);

    AddApplicationEnabledParameters(result.parameters);

    return result;
}

Configuration Configuration::CreateDisabled(const LayerManager& layers) {
    Configuration result;
    result.key = "_DisablingConfiguration";
    result.GatherParameters(layers);

    for (std::size_t i = 0, n = result.parameters.size(); i < n; ++i) {
        result.parameters[i].control = LAYER_CONTROL_OFF;
    }

    AddApplicationEnabledParameters(result.parameters);

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
    this->description = ReadString(json_configuration_object, "description").c_str();

    // Optional configuration values
    if (json_configuration_object.value("version") != QJsonValue::Undefined) {
        this->version = json_configuration_object.value("version").toVariant().toInt();
    }

    if (json_configuration_object.value("view_advanced_settings") != QJsonValue::Undefined) {
        this->view_advanced_settings = ReadBoolValue(json_configuration_object, "view_advanced_settings");
    }

    if (json_configuration_object.value("view_advanced_layers") != QJsonValue::Undefined) {
        this->view_advanced_layers = ReadBoolValue(json_configuration_object, "view_advanced_layers");
    }

    if (json_configuration_object.value("selected_layer_name") != QJsonValue::Undefined) {
        this->selected_layer_name = ReadString(json_configuration_object, "selected_layer_name");
    }

    if (json_configuration_object.value("loader_message_types") != QJsonValue::Undefined) {
        const std::vector<std::string>& loader_messsage_types = ReadStringArray(json_configuration_object, "loader_message_types");
        this->loader_log_messages_flags = GetLogFlags(loader_messsage_types);
    }

    if (json_configuration_object.value("platforms") != QJsonValue::Undefined) {
        this->platform_flags = GetPlatformFlags(ReadStringArray(json_configuration_object, "platforms"));
    }

    this->user_defined_paths.clear();
    if (json_configuration_object.value("layers_paths") != QJsonValue::Undefined) {
        std::vector<std::string> paths = ReadStringArray(json_configuration_object, "layers_paths");
        for (std::size_t i = 0, n = paths.size(); i < n; ++i) {
            const QFileInfo info(paths[i].c_str());
            if (info.exists()) {
                this->user_defined_paths.push_back(Path(paths[i]));
            } else {
                QMessageBox alert;
                alert.QDialog::setWindowTitle("User-defined layer path doesn't exist");
                alert.setText(format("'%s' user-defined layer path doesn't exist.", paths[i].c_str()).c_str());
                alert.setInformativeText(
                    format("'%s' configuration specifies this path, some expected layers might not be found.", this->key.c_str())
                        .c_str());
                alert.setStandardButtons(QMessageBox::Ok);
                alert.setDefaultButton(QMessageBox::Ok);
                alert.setIcon(QMessageBox::Warning);
                alert.exec();
            }
        }
    }

    // Required configuration layers values
    const QJsonArray& json_layers_array = ReadArray(json_configuration_object, "layers");
    for (int layer_index = 0, layer_count = json_layers_array.size(); layer_index < layer_count; ++layer_index) {
        const QJsonObject& json_layer_object = json_layers_array[layer_index].toObject();

        Parameter parameter;
        parameter.key = ReadStringValue(json_layer_object, "name").c_str();
        parameter.overridden_rank = ReadIntValue(json_layer_object, "rank");
        parameter.control = GetLayerControl(ReadStringValue(json_layer_object, "control").c_str());
        const std::string& version = ReadStringValue(json_layer_object, "version");
        parameter.api_version = version == "latest" ? Version::LATEST : Version(version.c_str());

        const QJsonValue& json_platform_value = json_layer_object.value("platforms");
        if (json_platform_value != QJsonValue::Undefined) {
            parameter.platform_flags = GetPlatformFlags(ReadStringArray(json_layer_object, "platforms"));
        }

        const QJsonValue& json_expanded_value = json_layer_object.value("expanded_states");
        if (json_expanded_value != QJsonValue::Undefined) {
            parameter.setting_tree_state = json_layer_object.value("expanded_states").toVariant().toByteArray();
        }

        const Layer* layer = layers.Find(parameter.key, parameter.api_version);

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

    this->GatherParameters(layers);

    AddApplicationEnabledParameters(this->parameters);

    return true;
}

bool Configuration::Save(const Path& full_path, bool exporter) const {
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

        QJsonObject json_layer;
        json_layer.insert("name", parameter.key.c_str());
        json_layer.insert("rank", parameter.overridden_rank);
        json_layer.insert("control", GetToken(parameter.control));
        json_layer.insert("version", parameter.api_version == Version::LATEST ? "latest" : parameter.api_version.str().c_str());
        SaveStringArray(json_layer, "platforms", GetPlatformTokens(parameter.platform_flags));
        if (!exporter && !parameter.setting_tree_state.isEmpty()) {
            json_layer.insert("expanded_states", parameter.setting_tree_state.data());
        }

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
    json_configuration.insert("description", this->description.c_str());
    SaveStringArray(json_configuration, "platforms", GetPlatformTokens(this->platform_flags));
    json_configuration.insert("view_advanced_settings", this->view_advanced_settings);
    json_configuration.insert("view_advanced_layers", this->view_advanced_layers);
    json_configuration.insert("selected_layer_name", this->selected_layer_name.c_str());
    SaveStringArray(json_configuration, "loader_message_types", GetLogTokens(this->loader_log_messages_flags));
    json_configuration.insert("layers", json_layers);

    QJsonArray json_paths;
    for (std::size_t i = 0, n = this->user_defined_paths.size(); i < n; ++i) {
        json_paths.append(this->user_defined_paths[i].AbsolutePath().c_str());
    }
    json_configuration.insert("layers_paths", json_paths);

    root.insert("configuration", json_configuration);

    QJsonDocument doc(root);

    QFile json_file(full_path.AbsolutePath().c_str());
    const bool result = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);

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

void Configuration::Reset(const LayerManager& layers) {
    // Case 1: reset using built-in configuration files
    const std::vector<Path>& builtin_configuration_files = CollectFilePaths(":/configurations/");
    for (std::size_t i = 0, n = builtin_configuration_files.size(); i < n; ++i) {
        const std::string& basename = builtin_configuration_files[i].Basename();

        if (this->key == basename) {
            const bool result = this->Load(builtin_configuration_files[i], layers);
            assert(result);

            OrderParameter(this->parameters, layers);
            return;
        }
    }

    // Case 2: reset using configuration files using saved configurations
    const Path full_path(Get(Path::CONFIGS) + "/" + this->key + ".json");

    std::FILE* file = std::fopen(full_path.AbsolutePath().c_str(), "r");
    if (file) {
        std::fclose(file);
        const bool result = this->Load(full_path, layers);
        assert(result);

        OrderParameter(this->parameters, layers);
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

        OrderParameter(this->parameters, layers);
    }
}

bool Configuration::HasOverride() const {
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        if (!IsPlatformSupported(this->parameters[i].platform_flags)) {
            continue;
        }

        if (this->parameters[i].control != LAYER_CONTROL_AUTO) {
            return true;
        }
    }

    return false;
}

void Configuration::SwitchLayerVersion(const LayerManager& layers, const std::string& layer_key, const Version& version) {
    Parameter* parameter = this->Find(layer_key);
    assert(parameter != nullptr);

    const Layer* new_layer = layers.Find(layer_key, version);

    parameter->api_version = version;
    CollectDefaultSettingData(new_layer->settings, parameter->settings);
}

void Configuration::GatherParameters(const LayerManager& layers) {
    const std::vector<Parameter>& previous_parameters = this->parameters;

    std::vector<Parameter> gathered_parameters;

    // Loop through the layers. They are expected to be in order
    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        assert(!parameter.key.empty());

        gathered_parameters.push_back(parameter);
    }

    const std::vector<std::string>& list = layers.BuildLayerNameList();

    for (std::size_t i = 0, n = list.size(); i < n; ++i) {
        const Layer* layer = layers.Find(list[i], Version::LATEST);

        // The layer is already in the layer tree
        if (this->Find(layer->key.c_str())) {
            continue;
        }

        Parameter parameter;
        parameter.key = layer->key;
        parameter.control = LAYER_CONTROL_AUTO;
        parameter.api_version = Version::LATEST;
        CollectDefaultSettingData(layer->settings, parameter.settings);

        gathered_parameters.push_back(parameter);
    }

    // OrderParameter(gathered_parameters, layers);

    this->parameters = gathered_parameters;
}

void Configuration::Reorder(const std::vector<std::string>& layer_names) {
    std::vector<Parameter> ordered_parameters;

    for (std::size_t i = 0, n = layer_names.size(); i < n; ++i) {
        Parameter* parameter = this->Find(layer_names[i]);
        assert(parameter != nullptr);
        ordered_parameters.push_back(*parameter);
    }

    if (!this->view_advanced_layers) {
        for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
            Parameter& parameter = this->parameters[i];

            bool found = false;
            for (std::size_t j = 0, o = ordered_parameters.size(); j < o; ++j) {
                if (ordered_parameters[j].key == parameter.key) {
                    found = true;
                }
            }

            if (!found) {
                ordered_parameters.push_back(parameter);
            }
        }
    }

    this->parameters = ordered_parameters;
}

bool Configuration::IsBuiltIn() const {
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

static std::size_t ExtractDuplicateNumber(const std::string& configuration_name) {
    const std::size_t name_open = configuration_name.find_last_of("(");
    if (name_open == NOT_FOUND) {
        return NOT_FOUND;
    }

    const std::size_t name_close = configuration_name.find_last_of(")");
    if (name_close == NOT_FOUND) {
        return NOT_FOUND;
    }

    const std::string number = configuration_name.substr(name_open + 1, name_close - (name_open + 1));
    if (!IsNumber(number)) {
        return NOT_FOUND;
    }

    return std::stoi(number);
}

static std::string ExtractDuplicateBaseName(const std::string& configuration_name) {
    assert(ExtractDuplicateNumber(configuration_name) != NOT_FOUND);
    const std::size_t found = configuration_name.find_last_of("(");
    assert(found != NOT_FOUND);
    return configuration_name.substr(0, found - 1);
}

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
