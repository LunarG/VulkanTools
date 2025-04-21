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

#include <QFileInfoList>

#include <string>

class Path {
   public:
    enum Builtin {
        HOME,  // Vulkan SDK user directory
        DOWNLOAD,
        DEFAULT_HOME,
        APPDATA,
        INIT,
        CONFIGS,
        LAYERS_SETTINGS,
        LOADER_SETTINGS,
        BIN,
        SDK,
        SDK_EXPLICIT_LAYERS,
        PROFILES,
        CONTENT,
        URL_SDK
    };

    Path();
    Path(Path::Builtin path);
    Path(const Path& path);
    Path(const char* path);
    Path(const std::string& path, bool native_separator = true);

    Path& operator=(const Path& path);
    Path& operator=(const char* path);
    Path& operator=(const std::string& path);

    bool IsFile() const;
    bool IsDir() const;

    bool IsBuiltIn() const;

    void Clear();
    bool Exists() const;
    bool Create(bool as_file = false) const;
    bool Remove() const;
    std::string Filename() const;
    std::string Basename() const;
    std::string AbsoluteDir() const;
    std::string AbsolutePath(bool native_separator = true) const;  // Translate built-in variables into actual path
    std::string RelativePath(bool native_separator = true) const;  // May contain built-in variables
    std::string LastModified() const;

    bool Empty() const { return this->data.empty(); }

    static const char* Separator();

   private:
    std::string data;

    friend bool operator==(const Path& a, const Path& b);
    friend bool operator!=(const Path& a, const Path& b);
};

std::string AbsolutePath(Path::Builtin path, bool native_separator = true);
std::string RelativePath(Path::Builtin path);

std::string ConvertNativeSeparators(const std::string& path);
std::string TrimEndSeparator(const std::string& path);

bool operator==(const Path& a, const Path& b);
bool operator!=(const Path& a, const Path& b);
bool operator<(const Path& a, const Path& b);

void SetHomePath(const std::string& path);

void SetDownloadPath(const std::string& path);

std::vector<Path> CollectFilePaths(const Path& directory, const char* filter = "*json");

std::vector<std::string> CollectProfileNamesFromFile(const Path& profile_path);

std::vector<std::string> CollectProfileNamesFromDir(const Path& profile_path);

bool IsPortableFilename(const std::string& s);
