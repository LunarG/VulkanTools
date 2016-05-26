/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#include "vktrace_lib_trim.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_vk_packets.h"
#include "vktrace_vk_packet_id.h"
#include "vk_struct_size_helper.h"
#include "vulkan.h"


// declared extern in vktrace_lib_trim.h
Trim_StateTracker g_trimGlobalStateTracker;

// List of all the packets that have been recorded for the frames of interest.
static std::list<vktrace_trace_packet_header*> trim_recorded_packets;

#define TRIM_DEFINE_OBJECT_TRACKER_FUNCS(type) \
Trim_ObjectInfo* trim_add_##type##_object(Vk##type var) { \
   Trim_ObjectInfo& info = g_trimGlobalStateTracker.created##type##s[var]; \
   memset(&info, 0, sizeof(Trim_ObjectInfo)); \
   info.vkObject = (uint64_t)var; \
   return &info; \
} \
void trim_remove_##type##_object(Vk##type var) { \
    /* make sure the object actually existed before we attempt to remove it. This is for testing and thus only happens in debug builds. */ \
    assert(g_trimGlobalStateTracker.created##type##s.find(var) != g_trimGlobalStateTracker.created##type##s.end()); \
    g_trimGlobalStateTracker.created##type##s.erase(var); \
} \
Trim_ObjectInfo* trim_get_##type##_objectInfo(Vk##type var) { \
   TrimObjectInfoMap::iterator iter  = g_trimGlobalStateTracker.created##type##s.find(var); \
   return &(iter->second); \
}

#define TRIM_DEFINE_MARK_REF(type) \
void trim_mark_##type##_reference(Vk##type var) { \
   TrimObjectInfoMap::iterator iter  = g_trimGlobalStateTracker.created##type##s.find(var); \
   if (iter != g_trimGlobalStateTracker.created##type##s.end()) \
   { \
       iter->second.bReferencedInTrim = true; \
   } \
}


TRIM_DEFINE_MARK_REF(Instance);
TRIM_DEFINE_MARK_REF(PhysicalDevice);
TRIM_DEFINE_MARK_REF(Device);

TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Instance);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(PhysicalDevice);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Device);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(SurfaceKHR);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Queue);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(SwapchainKHR);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(CommandPool);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(CommandBuffer);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DeviceMemory);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(ImageView);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Image);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(BufferView);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Buffer);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Sampler);

TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DescriptorPool);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(RenderPass);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Pipeline);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Semaphore);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Fence);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Framebuffer);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Event);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(QueryPool);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(ShaderModule);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(PipelineLayout);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DescriptorSetLayout);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DescriptorSet);

