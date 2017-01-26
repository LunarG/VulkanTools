/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once
#include "vktrace_lib_trim_statetracker.h"
#include "vktrace_lib_trim.h"

namespace trim
{
    // declared extern in statetracker.h
    VKTRACE_CRITICAL_SECTION trimTransitionMapLock;

    //-------------------------------------------------------------------------
#define COPY_PACKET(packet) packet = copy_packet(packet)

    vktrace_trace_packet_header* copy_packet(vktrace_trace_packet_header* pHeader)
    {
        if (pHeader == nullptr)
        {
            return nullptr;
        }

        uint64_t packetSize = pHeader->size;
        vktrace_trace_packet_header* pCopy = static_cast<vktrace_trace_packet_header*>(malloc(packetSize));
        memcpy(pCopy, pHeader, packetSize);
        return pCopy;
    }

    //-------------------------------------------------------------------------
    void StateTracker::AddImageTransition(VkCommandBuffer commandBuffer, ImageTransition transition)
    {
        vktrace_enter_critical_section(&trimTransitionMapLock);
        m_cmdBufferToImageTransitionsMap[commandBuffer].push_back(transition);
        vktrace_leave_critical_section(&trimTransitionMapLock);
    }

    //-------------------------------------------------------------------------
    void StateTracker::ClearImageTransitions(VkCommandBuffer commandBuffer)
    {
        vktrace_enter_critical_section(&trimTransitionMapLock);
        m_cmdBufferToImageTransitionsMap[commandBuffer].clear();
        m_cmdBufferToImageTransitionsMap.erase(commandBuffer);
        vktrace_leave_critical_section(&trimTransitionMapLock);
    }

    //-------------------------------------------------------------------------
    void StateTracker::AddBufferTransition(VkCommandBuffer commandBuffer, BufferTransition transition)
    {
        vktrace_enter_critical_section(&trimTransitionMapLock);
        m_cmdBufferToBufferTransitionsMap[commandBuffer].push_back(transition);
        vktrace_leave_critical_section(&trimTransitionMapLock);
    }

    void StateTracker::ClearBufferTransitions(VkCommandBuffer commandBuffer)
    {
        vktrace_enter_critical_section(&trimTransitionMapLock);
        m_cmdBufferToBufferTransitionsMap[commandBuffer].clear();
        m_cmdBufferToBufferTransitionsMap.erase(commandBuffer);
        vktrace_leave_critical_section(&trimTransitionMapLock);
    }

    //-------------------------------------------------------------------------
    StateTracker::StateTracker()
    {
    }

    StateTracker::StateTracker(const StateTracker& other)
    {
        *this = other;
    }

    //-------------------------------------------------------------------------
    StateTracker::~StateTracker()
    {
        clear();
    }

    //-------------------------------------------------------------------------
    void StateTracker::add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader) {
        if (pHeader != NULL)
        {
            m_cmdBufferPackets[commandBuffer].push_back(pHeader);
        }
    }

    //-------------------------------------------------------------------------
    void StateTracker::remove_CommandBuffer_calls(VkCommandBuffer commandBuffer)
    {
        std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header*>>::iterator cmdBufferMap = m_cmdBufferPackets.find(commandBuffer);
        if (cmdBufferMap != m_cmdBufferPackets.end())
        {
            for (auto packet = cmdBufferMap->second.begin(); packet != cmdBufferMap->second.end(); ++packet)
            {
                vktrace_trace_packet_header* pHeader = *packet;
                vktrace_delete_trace_packet(&pHeader);
            }
            cmdBufferMap->second.clear();

            m_cmdBufferPackets.erase(commandBuffer);
        }
    }

#if TRIM_USE_ORDERED_IMAGE_CREATION
    void StateTracker::add_Image_call(vktrace_trace_packet_header* pHeader)
    {
        m_image_calls.push_back(pHeader);
    }
