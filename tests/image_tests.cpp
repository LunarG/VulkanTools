// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


//
// Copyright (C) 2015-2016 Valve Corporation
// Copyright (C) 2015-2016 LunarG, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
// Author: Jon Ashburn <jon@lunarg.com>
// Author: Tony Barbour <tony@LunarG.com>


// Verify VK driver initialization

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <vulkan/vulkan.h>
#include "vktestbinding.h"
#include "test_common.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

class VkImageTest : public ::testing::Test {
public:
    void CreateImage(uint32_t w, uint32_t h);
    void DestroyImage();

    void CreateImageView(VkImageViewCreateInfo* pCreateInfo,
                         VkImageView* pView);
    void DestroyImageView(VkImageView imageView);
    VkDevice device() {return m_device->handle();}

protected:
    vk_testing::Device *m_device;
    VkApplicationInfo app_info;
    VkPhysicalDevice objs[16];
    uint32_t gpu_count;
    VkInstance inst;
    VkImage m_image;
    VkDeviceMemory m_image_mem;

    virtual void SetUp() {
        VkResult err;

        this->app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        this->app_info.pNext = NULL;
        this->app_info.pApplicationName = "base";
        this->app_info.applicationVersion = 1;
        this->app_info.pEngineName = "unittest";
        this->app_info.engineVersion = 1;
        this->app_info.apiVersion = VK_API_VERSION_1_0;
        VkInstanceCreateInfo inst_info = {};
        inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        inst_info.pNext = NULL;
        inst_info.pApplicationInfo = &app_info;
        inst_info.enabledLayerCount = 0;
        inst_info.ppEnabledLayerNames = NULL;
        inst_info.enabledExtensionCount = 0;
        inst_info.ppEnabledExtensionNames = NULL;
        err = vkCreateInstance(&inst_info, NULL, &this->inst);
        ASSERT_VK_SUCCESS(err);
        err = vkEnumeratePhysicalDevices(this->inst, &this->gpu_count, NULL);
        ASSERT_VK_SUCCESS(err);
        ASSERT_LE(this->gpu_count, ARRAY_SIZE(objs)) << "Too many GPUs";
        err = vkEnumeratePhysicalDevices(this->inst, &this->gpu_count, objs);
        ASSERT_VK_SUCCESS(err);
        ASSERT_GE(this->gpu_count, (uint32_t) 1) << "No GPU available";

        this->m_device = new vk_testing::Device(objs[0]);
        this->m_device->init();
    }

    virtual void TearDown() {
        delete this->m_device;
        vkDestroyInstance(this->inst, NULL);
    }
};


