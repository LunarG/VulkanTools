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

#include "util.h"
#include "path.h"
#include "path_manager.h"
#include "platform.h"

#include <QDir>

#include <cassert>
#include <cstring>

Path::Path() {}

Path::Path(const char* path) : data(ConvertNativeSeparators(path)) {}

Path::Path(const std::string& path) : data(ConvertNativeSeparators(path)) {}

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

std::string GetPath(BuiltinPath path) {
    switch (path) {
        case BUILTIN_PATH_HOME:
            return ConvertNativeSeparators(QDir().homePath().toStdString());
        case BUILTIN_PATH_LOCAL:
            return ConvertNativeSeparators(GetPlatformString(PLATFORM_STRING_VULKAN_SDK_LOCAL));
        case BUILTIN_PATH_CONFIG_REF: {
            return GetPath(BUILTIN_PATH_APPDATA) + GetPlatformString(PLATFORM_STRING_PATH_CONFIGURATION);
        }
        case BUILTIN_PATH_CONFIG_LAST: {
            const std::string config = format("_%d_%d_%d", Version::LAYER_CONFIG.GetMajor(), Version::LAYER_CONFIG.GetMinor(),
                                              Version::LAYER_CONFIG.GetPatch());

            return GetPath(BUILTIN_PATH_CONFIG_REF) + config;
        }
        case BUILTIN_PATH_OVERRIDE_SETTINGS: {
            std::string path(qgetenv("VK_LAYER_SETTINGS_PATH").toStdString());
            if (path.empty()) {
                path += GetPath(BUILTIN_PATH_APPDATA);
                path += GetPlatformString(PLATFORM_STRING_PATH_OVERRIDE_SETTINGS);
            }
            return ConvertNativeSeparators(path);
        }
        case BUILTIN_PATH_OVERRIDE_LAYERS: {
            const std::string path = GetPath(BUILTIN_PATH_APPDATA) + GetPlatformString(PLATFORM_STRING_PATH_OVERRIDE_LAYERS);
            return ConvertNativeSeparators(path);
        }
        case BUILTIN_PATH_APPDATA: {
            return ConvertNativeSeparators(GetPath(BUILTIN_PATH_HOME) + GetPlatformString(PLATFORM_STRING_APPDATA));
        }
        case BUILTIN_PATH_VULKAN_SDK: {
            std::string path(qgetenv("VULKAN_SDK"));
            if (path.empty()) {
                assert(VKC_PLATFORM != VKC_PLATFORM_WINDOWS);
                path = GetPlatformString(PLATFORM_STRING_VULKAN_SDK_SYSTEM);
            }
            return ConvertNativeSeparators(path);
        }
        case BUILTIN_PATH_VULKAN_CONTENT: {
            return ConvertNativeSeparators(GetPath(BUILTIN_PATH_VULKAN_SDK) + GetPlatformString(PLATFORM_STRING_VK_CONTENT));
        }
        default: {
            assert(0);
            return "";
        }
    }
}

struct BuiltinDesc {
    BuiltinPath type;
    const char* name;
};

std::string ReplaceBuiltInVariable(const std::string& path) {
    static const BuiltinDesc VARIABLES[] = {{BUILTIN_PATH_HOME, "${HOME}"},
                                            {BUILTIN_PATH_LOCAL_LEGACY, "${LOCAL}"},
                                            {BUILTIN_PATH_LOCAL, "${VK_LOCAL}"},
                                            {BUILTIN_PATH_APPDATA, "${VK_APPDATA}"},
                                            {BUILTIN_PATH_VULKAN_SDK, "${VULKAN_SDK}"},
                                            {BUILTIN_PATH_VULKAN_CONTENT, "${VULKAN_CONTENT}"}};

    for (std::size_t i = 0, n = countof(VARIABLES); i < n; ++i) {
        const std::size_t found = path.find(VARIABLES[i].name);
        if (found < path.size()) {
            assert(found == 0);  // The builtin variable must be first part of the path
            const std::size_t offset = found + std::strlen(VARIABLES[i].name);
            const std::string replaced_path = GetPath(VARIABLES[i].type) + path.substr(found + offset, path.size() - offset);
            return ConvertNativeSeparators(replaced_path);
        }
    }

    // No built-in variable found, return unchanged
    return path;
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

static bool IsPortableChar(char c) {
    if (c == '\\' || c == '/') return false;
    if (c == '|' || c == '<' || c == '>') return false;
    if (c == ':' || c == '*' || c == '?') return false;

    return true;
}

bool IsPortableFilename(const std::string& path) {
    if (path.empty()) return false;

    if (path == ".") return false;

    if (path.find("..") != std::string::npos) return false;

    for (std::size_t i = 0, n = path.size(); i < n; ++i) {
        if (!IsPortableChar(path[i])) return false;
    }

    return true;
}

QFileInfoList GetJSONFiles(const char* directory) {
    QDir dir(directory);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    return dir.entryInfoList();
}

std::string ExtractAbsoluteDir(const std::string& path) {
    assert(!path.empty());

    return ConvertNativeSeparators(QFileInfo(path.c_str()).absoluteDir().path().toStdString());
}
