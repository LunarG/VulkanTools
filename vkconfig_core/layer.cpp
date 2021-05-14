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

#include "layer.h"
#include "platform.h"
#include "util.h"
#include "path.h"
#include "json.h"
#include "json_validator.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QJsonArray>

#include <cassert>
#include <string>
#include <algorithm>

static std::string GetBuiltinFolder(const Version& version) {
    QDir dir(":/layers");
    dir.setFilter(QDir::Dirs);
    QFileInfoList list = dir.entryInfoList();

    std::vector<int> version_supported;

    for (int i = 0, n = list.size(); i < n; ++i) {
        const std::string value = list[i].baseName().toStdString();
        if (!IsNumber(value)) continue;
        version_supported.push_back(std::atoi(value.c_str()));
    }

    std::sort(version_supported.begin(), version_supported.end());

    const int searched_version = version.GetPatch();

    for (int i = static_cast<int>(version_supported.size()) - 1; i >= 0; --i) {
        const int current_version = version_supported[i];

        if (searched_version < current_version) continue;

        return format(":/layers/%d", current_version);
    }

    return ":/layers/latest";
}

const char* Layer::NO_PRESET = "User-Defined Settings";

Layer::Layer() : status(STATUS_STABLE), type(LAYER_TYPE_EXPLICIT) {}

Layer::Layer(const std::string& key, const LayerType layer_type) : key(key), status(STATUS_STABLE), type(layer_type) {}

Layer::Layer(const std::string& key, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
             const std::string& implementation_version, const std::string& library_path, const std::string& type)
    : key(key),
      file_format_version(file_format_version),
      library_path(library_path),
      api_version(api_version),
      implementation_version(implementation_version),
      status(STATUS_STABLE),
      type(layer_type) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return file_format_version != Version::VERSION_NULL && !key.empty() && !library_path.empty() &&
           api_version != Version::VERSION_NULL && !implementation_version.empty();
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
    if (json_object.value("view") != QJsonValue::Undefined) {
        header.view = GetSettingView(ReadStringValue(json_object, "view").c_str());
    }
    if (json_object.value("platforms") != QJsonValue::Undefined) {
        header.platform_flags = GetPlatformFlags(ReadStringArray(json_object, "platforms"));
    }
    if (json_object.value("expanded") != QJsonValue::Undefined) {
        header.expanded = ReadBoolValue(json_object, "expanded");
    }
}

static void AlertInvalidLayer(const std::string& path, const std::string& text) {
    const std::string title = format("Failed to load %s layer manifest. The layer will be ignored.", path.c_str());

    QMessageBox alert;
    alert.setWindowTitle(title.c_str());
    alert.setText(text.c_str());
    alert.setIcon(QMessageBox::Critical);
    alert.setInformativeText("The layer is being ignored");
    alert.exec();
}

