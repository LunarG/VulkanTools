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
 *
 */

#ifndef IMG_H
#define IMG_H

#include "kmd/winsys.h"
#include "intel.h"
#include "layout.h"
#include "obj.h"

struct intel_img {
    struct intel_obj obj;

    VkImageType type;
    int32_t depth;
    uint32_t mip_levels;
    uint32_t array_size;
    VkFlags usage;
    uint32_t sample_count;
    struct intel_layout layout;

    /* layout of separate stencil */
    struct intel_layout *s8_layout;

    size_t total_size;
    size_t aux_offset;
    size_t s8_offset;

    void *wsi_data;
};

static inline struct intel_img *intel_img(VkImage image)
{
    return *(struct intel_img **) &image;
}

static inline struct intel_img *intel_img_from_base(struct intel_base *base)
{
    return (struct intel_img *) base;
}

static inline struct intel_img *intel_img_from_obj(struct intel_obj *obj)
{
    return intel_img_from_base(&obj->base);
}

VkResult intel_img_create(struct intel_dev *dev,
                          const VkImageCreateInfo *info,
                          const VkAllocationCallbacks *allocator,
                          bool scanout,
                          struct intel_img **img_ret);

void intel_img_destroy(struct intel_img *img);

static inline bool intel_img_can_enable_hiz(const struct intel_img *img,
                                            uint32_t level)
{
    return (img->layout.aux == INTEL_LAYOUT_AUX_HIZ &&
            img->layout.aux_enables & (1 << level));
}

#endif /* IMG_H */
