/*
 * Copyright (c) 2022-2025 Valve Corporation
 * Copyright (c) 2022-2025 LunarG, Inc.
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
 * - David Pinedo <david@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "main_doc.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/layer_manager.h"
#include "../vkconfig_core/doc.h"
#include "../vkconfig_core/configuration_manager.h"

#include <cassert>

int run_doc_html(Configurator& configurator, const CommandLine& command_line) {
    const Layer* layer = configurator.layers.Find(command_line.selected_layer_name.c_str(), Version::LATEST);

    if (layer == nullptr) {
        fprintf(stderr, "vkconfig: Could not load layer \"%s\"\n", command_line.selected_layer_name.c_str());
        fprintf(stderr, "\n  (Run \"vkconfig layers --list\" to get list of available layers)\n");
        return -1;
    }

    const std::string path = format("%s/%s.html", command_line.GetOutputPath().AbsoluteDir().c_str(), layer->key.c_str());

    return ExportHtmlDoc(configurator, layer, path);
}

int run_doc_markdown(Configurator& configurator, const CommandLine& command_line) {
    const Layer* layer = configurator.layers.Find(command_line.selected_layer_name.c_str(), Version::LATEST);

    if (layer == nullptr) {
        fprintf(stderr, "vkconfig: Could not load layer \"%s\"\n", command_line.selected_layer_name.c_str());
        fprintf(stderr, "\n  (Run \"vkconfig layers --list\" to get list of available layers)\n");
        return -1;
    }

    const std::string path = format("%s/%s.md", command_line.GetOutputPath().AbsoluteDir().c_str(), layer->key.c_str());

    return ExportMarkdownDoc(configurator, layer, path);
}

int run_doc_settings(Configurator& configurator, const CommandLine& command_line) {
    const Layer* layer = configurator.layers.Find(command_line.selected_layer_name.c_str(), Version::LATEST);

    if (layer == nullptr) {
        fprintf(stderr, "vkconfig: Could not load layer \"%s\"\n", command_line.selected_layer_name.c_str());
        fprintf(stderr, "\n  (Run \"vkconfig layers --list\" to get list of available layers)\n");
        return -1;
    }

    Configuration& configuration = configurator.configurations.CreateConfiguration(configurator.layers, "Config");
    for (std::size_t i = 0, n = configuration.parameters.size(); i < n; ++i) {
        if (layer->key == configuration.parameters[i].key) {
            configuration.parameters[i].control = LAYER_CONTROL_ON;
        } else {
            configuration.parameters[i].control = LAYER_CONTROL_OFF;
        }
    }

    const std::string path = format("%s/vk_layer_settings.txt", command_line.GetOutputPath().AbsoluteDir().c_str());

    return ExportSettingsDoc(configurator, layer, path);
}

int run_doc(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_DOC);
    assert(command_line.error == ERROR_NONE);

    ConfiguratorGuard configurator_guard(CONFIGURATOR_MODE_CMD);

    switch (command_line.command_doc_arg) {
        case COMMAND_DOC_HTML: {
            return run_doc_html(configurator_guard.Get(), command_line);
        }
        case COMMAND_DOC_MARKDOWN: {
            return run_doc_markdown(configurator_guard.Get(), command_line);
        }
        case COMMAND_DOC_SETTINGS: {
            return run_doc_settings(configurator_guard.Get(), command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
