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

static const DefaultExecutable defaults_executables[] = {
    {"vkcube", "/vkcube", "--suppress_popups", "vkcube launcher options"},
    {"vkcubepp", "/vkcubepp", "--suppress_popups", "vkcubepp launcher options"},
#if VKC_ENV == VKC_ENV_WIN32
    {"vulkaninfoSDK", "/vulkaninfoSDK", "--json", "vulkaninfo launcher options"},
#else
    {"vulkaninfo", "/vulkaninfo", "--json", "vulkaninfo launcher options"},
#endif
};

std::string ExecutableManager::Log() const {
    std::string log;
    return log;
}

bool ExecutableManager::Empty() const { return this->data.empty(); }

std::size_t ExecutableManager::Size() const { return this->data.size(); }

bool ExecutableManager::Load(const QJsonObject& json_root_object) {
    // applications json object
    const QJsonObject& json_executables_object = json_root_object.value("executables").toObject();

    if (json_executables_object.value("active_executable") != QJsonValue::Undefined) {
        this->active_executable = Path(json_executables_object.value("active_executable").toString().toStdString());
    }

    if (json_executables_object.value("last_path_executable") != QJsonValue::Undefined) {
        this->last_path_executable = json_executables_object.value("last_path_executable").toString().toStdString();
    }

    if (json_executables_object.value("clear_on_launch") != QJsonValue::Undefined) {
        this->launcher_clear_on_launch = json_executables_object.value("clear_on_launch").toBool();
    }

    const QJsonObject& json_list_object = json_executables_object.value("list").toObject();

    const QStringList& json_list_keys = json_list_object.keys();

    this->data.clear();
    for (int i = 0, n = json_list_keys.length(); i < n; ++i) {
        Executable executable;

        const QJsonObject& json_application_object = json_list_object.value(json_list_keys[i]).toObject();
        executable.path = json_list_keys[i].toStdString();
        executable.enabled = json_application_object.value("enabled").toBool();
        if (json_application_object.value("configuration") != QJsonValue::Undefined) {
            executable.configuration = json_application_object.value("configuration").toString().toStdString();
        }

        const QJsonArray& json_options_array = json_application_object.value("options").toArray();
        for (int j = 0, o = json_options_array.size(); j < o; ++j) {
            const QJsonObject& json_options_object = json_options_array[j].toObject();

            ExecutableOptions executable_options;

            executable_options.label = json_options_object.value("label").toString().toStdString();
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

            executable.AddOptions(executable_options);
        }
        // Must be call after adding all options because `SetActiveOptions` check whether the label in `active_option` exist in the
        // options list
        executable.SetActiveOptions(json_application_object.value("active_option").toString().toStdString());

        this->data.push_back(executable);
    }

    if (this->data.empty()) {
        this->Reset();
    }

    return true;
}

void ExecutableManager::Reset() {
    this->data = this->CreateDefaultExecutables();
    this->active_executable = this->data[0].path;
}

bool ExecutableManager::Save(QJsonObject& json_root_object) const {
    QJsonObject json_executables_object;
    json_executables_object.insert("active_executable", this->active_executable.RelativePath().c_str());
    json_executables_object.insert("last_path_executable", this->last_path_executable.RelativePath().c_str());
    json_executables_object.insert("clear_on_launch", this->launcher_clear_on_launch);

    QJsonObject json_executables_list_object;
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        const Executable& executable = this->data[i];

        QJsonObject json_executable_object;
        json_executable_object.insert("enabled", executable.enabled);
        json_executable_object.insert("configuration", executable.configuration.c_str());
        json_executable_object.insert("active_option", executable.GetActiveOptionsName().c_str());

        QJsonArray json_options_array;
        const std::vector<ExecutableOptions>& options_list = executable.GetOptions();

        for (std::size_t j = 0, o = options_list.size(); j < o; ++j) {
            const ExecutableOptions& options = options_list[j];

            QJsonArray json_arg_array;
            for (std::size_t k = 0, p = options.args.size(); k < p; ++k) {
                json_arg_array.append(options.args[k].c_str());
            }

            QJsonArray json_env_array;
            for (std::size_t k = 0, p = options.envs.size(); k < p; ++k) {
                json_env_array.append(TrimSurroundingWhitespace(options.envs[k]).c_str());
            }

            QJsonObject json_option_object;
            json_option_object.insert("label", options.label.c_str());
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

    this->data.push_back(Executable(executable_path));
    return true;
}

bool ExecutableManager::RemoveExecutable() {
    std::size_t executable_index = static_cast<std::size_t>(this->GetActiveExecutableIndex());

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

bool ExecutableManager::RenameActiveExecutable(const Path& executable_path) {
    if (this->GetActiveExecutable() == nullptr) {
        return false;
    }

    this->GetActiveExecutable()->path = executable_path;
    this->last_path_executable = executable_path;
    this->active_executable = executable_path;

    return true;
}

bool ExecutableManager::UpdateConfigurations(std::vector<Path>& updated_executable_paths) {
    int result = 0;

    const Executable* active_executable = this->GetActiveExecutable();
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (!this->data[i].enabled) {
            continue;
        }

        if (this->data[i].GetActiveOptions()->working_folder.AbsolutePath() !=
            active_executable->GetActiveOptions()->working_folder.AbsolutePath()) {
            continue;
        }

        ++result;
        this->data[i].configuration = active_executable->configuration;
        updated_executable_paths.push_back(this->data[i].path);
    }

    return result > 1;
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

std::vector<Executable> ExecutableManager::CreateDefaultExecutables() const {
    std::vector<Executable> new_executables;

    for (std::size_t name_index = 0, name_count = std::size(defaults_executables); name_index < name_count; ++name_index) {
        const Executable executable(defaults_executables[name_index]);

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

bool ExecutableManager::HasIncompatiblePerExecutables(const Executable& executable,
                                                      std::vector<Executable>& imcompatible_executables) const {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        if (executable.path == this->data[i].path) {
            continue;
        }

        if (executable.GetActiveOptions()->working_folder == this->data[i].GetActiveOptions()->working_folder) {
            imcompatible_executables.push_back(this->data[i]);
        }
    }

    return !imcompatible_executables.empty();
}

bool ExecutableManager::HasIncompatiblePerExecutables(std::vector<Executable>& imcompatible_executables) const {
    for (std::size_t i = 0, n = this->data.size(); i < n; ++i) {
        bool incompatible = this->HasIncompatiblePerExecutables(this->data[i], imcompatible_executables);
        if (incompatible) {
            imcompatible_executables.push_back(this->data[i]);
        }
    }

    return !imcompatible_executables.empty();
}
