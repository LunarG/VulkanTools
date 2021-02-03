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
        return format(":/layers/%d", vulkan_version).c_str();
    else
        return ":/layers/latest";
}

const char* Layer::NO_PRESET = "User-Defined Settings";

Layer::Layer() : status(STATUS_STABLE), _layer_type(LAYER_TYPE_EXPLICIT) {}

Layer::Layer(const std::string& key, const LayerType layer_type) : key(key), status(STATUS_STABLE), _layer_type(layer_type) {}

Layer::Layer(const std::string& key, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
             const std::string& implementation_version, const std::string& library_path, const std::string& type)
    : key(key),
      file_format_version(file_format_version),
      _type(type),
      _library_path(library_path),
      _api_version(api_version),
      _implementation_version(implementation_version),
      status(STATUS_STABLE),
      _layer_type(layer_type) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return file_format_version != Version::VERSION_NULL && !key.empty() && !_type.empty() && !_library_path.empty() &&
           _api_version != Version::VERSION_NULL && !_implementation_version.empty();
}

std::string Layer::FindPresetLabel(const SettingDataSet& settings) const {
    for (std::size_t i = 0, n = this->presets.size(); i < n; ++i) {
        if (HasPreset(settings, this->presets[i].settings)) return this->presets[i].label;
    }

    return NO_PRESET;
}

