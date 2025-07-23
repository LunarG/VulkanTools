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

#include "type_tab.h"

#include <array>
#include <cstring>

const char* GetLabel(TabType type) {
    static const char* TOKENS[]{
        "Vulkan Loader Management",  // TAB_CONFIGURATIONS
        "Vulkan Drivers Paths",      // TAB_DRIVERS
        "Vulkan Layers Paths",       // TAB_LAYERS
        "Application Launcher",      // TAB_APPLICATIONS
        "Diagnostic",                // TAB_DIAGNOSTIC
        "Documentation",             // TAB_DOCUMENTATION
        "Preferences",               // TAB_PREFERENCES
        "About"                      // TAB_ABOUT
    };

    static_assert(std::size(TOKENS) == TAB_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TOKENS[type - TAB_FIRST];
}

const char* GetToken(TabType type) {
    static const char* TOKENS[]{
        "CONFIGURATIONS",  // TAB_CONFIGURATIONS
        "DRIVERS",         // TAB_DRIVERS
        "LAYERS",          // TAB_LAYERS
        "APPLICATIONS",    // TAB_APPLICATIONS
        "DIAGNOSTIC",      // TAB_DIAGNOSTIC
        "DOCUMENTATION",   // TAB_DOCUMENTATION
        "PREFERENCES",     // TAB_PREFERENCES
        "ABOUT"            // TAB_ABOUT
    };

    static_assert(std::size(TOKENS) == TAB_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TOKENS[type - TAB_FIRST];
}

TabType GetTabType(const char* token) {
    for (int i = TAB_FIRST, n = TAB_LAST; i <= n; ++i) {
        const TabType type = static_cast<TabType>(i);
        if (std::strcmp(::GetToken(type), token) == 0) {
            return type;
        }
    }

    return TAB_DIAGNOSTIC;
}
