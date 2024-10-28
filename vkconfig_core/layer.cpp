/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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
#include "type_platform.h"
#include "util.h"
#include "path.h"
#include "json.h"
#include "json_validator.h"
#include "alert.h"
#include "is_dll_32.h"
#include "configurator.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QJsonArray>
#include <QDateTime>
#include <QCheckBox>

#include <cassert>
#include <string>
#include <algorithm>

const char* Layer::NO_PRESET = "User-Defined Settings";

bool operator<(const LayersPathInfo& a, const LayersPathInfo& b) { return a.path.RelativePath() < b.path.RelativePath(); }

bool Found(const std::vector<LayersPathInfo>& data, const Path& path) {
    for (std::size_t i = 0, n = data.size(); i < n; ++i) {
        if (data[i].path == path) {
            return true;
        }
    }

    return false;
}

Layer::Layer() : status(STATUS_STABLE), platforms(PLATFORM_DESKTOP_BIT) {}

Layer::Layer(const std::string& key) : key(key), status(STATUS_STABLE), platforms(PLATFORM_DESKTOP_BIT) {}

Layer::Layer(const std::string& key, const Version& file_format_version, const Version& api_version,
             const std::string& implementation_version, const std::string& library_path)
    : key(key),
      file_format_version(file_format_version),
      binary_path(library_path),
      api_version(api_version),
      implementation_version(implementation_version),
      status(STATUS_STABLE),
      platforms(PLATFORM_DESKTOP_BIT) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return file_format_version != Version::NONE && !key.empty() && !binary_path.Empty() && api_version != Version::NONE &&
           !implementation_version.empty();
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
        case SETTING_LOAD_FOLDER:
            setting_meta = new SettingMetaFolderLoad(*this, key);
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

bool Layer::Load(const Path& full_path_to_file, LayerType type, bool request_validate_manifest,
                 const std::map<Path, std::string>& layers_validated) {
    this->type = type;  // Set layer type, no way to know this from the json file

    if (full_path_to_file.Empty()) {
        return false;
    }

    QFile file(full_path_to_file.AbsolutePath().c_str());
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
        const std::string message = format("Unsupported layer file format: %s", this->file_format_version.str().c_str());
        Alert::LayerInvalid(full_path_to_file, message.c_str());
        return false;
    }

    const QJsonObject& json_layer_object = ReadObject(json_root_object, "layer");

    const std::string& last_modified = full_path_to_file.LastModified();

    this->key = ReadStringValue(json_layer_object, "name");

    if (this->key == "VK_LAYER_LUNARG_override") {
        return false;
    }

    this->api_version = ReadVersionValue(json_layer_object, "api_version");

    JsonValidator validator;

    std::string cached_last_modified;
    auto it = layers_validated.find(this->manifest_path.AbsolutePath());
    if (it != layers_validated.end()) {
        cached_last_modified = it->second;
    }
    const bool should_validate = !this->manifest_path.IsBuiltIn() && last_modified != cached_last_modified;
    const bool is_valid = should_validate && request_validate_manifest ? validator.Check(json_text) : true;

    if (!is_valid) {
        this->validated_last_modified.clear();
        Alert::LayerInvalid(full_path_to_file, validator.message.toStdString().c_str());
        return false;
    } else {
        this->validated_last_modified = last_modified;
    }

    const QJsonValue& json_library_path_value = json_layer_object.value("library_path");
    if (json_library_path_value != QJsonValue::Undefined) {
        this->binary_path = json_library_path_value.toString().toStdString();
    }

    const Path& binary = Path(this->manifest_path.AbsoluteDir()) + "/" + this->binary_path.AbsolutePath();
    if (::IsDLL32Bit(binary.AbsolutePath())) {
        this->is_32bits = true;
        return false;
    }

    /*
        const Path& binary = Path(this->manifest_path.AbsoluteDir()) + "/" + this->binary_path.AbsolutePath();
        if (::IsDLL32Bit(binary.AbsolutePath())) {
            Configurator& configurator = Configurator::Get();
            if (!configurator.Get(HIDE_MESSAGE_ERROR_32BIT)) {
                QMessageBox message;
                message.setIcon(QMessageBox::Information);
                message.setWindowTitle("Trying to load a 32 bit layer...");
                message.setText(format("'%s' refers to a 32 bit layer which is not supported. This layer will be ignored.",
                                       this->manifest_path.AbsolutePath().c_str())
                                    .c_str());
                message.setCheckBox(new QCheckBox("Do not show again."));
                message.exec();
                if (message.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_ERROR_32BIT);
                }
            }
            return false;
        }
    */

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

    if (json_layer_object.value("disable_environment") != QJsonValue::Undefined) {
        const QJsonObject& json_env_object = json_layer_object.value("disable_environment").toObject();
        const QStringList keys = json_env_object.keys();
        this->disable_env = keys[0].toStdString();
        this->disable_value = ReadStringValue(json_env_object, this->disable_env.c_str()) == "1";
    }
    if (json_layer_object.value("enable_environment") != QJsonValue::Undefined) {
        const QJsonObject& json_env_object = json_layer_object.value("enable_environment").toObject();
        const QStringList keys = json_env_object.keys();
        this->enable_env = keys[0].toStdString();
        this->enable_value = ReadStringValue(json_env_object, this->enable_env.c_str()) == "1";
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
                const std::string& value = setting_file.default_value.AbsolutePath();
                const QJsonDocument& doc = ParseJsonFile(value.c_str());

                if (!doc.isNull() && !doc.isEmpty()) {
                    const QJsonObject& json_root_object = doc.object();
                    const std::string schema = json_root_object.value("$schema").toString().toStdString();
                    if (schema.find("https://schema.khronos.org/vulkan/profiles") == std::string::npos) {
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

void Layer::AddSettingData(SettingDataSet& settings_data, const QJsonValue& json_setting_value) {
    const QJsonObject& json_setting_object = json_setting_value.toObject();

    const std::string& key = ReadStringValue(json_setting_object, "key");

    SettingMeta* setting_meta = FindSetting(this->settings, key.c_str());
    assert(setting_meta);

    SettingData* setting_data = setting_meta->Instantiate();
    const bool result = setting_data->Load(json_setting_object);
    assert(result);

    settings_data.push_back(setting_data);
}
