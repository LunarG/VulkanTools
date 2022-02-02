/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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

#include "configuration.h"
#include "environment.h"
#include "application.h"

// Create the VkLayer_override.json and vk_layer_settings.txt files to take over Vulkan layers from Vulkan applications
bool OverrideConfiguration(const Environment& environment, const std::vector<Layer>& available_layers,
                           const Configuration& configuration);

// Remove the VkLayer_override.json and vk_layer_settings.txt files to return full control of the layers to the Vulkan applications
bool SurrenderConfiguration(const Environment& environment);

// Check whether a layers configuration is activated
bool HasOverride();

// Write the settings file for override layer
bool WriteSettingsOverride(const std::vector<Layer>& available_layers,
                           const Configuration& configuration, const std::string& settings_path);
