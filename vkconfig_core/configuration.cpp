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

Configuration::Configuration() : name("New Configuration"), _preset(ValidationPresetNone) {}

static Version GetConfigurationVersion(const QJsonValue& value) {
    if (SUPPORT_VKCONFIG_2_0_1) {
        return Version(value == QJsonValue::Undefined ? "2.0.1" : value.toString().toUtf8().constData());
    } else {
        assert(value != QJsonValue::Undefined);
        return Version(value.toString().toUtf8().constData());
    }
}

///////////////////////////////////////////////////////////////////////////////
// Load from a configuration file (.json really)
bool Configuration::Load(const QString& full_path) {
    assert(!full_path.isEmpty());

    QFile file(full_path);
    const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    QString json_text = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonParseError parse_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) return false;

    // Allocate a new profile container
    const QString& filename = QFileInfo(full_path).fileName();

    const QJsonObject& json_top_object = json_doc.object();
    const QStringList& key = json_top_object.keys();

    const QJsonValue& json_file_format_version = json_top_object.value("file_format_version");
    const Version version(GetConfigurationVersion(json_file_format_version));

    const QJsonValue& configuration_entry_value = json_top_object.value(key[0]);
    const QJsonObject& configuration_entry_object = configuration_entry_value.toObject();

    if (SUPPORT_VKCONFIG_2_0_1 && !HAS_SHADER_BASED) {
        if (full_path.contains("Validation - Shader Printf.json") || full_path.contains("Validation - Debug Printf.json") ||
            full_path.contains("Validation - GPU-Assisted.json")) {
            return false;
        }
    }

    if (SUPPORT_VKCONFIG_2_0_1 && version <= Version("2.0.1")) {
        name = filename.left(filename.length() - 5);
        if (name == "Validation - Shader Printf") {
            name = "Validation - Debug Printf";
            const int result = std::remove(full_path.toStdString().c_str());
            assert(result == 0);
        }
    } else {
        const QJsonValue& json_name_value = configuration_entry_object.value("name");
        assert(json_name_value != QJsonValue::Undefined);
        name = json_name_value.toString();
    }

    if (name.isEmpty()) {
        name = "Configuration";
        const int result = std::remove(full_path.toStdString().c_str());
        assert(result == 0);
    }

    const QJsonValue& excluded_value = configuration_entry_object.value("blacklisted_layers");
    assert(excluded_value != QJsonValue::Undefined);

    QJsonArray excluded_array = excluded_value.toArray();
    for (int i = 0; i < excluded_array.size(); i++) {
        Parameter parameter;
        parameter.name = excluded_array[i].toString();
        parameter.state = LAYER_STATE_EXCLUDED;

        parameters.push_back(parameter);
    }

    const QJsonValue& preset_index = configuration_entry_object.value("preset");
    _preset = static_cast<ValidationPreset>(preset_index.toInt());

    const QJsonValue& editor_state = configuration_entry_object.value("editor_state");
    _setting_tree_state = editor_state.toVariant().toByteArray();

    const QJsonValue& description = configuration_entry_object.value("description");
    assert(description != QJsonValue::Undefined);
    _description = description.toString();

    QJsonValue options_value = configuration_entry_object.value("layer_options");
    assert(options_value != QJsonValue::Undefined);

    QJsonObject layer_objects = options_value.toObject();
    const QStringList& layers = layer_objects.keys();

    if (options_value != QJsonValue::Undefined && version > Version::VKCONFIG) {
        QMessageBox alert;
        alert.setWindowTitle("Vulkan Configurator version is too old...");
        alert.setText(format("The \"%s\" configuration was created with a newer version of %s. Use %s from the "
                             "latest Vulkan SDK to resolve the issue. ",
                             name.toUtf8().constData(), VKCONFIG_NAME, VKCONFIG_NAME)
                          .c_str());
        alert.setInformativeText("Do you want to continue?");
        alert.setIcon(QMessageBox::Warning);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);
        if (alert.exec() == QMessageBox::No) exit(-1);
    }

    for (int layer_index = 0; layer_index < layers.length(); layer_index++) {
        const QJsonValue& layer_value = layer_objects.value(layers[layer_index]);
        const QJsonObject& layer_object = layer_value.toObject();
        const QJsonValue& layer_rank = layer_object.value("layer_rank");

        auto parameter = FindParameter(parameters, layers[layer_index]);
        if (parameter != parameters.end()) {
            parameter->overridden_rank = layer_rank == QJsonValue::Undefined ? Parameter::UNRANKED : layer_rank.toInt();
            LoadSettings(layer_object, *parameter);
        } else {
            Parameter parameter;
            parameter.name = layers[layer_index];
            parameter.state = LAYER_STATE_OVERRIDDEN;
            parameter.overridden_rank = Parameter::UNRANKED;
            LoadSettings(layer_object, parameter);
            parameters.push_back(parameter);
        }
    }

    return true;
}

bool Configuration::Save(const QString& full_path) const {
    assert(!full_path.isEmpty());

    QJsonObject root;
    root.insert("file_format_version", Version::VKCONFIG.str().c_str());

    // Build the json document
    QJsonArray excluded_list;
    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].state != LAYER_STATE_EXCLUDED) {
            continue;
        }
        excluded_list.append(parameters[i].name);
    }

    QJsonObject overridden_list;  // This list of layers

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        const Parameter& parameter = parameters[i];
        if (parameters[i].state == LAYER_STATE_APPLICATION_CONTROLLED) {
            continue;
        }

        QJsonObject json_settings;
        // Rank goes in here with settings
        json_settings.insert("layer_rank", parameter.overridden_rank);

        const bool result = SaveSettings(parameter, json_settings);
        assert(result);

        overridden_list.insert(parameter.name, json_settings);
    }

    QJsonObject json_configuration;
    json_configuration.insert("name", name);
    json_configuration.insert("blacklisted_layers", excluded_list);
    json_configuration.insert("description", _description);
    json_configuration.insert("preset", _preset);
    json_configuration.insert("editor_state", _setting_tree_state.data());
    json_configuration.insert("layer_options", overridden_list);
    root.insert("configuration", json_configuration);

    QJsonDocument doc(root);

    QFile json_file(full_path);
    const bool result = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);

    if (!result) {
        QMessageBox alert;
        alert.setText("Could not save the configuration file!");
        alert.setWindowTitle(name);
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

QString MakeConfigurationName(const std::vector<Configuration>& configurations, const QString& configuration_name) {
    const std::string name = configuration_name.toStdString();
    const std::string base_name = ExtractDuplicateNumber(name) != NOT_FOUND ? ExtractDuplicateBaseName(name) : name;

    std::size_t max_duplicate = 0;
    for (std::size_t i = 0, n = configurations.size(); i < n; ++i) {
        const std::string& search_name = configurations[i].name.toStdString();

        if (search_name.compare(0, base_name.length(), base_name) != 0) continue;

        const std::size_t found_number = ExtractDuplicateNumber(search_name);
        max_duplicate = std::max<std::size_t>(max_duplicate, found_number != NOT_FOUND ? found_number : 1);
    }

    return QString(base_name.c_str()) + (max_duplicate > 0 ? format(" (%d)", max_duplicate + 1).c_str() : "");
}

std::vector<Configuration>::iterator FindConfiguration(std::vector<Configuration>& configurations,
                                                       const QString& configuration_name) {
    for (auto it = configurations.begin(), end = configurations.end(); it != end; ++it)
        if (it->name == configuration_name) return it;
    return configurations.end();
}
