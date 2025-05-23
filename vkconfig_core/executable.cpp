/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "executable.h"
#include "util.h"
#include "type_platform.h"

#include <QTextStream>

///////////////////////////////////////////////////////////////////////////
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
/// Note, not ALL macOS executables are in a bundle, so if a non-bundled
/// executable is fed in here, it will silently just return without
/// modifying the path (which will be the correct behavior)
static bool ExactExecutableFromAppBundle(Path& app_path) {
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

DefaultPath GetDefaultExecutablePath(const std::string& executable_name) {
    const bool is_app = ::EndsWith(executable_name, ".app");

    DefaultPath default_path{executable_name, ""};

    // Using VULKAN_SDK environement variable
    const Path vulkan_sdk_bin_path(Path::BIN);
    if (!vulkan_sdk_bin_path.Empty() && !is_app) {
        const Path search_path(vulkan_sdk_bin_path.RelativePath() + "/" + executable_name);
        if (search_path.Exists()) {
            default_path.executable_path = search_path.AbsolutePath();
            default_path.working_folder = search_path.AbsoluteDir();
            return default_path;
        }
    }

    // Search the default applications from package installation (Linux)
    if (is_app) {
        Path search_path(std::string("/Applications/") + executable_name);
        if (search_path.Exists() && ExactExecutableFromAppBundle(search_path)) {
            default_path.executable_path = search_path.AbsolutePath();
            default_path.working_folder = search_path.AbsoluteDir();
            return default_path;
        }
    } else {
        const Path search_path(std::string("/usr/bin/") + executable_name);
        if (search_path.Exists()) {
            default_path.executable_path = search_path.AbsolutePath();
            default_path.working_folder = search_path.AbsoluteDir();
            return default_path;
        }
    }

    // Using relative path to vkconfig in case SDK is not "installed"
    if (VKC_PLATFORM == PLATFORM_MACOS) {
        if (is_app) {
            Path search_path(std::string("../../") + executable_name);
            if (search_path.Exists() && ExactExecutableFromAppBundle(search_path)) {
                default_path.executable_path = search_path.AbsolutePath();
                default_path.working_folder = search_path.AbsoluteDir();
                return default_path;
            }
        } else {
            Path search_path(std::string("../../../../macOS/bin/") + executable_name);
            if (search_path.Exists()) {
                default_path.executable_path = search_path.AbsolutePath();
                default_path.working_folder = search_path.AbsoluteDir();
                return default_path;
            }
        }
    } else {
        Path search_path(std::string("./") + executable_name);
        if (search_path.Exists()) {
            default_path.executable_path = search_path.AbsolutePath();
            default_path.working_folder = search_path.AbsoluteDir();
            return default_path;
        }
    }

    return default_path;
}

Executable::Executable() {}

Executable::Executable(const DefaultExecutable& default_executable) {
    const DefaultPath& default_paths = ::GetDefaultExecutablePath(default_executable.key);
    if (default_paths.executable_path.Empty()) {
        Executable();  // application could not be found..
    }

    this->path = default_paths.executable_path;

    if (default_executable.options.empty()) {
        ExecutableOptions options;
        options.label = "Default Options";
        options.working_folder = default_paths.working_folder;
        options.log_file = std::string("${VULKAN_HOME}/") + default_executable.name + ".txt";
        this->options_list.push_back(options);
    } else {
        for (std::size_t i = 0, n = default_executable.options.size(); i < n; ++i) {
            ExecutableOptions options;
            options.label = default_executable.options[i].label;
            if (default_executable.options[i].working_folder.Empty()) {
                options.working_folder = default_paths.working_folder;
            } else {
                options.working_folder = default_executable.options[i].working_folder;
            }
            options.args = SplitSpace(default_executable.options[i].args);
            options.envs = SplitSpace(default_executable.options[i].envs);

            // On all operating systems, but Windows we keep running into problems with this ending up
            // somewhere the user isn't allowed to create and write files. For consistncy sake, the log
            // initially will be set to the users home folder across all OS's. This is highly visible
            // in the application launcher and should not present a usability issue. The developer can
            // easily change this later to anywhere they like.
            options.log_file = std::string("${VULKAN_HOME}/") + default_executable.name + ".txt";

            this->options_list.push_back(options);
        }
    }

    this->active_options = this->options_list[0].label;
}

Executable::Executable(const Path& executable_path) {
    ExecutableOptions options;
    options.working_folder = executable_path.AbsoluteDir();
    options.log_file = std::string("${VULKAN_HOME}/") + executable_path.Basename() + ".txt";

    this->path = executable_path;
    this->options_list.push_back(options);
    this->active_options = options.label;
}

void Executable::RenameActiveOptions(const std::string& label) {
    ExecutableOptions* options = this->GetActiveOptions();
    this->active_options = options->label = label;

    this->SortOptions();
}

std::string Executable::GetActiveOptionsName() const { return this->active_options; }

bool Executable::SetActiveOptions(const std::string& label) {
    for (std::size_t i = 0, n = this->options_list.size(); i < n; ++i) {
        if (this->options_list[i].label == label) {
            this->active_options = label;
            return true;
        }
    }

    this->active_options.clear();
    return false;
}

ExecutableOptions* Executable::GetActiveOptions() {
    const int option_index = this->GetActiveOptionsIndex();

    if (option_index != INVALID_OPTIONS) {
        std::size_t index = static_cast<std::size_t>(option_index);

        assert(index < this->options_list.size());
        return &this->options_list[index];
    } else {
        return nullptr;
    }
}

const ExecutableOptions* Executable::GetActiveOptions() const { return const_cast<Executable*>(this)->GetActiveOptions(); }

bool Executable::HasActiveOptions() const {
    if (this->options_list.empty()) {
        return false;
    }

    return this->GetActiveOptions() != nullptr;
}

Path Executable::GetLocalLayersSettingsPath() const {
    assert(this->GetActiveOptions() != nullptr);
    if (this->GetActiveOptions()->working_folder.Empty()) {
        return this->path.AbsoluteDir() + "/vk_layer_settings.txt";
    } else {
        return this->GetActiveOptions()->working_folder.RelativePath() + "/vk_layer_settings.txt";
    }
}

void Executable::AddOptions(const ExecutableOptions& options) {
    this->options_list.push_back(options);
    this->SortOptions();
}

const std::vector<ExecutableOptions>& Executable::GetOptions() const { return this->options_list; }

int Executable::GetActiveOptionsIndex() const {
    if (this->options_list.empty()) {
        return INVALID_OPTIONS;
    }

    if (this->active_options.empty()) {
        return 0;
    }

    for (std::size_t i = 0, n = this->options_list.size(); i < n; ++i) {
        if (this->options_list[i].label == this->active_options) {
            return static_cast<int>(i);
        }
    }

    return 0;
}

bool Executable::RemoveActiveOptions() {
    if (this->options_list.empty()) {
        return false;
    }

    std::size_t executable_index = static_cast<std::size_t>(GetActiveOptionsIndex());

    std::vector<ExecutableOptions> new_options;

    bool removed = false;
    for (std::size_t i = 0, n = this->options_list.size(); i < n; ++i) {
        if (i == executable_index) {
            removed = true;
            continue;
        }

        new_options.push_back(this->options_list[i]);
    }

    std::swap(new_options, this->options_list);

    if (this->options_list.empty()) {
        this->active_options.clear();
    } else {
        this->active_options = this->options_list[std::max<int>(executable_index - 1, 0)].label;
    }

    return removed;
}

bool Executable::DuplicateActiveOptions() {
    int executable_index = GetActiveOptionsIndex();
    if (executable_index == INVALID_OPTIONS) {
        return false;
    }

    std::size_t index = static_cast<std::size_t>(executable_index);
    assert(index < this->options_list.size());
    ExecutableOptions new_options = this->options_list[index];
    new_options.label = MakeOptionsName(new_options.label);
    this->options_list.push_back(new_options);

    this->active_options = new_options.label;

    this->SortOptions();

    return true;
}

void Executable::SortOptions() {
    struct {
        bool operator()(const ExecutableOptions& a, const ExecutableOptions& b) { return a.label < b.label; }
    } sort_op;

    std::sort(this->options_list.begin(), this->options_list.end(), sort_op);
}

static const size_t NOT_FOUND = static_cast<size_t>(-1);

std::string Executable::MakeOptionsName(const std::string& name) const {
    const std::string key = name;
    const std::string base_name = ExtractDuplicateNumber(key) != NOT_FOUND ? ExtractDuplicateBaseName(key) : key;

    std::size_t max_duplicate = 0;
    for (std::size_t i = 0, n = this->options_list.size(); i < n; ++i) {
        const std::string& search_name = this->options_list[i].label;

        if (search_name.compare(0, base_name.length(), base_name) != 0) {
            continue;
        }

        const std::size_t found_number = ExtractDuplicateNumber(search_name);
        max_duplicate = std::max<std::size_t>(max_duplicate, found_number != NOT_FOUND ? found_number : 1);
    }

    return base_name + (max_duplicate > 0 ? format(" (%d)", max_duplicate + 1).c_str() : "");
}
