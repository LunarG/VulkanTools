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
#include "version.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

#include <cassert>

Configuration::Configuration() : _preset(ValidationPresetNone) {}

Configuration::~Configuration() {
    qDeleteAll(_layers.begin(), _layers.end());
    _layers.clear();
}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
Layer* Configuration::FindLayer(const QString& layer_name, const QString& full_path) const {
    assert(!layer_name.isEmpty());
    assert(!full_path.isEmpty());

    for (int i = 0; i < _layers.size(); i++)
        if (_layers[i]->_name == layer_name && _layers[i]->_layer_path == full_path) return _layers[i];

    return nullptr;
}

////////////////////////////////////////////////////////////
// Copy a profile so we can mess with it.
Configuration* Configuration::DuplicateConfiguration() {
    Configuration* duplicate = new Configuration;
    duplicate->_name = _name;
    duplicate->_description = _description;
    duplicate->_excluded_layers = _excluded_layers;
    duplicate->_preset = _preset;
    duplicate->_layers = _layers;
    return duplicate;
}

///////////////////////////////////////////////////////////
/// Remove unused layers and build the list of
/// black listed layers.
void Configuration::CollapseConfiguration() {
    _excluded_layers.clear();

    // Look for black listed layers, add them to the
    // string list of names, but remove them from
    // the list of layers
    int layer_index = 0;
    int new_rank = 0;
    while (layer_index < _layers.size()) {
        if (!_layers[layer_index]->IsValid()) continue;

        // Remove this layer?
        if (_layers[layer_index]->_state == LAYER_STATE_EXCLUDED) {
            _excluded_layers << _layers[layer_index]->_name;
            _layers.removeAt(layer_index);
            continue;
        }

        // If the layer is not active, also remove it
        // Important to do black list test FIRST
        if (!_layers[layer_index]->_state == LAYER_STATE_OVERRIDDEN) {
            _layers.removeAt(layer_index);
            continue;
        }

        // We are keeping this layer, reset it's rank
        _layers[layer_index]->_rank = new_rank++;

        layer_index++;
    }
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
bool Configuration::Load(const QString& full_path, const QVector<Layer*>& available_Layers) {
    assert(!full_path.isEmpty());

    QFile file(full_path);
    bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    QString json_text = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument json_doc;
    QJsonParseError parse_error;
    json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) return false;

    // Allocate a new profile container
    const QString& filename = QFileInfo(full_path).fileName();

    QJsonObject json_top_object = json_doc.object();
    QStringList key = json_top_object.keys();

    const QJsonValue& json_file_format_version = json_top_object.value("file_format_version");
    const Version version(GetConfigurationVersion(json_file_format_version));

    QJsonValue configuration_entry_value = json_top_object.value(key[0]);
    QJsonObject configuration_entry_object = configuration_entry_value.toObject();

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
        _excluded_layers << excluded_array[i].toString();
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
        alert.setWindowTitle("Could not load configuration file!");
        alert.setText(format("The \"%s\" configuration was created with a newer version of %s. The configuration is discarded",
                             _name.toUtf8().constData(), VKCONFIG_NAME)
                          .c_str());
        alert.setInformativeText("Use Vulkan Configurator from the latest Vulkan SDK to resolve the issue.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
        return false;
    }

    // Build the list of layers with their settings. If both the layers and
    // the blacklist are emtpy, then automatic fail

    for (int layer_index = 0; layer_index < layers.length(); layer_index++) {
        const Layer* layer = nullptr;
        QJsonValue layer_value = layer_objects.value(layers[layer_index]);
        QJsonObject layer_object = layer_value.toObject();

        // To match the layer we just need the name, paths are not
        // hard-matched to the configuration.
        // Find this in our lookup of layers. The standard layers are listed first
        layer = ::FindLayer(available_Layers, layers[layer_index]);
        if (layer == nullptr) {  // If not found, we have a layer missing....
            continue;
        }

        assert(layer->IsValid());

        // Make a copy add it to this layer
        Layer* layer_copy = new Layer(*layer);

        QJsonValue layerRank = layer_object.value("layer_rank");
        layer_copy->_rank = layerRank.toInt();
        layer_copy->_state = LAYER_STATE_OVERRIDDEN;  // Always because it's present in the file

        _layers.push_back(layer_copy);

        // Load the layer
        LoadSettings(layer_object, layer_copy->_layer_settings);
    }

    SortByRank(_layers);

    return true;
}

bool Configuration::Save(const QString& full_path) const {
    assert(!full_path.isEmpty());

    QJsonObject root;
    root.insert("file_format_version", Version::VKCONFIG.str().c_str());

    // Build the json document
    QJsonArray excluded_list;
    for (int i = 0, n = _excluded_layers.size(); i < n; i++) excluded_list.append(_excluded_layers[i]);

    QJsonObject layer_list;  // This list of layers

    for (int layer_index = 0, layer_count = _layers.size(); layer_index < layer_count; ++layer_index) {
        const Layer& layer = *_layers[layer_index];
        assert(layer.IsValid());

        QJsonObject json_settings;
        // Rank goes in here with settings
        json_settings.insert("layer_rank", layer._rank);

        const bool result = SaveSettings(layer._layer_settings, json_settings);
        assert(result);

        layer_list.insert(layer._name, json_settings);
    }

    QJsonObject json_configuration;
    json_configuration.insert("name", _name);
    json_configuration.insert("blacklisted_layers", excluded_list);
    json_configuration.insert("description", _description);
    json_configuration.insert("preset", _preset);
    json_configuration.insert("editor_state", _setting_tree_state.data());
    json_configuration.insert("layer_options", layer_list);
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
