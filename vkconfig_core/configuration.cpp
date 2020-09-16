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

void Sort(std::vector<Parameter>& parameters) {
    std::vector<Parameter> sorted_parameters;

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        if (parameters[i].state == LAYER_STATE_EXCLUDED) sorted_parameters.push_back(parameters[i]);
    }

    std::swap(parameters, sorted_parameters);
}

Configuration::Configuration() : _name("New Configuration"), _preset(ValidationPresetNone) {}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
Parameter* Configuration::FindParameter(const QString& layer_name) {
    assert(!layer_name.isEmpty());

    for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
        Parameter& parameter = parameters[i];
        if (parameter.name == layer_name) {
            return &parameters[i];
        }
    }

    return nullptr;
}

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
        _name = filename.left(filename.length() - 5);
        if (_name == "Validation - Shader Printf") {
            _name = "Validation - Debug Printf";
            QFile file(full_path);
            file.remove();
        }
    } else {
        const QJsonValue& json_name_value = configuration_entry_object.value("name");
        assert(json_name_value != QJsonValue::Undefined);
        _name = json_name_value.toString();
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
                             _name.toUtf8().constData(), VKCONFIG_NAME, VKCONFIG_NAME)
                          .c_str());
        alert.setInformativeText("Do you want to continue?");
        alert.setIcon(QMessageBox::Warning);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);
        if (alert.exec() == QMessageBox::No) exit(-1);
    }

    // Build the list of layers with their settings. If both the layers and
    // the blacklist are emtpy, then automatic fail

    for (int layer_index = 0; layer_index < layers.length(); layer_index++) {
        const QJsonValue& layer_value = layer_objects.value(layers[layer_index]);
        const QJsonObject& layer_object = layer_value.toObject();

        // const QJsonValue& layer_rank = layer_object.value("layer_rank");

        Parameter* parameter = FindParameter(layers[layer_index]);
        if (parameter) {
            LoadSettings(layer_object, parameter->settings);
        } else {
            Parameter parameter;
            parameter.name = layers[layer_index];
            parameter.state = LAYER_STATE_OVERRIDDEN;
            LoadSettings(layer_object, parameter.settings);
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
        json_settings.insert("layer_rank", static_cast<int>(i));

        const bool result = SaveSettings(parameter.settings, json_settings);
        assert(result);

        overridden_list.insert(parameter.name, json_settings);
    }

    QJsonObject json_configuration;
    json_configuration.insert("name", _name);
    json_configuration.insert("blacklisted_layers", excluded_list);
    json_configuration.insert("description", _description);
    json_configuration.insert("preset", _preset);
    json_configuration.insert("editor_state", _setting_tree_state.data());
    json_configuration.insert("layer_options", overridden_list);
    root.insert("configuration", json_configuration);

    QJsonDocument doc(root);

    ///////////////////////////////////////////////////////////
    // Write it out - file name is same as name. If it's been
    // changed, this corrects the behavior.

    QFile json_file(full_path);
    if (!json_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox alert;
        alert.setText("Could not save configuration file!");
        alert.setWindowTitle(_name);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    }

    json_file.write(doc.toJson());
    json_file.close();
    return true;
}

bool Configuration::IsEmpty() const { return parameters.empty(); }

bool operator==(const std::vector<Parameter>& a, const std::vector<Parameter>& b) {
    if (a.size() != b.size()) return false;

    for (std::size_t i = 0, n = a.size(); i < n; ++i) {
        if (a[i].name != b[i].name) return false;

        if (a[i].state != b[i].state) return false;

        if (a[i].settings.size() != b[i].settings.size()) return false;

        for (std::size_t j = 0, o = a[i].settings.size(); j < o; ++j) {
            if (a[i].settings[j] != b[i].settings[j]) return false;
        }
    }

    return true;
}

bool operator!=(const std::vector<Parameter>& a, const std::vector<Parameter>& b) { return !(a == b); }