//=============================================================================
// Recreate all objects
//=============================================================================
void trim_write_all_referenced_object_calls()
{
    // Instances (& PhysicalDevices)
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdInstances.begin(); obj != g_trimGlobalStateTracker.createdInstances.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Instance.pCreatePacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket));
    }

    // SurfaceKHR
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSurfaceKHRs.begin(); obj != g_trimGlobalStateTracker.createdSurfaceKHRs.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket));
    }

    // Devices
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdDevices.begin(); obj != g_trimGlobalStateTracker.createdDevices.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Device.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Device.pCreatePacket));
    }

    // Queue
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdQueues.begin(); obj != g_trimGlobalStateTracker.createdQueues.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Queue.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Queue.pCreatePacket));
    }

    // CommandPool
    for (TrimObjectInfoMap::iterator poolObj = g_trimGlobalStateTracker.createdCommandPools.begin(); poolObj != g_trimGlobalStateTracker.createdCommandPools.end(); poolObj++)
    {
        vktrace_write_trace_packet(poolObj->second.ObjectInfo.CommandPool.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(poolObj->second.ObjectInfo.CommandPool.pCreatePacket));

        // Now allocate command buffers that were allocated on this pool
        for (uint32_t level = VK_COMMAND_BUFFER_LEVEL_BEGIN_RANGE; level < VK_COMMAND_BUFFER_LEVEL_END_RANGE; level++)
        {
            VkCommandBufferAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.pNext = NULL;
            allocateInfo.commandPool = (VkCommandPool)poolObj->first;
            allocateInfo.level = (VkCommandBufferLevel)level;
            allocateInfo.commandBufferCount = poolObj->second.ObjectInfo.CommandPool.numCommandBuffersAllocated[level];
            if (allocateInfo.commandBufferCount > 0)
            {
                VkCommandBuffer* pCommandBuffers = new VkCommandBuffer[allocateInfo.commandBufferCount];
                uint32_t index = 0;
                for (TrimObjectInfoMap::iterator cbIter = g_trimGlobalStateTracker.createdCommandBuffers.begin(); cbIter != g_trimGlobalStateTracker.createdCommandBuffers.end(); cbIter++)
                {
                    if (cbIter->second.ObjectInfo.CommandBuffer.commandPool == (VkCommandPool)poolObj->first &&
                        cbIter->second.ObjectInfo.CommandBuffer.level == level)
                    {
                        pCommandBuffers[index] = (VkCommandBuffer)cbIter->first;
                        index++;
                    }
                }

                vktrace_trace_packet_header* pHeader;
                packet_vkAllocateCommandBuffers* pPacket = NULL;
                CREATE_TRACE_PACKET(vkAllocateCommandBuffers, get_struct_chain_size(&allocateInfo) + sizeof(VkCommandBuffer) * allocateInfo.commandBufferCount);
                vktrace_set_packet_entrypoint_end_time(pHeader);
                pPacket = interpret_body_as_vkAllocateCommandBuffers(pHeader);
                pPacket->device = poolObj->second.belongsToDevice;
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkCommandBufferAllocateInfo), &allocateInfo);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCommandBuffers), sizeof(VkCommandBuffer) * allocateInfo.commandBufferCount, pCommandBuffers);
                pPacket->result = VK_SUCCESS;
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCommandBuffers));
                FINISH_TRACE_PACKET();

                delete[] pCommandBuffers;
            }
        }
    }

    // SwapchainKHR
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSwapchainKHRs.begin(); obj != g_trimGlobalStateTracker.createdSwapchainKHRs.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket));
    }

    // DeviceMemory
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdDeviceMemorys.begin(); obj != g_trimGlobalStateTracker.createdDeviceMemorys.end(); obj++)
    {
        // AllocateMemory
        vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DeviceMemory.pCreatePacket));

        // will need to map / unmap and set the memory contents
    }

    // ImageView
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdImageViews.begin(); obj != g_trimGlobalStateTracker.createdImageViews.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.ImageView.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.ImageView.pCreatePacket));
    }

    // Image
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdImages.begin(); obj != g_trimGlobalStateTracker.createdImages.end(); obj++)
    {
        // CreateImage
        if (obj->second.ObjectInfo.Image.pCreatePacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pCreatePacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pCreatePacket));
        }

        // BindImageMemory
        if (obj->second.ObjectInfo.Image.pBindImageMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pBindImageMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pBindImageMemoryPacket));
        }
    }

    // BufferView
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdBufferViews.begin(); obj != g_trimGlobalStateTracker.createdBufferViews.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.BufferView.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.BufferView.pCreatePacket));
    }

    // Buffer
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdBuffers.begin(); obj != g_trimGlobalStateTracker.createdBuffers.end(); obj++)
    {
        // CreateBuffer
        if (obj->second.ObjectInfo.Buffer.pCreatePacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pCreatePacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Buffer.pCreatePacket));
        }

        // BindBufferMemory
        if (obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Buffer.pBindBufferMemoryPacket));
        }
    }

    // Sampler
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSamplers.begin(); obj != g_trimGlobalStateTracker.createdSamplers.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Sampler.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Sampler.pCreatePacket));
    }

    // DescriptorSetLayout
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdDescriptorSetLayouts.begin(); obj != g_trimGlobalStateTracker.createdDescriptorSetLayouts.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket));
    }
}

