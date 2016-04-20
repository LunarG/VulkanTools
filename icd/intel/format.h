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
 * Author: Jeremy Hayes <jeremy@lunarg.com>
 *
 */

#ifndef FORMAT_H
#define FORMAT_H

#include "intel.h"

struct intel_gpu;

static inline bool intel_format_has_depth(const struct intel_gpu *gpu,
                                          VkFormat format)
{
    bool has_depth = false;

    switch (format) {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D32_SFLOAT:
    /* VK_FORMAT_D16_UNORM_S8_UINT is unsupported */
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        has_depth = true;
        break;
    default:
        break;
    }

    return has_depth;
}

int intel_format_translate_color(const struct intel_gpu *gpu,
                                 VkFormat format);

#endif /* FORMAT_H */