/// Reports errors via a message box. This might be a bad idea?
bool Layer::Load(const std::vector<Layer>& available_layers, const std::string& full_path_to_file, LayerType layer_type) {
    this->type = layer_type;  // Set layer type, no way to know this from the json file

    if (full_path_to_file.empty()) return false;

    QFile file(full_path_to_file.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QString json_text = file.readAll();
    file.close();

    this->path = full_path_to_file;

    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonParseError json_parse_error;
    const QJsonDocument& json_document = QJsonDocument::fromJson(json_text.toUtf8(), &json_parse_error);
    if (json_parse_error.error != QJsonParseError::NoError) {
        return false;
    }

    // Make sure it's not empty
    if (json_document.isNull() || json_document.isEmpty()) {
        return false;
    }

    // First check it's a layer manifest, ignore otherwise.
    const QJsonObject& json_root_object = json_document.object();
    if (json_root_object.value("file_format_version") == QJsonValue::Undefined) {
        return false;  // Not a layer JSON file
    }
    if (json_root_object.value("layer") == QJsonValue::Undefined) {
        return false;  // Not a layer JSON file
    }

    this->file_format_version = ReadVersionValue(json_root_object, "file_format_version");
    if (this->file_format_version.GetMajor() > 1) {
        ::AlertInvalidLayer(full_path_to_file,
                            format("Unsupported layer file format: %s", this->file_format_version.str().c_str()));
        return false;
    }

    const QJsonObject& json_layer_object = ReadObject(json_root_object, "layer");

    this->key = ReadStringValue(json_layer_object, "name");

    if (this->key == "VK_LAYER_LUNARG_override") {
        return false;
    }

    // Check if a layer with the same name is already in the list
    if (FindByKey(available_layers, this->key.c_str()) != nullptr) {
        return false;
    }

    this->api_version = ReadVersionValue(json_layer_object, "api_version");

    const bool is_builtin_layer_file =
        full_path_to_file.rfind(":/") == 0;  // Check whether the path start with ":/" for resource file paths.

    JsonValidator validator;
#if defined(_DEBUG)
    const bool should_validate = (this->api_version > Version(1, 2, 170) && is_builtin_layer_file) || !is_builtin_layer_file;
#else
    const bool should_validate = !is_builtin_layer_file;
#endif
    const bool is_valid = should_validate ? validator.Check(json_text) : true;

    const QJsonValue& json_library_path_value = json_layer_object.value("library_path");
    if (json_library_path_value != QJsonValue::Undefined) {
        this->library_path = json_library_path_value.toString().toStdString();
    }

    this->implementation_version = ReadStringValue(json_layer_object, "implementation_version");
    if (json_layer_object.value("status") != QJsonValue::Undefined) {
        this->status = GetStatusType(ReadStringValue(json_layer_object, "status").c_str());
    }
    this->description = ReadStringValue(json_layer_object, "description");
    if (json_layer_object.value("url") != QJsonValue::Undefined) {
        this->url = ReadStringValue(json_layer_object, "url");
    }

    if (!is_valid && this->key != "VK_LAYER_LUNARG_override") {
        if (!is_builtin_layer_file || (is_builtin_layer_file && this->api_version >= Version(1, 2, 170))) {
            AlertInvalidLayer(full_path_to_file, validator.message.toStdString());
            return false;
        }
    }

    // Load default layer json file if necessary
    const QJsonValue& json_features_value = json_layer_object.value("features");

    Layer default_layer;
    if (json_features_value == QJsonValue::Undefined && !is_builtin_layer_file) {
        const std::string path = GetBuiltinFolder(this->api_version) + "/" + this->key + ".json";
        default_layer.Load(available_layers, path, this->type);
        this->status = default_layer.status;
        this->settings = default_layer.settings;
        this->presets = default_layer.presets;
    } else if (json_features_value != QJsonValue::Undefined) {
        const QJsonObject& json_features_object = json_features_value.toObject();

        // Load layer settings
        const QJsonValue& json_settings_value = json_features_object.value("settings");
        if (json_settings_value != QJsonValue::Undefined) {
            this->AddSettingsSet(this->settings, json_settings_value);
        }

        // Load layer presets
        const QJsonValue& json_presets_value = json_features_object.value("presets");
        if (json_presets_value != QJsonValue::Undefined) {
            assert(json_presets_value.isArray());
            const QJsonArray& json_preset_array = json_presets_value.toArray();
            for (int preset_index = 0, preset_count = json_preset_array.size(); preset_index < preset_count; ++preset_index) {
                const QJsonObject& json_preset_object = json_preset_array[preset_index].toObject();

                LayerPreset preset;
                LoadMetaHeader(preset, json_preset_object);

                const QJsonArray& json_setting_array = ReadArray(json_preset_object, "settings");
                for (int setting_index = 0, setting_count = json_setting_array.size(); setting_index < setting_count;
                     ++setting_index) {
                    this->AddSettingData(preset.settings, json_setting_array[setting_index]);
                }

                this->presets.push_back(preset);
            }
        }
    }

    return this->IsValid();  // Not all JSON file are layer JSON valid
}

static QString ReadAll(const std::string& path) {
    QString json_text;

    QFile file(path.c_str());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        json_text = file.readAll();
        file.close();
    }

    return json_text;
}

