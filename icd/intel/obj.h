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
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 *
 */

#ifndef OBJ_H
#define OBJ_H

#include "intel.h"

struct intel_dev;
struct intel_mem;

struct intel_base_dbg {
    VkDebugReportObjectTypeEXT type;

    void *create_info;
    size_t create_info_size;

    void *tag;
    size_t tag_size;
};

struct intel_base {
    struct intel_handle handle;

    struct intel_base_dbg *dbg;

    VkResult (*get_memory_requirements)(struct intel_base *base,
                         VkMemoryRequirements *data);
};

struct intel_obj {
    struct intel_base base;

    void (*destroy)(struct intel_obj *obj);

    /* for memory binding */
    struct intel_mem *mem;
    size_t offset;
};

static inline struct intel_base *intel_base(void * base)
{
    return (struct intel_base *) base;
}

static inline struct intel_obj *intel_obj(void * obj)
{
    return (struct intel_obj *) obj;
}

static inline void intel_obj_bind_mem(struct intel_obj *obj,
                                      struct intel_mem *mem,
                                      VkDeviceSize offset)
{
    obj->mem = mem;
    obj->offset = offset;
}

VkResult intel_base_get_memory_requirements(struct intel_base *base, VkMemoryRequirements *data);

struct intel_base_dbg *intel_base_dbg_create(const struct intel_handle *handle,
                                             VkDebugReportObjectTypeEXT type,
                                             const void *create_info,
                                             size_t dbg_size);
void intel_base_dbg_destroy(const struct intel_handle *handle,
                            struct intel_base_dbg *dbg);

struct intel_base *intel_base_create(const struct intel_handle *handle,
                                     size_t obj_size, bool debug,
                                     VkDebugReportObjectTypeEXT type,
                                     const void *create_info,
                                     size_t dbg_size);
void intel_base_destroy(struct intel_base *base);

#endif /* OBJ_H */
