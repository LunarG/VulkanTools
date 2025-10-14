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

#pragma once

#include "generate_layers_settings.h"

struct GenerateSettingsEnv : public GenerateSettings {
    GenerateSettingsEnv(GenerateSettingsMode type) : GenerateSettings(type) {}

    virtual std::string operator()(const LayerManager& layers, const Configuration* configuration) const;
};

struct GenerateSettingsCMD : public GenerateSettingsEnv {
    GenerateSettingsCMD() : GenerateSettingsEnv(GENERATE_SETTINGS_CMD) {}
};

struct GenerateSettingsBash : public GenerateSettingsEnv {
    GenerateSettingsBash() : GenerateSettingsEnv(GENERATE_SETTINGS_BASH) {}
};
