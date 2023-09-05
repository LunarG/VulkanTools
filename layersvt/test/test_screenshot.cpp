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

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_beta.h>

#include <gtest/gtest.h>
#include "layer_test_helper.h"

#include <cstdarg>

static const char* kLayerName = "VK_LAYER_LUNARG_screenshot";

class ScreenshotTests : public VkTestFramework {
   public:
    ~ScreenshotTests(){};

    static void SetUpTestSuite() {}
    static void TearDownTestSuite(){};
};

TEST_F(ScreenshotTests, init_layer) {
    TEST_DESCRIPTION("Test Creating a Vulkan Instance with a layer");

    VkBool32 file = VK_TRUE;
    const char* frames = "8-2";
    const char* format = "UNORM";

    const std::vector<VkLayerSettingEXT> settings = {{kLayerName, "frames", VK_LAYER_SETTING_TYPE_STRING_EXT, 1, &frames},
                                                     {kLayerName, "format", VK_LAYER_SETTING_TYPE_STRING_EXT, 1, &format}};

    layer_test::VulkanInstanceBuilder inst_builder;
    VkResult err = inst_builder.Init(settings);
    EXPECT_EQ(err, VK_SUCCESS);
}