#endif //TRIM_USE_ORDERED_IMAGE_CREATION

    //-------------------------------------------------------------------------
    void StateTracker::clear()
    {
        while (createdInstances.size() != 0)
        {
            remove_Instance(static_cast<VkInstance>(createdInstances.begin()->first));
        }

        while (createdPhysicalDevices.size() != 0)
        {
            remove_PhysicalDevice(static_cast<VkPhysicalDevice>(createdPhysicalDevices.begin()->first));
        }

        while (createdDevices.size() != 0)
        {
            remove_Device(static_cast<VkDevice>(createdDevices.begin()->first));
        }

        while (createdSurfaceKHRs.size() != 0)
        {
            remove_SurfaceKHR(static_cast<VkSurfaceKHR>(createdSurfaceKHRs.begin()->first));
        }

        while (createdCommandPools.size() != 0)
        {
            remove_CommandPool(static_cast<VkCommandPool>(createdCommandPools.begin()->first));
        }

        while (createdCommandBuffers.size() != 0)
        {
            remove_CommandBuffer(static_cast<VkCommandBuffer>(createdCommandBuffers.begin()->first));
        }

        while (createdDescriptorPools.size() != 0)
        {
            remove_DescriptorPool(static_cast<VkDescriptorPool>(createdDescriptorPools.begin()->first));
        }

        while (createdRenderPasss.size() != 0)
        {
            remove_RenderPass(static_cast<VkRenderPass>(createdRenderPasss.begin()->first));
        }

        while (createdPipelineCaches.size() != 0)
        {
            remove_PipelineCache(static_cast<VkPipelineCache>(createdPipelineCaches.begin()->first));
        }

        while (createdPipelines.size() != 0)
        {
            remove_Pipeline(static_cast<VkPipeline>(createdPipelines.begin()->first));
        }

        while (createdQueues.size() != 0)
        {
            remove_Queue(static_cast<VkQueue>(createdQueues.begin()->first));
        }

        while (createdSemaphores.size() != 0)
        {
            remove_Semaphore(static_cast<VkSemaphore>(createdSemaphores.begin()->first));
        }

        while (createdDeviceMemorys.size() != 0)
        {
            remove_DeviceMemory(static_cast<VkDeviceMemory>(createdDeviceMemorys.begin()->first));
        }

        while (createdFences.size() != 0)
        {
            remove_Fence(static_cast<VkFence>(createdFences.begin()->first));
        }

        while (createdSwapchainKHRs.size() != 0)
        {
            remove_SwapchainKHR(static_cast<VkSwapchainKHR>(createdSwapchainKHRs.begin()->first));
        }

        while (createdImages.size() != 0)
        {
            remove_Image(static_cast<VkImage>(createdImages.begin()->first));
        }

        while (createdImageViews.size() != 0)
        {
            remove_ImageView(static_cast<VkImageView>(createdImageViews.begin()->first));
        }

        while (createdBuffers.size() != 0)
        {
            remove_Buffer(static_cast<VkBuffer>(createdBuffers.begin()->first));
        }

        while (createdBufferViews.size() != 0)
        {
            remove_BufferView(static_cast<VkBufferView>(createdBufferViews.begin()->first));
        }

        while (createdFramebuffers.size() != 0)
        {
            remove_Framebuffer(static_cast<VkFramebuffer>(createdFramebuffers.begin()->first));
        }

        while (createdEvents.size() != 0)
        {
            remove_Event(static_cast<VkEvent>(createdEvents.begin()->first));
        }

        while (createdQueryPools.size() != 0)
        {
            remove_QueryPool(static_cast<VkQueryPool>(createdQueryPools.begin()->first));
        }

        while (createdShaderModules.size() != 0)
        {
            remove_ShaderModule(static_cast<VkShaderModule>(createdShaderModules.begin()->first));
        }

        while (createdPipelineLayouts.size() != 0)
        {
            remove_PipelineLayout(static_cast<VkPipelineLayout>(createdPipelineLayouts.begin()->first));
        }

        while (createdSamplers.size() != 0)
        {
            remove_Sampler(static_cast<VkSampler>(createdSamplers.begin()->first));
        }

        while (createdDescriptorSetLayouts.size() != 0)
        {
            remove_DescriptorSetLayout(static_cast<VkDescriptorSetLayout>(createdDescriptorSetLayouts.begin()->first));
        }

        while (createdDescriptorSets.size() != 0)
        {
            remove_DescriptorSet(static_cast<VkDescriptorSet>(createdDescriptorSets.begin()->first));
        }

        for (auto iter = m_cmdBufferPackets.begin(); iter != m_cmdBufferPackets.end(); ++iter)
        {
            std::list<vktrace_trace_packet_header*>& packets = iter->second;
            for (auto packetIter = iter->second.begin(); packetIter != iter->second.end(); ++packetIter)
            {
                vktrace_trace_packet_header* pHeader = *packetIter;
                vktrace_delete_trace_packet(&pHeader);
            }
            packets.clear();
        }
        m_cmdBufferPackets.clear();

        for (auto packet = m_image_calls.begin(); packet != m_image_calls.end(); ++packet)
        {
            vktrace_trace_packet_header* pHeader = *packet;
            vktrace_delete_trace_packet(&pHeader);
        }
        m_image_calls.clear();

        for (auto renderPassIter = m_renderPassVersions.begin(); renderPassIter != m_renderPassVersions.end(); ++renderPassIter)
        {
            std::vector<VkRenderPassCreateInfo*> versions = renderPassIter->second;

            for (uint32_t i = 0; i < versions.size(); i++)
            {
                VkRenderPassCreateInfo* pCreateInfo = versions[i];

                VkSubpassDescription* pSubPasses = const_cast<VkSubpassDescription*>(pCreateInfo->pSubpasses);
                for (uint32_t i = 0; i < pCreateInfo->subpassCount; i++)
                {
                    if (pCreateInfo->pSubpasses[i].pInputAttachments != nullptr)
                    {
                        delete[] pCreateInfo->pSubpasses[i].pInputAttachments;
                    }

                    if (pCreateInfo->pSubpasses[i].pColorAttachments != nullptr)
                    {
                        delete[] pCreateInfo->pSubpasses[i].pColorAttachments;
                    }

                    if (pCreateInfo->pSubpasses[i].pResolveAttachments != nullptr)
                    {
                        delete[] pCreateInfo->pSubpasses[i].pResolveAttachments;
                    }

                    if (pCreateInfo->pSubpasses[i].pDepthStencilAttachment != nullptr)
                    {
                        delete[] pCreateInfo->pSubpasses[i].pDepthStencilAttachment;
                    }

                    if (pCreateInfo->pSubpasses[i].preserveAttachmentCount > 0)
                    {
                        delete[] pCreateInfo->pSubpasses[i].pPreserveAttachments;
                    }
                }

                if (pCreateInfo->pAttachments != nullptr)
                {
                    delete [] pCreateInfo->pAttachments;
                }

                delete pCreateInfo;
            }
            versions.clear();
        }
        m_renderPassVersions.clear();
    }

    //-------------------------------------------------------------------------
    void StateTracker::add_RenderPassCreateInfo(VkRenderPass renderPass, const VkRenderPassCreateInfo* pCreateInfo)
    {
        VkRenderPassCreateInfo* pCopyCreateInfo = new VkRenderPassCreateInfo();
        *pCopyCreateInfo = *pCreateInfo;

        VkSubpassDescription* pSubPasses = new VkSubpassDescription[pCreateInfo->subpassCount];
        for (uint32_t i = 0; i < pCreateInfo->subpassCount; i++)
        {
            pSubPasses[i] = pCreateInfo->pSubpasses[i];

            if (pCreateInfo->pSubpasses[i].inputAttachmentCount > 0)
            {
                VkAttachmentReference* pInputAttachments = new VkAttachmentReference[pCopyCreateInfo->pSubpasses[i].inputAttachmentCount];
                for (uint32_t j = 0; j < pCreateInfo->pSubpasses[i].inputAttachmentCount; j++)
                {
                    pInputAttachments[j] = pCreateInfo->pSubpasses[i].pInputAttachments[j];
                }
                pSubPasses[i].pInputAttachments = pInputAttachments;
            }

            if (pCreateInfo->pSubpasses[i].colorAttachmentCount > 0)
            {
                VkAttachmentReference* pColorAttachments = new VkAttachmentReference[pCreateInfo->pSubpasses[i].colorAttachmentCount];
                for (uint32_t j = 0; j < pCreateInfo->pSubpasses[i].colorAttachmentCount; j++)
                {
                    pColorAttachments[j] = pCreateInfo->pSubpasses[i].pColorAttachments[j];
                }
                pSubPasses[i].pColorAttachments = pColorAttachments;

                if (pCreateInfo->pSubpasses[i].pResolveAttachments != nullptr)
                {
                    VkAttachmentReference* pResolveAttachments = new VkAttachmentReference[pCreateInfo->pSubpasses[i].colorAttachmentCount];
                    for (uint32_t j = 0; j < pCreateInfo->pSubpasses[i].colorAttachmentCount; j++)
                    {
                        pResolveAttachments[j] = pCreateInfo->pSubpasses[i].pResolveAttachments[j];
                    }
                    pSubPasses[i].pResolveAttachments = pResolveAttachments;
                }
            }

            if (pCreateInfo->pSubpasses[i].pDepthStencilAttachment != nullptr)
            {
                VkAttachmentReference* pDepthStencilAttachment = new VkAttachmentReference();
                *pDepthStencilAttachment = *pCreateInfo->pSubpasses[i].pDepthStencilAttachment;
                pSubPasses[i].pDepthStencilAttachment = pDepthStencilAttachment;
            }

            if (pCreateInfo->pSubpasses[i].preserveAttachmentCount > 0)
            {
                uint32_t* pPreserveAttachments = new uint32_t[pCreateInfo->pSubpasses[i].preserveAttachmentCount];
                for (uint32_t j = 0; j < pCreateInfo->pSubpasses[i].preserveAttachmentCount; j++)
                {
                    pPreserveAttachments[j] = pCreateInfo->pSubpasses[i].pPreserveAttachments[j];
                }
                pSubPasses[i].pPreserveAttachments = pPreserveAttachments;
            }
        }

        pCopyCreateInfo->pSubpasses = pSubPasses;

        if (pCreateInfo->attachmentCount > 0)
        {
            VkAttachmentDescription* pAttachments = new VkAttachmentDescription[pCreateInfo->attachmentCount];
            for (uint32_t i = 0; i < pCreateInfo->attachmentCount; i++)
            {
                pAttachments[i] = pCreateInfo->pAttachments[i];
            }

            pCopyCreateInfo->pAttachments = pAttachments;
        }


        m_renderPassVersions[renderPass].push_back(pCopyCreateInfo);
    }

    //-------------------------------------------------------------------------
    uint32_t StateTracker::get_RenderPassVersion(VkRenderPass renderPass)
    {
        return m_renderPassVersions[renderPass].size() - 1;
    }

    //-------------------------------------------------------------------------
    VkRenderPassCreateInfo* StateTracker::get_RenderPassCreateInfo(VkRenderPass renderPass, uint32_t version)
    {
        return m_renderPassVersions[renderPass][version];
    }

    //-------------------------------------------------------------------------
    StateTracker& StateTracker::operator= (const StateTracker& other)
    {
        if (this == &other)
            return *this;

        m_renderPassVersions = other.m_renderPassVersions;

        for (auto iter = other.m_cmdBufferPackets.cbegin(); iter != other.m_cmdBufferPackets.cend(); ++iter)
        {
            std::list<vktrace_trace_packet_header*> packets;
            for (auto packetIter = iter->second.cbegin(); packetIter != iter->second.cend(); ++packetIter)
            {
                packets.push_back(copy_packet(*packetIter));
            }
            m_cmdBufferPackets[iter->first] = packets;
        }

        for (auto packet = other.m_image_calls.cbegin(); packet != other.m_image_calls.cend(); ++packet)
        {
            m_image_calls.push_back(copy_packet(*packet));
        }

        createdInstances = other.createdInstances;
        for (TrimObjectInfoMap::iterator obj = createdInstances.begin(); obj != createdInstances.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Instance.pCreatePacket);
            COPY_PACKET(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket);
            COPY_PACKET(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket);
        }

        createdPhysicalDevices = other.createdPhysicalDevices;
        for (TrimObjectInfoMap::iterator obj = createdPhysicalDevices.begin(); obj != createdPhysicalDevices.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket);
            COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceSupportKHRPacket);
            COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket);
            COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket);
            COPY_PACKET(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket);
        }

        createdDevices = other.createdDevices;
        for (TrimObjectInfoMap::iterator obj = createdDevices.begin(); obj != createdDevices.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Device.pCreatePacket);
        }

        createdSurfaceKHRs = other.createdSurfaceKHRs;
        for (TrimObjectInfoMap::iterator obj = createdSurfaceKHRs.begin(); obj != createdSurfaceKHRs.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket);
        }

        createdCommandPools = other.createdCommandPools;
        for (TrimObjectInfoMap::iterator obj = createdCommandPools.begin(); obj != createdCommandPools.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.CommandPool.pCreatePacket);
        }

        createdCommandBuffers = other.createdCommandBuffers;

        createdDescriptorPools = other.createdDescriptorPools;
        for (TrimObjectInfoMap::iterator obj = createdDescriptorPools.begin(); obj != createdDescriptorPools.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.DescriptorPool.pCreatePacket);
        }

        createdSwapchainKHRs = other.createdSwapchainKHRs;
        for (TrimObjectInfoMap::iterator obj = createdSwapchainKHRs.begin(); obj != createdSwapchainKHRs.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket);
            COPY_PACKET(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket);
            COPY_PACKET(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket);
        }

        createdRenderPasss = other.createdRenderPasss;
        for (TrimObjectInfoMap::iterator obj = createdRenderPasss.begin(); obj != createdRenderPasss.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.RenderPass.pCreatePacket);
        }

        createdPipelineCaches = other.createdPipelineCaches;
        for (TrimObjectInfoMap::iterator obj = createdPipelineCaches.begin(); obj != createdPipelineCaches.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.PipelineCache.pCreatePacket);
        }

        createdPipelines = other.createdPipelines;
        for (TrimObjectInfoMap::iterator obj = createdPipelines.begin(); obj != createdPipelines.end(); obj++)
        {
            VkGraphicsPipelineCreateInfo* pCreateInfo = &obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo;

            // note: Using the same memory as both the destination and the source. We're copying what is currently there, which will properly result in new copies of any pointed-to objects and arrays.
            copy_VkGraphicsPipelineCreateInfo(pCreateInfo, obj->second.ObjectInfo.Pipeline.graphicsPipelineCreateInfo);
            copy_VkComputePipelineCreateInfo(const_cast<VkComputePipelineCreateInfo*>(&obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo), obj->second.ObjectInfo.Pipeline.computePipelineCreateInfo);
        }

        createdQueues = other.createdQueues;
        for (TrimObjectInfoMap::iterator obj = createdQueues.begin(); obj != createdQueues.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Queue.pCreatePacket);
        }

        createdSemaphores = other.createdSemaphores;
        for (TrimObjectInfoMap::iterator obj = createdSemaphores.begin(); obj != createdSemaphores.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Semaphore.pCreatePacket);
        }

        createdDeviceMemorys = other.createdDeviceMemorys;
        for (TrimObjectInfoMap::iterator obj = createdDeviceMemorys.begin(); obj != createdDeviceMemorys.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pCreatePacket);
            COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pMapMemoryPacket);
            COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pUnmapMemoryPacket);
            COPY_PACKET(obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket);
        }

        createdFences = other.createdFences;

        createdImages = other.createdImages;
        for (TrimObjectInfoMap::iterator obj = createdImages.begin(); obj != createdImages.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Image.pCreatePacket);
            COPY_PACKET(obj->second.ObjectInfo.Image.pMapMemoryPacket);
            COPY_PACKET(obj->second.ObjectInfo.Image.pUnmapMemoryPacket);
