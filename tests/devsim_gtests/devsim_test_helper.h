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
 * - Jeremy Kniager <jeremyk@lunarg.com>
 */

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace devsim_test {

void setEnvironmentSetting(std::string setting, const char* val);
void unsetEnvironmentSetting(std::string setting);

std::string getAbsolutePath(std::string filepath);

void initLayerPath();

enum ArrayCombinationMode {
    ARRAY_COMBINATION_MODE_NONE,
    ARRAY_COMBINATION_MODE_REPLACE,
    ARRAY_COMBINATION_MODE_WHITELIST,
    ARRAY_COMBINATION_MODE_BLACKLIST,
    ARRAY_COMBINATION_MODE_INTERSECT
};

std::string arrayCombinationModeToString(ArrayCombinationMode mode);

void setDevsimFilenames(std::vector<std::string>& filepaths);
void setDevsimDebugEnable(bool enable);
void setDevsimExitOnError(bool enable);
void setDevsimEmulatePortabilitySubsetExtension(bool enable);
void setDevsimModifyExtensionList(ArrayCombinationMode mode);
void setDevsimModifyMemoryFags(bool enable);
void setDevsimModifyFormatList(ArrayCombinationMode mode);
void setDevsimModifyFormatProperties(ArrayCombinationMode mode);
void setDevsimModifySurfaceFormats(ArrayCombinationMode mode);
void setDevsimModifyPresentModes(ArrayCombinationMode mode);

VkApplicationInfo GetDefaultApplicationInfo();

class VulkanInstanceBuilder {
   public:
    void setApiVersion(uint32_t apiVersion) { _app_info.apiVersion = apiVersion; }
    void addLayer(const char* layer_name) { _layer_names.push_back(layer_name); }
    void addExtension(const char* extension_name) { _extension_names.push_back(extension_name); }
    VkResult makeInstance();

    void reset();

    VkInstance getInstance() { return _instance; }

   protected:
    VkApplicationInfo _app_info{GetDefaultApplicationInfo()};
    VkInstanceCreateInfo _inst_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    VkInstance _instance{VK_NULL_HANDLE};

    std::vector<const char*> _layer_names;
    std::vector<const char*> _extension_names;
};
}