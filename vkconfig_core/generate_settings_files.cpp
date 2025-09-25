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

#include "generate_settings_files.h"

#include "../vkconfig_core/doc.h"

class ConfigurationSelection {
   public:
    ConfigurationSelection(Configurator& configurator, const CommandLine& command_line) : configurator(configurator) {
        if (configurator.GetActiveConfiguration() != nullptr) {
            this->saved_configuration_key = configurator.GetActiveConfiguration()->key;
        }

        if (command_line.selected_configuration_name == "default") {
            Configuration& configuration = configurator.configurations.CreateConfiguration(configurator.layers, "_default");
            if (!command_line.selected_layer_name.empty() && command_line.selected_layer_name != "default") {
                const Layer* layer = configurator.layers.Find(command_line.selected_layer_name.c_str(), Version::LATEST);

                if (layer == nullptr) {
                    fprintf(stderr, "vkconfig: Could not load layer \"%s\"\n", command_line.selected_layer_name.c_str());
                    fprintf(stderr, "\n  (Run \"vkconfig layers --list\" to get list of available layers)\n");
                } else {
                    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
                        if (command_line.selected_layer_name == configuration.parameters[i].key) {
                            configuration.parameters[i].control = LAYER_CONTROL_ON;
                        } else {
                            configuration.parameters[i].control = LAYER_CONTROL_OFF;
                        }
                    }
                }
            }

            configurator.SetActiveConfigurationName("_default");
        } else if (configurator.configurations.FindConfiguration(command_line.selected_configuration_name) != nullptr) {
            configurator.SetActiveConfigurationName(command_line.selected_configuration_name);
        } else {
            this->configuration_found = false;
            fprintf(stderr, "vkconfig: Could not load configuration \"%s\"\n", command_line.selected_configuration_name.c_str());
            fprintf(stderr, "\n  (Run \"vkconfig loader --list\" to get list of available configurations)\n");
        }
    }

    ~ConfigurationSelection() {
        if (configurator.configurations.FindConfiguration("_default") != nullptr) {
            configurator.configurations.RemoveConfiguration("_default");
        }

        if (!this->saved_configuration_key.empty()) {
            configurator.SetActiveConfigurationName(this->saved_configuration_key);
        }
    }

    bool Found() const { return configuration_found; }

   private:
    Configurator& configurator;
    std::string saved_configuration_key;
    bool configuration_found = true;
};

int generate_settings_html(Configurator& configurator, const CommandLine& command_line) {
    ConfigurationSelection selection(configurator, command_line);

    if (selection.Found()) {
        return ::ExportHtmlDoc(configurator, nullptr, command_line.GetOutputPath()) ? 0 : -1;
    } else {
        return -1;
    }
}

int generate_settings_markdown(Configurator& configurator, const CommandLine& command_line) {
    ConfigurationSelection selection(configurator, command_line);

    if (selection.Found()) {
        return ::ExportMarkdownDoc(configurator, nullptr, command_line.GetOutputPath()) ? 0 : -1;
    } else {
        return -1;
    }
}

int generate_settings_txt(Configurator& configurator, const CommandLine& command_line) {
    ConfigurationSelection selection(configurator, command_line);

    if (selection.Found()) {
        return configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT, command_line.GetOutputPath()) ? 0 : -1;
    } else {
        return -1;
    }
}

int generate_settings_bash(Configurator& configurator, const CommandLine& command_line) {
    ConfigurationSelection selection(configurator, command_line);

    if (selection.Found()) {
        return configurator.Export(EXPORT_ENV_BASH, command_line.GetOutputPath()) ? 0 : -1;
    } else {
        return -1;
    }
}

int generate_settings_cmd(Configurator& configurator, const CommandLine& command_line) {
    ConfigurationSelection selection(configurator, command_line);

    if (selection.Found()) {
        return configurator.Export(EXPORT_ENV_CMD, command_line.GetOutputPath()) ? 0 : -1;
    } else {
        return -1;
    }
}

int generate_settings_hpp(Configurator& configurator, const CommandLine& command_line) {
    ConfigurationSelection selection(configurator, command_line);

    if (selection.Found()) {
        return configurator.WriteExtensionCode(command_line.GetOutputPath()) ? 0 : -1;
    } else {
        return -1;
    }
}