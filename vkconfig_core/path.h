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

#include <string>

class Path {
   public:
    Path();
    explicit Path(const char* path);

    Path& operator=(const std::string& path);

    const char* c_str() const;
    bool empty() const { return data.empty(); }

   private:
    std::string data;
};

std::string ConvertNativeSeparators(const std::string& path);

const char* GetNativeSeparator();

// Create a directory if it doesn't exist
void CheckPathsExist(const std::string& path);

// Replace "$HOME" built-in variable by the actual system home directory
std::string ReplacePathBuiltInVariables(const std::string& path);

// Exact the filename and change the path to "$HOME" directory if necessary
std::string ValidatePath(const std::string& path);
