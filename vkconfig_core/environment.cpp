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

#include "environment.h"
#include "util.h"

// Temp:
#include "../vkconfig/configurator.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>

// Saved settings for the application
#define VKCONFIG_KEY_INITIALIZE_FILES "FirstTimeRun"
#define VKCONFIG_KEY_OVERRIDE_MODE "OverrideMode"

#define VKCONFIG_KEY_EXIT "warnAboutShutdownState"
#define VKCONFIG_KEY_RESTART "restartWarning"
#define VKCONFIG_KEY_VKCONFIG_VERSION "vkConfigVersion"

static const char* GetActiveToken(Active active) {
    assert(active >= ACTIVE_FIRST && active <= ACTIVE_LAST);

    static const char* table[] = {
        "activeProfile",  // ACTIVE_CONFIGURATION
        "launchApp"       // ACTIVE_EXECUTABLE
    };
    static_assert(countof(table) == ACTIVE_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[active];
}

static const char* GetActiveDefault(Active active) {
    assert(active >= ACTIVE_FIRST && active <= ACTIVE_LAST);

    static const char* table[] = {
        "Validation - Standard",  // ACTIVE_CONFIGURATION
        ""                        // ACTIVE_EXECUTABLE
    };
    static_assert(countof(table) == ACTIVE_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[active];
}

static const char* GetLayoutStateToken(LayoutState state) {
    assert(state >= LAYOUT_FIRST && state <= LAYOUT_LAST);

    static const char* table[] = {
        "geometry",           // LAYOUT_GEOMETRY
        "windowState",        // LAYOUT_WINDOW_STATE
        "splitter1State",     // LAYOUT_SPLITTER1
        "splitter2State",     // LAYOUT_SPLITTER2
        "splitter3State",     // LAYOUT_SPLITTER3
        "launcherCollapsed",  // LAYOUT_LAUNCHER_COLLAPSED
        "launcherOnClear"     // LAYOUT_LAUNCHER_CLEAR_ON
    };
    static_assert(countof(table) == LAYOUT_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[state];
}

Environment::Environment() : first_run(true), version(Version::VKCONFIG), override_state(OVERRIDE_STATE_GLOBAL_TEMPORARY) {
    for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
        actives[i] = GetActiveDefault(static_cast<Active>(i));
    }

    const bool result = Load();
    assert(result);
}

Environment::~Environment() {
    const bool result = Save();
    assert(result);
}

bool Environment::Notify(Notification notification) {
    if (hidden_notifications[notification]) return true;

    bool hide_notification = true;

    switch (notification) {
        case NOTIFICATION_RESTART: {
            QMessageBox alert;
            alert.setText(
                "Vulkan Layers are fully configured when creating a Vulkan Instance which typically happens at Vulkan Application "
                "start.\n\n"
                "For changes to take effect, running Vulkan Applications should be restarted.");
            alert.setWindowTitle("Any change requires Vulkan Applications restart");
            alert.setIcon(QMessageBox::Warning);
            alert.exec();
        } break;

        case NOTIFICATION_EXIT: {
            QString shut_down_state;

            if (override_state == OVERRIDE_STATE_DISABLED) {
                shut_down_state = "No Vulkan layers override will be active when Vulkan Configurator closes.";
            } else {
                shut_down_state = "Vulkan Layers override will remain in effect when Vulkan Configurator closes.";

                if (override_state & OVERRIDE_FLAG_SELECTED)
                    shut_down_state += " Overrides will be applied only to the application list.";
                else
                    shut_down_state += " Overrides will be applied to ALL Vulkan applications.";
            }

            shut_down_state += "\n\nAre you still ready to close Vulkan Configurator?";

            QMessageBox alert;
            ;
            alert.setWindowTitle("Vulkan Layers configuration state on exit");
            alert.setText(shut_down_state);
            alert.setIcon(QMessageBox::Question);
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setCheckBox(new QCheckBox("Do not show again."));

            int ret_val = alert.exec();
            hide_notification = alert.checkBox()->isChecked();
            if (ret_val == QMessageBox::No) {
                return false;
            }

        } break;

        default: {
            assert(0);
        } break;
    }

    hidden_notifications[notification] = hide_notification;
    return true;
}

bool Environment::Load() {
    QSettings settings;

    // Load "first_run"
    first_run = settings.value(VKCONFIG_KEY_INITIALIZE_FILES, QVariant(first_run)).toBool();

    // Load "version": If the version doesn't exist of it's an old version of vkconfig
    const Version default_version(first_run || !SUPPORT_VKCONFIG_2_0_0 ? version : Version("2.0.0"));
    version = Version(settings.value(VKCONFIG_KEY_VKCONFIG_VERSION, QVariant(default_version.str().c_str())).toString());

    if (version == Version("2.0.0")) {  // The version is too old and unknown, hard reset.
        version = Version::VKCONFIG;
        first_run = true;
        return false;
    }

    // Load 'override_mode"
    if (SUPPORT_VKCONFIG_2_0_1 && version <= Version("2.0.1")) {
        const bool active = settings.value("overrideActive", QVariant(first_run)).toBool();
        const bool active_list = settings.value("applyOnlyToList", QVariant(first_run)).toBool();
        const bool active_persistent = settings.value("keepActiveOnExit", QVariant(first_run)).toBool();
        if (!active)
            override_state = OVERRIDE_STATE_DISABLED;
        else if (active_list && active_persistent)
            override_state = OVERRIDE_STATE_SELECTED_PERSISTENT;
        else if (active_list && !active_persistent)
            override_state = OVERRIDE_STATE_SELECTED_TEMPORARY;
        else if (!active_list && active_persistent)
            override_state = OVERRIDE_STATE_GLOBAL_PERSISTENT;
        else if (!active_list && !active_persistent)
            override_state = OVERRIDE_STATE_GLOBAL_TEMPORARY;
        else {
            override_state = OVERRIDE_STATE_DISABLED;
            assert(0);
        }
    } else {
        override_state = static_cast<OverrideState>(settings.value(VKCONFIG_KEY_OVERRIDE_MODE, QVariant(override_state)).toInt());
    }

    // Load active configuration
    for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
        actives[i] = settings.value(GetActiveToken(static_cast<Active>(i)), actives[i]).toByteArray();
    }

    // Load notifications
    hidden_notifications[NOTIFICATION_RESTART] = settings.value(VKCONFIG_KEY_RESTART, false).toBool();

    // Load layout state
    for (std::size_t i = 0; i < LAYOUT_COUNT; ++i) {
        layout_states[i] = settings.value(GetLayoutStateToken(static_cast<LayoutState>(i))).toByteArray();
    }

    return true;
}

bool Environment::Save() const {
    QSettings settings;

    // Save "first_run"
    settings.setValue(VKCONFIG_KEY_INITIALIZE_FILES, first_run);

    // Save "version"
    settings.setValue(VKCONFIG_KEY_VKCONFIG_VERSION, Version::VKCONFIG.str().c_str());

    // Save 'override_mode"
    settings.setValue(VKCONFIG_KEY_OVERRIDE_MODE, override_state);

    // Save active state
    for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
        settings.setValue(GetActiveToken(static_cast<Active>(i)), actives[i]);
    }

    // Save notifications
    settings.setValue(VKCONFIG_KEY_RESTART, hidden_notifications[NOTIFICATION_RESTART]);

    // Save layout state
    for (std::size_t i = 0; i < LAYOUT_COUNT; ++i) {
        settings.setValue(GetLayoutStateToken(static_cast<LayoutState>(i)), layout_states[i]);
    }

    return true;
}

