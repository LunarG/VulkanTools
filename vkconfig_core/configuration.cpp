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

Configuration::Configuration() : key("New Configuration"), platform_flags(PLATFORM_ALL_BIT) {}

static Version GetConfigurationVersion(const QJsonValue& value) {
    if (SUPPORT_VKCONFIG_2_0_1) {
        return Version(value == QJsonValue::Undefined ? "2.0.1" : value.toString().toStdString().c_str());
    } else {
        assert(value != QJsonValue::Undefined);
        return Version(value.toString().toStdString().c_str());
    }
}

bool Configuration::Load2_0(const std::vector<Layer>& available_layers, const QJsonObject& json_root_object,
                            const std::string& full_path) {
    const QString& filename = QFileInfo(full_path.c_str()).fileName();

    const QJsonValue& json_file_format_version = json_root_object.value("file_format_version");
    const Version version(GetConfigurationVersion(json_file_format_version));

    const QStringList& keys = json_root_object.keys();

    const QJsonValue& configuration_entry_value = json_root_object.value(keys[0]);
    const QJsonObject& configuration_entry_object = configuration_entry_value.toObject();

    if (SUPPORT_VKCONFIG_2_0_1 && version <= Version("2.0.1")) {
        key = filename.left(filename.length() - 5).toStdString();
    } else {
        key = ReadString(configuration_entry_object, "name").c_str();
    }

    if (key.empty()) {
        key = "Configuration";
        const int result = std::remove(full_path.c_str());
        assert(result == 0);
    }

    setting_tree_state = configuration_entry_object.value("editor_state").toVariant().toByteArray();

    description = ReadString(configuration_entry_object, "description").c_str();

    const QJsonValue& json_platform_value = configuration_entry_object.value("platforms");
    if (json_platform_value != QJsonValue::Undefined) {
        platform_flags = GetPlatformFlags(ReadStringArray(configuration_entry_object, "platforms"));
    }

    const QJsonObject& layer_objects = ReadObject(configuration_entry_object, "layer_options");
    const QStringList& layers = layer_objects.keys();

    for (int layer_index = 0, layer_count = layers.size(); layer_index < layer_count; ++layer_index) {
        const QJsonValue& layer_value = layer_objects.value(layers[layer_index]);
        const QJsonObject& layer_object = layer_value.toObject();
        const QJsonValue& layer_rank = layer_object.value("layer_rank");

        Parameter parameter;
        parameter.key = layers[layer_index].toStdString();
        parameter.overridden_rank = layer_rank == QJsonValue::Undefined ? Parameter::NO_RANK : layer_rank.toInt();
        parameter.state = LAYER_STATE_OVERRIDDEN;

        SettingDataSet settings;
        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer != nullptr) {
            settings = CollectDefaultSettingData(layer->settings);
        }

        parameter.settings = settings;
        parameters.push_back(parameter);
    }

    const QJsonValue& excluded_value = configuration_entry_object.value("blacklisted_layers");
    assert(excluded_value != QJsonValue::Undefined);

    const QJsonArray& excluded_array = excluded_value.toArray();
    for (int i = 0, n = excluded_array.size(); i < n; ++i) {
        Parameter* parameter = FindByKey(parameters, excluded_array[i].toString().toStdString().c_str());
        if (parameter != nullptr) {
            parameter->state = LAYER_STATE_EXCLUDED;
        } else {
            Parameter parameter;
            parameter.key = excluded_array[i].toString().toStdString();
            parameter.state = LAYER_STATE_EXCLUDED;

            parameters.push_back(parameter);
        }
    }

    return true;
}

bool Configuration::Load2_1(const std::vector<Layer>& available_layers, const QJsonObject& json_root_object) {
    const QJsonValue& json_configuration_value = json_root_object.value("configuration");
    if (json_configuration_value == QJsonValue::Undefined) return false;  // Not a configuration file

    const QJsonObject& json_configuration_object = json_configuration_value.toObject();

    // Required configuration values
    key = ReadString(json_configuration_object, "name").c_str();
    setting_tree_state = json_configuration_object.value("editor_state").toVariant().toByteArray();
    description = ReadString(json_configuration_object, "description").c_str();

    // Optional configuration values
    const QJsonValue& json_platform_value = json_configuration_object.value("platforms");
    if (json_platform_value != QJsonValue::Undefined) {
        platform_flags = GetPlatformFlags(ReadStringArray(json_configuration_object, "platforms"));
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

        SettingDataSet settings;
        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer != nullptr) {
            settings = CollectDefaultSettingData(layer->settings);
        }

        parameter.settings = settings;
        parameters.push_back(parameter);
    }

    return true;
}

