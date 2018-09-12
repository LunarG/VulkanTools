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
#include "vktrace_lib_trim_statetracker.h"
#include "vktrace_lib_trim.h"

namespace trim {
// declared extern in statetracker.h
VKTRACE_CRITICAL_SECTION trimTransitionMapLock;

//-------------------------------------------------------------------------
#define COPY_PACKET(packet) packet = copy_packet(packet)

vktrace_trace_packet_header *copy_packet(vktrace_trace_packet_header *pHeader) {
    if (pHeader == nullptr) {
        return nullptr;
    }

    uint64_t packetSize = pHeader->size;
    vktrace_trace_packet_header *pCopy = static_cast<vktrace_trace_packet_header *>(malloc((size_t)packetSize));
    if (pCopy != nullptr) {
        memcpy(pCopy, pHeader, (size_t)packetSize);
    }
    return pCopy;
}

//-------------------------------------------------------------------------
void StateTracker::AddImageTransition(VkCommandBuffer commandBuffer, ImageTransition transition) {
    vktrace_enter_critical_section(&trimTransitionMapLock);
    m_cmdBufferToImageTransitionsMap[commandBuffer].push_back(transition);
    vktrace_leave_critical_section(&trimTransitionMapLock);
}

//-------------------------------------------------------------------------
void StateTracker::ClearImageTransitions(VkCommandBuffer commandBuffer) {
    vktrace_enter_critical_section(&trimTransitionMapLock);
    m_cmdBufferToImageTransitionsMap[commandBuffer].clear();
    m_cmdBufferToImageTransitionsMap.erase(commandBuffer);
    vktrace_leave_critical_section(&trimTransitionMapLock);
}

//-------------------------------------------------------------------------
void StateTracker::AddBufferTransition(VkCommandBuffer commandBuffer, BufferTransition transition) {
    vktrace_enter_critical_section(&trimTransitionMapLock);
    m_cmdBufferToBufferTransitionsMap[commandBuffer].push_back(transition);
    vktrace_leave_critical_section(&trimTransitionMapLock);
}

void StateTracker::ClearBufferTransitions(VkCommandBuffer commandBuffer) {
    vktrace_enter_critical_section(&trimTransitionMapLock);
    m_cmdBufferToBufferTransitionsMap[commandBuffer].clear();
    m_cmdBufferToBufferTransitionsMap.erase(commandBuffer);
    vktrace_leave_critical_section(&trimTransitionMapLock);
}

//-------------------------------------------------------------------------
StateTracker::StateTracker() {}

StateTracker::StateTracker(const StateTracker &other) { *this = other; }

//-------------------------------------------------------------------------
StateTracker::~StateTracker() { clear(); }

//-------------------------------------------------------------------------
void StateTracker::add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header *pHeader) {
    if (pHeader != NULL) {
        m_cmdBufferPackets[commandBuffer].push_back(pHeader);
    }
}

//-------------------------------------------------------------------------
void StateTracker::remove_CommandBuffer_calls(VkCommandBuffer commandBuffer) {
    std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header *>>::iterator cmdBufferMap =
        m_cmdBufferPackets.find(commandBuffer);
    if (cmdBufferMap != m_cmdBufferPackets.end()) {
        for (auto packet = cmdBufferMap->second.begin(); packet != cmdBufferMap->second.end(); ++packet) {
            vktrace_trace_packet_header *pHeader = *packet;
            vktrace_delete_trace_packet(&pHeader);
        }
        cmdBufferMap->second.clear();

        m_cmdBufferPackets.erase(commandBuffer);
    }
}

#if TRIM_USE_ORDERED_IMAGE_CREATION
void StateTracker::add_Image_call(vktrace_trace_packet_header *pHeader) { m_image_calls.push_back(pHeader); }
#endif  // TRIM_USE_ORDERED_IMAGE_CREATION

//-------------------------------------------------------------------------
void StateTracker::clear() {
    while (createdInstances.size() != 0) {
        remove_Instance(reinterpret_cast<const VkInstance>(createdInstances.begin()->first));
    }

    while (createdPhysicalDevices.size() != 0) {
        remove_PhysicalDevice(reinterpret_cast<VkPhysicalDevice>(createdPhysicalDevices.begin()->first));
    }

    while (createdDevices.size() != 0) {
        remove_Device(reinterpret_cast<VkDevice>(reinterpret_cast<void *>(createdDevices.begin()->first)));
    }

    while (createdSurfaceKHRs.size() != 0) {
        remove_SurfaceKHR(createdSurfaceKHRs.begin()->first);
    }

    while (createdCommandPools.size() != 0) {
        remove_CommandPool(createdCommandPools.begin()->first);
    }

    while (createdCommandBuffers.size() != 0) {
        remove_CommandBuffer(reinterpret_cast<VkCommandBuffer>(reinterpret_cast<void *>(createdCommandBuffers.begin()->first)));
    }

    while (createdDescriptorPools.size() != 0) {
        remove_DescriptorPool(createdDescriptorPools.begin()->first);
    }

    while (createdRenderPasss.size() != 0) {
        remove_RenderPass(createdRenderPasss.begin()->first);
    }

    while (createdPipelineCaches.size() != 0) {
        remove_PipelineCache(createdPipelineCaches.begin()->first);
    }

    while (createdPipelines.size() != 0) {
        remove_Pipeline(createdPipelines.begin()->first);
    }

    while (createdQueues.size() != 0) {
        remove_Queue(reinterpret_cast<const VkQueue>(createdQueues.begin()->first));
    }

    while (createdSemaphores.size() != 0) {
        remove_Semaphore(createdSemaphores.begin()->first);
    }

    while (createdDeviceMemorys.size() != 0) {
        remove_DeviceMemory(createdDeviceMemorys.begin()->first);
    }

    while (createdFences.size() != 0) {
        remove_Fence(createdFences.begin()->first);
    }

    while (createdSwapchainKHRs.size() != 0) {
        remove_SwapchainKHR(createdSwapchainKHRs.begin()->first);
    }

    while (createdImages.size() != 0) {
        remove_Image(createdImages.begin()->first);
    }

    while (createdImageViews.size() != 0) {
        remove_ImageView(createdImageViews.begin()->first);
    }

    while (createdBuffers.size() != 0) {
        remove_Buffer(createdBuffers.begin()->first);
    }

    while (createdBufferViews.size() != 0) {
        remove_BufferView(createdBufferViews.begin()->first);
    }

    while (createdFramebuffers.size() != 0) {
        remove_Framebuffer(createdFramebuffers.begin()->first);
    }

    while (createdEvents.size() != 0) {
        remove_Event(createdEvents.begin()->first);
    }

    while (createdQueryPools.size() != 0) {
        remove_QueryPool(createdQueryPools.begin()->first);
    }

    while (createdShaderModules.size() != 0) {
        remove_ShaderModule(createdShaderModules.begin()->first);
    }

    while (createdPipelineLayouts.size() != 0) {
        remove_PipelineLayout(createdPipelineLayouts.begin()->first);
    }

    while (createdSamplers.size() != 0) {
        remove_Sampler(createdSamplers.begin()->first);
    }

    while (createdDescriptorSetLayouts.size() != 0) {
        remove_DescriptorSetLayout(createdDescriptorSetLayouts.begin()->first);
    }

    while (createdDescriptorSets.size() != 0) {
        remove_DescriptorSet(createdDescriptorSets.begin()->first);
    }

    for (auto iter = m_cmdBufferPackets.begin(); iter != m_cmdBufferPackets.end(); ++iter) {
        std::list<vktrace_trace_packet_header *> &packets = iter->second;
        for (auto packetIter = iter->second.begin(); packetIter != iter->second.end(); ++packetIter) {
            vktrace_trace_packet_header *pHeader = *packetIter;
            vktrace_delete_trace_packet(&pHeader);
        }
        packets.clear();
    }
    m_cmdBufferPackets.clear();

    for (auto packet = m_image_calls.begin(); packet != m_image_calls.end(); ++packet) {
        vktrace_trace_packet_header *pHeader = *packet;
        vktrace_delete_trace_packet(&pHeader);
    }
    m_image_calls.clear();

    for (auto renderPassIter = m_renderPassVersions.begin(); renderPassIter != m_renderPassVersions.end(); ++renderPassIter) {
        std::vector<VkRenderPassCreateInfo *> versions = renderPassIter->second;

        for (uint32_t i = 0; i < versions.size(); i++) {
            VkRenderPassCreateInfo *pCreateInfo = versions[i];

            for (uint32_t subpass = 0; subpass < pCreateInfo->subpassCount; subpass++) {
                if (pCreateInfo->pSubpasses[subpass].inputAttachmentCount > 0 &&
                    pCreateInfo->pSubpasses[subpass].pInputAttachments != nullptr) {
                    VKTRACE_DELETE(const_cast<VkAttachmentReference *>(pCreateInfo->pSubpasses[subpass].pInputAttachments));
                }

                if (pCreateInfo->pSubpasses[subpass].colorAttachmentCount > 0) {
                    if (pCreateInfo->pSubpasses[subpass].pColorAttachments != nullptr) {
                        VKTRACE_DELETE(const_cast<VkAttachmentReference *>(pCreateInfo->pSubpasses[subpass].pColorAttachments));
                    }

                    if (pCreateInfo->pSubpasses[subpass].pResolveAttachments != nullptr) {
                        VKTRACE_DELETE(const_cast<VkAttachmentReference *>(pCreateInfo->pSubpasses[subpass].pResolveAttachments));
                    }
                }

                if (pCreateInfo->pSubpasses[subpass].pDepthStencilAttachment != nullptr) {
                    VKTRACE_DELETE(const_cast<VkAttachmentReference *>(pCreateInfo->pSubpasses[subpass].pDepthStencilAttachment));
                }

                if (pCreateInfo->pSubpasses[subpass].preserveAttachmentCount > 0 &&
                    pCreateInfo->pSubpasses[subpass].pPreserveAttachments != nullptr) {
                    VKTRACE_DELETE(const_cast<uint32_t *>(pCreateInfo->pSubpasses[subpass].pPreserveAttachments));
                }
            }

            if (pCreateInfo->pAttachments != nullptr) {
                VKTRACE_DELETE(const_cast<VkAttachmentDescription *>(pCreateInfo->pAttachments));
            }

            VKTRACE_DELETE(pCreateInfo);
        }
        versions.clear();
    }
    m_renderPassVersions.clear();
}

