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

#include "util.h"

#include <cstddef>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <cctype>

#include <QDir>

std::string format(const char* message, ...) {
    std::size_t const STRING_BUFFER(4096);

    assert(message != nullptr);
    assert(strlen(message) >= 0 && strlen(message) < STRING_BUFFER);

    char buffer[STRING_BUFFER];
    va_list list;

    va_start(list, message);
    vsprintf(buffer, message, list);
    va_end(list);

    return buffer;
}

bool IsNumber(const std::string& s) {
    if (s.empty()) return false;

    for (std::size_t i = 0, n = s.length(); i < n; ++i) {
        if (std::isdigit(s[i])) continue;

        return false;
    }

    return true;
}

void CheckPathsExist(const QString& path) {
    QDir dir;  // = QDir::home();
    if (!dir.exists(path)) {
        dir.mkpath(path);
        assert(dir.exists(path));
    }
}

std::string ReplacePathBuiltInVariables(const std::string& path) {
    static const std::string HOME("$HOME");

    const std::size_t found = path.find_first_of(HOME);
    if (found < path.size()) {
        assert(found == 0);  // The home variable must be first in the path
        const std::size_t offset = found + HOME.size();
        return QDir::toNativeSeparators(QDir().homePath() + path.substr(found + offset, path.size() - offset).c_str())
            .toStdString();
    }

    // No built-in variable found, return unchanged
    return path;
}

std::string ValidatePath(const std::string& path) {
    if (path.empty()) return path;

    FILE* file = fopen(path.c_str(), "w+");
    if (file == nullptr) {
        CheckPathsExist(QDir::homePath() + "/vulkan-sdk");
        const QFileInfo file_info(path.c_str());
        return QDir::toNativeSeparators(QDir().homePath() + "/vulkan-sdk/" + file_info.fileName()).toStdString();
    } else {
        fclose(file);
        return path;
    }
}
