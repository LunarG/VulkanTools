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
static bool WriteLayerOverride(const PathManager& path, const std::vector<Application>& applications,
                               const std::vector<Layer>& available_layers, const Configuration& configuration) {
    bool has_missing_layers = false;

    QStringList layer_override_paths;
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter& parameter = configuration.parameters[i];

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        const std::vector<Layer>::const_iterator layer = Find(available_layers, parameter.name);
        if (layer == available_layers.end()) {
            has_missing_layers = true;
            continue;
        }

        // Extract just the path
        const QFileInfo file(layer->_layer_path);
        const QString absolute_path = QDir().toNativeSeparators(file.absolutePath());

        // Make sure the path is not already in the list
        if (layer_override_paths.contains(absolute_path)) continue;

        // Okay, add to the list
        layer_override_paths << absolute_path;
    }

    QJsonArray json_paths;
    for (int i = 0, n = layer_override_paths.count(); i < n; ++i) {
        json_paths.append(QDir::toNativeSeparators(layer_override_paths[i]));
    }

    QJsonArray json_overridden_layers;
    QJsonArray json_excluded_layers;
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        const Parameter& parameter = configuration.parameters[i];
        if (parameter.state == LAYER_STATE_OVERRIDDEN)
            json_overridden_layers.append(parameter.name);
        else if (parameter.state == LAYER_STATE_EXCLUDED)
            json_excluded_layers.append(parameter.name);
    }

    QJsonObject disable;
    disable.insert("DISABLE_VK_LAYER_LUNARG_override", "1");

    QJsonObject layer;
    layer.insert("name", "VK_LAYER_LUNARG_override");
    layer.insert("type", "GLOBAL");
    layer.insert("api_version", "1.2." + QString::number(VK_HEADER_VERSION));
    layer.insert("implementation_version", "1");
    layer.insert("description", "LunarG Override Layer");
    layer.insert("override_paths", json_paths);
    layer.insert("component_layers", json_overridden_layers);
    layer.insert("blacklisted_layers", json_excluded_layers);
    layer.insert("disable_environment", disable);

    // This has to contain something, or it will apply globally!
    if (!applications.empty()) {
        QJsonArray json_applist;
        for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
            if (!applications[i].override_layers) continue;

            json_applist.append(QDir::toNativeSeparators(applications[i].executable_path));
        }

        layer.insert("app_keys", json_applist);
    }

    QJsonObject root;
    root.insert("file_format_version", QJsonValue("1.1.2"));
    root.insert("layer", layer);
    QJsonDocument doc(root);

    const QString override_layers_path = path.GetFullPath(PATH_OVERRIDE_LAYERS);
    assert(QFileInfo(override_layers_path).absoluteDir().exists());

    QFile json_file(override_layers_path);
    const bool result_layers_file = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result_layers_file);
    json_file.write(doc.toJson());
    json_file.close();

    return result_layers_file;
}

// Create and write vk_layer_settings.txt file
static bool WriteLayerSettings(const PathManager& path, const std::vector<Layer>& available_layers,
                               const Configuration& configuration) {
    const QString override_settings_path = path.GetFullPath(PATH_OVERRIDE_SETTINGS);
    assert(QFileInfo(override_settings_path).absoluteDir().exists());

    QFile file(override_settings_path);
    const bool result_settings_file = file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result_settings_file);
    QTextStream stream(&file);

    bool has_missing_layers = false;

    // Loop through all the layers
    for (std::size_t j = 0, n = configuration.parameters.size(); j < n; ++j) {
        const Parameter& parameter = configuration.parameters[j];

        const std::vector<Layer>::const_iterator layer = Find(available_layers, parameter.name);
        if (layer == available_layers.end()) {
            has_missing_layers = true;
            continue;
        }

        if (parameter.state != LAYER_STATE_OVERRIDDEN) continue;

        stream << "\n";
        stream << "# " << layer->name << "\n";

        QString short_layer_name = layer->name;
        short_layer_name.remove("VK_LAYER_");
        QString lc_layer_name = short_layer_name.toLower();

        for (std::size_t i = 0, m = parameter.settings.size(); i < m; ++i) {
            const LayerSetting& setting = parameter.settings[i];

            if (layer->name == "lunarg_gfxreconstruct" && layer->_api_version < Version("1.2.148")) {
                stream << "lunarg_gfxrecon"
                       << "." << setting.key << " = " << setting.value << "\n";
            } else {
                stream << lc_layer_name << "." << setting.key << " = " << setting.value << "\n";
            }
        }
    }
    file.close();

    return result_settings_file && !has_missing_layers;
}

bool SurrenderLayers(const Environment& environment) {
    const QString override_settings_path = environment.paths.GetFullPath(PATH_OVERRIDE_SETTINGS);
    const QString override_layers_path = environment.paths.GetFullPath(PATH_OVERRIDE_LAYERS);

    const bool result_override_settings = std::remove(override_settings_path.toUtf8().constData()) == 0;
    const bool result_override_layers = std::remove(override_layers_path.toUtf8().constData()) == 0;

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    RemoveRegistryEntriesForLayers(environment.running_as_administrator, override_layers_path, override_settings_path);
#endif

    return result_override_layers && result_override_settings;
}

bool OverrideLayers(const Environment& environment, const std::vector<Layer>& available_layers,
                    const Configuration& configuration) {
    // Clean up
    SurrenderLayers(environment);

    // vk_layer_settings.txt
    const bool result_settings = WriteLayerSettings(environment.paths, available_layers, configuration);

    // VkLayer_override.json
    const bool result_override =
        WriteLayerOverride(environment.paths, environment.GetApplications(), available_layers, configuration);

    // On Windows only, we need to write these values to the registry
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
    AppendRegistryEntriesForLayers(environment.running_as_administrator, environment.paths.GetFullPath(PATH_OVERRIDE_LAYERS),
                                   environment.paths.GetFullPath(PATH_OVERRIDE_SETTINGS));
#endif

    return result_settings && result_override;
}

bool HasOverriddenLayers(const Environment& environment) {
    const QString override_settings_path = environment.paths.GetFullPath(PATH_OVERRIDE_SETTINGS);
    const QString override_layers_path = environment.paths.GetFullPath(PATH_OVERRIDE_LAYERS);

    const QFileInfo override_settings_file_info(override_layers_path);
    const QFileInfo override_layers_file_info(override_settings_path);

    return override_settings_file_info.exists() || override_layers_file_info.exists();
}
