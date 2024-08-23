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

#include "version.h"
#include "application.h"
#include "type_tab.h"
#include "type_log.h"
#include "serialization.h"

#include <QByteArray>

#include <array>
#include <vector>
#include <string>

struct DefaultApplication {
    std::string name;
    std::string key;
    std::string arguments;
};

struct DefaultPath {
    Path executable_path;
    Path working_folder;
};

class Environment : public Serialize {
   public:
    Environment();
    ~Environment();

    bool Load(const QJsonObject& json_root_object) override;
    bool Save(QJsonObject& json_root_object) const override;
    void Reset() override;

    void SelectActiveApplication(std::size_t application_index);
    int GetActiveApplicationIndex() const;
    bool AppendApplication(const Application& application);
    bool RemoveApplication(std::size_t application_index);

    const std::vector<Application>& GetApplications() const { return applications; }
    const Application& GetActiveApplication() const;
    Application& GetActiveApplication();
    const Application& GetApplication(std::size_t application_index) const;
    Application& GetApplication(std::size_t application_index);

    LogFlags GetLoaderMessageFlags() const { return this->loader_message_types_flags; }
    void SetLoaderMessageFlags(LogFlags flags) { this->loader_message_types_flags = flags; }

    // Search for all the applications in the list, an remove the application which executable can't be found
    std::vector<Application> RemoveMissingApplications(const std::vector<Application>& applications) const;

    bool has_crashed = false;
    int hide_message_boxes_flags = 0;

    Path path_export;
    Path path_import;

    bool launcher_clear_on_launch = true;

   private:
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    TabType active_tab = TAB_DIAGNOSTIC;
    LogFlags loader_message_types_flags;
    Path home_sdk_path;

    int active_executable_index;
    std::vector<Application> applications;

    // Update default applications path to use relative path (really useful only on Windows)
    std::vector<Application> UpdateDefaultApplications(const std::vector<Application>& applications) const;

    // Create a list of default applications, eg vkcube
    std::vector<Application> CreateDefaultApplications() const;
    Application CreateDefaultApplication(const DefaultApplication& default_application) const;
    DefaultPath GetDefaultExecutablePath(const std::string& executable_name) const;
};

bool ExactExecutableFromAppBundle(Path& path);
