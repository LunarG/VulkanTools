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

#include <QFileInfoList>

#include <string>

class Path {
   public:
    enum Builtin {
        HOME,  // Vulkan SDK user directory
        APPDATA,
        INIT,
        CONFIGS,
        LAYERS_SETTINGS,
        LOADER_SETTINGS,
        SDK,
        SDK_BIN,
        EXPLICIT_LAYERS,
        CONTENT,
    };

    Path();
    Path(const Path& path);
    Path(const char* path);
    Path(const std::string& path, bool recover_vars = false);

    Path& operator=(const Path& path);
    Path& operator=(const char* path);
    Path& operator=(const std::string& path);
    Path& operator+=(const std::string& extend);

    bool IsFile() const;
    bool IsDir() const;

    void Clear();
    bool Exists() const;
    bool Create(bool as_file = false) const;
    bool Remove() const;
    std::string Filename() const;
    std::string Basename() const;
    std::string AbsoluteDir() const;
    std::string AbsolutePath() const;  // Translate built-in variables into actual path
    std::string RelativePath() const;  // May contain built-in variables
    std::string LastModified() const;

    bool Empty() const { return this->data.empty(); }

   private:
    std::string data;

    friend bool operator==(const Path& a, const Path& b);
    friend Path operator+(const Path& path, const std::string& extend);
};

Path Get(Path::Builtin path);
std::string AbsolutePath(Path::Builtin path);
std::string RelativePath(Path::Builtin path);

bool operator==(const Path& a, const Path& b);

Path operator+(const Path& path, const std::string& extend);

std::vector<Path> CollectFilePaths(const Path& directory, const char* filter = "*json");

std::vector<std::string> CollectProfileNamesFromFile(const Path& profile_path);

std::vector<std::string> CollectProfileNamesFromDir(const Path& profile_path);

bool IsPortableFilename(const std::string& s);
