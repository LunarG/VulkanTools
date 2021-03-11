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

#include "environment.h"
#include "platform.h"
#include "util.h"

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
#include <shlobj.h>
#endif

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <cassert>

// Saved settings for the application
#define VKCONFIG_KEY_INITIALIZE_FILES "FirstTimeRun"
#define VKCONFIG_KEY_OVERRIDE_MODE "OverrideMode"

#define VKCONFIG_KEY_EXIT "warnAboutShutdownState"
#define VKCONFIG_KEY_RESTART "restartWarning"
#define VKCONFIG_KEY_VKCONFIG_VERSION "vkConfigVersion"
#define VKCONFIG_KEY_CUSTOM_PATHS "customPaths"

static const char* GetNotificationToken(Notification notification) {
    assert(notification >= NOTIFICATION_FIRST && notification <= NOTIFICATION_LAST);

    static const char* table[] = {
        "restartWarning",         // NOTIFICATION_RESTART
        "warnAboutShutdownState"  // NOTIFICATION_EXIT
    };
    static_assert(countof(table) == NOTIFICATION_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[notification];
}

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
        "geometry",            // LAYOUT_GEOMETRY
        "windowState",         // LAYOUT_WINDOW_STATE
        "splitter1State",      // LAYOUT_MAIN_SPLITTER1
        "splitter2State",      // LAYOUT_MAIN_SPLITTER2
        "splitter3State",      // LAYOUT_MAIN_SPLITTER3
        "layerGeometry",       // LAYOUT_GEOMETRY_SPLITTER
        "splitterLayerState",  // LAYOUT_LAYER_SPLITTER
        "launcherCollapsed",   // LAYOUT_LAUNCHER_COLLAPSED
        "launcherOnClear"      // LAYOUT_LAUNCHER_CLEAR_ON
    };
    static_assert(countof(table) == LAYOUT_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[state];
}

Environment::Environment(PathManager& paths, const Version& api_version)
    : api_version(api_version),
// Hack for GitHub C.I.
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS && (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
      running_as_administrator(IsUserAnAdmin()),
#else
      running_as_administrator(false),
#endif
      paths_manager(paths),
      paths(paths_manager) {

    const bool result = Load();
    assert(result);
}

Environment::~Environment() {
    const bool result = Save();
    assert(result);
}

bool Environment::Notify(Notification notification) {
    if (hidden_notifications[notification]) return true;

    bool result = true;
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
            std::string shut_down_state;

            if (override_state & OVERRIDE_FLAG_PERSISTENT) {
                shut_down_state = "Vulkan Layers override will remain in effect when Vulkan Configurator closes.";

                if (override_state & OVERRIDE_FLAG_SELECTED)
                    shut_down_state += " Overrides will be applied only to the application list.";
                else
                    shut_down_state += " Overrides will be applied to ALL Vulkan applications.";
            } else {
                shut_down_state = "No Vulkan layers override will be active when Vulkan Configurator closes.";
            }

            QMessageBox alert;
            alert.setWindowTitle("Vulkan Layers configuration state on exit");
            alert.setText(shut_down_state.c_str());
            alert.setIcon(QMessageBox::Question);
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setCheckBox(new QCheckBox("Do not show again."));
            alert.setInformativeText("Are you still ready to close Vulkan Configurator?");

            int ret_val = alert.exec();
            hide_notification = alert.checkBox()->isChecked();
            if (ret_val == QMessageBox::No) {
                result = false;
            }
        } break;

        default:
            assert(0);
            break;
    }

    hidden_notifications[notification] = hide_notification;
    return result;
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

            applications = CreateDefaultApplications();

            Set(ACTIVE_CONFIGURATION, "Validation");
            break;
        }
        case SYSTEM: {
            const bool result_env = Load();
            assert(result_env);
            const bool result_path = paths_manager.Load();
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
    version =
        Version(settings.value(VKCONFIG_KEY_VKCONFIG_VERSION, QVariant(default_version.str().c_str())).toString().toStdString());

    if (version == Version("2.0.0")) {  // The version is an old development version, unknown and unsupported, hard reset.
        Reset(DEFAULT);
        return true;
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
        actives[i] = settings.value(GetActiveToken(static_cast<Active>(i)), actives[i].c_str()).toString().toStdString();
    }

    // Load notifications
    for (std::size_t i = 0; i < NOTIFICATION_COUNT; ++i) {
        hidden_notifications[i] = settings.value(GetNotificationToken(static_cast<Notification>(i)), false).toBool();
    }

    // Load layout state
    for (std::size_t i = 0; i < LAYOUT_COUNT; ++i) {
        layout_states[i] = settings.value(GetLayoutStateToken(static_cast<LayoutState>(i))).toByteArray();
    }

    // Load default configuration already init
    this->default_configuration_filenames = ConvertString(settings.value("default_configuration_files").toStringList());

    // Load custom paths
    user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_GUI] =
        ConvertString(settings.value(VKCONFIG_KEY_CUSTOM_PATHS).toStringList());

    // See if the VK_LAYER_PATH environment variable is set. If so, parse it and
    // assemble a list of paths that take precidence for layer discovery.
    const QString VK_LAYER_PATH(qgetenv("VK_LAYER_PATH"));
    if (!VK_LAYER_PATH.isEmpty()) {
        user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_ENV] =
            ConvertString(QString(qgetenv("VK_LAYER_PATH")).split(GetPlatformString(PLATFORM_STRING_SEPARATOR)));
    } else {
        user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_ENV].clear();
    }

    // Load application list
    const bool result = LoadApplications();
    assert(result);

    return result;
}

