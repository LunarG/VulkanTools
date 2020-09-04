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

#include <QByteArray>
#include <QString>
#include <QStringList>

#include <array>

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
    LAYOUT_GEOMETRY = 0,
    LAYOUT_WINDOW_STATE,
    LAYOUT_SPLITTER1,
    LAYOUT_SPLITTER2,
    LAYOUT_SPLITTER3,
    LAYOUT_LAUNCHER_COLLAPSED,
    LAYOUT_LAUNCHER_CLEAR_ON,

    LAYOUT_FIRST = LAYOUT_GEOMETRY,
    LAYOUT_LAST = LAYOUT_LAUNCHER_CLEAR_ON,
};

enum { LAYOUT_COUNT = LAYOUT_LAST - LAYOUT_FIRST + 1 };

enum Active {
    ACTIVE_CONFIGURATION = 0,
    ACTIVE_EXECUTABLE,

    ACTIVE_FIRST = ACTIVE_CONFIGURATION,
    ACTIVE_LAST = ACTIVE_EXECUTABLE,
};

enum { ACTIVE_COUNT = ACTIVE_LAST - ACTIVE_FIRST + 1 };

class Environment {
   public:
    Environment(bool test_mode = false);
    ~Environment();

    bool Notify(Notification notification);

    void Reset();

    bool Load();
    bool Save() const;

    const QString& Get(Active active) const;
    void Set(Active active, const QString& name);

    const QByteArray& Get(LayoutState state) const;
    void Set(LayoutState state, const QByteArray& data);

    bool UseOverride() const;
    bool UseApplicationListOverrideMode() const;
    bool UsePersistentOverrideMode() const;

    void SetMode(OverrideMode mode, bool enabled);

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

    const bool test_mode;
};
