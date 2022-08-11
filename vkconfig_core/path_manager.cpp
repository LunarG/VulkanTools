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

#include "path_manager.h"
#include "path.h"
#include "util.h"
#include "platform.h"

#include <cassert>
#include <cstddef>

#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QSettings>
#include <QWidget>
#include <QFileDialog>

struct DirectoryDesc {
    const char* default_extension;  // file extension used to path
    const char* setting_data;       // system token to store the path, if empty the directory is not saved
    const char* default_filename;   // a default filename, if empty there is no default filename
    PathType alternative;
};

static const DirectoryDesc& GetDesc(PathType directory) {
    assert(directory >= PATH_FIRST && directory <= PATH_LAST);

    static const DirectoryDesc table[] = {
        {".json", "lastImportPath", nullptr, PATH_EXPORT_CONFIGURATION},  // PATH_IMPORT
        {".json", "lastExportPath", nullptr, PATH_IMPORT_CONFIGURATION},  // PATH_EXPORT
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
        {".exe", "lastExecutablePath", nullptr, PATH_WORKING_DIR},  // PATH_EXECUTABLE
#else
        {"", "lastExecutablePath", nullptr, PATH_WORKING_DIR},  // PATH_EXECUTABLE
#endif
        {nullptr, "lastWorkingDirPath", nullptr, PATH_EXECUTABLE},            // PATH_EXECUTABLE
        {".txt", "lastLauncherLogFile", "log", PATH_LAUNCHER_LOG_FILE},       // PATH_LAUNCHER_LOG_FILE
        {".json", "lastLayersPaths", nullptr, PATH_USER_DEFINED_LAYERS_GUI},  // PATH_USER_DEFINED_LAYERS_GUI
        {nullptr, "vulkan_sdk", nullptr, PATH_VULKAN_SDK}                     // PATH_VULKAN_SDK
    };
    static_assert(countof(table) == PATH_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[directory];
}

PathManager::PathManager(const std::string& VULKAN_SDK) {
    const bool result = Load();

    const std::string path = VULKAN_SDK.empty() ? qgetenv("VULKAN_SDK").toStdString() : VULKAN_SDK;
    if (!path.empty()) {
        this->SetPath(PATH_VULKAN_SDK, path);
    }

    assert(result);
}

PathManager::~PathManager() {
    const bool result = Save();
    assert(result);
}

bool PathManager::Load() {
    QSettings settings;
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        const PathType type = static_cast<PathType>(i);
        if (GetDesc(type).setting_data == nullptr) continue;
        paths[type] = settings.value(GetDesc(type).setting_data).toString().toStdString();
    }

    CheckPathsExist(::GetPath(BUILTIN_PATH_LOCAL));
    CheckPathsExist(::GetPath(BUILTIN_PATH_CONFIG_LAST));

    CheckPathsExist(::GetPath(BUILTIN_PATH_OVERRIDE_LAYERS), true);
    CheckPathsExist(::GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS), true);

    return true;
}

bool PathManager::Save() {
    QSettings settings;
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        const PathType type = static_cast<PathType>(i);
        if (GetDesc(type).setting_data == nullptr) continue;
        settings.setValue(GetDesc(type).setting_data, paths[type].c_str());
    }

    return true;
}

void PathManager::Clear() {
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        const PathType type = static_cast<PathType>(i);
        paths[type].clear();
    }
}

void PathManager::Reset() {
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        paths[static_cast<PathType>(i)].clear();
    }
}

const std::string PathManager::GetPath(PathType path) const {
    assert(path >= PATH_FIRST && path <= PATH_LAST);

    if (!paths[path].empty()) {
        return paths[path].c_str();
    }

    const PathType alternative_path = GetDesc(path).alternative;
    if (!paths[alternative_path].empty()) {
        return paths[alternative_path].c_str();
    }

    return ::GetPath(BUILTIN_PATH_HOME);
}

void PathManager::SetPath(PathType path, const char* path_value) {
    assert(path >= PATH_FIRST && path <= PATH_LAST);
    assert(path_value);

    const QDir directory(path_value);
    const std::string native_path(ConvertNativeSeparators(directory.absolutePath().toStdString()));
    paths[path] = native_path;
}

void PathManager::SetPath(PathType directory, const std::string& path_value) {
    assert(!path_value.empty());

    SetPath(directory, path_value.c_str());
}

