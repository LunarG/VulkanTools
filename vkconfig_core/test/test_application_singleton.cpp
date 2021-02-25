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

#include "../application_singleton.h"

#include <gtest/gtest.h>

TEST(test_application_singleton, single_server) {
    ApplicationSingleton a("test_application_singleton_single_server");
    ApplicationSingleton b("test_application_singleton_single_server");

    EXPECT_EQ(true, a.IsFirstInstance());
    EXPECT_EQ(false, b.IsFirstInstance());
}

TEST(test_application_singleton, two_servers) {
    ApplicationSingleton a("test_application_singleton_0");
    ApplicationSingleton b("test_application_singleton_1");

    EXPECT_EQ(true, a.IsFirstInstance());
    EXPECT_EQ(true, b.IsFirstInstance());
}

TEST(test_application_singleton, multi_instance) {
    ApplicationSingleton a("test_application_singleton_multi_instance");
    ApplicationSingleton b("test_application_singleton_multi_instance");
    ApplicationSingleton c("test_application_singleton_multi_instance");
    ApplicationSingleton d("test_application_singleton_multi_instance");
    ApplicationSingleton e("test_application_singleton_multi_instance");

    EXPECT_EQ(true, a.IsFirstInstance());
    EXPECT_EQ(false, b.IsFirstInstance());
    EXPECT_EQ(false, c.IsFirstInstance());
    EXPECT_EQ(false, d.IsFirstInstance());
    EXPECT_EQ(false, e.IsFirstInstance());
}
