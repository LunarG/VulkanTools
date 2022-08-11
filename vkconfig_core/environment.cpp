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
#include "setting.h"
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
#include <QTextStream>

#include <cassert>

// Saved settings for the application
#define VKCONFIG_KEY_INITIALIZE_FILES "FirstTimeRun"
#define VKCONFIG_KEY_OVERRIDE_MODE "OverrideMode"
#define VKCONFIG_KEY_LOADER_MESSAGE "LoaderMessage"

#define VKCONFIG_KEY_VKCONFIG_VERSION "vkConfigVersion"
#define VKCONFIG_KEY_CUSTOM_PATHS "customPaths"

static const char* GetApplicationSuffix() {
    static const char* TABLE[] = {
        ".exe",  // PLATFORM_WINDOWS
        "",      // PLATFORM_LINUX
        ".app",  // PLATFORM_MACOS
        "N/A"    // PLATFORM_ANDROID
    };
    static_assert(countof(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
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
        "Validation",  // ACTIVE_CONFIGURATION
        ""             // ACTIVE_EXECUTABLE
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

LoaderMessageLevel GetLoaderDebug(const std::string& value) {
    for (int i = LOADER_MESSAGE_FIRST, n = LOADER_MESSAGE_LAST; i <= n; ++i) {
        const LoaderMessageLevel level = static_cast<LoaderMessageLevel>(i);
        if (GetLoaderDebugToken(level) == value) return level;
    }

    return LOADER_MESSAGE_NONE;
}

std::string GetLoaderDebugToken(LoaderMessageLevel level) {
    static const char* LOADER_MESSAGE_LEVEL[]{
        "",        // LOADER_MESSAGE_NONE
        "error",   // LOADER_MESSAGE_ERROR
        "warn",    // LOADER_MESSAGE_WARN
        "info",    // LOADER_MESSAGE_INFO
        "debug",   // LOADER_MESSAGE_DEBUG
        "layer",   // LOADER_MESSAGE_LAYER
        "implem",  // LOADER_MESSAGE_IMPLEMENTATION
        "all"      // LOADER_MESSAGE_ALL
    };

    static_assert(countof(LOADER_MESSAGE_LEVEL) == LOADER_MESSAGE_COUNT,
                  "The tranlation table size doesn't match the enum number of elements");

    return LOADER_MESSAGE_LEVEL[level];
}

Environment::Environment(PathManager& paths, const Version& api_version)
    : api_version(api_version),
      loader_message_level(GetLoaderDebug(qgetenv("VK_LOADER_DEBUG").toStdString())),
      paths_manager(paths),
      paths(paths_manager) {
    const bool result = Load();
    assert(result);
}

Environment::~Environment() {
    const bool result = Save();
    assert(result);
}

void Environment::Reset(ResetMode mode) {
    switch (mode) {
        case DEFAULT: {
            first_run = true;
            version = Version::LAYER_CONFIG;
            override_state = OVERRIDE_STATE_GLOBAL_TEMPORARY;

            for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
                actives[i] = GetActiveDefault(static_cast<Active>(i));
            }

            applications = CreateDefaultApplications();

            Set(ACTIVE_CONFIGURATION, "Validation");
            break;
        }
        case CLEAR: {
            this->Reset(DEFAULT);

            QSettings settings;
            settings.setValue("VKCONFIG_WIDGET_SETTING_INT", false);
            settings.setValue("VKCONFIG_WIDGET_SETTING_FLOAT", false);
            settings.setValue("VKCONFIG_WIDGET_SETTING_FRAMES", false);
            settings.setValue("VKCONFIG_WARN_MISSING_LAYERS_IGNORE", false);
            settings.setValue("VKCONFIG_WARN_CORE_SHADER_IGNORE", false);

            settings.setValue("overrideActive", false);
            settings.setValue("applyOnlyToList", false);
            settings.setValue("keepActiveOnExit", false);

            settings.setValue("restartWarning", false);
            settings.setValue("warnAboutShutdownState", false);

            const std::string loader_debug_message(qgetenv("VK_LOADER_DEBUG"));
            if (loader_debug_message.empty()) {
                loader_message_level = LOADER_MESSAGE_NONE;
            } else {
                loader_message_level = GetLoaderDebug(loader_debug_message);
            }
            settings.setValue(VKCONFIG_KEY_LOADER_MESSAGE, static_cast<int>(loader_message_level));
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
    this->first_run = settings.value(VKCONFIG_KEY_INITIALIZE_FILES, first_run).toBool();

    // Load "version": If the version doesn't exist of it's an old version of vkconfig
    this->version =
        Version(settings.value(VKCONFIG_KEY_VKCONFIG_VERSION, Version::VKCONFIG.str().c_str()).toString().toStdString());

    // Load 'override_mode"
    override_state = static_cast<OverrideState>(settings.value(VKCONFIG_KEY_OVERRIDE_MODE, QVariant(override_state)).toInt());

    // Load loader debug message state
    loader_message_level = static_cast<LoaderMessageLevel>(
        settings.value(VKCONFIG_KEY_LOADER_MESSAGE, static_cast<int>(loader_message_level)).toInt());

    // Load active configuration
    for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
        actives[i] = settings.value(GetActiveToken(static_cast<Active>(i)), actives[i].c_str()).toString().toStdString();
    }

    // Load layout state
    for (std::size_t i = 0; i < LAYOUT_COUNT; ++i) {
        layout_states[i] = settings.value(GetLayoutStateToken(static_cast<LayoutState>(i))).toByteArray();
    }

    // Load default configuration already init
    this->default_configuration_filenames = ConvertString(settings.value("default_configuration_files").toStringList());

    const char* SEPARATOR = GetToken(PARSE_ENV_VAR);

    // See if the VK_LAYER_PATH environment variable is set. If so, parse it and
    // assemble a list of paths that take precidence for layer discovery.
    const QString VK_LAYER_PATH(qgetenv("VK_LAYER_PATH"));
    if (!VK_LAYER_PATH.isEmpty()) {
        this->user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_ENV_SET] =
            ConvertString(QString(qgetenv("VK_LAYER_PATH")).split(SEPARATOR));
    } else {
        this->user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_ENV_SET].clear();
    }

    // See if the VK_ADD_LAYER_PATH environment variable is set. If so, parse it and
    // assemble a list of paths that take precidence for layer discovery.
    const QString VK_ADD_LAYER_PATH(qgetenv("VK_ADD_LAYER_PATH"));
    if (!VK_ADD_LAYER_PATH.isEmpty()) {
        this->user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_ENV_ADD] =
            ConvertString(QString(qgetenv("VK_ADD_LAYER_PATH")).split(SEPARATOR));
    } else {
        this->user_defined_layers_paths[USER_DEFINED_LAYERS_PATHS_ENV_ADD].clear();
    }

    // Load application list
    const bool result = LoadApplications();
    assert(result);

    return result;
}

