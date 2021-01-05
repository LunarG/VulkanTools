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

#include "layer.h"
#include "platform.h"
#include "util.h"
#include "path.h"
#include "json.h"

#include <QFile>
#include <QMessageBox>
#include <QJsonArray>

#include <cassert>
#include <string>
#include <algorithm>

static std::string GetBuiltinFolder(const Version& version) {
    const std::uint32_t vulkan_version = std::max(static_cast<std::uint32_t>(130), version.GetPatch());

    if (vulkan_version <= 162)
        return format(":/resourcefiles/layers_%d", vulkan_version).c_str();
    else
        return ":/resourcefiles/layers_latest";
}

const char* Layer::NO_PRESET = "User-Defined Settings";

Layer::Layer() {}

Layer::Layer(const std::string& key, const LayerType layer_type) : key(key), _layer_type(layer_type) {}

Layer::Layer(const std::string& key, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
             const std::string& implementation_version, const std::string& library_path, const std::string& type)
    : key(key),
      file_format_version(file_format_version),
      _type(type),
      _library_path(library_path),
      _api_version(api_version),
      _implementation_version(implementation_version),
      _layer_type(layer_type) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return file_format_version != Version::VERSION_NULL && !key.empty() && !_type.empty() && !_library_path.empty() &&
           _api_version != Version::VERSION_NULL && !_implementation_version.empty();
}

std::string Layer::FindPresetLabel(const std::vector<LayerSettingData>& layer_settings) const {
    for (std::size_t i = 0, n = presets.size(); i < n; ++i) {
        if (HasPreset(layer_settings, presets[i].settings)) return presets[i].label;
    }

    return NO_PRESET;
}

