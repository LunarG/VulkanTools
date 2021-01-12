/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
#include <QMessageBox>

#include <cassert>
#include <cstdio>
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

        std::vector<LayerSettingData> settings;
        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer) {
            settings = CollectDefaultSettingData(layer->settings);
        }

        const QStringList& layer_settings = layer_object.keys();
        for (int setting_index = 0, setting_count = layer_settings.size(); setting_index < setting_count; ++setting_index) {
            if (layer_settings[setting_index] == "layer_rank") continue;

            const QJsonObject& setting_object = ReadObject(layer_object, layer_settings[setting_index].toStdString().c_str());

            LayerSettingData json_setting_data;
            json_setting_data.key = layer_settings[setting_index].toStdString();
            json_setting_data.value = ReadString(setting_object, "default");

            LayerSettingData* layer_setting_data = FindByKey(settings, json_setting_data.key.c_str());
            if (layer_setting_data) {
                layer_setting_data->value = json_setting_data.value;
            } else {
                settings.push_back(json_setting_data);
            }
        }

        parameter.settings = settings;
        parameters.push_back(parameter);
    }

    const QJsonValue& excluded_value = configuration_entry_object.value("blacklisted_layers");
    assert(excluded_value != QJsonValue::Undefined);

    const QJsonArray& excluded_array = excluded_value.toArray();
    for (int i = 0, n = excluded_array.size(); i < n; ++i) {
        auto parameter = FindItByKey(parameters, excluded_array[i].toString().toStdString().c_str());
        if (parameter != parameters.end()) {
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
            const int layer_platform_flags = GetPlatformFlags(ReadStringArray(json_layer_object, "platforms"));
            if ((layer_platform_flags & (1 << VKC_PLATFORM)) == 0) continue;
        }

        std::vector<LayerSettingData> settings;
        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer) {
            settings = CollectDefaultSettingData(layer->settings);
        }

        const QJsonArray& json_settings = ReadArray(json_layer_object, "settings");
        for (int i = 0, n = json_settings.size(); i < n; ++i) {
            const QJsonObject& json_setting_object = json_settings[i].toObject();

            LayerSettingData json_setting_data;
            json_setting_data.key = ReadStringValue(json_setting_object, "key");
            json_setting_data.value = ReadString(json_setting_object, "value");

            LayerSettingData* layer_setting_data = FindByKey(settings, json_setting_data.key.c_str());
            if (layer_setting_data) {
                layer_setting_data->value = json_setting_data.value;
            } else {
                settings.push_back(json_setting_data);
            }
        }

        parameter.settings = settings;
        parameters.push_back(parameter);
    }

    return true;
}

bool Configuration::Load(const std::vector<Layer>& available_layers, const std::string& full_path) {
    assert(!full_path.empty());

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
    } else {
        return Load2_1(available_layers, json_root_object);
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

        QJsonArray json_settings;
        for (std::size_t j = 0, m = parameter.settings.size(); j < m; ++j) {
            QJsonObject json_setting;
            json_setting.insert("key", parameter.settings[j].key.c_str());
            json_setting.insert("value", parameter.settings[j].value.c_str());
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

bool Configuration::IsEmpty() const { return parameters.empty(); }

bool Configuration::IsAvailableOnThisPlatform() const { return platform_flags & (1 << VKC_PLATFORM); }

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
