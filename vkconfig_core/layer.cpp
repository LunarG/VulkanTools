/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include "is_dll_32.h"
#include "configurator.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QJsonArray>
#include <QDateTime>
#include <QCheckBox>
#include <QDesktopServices>
#include <QFileDialog>

#include <cassert>
#include <string>
#include <algorithm>

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

bool Layer::IsValid() const {
    return file_format_version != Version::NONE && !key.empty() && !binary_path.Empty() && api_version != Version::NONE &&
           !implementation_version.empty();
}

LayerControl Layer::GetActualControl() const {
    if (this->type == LAYER_TYPE_IMPLICIT) {
        if (!this->disable_env.empty()) {
            if (qEnvironmentVariableIsSet(this->disable_env.c_str())) {
                return LAYER_CONTROL_OFF;
            }
        }

        if (!this->enable_env.empty()) {
            if (qgetenv(this->enable_env.c_str()).toStdString() == this->enable_value) {
                return LAYER_CONTROL_ON;
            } else {
                return LAYER_CONTROL_OFF;  // Implicit layer which has an enable env must have it set to be enabled
            }
        }

        return LAYER_CONTROL_ON;  // Implicit layer
    } else {
        return LAYER_CONTROL_OFF;  // Explicit layer
    }
}

std::string Layer::GetActualControlTooltip() const {
    if (this->type == LAYER_TYPE_IMPLICIT) {
        if (!this->disable_env.empty()) {
            if (qEnvironmentVariableIsSet(this->disable_env.c_str())) {
                return format("'%s' is set", this->disable_env.c_str());
            }
        }

        if (!this->enable_env.empty()) {
            const std::string& value = qgetenv(this->enable_env.c_str()).toStdString();
            if (value == this->enable_value) {
                return format("'%s' is set to '%s'.", this->enable_env.c_str(), value.c_str());
            } else {
                return format("Set '%s' to '%s' to enable '%s' by default.", this->enable_env.c_str(), this->enable_value.c_str(),
                              this->key.c_str());
            }
        }

        return format("Set '%s' to disable '%s' by default.", this->disable_env.c_str(), this->key.c_str());
    } else {
        return ::GetDescription(LAYER_CONTROL_AUTO);
    }
}

