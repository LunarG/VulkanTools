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

#include "../layer_manager.h"

#include <gtest/gtest.h>

TEST(test_layer_manager, load_only_layer_json) {
    PathManager paths("");
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);

    LayerManager layer_manager(environment);
    layer_manager.LoadLayersFromPath(":/");

    EXPECT_EQ(10, layer_manager.available_layers.size());

    environment.Reset(Environment::SYSTEM);  // Don't change the system settings on exit
}
