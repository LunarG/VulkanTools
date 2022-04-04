/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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
#include "setting_bool.h"
#include "setting_filesystem.h"
#include "setting_flags.h"
#include "setting_float.h"
#include "setting_frames.h"
#include "setting_group.h"
#include "setting_int.h"
#include "setting_list.h"
#include "setting_string.h"
#include "platform.h"
#include "util.h"
#include "path.h"
#include "json.h"
#include "json_validator.h"
#include "alert.h"

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

    return ":/layers/170";
}

const char* Layer::NO_PRESET = "User-Defined Settings";

Layer::Layer() : status(STATUS_STABLE), platforms(PLATFORM_DESKTOP_BIT), type(LAYER_TYPE_EXPLICIT) {}

Layer::Layer(const std::string& key, const LayerType layer_type)
    : key(key), status(STATUS_STABLE), platforms(PLATFORM_DESKTOP_BIT), type(layer_type) {}

Layer::Layer(const std::string& key, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
             const std::string& implementation_version, const std::string& library_path)
    : key(key),
      file_format_version(file_format_version),
      binary_path(library_path),
      api_version(api_version),
      implementation_version(implementation_version),
      status(STATUS_STABLE),
      platforms(PLATFORM_DESKTOP_BIT),
      type(layer_type) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return file_format_version != Version::VERSION_NULL && !key.empty() && !binary_path.empty() &&
           api_version != Version::VERSION_NULL && !implementation_version.empty();
}

std::string Layer::FindPresetLabel(const SettingDataSet& settings) const {
    for (std::size_t i = 0, n = this->presets.size(); i < n; ++i) {
        if (HasPreset(settings, this->presets[i].settings)) return this->presets[i].label;
    }

    return NO_PRESET;
}

