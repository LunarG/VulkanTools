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

#include "serialization.h"
#include "executable.h"

#include <string>
#include <vector>

enum ExecutableId {
    EXECUTABLE_VKCONFIG_GUI = 0,
    EXECUTABLE_VKCUBE,
    EXECUTABLE_VKCUBEPP,
    EXECUTABLE_VKINFO,
    EXECUTABLE_VKCAPSVIEWER,

    EXECUTABLE_FIRST = EXECUTABLE_VKCONFIG_GUI,
    EXECUTABLE_LAST = EXECUTABLE_VKCAPSVIEWER,
};

enum { EXECUTABLE_COUNT = EXECUTABLE_LAST - EXECUTABLE_FIRST + 1 };

class ExecutableManager : public Serialize {
   public:
    bool Load(const QJsonObject& json_root_object, ConfiguratorMode configurator_mode) override;
    bool Save(QJsonObject& json_root_object) const override;
    std::string Log() const override;

    bool Empty() const;
    std::size_t Size() const;
    void Reset();

    void SetActiveExecutable(int executable_index);
    int GetActiveExecutableIndex() const;
    int GetVulkanInfoIndex() const;

    bool AppendExecutable(const Executable& executable);
    bool AppendExecutable(const Path& executable_path);
    bool RemoveExecutable();
    bool RenameActiveExecutable(const Path& executable_path);
    bool UpdateConfigurations(std::vector<Path>& updated_executable_paths);

    const std::vector<Executable>& GetExecutables() const { return this->data; }
    const Executable* GetActiveExecutable() const;
    Executable* GetActiveExecutable();
    const Executable* GetExecutable(std::size_t executable_index) const;
    Executable* GetExecutable(std::size_t executable_index);

    // Create a list of default executables, eg vkcube
    std::vector<Executable> CreateDefaultExecutables() const;

    // Search for all the applications in the list, an remove the application which executable can't be found
    std::vector<Executable> RemoveMissingExecutables(const std::vector<Executable>& executables) const;

    bool launcher_clear_on_launch = true;
    Path last_path_executable = Path(Path::HOME);

   private:
    Path active_executable;
    std::vector<Executable> data;

    // Executable CreateDefaultExecutable(const DefaultExecutable& default_executable) const;
};

const char* GetExecutableFilter();
const char* GetExecutable(ExecutableId id);