int Layer::FindPresetIndex(const SettingDataSet& settings) const {
    for (std::size_t i = 0, n = this->presets.size(); i < n; ++i) {
        if (::HasPreset(settings, this->presets[i].settings)) {
            return static_cast<int>(i);
        }
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

LayerLoadStatus Layer::Load(const Path& full_path_to_file, LayerType type, bool request_validate_manifest,
                            const std::map<Path, LayerStatus>& layers_found, ConfiguratorMode configurator_mode) {
    this->type = type;  // Set layer type, no way to know this from the json file

    if (full_path_to_file.Empty()) {
        return LAYER_LOAD_IGNORED;
    }

    QFile file(full_path_to_file.AbsolutePath().c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return LAYER_LOAD_FAILED;
    }

    QString json_text = file.readAll();
    file.close();

    this->manifest_path = full_path_to_file;
    this->last_modified = full_path_to_file.LastModified();

    auto it = layers_found.find(full_path_to_file.AbsolutePath().c_str());
    if (it != layers_found.end()) {
        if (it->second.disabled && it->second.last_modified == this->last_modified) {
            return LAYER_LOAD_FAILED;
        }
    }

    // Convert the text to a JSON document & validate it.
    // It does need to be a valid json formatted file.
    QJsonParseError json_parse_error;
    const QJsonDocument& json_document = QJsonDocument::fromJson(json_text.toUtf8(), &json_parse_error);
    if (json_parse_error.error != QJsonParseError::NoError) {
        return LAYER_LOAD_FAILED;
    }

    // Make sure it's not empty
    if (json_document.isNull() || json_document.isEmpty()) {
        return LAYER_LOAD_FAILED;
    }

    // First check it's a layer manifest, ignore otherwise.
    const QJsonObject& json_root_object = json_document.object();
    if (json_root_object.value("file_format_version") == QJsonValue::Undefined) {
        return LAYER_LOAD_IGNORED;  // Not a layer JSON file
    }
    if (json_root_object.value("layer") == QJsonValue::Undefined) {
        return LAYER_LOAD_IGNORED;  // Not a layer JSON file
    }

    this->file_format_version = ReadVersionValue(json_root_object, "file_format_version");
    if (this->file_format_version.GetMajor() > 1) {
        switch (configurator_mode) {
            default: {
            } break;
            case CONFIGURATOR_MODE_GUI: {
                QMessageBox alert;
                alert.setWindowTitle("Failed to load a layer manifest...");
                alert.setText(format("Unsupported layer file format: %s.", this->file_format_version.str().c_str()).c_str());
                alert.setInformativeText(
                    format("The %s layer is being ignored.", full_path_to_file.AbsolutePath().c_str()).c_str());
                alert.setIcon(QMessageBox::Critical);
                alert.exec();
            } break;
            case CONFIGURATOR_MODE_CMD: {
                fprintf(stderr, "vkconfig: [ERROR] Unsupported layer file format: %s\n", this->file_format_version.str().c_str());
                fprintf(stderr, "\n  (%s layer is ignored\n)", full_path_to_file.AbsolutePath().c_str());
            } break;
        }
        return LAYER_LOAD_INVALID;
    }

    const QJsonObject& json_layer_object = ReadObject(json_root_object, "layer");

    this->key = ReadStringValue(json_layer_object, "name");

    if (this->key == "VK_LAYER_LUNARG_override") {
        return LAYER_LOAD_IGNORED;
    }

    this->api_version = ReadVersionValue(json_layer_object, "api_version");

    JsonValidator validator;

    std::string cached_last_modified;
    if (it != layers_found.end()) {
        cached_last_modified = it->second.last_modified;
    }
    const bool should_validate = request_validate_manifest && ((last_modified != cached_last_modified) || !it->second.validated);
    const bool is_valid = should_validate ? validator.Check(json_text) : true;

    if (!is_valid) {
        switch (configurator_mode) {
            default: {
            } break;
            case CONFIGURATOR_MODE_GUI: {
                QMessageBox alert;
                alert.setWindowTitle("Failed to load a layer manifest...");
                alert.setText(format("%s is not a valid layer file", full_path_to_file.AbsolutePath().c_str()).c_str());
                alert.setInformativeText("Do you want to save the validation log?");
                alert.setIcon(QMessageBox::Critical);
                alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                alert.setDefaultButton(QMessageBox::Yes);
                int result = alert.exec();
                if (result == QMessageBox::Yes) {
                    const QString& selected_path = QFileDialog::getSaveFileName(
                        nullptr, format("Export %s validation log", full_path_to_file.AbsolutePath().c_str()).c_str(),
                        (AbsolutePath(Path::HOME) + "/" + full_path_to_file.Basename() + "_log.txt").c_str(), "Log(*.txt)");
                    QFile log_file(selected_path);
                    const bool result = log_file.open(QIODevice::WriteOnly | QIODevice::Text);
                    if (result) {
                        QDesktopServices::openUrl(QUrl::fromLocalFile(selected_path));
                        log_file.write(validator.message.toStdString().c_str());
                        log_file.close();
                    } else {
                        QMessageBox alert;
                        alert.setWindowTitle("Failed to save layer manifest log...");
                        alert.setText(format("Couldn't not open %s file...", selected_path.toStdString().c_str()).c_str());
                        alert.setIcon(QMessageBox::Critical);
                        alert.exec();
                    }
                }
            } break;
            case CONFIGURATOR_MODE_CMD: {
                fprintf(stderr, "vkconfig: [ERROR] Couldn't validate layer file: %s\n", full_path_to_file.AbsolutePath().c_str());
                fprintf(stderr, "\n%s\n)", validator.message.toStdString().c_str());
            } break;
        }
        return LAYER_LOAD_INVALID;
    }

    const QJsonValue& json_library_path_value = json_layer_object.value("library_path");
    if (json_library_path_value != QJsonValue::Undefined) {
        this->binary_path = json_library_path_value.toString().toStdString();
    }

    const Path& binary = this->manifest_path.AbsoluteDir() + "/" + this->binary_path.AbsolutePath();
    if (::IsDLL32Bit(binary.AbsolutePath())) {
        this->is_32bits = true;
        return LAYER_LOAD_INVALID;
    }

    if (json_layer_object.value("prefix") != QJsonValue::Undefined) {
        this->prefix = ReadStringValue(json_layer_object, "prefix");
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
        this->url = Path(ReadStringValue(json_layer_object, "url"), false);
    }

    if (json_layer_object.value("disable_environment") != QJsonValue::Undefined) {
        const QJsonObject& json_env_object = json_layer_object.value("disable_environment").toObject();
        const QStringList keys = json_env_object.keys();
        this->disable_env = keys[0].toStdString();
    }
    if (json_layer_object.value("enable_environment") != QJsonValue::Undefined) {
        const QJsonObject& json_env_object = json_layer_object.value("enable_environment").toObject();
        const QStringList keys = json_env_object.keys();
        this->enable_env = keys[0].toStdString();
        this->enable_value = ReadStringValue(json_env_object, this->enable_env.c_str());
    }

    const QJsonValue& json_features_value = json_layer_object.value("features");
    if (json_features_value != QJsonValue::Undefined) {
        const QJsonObject& json_features_object = json_features_value.toObject();

        // Load layer settings
        this->settings.clear();
        const QJsonValue& json_settings_value = json_features_object.value("settings");
        if (json_settings_value != QJsonValue::Undefined) {
            this->AddSettingsSet(this->settings, nullptr, json_settings_value);
        }

        // Load layer presets
        this->presets.clear();
        const QJsonValue& json_presets_value = json_features_object.value("presets");
        if (json_presets_value != QJsonValue::Undefined) {
            assert(json_presets_value.isArray());
            const QJsonArray& json_preset_array = json_presets_value.toArray();
            for (int preset_index = 0, preset_count = json_preset_array.size(); preset_index < preset_count; ++preset_index) {
                const QJsonObject& json_preset_object = json_preset_array[preset_index].toObject();

                LayerPreset preset;
                preset.platform_flags = this->platforms;
                ::LoadMetaHeader(preset, json_preset_object);

                const QJsonArray& json_setting_array = ReadArray(json_preset_object, "settings");
                for (int setting_index = 0, setting_count = json_setting_array.size(); setting_index < setting_count;
                     ++setting_index) {
                    this->AddSettingData((SettingDataSet&)preset.settings, json_setting_array[setting_index]);
                }

                this->presets.push_back(preset);
            }
        }

        // Load dependence
        if (json_settings_value != QJsonValue::Undefined) {
            this->AddSettingsDependences(json_settings_value);
        }

        // Load messages
        if (json_settings_value != QJsonValue::Undefined) {
            this->AddSettingsMessages(json_settings_value);
        }
    }

    return this->IsValid() ? LAYER_LOAD_ADDED : LAYER_LOAD_INVALID;  // Not all JSON file are layer JSON valid
}

void CollectDefaultSettingData(const SettingMetaSet& meta_set, SettingDataSet& data_set) {
    for (std::size_t i = 0, n = meta_set.size(); i < n; ++i) {
        SettingMeta* setting_meta = meta_set[i];

        bool setting_found = false;
        for (auto it = data_set.begin(), end = data_set.end(); it != end; ++it) {
            if ((*it)->key == setting_meta->key) {
                setting_found = true;
                break;
            }
        }

        if (!setting_found) {
            SettingData* setting_data = setting_meta->Instantiate();
            data_set.push_back(setting_data);
        }

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
        if (this->key == "VK_LAYER_KHRONOS_validation" && this->api_version > Version(1, 3, 261)) {
            if (key == "enables") {
                continue;
            }
            if (key == "disables") {
                continue;
            }
        }

        const std::string desc = ReadStringValue(json_setting, "description");
        const SettingType type = GetSettingType(ReadStringValue(json_setting, "type").c_str());
        if (type == SETTING_INVALID) {
            continue;
        }

        SettingView view = SETTING_VIEW_STANDARD;
        if (json_setting.value("view") != QJsonValue::Undefined) {
            view = GetSettingView(ReadStringValue(json_setting, "view").c_str());
        } else if (parent != nullptr) {
            view = parent->view;
        }
        if (view == SETTING_VIEW_HIDDEN) {
            continue;
        }

        SettingMeta* setting_meta = Instantiate(settings, key, type);
        setting_meta->platform_flags = parent == nullptr ? this->platforms : parent->platform_flags;
        setting_meta->status = parent == nullptr ? STATUS_STABLE : parent->status;
        setting_meta->view = view;

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
            this->AddSettingsSet(setting_meta->children, setting_meta, json_children);
        }
    }
}

void Layer::AddSettingsDependences(const QJsonValue& json_settings_value) {
    assert(json_settings_value.isArray());
    const QJsonArray& json_array = json_settings_value.toArray();
    for (int i = 0, n = json_array.size(); i < n; ++i) {
        const QJsonObject& json_setting = json_array[i].toObject();

        const std::string key = ReadStringValue(json_setting, "key");

        SettingMeta* setting_meta = FindSetting(this->settings, key.c_str());
        if (setting_meta == nullptr) {
            continue;
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
                    this->AddSettingData(setting_meta->dependence, json_settings_array[j]);
                }
            }
        }

        const QJsonValue& json_children = json_setting.value("settings");
        if (json_children != QJsonValue::Undefined) {
            this->AddSettingsDependences(json_children);
        }
    }
}

