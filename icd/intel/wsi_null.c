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
 *
 */

#include "wsi.h"

void intel_wsi_gpu_cleanup(struct intel_gpu *gpu)
{
}

VkResult intel_wsi_img_init(struct intel_img *img)
{
    return VK_SUCCESS;
}

void intel_wsi_img_cleanup(struct intel_img *img)
{
}

VkResult intel_wsi_fence_init(struct intel_fence *fence)
{
    return VK_SUCCESS;
}

void intel_wsi_fence_cleanup(struct intel_fence *fence)
{
}

void intel_wsi_fence_copy(struct intel_fence *fence,
                          const struct intel_fence *src)
{
}

VkResult intel_wsi_fence_wait(struct intel_fence *fence,
                              int64_t timeout_ns)
{
    return VK_SUCCESS;
}
