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

#include "version.h"
#include "application.h"
#include "path_manager.h"

#include <QByteArray>

#include <array>
#include <vector>
#include <string>

enum OverrideFlag { OVERRIDE_FLAG_ACTIVE = (1 << 0), OVERRIDE_FLAG_SELECTED = (1 << 1), OVERRIDE_FLAG_PERSISTENT = (1 << 2) };

enum OverrideState {
    OVERRIDE_STATE_DISABLED = 0,
    OVERRIDE_STATE_GLOBAL_TEMPORARY = OVERRIDE_FLAG_ACTIVE,
    OVERRIDE_STATE_GLOBAL_PERSISTENT = OVERRIDE_FLAG_ACTIVE | OVERRIDE_FLAG_PERSISTENT,
    OVERRIDE_STATE_SELECTED_TEMPORARY = OVERRIDE_FLAG_ACTIVE | OVERRIDE_FLAG_SELECTED,
    OVERRIDE_STATE_SELECTED_PERSISTENT = OVERRIDE_FLAG_ACTIVE | OVERRIDE_FLAG_SELECTED | OVERRIDE_FLAG_PERSISTENT,
};

enum OverrideMode { OVERRIDE_MODE_ACTIVE = 0, OVERRIDE_MODE_LIST, OVERRIDE_MODE_PERISTENT };

enum LayoutState {
    LAYOUT_MAIN_GEOMETRY = 0,
    LAYOUT_MAIN_WINDOW_STATE,
    LAYOUT_MAIN_SPLITTER1,
    LAYOUT_MAIN_SPLITTER2,
    LAYOUT_MAIN_SPLITTER3,
    LAYOUT_LAYER_GEOMETRY,
    LAYOUT_LAYER_SPLITTER,
    LAYOUT_LAUNCHER_COLLAPSED,
    LAYOUT_LAUNCHER_NOT_CLEAR,

    LAYOUT_FIRST = LAYOUT_MAIN_GEOMETRY,
    LAYOUT_LAST = LAYOUT_LAUNCHER_NOT_CLEAR,
};

enum { LAYOUT_COUNT = LAYOUT_LAST - LAYOUT_FIRST + 1 };

enum Active {
    ACTIVE_CONFIGURATION = 0,
    ACTIVE_APPLICATION,

    ACTIVE_FIRST = ACTIVE_CONFIGURATION,
    ACTIVE_LAST = ACTIVE_APPLICATION,
};

enum { ACTIVE_COUNT = ACTIVE_LAST - ACTIVE_FIRST + 1 };

enum UserDefinedLayersPaths {
    USER_DEFINED_LAYERS_PATHS_ENV_SET = 0,  // VK_LAYER_PATH
    USER_DEFINED_LAYERS_PATHS_ENV_ADD,      // VK_ADD_LAYER_PATH
    USER_DEFINED_LAYERS_PATHS_GUI,

    USER_DEFINED_LAYERS_PATHS_FIRST = USER_DEFINED_LAYERS_PATHS_ENV_SET,
    USER_DEFINED_LAYERS_PATHS_LAST = USER_DEFINED_LAYERS_PATHS_GUI,
};

enum { USER_DEFINED_LAYERS_PATHS_COUNT = USER_DEFINED_LAYERS_PATHS_LAST - USER_DEFINED_LAYERS_PATHS_FIRST + 1 };

enum LoaderMessageLevel {
    LOADER_MESSAGE_NONE = 0,
    LOADER_MESSAGE_ERROR,
    LOADER_MESSAGE_WARN,
    LOADER_MESSAGE_INFO,
    LOADER_MESSAGE_DEBUG,
    LOADER_MESSAGE_LAYER,
    LOADER_MESSAGE_IMPLEMENTATION,
    LOADER_MESSAGE_ALL,

    LOADER_MESSAGE_FIRST = LOADER_MESSAGE_NONE,
    LOADER_MESSAGE_LAST = LOADER_MESSAGE_ALL,
};

enum { LOADER_MESSAGE_COUNT = LOADER_MESSAGE_LAST - LOADER_MESSAGE_FIRST + 1 };

struct DefaultApplication {
    std::string name;
    std::string key;
    std::string arguments;
};

class Environment {
   public:
    Environment(PathManager& paths, const Version& api_version = Version::VKHEADER);
    ~Environment();

    enum ResetMode { DEFAULT = 0, CLEAR, SYSTEM };

    void Reset(ResetMode mode);

    bool Load();
    bool LoadApplications();
    bool Save() const;
    bool SaveApplications() const;

    void SelectActiveApplication(std::size_t application_index);
    int GetActiveApplicationIndex() const;
    bool HasOverriddenApplications() const;
    bool AppendApplication(const Application& application);
    bool RemoveApplication(std::size_t application_index);

    const std::vector<Application>& GetApplications() const { return applications; }
    const Application& GetActiveApplication() const;
    const Application& GetApplication(std::size_t application_index) const;
    Application& GetApplication(std::size_t application_index);

    const std::string& Get(Active active) const;
    void Set(Active active, const std::string& key);

    const QByteArray& Get(LayoutState state) const;
    void Set(LayoutState state, const QByteArray& data);

    bool UseOverride() const;
    bool UseApplicationListOverrideMode() const;
    bool UsePersistentOverrideMode() const;

    void SetMode(OverrideMode mode, bool enabled);

    LoaderMessageLevel GetLoaderMessage() const { return this->loader_message_level; }
    void SetLoaderMessage(LoaderMessageLevel level) { this->loader_message_level = level; }

    bool first_run;
    const Version api_version;

    void SetPerConfigUserDefinedLayersPaths(const std::vector<std::string>& paths) {
        std::vector<std::string>& custom_layer_paths_gui = user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_GUI];
        custom_layer_paths_gui.clear();
        for (std::size_t i = 0, n = paths.size(); i < n; ++i) {
            custom_layer_paths_gui.push_back(ConvertNativeSeparators(paths[i]).c_str());
        }
    }

    const std::vector<std::string>& GetUserDefinedLayersPaths(UserDefinedLayersPaths user_defined_layers_paths_id) const {
        return user_defined_layers_paths[user_defined_layers_paths_id];
    }

    bool IsDefaultConfigurationInit(const std::string& configuration_filename) const;
    void InitDefaultConfiguration(const std::string& configuration_filename);

    // Search for all the applications in the list, an remove the application which executable can't be found
    std::vector<Application> RemoveMissingApplications(const std::vector<Application>& applications) const;

   private:
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    Version version;
    OverrideState override_state;
    LoaderMessageLevel loader_message_level;

    std::array<std::string, ACTIVE_COUNT> actives;
    std::array<QByteArray, LAYOUT_COUNT> layout_states;
    std::array<std::vector<std::string>, USER_DEFINED_LAYERS_PATHS_COUNT> user_defined_layers_paths;
    std::vector<Application> applications;

    PathManager& paths_manager;

    std::vector<std::string> default_configuration_filenames;

    // Update default applications path to use relative path (really useful only on Windows)
    std::vector<Application> UpdateDefaultApplications(const std::vector<Application>& applications) const;

    // Create a list of default applications, eg vkcube
    std::vector<Application> CreateDefaultApplications() const;
    Application CreateDefaultApplication(const DefaultApplication& default_application) const;
    std::string GetDefaultExecutablePath(const std::string& executable_name) const;

   public:
    const PathManager& paths;
};

bool ExactExecutableFromAppBundle(std::string& path);

LoaderMessageLevel GetLoaderDebug(const std::string& value);
std::string GetLoaderDebugToken(LoaderMessageLevel level);
