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
#include "../vkconfig_core/type_override_area.h"
#include "../vkconfig_core/vulkan_info.h"

class Configurator {
   public:
    enum Mode { CMD, GUI };

    struct LayersSettings {
        std::string configuration_name;
        Path executable_path;
        Path settings_path;
    };

    struct LoaderLayerSettings {
        std::string key;
        std::string path;
        LayerControl control = LAYER_CONTROL_AUTO;
        bool implicit = false;
    };

    struct LoaderSettings {
        std::string executable_path;
        std::vector<LoaderLayerSettings> layers;
        LogFlags stderr_log_flags = LOG_ERROR;
    };

    static Configurator& Get();
    bool Init(Mode mode = GUI);

   public:
    bool Surrender(OverrideArea override_area);
    bool Override(OverrideArea override_area);
    bool HasOverride() const;

    void Reset();

    Configuration* GetActiveConfiguration();
    const Configuration* GetActiveConfiguration() const;
    bool HasActiveConfiguration() const;

   private:
    Configurator();
    ~Configurator();

    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

    bool WriteLayersSettings(OverrideArea override_area, const Path& layers_settings_path);
    bool WriteLoaderSettings(OverrideArea override_area, const Path& loader_settings_path);

    void BuildLoaderSettings(const ConfigurationInfo& info, const std::string& executable_path,
                             std::vector<LoaderSettings>& loader_settings_array) const;

   public:
    Mode mode;
    Environment environment;
    LayerManager layers;
    ConfigurationManager configurations;
    VulkanSystemInfo vulkan_system_info;
};
