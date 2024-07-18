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

#include "environment.h"
#include "setting.h"
#include "util.h"

#include "type_platform.h"
#include "type_layers_mode.h"
#include "type_log.h"

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

static const char* GetApplicationSuffix() {
    static const char* TABLE[] = {
        ".exe",  // PLATFORM_WINDOWS
        "",      // PLATFORM_LINUX
        ".app",  // PLATFORM_MACOS
        "N/A"    // PLATFORM_ANDROID
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
}

static const char* GetLayoutStateToken(LayoutState state) {
    assert(state >= LAYOUT_FIRST && state <= LAYOUT_LAST);

    static const char* table[] = {
        "geometry",                    // LAYOUT_GEOMETRY
        "windowState",                 // LAYOUT_WINDOW_STATE
        "splitter1State",              // LAYOUT_MAIN_SPLITTER1
        "splitter2State",              // LAYOUT_MAIN_SPLITTER2
        "splitter3State",              // LAYOUT_MAIN_SPLITTER3
        "vkconfig2_6_geometry",        // VKCONFIG3_LAYOUT_GEOMETRY
        "vkconfig2_6_windowState",     // VKCONFIG3_LAYOUT_WINDOW_STATE
        "vkconfig2_6_splitter1State",  // VKCONFIG3_LAYOUT_MAIN_SPLITTER1
        "vkconfig2_6_splitter2State",  // VKCONFIG3_LAYOUT_MAIN_SPLITTER2
        "vkconfig2_6_splitter3State",  // VKCONFIG3_LAYOUT_MAIN_SPLITTER3
        "layerGeometry",               // LAYOUT_GEOMETRY_SPLITTER
        "splitterLayerState",          // LAYOUT_LAYER_SPLITTER
        "launcherCollapsed",           // LAYOUT_LAUNCHER_COLLAPSED
        "launcherOnClear"              // LAYOUT_LAUNCHER_CLEAR_ON
    };
    static_assert(std::size(table) == LAYOUT_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[state];
}

Environment::Environment(Mode mode)
    : mode(mode), loader_message_types_flags(::GetLogFlags(qgetenv("VK_LOADER_DEBUG").toStdString())) {
    if (mode == MODE_AUTO_LOAD_SAVE) {
        const bool result = Load();
        assert(result);
    }

    if (this->applications.empty()) {
        this->applications = CreateDefaultApplications();
    }
}

Environment::~Environment() {
    if (mode == MODE_AUTO_LOAD_SAVE) {
        const bool result = Save();
        assert(result);
    }
}

void Environment::Reset(ResetMode mode) {
    switch (mode) {
        case DEFAULT: {
            this->global_configuration = ConfigurationInfo();
            this->use_per_application_configuration = false;
            this->use_system_tray = false;
            this->active_executable_index = 0;

            this->applications = CreateDefaultApplications();
            break;
        }
        case CLEAR: {
            this->Reset(DEFAULT);

            break;
        }
        case SYSTEM: {
            const bool result_env = Load();
            assert(result_env);
            break;
        }
        default: {
            assert(0);
            break;
        }
    }
}

std::string GetPath() {
    static const char* TABLE[] = {
        "/AppData/Local/LunarG",  // ENVIRONMENT_WIN32
        "/.local/share/vulkan"    // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT, "The tranlation table size doesn't match the enum number of elements");

    const std::string home = QDir().homePath().toStdString();
    return home + TABLE[VKC_ENV];
}

bool Environment::Load() {
    Reset(DEFAULT);

    const Path& vkconfig_init_path = ::Get(Path::INIT);
    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString data = file.readAll();
        file.close();

        const QJsonDocument& json_doc = QJsonDocument::fromJson(data.toLocal8Bit());
        const QJsonObject& json_root_object = json_doc.object();

        const Version file_format_version = Version(json_root_object.value("file_format_version").toString().toStdString());
        if (file_format_version > Version::VKCONFIG) {
            return false;  // Vulkan Configurator needs to be updated
        }

        // interface json object
        const QJsonObject& json_interface_object = json_root_object.value("interface").toObject();
        this->active_tab = GetTabType(json_interface_object.value("active_tab").toString().toStdString().c_str());
        this->has_crashed = json_interface_object.value("has_crashed").toBool();

        // diagnostic json object
        const QJsonObject& json_diagnostic_object = json_root_object.value("diagnostic").toObject();
        const QJsonArray& json_loader_messages_array = json_diagnostic_object.value("loader_messages").toArray();
        this->loader_message_types_flags = 0;
        for (int i = 0, n = json_loader_messages_array.size(); i < n; ++i) {
            const std::string& token = json_loader_messages_array[i].toString().toStdString();
            this->loader_message_types_flags |= GetLogBit(token.c_str());
        }

        // applications json object
        const QJsonObject& json_applications_object = json_root_object.value("applications").toObject();

        this->active_executable_index = json_applications_object.value("active_executable_index").toInt();

        const QJsonObject& json_list_object = json_applications_object.value("list").toObject();

        const QStringList& json_list_keys = json_list_object.keys();

        this->applications.clear();
        for (int i = 0, n = json_list_keys.length(); i < n; ++i) {
            Application application;

            const QJsonObject& json_application_object = json_list_object.value(json_list_keys[i]).toObject();
            application.executable_path = json_list_keys[i].toStdString();
            application.configuration.SetName(json_application_object.value("configuration").toString().toStdString());
            application.configuration.SetMode(
                ::GetLayersMode(json_application_object.value("mode").toString().toStdString().c_str()));
            application.active_option_index = json_application_object.value("active_option_index").toInt();

            const QJsonArray& json_options_array = json_application_object.value("options").toArray();
            for (int j = 0, o = json_options_array.size(); j < o; ++j) {
                const QJsonObject& json_options_object = json_options_array[j].toObject();

                ApplicationOptions application_options;

                application_options.label = json_options_object.value("label").toString().toStdString();
                application_options.working_folder = json_options_object.value("working_folder").toString().toStdString();

                const QJsonArray& json_command_lines_array = json_options_object.value("arguments").toArray();
                for (int k = 0, p = json_command_lines_array.size(); k < p; ++k) {
                    application_options.arguments.push_back(json_command_lines_array[k].toString().toStdString());
                }

                const QJsonArray& json_environment_variables_array = json_options_object.value("environment_variables").toArray();
                for (int k = 0, p = json_environment_variables_array.size(); k < p; ++k) {
                    application_options.environment_variables.push_back(
                        json_environment_variables_array[k].toString().toStdString());
                }

                application_options.log_file = json_options_array[j].toObject().value("log_file").toString().toStdString();

                application.options.push_back(application_options);
            }

            this->applications.push_back(application);
        }

        // configurations json object
        const QJsonObject& json_configurations_object = json_root_object.value("configurations").toObject();
        this->use_per_application_configuration = json_configurations_object.value("use_per_application").toBool();
        this->global_configuration.SetName(json_configurations_object.value("configuration").toString().toStdString());
        this->global_configuration.SetMode(
            ::GetLayersMode(json_configurations_object.value("mode").toString().toStdString().c_str()));

        // preferences json object
        const QJsonObject& json_preferences_object = json_root_object.value("preferences").toObject();
        this->use_system_tray = json_preferences_object.value("use_system_tray").toBool();
        this->home_sdk_path = json_preferences_object.value("VK_HOME").toString().toStdString();
        if (this->home_sdk_path.Empty()) {
            this->home_sdk_path = ::Get(Path::HOME);
        }

        const QJsonArray& json_hide_message_boxes_array = json_preferences_object.value("hide_message_boxes").toArray();
        this->hide_message_boxes_flags = 0;
        for (int i = 0, n = json_hide_message_boxes_array.size(); i < n; ++i) {
            const std::string& token = json_hide_message_boxes_array[i].toString().toStdString();
            this->hide_message_boxes_flags |= GetLogBit(token.c_str());
        }
    }

    return true;
}

bool Environment::Save() const {
    const Path& vkconfig_init_path = ::Get(Path::INIT);

    QFile file(vkconfig_init_path.AbsolutePath().c_str());
    const bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
    assert(result);

    QJsonObject json_root_object;
    json_root_object.insert("file_format_version", Version::VKCONFIG.str().c_str());

    QJsonObject json_interface_object;
    json_interface_object.insert("active_tab", GetToken(this->active_tab));
    json_interface_object.insert("has_crashed", this->has_crashed);
    json_root_object.insert("interface", json_interface_object);

    QJsonObject json_diagnostic_object;

    QJsonArray json_loader_messages_array;
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        LogType type = static_cast<LogType>(i);
        if (this->loader_message_types_flags & (1 << i)) {
            json_loader_messages_array.append(GetToken(type));
        }
    }
    json_diagnostic_object.insert("loader_messages", json_loader_messages_array);

    json_root_object.insert("diagnostic", json_diagnostic_object);

    QJsonObject json_applications_object;
    json_applications_object.insert("active_executable", this->active_executable_index);
    QJsonObject json_applications_list_object;

    for (std::size_t i = 0, n = this->applications.size(); i < n; ++i) {
        const Application& application = this->applications[i];

        QJsonObject json_application_object;
        json_application_object.insert("configuration", application.configuration.GetName());
        json_application_object.insert("mode", ::GetToken(application.configuration.GetMode()));
        json_application_object.insert("active_option", application.active_option_index);

        QJsonArray json_options_array;
        for (std::size_t j = 0, o = application.options.size(); j < o; ++j) {
            const ApplicationOptions& options = application.options[j];

            QJsonArray json_arg_array;
            for (std::size_t k = 0, p = options.arguments.size(); k < p; ++k) {
                json_arg_array.append(options.arguments[k].c_str());
            }

            QJsonArray json_env_array;
            for (std::size_t k = 0, p = options.environment_variables.size(); k < p; ++k) {
                json_env_array.append(options.environment_variables[k].c_str());
            }

            QJsonObject json_option_object;
            json_option_object.insert("label", options.label.c_str());
            json_option_object.insert("working_folder", options.working_folder.RelativePath().c_str());
            json_option_object.insert("arguments", json_arg_array);
            json_option_object.insert("environment_variables", json_env_array);
            json_option_object.insert("log_file", options.log_file.RelativePath().c_str());

            json_options_array.append(json_option_object);
        }
        json_application_object.insert("options", json_options_array);

        json_applications_list_object.insert(application.executable_path.RelativePath().c_str(), json_application_object);
    }

    json_applications_object.insert("list", json_applications_list_object);

    json_root_object.insert("applications", json_applications_object);

    QJsonObject json_layers_object;

    json_root_object.insert("layers", json_layers_object);

    QJsonObject json_configurations_object;
    json_configurations_object.insert("use_per_application", this->use_per_application_configuration);
    json_configurations_object.insert("configuration", this->global_configuration.GetName());
    json_configurations_object.insert("mode", ::GetToken(this->global_configuration.GetMode()));

    json_root_object.insert("configurations", json_configurations_object);

    QJsonObject json_preferences_object;
    json_preferences_object.insert("use_system_tray", this->use_system_tray);
    json_preferences_object.insert("VK_HOME", this->home_sdk_path.RelativePath().c_str());

    QJsonArray json_hide_message_boxes_array;
    for (int i = LOG_FIRST, n = LOG_COUNT; i < n; ++i) {
        LogType type = static_cast<LogType>(i);
        if (this->hide_message_boxes_flags & (1 << i)) {
            json_hide_message_boxes_array.append(GetToken(type));
        }
    }
    json_preferences_object.insert("hide_message_boxes", json_hide_message_boxes_array);

    json_root_object.insert("preferences", json_preferences_object);

    QJsonDocument json_doc(json_root_object);
    file.write(json_doc.toJson());
    file.close();

    return true;
}

void Environment::SelectActiveApplication(std::size_t application_index) {
    assert(application_index < this->applications.size());
    this->active_executable_index = application_index;
}

int Environment::GetActiveApplicationIndex() const {
    return this->active_executable_index;  // Not found, but the list is present, so return the first item.
}

bool Environment::HasOverriddenApplications() const {
    for (std::size_t i = 0, n = this->applications.size(); i < n; ++i) {
        if (this->applications[i].configuration.GetMode() != LAYERS_CONTROLLED_BY_APPLICATIONS) {
            return true;
        }
    }

    return false;
}

bool Environment::AppendApplication(const Application& application) {
    this->applications.push_back(application);
    return true;
}

bool Environment::RemoveApplication(std::size_t application_index) {
    assert(!this->applications.empty());
    assert(application_index < this->applications.size());

    if (this->applications.size() == 1u) {
        this->applications.clear();
        return true;
    }

    std::vector<Application> new_applications;
    new_applications.reserve(this->applications.size() - 1);

    for (std::size_t i = 0, n = this->applications.size(); i < n; ++i) {
        if (i == application_index) {
            continue;
        }
        new_applications.push_back(this->applications[i]);
    }

    std::swap(this->applications, new_applications);
    return true;
}

const ConfigurationInfo& Environment::GetActiveConfigurationInfo() const {
    if (this->GetPerApplicationConfig()) {
        return this->GetActiveApplication().configuration;
    } else {
        return this->global_configuration;
    }
}

ConfigurationInfo& Environment::GetActiveConfigurationInfo() {
    if (this->GetPerApplicationConfig()) {
        return this->GetActiveApplication().configuration;
    } else {
        return this->global_configuration;
    }
}

const Application& Environment::GetActiveApplication() const {
    assert(!this->applications.empty());

    return this->applications[this->active_executable_index];  // Not found, but the list is present, so return the first item.
}

Application& Environment::GetActiveApplication() {
    assert(!this->applications.empty());

    return this->applications[this->active_executable_index];  // Not found, but the list is present, so return the first item.
}

const Application& Environment::GetApplication(std::size_t application_index) const {
    assert(application_index < this->applications.size());

    return this->applications[application_index];
}

Application& Environment::GetApplication(std::size_t application_index) {
    assert(application_index < this->applications.size());

    return this->applications[application_index];
}

bool Environment::GetPerApplicationConfig() const { return this->use_per_application_configuration; }

void Environment::SetPerApplicationConfig(bool enable) { this->use_per_application_configuration = enable; }

void Environment::Set(LayoutState state, const QByteArray& data) {
    assert(state >= LAYOUT_FIRST && state <= LAYOUT_LAST);
    this->layout_states[state] = data;
}

const QByteArray& Environment::Get(LayoutState state) const {
    assert(state >= LAYOUT_FIRST && state <= LAYOUT_LAST);
    return this->layout_states[state];
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
bool ExactExecutableFromAppBundle(Path& app_path) {
    std::string path = app_path.AbsolutePath();
    path += "/Contents/";
    std::string list_file = path + "Info.plist";
    QFile file(list_file.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);

    // Read a line at a time looking for the executable tag
    QString line_buffer;
    while (!stream.atEnd()) {
        line_buffer = stream.readLine();
        if (line_buffer.contains("<key>CFBundleExecutable</key>")) {  // Exe follows this
            line_buffer = stream.readLine();                          // <string>Qt Creator</string>
            char* cExeName = new char[line_buffer.length()];          // Prevent buffer overrun

            QByteArray line_array = line_buffer.toUtf8();
            const char* pStart = strstr(line_array.constData(), "<string>");
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
            app_path = Path(path);

            delete[] cExeName;
            break;
        }
    }

    file.close();

    return true;
}

DefaultPath Environment::GetDefaultExecutablePath(const std::string& executable_name) const {
    static const char* DEFAULT_PATH = VKC_PLATFORM == VKC_PLATFORM_MACOS ? "/../.." : "";

    DefaultPath default_path{"." + executable_name, "."};

    // Using VULKAN_SDK environement variable
    const Path env = ::Get(Path::SDK_BIN);
    if (!env.Empty()) {
        const Path search_path(env + DEFAULT_PATH + executable_name.c_str());
        if (search_path.Exists()) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    }

    // Search the default applications from package installation (Linux)
    if (VKC_PLATFORM == VKC_PLATFORM_LINUX) {
        const Path search_path(std::string("/usr/bin") + DEFAULT_PATH + executable_name);
        if (search_path.Exists()) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    } else if (VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        Path search_path(std::string("/Applications") + executable_name);
        if (search_path.Exists() && ExactExecutableFromAppBundle(search_path)) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    }

    // Using relative path to vkconfig in case SDK is not "installed"
    if (VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        Path search_path(std::string("..") + DEFAULT_PATH + executable_name);
        if (search_path.Exists() && ExactExecutableFromAppBundle(search_path)) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    } else {
        Path search_path(std::string(".") + DEFAULT_PATH + executable_name);
        if (search_path.Exists()) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    }

    return default_path;
}

static const DefaultApplication defaults_applications[] = {{"vkcube", "/vkcube", "--suppress_popups"},
                                                           {"vkcubepp", "/vkcubepp", "--suppress_popups"}};

Application Environment::CreateDefaultApplication(const DefaultApplication& default_application) const {
    const DefaultPath& default_paths = GetDefaultExecutablePath((default_application.key + GetApplicationSuffix()).c_str());
    if (default_paths.executable_path.Empty()) {
        Application();  // application could not be found..
    }

    ApplicationOptions options;
    options.label = "Default";
    options.working_folder = default_paths.working_folder;
    // On all operating systems, but Windows we keep running into problems with this ending up
    // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
    // initially will be set to the users home folder across all OS's. This is highly visible
    // in the application launcher and should not present a usability issue. The developer can
    // easily change this later to anywhere they like.
    options.log_file = std::string("${VK_LOCAL}") + default_application.key + ".txt";

    Application application;
    application.executable_path = Path(default_paths.executable_path.AbsolutePath(), true);
    application.options.push_back(options);
    application.active_option_index = 0;

    return application;
}

std::vector<Application> Environment::CreateDefaultApplications() const {
    std::vector<Application> new_applications;

    for (std::size_t name_index = 0, name_count = std::size(defaults_applications); name_index < name_count; ++name_index) {
        const Application& application = CreateDefaultApplication(defaults_applications[name_index]);

        if (application.executable_path.Empty()) {
            continue;
        }

        new_applications.push_back(application);
    }

    return new_applications;
}

std::vector<Application> Environment::RemoveMissingApplications(const std::vector<Application>& applications) const {
    std::vector<Application> valid_applications;

    // Remove applications that can't be found
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        const Application& application = applications[i];

        const QFileInfo file_info(application.executable_path.AbsolutePath().c_str());
        if (!file_info.exists()) {
            continue;
        }

        valid_applications.push_back(application);
    }

    return valid_applications;
}

std::vector<Application> Environment::UpdateDefaultApplications(const std::vector<Application>& applications) const {
    std::vector<Application> search_applications;
    std::vector<Application> updated_applications = applications;

    for (std::size_t default_index = 0, default_count = std::size(defaults_applications); default_index < default_count;
         ++default_index) {
        const std::string defaults_name = defaults_applications[default_index].key + GetApplicationSuffix();

        std::swap(updated_applications, search_applications);
        updated_applications.clear();

        for (std::size_t application_index = 0, application_count = search_applications.size();
             application_index < application_count; ++application_index) {
            const Application& application = search_applications[application_index];

            if (QString(application.executable_path.AbsolutePath().c_str()).endsWith(defaults_name.c_str())) {
                updated_applications.push_back(CreateDefaultApplication(defaults_applications[default_index]));
            } else {
                updated_applications.push_back(application);
            }
        }
    }

    return updated_applications;
}
