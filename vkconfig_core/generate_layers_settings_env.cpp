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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "generate_layers_settings_env.h"

#include "vulkan_util.h"
#include "setting_flags.h"
#include "layer.h"
#include "configuration.h"

#include <vector>

std::string GenerateSettingsEnv::operator()(const LayerManager& layers, const Configuration* configuration) const {
    std::string text;

    const std::string COMMENT = this->type == GENERATE_SETTINGS_BASH ? "#! " : ":: ";
    const std::string EXPORT = this->type == GENERATE_SETTINGS_BASH ? "export " : "set ";

    text += COMMENT + "Loader Settings:\n";

    const std::vector<std::string>& stderr_log = ::GetLogTokens(this->loader_log_messages_flags);
    const std::string stderr_logs = Merge(stderr_log, ",");

    if (this->loader_log_enabled) {
        text += EXPORT + "VK_LOADER_DEBUG=" + stderr_logs + "\n";
    }

    {
        text += EXPORT + "VK_INSTANCE_LAYERS=";
        std::vector<std::string> layer_list;
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            const Parameter& parameter = configuration->parameters[i];
            if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
                continue;
            }
            if (parameter.control != LAYER_CONTROL_ON) {
                continue;
            }
            layer_list.push_back(parameter.key);
        }
        text += Merge(layer_list, ",").c_str();
        text += "\n";
    }

    text += "\n";

    text += COMMENT + "Layers Settings:\n";

    // Loop through all the layers
    for (std::size_t j = 0, n = configuration->parameters.size(); j < n; ++j) {
        const Parameter& parameter = configuration->parameters[j];
        if (!parameter.override_settings) {
            continue;
        }

        if (!(parameter.platform_flags & (1 << VKC_PLATFORM))) {
            continue;
        }

        if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
            continue;
        }

        if (parameter.control == LAYER_CONTROL_DISCARD || parameter.control == LAYER_CONTROL_OFF) {
            continue;
        }

        const Layer* layer = layers.Find(parameter.key.c_str(), parameter.api_version);
        if (layer == nullptr) {
            if (parameter.control == LAYER_CONTROL_ON) {
                this->Log(LOG_ERROR,
                          format("`%s` layer is set to `%s` in `%s` layers configuration but missing and being ignored\n",
                                 parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str()));
            } else {
                this->Log(LOG_WARN, format("`%s` layer is set to `%s` in `%s` layers configuration but missing and being ignored\n",
                                           parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str()));
            }
            continue;
        }

        if (layer->settings.empty()) {
            continue;
        }

        std::string status;
        if (layer->status != STATUS_STABLE) {
            status = format(" (%s)", GetToken(layer->status));
        }

        std::string platforms = " (" + Merge(::GetPlatformTokens(layer->platforms), ", ") + ")";

        text += "\n";
        text += COMMENT + layer->description.c_str() + "\n";
        text += COMMENT + "==========================================\n";
        text += COMMENT +
                format("%s - %s%s%s\n\n", layer->key.c_str(), layer->api_version.str().c_str(), status.c_str(), platforms.c_str());

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

            if (meta->view == SETTING_VIEW_HIDDEN) {
                continue;
            }

            // Skip overriden settings
            if (::CheckSettingOverridden(*meta)) {
                continue;
            }

            std::string platforms = " (" + Merge(::GetPlatformTokens(layer->platforms), ", ") + ")";

            text += COMMENT + meta->label + "\n";
            text += COMMENT + "------------------------------------------\n";
            text += COMMENT + meta->key;
            if (meta->status != STATUS_STABLE) {
                text += format(" (%s)", GetToken(meta->status));
            }
            text += platforms.c_str();
            text += "\n";

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
                text += COMMENT;
                std::size_t nchars = COMMENT.size();
                for (auto word : words) {
                    if (word.size() + nchars > 80) {
                        text += "\n";
                        text += COMMENT;
                        nchars = COMMENT.size();
                    }
                    text += " " + word;
                    nchars += (word.size() + 1);
                }
            }
            text += "\n";

            if (!meta->detailed.empty()) {
                text += COMMENT + format("%s\n", meta->detailed.c_str());
            }
            if (!meta->url.Empty()) {
                text += COMMENT + format("For more information about the feature: %s\n",
                                         ConvertStandardSeparators(meta->url.AbsolutePath()).c_str());
            }

            if (meta->status == STATUS_DEPRECATED && !meta->deprecated_by_key.empty()) {
                const SettingMeta* replaced_setting = FindSetting(layer->settings, meta->deprecated_by_key.c_str());
                text += COMMENT + format("This setting was deprecated and replaced by '%s' (%s) setting.\n",
                                         replaced_setting->label.c_str(), replaced_setting->key.c_str());
            }

            if (!meta->dependence.empty()) {
                text += COMMENT + "This setting requires " + ::GetToken(meta->dependence_mode) + " of the following values:\n";
                for (std::size_t i = 0, n = meta->dependence.size(); i < n; ++i) {
                    const SettingData* setting_data = meta->dependence[i];
                    std::vector<std::string> data = ::BuildEnvVariablesList(layer->key.c_str(), setting_data->key.c_str(), false);
                    text += COMMENT + "- " + EXPORT + data[0].c_str() + "=";
                    text += setting_data->Export(EXPORT_MODE_OVERRIDE) + "\n";
                }
            }

            // If feature has unmet dependency, output it but comment it out
            bool dependence_not_satisfied = false;
            if (::CheckDependence(*meta, parameter.settings) != SETTING_DEPENDENCE_ENABLE) {
                dependence_not_satisfied = true;
                text += COMMENT;
            }

            std::vector<std::string> data = ::BuildEnvVariablesList(layer->key.c_str(), setting_data->key.c_str(), false);
            const bool need_wordaround = this->type == GENERATE_SETTINGS_BASH && setting_data->key == "force_device_name";

            text += EXPORT + data[0] + "=";

            if (need_wordaround) {
                text += "\"";
            }

            text += setting_data->Export(EXPORT_MODE_OVERRIDE);

            if (need_wordaround) {
                text += "\"";
            }

            if (dependence_not_satisfied) {
                text += " (Commented out as the set of dependences is not satisfied)";
            }

            text += "\n\n";
        }
    }

    return text;
}
