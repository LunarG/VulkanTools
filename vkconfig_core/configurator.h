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

#include "version.h"
#include "configuration_manager.h"
#include "configuration_manager.h"
#include "layer_manager.h"
#include "executable_manager.h"
#include "type_platform.h"
#include "type_override_area.h"
#include "type_hide_message.h"
#include "type_layers_view.h"
#include "type_tab.h"
#include "vulkan_info.h"
#include "serialization.h"

class Configurator : public Serialize {
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
    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;

    bool Surrender(OverrideArea override_area);
    bool Override(OverrideArea override_area);
    bool HasOverride() const;

    void Reset(bool hard);

    void SetActiveConfigurationName(const std::string& configuration_name);
    std::string GetActionConfigurationName() const;
    void SetActiveLayersMode(LayersMode mode);
    LayersMode GetActiveLayersMode() const;

    Configuration* GetActiveConfiguration();
    const Configuration* GetActiveConfiguration() const;
    bool HasActiveConfiguration() const;

    bool WriteLayersSettings(OverrideArea override_area, const Path& layers_settings_path);
    bool WriteLoaderSettings(OverrideArea override_area, const Path& loader_settings_path);

    void Set(HideMessageType type);
    bool Get(HideMessageType type) const;

    bool GetPerExecutableConfig() const;
    void SetPerExecutableConfig(bool enabled);

    bool GetUseSystemTray() const;
    void SetUseSystemTray(bool enabled);

    LayersView GetLayersView() const;
    void SetLayersView(LayersView view);

    ~Configurator();

   private:
    Configurator();

    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

    void BuildLoaderSettings(const std::string& configuration_key, LayersMode mode, const std::string& executable_path,
                             std::vector<LoaderSettings>& loader_settings_array) const;

   public:
    Mode mode;
    LayerManager layers;
    ConfigurationManager configurations;
    ExecutableManager executables;
    VulkanSystemInfo vulkan_system_info;

    bool has_crashed = false;
    TabType active_tab = TAB_CONFIGURATIONS;

   private:
    Path home_sdk_path;
    int hide_message_boxes_flags = 0;
    bool use_system_tray = false;
    bool use_per_executable_configuration = false;
    std::string selected_global_configuration;
    LayersMode selected_global_layers_mode;
    LayersView selected_layers_view;
};
