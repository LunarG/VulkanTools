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

#pragma once

#include "path.h"

struct DefaultExecutable {
    std::string key;
    std::string name;
    std::string arguments;
    std::string label;
};

struct DefaultPath {
    Path executable_path;
    Path working_folder;
};

struct ExecutableOptions {
    std::string label = "Default Options";
    Path working_folder;
    std::vector<std::string> args;
    std::vector<std::string> envs;
    Path log_file;
};

struct Executable {
    Executable();
    Executable(const DefaultExecutable& default_executable);
    Executable(const Path& executable_path);

    enum { INVALID_OPTIONS = -1 };

    Path path;
    std::string configuration = "Validation";
    bool enabled = true;

    int GetActiveOptionsIndex() const;
    bool RemoveActiveOptions();
    bool DuplicateActiveOptions();

    void RenameActiveOptions(const std::string& label);
    std::string GetActiveOptionsName() const;
    bool SetActiveOptions(const std::string& label);
    ExecutableOptions* GetActiveOptions();
    const ExecutableOptions* GetActiveOptions() const;
    bool HasActiveOptions() const;
    Path GetLocalLayersSettingsPath() const;

    void AddOptions(const ExecutableOptions& options);
    const std::vector<ExecutableOptions>& GetOptions() const;

   private:
    std::string active_options;
    std::vector<ExecutableOptions> options_list;

    void SortOptions();
    std::string MakeOptionsName(const std::string& name) const;
};

DefaultPath GetDefaultExecutablePath(const std::string& executable_key);
