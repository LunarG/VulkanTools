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
 * Author: Jon Ashburn <jon@lunarg.com>
 *
 */

#include "dev.h"
#include "obj.h"
#include "buf.h"

static void buf_destroy(struct intel_obj *obj)
{
    struct intel_buf *buf = intel_buf_from_obj(obj);

    intel_buf_destroy(buf);
}

static VkResult buf_get_memory_requirements(struct intel_base *base,
                               VkMemoryRequirements *pRequirements)
{
    struct intel_buf *buf = intel_buf_from_base(base);

    /*
     * From the Sandy Bridge PRM, volume 1 part 1, page 118:
     *
     *     "For buffers, which have no inherent "height," padding
     *      requirements are different. A buffer must be padded to the
     *      next multiple of 256 array elements, with an additional 16
     *      bytes added beyond that to account for the L1 cache line."
    */
    pRequirements->size = buf->size;
    if (buf->usage & (VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT |
                      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)) {
        pRequirements->size = u_align(pRequirements->size, 256) + 16;
    }

    pRequirements->alignment      = 4096;
    pRequirements->memoryTypeBits = (1 << INTEL_MEMORY_TYPE_COUNT) - 1;

    return VK_SUCCESS;
}

VkResult intel_buf_create(struct intel_dev *dev,
                            const VkBufferCreateInfo *info,
                            struct intel_buf **buf_ret)
{
    struct intel_buf *buf;

    buf = (struct intel_buf *) intel_base_create(&dev->base.handle,
            sizeof(*buf), dev->base.dbg, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, info, 0);
    if (!buf)
        return VK_ERROR_OUT_OF_HOST_MEMORY;

    buf->size = info->size;
    buf->usage = info->usage;

    buf->obj.destroy = buf_destroy;
    buf->obj.base.get_memory_requirements = buf_get_memory_requirements;

    *buf_ret = buf;

    return VK_SUCCESS;
}

void intel_buf_destroy(struct intel_buf *buf)
{
    intel_base_destroy(&buf->obj.base);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateBuffer(
    VkDevice                                  device,
    const VkBufferCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                     pAllocator,
    VkBuffer*                                 pBuffer)
{
    struct intel_dev *dev = intel_dev(device);

    return intel_buf_create(dev, pCreateInfo, (struct intel_buf **) pBuffer);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyBuffer(
    VkDevice                                device,
    VkBuffer                                buffer,
    const VkAllocationCallbacks*                     pAllocator)
{
    struct intel_obj *obj = intel_obj(buffer);

    obj->destroy(obj);
}
