/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "header.h"
#include "setting.h"

#include <vector>

struct LayerPreset : public Header {
    SettingDataSetConst settings;
};

const LayerPreset* GetPreset(const std::vector<LayerPreset>& presets, const char* preset_label);

// Check whether "layer_settings" has all the settings set in "preset_settings"
// "layer_settings" may have more settings then "preset_settings" and return true
bool HasPreset(const SettingDataSet& layer_settings, const SettingDataSetConst& preset_settings);
