/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#include "vktrace_lib_trim.h"
#include "vktrace_lib_helpers.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_vk_packets.h"
#include "vktrace_vk_packet_id.h"
#include "vk_struct_size_helper.h"
#include "vulkan.h"

// Tracks the existence of objects from the very beginning of the application
static Trim_StateTracker s_trimGlobalStateTracker;

// A snapshot of the GlobalStateTracker taken at the start of the trim frames.
static Trim_StateTracker s_trimStateTrackerSnapshot;

bool g_trimEnabled = false;
bool g_trimIsPreTrim = false;
bool g_trimIsInTrim = false;
bool g_trimIsPostTrim = false;
uint64_t g_trimFrameCounter = 0;
uint64_t g_trimStartFrame = 0;
uint64_t g_trimEndFrame = UINT64_MAX;
VKTRACE_CRITICAL_SECTION trimRecordedPacketLock;
VKTRACE_CRITICAL_SECTION trimStateTrackerLock;

// implemented in vktrace_lib_trace.cpp
extern layer_device_data* mdd(void* object);

// Typically an application will have one VkAllocationCallbacks struct and 
// will pass in that same address as needed, so we'll keep a map to correlate
// the supplied address to the AllocationCallbacks object.
static std::unordered_map<const void*, VkAllocationCallbacks> s_trimAllocatorMap;

void trim_initialize()
{
    vktrace_create_critical_section(&trimStateTrackerLock);
    vktrace_create_critical_section(&trimRecordedPacketLock);
}

void trim_add_Allocator(const VkAllocationCallbacks* pAllocator)
{
    if (pAllocator != NULL)
    {
        if (s_trimAllocatorMap.find(pAllocator) == s_trimAllocatorMap.end())
        {
            // need to add this allocator address
            s_trimAllocatorMap[pAllocator] = *pAllocator;
        }
    }
}

VkAllocationCallbacks* trim_get_Allocator(const VkAllocationCallbacks* pAllocator)
{
    if (pAllocator == NULL)
    {
        return NULL;
    }
    const void* pAddress = pAllocator;
    std::unordered_map<const void*, VkAllocationCallbacks>::iterator iter = s_trimAllocatorMap.find(pAddress);
    assert(iter != s_trimAllocatorMap.end());
    VkAllocationCallbacks* pStoredAllocator = &(iter->second);
    return pStoredAllocator;
}

//=============================================================================
// Use this to snapshot the global state tracker at the start of the trim frames.
//=============================================================================
void trim_snapshot_state_tracker()
{
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimStateTrackerSnapshot = s_trimGlobalStateTracker;

    // copy all buffer contents and update the snapshot with the memory contents
    for (TrimObjectInfoMap::iterator iter = s_trimStateTrackerSnapshot.createdDeviceMemorys.begin(); iter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end(); iter++)
    {
        if (iter->second.ObjectInfo.DeviceMemory.bIsEverMapped == false)
        {
            continue;
        }
        VkResult result = VK_SUCCESS;
        VkDevice device = iter->second.belongsToDevice;
        VkDeviceMemory memory = (VkDeviceMemory)iter->first;
        VkDeviceSize offset = 0;
        VkDeviceSize size = iter->second.ObjectInfo.DeviceMemory.size;
        VkMemoryMapFlags flags = 0;
        void* pData = NULL;

        // make a packet to map the memory in the generated trace file.
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkMapMemory* pPacket = NULL;
            CREATE_TRACE_PACKET(vkMapMemory, sizeof(void*));

            // actually map the memory if it was not already mapped.
            if (iter->second.ObjectInfo.DeviceMemory.mappedAddress == NULL)
            {
                result = mdd(device)->devTable.MapMemory(device, memory, offset, size, flags, &pData);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);

            pPacket = interpret_body_as_vkMapMemory(pHeader);
            pPacket->device = device;
            pPacket->memory = memory;
            pPacket->offset = offset;
            pPacket->size = size;
            pPacket->flags = flags;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData), sizeof(void*), pData);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData));
            pPacket->result = result;
            vktrace_finalize_trace_packet(pHeader);
            iter->second.ObjectInfo.DeviceMemory.pMapMemoryPacket = pHeader;
        }
        
        // By creating the packet for UnmapMemory, we'll be adding the pData buffer to it, which inherently copies it.
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

            // actually unmap the memory if wasn't already mapped by the application
            if (iter->second.ObjectInfo.DeviceMemory.mappedAddress == NULL)
            {
                mdd(device)->devTable.UnmapMemory(device, memory);
            }

            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket->device = device;
            pPacket->memory = memory;
            vktrace_finalize_trace_packet(pHeader);
            iter->second.ObjectInfo.DeviceMemory.pUnmapMemoryPacket = pHeader;
        }
    }

    vktrace_leave_critical_section(&trimStateTrackerLock);
}

