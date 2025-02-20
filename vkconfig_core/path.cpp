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

#include "util.h"
#include "json.h"
#include "alert.h"
#include "path.h"
#include "type_platform.h"

#include <QDir>
#include <QDateTime>
#include <QJsonDocument>

#include <cassert>
#include <cstring>

static const std::string VKCONFIG_VERSION("vkconfig");

struct BuiltinDesc {
    const char* key;
    const Path::Builtin path;
};

static const BuiltinDesc VARIABLES[] = {{"${VK_HOME}", Path::HOME},          {"${VK_DOWNLOAD}", Path::DOWNLOAD},
                                        {"${VK_APPDATA}", Path::APPDATA},    {"${VULKAN_BIN}", Path::BIN},
                                        {"${VULKAN_SDK}", Path::SDK},        {"${VULKAN_PROFILES}", Path::PROFILES},
                                        {"${VULKAN_CONTENT}", Path::CONTENT}};

static std::string ConvertSeparators(const std::string& path, const char* native_separator, const char* alien_separator) {
    const std::size_t native_separator_size = std::strlen(native_separator);
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
    if (found == size - native_separator_size) {
        current_path = current_path.substr(0, found);
    }

    if (!path.empty()) {
        if (path.back() == '\\' || path.back() == '/') {
            current_path += native_separator;
        }
    }

    return current_path;
}

const char* Path::Separator() {
    static const char* native_separator = VKC_ENV == VKC_ENV_WIN32 ? "\\" : "/";
    return native_separator;
}

static std::string ConvertNativeSeparators(const std::string& path) {
    const char* native_separator = Path::Separator();
    const char* alien_separator = VKC_ENV != VKC_ENV_WIN32 ? "\\" : "/";

    return ConvertSeparators(path, native_separator, alien_separator);
}

Path::Path() {}

Path::Path(const Path& path) : data(path.data) {}

Path::Path(const char* path) : Path(std::string(path)) {}

Path::Path(const std::string& path) {
    std::string result = ConvertNativeSeparators(path);

    if (true) {
        for (std::size_t i = 0, n = std::size(VARIABLES); i < n; ++i) {
            std::string substring = ::AbsolutePath(VARIABLES[i].path);

            const std::size_t found = result.find(substring);
            if (found < substring.size()) {
                assert(found == 0);  // The builtin variable must be first part of the path
                const std::size_t offset = found + substring.size();
                const std::string replaced_path = VARIABLES[i].key + result.substr(found + offset, result.size() - offset);
                result = ConvertNativeSeparators(replaced_path);
                break;
            }
        }
    }

    this->data = result;
}

bool Path::IsFile() const { return QFileInfo(this->AbsolutePath().c_str()).isFile(); }

bool Path::IsDir() const {
    QFileInfo file_info(this->AbsolutePath().c_str());

    return file_info.isDir();
}

bool Path::IsBuiltIn() const { return this->data.find(":\\") == 0 || this->data.find(":/") == 0; }

Path& Path::operator=(const Path& path) {
    this->data = path.data;
    return *this;
}

Path& Path::operator=(const char* path) {
    this->data = Path(path).data;
    return *this;
}

Path& Path::operator=(const std::string& path) {
    this->data = Path(path).data;
    return *this;
}

void Path::Clear() { this->data.clear(); }

bool Path::Exists() const { return QFileInfo(this->AbsolutePath().c_str()).exists(); }

bool Path::Create(bool as_file) const {
    if (this->data.empty()) {
        return false;
    }

    if (as_file) {
        QFile file(this->AbsolutePath().c_str());
        if (!file.exists()) {
            return file.open(QFile::WriteOnly);
        }
    } else {
        QDir dir;
        std::string actual_path = this->AbsolutePath();
        if (!dir.exists(actual_path.c_str())) {
            return dir.mkpath(actual_path.c_str());
        }
    }

    return true;
}

bool Path::Remove() const {
    if (!this->Exists()) {
        return false;
    }

    if (this->IsFile()) {
        return QFile::remove(this->AbsolutePath().c_str());
    } else {
        return QDir(this->AbsolutePath().c_str()).removeRecursively();
    }
}

