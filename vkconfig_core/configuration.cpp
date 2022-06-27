/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
#include "util.h"
#include "path.h"
#include "json.h"
#include "platform.h"
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

static const char* SUPPORTED_CONFIG_FILES[] = {"_2_2_3", "_2_2_2", "_2_2_1"};

Configuration::Configuration() : key("New Configuration"), platform_flags(PLATFORM_DESKTOP_BIT), view_advanced_settings(false) {}

bool Configuration::Load2_2(const std::vector<Layer>& available_layers, const QJsonObject& json_root_object) {
    const QJsonValue& json_configuration_value = json_root_object.value("configuration");
    if (json_configuration_value == QJsonValue::Undefined) return false;  // Not a configuration file

    const QJsonObject& json_configuration_object = json_configuration_value.toObject();

    // Required configuration values
    this->key = ReadString(json_configuration_object, "name").c_str();
    this->description = ReadString(json_configuration_object, "description").c_str();

    // Optional configuration values
    if (json_configuration_object.value("expanded_states") != QJsonValue::Undefined) {
        this->setting_tree_state = json_configuration_object.value("expanded_states").toVariant().toByteArray();
    }

    if (json_configuration_object.value("view_advanced_settings") != QJsonValue::Undefined) {
        this->view_advanced_settings = ReadBoolValue(json_configuration_object, "view_advanced_settings");
    }

    if (json_configuration_object.value("platforms") != QJsonValue::Undefined) {
        this->platform_flags = GetPlatformFlags(ReadStringArray(json_configuration_object, "platforms"));
    }

    if (json_configuration_object.value("layers_paths") != QJsonValue::Undefined) {
        std::vector<std::string> paths = ReadStringArray(json_configuration_object, "layers_paths");
        for (std::size_t i = 0, n = paths.size(); i < n; ++i) {
            const QFileInfo info(paths[i].c_str());
            if (info.exists()) {
                this->user_defined_paths.push_back(paths[i]);
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
        parameter.state = GetLayerState(ReadStringValue(json_layer_object, "state").c_str());

        const QJsonValue& json_platform_value = json_layer_object.value("platforms");
        if (json_platform_value != QJsonValue::Undefined) {
            parameter.platform_flags = GetPlatformFlags(ReadStringArray(json_layer_object, "platforms"));
        }

        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer != nullptr) {
            CollectDefaultSettingData(layer->settings, parameter.settings);
        }

        const QJsonArray& json_settings = ReadArray(json_layer_object, "settings");
        for (int i = 0, n = json_settings.size(); i < n; ++i) {
            const QJsonObject& json_setting_object = json_settings[i].toObject();

            const std::string setting_key = ReadStringValue(json_setting_object, "key");
            const SettingType setting_type = GetSettingType(ReadStringValue(json_setting_object, "type").c_str());

            SettingData* setting_data = FindSetting(parameter.settings, setting_key.c_str());
            if (setting_data == nullptr) continue;

            // Configuration type and layer type are differents, use layer default value
            if (setting_data->type != setting_type) continue;

            const bool result = setting_data->Load(json_setting_object);
            assert(result);
        }

        this->parameters.push_back(parameter);
    }

    return true;
}

bool Configuration::Load(const std::vector<Layer>& available_layers, const std::string& full_path) {
    assert(!full_path.empty());

    this->parameters.clear();

    QFile file(full_path.c_str());
    const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    QString json_text = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
        return false;
    }

    return Load2_2(available_layers, json_doc.object());
}

