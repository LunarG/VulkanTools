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

#include "path_manager.h"
#include "util.h"

#include <cassert>
#include <cstddef>

#include <QDir.h>
#include <QString.h>
#include <QSettings.h>
#include <QDebug.h>

#include <windows.h>

struct DirectoryDesc {
    const char* label;
    const char* default_extension;  // file extension
    const char* setting;            // system token to store the path, if empty the directory is not saved
    const char* default_filename;   // a default filename, if empty there is no default filename
    const bool resettable;
    Path alternative;
};

static const DirectoryDesc& GetDesc(Path directory) {
    assert(directory >= PATH_FIRST && directory <= PATH_LAST);

    static const DirectoryDesc table[] = {
        // label, extension, setting, default_filename, resettable, alternative
        {"configuration", ".json", nullptr, nullptr, false, DIRECTORY_CONFIGURATION},                     // PATH_CONFIGURATION
        {"override settings", ".txt", nullptr, "vk_layer_settings", false, DIRECTORY_OVERRIDE_SETTINGS},  // PATH_OVERRIDE_SETTINGS
        {"override layers", ".json", nullptr, "VkLayer_override", false, DIRECTORY_OVERRIDE_LAYERS},      // PATH_OVERRIDE_LAYERS
        {"configuration import", ".json", "lastImportPath", nullptr, true, DIRECTORY_IMPORT_CONFIGURATION},  // PATH_IMPORT
        {"configuration export", ".json", "lastExportPath", nullptr, true, DIRECTORY_EXPORT_CONFIGURATION},  // PATH_EXPORT
#ifdef _WIN32
        {"executable", ".exe", "lastExecutablePath", nullptr, true, DIRECTORY_EXECUTABLE}  // PATH_EXECUTABLE
#else
        {"executable", "", "lastExecutablePath", true, PATH_EXECUTABLE}  // PATH_EXECUTABLE
#endif
    };
    static_assert(countof(table) == PATH_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return table[directory];
}

struct FilenameDesc {
    const char* filename;
};

static const FilenameDesc& GetDesc(Filename filename) {
    static const FilenameDesc table[] = {"applist.json"};

    return table[filename];
}

PathManager::PathManager() {
    QSettings settings;
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        const Path type = static_cast<Path>(i);
        if (GetDesc(type).setting == nullptr) continue;
        paths[type] = settings.value(GetDesc(type).setting).toString().toUtf8().constData();
    }
}

PathManager::~PathManager() {
    QSettings settings;
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        const Path type = static_cast<Path>(i);
        if (GetDesc(type).setting == nullptr) continue;
        settings.setValue(GetDesc(type).setting, paths[type].c_str());
    }
}

PathManager& PathManager::Get() {
    static PathManager instance;
    return instance;
}

void PathManager::ResetToDefault() {
    for (std::size_t i = 0; i < PATH_COUNT; ++i) {
        const Path type = static_cast<Path>(i);

        if (GetDesc(type).resettable) {
            paths[type].clear();
        }
    }
}

const char* PathManager::GetDirectory(Path path) const {
    assert(path >= PATH_FIRST && path <= PATH_LAST);

    const char* path_value = paths[path].c_str();
    if (!paths[path].empty()) {
        return path_value;
    }

    const Path alternative_path = GetDesc(path).alternative;
    if (!paths[alternative_path].empty()) {
        return paths[alternative_path].c_str();
    }

    // No path found, return home directory
    return QDir::homePath().toUtf8().constData();
}

void PathManager::SetDirectory(Path path, const char* path_value) {
    assert(path >= PATH_FIRST && path <= PATH_LAST);
    assert(path_value);

    const QFileInfo file_info(path_value);
    assert(file_info.baseName().isEmpty());

    const QString native_path = QDir::toNativeSeparators(file_info.absolutePath());

    OutputDebugString("LOG: SetDirectory(Path path, const char* path_value)\n");
    OutputDebugString(native_path.toUtf8().constData());
    OutputDebugString("\n");

    paths[path] = native_path.toUtf8().constData();
}

void PathManager::SetDirectory(Path directory, const QString& path_value) {
    SetDirectory(directory, path_value.toStdString().c_str());
}

QString GetFullPath(Path path, const char* filename) {
    const QFileInfo file_info(filename);

    const QString path_base = PathManager::Get().GetDirectory(path);

    const QString path_filename = filename != nullptr ? filename : GetDesc(path).default_filename;
    assert(!path_filename.isEmpty());

    const QString path_suffix =
        !file_info.completeSuffix().isEmpty() ? file_info.completeSuffix() : GetDesc(path).default_extension;
    assert(!path_suffix.isEmpty());

    const QString full_path = QDir::toNativeSeparators(path_base + "/" + path_filename + path_suffix);

    OutputDebugString("LOG: GetFullPath(Path path, const char* filename)\n");
    OutputDebugString(full_path.toUtf8().constData());
    OutputDebugString("\n");

    return full_path;
}

QString GetFullPath(Path path, const QString& filename) { return GetFullPath(path, filename.toStdString().c_str()); }

QString GetFullPath(Filename filename) {
    const char* dir = PathManager::Get().GetDirectory(DIRECTORY_CONFIGURATION);

    const QString full_path = QDir::toNativeSeparators(QString(dir) + "/" + GetDesc(filename).filename);

    OutputDebugString("LOG: GetFullPath(Filename filename)\n");
    OutputDebugString(full_path.toUtf8().constData());
    OutputDebugString("\n");

    return full_path;
}

QString ExactFilename(const char* full_path) { return QFileInfo(full_path).fileName(); }
