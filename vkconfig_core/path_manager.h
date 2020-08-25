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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <QString>

#include <array>
#include <string>

enum Path {
    DIRECTORY_CONFIGURATION = 0,         // Where config working files live
    DIRECTORY_OVERRIDE_SETTINGS = 1,     // Where settings go when profile is active
    DIRECTORY_OVERRIDE_LAYERS = 2,       // Where json goes when profile is active
    DIRECTORY_IMPORT_CONFIGURATION = 3,  // The last path used by the user to import a configuration
    DIRECTORY_EXPORT_CONFIGURATION = 4,  // The last path used by the user to export a configuration
    DIRECTORY_EXECUTABLE = 5,            // The last path used by the user when adding an executable to the application list

    PATH_FIRST = DIRECTORY_CONFIGURATION,
    PATH_LAST = DIRECTORY_EXECUTABLE
};

enum { PATH_COUNT = PATH_LAST - PATH_FIRST + 1 };

enum Filename { FILENAME_APPLIST };

class PathManager {
   public:
    static PathManager& Get();

    const char* GetDirectory(Path path) const;

    // The path value should not have the filename
    void SetDirectory(Path path, const char* path_value);

    // The path value should not have the filename
    void SetDirectory(Path path, const QString& path_value);

    void ResetToDefault();

   private:
    PathManager();
    ~PathManager();

    std::array<std::string, PATH_COUNT> paths;
};

// When filename is set to nullptr, the function will try to use the default filename if there is one for the DirectoryType
QString GetFullPath(Path directory, const char* filename = nullptr);
QString GetFullPath(Path directory, const QString& filename);
QString GetFullPath(Filename filename);

QString ExactFilename(const char* full_path);