bool Environment::LoadApplications() {
    const std::string& application_list_json = GetPath(BUILTIN_PATH_APPLIST);
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
                    application.app_name = app_object.value("app_name").toString().toStdString();
                    application.executable_path = app_object.value("app_path").toString().toStdString();
                    application.working_folder = app_object.value("app_folder").toString().toStdString();
                    application.override_layers = !app_object.value("exclude_override").toBool();
                    application.log_file = app_object.value("log_file").toString().toStdString();
                    if (application.app_name.length() == 0) {
                        std::string path = application.executable_path.c_str();
                        if (path.find(GetNativeSeparator()) != std::string::npos) {
                            application.app_name = path.substr(path.rfind(GetNativeSeparator()) + 1);
                        } else {
                            application.app_name = path;
                        }
                    }

                    // Arguments are in an array to make room for adding more in a future version
                    const QJsonArray& args = app_object.value("command_lines").toArray();
                    application.arguments = args[0].toString().toStdString();

                    applications.push_back(application);
                }
            }
        }

        applications = RemoveMissingApplications(applications);
        applications = UpdateDefaultApplications(applications);
        if (applications.empty()) applications = CreateDefaultApplications();
    }

    return true;
}

bool Environment::Save() const {
    QSettings settings;

    // Save 'first_run'
    settings.setValue(VKCONFIG_KEY_INITIALIZE_FILES, first_run);

    // Save 'version'
    settings.setValue(VKCONFIG_KEY_VKCONFIG_VERSION, Version::LAYER_CONFIG.str().c_str());

    // Save 'override_mode'
    settings.setValue(VKCONFIG_KEY_OVERRIDE_MODE, override_state);

    // Save 'loader_message'
    settings.setValue(VKCONFIG_KEY_LOADER_MESSAGE, static_cast<int>(loader_message_level));

    // Save active state
    for (std::size_t i = 0; i < ACTIVE_COUNT; ++i) {
        settings.setValue(GetActiveToken(static_cast<Active>(i)), actives[i].c_str());
    }

    // Save layout state
    for (std::size_t i = 0; i < LAYOUT_COUNT; ++i) {
        settings.setValue(GetLayoutStateToken(static_cast<LayoutState>(i)), layout_states[i]);
    }

    // Save default configuration initizalized
    settings.setValue("default_configuration_files", ConvertString(this->default_configuration_filenames));

    const bool result = SaveApplications();
    assert(result);

    return true;
}