static void LoadVUIDs(const Version& version, std::vector<NumberOrString>& value) {
    const std::string vulkan_sdk_path(qgetenv("VULKAN_SDK").toStdString());
    const std::string path = vulkan_sdk_path.empty() ? "/usr" : vulkan_sdk_path;

    QString json_text;

    if (!vulkan_sdk_path.empty() && json_text.isEmpty()) {
        json_text = ReadAll(vulkan_sdk_path + "/share/vulkan/registry/validusage.json");
    }

    if (json_text.isEmpty()) {
        json_text = ReadAll(GetBuiltinFolder(version) + "/validusage.json");
    }

    if (json_text.isEmpty()) {
        return;
    }

    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonParseError json_parse_error;
    const QJsonDocument& json_document = QJsonDocument::fromJson(json_text.toUtf8(), &json_parse_error);

    const QJsonObject& json_root_object = json_document.object();
    if (json_root_object.value("validation") == QJsonValue::Undefined) {
        return;
    }

    const QJsonObject& json_validation_object = json_root_object.value("validation").toObject();

    const QStringList& keys_depth0 = json_validation_object.keys();
    for (int i = 0, n = keys_depth0.size(); i < n; ++i) {
        const QJsonValue& value_depth1 = json_validation_object.value(keys_depth0[i]);
        assert(value_depth1 != QJsonValue::Undefined);

        const QJsonObject& object_depth1 = value_depth1.toObject();
        const QStringList& keys_depth1 = object_depth1.keys();

        for (int j = 0, o = keys_depth1.size(); j < o; ++j) {
            const QJsonValue& value_depth2 = object_depth1.value(keys_depth1[j]);
            assert(value_depth2 != QJsonValue::Undefined);

            const QJsonArray& json_array = value_depth2.toArray();
            for (int l = 0, q = json_array.size(); l < q; ++l) {
                const QString vuid_value = json_array[l].toObject().value("vuid").toString();

                NumberOrString data;
                data.key = vuid_value.toStdString();
                value.push_back(data);
            }
        }
    }
}

void Layer::AddSettingData(SettingDataSet& settings, const QJsonValue& json_setting_value) {
    const QJsonObject& json_setting_object = json_setting_value.toObject();

    const std::string& key = ReadStringValue(json_setting_object, "key");

    const SettingMeta* setting_meta = FindSettingMeta(this->settings, key.c_str());
    assert(setting_meta);

    SettingData& setting_data = settings.Create(key, setting_meta->type);
    switch (setting_data.type) {
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FILE:
        case SETTING_SAVE_FOLDER:
        case SETTING_ENUM:
        case SETTING_FRAMES:
        case SETTING_STRING: {
            static_cast<SettingDataString&>(setting_data).value = ReadStringValue(json_setting_object, "value");
            break;
        }
        case SETTING_INT: {
            static_cast<SettingDataInt&>(setting_data).value = ReadIntValue(json_setting_object, "value");
            break;
        }
        case SETTING_BOOL_NUMERIC_DEPRECATED:
        case SETTING_BOOL: {
            static_cast<SettingDataBool&>(setting_data).value = ReadBoolValue(json_setting_object, "value");
            break;
        }
        case SETTING_LIST: {
            SettingDataList& list = static_cast<SettingDataList&>(setting_data);

            const QJsonArray& array = ReadArray(json_setting_object, "value");
            for (int i = 0, n = array.size(); i < n; ++i) {
                const QJsonObject& object = array[i].toObject();

                const NumberOrString& number_or_string = ReadNumberOrStringValue(object, "key");

                EnabledNumberOrString enabled_string;
                enabled_string.key = number_or_string.key;
                enabled_string.number = number_or_string.number;
                enabled_string.enabled = ReadBoolValue(object, "enabled");

                list.value.push_back(enabled_string);
            }
            break;
        }
        case SETTING_FLAGS: {
            static_cast<SettingDataFlags&>(setting_data).value = ReadStringArray(json_setting_object, "value");
            break;
        }
        default: {
            assert(0);
            break;
        }
    }
}

