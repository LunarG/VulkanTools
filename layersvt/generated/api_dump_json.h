
/* Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (c) 2015-2017, 2019 Google Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Joey Bzdek <joey@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: David Pinedo <david@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#pragma once

#include "api_dump.h"


//========================= Function Helpers ================================//

void dump_json_params_vkCreateInstance(ApiDumpInstance& dump_inst, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void dump_json_params_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void dump_json_params_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
void dump_json_params_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void dump_json_params_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void dump_json_params_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void dump_json_params_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void dump_json_params_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void dump_json_params_vkCreateDevice(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
void dump_json_params_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_json_params_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_json_params_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_json_params_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_json_params_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void dump_json_params_vkQueueSubmit(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
void dump_json_params_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_json_params_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_params_vkAllocateMemory(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
void dump_json_params_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkMapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void dump_json_params_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory);
void dump_json_params_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_json_params_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_json_params_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
void dump_json_params_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_json_params_vkBindImageMemory(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_json_params_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
void dump_json_params_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void dump_json_params_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void dump_json_params_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void dump_json_params_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);
void dump_json_params_vkCreateFence(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_params_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkResetFences(ApiDumpInstance& dump_inst, VkDevice device, uint32_t fenceCount, const VkFence* pFences);
void dump_json_params_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence);
void dump_json_params_vkWaitForFences(ApiDumpInstance& dump_inst, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
void dump_json_params_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
void dump_json_params_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateEvent(ApiDumpInstance& dump_inst, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void dump_json_params_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetEventStatus(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event);
void dump_json_params_vkSetEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event);
void dump_json_params_vkResetEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event);
void dump_json_params_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
void dump_json_params_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_json_params_vkCreateBuffer(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
void dump_json_params_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateBufferView(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void dump_json_params_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateImage(ApiDumpInstance& dump_inst, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
void dump_json_params_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void dump_json_params_vkCreateImageView(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
void dump_json_params_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void dump_json_params_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void dump_json_params_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void dump_json_params_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);
void dump_json_params_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_params_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_params_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void dump_json_params_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateSampler(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void dump_json_params_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void dump_json_params_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void dump_json_params_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
void dump_json_params_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
void dump_json_params_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
void dump_json_params_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
void dump_json_params_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void dump_json_params_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_params_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void dump_json_params_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void dump_json_params_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkResetCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void dump_json_params_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void dump_json_params_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_json_params_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
void dump_json_params_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void dump_json_params_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_json_params_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_params_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_params_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth);
void dump_json_params_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void dump_json_params_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void dump_json_params_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void dump_json_params_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void dump_json_params_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void dump_json_params_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void dump_json_params_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
void dump_json_params_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void dump_json_params_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void dump_json_params_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void dump_json_params_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void dump_json_params_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_params_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_params_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_params_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_json_params_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
void dump_json_params_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
void dump_json_params_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
void dump_json_params_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_json_params_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_json_params_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void dump_json_params_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void dump_json_params_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_json_params_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_json_params_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void dump_json_params_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);
void dump_json_params_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_json_params_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_json_params_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_json_params_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_json_params_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void dump_json_params_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void dump_json_params_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_params_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void dump_json_params_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_json_params_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void dump_json_params_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void dump_json_params_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void dump_json_params_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_json_params_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_json_params_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_json_params_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_json_params_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_json_params_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_params_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_json_params_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_params_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_json_params_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_json_params_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_json_params_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_json_params_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_json_params_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_json_params_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_json_params_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_json_params_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void dump_json_params_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_json_params_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_json_params_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_json_params_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
void dump_json_params_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
void dump_json_params_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void dump_json_params_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_json_params_vkCmdDrawIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkCmdDrawIndexedIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkCreateRenderPass2(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_params_vkCmdBeginRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo);
void dump_json_params_vkCmdNextSubpass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_params_vkCmdEndRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_params_vkResetQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_params_vkGetSemaphoreCounterValue(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_json_params_vkWaitSemaphores(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_json_params_vkSignalSemaphore(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_json_params_vkGetBufferDeviceAddress(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_params_vkGetBufferOpaqueCaptureAddress(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_params_vkGetDeviceMemoryOpaqueCaptureAddress(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_json_params_vkGetPhysicalDeviceToolProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_json_params_vkCreatePrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_json_params_vkDestroyPrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkSetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_json_params_vkGetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_json_params_vkCmdSetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo);
void dump_json_params_vkCmdResetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);
void dump_json_params_vkCmdWaitEvents2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos);
void dump_json_params_vkCmdPipelineBarrier2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo);
void dump_json_params_vkCmdWriteTimestamp2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);
void dump_json_params_vkQueueSubmit2(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);
void dump_json_params_vkCmdCopyBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_json_params_vkCmdCopyImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_json_params_vkCmdCopyBufferToImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_json_params_vkCmdCopyImageToBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_json_params_vkCmdBlitImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_json_params_vkCmdResolveImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_json_params_vkCmdBeginRendering(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo);
void dump_json_params_vkCmdEndRendering(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkCmdSetCullMode(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_json_params_vkCmdSetFrontFace(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_json_params_vkCmdSetPrimitiveTopology(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_json_params_vkCmdSetViewportWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_params_vkCmdSetScissorWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_params_vkCmdBindVertexBuffers2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_json_params_vkCmdSetDepthTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_json_params_vkCmdSetDepthWriteEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_json_params_vkCmdSetDepthCompareOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_json_params_vkCmdSetDepthBoundsTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_json_params_vkCmdSetStencilTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_json_params_vkCmdSetStencilOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_json_params_vkCmdSetRasterizerDiscardEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_json_params_vkCmdSetDepthBiasEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_json_params_vkCmdSetPrimitiveRestartEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
void dump_json_params_vkGetDeviceBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetDeviceImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetDeviceImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_params_vkCmdSetLineStipple(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_params_vkMapMemory2(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_json_params_vkUnmapMemory2(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_json_params_vkCmdBindIndexBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_json_params_vkGetRenderingAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_json_params_vkGetDeviceImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_json_params_vkGetImageSubresourceLayout2(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_params_vkCmdPushDescriptorSet(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_json_params_vkCmdPushDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_json_params_vkCmdSetRenderingAttachmentLocations(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_json_params_vkCmdSetRenderingInputAttachmentIndices(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_json_params_vkCmdBindDescriptorSets2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo);
void dump_json_params_vkCmdPushConstants2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo* pPushConstantsInfo);
void dump_json_params_vkCmdPushDescriptorSet2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo* pPushDescriptorSetInfo);
void dump_json_params_vkCmdPushDescriptorSetWithTemplate2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_json_params_vkCopyMemoryToImage(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyMemoryToImageInfo* pCopyMemoryToImageInfo);
void dump_json_params_vkCopyImageToMemory(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToMemoryInfo* pCopyImageToMemoryInfo);
void dump_json_params_vkCopyImageToImage(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToImageInfo* pCopyImageToImageInfo);
void dump_json_params_vkTransitionImageLayout(ApiDumpInstance& dump_inst, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo* pTransitions);
void dump_json_params_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void dump_json_params_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
void dump_json_params_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
void dump_json_params_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
void dump_json_params_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
void dump_json_params_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void dump_json_params_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
void dump_json_params_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
void dump_json_params_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
void dump_json_params_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
void dump_json_params_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
void dump_json_params_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
void dump_json_params_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
void dump_json_params_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
void dump_json_params_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
void dump_json_params_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
void dump_json_params_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
void dump_json_params_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
void dump_json_params_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_json_params_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_params_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_params_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_params_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_params_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_params_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_params_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_params_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_params_vkGetPhysicalDeviceVideoCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
void dump_json_params_vkGetPhysicalDeviceVideoFormatPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
void dump_json_params_vkCreateVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);
void dump_json_params_vkDestroyVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetVideoSessionMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);
void dump_json_params_vkBindVideoSessionMemoryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);
void dump_json_params_vkCreateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);
void dump_json_params_vkUpdateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);
void dump_json_params_vkDestroyVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCmdBeginVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo);
void dump_json_params_vkCmdEndVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo);
void dump_json_params_vkCmdControlVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo);
void dump_json_params_vkCmdDecodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo);
void dump_json_params_vkCmdBeginRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo);
void dump_json_params_vkCmdEndRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_json_params_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_json_params_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_json_params_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_json_params_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_json_params_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_json_params_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_json_params_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_json_params_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_json_params_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_params_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_json_params_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_json_params_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_params_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_params_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);
void dump_json_params_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_params_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
void dump_json_params_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_params_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_json_params_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_json_params_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_json_params_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_json_params_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_params_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo);
void dump_json_params_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_params_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_params_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain);
void dump_json_params_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_params_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo);
void dump_json_params_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_params_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
void dump_json_params_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
void dump_json_params_vkAcquireProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo);
void dump_json_params_vkReleaseProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_params_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
void dump_json_params_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
void dump_json_params_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
void dump_json_params_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
void dump_json_params_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
void dump_json_params_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
void dump_json_params_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_params_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_json_params_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_json_params_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_json_params_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_json_params_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkGetSemaphoreCounterValueKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_json_params_vkWaitSemaphoresKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_json_params_vkSignalSemaphoreKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_json_params_vkGetPhysicalDeviceFragmentShadingRatesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
void dump_json_params_vkCmdSetFragmentShadingRateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]);
void dump_json_params_vkCmdSetRenderingAttachmentLocationsKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_json_params_vkCmdSetRenderingInputAttachmentIndicesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_json_params_vkWaitForPresentKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);
void dump_json_params_vkGetBufferDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_params_vkGetBufferOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_params_vkGetDeviceMemoryOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_json_params_vkCreateDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);
void dump_json_params_vkDestroyDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetDeferredOperationMaxConcurrencyKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_params_vkGetDeferredOperationResultKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_params_vkDeferredOperationJoinKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_params_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);
void dump_json_params_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);
void dump_json_params_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);
void dump_json_params_vkMapMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_json_params_vkUnmapMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_json_params_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);
void dump_json_params_vkGetEncodedVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);
void dump_json_params_vkCmdEncodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo);
void dump_json_params_vkCmdSetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo);
void dump_json_params_vkCmdResetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);
void dump_json_params_vkCmdWaitEvents2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos);
void dump_json_params_vkCmdPipelineBarrier2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo);
void dump_json_params_vkCmdWriteTimestamp2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);
void dump_json_params_vkQueueSubmit2KHR(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);
void dump_json_params_vkCmdCopyBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_json_params_vkCmdCopyImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_json_params_vkCmdCopyBufferToImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_json_params_vkCmdCopyImageToBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_json_params_vkCmdBlitImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_json_params_vkCmdResolveImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_json_params_vkCmdTraceRaysIndirect2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);
void dump_json_params_vkGetDeviceBufferMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetDeviceImageMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkGetDeviceImageSparseMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_params_vkCmdBindIndexBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_json_params_vkGetRenderingAreaGranularityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_json_params_vkGetDeviceImageSubresourceLayoutKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_json_params_vkGetImageSubresourceLayout2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_params_vkWaitForPresent2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkPresentWait2InfoKHR* pPresentWait2Info);
void dump_json_params_vkCreatePipelineBinariesKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries);
void dump_json_params_vkDestroyPipelineBinaryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetPipelineKeyKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey);
void dump_json_params_vkGetPipelineBinaryDataKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData);
void dump_json_params_vkReleaseCapturedPipelineDataKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
void dump_json_params_vkCmdSetLineStippleKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_params_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_json_params_vkGetCalibratedTimestampsKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_json_params_vkCmdBindDescriptorSets2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo);
void dump_json_params_vkCmdPushConstants2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo* pPushConstantsInfo);
void dump_json_params_vkCmdPushDescriptorSet2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo* pPushDescriptorSetInfo);
void dump_json_params_vkCmdPushDescriptorSetWithTemplate2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_json_params_vkCmdSetDescriptorBufferOffsets2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);
void dump_json_params_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);
void dump_json_params_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void dump_json_params_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
void dump_json_params_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo);
void dump_json_params_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo);
void dump_json_params_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_json_params_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_json_params_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes);
void dump_json_params_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_json_params_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_json_params_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);
void dump_json_params_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);
void dump_json_params_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);
void dump_json_params_vkCreateCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);
void dump_json_params_vkCreateCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);
void dump_json_params_vkDestroyCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkDestroyCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCmdCuLaunchKernelNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);
void dump_json_params_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_json_params_vkGetImageViewHandle64NVX(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_json_params_vkGetImageViewAddressNVX(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);
void dump_json_params_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_params_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
void dump_json_params_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_json_params_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
void dump_json_params_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);
void dump_json_params_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void dump_json_params_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_json_params_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_json_params_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
void dump_json_params_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
void dump_json_params_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);
void dump_json_params_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_params_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_params_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);
void dump_json_params_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);
void dump_json_params_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);
void dump_json_params_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles);
void dump_json_params_vkCmdSetDiscardRectangleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);
void dump_json_params_vkCmdSetDiscardRectangleModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);
void dump_json_params_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_json_params_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_json_params_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_json_params_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
void dump_json_params_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo);
void dump_json_params_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_params_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_json_params_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_params_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_params_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_params_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void dump_json_params_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_params_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_params_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCreateExecutionGraphPipelinesAMDX(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkGetExecutionGraphPipelineScratchSizeAMDX(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkGetExecutionGraphPipelineNodeIndexAMDX(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCmdInitializeGraphScratchMemoryAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipeline executionGraph, VkDeviceAddress scratch, VkDeviceSize scratchSize);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCmdDispatchGraphAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceSize scratchSize, const VkDispatchGraphCountInfoAMDX* pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCmdDispatchGraphIndirectAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceSize scratchSize, const VkDispatchGraphCountInfoAMDX* pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCmdDispatchGraphIndirectCountAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceSize scratchSize, VkDeviceAddress countInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_params_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);
void dump_json_params_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
void dump_json_params_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);
void dump_json_params_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);
void dump_json_params_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);
void dump_json_params_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);
void dump_json_params_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_json_params_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes);
void dump_json_params_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);
void dump_json_params_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);
void dump_json_params_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);
void dump_json_params_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);
void dump_json_params_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);
void dump_json_params_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);
void dump_json_params_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);
void dump_json_params_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_params_vkGetRayTracingShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_params_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_params_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);
void dump_json_params_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_params_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t shader);
void dump_json_params_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);
void dump_json_params_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_json_params_vkCmdWriteBufferMarker2AMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_json_params_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_json_params_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_json_params_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);
void dump_json_params_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_params_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_params_vkCmdSetExclusiveScissorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables);
void dump_json_params_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors);
void dump_json_params_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker);
void dump_json_params_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);
void dump_json_params_vkGetQueueCheckpointData2NV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);
void dump_json_params_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);
void dump_json_params_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_params_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);
void dump_json_params_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);
void dump_json_params_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo);
void dump_json_params_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);
void dump_json_params_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkDevice device, VkPerformanceConfigurationINTEL configuration);
void dump_json_params_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkQueue queue, VkPerformanceConfigurationINTEL configuration);
void dump_json_params_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);
void dump_json_params_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_params_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_params_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_params_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_json_params_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
void dump_json_params_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_params_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_json_params_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_params_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_params_vkCmdSetCullModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_json_params_vkCmdSetFrontFaceEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_json_params_vkCmdSetPrimitiveTopologyEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_json_params_vkCmdSetViewportWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_params_vkCmdSetScissorWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_params_vkCmdBindVertexBuffers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_json_params_vkCmdSetDepthTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_json_params_vkCmdSetDepthWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_json_params_vkCmdSetDepthCompareOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_json_params_vkCmdSetDepthBoundsTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_json_params_vkCmdSetStencilTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_json_params_vkCmdSetStencilOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_json_params_vkCopyMemoryToImageEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyMemoryToImageInfo* pCopyMemoryToImageInfo);
void dump_json_params_vkCopyImageToMemoryEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToMemoryInfo* pCopyImageToMemoryInfo);
void dump_json_params_vkCopyImageToImageEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToImageInfo* pCopyImageToImageInfo);
void dump_json_params_vkTransitionImageLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo* pTransitions);
void dump_json_params_vkGetImageSubresourceLayout2EXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_params_vkReleaseSwapchainImagesEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);
void dump_json_params_vkGetGeneratedCommandsMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkCmdPreprocessGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_json_params_vkCmdExecuteGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_json_params_vkCmdBindPipelineShaderGroupNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);
void dump_json_params_vkCreateIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);
void dump_json_params_vkDestroyIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCmdSetDepthBias2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT* pDepthBiasInfo);
void dump_json_params_vkAcquireDrmDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
void dump_json_params_vkGetDrmDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
void dump_json_params_vkCreatePrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_json_params_vkDestroyPrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkSetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_json_params_vkGetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCreateCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkGetCudaModuleCacheNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCreateCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkDestroyCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkDestroyCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_params_vkCmdCudaLaunchKernelNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_params_vkCmdDispatchTileQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDispatchTileInfoQCOM* pDispatchTileInfo);
void dump_json_params_vkCmdBeginPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileBeginInfoQCOM* pPerTileBeginInfo);
void dump_json_params_vkCmdEndPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileEndInfoQCOM* pPerTileEndInfo);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_params_vkExportMetalObjectsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_params_vkGetDescriptorSetLayoutSizeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);
void dump_json_params_vkGetDescriptorSetLayoutBindingOffsetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);
void dump_json_params_vkGetDescriptorEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);
void dump_json_params_vkCmdBindDescriptorBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos);
void dump_json_params_vkCmdSetDescriptorBufferOffsetsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets);
void dump_json_params_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);
void dump_json_params_vkGetBufferOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_params_vkGetImageOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_params_vkGetImageViewOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_params_vkGetSamplerOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_params_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_params_vkCmdSetFragmentShadingRateEnumNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]);
void dump_json_params_vkGetDeviceFaultInfoEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkAcquireWinrtDisplayNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_params_vkGetWinrtDisplayNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_params_vkCreateDirectFBSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_params_vkGetPhysicalDeviceDirectFBPresentationSupportEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_json_params_vkCmdSetVertexInputEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkGetMemoryZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkGetMemoryZirconHandlePropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkImportSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkGetSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkCreateBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkSetBufferCollectionImageConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkSetBufferCollectionBufferConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkDestroyBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_params_vkGetBufferCollectionPropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_params_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);
void dump_json_params_vkCmdSubpassShadingHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_params_vkCmdBindInvocationMaskHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_json_params_vkGetMemoryRemoteAddressNV(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);
void dump_json_params_vkGetPipelinePropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);
void dump_json_params_vkCmdSetPatchControlPointsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t patchControlPoints);
void dump_json_params_vkCmdSetRasterizerDiscardEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_json_params_vkCmdSetDepthBiasEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_json_params_vkCmdSetLogicOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLogicOp logicOp);
void dump_json_params_vkCmdSetPrimitiveRestartEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_params_vkCreateScreenSurfaceQNX(ApiDumpInstance& dump_inst, VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_params_vkGetPhysicalDeviceScreenPresentationSupportQNX(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_params_vkCmdSetColorWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables);
void dump_json_params_vkCmdDrawMultiEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);
void dump_json_params_vkCmdDrawMultiIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset);
void dump_json_params_vkCreateMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap);
void dump_json_params_vkDestroyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCmdBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_json_params_vkBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_json_params_vkCopyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo);
void dump_json_params_vkCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_json_params_vkCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_json_params_vkWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);
void dump_json_params_vkCmdCopyMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo);
void dump_json_params_vkCmdCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_json_params_vkCmdCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_json_params_vkCmdWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_params_vkGetDeviceMicromapCompatibilityEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_json_params_vkGetMicromapBuildSizesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo);
void dump_json_params_vkCmdDrawClusterHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_params_vkCmdDrawClusterIndirectHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_json_params_vkSetDeviceMemoryPriorityEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, float priority);
void dump_json_params_vkGetDescriptorSetLayoutHostMappingInfoVALVE(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);
void dump_json_params_vkGetDescriptorSetHostMappingVALVE(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, void** ppData);
void dump_json_params_vkCmdCopyMemoryIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);
void dump_json_params_vkCmdCopyMemoryToImageIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources);
void dump_json_params_vkCmdDecompressMemoryNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions);
void dump_json_params_vkCmdDecompressMemoryIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);
void dump_json_params_vkGetPipelineIndirectMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkCmdUpdatePipelineIndirectBufferNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_json_params_vkGetPipelineIndirectDeviceAddressNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo);
void dump_json_params_vkCmdSetDepthClampEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);
void dump_json_params_vkCmdSetPolygonModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);
void dump_json_params_vkCmdSetRasterizationSamplesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples);
void dump_json_params_vkCmdSetSampleMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, const VkSampleMask* pSampleMask);
void dump_json_params_vkCmdSetAlphaToCoverageEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);
void dump_json_params_vkCmdSetAlphaToOneEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);
void dump_json_params_vkCmdSetLogicOpEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);
void dump_json_params_vkCmdSetColorBlendEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables);
void dump_json_params_vkCmdSetColorBlendEquationEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations);
void dump_json_params_vkCmdSetColorWriteMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks);
void dump_json_params_vkCmdSetTessellationDomainOriginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);
void dump_json_params_vkCmdSetRasterizationStreamEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t rasterizationStream);
void dump_json_params_vkCmdSetConservativeRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);
void dump_json_params_vkCmdSetExtraPrimitiveOverestimationSizeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);
void dump_json_params_vkCmdSetDepthClipEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);
void dump_json_params_vkCmdSetSampleLocationsEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);
void dump_json_params_vkCmdSetColorBlendAdvancedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced);
void dump_json_params_vkCmdSetProvokingVertexModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);
void dump_json_params_vkCmdSetLineRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);
void dump_json_params_vkCmdSetLineStippleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);
void dump_json_params_vkCmdSetDepthClipNegativeOneToOneEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);
void dump_json_params_vkCmdSetViewportWScalingEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);
void dump_json_params_vkCmdSetViewportSwizzleNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles);
void dump_json_params_vkCmdSetCoverageToColorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);
void dump_json_params_vkCmdSetCoverageToColorLocationNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);
void dump_json_params_vkCmdSetCoverageModulationModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);
void dump_json_params_vkCmdSetCoverageModulationTableEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);
void dump_json_params_vkCmdSetCoverageModulationTableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable);
void dump_json_params_vkCmdSetShadingRateImageEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);
void dump_json_params_vkCmdSetRepresentativeFragmentTestEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);
void dump_json_params_vkCmdSetCoverageReductionModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);
void dump_json_params_vkCreateTensorARM(ApiDumpInstance& dump_inst, VkDevice device, const VkTensorCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkTensorARM* pTensor);
void dump_json_params_vkDestroyTensorARM(ApiDumpInstance& dump_inst, VkDevice device, VkTensorARM tensor, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateTensorViewARM(ApiDumpInstance& dump_inst, VkDevice device, const VkTensorViewCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkTensorViewARM* pView);
void dump_json_params_vkDestroyTensorViewARM(ApiDumpInstance& dump_inst, VkDevice device, VkTensorViewARM tensorView, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetTensorMemoryRequirementsARM(ApiDumpInstance& dump_inst, VkDevice device, const VkTensorMemoryRequirementsInfoARM* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkBindTensorMemoryARM(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindTensorMemoryInfoARM* pBindInfos);
void dump_json_params_vkGetDeviceTensorMemoryRequirementsARM(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceTensorMemoryRequirementsARM* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkCmdCopyTensorARM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyTensorInfoARM* pCopyTensorInfo);
void dump_json_params_vkGetPhysicalDeviceExternalTensorPropertiesARM(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalTensorInfoARM* pExternalTensorInfo, VkExternalTensorPropertiesARM* pExternalTensorProperties);
void dump_json_params_vkGetTensorOpaqueCaptureDescriptorDataARM(ApiDumpInstance& dump_inst, VkDevice device, const VkTensorCaptureDescriptorDataInfoARM* pInfo, void* pData);
void dump_json_params_vkGetTensorViewOpaqueCaptureDescriptorDataARM(ApiDumpInstance& dump_inst, VkDevice device, const VkTensorViewCaptureDescriptorDataInfoARM* pInfo, void* pData);
void dump_json_params_vkGetShaderModuleIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_json_params_vkGetShaderModuleCreateInfoIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_json_params_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
void dump_json_params_vkCreateOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);
void dump_json_params_vkDestroyOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkBindOpticalFlowSessionImageNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);
void dump_json_params_vkCmdOpticalFlowExecuteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo);
void dump_json_params_vkAntiLagUpdateAMD(ApiDumpInstance& dump_inst, VkDevice device, const VkAntiLagDataAMD* pData);
void dump_json_params_vkCreateShadersEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);
void dump_json_params_vkDestroyShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetShaderBinaryDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);
void dump_json_params_vkCmdBindShadersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders);
void dump_json_params_vkCmdSetDepthClampRangeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange);
void dump_json_params_vkGetFramebufferTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);
void dump_json_params_vkGetDynamicRenderingTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);
void dump_json_params_vkGetPhysicalDeviceCooperativeVectorPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeVectorPropertiesNV* pProperties);
void dump_json_params_vkConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkDevice device, const VkConvertCooperativeVectorMatrixInfoNV* pInfo);
void dump_json_params_vkCmdConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkConvertCooperativeVectorMatrixInfoNV* pInfos);
void dump_json_params_vkSetLatencySleepModeNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo);
void dump_json_params_vkLatencySleepNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo);
void dump_json_params_vkSetLatencyMarkerNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_json_params_vkGetLatencyTimingsNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_json_params_vkQueueNotifyOutOfBandNV(ApiDumpInstance& dump_inst, VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);
void dump_json_params_vkCmdSetAttachmentFeedbackLoopEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_params_vkGetScreenBufferPropertiesQNX(ApiDumpInstance& dump_inst, VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_params_vkCmdBindTileMemoryQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkTileMemoryBindInfoQCOM* pTileMemoryBindInfo);
void dump_json_params_vkCreateExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, const VkExternalComputeQueueCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkExternalComputeQueueNV* pExternalQueue);
void dump_json_params_vkDestroyExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, VkExternalComputeQueueNV externalQueue, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkGetExternalComputeQueueDataNV(ApiDumpInstance& dump_inst, VkExternalComputeQueueNV externalQueue, VkExternalComputeQueueDataParamsNV* params, void* pData);
void dump_json_params_vkGetClusterAccelerationStructureBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkClusterAccelerationStructureInputInfoNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_json_params_vkCmdBuildClusterAccelerationStructureIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkClusterAccelerationStructureCommandsInfoNV* pCommandInfos);
void dump_json_params_vkGetPartitionedAccelerationStructuresBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPartitionedAccelerationStructureInstancesInputNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_json_params_vkCmdBuildPartitionedAccelerationStructuresNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBuildPartitionedAccelerationStructureInfoNV* pBuildInfo);
void dump_json_params_vkGetGeneratedCommandsMemoryRequirementsEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_params_vkCmdPreprocessGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer);
void dump_json_params_vkCmdExecuteGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo);
void dump_json_params_vkCreateIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout);
void dump_json_params_vkDestroyIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCreateIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet);
void dump_json_params_vkDestroyIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkUpdateIndirectExecutionSetPipelineEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites);
void dump_json_params_vkUpdateIndirectExecutionSetShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites);
#if defined(VK_USE_PLATFORM_OHOS)
void dump_json_params_vkCreateSurfaceOHOS(ApiDumpInstance& dump_inst, VkInstance instance, const VkSurfaceCreateInfoOHOS* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_OHOS
void dump_json_params_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_params_vkGetMemoryMetalHandleEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetMetalHandleInfoEXT* pGetMetalHandleInfo, void** pHandle);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_params_vkGetMemoryMetalHandlePropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHandle, VkMemoryMetalHandlePropertiesEXT* pMemoryMetalHandleProperties);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_params_vkCmdEndRendering2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingEndInfoEXT* pRenderingEndInfo);
void dump_json_params_vkCreateAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure);
void dump_json_params_vkDestroyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_json_params_vkCmdBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_json_params_vkCmdBuildAccelerationStructuresIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts);
void dump_json_params_vkBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_json_params_vkCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_json_params_vkCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_json_params_vkCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_json_params_vkWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);
void dump_json_params_vkCmdCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_json_params_vkCmdCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_json_params_vkCmdCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_json_params_vkGetAccelerationStructureDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
void dump_json_params_vkCmdWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_params_vkGetDeviceAccelerationStructureCompatibilityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_json_params_vkGetAccelerationStructureBuildSizesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_json_params_vkCmdTraceRaysKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
void dump_json_params_vkCreateRayTracingPipelinesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_params_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_params_vkCmdTraceRaysIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);
void dump_json_params_vkGetRayTracingShaderGroupStackSizeKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);
void dump_json_params_vkCmdSetRayTracingPipelineStackSizeKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);
void dump_json_params_vkCmdDrawMeshTasksEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_params_vkCmdDrawMeshTasksIndirectEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_params_vkCmdDrawMeshTasksIndirectCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

//========================= Function Implementations ========================//

void dump_json_vkCreateInstance(ApiDumpInstance& dump_inst, VkResult result, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void dump_json_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator);
void dump_json_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void dump_json_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
void dump_json_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void dump_json_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void dump_json_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void dump_json_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void dump_json_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void dump_json_vkCreateDevice(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
void dump_json_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator);
void dump_json_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_json_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_json_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_json_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_json_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void dump_json_vkQueueSubmit(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
void dump_json_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue);
void dump_json_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkDevice device);
void dump_json_vkAllocateMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
void dump_json_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
void dump_json_vkMapMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void dump_json_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory);
void dump_json_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_json_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_json_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
void dump_json_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_json_vkBindImageMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_json_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
void dump_json_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void dump_json_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void dump_json_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void dump_json_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);
void dump_json_vkCreateFence(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
void dump_json_vkResetFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences);
void dump_json_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFence fence);
void dump_json_vkWaitForFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
void dump_json_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
void dump_json_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void dump_json_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetEventStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_json_vkSetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_json_vkResetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_json_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
void dump_json_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_json_vkCreateBuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
void dump_json_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateBufferView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void dump_json_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
void dump_json_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void dump_json_vkCreateImageView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
void dump_json_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void dump_json_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void dump_json_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void dump_json_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);
void dump_json_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void dump_json_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateSampler(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void dump_json_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void dump_json_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void dump_json_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
void dump_json_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
void dump_json_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
void dump_json_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
void dump_json_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
void dump_json_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void dump_json_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void dump_json_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void dump_json_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
void dump_json_vkResetCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void dump_json_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void dump_json_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_json_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
void dump_json_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer);
void dump_json_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void dump_json_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_json_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth);
void dump_json_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void dump_json_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void dump_json_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void dump_json_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void dump_json_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void dump_json_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void dump_json_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
void dump_json_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void dump_json_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void dump_json_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void dump_json_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void dump_json_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_json_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
void dump_json_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
void dump_json_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
void dump_json_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_json_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_json_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void dump_json_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void dump_json_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_json_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_json_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void dump_json_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);
void dump_json_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_json_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_json_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_json_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_json_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void dump_json_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void dump_json_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void dump_json_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_json_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void dump_json_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void dump_json_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void dump_json_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_json_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_json_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_json_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_json_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_json_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_json_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_json_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_json_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_json_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_json_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_json_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_json_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_json_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_json_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void dump_json_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_json_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_json_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_json_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_json_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
void dump_json_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
void dump_json_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void dump_json_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_json_vkCmdDrawIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCreateRenderPass2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_vkCmdBeginRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo);
void dump_json_vkCmdNextSubpass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_vkCmdEndRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_vkResetQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_vkGetSemaphoreCounterValue(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_json_vkWaitSemaphores(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_json_vkSignalSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_json_vkGetBufferDeviceAddress(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetBufferOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetDeviceMemoryOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_json_vkGetPhysicalDeviceToolProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_json_vkCreatePrivateDataSlot(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_json_vkDestroyPrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_json_vkSetPrivateData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_json_vkGetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_json_vkCmdSetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo);
void dump_json_vkCmdResetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);
void dump_json_vkCmdWaitEvents2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos);
void dump_json_vkCmdPipelineBarrier2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo);
void dump_json_vkCmdWriteTimestamp2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);
void dump_json_vkQueueSubmit2(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);
void dump_json_vkCmdCopyBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_json_vkCmdCopyImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_json_vkCmdCopyBufferToImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_json_vkCmdCopyImageToBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_json_vkCmdBlitImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_json_vkCmdResolveImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_json_vkCmdBeginRendering(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo);
void dump_json_vkCmdEndRendering(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdSetCullMode(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_json_vkCmdSetFrontFace(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_json_vkCmdSetPrimitiveTopology(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_json_vkCmdSetViewportWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_vkCmdSetScissorWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_vkCmdBindVertexBuffers2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_json_vkCmdSetDepthTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_json_vkCmdSetDepthWriteEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_json_vkCmdSetDepthCompareOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_json_vkCmdSetDepthBoundsTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_json_vkCmdSetStencilTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_json_vkCmdSetStencilOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_json_vkCmdSetRasterizerDiscardEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_json_vkCmdSetDepthBiasEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_json_vkCmdSetPrimitiveRestartEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
void dump_json_vkGetDeviceBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkCmdSetLineStipple(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_vkMapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_json_vkUnmapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_json_vkCmdBindIndexBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_json_vkGetRenderingAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_json_vkGetDeviceImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_json_vkGetImageSubresourceLayout2(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_vkCmdPushDescriptorSet(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_json_vkCmdPushDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_json_vkCmdSetRenderingAttachmentLocations(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_json_vkCmdSetRenderingInputAttachmentIndices(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_json_vkCmdBindDescriptorSets2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo);
void dump_json_vkCmdPushConstants2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo* pPushConstantsInfo);
void dump_json_vkCmdPushDescriptorSet2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo* pPushDescriptorSetInfo);
void dump_json_vkCmdPushDescriptorSetWithTemplate2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_json_vkCopyMemoryToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo* pCopyMemoryToImageInfo);
void dump_json_vkCopyImageToMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo* pCopyImageToMemoryInfo);
void dump_json_vkCopyImageToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo* pCopyImageToImageInfo);
void dump_json_vkTransitionImageLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo* pTransitions);
void dump_json_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void dump_json_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
void dump_json_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
void dump_json_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
void dump_json_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
void dump_json_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void dump_json_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
void dump_json_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
void dump_json_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
void dump_json_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
void dump_json_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
void dump_json_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
void dump_json_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
void dump_json_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
void dump_json_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
void dump_json_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
void dump_json_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
void dump_json_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
void dump_json_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_json_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkGetPhysicalDeviceVideoCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
void dump_json_vkGetPhysicalDeviceVideoFormatPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
void dump_json_vkCreateVideoSessionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);
void dump_json_vkDestroyVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetVideoSessionMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);
void dump_json_vkBindVideoSessionMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);
void dump_json_vkCreateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);
void dump_json_vkUpdateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);
void dump_json_vkDestroyVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdBeginVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo);
void dump_json_vkCmdEndVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo);
void dump_json_vkCmdControlVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo);
void dump_json_vkCmdDecodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo);
void dump_json_vkCmdBeginRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo);
void dump_json_vkCmdEndRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_json_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_json_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_json_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_json_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_json_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_json_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_json_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_json_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_json_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_json_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_json_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);
void dump_json_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
void dump_json_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_json_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_json_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_json_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_json_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_json_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo);
void dump_json_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo);
void dump_json_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
void dump_json_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo);
void dump_json_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
void dump_json_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
void dump_json_vkAcquireProfilingLockKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo);
void dump_json_vkReleaseProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
void dump_json_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
void dump_json_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
void dump_json_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
void dump_json_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
void dump_json_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
void dump_json_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_json_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_json_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_json_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_json_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_json_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkGetSemaphoreCounterValueKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_json_vkWaitSemaphoresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_json_vkSignalSemaphoreKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_json_vkGetPhysicalDeviceFragmentShadingRatesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
void dump_json_vkCmdSetFragmentShadingRateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]);
void dump_json_vkCmdSetRenderingAttachmentLocationsKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_json_vkCmdSetRenderingInputAttachmentIndicesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_json_vkWaitForPresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);
void dump_json_vkGetBufferDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetBufferOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetDeviceMemoryOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_json_vkCreateDeferredOperationKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);
void dump_json_vkDestroyDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetDeferredOperationMaxConcurrencyKHR(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_vkGetDeferredOperationResultKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_vkDeferredOperationJoinKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);
void dump_json_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);
void dump_json_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);
void dump_json_vkMapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_json_vkUnmapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_json_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);
void dump_json_vkGetEncodedVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);
void dump_json_vkCmdEncodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo);
void dump_json_vkCmdSetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo);
void dump_json_vkCmdResetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);
void dump_json_vkCmdWaitEvents2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos);
void dump_json_vkCmdPipelineBarrier2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo);
void dump_json_vkCmdWriteTimestamp2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);
void dump_json_vkQueueSubmit2KHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence);
void dump_json_vkCmdCopyBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_json_vkCmdCopyImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_json_vkCmdCopyBufferToImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_json_vkCmdCopyImageToBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_json_vkCmdBlitImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_json_vkCmdResolveImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_json_vkCmdTraceRaysIndirect2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);
void dump_json_vkGetDeviceBufferMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageSparseMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkCmdBindIndexBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_json_vkGetRenderingAreaGranularityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_json_vkGetDeviceImageSubresourceLayoutKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_json_vkGetImageSubresourceLayout2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_vkWaitForPresent2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkPresentWait2InfoKHR* pPresentWait2Info);
void dump_json_vkCreatePipelineBinariesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries);
void dump_json_vkDestroyPipelineBinaryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPipelineKeyKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey);
void dump_json_vkGetPipelineBinaryDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData);
void dump_json_vkReleaseCapturedPipelineDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
void dump_json_vkCmdSetLineStippleKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_json_vkGetCalibratedTimestampsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_json_vkCmdBindDescriptorSets2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo);
void dump_json_vkCmdPushConstants2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo* pPushConstantsInfo);
void dump_json_vkCmdPushDescriptorSet2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo* pPushDescriptorSetInfo);
void dump_json_vkCmdPushDescriptorSetWithTemplate2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_json_vkCmdSetDescriptorBufferOffsets2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);
void dump_json_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);
void dump_json_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void dump_json_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
void dump_json_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
void dump_json_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo);
void dump_json_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo);
void dump_json_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_json_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_json_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes);
void dump_json_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_json_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_json_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);
void dump_json_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);
void dump_json_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);
void dump_json_vkCreateCuModuleNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);
void dump_json_vkCreateCuFunctionNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);
void dump_json_vkDestroyCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);
void dump_json_vkDestroyCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdCuLaunchKernelNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);
void dump_json_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_json_vkGetImageViewHandle64NVX(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_json_vkGetImageViewAddressNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);
void dump_json_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
void dump_json_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_json_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
void dump_json_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);
void dump_json_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void dump_json_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_json_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_json_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
void dump_json_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
void dump_json_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);
void dump_json_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);
void dump_json_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);
void dump_json_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);
void dump_json_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles);
void dump_json_vkCmdSetDiscardRectangleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);
void dump_json_vkCmdSetDiscardRectangleModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);
void dump_json_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_json_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_json_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_json_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
void dump_json_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo);
void dump_json_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_json_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void dump_json_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
void dump_json_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCreateExecutionGraphPipelinesAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkGetExecutionGraphPipelineScratchSizeAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkGetExecutionGraphPipelineNodeIndexAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdInitializeGraphScratchMemoryAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipeline executionGraph, VkDeviceAddress scratch, VkDeviceSize scratchSize);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdDispatchGraphAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceSize scratchSize, const VkDispatchGraphCountInfoAMDX* pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdDispatchGraphIndirectAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceSize scratchSize, const VkDispatchGraphCountInfoAMDX* pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdDispatchGraphIndirectCountAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceSize scratchSize, VkDeviceAddress countInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);
void dump_json_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
void dump_json_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);
void dump_json_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);
void dump_json_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);
void dump_json_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);
void dump_json_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);
void dump_json_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_json_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes);
void dump_json_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);
void dump_json_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);
void dump_json_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);
void dump_json_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);
void dump_json_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);
void dump_json_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);
void dump_json_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);
void dump_json_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkGetRayTracingShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);
void dump_json_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t shader);
void dump_json_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);
void dump_json_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_json_vkCmdWriteBufferMarker2AMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_json_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_json_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_json_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);
void dump_json_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdSetExclusiveScissorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables);
void dump_json_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors);
void dump_json_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker);
void dump_json_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);
void dump_json_vkGetQueueCheckpointData2NV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);
void dump_json_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);
void dump_json_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);
void dump_json_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);
void dump_json_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo);
void dump_json_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);
void dump_json_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceConfigurationINTEL configuration);
void dump_json_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, VkPerformanceConfigurationINTEL configuration);
void dump_json_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);
void dump_json_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_json_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
void dump_json_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_json_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_vkCmdSetCullModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_json_vkCmdSetFrontFaceEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_json_vkCmdSetPrimitiveTopologyEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_json_vkCmdSetViewportWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_vkCmdSetScissorWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_vkCmdBindVertexBuffers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_json_vkCmdSetDepthTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_json_vkCmdSetDepthWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_json_vkCmdSetDepthCompareOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_json_vkCmdSetDepthBoundsTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_json_vkCmdSetStencilTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_json_vkCmdSetStencilOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_json_vkCopyMemoryToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo* pCopyMemoryToImageInfo);
void dump_json_vkCopyImageToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo* pCopyImageToMemoryInfo);
void dump_json_vkCopyImageToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo* pCopyImageToImageInfo);
void dump_json_vkTransitionImageLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo* pTransitions);
void dump_json_vkGetImageSubresourceLayout2EXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_vkReleaseSwapchainImagesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);
void dump_json_vkGetGeneratedCommandsMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdPreprocessGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_json_vkCmdExecuteGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_json_vkCmdBindPipelineShaderGroupNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);
void dump_json_vkCreateIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);
void dump_json_vkDestroyIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdSetDepthBias2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT* pDepthBiasInfo);
void dump_json_vkAcquireDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
void dump_json_vkGetDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
void dump_json_vkCreatePrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_json_vkDestroyPrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_json_vkSetPrivateDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_json_vkGetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCreateCudaModuleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkGetCudaModuleCacheNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCreateCudaFunctionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkDestroyCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkDestroyCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdCudaLaunchKernelNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_vkCmdDispatchTileQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDispatchTileInfoQCOM* pDispatchTileInfo);
void dump_json_vkCmdBeginPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileBeginInfoQCOM* pPerTileBeginInfo);
void dump_json_vkCmdEndPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileEndInfoQCOM* pPerTileEndInfo);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_vkExportMetalObjectsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_vkGetDescriptorSetLayoutSizeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);
void dump_json_vkGetDescriptorSetLayoutBindingOffsetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);
void dump_json_vkGetDescriptorEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);
void dump_json_vkCmdBindDescriptorBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos);
void dump_json_vkCmdSetDescriptorBufferOffsetsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets);
void dump_json_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);
void dump_json_vkGetBufferOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetImageOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetImageViewOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetSamplerOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkCmdSetFragmentShadingRateEnumNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]);
void dump_json_vkGetDeviceFaultInfoEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkAcquireWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_vkCreateDirectFBSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_vkGetPhysicalDeviceDirectFBPresentationSupportEXT(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_json_vkCmdSetVertexInputEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetMemoryZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetMemoryZirconHandlePropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkImportSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkCreateBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkSetBufferCollectionImageConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkSetBufferCollectionBufferConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkDestroyBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetBufferCollectionPropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);
void dump_json_vkCmdSubpassShadingHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdBindInvocationMaskHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_json_vkGetMemoryRemoteAddressNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);
void dump_json_vkGetPipelinePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);
void dump_json_vkCmdSetPatchControlPointsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t patchControlPoints);
void dump_json_vkCmdSetRasterizerDiscardEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_json_vkCmdSetDepthBiasEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_json_vkCmdSetLogicOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLogicOp logicOp);
void dump_json_vkCmdSetPrimitiveRestartEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_vkCreateScreenSurfaceQNX(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_vkGetPhysicalDeviceScreenPresentationSupportQNX(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_vkCmdSetColorWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables);
void dump_json_vkCmdDrawMultiEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);
void dump_json_vkCmdDrawMultiIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset);
void dump_json_vkCreateMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMicromapCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap);
void dump_json_vkDestroyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_json_vkBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_json_vkCopyMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo);
void dump_json_vkCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_json_vkCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_json_vkWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);
void dump_json_vkCmdCopyMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo);
void dump_json_vkCmdCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_json_vkCmdCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_json_vkCmdWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_vkGetDeviceMicromapCompatibilityEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_json_vkGetMicromapBuildSizesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo);
void dump_json_vkCmdDrawClusterHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkCmdDrawClusterIndirectHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_json_vkSetDeviceMemoryPriorityEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, float priority);
void dump_json_vkGetDescriptorSetLayoutHostMappingInfoVALVE(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);
void dump_json_vkGetDescriptorSetHostMappingVALVE(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, void** ppData);
void dump_json_vkCmdCopyMemoryIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);
void dump_json_vkCmdCopyMemoryToImageIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources);
void dump_json_vkCmdDecompressMemoryNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions);
void dump_json_vkCmdDecompressMemoryIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);
void dump_json_vkGetPipelineIndirectMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdUpdatePipelineIndirectBufferNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_json_vkGetPipelineIndirectDeviceAddressNV(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo);
void dump_json_vkCmdSetDepthClampEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);
void dump_json_vkCmdSetPolygonModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);
void dump_json_vkCmdSetRasterizationSamplesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples);
void dump_json_vkCmdSetSampleMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, const VkSampleMask* pSampleMask);
void dump_json_vkCmdSetAlphaToCoverageEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);
void dump_json_vkCmdSetAlphaToOneEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);
void dump_json_vkCmdSetLogicOpEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);
void dump_json_vkCmdSetColorBlendEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables);
void dump_json_vkCmdSetColorBlendEquationEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations);
void dump_json_vkCmdSetColorWriteMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks);
void dump_json_vkCmdSetTessellationDomainOriginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);
void dump_json_vkCmdSetRasterizationStreamEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t rasterizationStream);
void dump_json_vkCmdSetConservativeRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);
void dump_json_vkCmdSetExtraPrimitiveOverestimationSizeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);
void dump_json_vkCmdSetDepthClipEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);
void dump_json_vkCmdSetSampleLocationsEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);
void dump_json_vkCmdSetColorBlendAdvancedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced);
void dump_json_vkCmdSetProvokingVertexModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);
void dump_json_vkCmdSetLineRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);
void dump_json_vkCmdSetLineStippleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);
void dump_json_vkCmdSetDepthClipNegativeOneToOneEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);
void dump_json_vkCmdSetViewportWScalingEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);
void dump_json_vkCmdSetViewportSwizzleNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles);
void dump_json_vkCmdSetCoverageToColorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);
void dump_json_vkCmdSetCoverageToColorLocationNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);
void dump_json_vkCmdSetCoverageModulationModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);
void dump_json_vkCmdSetCoverageModulationTableEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);
void dump_json_vkCmdSetCoverageModulationTableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable);
void dump_json_vkCmdSetShadingRateImageEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);
void dump_json_vkCmdSetRepresentativeFragmentTestEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);
void dump_json_vkCmdSetCoverageReductionModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);
void dump_json_vkCreateTensorARM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkTensorCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkTensorARM* pTensor);
void dump_json_vkDestroyTensorARM(ApiDumpInstance& dump_inst, VkDevice device, VkTensorARM tensor, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateTensorViewARM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkTensorViewCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkTensorViewARM* pView);
void dump_json_vkDestroyTensorViewARM(ApiDumpInstance& dump_inst, VkDevice device, VkTensorViewARM tensorView, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetTensorMemoryRequirementsARM(ApiDumpInstance& dump_inst, VkDevice device, const VkTensorMemoryRequirementsInfoARM* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkBindTensorMemoryARM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindTensorMemoryInfoARM* pBindInfos);
void dump_json_vkGetDeviceTensorMemoryRequirementsARM(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceTensorMemoryRequirementsARM* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdCopyTensorARM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyTensorInfoARM* pCopyTensorInfo);
void dump_json_vkGetPhysicalDeviceExternalTensorPropertiesARM(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalTensorInfoARM* pExternalTensorInfo, VkExternalTensorPropertiesARM* pExternalTensorProperties);
void dump_json_vkGetTensorOpaqueCaptureDescriptorDataARM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkTensorCaptureDescriptorDataInfoARM* pInfo, void* pData);
void dump_json_vkGetTensorViewOpaqueCaptureDescriptorDataARM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkTensorViewCaptureDescriptorDataInfoARM* pInfo, void* pData);
void dump_json_vkGetShaderModuleIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_json_vkGetShaderModuleCreateInfoIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_json_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
void dump_json_vkCreateOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);
void dump_json_vkDestroyOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator);
void dump_json_vkBindOpticalFlowSessionImageNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);
void dump_json_vkCmdOpticalFlowExecuteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo);
void dump_json_vkAntiLagUpdateAMD(ApiDumpInstance& dump_inst, VkDevice device, const VkAntiLagDataAMD* pData);
void dump_json_vkCreateShadersEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);
void dump_json_vkDestroyShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetShaderBinaryDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);
void dump_json_vkCmdBindShadersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders);
void dump_json_vkCmdSetDepthClampRangeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange);
void dump_json_vkGetFramebufferTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);
void dump_json_vkGetDynamicRenderingTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);
void dump_json_vkGetPhysicalDeviceCooperativeVectorPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeVectorPropertiesNV* pProperties);
void dump_json_vkConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkConvertCooperativeVectorMatrixInfoNV* pInfo);
void dump_json_vkCmdConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkConvertCooperativeVectorMatrixInfoNV* pInfos);
void dump_json_vkSetLatencySleepModeNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo);
void dump_json_vkLatencySleepNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo);
void dump_json_vkSetLatencyMarkerNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_json_vkGetLatencyTimingsNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_json_vkQueueNotifyOutOfBandNV(ApiDumpInstance& dump_inst, VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);
void dump_json_vkCmdSetAttachmentFeedbackLoopEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_vkGetScreenBufferPropertiesQNX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_vkCmdBindTileMemoryQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkTileMemoryBindInfoQCOM* pTileMemoryBindInfo);
void dump_json_vkCreateExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkExternalComputeQueueCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkExternalComputeQueueNV* pExternalQueue);
void dump_json_vkDestroyExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, VkExternalComputeQueueNV externalQueue, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetExternalComputeQueueDataNV(ApiDumpInstance& dump_inst, VkExternalComputeQueueNV externalQueue, VkExternalComputeQueueDataParamsNV* params, void* pData);
void dump_json_vkGetClusterAccelerationStructureBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkClusterAccelerationStructureInputInfoNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_json_vkCmdBuildClusterAccelerationStructureIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkClusterAccelerationStructureCommandsInfoNV* pCommandInfos);
void dump_json_vkGetPartitionedAccelerationStructuresBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPartitionedAccelerationStructureInstancesInputNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_json_vkCmdBuildPartitionedAccelerationStructuresNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBuildPartitionedAccelerationStructureInfoNV* pBuildInfo);
void dump_json_vkGetGeneratedCommandsMemoryRequirementsEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdPreprocessGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer);
void dump_json_vkCmdExecuteGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo);
void dump_json_vkCreateIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout);
void dump_json_vkDestroyIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet);
void dump_json_vkDestroyIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator);
void dump_json_vkUpdateIndirectExecutionSetPipelineEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites);
void dump_json_vkUpdateIndirectExecutionSetShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites);
#if defined(VK_USE_PLATFORM_OHOS)
void dump_json_vkCreateSurfaceOHOS(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkSurfaceCreateInfoOHOS* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_OHOS
void dump_json_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_vkGetMemoryMetalHandleEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetMetalHandleInfoEXT* pGetMetalHandleInfo, void** pHandle);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_vkGetMemoryMetalHandlePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHandle, VkMemoryMetalHandlePropertiesEXT* pMemoryMetalHandleProperties);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_vkCmdEndRendering2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingEndInfoEXT* pRenderingEndInfo);
void dump_json_vkCreateAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure);
void dump_json_vkDestroyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_json_vkCmdBuildAccelerationStructuresIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts);
void dump_json_vkBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_json_vkCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_json_vkCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_json_vkCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_json_vkWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);
void dump_json_vkCmdCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_json_vkCmdCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_json_vkCmdCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_json_vkGetAccelerationStructureDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
void dump_json_vkCmdWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_vkGetDeviceAccelerationStructureCompatibilityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_json_vkGetAccelerationStructureBuildSizesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_json_vkCmdTraceRaysKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
void dump_json_vkCreateRayTracingPipelinesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_vkCmdTraceRaysIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);
void dump_json_vkGetRayTracingShaderGroupStackSizeKHR(ApiDumpInstance& dump_inst, VkDeviceSize result, VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);
void dump_json_vkCmdSetRayTracingPipelineStackSizeKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);
void dump_json_vkCmdDrawMeshTasksEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkCmdDrawMeshTasksIndirectEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDrawMeshTasksIndirectCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);


