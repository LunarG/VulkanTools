/*
 * Copyright (c) 2022-2024 Valve Corporation
 * Copyright (c) 2022-2024 LunarG, Inc.
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

int run_doc_html(const CommandLine& command_line) {
    LayerManager layers;

    for (std::size_t i = 0, n = layers.selected_layers.size(); i < n; ++i) {
        const Layer& layer = layers.selected_layers[i];
        if (layer.key == command_line.doc_layer_name) {
            const std::string path = format("%s/%s.html", command_line.doc_out_dir.c_str(), layer.key.c_str());
            ExportHtmlDoc(layer, path);
            return 0;
        }
    }
    fprintf(stderr, "vkconfig: Could not load layer %s\n", command_line.doc_layer_name.c_str());
    fprintf(stderr, "Run \"vkconfig layers --list\" to get list of available layers\n");
    return -1;
}

int run_doc_markdown(const CommandLine& command_line) {
    LayerManager layers;

    for (std::size_t i = 0, n = layers.selected_layers.size(); i < n; ++i) {
        const Layer& layer = layers.selected_layers[i];
        if (layer.key == command_line.doc_layer_name) {
            const std::string path = format("%s/%s.md", command_line.doc_out_dir.c_str(), layer.key.c_str());
            ExportMarkdownDoc(layer, path);
            return 0;
        }
    }
    fprintf(stderr, "vkconfig: Could not load layer %s\n", command_line.doc_layer_name.c_str());
    fprintf(stderr, "Run \"vkconfig layers --list\" to get list of available layers\n");
    return -1;
}

int run_doc_settings(const CommandLine& command_line) {
    int rval = 0;

    LayerManager layers;

    const Layer* layer = layers.Find(command_line.doc_layer_name.c_str(), Version::LATEST);
    if (!layer) {
        fprintf(stderr, "vkconfig: Could not load layer %s\n", command_line.doc_layer_name.c_str());
        fprintf(stderr, "Run \"vkconfig layers --list\" to get list of available layers\n");
        return -1;
    }

    ConfigurationManager configuration_manager;
    Configuration config = configuration_manager.CreateConfiguration(layers, "Config");
    config.parameters = GatherParameters(config.parameters, layers);
    config.parameters[0].control = LAYER_CONTROL_ON;
    ExportSettingsDoc(layers.selected_layers, config, command_line.doc_out_dir + "/vk_layer_settings.txt");

    return rval;
}

int run_doc(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_DOC);
    assert(command_line.error == ERROR_NONE);

    switch (command_line.command_doc_arg) {
        case COMMAND_DOC_HTML: {
            return run_doc_html(command_line);
        }
        case COMMAND_DOC_MARKDOWN: {
            return run_doc_markdown(command_line);
        }
        case COMMAND_DOC_SETTINGS: {
            return run_doc_settings(command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