SettingMeta* Layer::Instantiate(SettingMetaSet& meta_set, const std::string& key, const SettingType type) {
    SettingMeta* setting_meta = nullptr;

    switch (type) {
        case SETTING_STRING:
            setting_meta = new SettingMetaString(*this, key);
            break;
        case SETTING_INT:
            setting_meta = new SettingMetaInt(*this, key);
            break;
        case SETTING_FLOAT:
            setting_meta = new SettingMetaFloat(*this, key);
            break;
        case SETTING_GROUP:
            setting_meta = new SettingMetaGroup(*this, key);
            break;
        case SETTING_SAVE_FILE:
            setting_meta = new SettingMetaFileSave(*this, key);
            break;
        case SETTING_LOAD_FILE:
            setting_meta = new SettingMetaFileLoad(*this, key);
            break;
        case SETTING_SAVE_FOLDER:
            setting_meta = new SettingMetaFolderSave(*this, key);
            break;
        case SETTING_BOOL:
            setting_meta = new SettingMetaBool(*this, key);
            break;
        case SETTING_BOOL_NUMERIC_DEPRECATED:
            setting_meta = new SettingMetaBoolNumeric(*this, key);
            break;
        case SETTING_ENUM:
            setting_meta = new SettingMetaEnum(*this, key);
            break;
        case SETTING_FLAGS:
            setting_meta = new SettingMetaFlags(*this, key);
            break;
        case SETTING_FRAMES:
            setting_meta = new SettingMetaFrames(*this, key);
            break;
        case SETTING_LIST:
            setting_meta = new SettingMetaList(*this, key);
            break;
        default:
            assert(0);
            break;
    }

    assert(setting_meta != nullptr);
    this->memory.push_back(std::shared_ptr<SettingMeta>(setting_meta));
    meta_set.push_back(setting_meta);
    return setting_meta;
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

    this->manifest_path = full_path_to_file;

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
        Alert::LayerInvalid(full_path_to_file.c_str(),
                            format("Unsupported layer file format: %s", this->file_format_version.str().c_str()).c_str());
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
    const bool should_validate = (this->api_version >= Version(1, 2, 170) && is_builtin_layer_file) || !is_builtin_layer_file;
#else
    const bool should_validate = !is_builtin_layer_file;
#endif
    const bool is_valid = should_validate ? validator.Check(json_text) : true;

    const QJsonValue& json_library_path_value = json_layer_object.value("library_path");
    if (json_library_path_value != QJsonValue::Undefined) {
        this->binary_path = json_library_path_value.toString().toStdString();
    }

    this->implementation_version = ReadStringValue(json_layer_object, "implementation_version");
    if (json_layer_object.value("status") != QJsonValue::Undefined) {
        this->status = GetStatusType(ReadStringValue(json_layer_object, "status").c_str());
    }
    if (json_layer_object.value("platforms") != QJsonValue::Undefined) {
        this->platforms = GetPlatformFlags(ReadStringArray(json_layer_object, "platforms"));
    }
    this->description = ReadStringValue(json_layer_object, "description");
    if (json_layer_object.value("introduction") != QJsonValue::Undefined) {
        this->introduction = ReadStringValue(json_layer_object, "introduction");
    }
    if (json_layer_object.value("url") != QJsonValue::Undefined) {
        this->url = ReadStringValue(json_layer_object, "url");
    }

    if (!is_valid && this->key != "VK_LAYER_LUNARG_override") {
        if (!is_builtin_layer_file || (is_builtin_layer_file && this->api_version >= Version(1, 2, 170))) {
            Alert::LayerInvalid(full_path_to_file.c_str(), validator.message.toStdString().c_str());
            return false;
        }
    }

    const QJsonValue& json_features_value = json_layer_object.value("features");
    if (json_features_value != QJsonValue::Undefined) {
        const QJsonObject& json_features_object = json_features_value.toObject();

        // Load layer settings
        const QJsonValue& json_settings_value = json_features_object.value("settings");
        if (json_settings_value != QJsonValue::Undefined) {
            AddSettingsSet(this->settings, nullptr, json_settings_value);
        }

        // Load layer presets
        const QJsonValue& json_presets_value = json_features_object.value("presets");
        if (json_presets_value != QJsonValue::Undefined) {
            assert(json_presets_value.isArray());
            const QJsonArray& json_preset_array = json_presets_value.toArray();
            for (int preset_index = 0, preset_count = json_preset_array.size(); preset_index < preset_count; ++preset_index) {
                const QJsonObject& json_preset_object = json_preset_array[preset_index].toObject();

                LayerPreset preset;
                preset.platform_flags = this->platforms;
                preset.status = this->status;
                LoadMetaHeader(preset, json_preset_object);

                const QJsonArray& json_setting_array = ReadArray(json_preset_object, "settings");
                for (int setting_index = 0, setting_count = json_setting_array.size(); setting_index < setting_count;
                     ++setting_index) {
                    AddSettingData((SettingDataSet&)preset.settings, json_setting_array[setting_index]);
                }

                this->presets.push_back(preset);
            }
        }
    }

    // Override old built-in layer settings
    if (!is_builtin_layer_file && this->api_version <= Version(1, 2, 176)) {
        const std::string path = GetBuiltinFolder(this->api_version) + "/" + this->key + ".json";

        Layer default_layer;
        if (default_layer.Load(available_layers, path, this->type)) {
            this->introduction = default_layer.introduction;
            this->url = default_layer.url;
            this->platforms = default_layer.platforms;
            this->status = default_layer.status;
            std::swap(this->settings, default_layer.settings);
            std::swap(this->presets, default_layer.presets);
            this->memory = default_layer.memory;
        }
    }

    return this->IsValid();  // Not all JSON file are layer JSON valid
}

