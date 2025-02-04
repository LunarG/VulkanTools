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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../application_singleton.h"

#include <gtest/gtest.h>

TEST(test_application_singleton, single_server) {
    ApplicationSingleton a("test_application_singleton_single_server");
    ApplicationSingleton b("test_application_singleton_single_server");

    EXPECT_EQ(false, a.IsLocked());
    EXPECT_EQ(true, b.IsLocked());

    EXPECT_EQ(true, a.IsLocked());  // IsLocked was call once already
    EXPECT_EQ(true, b.IsLocked());
}

TEST(test_application_singleton, two_servers) {
    ApplicationSingleton a("test_application_singleton_0");
    ApplicationSingleton b("test_application_singleton_1");

    EXPECT_EQ(false, a.IsLocked());
    EXPECT_EQ(false, b.IsLocked());
}

TEST(test_application_singleton, multi_instance) {
    ApplicationSingleton a("test_application_singleton_multi_instance");
    ApplicationSingleton b("test_application_singleton_multi_instance");
    ApplicationSingleton c("test_application_singleton_multi_instance");
    ApplicationSingleton d("test_application_singleton_multi_instance");
    ApplicationSingleton e("test_application_singleton_multi_instance");

    EXPECT_EQ(false, a.IsLocked());
    EXPECT_EQ(true, b.IsLocked());
    EXPECT_EQ(true, c.IsLocked());
    EXPECT_EQ(true, d.IsLocked());
    EXPECT_EQ(true, e.IsLocked());
}

TEST(test_application_singleton, release_instance) {
    ApplicationSingleton a("test_application_singleton_release");
    ApplicationSingleton b("test_application_singleton_release");

    a.Release();

    EXPECT_EQ(false, a.IsLocked());
    EXPECT_EQ(true, b.IsLocked());

    a.Release();

    EXPECT_EQ(false, b.IsLocked());
    EXPECT_EQ(true, a.IsLocked());

    b.Release();

    EXPECT_EQ(false, a.IsLocked());
    EXPECT_EQ(true, b.IsLocked());

    b.Release();

    EXPECT_EQ(true, a.IsLocked());
}
