/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
#include <QString>
#include <QStringList>

#include <array>
#include <vector>

enum Notification {
    NOTIFICATION_RESTART = 0,
    NOTIFICATION_EXIT,

    NOTIFICATION_FIRST = NOTIFICATION_RESTART,
    NOTIFICATION_LAST = NOTIFICATION_EXIT
};

enum { NOTIFICATION_COUNT = NOTIFICATION_LAST - NOTIFICATION_FIRST + 1 };

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

class Environment {
   public:
    Environment(PathManager& paths);
    ~Environment();

    enum ResetMode { DEFAULT = 0, SYSTEM };

    bool Notify(Notification notification);

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

    const QString& Get(Active active) const;
    void Set(Active active, const QString& key);

    const QByteArray& Get(LayoutState state) const;
    void Set(LayoutState state, const QByteArray& data);

    bool UseOverride() const;
    bool UseApplicationListOverrideMode() const;
    bool UsePersistentOverrideMode() const;

    void SetMode(OverrideMode mode, bool enabled);

    const bool running_as_administrator;  // Are we being "Run as Administrator"

    bool first_run;

    bool AppendCustomLayerPath(const QString& path);
    bool RemoveCustomLayerPath(const QString& path);
    const QStringList& GetCustomLayerPaths() const { return custom_layer_paths; }

   private:
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    Version version;
    OverrideState override_state;

    std::array<QString, ACTIVE_COUNT> actives;
    std::array<bool, NOTIFICATION_COUNT> hidden_notifications;
    std::array<QByteArray, LAYOUT_COUNT> layout_states;
    QStringList custom_layer_paths;
    std::vector<Application> applications;

    PathManager& paths_manager;

   public:
    const PathManager& paths;
};

bool ExactExecutableFromAppBundle(QString& path);

// Search for all the applications in the list, an remove the application which executable can't be found
std::vector<Application> RemoveMissingApplications(const std::vector<Application>& applications);

// Create a list of default applications, eg vkcube
std::vector<Application> CreateDefaultApplications(const PathManager& paths);

// Update default applications path to use relative path (really useful only on Windows)
std::vector<Application> UpdateDefaultApplications(const PathManager& paths, const std::vector<Application>& applications);