bool Environment::LoadApplications() {
    const std::string& application_list_json = paths.GetFullPath(FILENAME_APPLIST);
    QFile file(application_list_json.c_str());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {  // if applist.json exist, load saved applications
        QString data = file.readAll();
        file.close();

        applications.clear();

        if (!data.isEmpty()) {
            const QJsonDocument& json_doc = QJsonDocument::fromJson(data.toLocal8Bit());
            assert(json_doc.isObject());
            if (!json_doc.isEmpty()) {
                // Get the list of apps
                const QJsonObject& json_doc_object = json_doc.object();
                const QStringList& app_keys = json_doc_object.keys();

                for (int i = 0, n = app_keys.length(); i < n; ++i) {
                    const QJsonValue& app_value = json_doc_object.value(app_keys[i]);
                    const QJsonObject& app_object = app_value.toObject();

                    Application application;
                    application.executable_path = app_object.value("app_path").toString().toStdString();
                    application.working_folder = app_object.value("app_folder").toString().toStdString();
                    application.override_layers = !app_object.value("exclude_override").toBool();
                    application.log_file = app_object.value("log_file").toString().toStdString();

                    // Arguments are in an array to make room for adding more in a future version
                    const QJsonArray& args = app_object.value("command_lines").toArray();
                    application.arguments = args[0].toString().toStdString();

                    applications.push_back(application);
                }
            }
        }

        applications = RemoveMissingApplications(applications);
        if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) applications = UpdateDefaultApplications(applications);
        if (applications.empty()) applications = CreateDefaultApplications();
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
        settings.setValue(GetActiveToken(static_cast<Active>(i)), actives[i].c_str());
    }

    // Save notifications
    for (std::size_t i = 0; i < NOTIFICATION_COUNT; ++i) {
        settings.setValue(GetNotificationToken(static_cast<Notification>(i)), hidden_notifications[i]);
    }

    // Save layout state
    for (std::size_t i = 0; i < LAYOUT_COUNT; ++i) {
        settings.setValue(GetLayoutStateToken(static_cast<LayoutState>(i)), layout_states[i]);
    }

    // Save custom paths
    settings.setValue(VKCONFIG_KEY_CUSTOM_PATHS, ConvertString(user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_GUI]));

    // Save default configuration initizalized
    settings.setValue("default_configuration_files", ConvertString(this->default_configuration_filenames));

    const bool result = SaveApplications();
    assert(result);

    return true;
}

bool Environment::SaveApplications() const {
    QJsonObject root;
    // root.insert("file_format_version", Version::VKCONFIG.str().c_str());

    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        // Build an array of appnames with associated data
        QJsonObject application_object;
        application_object.insert("app_path", applications[i].executable_path.c_str());
        application_object.insert("app_folder", applications[i].working_folder.c_str());
        application_object.insert("exclude_override", !applications[i].override_layers);
        application_object.insert("log_file", applications[i].log_file.c_str());

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(applications[i].arguments.c_str()));

        application_object.insert("command_lines", argsArray);
        root.insert(QFileInfo(applications[i].executable_path.c_str()).fileName(), application_object);
    }

    const std::string& app_list_json = paths.GetFullPath(FILENAME_APPLIST);
    assert(QFileInfo(app_list_json.c_str()).absoluteDir().exists());

    QFile file(app_list_json.c_str());
    const bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    return true;
}