#define TRIM_ADD_OBJECT_CALL(type) \
void trim_add_##type##_call(Vk##type var, vktrace_trace_packet_header* pHeader) { \
    /* if it's in the created list, add it there*/ \
    TrimObjectInfoMap::iterator iter = g_trimGlobalStateTracker.created##type##s.find(var); \
}

#define TRIM_MARK_OBJECT_REFERENCE(type) \
void trim_mark_##type##_reference(Vk##type var) { \
    Trim_ObjectInfo* info = &g_trimGlobalStateTracker.created##type##s[var]; \
    info->bReferencedInTrim = true; \
}

#define TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(type) \
void trim_mark_##type##_reference(Vk##type var) { \
    Trim_ObjectInfo* info = &g_trimGlobalStateTracker.created##type##s[var]; \
    info->bReferencedInTrim = true; \
    trim_mark_Device_reference((VkDevice)info->belongsToDevice); \
}

//===============================================
// Object tracking
//===============================================
TRIM_ADD_OBJECT_CALL(PhysicalDevice)
TRIM_ADD_OBJECT_CALL(Device)

TRIM_ADD_OBJECT_CALL(CommandPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(CommandPool)

TRIM_ADD_OBJECT_CALL(CommandBuffer)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(CommandBuffer)

TRIM_ADD_OBJECT_CALL(DescriptorPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorPool)

TRIM_ADD_OBJECT_CALL(DescriptorSet)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorSet)

TRIM_ADD_OBJECT_CALL(RenderPass)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(RenderPass)

TRIM_ADD_OBJECT_CALL(Pipeline)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Pipeline)

TRIM_ADD_OBJECT_CALL(Queue)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Queue);

TRIM_ADD_OBJECT_CALL(Semaphore)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Semaphore)

TRIM_ADD_OBJECT_CALL(DeviceMemory)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DeviceMemory)

TRIM_ADD_OBJECT_CALL(Fence)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Fence)

TRIM_ADD_OBJECT_CALL(SwapchainKHR)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(SwapchainKHR)

TRIM_ADD_OBJECT_CALL(Image)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Image)

TRIM_ADD_OBJECT_CALL(ImageView)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(ImageView)

TRIM_ADD_OBJECT_CALL(Buffer)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Buffer)

TRIM_ADD_OBJECT_CALL(BufferView)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(BufferView)

TRIM_ADD_OBJECT_CALL(Framebuffer)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Framebuffer)

TRIM_ADD_OBJECT_CALL(Event)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Event)

TRIM_ADD_OBJECT_CALL(QueryPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(QueryPool)

TRIM_ADD_OBJECT_CALL(ShaderModule)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(ShaderModule)

TRIM_ADD_OBJECT_CALL(PipelineLayout)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(PipelineLayout)

TRIM_ADD_OBJECT_CALL(Sampler)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Sampler)

TRIM_ADD_OBJECT_CALL(DescriptorSetLayout)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorSetLayout)

//===============================================
// Packet Recording for frames of interest
//===============================================
void trim_add_recorded_packet(vktrace_trace_packet_header* pHeader)
{
    trim_recorded_packets.push_back(pHeader);
}

void trim_write_recorded_packets()
{
    for (std::list<vktrace_trace_packet_header*>::iterator call = trim_recorded_packets.begin(); call != trim_recorded_packets.end(); call++)
    {
        vktrace_write_trace_packet(*call, vktrace_trace_get_trace_file());
    }
}


