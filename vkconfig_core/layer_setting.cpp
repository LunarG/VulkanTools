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

#include "layer_setting.h"

#include "util.h"
#include "version.h"

#include <QJsonArray>

#include <cassert>

SettingType GetSettingType(const char* token) {
    for (int i = SETTING_FIRST; i <= SETTING_LAST; ++i) {
        const SettingType type = static_cast<SettingType>(i);
        if (strcmp(token, GetSettingTypeToken(type)) == 0) return type;
    }

    assert(0);  // Unknown token
    return static_cast<SettingType>(-1);
}

const char* GetSettingTypeToken(SettingType type) {
    assert(type >= SETTING_FIRST && type <= SETTING_LAST);

    static const char* table[] = {
        "string",        // SETTING_STRING
        "save_file",     // SETTING_SAVE_FILE
        "load_file",     // SETTING_LOAD_FILE
        "save_folder",   // SETTING_SAVE_FOLDER
        "bool",          // SETTING_BOOL
        "bool_numeric",  // SETTING_BOOL_NUMERIC
        "enum",          // SETTING_EXCLUSIVE_LIST
        "multi_enum",    // SETTING_INCLUSIVE_LIST
        "range",         // SETTING_RANGE_INT
        "vuid_exclude"   // SETTING_VUID_FILTER
    };
    static_assert(countof(table) == SETTING_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[type];
}

LayerSetting* FindSetting(std::vector<LayerSetting>& settings, const char* key) {
    for (std::size_t i = 0, n = settings.size(); i < n; i++) {
        if (settings[i].key == key) {
            return &settings[i];
        }
    }

    return nullptr;
}

bool LoadLayerSettings(const QJsonObject& json_layer_settings, std::vector<LayerSetting>& settings) {
    const QStringList& settings_names = json_layer_settings.keys();

    for (int setting_index = 0, setting_count = settings_names.size(); setting_index < setting_count; ++setting_index) {
        // The layer rank may or may not be here, but it is not a
        // user setting.
        if (settings_names[setting_index] == "layer_rank") continue;

        LayerSetting setting;
        setting.key = settings_names[setting_index];

        const QJsonValue& json_value = json_layer_settings.value(settings_names[setting_index]);
        const QJsonObject& json_object = json_value.toObject();

        // The easy stuff...
        const QJsonValue& json_value_description = json_object.value("description");
        assert(json_value_description != QJsonValue::Undefined);

        setting.description = json_value_description.toString();

        const QJsonValue& json_value_name = json_object.value("name");
        assert(json_value_name != QJsonValue::Undefined);

        setting.label = json_value_name.toString();

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        const QJsonValue& json_value_default = json_object.value("default");
        if (json_value_default.isArray()) {
            const QJsonArray& json_array = json_value_default.toArray();
            for (int a = 0; a < json_array.size(); a++) {
                setting.value += json_array[a].toString();
                if (a != json_array.size() - 1) setting.value += ",";
            }

        } else
            setting.value = json_value_default.toString();

        // Everything from here down revolves around the data type
        // Data types and values start getting a little more involved.
        const QJsonValue& json_value_type = json_object.value("type");
        assert(json_value_type != QJsonValue::Undefined);

        setting.type = GetSettingType(json_value_type.toString().toUtf8().constData());

        // debug_action used to be stored as SETTING_EXCLUSIVE_LIST
        const bool convert_debug_action_to_inclusive =
            SUPPORT_VKCONFIG_2_0_1 && setting.key == "debug_action" && setting.type == SETTING_EXCLUSIVE_LIST;
        if (convert_debug_action_to_inclusive) setting.type = SETTING_INCLUSIVE_LIST;

        switch (setting.type) {
            case SETTING_EXCLUSIVE_LIST:
            case SETTING_INCLUSIVE_LIST: {
                // Now we have a list of options, both the enum for the settings file, and the prompts
                const QJsonValue& json_value_options = json_object.value("options");
                assert(json_value_options != QJsonValue::Undefined);

                const QJsonObject& object = json_value_options.toObject();
                const QStringList& keys = object.keys();
                for (int v = 0; v < keys.size(); v++) {
                    QString key = keys[v];
                    const QString value = object.value(key).toString();

                    // The configuration files used to store VK_DBG_LAYER_DEBUG_OUTPUT isntead of VK_DBG_LAYER_ACTION_DEBUG_OUTPUT
                    if (SUPPORT_VKCONFIG_2_0_1 && key == "VK_DBG_LAYER_DEBUG_OUTPUT") key = "VK_DBG_LAYER_ACTION_DEBUG_OUTPUT";

                    // Remove ignore now that we are an inclusive list instead of exclusive
                    if (convert_debug_action_to_inclusive && key == "VK_DBG_LAYER_ACTION_IGNORE") continue;

                    if (setting.type == SETTING_INCLUSIVE_LIST) {
                        setting.inclusive_values << key;
                        setting.inclusive_labels << value;
                    } else if (setting.type == SETTING_EXCLUSIVE_LIST) {
                        setting.exclusive_values << key;
                        setting.exclusive_labels << value;
                    } else
                        assert(0);
                }
            } break;
            case SETTING_SAVE_FILE: {
                setting.value = ValidatePath(setting.value.toStdString()).c_str();
                setting.value = ReplacePathBuiltInVariables(setting.value.toStdString()).c_str();
            } break;
            case SETTING_LOAD_FILE:
            case SETTING_SAVE_FOLDER:
            case SETTING_BOOL:
            case SETTING_BOOL_NUMERIC:
            case SETTING_VUID_FILTER:
            case SETTING_STRING:
                break;
            default:
                assert(0);
                break;
        }

        settings.push_back(setting);
    }

    struct ParameterCompare {
        bool operator()(const LayerSetting& a, const LayerSetting& b) const { return a.key < b.key; }
    };

    std::sort(settings.begin(), settings.end(), ParameterCompare());

    return true;
}

bool SaveLayerSettings(const std::vector<LayerSetting>& settings, QJsonObject& json_settings) {
    assert(&json_settings);

    // Loop through the actual settings
    for (std::size_t setting_index = 0, setting_count = settings.size(); setting_index < setting_count; ++setting_index) {
        QJsonObject json_setting;
        const LayerSetting& setting = settings[setting_index];

        json_setting.insert("name", setting.label);
        json_setting.insert("description", setting.description);

        switch (setting.type) {
            case SETTING_STRING:
            case SETTING_SAVE_FILE:
            case SETTING_LOAD_FILE:
            case SETTING_SAVE_FOLDER:
            case SETTING_BOOL:
            case SETTING_BOOL_NUMERIC:
            case SETTING_VUID_FILTER:
                json_setting.insert("type", GetSettingTypeToken(setting.type));
                json_setting.insert("default", setting.value);
                break;

            case SETTING_EXCLUSIVE_LIST: {
                json_setting.insert("type", GetSettingTypeToken(setting.type));
                json_setting.insert("default", setting.value);

                QJsonObject options;
                for (int i = 0; i < setting.exclusive_labels.size(); i++)
                    options.insert(setting.exclusive_values[i], setting.exclusive_labels[i]);
                json_setting.insert("options", options);
            } break;

            case SETTING_INCLUSIVE_LIST: {
                json_setting.insert("type", GetSettingTypeToken(setting.type));

                QJsonObject options;
                for (int i = 0; i < setting.inclusive_labels.size(); i++)
                    options.insert(setting.inclusive_values[i], setting.inclusive_labels[i]);
                json_setting.insert("options", options);

                QJsonArray defaults;
                if (!setting.value.isEmpty()) {
                    QStringList list = setting.value.split(",");
                    for (int i = 0; i < list.size(); i++) defaults.append(list[i]);
                }

                json_setting.insert("default", defaults);
            } break;

            // There is a string field that is actually a complicted series of number or
            // ranges of numbers. We should at some point add this to allow more error free editing of it.
            case SETTING_RANGE_INT:
            default:
                assert(0);
                break;
        }

        json_settings.insert(setting.key, json_setting);
    }

    return true;
}