void Environment::SelectActiveApplication(std::size_t application_index) {
    assert(application_index < applications.size());

    Set(ACTIVE_APPLICATION, applications[application_index].executable_path.c_str());
}

int Environment::GetActiveApplicationIndex() const {
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (applications[i].executable_path.c_str() == Get(ACTIVE_APPLICATION)) {
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

bool Environment::RemoveApplication(std::size_t application_index) {
    assert(!applications.empty());
    assert(application_index < applications.size());

    if (applications.size() == 1u) {
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
        if (applications[i].executable_path.c_str() == Get(ACTIVE_APPLICATION)) {
            return applications[i];
        }
    }

    return applications[0];  // Not found, but the list is present, so return the first item.
}

const Application& Environment::GetApplication(std::size_t application_index) const {
    assert(application_index < applications.size());

    return applications[application_index];
}

Application& Environment::GetApplication(std::size_t application_index) {
    assert(application_index < applications.size());

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

const std::string& Environment::Get(Active active) const { return actives[active]; }

void Environment::Set(Active active, const std::string& key) { actives[active] = key; }

bool Environment::AppendCustomLayerPath(const std::string& path) {
    assert(!path.empty());

    std::vector<std::string>& custom_layer_paths_gui = user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_GUI];

    for (std::size_t i = 0, n = custom_layer_paths_gui.size(); i < n; ++i) {
        if (ConvertNativeSeparators(custom_layer_paths_gui[i]) == ConvertNativeSeparators(path)) {
            return false;
        }
    }

    custom_layer_paths_gui.push_back(ConvertNativeSeparators(path).c_str());
    return true;
}

bool Environment::RemoveCustomLayerPath(const std::string& path) {
    assert(!path.empty());

    bool found = false;

    std::vector<std::string>& custom_layer_paths_gui = user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_GUI];

    std::vector<std::string> new_custom_layer_paths_gui;
    for (std::size_t i = 0, n = custom_layer_paths_gui.size(); i < n; ++i) {
        if (custom_layer_paths_gui[i] == ConvertNativeSeparators(path)) {
            found = true;
            continue;
        }

        new_custom_layer_paths_gui.push_back(custom_layer_paths_gui[i]);
    }

    std::swap(custom_layer_paths_gui, new_custom_layer_paths_gui);
    return found;
}

void Environment::ClearCustomLayerPath() { user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_GUI].clear(); }

bool Environment::IsDefaultConfigurationInit(const std::string& default_configuration_filename) const {
    for (std::size_t i = 0, n = default_configuration_filenames.size(); i < n; ++i) {
        if (default_configuration_filenames[i] == default_configuration_filename) return true;
    }

    return false;
}

void Environment::InitDefaultConfiguration(const std::string& configuration_filename) {
    AppendString(this->default_configuration_filenames, configuration_filename);
}