// List of all the packets that have been recorded for the frames of interest.
static std::list<vktrace_trace_packet_header*> trim_recorded_packets;

static std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header*>> s_cmdBufferPackets;

#define TRIM_DEFINE_OBJECT_TRACKER_FUNCS(type) \
Trim_ObjectInfo* trim_add_##type##_object(Vk##type var) { \
    vktrace_enter_critical_section(&trimStateTrackerLock); \
    Trim_ObjectInfo& info = s_trimGlobalStateTracker.created##type##s[var]; \
    memset(&info, 0, sizeof(Trim_ObjectInfo)); \
    info.vkObject = (uint64_t)var; \
    vktrace_leave_critical_section(&trimStateTrackerLock); \
    return &info; \
} \
void trim_remove_##type##_object(Vk##type var) { \
    /* make sure the object actually existed before we attempt to remove it. This is for testing and thus only happens in debug builds. */ \
    vktrace_enter_critical_section(&trimStateTrackerLock); \
    assert(s_trimGlobalStateTracker.created##type##s.find(var) != s_trimGlobalStateTracker.created##type##s.end()); \
    s_trimGlobalStateTracker.created##type##s.erase(var); \
    vktrace_leave_critical_section(&trimStateTrackerLock); \
} \
Trim_ObjectInfo* trim_get_##type##_objectInfo(Vk##type var) { \
    vktrace_enter_critical_section(&trimStateTrackerLock); \
    TrimObjectInfoMap::iterator iter  = s_trimGlobalStateTracker.created##type##s.find(var); \
    Trim_ObjectInfo* pResult = NULL; \
    if (iter != s_trimGlobalStateTracker.created##type##s.end()) { \
        pResult = &(iter->second); \
    } \
    vktrace_leave_critical_section(&trimStateTrackerLock); \
    return pResult;\
}

#define TRIM_DEFINE_MARK_REF(type) \
void trim_mark_##type##_reference(Vk##type var) { \
    vktrace_enter_critical_section(&trimStateTrackerLock); \
    TrimObjectInfoMap::iterator iter  = s_trimGlobalStateTracker.created##type##s.find(var); \
    if (iter != s_trimGlobalStateTracker.created##type##s.end()) \
    { \
        iter->second.bReferencedInTrim = true; \
    } \
    vktrace_leave_critical_section(&trimStateTrackerLock); \
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
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DescriptorSetLayout);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(PipelineLayout);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(RenderPass);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(ShaderModule);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(PipelineCache);

TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DescriptorPool);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Pipeline);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Semaphore);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Fence);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Framebuffer);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(Event);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(QueryPool);
TRIM_DEFINE_OBJECT_TRACKER_FUNCS(DescriptorSet);