#if !TRIM_USE_ORDERED_IMAGE_CREATION
            COPY_PACKET(obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket);
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION
            COPY_PACKET(obj->second.ObjectInfo.Image.pBindImageMemoryPacket);
        }

        createdImageViews = other.createdImageViews;
        for (TrimObjectInfoMap::iterator obj = createdImageViews.begin(); obj != createdImageViews.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.ImageView.pCreatePacket);
        }

        createdBuffers = other.createdBuffers;
        for (TrimObjectInfoMap::iterator obj = createdBuffers.begin(); obj != createdBuffers.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Buffer.pCreatePacket);
            COPY_PACKET(obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket);
            COPY_PACKET(obj->second.ObjectInfo.Buffer.pMapMemoryPacket);
            COPY_PACKET(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket);
        }

        createdBufferViews = other.createdBufferViews;
        for (TrimObjectInfoMap::iterator obj = createdBufferViews.begin(); obj != createdBufferViews.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.BufferView.pCreatePacket);
        }

        createdFramebuffers = other.createdFramebuffers;
        for (TrimObjectInfoMap::iterator obj = createdFramebuffers.begin(); obj != createdFramebuffers.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Framebuffer.pCreatePacket);
        }

        createdEvents = other.createdEvents;
        for (TrimObjectInfoMap::iterator obj = createdEvents.begin(); obj != createdEvents.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Event.pCreatePacket);
        }

        createdQueryPools = other.createdQueryPools;
        for (TrimObjectInfoMap::iterator obj = createdQueryPools.begin(); obj != createdQueryPools.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.QueryPool.pCreatePacket);

            uint32_t queryCount = obj->second.ObjectInfo.QueryPool.size;
            if (queryCount > 0)
            {
                bool* tmp = new bool[queryCount];
                memcpy(tmp, obj->second.ObjectInfo.QueryPool.pResultsAvailable, queryCount*sizeof(bool));
                obj->second.ObjectInfo.QueryPool.pResultsAvailable = tmp;
            }
            else
            {
                obj->second.ObjectInfo.QueryPool.pResultsAvailable = nullptr;
            }
        }

        createdShaderModules = other.createdShaderModules;
        for (TrimObjectInfoMap::iterator obj = createdShaderModules.begin(); obj != createdShaderModules.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.ShaderModule.pCreatePacket);
        }

        createdPipelineLayouts = other.createdPipelineLayouts;
        for (TrimObjectInfoMap::iterator obj = createdPipelineLayouts.begin(); obj != createdPipelineLayouts.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.PipelineLayout.pCreatePacket);

            if (obj->second.ObjectInfo.PipelineLayout.pDescriptorSetLayouts != nullptr)
            {
                VkDescriptorSetLayout* pLayouts = new VkDescriptorSetLayout[obj->second.ObjectInfo.PipelineLayout.descriptorSetLayoutCount];
                memcpy(pLayouts, obj->second.ObjectInfo.PipelineLayout.pDescriptorSetLayouts, obj->second.ObjectInfo.PipelineLayout.descriptorSetLayoutCount * sizeof(VkDescriptorSetLayout));
                obj->second.ObjectInfo.PipelineLayout.pDescriptorSetLayouts = pLayouts;
            }
        }

        createdSamplers = other.createdSamplers;
        for (TrimObjectInfoMap::iterator obj = createdSamplers.begin(); obj != createdSamplers.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.Sampler.pCreatePacket);
        }

        createdDescriptorSetLayouts = other.createdDescriptorSetLayouts;
        for (TrimObjectInfoMap::iterator obj = createdDescriptorSetLayouts.begin(); obj != createdDescriptorSetLayouts.end(); obj++)
        {
            COPY_PACKET(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket);

            uint32_t bindingCount = obj->second.ObjectInfo.DescriptorSetLayout.bindingCount;
            if (bindingCount > 0)
            {
                VkDescriptorSetLayoutBinding* tmp = new VkDescriptorSetLayoutBinding[bindingCount];
                memcpy(tmp, obj->second.ObjectInfo.DescriptorSetLayout.pBindings, bindingCount*sizeof(VkDescriptorSetLayoutBinding));
                obj->second.ObjectInfo.DescriptorSetLayout.pBindings = tmp;
            }
            else
            {
                obj->second.ObjectInfo.DescriptorSetLayout.pBindings = nullptr;
            }
        }

        createdDescriptorSets = other.createdDescriptorSets;
        for (TrimObjectInfoMap::iterator obj = createdDescriptorSets.begin(); obj != createdDescriptorSets.end(); obj++)
        {
            uint32_t numBindings = obj->second.ObjectInfo.DescriptorSet.numBindings;
            if (numBindings > 0)
            {
                VkWriteDescriptorSet* tmp = new VkWriteDescriptorSet[numBindings];
                memcpy(tmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets, numBindings*sizeof(VkWriteDescriptorSet));
                obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets = tmp;

                for (uint32_t s = 0; s < obj->second.ObjectInfo.DescriptorSet.writeDescriptorCount; s++)
                {
                    uint32_t count = obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].descriptorCount;

                    if (obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo != nullptr)
                    {
                        VkDescriptorImageInfo* pTmp = new VkDescriptorImageInfo[count];
                        memcpy(pTmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo, count * sizeof(VkDescriptorImageInfo));
                        obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo = pTmp;
                    }
                    if (obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo != nullptr)
                    {
                        VkDescriptorBufferInfo* pTmp = new VkDescriptorBufferInfo[count];
                        memcpy(pTmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo, count * sizeof(VkDescriptorBufferInfo));
                        obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo = pTmp;
                    }
                    if (obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView != nullptr)
                    {
                        VkBufferView* pTmp = new VkBufferView[count];
                        memcpy(pTmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView, count * sizeof(VkBufferView));
                        obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView = pTmp;
                    }
                }
            }
            else
            {
                obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets = nullptr;
            }

            if (numBindings > 0)
            {
                VkCopyDescriptorSet* tmp = new VkCopyDescriptorSet[numBindings];
                memcpy(tmp, obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets, numBindings*sizeof(VkCopyDescriptorSet));
                obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets = tmp;
            }
            else
            {
                obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets = nullptr;
            }
        }

        return *this;
    }

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    void StateTracker::copy_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo* pDstStage, const VkPipelineShaderStageCreateInfo& srcStage)
    {
        *pDstStage = srcStage;

        if (srcStage.pName != nullptr)
        {
            char* pName = new char[strlen(srcStage.pName) + 1]();
            strcpy(pName, srcStage.pName);
            pDstStage->pName = pName;
        }

        if (srcStage.pSpecializationInfo != nullptr)
        {
            VkSpecializationInfo* pSI = new VkSpecializationInfo();
            *pSI = *(srcStage.pSpecializationInfo);

            if (srcStage.pSpecializationInfo->pData != nullptr && srcStage.pSpecializationInfo->dataSize > 0)
            {
                void* pData = malloc(srcStage.pSpecializationInfo->dataSize);
                memcpy(pData, srcStage.pSpecializationInfo->pData, srcStage.pSpecializationInfo->dataSize);
                pSI->pData = pData;
            }

            if (pSI->pMapEntries != nullptr)
            {
                VkSpecializationMapEntry* pSMEs = new VkSpecializationMapEntry[srcStage.pSpecializationInfo->mapEntryCount];
                memcpy(pSMEs, srcStage.pSpecializationInfo->pMapEntries, srcStage.pSpecializationInfo->mapEntryCount * sizeof(VkSpecializationMapEntry));
                pSI->pMapEntries = pSMEs;
            }

            pDstStage->pSpecializationInfo = pSI;
        }
    }

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    void StateTracker::delete_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo* pStage)
    {
        if (pStage->pName != nullptr)
        {
            delete[] pStage->pName;
        }

        if (pStage->pSpecializationInfo != nullptr)
        {
            if (pStage->pSpecializationInfo->pData != nullptr)
            {
                free(const_cast<void*>(pStage->pSpecializationInfo->pData));
            }
            if (pStage->pSpecializationInfo->pMapEntries != nullptr)
            {
                delete[] pStage->pSpecializationInfo->pMapEntries;
            }
            delete pStage->pSpecializationInfo;
        }
    }

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    void StateTracker::copy_VkGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo* pDst, const VkGraphicsPipelineCreateInfo& src)
    {
        *pDst = src;

        if (src.pStages != nullptr)
        {
            VkPipelineShaderStageCreateInfo* pStages = new VkPipelineShaderStageCreateInfo[src.stageCount]();
            for (uint32_t i = 0; i < src.stageCount; i++)
            {
                copy_VkPipelineShaderStageCreateInfo(&pStages[i], src.pStages[i]);
            }

            pDst->pStages = pStages;
        }

        if (src.pVertexInputState != nullptr)
        {
            VkPipelineVertexInputStateCreateInfo* pVIS = new VkPipelineVertexInputStateCreateInfo();
            *pVIS = *(src.pVertexInputState);
            pDst->pVertexInputState = pVIS;

            if (src.pVertexInputState->pVertexAttributeDescriptions != nullptr)
            {
                VkVertexInputAttributeDescription* pVIADs = new VkVertexInputAttributeDescription[pVIS->vertexAttributeDescriptionCount]();
                memcpy(pVIADs, pVIS->pVertexAttributeDescriptions, pVIS->vertexAttributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
                pVIS->pVertexAttributeDescriptions = pVIADs;
            }

            if (src.pVertexInputState->pVertexBindingDescriptions != nullptr)
            {
                VkVertexInputBindingDescription* pVIBDs = new VkVertexInputBindingDescription[pVIS->vertexBindingDescriptionCount]();
                memcpy(pVIBDs, pVIS->pVertexBindingDescriptions, pVIS->vertexBindingDescriptionCount * sizeof(VkVertexInputBindingDescription));
                pVIS->pVertexBindingDescriptions = pVIBDs;
            }
        }

        if (src.pInputAssemblyState != nullptr)
        {
            VkPipelineInputAssemblyStateCreateInfo* pIAS = new VkPipelineInputAssemblyStateCreateInfo();
            *pIAS = *(src.pInputAssemblyState);
            pDst->pInputAssemblyState = pIAS;
        }

        if (src.pTessellationState != nullptr)
        {
            VkPipelineTessellationStateCreateInfo* pTS = new VkPipelineTessellationStateCreateInfo();
            *pTS = *(src.pTessellationState);
            pDst->pTessellationState = pTS;
        }

        if (src.pViewportState != nullptr)
        {
            VkPipelineViewportStateCreateInfo* pVS = new VkPipelineViewportStateCreateInfo();
            *pVS = *(src.pViewportState);
            pDst->pViewportState = pVS;

            if (src.pViewportState->pViewports != nullptr)
            {
                VkViewport* pViewports = new VkViewport[pVS->viewportCount];
                memcpy(pViewports, pVS->pViewports, pVS->viewportCount * sizeof(VkViewport));
                pVS->pViewports = pViewports;
            }

            if (src.pViewportState->pScissors != nullptr)
            {
                VkRect2D* pScissors = new VkRect2D[pVS->scissorCount];
                memcpy(pScissors, pVS->pScissors, pVS->scissorCount * sizeof(VkRect2D));
                pVS->pScissors = pScissors;
            }
        }

        if (src.pRasterizationState != nullptr)
        {
            VkPipelineRasterizationStateCreateInfo* pRS = new VkPipelineRasterizationStateCreateInfo();
            *pRS = *(src.pRasterizationState);
            pDst->pRasterizationState = pRS;
        }

        if (src.pMultisampleState != nullptr)
        {
            VkPipelineMultisampleStateCreateInfo* pMS = new VkPipelineMultisampleStateCreateInfo();
            *pMS = *(src.pMultisampleState);
            pDst->pMultisampleState = pMS;

            if (src.pMultisampleState->pSampleMask != nullptr)
            {
                VkSampleMask* pMask = new VkSampleMask();
                *pMask = *(pMS->pSampleMask);
                pMS->pSampleMask = pMask;
            }
        }

        if (src.pDepthStencilState != nullptr)
        {
            VkPipelineDepthStencilStateCreateInfo* pDSS = new VkPipelineDepthStencilStateCreateInfo();
            *pDSS = *(src.pDepthStencilState);
            pDst->pDepthStencilState = pDSS;
        }

        if (src.pColorBlendState != nullptr)
        {
            VkPipelineColorBlendStateCreateInfo* pCBS = new VkPipelineColorBlendStateCreateInfo();
            *pCBS = *(src.pColorBlendState);
            pDst->pColorBlendState = pCBS;

            if (src.pColorBlendState->pAttachments != nullptr)
            {
                VkPipelineColorBlendAttachmentState* pAttachments = new VkPipelineColorBlendAttachmentState[pCBS->attachmentCount];
                memcpy(pAttachments, pCBS->pAttachments, pCBS->attachmentCount * sizeof(VkPipelineColorBlendAttachmentState));
                pCBS->pAttachments = pAttachments;
            }
        }

        if (src.pDynamicState != nullptr)
        {
            VkPipelineDynamicStateCreateInfo* pDS = new VkPipelineDynamicStateCreateInfo();
            *pDS = *(src.pDynamicState);
            pDst->pDynamicState = pDS;

            if (src.pDynamicState->pDynamicStates != nullptr)
            {
                VkDynamicState* pDynamicStates = new VkDynamicState[pDS->dynamicStateCount];
                memcpy(pDynamicStates, pDS->pDynamicStates, pDS->dynamicStateCount * sizeof(VkDynamicState));
                pDS->pDynamicStates = pDynamicStates;
            }
        }
    }

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    void StateTracker::copy_VkComputePipelineCreateInfo(VkComputePipelineCreateInfo* pDst, const VkComputePipelineCreateInfo& src)
    {
        *pDst = src;

        copy_VkPipelineShaderStageCreateInfo(&pDst->stage, src.stage);
    }

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    ObjectInfo* StateTracker::add_Instance(VkInstance var)
    {
        ObjectInfo& info = createdInstances[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_PhysicalDevice(VkPhysicalDevice var)
    {
        ObjectInfo& info = createdPhysicalDevices[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Device(VkDevice var)
    {
        ObjectInfo& info = createdDevices[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_SurfaceKHR(VkSurfaceKHR var)
    {
        ObjectInfo& info = createdSurfaceKHRs[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_CommandPool(VkCommandPool var)
    {
        ObjectInfo& info = createdCommandPools[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_CommandBuffer(VkCommandBuffer var)
    {
        ObjectInfo& info = createdCommandBuffers[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_DescriptorPool(VkDescriptorPool var)
    {
        ObjectInfo& info = createdDescriptorPools[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_RenderPass(VkRenderPass var)
    {
        ObjectInfo& info = createdRenderPasss[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_PipelineCache(VkPipelineCache var)
    {
        ObjectInfo& info = createdPipelineCaches[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Pipeline(VkPipeline var)
    {
        ObjectInfo& info = createdPipelines[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Queue(VkQueue var)
    {
        ObjectInfo& info = createdQueues[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Semaphore(VkSemaphore var)
    {
        ObjectInfo& info = createdSemaphores[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_DeviceMemory(VkDeviceMemory var)
    {
        ObjectInfo& info = createdDeviceMemorys[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Fence(VkFence var)
    {
        ObjectInfo& info = createdFences[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_SwapchainKHR(VkSwapchainKHR var)
    {
        ObjectInfo& info = createdSwapchainKHRs[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }
    
    ObjectInfo* StateTracker::add_Image(VkImage var)
    {
        ObjectInfo& info = createdImages[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }
    
    ObjectInfo* StateTracker::add_ImageView(VkImageView var)
    {
        ObjectInfo& info = createdImageViews[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }
    
    ObjectInfo* StateTracker::add_Buffer(VkBuffer var)
    {
        ObjectInfo& info = createdBuffers[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_BufferView(VkBufferView var)
    {
        ObjectInfo& info = createdBufferViews[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Framebuffer(VkFramebuffer var)
    {
        ObjectInfo& info = createdFramebuffers[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Event(VkEvent var)
    {
        ObjectInfo& info = createdEvents[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_QueryPool(VkQueryPool var)
    {
        ObjectInfo& info = createdQueryPools[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_ShaderModule(VkShaderModule var)
    {
        ObjectInfo& info = createdShaderModules[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_PipelineLayout(VkPipelineLayout var)
    {
        ObjectInfo& info = createdPipelineLayouts[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_Sampler(VkSampler var)
    {
        ObjectInfo& info = createdSamplers[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_DescriptorSetLayout(VkDescriptorSetLayout var)
    {
        ObjectInfo& info = createdDescriptorSetLayouts[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    ObjectInfo* StateTracker::add_DescriptorSet(VkDescriptorSet var)
    {
        ObjectInfo& info = createdDescriptorSets[var];
        memset(&info, 0, sizeof(ObjectInfo));
        info.vkObject = (uint64_t)var;
        return &info;
    }

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    ObjectInfo* StateTracker::get_Instance(VkInstance var)
    {
        TrimObjectInfoMap::iterator iter = createdInstances.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdInstances.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_PhysicalDevice(VkPhysicalDevice var)
    {
        TrimObjectInfoMap::iterator iter = createdPhysicalDevices.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdPhysicalDevices.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Device(VkDevice var)
    {
        TrimObjectInfoMap::iterator iter = createdDevices.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdDevices.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_SurfaceKHR(VkSurfaceKHR var)
    {
        TrimObjectInfoMap::iterator iter = createdSurfaceKHRs.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdSurfaceKHRs.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Queue(VkQueue var)
    {
        TrimObjectInfoMap::iterator iter = createdQueues.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdQueues.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_SwapchainKHR(VkSwapchainKHR var)
    {
        TrimObjectInfoMap::iterator iter = createdSwapchainKHRs.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdSwapchainKHRs.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_CommandPool(VkCommandPool var)
    {
        TrimObjectInfoMap::iterator iter = createdCommandPools.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdCommandPools.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_CommandBuffer(VkCommandBuffer var)
    {
        TrimObjectInfoMap::iterator iter = createdCommandBuffers.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdCommandBuffers.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_DeviceMemory(VkDeviceMemory var)
    {
        TrimObjectInfoMap::iterator iter = createdDeviceMemorys.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdDeviceMemorys.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_ImageView(VkImageView var)
    {
        TrimObjectInfoMap::iterator iter = createdImageViews.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdImageViews.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Image(VkImage var)
    {
        TrimObjectInfoMap::iterator iter = createdImages.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdImages.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_BufferView(VkBufferView var)
    {
        TrimObjectInfoMap::iterator iter = createdBufferViews.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdBufferViews.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Buffer(VkBuffer var)
    {
        TrimObjectInfoMap::iterator iter = createdBuffers.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdBuffers.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Sampler(VkSampler var)
    {
        TrimObjectInfoMap::iterator iter = createdSamplers.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdSamplers.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_DescriptorSetLayout(VkDescriptorSetLayout var)
    {
        TrimObjectInfoMap::iterator iter = createdDescriptorSetLayouts.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdDescriptorSetLayouts.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_PipelineLayout(VkPipelineLayout var)
    {
        TrimObjectInfoMap::iterator iter = createdPipelineLayouts.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdPipelineLayouts.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_RenderPass(VkRenderPass var)
    {
        TrimObjectInfoMap::iterator iter = createdRenderPasss.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdRenderPasss.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_ShaderModule(VkShaderModule var)
    {
        TrimObjectInfoMap::iterator iter = createdShaderModules.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdShaderModules.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_PipelineCache(VkPipelineCache var)
    {
        TrimObjectInfoMap::iterator iter = createdPipelineCaches.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdPipelineCaches.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_DescriptorPool(VkDescriptorPool var)
    {
        TrimObjectInfoMap::iterator iter = createdDescriptorPools.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdDescriptorPools.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Pipeline(VkPipeline var)
    {
        TrimObjectInfoMap::iterator iter = createdPipelines.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdPipelines.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Semaphore(VkSemaphore var)
    {
        TrimObjectInfoMap::iterator iter = createdSemaphores.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdSemaphores.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Fence(VkFence var)
    {
        TrimObjectInfoMap::iterator iter = createdFences.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdFences.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Framebuffer(VkFramebuffer var)
    {
        TrimObjectInfoMap::iterator iter = createdFramebuffers.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdFramebuffers.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_Event(VkEvent var)
    {
        TrimObjectInfoMap::iterator iter = createdEvents.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdEvents.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_QueryPool(VkQueryPool var)
    {
        TrimObjectInfoMap::iterator iter = createdQueryPools.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdQueryPools.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }

    ObjectInfo* StateTracker::get_DescriptorSet(VkDescriptorSet var)
    {
        TrimObjectInfoMap::iterator iter = createdDescriptorSets.find(var);
        ObjectInfo* pResult = NULL;
        if (iter != createdDescriptorSets.end())
        {
            pResult = &(iter->second);
        }
        return pResult;
    }


    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    void StateTracker::remove_Instance(VkInstance var)
    {
        ObjectInfo* pInfo = get_Instance(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Instance.pCreatePacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket);
        }
        createdInstances.erase(var);
    }

    void StateTracker::remove_PhysicalDevice(VkPhysicalDevice var)
    {
        ObjectInfo* pInfo = get_PhysicalDevice(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceSupportKHRPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket);
        }
        createdPhysicalDevices.erase(var);
    }

    void StateTracker::remove_Device(VkDevice var)
    {
        ObjectInfo* pInfo = get_Device(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Device.pCreatePacket);
        }
        createdDevices.erase(var);
    }

    void StateTracker::remove_SurfaceKHR(VkSurfaceKHR var)
    {
        ObjectInfo* pInfo = get_SurfaceKHR(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.SurfaceKHR.pCreatePacket);
        }
        createdSurfaceKHRs.erase(var);
    }

    void StateTracker::remove_Queue(VkQueue var)
    {
        ObjectInfo* pInfo = get_Queue(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Queue.pCreatePacket);
        }
        createdQueues.erase(var);
    }

    void StateTracker::remove_CommandPool(VkCommandPool var)
    {
        ObjectInfo* pInfo = get_CommandPool(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.CommandPool.pCreatePacket);
        }
        createdCommandPools.erase(var);
    }

    void StateTracker::remove_SwapchainKHR(VkSwapchainKHR var)
    {
        ObjectInfo* pInfo = get_SwapchainKHR(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.SwapchainKHR.pCreatePacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket);
        }
        createdSwapchainKHRs.erase(var);
    }

    void StateTracker::remove_CommandBuffer(VkCommandBuffer var)
    {
        createdCommandBuffers.erase(var);
    }

    void StateTracker::remove_DeviceMemory(VkDeviceMemory var)
    {
        ObjectInfo* pInfo = get_DeviceMemory(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pCreatePacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pMapMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pUnmapMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket);
        }
        createdDeviceMemorys.erase(var);
    }

    void StateTracker::remove_Image(VkImage var)
    {
        ObjectInfo* pInfo = get_Image(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pCreatePacket);
#if !TRIM_USE_ORDERED_IMAGE_CREATION
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pGetImageMemoryRequirementsPacket);
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pBindImageMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pMapMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pUnmapMemoryPacket);
        }
        createdImages.erase(var);
    }

    void StateTracker::remove_ImageView(VkImageView var)
    {
        ObjectInfo* pInfo = get_ImageView(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.ImageView.pCreatePacket);
        }
        createdImageViews.erase(var);
    }

    void StateTracker::remove_Buffer(VkBuffer var)
    {
        ObjectInfo* pInfo = get_Buffer(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pCreatePacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pBindBufferMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pMapMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Buffer.pUnmapMemoryPacket);
        }
        createdBuffers.erase(var);
    }

    void StateTracker::remove_BufferView(VkBufferView var)
    {
        ObjectInfo* pInfo = get_BufferView(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.BufferView.pCreatePacket);
        }
        createdBufferViews.erase(var);
    }

    void StateTracker::remove_Sampler(VkSampler var)
    {
        ObjectInfo* pInfo = get_Sampler(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Sampler.pCreatePacket);
        }
        createdSamplers.erase(var);
    }

    void StateTracker::remove_DescriptorSetLayout(VkDescriptorSetLayout var)
    {
        ObjectInfo* pInfo = get_DescriptorSetLayout(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DescriptorSetLayout.pCreatePacket);

            if (pInfo->ObjectInfo.DescriptorSetLayout.pBindings != nullptr)
            {
                delete[] pInfo->ObjectInfo.DescriptorSetLayout.pBindings;
                pInfo->ObjectInfo.DescriptorSetLayout.pBindings = nullptr;
                pInfo->ObjectInfo.DescriptorSetLayout.bindingCount = 0;
            }
        }
        createdDescriptorSetLayouts.erase(var);
    }

    void StateTracker::remove_PipelineLayout(VkPipelineLayout var)
    {
        ObjectInfo* pInfo = get_PipelineLayout(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PipelineLayout.pCreatePacket);
            if (pInfo->ObjectInfo.PipelineLayout.pDescriptorSetLayouts != nullptr)
            {
                delete[] pInfo->ObjectInfo.PipelineLayout.pDescriptorSetLayouts;
                pInfo->ObjectInfo.PipelineLayout.pDescriptorSetLayouts = nullptr;
                pInfo->ObjectInfo.PipelineLayout.descriptorSetLayoutCount = 0;
            }
        }
        createdPipelineLayouts.erase(var);
    }

    void StateTracker::remove_RenderPass(VkRenderPass var)
    {
        ObjectInfo* pInfo = get_RenderPass(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.RenderPass.pCreatePacket);
        }
        createdRenderPasss.erase(var);
    }

    void StateTracker::remove_ShaderModule(VkShaderModule var)
    {
        ObjectInfo* pInfo = get_ShaderModule(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.ShaderModule.pCreatePacket);
        }
        createdShaderModules.erase(var);
    }

    void StateTracker::remove_PipelineCache(VkPipelineCache var)
    {
        ObjectInfo* pInfo = get_PipelineCache(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PipelineCache.pCreatePacket);
        }
        createdPipelineCaches.erase(var);
    }

    void StateTracker::remove_Pipeline(VkPipeline var)
    {
        ObjectInfo* pInfo = get_Pipeline(var);
        if (pInfo != nullptr)
        {
            delete_VkPipelineShaderStageCreateInfo(&pInfo->ObjectInfo.Pipeline.computePipelineCreateInfo.stage);

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages != nullptr)
            {
                for (uint32_t i = 0; i < pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.stageCount; ++i)
                {
                    delete_VkPipelineShaderStageCreateInfo(const_cast<VkPipelineShaderStageCreateInfo*>(&pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages[i]));
                }

                delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pStages;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState != nullptr)
            {
                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexAttributeDescriptions != nullptr)
                {
                    delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexAttributeDescriptions;
                }
                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexBindingDescriptions != nullptr)
                {
                    delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState->pVertexBindingDescriptions;
                }

                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pVertexInputState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pInputAssemblyState != nullptr)
            {
                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pInputAssemblyState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pTessellationState != nullptr)
            {
                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pTessellationState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState != nullptr)
            {
                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pViewports != nullptr)
                {
                    delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pViewports;
                }

                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pScissors != nullptr)
                {
                    delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState->pScissors;
                }

                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pViewportState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pRasterizationState != nullptr)
            {
                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pRasterizationState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState != nullptr)
            {
                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState->pSampleMask != nullptr)
                {
                    delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState->pSampleMask;
                }
                
                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pMultisampleState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDepthStencilState != nullptr)
            {
                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDepthStencilState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState != nullptr)
            {
                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState->pAttachments != nullptr)
                {
                    delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState->pAttachments;
                }

                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pColorBlendState;
            }

            if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState != nullptr)
            {
                if (pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState->pDynamicStates != nullptr)
                {
                    delete[] pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState->pDynamicStates;
                }

                delete pInfo->ObjectInfo.Pipeline.graphicsPipelineCreateInfo.pDynamicState;
            }
        }
        createdPipelines.erase(var);
    }

    void StateTracker::remove_DescriptorPool(VkDescriptorPool var)
    {
        ObjectInfo* pInfo = get_DescriptorPool(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DescriptorPool.pCreatePacket);
        }
        createdDescriptorPools.erase(var);
    }

    void StateTracker::remove_DescriptorSet(VkDescriptorSet var)
    {
        ObjectInfo* pInfo = get_DescriptorSet(var);
        if (pInfo != nullptr)
        {
            if (pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets != nullptr)
            {
                delete[] pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets;
                pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets = nullptr;
            }
            if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets != nullptr)
            {
                for (uint32_t s = 0; s < pInfo->ObjectInfo.DescriptorSet.numBindings; s++)
                {
                    if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo != nullptr)
                    {
                        delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo;
                        pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo = nullptr;
                    }
                    if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo != nullptr)
                    {
                        delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo;
                        pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo = nullptr;
                    }
                    if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView != nullptr)
                    {
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

    void StateTracker::remove_Framebuffer(VkFramebuffer var)
    {
        ObjectInfo* pInfo = get_Framebuffer(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Framebuffer.pCreatePacket);
        }
        createdFramebuffers.erase(var);
    }

    void StateTracker::remove_Semaphore(VkSemaphore var)
    {
        ObjectInfo* pInfo = get_Semaphore(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Semaphore.pCreatePacket);
        }
        createdSemaphores.erase(var);
    }

    void StateTracker::remove_Fence(VkFence var)
    {
        createdFences.erase(var);
    }

    void StateTracker::remove_Event(VkEvent var)
    {
        ObjectInfo* pInfo = get_Event(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Event.pCreatePacket);
        }
        createdEvents.erase(var);
    }

    void StateTracker::remove_QueryPool(VkQueryPool var)
    {
        ObjectInfo* pInfo = get_QueryPool(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.QueryPool.pCreatePacket);
            pInfo->ObjectInfo.QueryPool.size = 0;
            if (pInfo->ObjectInfo.QueryPool.pResultsAvailable != nullptr)
            {
                delete[] pInfo->ObjectInfo.QueryPool.pResultsAvailable;
                pInfo->ObjectInfo.QueryPool.pResultsAvailable = nullptr;
            }
        }
        createdQueryPools.erase(var);
    }
}
