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

#include "type_configuration_area.h"

#include <cstring>

const char* GetToken(ConfigurationAreaType area) {
    static const char* TOKENS[] = {
        "layers_ordering",  // CONFIURATION_AREA_LAYERS_ORDERING
        "loader_messages",  // CONFIURATION_AREA_LOADER_MESSAGES
        "layers_settings",  // CONFIURATION_AREA_LAYERS_SETTINGS
    };
    static_assert(std::size(TOKENS) == CONFIURATION_AREA_COUNT);

    if (area >= CONFIURATION_AREA_FIRST && area <= CONFIURATION_AREA_LAST) {
        return TOKENS[area - CONFIURATION_AREA_FIRST];
    } else {
        return nullptr;
    }
}

ConfigurationAreaType GetConfigurationAreaType(const char* token) {
    for (int i = CONFIURATION_AREA_FIRST, l = CONFIURATION_AREA_LAST; i <= l; ++i) {
        const ConfigurationAreaType value = static_cast<ConfigurationAreaType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return CONFIURATION_AREA_INVALID;
}

ConfigurationAreaFlags GetConfigurationAreaFlags(const std::vector<std::string>& tokens) {
    ConfigurationAreaFlags flags = 0;

    for (std::size_t i = 0, n = tokens.size(); i < n; ++i) {
        ConfigurationAreaType value = GetConfigurationAreaType(tokens[i].c_str());
        if (value == CONFIURATION_AREA_INVALID) {
            continue;
        }
        flags |= (1 << value);
    }

    return flags;
}

std::vector<std::string> GetConfigurationAreaTokens(ConfigurationAreaFlags flags) {
    std::vector<std::string> result;

    for (int i = CONFIURATION_AREA_FIRST, l = CONFIURATION_AREA_LAST; i <= l; ++i) {
        ConfigurationAreaType value = static_cast<ConfigurationAreaType>(i);
        if (flags & (1 << value)) {
            result.push_back(GetToken(value));
        }
    }

    return result;
}