bool Environment::SaveApplications() const {
    QJsonObject root;

    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        // Build an array of appnames with associated data
        QJsonObject application_object;
        application_object.insert("app_name", applications[i].app_name.c_str());
        application_object.insert("app_path", applications[i].executable_path.c_str());
        application_object.insert("app_folder", applications[i].working_folder.c_str());
        application_object.insert("exclude_override", !applications[i].override_layers);
        application_object.insert("log_file", applications[i].log_file.c_str());

        // Ground work for mulitiple sets of command line arguments
        QJsonArray argsArray;
        argsArray.append(QJsonValue(applications[i].arguments.c_str()));

        application_object.insert("command_lines", argsArray);
        root.insert(QFileInfo(applications[i].app_name.c_str()).fileName(), application_object);
    }

    const std::string& app_list_json = GetPath(BUILTIN_PATH_APPLIST);
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

    Set(ACTIVE_APPLICATION, applications[application_index].app_name.c_str());
}

int Environment::GetActiveApplicationIndex() const {
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        if (applications[i].app_name.c_str() == Get(ACTIVE_APPLICATION)) {
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
        if (applications[i].app_name.c_str() == Get(ACTIVE_APPLICATION)) {
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

std::string Environment::GetDefaultExecutablePath(const std::string& executable_name) const {
    static const char* DEFAULT_PATH = VKC_PLATFORM == VKC_PLATFORM_MACOS ? "/../.." : "";

    // Using VULKAN_SDK environement variable
    const std::string env(paths_manager.GetPath(PATH_VULKAN_SDK));
    if (!env.empty()) {
        static const char* TABLE[] = {
            "/Bin",  // ENVIRONMENT_WIN32
            "/bin",  // ENVIRONMENT_UNIX
        };
        static_assert(countof(TABLE) == ENVIRONMENT_COUNT, "The tranlation table size doesn't match the enum number of elements");

        const std::string search_path = env + TABLE[VKC_ENV] + DEFAULT_PATH + executable_name.c_str();
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

    return "./" + executable_name;
}

static const DefaultApplication defaults_applications[] = {{"vkcube", ConvertNativeSeparators("/vkcube"), "--suppress_popups"},
                                                           {"vkcubepp", ConvertNativeSeparators("/vkcubepp"), "--suppress_popups"}};

Application Environment::CreateDefaultApplication(const DefaultApplication& default_application) const {
    const std::string executable_path = GetDefaultExecutablePath((default_application.key + GetApplicationSuffix()).c_str());
    if (executable_path.empty()) Application();  // application could not be found..

    Application application(default_application.name, executable_path, default_application.arguments);

    // On all operating systems, but Windows we keep running into problems with this ending up
    // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
    // initially will be set to the users home folder across all OS's. This is highly visible
    // in the application launcher and should not present a usability issue. The developer can
    // easily change this later to anywhere they like.
    application.log_file = std::string("${VK_LOCAL}") + default_application.key + ".txt";

    return application;
}

std::vector<Application> Environment::CreateDefaultApplications() const {
    std::vector<Application> new_applications;

    for (std::size_t name_index = 0, name_count = countof(defaults_applications); name_index < name_count; ++name_index) {
        const Application& application = CreateDefaultApplication(defaults_applications[name_index]);

        if (application.executable_path.empty()) continue;

        new_applications.push_back(application);
    }

    return new_applications;
}

std::vector<Application> Environment::RemoveMissingApplications(const std::vector<Application>& applications) const {
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

std::vector<Application> Environment::UpdateDefaultApplications(const std::vector<Application>& applications) const {
    std::vector<Application> search_applications;
    std::vector<Application> updated_applications = applications;

    for (std::size_t default_index = 0, default_count = countof(defaults_applications); default_index < default_count;
         ++default_index) {
        const std::string defaults_name = defaults_applications[default_index].key + GetApplicationSuffix();

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
