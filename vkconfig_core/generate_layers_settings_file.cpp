/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include "generate_layers_settings_file.h"

// Create and write vk_layer_settings.txt file
bool GenerateSettingsTXT(Configurator& configurator, OverrideArea override_area, const Path& layers_settings_path) {
    if (override_area & OVERRIDE_AREA_LAYERS_SETTINGS_BIT) {
        std::vector<LayersSettings> layers_settings_array;

        switch (configurator.GetExecutableScope()) {
            case EXECUTABLE_ANY: {
                LayersSettings settings;
                settings.configuration_name = configurator.GetSelectedGlobalConfiguration();
                settings.settings_path = layers_settings_path;
                layers_settings_array.push_back(settings);
                break;
            }
            case EXECUTABLE_ALL:
            case EXECUTABLE_PER: {
                const std::vector<Executable>& executables = configurator.executables.GetExecutables();

                std::string configuration_name = configurator.GetSelectedGlobalConfiguration();

                for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                    if (!executables[i].enabled) {
                        continue;
                    }

                    if (::EnabledExecutables(configurator.GetExecutableScope())) {
                        configuration_name = executables[i].configuration;
                    }

                    LayersSettings settings;
                    settings.executable_path = executables[i].path;
                    settings.configuration_name = configuration_name;
                    settings.settings_path = executables[i].GetLocalLayersSettingsPath();
                    layers_settings_array.push_back(settings);
                }
                break;
            }
            default:
            case EXECUTABLE_NONE:
                break;
        }

        bool has_missing_layers = false;
        bool result_settings_file = true;

        for (std::size_t i = 0, n = layers_settings_array.size(); i < n; ++i) {
            const LayersSettings& layers_settings = layers_settings_array[i];

            Configuration* configuration = configurator.configurations.FindConfiguration(layers_settings.configuration_name);
            if (configuration == nullptr) {
                if (layers_settings.configuration_name.empty()) {
                    configurator.Log(LOG_WARN, "No global configuration selected");
                } else if (layers_settings.executable_path.Empty()) {
                    configurator.Log(LOG_ERROR, format("Fail to apply unknown '%s' global configuration",
                                                       layers_settings.configuration_name.c_str()));
                } else {
                    configurator.Log(LOG_ERROR, format("Fail to apply unknown '%s' configuration to '%s'\n",
                                                       layers_settings.configuration_name.c_str(),
                                                       layers_settings.executable_path.AbsolutePath().c_str()));
                }

                result_settings_file = false;
                continue;
            }

            if (layers_settings.settings_path.Exists()) {
                if (configurator.GetActiveConfiguration()->key != configuration->key) {
                    continue;
                }
            }

            QFile file(layers_settings.settings_path.AbsolutePath().c_str());
            result_settings_file = result_settings_file && file.open(QIODevice::WriteOnly | QIODevice::Text);
            if (!result_settings_file) {
                configurator.Log(LOG_ERROR,
                                 format("Cannot open file:\n\t%s", layers_settings.settings_path.AbsolutePath().c_str()));
                continue;
            }
            QTextStream stream(&file);

            stream << "# Copyright (c) 2020-2026 Valve Corporation\n";
            stream << "# Copyright (c) 2020-2026 LunarG, Inc.\n";
            stream << "# \n";
            stream << "# Licensed under the Apache License, Version 2.0 (the \"License\");\n";
            stream << "# you may not use this file except in compliance with the License.\n";
            stream << "# You may obtain a copy of the License at\n";
            stream << "# \n";
            stream << "#     http://www.apache.org/licenses/LICENSE-2.0\n";
            stream << "# \n";
            stream << "# Unless required by applicable law or agreed to in writing, software\n";
            stream << "# distributed under the License is distributed on an \"AS IS\" BASIS,\n";
            stream << "# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n";
            stream << "# See the License for the specific language governing permissions and\n";
            stream << "# limitations under the License.\n";
            stream << "# \n";
            stream << format("# This code was generated by Vulkan Configurator %s\n\n", Version::VKCONFIG.str().c_str()).c_str();

            if (configuration->override_settings) {
                QFile original_file(configuration->override_settings_path.AbsolutePath().c_str());
                bool result_original_file = original_file.open(QIODevice::ReadOnly);
                if (!result_original_file) {
                    configurator.Log(LOG_ERROR, format("Cannot open override settings file:\n\t%s",
                                                       configuration->override_settings_path.AbsolutePath().c_str()));
                    continue;
                }

                QTextStream instream(&original_file);
                while (!instream.atEnd()) {
                    QString line = instream.readLine();
                    stream << line << '\n';
                }
            } else {
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

                    const Layer* layer = configurator.layers.Find(parameter.key.c_str(), parameter.api_version);
                    if (layer == nullptr) {
                        if (parameter.control == LAYER_CONTROL_ON) {
                            has_missing_layers = true;
                            configurator.Log(
                                LOG_ERROR,
                                format("`%s` layer is set to `%s` in `%s` loader configuration but missing and being ignored\n",
                                       parameter.key.c_str(), ::GetLabel(parameter.control), configuration->key.c_str()));
                        } else {
                            configurator.Log(
                                LOG_WARN,
                                format("`%s` layer is set to `%s` in `%s` loader configuration but missing and being ignored\n",
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

                    stream << format("# %s\n", layer->description.c_str()).c_str();
                    stream << "# ==========================================\n";
                    stream << format("# %s - %s%s%s\n", layer->key.c_str(), layer->api_version.str().c_str(), status.c_str(),
                                     platforms.c_str())
                                  .c_str();

                    if (!layer->introduction.empty()) {
                        // Break up description into smaller words
                        std::string description = layer->introduction;
                        std::vector<std::string> paragraphs;
                        std::size_t pos;
                        while ((pos = description.find("\n")) != std::string::npos) {
                            paragraphs.push_back(description.substr(0, pos));
                            description.erase(0, pos + 1);
                        }

                        for (std::size_t i = 0, n = paragraphs.size(); i < n; ++i) {
                            description = paragraphs[i];

                            std::vector<std::string> words;
                            while ((pos = description.find(" ")) != std::string::npos) {
                                words.push_back(description.substr(0, pos));
                                description.erase(0, pos + 1);
                            }
                            if (description.size() > 0) {
                                words.push_back(description);
                            }
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
                        }
                    }

                    if (!layer->url.Empty()) {
                        stream << format("# For more information about the layer: %s\n",
                                         ConvertStandardSeparators(layer->url.AbsolutePath()).c_str())
                                      .c_str();
                    }
                    stream << "\n";

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

                        stream << "# " << meta->label.c_str() << "\n";
                        stream << "# ------------------------------------------\n";

                        stream << "# " << meta->key.c_str();
                        if (meta->status != STATUS_STABLE) {
                            stream << format(" (%s)", GetToken(meta->status)).c_str();
                        }
                        stream << platforms.c_str();
                        stream << "\n";

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

                        if (!meta->detailed.empty()) {
                            stream << format("# %s\n", meta->detailed.c_str()).c_str();
                        }

                        if (IsArray(meta->type)) {
                            stream << "# This setting can list multiple values using the \",\" (comma) separator.\n";
                        }

                        if (!meta->url.Empty()) {
                            stream << format("# For more information about the feature: %s\n",
                                             ConvertStandardSeparators(meta->url.AbsolutePath()).c_str())
                                          .c_str();
                        }

                        if (meta->status == STATUS_DEPRECATED && !meta->deprecated_by_key.empty()) {
                            const SettingMeta* replaced_setting = FindSetting(layer->settings, meta->deprecated_by_key.c_str());

                            stream << format("# This setting was deprecated and replaced by '%s' (%s) setting.\n",
                                             replaced_setting->label.c_str(), replaced_setting->key.c_str())
                                          .c_str();
                        }

                        if (!meta->children.empty()) {
                            stream << "# This setting has sub-settings:\n";
                            for (std::size_t i = 0, n = meta->children.size(); i < n; ++i) {
                                std::string setting_key = meta->children[i]->key;
                                const SettingData* setting_data = ::FindSetting(parameter.settings, setting_key.c_str());

                                if (meta->children[i]->type == SETTING_GROUP) {
                                    stream << "# - " << lc_layer_name.c_str() << setting_data->key.c_str() << " (Setting Group)\n";
                                } else {
                                    stream << "# - " << lc_layer_name.c_str() << setting_data->key.c_str() << " = ";
                                    stream << setting_data->Export(EXPORT_MODE_OVERRIDE).c_str() << "\n";
                                }
                            }
                        }

                        if (!meta->dependence.empty()) {
                            stream << "# This setting requires " << ::GetToken(meta->dependence_mode)
                                   << " of the following values:\n";
                            for (std::size_t i = 0, n = meta->dependence.size(); i < n; ++i) {
                                const SettingData* data = meta->dependence[i];
                                stream << "# - " << lc_layer_name.c_str() << data->key.c_str() << " = "
                                       << data->Export(EXPORT_MODE_OVERRIDE).c_str() << "\n";
                            }
                        }

                        // If feature has unmet dependency, output it but comment it out
                        bool dependence_not_satisfied = false;
                        if (::CheckDependence(*meta, parameter.settings) != SETTING_DEPENDENCE_ENABLE) {
                            dependence_not_satisfied = true;
                            stream << "# ";
                        }
                        stream << lc_layer_name.c_str() << setting_data->key.c_str() << " = ";
                        stream << setting_data->Export(EXPORT_MODE_OVERRIDE).c_str();
                        if (dependence_not_satisfied) {
                            stream << " (Commented out as the set of dependences is not satisfied)";
                        }
                        stream << "\n\n";
                    }

                    stream << "\n";
                }
            }
            file.close();
        }

        return result_settings_file && !has_missing_layers;
    } else {
        return true;
    }
}
