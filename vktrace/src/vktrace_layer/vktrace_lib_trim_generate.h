/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once
#include "vktrace_trace_packet_identifiers.h"
#include "vulkan.h"

namespace trim {
    namespace generate
    {
        vktrace_trace_packet_header* vkCreateCommandPool(
            bool makeCall,
            VkDevice device,
            const VkCommandPoolCreateInfo* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkCommandPool* pCommandPool);

        vktrace_trace_packet_header* vkAllocateCommandBuffers(
            bool makeCall,
            VkDevice device,
            const VkCommandBufferAllocateInfo* pAllocateInfo,
            VkCommandBuffer* pCommandBuffers);

        vktrace_trace_packet_header* vkBeginCommandBuffer(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer,
            const VkCommandBufferBeginInfo* pBeginInfo);

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
            const VkImageMemoryBarrier* pImageMemoryBarriers);

        vktrace_trace_packet_header* vkEndCommandBuffer(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer);

        vktrace_trace_packet_header* vkQueueSubmit(
            bool makeCall,
            VkDevice device,
            VkQueue queue,
            uint32_t submitCount,
            const VkSubmitInfo* pSubmits,
            VkFence fence);

        vktrace_trace_packet_header* vkQueueWaitIdle(
            bool makeCall,
            VkDevice device,
            VkQueue queue);

        vktrace_trace_packet_header* vkResetCommandPool(
            bool makeCall,
            VkDevice device,
            VkCommandPool commandPool,
            VkCommandPoolResetFlags flags);

        vktrace_trace_packet_header* vkFreeCommandBuffers(
            bool makeCall,
            VkDevice device,
            VkCommandPool commandPool,
            uint32_t commandBufferCount,
            const VkCommandBuffer* pCommandBuffers);

        vktrace_trace_packet_header* vkDestroyCommandPool(
            bool makeCall,
            VkDevice device,
            VkCommandPool commandPool,
            const VkAllocationCallbacks* pAllocator);

        vktrace_trace_packet_header* vkMapMemory(
            bool makeCall,
            VkDevice device,
            VkDeviceMemory memory,
            VkDeviceSize offset,
            VkDeviceSize size,
            VkMemoryMapFlags flags,
            void** ppData);

        vktrace_trace_packet_header* vkUnmapMemory(
            bool makeCall,
            VkDeviceSize size,
            void* pData,
            VkDevice device,
            VkDeviceMemory memory);

        vktrace_trace_packet_header* vkCreateBuffer(
            bool makeCall,
            VkDevice device,
            const VkBufferCreateInfo* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkBuffer* pBuffer);

        vktrace_trace_packet_header* vkGetBufferMemoryRequirements(
            bool makeCall,
            VkDevice device,
            VkBuffer buffer,
            VkMemoryRequirements* pMemoryRequirements);

        vktrace_trace_packet_header* vkAllocateMemory(
            bool makeCall,
            VkDevice device,
            const VkMemoryAllocateInfo* pAllocateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDeviceMemory* pMemory);

        vktrace_trace_packet_header* vkBindBufferMemory(
            bool makeCall,
            VkDevice device,
            VkBuffer buffer,
            VkDeviceMemory memory,
            VkDeviceSize memoryOffset);

        vktrace_trace_packet_header* vkCmdCopyBuffer(
            bool makeCall,
            VkDevice device,
            VkCommandBuffer commandBuffer,
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            uint32_t regionCount,
            const VkBufferCopy* pRegions);

        vktrace_trace_packet_header* vkCreateFence(
            bool makeCall,
            VkDevice device,
            const VkFenceCreateInfo* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkFence* pFence);

        vktrace_trace_packet_header* vkWaitForFences(
            bool makeCall,
            VkDevice device,
            uint32_t fenceCount,
            const VkFence* pFences,
            VkBool32 waitAll,
            uint64_t timeout);

        vktrace_trace_packet_header* vkResetFences(
            bool makeCall,
            VkDevice device,
            uint32_t fenceCount,
            const VkFence* pFences);

        vktrace_trace_packet_header* vkDestroyBuffer(
            bool makeCall,
            VkDevice device,
            VkBuffer buffer,
            const VkAllocationCallbacks* pAllocator);

        vktrace_trace_packet_header* vkFreeMemory(
            bool makeCall,
            VkDevice device,
            VkDeviceMemory memory,
            const VkAllocationCallbacks* pAllocator);
    } // namespace generate
} // namespace trim
