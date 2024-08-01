/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "ui.h"
#include "date.h"

std::string ExtractLayerName(const LayerManager& layer_manager, const std::string& layer_string) {
    for (std::size_t i = 0, n = layer_manager.Size(); i < n; ++i) {
        const Layer& layer = layer_manager.selected_layers[i];
        if (layer_string.find(layer.key) != std::string::npos) {
            return layer.key;
        }
    }

    return std::string();
}

std::string GetMainWindowTitle(bool active, bool display_date) {
    std::string title = format("%s %s", VKCONFIG_NAME, Version::VKCONFIG.str().c_str());

#if VKCONFIG_DATE
    if (display_date) {
        title += format("-%s", GetBuildDate().c_str());
    }
#endif

    if (active) {
        title += " <ACTIVE>";
    }

    return title;
}
