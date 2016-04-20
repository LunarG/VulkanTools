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
 * Author: Chia-I Wu <olv@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 *
 */

#ifndef ICD_FORMAT_H
#define ICD_FORMAT_H

#include <stdbool.h>
#include "icd.h"

static inline bool icd_format_is_undef(VkFormat format)
{
    return (format == VK_FORMAT_UNDEFINED);
}

bool icd_format_is_ds(VkFormat format);

static inline bool icd_format_is_color(VkFormat format)
{
    return !(icd_format_is_undef(format) || icd_format_is_ds(format));
}

bool icd_format_is_norm(VkFormat format);

bool icd_format_is_int(VkFormat format);

bool icd_format_is_float(VkFormat format);

bool icd_format_is_srgb(VkFormat format);

bool icd_format_is_compressed(VkFormat format);

static inline int icd_format_get_block_width(VkFormat format)
{
    /* all compressed formats use 4x4 blocks */
    return (icd_format_is_compressed(format)) ? 4 : 1;
}

static inline bool icd_blend_mode_is_dual_src(VkBlendFactor mode)
{
    return (mode == VK_BLEND_FACTOR_SRC1_COLOR) ||
           (mode == VK_BLEND_FACTOR_SRC1_ALPHA) ||
           (mode == VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR) ||
           (mode == VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA);
}

static inline bool icd_pipeline_cb_att_needs_dual_source_blending(const VkPipelineColorBlendAttachmentState *att)
{
    if (icd_blend_mode_is_dual_src(att->srcColorBlendFactor) ||
        icd_blend_mode_is_dual_src(att->srcAlphaBlendFactor) ||
        icd_blend_mode_is_dual_src(att->dstColorBlendFactor) ||
        icd_blend_mode_is_dual_src(att->dstAlphaBlendFactor)) {
        return true;
    }
    return false;
}

size_t icd_format_get_size(VkFormat format);

unsigned int icd_format_get_channel_count(VkFormat format);

void icd_format_get_raw_value(VkFormat format,
                              const uint32_t color[4],
                              void *value);

#endif /* ICD_FORMAT_H */
