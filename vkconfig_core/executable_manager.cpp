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

#include "executable_manager.h"
#include "version.h"
#include "type_platform.h"
#include "util.h"

#include <QJsonArray>
#include <QTextStream>

const char* GetExecutableFilter() {
    static const char* TABLE[] = {
        "Executables (*.exe)",     // PLATFORM_WINDOWS_X86
        "Executables (*.exe)",     // PLATFORM_WINDOWS_ARM
        "Executables (*)",         // PLATFORM_LINUX
        "Executables (*.app, *)",  // PLATFORM_MACOS
        "N/A",                     // PLATFORM_ANDROID
        "N/A"                      // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
}

static const char* GetExecutableSuffix() {
    static const char* TABLE[] = {
        ".exe",  // PLATFORM_WINDOWS_X86
        ".exe",  // PLATFORM_WINDOWS_ARM
        "",      // PLATFORM_LINUX
        ".app",  // PLATFORM_MACOS
        "N/A",   // PLATFORM_ANDROID
        "N/A"    // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
}

static const DefaultExecutable defaults_executables[] = {
    {"vkcube", "/vkcube", "--suppress_popups", "VkCube launcher options"},
    {"vkcubepp", "/vkcubepp", "--suppress_popups", "VkCubepp launcher options"}};

ExecutableOptions* Executable::GetActiveOptions() {
    assert(!this->options.empty());

    if (this->active_option.empty()) {
        return &this->options[0];
    }

    for (std::size_t i = 0, n = this->options.size(); i < n; ++i) {
        if (this->options[i].label == this->active_option) {
            return &this->options[i];
        }
    }
    return nullptr;
}

const ExecutableOptions* Executable::GetActiveOptions() const {
    assert(!this->options.empty());

    if (this->active_option.empty()) {
        return &this->options[0];
    }

    for (std::size_t i = 0, n = this->options.size(); i < n; ++i) {
        if (this->options[i].label == this->active_option) {
            return &this->options[i];
        }
    }
    return nullptr;
}

int Executable::GetActiveOptionsIndex() const {
    assert(!this->options.empty());

    if (this->active_option.empty()) {
        return 0;
    }

    for (std::size_t i = 0, n = this->options.size(); i < n; ++i) {
        if (this->options[i].label == this->active_option) {
            return static_cast<int>(i);
        }
    }

    return 0;
}

void ExecutableManager::Reset() {
    this->data = this->CreateDefaultExecutables();
    this->active_executable = this->data[0].path;
}

std::string ExecutableManager::Log() const {
    std::string log;
    return log;
}

void ExecutableManager::Clear() {
    this->data.clear();
    this->active_executable.Clear();
}

bool ExecutableManager::Empty() const { return this->data.empty(); }

std::size_t ExecutableManager::Size() const { return this->data.size(); }

static const size_t NOT_FOUND = static_cast<size_t>(-1);

std::string ExecutableManager::MakeOptionsName(const std::string& name) const {
    const std::string key = name;
    const std::string base_name = ExtractDuplicateNumber(key) != NOT_FOUND ? ExtractDuplicateBaseName(key) : key;

    const Executable* executable = GetActiveExecutable();

    std::size_t max_duplicate = 0;
    for (std::size_t i = 0, n = executable->options.size(); i < n; ++i) {
        const std::string& search_name = executable->options[i].label;

        if (search_name.compare(0, base_name.length(), base_name) != 0) {
            continue;
        }

        const std::size_t found_number = ExtractDuplicateNumber(search_name);
        max_duplicate = std::max<std::size_t>(max_duplicate, found_number != NOT_FOUND ? found_number : 1);
    }

    return base_name + (max_duplicate > 0 ? format(" (%d)", max_duplicate + 1).c_str() : "");
}

bool ExecutableManager::Load(const QJsonObject& json_root_object) {
    // applications json object
    const QJsonObject& json_executables_object = json_root_object.value("executables").toObject();

    if (json_executables_object.value("active_executable") != QJsonValue::Undefined) {
        this->active_executable = Path(json_executables_object.value("active_executable").toString().toStdString());
    }

    if (json_executables_object.value("last_path_executable") != QJsonValue::Undefined) {
        this->last_path_executable = json_executables_object.value("last_path_executable").toString().toStdString();
    }

    const QJsonObject& json_list_object = json_executables_object.value("list").toObject();

    const QStringList& json_list_keys = json_list_object.keys();

    this->data.clear();
    for (int i = 0, n = json_list_keys.length(); i < n; ++i) {
        Executable executable;

        const QJsonObject& json_application_object = json_list_object.value(json_list_keys[i]).toObject();
        executable.path = json_list_keys[i].toStdString();
        executable.enabled = json_application_object.value("enabled").toBool();
        executable.active_option = json_application_object.value("active_option").toString().toStdString();

        const QJsonArray& json_options_array = json_application_object.value("options").toArray();
        for (int j = 0, o = json_options_array.size(); j < o; ++j) {
            const QJsonObject& json_options_object = json_options_array[j].toObject();

            ExecutableOptions executable_options;

            executable_options.label = json_options_object.value("label").toString().toStdString();
            executable_options.configuration = json_options_object.value("configuration").toString().toStdString();
            executable_options.working_folder = json_options_object.value("working_folder").toString().toStdString();

            const QJsonArray& json_command_lines_array = json_options_object.value("arguments").toArray();
            for (int k = 0, p = json_command_lines_array.size(); k < p; ++k) {
                executable_options.args.push_back(json_command_lines_array[k].toString().toStdString());
            }

            const QJsonArray& json_environment_variables_array = json_options_object.value("environment_variables").toArray();
            for (int k = 0, p = json_environment_variables_array.size(); k < p; ++k) {
                executable_options.envs.push_back(json_environment_variables_array[k].toString().toStdString());
            }

            executable_options.log_file = json_options_array[j].toObject().value("log_file").toString().toStdString();

            executable.options.push_back(executable_options);
        }

        this->data.push_back(executable);
    }

    if (this->data.empty()) {
        this->Reset();
    }

    return true;
}

bool ExecutableManager::Save(QJsonObject& json_root_object) const {
    QJsonObject json_executables_object;
    json_executables_object.insert("active_executable", this->active_executable.RelativePath().c_str());
    json_executables_object.insert("last_path_executable", this->last_path_executable.RelativePath().c_str());

    QJsonObject json_executables_list_object;
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        const Executable& executable = this->data[i];

        QJsonObject json_executable_object;
        json_executable_object.insert("enabled", executable.enabled);
        json_executable_object.insert("active_option", executable.active_option.c_str());

        QJsonArray json_options_array;
        for (std::size_t j = 0, o = executable.options.size(); j < o; ++j) {
            const ExecutableOptions& options = executable.options[j];

            QJsonArray json_arg_array;
            for (std::size_t k = 0, p = options.args.size(); k < p; ++k) {
                json_arg_array.append(options.args[k].c_str());
            }

            QJsonArray json_env_array;
            for (std::size_t k = 0, p = options.envs.size(); k < p; ++k) {
                json_env_array.append(options.envs[k].c_str());
            }

            QJsonObject json_option_object;
            json_option_object.insert("label", options.label.c_str());
            json_option_object.insert("configuration", options.configuration.c_str());
            json_option_object.insert("working_folder", options.working_folder.RelativePath().c_str());
            json_option_object.insert("arguments", json_arg_array);
            json_option_object.insert("environment_variables", json_env_array);
            json_option_object.insert("log_file", options.log_file.RelativePath().c_str());

            json_options_array.append(json_option_object);
        }
        json_executable_object.insert("options", json_options_array);

        json_executables_list_object.insert(executable.path.RelativePath().c_str(), json_executable_object);
    }

    json_executables_object.insert("list", json_executables_list_object);

    json_root_object.insert("executables", json_executables_object);

    return true;
}

void ExecutableManager::SetActiveExecutable(int executable_index) {
    assert(executable_index < static_cast<int>(this->data.size()));

    if (executable_index < 0) {
        this->active_executable.Clear();
    } else {
        this->active_executable = this->data[executable_index].path;
    }
}

int ExecutableManager::GetActiveExecutableIndex() const {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (this->data[i].path == this->active_executable) {
            return static_cast<int>(i);
        }
    }

    return -1;  // Not found, but the list is present, so return the first item.
}

