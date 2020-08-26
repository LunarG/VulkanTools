/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include <QString>

#include <array>
#include <string>

enum Path {
    PATH_CONFIGURATION = 0,     // Where config working files live
    PATH_OVERRIDE_SETTINGS,     // Where settings go when profile is active
    PATH_OVERRIDE_LAYERS,       // Where json goes when profile is active
    PATH_IMPORT_CONFIGURATION,  // The last path used by the user to import a configuration
    PATH_EXPORT_CONFIGURATION,  // The last path used by the user to export a configuration
    PATH_EXECUTABLE,            // The last path used by the user when adding an executable to the application list
    PATH_HOME,

    PATH_FIRST = PATH_CONFIGURATION,
    PATH_LAST = PATH_HOME
};

enum { PATH_COUNT = PATH_LAST - PATH_FIRST + 1 };

enum Filename { FILENAME_APPLIST };

class PathManager {
   public:
    PathManager();
    ~PathManager();

    const char* GetPath(Path path) const;

    // The path value should not have the filename
    void SetPath(Path path, const char* path_value);

    // The path value should not have the filename
    void SetPath(Path path, const QString& path_value);

    // When filename is set to nullptr, the function will try to use the default filename if there is one for the DirectoryType
    QString GetFullPath(Path directory, const char* filename = nullptr) const;
    QString GetFullPath(Path directory, const QString& filename) const;
    QString GetFullPath(Filename filename) const;

    QString GetFilename(const char* full_path) const;

    void Clear();
    void Reset();

   private:
    std::array<std::string, PATH_COUNT> paths;
};