std::string Path::Filename() const {
    QFileInfo info(this->AbsolutePath().c_str());
    return info.fileName().toStdString();
}

std::string Path::Basename() const {
    QFileInfo info(this->AbsolutePath().c_str());
    return info.baseName().toStdString();
}

std::string Path::AbsoluteDir() const {
    QFileInfo info(this->AbsolutePath().c_str());
    return ConvertNativeSeparators(info.absoluteDir().path().toStdString());
}

std::string Path::AbsolutePath() const {
    for (std::size_t i = 0, n = std::size(VARIABLES); i < n; ++i) {
        const std::size_t found = this->data.find(VARIABLES[i].key);
        if (found < this->data.size()) {
            assert(found == 0);  // The builtin variable must be first part of the path
            const std::size_t offset = found + std::strlen(VARIABLES[i].key);
            const std::string replaced_path =
                Path(VARIABLES[i].path).RelativePath() + this->data.substr(found + offset, this->data.size() - offset);
            return ConvertNativeSeparators(replaced_path);
        }
    }

    // No built-in variable found, return unchanged
    return ConvertNativeSeparators(this->data);
}

std::string Path::RelativePath() const {
    // Store trailing separator to enable operator + and +=
    return ConvertNativeSeparators(this->data);
}

std::string Path::LastModified() const {
    QFileInfo file_info(this->data.c_str());

    return file_info.lastModified().toString(Qt::ISODate).toStdString();
}

bool operator==(const Path& a, const Path& b) { return a.AbsolutePath() == b.AbsolutePath(); }

bool operator!=(const Path& a, const Path& b) { return !(a == b); }

bool operator<(const Path& a, const Path& b) { return a.RelativePath() < b.RelativePath(); }

static const std::string GetDefaultHomeDir() {
    std::string absolute_path = qgetenv("VK_HOME").toStdString();

    if (absolute_path.empty()) {  // Default path
        absolute_path = QDir().homePath().toStdString() + "/VulkanSDK";
    }

    return absolute_path;
}

static std::string VK_CURRENT_HOME_PATH = GetDefaultHomeDir();

void SetHomePath(const std::string& path) { ::VK_CURRENT_HOME_PATH = path; }

static const std::string GetHomeDir() {
    std::string absolute_path = qgetenv("VK_HOME").toStdString();

    if (absolute_path.empty()) {  // Default path
        absolute_path = VK_CURRENT_HOME_PATH;
    }

    return absolute_path;
}

static std::string VK_CURRENT_DOWNLOAD_PATH = GetDefaultHomeDir() + "/Releases";

void SetDownloadPath(const std::string& path) { ::VK_CURRENT_DOWNLOAD_PATH = path; }

static const std::string GetDownloadDir() {
    std::string absolute_path = qgetenv("VK_DOWNLOAD").toStdString();

    if (absolute_path.empty()) {  // Default path
        absolute_path = VK_CURRENT_DOWNLOAD_PATH;
    }

    return absolute_path;
}

