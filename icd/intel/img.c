/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Chia-I Wu <olvaffe@gmail.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */

#include "kmd/winsys.h"
#include "dev.h"
#include "gpu.h"
#include "wsi.h"
#include "img.h"

/*
 * From the Ivy Bridge PRM, volume 1 part 1, page 105:
 *
 *     "In addition to restrictions on maximum height, width, and depth,
 *      surfaces are also restricted to a maximum size in bytes. This
 *      maximum is 2 GB for all products and all surface types."
 */
static const size_t intel_max_resource_size = 1u << 31;

static void img_destroy(struct intel_obj *obj)
{
    struct intel_img *img = intel_img_from_obj(obj);

    intel_img_destroy(img);
}

static VkResult img_get_memory_requirements(struct intel_base *base, VkMemoryRequirements *pRequirements)
{
    struct intel_img *img = intel_img_from_base(base);

    pRequirements->size = img->total_size;
    pRequirements->alignment = 4096;
    pRequirements->memoryTypeBits = (1 << INTEL_MEMORY_TYPE_COUNT) - 1;

    return VK_SUCCESS;
}

VkResult intel_img_create(struct intel_dev *dev,
                          const VkImageCreateInfo *info,
                          const VkAllocationCallbacks *allocator,
                          bool scanout,
                          struct intel_img **img_ret)
{
    struct intel_img *img;
    struct intel_layout *layout;

    img = (struct intel_img *) intel_base_create(&dev->base.handle,
            sizeof(*img), dev->base.dbg, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, info, 0);
    if (!img)
        return VK_ERROR_OUT_OF_HOST_MEMORY;

    layout = &img->layout;

    img->type = info->imageType;
    img->depth = info->extent.depth;
    img->mip_levels = info->mipLevels;
    img->array_size = info->arrayLayers;
    img->usage = info->usage;
    img->sample_count = (uint32_t) info->samples;
    intel_layout_init(layout, dev, info, scanout);

    img->total_size = img->layout.bo_stride * img->layout.bo_height;

    if (layout->aux != INTEL_LAYOUT_AUX_NONE) {
        img->aux_offset = u_align(img->total_size, 4096);
        img->total_size = img->aux_offset +
            layout->aux_stride * layout->aux_height;
    }

    if (layout->separate_stencil) {
        VkImageCreateInfo s8_info;

        img->s8_layout = intel_alloc(img, sizeof(*img->s8_layout), sizeof(int),
                VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
        if (!img->s8_layout) {
            intel_img_destroy(img);
            return VK_ERROR_OUT_OF_HOST_MEMORY;
        }

        s8_info = *info;
        s8_info.format = VK_FORMAT_S8_UINT;
        /* no stencil texturing */
        s8_info.usage &= ~VK_IMAGE_USAGE_SAMPLED_BIT;
        assert(icd_format_is_ds(info->format));

        intel_layout_init(img->s8_layout, dev, &s8_info, scanout);

        img->s8_offset = u_align(img->total_size, 4096);
        img->total_size = img->s8_offset +
            img->s8_layout->bo_stride * img->s8_layout->bo_height;
    }

    if (scanout) {
        VkResult ret = intel_wsi_img_init(img);
        if (ret != VK_SUCCESS) {
            intel_img_destroy(img);
            return ret;
        }
    }

    img->obj.destroy = img_destroy;
    img->obj.base.get_memory_requirements = img_get_memory_requirements;

    *img_ret = img;

    return VK_SUCCESS;
}

void intel_img_destroy(struct intel_img *img)
{
    if (img->wsi_data)
        intel_wsi_img_cleanup(img);

    if (img->s8_layout)
        intel_free(img, img->s8_layout);

    intel_base_destroy(&img->obj.base);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(
    VkDevice                                  device,
    const VkImageCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                     pAllocator,
    VkImage*                                  pImage)
{
    struct intel_dev *dev = intel_dev(device);

    return intel_img_create(dev, pCreateInfo, pAllocator, false,
            (struct intel_img **) pImage);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyImage(
    VkDevice                                device,
    VkImage                                 image,
    const VkAllocationCallbacks*                     pAllocator)

 {
    struct intel_obj *obj = intel_obj(image);

    obj->destroy(obj);
 }

VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(
    VkDevice                                    device,
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout)
{
    const struct intel_img *img = intel_img(image);
    unsigned x, y;

    intel_layout_get_slice_pos(&img->layout, pSubresource->mipLevel,
                               pSubresource->arrayLayer, &x, &y);
    intel_layout_pos_to_mem(&img->layout, x, y, &x, &y);

    pLayout->offset = intel_layout_mem_to_linear(&img->layout, x, y);
    pLayout->size = intel_layout_get_slice_size(&img->layout,
                                               pSubresource->mipLevel);
    pLayout->rowPitch = img->layout.bo_stride;
    pLayout->depthPitch = intel_layout_get_slice_stride(&img->layout,
                                                       pSubresource->mipLevel);
    pLayout->arrayPitch = intel_layout_get_slice_stride(&img->layout,
                                                       pSubresource->arrayLayer);
}

VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    uint32_t*                                   pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements*            pSparseMemoryRequirements)
{
    *pSparseMemoryRequirementCount = 0;
}
