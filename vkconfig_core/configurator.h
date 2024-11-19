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
#include "vulkan_util.h"
#include "configuration_manager.h"
#include "configuration_manager.h"
#include "layer_manager.h"
#include "executable_manager.h"
#include "type_platform.h"
#include "type_override_area.h"
#include "type_hide_message.h"
#include "type_tab.h"
#include "type_executable_mode.h"
#include "serialization.h"

enum EnabledUI {
    ENABLE_UI_CONFIG = 0,
    ENABLE_UI_LAYERS,
    ENABLE_UI_LOADER,
    ENABLE_UI_SETTINGS,
};

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
        LayerBuiltin builtin = LAYER_BUILTIN_NONE;
        bool implicit = false;
    };

    struct LoaderSettings {
        std::string executable_path;
        std::vector<LoaderLayerSettings> layers;
        LogFlags stderr_log_flags = LOG_ERROR;
        bool override_layers = true;
        bool override_loader = true;
    };

    static Configurator& Get();
    bool Init(Mode mode = GUI);

   public:
    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;
    std::string Log() const override;
    std::string LogConfiguration(const std::string& configuration_key) const;

    bool Surrender(OverrideArea override_area);
    bool Override(OverrideArea override_area);
    bool HasOverride() const;

    void Reset(bool hard);

    void SetActiveConfigurationName(const std::string& configuration_name);

    Configuration* GetActiveConfiguration();
    const Configuration* GetActiveConfiguration() const;

    Parameter* GetActiveParameter();
    const Parameter* GetActiveParameter() const;
    bool HasActiveParameter() const;

    Executable* GetActiveExecutable();
    const Executable* GetActiveExecutable() const;

    bool WriteLayersSettings(OverrideArea override_area, const Path& layers_settings_path);
    bool WriteLoaderSettings(OverrideArea override_area, const Path& loader_settings_path);

    void Set(HideMessageType type);
    bool Get(HideMessageType type) const;

    ExecutableScope GetExecutableScope() const;
    void SetExecutableScope(ExecutableScope scope);

    bool GetUseSystemTray() const;
    void SetUseSystemTray(bool enabled);

    Path GetHomeSDK() const;
    void SetHomeSDK(const Path& path);

    bool HasActiveSettings() const;
    bool HasEnabledUI(EnabledUI enabled_ui) const;

    std::string GenerateVulkanStatus() const;

    ~Configurator();

   private:
    Configurator();

    Configurator(const Configurator&) = delete;
    Configurator& operator=(const Configurator&) = delete;

    void BuildLoaderSettings(const std::string& configuration_key, const std::string& executable_path,
                             std::vector<LoaderSettings>& loader_settings_array) const;

   public:
    Mode mode;
    LayerManager layers;
    ConfigurationManager configurations;
    ExecutableManager executables;
    VulkanSystemInfo vulkan_system_info;

    bool has_crashed = false;
    TabType active_tab = TAB_CONFIGURATIONS;
    bool advanced = true;
    Path last_path_status = ::Get(Path::HOME) + "/vkconfig.txt";

   private:
    Path home_sdk_path = ::Get(Path::HOME);
    int hide_message_boxes_flags = 0;
    bool use_system_tray = false;
    ExecutableScope executable_scope = EXECUTABLE_ANY;
    std::string selected_global_configuration = "Validation";
};