bool Configuration::Save(const std::vector<Layer>& available_layers, const std::string& full_path, bool exporter) const {
    assert(!full_path.empty());

    QJsonObject root;
    root.insert("file_format_version", Version::LAYER_CONFIG.str().c_str());

    // Build the json document
    QJsonArray excluded_list;
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        if (this->parameters[i].state != LAYER_STATE_EXCLUDED) {
            continue;
        }
        excluded_list.append(this->parameters[i].key.c_str());
    }

    QJsonArray json_layers;  // This list of layers

    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        const Parameter& parameter = this->parameters[i];
        if (parameter.state == LAYER_STATE_APPLICATION_CONTROLLED) {
            continue;
        }

        QJsonObject json_layer;
        json_layer.insert("name", parameter.key.c_str());
        json_layer.insert("rank", parameter.overridden_rank);
        json_layer.insert("state", GetToken(parameter.state));
        SaveStringArray(json_layer, "platforms", GetPlatformTokens(parameter.platform_flags));

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
    json_configuration.insert("description", this->description.c_str());
    SaveStringArray(json_configuration, "platforms", GetPlatformTokens(this->platform_flags));
    if (!exporter && !this->setting_tree_state.isEmpty()) {
        json_configuration.insert("expanded_states", this->setting_tree_state.data());
    }
    json_configuration.insert("view_advanced_settings", this->view_advanced_settings);
    json_configuration.insert("layers", json_layers);

    QJsonArray json_paths;
    for (std::size_t i = 0, n = user_defined_paths.size(); i < n; ++i) {
        json_paths.append(user_defined_paths[i].c_str());
    }
    json_configuration.insert("layers_paths", json_paths);

    root.insert("configuration", json_configuration);

    QJsonDocument doc(root);

    QFile json_file(full_path.c_str());
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

void Configuration::Reset(const std::vector<Layer>& available_layers, const PathManager& path_manager) {
    (void)path_manager;

    // Case 1: reset using built-in configuration files
    const QFileInfoList& builtin_configuration_files = GetJSONFiles(":/configurations/");
    for (int i = 0, n = builtin_configuration_files.size(); i < n; ++i) {
        const std::string& basename = builtin_configuration_files[i].baseName().toStdString();

        if (this->key == basename) {
            const bool result = this->Load(available_layers, builtin_configuration_files[i].absoluteFilePath().toStdString());
            assert(result);

            OrderParameter(this->parameters, available_layers);
            return;
        }
    }

    // Case 2: reset using configuration files using saved configurations
    const std::string base_config_path = GetPath(BUILTIN_PATH_CONFIG_REF);

    for (std::size_t i = 0, n = countof(SUPPORTED_CONFIG_FILES); i < n; ++i) {
        const std::string path = base_config_path + SUPPORTED_CONFIG_FILES[i] + "/" + this->key + ".json";

        std::FILE* file = std::fopen(path.c_str(), "r");
        if (file) {
            std::fclose(file);
            const bool result = this->Load(available_layers, path);
            assert(result);

            OrderParameter(this->parameters, available_layers);
            return;
        }
    }

    // Case 3: reset to default values
    {
        for (auto it = this->parameters.begin(); it != this->parameters.end(); ++it) {
            it->state = LAYER_STATE_APPLICATION_CONTROLLED;
            it->overridden_rank = Parameter::NO_RANK;
            for (std::size_t i = 0, n = it->settings.size(); i < n; ++i) {
                it->settings[i]->Reset();
            }
        }

        OrderParameter(this->parameters, available_layers);
    }
}

bool Configuration::HasOverride() const {
    for (std::size_t i = 0, n = this->parameters.size(); i < n; ++i) {
        if (!IsPlatformSupported(this->parameters[i].platform_flags)) {
            continue;
        }

        if (this->parameters[i].state != LAYER_STATE_APPLICATION_CONTROLLED) {
            return true;
        }
    }

    return false;
}

bool Configuration::IsBuiltIn() const {
    const QFileInfoList& builtin_configuration_files = GetJSONFiles(":/configurations/");
    for (int i = 0, n = builtin_configuration_files.size(); i < n; ++i) {
        const std::string& basename = builtin_configuration_files[i].baseName().toStdString();

        if (basename == this->key) {
            return true;
        }
    }

    return false;
}

static const size_t NOT_FOUND = static_cast<size_t>(-1);

static std::size_t ExtractDuplicateNumber(const std::string& configuration_name) {
    const std::size_t name_open = configuration_name.find_last_of("(");
    if (name_open == NOT_FOUND) return NOT_FOUND;

    const std::size_t name_close = configuration_name.find_last_of(")");
    if (name_close == NOT_FOUND) return NOT_FOUND;

    const std::string number = configuration_name.substr(name_open + 1, name_close - (name_open + 1));
    if (!IsNumber(number)) return NOT_FOUND;

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