void Layer::AddSettingsSet(SettingMetaSet& settings, const QJsonValue& json_settings_value) {
    assert(json_settings_value.isArray());
    const QJsonArray& json_array = json_settings_value.toArray();
    for (int i = 0, n = json_array.size(); i < n; ++i) {
        const QJsonObject& json_setting = json_array[i].toObject();

        const std::string key = ReadStringValue(json_setting, "key");
        const SettingType type = GetSettingType(ReadStringValue(json_setting, "type").c_str());
        SettingMeta& setting_meta = settings.Create(key, type);

        const QJsonValue& json_children = json_setting.value("settings");
        if (json_children != QJsonValue::Undefined) {
            this->AddSettingsSet(setting_meta.children, json_children);
        }

        const QJsonValue& json_dependence_value = json_setting.value("dependence");
        if (json_dependence_value != QJsonValue::Undefined) {
            const QJsonObject& json_dependence_object = json_dependence_value.toObject();

            if (json_dependence_object.value("mode") != QJsonValue::Undefined)
                setting_meta.dependence_mode = GetDependenceMode(ReadStringValue(json_dependence_object, "mode").c_str());

            const QJsonValue& json_settings_value = json_dependence_object.value("settings");
            if (json_settings_value != QJsonValue::Undefined) {
                const QJsonArray& json_settings_array = json_settings_value.toArray();
                for (int i = 0, n = json_settings_array.size(); i < n; ++i) {
                    this->AddSettingData(setting_meta.dependence, json_settings_array[i]);
                }
            }
        }

        switch (type) {
            case SETTING_GROUP: {
                break;
            }
            case SETTING_FRAMES:
            case SETTING_STRING: {
                static_cast<SettingMetaString&>(setting_meta).default_value = ReadStringValue(json_setting, "default");
                break;
            }
            case SETTING_INT: {
                SettingMetaInt& meta = static_cast<SettingMetaInt&>(setting_meta);
                meta.default_value = ReadIntValue(json_setting, "default");
                if (json_setting.value("range") != QJsonValue::Undefined) {
                    const QJsonObject& json_setting_range = ReadObject(json_setting, "range");
                    if (json_setting_range.value("min") != QJsonValue::Undefined) {
                        meta.min_value = ReadIntValue(json_setting_range, "min");
                    }
                    if (json_setting_range.value("max") != QJsonValue::Undefined) {
                        meta.max_value = ReadIntValue(json_setting_range, "max");
                    }
                }
                if (json_setting.value("unit") != QJsonValue::Undefined) {
                    meta.unit = ReadStringValue(json_setting, "unit");
                }
                break;
            }
            case SETTING_FLOAT: {
                SettingMetaFloat& meta = static_cast<SettingMetaFloat&>(setting_meta);
                meta.default_value = ReadFloatValue(json_setting, "default");
                if (json_setting.value("range") != QJsonValue::Undefined) {
                    const QJsonObject& json_setting_range = ReadObject(json_setting, "range");
                    if (json_setting_range.value("min") != QJsonValue::Undefined) {
                        meta.min_value = ReadFloatValue(json_setting_range, "min");
                    }
                    if (json_setting_range.value("max") != QJsonValue::Undefined) {
                        meta.max_value = ReadFloatValue(json_setting_range, "max");
                    }
                    if (json_setting_range.value("precision") != QJsonValue::Undefined) {
                        meta.precision = ReadIntValue(json_setting_range, "precision");
                    }
                    if (json_setting_range.value("width") != QJsonValue::Undefined) {
                        meta.width = ReadIntValue(json_setting_range, "width");
                    }
                }
                if (json_setting.value("unit") != QJsonValue::Undefined) {
                    meta.unit = ReadStringValue(json_setting, "unit");
                }
                break;
            }
            case SETTING_SAVE_FOLDER:
            case SETTING_SAVE_FILE:
            case SETTING_LOAD_FILE: {
                SettingMetaFilesystem& meta = static_cast<SettingMetaFilesystem&>(setting_meta);
                if (json_setting.value("filter") != QJsonValue::Undefined) {
                    meta.filter = ReadStringValue(json_setting, "filter");
                }
                meta.default_value = ReadStringValue(json_setting, "default");
                break;
            }
            case SETTING_BOOL_NUMERIC_DEPRECATED:
            case SETTING_BOOL: {
                static_cast<SettingMetaBool&>(setting_meta).default_value = ReadBoolValue(json_setting, "default");
                break;
            }
            case SETTING_FLAGS:
            case SETTING_ENUM: {
                SettingMetaEnumeration& meta = static_cast<SettingMetaEnumeration&>(setting_meta);

                const QJsonArray& json_array_flags = ReadArray(json_setting, "flags");
                for (int i = 0, n = json_array_flags.size(); i < n; ++i) {
                    const QJsonObject& json_object = json_array_flags[i].toObject();

                    SettingEnumValue setting_enum_value;
                    setting_enum_value.key = ReadStringValue(json_object, "key");
                    LoadMetaHeader(setting_enum_value, json_object);

                    if (json_object.value("settings") != QJsonValue::Undefined) {
                        this->AddSettingsSet(setting_enum_value.settings, json_object.value("settings"));
                    }

                    meta.enum_values.push_back(setting_enum_value);
                }
                if (type == SETTING_FLAGS) {
                    static_cast<SettingMetaFlags&>(setting_meta).default_value = ReadStringArray(json_setting, "default");
                } else {
                    static_cast<SettingMetaEnum&>(setting_meta).default_value = ReadStringValue(json_setting, "default");
                }
                break;
            }
            case SETTING_LIST: {
                SettingMetaList& meta = static_cast<SettingMetaList&>(setting_meta);

                if (json_setting.value("list") != QJsonValue::Undefined) {
                    const QJsonArray& json_list_array = ReadArray(json_setting, "list");
                    for (int i = 0, n = json_list_array.size(); i < n; ++i) {
                        NumberOrString key;

                        if (json_list_array[i].isString()) {
                            key.key = json_list_array[i].toString().toStdString();
                        } else {
                            key.number = json_list_array[i].toInt();
                        }

                        meta.list.push_back(key);
                    }
                }

                if (json_setting.value("list_only") != QJsonValue::Undefined) {
                    meta.list_only = ReadBoolValue(json_setting, "list_only");
                }

                if (this->key == "VK_LAYER_KHRONOS_validation") {
                    ::LoadVUIDs(this->api_version, meta.list);
                }

                std::sort(meta.list.begin(), meta.list.end());

                const QJsonArray& json_default = ReadArray(json_setting, "default");
                for (int i = 0, n = json_default.size(); i < n; ++i) {
                    const QJsonObject& json_object = json_default[i].toObject();

                    const NumberOrString& number_or_string = ReadNumberOrStringValue(json_object, "key");

                    EnabledNumberOrString enabled_string;
                    enabled_string.key = number_or_string.key;
                    enabled_string.number = number_or_string.number;
                    enabled_string.enabled = ReadBoolValue(json_object, "enabled");
                    meta.default_value.push_back(enabled_string);
                }

                break;
            }
            default: {
                assert(0);
                break;
            }
        }

        LoadMetaHeader(setting_meta, json_setting);
        if (json_setting.value("env") != QJsonValue::Undefined) {
            setting_meta.env = ReadStringValue(json_setting, "env");
        }
    }
}

