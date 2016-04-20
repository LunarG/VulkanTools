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

#ifndef VIEW_H
#define VIEW_H

#include "obj.h"
#include "intel.h"

struct intel_img;
struct intel_mem;

struct intel_null_view {
    /* this is not an intel_obj */

    /* SURFACE_STATE */
    uint32_t cmd[8];
    uint32_t cmd_len;
};

struct intel_buf_view {
    struct intel_obj obj;

    struct intel_buf *buf;

    /* SURFACE_STATE */
    uint32_t cmd[8];
    uint32_t fs_cmd[8];
    uint32_t cmd_len;
};

struct intel_att_view {
    struct intel_img *img;

    uint32_t mipLevel;
    uint32_t baseArrayLayer;
    uint32_t array_size;

    /* SURFACE_STATE for readback */
    uint32_t cmd[8];
    uint32_t cmd_len;

    /*
     * SURFACE_STATE when is_rt is true.  Otherwise,
     *
     * 3DSTATE_DEPTH_BUFFER
     * 3DSTATE_STENCIL_BUFFER
     * 3DSTATE_HIER_DEPTH_BUFFER
     */
    uint32_t att_cmd[10];
    bool is_rt;
    bool has_stencil;
    bool has_hiz;
};

struct intel_img_view {
    struct intel_obj obj;

    struct intel_img *img;

    VkComponentMapping shader_swizzles;

    /* SURFACE_STATE */
    uint32_t cmd[8];
    uint32_t cmd_len;

    struct intel_att_view att_view;
};

static inline struct intel_buf_view *intel_buf_view(VkBufferView view)
{
    return *(struct intel_buf_view **) &view;
}

static inline struct intel_buf_view *intel_buf_view_from_obj(struct intel_obj *obj)
{
    return (struct intel_buf_view *) obj;
}

static inline struct intel_img_view *intel_img_view(VkImageView view)
{
    return *(struct intel_img_view **) &view;
}

static inline struct intel_img_view *intel_img_view_from_obj(struct intel_obj *obj)
{
    return (struct intel_img_view *) obj;
}

void intel_null_view_init(struct intel_null_view *view,
                          struct intel_dev *dev);

void intel_buf_view_init(const struct intel_dev *dev,
                         const VkBufferViewCreateInfo *info,
                         struct intel_buf_view *view,
                         bool raw);

VkResult intel_buf_view_create(struct intel_dev *dev,
                               const VkBufferViewCreateInfo *info,
                               struct intel_buf_view **view_ret);

void intel_buf_view_destroy(struct intel_buf_view *view);

void intel_img_view_init(struct intel_dev *dev, const VkImageViewCreateInfo *info,
                         struct intel_img_view *view);

VkResult intel_img_view_create(struct intel_dev *dev,
                                 const VkImageViewCreateInfo *info,
                                 struct intel_img_view **view_ret);
void intel_img_view_destroy(struct intel_img_view *view);

void intel_att_view_init(struct intel_dev *dev,
                         const VkImageViewCreateInfo *info,
                         struct intel_att_view *att_view);
void intel_att_view_destroy(struct intel_att_view *view);

#endif /* VIEW_H */