//-------------------------------------------------------------------------
void StateTracker::copy_VkRenderPassCreateInfo(VkRenderPassCreateInfo *pDst, const VkRenderPassCreateInfo &src) {
    if (pDst != nullptr) {
        *pDst = src;

        VkSubpassDescription *pSubPasses =
            static_cast<VkSubpassDescription *>(VKTRACE_NEW_ARRAY(VkSubpassDescription, src.subpassCount));
        if (pSubPasses != nullptr) {
            for (uint32_t i = 0; i < src.subpassCount; i++) {
                pSubPasses[i] = src.pSubpasses[i];

                if (src.pSubpasses[i].inputAttachmentCount > 0) {
                    VkAttachmentReference *pInputAttachments = static_cast<VkAttachmentReference *>(
                        VKTRACE_NEW_ARRAY(VkAttachmentReference, src.pSubpasses[i].inputAttachmentCount));
                    if (pInputAttachments != nullptr) {
                        for (uint32_t j = 0; j < src.pSubpasses[i].inputAttachmentCount; j++) {
                            pInputAttachments[j] = src.pSubpasses[i].pInputAttachments[j];
                        }
                    }
                    pSubPasses[i].pInputAttachments = pInputAttachments;
                }

                if (src.pSubpasses[i].colorAttachmentCount > 0) {
                    VkAttachmentReference *pColorAttachments = static_cast<VkAttachmentReference *>(
                        VKTRACE_NEW_ARRAY(VkAttachmentReference, src.pSubpasses[i].colorAttachmentCount));
                    if (pColorAttachments != nullptr) {
                        for (uint32_t j = 0; j < src.pSubpasses[i].colorAttachmentCount; j++) {
                            pColorAttachments[j] = src.pSubpasses[i].pColorAttachments[j];
                        }
                    }
                    pSubPasses[i].pColorAttachments = pColorAttachments;

                    if (src.pSubpasses[i].pResolveAttachments != nullptr) {
                        VkAttachmentReference *pResolveAttachments = static_cast<VkAttachmentReference *>(
                            VKTRACE_NEW_ARRAY(VkAttachmentReference, src.pSubpasses[i].colorAttachmentCount));
                        if (pResolveAttachments != nullptr) {
                            for (uint32_t j = 0; j < src.pSubpasses[i].colorAttachmentCount; j++) {
                                pResolveAttachments[j] = src.pSubpasses[i].pResolveAttachments[j];
                            }
                        }
                        pSubPasses[i].pResolveAttachments = pResolveAttachments;
                    }
                }

                if (src.pSubpasses[i].pDepthStencilAttachment != nullptr) {
                    VkAttachmentReference *pDepthStencilAttachment =
                        static_cast<VkAttachmentReference *>(VKTRACE_NEW(VkAttachmentReference));
                    if (pDepthStencilAttachment != nullptr) {
                        *pDepthStencilAttachment = *src.pSubpasses[i].pDepthStencilAttachment;
                    }
                    pSubPasses[i].pDepthStencilAttachment = pDepthStencilAttachment;
                }

                if (src.pSubpasses[i].preserveAttachmentCount > 0) {
                    uint32_t *pPreserveAttachments =
                        static_cast<uint32_t *>(VKTRACE_NEW_ARRAY(uint32_t, src.pSubpasses[i].preserveAttachmentCount));
                    if (pPreserveAttachments != nullptr) {
                        for (uint32_t j = 0; j < src.pSubpasses[i].preserveAttachmentCount; j++) {
                            pPreserveAttachments[j] = src.pSubpasses[i].pPreserveAttachments[j];
                        }
                    }
                    pSubPasses[i].pPreserveAttachments = pPreserveAttachments;
                }
            }
        }

        pDst->pSubpasses = pSubPasses;

        VkAttachmentDescription *pAttachments =
            static_cast<VkAttachmentDescription *>(VKTRACE_NEW_ARRAY(VkAttachmentDescription, src.attachmentCount));
        if (pAttachments != nullptr) {
            for (uint32_t i = 0; i < src.attachmentCount; i++) {
                pAttachments[i] = src.pAttachments[i];
            }
        }
        pDst->pAttachments = pAttachments;
    }
}

