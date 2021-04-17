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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "override.h"
#include "util.h"
#include "platform.h"
#include "registry.h"

#include <QString>
#include <QJsonArray>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <vulkan/vulkan.h>

#include <cstdio>

// Create and write VkLayer_override.json file
bool WriteLayersOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                         const Configuration& configuration, const std::string& layers_path) {
    assert(!layers_path.empty());
    assert(QFileInfo(layers_path.c_str()).absoluteDir().exists());

    bool has_missing_layers = false;

    const QStringList& path_gui = ConvertString(environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI));
    const QStringList& path_env = ConvertString(environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV));

    QStringList layer_override_paths;
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter& parameter = configuration.parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer == nullptr) {
            has_missing_layers = true;
            continue;
        }

        // Extract just the path
        assert(!layer->path.empty());
        const QFileInfo file(layer->path.c_str());
        const std::string absolute_path(ConvertNativeSeparators(file.absolutePath().toStdString()).c_str());

        // Make sure the path is not already in the list
        if (layer_override_paths.contains(absolute_path.c_str())) continue;

        if (!path_gui.contains(absolute_path.c_str()) && !path_env.contains(absolute_path.c_str())) continue;

        // Okay, add to the list
        layer_override_paths << absolute_path.c_str();
    }

    QJsonArray json_paths;
    for (int i = 0, n = layer_override_paths.count(); i < n; ++i) {
        json_paths.append(ConvertNativeSeparators(layer_override_paths[i].toStdString()).c_str());
    }

    QJsonArray json_overridden_layers;
    QJsonArray json_excluded_layers;
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter& parameter = configuration.parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.state == LAYER_STATE_OVERRIDDEN)
            json_overridden_layers.append(parameter.key.c_str());
        else if (parameter.state == LAYER_STATE_EXCLUDED)
            json_excluded_layers.append(parameter.key.c_str());
    }

    QJsonObject disable;
    disable.insert("DISABLE_VK_LAYER_LUNARG_override", "1");

    QJsonObject layer;
    layer.insert("name", "VK_LAYER_LUNARG_override");
    layer.insert("type", "GLOBAL");
    layer.insert("api_version", environment.api_version.str().c_str());
    layer.insert("implementation_version", "1");
    layer.insert("description", "LunarG Override Layer");
    layer.insert("override_paths", json_paths);
    layer.insert("component_layers", json_overridden_layers);
    layer.insert("blacklisted_layers", json_excluded_layers);
    layer.insert("disable_environment", disable);

    // This has to contain something, or it will apply globally!
    if (environment.UseApplicationListOverrideMode() && environment.HasOverriddenApplications()) {
        const std::vector<Application>& applications = environment.GetApplications();

        QJsonArray json_applist;
        for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
            if (!applications[i].override_layers) continue;

            const std::string& executable_path(
                ConvertNativeSeparators(QFileInfo(applications[i].executable_path.c_str()).absoluteFilePath().toStdString()));
            assert(QFileInfo(executable_path.c_str()).exists());
            json_applist.append(executable_path.c_str());
        }

        layer.insert("app_keys", json_applist);
    }

    QJsonObject root;
    root.insert("file_format_version", "1.1.2");
    root.insert("layer", layer);
    QJsonDocument doc(root);

    QFile json_file(layers_path.c_str());
    const bool result_layers_file = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result_layers_file);
    json_file.write(doc.toJson());
    json_file.close();

    return result_layers_file;
}

