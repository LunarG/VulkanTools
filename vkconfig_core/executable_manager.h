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

#pragma once

#include "serialization.h"
#include "executable.h"

#include <string>
#include <vector>

class ExecutableManager : public Serialize {
   public:
    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;
    std::string Log() const override;

    void Clear();
    bool Empty() const;
    std::size_t Size() const;

    void SetActiveExecutable(int executable_index);
    int GetActiveExecutableIndex() const;
    bool AppendExecutable(const Executable& executable);
    bool AppendExecutable(const Path& executable_path);
    bool RemoveExecutable();

    const std::vector<Executable>& GetExecutables() const { return this->data; }
    const Executable* GetActiveExecutable() const;
    Executable* GetActiveExecutable();
    const Executable* GetExecutable(std::size_t executable_index) const;
    Executable* GetExecutable(std::size_t executable_index);

    // Search for all the applications in the list, an remove the application which executable can't be found
    std::vector<Executable> RemoveMissingExecutables(const std::vector<Executable>& executables) const;

    bool launcher_clear_on_launch = true;
    Path last_path_executable = ::Get(Path::HOME);

   private:
    Path active_executable;
    std::vector<Executable> data;

    // Create a list of default executables, eg vkcube
    std::vector<Executable> CreateDefaultExecutables() const;
    Executable CreateDefaultExecutable(const DefaultExecutable& default_executable) const;
};

const char* GetExecutableFilter();
