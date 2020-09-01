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
#include "configurator.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/version.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

#include <cassert>

static const Version FILE_FORMAT_VERSION("1.2.0");

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

///////////////////////////////////////////////////////////
// Find the layer if it exists. Only care about the name
Layer* Configuration::FindLayerNamed(const QString& layer_name) const {
    assert(!layer_name.isEmpty());

    for (int i = 0; i < _layers.size(); i++)
        if (_layers[i]->_name == layer_name) return _layers[i];

    return nullptr;
}

////////////////////////////////////////////////////////////
// Copy a profile so we can mess with it.
Configuration* Configuration::DuplicateConfiguration() {
    Configuration* duplicate = new Configuration;
    duplicate->_name = _name;
    duplicate->_file = _file;
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

bool Configuration::IsValid() const {
    Configurator& configurator = Configurator::Get();

    if (_excluded_layers.empty() && _layers.empty()) return false;

    for (int i = 0, n = _layers.size(); i < n; ++i) {
        if (configurator.FindLayerNamed(_layers[i]->_name) == nullptr) return false;
    }

    for (int i = 0, n = _excluded_layers.size(); i < n; ++i) {
        if (configurator.FindLayerNamed(_excluded_layers[i]) == nullptr) return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Load from a configuration file (.json really)
bool Configuration::Load(const QString& path_to_configuration) {
    // Just load the name for now, and if it's read only
    if (path_to_configuration.isEmpty()) return nullptr;

    QFile file(path_to_configuration);
    bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    QString json_text = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument json_doc;
    QJsonParseError parse_error;
    json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) return nullptr;

    // Allocate a new profile container
    const QString& filename = _file = QFileInfo(path_to_configuration).fileName();

    QJsonObject json_top_object = json_doc.object();
    QStringList key = json_top_object.keys();

    const QJsonValue& json_file_format_version = json_top_object.value("file_format_version");
    const Version version = Version(
        json_file_format_version == QJsonValue::Undefined ? "1.1.0" : json_file_format_version.toString().toUtf8().constData());

    QJsonValue configuration_entry_value = json_top_object.value(key[0]);
    QJsonObject configuration_entry_object = configuration_entry_value.toObject();

    if (version <= Version("1.1.0")) {
        _name = filename.left(filename.length() - 5);
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

    if (options_value != QJsonValue::Undefined && version > FILE_FORMAT_VERSION) {
        QMessageBox alert;
        alert.setWindowTitle("Could not load configuration file!");
        alert.setText(
            format(
                "The \"%s\" configuration was created with a newer version of Vulkan Configurator. The configuration is discarded",
                _name.toUtf8().constData())
                .c_str());
        alert.setInformativeText("Use Vulkan Configurator from the latest Vulkan SDK to resolve the issue.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
        return nullptr;
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
        layer = Configurator::Get().FindLayerNamed(layers[layer_index]);
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

bool Configuration::Save() const {
    QJsonObject root;
    root.insert("file_format_version", FILE_FORMAT_VERSION.str().c_str());

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

        // Loop through the actual settings
        for (std::size_t setting_index = 0, setting_count = layer._layer_settings.size(); setting_index < setting_count;
             setting_index++) {
            QJsonObject setting;
            const LayerSetting& layer_setting = layer._layer_settings[setting_index];

            setting.insert("name", layer_setting.label);
            setting.insert("description", layer_setting.description);

            switch (layer_setting.type) {
                case SETTING_STRING:
                case SETTING_SAVE_FILE:
                case SETTING_LOAD_FILE:
                case SETTING_SAVE_FOLDER:
                case SETTING_BOOL:
                case SETTING_BOOL_NUMERIC:
                case SETTING_VUID_FILTER:
                    setting.insert("type", GetSettingTypeToken(layer_setting.type));
                    setting.insert("default", layer_setting.value);
                    break;

                case SETTING_EXCLUSIVE_LIST: {
                    setting.insert("type", GetSettingTypeToken(layer_setting.type));
                    setting.insert("default", layer_setting.value);

                    QJsonObject options;
                    for (int i = 0; i < layer_setting.exclusive_labels.size(); i++)
                        options.insert(layer_setting.exclusive_values[i], layer_setting.exclusive_labels[i]);
                    setting.insert("options", options);
                } break;

                case SETTING_INCLUSIVE_LIST: {
                    setting.insert("type", GetSettingTypeToken(layer_setting.type));

                    QJsonObject options;
                    for (int i = 0; i < layer_setting.inclusive_labels.size(); i++)
                        options.insert(layer_setting.inclusive_values[i], layer_setting.inclusive_labels[i]);
                    setting.insert("options", options);

                    QJsonArray defaults;
                    if (!layer_setting.value.isEmpty()) {
                        QStringList list = layer_setting.value.split(",");
                        for (int i = 0; i < list.size(); i++) defaults.append(list[i]);
                    }

                    setting.insert("default", defaults);
                } break;

                // There is a string field that is actually a complicted series of number or
                // ranges of numbers. We should at some point add this to allow more error free editing of it.
                case SETTING_RANGE_INT:
                default:
                    assert(0);
                    break;
            }

            json_settings.insert(layer_setting.name, setting);
        }

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
    const QString path_to_configuration = Configurator::Get().path.GetFullPath(PATH_CONFIGURATION, _name);

    QFile json_file(path_to_configuration);
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