//=============================================================================
// Recreate all objects
//=============================================================================
void trim_write_all_referenced_object_calls()
{
    // write the referenced objects from the snapshot
    Trim_StateTracker& stateTracker = s_trimStateTrackerSnapshot;

    // Instances (& PhysicalDevices)
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdInstances.begin(); obj != stateTracker.createdInstances.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Instance.pCreatePacket));

        if (obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket));
        }

        if (obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket));
        }
    }

    // SurfaceKHR
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdSurfaceKHRs.begin(); obj != stateTracker.createdSurfaceKHRs.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SurfaceKHR.pCreatePacket));
    }

    // Devices
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdDevices.begin(); obj != stateTracker.createdDevices.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Device.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Device.pCreatePacket));
    }

    // Queue
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdQueues.begin(); obj != stateTracker.createdQueues.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Queue.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Queue.pCreatePacket));
    }

    // CommandPool
    for (TrimObjectInfoMap::iterator poolObj = stateTracker.createdCommandPools.begin(); poolObj != stateTracker.createdCommandPools.end(); poolObj++)
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
                for (TrimObjectInfoMap::iterator cbIter = stateTracker.createdCommandBuffers.begin(); cbIter != stateTracker.createdCommandBuffers.end(); cbIter++)
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
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdSwapchainKHRs.begin(); obj != stateTracker.createdSwapchainKHRs.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SwapchainKHR.pCreatePacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket));

        vktrace_write_trace_packet(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket));
    }

    // DeviceMemory
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdDeviceMemorys.begin(); obj != stateTracker.createdDeviceMemorys.end(); obj++)
    {
        // AllocateMemory
        vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DeviceMemory.pCreatePacket));

        // write map / unmap packets so the memory contents gets set on replay
        if (obj->second.ObjectInfo.DeviceMemory.pMapMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pMapMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DeviceMemory.pMapMemoryPacket));
        }

        if (obj->second.ObjectInfo.DeviceMemory.pUnmapMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DeviceMemory.pUnmapMemoryPacket));
        }
    }

    // Image
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdImages.begin(); obj != stateTracker.createdImages.end(); obj++)
    {
        // CreateImage
        if (obj->second.ObjectInfo.Image.pCreatePacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pCreatePacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pCreatePacket));
        }

        // GetImageMemoryRequirements
        if (obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pGetImageMemoryRequirementsPacket));
        }

        // BindImageMemory
        if (obj->second.ObjectInfo.Image.pBindImageMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pBindImageMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pBindImageMemoryPacket));
        }
    }

    // ImageView
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdImageViews.begin(); obj != stateTracker.createdImageViews.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.ImageView.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.ImageView.pCreatePacket));
    }

    // Buffer
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdBuffers.begin(); obj != stateTracker.createdBuffers.end(); obj++)
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

    // BufferView
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdBufferViews.begin(); obj != stateTracker.createdBufferViews.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.BufferView.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.BufferView.pCreatePacket));
    }

    // Sampler
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdSamplers.begin(); obj != stateTracker.createdSamplers.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Sampler.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Sampler.pCreatePacket));
    }

    // DescriptorSetLayout
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdDescriptorSetLayouts.begin(); obj != stateTracker.createdDescriptorSetLayouts.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DescriptorSetLayout.pCreatePacket));
    }

    // PipelineLayout
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdPipelineLayouts.begin(); obj != stateTracker.createdPipelineLayouts.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.PipelineLayout.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PipelineLayout.pCreatePacket));
    }

    // RenderPass
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdRenderPasss.begin(); obj != stateTracker.createdRenderPasss.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.RenderPass.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.RenderPass.pCreatePacket));
    }

    // ShaderModule
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdShaderModules.begin(); obj != stateTracker.createdShaderModules.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.ShaderModule.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.ShaderModule.pCreatePacket));
    }

    // PipelineCache
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdPipelineCaches.begin(); obj != stateTracker.createdPipelineCaches.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.PipelineCache.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PipelineCache.pCreatePacket));
    }

    // Pipeline
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdPipelines.begin(); obj != stateTracker.createdPipelines.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Pipeline.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Pipeline.pCreatePacket));
    }

    // DescriptorPool
    for (TrimObjectInfoMap::iterator poolObj = stateTracker.createdDescriptorPools.begin(); poolObj != stateTracker.createdDescriptorPools.end(); poolObj++)
    {
        // write the createDescriptorPool packet
        vktrace_write_trace_packet(poolObj->second.ObjectInfo.DescriptorPool.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(poolObj->second.ObjectInfo.DescriptorPool.pCreatePacket));

        if (poolObj->second.ObjectInfo.DescriptorPool.numSets > 0)
        {
            // now allocate all DescriptorSets that are part of this pool
            vktrace_trace_packet_header* pHeader;
            packet_vkAllocateDescriptorSets* pPacket = NULL;
            uint64_t vktraceStartTime = vktrace_get_time();
            SEND_ENTRYPOINT_ID(vkAllocateDescriptorSets);
            VkDescriptorSetAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocateInfo.pNext = NULL;
            allocateInfo.descriptorPool = static_cast<VkDescriptorPool>(poolObj->first);

            VkDescriptorSetLayout* pSetLayouts = new VkDescriptorSetLayout[poolObj->second.ObjectInfo.DescriptorPool.numSets];
            allocateInfo.pSetLayouts = pSetLayouts;
            VkDescriptorSet* pDescriptorSets = new VkDescriptorSet[poolObj->second.ObjectInfo.DescriptorPool.numSets];
            uint32_t setIndex = 0;

            for (TrimObjectInfoMap::iterator setObj = stateTracker.createdDescriptorSets.begin(); setObj != stateTracker.createdDescriptorSets.end(); setObj++)
            {
                // get descriptorSetLayouts and DescriptorSets specific to this pool
                if (setObj->second.ObjectInfo.DescriptorSet.descriptorPool == allocateInfo.descriptorPool &&
                    setIndex < poolObj->second.ObjectInfo.DescriptorPool.numSets)
                {
                    pSetLayouts[setIndex] = setObj->second.ObjectInfo.DescriptorSet.layout;
                    pDescriptorSets[setIndex] = static_cast<VkDescriptorSet>(setObj->first);
                    setIndex++;
                }
            }

            // update descriptorSetCount to ensure it is correct
            allocateInfo.descriptorSetCount = setIndex;

            CREATE_TRACE_PACKET(vkAllocateDescriptorSets, vk_size_vkdescriptorsetallocateinfo(&allocateInfo) + (allocateInfo.descriptorSetCount * sizeof(VkDescriptorSet)));
            pHeader->vktrace_begin_time = vktraceStartTime;

            pHeader->entrypoint_begin_time = vktrace_get_time();
            pHeader->entrypoint_end_time = vktrace_get_time();
            pPacket = interpret_body_as_vkAllocateDescriptorSets(pHeader);
            pPacket->device = poolObj->second.belongsToDevice;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkDescriptorSetAllocateInfo), &allocateInfo);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo->pSetLayouts), allocateInfo.descriptorSetCount * sizeof(VkDescriptorSetLayout), allocateInfo.pSetLayouts);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorSets), allocateInfo.descriptorSetCount * sizeof(VkDescriptorSet), pDescriptorSets);
            pPacket->result = VK_SUCCESS;
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo->pSetLayouts));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorSets));
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
            FINISH_TRACE_PACKET();

            delete[] pSetLayouts;
            delete[] pDescriptorSets;
        }
    }

    // Update DescriptorSets
    // needs to be done per-Device
    for (TrimObjectInfoMap::iterator deviceObj = stateTracker.createdDevices.begin(); deviceObj != stateTracker.createdDevices.end(); deviceObj++)
    {
        for (TrimObjectInfoMap::iterator setObj = stateTracker.createdDescriptorSets.begin(); setObj != stateTracker.createdDescriptorSets.end(); setObj++)
        {
            if (setObj->second.belongsToDevice == (VkDevice)deviceObj->first)
            {
                uint32_t descriptorWriteCount = setObj->second.ObjectInfo.DescriptorSet.writeDescriptorCount;
                uint32_t descriptorCopyCount = setObj->second.ObjectInfo.DescriptorSet.copyDescriptorCount;
                VkWriteDescriptorSet* pDescriptorWrites = setObj->second.ObjectInfo.DescriptorSet.pWriteDescriptorSets;
                VkCopyDescriptorSet* pDescriptorCopies = setObj->second.ObjectInfo.DescriptorSet.pCopyDescriptorSets;

                vktrace_trace_packet_header* pHeader;
                packet_vkUpdateDescriptorSets* pPacket = NULL;
                // begin custom code
                size_t arrayByteCount = 0;

                for (uint32_t i = 0; i < descriptorWriteCount; i++)
                {
                    arrayByteCount += get_struct_chain_size(&pDescriptorWrites[i]);
                }

                for (uint32_t i = 0; i < descriptorCopyCount; i++)
                {
                    arrayByteCount += get_struct_chain_size(&pDescriptorCopies[i]);
                }

                CREATE_TRACE_PACKET(vkUpdateDescriptorSets, arrayByteCount);
                // end custom code

                vktrace_set_packet_entrypoint_end_time(pHeader);
                pPacket = interpret_body_as_vkUpdateDescriptorSets(pHeader);
                pPacket->device = (VkDevice)deviceObj->first;
                pPacket->descriptorWriteCount = descriptorWriteCount;
                // begin custom code
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites), descriptorWriteCount * sizeof(VkWriteDescriptorSet), pDescriptorWrites);
                for (uint32_t i = 0; i < descriptorWriteCount; i++)
                {
                    switch (pPacket->pDescriptorWrites[i].descriptorType) {
                    case VK_DESCRIPTOR_TYPE_SAMPLER:
                    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
                    {
                        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pImageInfo),
                            pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo),
                            pDescriptorWrites[i].pImageInfo);
                        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pImageInfo));

                        // special to trim code
                        free((void*)pDescriptorWrites[i].pImageInfo);
                    }
                    break;
                    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                    {
                        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pTexelBufferView),
                            pDescriptorWrites[i].descriptorCount * sizeof(VkBufferView),
                            pDescriptorWrites[i].pTexelBufferView);
                        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pTexelBufferView));

                        // special to trim code
                        free((void*)pDescriptorWrites[i].pTexelBufferView);
                    }
                    break;
                    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
                    {
                        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pBufferInfo),
                            pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorBufferInfo),
                            pDescriptorWrites[i].pBufferInfo);
                        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pBufferInfo));

                        // special to trim code
                        free((void*)pDescriptorWrites[i].pBufferInfo);
                    }
                    break;
                    default:
                        break;
                    }
                }
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites));

                pPacket->descriptorCopyCount = descriptorCopyCount;
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorCopies), descriptorCopyCount * sizeof(VkCopyDescriptorSet), pDescriptorCopies);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorCopies));
                // end custom code

                // special to trim code
                free(pDescriptorWrites);
                free(pDescriptorCopies);

                FINISH_TRACE_PACKET();
            }
        }
    }

    // Framebuffer
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdFramebuffers.begin(); obj != stateTracker.createdFramebuffers.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Framebuffer.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Framebuffer.pCreatePacket));
    }

    // Semaphore
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdSemaphores.begin(); obj != stateTracker.createdSemaphores.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Semaphore.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Semaphore.pCreatePacket));
    }

    // Fence
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdFences.begin(); obj != stateTracker.createdFences.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Fence.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Fence.pCreatePacket));
    }

    // Event
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdEvents.begin(); obj != stateTracker.createdEvents.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.Event.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Event.pCreatePacket));
    }

    // QueryPool
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdQueryPools.begin(); obj != stateTracker.createdQueryPools.end(); obj++)
    {
        vktrace_write_trace_packet(obj->second.ObjectInfo.QueryPool.pCreatePacket, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(obj->second.ObjectInfo.QueryPool.pCreatePacket));
    }

    // write out the packets to recreate the command buffers that were just allocated
    for (TrimObjectInfoMap::iterator cmdBuffer = stateTracker.createdCommandBuffers.begin(); cmdBuffer != stateTracker.createdCommandBuffers.end(); cmdBuffer++)
    {
        std::list<vktrace_trace_packet_header*>& packets = s_cmdBufferPackets[(VkCommandBuffer)cmdBuffer->first];
        for (std::list<vktrace_trace_packet_header*>::iterator packet = packets.begin(); packet != packets.end(); packet++)
        {
            vktrace_trace_packet_header* pHeader = *packet;
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    // Collect semaphores that need signaling
    size_t maxSemaphores = stateTracker.createdSemaphores.size();
    uint32_t signalSemaphoreCount = 0;
    VkSemaphore* pSignalSemaphores = VKTRACE_NEW_ARRAY(VkSemaphore, maxSemaphores);
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdSemaphores.begin(); obj != stateTracker.createdSemaphores.end(); obj++)
    {
        VkQueue queue = obj->second.ObjectInfo.Semaphore.signaledOnQueue;
        if (queue != VK_NULL_HANDLE)
        {
            VkSemaphore semaphore = static_cast<VkSemaphore>(obj->first);
            pSignalSemaphores[signalSemaphoreCount++] = semaphore;

            VkSubmitInfo submit_info;
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info.pNext = NULL;
            submit_info.waitSemaphoreCount = 0;
            submit_info.pWaitSemaphores = NULL;
            submit_info.pWaitDstStageMask = NULL;
            submit_info.commandBufferCount = 0;
            submit_info.pCommandBuffers = NULL;
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &semaphore;

            // generating a packet for: vkQueueSubmit(queue, 1, pSubmits, VK_NULL_HANDLE);
            vktrace_trace_packet_header* pHeader;
            packet_vkQueueSubmit* pPacket = NULL;
            CREATE_TRACE_PACKET(vkQueueSubmit, sizeof(VkSubmitInfo) + sizeof(VkSemaphore));
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkQueueSubmit(pHeader);
            pPacket->queue = queue;
            pPacket->submitCount = 1;
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&pPacket->pSubmits, sizeof(VkSubmitInfo), &submit_info);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&pPacket->pSubmits->pSignalSemaphores, sizeof(VkSemaphore), &semaphore);
            vktrace_finalize_buffer_address(pHeader, (void**)&pPacket->pSubmits->pSignalSemaphores);
            vktrace_finalize_buffer_address(pHeader, (void**)&pPacket->pSubmits);
            pPacket->fence = VK_NULL_HANDLE;
            pPacket->result = VK_SUCCESS;
            FINISH_TRACE_PACKET();
        }
    }

    // Submit Queues in order to signal necessary fences
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdFences.begin(); obj != stateTracker.createdFences.end(); obj++)
    {
        VkQueue queue = obj->second.ObjectInfo.Fence.pendingOnQueue;
        if (queue != VK_NULL_HANDLE)
        {
            VkFence fence = static_cast<VkFence>(obj->first);

            // The packet below will simulate the follow call:
            //vkQueueSubmit(queue, 0, NULL, fence);

            vktrace_trace_packet_header* pHeader;
            packet_vkQueueSubmit* pPacket = NULL;
            CREATE_TRACE_PACKET(vkQueueSubmit, 0);
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkQueueSubmit(pHeader);
            pPacket->queue = queue;
            pPacket->submitCount = 0;
            pPacket->pSubmits = NULL;
            pPacket->fence = fence;
            pPacket->result = VK_SUCCESS;
            FINISH_TRACE_PACKET();
        }
    }

    VKTRACE_DELETE(pSignalSemaphores);
}