bool ExecutableManager::AppendExecutable(const Executable& executable) {
    this->data.push_back(executable);
    return true;
}

bool ExecutableManager::AppendExecutable(const Path& executable_path) {
    this->last_path_executable = executable_path;
    this->active_executable = executable_path;

    ExecutableOptions options;
    options.working_folder = executable_path.AbsoluteDir();

    Executable executable;
    executable.path = executable_path;
    executable.options.push_back(options);
    executable.active_option.clear();

    this->data.push_back(executable);
    return true;
}

bool ExecutableManager::RemoveExecutable(std::size_t executable_index) {
    assert(!this->data.empty());
    assert(executable_index < this->data.size());

    if (this->data.size() == 1u) {
        this->data.clear();
        return true;
    }

    std::vector<Executable> new_executables;
    new_executables.reserve(this->data.size() - 1);

    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (i == executable_index) {
            continue;
        }
        new_executables.push_back(this->data[i]);
    }

    std::swap(this->data, new_executables);

    if (this->data.empty()) {
        this->active_executable.Clear();
    } else {
        this->active_executable = this->data[0].path;
    }

    return true;
}

const Executable* ExecutableManager::GetActiveExecutable() const {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (this->data[i].path == this->active_executable) {
            return &this->data[i];
        }
    }

    return nullptr;  // Not found, but the list is present, so return the first item.
}

