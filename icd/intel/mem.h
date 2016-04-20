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

#ifndef MEM_H
#define MEM_H

#include "kmd/winsys.h"
#include "intel.h"
#include "obj.h"

struct intel_mem {
    struct intel_base base;

    struct intel_bo *bo;
    VkDeviceSize size;
};

VkResult intel_mem_alloc(struct intel_dev *dev,
                           const VkMemoryAllocateInfo *info,
                           struct intel_mem **mem_ret);
void intel_mem_free(struct intel_mem *mem);

static inline void *intel_mem_map(struct intel_mem *mem, VkFlags flags)
{
    return intel_bo_map_async(mem->bo);
}

static inline void *intel_mem_map_sync(struct intel_mem *mem, bool rw)
{
    return intel_bo_map(mem->bo, rw);
}

static inline void intel_mem_unmap(struct intel_mem *mem)
{
    intel_bo_unmap(mem->bo);
}

static inline bool intel_mem_is_busy(struct intel_mem *mem)
{
    return intel_bo_is_busy(mem->bo);
}

static inline struct intel_mem *intel_mem(VkDeviceMemory mem)
{
    return *(struct intel_mem **) &mem;
}

#endif /* MEM_H */
