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

#include "path.h"
#include "platform.h"

#include <QDir>

#include <cassert>
#include <cstring>

Path::Path() {}

Path::Path(const char* path) : data(ConvertNativeSeparators(path)) {}

Path& Path::operator=(const std::string& path) {
    data = ConvertNativeSeparators(path);
    return *this;
}

const char* Path::c_str() const {
    assert(ConvertNativeSeparators(data) == data);

    return data.c_str();
}

void CheckPathsExist(const std::string& path) {
    const QString tmp_path(ConvertNativeSeparators(path).c_str());

    QDir dir;
    if (!dir.exists(tmp_path)) {
        dir.mkpath(tmp_path);
        assert(dir.exists(tmp_path));
    }
}

std::string ReplacePathBuiltInVariables(const std::string& path) {
    static const std::string HOME("$HOME");

    const std::size_t found = path.find_first_of(HOME);
    if (found < path.size()) {
        assert(found == 0);  // The home variable must be first in the path
        const std::size_t offset = found + HOME.size();
        return ConvertNativeSeparators(QDir().homePath().toStdString() + path.substr(found + offset, path.size() - offset).c_str());
    }

    // No built-in variable found, return unchanged
    return path;
}

std::string ValidatePath(const std::string& path) {
    if (path.find("stdout") != std::string::npos)
        return "stdout";
    else if (path.find("stderr") != std::string::npos)
        return "stderr";

    if (path.empty()) return path;

    FILE* file = fopen(path.c_str(), "w+");
    if (file == nullptr) {
        CheckPathsExist(QDir::homePath().toStdString() + GetNativeSeparator() + "vulkan-sdk");
        const QFileInfo file_info(path.c_str());
        return ConvertNativeSeparators(QDir().homePath().toStdString() + "/vulkan-sdk/" + file_info.fileName().toStdString());
    } else {
        fclose(file);
        return path;
    }
}

std::string ConvertNativeSeparators(const std::string& path) {
    const char* native_separator = GetNativeSeparator();
    const std::size_t native_separator_size = std::strlen(native_separator);
    const char* alien_separator = VKC_PLATFORM != VKC_PLATFORM_WINDOWS ? "\\" : "/";
    const std::size_t alien_separator_size = std::strlen(alien_separator);

    std::string current_path = path;
    while (current_path.find_first_of(alien_separator) < current_path.size()) {
        const std::size_t found = current_path.find_first_of(alien_separator);
        const std::size_t size = current_path.size();

        if (found < size - alien_separator_size)
            current_path = current_path.substr(0, found) + native_separator +
                           current_path.substr(found + alien_separator_size, size - found - alien_separator_size);
        else
            current_path = current_path.substr(0, found);
    }

    // Remove trailing native separator
    const std::size_t found = current_path.find_last_of(native_separator);
    const std::size_t size = current_path.size();
    if (found == size - native_separator_size) current_path = current_path.substr(0, found);

    return current_path;
}

const char* GetNativeSeparator() {
    static const char* native_separator = VKC_PLATFORM == VKC_PLATFORM_WINDOWS ? "\\" : "/";
    return native_separator;
}
