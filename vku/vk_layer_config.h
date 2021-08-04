/*
 * Copyright (c) 2015-2021 The Khronos Group Inc.
 * Copyright (c) 2015-2021 Valve Corporation
 * Copyright (c) 2015-2021 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 * - Mark Lobodzinski <mark@lunarg.com>
 * - Jon Ashburn
 * - Courtney Goeltzenleuchter
 * - Tobin Ehlis
 */

#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>

#include <vulkan/vk_layer.h>
#include <vulkan/vulkan.h>

namespace vku {
VK_LAYER_EXPORT bool GetLayerSettingBool(const char *setting_namespace, const char *key);
VK_LAYER_EXPORT int GetLayerSettingInt(const char *setting_namespace, const char *key);
VK_LAYER_EXPORT double GetLayerSettingFloat(const char *setting_namespace, const char *key);
VK_LAYER_EXPORT std::string GetLayerSettingString(const char *setting_namespace, const char *key);
VK_LAYER_EXPORT std::vector<std::string> GetLayerSettingStrings(const char *setting_namespace, const char *key);
VK_LAYER_EXPORT std::vector<std::pair<std::string, int>> GetLayerSettingList(const char *setting_namespace, const char *key);

VK_LAYER_EXPORT void SetLayerSetting(const char *option, const char *val);

VK_LAYER_EXPORT const char *GetLayerSetting(const char *setting);
VK_LAYER_EXPORT const char *GetLayerEnvVar(const char *setting);
}  // namespace vku
