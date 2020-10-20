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

bool IsStringFound(const std::vector<QString>& data, const QString& token) {
    for (std::size_t i = 0, n = data.size(); i < n; ++i) {
        if (data[i] == token) return true;
    }

    return false;
}

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
        "int",           // SETTING_INT
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

bool LoadLayerSettings(const QJsonValue& json_layer_settings, std::vector<LayerSetting>& settings) {
    assert(json_layer_settings.isArray());

    const QJsonArray& json_settings = json_layer_settings.toArray();

    for (int setting_index = 0, setting_count = json_settings.size(); setting_index < setting_count; ++setting_index) {
        LayerSetting setting;

        const QJsonObject& json_setting = json_settings[setting_index].toObject();

        const QJsonValue& json_key = json_setting.value("key");
        assert(json_key != QJsonValue::Undefined);
        setting.key = json_key.toString();

        const QJsonValue& json_label = json_setting.value("label");
        assert(json_label != QJsonValue::Undefined);
        setting.label = json_label.toString();

        const QJsonValue& json_description = json_setting.value("description");
        assert(json_description != QJsonValue::Undefined);
        setting.description = json_description.toString();

        const QJsonValue& json_type = json_setting.value("type");
        assert(json_type != QJsonValue::Undefined);
        setting.type = GetSettingType(json_type.toString().toStdString().c_str());

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        const QJsonValue& json_default = json_setting.value("default");
        assert(json_default != QJsonValue::Undefined);
        if (json_default.isArray()) {
            const QJsonArray& json_array = json_default.toArray();
            for (int i = 0, n = json_array.size(); i < n; ++i) {
                setting.defaults.push_back(json_array[i].toString());
            }
        } else
            setting.defaults.push_back(json_default.toString());

        switch (setting.type) {
            case SETTING_EXCLUSIVE_LIST:
            case SETTING_INCLUSIVE_LIST: {
                const QJsonValue& json_options = json_setting.value("options");
                assert(json_options != QJsonValue::Undefined);

                const QJsonObject& object = json_options.toObject();
                const QStringList& keys = object.keys();
                for (int v = 0; v < keys.size(); v++) {
                    QString key = keys[v];
                    const QString value = object.value(key).toString();
                    setting.values.push_back(key);
                    setting.labels.push_back(value);
                }
            } break;
            case SETTING_SAVE_FILE: {
                setting.defaults[0] = ValidatePath(setting.defaults[0].toStdString()).c_str();
                setting.defaults[0] = ReplacePathBuiltInVariables(setting.defaults[0].toStdString()).c_str();
            } break;
            case SETTING_LOAD_FILE:
            case SETTING_SAVE_FOLDER:
            case SETTING_BOOL:
            case SETTING_BOOL_NUMERIC:
            case SETTING_VUID_FILTER:
            case SETTING_STRING:
            case SETTING_INT:
                break;
            default:
                assert(0);
                break;
        }

        settings.push_back(setting);
    }

    struct LayerCompare {
        bool operator()(const LayerSetting& a, const LayerSetting& b) const { return a.key < b.key; }
    };

    std::sort(settings.begin(), settings.end(), LayerCompare());

    return true;
}

bool SaveLayerSettings(const std::vector<LayerSetting>& settings, QJsonArray& json_settings) {
    assert(&json_settings);

    // Loop through the actual settings
    for (std::size_t setting_index = 0, setting_count = settings.size(); setting_index < setting_count; ++setting_index) {
        QJsonObject json_setting;
        const LayerSetting& setting = settings[setting_index];

        json_setting.insert("key", setting.key);
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
                json_setting.insert("default", setting.defaults[0]);
                break;

            case SETTING_EXCLUSIVE_LIST: {
                json_setting.insert("type", GetSettingTypeToken(setting.type));
                json_setting.insert("default", setting.defaults[0]);

                QJsonObject options;
                for (std::size_t i = 0, n = setting.labels.size(); i < n; ++i) {
                    options.insert(setting.values[i], setting.labels[i]);
                }

                json_setting.insert("options", options);
            } break;

            case SETTING_INCLUSIVE_LIST: {
                json_setting.insert("type", GetSettingTypeToken(setting.type));

                QJsonObject options;
                for (std::size_t i = 0, n = setting.labels.size(); i < n; ++i) {
                    options.insert(setting.values[i], setting.labels[i]);
                }
                json_setting.insert("options", options);

                QJsonArray defaults;
                for (std::size_t i = 0; i < setting.values.size(); ++i) {
                    defaults.append(setting.values[i]);
                }
                json_setting.insert("default", defaults);
            } break;

            // There is a string field that is actually a complicted series of number or
            // ranges of numbers. We should at some point add this to allow more error free editing of it.
            default:
            case SETTING_RANGE_INT:
                assert(0);
                break;
        }

        json_settings.append(json_setting);
    }

    return true;
}