Executable* ExecutableManager::GetActiveExecutable() {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (this->data[i].path == this->active_executable) {
            return &this->data[i];
        }
    }

    return nullptr;  // Not found, but the list is present, so return the first item.
}

const Executable* ExecutableManager::GetExecutable(std::size_t executable_index) const {
    assert(executable_index < this->data.size());

    return &this->data[executable_index];
}

Executable* ExecutableManager::GetExecutable(std::size_t executable_index) {
    assert(executable_index < this->data.size());

    return &this->data[executable_index];
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

DefaultPath ExecutableManager::GetDefaultExecutablePath(const std::string& executable_name) const {
    static const char* DEFAULT_PATH = VKC_PLATFORM == PLATFORM_MACOS ? "/../.." : "";

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
    if (VKC_PLATFORM == PLATFORM_LINUX) {
        const Path search_path(std::string("/usr/bin") + DEFAULT_PATH + executable_name);
        if (search_path.Exists()) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    } else if (VKC_PLATFORM == PLATFORM_MACOS) {
        Path search_path(std::string("/Applications") + executable_name);
        if (search_path.Exists() && ExactExecutableFromAppBundle(search_path)) {
            default_path.executable_path = Path(search_path.AbsolutePath(), true);
            default_path.working_folder = Path(search_path.AbsoluteDir(), true);
            return default_path;
        }
    }

    // Using relative path to vkconfig in case SDK is not "installed"
    if (VKC_PLATFORM == PLATFORM_MACOS) {
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

Executable ExecutableManager::CreateDefaultExecutable(const DefaultExecutable& default_executable) const {
    const DefaultPath& default_paths = GetDefaultExecutablePath((default_executable.key + GetExecutableSuffix()).c_str());
    if (default_paths.executable_path.Empty()) {
        Executable();  // application could not be found..
    }

    ExecutableOptions options;
    options.label = default_executable.label;
    options.working_folder = default_paths.working_folder;
    options.args.push_back(default_executable.arguments);

    // On all operating systems, but Windows we keep running into problems with this ending up
    // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
    // initially will be set to the users home folder across all OS's. This is highly visible
    // in the application launcher and should not present a usability issue. The developer can
    // easily change this later to anywhere they like.
    options.log_file = std::string("${VK_HOME}") + default_executable.key + ".txt";

    Executable executable;
    executable.path = Path(default_paths.executable_path.AbsolutePath(), true);
    executable.options.push_back(options);
    executable.active_option.clear();
    return executable;
}

std::vector<Executable> ExecutableManager::CreateDefaultExecutables() const {
    std::vector<Executable> new_executables;

    for (std::size_t name_index = 0, name_count = std::size(defaults_executables); name_index < name_count; ++name_index) {
        const Executable& executable = CreateDefaultExecutable(defaults_executables[name_index]);

        if (executable.path.Empty()) {
            continue;
        }

        new_executables.push_back(executable);
    }

    return new_executables;
}

std::vector<Executable> ExecutableManager::RemoveMissingExecutables(const std::vector<Executable>& executables) const {
    std::vector<Executable> valid_applications;

    // Remove applications that can't be found
    for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
        const Executable& application = executables[i];

        const QFileInfo file_info(application.path.AbsolutePath().c_str());
        if (!file_info.exists()) {
            continue;
        }

        valid_applications.push_back(application);
    }

    return valid_applications;
}
