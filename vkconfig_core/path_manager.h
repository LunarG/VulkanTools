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

#pragma once

#include "path.h"
#include "version.h"

#include <array>
#include <string>

enum PathType {
    PATH_IMPORT_CONFIGURATION = 0,  // The last path used by the user to import a configuration
    PATH_EXPORT_CONFIGURATION,      // The last path used by the user to export a configuration
    PATH_EXECUTABLE,                // The last path used by the user when adding an executable to the application list
    PATH_WORKING_DIR,               // The last path used as a working directory
    PATH_LAUNCHER_LOG_FILE,         // The last path used by the user to set the launcher log file
    PATH_USER_DEFINED_LAYERS_GUI,   // The last user-defined layers path
    PATH_VULKAN_SDK,                // Path of the Vulkan SDK

    PATH_FIRST = PATH_IMPORT_CONFIGURATION,
    PATH_LAST = PATH_VULKAN_SDK
};

enum { PATH_COUNT = PATH_LAST - PATH_FIRST + 1 };

class PathManager {
   public:
    PathManager(const std::string& VULKAN_SDK);
    ~PathManager();

    bool Load();
    bool Save();

    const std::string GetPath(PathType path) const;

    // The path value should not have the filename
    void SetPath(PathType path, const char* path_value);

    // The path value should not have the filename
    void SetPath(PathType path, const std::string& path_value);

    // When filename is set to nullptr, the function will try to use the default filename if there is one for the DirectoryType
    std::string GetFullPath(PathType path, const char* filename = nullptr) const;
    std::string GetFullPath(PathType path, const std::string& filename) const;

    std::string SelectPath(QWidget* parent, PathType path);
    std::string SelectPath(QWidget* parent, PathType path, const std::string& suggested_path);

    void Clear();
    void Reset();

   private:
    PathManager(const PathManager&) = delete;
    PathManager& operator=(const PathManager&) = delete;

    std::string SelectPathImpl(QWidget* parent, PathType path, const std::string& suggested_path);

    std::array<std::string, PATH_COUNT> paths;
};
