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
#include "platform.h"
#include "util.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <cassert>

// Saved settings for the application
#define VKCONFIG_KEY_INITIALIZE_FILES "FirstTimeRun"
#define VKCONFIG_KEY_OVERRIDE_MODE "OverrideMode"

#define VKCONFIG_KEY_EXIT "warnAboutShutdownState"
#define VKCONFIG_KEY_RESTART "restartWarning"
#define VKCONFIG_KEY_VKCONFIG_VERSION "vkConfigVersion"
#define VKCONFIG_KEY_CUSTOM_PATHS "customPaths"

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

Application::Application(const QString& executable_full_path, const QString& arguments)
    : executable_path(QDir::toNativeSeparators(executable_full_path)),
      working_folder(QDir::toNativeSeparators(QFileInfo(executable_full_path).path())),
      arguments(arguments),
      log_file(
          QDir::toNativeSeparators(QDir::homePath() + QDir::separator() + QFileInfo(executable_full_path).baseName() + ".txt")),
      override_layers(true) {}

Environment::Environment(PathManager& paths) : paths(paths) {
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

        default:
            assert(0);
            break;
    }

    hidden_notifications[notification] = hide_notification;
    return true;
}

void Environment::Reset(ResetMode mode) {
    switch (mode) {
        case DEFAULT: {
            first_run = true;
            version = Version::VKCONFIG;
            override_state = OVERRIDE_STATE_GLOBAL_TEMPORARY;

            for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
                actives[i] = GetActiveDefault(static_cast<Active>(i));
            }

            UpdateDefaultApplications(true);
            break;
        }
        case SYSTEM: {
            const bool result_env = Load();
            assert(result_env);
            const bool result_path = paths.Load();
            assert(result_path);
            break;
        }
        default: {
            assert(0);
            break;
        }
    }
}

bool Environment::Load() {
    Reset(DEFAULT);

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

    // Load custom paths
    custom_layer_paths = settings.value(VKCONFIG_KEY_CUSTOM_PATHS).toStringList();

    // Load application list
    const bool result = LoadApplications();
    assert(result);

    return true;
}

bool Environment::LoadApplications() {
    /////////////////////////////////////////////////////////////
    // Now, use the list. If the file doesn't exist, this is not an error
    QString data;
    const QString& application_list_json = paths.GetFullPath(FILENAME_APPLIST);
    QFile file(application_list_json);
    if (file.open(QFile::ReadOnly)) {
        data = file.readAll();
        file.close();
    }

    QJsonDocument json_app_list;
    json_app_list = QJsonDocument::fromJson(data.toLocal8Bit());
    if (json_app_list.isObject()) {
        if (!json_app_list.isEmpty()) {
            // Get the list of apps
            QStringList app_keys;
            QJsonObject json_doc_object = json_app_list.object();
            app_keys = json_doc_object.keys();

            // Get them...
            for (int i = 0; i < app_keys.length(); i++) {
                QJsonValue app_value = json_doc_object.value(app_keys[i]);
                QJsonObject app_object = app_value.toObject();

                Application application;
                application.working_folder = app_object.value("app_folder").toString();
                application.executable_path = app_object.value("app_path").toString();
                application.override_layers = !app_object.value("exclude_override").toBool();
                application.log_file = app_object.value("log_file").toString();

                // Arguments are in an array to make room for adding more in a future version
                QJsonArray args = app_object.value("command_lines").toArray();
                application.arguments = args[0].toString();

                applications.push_back(application);
            }
        }
    }

    UpdateDefaultApplications(first_run || applications.empty());

    return true;
}

static QString GetDefaultExecutablePath(const QString& executable_name) {
#if PLATFORM_MACOS
    static const char* DEFAULT_PATH = "/../..";
#elif PLATFORM_WINDOWS || PLATFORM_LINUX
    static const char* DEFAULT_PATH = "";
#else
#error "Unknown platform"
#endif

    // Using relative path to vkconfig
    {
        const QString search_path = QString("../bin") + DEFAULT_PATH + executable_name;
        QFileInfo file_info(search_path);
        if (file_info.exists())  // Couldn't find vkcube
            return file_info.filePath();
    }

    // Using VULKAN_SDK environement variable
    {
        const QString search_path = QString(qgetenv("VULKAN_SDK")) + "/bin" + DEFAULT_PATH + executable_name;
        QFileInfo file_info(search_path);
        if (file_info.exists())  // Couldn't find vkcube
            return file_info.absoluteFilePath();
    }

    return "";
}

/////////////////////////////////////////////////////////////////////////////
// Search for vkcube and add it to the app list.
void Environment::UpdateDefaultApplications(const bool add_default_applications) {
    std::vector<Application> new_applications;

    // Remove application that can't be found
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        const Application& application = applications[i];

        const QFileInfo file_info(application.executable_path);
        if (!file_info.exists()) continue;

        new_applications.push_back(application);
    }

    if (!add_default_applications) return;