//===============================================
// Write packets to destroy all created created objects.
// Remember that we want to destroy them roughly in the opposite 
// order they were created, so that means the Instance is the last
// object to destroy!
//===============================================
void trim_write_destroy_packets()
{
    // DescriptorSetLayout
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSamplers.begin(); obj != g_trimGlobalStateTracker.createdSamplers.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyDescriptorSetLayout* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyDescriptorSetLayout, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyDescriptorSetLayout(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->descriptorSetLayout = (VkDescriptorSetLayout)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.DescriptorSetLayout.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // Sampler
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSamplers.begin(); obj != g_trimGlobalStateTracker.createdSamplers.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySampler* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySampler, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySampler(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->sampler = (VkSampler)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.Sampler.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // Buffer
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdBuffers.begin(); obj != g_trimGlobalStateTracker.createdBuffers.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyBuffer* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyBuffer, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyBuffer(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->buffer = (VkBuffer)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.Buffer.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // BufferView
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdBufferViews.begin(); obj != g_trimGlobalStateTracker.createdBufferViews.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyBufferView* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyBufferView, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyBufferView(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->bufferView = (VkBufferView)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.BufferView.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // Image
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdImages.begin(); obj != g_trimGlobalStateTracker.createdImages.end(); obj++)
    {
        if (obj->second.ObjectInfo.Image.bIsSwapchainImage == false)
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkDestroyImage* pPacket = NULL;
            CREATE_TRACE_PACKET(vkDestroyImage, sizeof(VkAllocationCallbacks));
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkDestroyImage(pHeader);
            pPacket->device = obj->second.belongsToDevice;
            pPacket->image = (VkImage)obj->first;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.Image.allocator));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            FINISH_TRACE_PACKET();
        }
    }

    // ImageView
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdImageViews.begin(); obj != g_trimGlobalStateTracker.createdImageViews.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyImageView* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyImageView, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyImageView(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->imageView = (VkImageView)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.ImageView.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // DeviceMemory
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdDeviceMemorys.begin(); obj != g_trimGlobalStateTracker.createdDeviceMemorys.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkFreeMemory* pPacket = NULL;
        CREATE_TRACE_PACKET(vkFreeMemory, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkFreeMemory(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->memory = (VkDeviceMemory)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.DeviceMemory.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // SwapchainKHR
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSwapchainKHRs.begin(); obj != g_trimGlobalStateTracker.createdSwapchainKHRs.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySwapchainKHR* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySwapchainKHR, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySwapchainKHR(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->swapchain = (VkSwapchainKHR)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.SwapchainKHR.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // CommandPool
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdCommandPools.begin(); obj != g_trimGlobalStateTracker.createdCommandPools.end(); obj++)
    {
        // free the command buffers
        for (uint32_t level = VK_COMMAND_BUFFER_LEVEL_BEGIN_RANGE; level < VK_COMMAND_BUFFER_LEVEL_END_RANGE; level++)
        {
            uint32_t commandBufferCount = obj->second.ObjectInfo.CommandPool.numCommandBuffersAllocated[level];
            if (commandBufferCount > 0)
            {
                vktrace_trace_packet_header* pHeader;
                packet_vkFreeCommandBuffers* pPacket = NULL;
                CREATE_TRACE_PACKET(vkFreeCommandBuffers, commandBufferCount*sizeof(VkCommandBuffer));
                vktrace_set_packet_entrypoint_end_time(pHeader);
                pPacket = interpret_body_as_vkFreeCommandBuffers(pHeader);
                pPacket->device = obj->second.belongsToDevice;
                pPacket->commandPool = (VkCommandPool)obj->first;
                pPacket->commandBufferCount = commandBufferCount;

                VkCommandBuffer* pCommandBuffers = new VkCommandBuffer[commandBufferCount];
                uint32_t index = 0;
                for (TrimObjectInfoMap::iterator cbIter = g_trimGlobalStateTracker.createdCommandBuffers.begin(); cbIter != g_trimGlobalStateTracker.createdCommandBuffers.end(); cbIter++)
                {
                    if (cbIter->second.ObjectInfo.CommandBuffer.commandPool == (VkCommandPool)obj->first &&
                        cbIter->second.ObjectInfo.CommandBuffer.level == level)
                    {
                        pCommandBuffers[index] = (VkCommandBuffer)cbIter->first;
                        index++;
                    }
                }

                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCommandBuffers), commandBufferCount*sizeof(VkCommandBuffer), pCommandBuffers);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCommandBuffers));
                FINISH_TRACE_PACKET();

                delete[] pCommandBuffers;
            }
        }

        // Destroy the commandPool
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyCommandPool* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyCommandPool, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyCommandPool(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->commandPool = (VkCommandPool) obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.CommandPool.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // Devices
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdDevices.begin(); obj != g_trimGlobalStateTracker.createdDevices.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyDevice* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyDevice, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyDevice(pHeader);
        pPacket->device = (VkDevice)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.Device.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // SurfaceKHR
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdSurfaceKHRs.begin(); obj != g_trimGlobalStateTracker.createdSurfaceKHRs.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySurfaceKHR* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySurfaceKHR, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySurfaceKHR(pHeader);
        pPacket->surface = (VkSurfaceKHR)obj->first;
        pPacket->instance = obj->second.belongsToInstance;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.SurfaceKHR.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }

    // Instances
    for (TrimObjectInfoMap::iterator obj = g_trimGlobalStateTracker.createdInstances.begin(); obj != g_trimGlobalStateTracker.createdInstances.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyInstance* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyInstance, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyInstance(pHeader);
        pPacket->instance = (VkInstance)obj->first;
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &(obj->second.ObjectInfo.Instance.allocator));
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        FINISH_TRACE_PACKET();
    }
}