void Layer::AddSettingsMessages(const QJsonValue& json_settings_value) {
    assert(json_settings_value.isArray());
    const QJsonArray& json_array = json_settings_value.toArray();
    for (int i = 0, n = json_array.size(); i < n; ++i) {
        const QJsonObject& json_setting = json_array[i].toObject();

        const std::string key = ReadStringValue(json_setting, "key");

        SettingMeta* setting_meta = FindSetting(this->settings, key.c_str());
        if (setting_meta == nullptr) {
            continue;
        }

        const QJsonValue& json_messages_value = json_setting.value("messages");
        if (json_messages_value != QJsonValue::Undefined) {
            setting_meta->messages.clear();

            assert(json_messages_value.isArray());
            const QJsonArray& json_messages_array = json_messages_value.toArray();
            for (int i = 0, n = json_messages_array.size(); i < n; ++i) {
                Message message;
                const QJsonObject& json_message_object = json_messages_array[i].toObject();

                message.key = json_message_object.value("key").toString().toStdString();
                message.version = json_message_object.value("version").toInt();
                message.title = json_message_object.value("title").toString().toStdString();
                message.description = json_message_object.value("description").toString().toStdString();
                message.informative = json_message_object.value("informative").toString().toStdString();
                message.severity = ::GetSeverityType(json_message_object.value("severity").toString().toStdString().c_str());

                const QJsonValue& json_conditions_value = json_message_object.value("conditions");
                assert(json_conditions_value != QJsonValue::Undefined);
                const QJsonArray& json_settings_array = json_conditions_value.toArray();

                message.conditions.clear();
                for (int j = 0, o = json_settings_array.size(); j < o; ++j) {
                    const QJsonObject& json_conditions_object = json_settings_array[j].toObject();
                    Condition condition;

                    SettingDataSet setting;
                    this->AddSettingData(setting, json_conditions_object.value("setting"));
                    condition.setting = setting[0];
                    condition.op =
                        GetConditionOperatorType(json_conditions_object.value("operator").toString().toStdString().c_str());

                    message.conditions.push_back(condition);
                }

                const QJsonValue& json_actions_value = json_message_object.value("actions");
                if (json_actions_value != QJsonValue::Undefined) {
                    const QJsonObject& json_actions_object = json_actions_value.toObject();

                    if (json_actions_object.value("default") != QJsonValue::Undefined) {
                        message.default_action =
                            GetActionType(json_actions_object.value("default").toString().toStdString().c_str());
                    }

                    if (json_actions_object.value("BUTTON0") != QJsonValue::Undefined) {
                        const QJsonObject& json_button0_object = json_actions_object.value("BUTTON0").toObject();
                        const QJsonValue& json_type_value = json_button0_object.value("type");

                        message.actions[ACTION0].type = ::GetButtonType(json_type_value.toString().toStdString().c_str());

                        const QJsonArray& json_changes_array = json_button0_object.value("changes").toArray();
                        for (int j = 0, o = json_changes_array.size(); j < o; ++j) {
                            const QJsonObject& json_changes_object = json_changes_array[j].toObject();
                            Action action;

                            SettingDataSet setting;
                            this->AddSettingData(setting, json_changes_object.value("setting"));
                            action.setting = setting[0];
                            action.op =
                                GetActionOperatorType(json_changes_object.value("operator").toString().toStdString().c_str());

                            message.actions[ACTION0].actions.push_back(action);
                        }
                    }

                    if (json_actions_object.value("BUTTON1") != QJsonValue::Undefined) {
                        const QJsonObject& json_button1_object = json_actions_object.value("BUTTON1").toObject();
                        const QJsonValue& json_type_value = json_button1_object.value("type");

                        message.actions[ACTION1].type = ::GetButtonType(json_type_value.toString().toStdString().c_str());

                        const QJsonArray& json_changes_array = json_button1_object.value("changes").toArray();
                        for (int j = 0, o = json_changes_array.size(); j < o; ++j) {
                            const QJsonObject& json_changes_object = json_changes_array[j].toObject();
                            Action action;

                            SettingDataSet setting;
                            this->AddSettingData(setting, json_changes_object.value("setting"));
                            action.setting = setting[0];
                            action.op =
                                GetActionOperatorType(json_changes_object.value("operator").toString().toStdString().c_str());

                            message.actions[ACTION1].actions.push_back(action);
                        }
                    }
                }

                setting_meta->messages.push_back(message);
            }
        }

        const QJsonValue& json_children = json_setting.value("settings");
        if (json_children != QJsonValue::Undefined) {
            this->AddSettingsMessages(json_children);
        }
    }
}

void Layer::AddSettingData(SettingDataSet& settings_data, const QJsonValue& json_setting_value) {
    const QJsonObject& json_setting_object = json_setting_value.toObject();

    const std::string& key = ReadStringValue(json_setting_object, "key");

    SettingMeta* setting_meta = ::FindSetting(this->settings, key.c_str());
    if (setting_meta == nullptr) {
        return;
    }
    assert(setting_meta);

    SettingData* setting_data = setting_meta->Instantiate();
    const bool result = setting_data->Load(json_setting_object);
    assert(result);

    settings_data.push_back(setting_data);
}
