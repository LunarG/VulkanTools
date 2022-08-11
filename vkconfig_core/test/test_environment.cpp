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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../path_manager.h"
#include "../environment.h"

#include <QFile>

#include <gtest/gtest.h>

TEST(test_environment, remove_missing_applications) {
    PathManager path_manager("");
    Environment environment(path_manager);

    QFile file("my_exciting_executable");
    const bool result = file.open(QIODevice::WriteOnly);
    ASSERT_TRUE(result);

    std::vector<Application> applications;
    applications.push_back(Application("missing", "my_missing_executable", ""));
    applications.push_back(Application("exciting", "my_exciting_executable", ""));

    EXPECT_EQ(1, environment.RemoveMissingApplications(applications).size());
}