#if PLATFORM_WINDOWS
    static const char* SUFFIX = ".exe";
#elif PLATFORM_MACOS
    static const char* SUFFIX = ".app";
#elif PLATFORM_LINUX
    static const char* SUFFIX = "";
#else
#error "Unknown platform"
#endif

    struct Default {
        QString name;
        QString arguments;
    };

    static const Default defaults[] = {{QDir::toNativeSeparators("/vkcube"), "--suppress_popups"},
                                       {QDir::toNativeSeparators("/vkcubepp"), "--suppress_popups"}};

    for (std::size_t name_index = 0, name_count = countof(defaults); name_index < name_count; ++name_index) {
        bool found = false;

        for (std::size_t i = 0; i < applications.size(); ++i) {
            const Application& application = applications[i];

            if (!application.executable_path.endsWith(defaults[name_index].name + SUFFIX)) continue;

            found;
            break;
        }

        if (found) continue;

        const QString executable_path = GetDefaultExecutablePath(defaults[name_index].name + SUFFIX);
        if (executable_path.isEmpty()) continue;  // application could not be found..

        Application application(executable_path, "--suppress_popups");

        // On all operating systems, but Windows we keep running into problems with this ending up
        // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
        // initially will be set to the users home folder across all OS's. This is highly visible
        // in the application launcher and should not present a usability issue. The developer can
        // easily change this later to anywhere they like.
        application.log_file = paths.GetPath(PATH_HOME) + defaults[name_index].name + ".txt";

        new_applications.push_back(application);
    }

    std::swap(applications, new_applications);
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

    // Save custom paths
    settings.setValue(VKCONFIG_KEY_CUSTOM_PATHS, custom_layer_paths);

    const bool result = SaveApplications();
    assert(result);

    return true;
}

bool Environment::SaveApplications() const {
    QJsonObject root;

    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        // Build an array of appnames with associated data
        QJsonObject application_object;
        application_object.insert("app_path", applications[i].executable_path);
        application_object.insert("app_folder", applications[i].working_folder);
        application_object.insert("exclude_override", !applications[i].override_layers);
        application_object.insert("log_file", applications[i].log_file);

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(applications[i].arguments));  // [J] PROBABLY

        application_object.insert("command_lines", argsArray);
        root.insert(QFileInfo(applications[i].executable_path).fileName(), application_object);
    }

    const QString& app_list_json = paths.GetFullPath(FILENAME_APPLIST);
    QFile file(app_list_json);
    file.open(QFile::WriteOnly);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    return true;
}

void Environment::SelectActiveApplication(int application_index) {
    if (application_index < 0) return;

    assert(application_index >= 0 && application_index < applications.size());

    Set(ACTIVE_APPLICATION, applications[application_index].executable_path);
}

int Environment::GetActiveApplicationIndex() const {
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (applications[i].executable_path == Get(ACTIVE_APPLICATION)) {
            return static_cast<int>(i);
        }
    }

    return 0;  // Not found, but the list is present, so return the first item.
}

bool Environment::HasOverriddenApplications() const {
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (applications[i].override_layers) return true;
    }

    return false;
}

bool Environment::AppendApplication(const Application& application) {
    applications.push_back(application);
    return true;
}

bool Environment::RemoveApplication(int application_index) {
    assert(!applications.empty());
    assert(application_index >= 0 && application_index < applications.size());

    if (applications.size() == 1) {
        applications.clear();
        return true;
    }

    std::vector<Application> new_applications;
    new_applications.reserve(applications.size() - 1);

    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (i == application_index) continue;
        new_applications.push_back(applications[i]);
    }

    std::swap(applications, new_applications);
    return true;
}

const Application& Environment::GetActiveApplication() const {
    assert(!applications.empty());

    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (applications[i].executable_path == Get(ACTIVE_APPLICATION)) {
            return applications[i];
        }
    }

    return applications[0];  // Not found, but the list is present, so return the first item.
}

const Application& Environment::GetApplication(int application_index) const {
    assert(application_index >= 0 && application_index < applications.size());

    return applications[application_index];
}

Application& Environment::GetApplication(int application_index) {
    assert(application_index >= 0 && application_index < applications.size());

    return applications[application_index];
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

bool Environment::AppendCustomLayerPath(const QString& path) {
    assert(!path.isEmpty());

    for (int i = 0, n = custom_layer_paths.size(); i < n; ++i) {
        if (custom_layer_paths[i] == QDir::toNativeSeparators(path)) {
            return false;
        }
    }

    custom_layer_paths.append(QDir::toNativeSeparators(path));
    return true;
}

bool Environment::RemoveCustomLayerPath(const QString& path) {
    assert(!path.isEmpty());

    for (int i = 0, n = custom_layer_paths.size(); i < n; ++i) {
        if (custom_layer_paths[i] == QDir::toNativeSeparators(path)) {
            custom_layer_paths.removeAt(i);
            return true;
        }
    }
    return false;
}
