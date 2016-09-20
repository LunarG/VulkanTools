/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once
#include "vktrace_trace_packet_identifiers.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_lib_helpers.h"
#include "vktrace_vk_vk_packets.h"
#include "vktrace_vk_packet_id.h"
#include "vulkan.h"

// defined in vktrace_lib_trace.cpp
extern layer_device_data *mdd(void* object);
extern layer_instance_data *mid(void *object);

namespace trim {
    //=============================================================================
    // Helpers to generate trace packets
    //=============================================================================
    namespace generate
    {
        vktrace_trace_packet_header* vkCreateCommandPool(
            bool makeCall,
            VkDevice device,
            const VkCommandPoolCreateInfo* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkCommandPool* pCommandPool)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkCreateCommandPool* pPacket = NULL;
            CREATE_TRACE_PACKET(vkCreateCommandPool, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkCommandPool));

            if (makeCall)
            {
                result = mdd(device)->devTable.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkCreateCommandPool(pHeader);
            pPacket->device = device;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkCommandPoolCreateInfo), pCreateInfo);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCommandPool), sizeof(VkCommandPool), pCommandPool);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCommandPool));
            vktrace_finalize_trace_packet(pHeader);

            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkAllocateCommandBuffers(
            bool makeCall,
            VkDevice device,
            const VkCommandBufferAllocateInfo* pAllocateInfo,
            VkCommandBuffer* pCommandBuffers)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkAllocateCommandBuffers* pPacket = NULL;
            CREATE_TRACE_PACKET(vkAllocateCommandBuffers, get_struct_chain_size((void*)pAllocateInfo) + sizeof(VkCommandBuffer) * pAllocateInfo->commandBufferCount);

            if (makeCall)
            {
                // Need to actually allocate the CommandBuffer(s) so that we have a valid handle.
                result = mdd(device)->devTable.AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkAllocateCommandBuffers(pHeader);
            pPacket->device = device;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkCommandBufferAllocateInfo), pAllocateInfo);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCommandBuffers), sizeof(VkCommandBuffer) * pAllocateInfo->commandBufferCount, pCommandBuffers);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCommandBuffers));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkBeginCommandBuffer(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer,
            const VkCommandBufferBeginInfo* pBeginInfo)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkBeginCommandBuffer* pPacket = NULL;
            CREATE_TRACE_PACKET(vkBeginCommandBuffer, get_struct_chain_size((void*)pBeginInfo));

            if (makeCall)
            {
                mdd(device)->devTable.BeginCommandBuffer(commandBuffer, pBeginInfo);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkBeginCommandBuffer(pHeader);
            pPacket->commandBuffer = commandBuffer;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBeginInfo), sizeof(VkCommandBufferBeginInfo), pBeginInfo);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBeginInfo->pInheritanceInfo), sizeof(VkCommandBufferInheritanceInfo), pBeginInfo->pInheritanceInfo);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBeginInfo->pInheritanceInfo));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBeginInfo));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkCmdPipelineBarrier(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkDependencyFlags dependencyFlags,
            uint32_t memoryBarrierCount,
            const VkMemoryBarrier* pMemoryBarriers,
            uint32_t bufferMemoryBarrierCount,
            const VkBufferMemoryBarrier* pBufferMemoryBarriers,
            uint32_t imageMemoryBarrierCount,
            const VkImageMemoryBarrier* pImageMemoryBarriers)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkCmdPipelineBarrier* pPacket = NULL;
            size_t customSize;
            customSize = (memoryBarrierCount * sizeof(VkMemoryBarrier)) +
                (bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier)) +
                (imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier));
            CREATE_TRACE_PACKET(vkCmdPipelineBarrier, customSize);

            if (makeCall)
            {
                mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
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
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemoryBarriers), memoryBarrierCount * sizeof(VkMemoryBarrier), pMemoryBarriers);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBufferMemoryBarriers), bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier), pBufferMemoryBarriers);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pImageMemoryBarriers), imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier), pImageMemoryBarriers);

            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemoryBarriers));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBufferMemoryBarriers));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pImageMemoryBarriers));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkEndCommandBuffer(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkEndCommandBuffer* pPacket = NULL;
            CREATE_TRACE_PACKET(vkEndCommandBuffer, 0);

            if (makeCall)
            {
                mdd(device)->devTable.EndCommandBuffer(commandBuffer);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkEndCommandBuffer(pHeader);
            pPacket->commandBuffer = commandBuffer;
            pPacket->result = result;
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkQueueSubmit(
            bool makeCall,
            VkDevice device,
            VkQueue queue,
            uint32_t submitCount,
            const VkSubmitInfo* pSubmits,
            VkFence fence)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkQueueSubmit* pPacket = NULL;
            size_t arrayByteCount = 0;
            uint32_t i = 0;
            for (i = 0; i < submitCount; ++i) {
                arrayByteCount += vk_size_vksubmitinfo(&pSubmits[i]);
            }
            CREATE_TRACE_PACKET(vkQueueSubmit, arrayByteCount);

            if (makeCall)
            {
                mdd(device)->devTable.QueueSubmit(queue, submitCount, pSubmits, fence);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkQueueSubmit(pHeader);
            pPacket->queue = queue;
            pPacket->submitCount = submitCount;
            pPacket->fence = fence;
            pPacket->result = result;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits), submitCount*sizeof(VkSubmitInfo), pSubmits);
            for (i = 0; i < submitCount; ++i) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pCommandBuffers), pPacket->pSubmits[i].commandBufferCount * sizeof(VkCommandBuffer), pSubmits[i].pCommandBuffers);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pCommandBuffers));
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pWaitSemaphores), pPacket->pSubmits[i].waitSemaphoreCount * sizeof(VkSemaphore), pSubmits[i].pWaitSemaphores);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pWaitSemaphores));
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pSignalSemaphores), pPacket->pSubmits[i].signalSemaphoreCount * sizeof(VkSemaphore), pSubmits[i].pSignalSemaphores);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pSignalSemaphores));
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pWaitDstStageMask), sizeof(VkPipelineStageFlags), pSubmits[i].pWaitDstStageMask);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pWaitDstStageMask));
            }
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkQueueWaitIdle(
            bool makeCall,
            VkDevice device,
            VkQueue queue)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkQueueWaitIdle* pPacket = NULL;
            CREATE_TRACE_PACKET(vkQueueWaitIdle, 0);

            if (makeCall)
            {
                mdd(device)->devTable.QueueWaitIdle(queue);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkQueueWaitIdle(pHeader);
            pPacket->queue = queue;
            pPacket->result = result;
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkResetCommandPool(
            bool makeCall,
            VkDevice device,
            VkCommandPool commandPool,
            VkCommandPoolResetFlags flags)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkResetCommandPool* pPacket = NULL;
            CREATE_TRACE_PACKET(vkResetCommandPool, 0);

            if (makeCall)
            {
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
        vktrace_trace_packet_header* vkFreeCommandBuffers(
            bool makeCall,
            VkDevice device,
            VkCommandPool commandPool,
            uint32_t commandBufferCount,
            const VkCommandBuffer* pCommandBuffers)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkFreeCommandBuffers* pPacket = NULL;
            CREATE_TRACE_PACKET(vkFreeCommandBuffers, commandBufferCount*sizeof(VkCommandBuffer));

            if (makeCall)
            {
                // Actually make this call because we need to Free the commandBuffer(s) we created.
                mdd(device)->devTable.FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkFreeCommandBuffers(pHeader);
            pPacket->device = device;
            pPacket->commandPool = commandPool;
            pPacket->commandBufferCount = commandBufferCount;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCommandBuffers), commandBufferCount*sizeof(VkCommandBuffer), pCommandBuffers);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCommandBuffers));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkDestroyCommandPool(
            bool makeCall,
            VkDevice device,
            VkCommandPool commandPool,
            const VkAllocationCallbacks* pAllocator)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkDestroyCommandPool* pPacket = NULL;
            CREATE_TRACE_PACKET(vkDestroyCommandPool, sizeof(VkAllocationCallbacks));

            if (makeCall)
            {
                // Actually make this call to destroy the commandPool that we created
                mdd(device)->devTable.DestroyCommandPool(device, commandPool, pAllocator);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkDestroyCommandPool(pHeader);
            pPacket->device = device;
            pPacket->commandPool = commandPool;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkMapMemory(
            bool makeCall,
            VkDevice device,
            VkDeviceMemory memory,
            VkDeviceSize offset,
            VkDeviceSize size,
            VkMemoryMapFlags flags,
            void** ppData)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkMapMemory* pPacket = NULL;
            CREATE_TRACE_PACKET(vkMapMemory, sizeof(void*));
            VkResult result = VK_SUCCESS;

            // actually map the memory if it was not already mapped.
            if (makeCall)
            {
                result = mdd(device)->devTable.MapMemory(device, memory, offset, size, flags, &(*ppData));
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);

            pPacket = interpret_body_as_vkMapMemory(pHeader);
            pPacket->device = device;
            pPacket->memory = memory;
            pPacket->offset = offset;
            pPacket->size = size;
            pPacket->flags = flags;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData), sizeof(void*), *ppData);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData));
            pPacket->result = result;
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkUnmapMemory(
            bool makeCall,
            VkDeviceSize size,
            void* pData,
            VkDevice device,
            VkDeviceMemory memory)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkUnmapMemory* pPacket;
            CREATE_TRACE_PACKET(vkUnmapMemory, size);
            pPacket = interpret_body_as_vkUnmapMemory(pHeader);
            if (size > 0)
            {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pData), size, pData);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pData));
            }

            if (makeCall)
            {
                mdd(device)->devTable.UnmapMemory(device, memory);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket->device = device;
            pPacket->memory = memory;
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkCreateBuffer(
            bool makeCall,
            VkDevice device,
            const VkBufferCreateInfo* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkBuffer* pBuffer)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkCreateBuffer* pPacket = NULL;
            CREATE_TRACE_PACKET(vkCreateBuffer, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkBuffer));

            if (makeCall)
            {
                result = mdd(device)->devTable.CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkCreateBuffer(pHeader);
            pPacket->device = device;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkBufferCreateInfo), pCreateInfo);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices), sizeof(uint32_t) * pCreateInfo->queueFamilyIndexCount, pCreateInfo->pQueueFamilyIndices);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBuffer), sizeof(VkBuffer), pBuffer);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBuffer));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkGetBufferMemoryRequirements(
            bool makeCall,
            VkDevice device,
            VkBuffer buffer,
            VkMemoryRequirements* pMemoryRequirements)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkGetBufferMemoryRequirements* pPacket = NULL;
            CREATE_TRACE_PACKET(vkGetBufferMemoryRequirements, sizeof(VkMemoryRequirements));
            if (makeCall)
            {
                mdd(device)->devTable.GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkGetBufferMemoryRequirements(pHeader);
            pPacket->device = device;
            pPacket->buffer = buffer;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemoryRequirements), sizeof(VkMemoryRequirements), pMemoryRequirements);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemoryRequirements));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkAllocateMemory(
            bool makeCall,
            VkDevice device,
            const VkMemoryAllocateInfo* pAllocateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDeviceMemory* pMemory)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkAllocateMemory* pPacket = NULL;
            CREATE_TRACE_PACKET(vkAllocateMemory, get_struct_chain_size((void*)pAllocateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkDeviceMemory));
            if (makeCall)
            {
                result = mdd(device)->devTable.AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkAllocateMemory(pHeader);
            pPacket->device = device;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkMemoryAllocateInfo), pAllocateInfo);
            add_alloc_memory_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo->pNext), pAllocateInfo->pNext);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemory), sizeof(VkDeviceMemory), pMemory);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemory));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkBindBufferMemory(
            bool makeCall,
            VkDevice device,
            VkBuffer buffer,
            VkDeviceMemory memory,
            VkDeviceSize memoryOffset)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkBindBufferMemory* pPacket = NULL;
            CREATE_TRACE_PACKET(vkBindBufferMemory, 0);
            if (makeCall)
            {
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
        vktrace_trace_packet_header* vkCmdCopyBuffer(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer,
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            uint32_t regionCount,
            const VkBufferCopy* pRegions)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkCmdCopyBuffer* pPacket = NULL;
            CREATE_TRACE_PACKET(vkCmdCopyBuffer, regionCount*sizeof(VkBufferCopy));
            if (makeCall)
            {
                mdd(device)->devTable.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkCmdCopyBuffer(pHeader);
            pPacket->commandBuffer = commandBuffer;
            pPacket->srcBuffer = srcBuffer;
            pPacket->dstBuffer = dstBuffer;
            pPacket->regionCount = regionCount;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pRegions), regionCount*sizeof(VkBufferCopy), pRegions);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pRegions));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkCreateFence(
            bool makeCall,
            VkDevice device,
            const VkFenceCreateInfo* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkFence* pFence)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkCreateFence* pPacket = NULL;
            CREATE_TRACE_PACKET(vkCreateFence, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkFence));
            if (makeCall)
            {
                result = mdd(device)->devTable.CreateFence(device, pCreateInfo, pAllocator, pFence);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkCreateFence(pHeader);
            pPacket->device = device;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkFenceCreateInfo), pCreateInfo);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pFence), sizeof(VkFence), pFence);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pFence));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkWaitForFences(
            bool makeCall,
            VkDevice device,
            uint32_t fenceCount,
            const VkFence* pFences,
            VkBool32 waitAll,
            uint64_t timeout)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkWaitForFences* pPacket = NULL;
            CREATE_TRACE_PACKET(vkWaitForFences, fenceCount*sizeof(VkFence));
            if (makeCall)
            {
                result = mdd(device)->devTable.WaitForFences(device, fenceCount, pFences, waitAll, timeout);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkWaitForFences(pHeader);
            pPacket->device = device;
            pPacket->fenceCount = fenceCount;
            pPacket->waitAll = waitAll;
            pPacket->timeout = timeout;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pFences), fenceCount*sizeof(VkFence), pFences);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pFences));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkResetFences(
            bool makeCall,
            VkDevice device,
            uint32_t fenceCount,
            const VkFence* pFences)
        {
            VkResult result = VK_SUCCESS;
            vktrace_trace_packet_header* pHeader;
            packet_vkResetFences* pPacket = NULL;
            CREATE_TRACE_PACKET(vkResetFences, fenceCount*sizeof(VkFence));
            if (makeCall)
            {
                result = mdd(device)->devTable.ResetFences(device, fenceCount, pFences);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkResetFences(pHeader);
            pPacket->device = device;
            pPacket->fenceCount = fenceCount;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pFences), fenceCount*sizeof(VkFence), pFences);
            pPacket->result = result;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pFences));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkDestroyBuffer(
            bool makeCall,
            VkDevice device,
            VkBuffer buffer,
            const VkAllocationCallbacks* pAllocator)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkDestroyBuffer* pPacket = NULL;
            CREATE_TRACE_PACKET(vkDestroyBuffer, sizeof(VkAllocationCallbacks));
            if (makeCall)
            {
                mdd(device)->devTable.DestroyBuffer(device, buffer, pAllocator);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkDestroyBuffer(pHeader);
            pPacket->device = device;
            pPacket->buffer = buffer;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }

        //=====================================================================
        vktrace_trace_packet_header* vkFreeMemory(
            bool makeCall,
            VkDevice device,
            VkDeviceMemory memory,
            const VkAllocationCallbacks* pAllocator)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkFreeMemory* pPacket = NULL;
            CREATE_TRACE_PACKET(vkFreeMemory, sizeof(VkAllocationCallbacks));
            if (makeCall)
            {
                mdd(device)->devTable.FreeMemory(device, memory, pAllocator);
            }
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkFreeMemory(pHeader);
            pPacket->device = device;
            pPacket->memory = memory;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            vktrace_finalize_trace_packet(pHeader);
            return pHeader;
        }
    } // namespace generate
} // namespace trim