static void LoadMetaHeader(Header& header, const QJsonObject& json_object) {
    header.label = ReadStringValue(json_object, "label");
    header.description = ReadStringValue(json_object, "description");
    if (json_object.value("url") != QJsonValue::Undefined) {
        header.url = ReadStringValue(json_object, "url");
    }
    if (json_object.value("status") != QJsonValue::Undefined) {
        header.status = GetStatusType(ReadStringValue(json_object, "status").c_str());
    }
    if (json_object.value("platforms") != QJsonValue::Undefined) {
        header.platform_flags = GetPlatformFlags(ReadStringArray(json_object, "platforms"));
    }
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
    if (json_root_object.value("file_format_version") == QJsonValue::Undefined) {
        return false;  // Not a layer JSON file
    }
    if (json_root_object.value("layer") == QJsonValue::Undefined) {
        return false;  // Not a layer JSON file
    }

    file_format_version = ReadVersionValue(json_root_object, "file_format_version");
    const QJsonObject& json_layer_object = ReadObject(json_root_object, "layer");

    key = ReadStringValue(json_layer_object, "name");
    _type = ReadStringValue(json_layer_object, "type");

    const QJsonValue& json_library_path_value = json_layer_object.value("library_path");
    if (json_library_path_value != QJsonValue::Undefined) {
        this->_library_path = json_library_path_value.toString().toStdString();
    }

    this->_api_version = ReadVersionValue(json_layer_object, "api_version");
    this->_implementation_version = ReadStringValue(json_layer_object, "implementation_version");
    if (json_layer_object.value("status") != QJsonValue::Undefined) {
        this->status = GetStatusType(ReadStringValue(json_layer_object, "status").c_str());
    }
    this->description = ReadStringValue(json_layer_object, "description");
    if (json_layer_object.value("url") != QJsonValue::Undefined) {
        this->url = ReadStringValue(json_layer_object, "url");
    }

    // Load default layer json file if necessary
    const bool is_missing_layer_data =
        json_layer_object.value("settings") == QJsonValue::Undefined || json_layer_object.value("presets") == QJsonValue::Undefined;
    const bool is_builtin_layer_file =
        full_path_to_file.rfind(":/") == 0;  // Check whether the path start with ":/" for resource file paths.

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

            const std::string key = ReadStringValue(json_setting, "key");
            const SettingType type = GetSettingType(ReadStringValue(json_setting, "type").c_str());
            std::shared_ptr<SettingMeta> meta;

            switch (type) {
                case SETTING_STRING: {
                    SettingMetaString* meta_object = new SettingMetaString(key);
                    meta_object->default_value = ReadStringValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_INT: {
                    SettingMetaInt* meta_object = new SettingMetaInt(key);
                    meta_object->default_value = ReadIntValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_INT_RANGE: {
                    SettingMetaIntRange* meta_object = new SettingMetaIntRange(key);
                    const QJsonObject& json_range_object = ReadObject(json_setting, "default");
                    meta_object->default_min_value = ReadIntValue(json_range_object, "min");
                    meta_object->default_max_value = ReadIntValue(json_range_object, "max");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_LOAD_FILE: {
                    SettingMetaFileLoad* meta_object = new SettingMetaFileLoad(key);
                    if (json_setting.value("filter") != QJsonValue::Undefined) {
                        meta_object->filter = ReadStringValue(json_setting, "filter");
                    }
                    meta_object->default_value = ReadStringValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_SAVE_FILE: {
                    SettingMetaFileSave* meta_object = new SettingMetaFileSave(key);
                    if (json_setting.value("filter") != QJsonValue::Undefined) {
                        meta_object->filter = ReadStringValue(json_setting, "filter");
                    }
                    meta_object->default_value = ReadStringValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_SAVE_FOLDER: {
                    SettingMetaFolderSave* meta_object = new SettingMetaFolderSave(key);
                    meta_object->default_value = ReadStringValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_BOOL: {
                    SettingMetaBool* meta_object = new SettingMetaBool(key);
                    meta_object->default_value = ReadBoolValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_BOOL_NUMERIC_DEPRECATED: {
                    SettingMetaBoolNumeric* meta_object = new SettingMetaBoolNumeric(key);
                    meta_object->default_value = ReadBoolValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_ENUM: {
                    SettingMetaEnum* meta_object = new SettingMetaEnum(key);
                    const QJsonArray& json_array_flags = ReadArray(json_setting, "flags");
                    for (int i = 0, n = json_array_flags.size(); i < n; ++i) {
                        const QJsonObject& json_object = json_array_flags[i].toObject();

                        SettingEnumValue setting_enum_value;
                        setting_enum_value.key = ReadStringValue(json_object, "key");
                        LoadMetaHeader(setting_enum_value, json_object);

                        meta_object->enum_values.push_back(setting_enum_value);
                    }
                    meta_object->default_value = ReadStringValue(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_FLAGS: {
                    SettingMetaFlags* meta_object = new SettingMetaFlags(key);
                    const QJsonArray& json_array_flags = ReadArray(json_setting, "flags");
                    for (int i = 0, n = json_array_flags.size(); i < n; ++i) {
                        const QJsonObject& json_object = json_array_flags[i].toObject();

                        SettingEnumValue setting_enum_value;
                        setting_enum_value.key = ReadStringValue(json_object, "key");
                        LoadMetaHeader(setting_enum_value, json_object);

                        meta_object->enum_values.push_back(setting_enum_value);
                    }
                    meta_object->default_value = ReadStringArray(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                case SETTING_VUID_FILTER: {
                    SettingMetaVUIDFilter* meta_object = new SettingMetaVUIDFilter(key);
                    meta_object->list = ReadStringArray(json_setting, "list");
                    meta_object->default_value = ReadStringArray(json_setting, "default");
                    meta.reset(meta_object);
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }

            LoadMetaHeader(*meta, json_setting);

            settings.data.push_back(meta);

            SettingMeta& setting_meta = settings.Create(key, type);
        }
    } else {
        this->settings = default_layer.settings;
        this->status = default_layer.status;
    }

    // Load layer presets
    const QJsonValue& json_presets_value = json_layer_object.value("presets");
    if (json_presets_value != QJsonValue::Undefined) {
        assert(json_presets_value.isArray());
        const QJsonArray& json_preset_array = json_presets_value.toArray();
        for (int preset_index = 0, preset_count = json_preset_array.size(); preset_index < preset_count; ++preset_index) {
            const QJsonObject& json_preset_object = json_preset_array[preset_index].toObject();

            LayerPreset preset;
            LoadMetaHeader(preset, json_preset_object);

            const QJsonArray& json_setting_array = ReadArray(json_preset_object, "settings");
            for (int setting_index = 0, setting_count = json_setting_array.size(); setting_index < setting_count; ++setting_index) {
                const QJsonObject& json_setting_object = json_setting_array[setting_index].toObject();

                const std::string key = ReadStringValue(json_setting_object, "key");

                SettingData& setting_data = preset.settings.Create(key, settings.Get(key.c_str())->type);
                const bool result = setting_data.Load(json_setting_object);
                assert(result);
            }

            presets.push_back(preset);
        }
    } else {
        presets = default_layer.presets;
    }

    return IsValid();  // Not all JSON file are layer JSON valid
}

void InitSettingDefaultValue(SettingData& setting_data, const SettingMeta& setting_meta) {
    assert(setting_data.GetType() == setting_meta.type);

    switch (setting_meta.type) {
        case SETTING_STRING: {
            const SettingMetaString& meta_object = static_cast<const SettingMetaString&>(setting_meta);
            static_cast<SettingDataString&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_INT: {
            const SettingMetaInt& meta_object = static_cast<const SettingMetaInt&>(setting_meta);
            static_cast<SettingDataInt&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_SAVE_FILE: {
            const SettingMetaFileSave& meta_object = static_cast<const SettingMetaFileSave&>(setting_meta);
            static_cast<SettingDataFileSave&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_LOAD_FILE: {
            const SettingMetaFileLoad& meta_object = static_cast<const SettingMetaFileLoad&>(setting_meta);
            static_cast<SettingDataFileLoad&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_SAVE_FOLDER: {
            const SettingMetaFolderSave& meta_object = static_cast<const SettingMetaFolderSave&>(setting_meta);
            static_cast<SettingDataFolderSave&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_BOOL: {
            const SettingMetaBool& meta_object = static_cast<const SettingMetaBool&>(setting_meta);
            static_cast<SettingDataBool&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_BOOL_NUMERIC_DEPRECATED: {
            const SettingMetaBoolNumeric& meta_object = static_cast<const SettingMetaBoolNumeric&>(setting_meta);
            static_cast<SettingDataBoolNumeric&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_ENUM: {
            const SettingMetaEnum& meta_object = static_cast<const SettingMetaEnum&>(setting_meta);
            static_cast<SettingDataEnum&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_FLAGS: {
            const SettingMetaFlags& meta_object = static_cast<const SettingMetaFlags&>(setting_meta);
            static_cast<SettingDataFlags&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_INT_RANGE: {
            const SettingMetaIntRange& meta_object = static_cast<const SettingMetaIntRange&>(setting_meta);
            static_cast<SettingDataIntRange&>(setting_data).min_value = meta_object.default_min_value;
            static_cast<SettingDataIntRange&>(setting_data).max_value = meta_object.default_max_value;
            break;
        }
        case SETTING_VUID_FILTER: {
            const SettingMetaVUIDFilter& meta_object = static_cast<const SettingMetaVUIDFilter&>(setting_meta);
            static_cast<SettingDataVUIDFilter&>(setting_data).value = meta_object.default_value;
            break;
        }
        default: {
            assert(0);
            break;
        }
    }
}

SettingDataSet CollectDefaultSettingData(const SettingMetaSet& meta) {
    SettingDataSet result;

    for (std::size_t i = 0, n = meta.data.size(); i < n; ++i) {
        const SettingMeta& setting_meta = *meta.data[i];
        SettingData& setting_data = result.Create(setting_meta.key, setting_meta.type);
        InitSettingDefaultValue(setting_data, setting_meta);
    }

    return result;
}
