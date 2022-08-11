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
 * Author:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../path_manager.h"
#include "../util.h"
#include "../platform.h"

#include <QDir>
#include <QFileInfo>

#include <gtest/gtest.h>

static void Init(PathManager& paths, const std::string& path_value) {
    for (int i = 0, n = PATH_COUNT; i < n; ++i) {
        const PathType path = static_cast<PathType>(i);
        paths.SetPath(path, path_value);
    }
}

static std::string InitPath(const char* tail) {
    const QDir dir(QString("vkconfig/test_path_manager/") + tail);
    const std::string native_path(ConvertNativeSeparators(dir.absolutePath().toStdString()));
    return native_path;
}

TEST(test_path_manager, init_first) {
    const std::string path_value = InitPath("init_first");
    PathManager paths("");
    Init(paths, path_value);

    EXPECT_STREQ(path_value.c_str(), paths.GetPath(PATH_FIRST).c_str());
}

TEST(test_path_manager, init_last) {
    const std::string path_value = InitPath("init_last");
    PathManager paths("");
    Init(paths, path_value);

    EXPECT_STREQ(path_value.c_str(), paths.GetPath(PATH_LAST).c_str());
}

TEST(test_path_manager, init_all) {
    PathManager paths("");
    paths.Clear();

    for (int i = PATH_FIRST, n = PATH_LAST; i <= n; ++i) {
        const PathType path = static_cast<PathType>(i);

        std::string init_path = InitPath("init_all_%d");
        std::string path_string = format(init_path.c_str(), i);

        paths.SetPath(path, path_string.c_str());

        EXPECT_STREQ(path_string.c_str(), paths.GetPath(path).c_str());
    }
}

TEST(test_path_manager, path_format) {
    static const char* table[] = {
        "/vkconfig/test\\path/format/",  "/vkconfig/test\\path/format\\", "/vkconfig\\test/path/format/",
        "/vkconfig\\test/path\\format/", "/vkconfig\\test/path/format",   "/vkconfig/test/path/format",
        "\\vkconfig\\test/path\\format", "/vkconfig/test/path/format/",   "\\vkconfig\\test/path\\format\\"};

    for (std::size_t i = 0, n = countof(table); i < n; ++i) {
        PathManager paths("");
        paths.Clear();
        paths.SetPath(PATH_EXPORT_CONFIGURATION, QDir::homePath().toStdString() + table[i]);

        const std::string path(paths.GetPath(PATH_EXPORT_CONFIGURATION).c_str());
        const std::string home_path(GetPath(BUILTIN_PATH_HOME).c_str());

        if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
            EXPECT_STREQ((home_path + "\\vkconfig\\test\\path\\format").c_str(), path.c_str());
        } else {
            EXPECT_STREQ((home_path + "/vkconfig/test/path/format").c_str(), path.c_str());
        }
    }
}

// Test that export path is used as an alternative to import path when import path is empty
TEST(test_path_manager, empty_import) {
    PathManager paths("");
    paths.Clear();
    paths.SetPath(PATH_EXPORT_CONFIGURATION, InitPath("empty_import"));

    EXPECT_STRNE(paths.GetPath(PATH_IMPORT_CONFIGURATION).c_str(), GetPath(BUILTIN_PATH_HOME).c_str());
    EXPECT_STREQ(paths.GetPath(PATH_EXPORT_CONFIGURATION).c_str(), paths.GetPath(PATH_IMPORT_CONFIGURATION).c_str());
}

// Test that import path is used as an alternative to export path when export path is empty
TEST(test_path_manager, empty_export) {
    PathManager paths("");
    paths.Clear();
    paths.SetPath(PATH_IMPORT_CONFIGURATION, InitPath("empty_export"));

    EXPECT_STRNE(paths.GetPath(PATH_EXPORT_CONFIGURATION).c_str(), GetPath(BUILTIN_PATH_HOME).c_str());
    EXPECT_STREQ(paths.GetPath(PATH_IMPORT_CONFIGURATION).c_str(), paths.GetPath(PATH_EXPORT_CONFIGURATION).c_str());

    paths.Load();
}

TEST(test_path_manager, check_missing_dir) {
    PathManager paths("");
    paths.Clear();
    paths.SetPath(PATH_EXPORT_CONFIGURATION, InitPath("check_missing_dir"));

    EXPECT_TRUE(strstr(paths.GetPath(PATH_EXPORT_CONFIGURATION).c_str(), "check_missing_dir") != nullptr);

    paths.Load();
}

TEST(test_path_manager, check_default_suffix) {
    PathManager paths("");
    paths.Clear();

    const QString string = paths.GetFullPath(PATH_EXPORT_CONFIGURATION, "my_configuration").c_str();

    EXPECT_TRUE(string.endsWith("my_configuration.json"));

    paths.Load();
}

TEST(test_path_manager, check_with_suffix) {
    PathManager paths("");
    paths.Clear();

    const QString string = paths.GetFullPath(PATH_EXPORT_CONFIGURATION, "my_configuration.json").c_str();

    EXPECT_TRUE(string.endsWith("my_configuration.json"));

    paths.Load();
}
