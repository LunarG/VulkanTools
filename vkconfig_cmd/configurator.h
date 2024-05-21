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

#pragma once

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/layer_manager.h"
#include "../vkconfig_core/path_manager.h"
#include "../vkconfig_core/environment.h"
#include "../vkconfig_core/configuration_manager.h"
#include "../vkconfig_core/platform.h"

class Configurator {
   public:
    static Configurator& Get(const std::string& VULKAN_SDK = "");
    bool Init();

    // The list of applications affected
   public:
    bool SupportLoaderSettings(Version* return_loader_version = nullptr) const;

    void ActivateConfiguration(const std::string& configuration_name);

    void ResetToDefault(bool hard);

    std::vector<std::string> GetDeviceNames() const;

   private:
    Configurator(const std::string& VULKAN_SDK);
    ~Configurator();

    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

   public:
    PathManager path;
    Environment environment;
    LayerManager layers;
    ConfigurationManager configurations;
};
