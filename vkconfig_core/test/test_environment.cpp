/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "../environment.h"
#include "../type_platform.h"

#include <QFile>

#include <gtest/gtest.h>

TEST(test_environment, reset_default_applications_sdk_found) {
    Environment environment(Environment::MODE_UNINITIALIZED);
    environment.Reset(Environment::DEFAULT);

    const std::vector<Application>& applications = environment.GetApplications();

    std::string result = qgetenv("VULKAN_SDK").toStdString();

    if (!result.empty()) {
        EXPECT_EQ(2, applications.size());

        // Make sure the variable are not replaced
        EXPECT_TRUE(applications[0].executable_path.RelativePath().find("${VULKAN_SDK}") != std::string::npos);
        EXPECT_TRUE(applications[0].options[0].working_folder.RelativePath().find("${VULKAN_SDK}") != std::string::npos);
        EXPECT_TRUE(applications[0].options[0].log_file.RelativePath().find("${VK_LOCAL}") != std::string::npos);
        EXPECT_TRUE(applications[1].executable_path.RelativePath().find("${VULKAN_SDK}") != std::string::npos);
        EXPECT_TRUE(applications[1].options[0].working_folder.RelativePath().find("${VULKAN_SDK}") != std::string::npos);
        EXPECT_TRUE(applications[1].options[0].log_file.RelativePath().find("${VK_LOCAL}") != std::string::npos);
    }

    environment.Reset(Environment::SYSTEM);
}

TEST(test_environment, reset_default_applications_no_sdk) {
    qunsetenv("VULKAN_SDK");

    Environment environment(Environment::MODE_UNINITIALIZED);
    environment.Reset(Environment::DEFAULT);

    const std::vector<Application>& applications = environment.GetApplications();

    EXPECT_EQ(2, applications.size());

    // Make sure the variable are not replaced
    EXPECT_TRUE(applications[0].executable_path.RelativePath().find("vkcube") != std::string::npos);
    EXPECT_TRUE(applications[0].options[0].working_folder.RelativePath().find(".") != std::string::npos);
    EXPECT_TRUE(applications[0].options[0].log_file.RelativePath().find("${VK_LOCAL}") != std::string::npos);
    EXPECT_TRUE(applications[1].executable_path.RelativePath().find("vkcubepp") != std::string::npos);
    EXPECT_TRUE(applications[1].options[0].working_folder.RelativePath().find(".") != std::string::npos);
    EXPECT_TRUE(applications[1].options[0].log_file.RelativePath().find("${VK_LOCAL}") != std::string::npos);

    environment.Reset(Environment::SYSTEM);
}

TEST(test_environment, remove_missing_applications) {
    Environment environment(Environment::MODE_UNINITIALIZED);

    const Path& path_missing = ::Get(Path::HOME) + "my_missing_executable";
    const Path& path_exciting = ::Get(Path::HOME) + "my_exciting_executable";

    QFile file(path_exciting.AbsolutePath().c_str());
    const bool result = file.open(QIODevice::WriteOnly);
    ASSERT_TRUE(result);

    std::vector<Application> applications;
    Application application;

    application.executable_path = path_missing.AbsolutePath();
    applications.push_back(application);

    application.executable_path = path_exciting.AbsolutePath();
    applications.push_back(application);

    EXPECT_EQ(1, environment.RemoveMissingApplications(applications).size());
}