/// Reports errors via a message box. This might be a bad idea?
bool Layer::Load(const std::string& full_path_to_file, LayerType layer_type) {
    _layer_type = layer_type;  // Set layer type, no way to know this from the json file

    if (full_path_to_file.empty()) return false;

    QFile file(full_path_to_file.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QString json_text = file.readAll();
    file.close();

    _layer_path = full_path_to_file;

    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonParseError json_parse_error;
    const QJsonDocument& json_document = QJsonDocument::fromJson(json_text.toUtf8(), &json_parse_error);
    if (json_parse_error.error != QJsonParseError::NoError) {
        QMessageBox message_box;
        message_box.setText(json_parse_error.errorString());
        message_box.exec();
        return false;
    }

    // Make sure it's not empty
    if (json_document.isNull() || json_document.isEmpty()) {
        QMessageBox message_box;
        message_box.setText("Json document is empty!");
        message_box.exec();
        return false;
    }

    // Populate key items about the layer
    const QJsonObject& json_root_object = json_document.object();
    file_format_version = ReadVersionValue(json_root_object, "file_format_version");

    const QJsonObject& json_layer_object = ReadObject(json_root_object, "layer");

    key = ReadStringValue(json_layer_object, "name");
    _type = ReadStringValue(json_layer_object, "type");

    const QJsonValue& json_library_path_value = json_layer_object.value("library_path");
    if (json_library_path_value != QJsonValue::Undefined) {
        _library_path = json_library_path_value.toString().toStdString();
    }

    _api_version = ReadVersionValue(json_layer_object, "api_version");
    _implementation_version = ReadStringValue(json_layer_object, "implementation_version");
    description = ReadStringValue(json_layer_object, "description");

    // Load default layer json file if necessary
    const bool is_missing_layer_data =
        json_layer_object.value("settings") == QJsonValue::Undefined || json_layer_object.value("presets") == QJsonValue::Undefined;
    const bool is_builtin_layer_file = full_path_to_file.find(":/resourcefiles/") != std::string::npos;

    Layer default_layer;
    if (is_missing_layer_data && !is_builtin_layer_file) {
        const std::string path = GetBuiltinFolder(_api_version) + "/" + key + ".json";
        default_layer.Load(path, _layer_type);
    }

    // Load layer settings
    const QJsonValue& json_settings_value = json_layer_object.value("settings");
    if (json_settings_value != QJsonValue::Undefined) {
        assert(json_settings_value.isArray());
        const QJsonArray& json_array = json_settings_value.toArray();
        for (int i = 0, n = json_array.size(); i < n; ++i) {
            const QJsonObject& json_setting = json_array[i].toObject();

            LayerSettingMeta setting;

            setting.key = ReadStringValue(json_setting, "key");
            setting.label = ReadStringValue(json_setting, "label");
            setting.description = ReadStringValue(json_setting, "description");
            if (json_setting.value("platforms") != QJsonValue::Undefined)
                setting.platform_flags = GetPlatformFlags(ReadStringArray(json_setting, "platforms"));
            else
                setting.platform_flags = PLATFORM_ALL_BIT;
            setting.type = GetSettingType(ReadStringValue(json_setting, "type").c_str());
            setting.default_value = ReadString(json_setting, "default");

            switch (setting.type) {
                case SETTING_EXCLUSIVE_LIST:
                case SETTING_INCLUSIVE_LIST: {
                    // Now we have a list of options, both the enum for the settings file, and the prompts
                    const QJsonValue& json_value_options = json_setting.value("options");
                    assert(json_value_options != QJsonValue::Undefined);

                    const QJsonObject& object = json_value_options.toObject();
                    const QStringList& keys = object.keys();
                    for (int v = 0; v < keys.size(); v++) {
                        QString key = keys[v];
                        const QString default_value = object.value(key).toString();

                        setting.enum_values << key;
                        setting.enum_labels << default_value;
                    }
                } break;
                case SETTING_LOAD_FILE:
                case SETTING_SAVE_FILE: {
                    setting.default_value = setting.default_value.c_str();
                } break;
                case SETTING_VUID_FILTER: {
                    const QJsonValue& json_value_options = json_setting.value("options");
                    assert(json_value_options != QJsonValue::Undefined);
                    assert(json_value_options.isArray());
                    const QJsonArray& json_value_array = json_value_options.toArray();
                    for (int i = 0, n = json_value_array.size(); i < n; ++i) {
                        setting.enum_values.append(json_value_array[i].toString());
                    }
                } break;
                case SETTING_SAVE_FOLDER:
                case SETTING_BOOL:
                case SETTING_BOOL_NUMERIC:
                case SETTING_STRING:
                case SETTING_INT:
                    break;
                default:
                    assert(0);
                    break;
            }

            settings.push_back(setting);
        }
    } else {
        settings = default_layer.settings;
    }

    // Load layer presets
    const QJsonValue& json_presets_value = json_layer_object.value("presets");
    if (json_presets_value != QJsonValue::Undefined) {
        assert(json_presets_value.isArray());
        const QJsonArray& json_preset_array = json_presets_value.toArray();
        for (int preset_index = 0, preset_count = json_preset_array.size(); preset_index < preset_count; ++preset_index) {
            const QJsonObject& json_preset_object = json_preset_array[preset_index].toObject();

            LayerPreset preset;
            preset.label = ReadStringValue(json_preset_object, "label");
            preset.description = ReadStringValue(json_preset_object, "description");
            preset.platform_flags = GetPlatformFlags(ReadStringArray(json_preset_object, "platforms"));
            preset.status_type = GetStatusType(ReadStringValue(json_preset_object, "status").c_str());

            const QJsonArray& json_setting_array = ReadArray(json_preset_object, "settings");
            for (int setting_index = 0, setting_count = json_setting_array.size(); setting_index < setting_count; ++setting_index) {
                const QJsonObject& json_setting_object = json_setting_array[setting_index].toObject();

                LayerSettingData setting_value;
                setting_value.key = ReadStringValue(json_setting_object, "key");
                setting_value.value = ReadString(json_setting_object, "value");
                preset.settings.push_back(setting_value);
            }

            presets.push_back(preset);
        }
    } else {
        presets = default_layer.presets;
    }

    return IsValid();  // Not all JSON file are layer JSON valid
}

std::vector<LayerSettingData> CollectDefaultSettingData(const std::vector<LayerSettingMeta>& meta) {
    std::vector<LayerSettingData> result;

    for (std::size_t i = 0, n = meta.size(); i < n; ++i) {
        result.push_back(LayerSettingData(meta[i].key.c_str(), meta[i].default_value.c_str()));
    }

    return result;
}
