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

#include <gtest/gtest.h>

#include "../path_manager.h"
#include "../environment.h"

TEST(test_environment, custom_path_no_duplicate) {
    PathManager paths;
    Environment environment(paths);

    environment.Reset(Environment::DEFAULT);

    EXPECT_EQ(true, environment.AppendCustomLayerPath("./path"));
    EXPECT_EQ(1, environment.GetCustomLayerPaths().size());
    EXPECT_EQ(false, environment.AppendCustomLayerPath("./path"));
    EXPECT_EQ(1, environment.GetCustomLayerPaths().size());

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings
}

TEST(test_environment, custom_path_not_found) {
    PathManager paths;
    Environment environment(paths);

    environment.Reset(Environment::DEFAULT);

    EXPECT_EQ(false, environment.RemoveCustomLayerPath("./path"));
    EXPECT_EQ(0, environment.GetCustomLayerPaths().size());
    EXPECT_EQ(true, environment.AppendCustomLayerPath("./path"));
    EXPECT_EQ(1, environment.GetCustomLayerPaths().size());
    EXPECT_EQ(true, environment.RemoveCustomLayerPath("./path"));
    EXPECT_EQ(0, environment.GetCustomLayerPaths().size());

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings
}
