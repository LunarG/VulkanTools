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

#ifndef FENCE_H
#define FENCE_H

#include "intel.h"
#include "obj.h"

struct intel_bo;
struct intel_dev;

struct intel_fence {
    struct intel_obj obj;

    struct intel_bo *seqno_bo;
    bool    signaled;

    void *wsi_data;
};

static inline struct intel_fence *intel_fence(VkFence fence)
{
    return *(struct intel_fence **) &fence;
}

static inline struct intel_fence *intel_fence_from_obj(struct intel_obj *obj)
{
    return (struct intel_fence *) obj;
}

VkResult intel_fence_create(struct intel_dev *dev,
                              const VkFenceCreateInfo *info,
                              struct intel_fence **fence_ret);
void intel_fence_destroy(struct intel_fence *fence);

VkResult intel_fence_wait(struct intel_fence *fence, int64_t timeout_ns);

void intel_fence_copy(struct intel_fence *fence,
                      const struct intel_fence *src);

void intel_fence_set_seqno(struct intel_fence *fence,
                           struct intel_bo *seqno_bo);

#endif /* FENCE_H */