bool Configuration::Load2_2(const std::vector<Layer>& available_layers, const QJsonObject& json_root_object) {
    const QJsonValue& json_configuration_value = json_root_object.value("configuration");
    if (json_configuration_value == QJsonValue::Undefined) return false;  // Not a configuration file

    const QJsonObject& json_configuration_object = json_configuration_value.toObject();

    // Required configuration values
    key = ReadString(json_configuration_object, "name").c_str();
    setting_tree_state = json_configuration_object.value("editor_state").toVariant().toByteArray();
    description = ReadString(json_configuration_object, "description").c_str();

    // Optional configuration values
    const QJsonValue& json_platform_value = json_configuration_object.value("platforms");
    if (json_platform_value != QJsonValue::Undefined) {
        platform_flags = GetPlatformFlags(ReadStringArray(json_configuration_object, "platforms"));
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

        SettingDataSet settings;
        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer != nullptr) {
            settings = CollectDefaultSettingData(layer->settings);
        }

        const QJsonArray& json_settings = ReadArray(json_layer_object, "settings");
        for (int i = 0, n = json_settings.size(); i < n; ++i) {
            const QJsonObject& json_setting_object = json_settings[i].toObject();

            const std::string setting_key = ReadStringValue(json_setting_object, "key");
            const SettingType setting_type = GetSettingType(ReadStringValue(json_setting_object, "type").c_str());

            SettingData& setting_data = settings.Create(setting_key, setting_type);

            // Configuration type and layer type are differents, use layer default value
            if (setting_data.type != setting_type) continue;

            switch (setting_data.type) {
                case SETTING_LOAD_FILE:
                case SETTING_SAVE_FILE:
                case SETTING_SAVE_FOLDER:
                case SETTING_ENUM:
                case SETTING_STRING: {
                    static_cast<SettingDataString&>(setting_data).value = ReadStringValue(json_setting_object, "value");
                    break;
                }
                case SETTING_INT: {
                    static_cast<SettingDataInt&>(setting_data).value = ReadIntValue(json_setting_object, "value");
                    break;
                }
                case SETTING_INT_RANGES: {
                    static_cast<SettingDataIntRange&>(setting_data).value = ReadStringValue(json_setting_object, "value");
                    break;
                }
                case SETTING_BOOL_NUMERIC_DEPRECATED:
                case SETTING_BOOL: {
                    static_cast<SettingDataBool&>(setting_data).value = ReadBoolValue(json_setting_object, "value");
                    break;
                }
                case SETTING_LIST:
                case SETTING_FLAGS: {
                    static_cast<SettingDataVector&>(setting_data).value = ReadStringArray(json_setting_object, "value");
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }

        parameter.settings = settings;
        parameters.push_back(parameter);
    }

    return true;
}

bool Configuration::Load(const std::vector<Layer>& available_layers, const std::string& full_path) {
    assert(!full_path.empty());

    parameters.clear();

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

    const QJsonObject& json_root_object = json_doc.object();

    const Version version(GetConfigurationVersion(json_root_object.value("file_format_version")));
    if (SUPPORT_VKCONFIG_2_0_3 && version < Version(2, 1, 0)) {
        return Load2_0(available_layers, json_root_object, full_path);
    } else if (SUPPORT_VKCONFIG_2_1_0 && version < Version(2, 2, 0)) {
        return Load2_1(available_layers, json_root_object);
    } else {
        return Load2_2(available_layers, json_root_object);
    }
}

bool Configuration::Save(const std::vector<Layer>& available_layers, const std::string& full_path) const {
    assert(!full_path.empty());

    QJsonObject root;
    root.insert("file_format_version", Version::VKCONFIG.str().c_str());

    // Build the json document
    QJsonArray excluded_list;
    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].state != LAYER_STATE_EXCLUDED) {
            continue;
        }
        excluded_list.append(parameters[i].key.c_str());
    }

    QJsonArray json_layers;  // This list of layers

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        if (parameter.state == LAYER_STATE_APPLICATION_CONTROLLED) {
            continue;
        }

        QJsonObject json_layer;
        json_layer.insert("name", parameter.key.c_str());
        json_layer.insert("rank", parameter.overridden_rank);
        json_layer.insert("state", GetToken(parameter.state));
        SaveStringArray(json_layer, "platforms", GetPlatformTokens(parameter.platform_flags));

        QJsonArray json_settings;
        for (std::size_t j = 0, m = parameter.settings.Size(); j < m; ++j) {
            const SettingData& setting_data = parameter.settings[j];

            QJsonObject json_setting;
            json_setting.insert("key", parameter.settings[j].key.c_str());
            json_setting.insert("type", GetSettingToken(setting_data.type));

            switch (setting_data.type) {
                case SETTING_LOAD_FILE:
                case SETTING_SAVE_FILE:
                case SETTING_SAVE_FOLDER:
                case SETTING_ENUM:
                case SETTING_INT_RANGES:
                case SETTING_STRING: {
                    json_setting.insert("value", static_cast<const SettingDataString&>(setting_data).value.c_str());
                    break;
                }
                case SETTING_INT: {
                    json_setting.insert("value", static_cast<const SettingDataInt&>(setting_data).value);
                    break;
                }
                case SETTING_BOOL_NUMERIC_DEPRECATED:
                case SETTING_BOOL: {
                    json_setting.insert("value", static_cast<const SettingDataBool&>(setting_data).value);
                    break;
                }
                case SETTING_LIST:
                case SETTING_FLAGS: {
                    QJsonArray json_array;

                    for (std::size_t i = 0, n = static_cast<const SettingDataVector&>(setting_data).value.size(); i < n; ++i) {
                        json_array.append(static_cast<const SettingDataVector&>(setting_data).value[i].c_str());
                    }

                    json_setting.insert("value", json_array);
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }

            json_settings.append(json_setting);
        }

        json_layer.insert("settings", json_settings);

        json_layers.append(json_layer);
    }

    QJsonObject json_configuration;
    json_configuration.insert("name", key.c_str());
    json_configuration.insert("description", description.c_str());
    SaveStringArray(json_configuration, "platforms", GetPlatformTokens(platform_flags));
    json_configuration.insert("editor_state", setting_tree_state.data());
    json_configuration.insert("layers", json_layers);
    root.insert("configuration", json_configuration);

    QJsonDocument doc(root);

    QFile json_file(full_path.c_str());
    const bool result = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);

    if (!result) {
        QMessageBox alert;
        alert.setText("Could not save the configuration file!");
        alert.setWindowTitle(key.c_str());
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
    for (std::size_t i = PATH_LAST_CONFIGURATION; i >= PATH_FIRST_CONFIGURATION; --i) {
        const std::string full_path(path_manager.GetFullPath(static_cast<PathType>(i), this->key.c_str()));
        std::FILE* file = std::fopen(full_path.c_str(), "r");
        if (file) {
            std::fclose(file);
            const bool result = this->Load(available_layers, full_path);
            assert(result);

            OrderParameter(this->parameters, available_layers);
            return;
        }
    }

    // Case 3: reset to zero
    {
        for (auto it = this->parameters.begin(); it != this->parameters.end(); ++it) {
            it->state = LAYER_STATE_APPLICATION_CONTROLLED;
            it->overridden_rank = Parameter::NO_RANK;
            it->settings = CollectDefaultSettingData(FindByKey(available_layers, it->key.c_str())->settings);
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

bool Configuration::HasSavedFile(const PathManager& path_manager) const {
    for (std::size_t i = PATH_LAST_CONFIGURATION; i >= PATH_FIRST_CONFIGURATION; --i) {
        const std::string full_path(path_manager.GetFullPath(static_cast<PathType>(i), this->key.c_str()));
        std::FILE* file = std::fopen(full_path.c_str(), "r");
        if (file) {
            std::fclose(file);
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