void StateTracker::add_RenderPassCreateInfo(VkRenderPass renderPass, const VkRenderPassCreateInfo *pCreateInfo) {
    VkRenderPassCreateInfo *pCopyCreateInfo = static_cast<VkRenderPassCreateInfo *>(VKTRACE_NEW(VkRenderPassCreateInfo));
    copy_VkRenderPassCreateInfo(pCopyCreateInfo, *pCreateInfo);
    m_renderPassVersions[renderPass].push_back(pCopyCreateInfo);
}

//-------------------------------------------------------------------------
uint32_t StateTracker::get_RenderPassVersion(VkRenderPass renderPass) {
    return static_cast<uint32_t>(m_renderPassVersions[renderPass].size() - 1);
}

//-------------------------------------------------------------------------
VkRenderPassCreateInfo *StateTracker::get_RenderPassCreateInfo(VkRenderPass renderPass, uint32_t version) {
    return m_renderPassVersions[renderPass][version];
}

//-------------------------------------------------------------------------
StateTracker &StateTracker::operator=(const StateTracker &other) {
    if (this == &other) return *this;

    m_renderPassVersions = other.m_renderPassVersions;
    for (auto iter = m_renderPassVersions.begin(); iter != m_renderPassVersions.end(); ++iter) {
        for (uint32_t i = 0; i < iter->second.size(); i++) {
            VkRenderPassCreateInfo *pCreateInfoVersion = iter->second[i];
            VkRenderPassCreateInfo *pCopiedCreateInfo = static_cast<VkRenderPassCreateInfo *>(VKTRACE_NEW(VkRenderPassCreateInfo));
            copy_VkRenderPassCreateInfo(pCopiedCreateInfo, *pCreateInfoVersion);
            iter->second[i] = pCopiedCreateInfo;
        }
    }

    for (auto iter = other.m_cmdBufferPackets.cbegin(); iter != other.m_cmdBufferPackets.cend(); ++iter) {
        std::list<vktrace_trace_packet_header *> packets;
        for (auto packetIter = iter->second.cbegin(); packetIter != iter->second.cend(); ++packetIter) {
            packets.push_back(copy_packet(*packetIter));
        }
        m_cmdBufferPackets[iter->first] = packets;
    }

    for (auto packet = other.m_image_calls.cbegin(); packet != other.m_image_calls.cend(); ++packet) {
        m_image_calls.push_back(copy_packet(*packet));
    }

    createdInstances = other.createdInstances;
    for (auto obj = createdInstances.begin(); obj != createdInstances.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Instance.pCreatePacket);
        COPY_PACKET(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket);
        COPY_PACKET(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket);
    }

    createdPhysicalDevices = other.createdPhysicalDevices;
    for (auto obj = createdPhysicalDevices.begin(); obj != createdPhysicalDevices.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDevicePropertiesPacket);
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceProperties2KHRPacket);
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket);
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket);
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket);
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRCountPacket);
        COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRPacket);
    }

    createdDevices = other.createdDevices;
    for (auto obj = createdDevices.begin(); obj != createdDevices.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Device.pCreatePacket);

        trim::QueueFamily *pExistingFamilies = obj->second.ObjectInfo.Device.pQueueFamilies;

        obj->second.ObjectInfo.Device.pQueueFamilies =
            VKTRACE_NEW_ARRAY(trim::QueueFamily, obj->second.ObjectInfo.Device.queueFamilyCount);
        for (uint32_t family = 0; family < obj->second.ObjectInfo.Device.queueFamilyCount; family++) {
            uint32_t count = pExistingFamilies[family].count;
            obj->second.ObjectInfo.Device.pQueueFamilies[family].count = count;
            obj->second.ObjectInfo.Device.pQueueFamilies[family].queues = VKTRACE_NEW_ARRAY(VkQueue, count);

            for (uint32_t q = 0; q < count; q++) {
                VkQueue queue = pExistingFamilies[family].queues[q];
                obj->second.ObjectInfo.Device.pQueueFamilies[family].queues[q] = queue;
            }
        }
    }

    createdSurfaceKHRs = other.createdSurfaceKHRs;
    for (auto obj = createdSurfaceKHRs.begin(); obj != createdSurfaceKHRs.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket);
    }

    createdCommandPools = other.createdCommandPools;
    for (auto obj = createdCommandPools.begin(); obj != createdCommandPools.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.CommandPool.pCreatePacket);
    }

    createdCommandBuffers = other.createdCommandBuffers;

    createdDescriptorPools = other.createdDescriptorPools;
    for (auto obj = createdDescriptorPools.begin(); obj != createdDescriptorPools.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.DescriptorPool.pCreatePacket);
    }

    createdDescriptorUpdateTemplates = other.createdDescriptorUpdateTemplates;
    for (auto obj = createdDescriptorUpdateTemplates.begin(); obj != createdDescriptorUpdateTemplates.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.DescriptorUpdateTemplate.pCreatePacket);

        if (obj->second.ObjectInfo.DescriptorUpdateTemplate.descriptorUpdateEntryCount != 0) {
            const VkDescriptorUpdateTemplateEntry *pOtherDescriptorUpdateEntries =
                obj->second.ObjectInfo.DescriptorUpdateTemplate.pDescriptorUpdateEntries;

            obj->second.ObjectInfo.DescriptorUpdateTemplate.pDescriptorUpdateEntries = VKTRACE_NEW_ARRAY(
                VkDescriptorUpdateTemplateEntry, obj->second.ObjectInfo.DescriptorUpdateTemplate.descriptorUpdateEntryCount);

            assert(obj->second.ObjectInfo.DescriptorUpdateTemplate.pDescriptorUpdateEntries != nullptr);

            memcpy(reinterpret_cast<void *>(const_cast<VkDescriptorUpdateTemplateEntry *>(
                       obj->second.ObjectInfo.DescriptorUpdateTemplate.pDescriptorUpdateEntries)),
                   reinterpret_cast<const void *>(const_cast<VkDescriptorUpdateTemplateEntry *>(pOtherDescriptorUpdateEntries)),
                   obj->second.ObjectInfo.DescriptorUpdateTemplate.descriptorUpdateEntryCount *
                       sizeof(VkDescriptorUpdateTemplateEntry));
        }
    }

    createdSwapchainKHRs = other.createdSwapchainKHRs;
    for (auto obj = createdSwapchainKHRs.begin(); obj != createdSwapchainKHRs.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket);
        COPY_PACKET(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket);
        COPY_PACKET(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket);
    }

    createdRenderPasss = other.createdRenderPasss;
    for (auto obj = createdRenderPasss.begin(); obj != createdRenderPasss.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.RenderPass.pCreatePacket);
    }

    createdPipelineCaches = other.createdPipelineCaches;
    for (auto obj = createdPipelineCaches.begin(); obj != createdPipelineCaches.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.PipelineCache.pCreatePacket);
    }

    createdPipelines = other.createdPipelines;
    for (auto obj = createdPipelines.begin(); obj != createdPipelines.end(); obj++) {
        VkShaderModuleCreateInfo *pShaderModuleCreateInfos =
            VKTRACE_NEW_ARRAY(VkShaderModuleCreateInfo, obj->second.ObjectInfo.Pipeline.shaderModuleCreateInfoCount);
        for (uint32_t stageIndex = 0; stageIndex < obj->second.ObjectInfo.Pipeline.shaderModuleCreateInfoCount; stageIndex++) {
            trim::StateTracker::copy_VkShaderModuleCreateInfo(&pShaderModuleCreateInfos[stageIndex],
                                                              obj->second.ObjectInfo.Pipeline.pShaderModuleCreateInfos[stageIndex]);
        }
        obj->second.ObjectInfo.Pipeline.pShaderModuleCreateInfos = pShaderModuleCreateInfos;

        VkGraphicsPipelineCreateInfo *pCreateInfo = &obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo;

        // note: Using the same memory as both the destination and the source.
        // We're copying what is currently there, which will properly result in
        // new copies of any pointed-to objects and arrays.
        if (obj->second.ObjectInfo.Pipeline.isGraphicsPipeline) {
            // here we keep the original value of source to a copy, so we can
            // change the destination value in pCreateInfo and make it a
            // deepcopy of the source. note: src parameter in the define of
            // copy_VkGraphicsPipelineCreateInfo function is a reference.
            VkGraphicsPipelineCreateInfo createInfoCopy = obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo;
            copy_VkGraphicsPipelineCreateInfo(pCreateInfo, createInfoCopy);
        } else {
            VkComputePipelineCreateInfo createInfoCopy = obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo;
            copy_VkComputePipelineCreateInfo(
                const_cast<VkComputePipelineCreateInfo *>(&obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo),
                createInfoCopy);
        }
    }

    createdQueues = other.createdQueues;
    for (auto obj = createdQueues.begin(); obj != createdQueues.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Queue.pCreatePacket);
    }

    createdSemaphores = other.createdSemaphores;
    for (auto obj = createdSemaphores.begin(); obj != createdSemaphores.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Semaphore.pCreatePacket);
    }

    createdDeviceMemorys = other.createdDeviceMemorys;
    for (auto obj = createdDeviceMemorys.begin(); obj != createdDeviceMemorys.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pCreatePacket);
        COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pMapMemoryPacket);
        COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pUnmapMemoryPacket);
        COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket);
    }

    createdFences = other.createdFences;

    createdImages = other.createdImages;
    for (auto obj = createdImages.begin(); obj != createdImages.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Image.pCreatePacket);
        COPY_PACKET(obj->second.ObjectInfo.Image.pMapMemoryPacket);
        COPY_PACKET(obj->second.ObjectInfo.Image.pUnmapMemoryPacket);
