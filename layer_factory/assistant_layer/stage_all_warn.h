/*
 * Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

#pragma once

class WarnOnPipelineStageAll : public layer_factory {
   public:
    // Constructor for interceptor
    WarnOnPipelineStageAll() : layer_factory(this){};

    void CheckPipelineStageFlags(std::string api_name, const VkPipelineStageFlags flags) {
        if (flags & VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT) {
            std::string msg = "You are using VK_PIPELINE_STAGE_ALL_GRAPHICS_bit when " + api_name + " is called\n";
            Warning(msg);
        } else if (flags & VK_PIPELINE_STAGE_ALL_COMMANDS_BIT) {
            std::string msg = "You are using VK_PIPELINE_STAGE_ALL_COMMANDS_BIT when " + api_name + " is called\n";
            Warning(msg);
        }
    };

    VkResult PreCallQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) {
        for (uint32_t submit = 0; submit < submitCount; submit++) {
            for (uint32_t semaphore = 0; semaphore < pSubmits[submit].waitSemaphoreCount; semaphore++) {
                CheckPipelineStageFlags("vkQueueSubmit", pSubmits[submit].pWaitDstStageMask[semaphore]);
            }
        }
        return VK_SUCCESS;
    };

    VkResult PreCallCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo,
                                     const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
        for (uint32_t dependency = 0; dependency < pCreateInfo->dependencyCount; dependency++) {
            CheckPipelineStageFlags("vkCreateRenderPass", pCreateInfo->pDependencies[dependency].srcStageMask);
            CheckPipelineStageFlags("vkCreateRenderPass", pCreateInfo->pDependencies[dependency].dstStageMask);
        }
        return VK_SUCCESS;
    };

    void PreCallCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
        CheckPipelineStageFlags("vkCmdSetEvent", stageMask);
    };

    void PreCallCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
        CheckPipelineStageFlags("vkCmdResetEvent", stageMask);
    };

    void PreCallCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents,
                              VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount,
                              const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                              const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                              const VkImageMemoryBarrier* pImageMemoryBarriers) {
        CheckPipelineStageFlags("vkCmdWaitEvents", srcStageMask);
        CheckPipelineStageFlags("vkCmdWaitEvents", dstStageMask);
    };

    void PreCallCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                   VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                                   uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers,
                                   uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers,
                                   uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
        CheckPipelineStageFlags("vkCmdPipelineBarrier", srcStageMask);
        CheckPipelineStageFlags("vkCmdPipelineBarrier", dstStageMask);
    };

    void PreCallCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool,
                                  uint32_t query) {
        CheckPipelineStageFlags("vkCmdWriteTimestamp", pipelineStage);
    };
};

WarnOnPipelineStageAll warn_stage_all;
