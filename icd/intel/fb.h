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
 * Author: Chia-I Wu <olv@lunarg.com>
 * Author: Chris Forbes <chrisf@ijw.co.nz>
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */

#ifndef FB_H
#define FB_H

#include "intel.h"
#include "obj.h"

struct intel_fb {
    struct intel_obj obj;

    const struct intel_att_view **views;
    uint32_t view_count;

    uint32_t width;
    uint32_t height;
    uint32_t array_size;
};

struct intel_render_pass_attachment {
    VkFormat format;
    uint32_t sample_count;

    VkImageLayout initial_layout;
    VkImageLayout final_layout;

    bool clear_on_load;
    bool disable_store;

    bool stencil_clear_on_load;
    bool stencil_disable_store;
};

struct intel_render_pass_subpass {
    uint32_t color_indices[INTEL_MAX_RENDER_TARGETS];
    uint32_t resolve_indices[INTEL_MAX_RENDER_TARGETS];
    VkImageLayout color_layouts[INTEL_MAX_RENDER_TARGETS];
    uint32_t color_count;

    uint32_t ds_index;
    VkImageLayout ds_layout;
    bool ds_optimal;
};

struct intel_render_pass {
    struct intel_obj obj;

    struct intel_render_pass_attachment *attachments;
    uint32_t attachment_count;

    struct intel_render_pass_subpass *subpasses;
    uint32_t subpass_count;
};

static inline struct intel_fb *intel_fb(VkFramebuffer fb)
{
    return *(struct intel_fb **) &fb;
}

static inline struct intel_fb *intel_fb_from_obj(struct intel_obj *obj)
{
    return (struct intel_fb *) obj;
}

static inline struct intel_render_pass *intel_render_pass(VkRenderPass rp)
{
    return *(struct intel_render_pass **) &rp;
}

static inline struct intel_render_pass *intel_render_pass_from_obj(struct intel_obj *obj)
{
    return (struct intel_render_pass *) obj;
}

VkResult intel_fb_create(struct intel_dev *dev,
                         const VkFramebufferCreateInfo *pInfo,
                         const VkAllocationCallbacks *allocator,
                         struct intel_fb **fb_ret);
void intel_fb_destroy(struct intel_fb *fb);

VkResult intel_render_pass_create(struct intel_dev *dev,
                                  const VkRenderPassCreateInfo *pInfo,
                                  const VkAllocationCallbacks *allocator,
                                  struct intel_render_pass **rp_ret);
void intel_render_pass_destroy(struct intel_render_pass *rp);

#endif /* FB_H */
