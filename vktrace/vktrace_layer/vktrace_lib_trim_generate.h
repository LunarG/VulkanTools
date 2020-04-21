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
#pragma once
#include "vktrace_trace_packet_identifiers.h"
#include "vulkan/vulkan.h"

namespace trim {
namespace generate {
vktrace_trace_packet_header *vkGetPhysicalDeviceSurfacePresentModesKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                       VkSurfaceKHR surface, uint32_t *pPresentModeCount,
                                                                       VkPresentModeKHR *pPresentModes);

vktrace_trace_packet_header *vkGetPhysicalDeviceSurfaceSupportKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                  uint32_t queueFamilyIndex, VkSurfaceKHR surface,
                                                                  VkBool32 *pSupported);
vktrace_trace_packet_header *vkGetPhysicalDeviceSurfaceCapabilitiesKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                       VkSurfaceKHR surface,
                                                                       VkSurfaceCapabilitiesKHR *pSurfaceCapabilities);

vktrace_trace_packet_header *vkGetPhysicalDeviceSurfaceFormatsKHR(bool makeCall, VkPhysicalDevice physicalDevice,
                                                                  VkSurfaceKHR surface, uint32_t *pSurfaceFormatCount,
                                                                  VkSurfaceFormatKHR *pSurfaceFormats);

vktrace_trace_packet_header *vkCreateCommandPool(bool makeCall, VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
                                                 const VkAllocationCallbacks *pAllocator, VkCommandPool *pCommandPool);

vktrace_trace_packet_header *vkAllocateCommandBuffers(bool makeCall, VkDevice device,
                                                      const VkCommandBufferAllocateInfo *pAllocateInfo,
                                                      VkCommandBuffer *pCommandBuffers);

vktrace_trace_packet_header *vkBeginCommandBuffer(bool makeCall, VkCommandBuffer commandBuffer,
                                                  const VkCommandBufferBeginInfo *pBeginInfo);

vktrace_trace_packet_header *vkCmdPipelineBarrier(bool makeCall, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                                  VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags,
                                                  uint32_t memoryBarrierCount, const VkMemoryBarrier *pMemoryBarriers,
                                                  uint32_t bufferMemoryBarrierCount,
                                                  const VkBufferMemoryBarrier *pBufferMemoryBarriers,
                                                  uint32_t imageMemoryBarrierCount,
                                                  const VkImageMemoryBarrier *pImageMemoryBarriers);

vktrace_trace_packet_header *vkEndCommandBuffer(bool makeCall, VkCommandBuffer commandBuffer);

vktrace_trace_packet_header *vkQueueSubmit(bool makeCall, VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits,
                                           VkFence fence);

vktrace_trace_packet_header *vkQueueWaitIdle(bool makeCall, VkQueue queue);

vktrace_trace_packet_header *vkResetCommandPool(bool makeCall, VkDevice device, VkCommandPool commandPool,
                                                VkCommandPoolResetFlags flags);

vktrace_trace_packet_header *vkFreeCommandBuffers(bool makeCall, VkDevice device, VkCommandPool commandPool,
                                                  uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers);

