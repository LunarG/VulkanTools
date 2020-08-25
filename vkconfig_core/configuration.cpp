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
#include "../vkconfig/configurator.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

#include <windows.h>

#include <cassert>

Configuration::Configuration() : _preset(ValidationPresetNone) {}

///////////////////////////////////////////////////////////////////////////////
// Load from a configuration file (.json really)
bool Configuration::Load(const char* full_path) {
    assert(full_path);

    OutputDebugString("LOG: Configuration load\n");
    OutputDebugString(full_path);
    OutputDebugString("\n");

    QFile file(full_path);
    bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    assert(result);
    if (!result) return false;

    QString json_text = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument json_doc;
    QJsonParseError parse_error;
    json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
        return false;
    }

    QJsonObject json_top_object = json_doc.object();
    QStringList key = json_top_object.keys();

    if (key.isEmpty()) {  // Not a valid configuration Json file.
        return false;
    }

    // The file name overrides the stored name. Otherwise
    // we can end up with duplicate profiles
    QString filename = QFileInfo(full_path).fileName();
    _name = filename.left(filename.length() - 5);

    QJsonValue configuration_entry_value = json_top_object.value(key[0]);
    QJsonObject configuration_entry_object = configuration_entry_value.toObject();

    // Build the list of blacklisted layers, check each one to see if it's present,
    // If not present, don't add it to the list
    QJsonValue excluded_value = configuration_entry_object.value("blacklisted_layers");
    QJsonArray excluded_array = excluded_value.toArray();
    for (int i = 0; i < excluded_array.size(); i++) {
        _excluded_layers << excluded_array[i].toString();
    }

    QJsonValue preset_index = configuration_entry_object.value("preset");
    _preset = static_cast<ValidationPreset>(preset_index.toInt());

    QJsonValue editorState = configuration_entry_object.value("editor_state");
    _setting_tree_state = editorState.toVariant().toByteArray();

    QJsonValue description = configuration_entry_object.value("description");
    assert(description != QJsonValue::Undefined);
    _description = description.toString();

    QJsonValue options_value = configuration_entry_object.value("layer_options");

    QJsonObject layer_objects = options_value.toObject();
    QStringList layer_list = layer_objects.keys();

    for (int layer_index = 0; layer_index < layer_list.length(); layer_index++) {
        QJsonValue layer_value = layer_objects.value(layer_list[layer_index]);
        QJsonObject layer_object = layer_value.toObject();

        // To match the layer we just need the name, paths are not
        // hard-matched to the configuration.
        // Find this in our lookup of layers. The standard layers are listed first
        const Layer* layer = Configurator::Get().FindLayer(layer_list[layer_index]);
        if (layer == nullptr) {  // If not found, we have a layer missing....
            continue;
        }

        assert(layer->IsValid());

        OverriddenLayer overridden_layer;
        overridden_layer.name = layer->_name;
        overridden_layer.path = layer->_layer_path;
        overridden_layer.type = layer->_layer_type;

        QJsonValue layerRank = layer_object.value("layer_rank");
        overridden_layer.rank = layerRank.toInt();
        overridden_layer.state = LAYER_STATE_OVERRIDDEN;  // Always because it's present in the file

        // Load the layer
        Layer::LoadSettingsFromJson(layer_object, overridden_layer.settings);
    }

    // We need to sort the layers by their rank. The json sorts alphebetically and we
    // need to undo it.... A bubble quick sort is fine, it's a small list
    if (_overridden_layers.size() > 1) {
        for (std::size_t i = 0, m = _overridden_layers.size() - 1; i < m; i++) {
            for (std::size_t j = i + 1, n = _overridden_layers.size(); j < n; j++) {
                if (_overridden_layers[i].rank > _overridden_layers[j].rank) {
                    std::swap(_overridden_layers[i], _overridden_layers[j]);
                }
            }
        }
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
// This saves or resaves a configuration. Bear in mind it is called everytime
// any edit is made to a configuration at all.
bool Configuration::Save() {
    // Build the json document
    QJsonArray excluded_list;
    for (int i = 0, n = _excluded_layers.size(); i < n; i++) excluded_list.append(_excluded_layers[i]);

    QJsonObject layer_list;  // This list of layers

    for (std::size_t layer_index = 0, layer_count = _overridden_layers.size(); layer_index < layer_count; ++layer_index) {
        const Layer& layer = *Configurator::Get().FindLayer(_overridden_layers[layer_index].name);
        assert(layer.IsValid());

        QJsonObject json_settings;

        // Rank goes in here with settings
        json_settings.insert("layer_rank", layer._rank);

        const std::vector<LayerSetting>& settings = layer._settings;

        // Loop through the actual settings
        for (std::size_t setting_index = 0, setting_count = settings.size(); setting_index < setting_count; ++setting_index) {
            QJsonObject setting;
            const LayerSetting& layer_setting = layer._settings[setting_index];

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

    //////////////////////////////////////////////////////////
    // Assemble the json
    QJsonObject root;
    QJsonObject json_configuration;
    json_configuration.insert("blacklisted_layers", excluded_list);
    json_configuration.insert("description", _description);
    json_configuration.insert("preset", _preset);
    json_configuration.insert("editor_state", _setting_tree_state.data());
    json_configuration.insert("layer_options", layer_list);
    root.insert(_name, json_configuration);
    QJsonDocument doc(root);

    ///////////////////////////////////////////////////////////
    // Write it out - file name is same as name. If it's been
    // changed, this corrects the behavior.
    const QString& full_path = GetFullPath(DIRECTORY_CONFIGURATION, _name);
    OutputDebugString("LOG: Configuration save\n");
    OutputDebugString(full_path.toUtf8().constData());
    OutputDebugString("\n");

    qDebug() << "CONFIGURATION SAVE";
    qDebug() << full_path;
    QFile file(full_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox alert;
        alert.setText("Could not save configuration file!");
        alert.setWindowTitle(_name);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

///////////////////////////////////////////////////////////
// Find the layer if it exists.
OverriddenLayer* Configuration::FindOverriddenLayer(const QString& layer_name, const QString& full_path) {
    assert(!layer_name.isEmpty());
    assert(!full_path.isEmpty());

    for (std::size_t i = 0, n = _overridden_layers.size(); i < n; i++)
        if (_overridden_layers[i].name == layer_name && _overridden_layers[i].path == full_path) return &_overridden_layers[i];

    return nullptr;
}

///////////////////////////////////////////////////////////
// Find the layer if it exists. Only care about the name
OverriddenLayer* Configuration::FindOverriddenLayer(const QString& layer_name) {
    assert(!layer_name.isEmpty());

    for (int i = 0; i < _overridden_layers.size(); i++)
        if (_overridden_layers[i].name == layer_name) return &_overridden_layers[i];

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
    duplicate->_overridden_layers = _overridden_layers;
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
    int new_rank = 0;

    std::vector<OverriddenLayer> overridden_layers;

    for (std::size_t layer_index = 0, layer_count = _overridden_layers.size(); layer_index < layer_count; ++layer_index) {
        OverriddenLayer& overridden_layer = _overridden_layers[layer_index];

        const Layer* layer = Configurator::Get().FindLayer(overridden_layer.name);
        if (layer == nullptr) {
            continue;
        }

        if (!layer->IsValid()) {
            continue;
        }

        if (overridden_layer.state == LAYER_STATE_EXCLUDED) {
            _excluded_layers << overridden_layer.name;
            continue;
        }

        if (!overridden_layer.state == LAYER_STATE_APPLICATION_CONTROLLED) {
            continue;
        }

        overridden_layer.rank = new_rank++;
        overridden_layers.push_back(overridden_layer);
    }

    std::swap(_overridden_layers, overridden_layers);
}

bool Configuration::IsValid() const {
    Configurator& configurator = Configurator::Get();

    if (_excluded_layers.empty() && _overridden_layers.empty()) return false;

    for (std::size_t i = 0, n = _overridden_layers.size(); i < n; ++i) {
        if (configurator.FindLayer(_overridden_layers[i].name) == nullptr) return false;
    }

    for (int i = 0, n = _excluded_layers.size(); i < n; ++i) {
        if (configurator.FindLayer(_excluded_layers[i]) == nullptr) return false;
    }

    return true;
}