void InitSettingDefaultValue(SettingData& setting_data, const SettingMeta& setting_meta) {
    assert(setting_data.type == setting_meta.type);

    switch (setting_meta.type) {
        case SETTING_GROUP: {
            break;
        }
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
        case SETTING_FLOAT: {
            const SettingMetaFloat& meta_object = static_cast<const SettingMetaFloat&>(setting_meta);
            static_cast<SettingDataFloat&>(setting_data).value = meta_object.default_value;
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
        case SETTING_FRAMES: {
            const SettingMetaFrames& meta_object = static_cast<const SettingMetaFrames&>(setting_meta);
            static_cast<SettingDataFrames&>(setting_data).value = meta_object.default_value;
            break;
        }
        case SETTING_LIST: {
            const SettingMetaList& meta_object = static_cast<const SettingMetaList&>(setting_meta);
            static_cast<SettingDataList&>(setting_data).value = meta_object.default_value;
            break;
        }
        default: {
            assert(0);
            break;
        }
    }
}

void CollectDefaultSettingData(const SettingMetaSet& meta_set, SettingDataSet& data_set) {
    for (std::size_t i = 0, n = meta_set.Size(); i < n; ++i) {
        const SettingMeta& setting_meta = meta_set[i];
        SettingData& setting_data = data_set.Create(setting_meta.key, setting_meta.type);
        InitSettingDefaultValue(setting_data, setting_meta);

        CollectDefaultSettingData(setting_meta.children, data_set);

        if (setting_meta.type == SETTING_ENUM || setting_meta.type == SETTING_FLAGS) {
            const SettingMetaEnumeration& setting_meta_enum = static_cast<const SettingMetaEnumeration&>(setting_meta);
            for (std::size_t j = 0, o = setting_meta_enum.enum_values.size(); j < o; ++j) {
                CollectDefaultSettingData(setting_meta_enum.enum_values[j].settings, data_set);
            }
        }
    }
}