bool Environment::UseOverride() const { return (override_state & OVERRIDE_FLAG_ACTIVE) != 0; }

bool Environment::UseApplicationListOverrideMode() const { return (override_state & OVERRIDE_FLAG_SELECTED) != 0; }

bool Environment::UsePersistentOverrideMode() const { return (override_state & OVERRIDE_FLAG_PERSISTENT) != 0; }

void Environment::SetMode(OverrideMode mode, bool enabled) {
    switch (mode) {
        case OVERRIDE_MODE_ACTIVE:
            if (enabled)
                override_state = static_cast<OverrideState>(override_state | OVERRIDE_FLAG_ACTIVE);
            else
                override_state = static_cast<OverrideState>(override_state & ~OVERRIDE_FLAG_ACTIVE);
            break;
        case OVERRIDE_MODE_LIST:
            if (enabled)
                override_state = static_cast<OverrideState>(override_state | OVERRIDE_FLAG_SELECTED);
            else
                override_state = static_cast<OverrideState>(override_state & ~OVERRIDE_FLAG_SELECTED);
            break;
        case OVERRIDE_MODE_PERISTENT:
            if (enabled)
                override_state = static_cast<OverrideState>(override_state | OVERRIDE_FLAG_PERSISTENT);
            else
                override_state = static_cast<OverrideState>(override_state & ~OVERRIDE_FLAG_PERSISTENT);
            break;
        default:
            assert(0);
            break;
    }
}

void Environment::Set(LayoutState state, const QByteArray& data) {
    assert(state >= LAYOUT_FIRST && state <= LAYOUT_LAST);
    layout_states[state] = data;
}

const QByteArray& Environment::Get(LayoutState state) const {
    assert(state >= LAYOUT_FIRST && state <= LAYOUT_LAST);
    return layout_states[state];
}

const QString& Environment::Get(Active active) const { return actives[active]; }

void Environment::Set(Active active, const QString& name) { actives[active] = name; }
