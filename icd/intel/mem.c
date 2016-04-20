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
 * Author: Mike Stroyan <mike@LunarG.com>
 * Author: Tony Barbour <tony@LunarG.com>
 *
 */

#include "dev.h"
#include "mem.h"

VkResult intel_mem_alloc(struct intel_dev *dev,
                           const VkMemoryAllocateInfo *info,
                           struct intel_mem **mem_ret)
{
    struct intel_mem *mem;

    /* ignore any IMAGE_INFO and BUFFER_INFO usage: they don't alter allocations */

    mem = (struct intel_mem *) intel_base_create(&dev->base.handle,
            sizeof(*mem), dev->base.dbg, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, info, 0);
    if (!mem)
        return VK_ERROR_OUT_OF_HOST_MEMORY;

    mem->bo = intel_winsys_alloc_bo(dev->winsys,
            "vk-gpu-memory", info->allocationSize, 0);
    if (!mem->bo) {
        intel_mem_free(mem);
        return VK_ERROR_OUT_OF_DEVICE_MEMORY;
    }

    mem->size = info->allocationSize;

    *mem_ret = mem;

    return VK_SUCCESS;
}

void intel_mem_free(struct intel_mem *mem)
{
    intel_bo_unref(mem->bo);

    intel_base_destroy(&mem->base);
}

VKAPI_ATTR VkResult VKAPI_CALL vkAllocateMemory(
    VkDevice                                device,
    const VkMemoryAllocateInfo*                pAllocateInfo,
    const VkAllocationCallbacks*                     pAllocator,
    VkDeviceMemory*                         pMemory)
{
    struct intel_dev *dev = intel_dev(device);

    return intel_mem_alloc(dev, pAllocateInfo, (struct intel_mem **) pMemory);
}

VKAPI_ATTR void VKAPI_CALL vkFreeMemory(
    VkDevice                                  device,
    VkDeviceMemory                            mem_,
    const VkAllocationCallbacks*                     pAllocator)
{
    struct intel_mem *mem = intel_mem(mem_);

    intel_mem_free(mem);
}

VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory(
    VkDevice                                  device,
    VkDeviceMemory                            mem_,
    VkDeviceSize                              offset,
    VkDeviceSize                              size,
    VkFlags                                   flags,
    void**                                    ppData)
{
    struct intel_mem *mem = intel_mem(mem_);
    void *ptr = intel_mem_map(mem, flags);

    *ppData = (void *)((size_t)ptr + offset);

    return (ptr) ? VK_SUCCESS : VK_ERROR_MEMORY_MAP_FAILED;
}

VKAPI_ATTR void VKAPI_CALL vkUnmapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              mem_)
{
    struct intel_mem *mem = intel_mem(mem_);

    intel_mem_unmap(mem);
}

VKAPI_ATTR VkResult VKAPI_CALL vkFlushMappedMemoryRanges(
    VkDevice                                  device,
    uint32_t                                  memoryRangeCount,
    const VkMappedMemoryRange*                pMemoryRanges)
{
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(
    VkDevice                                  device,
    uint32_t                                  memoryRangeCount,
    const VkMappedMemoryRange*                pMemoryRanges)
{
    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(
    VkDevice                                  device,
    VkDeviceMemory                            memory,
    VkDeviceSize*                             pCommittedMemoryInBytes)
{
}