///////////////////////////////////////////////////////////////////////////
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
/// Note, not ALL macOS executables are in a bundle, so if a non-bundled
/// executable is fed in here, it will silently just return without
/// modifying the path (which will be the correct behavior)
bool ExactExecutableFromAppBundle(std::string& app_path) {
    std::string path = app_path;
    path += "/Contents/";
    std::string list_file = path + "Info.plist";
    QFile file(list_file.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream stream(&file);

    // Read a line at a time looking for the executable tag
    QString line_buffer;
    while (!stream.atEnd()) {
        line_buffer = stream.readLine();
        if (line_buffer.contains("<key>CFBundleExecutable</key>")) {  // Exe follows this
            line_buffer = stream.readLine();                          // <string>Qt Creator</string>
            char* cExeName = new char[line_buffer.length()];          // Prevent buffer overrun

            const char* pStart = strstr(line_buffer.toUtf8().constData(), "<string>");
            if (pStart == nullptr) return false;

            // We found it, now extract it out
            pStart += 8;
            int iIndex = 0;
            while (*pStart != '<') {
                cExeName[iIndex++] = *pStart++;
            }
            cExeName[iIndex] = '\0';

            // Complete the partial path
            path += "MacOS/";
            path += cExeName;

            // Return original if not found, but root if found
            app_path = path;

            delete[] cExeName;
            break;
        }
    }

    file.close();

    return true;
}

static std::string GetDefaultExecutablePath(const std::string& executable_name) {
    static const char* DEFAULT_PATH = VKC_PLATFORM == VKC_PLATFORM_MACOS ? "/../.." : "";

    // Using VULKAN_SDK environement variable
    const QString env(qgetenv("VULKAN_SDK"));
    if (!env.isEmpty()) {
        const std::string search_path =
            env.toStdString() + GetPlatformString(PLATFORM_STRING_SDK_BIN_DIR) + DEFAULT_PATH + executable_name.c_str();
        const QFileInfo file_info(search_path.c_str());
        if (file_info.exists()) {
            return file_info.absoluteFilePath().toStdString();
        }
    }

    // Such the default applications from package installation (Linux)
    if (VKC_PLATFORM == VKC_PLATFORM_LINUX) {
        const std::string search_path = std::string("/usr/bin") + DEFAULT_PATH + executable_name.c_str();
        const QFileInfo file_info(search_path.c_str());
        if (file_info.exists()) {
            return file_info.absoluteFilePath().toStdString();
        }
    } else if (VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        std::string search_path = std::string("/Applications") + executable_name.c_str();
        const QFileInfo file_info(search_path.c_str());
        if (file_info.exists() && ExactExecutableFromAppBundle(search_path)) {
            return search_path;
        }
    }

    // Using relative path to vkconfig in case SDK is not "installed"
    if (VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        std::string search_path = std::string("..") + DEFAULT_PATH + executable_name.c_str();
        const QFileInfo file_info(search_path.c_str());
        if (file_info.exists() && ExactExecutableFromAppBundle(search_path)) {
            return search_path;
        }
    } else {
        const QString search_path = QString(".") + DEFAULT_PATH + executable_name.c_str();
        const QFileInfo file_info(search_path);
        if (file_info.exists()) {
            return file_info.absoluteFilePath().toStdString();
        }
    }

    return "";
}

struct DefaultApplication {
    std::string key;
    std::string arguments;
};

static const DefaultApplication defaults_applications[] = {{ConvertNativeSeparators("/vkcube"), "--suppress_popups"},
                                                           {ConvertNativeSeparators("/vkcubepp"), "--suppress_popups"}};

static Application CreateDefaultApplication(const DefaultApplication& default_application) {
    const std::string executable_path =
        GetDefaultExecutablePath((default_application.key + GetPlatformString(PLATFORM_STRING_APP_SUFFIX)).c_str());
    if (executable_path.empty()) Application();  // application could not be found..

    Application application(executable_path, "--suppress_popups");

    // On all operating systems, but Windows we keep running into problems with this ending up
    // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
    // initially will be set to the users home folder across all OS's. This is highly visible
    // in the application launcher and should not present a usability issue. The developer can
    // easily change this later to anywhere they like.
    application.log_file = std::string("${LOCAL}") + default_application.key + ".txt";

    return application;
}

std::vector<Application> CreateDefaultApplications() {
    std::vector<Application> new_applications;

    for (std::size_t name_index = 0, name_count = countof(defaults_applications); name_index < name_count; ++name_index) {
        const Application& application = CreateDefaultApplication(defaults_applications[name_index]);

        if (application.executable_path.empty()) continue;

        new_applications.push_back(application);
    }

    return new_applications;
}

std::vector<Application> RemoveMissingApplications(const std::vector<Application>& applications) {
    std::vector<Application> valid_applications;

    // Remove applications that can't be found
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        const Application& application = applications[i];

        const QFileInfo file_info(application.executable_path.c_str());
        if (!file_info.exists()) continue;

        valid_applications.push_back(application);
    }

    return valid_applications;
}

std::vector<Application> UpdateDefaultApplications(const std::vector<Application>& applications) {
    std::vector<Application> search_applications;
    std::vector<Application> updated_applications = applications;

    for (std::size_t default_index = 0, default_count = countof(defaults_applications); default_index < default_count;
         ++default_index) {
        std::string const defaults_name = defaults_applications[default_index].key + GetPlatformString(PLATFORM_STRING_APP_SUFFIX);

        std::swap(updated_applications, search_applications);
        updated_applications.clear();

        for (std::size_t application_index = 0, application_count = search_applications.size();
             application_index < application_count; ++application_index) {
            const Application& application = search_applications[application_index];

            if (QString(application.executable_path.c_str()).endsWith(defaults_name.c_str())) {
                updated_applications.push_back(CreateDefaultApplication(defaults_applications[default_index]));
            } else {
                updated_applications.push_back(application);
            }
        }
    }

    return updated_applications;
}
