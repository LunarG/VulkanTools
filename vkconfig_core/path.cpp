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
#include "json.h"
#include "alert.h"
#include "path.h"
#include "path_manager.h"
#include "platform.h"

#include <QDir>
#include <QJsonDocument>

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

void CheckPathsExist(const std::string& path, bool is_full_path) {
    QString tmp_path(ConvertNativeSeparators(path).c_str());

    if (is_full_path) {
        QFileInfo file_info(tmp_path);
        tmp_path = file_info.absoluteDir().absolutePath();
    }

    QDir dir;
    if (!dir.exists(tmp_path)) {
        dir.mkpath(tmp_path);
        assert(dir.exists(tmp_path));
    }
}

std::string GetPath(BuiltinPath path) {
    std::string result;

    switch (path) {
        case BUILTIN_PATH_HOME: {
            result = QDir().homePath().toStdString();
            break;
        }
        case BUILTIN_PATH_LOCAL_LEGACY:
        case BUILTIN_PATH_LOCAL: {
            result = GetPath(BUILTIN_PATH_HOME) + "/VulkanSDK";
            break;
        }
        case BUILTIN_PATH_CONFIG_REF: {
            static const char* TABLE[] = {
                "/vkconfig/configurations",        // ENVIRONMENT_WIN32
                "/lunarg-vkconfig/configurations"  // ENVIRONMENT_UNIX
            };
            static_assert(countof(TABLE) == ENVIRONMENT_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            result = GetPath(BUILTIN_PATH_APPDATA) + TABLE[VKC_ENV];
            break;
        }
        case BUILTIN_PATH_CONFIG_LAST: {
            const std::string config = format("_%d_%d_%d", Version::LAYER_CONFIG.GetMajor(), Version::LAYER_CONFIG.GetMinor(),
                                              Version::LAYER_CONFIG.GetPatch());

            result = GetPath(BUILTIN_PATH_CONFIG_REF) + config;
            break;
        }
        case BUILTIN_PATH_APPLIST: {
            result = GetPath(BUILTIN_PATH_CONFIG_LAST) + "/../applist.json";
            break;
        }
        case BUILTIN_PATH_OVERRIDE_SETTINGS: {
            static const char* TABLE[] = {
                "/vkconfig/override",  // ENVIRONMENT_WIN32
                "/settings.d"          // ENVIRONMENT_UNIX
            };
            static_assert(countof(TABLE) == ENVIRONMENT_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            result = qgetenv("VK_LAYER_SETTINGS_PATH").toStdString();
            if (result.empty()) {
                result = GetPath(BUILTIN_PATH_APPDATA) + TABLE[VKC_ENV];
            }
            if (result.find("vk_layer_settings.txt") == std::string::npos) {
                result += "/vk_layer_settings.txt";
            }
            break;
        }
        case BUILTIN_PATH_OVERRIDE_LAYERS: {
            static const char* TABLE[] = {
                "/vkconfig/override",  // ENVIRONMENT_WIN32
                "/implicit_layer.d"    // ENVIRONMENT_UNIX
            };
            static_assert(countof(TABLE) == ENVIRONMENT_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            result = GetPath(BUILTIN_PATH_APPDATA) + TABLE[VKC_ENV];
            result += "/VkLayer_override.json";
            break;
        }
        case BUILTIN_PATH_APPDATA: {
            static const char* TABLE[] = {
                "/AppData/Local/LunarG",  // ENVIRONMENT_WIN32
                "/.local/share/vulkan"    // ENVIRONMENT_UNIX
            };
            static_assert(countof(TABLE) == ENVIRONMENT_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            result = GetPath(BUILTIN_PATH_HOME) + TABLE[VKC_ENV];
            break;
        }
        case BUILTIN_PATH_EXPLICIT_LAYERS: {
            static const char* TABLE[] = {
                "/Bin",                         // ENVIRONMENT_WIN32
                "/etc/vulkan/explicit_layer.d"  // ENVIRONMENT_UNIX
            };
            static_assert(countof(TABLE) == ENVIRONMENT_COUNT,
                          "The tranlation table size doesn't match the enum number of elements");

            result = GetPath(BUILTIN_PATH_VULKAN_SDK) + TABLE[VKC_ENV];
            break;
        }
        case BUILTIN_PATH_VULKAN_SDK: {
            static const char* TABLE[] = {
                "N/A",                      // PLATFORM_WINDOWS
                "/usr",                     // PLATFORM_LINUX
                "/usr/local/share/vulkan",  // PLATFORM_MACOS
                "N/A"                       // PLATFORM_ANDROID
            };
            static_assert(countof(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

            result = qgetenv("VULKAN_SDK").toStdString();
            if (result.empty()) {
                if (VKC_PLATFORM != VKC_PLATFORM_WINDOWS) {
                    result = TABLE[VKC_PLATFORM];
                } else {
                    result = GetPath(BUILTIN_PATH_LOCAL);
                }
            } else {  // VULKAN_SDK may be set on macOS
                if (VKC_PLATFORM == VKC_PLATFORM_MACOS) result += "/share/vulkan";
            }

            break;
        }
        case BUILTIN_PATH_VULKAN_CONTENT: {
            static const char* TABLE[] = {
                "/Config",               // PLATFORM_WINDOWS
                "/share/vulkan/config",  // PLATFORM_LINUX
                "/config",               // PLATFORM_MACOS
                "N/A"                    // PLATFORM_ANDROID
            };
            static_assert(countof(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

            result = GetPath(BUILTIN_PATH_VULKAN_SDK) + TABLE[VKC_PLATFORM];
            break;
        }
        default: {
            assert(0);
            return "";
        }
    }

    return ConvertNativeSeparators(result);
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
    const char* alien_separator = VKC_ENV != VKC_ENV_WIN32 ? "\\" : "/";
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
    static const char* native_separator = VKC_ENV == VKC_ENV_WIN32 ? "\\" : "/";
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

static std::vector<std::string> LoadProfiles(const QJsonDocument& doc) {
    assert(!doc.isNull() && !doc.isEmpty());

    const QJsonObject& json_root_object = doc.object();
    if (json_root_object.value("profiles") == QJsonValue::Undefined) {
        return std::vector<std::string>();
    }

    return ConvertString(ReadObject(json_root_object, "profiles").keys());
}

std::vector<std::string> GetProfileNames(const std::string& profile_path) {
    const std::string& value = ReplaceBuiltInVariable(profile_path);
    const QJsonDocument& doc = ParseJsonFile(value.c_str());

    if (doc.isNull() || doc.isEmpty()) {
        return std::vector<std::string>();
    }

    const QJsonObject& json_root_object = doc.object();
    if (json_root_object.value("$schema").toString().toStdString().find("https://schema.khronos.org/vulkan/profiles") ==
        std::string::npos) {
        Alert::FileNotProfile(value.c_str());
        return std::vector<std::string>();
    }

    return LoadProfiles(doc);
}