std::string PathManager::GetFullPath(PathType path, const char* filename) const {
    const QFileInfo file_info(filename);

    const std::string path_base(GetPath(path).c_str());

    const std::string path_filename =
        filename != nullptr ? QFileInfo(filename).baseName().toStdString() : GetDesc(path).default_filename;
    assert(!path_filename.empty());  // Did you really mean to use GetFullPath? GetPath seem to be the right function here

    const std::string path_suffix = !file_info.completeSuffix().isEmpty()
                                        ? std::string(".") + file_info.completeSuffix().toStdString()
                                        : GetDesc(path).default_extension;
    assert(!path_suffix.empty() || VKC_PLATFORM != VKC_PLATFORM_WINDOWS);  // Only Windows has a suffix for executable

    const std::string full_path(ConvertNativeSeparators(path_base + GetNativeSeparator() + path_filename + path_suffix));
    return full_path;
}

std::string PathManager::GetFullPath(PathType path, const std::string& filename) const {
    return GetFullPath(path, filename.c_str());
}

std::string PathManager::SelectPath(QWidget* parent, PathType path) {
    assert(parent);
    assert(path >= PATH_FIRST && path <= PATH_LAST);

    return SelectPathImpl(parent, path, GetPath(path).c_str());
}

std::string PathManager::SelectPath(QWidget* parent, PathType path, const std::string& suggested_path) {
    assert(parent);
    assert(path >= PATH_FIRST && path <= PATH_LAST);

    if (suggested_path.empty())
        return SelectPathImpl(parent, path, GetPath(path).c_str());
    else
        return SelectPathImpl(parent, path, suggested_path);
}

std::string PathManager::SelectPathImpl(QWidget* parent, PathType path, const std::string& suggested_path) {
    assert(parent);
    assert(path >= PATH_FIRST && path <= PATH_LAST);
    assert(!suggested_path.empty());

    switch (path) {
        case PATH_LAUNCHER_LOG_FILE: {
            const std::string selected_path =
                QFileDialog::getSaveFileName(parent, "Set Log File To...", suggested_path.c_str(), "Log text(*.txt)").toStdString();
            if (selected_path.empty())  // The user cancelled
                return "";

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return GetFullPath(path, QFileInfo(selected_path.c_str()).baseName().toStdString());
        } break;
        case PATH_EXECUTABLE: {
            static const char* TABLE[] = {
                "Applications (*.exe)",     // PLATFORM_WINDOWS
                "Applications (*)",         // PLATFORM_LINUX
                "Applications (*.app, *)",  // PLATFORM_MACOS
                "N/A"                       // PLATFORM_ANDROID
            };
            static_assert(countof(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

            const std::string filter = TABLE[VKC_PLATFORM];
            const std::string selected_path =
                QFileDialog::getOpenFileName(parent, "Select a Vulkan Executable...", suggested_path.c_str(), filter.c_str())
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return "";

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return GetFullPath(path, QFileInfo(selected_path.c_str()).fileName().toStdString());
        }
        case PATH_WORKING_DIR: {
            const std::string selected_path =
                QFileDialog::getExistingDirectory(parent, "Set Working Folder To...", suggested_path.c_str()).toStdString();
            if (selected_path.empty())  // The user cancelled
                return "";

            SetPath(path, selected_path);
            return GetPath(path).c_str();
        }
        case PATH_USER_DEFINED_LAYERS_GUI: {
            const std::string selected_path =
                QFileDialog::getExistingDirectory(parent, "Add User-Defined Layers Folder...", suggested_path.c_str(),
                                                  QFileDialog::DontUseNativeDialog)
                    .toStdString();
            if (selected_path.empty())  // The user cancelled
                return "";

            SetPath(path, selected_path);
            return GetPath(path).c_str();
        }
        case PATH_IMPORT_CONFIGURATION: {
            const std::string selected_path = QFileDialog::getOpenFileName(parent, "Import Layers Configuration File",
                                                                           suggested_path.c_str(), "JSON configuration(*.json)")
                                                  .toStdString();
            if (selected_path.empty())  // The user cancelled
                return "";

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return selected_path;
        }
        case PATH_EXPORT_CONFIGURATION: {
            const std::string selected_path = QFileDialog::getSaveFileName(parent, "Export Layers Configuration File",
                                                                           suggested_path.c_str(), "JSON configuration(*.json)")
                                                  .toStdString();
            if (selected_path.empty())  // The user cancelled
                return "";

            SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
            return selected_path;
        }
        default:
            assert(0);
            return "";
    }
}
