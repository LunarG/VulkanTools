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

#include "../path.h"
#include "../util.h"
#include "../platform.h"

#include <array>

#include <QDir>

#include <gtest/gtest.h>

TEST(test_util, validate_path) {
    const std::string validated_path = ValidatePath("/waotsra/aflkshjaksl/test.txt");

    const QString ref_path(ConvertNativeSeparators(QDir().homePath().toStdString() + "/vulkan-sdk/test.txt").c_str());

    EXPECT_STREQ(ref_path.toStdString().c_str(), validated_path.c_str());
}

TEST(test_util, replace_path) {
    const std::string replaced_path = ReplacePathBuiltInVariables("$HOME/test.txt");

    EXPECT_TRUE(replaced_path.find_first_of("$HOME") > replaced_path.size());
}

TEST(test_util, native_path) {
    static const char* table[] = {
        "/vkconfig/test\\path/format/",  "/vkconfig/test\\path/format\\", "/vkconfig\\test/path/format/",
        "/vkconfig\\test/path\\format/", "/vkconfig\\test/path/format",   "/vkconfig/test/path/format",
        "\\vkconfig\\test/path\\format", "/vkconfig/test/path/format/",   "\\vkconfig\\test/path\\format\\"};

    for (std::size_t i = 0, n = countof(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format", ConvertNativeSeparators(test_case).c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format", ConvertNativeSeparators(test_case).c_str());
        }
    }
}

TEST(test_util, native_path_with_file) {
    static const char* table[] = {"/vkconfig/test/path/format/file.txt", "\\vkconfig\\test/path\\format\\file.txt"};

    for (std::size_t i = 0, n = countof(table); i < n; ++i) {
        const std::string test_case = table[i];

        if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
            EXPECT_STREQ("\\vkconfig\\test\\path\\format\\file.txt", ConvertNativeSeparators(test_case).c_str());
        } else {
            EXPECT_STREQ("/vkconfig/test/path/format/file.txt", ConvertNativeSeparators(test_case).c_str());
        }
    }
}