#define TRIM_MARK_OBJECT_REFERENCE(type) \
void trim_mark_##type##_reference(Vk##type var) { \
    Trim_ObjectInfo* info = &s_trimGlobalStateTracker.created##type##s[var]; \
    info->bReferencedInTrim = true; \
}

#define TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(type) \
void trim_mark_##type##_reference(Vk##type var) { \
    Trim_ObjectInfo* info = &s_trimGlobalStateTracker.created##type##s[var]; \
    info->bReferencedInTrim = true; \
    trim_mark_Device_reference((VkDevice)info->belongsToDevice); \
}

void trim_add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader)
{
    s_cmdBufferPackets[commandBuffer].push_back(pHeader);
}

void trim_remove_CommandBuffer_calls(VkCommandBuffer commandBuffer)
{
    s_cmdBufferPackets.erase(commandBuffer);
}

void trim_reset_DescriptorPool(VkDescriptorPool descriptorPool)
{
    for (TrimObjectInfoMap::iterator dsIter = s_trimGlobalStateTracker.createdDescriptorSets.begin(); dsIter != s_trimGlobalStateTracker.createdDescriptorSets.end(); )
    {
        if (dsIter->second.ObjectInfo.DescriptorSet.descriptorPool == descriptorPool)
        {
            VkDescriptorSet setToRemove = (VkDescriptorSet)dsIter->first;
            dsIter++;
            s_trimGlobalStateTracker.createdDescriptorSets.erase(setToRemove);
        }
        else
        {
            dsIter++;
        }
    }
}

