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

#include <QFileInfoList>

#include <string>

enum BuiltinPath {
    BUILTIN_PATH_HOME = 0,
    BUILTIN_PATH_LOCAL_LEGACY,
    BUILTIN_PATH_LOCAL,
    BUILTIN_PATH_APPDATA,
    BUILTIN_PATH_CONFIG_REF,
    BUILTIN_PATH_CONFIG_LAST,
    BUILTIN_PATH_APPLIST,
    BUILTIN_PATH_OVERRIDE_LAYERS,
    BUILTIN_PATH_OVERRIDE_SETTINGS,
    BUILTIN_PATH_EXPLICIT_LAYERS,
    BUILTIN_PATH_VULKAN_SDK,
    BUILTIN_PATH_VULKAN_CONTENT,

    BUILTIN_PATH_FIRST = BUILTIN_PATH_HOME,
    BUILTIN_PATH_LAST = BUILTIN_PATH_VULKAN_CONTENT,
};

enum { BUILTIN_PATH_COUNT = BUILTIN_PATH_LAST - BUILTIN_PATH_FIRST + 1 };

class Path {
   public:
    Path();
    explicit Path(const char* path);
    explicit Path(const std::string& path);

    Path& operator=(const std::string& path);

    const char* c_str() const;
    bool empty() const { return data.empty(); }

   private:
    std::string data;
};

std::string ConvertNativeSeparators(const std::string& path);

const char* GetNativeSeparator();

// Create a directory if it doesn't exist
void CheckPathsExist(const std::string& path, bool is_full_path = false);

std::string GetPath(BuiltinPath path);

// Replace built-in variable by the actual path
std::string ReplaceBuiltInVariable(const std::string& path);

bool IsPortableFilename(const std::string& path);

QFileInfoList GetJSONFiles(const char* directory);

std::string ExtractAbsoluteDir(const std::string& path);

std::vector<std::string> GetProfileNames(const std::string& profile_path);
