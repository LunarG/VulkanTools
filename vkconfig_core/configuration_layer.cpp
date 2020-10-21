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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "util.h"
#include "configuration_layer.h"

#include <QStringList>
#include <QJsonArray>
/*
bool LoadConfigurationSettings(const QJsonObject& json_layer_settings, ConfigurationLayer& configuration_layer) {
    const QStringList& settings_names = json_layer_settings.keys();

    for (int setting_index = 0, setting_count = settings_names.size(); setting_index < setting_count; ++setting_index) {
        // The layer rank may or may not be here, but it is not a
        // user setting.
        if (settings_names[setting_index] == "layer_rank") continue;

        ConfigurationSetting setting;
        setting.key = settings_names[setting_index];

        const QJsonValue& json_value = json_layer_settings.value(settings_names[setting_index]);
        const QJsonObject& json_object = json_value.toObject();

        const QJsonValue& json_value_name = json_object.value("key");
        assert(json_value_name != QJsonValue::Undefined);
        setting.key = json_value_name.toString();

        // This is either a single value, or a comma delimted set of strings
        // selected from a nonexclusive list
        const QJsonValue& json_value_default = json_object.value("default");
        if (json_value_default.isArray()) {
            const QJsonArray& array = json_value_default.toArray();
            for (int a = 0; a < array.size(); a++) {
                setting.defaults.push_back(array[a].toString());
            }

        } else
            setting.defaults.push_back(json_value_default.toString());

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
                break;
            default:
                assert(0);
                break;
        }

        configuration_layer.settings.push_back(setting);
    }

    // Hack to fix in the future
    if (configuration_layer.name == "VK_LAYER_KHRONOS_validation" && configuration_layer.state == LAYER_STATE_OVERRIDDEN) {
        LayerSetting* searched_setting = FindSetting(configuration_layer.settings, "duplicate_message_limit");
        if (!searched_setting) {
            LayerSetting setting;
            setting.key = "duplicate_message_limit";
            setting.label = "Duplicated messages limit";
            setting.description = "Limit the number of times any single validation message would be reported. Empty is unlimited.";
            setting.type = SETTING_STRING;
            setting.defaults.push_back("10");

            configuration_layer.settings.push_back(setting);
        }
    }

    struct ConfigurationSettingCompare {
        bool operator()(const ConfigurationSetting& a, const ConfigurationSetting& b) const { return a.key < b.key; }
    };

    std::sort(configuration_layer.settings.begin(), configuration_layer.settings.end(), ConfigurationSettingCompare());

    return true;
}
*/
void FilterConfiguratorLayers(std::vector<ConfigurationLayer>& layers, const LayerState state) {
    std::vector<ConfigurationLayer> filtered_configuration_layers;

    for (std::size_t i = 0, n = layers.size(); i < n; ++i) {
        if (layers[i].state == state) continue;

        filtered_configuration_layers.push_back(layers[i]);
    }

    layers = filtered_configuration_layers;
}

std::vector<ConfigurationLayer>::iterator FindConfigurationLayer(std::vector<ConfigurationLayer>& layers,
                                                                 const QString& layer_name) {
    assert(!layer_name.isEmpty());

    for (auto it = layers.begin(); it != layers.end(); ++it) {
        if (it->name == layer_name) {
            return it;
        }
    }

    return layers.end();
}

bool HasMissingLayer(const std::vector<ConfigurationLayer>& configuration_layers, const std::vector<Layer>& layers) {
    for (auto it = layers.begin(), end = layers.end(); it != end; ++it) {
        if (!IsFound(configuration_layers, it->name)) return true;
    }
    return false;
}
