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

#include "version.h"
#include "application.h"
#include "type_tab.h"
#include "type_log.h"

#include <QByteArray>

#include <array>
#include <vector>
#include <string>

enum LayoutState {
    VKCONFIG2_LAYOUT_MAIN_GEOMETRY = 0,
    VKCONFIG2_LAYOUT_MAIN_WINDOW_STATE,
    VKCONFIG2_LAYOUT_MAIN_SPLITTER1,
    VKCONFIG2_LAYOUT_MAIN_SPLITTER2,
    VKCONFIG2_LAYOUT_MAIN_SPLITTER3,
    VKCONFIG3_LAYOUT_MAIN_GEOMETRY,
    VKCONFIG3_LAYOUT_MAIN_WINDOW_STATE,
    VKCONFIG3_LAYOUT_MAIN_SPLITTER1,
    VKCONFIG3_LAYOUT_MAIN_SPLITTER2,
    VKCONFIG3_LAYOUT_MAIN_SPLITTER3,
    LAYOUT_LAYER_GEOMETRY,
    LAYOUT_LAYER_SPLITTER,
    LAYOUT_LAUNCHER_COLLAPSED,
    LAYOUT_LAUNCHER_NOT_CLEAR,

    LAYOUT_FIRST = VKCONFIG2_LAYOUT_MAIN_GEOMETRY,
    LAYOUT_LAST = LAYOUT_LAUNCHER_NOT_CLEAR,
};

enum { LAYOUT_COUNT = LAYOUT_LAST - LAYOUT_FIRST + 1 };

struct DefaultApplication {
    std::string name;
    std::string key;
    std::string arguments;
};

struct DefaultPath {
    Path executable_path;
    Path working_folder;
};

class Environment {
   public:
    enum Mode { MODE_AUTO_LOAD_SAVE = 0, MODE_UNINITIALIZED };

    Environment(Mode mode = MODE_AUTO_LOAD_SAVE);
    ~Environment();

    enum ResetMode { DEFAULT = 0, CLEAR, SYSTEM };

    void Reset(ResetMode mode);

    bool Load();
    bool Save() const;

    void SelectActiveApplication(std::size_t application_index);
    int GetActiveApplicationIndex() const;
    bool HasOverriddenApplications() const;
    bool AppendApplication(const Application& application);
    bool RemoveApplication(std::size_t application_index);

    const ConfigurationInfo& GetActiveConfigurationInfo() const;
    ConfigurationInfo& GetActiveConfigurationInfo();

    const std::vector<Application>& GetApplications() const { return applications; }
    const Application& GetActiveApplication() const;
    Application& GetActiveApplication();
    const Application& GetApplication(std::size_t application_index) const;
    Application& GetApplication(std::size_t application_index);

    const QByteArray& Get(LayoutState state) const;
    void Set(LayoutState state, const QByteArray& data);

    bool GetPerApplicationConfig() const;
    void SetPerApplicationConfig(bool enable);

    bool GetUseSystemTray() const { return this->use_system_tray; }
    void SetUseSystemTray(bool enable) { this->use_system_tray = enable; }

    LogFlags GetLoaderMessageFlags() const { return this->loader_message_types_flags; }
    void SetLoaderMessageFlags(LogFlags flags) { this->loader_message_types_flags = flags; }

    // Search for all the applications in the list, an remove the application which executable can't be found
    std::vector<Application> RemoveMissingApplications(const std::vector<Application>& applications) const;

    bool has_crashed = false;
    int hide_message_boxes_flags = 0;

    Path path_export;
    Path path_import;

    ConfigurationInfo global_configuration;

   private:
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    const Mode mode;

    TabType active_tab = TAB_DIAGNOSTIC;
    bool use_system_tray = false;
    bool use_per_application_configuration = false;
    LogFlags loader_message_types_flags;
    Path home_sdk_path;

    int active_executable_index;
    std::array<QByteArray, LAYOUT_COUNT> layout_states;
    std::vector<Application> applications;

    // Update default applications path to use relative path (really useful only on Windows)
    std::vector<Application> UpdateDefaultApplications(const std::vector<Application>& applications) const;

    // Create a list of default applications, eg vkcube
    std::vector<Application> CreateDefaultApplications() const;
    Application CreateDefaultApplication(const DefaultApplication& default_application) const;
    DefaultPath GetDefaultExecutablePath(const std::string& executable_name) const;
};

bool ExactExecutableFromAppBundle(Path& path);
