/*
 * Copyright (c) 2023 The Khronos Group Inc.
 * Copyright (c) 2023 Valve Corporation
 * Copyright (c) 2023 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Author: Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <gtest/gtest.h>

// Can be used by tests to record additional details / description of test
#define TEST_DESCRIPTION(desc) RecordProperty("description", desc)

class VkTestFramework : public ::testing::Test {
   public:
    VkTestFramework();
    ~VkTestFramework() = 0;

    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestSuite();

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    static void TearDownTestSuite();
};