static const std::string GetAppDataDir() {
    const char* TABLE[] = {
        "/AppData/Local/LunarG",  // ENVIRONMENT_WIN32
        "/.local/share/vulkan"    // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    std::string absolute_path = QDir().homePath().toStdString() + TABLE[VKC_ENV];
    return absolute_path;
}

static const std::string GetInitPath() {
    const std::string TABLE[] = {
        "/" + VKCONFIG_VERSION + "/vkconfig.json",        // ENVIRONMENT_WIN32
        "/lunarg-" + VKCONFIG_VERSION + "/vkconfig.json"  // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    std::string absolute_path = GetAppDataDir() + TABLE[VKC_ENV];
    return absolute_path;
}

static const std::string GetConfigsDir() {
    const std::string TABLE[] = {
        "/" + VKCONFIG_VERSION + "/configurations",        // ENVIRONMENT_WIN32
        "/lunarg-" + VKCONFIG_VERSION + "/configurations"  // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    std::string absolute_path(GetAppDataDir() + TABLE[VKC_ENV]);
    return absolute_path;
}

static const std::string GetLayersSettingsPath() {
#if VKC_ENV == VKC_ENV_WIN32
    const std::string layers_settings_path = "/" + VKCONFIG_VERSION + "/override";
#elif VKC_ENV == VKC_ENV_UNIX
    const std::string layers_settings_path = "/settings.d";
#else
#error Unknown platform
#endif

    std::string result = qgetenv("VK_LAYER_SETTINGS_PATH").toStdString();
    if (result.empty()) {
        result = GetAppDataDir() + layers_settings_path;
    }
    if (result.find("vk_layer_settings.txt") == std::string::npos) {
        result += "/vk_layer_settings.txt";
    }

    return result;
}

static const std::string GetLoaderSettingsPath() {
#if VKC_ENV == VKC_ENV_WIN32
    const std::string loader_settings_path = "/vulkan";
#elif VKC_ENV == VKC_ENV_UNIX
    const std::string loader_settings_path = "/loader_settings.d";
#else
#error Unknown platform
#endif

    std::string result = GetAppDataDir() + loader_settings_path;
    if (result.find("vk_loader_settings.json") == std::string::npos) {
        result += "/vk_loader_settings.json";
    }

    return result;
}

static const std::string GetSDKDir() { return qgetenv("VULKAN_SDK").toStdString(); }

static const std::string GetExplicitLayersDir() {
    const std::string TABLE[] = {
        "/Bin",                           // ENVIRONMENT_WIN32
        "/share/vulkan/explicit_layer.d"  // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    return GetSDKDir() + TABLE[VKC_ENV];
}

static const std::string GetVulkanDir() {
    const char* TABLE[] = {
        "",            // PLATFORM_WINDOWS_X86
        "",            // PLATFORM_WINDOWS_ARM
        "/usr",        // PLATFORM_LINUX
        "/usr/local",  // PLATFORM_MACOS
        "",            // PLATFORM_ANDROID
        ""             // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    std::string absolute_path = ::GetSDKDir();
    if (absolute_path.empty()) {
        return TABLE[VKC_PLATFORM];
    } else {
        return absolute_path;
    }
}

static const std::string GetVulkanBinDir() {
    const std::string TABLE[] = {
        "/Bin",  // ENVIRONMENT_WIN32
        "/bin"   // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    std::string absolute_path = GetVulkanDir();
    if (absolute_path.empty()) {
        return "";
    }

    return ::GetVulkanDir() + TABLE[VKC_ENV];
}

static const std::string GetVulkanContentDir() {
    const std::string TABLE[] = {
        "/Config",              // ENVIRONMENT_WIN32
        "/share/vulkan/config"  // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    return ::GetVulkanDir() + TABLE[VKC_ENV];
}

static const std::string GetVulkanProfilesDir() {
    const std::string TABLE[] = {
        "/Config/VK_LAYER_KHRONOS_profiles",              // ENVIRONMENT_WIN32
        "/share/vulkan/config/VK_LAYER_KHRONOS_profiles"  // ENVIRONMENT_UNIX
    };
    static_assert(std::size(TABLE) == ENVIRONMENT_COUNT);

    return ::GetVulkanDir() + TABLE[VKC_ENV];
}

Path::Path(Path::Builtin path) {
    bool is_file = false;

    switch (path) {
        default:
            assert(0);
        case HOME:
            this->data = ::GetHomeDir();
            break;
        case DOWNLOAD:
            this->data = ::GetDownloadDir();
            break;
        case DEFAULT_HOME:
            this->data = ::GetDefaultHomeDir();
            break;
        case APPDATA:
            this->data = ::GetAppDataDir();
            break;
        case INIT:
            this->data = ::GetInitPath();
            is_file = true;
            break;
        case CONFIGS:
            this->data = ::GetConfigsDir();
            break;
        case LAYERS_SETTINGS:
            this->data = ::GetLayersSettingsPath();
            is_file = true;
            break;
        case LOADER_SETTINGS:
            this->data = ::GetLoaderSettingsPath();
            is_file = true;
            break;
        case BIN:
            this->data = ::GetVulkanBinDir();
            break;
        case SDK:
            this->data = ::GetSDKDir();
            break;
        case SDK_EXPLICIT_LAYERS:
            this->data = ::GetExplicitLayersDir();
            break;
        case CONTENT:
            this->data = ::GetVulkanContentDir();
            break;
        case PROFILES:
            this->data = ::GetVulkanProfilesDir();
            break;
    }

    if (!is_file) {
        if (!this->Exists()) {
            this->Create(false);
        }
    } else {
        Path dir;
        dir.data = this->AbsoluteDir();
        if (!dir.Exists()) {
            dir.Create(false);
        }
    }
}

std::string AbsolutePath(Path::Builtin path) { return Path(path).AbsolutePath(); }

std::string RelativePath(Path::Builtin path) { return Path(path).RelativePath(); }

std::vector<Path> CollectFilePaths(const Path& path, const char* filter) {
    std::vector<Path> result;

    if (path.IsDir()) {
        const std::string& absolute_path = path.AbsolutePath();

        const QDir dir(absolute_path.c_str());
        const QFileInfoList& list = dir.entryInfoList(QStringList() << filter, QDir::Files | QDir::NoSymLinks);

        for (int i = 0, n = list.size(); i < n; ++i) {
            result.push_back(list[i].absoluteFilePath().toStdString());
        }
    } else if (path.IsFile()) {
        result.push_back(path);
    }

    return result;
}

static std::vector<std::string> LoadProfiles(const QJsonDocument& doc) {
    assert(!doc.isNull() && !doc.isEmpty());

    const QJsonObject& json_root_object = doc.object();
    if (json_root_object.value("profiles") == QJsonValue::Undefined) {
        return std::vector<std::string>();
    }

    return ConvertString(ReadObject(json_root_object, "profiles").keys());
}

std::vector<std::string> CollectProfileNamesFromFile(const Path& profile_path) {
    if (profile_path.Empty()) {
        return std::vector<std::string>();
    }

    const QJsonDocument& doc = ParseJsonFile(profile_path.AbsolutePath().c_str());

    if (doc.isNull() || doc.isEmpty()) {
        return std::vector<std::string>();
    }

    const QJsonObject& json_root_object = doc.object();
    if (json_root_object.value("$schema").toString().toStdString().find("https://schema.khronos.org/vulkan/profiles") ==
        std::string::npos) {
        return std::vector<std::string>();
    }

    return LoadProfiles(doc);
}

std::vector<std::string> CollectProfileNamesFromDir(const Path& profile_path) {
    if (profile_path.Empty()) {
        return std::vector<std::string>();
    }

    Path path = profile_path;
    /*
        // Windows workaround so that the user could select C: D: etc QDir doesn't recognize it as valid path otherwise
        if (VKC_ENV == VKC_ENV_WIN32) {
            if (!EndsWith(path.AbsolutePath().c_str(), "\\")) {
                path = path.RelativePath() + "\\";
            }
        }
    */
    const std::vector<Path>& files = CollectFilePaths(path.AbsolutePath().c_str());

    std::vector<std::string> profile_names;
    for (std::size_t i = 0, n = files.size(); i < n; ++i) {
        const std::vector<std::string>& profile_names_of_files = CollectProfileNamesFromFile(files[i]);
        profile_names.insert(profile_names.end(), profile_names_of_files.begin(), profile_names_of_files.end());
    }

    return profile_names;
}

static bool IsPortableChar(char c) {
    if (c == '\\' || c == '/') return false;
    if (c == '|' || c == '<' || c == '>') return false;
    if (c == ':' || c == '*' || c == '?') return false;

    return true;
}

bool IsPortableFilename(const std::string& s) {
    if (s.empty()) {
        return false;
    }

    if (s == ".") {
        return false;
    }

    if (s.find("..") != std::string::npos) {
        return false;
    }

    for (std::size_t i = 0, n = s.size(); i < n; ++i) {
        if (!IsPortableChar(s[i])) {
            return false;
        }
    }

    return true;
}
