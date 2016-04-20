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

#ifndef QUERY_H
#define QUERY_H

#include "intel.h"
#include "obj.h"

struct intel_query {
    struct intel_obj obj;

    VkQueryType type;
    uint32_t reg_count;
    uint32_t slot_stride;
    uint32_t slot_count;
    uint32_t regs[32];
};

static inline struct intel_query *intel_query(VkQueryPool pool)
{
    return *(struct intel_query **) &pool;
}

static inline struct intel_query *intel_query_from_base(struct intel_base *base)
{
    return (struct intel_query *) base;
}

static inline struct intel_query *intel_query_from_obj(struct intel_obj *obj)
{
    return intel_query_from_base(&obj->base);
}

VkResult intel_query_create(struct intel_dev *dev,
                              const VkQueryPoolCreateInfo *info,
                              struct intel_query **query_ret);
void intel_query_destroy(struct intel_query *query);

VkResult intel_query_get_results(struct intel_query *query,
                                   uint32_t slot_start, uint32_t slot_count,
                                   void *results);

#endif /* QUERY_H */
