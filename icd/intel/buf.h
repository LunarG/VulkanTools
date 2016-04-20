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

#ifndef BUF_H
#define BUF_H

#include "intel.h"
#include "obj.h"

struct intel_buf {
    struct intel_obj obj;

    VkDeviceSize size;
    VkFlags usage;
};

static inline struct intel_buf *intel_buf(VkBuffer buf)
{
    return *(struct intel_buf **) &buf;
}

static inline struct intel_buf *intel_buf_from_base(struct intel_base *base)
{
    return (struct intel_buf *) base;
}

static inline struct intel_buf *intel_buf_from_obj(struct intel_obj *obj)
{
    return intel_buf_from_base(&obj->base);
}

VkResult intel_buf_create(struct intel_dev *dev,
                            const VkBufferCreateInfo *info,
                            struct intel_buf **buf_ret);

void intel_buf_destroy(struct intel_buf *buf);

#endif /* BUF_H */