//===============================================
// Object tracking
//===============================================
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(CommandPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(CommandBuffer)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorSet)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(RenderPass)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(PipelineCache)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Pipeline)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Queue)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Semaphore)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DeviceMemory)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Fence)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(SwapchainKHR)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Image)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(ImageView)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Buffer)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(BufferView)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Framebuffer)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Event)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(QueryPool)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(ShaderModule)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(PipelineLayout)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(Sampler)
TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(DescriptorSetLayout)

//===============================================
// Packet Recording for frames of interest
//===============================================
void trim_add_recorded_packet(vktrace_trace_packet_header* pHeader)
{
    vktrace_enter_critical_section(&trimRecordedPacketLock);
    trim_recorded_packets.push_back(pHeader);
    vktrace_leave_critical_section(&trimRecordedPacketLock);
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
    // QueryPool
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdQueryPools.begin(); obj != s_trimGlobalStateTracker.createdQueryPools.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyQueryPool* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyQueryPool, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyQueryPool(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->queryPool = (VkQueryPool)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.QueryPool.pAllocator;
        if (obj->second.ObjectInfo.QueryPool.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.QueryPool.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Event
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdEvents.begin(); obj != s_trimGlobalStateTracker.createdEvents.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyEvent* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyEvent, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyEvent(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->event = (VkEvent)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Event.pAllocator;
        if (obj->second.ObjectInfo.Event.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Event.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Fence
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdFences.begin(); obj != s_trimGlobalStateTracker.createdFences.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyFence* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyFence, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyFence(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->fence = (VkFence)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Fence.pAllocator;
        if (obj->second.ObjectInfo.Fence.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Fence.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Semaphore
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSemaphores.begin(); obj != s_trimGlobalStateTracker.createdSemaphores.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySemaphore* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySemaphore, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySemaphore(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->semaphore = (VkSemaphore)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Semaphore.pAllocator;
        if (obj->second.ObjectInfo.Semaphore.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Semaphore.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Framebuffer
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdFramebuffers.begin(); obj != s_trimGlobalStateTracker.createdFramebuffers.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyFramebuffer* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyFramebuffer, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyFramebuffer(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->framebuffer = (VkFramebuffer)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Framebuffer.pAllocator;
        if (obj->second.ObjectInfo.Framebuffer.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Framebuffer.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // DescriptorPool
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDescriptorPools.begin(); obj != s_trimGlobalStateTracker.createdDescriptorPools.end(); obj++)
    {
        // Free the associated DescriptorSets
        VkDescriptorPool descriptorPool = static_cast<VkDescriptorPool>(obj->first);

        // We can always call ResetDescriptorPool, but can only use FreeDescriptorSets if the pool was created with VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT.
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkResetDescriptorPool* pPacket = NULL;
            CREATE_TRACE_PACKET(vkResetDescriptorPool, 0);
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkResetDescriptorPool(pHeader);
            pPacket->device = obj->second.belongsToDevice;
            pPacket->descriptorPool = descriptorPool;
            pPacket->flags = 0;
            pPacket->result = VK_SUCCESS;
            FINISH_TRACE_PACKET();
        }

        // Now destroy the DescriptorPool
        {
            vktrace_trace_packet_header* pHeader;
            packet_vkDestroyDescriptorPool* pPacket = NULL;
            CREATE_TRACE_PACKET(vkDestroyDescriptorPool, sizeof(VkAllocationCallbacks));
            vktrace_set_packet_entrypoint_end_time(pHeader);
            pPacket = interpret_body_as_vkDestroyDescriptorPool(pHeader);
            pPacket->device = obj->second.belongsToDevice;
            pPacket->descriptorPool = descriptorPool;
            pPacket->pAllocator = obj->second.ObjectInfo.DescriptorPool.pAllocator;
            if (obj->second.ObjectInfo.DescriptorPool.pAllocator != NULL)
            {
                VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.DescriptorPool.pAllocator);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            }
            FINISH_TRACE_PACKET();
        }
    }

    // Pipeline
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdPipelines.begin(); obj != s_trimGlobalStateTracker.createdPipelines.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyPipeline* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyPipeline, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyPipeline(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->pipeline = (VkPipeline)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Pipeline.pAllocator;
        if (obj->second.ObjectInfo.Pipeline.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Pipeline.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // PipelineCache
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdPipelineCaches.begin(); obj != s_trimGlobalStateTracker.createdPipelineCaches.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyPipelineCache* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyPipelineCache, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyPipelineCache(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->pipelineCache = (VkPipelineCache)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.PipelineCache.pAllocator;
        if (obj->second.ObjectInfo.PipelineCache.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.PipelineCache.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // ShaderModule
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdShaderModules.begin(); obj != s_trimGlobalStateTracker.createdShaderModules.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyShaderModule* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyShaderModule, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyShaderModule(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->shaderModule = (VkShaderModule)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.ShaderModule.pAllocator;
        if (obj->second.ObjectInfo.ShaderModule.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.ShaderModule.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // RenderPass
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdRenderPasss.begin(); obj != s_trimGlobalStateTracker.createdRenderPasss.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyRenderPass* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyRenderPass, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyRenderPass(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->renderPass = (VkRenderPass)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.RenderPass.pAllocator;
        if (obj->second.ObjectInfo.RenderPass.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.RenderPass.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // PipelineLayout
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdPipelineLayouts.begin(); obj != s_trimGlobalStateTracker.createdPipelineLayouts.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyPipelineLayout* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyPipelineLayout, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyPipelineLayout(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->pipelineLayout = (VkPipelineLayout)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.PipelineLayout.pAllocator;
        if (obj->second.ObjectInfo.PipelineLayout.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.PipelineLayout.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // DescriptorSetLayout
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDescriptorSetLayouts.begin(); obj != s_trimGlobalStateTracker.createdDescriptorSetLayouts.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyDescriptorSetLayout* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyDescriptorSetLayout, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyDescriptorSetLayout(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->descriptorSetLayout = (VkDescriptorSetLayout)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.DescriptorSetLayout.pAllocator;
        if (obj->second.ObjectInfo.DescriptorSetLayout.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.DescriptorSetLayout.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Sampler
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSamplers.begin(); obj != s_trimGlobalStateTracker.createdSamplers.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySampler* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySampler, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySampler(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->sampler = (VkSampler)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Sampler.pAllocator;
        if (obj->second.ObjectInfo.Sampler.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Sampler.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Buffer
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdBuffers.begin(); obj != s_trimGlobalStateTracker.createdBuffers.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyBuffer* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyBuffer, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyBuffer(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->buffer = (VkBuffer)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Buffer.pAllocator;
        if (obj->second.ObjectInfo.Buffer.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Buffer.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // BufferView
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdBufferViews.begin(); obj != s_trimGlobalStateTracker.createdBufferViews.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyBufferView* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyBufferView, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyBufferView(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->bufferView = (VkBufferView)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.BufferView.pAllocator;
        if (obj->second.ObjectInfo.BufferView.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.BufferView.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Image
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdImages.begin(); obj != s_trimGlobalStateTracker.createdImages.end(); obj++)
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
            pPacket->pAllocator = obj->second.ObjectInfo.Image.pAllocator;
            if (obj->second.ObjectInfo.Image.pAllocator != NULL)
            {
                VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Image.pAllocator);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
            }
            FINISH_TRACE_PACKET();
        }
    }

    // ImageView
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdImageViews.begin(); obj != s_trimGlobalStateTracker.createdImageViews.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyImageView* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyImageView, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyImageView(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->imageView = (VkImageView)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.ImageView.pAllocator;
        if (obj->second.ObjectInfo.ImageView.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.ImageView.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // DeviceMemory
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDeviceMemorys.begin(); obj != s_trimGlobalStateTracker.createdDeviceMemorys.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkFreeMemory* pPacket = NULL;
        CREATE_TRACE_PACKET(vkFreeMemory, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkFreeMemory(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->memory = (VkDeviceMemory)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.DeviceMemory.pAllocator;
        if (obj->second.ObjectInfo.DeviceMemory.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.DeviceMemory.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // SwapchainKHR
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSwapchainKHRs.begin(); obj != s_trimGlobalStateTracker.createdSwapchainKHRs.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySwapchainKHR* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySwapchainKHR, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySwapchainKHR(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->swapchain = (VkSwapchainKHR)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.SwapchainKHR.pAllocator;
        if (obj->second.ObjectInfo.SwapchainKHR.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.SwapchainKHR.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // CommandPool
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdCommandPools.begin(); obj != s_trimGlobalStateTracker.createdCommandPools.end(); obj++)
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
                for (TrimObjectInfoMap::iterator cbIter = s_trimGlobalStateTracker.createdCommandBuffers.begin(); cbIter != s_trimGlobalStateTracker.createdCommandBuffers.end(); cbIter++)
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

        // Destroy the CommandPool
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyCommandPool* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyCommandPool, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyCommandPool(pHeader);
        pPacket->device = obj->second.belongsToDevice;
        pPacket->commandPool = (VkCommandPool) obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.CommandPool.pAllocator;
        if (obj->second.ObjectInfo.CommandPool.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.CommandPool.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Device
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDevices.begin(); obj != s_trimGlobalStateTracker.createdDevices.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyDevice* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyDevice, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyDevice(pHeader);
        pPacket->device = (VkDevice)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Device.pAllocator;
        if (obj->second.ObjectInfo.Device.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Device.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // SurfaceKHR
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSurfaceKHRs.begin(); obj != s_trimGlobalStateTracker.createdSurfaceKHRs.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroySurfaceKHR* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroySurfaceKHR, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroySurfaceKHR(pHeader);
        pPacket->surface = (VkSurfaceKHR)obj->first;
        pPacket->instance = obj->second.belongsToInstance;
        pPacket->pAllocator = obj->second.ObjectInfo.SurfaceKHR.pAllocator;
        if (obj->second.ObjectInfo.SurfaceKHR.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.SurfaceKHR.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }

    // Instance
    for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdInstances.begin(); obj != s_trimGlobalStateTracker.createdInstances.end(); obj++)
    {
        vktrace_trace_packet_header* pHeader;
        packet_vkDestroyInstance* pPacket = NULL;
        CREATE_TRACE_PACKET(vkDestroyInstance, sizeof(VkAllocationCallbacks));
        vktrace_set_packet_entrypoint_end_time(pHeader);
        pPacket = interpret_body_as_vkDestroyInstance(pHeader);
        pPacket->instance = (VkInstance)obj->first;
        pPacket->pAllocator = obj->second.ObjectInfo.Instance.pAllocator;
        if (obj->second.ObjectInfo.Instance.pAllocator != NULL)
        {
            VkAllocationCallbacks* pAllocator = trim_get_Allocator(obj->second.ObjectInfo.Instance.pAllocator);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), &pAllocator);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
        }
        FINISH_TRACE_PACKET();
    }
}

//===============================================
// Delete all the created packets
//===============================================
#define TRIM_DELETE_ALL_PACKETS(type) \
    s_trimGlobalStateTracker.created##type##s.clear();

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

    vktrace_delete_critical_section(&trimRecordedPacketLock);
    vktrace_delete_critical_section(&trimStateTrackerLock);
}