void VkImageTest::CreateImage(uint32_t w, uint32_t h)
{
    VkResult err;
    bool pass;
    uint32_t mipCount;
    VkFormat fmt;
    VkFormatProperties image_fmt;

    mipCount = 0;

    uint32_t _w = w;
    uint32_t _h = h;
    while( ( _w > 0 ) || ( _h > 0 ) )
    {
        _w >>= 1;
        _h >>= 1;
        mipCount++;
    }

    fmt = VK_FORMAT_R8G8B8A8_UINT;
    // TODO: Pick known good format rather than just expect common format
    /*
     * XXX: What should happen if given NULL HANDLE for the pData argument?
     * We're not requesting VK_OBJECT_INFO_TYPE_MEMORY_REQUIREMENTS so there is
     * an expectation that pData is a valid pointer.
     * However, why include a returned size value? That implies that the
     * amount of data may vary and that doesn't work well for using a
     * fixed structure.
     */
    vkGetPhysicalDeviceFormatProperties(this->objs[0], fmt, &image_fmt);

    //    typedef struct VkImageCreateInfo_
    //    {
    //        VkStructureType                      sType;                      // Must be VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
    //        const void*                             pNext;                      // Pointer to next structure.
    //        VkImageType                          imageType;
    //        VkFormat                              format;
    //        VkExtent3D                            extent;
    //        uint32_t                                mipLevels;
    //        uint32_t                                arraySize;
    //        VkSampleCountFlagBits                   samples;
    //        VkImageTiling                        tiling;
    //        VkFlags                               usage;                      // VkImageUsageFlags
    //        VkFlags                               flags;                      // VkImageCreateFlags
    //    } VkImageCreateInfo;


    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = fmt;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.extent.width = w;
    imageCreateInfo.extent.height = h;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = mipCount;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    if ((image_fmt.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) &&
        (image_fmt.linearTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT))
    {
        imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
    }
    else if ((image_fmt.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) &&
        (image_fmt.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT))
    {
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    }
    else {
        ASSERT_TRUE(false) << "Cannot find supported tiling format - Exiting";
    }

    // Image usage flags
    //        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT                       = 0x00000008,
    //typedef enum VkImageUsageFlags_
    //{
    //    VK_IMAGE_USAGE_TRANSFER_SRC_BIT                      = 0x00000001,   // Can be used as a source of transfer operations
    //    VK_IMAGE_USAGE_TRANSFER_DST_BIT                 = 0x00000002,   // Can be used as a destination of transfer operations
    //    VK_IMAGE_USAGE_SAMPLED_BIT                              = 0x00000004,   // Can be sampled from (SAMPLED_IMAGE and COMBINED_IMAGE_SAMPLER descriptor types)
    //    VK_IMAGE_USAGE_STORAGE_BIT                              = 0x00000008,   // Can be used as storage image (STORAGE_IMAGE descriptor type)
    //    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                     = 0x00000010,   // Can be used as framebuffer color attachment
    //    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT                        = 0x00000020,   // Can be used as framebuffer depth/stencil attachment
    //    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT                 = 0x00000040,   // Image data not needed outside of rendering
    //    } VkImageUsageFlags;
    imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                            VK_IMAGE_USAGE_SAMPLED_BIT;

    //    VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(
    //        VkDevice                                  device,
    //        const VkImageCreateInfo*                pCreateInfo,
    //        VkImage*                                  pImage);
    err = vkCreateImage(device(), &imageCreateInfo, NULL, &m_image);
    ASSERT_VK_SUCCESS(err);

    VkMemoryRequirements mem_req;

    VkMemoryAllocateInfo mem_info = {};
    mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_info.pNext = NULL;

    vkGetImageMemoryRequirements(device(), m_image, &mem_req);

    ASSERT_NE(0u, mem_req.size) << "vkGetObjectMemoryRequirements (Image): Failed - expect images to require memory";
    mem_info.allocationSize = mem_req.size;
    mem_info.memoryTypeIndex = 0;

    pass = m_device->phy().set_memory_type(mem_req.memoryTypeBits, &mem_info, 0);
    ASSERT_TRUE(pass);

    /* allocate memory */
    err = vkAllocateMemory(device(), &mem_info, NULL, &m_image_mem);
    ASSERT_VK_SUCCESS(err);

    /* bind memory */
    err = vkBindImageMemory(device(), m_image, m_image_mem, 0);
    ASSERT_VK_SUCCESS(err);
}

void VkImageTest::DestroyImage()
{
    // All done with image object and memory, clean up
    vkDestroyImage(device(), m_image, NULL);
    vkFreeMemory(device(), m_image_mem, NULL);
}

void VkImageTest::CreateImageView(VkImageViewCreateInfo *pCreateInfo,
                                   VkImageView *pView)
{
    VkResult err;
    pCreateInfo->image = this->m_image;
    err = vkCreateImageView(device(), pCreateInfo, NULL, pView);
    ASSERT_VK_SUCCESS(err);
}

void VkImageTest::DestroyImageView(VkImageView imageView)
{
    vkDestroyImageView(device(), imageView, NULL);
}

TEST_F(VkImageTest, CreateImageViewTest) {
    VkFormat fmt;
    VkImageView imageView;

    fmt = VK_FORMAT_R8G8B8A8_UINT;

    CreateImage(512, 256);

    //    typedef struct VkImageViewCreateInfo_
    //    {
    //        VkStructureType                      sType;                  // Must be VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
    //        const void*                             pNext;                  // Pointer to next structure
    //        VkImage                               image;
    //        VkImageViewType                     viewType;
    //        VkFormat                              format;
    //        VkComponentMapping                     channels;
    //        VkImageSubresourceRange             subresourceRange;
    //        float                                   minLod;
    //    } VkImageViewCreateInfo;
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = fmt;

    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;

    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    //    VKAPI_ATTR VkResult VKAPI_CALL vkCreateImageView(
    //        VkDevice                                  device,
    //        const VkImageViewCreateInfo*           pCreateInfo,
    //        VkImageView*                             pView);

    CreateImageView(&viewInfo, &imageView);

    DestroyImageView(imageView);
    DestroyImage();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    vk_testing::set_error_callback(test_error_callback);
    return RUN_ALL_TESTS();
}