void CollectDefaultSettingData(const SettingMetaSet& meta_set, SettingDataSet& data_set) {
    for (std::size_t i = 0, n = meta_set.size(); i < n; ++i) {
        SettingMeta* setting_meta = meta_set[i];
        SettingData* setting_data = setting_meta->Instantiate();
        data_set.push_back(setting_data);

        CollectDefaultSettingData(setting_meta->children, data_set);

        if (IsEnum(setting_meta->type) || IsFlags(setting_meta->type)) {
            const SettingMetaEnumeration* setting_meta_enum = static_cast<const SettingMetaEnumeration*>(setting_meta);
            for (std::size_t j = 0, o = setting_meta_enum->enum_values.size(); j < o; ++j) {
                CollectDefaultSettingData(setting_meta_enum->enum_values[j].settings, data_set);
            }
        }
    }
}

void Layer::AddSettingsSet(SettingMetaSet& settings, const SettingMeta* parent, const QJsonValue& json_settings_value) {
    assert(json_settings_value.isArray());
    const QJsonArray& json_array = json_settings_value.toArray();
    for (int i = 0, n = json_array.size(); i < n; ++i) {
        const QJsonObject& json_setting = json_array[i].toObject();

        const std::string key = ReadStringValue(json_setting, "key");
        const std::string desc = ReadStringValue(json_setting, "description");
        const SettingType type = GetSettingType(ReadStringValue(json_setting, "type").c_str());
        SettingMeta* setting_meta = Instantiate(settings, key, type);
        setting_meta->platform_flags = parent == nullptr ? this->platforms : parent->platform_flags;
        setting_meta->status = parent == nullptr ? this->status : parent->status;
        if (parent != nullptr) setting_meta->view = parent->view;

        LoadMetaHeader(*setting_meta, json_setting);
        if (json_setting.value("env") != QJsonValue::Undefined) {
            setting_meta->env = ReadStringValue(json_setting, "env");
        }

        const bool result = setting_meta->Load(json_setting);
        assert(result);

        if (setting_meta->type == SETTING_LOAD_FILE) {
            const SettingMetaFileLoad& setting_file = static_cast<const SettingMetaFileLoad&>(*setting_meta);
            if (setting_file.format == "PROFILE") {
                const std::string& value = ReplaceBuiltInVariable(setting_file.default_value);
                const QJsonDocument& doc = ParseJsonFile(value.c_str());

                if (!doc.isNull() && !doc.isEmpty()) {
                    const QJsonObject& json_root_object = doc.object();
                    const std::string schema = json_root_object.value("$schema").toString().toStdString();
                    if (schema.find("https://schema.khronos.org/vulkan/profiles") == std::string::npos) {
                        Alert::FileNotProfile(value.c_str());
                        return;
                    }
                }
            }
        }

        const QJsonValue& json_children = json_setting.value("settings");
        if (json_children != QJsonValue::Undefined) {
            AddSettingsSet(setting_meta->children, setting_meta, json_children);
        }

        const QJsonValue& json_dependence_value = json_setting.value("dependence");
        if (json_dependence_value != QJsonValue::Undefined) {
            const QJsonObject& json_dependence_object = json_dependence_value.toObject();

            if (json_dependence_object.value("mode") != QJsonValue::Undefined)
                setting_meta->dependence_mode = GetDependenceMode(ReadStringValue(json_dependence_object, "mode").c_str());

            const QJsonValue& json_settings_value = json_dependence_object.value("settings");
            if (json_settings_value != QJsonValue::Undefined) {
                const QJsonArray& json_settings_array = json_settings_value.toArray();
                for (int j = 0, o = json_settings_array.size(); j < o; ++j) {
                    AddSettingData(setting_meta->dependence, json_settings_array[j]);
                }
            }
        }
    }
}

void Layer::AddSettingData(SettingDataSet& settings, const QJsonValue& json_setting_value) {
    const QJsonObject& json_setting_object = json_setting_value.toObject();

    const std::string& key = ReadStringValue(json_setting_object, "key");

    SettingMeta* setting_meta = FindSetting(this->settings, key.c_str());
    assert(setting_meta);

    SettingData* setting_data = setting_meta->Instantiate();
    const bool result = setting_data->Load(json_setting_object);
    assert(result);

    settings.push_back(setting_data);
}
