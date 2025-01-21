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

#include "../configurator.h"

#include <gtest/gtest.h>

TEST(test_configurator, get) {
    Configurator& configurator = Configurator::Get();

    EXPECT_EQ(configurator.has_crashed, false);
    EXPECT_EQ(configurator.active_tab, TAB_CONFIGURATIONS);
    EXPECT_FALSE(configurator.GetUseSystemTray());
    EXPECT_EQ(configurator.GetExecutableScope(), EXECUTABLE_ANY);
}

TEST(test_configurator, init) {
    Configurator& configurator = Configurator::Get();
    configurator.Init();

    EXPECT_TRUE(configurator.layers.Size() > 0);
    EXPECT_TRUE(configurator.configurations.Size() > 0);
}
