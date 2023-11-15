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

#include "layer_test_helper.h"

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_beta.h>

#include <gtest/gtest.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>

static const char* kLayerName = "VK_LAYER_LUNARG_api_dump";

class ApiDumpTests : public VkTestFramework {
   public:
    ~ApiDumpTests(){};

    static void SetUpTestSuite() {}
    static void TearDownTestSuite(){};
};

TEST_F(ApiDumpTests, init_layer) {
    TEST_DESCRIPTION("Test Creating a Vulkan Instance with a layer");

    VkBool32 use_file = VK_TRUE;
    const char* filename_string = "api_dump_output.html";
    const char* output_format = "html";

    const std::vector<VkLayerSettingEXT> settings = {
        {kLayerName, "file", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1, &use_file},
        {kLayerName, "log_filename", VK_LAYER_SETTING_TYPE_STRING_EXT, 1, &filename_string},
        {kLayerName, "output_format", VK_LAYER_SETTING_TYPE_STRING_EXT, 1, &output_format}};

    layer_test::VulkanInstanceBuilder inst_builder;
    VkResult err = inst_builder.Init(settings);
    EXPECT_EQ(err, VK_SUCCESS);

    // check the output file is generated
    const std::string path = std::string(TEST_BINARY_PATH) + "/test/" + filename_string;
    FILE* file = fopen(path.c_str(), "r");
    ASSERT_TRUE(file != NULL);

    const char* file_start_content_expected = "<!doctype html>";
    std::string file_start_content_read;
    file_start_content_read.resize(std::strlen(file_start_content_expected));

    fread(&file_start_content_read[0], 1, file_start_content_read.size(), file);
    fclose(file);

    EXPECT_STREQ(file_start_content_read.c_str(), file_start_content_expected);
}
