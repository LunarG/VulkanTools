/*
* Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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
*/
#include "vktrace_trace_packet_identifiers.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_lib_helpers.h"
#include "vktrace_vk_vk_packets.h"
#include "vktrace_vk_packet_id.h"
#include "vulkan.h"

// defined in vktrace_lib_trace.cpp
extern layer_device_data *mdd(void *object);
extern layer_instance_data *mid(void *object);

namespace trim {
//=============================================================================
// Helpers to generate trace packets
//=============================================================================
namespace generate {

vktrace_trace_packet_header *vkGetPhysicalDeviceSurfacePresentModesKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                       VkSurfaceKHR surface, uint32_t *pPresentModeCount,
                                                                       VkPresentModeKHR *pPresentModes) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    size_t _dataSize;
    packet_vkGetPhysicalDeviceSurfacePresentModesKHR *pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    if (makeCall) {
        result = mid(physicalDevice)
                     ->instTable.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    }
    endTime = vktrace_get_time();
    _dataSize = (pPresentModeCount == NULL || pPresentModes == NULL) ? 0 : (*pPresentModeCount * sizeof(VkPresentModeKHR));
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfacePresentModesKHR, sizeof(uint32_t) + _dataSize);
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfacePresentModesKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pPresentModeCount), sizeof(uint32_t), pPresentModeCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pPresentModes), _dataSize, pPresentModes);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pPresentModeCount));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pPresentModes));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header *vkGetPhysicalDeviceSurfaceSupportKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                  uint32_t queueFamilyIndex, VkSurfaceKHR surface,
                                                                  VkBool32 *pSupported) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkGetPhysicalDeviceSurfaceSupportKHR *pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfaceSupportKHR, sizeof(VkBool32));
    if (makeCall) {
        result = mid(physicalDevice)
                     ->instTable.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfaceSupportKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->queueFamilyIndex = queueFamilyIndex;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSupported), sizeof(VkBool32), pSupported);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSupported));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header *vkGetPhysicalDeviceSurfaceCapabilitiesKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                       VkSurfaceKHR surface,
                                                                       VkSurfaceCapabilitiesKHR *pSurfaceCapabilities) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkGetPhysicalDeviceSurfaceCapabilitiesKHR *pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfaceCapabilitiesKHR, sizeof(VkSurfaceCapabilitiesKHR));
    if (makeCall) {
        result =
            mid(physicalDevice)->instTable.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    }
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSurfaceCapabilities), sizeof(VkSurfaceCapabilitiesKHR),
                                       pSurfaceCapabilities);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSurfaceCapabilities));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header *vkGetPhysicalDeviceSurfaceFormatsKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                  VkSurfaceKHR surface, uint32_t *pSurfaceFormatCount,
                                                                  VkSurfaceFormatKHR *pSurfaceFormats) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    size_t _dataSize;
    packet_vkGetPhysicalDeviceSurfaceFormatsKHR *pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    if (makeCall) {
        result = mid(physicalDevice)
                     ->instTable.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    }
    endTime = vktrace_get_time();
    _dataSize = (pSurfaceFormatCount == NULL || pSurfaceFormats == NULL) ? 0 : (*pSurfaceFormatCount * sizeof(VkSurfaceFormatKHR));
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfaceFormatsKHR, sizeof(uint32_t) + _dataSize);
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfaceFormatsKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSurfaceFormatCount), sizeof(uint32_t), pSurfaceFormatCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSurfaceFormats), _dataSize, pSurfaceFormats);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSurfaceFormatCount));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSurfaceFormats));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header *vkCreateCommandPool(bool makeCall, VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
                                                 const VkAllocationCallbacks *pAllocator, VkCommandPool *pCommandPool) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateCommandPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateCommandPool,
                        get_struct_chain_size((void *)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkCommandPool));

    if (makeCall) {
        result = mdd(device)->devTable.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateCommandPool(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo), sizeof(VkCommandPoolCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCommandPool), sizeof(VkCommandPool), pCommandPool);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCommandPool));
    vktrace_finalize_trace_packet(pHeader);

    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkAllocateCommandBuffers(bool makeCall, VkDevice device,
                                                      const VkCommandBufferAllocateInfo *pAllocateInfo,
                                                      VkCommandBuffer *pCommandBuffers) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkAllocateCommandBuffers *pPacket = NULL;
    CREATE_TRACE_PACKET(vkAllocateCommandBuffers,
                        get_struct_chain_size((void *)pAllocateInfo) + sizeof(VkCommandBuffer) * pAllocateInfo->commandBufferCount);

    if (makeCall) {
        // Need to actually allocate the CommandBuffer(s) so that we have a
        // valid handle.
        result = mdd(device)->devTable.AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkAllocateCommandBuffers(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocateInfo), sizeof(VkCommandBufferAllocateInfo),
                                       pAllocateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCommandBuffers),
                                       sizeof(VkCommandBuffer) * pAllocateInfo->commandBufferCount, pCommandBuffers);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCommandBuffers));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkBeginCommandBuffer(bool makeCall, VkCommandBuffer commandBuffer,
                                                  const VkCommandBufferBeginInfo *pBeginInfo) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkBeginCommandBuffer *pPacket = NULL;
    CREATE_TRACE_PACKET(vkBeginCommandBuffer, get_struct_chain_size((void *)pBeginInfo));

    if (makeCall) {
        mdd(commandBuffer)->devTable.BeginCommandBuffer(commandBuffer, pBeginInfo);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkBeginCommandBuffer(pHeader);
    pPacket->commandBuffer = commandBuffer;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pBeginInfo), sizeof(VkCommandBufferBeginInfo), pBeginInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pBeginInfo->pInheritanceInfo),
                                       sizeof(VkCommandBufferInheritanceInfo), pBeginInfo->pInheritanceInfo);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pBeginInfo->pInheritanceInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pBeginInfo));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdPipelineBarrier(bool makeCall, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                                  VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                                                  uint32_t memoryBarrierCount, const VkMemoryBarrier *pMemoryBarriers,
                                                  uint32_t bufferMemoryBarrierCount,
                                                  const VkBufferMemoryBarrier *pBufferMemoryBarriers,
                                                  uint32_t imageMemoryBarrierCount,
                                                  const VkImageMemoryBarrier *pImageMemoryBarriers) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdPipelineBarrier *pPacket = NULL;
    size_t customSize;
    customSize = (memoryBarrierCount * sizeof(VkMemoryBarrier)) + (bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier)) +
                 (imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier));
    CREATE_TRACE_PACKET(vkCmdPipelineBarrier, customSize);

    if (makeCall) {
        mdd(commandBuffer)
            ->devTable.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount,
                                          pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount,
                                          pImageMemoryBarriers);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdPipelineBarrier(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->srcStageMask = srcStageMask;
    pPacket->dstStageMask = dstStageMask;
    pPacket->dependencyFlags = dependencyFlags;
    pPacket->memoryBarrierCount = memoryBarrierCount;
    pPacket->bufferMemoryBarrierCount = bufferMemoryBarrierCount;
    pPacket->imageMemoryBarrierCount = imageMemoryBarrierCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pMemoryBarriers), memoryBarrierCount * sizeof(VkMemoryBarrier),
                                       pMemoryBarriers);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pBufferMemoryBarriers),
                                       bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier), pBufferMemoryBarriers);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pImageMemoryBarriers),
                                       imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier), pImageMemoryBarriers);

    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pMemoryBarriers));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pBufferMemoryBarriers));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pImageMemoryBarriers));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkEndCommandBuffer(bool makeCall, VkCommandBuffer commandBuffer) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkEndCommandBuffer *pPacket = NULL;
    CREATE_TRACE_PACKET(vkEndCommandBuffer, 0);

    if (makeCall) {
        mdd(commandBuffer)->devTable.EndCommandBuffer(commandBuffer);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkEndCommandBuffer(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkQueueSubmit(bool makeCall, VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits,
                                           VkFence fence) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkQueueSubmit *pPacket = NULL;
    size_t arrayByteCount = 0;
    uint32_t i = 0;
    for (i = 0; i < submitCount; ++i) {
        arrayByteCount += vk_size_vksubmitinfo(&pSubmits[i]);
    }
    CREATE_TRACE_PACKET(vkQueueSubmit, arrayByteCount);

    if (makeCall) {
        mdd(queue)->devTable.QueueSubmit(queue, submitCount, pSubmits, fence);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkQueueSubmit(pHeader);
    pPacket->queue = queue;
    pPacket->submitCount = submitCount;
    pPacket->fence = fence;
    pPacket->result = result;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSubmits), submitCount * sizeof(VkSubmitInfo), pSubmits);
    for (i = 0; i < submitCount; ++i) {
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSubmits[i].pCommandBuffers),
                                           pPacket->pSubmits[i].commandBufferCount * sizeof(VkCommandBuffer),
                                           pSubmits[i].pCommandBuffers);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSubmits[i].pCommandBuffers));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSubmits[i].pWaitSemaphores),
                                           pPacket->pSubmits[i].waitSemaphoreCount * sizeof(VkSemaphore),
                                           pSubmits[i].pWaitSemaphores);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSubmits[i].pWaitSemaphores));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSubmits[i].pSignalSemaphores),
                                           pPacket->pSubmits[i].signalSemaphoreCount * sizeof(VkSemaphore),
                                           pSubmits[i].pSignalSemaphores);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSubmits[i].pSignalSemaphores));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pSubmits[i].pWaitDstStageMask),
                                           sizeof(VkPipelineStageFlags), pSubmits[i].pWaitDstStageMask);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSubmits[i].pWaitDstStageMask));
    }
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pSubmits));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkQueueWaitIdle(bool makeCall, VkQueue queue) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkQueueWaitIdle *pPacket = NULL;
    CREATE_TRACE_PACKET(vkQueueWaitIdle, 0);

    if (makeCall) {
        mdd(queue)->devTable.QueueWaitIdle(queue);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkQueueWaitIdle(pHeader);
    pPacket->queue = queue;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkResetCommandPool(bool makeCall, VkDevice device, VkCommandPool commandPool,
                                                VkCommandPoolResetFlags flags) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkResetCommandPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkResetCommandPool, 0);

    if (makeCall) {
        result = mdd(device)->devTable.ResetCommandPool(device, commandPool, flags);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkResetCommandPool(pHeader);
    pPacket->device = device;
    pPacket->commandPool = commandPool;
    pPacket->flags = flags;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkFreeCommandBuffers(bool makeCall, VkDevice device, VkCommandPool commandPool,
                                                  uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers) {
    vktrace_trace_packet_header *pHeader;
    packet_vkFreeCommandBuffers *pPacket = NULL;
    CREATE_TRACE_PACKET(vkFreeCommandBuffers, commandBufferCount * sizeof(VkCommandBuffer));

    if (makeCall) {
        // Actually make this call because we need to Free the commandBuffer(s)
        // we created.
        mdd(device)->devTable.FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkFreeCommandBuffers(pHeader);
    pPacket->device = device;
    pPacket->commandPool = commandPool;
    pPacket->commandBufferCount = commandBufferCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCommandBuffers), commandBufferCount * sizeof(VkCommandBuffer),
                                       pCommandBuffers);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCommandBuffers));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyCommandPool(bool makeCall, VkDevice device, VkCommandPool commandPool,
                                                  const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyCommandPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyCommandPool, sizeof(VkAllocationCallbacks));

    if (makeCall) {
        // Actually make this call to destroy the commandPool that we created
        mdd(device)->devTable.DestroyCommandPool(device, commandPool, pAllocator);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyCommandPool(pHeader);
    pPacket->device = device;
    pPacket->commandPool = commandPool;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkMapMemory(bool makeCall, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset,
                                         VkDeviceSize size, VkMemoryMapFlags flags, void **ppData) {
    vktrace_trace_packet_header *pHeader;
    packet_vkMapMemory *pPacket = NULL;
    CREATE_TRACE_PACKET(vkMapMemory, sizeof(void *));
    VkResult result = VK_SUCCESS;

    // actually map the memory if it was not already mapped.
    if (makeCall) {
        result = mdd(device)->devTable.MapMemory(device, memory, offset, size, flags, &(*ppData));
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);

    pPacket = interpret_body_as_vkMapMemory(pHeader);
    pPacket->device = device;
    pPacket->memory = memory;
    pPacket->offset = offset;
    pPacket->size = size;
    pPacket->flags = flags;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->ppData), sizeof(void *), *ppData);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->ppData));
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkUnmapMemory(bool makeCall, VkDeviceSize size, void *pData, VkDevice device, VkDeviceMemory memory) {
    vktrace_trace_packet_header *pHeader;
    packet_vkUnmapMemory *pPacket;
    CREATE_TRACE_PACKET(vkUnmapMemory, size);
    pPacket = interpret_body_as_vkUnmapMemory(pHeader);
    if (size > 0) {
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pData), size, pData);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pData));
    }

    if (makeCall) {
        mdd(device)->devTable.UnmapMemory(device, memory);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket->device = device;
    pPacket->memory = memory;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCreateBuffer(bool makeCall, VkDevice device, const VkBufferCreateInfo *pCreateInfo,
                                            const VkAllocationCallbacks *pAllocator, VkBuffer *pBuffer) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateBuffer *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateBuffer,
                        get_struct_chain_size((void *)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkBuffer));

    if (makeCall) {
        result = mdd(device)->devTable.CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateBuffer(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo), sizeof(VkBufferCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo->pQueueFamilyIndices),
                                       sizeof(uint32_t) * pCreateInfo->queueFamilyIndexCount, pCreateInfo->pQueueFamilyIndices);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pBuffer), sizeof(VkBuffer), pBuffer);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo->pQueueFamilyIndices));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pBuffer));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkGetBufferMemoryRequirements(bool makeCall, VkDevice device, VkBuffer buffer,
                                                           VkMemoryRequirements *pMemoryRequirements) {
    vktrace_trace_packet_header *pHeader;
    packet_vkGetBufferMemoryRequirements *pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetBufferMemoryRequirements, sizeof(VkMemoryRequirements));
    if (makeCall) {
        mdd(device)->devTable.GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkGetBufferMemoryRequirements(pHeader);
    pPacket->device = device;
    pPacket->buffer = buffer;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pMemoryRequirements), sizeof(VkMemoryRequirements),
                                       pMemoryRequirements);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pMemoryRequirements));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkAllocateMemory(bool makeCall, VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                              const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkAllocateMemory *pPacket = NULL;
    CREATE_TRACE_PACKET(vkAllocateMemory,
                        get_struct_chain_size((void *)pAllocateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkDeviceMemory));
    if (makeCall) {
        result = mdd(device)->devTable.AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkAllocateMemory(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocateInfo), sizeof(VkMemoryAllocateInfo), pAllocateInfo);
    add_alloc_memory_to_trace_packet(pHeader, (void **)&(pPacket->pAllocateInfo->pNext), pAllocateInfo->pNext);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pMemory), sizeof(VkDeviceMemory), pMemory);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pMemory));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkBindBufferMemory(bool makeCall, VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                                VkDeviceSize memoryOffset) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkBindBufferMemory *pPacket = NULL;
    CREATE_TRACE_PACKET(vkBindBufferMemory, 0);
    if (makeCall) {
        result = mdd(device)->devTable.BindBufferMemory(device, buffer, memory, memoryOffset);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkBindBufferMemory(pHeader);
    pPacket->device = device;
    pPacket->buffer = buffer;
    pPacket->memory = memory;
    pPacket->memoryOffset = memoryOffset;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdCopyBuffer(bool makeCall, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                                             uint32_t regionCount, const VkBufferCopy *pRegions) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdCopyBuffer *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdCopyBuffer, regionCount * sizeof(VkBufferCopy));
    if (makeCall) {
        mdd(commandBuffer)->devTable.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdCopyBuffer(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->srcBuffer = srcBuffer;
    pPacket->dstBuffer = dstBuffer;
    pPacket->regionCount = regionCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pRegions), regionCount * sizeof(VkBufferCopy), pRegions);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pRegions));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCreateFence(bool makeCall, VkDevice device, const VkFenceCreateInfo *pCreateInfo,
                                           const VkAllocationCallbacks *pAllocator, VkFence *pFence) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateFence *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateFence,
                        get_struct_chain_size((void *)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkFence));
    if (makeCall) {
        result = mdd(device)->devTable.CreateFence(device, pCreateInfo, pAllocator, pFence);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateFence(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo), sizeof(VkFenceCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pFence), sizeof(VkFence), pFence);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pFence));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkWaitForFences(bool makeCall, VkDevice device, uint32_t fenceCount, const VkFence *pFences,
                                             VkBool32 waitAll, uint64_t timeout) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkWaitForFences *pPacket = NULL;
    CREATE_TRACE_PACKET(vkWaitForFences, fenceCount * sizeof(VkFence));
    if (makeCall) {
        result = mdd(device)->devTable.WaitForFences(device, fenceCount, pFences, waitAll, timeout);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkWaitForFences(pHeader);
    pPacket->device = device;
    pPacket->fenceCount = fenceCount;
    pPacket->waitAll = waitAll;
    pPacket->timeout = timeout;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pFences), fenceCount * sizeof(VkFence), pFences);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pFences));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkResetFences(bool makeCall, VkDevice device, uint32_t fenceCount, const VkFence *pFences) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkResetFences *pPacket = NULL;
    CREATE_TRACE_PACKET(vkResetFences, fenceCount * sizeof(VkFence));
    if (makeCall) {
        result = mdd(device)->devTable.ResetFences(device, fenceCount, pFences);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkResetFences(pHeader);
    pPacket->device = device;
    pPacket->fenceCount = fenceCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pFences), fenceCount * sizeof(VkFence), pFences);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pFences));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyBuffer(bool makeCall, VkDevice device, VkBuffer buffer,
                                             const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyBuffer *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyBuffer, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyBuffer(device, buffer, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyBuffer(pHeader);
    pPacket->device = device;
    pPacket->buffer = buffer;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyBufferView(bool makeCall, VkDevice device, VkBufferView bufferView,
                                                 const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyBufferView *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyBufferView, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyBufferView(device, bufferView, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyBufferView(pHeader);
    pPacket->device = device;
    pPacket->bufferView = bufferView;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkFreeMemory(bool makeCall, VkDevice device, VkDeviceMemory memory,
                                          const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkFreeMemory *pPacket = NULL;
    CREATE_TRACE_PACKET(vkFreeMemory, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.FreeMemory(device, memory, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkFreeMemory(pHeader);
    pPacket->device = device;
    pPacket->memory = memory;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkAllocateDescriptorSets(bool makeCall, VkDevice device,
                                                      const VkDescriptorSetAllocateInfo *pAllocateInfo,
                                                      VkDescriptorSet *pDescriptorSets) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkAllocateDescriptorSets *pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();

    if (makeCall) {
        result = mdd(device)->devTable.AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
    }

    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkAllocateDescriptorSets, vk_size_vkdescriptorsetallocateinfo(pAllocateInfo) +
                                                      (pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSet)));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkAllocateDescriptorSets(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocateInfo), sizeof(VkDescriptorSetAllocateInfo),
                                       pAllocateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocateInfo->pSetLayouts),
                                       pPacket->pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSetLayout),
                                       pAllocateInfo->pSetLayouts);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pDescriptorSets),
                                       pPacket->pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSet), pDescriptorSets);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocateInfo->pSetLayouts));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pDescriptorSets));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocateInfo));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkUpdateDescriptorSets(bool makeCall, VkDevice device, uint32_t descriptorWriteCount,
                                                    const VkWriteDescriptorSet *pDescriptorWrites, uint32_t descriptorCopyCount,
                                                    const VkCopyDescriptorSet *pDescriptorCopies) {
    vktrace_trace_packet_header *pHeader;
    packet_vkUpdateDescriptorSets *pPacket = NULL;
    // begin custom code
    size_t arrayByteCount = 0;
    size_t i;

    for (i = 0; i < descriptorWriteCount; i++) {
        arrayByteCount += get_struct_chain_size(&pDescriptorWrites[i]);
    }

    for (i = 0; i < descriptorCopyCount; i++) {
        arrayByteCount += get_struct_chain_size(&pDescriptorCopies[i]);
    }

    CREATE_TRACE_PACKET(vkUpdateDescriptorSets, arrayByteCount);
    // end custom code

    if (makeCall) {
        mdd(device)->devTable.UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount,
                                                   pDescriptorCopies);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkUpdateDescriptorSets(pHeader);
    pPacket->device = device;
    pPacket->descriptorWriteCount = descriptorWriteCount;
    // begin custom code
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pDescriptorWrites),
                                       descriptorWriteCount * sizeof(VkWriteDescriptorSet), pDescriptorWrites);
    for (i = 0; i < descriptorWriteCount; i++) {
        switch (pPacket->pDescriptorWrites[i].descriptorType) {
            case VK_DESCRIPTOR_TYPE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pDescriptorWrites[i].pImageInfo),
                                                   pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo),
                                                   pDescriptorWrites[i].pImageInfo);
                vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pDescriptorWrites[i].pImageInfo));
            } break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pDescriptorWrites[i].pTexelBufferView),
                                                   pDescriptorWrites[i].descriptorCount * sizeof(VkBufferView),
                                                   pDescriptorWrites[i].pTexelBufferView);
                vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pDescriptorWrites[i].pTexelBufferView));
            } break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pDescriptorWrites[i].pBufferInfo),
                                                   pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorBufferInfo),
                                                   pDescriptorWrites[i].pBufferInfo);
                vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pDescriptorWrites[i].pBufferInfo));
            } break;
            default:
                break;
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pDescriptorWrites));

    pPacket->descriptorCopyCount = descriptorCopyCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pDescriptorCopies),
                                       descriptorCopyCount * sizeof(VkCopyDescriptorSet), pDescriptorCopies);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pDescriptorCopies));
    // end custom code

    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkResetDescriptorPool(bool makeCall, VkDevice device, VkDescriptorPool descriptorPool,
                                                   VkDescriptorPoolResetFlags flags) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkResetDescriptorPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkResetDescriptorPool, 0);
    if (makeCall) {
        result = mdd(device)->devTable.ResetDescriptorPool(device, descriptorPool, flags);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkResetDescriptorPool(pHeader);
    pPacket->device = device;
    pPacket->descriptorPool = descriptorPool;
    pPacket->flags = 0;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdBeginQuery(bool makeCall, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query,
                                             VkQueryControlFlags flags) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdBeginQuery *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdBeginQuery, 0);
    if (makeCall) {
        mdd(commandBuffer)->devTable.CmdBeginQuery(commandBuffer, queryPool, query, flags);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdBeginQuery(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->queryPool = queryPool;
    pPacket->query = query;
    pPacket->flags = flags;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdEndQuery(bool makeCall, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdEndQuery *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdEndQuery, 0);
    if (makeCall) {
        mdd(commandBuffer)->devTable.CmdEndQuery(commandBuffer, queryPool, query);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdEndQuery(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->queryPool = queryPool;
    pPacket->query = query;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdWriteTimestamp(bool makeCall, VkCommandBuffer commandBuffer,
                                                 VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdWriteTimestamp *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdWriteTimestamp, 0);
    if (makeCall) {
        mdd(commandBuffer)->devTable.CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdWriteTimestamp(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->pipelineStage = pipelineStage;
    pPacket->queryPool = queryPool;
    pPacket->query = query;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdResetQueryPool(bool makeCall, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                                                 uint32_t firstQuery, uint32_t queryCount) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdResetQueryPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdResetQueryPool, 0);
    if (makeCall) {
        mdd(commandBuffer)->devTable.CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdResetQueryPool(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->queryPool = queryPool;
    pPacket->firstQuery = firstQuery;
    pPacket->queryCount = queryCount;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyQueryPool(bool makeCall, VkDevice device, VkQueryPool queryPool,
                                                const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyQueryPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyQueryPool, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyQueryPool(device, queryPool, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyQueryPool(pHeader);
    pPacket->device = device;
    pPacket->queryPool = queryPool;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyEvent(bool makeCall, VkDevice device, VkEvent event,
                                            const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyEvent *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyEvent, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyEvent(device, event, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyEvent(pHeader);
    pPacket->device = device;
    pPacket->event = event;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyFence(bool makeCall, VkDevice device, VkFence fence,
                                            const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyFence *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyFence, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyFence(device, fence, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyFence(pHeader);
    pPacket->device = device;
    pPacket->fence = fence;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroySemaphore(bool makeCall, VkDevice device, VkSemaphore semaphore,
                                                const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroySemaphore *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroySemaphore, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroySemaphore(device, semaphore, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroySemaphore(pHeader);
    pPacket->device = device;
    pPacket->semaphore = semaphore;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyFramebuffer(bool makeCall, VkDevice device, VkFramebuffer framebuffer,
                                                  const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyFramebuffer *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyFramebuffer, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyFramebuffer(device, framebuffer, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyFramebuffer(pHeader);
    pPacket->device = device;
    pPacket->framebuffer = framebuffer;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyDescriptorPool(bool makeCall, VkDevice device, VkDescriptorPool descriptorPool,
                                                     const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyDescriptorPool *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyDescriptorPool, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyDescriptorPool(device, descriptorPool, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyDescriptorPool(pHeader);
    pPacket->device = device;
    pPacket->descriptorPool = descriptorPool;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCreateRenderPass(bool makeCall, VkDevice device, const VkRenderPassCreateInfo *pCreateInfo,
                                                const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateRenderPass *pPacket = NULL;
    // begin custom code (get_struct_chain_size)
    uint32_t attachmentCount = (pCreateInfo != NULL && (pCreateInfo->pAttachments != NULL)) ? pCreateInfo->attachmentCount : 0;
    uint32_t dependencyCount = (pCreateInfo != NULL && (pCreateInfo->pDependencies != NULL)) ? pCreateInfo->dependencyCount : 0;
    uint32_t subpassCount = (pCreateInfo != NULL && (pCreateInfo->pSubpasses != NULL)) ? pCreateInfo->subpassCount : 0;
    CREATE_TRACE_PACKET(vkCreateRenderPass,
                        get_struct_chain_size((void *)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkRenderPass));
    // end custom code
    if (makeCall) {
        result = mdd(device)->devTable.CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateRenderPass(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo), sizeof(VkRenderPassCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo->pAttachments),
                                       attachmentCount * sizeof(VkAttachmentDescription), pCreateInfo->pAttachments);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo->pDependencies),
                                       dependencyCount * sizeof(VkSubpassDependency), pCreateInfo->pDependencies);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo->pSubpasses),
                                       subpassCount * sizeof(VkSubpassDescription), pCreateInfo->pSubpasses);
    uint32_t i;
    for (i = 0; i < pPacket->pCreateInfo->subpassCount; i++) {
        VkSubpassDescription *pSubpass = (VkSubpassDescription *)&pPacket->pCreateInfo->pSubpasses[i];
        const VkSubpassDescription *pSp = &pCreateInfo->pSubpasses[i];
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pSubpass->pInputAttachments),
                                           pSubpass->inputAttachmentCount * sizeof(VkAttachmentReference), pSp->pInputAttachments);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pSubpass->pInputAttachments));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pSubpass->pColorAttachments),
                                           pSubpass->colorAttachmentCount * sizeof(VkAttachmentReference), pSp->pColorAttachments);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pSubpass->pColorAttachments));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pSubpass->pResolveAttachments),
                                           pSubpass->colorAttachmentCount * sizeof(VkAttachmentReference),
                                           pSp->pResolveAttachments);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pSubpass->pResolveAttachments));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pSubpass->pDepthStencilAttachment),
                                           1 * sizeof(VkAttachmentReference), pSp->pDepthStencilAttachment);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pSubpass->pDepthStencilAttachment));
        vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pSubpass->pPreserveAttachments),
                                           pSubpass->preserveAttachmentCount * sizeof(VkAttachmentReference),
                                           pSp->pPreserveAttachments);
        vktrace_finalize_buffer_address(pHeader, (void **)&(pSubpass->pPreserveAttachments));
    }
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pRenderPass), sizeof(VkRenderPass), pRenderPass);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo->pAttachments));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo->pDependencies));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo->pSubpasses));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pRenderPass));
    vktrace_finalize_trace_packet(pHeader);

    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCreateGraphicsPipelines(bool makeCall, VkDevice device, VkPipelineCache pipelineCache,
                                                       uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo *pCreateInfos,
                                                       const VkAllocationCallbacks *pAllocator, VkPipeline *pPipelines) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateGraphicsPipelines *pPacket = NULL;
    size_t total_size = 0;
    uint32_t i;
    for (i = 0; i < createInfoCount; i++) {
        total_size += get_VkGraphicsPipelineCreateInfo_size_ROUNDUP_TO_4(&pCreateInfos[i]);
    }
    CREATE_TRACE_PACKET(vkCreateGraphicsPipelines,
                        total_size + sizeof(VkAllocationCallbacks) + createInfoCount * sizeof(VkPipeline));
    if (makeCall) {
        result = mdd(device)->devTable.CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator,
                                                               pPipelines);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateGraphicsPipelines(pHeader);
    pPacket->device = device;
    pPacket->pipelineCache = pipelineCache;
    pPacket->createInfoCount = createInfoCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfos),
                                       createInfoCount * sizeof(VkGraphicsPipelineCreateInfo), pCreateInfos);
    add_VkGraphicsPipelineCreateInfos_to_trace_packet(pHeader, (VkGraphicsPipelineCreateInfo *)pPacket->pCreateInfos, pCreateInfos,
                                                      createInfoCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pPipelines), createInfoCount * sizeof(VkPipeline), pPipelines);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfos));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pPipelines));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCreateComputePipelines(bool makeCall, VkDevice device, VkPipelineCache pipelineCache,
                                                      uint32_t createInfoCount, const VkComputePipelineCreateInfo *pCreateInfos,
                                                      const VkAllocationCallbacks *pAllocator, VkPipeline *pPipelines) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateComputePipelines *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateComputePipelines, createInfoCount * sizeof(VkComputePipelineCreateInfo) +
                                                      getVkComputePipelineCreateInfosAdditionalSize(createInfoCount, pCreateInfos) +
                                                      sizeof(VkAllocationCallbacks) + createInfoCount * sizeof(VkPipeline));

    if (makeCall) {
        result = mdd(device)->devTable.CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator,
                                                              pPipelines);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateComputePipelines(pHeader);
    pPacket->device = device;
    pPacket->pipelineCache = pipelineCache;
    pPacket->createInfoCount = createInfoCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfos),
                                       createInfoCount * sizeof(VkComputePipelineCreateInfo), pCreateInfos);
    add_VkComputePipelineCreateInfos_to_trace_packet(pHeader, (VkComputePipelineCreateInfo *)pPacket->pCreateInfos, pCreateInfos,
                                                     createInfoCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pPipelines), createInfoCount * sizeof(VkPipeline), pPipelines);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfos));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pPipelines));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyPipeline(bool makeCall, VkDevice device, VkPipeline pipeline,
                                               const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyPipeline *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyPipeline, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyPipeline(device, pipeline, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyPipeline(pHeader);
    pPacket->device = device;
    pPacket->pipeline = pipeline;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyPipelineCache(bool makeCall, VkDevice device, VkPipelineCache pipelineCache,
                                                    const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyPipelineCache *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyPipelineCache, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyPipelineCache(device, pipelineCache, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyPipelineCache(pHeader);
    pPacket->device = device;
    pPacket->pipelineCache = pipelineCache;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyPipelineLayout(bool makeCall, VkDevice device, VkPipelineLayout pipelineLayout,
                                                     const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyPipelineLayout *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyPipelineLayout, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyPipelineLayout(device, pipelineLayout, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyPipelineLayout(pHeader);
    pPacket->device = device;
    pPacket->pipelineLayout = pipelineLayout;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCreateShaderModule(bool makeCall, VkDevice device, const VkShaderModuleCreateInfo *pCreateInfo,
                                                  const VkAllocationCallbacks *pAllocator, VkShaderModule *pShaderModule) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header *pHeader;
    packet_vkCreateShaderModule *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateShaderModule,
                        get_struct_chain_size((void *)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkShaderModule));
    if (makeCall) {
        result = mdd(device)->devTable.CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateShaderModule(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo), sizeof(VkShaderModuleCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pCreateInfo->pCode), pPacket->pCreateInfo->codeSize,
                                       pCreateInfo->pCode);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pShaderModule), sizeof(VkShaderModule), pShaderModule);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo->pCode));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pShaderModule));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyShaderModule(bool makeCall, VkDevice device, VkShaderModule shaderModule,
                                                   const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyShaderModule *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyShaderModule, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyShaderModule(device, shaderModule, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyShaderModule(pHeader);
    pPacket->device = device;
    pPacket->shaderModule = shaderModule;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyRenderPass(bool makeCall, VkDevice device, VkRenderPass renderPass,
                                                 const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyRenderPass *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyRenderPass, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyRenderPass(device, renderPass, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyRenderPass(pHeader);
    pPacket->device = device;
    pPacket->renderPass = renderPass;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyDescriptorSetLayout(bool makeCall, VkDevice device, VkDescriptorSetLayout descriptorSetLayout,
                                                          const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyDescriptorSetLayout *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyDescriptorSetLayout, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyDescriptorSetLayout(pHeader);
    pPacket->device = device;
    pPacket->descriptorSetLayout = descriptorSetLayout;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroySampler(bool makeCall, VkDevice device, VkSampler sampler,
                                              const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroySampler *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroySampler, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroySampler(device, sampler, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroySampler(pHeader);
    pPacket->device = device;
    pPacket->sampler = sampler;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyImage(bool makeCall, VkDevice device, VkImage image,
                                            const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyImage *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyImage, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyImage(device, image, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyImage(pHeader);
    pPacket->device = device;
    pPacket->image = image;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyImageView(bool makeCall, VkDevice device, VkImageView imageView,
                                                const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyImageView *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyImageView, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyImageView(device, imageView, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyImageView(pHeader);
    pPacket->device = device;
    pPacket->imageView = imageView;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroySwapchainKHR(bool makeCall, VkDevice device, VkSwapchainKHR swapchain,
                                                   const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroySwapchainKHR *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroySwapchainKHR, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroySwapchainKHR(device, swapchain, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroySwapchainKHR(pHeader);
    pPacket->device = device;
    pPacket->swapchain = swapchain;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroySurfaceKHR(bool makeCall, VkInstance instance, VkSurfaceKHR surface,
                                                 const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroySurfaceKHR *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroySurfaceKHR, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mid(instance)->instTable.DestroySurfaceKHR(instance, surface, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroySurfaceKHR(pHeader);
    pPacket->instance = instance;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyDevice(bool makeCall, VkDevice device, const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyDevice *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyDevice, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mdd(device)->devTable.DestroyDevice(device, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyDevice(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkDestroyInstance(bool makeCall, VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    vktrace_trace_packet_header *pHeader;
    packet_vkDestroyInstance *pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyInstance, sizeof(VkAllocationCallbacks));
    if (makeCall) {
        mid(instance)->instTable.DestroyInstance(instance, pAllocator);
    }
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyInstance(pHeader);
    pPacket->instance = instance;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
vktrace_trace_packet_header *vkCmdCopyBufferToImage(bool makeCall, VkCommandBuffer commandBuffer, VkBuffer srcBuffer,
                                                    VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                                    const VkBufferImageCopy *pRegions) {
    vktrace_trace_packet_header *pHeader;
    packet_vkCmdCopyBufferToImage *pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdCopyBufferToImage, regionCount * sizeof(VkBufferImageCopy));

    if (makeCall) {
        mdd(commandBuffer)
            ->devTable.CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    }

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdCopyBufferToImage(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->srcBuffer = srcBuffer;
    pPacket->dstImage = dstImage;
    pPacket->dstImageLayout = dstImageLayout;
    pPacket->regionCount = regionCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void **)&(pPacket->pRegions), regionCount * sizeof(VkBufferImageCopy), pRegions);
    vktrace_finalize_buffer_address(pHeader, (void **)&(pPacket->pRegions));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

//=====================================================================
}  // namespace generate
}  // namespace trim
