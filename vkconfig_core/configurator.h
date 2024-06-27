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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/layer_manager.h"
#include "../vkconfig_core/environment.h"
#include "../vkconfig_core/configuration_manager.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/vulkan_info.h"

class Configurator {
   public:
    static Configurator& Get();
    bool Init();

   public:
    bool Surrender();
    bool Override();
    bool HasOverride() const;

    void ActivateConfiguration(const std::string& configuration_name);

    // The only function that actually configure the system, the Vulkan Loader, the Vulkan layer settings, creating and deleting
    // system files
    void Configure(const std::vector<Layer>& available_layers, const std::string& configuration_name);
    void Configure(const std::vector<Layer>& available_layers);

    void ResetToDefault(bool hard);

    bool SupportLoaderSettings(Version* return_loader_version = nullptr) const;

   private:
    Configurator();
    ~Configurator();

    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

    bool WriteLayersSettings(const Path& layers_settings_path);
    bool WriteLoaderSettings(const Path& loader_settings_path);

   public:
    Environment environment;
    LayerManager layers;
    ConfigurationManager configurations;
    VulkanSystemInfo vulkan_system_info;
};
