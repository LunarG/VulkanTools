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

    const QStringList& path_gui = ConvertString(environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI));
    const QStringList& path_env_set = ConvertString(environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV_SET));
    const QStringList& path_env_add = ConvertString(environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV_ADD));

    QStringList layer_system_paths;

    QStringList layer_override_paths;
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter& parameter = configuration.parameters[i];
        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        const Layer* layer = FindByKey(available_layers, parameter.key.c_str());
        if (layer == nullptr) {
            continue;
        }

        // Extract just the path
        assert(!layer->manifest_path.empty());
        const QFileInfo file(layer->manifest_path.c_str());
        const std::string absolute_path(file.absolutePath().toStdString().c_str());

        if (!path_gui.contains(ConvertNativeSeparators(absolute_path.c_str()).c_str()) &&
            !path_env_set.contains(ConvertNativeSeparators(absolute_path.c_str()).c_str()) &&
            !path_env_add.contains(ConvertNativeSeparators(absolute_path.c_str()).c_str())) {
            // Make sure the path is not already in the system path list
            if (layer_system_paths.contains(absolute_path.c_str())) continue;

            layer_system_paths << absolute_path.c_str();
        } else {
            // Make sure the path is not already in the override path list
            if (layer_override_paths.contains(absolute_path.c_str())) continue;

            // Okay, add to the list
            layer_override_paths << absolute_path.c_str();
        }
    }

    const Version version = ComputeMinApiVersion(environment.api_version, configuration.parameters, available_layers);

    QJsonArray json_paths;

    // First add override paths so that they take precedent over system paths
    for (int i = 0, n = layer_override_paths.count(); i < n; ++i) {
        json_paths.append(layer_override_paths[i].toStdString().c_str());
    }

    // Second add system paths, so that a layers configuration can run with both system and user-defined layers
    for (int i = 0, n = layer_system_paths.count(); i < n; ++i) {
        json_paths.append(layer_system_paths[i].toStdString().c_str());
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
    layer.insert("api_version", version.str().c_str());
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
bool WriteSettingsOverride(const std::vector<Layer>& available_layers, const Configuration& configuration,
                           const std::string& settings_path) {
    if (settings_path.empty() || !QFileInfo(settings_path.c_str()).absoluteDir().exists()) {
        fprintf(stderr, "Cannot open file %s\n", settings_path.c_str());
        exit(1);
    };
    QFile file(settings_path.c_str());
    const bool result_settings_file = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!result_settings_file) {
        fprintf(stderr, "Cannot open file %s\n", settings_path.c_str());
        exit(1);
    }
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
        stream << "# " << layer->key.c_str() << "\n\n";

        std::string lc_layer_name = GetLayerSettingPrefix(layer->key);

        for (std::size_t i = 0, m = parameter.settings.size(); i < m; ++i) {
            const SettingData* setting_data = parameter.settings[i];

            // Skip groups - they aren't settings, so not relevant in this output
            if (setting_data->type == SETTING_GROUP) {
                continue;
            }

            // Skip missing settings
            const SettingMeta* meta = FindSetting(layer->settings, setting_data->key.c_str());
            if (meta == nullptr) {
                continue;
            }

            // Skip overriden settings
            if (::CheckSettingOverridden(*meta)) {
                continue;
            }

            stream << "# ";
            stream << meta->label.c_str();
            stream << "\n# =====================\n# <LayerIdentifier>.";
            stream << meta->key.c_str() << "\n";

            // Break up description into smaller words
            std::string description = meta->description;
            std::vector<std::string> words;
            std::size_t pos;
            while ((pos = description.find(" ")) != std::string::npos) {
                words.push_back(description.substr(0, pos));
                description.erase(0, pos + 1);
            }
            if (description.size() > 0) words.push_back(description);
            if (words.size() > 0) {
                stream << "#";
                std::size_t nchars = 2;
                for (auto word : words) {
                    if (word.size() + nchars > 80) {
                        stream << "\n#";
                        nchars = 2;
                    }
                    stream << " " << word.c_str();
                    nchars += (word.size() + 1);
                }
            }
            stream << "\n";

            // If feature has unmet dependency, output it but comment it out
            if (::CheckDependence(*meta, parameter.settings) != SETTING_DEPENDENCE_ENABLE) {
                stream << "#";
            }

            stream << lc_layer_name.c_str() << setting_data->key.c_str() << " = ";
            stream << setting_data->Export(EXPORT_MODE_OVERRIDE).c_str();
            stream << "\n\n";
        }
    }
    file.close();

    return result_settings_file && !has_missing_layers;
}

bool OverrideConfiguration(const Environment& environment, const std::vector<Layer>& available_layers,
                           const Configuration& configuration) {
    const std::string layers_path = GetPath(BUILTIN_PATH_OVERRIDE_LAYERS);
    const std::string settings_path = GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);

    // Clean up
    SurrenderConfiguration(environment);

    // VkLayer_override.json
    const bool result_layers = WriteLayersOverride(environment, available_layers, configuration, layers_path);

    // vk_layer_settings.txt
    const bool result_settings = WriteSettingsOverride(available_layers, configuration, settings_path);

    // On Windows only, we need to write these values to the registry
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    AppendRegistryEntriesForLayers(layers_path.c_str(), settings_path.c_str());
#endif

    return result_settings && result_layers;
}

bool EraseLayersOverride(const std::string& layers_path) { return std::remove(layers_path.c_str()) == 0; }

bool EraseSettingsOverride(const std::string& settings_path) { return std::remove(settings_path.c_str()) == 0; }

bool SurrenderConfiguration(const Environment& environment) {
    (void)environment;

    const std::string layers_path = GetPath(BUILTIN_PATH_OVERRIDE_LAYERS);
    const std::string settings_path = GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);

    const bool result_layers = EraseLayersOverride(layers_path);
    const bool result_settings = EraseSettingsOverride(settings_path);

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    RemoveRegistryEntriesForLayers(layers_path.c_str(), settings_path.c_str());
#endif

    return result_layers && result_settings;
}

bool HasOverride() {
    const std::string layers_path = GetPath(BUILTIN_PATH_OVERRIDE_LAYERS);
    const std::string settings_path = GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);

    const QFileInfo override_settings_file_info(layers_path.c_str());
    const QFileInfo override_layers_file_info(settings_path.c_str());

    return override_settings_file_info.exists() || override_layers_file_info.exists();
}