#if !TRIM_USE_ORDERED_IMAGE_CREATION
        COPY_PACKET(obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket);
#endif  //! TRIM_USE_ORDERED_IMAGE_CREATION
        COPY_PACKET(obj->second.ObjectInfo.Image.pBindImageMemoryPacket);
    }

    createdImageViews = other.createdImageViews;
    for (auto obj = createdImageViews.begin(); obj != createdImageViews.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.ImageView.pCreatePacket);
    }

    createdBuffers = other.createdBuffers;
    for (auto obj = createdBuffers.begin(); obj != createdBuffers.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Buffer.pCreatePacket);
        COPY_PACKET(obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket);
        COPY_PACKET(obj->second.ObjectInfo.Buffer.pMapMemoryPacket);
        COPY_PACKET(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket);
    }

    createdBufferViews = other.createdBufferViews;
    for (auto obj = createdBufferViews.begin(); obj != createdBufferViews.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.BufferView.pCreatePacket);
    }

    createdFramebuffers = other.createdFramebuffers;
    for (auto obj = createdFramebuffers.begin(); obj != createdFramebuffers.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Framebuffer.pCreatePacket);
    }

    createdEvents = other.createdEvents;
    for (auto obj = createdEvents.begin(); obj != createdEvents.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Event.pCreatePacket);
    }

    createdQueryPools = other.createdQueryPools;
    for (auto obj = createdQueryPools.begin(); obj != createdQueryPools.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.QueryPool.pCreatePacket);

        uint32_t queryCount = obj->second.ObjectInfo.QueryPool.size;
        if (queryCount > 0) {
            bool *tmp = new bool[queryCount];
            memcpy(tmp, obj->second.ObjectInfo.QueryPool.pResultsAvailable, queryCount * sizeof(bool));
            obj->second.ObjectInfo.QueryPool.pResultsAvailable = tmp;
        } else {
            obj->second.ObjectInfo.QueryPool.pResultsAvailable = nullptr;
        }
    }

    createdShaderModules = other.createdShaderModules;
    for (auto obj = createdShaderModules.begin(); obj != createdShaderModules.end(); obj++) {
        uint32_t *pCodeCopy = static_cast<uint32_t *>(malloc(obj->second.ObjectInfo.ShaderModule.createInfo.codeSize));
        memcpy(pCodeCopy, obj->second.ObjectInfo.ShaderModule.createInfo.pCode,
               obj->second.ObjectInfo.ShaderModule.createInfo.codeSize);
        obj->second.ObjectInfo.ShaderModule.createInfo.pCode = pCodeCopy;
    }

    createdPipelineLayouts = other.createdPipelineLayouts;
    for (auto obj = createdPipelineLayouts.begin(); obj != createdPipelineLayouts.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.PipelineLayout.pCreatePacket);

        if (obj->second.ObjectInfo.PipelineLayout.pDescriptorSetLayouts != nullptr) {
            VkDescriptorSetLayout *pLayouts =
                new VkDescriptorSetLayout[obj->second.ObjectInfo.PipelineLayout.descriptorSetLayoutCount];
            memcpy(pLayouts, obj->second.ObjectInfo.PipelineLayout.pDescriptorSetLayouts,
                   obj->second.ObjectInfo.PipelineLayout.descriptorSetLayoutCount * sizeof(VkDescriptorSetLayout));
            obj->second.ObjectInfo.PipelineLayout.pDescriptorSetLayouts = pLayouts;
        }
    }

    createdSamplers = other.createdSamplers;
    for (auto obj = createdSamplers.begin(); obj != createdSamplers.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.Sampler.pCreatePacket);
    }

    createdDescriptorSetLayouts = other.createdDescriptorSetLayouts;
    for (auto obj = createdDescriptorSetLayouts.begin(); obj != createdDescriptorSetLayouts.end(); obj++) {
        COPY_PACKET(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket);

        uint32_t bindingCount = obj->second.ObjectInfo.DescriptorSetLayout.bindingCount;
        if (bindingCount > 0) {
            VkDescriptorSetLayoutBinding *tmp = new VkDescriptorSetLayoutBinding[bindingCount];
            memcpy(tmp, obj->second.ObjectInfo.DescriptorSetLayout.pBindings, bindingCount * sizeof(VkDescriptorSetLayoutBinding));
            obj->second.ObjectInfo.DescriptorSetLayout.pBindings = tmp;
        } else {
            obj->second.ObjectInfo.DescriptorSetLayout.pBindings = nullptr;
        }
    }

    createdDescriptorSets = other.createdDescriptorSets;
    for (auto obj = createdDescriptorSets.begin(); obj != createdDescriptorSets.end(); obj++) {
        uint32_t numBindings = obj->second.ObjectInfo.DescriptorSet.numBindings;
        if (numBindings > 0) {
            VkWriteDescriptorSet *tmp = new VkWriteDescriptorSet[numBindings];
            memcpy(tmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets, numBindings * sizeof(VkWriteDescriptorSet));
            obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets = tmp;

            for (uint32_t s = 0; s < numBindings; s++) {
                uint32_t count = obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].descriptorCount;

                if (obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo != nullptr) {
                    VkDescriptorImageInfo *pTmp = new VkDescriptorImageInfo[count];
                    memcpy(pTmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo,
                           count * sizeof(VkDescriptorImageInfo));
                    obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo = pTmp;
                }
                if (obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo != nullptr) {
                    VkDescriptorBufferInfo *pTmp = new VkDescriptorBufferInfo[count];
                    memcpy(pTmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo,
                           count * sizeof(VkDescriptorBufferInfo));
                    obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo = pTmp;
                }
                if (obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView != nullptr) {
                    VkBufferView *pTmp = new VkBufferView[count];
                    memcpy(pTmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView,
                           count * sizeof(VkBufferView));
                    obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView = pTmp;
                }
            }
        } else {
            obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets = nullptr;
        }

        if (numBindings > 0) {
            VkCopyDescriptorSet *tmp = new VkCopyDescriptorSet[numBindings];
            memcpy(tmp, obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets, numBindings * sizeof(VkCopyDescriptorSet));
            obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets = tmp;
        } else {
            obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets = nullptr;
        }
    }

    return *this;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::copy_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo *pDstStage,
                                                        const VkPipelineShaderStageCreateInfo &srcStage) {
    *pDstStage = srcStage;

    if (srcStage.pName != nullptr) {
        char *pName = new char[strlen(srcStage.pName) + 1]();
        strcpy(pName, srcStage.pName);
        pDstStage->pName = pName;
    }

    if (srcStage.pSpecializationInfo != nullptr) {
        VkSpecializationInfo *pSI = new VkSpecializationInfo();
        *pSI = *(srcStage.pSpecializationInfo);

        if (srcStage.pSpecializationInfo->pData != nullptr && srcStage.pSpecializationInfo->dataSize > 0) {
            void *pData = malloc(srcStage.pSpecializationInfo->dataSize);
            memcpy(pData, srcStage.pSpecializationInfo->pData, srcStage.pSpecializationInfo->dataSize);
            pSI->pData = pData;
        }

        if (pSI->pMapEntries != nullptr) {
            VkSpecializationMapEntry *pSMEs = new VkSpecializationMapEntry[srcStage.pSpecializationInfo->mapEntryCount];
            memcpy(pSMEs, srcStage.pSpecializationInfo->pMapEntries,
                   srcStage.pSpecializationInfo->mapEntryCount * sizeof(VkSpecializationMapEntry));
            pSI->pMapEntries = pSMEs;
        }

        pDstStage->pSpecializationInfo = pSI;
    }
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::delete_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo *pStage) {
    if (pStage->pName != nullptr) {
        delete[] pStage->pName;
    }

    if (pStage->pSpecializationInfo != nullptr) {
        if (pStage->pSpecializationInfo->pData != nullptr) {
            free(const_cast<void *>(pStage->pSpecializationInfo->pData));
        }
        if (pStage->pSpecializationInfo->pMapEntries != nullptr) {
            delete[] pStage->pSpecializationInfo->pMapEntries;
        }
        delete pStage->pSpecializationInfo;
    }
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::copy_VkShaderModuleCreateInfo(VkShaderModuleCreateInfo *pDst, const VkShaderModuleCreateInfo &src) {
    if (pDst != nullptr) {
        *pDst = src;

        if (src.pCode != nullptr) {
            uint32_t *pCodeCopy = static_cast<uint32_t *>(malloc(src.codeSize));
            memcpy(pCodeCopy, src.pCode, src.codeSize);
            pDst->pCode = pCodeCopy;
        }
        pDst->pNext = nullptr;
    }
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::delete_VkShaderModuleCreateInfo(VkShaderModuleCreateInfo *pModule) {
    if (pModule != nullptr) {
        if (pModule->pCode != nullptr) {
            uint32_t *pCode = const_cast<uint32_t *>(pModule->pCode);
            free(pCode);
            pModule->pCode = nullptr;
        }
        pModule->codeSize = 0;
        pModule->pNext = nullptr;
    }
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::copy_VkGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo *pDst, const VkGraphicsPipelineCreateInfo &src) {
    *pDst = src;

    if (src.pStages != nullptr) {
        VkPipelineShaderStageCreateInfo *pStages = new VkPipelineShaderStageCreateInfo[src.stageCount]();
        for (uint32_t i = 0; i < src.stageCount; i++) {
            copy_VkPipelineShaderStageCreateInfo(&pStages[i], src.pStages[i]);
        }

        pDst->pStages = pStages;
    }

    if (src.pVertexInputState != nullptr) {
        VkPipelineVertexInputStateCreateInfo *pVIS = new VkPipelineVertexInputStateCreateInfo();
        *pVIS = *(src.pVertexInputState);
        pDst->pVertexInputState = pVIS;

        if (src.pVertexInputState->pVertexAttributeDescriptions != nullptr) {
            VkVertexInputAttributeDescription *pVIADs =
                new VkVertexInputAttributeDescription[pVIS->vertexAttributeDescriptionCount]();
            memcpy(pVIADs, pVIS->pVertexAttributeDescriptions,
                   pVIS->vertexAttributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
            pVIS->pVertexAttributeDescriptions = pVIADs;
        }

        if (src.pVertexInputState->pVertexBindingDescriptions != nullptr) {
            VkVertexInputBindingDescription *pVIBDs = new VkVertexInputBindingDescription[pVIS->vertexBindingDescriptionCount]();
            memcpy(pVIBDs, pVIS->pVertexBindingDescriptions,
                   pVIS->vertexBindingDescriptionCount * sizeof(VkVertexInputBindingDescription));
            pVIS->pVertexBindingDescriptions = pVIBDs;
        }
    }

    if (src.pInputAssemblyState != nullptr) {
        VkPipelineInputAssemblyStateCreateInfo *pIAS = new VkPipelineInputAssemblyStateCreateInfo();
        *pIAS = *(src.pInputAssemblyState);
        pDst->pInputAssemblyState = pIAS;
    }

    if (src.pTessellationState != nullptr) {
        VkPipelineTessellationStateCreateInfo *pTS = new VkPipelineTessellationStateCreateInfo();
        *pTS = *(src.pTessellationState);
        pDst->pTessellationState = pTS;
    }

    if (src.pViewportState != nullptr) {
        VkPipelineViewportStateCreateInfo *pVS = new VkPipelineViewportStateCreateInfo();
        *pVS = *(src.pViewportState);
        pDst->pViewportState = pVS;

        if (src.pViewportState->pViewports != nullptr) {
            VkViewport *pViewports = new VkViewport[pVS->viewportCount];
            memcpy(pViewports, pVS->pViewports, pVS->viewportCount * sizeof(VkViewport));
            pVS->pViewports = pViewports;
        }

        if (src.pViewportState->pScissors != nullptr) {
            VkRect2D *pScissors = new VkRect2D[pVS->scissorCount];
            memcpy(pScissors, pVS->pScissors, pVS->scissorCount * sizeof(VkRect2D));
            pVS->pScissors = pScissors;
        }
    }

    if (src.pRasterizationState != nullptr) {
        VkPipelineRasterizationStateCreateInfo *pRS = new VkPipelineRasterizationStateCreateInfo();
        *pRS = *(src.pRasterizationState);
        pDst->pRasterizationState = pRS;
        if (pDst->pRasterizationState->pNext != nullptr) {
            // there's an extension struct here, we need to do a deep copy for it.

            // we first use sType to detect the type of extension that pNext
            // struct belong to.
            const VkApplicationInfo *pNextStruct =
                reinterpret_cast<const VkApplicationInfo *>(pDst->pRasterizationState->pNext);
            if (pNextStruct->sType ==
                VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD) {  // it's an AMD extension.

                // cast to a reference so we can change pDst->pRasterizationState->pNext,
                void*& pNext = const_cast<void*&>(pDst->pRasterizationState->pNext);
                // then make pNext point to the newly created extension struct.
                pNext = reinterpret_cast<void *>(new VkPipelineRasterizationStateRasterizationOrderAMD());
                //make a copy of extension struct which is used by target app.
                memcpy(pNext, src.pRasterizationState->pNext,sizeof(VkPipelineRasterizationStateRasterizationOrderAMD));
            }
            else {
                // so far we only handle this extension, more extension
                // handling can be added here;
                assert(false);
            }
        }
    }

    if (src.pMultisampleState != nullptr) {
        VkPipelineMultisampleStateCreateInfo *pMS = new VkPipelineMultisampleStateCreateInfo();
        *pMS = *(src.pMultisampleState);
        pDst->pMultisampleState = pMS;

        if (src.pMultisampleState->pSampleMask != nullptr) {
            VkSampleMask *pMask = new VkSampleMask();
            *pMask = *(pMS->pSampleMask);
            pMS->pSampleMask = pMask;
        }
    }

    if (src.pDepthStencilState != nullptr) {
        VkPipelineDepthStencilStateCreateInfo *pDSS = new VkPipelineDepthStencilStateCreateInfo();
        *pDSS = *(src.pDepthStencilState);
        pDst->pDepthStencilState = pDSS;
    }

    if (src.pColorBlendState != nullptr) {
        VkPipelineColorBlendStateCreateInfo *pCBS = new VkPipelineColorBlendStateCreateInfo();
        *pCBS = *(src.pColorBlendState);
        pDst->pColorBlendState = pCBS;

        if (src.pColorBlendState->pAttachments != nullptr) {
            VkPipelineColorBlendAttachmentState *pAttachments = new VkPipelineColorBlendAttachmentState[pCBS->attachmentCount];
            memcpy(pAttachments, pCBS->pAttachments, pCBS->attachmentCount * sizeof(VkPipelineColorBlendAttachmentState));
            pCBS->pAttachments = pAttachments;
        }
    }

    if (src.pDynamicState != nullptr) {
        VkPipelineDynamicStateCreateInfo *pDS = new VkPipelineDynamicStateCreateInfo();
        *pDS = *(src.pDynamicState);
        pDst->pDynamicState = pDS;

        if (src.pDynamicState->pDynamicStates != nullptr) {
            VkDynamicState *pDynamicStates = new VkDynamicState[pDS->dynamicStateCount];
            memcpy(pDynamicStates, pDS->pDynamicStates, pDS->dynamicStateCount * sizeof(VkDynamicState));
            pDS->pDynamicStates = pDynamicStates;
        }
    }
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::copy_VkComputePipelineCreateInfo(VkComputePipelineCreateInfo *pDst, const VkComputePipelineCreateInfo &src) {
    *pDst = src;

    copy_VkPipelineShaderStageCreateInfo(&pDst->stage, src.stage);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
ObjectInfo &StateTracker::add_Instance(VkInstance var) {
    ObjectInfo &info = createdInstances[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_PhysicalDevice(VkPhysicalDevice var) {
    ObjectInfo &info = createdPhysicalDevices[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Device(VkDevice var) {
    ObjectInfo &info = createdDevices[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_SurfaceKHR(VkSurfaceKHR var) {
    ObjectInfo &info = createdSurfaceKHRs[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_CommandPool(VkCommandPool var) {
    ObjectInfo &info = createdCommandPools[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_CommandBuffer(VkCommandBuffer var) {
    ObjectInfo &info = createdCommandBuffers[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_DescriptorPool(VkDescriptorPool var) {
    ObjectInfo &info = createdDescriptorPools[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_RenderPass(VkRenderPass var) {
    ObjectInfo &info = createdRenderPasss[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_PipelineCache(VkPipelineCache var) {
    ObjectInfo &info = createdPipelineCaches[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Pipeline(VkPipeline var) {
    ObjectInfo &info = createdPipelines[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Queue(VkQueue var) {
    ObjectInfo &info = createdQueues[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Semaphore(VkSemaphore var) {
    ObjectInfo &info = createdSemaphores[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_DeviceMemory(VkDeviceMemory var) {
    ObjectInfo &info = createdDeviceMemorys[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Fence(VkFence var) {
    ObjectInfo &info = createdFences[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_SwapchainKHR(VkSwapchainKHR var) {
    ObjectInfo &info = createdSwapchainKHRs[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Image(VkImage var) {
    ObjectInfo &info = createdImages[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_ImageView(VkImageView var) {
    ObjectInfo &info = createdImageViews[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Buffer(VkBuffer var) {
    ObjectInfo &info = createdBuffers[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_BufferView(VkBufferView var) {
    ObjectInfo &info = createdBufferViews[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Framebuffer(VkFramebuffer var) {
    ObjectInfo &info = createdFramebuffers[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Event(VkEvent var) {
    ObjectInfo &info = createdEvents[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_QueryPool(VkQueryPool var) {
    ObjectInfo &info = createdQueryPools[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_ShaderModule(VkShaderModule var) {
    ObjectInfo &info = createdShaderModules[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_PipelineLayout(VkPipelineLayout var) {
    ObjectInfo &info = createdPipelineLayouts[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_Sampler(VkSampler var) {
    ObjectInfo &info = createdSamplers[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_DescriptorSetLayout(VkDescriptorSetLayout var) {
    ObjectInfo &info = createdDescriptorSetLayouts[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_DescriptorSet(VkDescriptorSet var) {
    ObjectInfo &info = createdDescriptorSets[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

ObjectInfo &StateTracker::add_DescriptorUpdateTemplate(VkDescriptorUpdateTemplate var) {
    ObjectInfo &info = createdDescriptorUpdateTemplates[var];
    memset(&info, 0, sizeof(ObjectInfo));
    info.vkObject = (uint64_t)var;
    return info;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
ObjectInfo *StateTracker::get_Instance(VkInstance var) {
    auto iter = createdInstances.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdInstances.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_PhysicalDevice(VkPhysicalDevice var) {
    auto iter = createdPhysicalDevices.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdPhysicalDevices.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Device(VkDevice var) {
    auto iter = createdDevices.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdDevices.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_SurfaceKHR(VkSurfaceKHR var) {
    auto iter = createdSurfaceKHRs.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdSurfaceKHRs.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Queue(VkQueue var) {
    auto iter = createdQueues.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdQueues.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_SwapchainKHR(VkSwapchainKHR var) {
    auto iter = createdSwapchainKHRs.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdSwapchainKHRs.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_CommandPool(VkCommandPool var) {
    auto iter = createdCommandPools.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdCommandPools.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_CommandBuffer(VkCommandBuffer var) {
    auto iter = createdCommandBuffers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdCommandBuffers.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_DeviceMemory(VkDeviceMemory var) {
    auto iter = createdDeviceMemorys.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdDeviceMemorys.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_ImageView(VkImageView var) {
    auto iter = createdImageViews.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdImageViews.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Image(VkImage var) {
    auto iter = createdImages.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdImages.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_BufferView(VkBufferView var) {
    auto iter = createdBufferViews.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdBufferViews.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Buffer(VkBuffer var) {
    auto iter = createdBuffers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdBuffers.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Sampler(VkSampler var) {
    auto iter = createdSamplers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdSamplers.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_DescriptorSetLayout(VkDescriptorSetLayout var) {
    auto iter = createdDescriptorSetLayouts.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdDescriptorSetLayouts.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_DescriptorUpdateTemplate(VkDescriptorUpdateTemplate var) {
    auto iter = createdDescriptorUpdateTemplates.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdDescriptorUpdateTemplates.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_PipelineLayout(VkPipelineLayout var) {
    auto iter = createdPipelineLayouts.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdPipelineLayouts.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_RenderPass(VkRenderPass var) {
    auto iter = createdRenderPasss.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdRenderPasss.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_ShaderModule(VkShaderModule var) {
    auto iter = createdShaderModules.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdShaderModules.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_PipelineCache(VkPipelineCache var) {
    auto iter = createdPipelineCaches.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdPipelineCaches.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_DescriptorPool(VkDescriptorPool var) {
    auto iter = createdDescriptorPools.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdDescriptorPools.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Pipeline(VkPipeline var) {
    auto iter = createdPipelines.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdPipelines.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

std::set<VkCommandBuffer> *StateTracker::get_BoundCommandBuffers(VkPipeline var, bool createFlag) {
    auto iter = m_BindingPipelineTocmdBuffersMap.find(var);
    std::set<VkCommandBuffer> *pResult = NULL;
    if (iter != m_BindingPipelineTocmdBuffersMap.end()) {
        pResult = &(iter->second);
    } else {
        if (createFlag) {
            pResult = &m_BindingPipelineTocmdBuffersMap[var];
        }
    }
    return pResult;
}

std::set<VkPipeline> *StateTracker::get_BindingPipelines(VkCommandBuffer var, bool createFlag) {
    auto iter = m_cmdBufferToBindingPipelinesMap.find(var);
    std::set<VkPipeline> *pResult = NULL;
    if (iter != m_cmdBufferToBindingPipelinesMap.end()) {
        pResult = &(iter->second);
    } else {
        if (createFlag) {
            pResult = &m_cmdBufferToBindingPipelinesMap[var];
        }
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Semaphore(VkSemaphore var) {
    auto iter = createdSemaphores.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdSemaphores.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Fence(VkFence var) {
    auto iter = createdFences.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdFences.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Framebuffer(VkFramebuffer var) {
    auto iter = createdFramebuffers.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdFramebuffers.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_Event(VkEvent var) {
    auto iter = createdEvents.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdEvents.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_QueryPool(VkQueryPool var) {
    auto iter = createdQueryPools.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdQueryPools.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

ObjectInfo *StateTracker::get_DescriptorSet(VkDescriptorSet var) {
    auto iter = createdDescriptorSets.find(var);
    ObjectInfo *pResult = NULL;
    if (iter != createdDescriptorSets.end()) {
        pResult = &(iter->second);
    }
    return pResult;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void StateTracker::remove_Instance(const VkInstance var) {
    ObjectInfo *pInfo = get_Instance(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Instance.pCreatePacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket);
    }
    createdInstances.erase(var);
}

void StateTracker::remove_PhysicalDevice(const VkPhysicalDevice var) {
    ObjectInfo *pInfo = get_PhysicalDevice(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRCountPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyProperties2KHRPacket);
    }
    createdPhysicalDevices.erase(var);
}

void StateTracker::remove_Device(const VkDevice var) {
    ObjectInfo *pInfo = get_Device(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Device.pCreatePacket);
    }

    for (uint32_t family = 0; family < pInfo->ObjectInfo.Device.queueFamilyCount; family++) {
        VKTRACE_DELETE(pInfo->ObjectInfo.Device.pQueueFamilies[family].queues);
    }
    VKTRACE_DELETE(pInfo->ObjectInfo.Device.pQueueFamilies);

    createdDevices.erase(var);
}

void StateTracker::remove_SurfaceKHR(const VkSurfaceKHR var) {
    ObjectInfo *pInfo = get_SurfaceKHR(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.SurfaceKHR.pCreatePacket);
    }
    createdSurfaceKHRs.erase(var);
}

void StateTracker::remove_Queue(const VkQueue var) {
    ObjectInfo *pInfo = get_Queue(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Queue.pCreatePacket);
    }
    createdQueues.erase(var);
}

void StateTracker::remove_CommandPool(const VkCommandPool var) {
    ObjectInfo *pInfo = get_CommandPool(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.CommandPool.pCreatePacket);
    }
    createdCommandPools.erase(var);
}

void StateTracker::remove_SwapchainKHR(const VkSwapchainKHR var) {
    ObjectInfo *pInfo = get_SwapchainKHR(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.SwapchainKHR.pCreatePacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket);
    }
    createdSwapchainKHRs.erase(var);
}

void StateTracker::remove_CommandBuffer(const VkCommandBuffer var) { createdCommandBuffers.erase(var); }

void StateTracker::remove_DeviceMemory(const VkDeviceMemory var) {
    ObjectInfo *pInfo = get_DeviceMemory(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pCreatePacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pMapMemoryPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pUnmapMemoryPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket);
    }
    createdDeviceMemorys.erase(var);
}

void StateTracker::remove_Image(const VkImage var) {
    ObjectInfo *pInfo = get_Image(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pCreatePacket);
#if !TRIM_USE_ORDERED_IMAGE_CREATION
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pGetImageMemoryRequirementsPacket);
#endif  //! TRIM_USE_ORDERED_IMAGE_CREATION
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pBindImageMemoryPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pMapMemoryPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pUnmapMemoryPacket);
    }
    createdImages.erase(var);
}

void StateTracker::remove_ImageView(const VkImageView var) {
    ObjectInfo *pInfo = get_ImageView(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.ImageView.pCreatePacket);
    }
    createdImageViews.erase(var);
}

void StateTracker::remove_Buffer(const VkBuffer var) {
    ObjectInfo *pInfo = get_Buffer(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pCreatePacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pBindBufferMemoryPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pMapMemoryPacket);
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pUnmapMemoryPacket);
    }
    createdBuffers.erase(var);
}

void StateTracker::remove_BufferView(const VkBufferView var) {
    ObjectInfo *pInfo = get_BufferView(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.BufferView.pCreatePacket);
    }
    createdBufferViews.erase(var);
}

void StateTracker::remove_Sampler(const VkSampler var) {
    ObjectInfo *pInfo = get_Sampler(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Sampler.pCreatePacket);
    }
    createdSamplers.erase(var);
}

void StateTracker::remove_DescriptorSetLayout(const VkDescriptorSetLayout var) {
    ObjectInfo *pInfo = get_DescriptorSetLayout(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DescriptorSetLayout.pCreatePacket);

        if (pInfo->ObjectInfo.DescriptorSetLayout.pBindings != nullptr) {
            delete[] pInfo->ObjectInfo.DescriptorSetLayout.pBindings;
            pInfo->ObjectInfo.DescriptorSetLayout.pBindings = nullptr;
            pInfo->ObjectInfo.DescriptorSetLayout.bindingCount = 0;
        }
    }
    createdDescriptorSetLayouts.erase(var);
}

void StateTracker::remove_DescriptorUpdateTemplate(const VkDescriptorUpdateTemplate var) {
    ObjectInfo *pInfo = get_DescriptorUpdateTemplate(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DescriptorUpdateTemplate.pCreatePacket);
        if ((pInfo->ObjectInfo.DescriptorUpdateTemplate.descriptorUpdateEntryCount != 0) &&
            (pInfo->ObjectInfo.DescriptorUpdateTemplate.pDescriptorUpdateEntries != nullptr)) {
            delete[] pInfo->ObjectInfo.DescriptorUpdateTemplate.pDescriptorUpdateEntries;
        }
    }
    createdDescriptorUpdateTemplates.erase(var);
}

void StateTracker::remove_PipelineLayout(const VkPipelineLayout var) {
    ObjectInfo *pInfo = get_PipelineLayout(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PipelineLayout.pCreatePacket);
        if (pInfo->ObjectInfo.PipelineLayout.pDescriptorSetLayouts != nullptr) {
            delete[] pInfo->ObjectInfo.PipelineLayout.pDescriptorSetLayouts;
            pInfo->ObjectInfo.PipelineLayout.pDescriptorSetLayouts = nullptr;
            pInfo->ObjectInfo.PipelineLayout.descriptorSetLayoutCount = 0;
        }
    }
    createdPipelineLayouts.erase(var);
}

void StateTracker::remove_RenderPass(const VkRenderPass var) {
    ObjectInfo *pInfo = get_RenderPass(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.RenderPass.pCreatePacket);
    }
    createdRenderPasss.erase(var);
}

void StateTracker::remove_ShaderModule(const VkShaderModule var) {
    ObjectInfo *pInfo = get_ShaderModule(var);
    if (pInfo != nullptr) {
        uint32_t *pCode = const_cast<uint32_t *>(pInfo->ObjectInfo.ShaderModule.createInfo.pCode);
        free(pCode);
        pInfo->ObjectInfo.ShaderModule.createInfo.pCode = nullptr;
    }
    createdShaderModules.erase(var);
}

void StateTracker::remove_PipelineCache(const VkPipelineCache var) {
    ObjectInfo *pInfo = get_PipelineCache(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.PipelineCache.pCreatePacket);
    }
    createdPipelineCaches.erase(var);
}

void StateTracker::remove_Pipeline(const VkPipeline var) {
    ObjectInfo *pInfo = get_Pipeline(var);
    if (pInfo != nullptr) {
        for (uint32_t i = 0; i < pInfo->ObjectInfo.Pipeline.shaderModuleCreateInfoCount; i++) {
            delete_VkShaderModuleCreateInfo(&pInfo->ObjectInfo.Pipeline.pShaderModuleCreateInfos[i]);
        }
        VKTRACE_DELETE(pInfo->ObjectInfo.Pipeline.pShaderModuleCreateInfos);
        pInfo->ObjectInfo.Pipeline.shaderModuleCreateInfoCount = 0;

        delete_VkPipelineShaderStageCreateInfo(&pInfo->ObjectInfo.Pipeline.computePipelineCreateInfo.stage);

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages != nullptr) {
            for (uint32_t i = 0; i < pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.stageCount; ++i) {
                delete_VkPipelineShaderStageCreateInfo(const_cast<VkPipelineShaderStageCreateInfo *>(
                    &pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages[i]));
            }

            delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState != nullptr) {
            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexAttributeDescriptions != nullptr) {
                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexAttributeDescriptions;
            }
            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexBindingDescriptions != nullptr) {
                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexBindingDescriptions;
            }

            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pInputAssemblyState != nullptr) {
            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pInputAssemblyState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pTessellationState != nullptr) {
            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pTessellationState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState != nullptr) {
            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pViewports != nullptr) {
                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pViewports;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pScissors != nullptr) {
                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pScissors;
            }

            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pRasterizationState != nullptr) {
            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pRasterizationState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState != nullptr) {
            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState->pSampleMask != nullptr) {
                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState->pSampleMask;
            }

            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDepthStencilState != nullptr) {
            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDepthStencilState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState != nullptr) {
            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState->pAttachments != nullptr) {
                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState->pAttachments;
            }

            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState;
        }

        if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState != nullptr) {
            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState->pDynamicStates != nullptr) {
                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState->pDynamicStates;
            }

            delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState;
        }
    }
    createdPipelines.erase(var);
}

void StateTracker::remove_DescriptorPool(const VkDescriptorPool var) {
    ObjectInfo *pInfo = get_DescriptorPool(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.DescriptorPool.pCreatePacket);
    }
    createdDescriptorPools.erase(var);
}

void StateTracker::remove_DescriptorSet(const VkDescriptorSet var) {
    ObjectInfo *pInfo = get_DescriptorSet(var);
    if (pInfo != nullptr) {
        if (pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets != nullptr) {
            delete[] pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets;
            pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets = nullptr;
        }
        if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets != nullptr) {
            for (uint32_t s = 0; s < pInfo->ObjectInfo.DescriptorSet.numBindings; s++) {
                if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo != nullptr) {
                    delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo;
                    pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo = nullptr;
                }
                if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo != nullptr) {
                    delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo;
                    pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo = nullptr;
                }
                if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView != nullptr) {
                    delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView;
                    pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView = nullptr;
                }
            }

            delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets;
            pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets = nullptr;
        }
    }
    createdDescriptorSets.erase(var);
}

void StateTracker::remove_Framebuffer(const VkFramebuffer var) {
    ObjectInfo *pInfo = get_Framebuffer(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Framebuffer.pCreatePacket);
    }
    createdFramebuffers.erase(var);
}

void StateTracker::remove_Semaphore(const VkSemaphore var) {
    ObjectInfo *pInfo = get_Semaphore(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Semaphore.pCreatePacket);
    }
    createdSemaphores.erase(var);
}

void StateTracker::remove_Fence(const VkFence var) { createdFences.erase(var); }

void StateTracker::remove_Event(const VkEvent var) {
    ObjectInfo *pInfo = get_Event(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.Event.pCreatePacket);
    }
    createdEvents.erase(var);
}

void StateTracker::remove_QueryPool(const VkQueryPool var) {
    ObjectInfo *pInfo = get_QueryPool(var);
    if (pInfo != nullptr) {
        vktrace_delete_trace_packet(&pInfo->ObjectInfo.QueryPool.pCreatePacket);
        pInfo->ObjectInfo.QueryPool.size = 0;
        if (pInfo->ObjectInfo.QueryPool.pResultsAvailable != nullptr) {
            delete[] pInfo->ObjectInfo.QueryPool.pResultsAvailable;
            pInfo->ObjectInfo.QueryPool.pResultsAvailable = nullptr;
        }
    }
    createdQueryPools.erase(var);
}
}
