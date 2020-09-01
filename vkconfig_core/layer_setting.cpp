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

#include "util.h"
#include "platform.h"

#include "layer_setting.h"

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

    const char* table[] = {
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

bool operator==(const LayerSetting& a, const LayerSetting& b) {
    if (a.name != b.name)
        return false;
    else if (a.label != b.label)
        return false;
    else if (a.description != b.description)
        return false;
    else if (a.type != b.type)
        return false;
    else if (a.max_value != b.max_value)
        return false;
    else if (a.min_value != b.min_value)
        return false;
    else if (a.exclusive_values != b.exclusive_values)
        return false;
    else if (a.exclusive_labels != b.exclusive_labels)
        return false;
    else if (a.inclusive_values != b.inclusive_values)
        return false;
    else if (a.inclusive_labels != b.inclusive_labels)
        return false;
    else if (a.value != b.value)
        return false;
    return true;
}

bool operator!=(const LayerSetting& a, const LayerSetting& b) { return !(a == b); }

LayerSetting& FindSetting(std::vector<LayerSetting>& settings, const char* name) {
    for (std::size_t i = 0, n = settings.size(); i < n; i++) {
        if (settings[i].name == name) {
            return settings[i];
        }
    }

    assert(0);
    return settings[0];
}

void LoadSettings(QJsonObject& json_layer_settings, std::vector<LayerSetting>& settings) {
    // Okay, how many settings do we have?
    QStringList settings_names = json_layer_settings.keys();

    for (int setting_index = 0, setting_count = settings_names.size(); setting_index < setting_count; setting_index++) {
        // The layer rank may or may not be here, but it is not a
        // user setting.
        if (settings_names[setting_index] == QString("layer_rank")) continue;

        LayerSetting setting;
        setting.name = settings_names[setting_index];

        QJsonValue json_value = json_layer_settings.value(settings_names[setting_index]);
        QJsonObject json_object = json_value.toObject();

        // The easy stuff...
        QJsonValue value = json_object.value("description");
        setting.description = value.toString();

        value = json_object.value("name");
        setting.label = value.toString();

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        value = json_object.value("default");
        if (value.isArray()) {
            QJsonArray array = value.toArray();
            for (int a = 0; a < array.size(); a++) {
                setting.value += array[a].toString();
                if (a != array.size() - 1) setting.value += ",";
            }

        } else
            setting.value = value.toString();

        ///////////////////////////////////////////////////////////////////////
        // Everything from here down revolves around the data type
        // Data types and values start getting a little more involved.
        value = json_object.value("type");

        setting.type = GetSettingType(value.toString().toUtf8().constData());

        switch (setting.type) {
            case SETTING_EXCLUSIVE_LIST: {
                // Now we have a list of options, both the enum for the settings file, and the prompts
                value = json_object.value("options");
                QJsonObject object = value.toObject();
                QStringList keys, values;
                keys = object.keys();
                for (int v = 0; v < keys.size(); v++) {
                    if (!PLATFORM_WINDOWS && keys[v] == "VK_DBG_LAYER_ACTION_DEBUG_OUTPUT") continue;
                    setting.exclusive_values << keys[v];
                    setting.exclusive_labels << object.value(keys[v]).toString();
                }
            } break;
            case SETTING_INCLUSIVE_LIST: {
                // Now we have a list of options, both the enum for the settings file, and the prompts
                value = json_object.value("options");
                QJsonObject object = value.toObject();
                QStringList keys, values;
                keys = object.keys();
                for (int v = 0; v < keys.size(); v++) {
                    if (!PLATFORM_WINDOWS && keys[v] == "VK_DBG_LAYER_ACTION_DEBUG_OUTPUT") continue;
                    setting.inclusive_values << keys[v];
                    setting.inclusive_labels << object.value(keys[v]).toString();
                }
            } break;
            case SETTING_SAVE_FILE:
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
}