vktrace_trace_packet_header *vkDestroyCommandPool(bool makeCall, VkDevice device, VkCommandPool commandPool,
                                                  const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkMapMemory(bool makeCall, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset,
                                         VkDeviceSize size, VkMemoryMapFlags flags, void **ppData);

vktrace_trace_packet_header *vkUnmapMemory(bool makeCall, VkDeviceSize size, void *pData, VkDevice device, VkDeviceMemory memory);

vktrace_trace_packet_header *vkCreateBuffer(bool makeCall, VkDevice device, const VkBufferCreateInfo *pCreateInfo,
                                            const VkAllocationCallbacks *pAllocator, VkBuffer *pBuffer);

vktrace_trace_packet_header *vkGetBufferMemoryRequirements(bool makeCall, VkDevice device, VkBuffer buffer,
                                                           VkMemoryRequirements *pMemoryRequirements);

vktrace_trace_packet_header *vkAllocateMemory(bool makeCall, VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                              const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory);

vktrace_trace_packet_header *vkBindBufferMemory(bool makeCall, VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                                VkDeviceSize memoryOffset);

vktrace_trace_packet_header *vkCmdCopyBuffer(bool makeCall, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                                             uint32_t regionCount, const VkBufferCopy *pRegions);

vktrace_trace_packet_header *vkCreateFence(bool makeCall, VkDevice device, const VkFenceCreateInfo *pCreateInfo,
                                           const VkAllocationCallbacks *pAllocator, VkFence *pFence);

vktrace_trace_packet_header *vkWaitForFences(bool makeCall, VkDevice device, uint32_t fenceCount, const VkFence *pFences,
                                             VkBool32 waitAll, uint64_t timeout);

vktrace_trace_packet_header *vkResetFences(bool makeCall, VkDevice device, uint32_t fenceCount, const VkFence *pFences);

vktrace_trace_packet_header *vkResetEvent(bool makeCall, VkDevice device, VkEvent event);

vktrace_trace_packet_header *vkSetEvent(bool makeCall, VkDevice device, VkEvent event);

vktrace_trace_packet_header *vkDestroyBuffer(bool makeCall, VkDevice device, VkBuffer buffer,
                                             const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyBufferView(bool makeCall, VkDevice device, VkBufferView bufferView,
                                                 const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkFreeMemory(bool makeCall, VkDevice device, VkDeviceMemory memory,
                                          const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkAllocateDescriptorSets(bool makeCall, VkDevice device,
                                                      const VkDescriptorSetAllocateInfo *pAllocateInfo,
                                                      VkDescriptorSet *pDescriptorSets);

vktrace_trace_packet_header *vkUpdateDescriptorSets(bool makeCall, VkDevice device, uint32_t descriptorWriteCount,
                                                    const VkWriteDescriptorSet *pDescriptorWrites, uint32_t descriptorCopyCount,
                                                    const VkCopyDescriptorSet *pDescriptorCopies);

vktrace_trace_packet_header *vkResetDescriptorPool(bool makeCall, VkDevice device, VkDescriptorPool descriptorPool,
                                                   VkDescriptorPoolResetFlags flags);

vktrace_trace_packet_header *vkCmdBeginQuery(bool makeCall, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query,
                                             VkQueryControlFlags flags);

vktrace_trace_packet_header *vkCmdEndQuery(bool makeCall, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);

vktrace_trace_packet_header *vkCmdWriteTimestamp(bool makeCall, VkCommandBuffer commandBuffer,
                                                 VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);

vktrace_trace_packet_header *vkCmdResetQueryPool(bool makeCall, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                                                 uint32_t firstQuery, uint32_t queryCount);

vktrace_trace_packet_header *vkDestroyQueryPool(bool makeCall, VkDevice device, VkQueryPool queryPool,
                                                const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyEvent(bool makeCall, VkDevice device, VkEvent event, const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyFence(bool makeCall, VkDevice device, VkFence fence, const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroySemaphore(bool makeCall, VkDevice device, VkSemaphore semaphore,
                                                const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyFramebuffer(bool makeCall, VkDevice device, VkFramebuffer framebuffer,
                                                  const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyDescriptorPool(bool makeCall, VkDevice device, VkDescriptorPool descriptorPool,
                                                     const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkCreateRenderPass(bool makeCall, VkDevice device, const VkRenderPassCreateInfo *pCreateInfo,
                                                const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass);

vktrace_trace_packet_header *vkCreateGraphicsPipelines(bool makeCall, VkDevice device, VkPipelineCache pipelineCache,
                                                       uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo *pCreateInfos,
                                                       const VkAllocationCallbacks *pAllocator, VkPipeline *pPipelines);

vktrace_trace_packet_header *vkCreateComputePipelines(bool makeCall, VkDevice device, VkPipelineCache pipelineCache,
                                                      uint32_t createInfoCount, const VkComputePipelineCreateInfo *pCreateInfos,
                                                      const VkAllocationCallbacks *pAllocator, VkPipeline *pPipelines);

vktrace_trace_packet_header *vkDestroyPipeline(bool makeCall, VkDevice device, VkPipeline pipeline,
                                               const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyPipelineCache(bool makeCall, VkDevice device, VkPipelineCache pipelineCache,
                                                    const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyPipelineLayout(bool makeCall, VkDevice device, VkPipelineLayout pipelineLayout,
                                                     const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkCreateShaderModule(bool makeCall, VkDevice device, const VkShaderModuleCreateInfo *pCreateInfo,
                                                  const VkAllocationCallbacks *pAllocator, VkShaderModule *pShaderModule);

vktrace_trace_packet_header *vkDestroyShaderModule(bool makeCall, VkDevice device, VkShaderModule shaderModule,
                                                   const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyRenderPass(bool makeCall, VkDevice device, VkRenderPass renderPass,
                                                 const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyDescriptorSetLayout(bool makeCall, VkDevice device, VkDescriptorSetLayout descriptorSetLayout,
                                                          const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroySampler(bool makeCall, VkDevice device, VkSampler sampler,
                                              const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyImage(bool makeCall, VkDevice device, VkImage image, const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyImageView(bool makeCall, VkDevice device, VkImageView imageView,
                                                const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroySwapchainKHR(bool makeCall, VkDevice device, VkSwapchainKHR swapchain,
                                                   const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroySurfaceKHR(bool makeCall, VkInstance instance, VkSurfaceKHR surface,
                                                 const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyDevice(bool makeCall, VkDevice device, const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkDestroyInstance(bool makeCall, VkInstance instance, const VkAllocationCallbacks *pAllocator);

vktrace_trace_packet_header *vkCmdCopyBufferToImage(bool makeCall, VkCommandBuffer commandBuffer, VkBuffer srcBuffer,
                                                    VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                                    const VkBufferImageCopy *pRegions);
}  // namespace generate
}  // namespace trim