//===============================================
// Delete all the created packets
//===============================================
#define TRIM_DELETE_ALL_PACKETS(type) \
    g_trimGlobalStateTracker.created##type##s.clear();

void trim_delete_all_packets()
{
    // delete all recorded packets
    for (std::list<vktrace_trace_packet_header*>::iterator call = trim_recorded_packets.begin(); call != trim_recorded_packets.end(); call++)
    {
        vktrace_delete_trace_packet(&(*call));
    }
    trim_recorded_packets.clear();

    TRIM_DELETE_ALL_PACKETS(Instance);
    TRIM_DELETE_ALL_PACKETS(PhysicalDevice);
    TRIM_DELETE_ALL_PACKETS(Device);
    TRIM_DELETE_ALL_PACKETS(CommandPool);
    TRIM_DELETE_ALL_PACKETS(SwapchainKHR);
    TRIM_DELETE_ALL_PACKETS(Queue);
    TRIM_DELETE_ALL_PACKETS(DeviceMemory);
    TRIM_DELETE_ALL_PACKETS(Image);
    TRIM_DELETE_ALL_PACKETS(ImageView);
    TRIM_DELETE_ALL_PACKETS(Buffer);
    TRIM_DELETE_ALL_PACKETS(BufferView);
    TRIM_DELETE_ALL_PACKETS(ShaderModule);
    TRIM_DELETE_ALL_PACKETS(Sampler);
    TRIM_DELETE_ALL_PACKETS(RenderPass);
    TRIM_DELETE_ALL_PACKETS(Framebuffer);
    TRIM_DELETE_ALL_PACKETS(DescriptorSetLayout);
    TRIM_DELETE_ALL_PACKETS(DescriptorPool);
    TRIM_DELETE_ALL_PACKETS(DescriptorSet);
    TRIM_DELETE_ALL_PACKETS(PipelineLayout);
    TRIM_DELETE_ALL_PACKETS(Pipeline);
    TRIM_DELETE_ALL_PACKETS(Semaphore);
    TRIM_DELETE_ALL_PACKETS(Fence);
    TRIM_DELETE_ALL_PACKETS(Event);
    TRIM_DELETE_ALL_PACKETS(QueryPool);
    TRIM_DELETE_ALL_PACKETS(CommandBuffer);
}