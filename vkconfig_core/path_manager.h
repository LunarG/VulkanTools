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
#include <QWidget>

#include <array>
#include <string>

enum Path {
    PATH_CONFIGURATION = 0,     // Where config working files live
    PATH_OVERRIDE_SETTINGS,     // Where settings go when profile is active
    PATH_OVERRIDE_LAYERS,       // Where json goes when profile is active
    PATH_IMPORT_CONFIGURATION,  // The last path used by the user to import a configuration
    PATH_EXPORT_CONFIGURATION,  // The last path used by the user to export a configuration
    PATH_EXECUTABLE,            // The last path used by the user when adding an executable to the application list
    PATH_WORKING_DIR,           // The last path used as a working directory
    PATH_HOME,                  // The user home directory
    PATH_LAUNCHER_LOG_FILE,     // The last path used by the user to set the launcher log file
    PATH_CUSTOM_LAYER_PATH,     // The last custom layer path

    PATH_FIRST = PATH_CONFIGURATION,
    PATH_LAST = PATH_CUSTOM_LAYER_PATH
};

enum { PATH_COUNT = PATH_LAST - PATH_FIRST + 1 };

enum PathMode {
    PATH_MODE_OPEN_DIRECTORY = 0,
    PATH_MODE_OPEN_FILENAME,
    PATH_MODE_SAVE_FILENAME,
};

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
    QString GetFullPath(Path path, const char* filename = nullptr) const;
    QString GetFullPath(Path path, const QString& filename) const;
    QString GetFullPath(Filename filename) const;

    QString GetFilename(const char* full_path) const;

    QString SelectPath(QWidget* parent, Path path);
    QString SelectPath(QWidget* parent, Path path, const QString& suggested_path);

    void Clear();
    void Reset();

   private:
    PathManager(const PathManager&) = delete;
    PathManager& operator=(const PathManager&) = delete;

    QString SelectPathImpl(QWidget* parent, Path path, const QString& suggested_path);

    std::array<std::string, PATH_COUNT> paths;
};
