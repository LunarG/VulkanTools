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
    StateTracker::StateTracker() {}
    StateTracker::StateTracker(const StateTracker& other)
    {
        *this = other;
    }

    StateTracker::~StateTracker() {}

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
    }

    //-------------------------------------------------------------------------

    StateTracker& StateTracker::operator= (const StateTracker& other)
    {
        if (this == &other)
            return *this;

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
            COPY_PACKET(obj->second.ObjectInfo.Pipeline.pCreatePacket);
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
            COPY_PACKET(obj->second.ObjectInfo.Image.pBindImageMemoryPacket);
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION
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
            uint32_t writeDescriptorCount = obj->second.ObjectInfo.DescriptorSet.writeDescriptorCount;
            if (writeDescriptorCount > 0)
            {
                VkWriteDescriptorSet* tmp = new VkWriteDescriptorSet[writeDescriptorCount];
                memcpy(tmp, obj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets, writeDescriptorCount*sizeof(VkWriteDescriptorSet));
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

            uint32_t copyDescriptorCount = obj->second.ObjectInfo.DescriptorSet.copyDescriptorCount;
            if (copyDescriptorCount > 0)
            {
                VkCopyDescriptorSet* tmp = new VkCopyDescriptorSet[copyDescriptorCount];
                memcpy(tmp, obj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets, copyDescriptorCount*sizeof(VkCopyDescriptorSet));
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
    void StateTracker::remove_Instance(VkInstance var)
    {
        ObjectInfo* pInfo = get_Instance_objectInfo(var);
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
        ObjectInfo* pInfo = get_PhysicalDevice_objectInfo(var);
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
        ObjectInfo* pInfo = get_Device_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Device.pCreatePacket);
        }
        createdDevices.erase(var);
    }

    void StateTracker::remove_SurfaceKHR(VkSurfaceKHR var)
    {
        ObjectInfo* pInfo = get_SurfaceKHR_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.SurfaceKHR.pCreatePacket);
        }
        createdSurfaceKHRs.erase(var);
    }

    void StateTracker::remove_Queue(VkQueue var)
    {
        ObjectInfo* pInfo = get_Queue_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Queue.pCreatePacket);
        }
        createdQueues.erase(var);
    }

    void StateTracker::remove_CommandPool(VkCommandPool var)
    {
        ObjectInfo* pInfo = get_CommandPool_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.CommandPool.pCreatePacket);
        }
        createdCommandPools.erase(var);
    }

    void StateTracker::remove_SwapchainKHR(VkSwapchainKHR var)
    {
        ObjectInfo* pInfo = get_SwapchainKHR_objectInfo(var);
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
        ObjectInfo* pInfo = get_DeviceMemory_objectInfo(var);
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
        ObjectInfo* pInfo = get_Image_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pCreatePacket);
#if !TRIM_USE_ORDERED_IMAGE_CREATION
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pGetImageMemoryRequirementsPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pBindImageMemoryPacket);
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pMapMemoryPacket);
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Image.pUnmapMemoryPacket);
        }
        createdImages.erase(var);
    }

    void StateTracker::remove_ImageView(VkImageView var)
    {
        ObjectInfo* pInfo = get_ImageView_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.ImageView.pCreatePacket);
        }
        createdImageViews.erase(var);
    }

    void StateTracker::remove_Buffer(VkBuffer var)
    {
        ObjectInfo* pInfo = get_Buffer_objectInfo(var);
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
        ObjectInfo* pInfo = get_BufferView_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.BufferView.pCreatePacket);
        }
        createdBufferViews.erase(var);
    }

    void StateTracker::remove_Sampler(VkSampler var)
    {
        ObjectInfo* pInfo = get_Sampler_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Sampler.pCreatePacket);
        }
        createdSamplers.erase(var);
    }

    void StateTracker::remove_DescriptorSetLayout(VkDescriptorSetLayout var)
    {
        ObjectInfo* pInfo = get_DescriptorSetLayout_objectInfo(var);
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
        ObjectInfo* pInfo = get_PipelineLayout_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PipelineLayout.pCreatePacket);
        }
        createdPipelineLayouts.erase(var);
    }

    void StateTracker::remove_RenderPass(VkRenderPass var)
    {
        ObjectInfo* pInfo = get_RenderPass_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.RenderPass.pCreatePacket);
        }
        createdRenderPasss.erase(var);
    }

    void StateTracker::remove_ShaderModule(VkShaderModule var)
    {
        ObjectInfo* pInfo = get_ShaderModule_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.ShaderModule.pCreatePacket);
        }
        createdShaderModules.erase(var);
    }

    void StateTracker::remove_PipelineCache(VkPipelineCache var)
    {
        ObjectInfo* pInfo = get_PipelineCache_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.PipelineCache.pCreatePacket);
        }
        createdPipelineCaches.erase(var);
    }

    void StateTracker::remove_Pipeline(VkPipeline var)
    {
        ObjectInfo* pInfo = get_Pipeline_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Pipeline.pCreatePacket);
        }
        createdPipelines.erase(var);
    }

    void StateTracker::remove_DescriptorPool(VkDescriptorPool var)
    {
        ObjectInfo* pInfo = get_DescriptorPool_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.DescriptorPool.pCreatePacket);
        }
        createdDescriptorPools.erase(var);
    }

    void StateTracker::remove_DescriptorSet(VkDescriptorSet var)
    {
        ObjectInfo* pInfo = get_DescriptorSet_objectInfo(var);
        if (pInfo != nullptr)
        {
            if (pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets != nullptr)
            {
                delete[] pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets;
                pInfo->ObjectInfo.DescriptorSet.pCopyDescriptorSets = nullptr;
            }
            if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets != nullptr)
            {
                for (uint32_t s = 0; s < pInfo->ObjectInfo.DescriptorSet.writeDescriptorCount; s++)
                {
                    if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo != nullptr)
                    {
                        delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pImageInfo;
                    }
                    if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo != nullptr)
                    {
                        delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pBufferInfo;
                    }
                    if (pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView != nullptr)
                    {
                        delete[] pInfo->ObjectInfo.DescriptorSet.pWriteDescriptorSets[s].pTexelBufferView;
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
        ObjectInfo* pInfo = get_Framebuffer_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Framebuffer.pCreatePacket);
        }
        createdFramebuffers.erase(var);
    }

    void StateTracker::remove_Semaphore(VkSemaphore var)
    {
        ObjectInfo* pInfo = get_Semaphore_objectInfo(var);
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
        ObjectInfo* pInfo = get_Event_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.Event.pCreatePacket);
        }
        createdEvents.erase(var);
    }

    void StateTracker::remove_QueryPool(VkQueryPool var)
    {
        ObjectInfo* pInfo = get_QueryPool_objectInfo(var);
        if (pInfo != nullptr)
        {
            vktrace_delete_trace_packet(&pInfo->ObjectInfo.QueryPool.pCreatePacket);
        }
        createdQueryPools.erase(var);
    }
}