// Create and write vk_layer_settings.txt file
bool WriteSettingsOverride(const Environment& environment, const std::vector<Layer>& available_layers,
                           const Configuration& configuration, const std::string& settings_path) {
    assert(!settings_path.empty());
    assert(QFileInfo(settings_path.c_str()).absoluteDir().exists());

    QFile file(settings_path.c_str());
    const bool result_settings_file = file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result_settings_file);
    QTextStream stream(&file);

    bool has_missing_layers = false;

    // Loop through all the layers
    for (std::size_t j = 0, n = configuration.parameters.size(); j < n; ++j) {
        const Parameter& parameter = configuration.parameters[j];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer == nullptr) {
            has_missing_layers = true;
            continue;
        }

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        stream << "\n";
        stream << "# " << layer->key.c_str() << "\n";

        QString short_layer_name(layer->key.c_str());
        short_layer_name.remove("VK_LAYER_");
        QString lc_layer_name = short_layer_name.toLower();

        for (std::size_t i = 0, m = parameter.settings.Size(); i < m; ++i) {
            const SettingData& setting_data = parameter.settings[i];

            // Skip missing settings
            if (FindSettingMeta<SettingMetaInt>(layer->settings, setting_data.key.c_str()) == nullptr) continue;

            stream << lc_layer_name << "." << setting_data.key.c_str() << " = ";
            switch (setting_data.type) {
                case SETTING_GROUP: {
                    break;
                }
                case SETTING_LOAD_FILE:
                case SETTING_SAVE_FILE:
                case SETTING_SAVE_FOLDER: {
                    stream << ReplaceBuiltInVariable(static_cast<const SettingDataString&>(setting_data).value.c_str()).c_str();
                    break;
                }
                case SETTING_FRAMES: {
                    const SettingDataFrames& data = static_cast<const SettingDataFrames&>(setting_data);
                    const SettingMetaFrames* meta = FindSettingMeta<SettingMetaFrames>(layer->settings, data.key.c_str());
                    if (meta->IsValid(data)) {
                        stream << data.value.c_str();
                    } else {
                        stream << meta->default_value.c_str();
                    }
                    break;
                }
                case SETTING_ENUM:
                case SETTING_STRING: {
                    stream << static_cast<const SettingDataString&>(setting_data).value.c_str();
                    break;
                }
                case SETTING_INT: {
                    const SettingDataInt& data = static_cast<const SettingDataInt&>(setting_data);
                    const SettingMetaInt* meta = FindSettingMeta<SettingMetaInt>(layer->settings, data.key.c_str());

                    if (meta->IsValid(data)) {
                        stream << data.value;
                    } else {
                        stream << meta->default_value;
                    }

                    break;
                }
                case SETTING_FLOAT: {
                    const SettingDataFloat& data = static_cast<const SettingDataFloat&>(setting_data);
                    const SettingMetaFloat* meta = FindSettingMeta<SettingMetaFloat>(layer->settings, data.key.c_str());

                    const std::string float_format = meta->GetFloatFormat();

                    if (meta->IsValid(data)) {
                        stream << format(float_format.c_str(), data.value).c_str();
                    } else {
                        stream << format(float_format.c_str(), meta->default_value).c_str();
                    }

                    break;
                }
                case SETTING_BOOL_NUMERIC_DEPRECATED: {
                    stream << (static_cast<const SettingDataBool&>(setting_data).value ? "1" : "0");
                    break;
                }
                case SETTING_BOOL: {
                    stream << (static_cast<const SettingDataBool&>(setting_data).value ? "TRUE" : "FALSE");
                    break;
                }
                case SETTING_LIST: {
                    const SettingDataList& setting_object = static_cast<const SettingDataList&>(setting_data);

                    for (std::size_t i = 0, n = setting_object.value.size(); i < n; ++i) {
                        if (!setting_object.value[i].enabled) continue;

                        if (i != 0) stream << ",";
                        if (setting_object.value[i].key.empty()) {
                            stream << setting_object.value[i].number;
                        } else {
                            stream << setting_object.value[i].key.c_str();
                        }
                    }

                    break;
                }
                case SETTING_FLAGS: {
                    const SettingDataFlags& setting_object = static_cast<const SettingDataFlags&>(setting_data);

                    for (std::size_t i = 0, n = setting_object.value.size(); i < n; ++i) {
                        stream << setting_object.value[i].c_str();
                        if (i < n - 1) stream << ",";
                    }

                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }

            stream << "\n";
        }
    }
    file.close();

    return result_settings_file && !has_missing_layers;
}

bool OverrideConfiguration(const Environment& environment, const std::vector<Layer>& available_layers,
                           const Configuration& configuration) {
    const std::string layers_path = environment.paths.GetFullPath(PATH_OVERRIDE_LAYERS);
    const std::string settings_path = environment.paths.GetFullPath(PATH_OVERRIDE_SETTINGS);

    // Clean up
    SurrenderConfiguration(environment);

    // VkLayer_override.json
    const bool result_layers = WriteLayersOverride(environment, available_layers, configuration, layers_path);

    // vk_layer_settings.txt
    const bool result_settings = WriteSettingsOverride(environment, available_layers, configuration, settings_path);

    // On Windows only, we need to write these values to the registry
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    AppendRegistryEntriesForLayers(environment.running_as_administrator, layers_path.c_str(), settings_path.c_str());
#endif

    return result_settings && result_layers;
}

bool EraseLayersOverride(const std::string& layers_path) { return std::remove(layers_path.c_str()) == 0; }

bool EraseSettingsOverride(const std::string& settings_path) { return std::remove(settings_path.c_str()) == 0; }

bool SurrenderConfiguration(const Environment& environment) {
    const std::string layers_path = environment.paths.GetFullPath(PATH_OVERRIDE_LAYERS);
    const std::string settings_path = environment.paths.GetFullPath(PATH_OVERRIDE_SETTINGS);

    const bool result_layers = EraseLayersOverride(layers_path);
    const bool result_settings = EraseSettingsOverride(settings_path);

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    RemoveRegistryEntriesForLayers(environment.running_as_administrator, layers_path.c_str(), settings_path.c_str());
#endif

    return result_layers && result_settings;
}

bool HasOverride(const Environment& environment) {
    const std::string settings_path = environment.paths.GetFullPath(PATH_OVERRIDE_SETTINGS);
    const std::string layers_path = environment.paths.GetFullPath(PATH_OVERRIDE_LAYERS);

    const QFileInfo override_settings_file_info(layers_path.c_str());
    const QFileInfo override_layers_file_info(settings_path.c_str());

    return override_settings_file_info.exists() || override_layers_file_info.exists();
}
