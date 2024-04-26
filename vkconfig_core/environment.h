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
#include "environment_key.h"
#include "application.h"
#include "path_manager.h"

#include <QByteArray>

#include <array>
#include <vector>
#include <string>

enum LayersMode { LAYERS_MODE_BY_APPLICATIONS = 0, LAYERS_MODE_BY_CONFIGURATOR_RUNNING, LAYERS_MODE_BY_CONFIGURATOR_ALL_DISABLED };

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

enum UserDefinedLayersPaths {
    USER_DEFINED_LAYERS_PATHS_ENV_SET = 0,  // VK_LAYER_PATH
    USER_DEFINED_LAYERS_PATHS_ENV_ADD,      // VK_ADD_LAYER_PATH
    USER_DEFINED_LAYERS_PATHS_GUI,

    USER_DEFINED_LAYERS_PATHS_FIRST = USER_DEFINED_LAYERS_PATHS_ENV_SET,
    USER_DEFINED_LAYERS_PATHS_LAST = USER_DEFINED_LAYERS_PATHS_GUI,
};

enum { USER_DEFINED_LAYERS_PATHS_COUNT = USER_DEFINED_LAYERS_PATHS_LAST - USER_DEFINED_LAYERS_PATHS_FIRST + 1 };

enum LoaderMessageType {
    LOADER_MESSAGE_NONE = 0,
    LOADER_MESSAGE_ERROR,
    LOADER_MESSAGE_WARN,
    LOADER_MESSAGE_INFO,
    LOADER_MESSAGE_DEBUG,
    LOADER_MESSAGE_LAYER,
    LOADER_MESSAGE_IMPLEMENTATION,
    LOADER_MESSAGE_ALL,

    LOADER_MESSAGE_FIRST = LOADER_MESSAGE_ERROR,
    LOADER_MESSAGE_LAST = LOADER_MESSAGE_IMPLEMENTATION,
};

enum { LOADER_MESSAGE_COUNT = LOADER_MESSAGE_LAST - LOADER_MESSAGE_FIRST + 1 };

enum {
    LOADER_MESSAGE_ERROR_BIT = (1 << LOADER_MESSAGE_ERROR),
    LOADER_MESSAGE_WARN_BIT = (1 << LOADER_MESSAGE_WARN),
    LOADER_MESSAGE_INFO_BIT = (1 << LOADER_MESSAGE_INFO),
    LOADER_MESSAGE_DEBUG_BIT = (1 << LOADER_MESSAGE_DEBUG),
    LOADER_MESSAGE_LAYER_BIT = (1 << LOADER_MESSAGE_LAYER),
    LOADER_MESSAGE_IMPLEMENTATION_BIT = (1 << LOADER_MESSAGE_IMPLEMENTATION),
    LOADER_MESSAGE_ALL_BIT = LOADER_MESSAGE_ERROR_BIT | LOADER_MESSAGE_WARN_BIT | LOADER_MESSAGE_INFO_BIT |
                             LOADER_MESSAGE_DEBUG_BIT | LOADER_MESSAGE_LAYER_BIT | LOADER_MESSAGE_IMPLEMENTATION_BIT
};

inline int GetLoaderMessageFlags(LoaderMessageType level) {
    int flags = 0;

    switch (level) {
        default:
        case LOADER_MESSAGE_ALL:
        case LOADER_MESSAGE_IMPLEMENTATION:
            flags |= LOADER_MESSAGE_IMPLEMENTATION_BIT;
        case LOADER_MESSAGE_LAYER:
            flags |= LOADER_MESSAGE_LAYER_BIT;
        case LOADER_MESSAGE_DEBUG:
            flags |= LOADER_MESSAGE_DEBUG_BIT;
        case LOADER_MESSAGE_INFO:
            flags |= LOADER_MESSAGE_INFO_BIT;
        case LOADER_MESSAGE_WARN:
            flags |= LOADER_MESSAGE_WARN_BIT;
        case LOADER_MESSAGE_ERROR:
            flags |= LOADER_MESSAGE_ERROR_BIT;
        case LOADER_MESSAGE_NONE:
            flags |= 0;
    }

    return flags;
}

inline LoaderMessageType GetLoaderMessageType(int flags) {
    if (flags == LOADER_MESSAGE_ALL_BIT) {
        return LOADER_MESSAGE_ALL;
    } else if (flags & LOADER_MESSAGE_IMPLEMENTATION_BIT) {
        return LOADER_MESSAGE_IMPLEMENTATION;
    } else if (flags & LOADER_MESSAGE_LAYER_BIT) {
        return LOADER_MESSAGE_LAYER;
    } else if (flags & LOADER_MESSAGE_DEBUG_BIT) {
        return LOADER_MESSAGE_DEBUG;
    } else if (flags & LOADER_MESSAGE_INFO_BIT) {
        return LOADER_MESSAGE_INFO;
    } else if (flags & LOADER_MESSAGE_WARN_BIT) {
        return LOADER_MESSAGE_WARN;
    } else if (flags & LOADER_MESSAGE_ERROR_BIT) {
        return LOADER_MESSAGE_ERROR;
    } else {
        return LOADER_MESSAGE_NONE;
    }
}

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

    void SetActiveApplication(const std::string& name) { this->active_application = name; }
    const std::vector<Application>& GetApplications() const { return applications; }
    const Application& GetActiveApplication() const;
    const Application& GetApplication(std::size_t application_index) const;
    Application& GetApplication(std::size_t application_index);

    const std::string& GetActiveConfiguration() const { return this->active_configuration; }
    void SetActiveConfiguration(const std::string& name) { this->active_configuration = name; }

    const QByteArray& Get(LayoutState state) const;
    void Set(LayoutState state, const QByteArray& data);

    bool GetUseApplicationList() const;
    void SetUseApplicationList(bool enable);

    bool GetUseSystemTray() const { return this->use_system_tray; }
    void SetUseSystemTray(bool enable) { this->use_system_tray = enable; }

    LayersMode GetMode() const;
    void SetMode(LayersMode mode);

    int GetLoaderMessageTypes() const { return this->loader_message_types; }
    void SetLoaderMessageTypes(int types) { this->loader_message_types = types; }

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

    Version vkconfig2_version;
    Version vkconfig3_version;
    LayersMode layers_mode;
    bool use_application_list;
    bool use_system_tray;
    int loader_message_types;

    std::string active_configuration;
    std::string active_application;
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

LoaderMessageType GetLoaderMessageType(const std::string& value);
int GetLoaderMessageTypes(const std::string& values);
std::string GetLoaderMessageToken(LoaderMessageType mesage_type);
std::string GetLoaderMessageTokens(int mesage_types);
