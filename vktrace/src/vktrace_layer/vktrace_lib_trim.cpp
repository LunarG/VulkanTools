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

// defined in vktrace_lib_trace.cpp
extern layer_device_data *mdd(void* object);
extern layer_instance_data *mid(void *object);

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
VKTRACE_CRITICAL_SECTION trimCommandBufferPacketLock;

// Typically an application will have one VkAllocationCallbacks struct and 
// will pass in that same address as needed, so we'll keep a map to correlate
// the supplied address to the AllocationCallbacks object.
static std::unordered_map<const void*, VkAllocationCallbacks> s_trimAllocatorMap;

// List of all the packets that have been recorded for the frames of interest.
std::list<vktrace_trace_packet_header*> trim_recorded_packets;

std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header*>> s_cmdBufferPackets;

// List of all packets used to create or delete images.
// We need to recreate them in the same order to ensure they will have the same size requirements as they had a trace-time.
std::list<vktrace_trace_packet_header*> trim_image_calls;

void trim_initialize()
{
    vktrace_create_critical_section(&trimStateTrackerLock);
    vktrace_create_critical_section(&trimRecordedPacketLock);
    vktrace_create_critical_section(&trimCommandBufferPacketLock);
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

bool trim_IsMemoryDeviceOnly(VkDevice device, VkDeviceMemory memory)
{
    Trim_ObjectInfo* pInfo = trim_get_DeviceMemory_objectInfo(memory);
    assert(pInfo != NULL);
    if (pInfo == NULL)
        return false;

    // Is device only if it has VK_MEMORY_PROPERTY_DEVICE_LOCAL and not VK_MEMORY_PROPERTY_HOST_VISIBLE
    bool isDeviceOnly = (pInfo->ObjectInfo.DeviceMemory.propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    return isDeviceOnly;
}

VkMemoryPropertyFlags Trim_LookUpMemoryProperties(VkDevice device, uint32_t memoryTypeIndex)
{
    assert(memoryTypeIndex < VK_MAX_MEMORY_TYPES);
    if (memoryTypeIndex >= VK_MAX_MEMORY_TYPES)
        return false;

    VkPhysicalDevice physicalDevice = trim_get_Device_objectInfo(device)->belongsToPhysicalDevice;
    assert(physicalDevice != VK_NULL_HANDLE);
    if (physicalDevice == VK_NULL_HANDLE)
        return false;

    Trim_ObjectInfo* pInfo = trim_get_PhysicalDevice_objectInfo(physicalDevice);
    assert(pInfo != NULL);
    if (pInfo == NULL)
        return false;

    if (memoryTypeIndex >= pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypeCount)
        return false;

    return pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;
}

uint32_t trim_FindMemoryTypeIndex(VkDevice device, uint32_t memoryTypeBits, VkMemoryPropertyFlags propertyFlags)
{
    VkPhysicalDevice physicalDevice = trim_get_Device_objectInfo(device)->belongsToPhysicalDevice;
    assert(physicalDevice != VK_NULL_HANDLE);
    if (physicalDevice == VK_NULL_HANDLE)
        return 0;

    Trim_ObjectInfo* pInfo = trim_get_PhysicalDevice_objectInfo(physicalDevice);
    assert(pInfo != NULL);
    for (uint32_t i = 0; i, pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((memoryTypeBits & (1 << i)) &&
            ((pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags))
            return i;
    }
    // default to 0 if a more specific match is not found
    return 0;
}

VkImageAspectFlags trim_getImageAspectFromFormat(VkFormat format)
{
    VkImageAspectFlags aspectMask;
    switch (format)
    {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
        aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        break;
    case VK_FORMAT_S8_UINT:
        aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
        break;
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        break;
    default:
        aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        break;
    }

    return aspectMask;
}

//=============================================================================
// Helpers to generate trace packets
//=============================================================================
vktrace_trace_packet_header* trim_generate_vkCreateCommandPool(
    VkDevice device, 
    const VkCommandPoolCreateInfo* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, 
    VkCommandPool* pCommandPool)
{
    VkResult result;
    vktrace_trace_packet_header* pHeader;
    packet_vkCreateCommandPool* pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateCommandPool, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkCommandPool));

    // need to actually create the command Pool so that we have a valid handle to supply to the trace file for use on replay
    result = mdd(device)->devTable.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);

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

vktrace_trace_packet_header* trim_generate_vkAllocateCommandBuffers(
    bool makeCall,
    VkDevice device,
    const VkCommandBufferAllocateInfo* pAllocateInfo,
    VkCommandBuffer* pCommandBuffers)
{
    VkResult result;
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

// Doesn't allow making the actual call (partially because we don't have a device handle, also because we don't need to actually call it).
vktrace_trace_packet_header* trim_generate_vkBeginCommandBuffer(
    VkCommandBuffer commandBuffer,
    const VkCommandBufferBeginInfo* pBeginInfo)
{
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header* pHeader;
    packet_vkBeginCommandBuffer* pPacket = NULL;
    CREATE_TRACE_PACKET(vkBeginCommandBuffer, get_struct_chain_size((void*)pBeginInfo));

    // Not allowing being able to make this call. 

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

vktrace_trace_packet_header* trim_generate_vkCmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkCmdPipelineBarrier* pPacket = NULL;
    size_t customSize;
    customSize = (memoryBarrierCount * sizeof(VkMemoryBarrier)) +
        (bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier)) +
        (imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier));
    CREATE_TRACE_PACKET(vkCmdPipelineBarrier, customSize);

    // Don't actually need to issue the PipelineBarrier, just need the packet.

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

vktrace_trace_packet_header* trim_generate_vkEndCommandBuffer(
    VkCommandBuffer commandBuffer)
{
    VkResult result;
    vktrace_trace_packet_header* pHeader;
    packet_vkEndCommandBuffer* pPacket = NULL;
    CREATE_TRACE_PACKET(vkEndCommandBuffer, 0);

    // Don't need to actually end command buffer, we just need the packet, pretend the call was successful.
    result = VK_SUCCESS;

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkEndCommandBuffer(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header* trim_generate_vkQueueSubmit(
    VkQueue queue,
    uint32_t submitCount,
    const VkSubmitInfo* pSubmits,
    VkFence fence)
{
    VkResult result;
    vktrace_trace_packet_header* pHeader;
    packet_vkQueueSubmit* pPacket = NULL;
    size_t arrayByteCount = 0;
    uint32_t i = 0;
    for (i = 0; i < submitCount; ++i) {
        arrayByteCount += vk_size_vksubmitinfo(&pSubmits[i]);
    }
    CREATE_TRACE_PACKET(vkQueueSubmit, arrayByteCount);

    // Don't actually need to submit the queue, just pretend the call was successful.
    result = VK_SUCCESS;

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

vktrace_trace_packet_header* trim_generate_vkQueueWaitIdle(
    VkQueue queue)
{
    VkResult result;
    vktrace_trace_packet_header* pHeader;
    packet_vkQueueWaitIdle* pPacket = NULL;
    CREATE_TRACE_PACKET(vkQueueWaitIdle, 0);

    // Don't want to actually wait on the queue, so pretend it completed successfully
    result = VK_SUCCESS;

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkQueueWaitIdle(pHeader);
    pPacket->queue = queue;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header* trim_generate_vkResetCommandPool(
    VkDevice device,
    VkCommandPool commandPool,
    VkCommandPoolResetFlags flags)
{
    VkResult result;
    vktrace_trace_packet_header* pHeader;
    packet_vkResetCommandPool* pPacket = NULL;
    CREATE_TRACE_PACKET(vkResetCommandPool, 0);

    // Actually make this call, because we need to release the commandbuffer(s) we created.
    result = mdd(device)->devTable.ResetCommandPool(device, commandPool, flags);

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkResetCommandPool(pHeader);
    pPacket->device = device;
    pPacket->commandPool = commandPool;
    pPacket->flags = flags;
    pPacket->result = result;
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header* trim_generate_vkFreeCommandBuffers(
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

vktrace_trace_packet_header* trim_generate_vkDestroyCommandPool(
    VkDevice device,
    VkCommandPool commandPool,
    const VkAllocationCallbacks* pAllocator)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkDestroyCommandPool* pPacket = NULL;
    CREATE_TRACE_PACKET(vkDestroyCommandPool, sizeof(VkAllocationCallbacks));

    // Actually make this call to destroy the commandPool that we created
    mdd(device)->devTable.DestroyCommandPool(device, commandPool, pAllocator);

    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkDestroyCommandPool(pHeader);
    pPacket->device = device;
    pPacket->commandPool = commandPool;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_trace_packet(pHeader);
    return pHeader;
}

vktrace_trace_packet_header* trim_generate_vkMapMemory(
    bool                                        makeCall,
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize                                offset,
    VkDeviceSize                                size,
    VkMemoryMapFlags                            flags,
    void**                                      ppData)
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

vktrace_trace_packet_header* trim_generate_vkUnmapMemory(
    bool                                        makeCall,
    VkDeviceSize                                size,
    void*                                       pData,
    VkDevice                                    device,
    VkDeviceMemory                              memory)
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

vktrace_trace_packet_header* trim_generate_vkCreateBuffer(
    bool                                        makeCall,
    VkDevice                                    device,
    const VkBufferCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBuffer*                                   pBuffer)
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

vktrace_trace_packet_header* trim_generate_vkGetBufferMemoryRequirements(
    bool                    makeCall,
    VkDevice                device,
    VkBuffer                buffer,
   VkMemoryRequirements*    pMemoryRequirements)
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

vktrace_trace_packet_header* trim_generate_vkAllocateMemory(
    bool                            makeCall,
    VkDevice                        device,
    const VkMemoryAllocateInfo*     pAllocateInfo,
    const VkAllocationCallbacks*    pAllocator,
    VkDeviceMemory*                 pMemory)
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

vktrace_trace_packet_header* trim_generate_vkBindBufferMemory(
    bool            makeCall,
    VkDevice        device,
    VkBuffer        buffer,
    VkDeviceMemory  memory,
    VkDeviceSize    memoryOffset)
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

vktrace_trace_packet_header* trim_generate_vkCmdCopyBuffer(
    bool                makeCall,
    VkDevice            device,
    VkCommandBuffer     commandBuffer,
    VkBuffer            srcBuffer,
    VkBuffer            dstBuffer,
    uint32_t            regionCount,
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

vktrace_trace_packet_header* trim_generate_vkCreateFence(
    bool                        makeCall,
    VkDevice                    device,
    const VkFenceCreateInfo*    pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkFence*                    pFence)
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

vktrace_trace_packet_header* trim_generate_vkWaitForFences(
    bool            makeCall,
    VkDevice        device,
    uint32_t        fenceCount,
    const VkFence*  pFences,
    VkBool32        waitAll,
    uint64_t        timeout)
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

vktrace_trace_packet_header* trim_generate_vkDestroyBuffer(
    bool            makeCall,
    VkDevice        device,
    VkBuffer        buffer,
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

vktrace_trace_packet_header* trim_generate_vkFreeMemory(
    bool                            makeCall,
    VkDevice                        device,
    VkDeviceMemory                  memory,
    const VkAllocationCallbacks*    pAllocator)
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

// Information necessary to create the staged buffer and memory for DEVICE_LOCAL buffers.
// This holds all the necessary structures so that we can fill them in once when we make
// the call during trim snapshot, and then reuse them when generating the calls to recreate
// the buffer into the trace file.
struct Trim_StagingInfo
{
    VkBuffer buffer = {};
    VkBufferCreateInfo bufferCreateInfo = {};
    VkMemoryRequirements bufferMemoryRequirements = {};

    VkDeviceMemory memory = {};
    VkMemoryAllocateInfo memoryAllocationInfo = {};

    VkBufferCopy copyRegion = {};

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkFence fence;
    VkQueue queue;
};

// map to associate trace-time buffer with a staged buffer
std::unordered_map<VkBuffer, Trim_StagingInfo> trim_bufferToStagedInfoMap;

//=============================================================================
// Use this to snapshot the global state tracker at the start of the trim frames.
//=============================================================================
void trim_snapshot_state_tracker()
{
    vktrace_enter_critical_section(&trimStateTrackerLock);
    s_trimStateTrackerSnapshot = s_trimGlobalStateTracker;

    std::unordered_map<VkDevice, VkCommandPool> deviceToCommandPoolMap;
    std::unordered_map<VkDevice, VkCommandBuffer> deviceToCommandBufferMap;
    std::unordered_map<VkDevice, VkFence> deviceToFenceMap;

    VkInstance instance = VK_NULL_HANDLE;
    if (s_trimStateTrackerSnapshot.createdInstances.size() > 0)
    {
        instance = (VkInstance)s_trimStateTrackerSnapshot.createdInstances.begin()->first;
    }
    if (instance != VK_NULL_HANDLE && mid(instance)->LunargDebugReportEnabled)
    {
        mid(instance)->instTable.DebugReportMessageEXT(instance, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT, (uint64_t)instance, __LINE__, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, "trace_trim", "Starting State Snapshot");
    }

    // Copying all the buffers is a length process:
    // 1) Create a cmd pool and cmd buffer on each device; begin the command buffer.
    // 2a) Transition all images into host-readable state.
    // 2b) Transition all buffers into host-readable state.
    // 3) End the cmd buffers and submit them on the right queue.
    // 4a) Map, copy, unmap each image.
    // 4b) Map, copy, unmap each buffer.
    // 5) Begin the cmd buffers again.
    // 6a) Transition all the images back to their previous state.
    // 6b) Transition all the images back to their previous state.
    // 7) End the cmd buffers, submit, and wait for them.
    // 8) destroy the command pools, command buffers, and fences.

    // 1) Create a cmd pool and cmd buffer on each device; begin the command buffer.
    for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
    {
        VkDevice device = static_cast<VkDevice>(deviceIter->first);

        // Find or create an existing command pool
        VkCommandPool commandPool = VK_NULL_HANDLE;
        if (deviceToCommandPoolMap.find(device) == deviceToCommandPoolMap.end())
        {
            // create a new command pool on the device
            VkCommandPoolCreateInfo cmdPoolCreateInfo;
            cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cmdPoolCreateInfo.pNext = NULL;
            cmdPoolCreateInfo.queueFamilyIndex = 0; // TODO: just a guess - probably need to do this for each queue family
            cmdPoolCreateInfo.flags = 0;

            VkResult result = mdd(device)->devTable.CreateCommandPool(device, &cmdPoolCreateInfo, NULL, &commandPool);
            assert(result == VK_SUCCESS);
            if (result == VK_SUCCESS)
            {
                deviceToCommandPoolMap[device] = commandPool;
            }
        }
        else
        {
            commandPool = deviceToCommandPoolMap[device];
        }

        // Find or create an existing command buffer
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        if (deviceToCommandBufferMap.find(device) == deviceToCommandBufferMap.end())
        {
            // allocate a new command buffer on the device
            VkCommandBufferAllocateInfo allocateInfo;
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.pNext = NULL;
            allocateInfo.commandPool = commandPool;
            allocateInfo.level = (VkCommandBufferLevel)VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocateInfo.commandBufferCount = 1;

            VkResult result = mdd(device)->devTable.AllocateCommandBuffers(device, &allocateInfo, &commandBuffer);
            assert(result == VK_SUCCESS);
            if (result == VK_SUCCESS)
            {
                deviceToCommandBufferMap[device] = commandBuffer;
            }
        }
        else
        {
            commandBuffer = deviceToCommandBufferMap[device];
        }

        // Begin the command buffer
        VkCommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = NULL;
        commandBufferBeginInfo.pInheritanceInfo = NULL;
        commandBufferBeginInfo.flags = 0;
        VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
        assert(result == VK_SUCCESS);

        // Find or create a Fence for this device
        if (deviceToFenceMap.find(device) == deviceToFenceMap.end())
        {
            // create a new fence on the device
            VkFenceCreateInfo fenceCreateInfo;
            fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCreateInfo.pNext = NULL;
            fenceCreateInfo.flags = 0;

            VkFence fence = VK_NULL_HANDLE;
            VkResult result = mdd(device)->devTable.CreateFence(device, &fenceCreateInfo, NULL, &fence);
            assert(result == VK_SUCCESS);
            if (result == VK_SUCCESS)
            {
                deviceToFenceMap[device] = fence;
            }
        }
    }

    // 2a) Transition all images into host-readable state.
    for (TrimObjectInfoMap::iterator imageIter = s_trimStateTrackerSnapshot.createdImages.begin(); imageIter != s_trimStateTrackerSnapshot.createdImages.end(); imageIter++)
    {
        VkDevice device = imageIter->second.belongsToDevice;
        VkImage image = static_cast<VkImage>(imageIter->first);

        if (device == VK_NULL_HANDLE)
        {
            // this is likely a swapchain image which we haven't associated a device to, just skip over it.
            continue;
        }

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        // Create a memory barrier to make it host readable
        VkImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = NULL;
        imageMemoryBarrier.srcAccessMask = imageIter->second.ObjectInfo.Image.accessFlags;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
        imageMemoryBarrier.oldLayout = imageIter->second.ObjectInfo.Image.mostRecentLayout;
        imageMemoryBarrier.newLayout = imageIter->second.ObjectInfo.Image.mostRecentLayout;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = imageIter->second.ObjectInfo.Image.aspectMask;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = imageIter->second.ObjectInfo.Image.mipLevels;
        imageMemoryBarrier.srcQueueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;
        imageMemoryBarrier.dstQueueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;

        mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
    }

    // 2b) Transition all buffers into host-readable state.
    for (TrimObjectInfoMap::iterator bufferIter = s_trimStateTrackerSnapshot.createdBuffers.begin(); bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end(); bufferIter++)
    {
        VkDevice device = bufferIter->second.belongsToDevice;
        VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        // If the buffer needs a staging buffer, it's because it's on DEVICE_LOCAL memory that is not HOST_VISIBLE.
        // So we have to create another buffer and memory that IS HOST_VISIBLE so that we can copy the data
        // from the DEVICE_LOCAL memory into HOST_VISIBLE memory, then map / unmap the HOST_VISIBLE memory object.
        // The staging info is kept so that we can generate similar calls in the trace file in order to recreate
        // the DEVICE_LOCAL buffer.
        if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer)
        {
            // TODO: Need to store this in the trim buffer info.
            Trim_StagingInfo stagingInfo = {};

            stagingInfo.commandPool = deviceToCommandPoolMap[device];
            stagingInfo.commandBuffer = commandBuffer;
            stagingInfo.fence = deviceToFenceMap[device];
            mdd(device)->devTable.GetDeviceQueue(device, 0, 0, &stagingInfo.queue);

            stagingInfo.bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            stagingInfo.bufferCreateInfo.pNext = NULL;
            stagingInfo.bufferCreateInfo.flags = 0;
            stagingInfo.bufferCreateInfo.size = bufferIter->second.ObjectInfo.Buffer.size;
            stagingInfo.bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            stagingInfo.bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            stagingInfo.bufferCreateInfo.queueFamilyIndexCount = 0;
            stagingInfo.bufferCreateInfo.pQueueFamilyIndices = NULL;

            mdd(device)->devTable.CreateBuffer(device, &stagingInfo.bufferCreateInfo, NULL, &stagingInfo.buffer);

            mdd(device)->devTable.GetBufferMemoryRequirements(device, stagingInfo.buffer, &stagingInfo.bufferMemoryRequirements);

            stagingInfo.memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            stagingInfo.memoryAllocationInfo.pNext = NULL;
            stagingInfo.memoryAllocationInfo.allocationSize = stagingInfo.bufferMemoryRequirements.size;
            stagingInfo.memoryAllocationInfo.memoryTypeIndex = trim_FindMemoryTypeIndex(device, stagingInfo.bufferMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

            mdd(device)->devTable.AllocateMemory(device, &stagingInfo.memoryAllocationInfo, nullptr, &stagingInfo.memory);

            mdd(device)->devTable.BindBufferMemory(device, stagingInfo.buffer, stagingInfo.memory, 0);

            // Copy from device_local buffer to host_visible buffer
            stagingInfo.copyRegion.size = bufferIter->second.ObjectInfo.Buffer.size;

            mdd(device)->devTable.CmdCopyBuffer(
                commandBuffer,
                buffer,
                stagingInfo.buffer,
                1,
                &stagingInfo.copyRegion);

            // save the staging info for later
            trim_bufferToStagedInfoMap[buffer] = stagingInfo;
        }
        else
        {
            // Create a memory barrier to make it host readable
            VkBufferMemoryBarrier bufferMemoryBarrier;
            bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            bufferMemoryBarrier.pNext = NULL;
            bufferMemoryBarrier.srcAccessMask = bufferIter->second.ObjectInfo.Buffer.accessFlags;
            bufferMemoryBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
            bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufferMemoryBarrier.buffer = buffer;
            bufferMemoryBarrier.offset = bufferIter->second.ObjectInfo.Buffer.memoryOffset;
            bufferMemoryBarrier.size = bufferIter->second.ObjectInfo.Buffer.size;

            mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
        }
    }

    // 3) End the cmd buffers and submit them on the right queue.
    for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
    {
        VkDevice device = static_cast<VkDevice>(deviceIter->first);

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        mdd(device)->devTable.EndCommandBuffer(commandBuffer);

        // now submit the command buffer
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = NULL;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = NULL;
        submitInfo.pWaitDstStageMask = NULL;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = NULL;

        // Submit the queue and wait for it to complete
        VkQueue queue = VK_NULL_HANDLE;
        mdd(device)->devTable.GetDeviceQueue(device, 0, 0, &queue);
        VkFence fence = deviceToFenceMap[device];
        mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, fence);
        mdd(device)->devTable.WaitForFences(device, 1, &fence, VK_TRUE, 10*1000*1000);
    }

    // 4a) Map, copy, unmap each image.
    for (TrimObjectInfoMap::iterator imageIter = s_trimStateTrackerSnapshot.createdImages.begin(); imageIter != s_trimStateTrackerSnapshot.createdImages.end(); imageIter++)
    {
        VkDevice device = imageIter->second.belongsToDevice;
        VkImage image = static_cast<VkImage>(imageIter->first);

        if (device == VK_NULL_HANDLE)
        {
            // this is likely a swapchain image which we haven't associated a device to, just skip over it.
            continue;
        }

        VkDeviceMemory memory = imageIter->second.ObjectInfo.Image.memory;
        VkDeviceSize offset = imageIter->second.ObjectInfo.Image.memoryOffset;
        VkDeviceSize size = imageIter->second.ObjectInfo.Image.memorySize;
        VkMemoryMapFlags flags = 0;

        TrimObjectInfoMap::iterator memoryIter = s_trimStateTrackerSnapshot.createdDeviceMemorys.find(memory);

        if (memoryIter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end())
        {
            void* mappedAddress = memoryIter->second.ObjectInfo.DeviceMemory.mappedAddress;
            VkDeviceSize mappedOffset = memoryIter->second.ObjectInfo.DeviceMemory.mappedOffset;
            VkDeviceSize mappedSize = memoryIter->second.ObjectInfo.DeviceMemory.mappedSize;

            if (size != 0)
            {
                // actually map the memory if it was not already mapped.
                bool bAlreadyMapped = (mappedAddress != NULL);
                if (bAlreadyMapped)
                {
                    // I imagine there could be a scenario where the application has persistently
                    // mapped PART of the memory, which may not contain the image that we're trying to copy right now.
                    // In that case, there will be errors due to this code. We know the range of memory that is mapped
                    // so we should be able to confirm whether or not we get into this situation.
                    bAlreadyMapped = (offset >= mappedOffset && (offset + size) <= (mappedOffset + mappedSize));
                }

                vktrace_trace_packet_header* pMapMemory = trim_generate_vkMapMemory(
                    !bAlreadyMapped,
                    device,
                    memory,
                    offset,
                    size,
                    flags,
                    &mappedAddress
                    );

                if (mappedAddress == NULL)
                {
                    // if the mapped address is still NULL, that means the map failed for some reason.
                    // One reason is that the memory does NOT have VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. (TODO)
                    // Since it failed, we'll just delete the packet and won't create an unmap for it.
                    vktrace_delete_trace_packet(&pMapMemory);
                }
                else
                {
                    imageIter->second.ObjectInfo.Image.pMapMemoryPacket = pMapMemory;

                    // By creating the packet for UnmapMemory, we'll be adding the pData buffer to it, which inherently copies it.

                    // need to adjust the pointer to ensure it points to the beginning of the image memory, which may NOT be
                    // the same as the offset of the mapped address.
                    void* imageAddress = (BYTE*)mappedAddress + (offset - mappedOffset);

                    // Actually unmap the memory if it wasn't already mapped by the application
                    vktrace_trace_packet_header* pUnmapMemory = trim_generate_vkUnmapMemory(
                        !bAlreadyMapped,
                        size,
                        imageAddress,
                        device,
                        memory);
                    imageIter->second.ObjectInfo.Image.pUnmapMemoryPacket = pUnmapMemory;
                }
            }
        }
    }

    // 4b) Map, copy, unmap each buffer.
    for (TrimObjectInfoMap::iterator bufferIter = s_trimStateTrackerSnapshot.createdBuffers.begin(); bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end(); bufferIter++)
    {
        VkDevice device = bufferIter->second.belongsToDevice;
        VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);

        VkDeviceMemory memory = bufferIter->second.ObjectInfo.Buffer.memory;
        VkDeviceSize offset = bufferIter->second.ObjectInfo.Buffer.memoryOffset;
        VkDeviceSize size = bufferIter->second.ObjectInfo.Buffer.size;
        VkMemoryMapFlags flags = 0;

        void* mappedAddress = NULL;
        VkDeviceSize mappedOffset = 0;
        VkDeviceSize mappedSize = 0;

        if (trim_bufferToStagedInfoMap.find(buffer) != trim_bufferToStagedInfoMap.end())
        {
            // Note that the staged memory object won't be in the state tracker, so we want to swap out the buffer and memory
            // that will be mapped / unmapped.
            Trim_StagingInfo staged = trim_bufferToStagedInfoMap[buffer];
            buffer = staged.buffer;
            memory = staged.memory;
            offset = 0;
        }
        else
        {
            TrimObjectInfoMap::iterator memoryIter = s_trimStateTrackerSnapshot.createdDeviceMemorys.find(memory);
            assert(memoryIter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end());
            if (memoryIter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end())
            {
                mappedAddress = memoryIter->second.ObjectInfo.DeviceMemory.mappedAddress;
                mappedOffset = memoryIter->second.ObjectInfo.DeviceMemory.mappedOffset;
                mappedSize = memoryIter->second.ObjectInfo.DeviceMemory.mappedSize;
            }
        }

        if (size != 0)
        {
            // actually map the memory if it was not already mapped.
            bool bAlreadyMapped = (mappedAddress != NULL);
            if (bAlreadyMapped)
            {
                // I imagine there could be a scenario where the application has persistently
                // mapped PART of the memory, which may not contain the image that we're trying to copy right now.
                // In that case, there will be errors due to this code. We know the range of memory that is mapped
                // so we should be able to confirm whether or not we get into this situation.
                bAlreadyMapped = (offset >= mappedOffset && (offset + size) <= (mappedOffset + mappedSize));
            }

            vktrace_trace_packet_header* pMapMemory = trim_generate_vkMapMemory(
                !bAlreadyMapped,
                device,
                memory,
                offset,
                size,
                flags,
                &mappedAddress
                );
            if (mappedAddress == NULL)
            {
                // if the mapped address is still NULL, that means the map failed for some reason.
                // One reason is that the memory does NOT have VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. (TODO)
                // Since it failed, we'll just delete the packet and won't create an unmap for it.
                vktrace_delete_trace_packet(&pMapMemory);
            }
            else
            {
                bufferIter->second.ObjectInfo.Buffer.pMapMemoryPacket = pMapMemory;

                // By creating the packet for UnmapMemory, we'll be adding the pData buffer to it, which inherently copies it.

                // need to adjust the pointer to ensure it points to the beginning of the image memory, which may NOT be
                // the same as the offset of the mapped address.
                void* bufferAddress = (BYTE*)mappedAddress + (offset - mappedOffset);

                // Actually unmap the memory if it wasn't already mapped by the application
                vktrace_trace_packet_header* pUnmapMemory = trim_generate_vkUnmapMemory(
                    !bAlreadyMapped,
                    size,
                    bufferAddress,
                    device,
                    memory);
                bufferIter->second.ObjectInfo.Buffer.pUnmapMemoryPacket = pUnmapMemory;
            }
        }
    }

    // 5) Begin the cmd buffers again.
    for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
    {
        VkDevice device = static_cast<VkDevice>(deviceIter->first);

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        // Begin the command buffer
        VkCommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = NULL;
        commandBufferBeginInfo.pInheritanceInfo = NULL;
        commandBufferBeginInfo.flags = 0;
        VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
        assert(result == VK_SUCCESS);
    }

    // 6a) Transition all the images back to their previous state.
    for (TrimObjectInfoMap::iterator imageIter = s_trimStateTrackerSnapshot.createdImages.begin(); imageIter != s_trimStateTrackerSnapshot.createdImages.end(); imageIter++)
    {
        VkDevice device = imageIter->second.belongsToDevice;
        VkImage image = static_cast<VkImage>(imageIter->first);

        if (device == VK_NULL_HANDLE)
        {
            // this is likely a swapchain image which we haven't associated a device to, just skip over it.
            continue;
        }

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        // Create a memory barrier to make it host readable
        VkImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = NULL;
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_READ_BIT;
        imageMemoryBarrier.dstAccessMask = imageIter->second.ObjectInfo.Image.accessFlags;
        imageMemoryBarrier.oldLayout = imageIter->second.ObjectInfo.Image.mostRecentLayout;
        imageMemoryBarrier.newLayout = imageIter->second.ObjectInfo.Image.mostRecentLayout;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = imageIter->second.ObjectInfo.Image.aspectMask;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = imageIter->second.ObjectInfo.Image.mipLevels;

        imageMemoryBarrier.srcQueueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;
        imageMemoryBarrier.dstQueueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;

        mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
    }

    // 6b) Transition all the buffers back to their previous state.
    for (TrimObjectInfoMap::iterator bufferIter = s_trimStateTrackerSnapshot.createdBuffers.begin(); bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end(); bufferIter++)
    {
        VkDevice device = bufferIter->second.belongsToDevice;
        VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        if (trim_bufferToStagedInfoMap.find(buffer) != trim_bufferToStagedInfoMap.end())
        {
            // if this buffer had a staging buffer, then we only need to do delete the staging objects
            Trim_StagingInfo staged = trim_bufferToStagedInfoMap[buffer];
            mdd(device)->devTable.DestroyBuffer(device, staged.buffer, NULL);
            mdd(device)->devTable.FreeMemory(device, staged.memory, NULL);
        }
        else
        {
            // Create a memory barrier to make it host readable
            VkBufferMemoryBarrier bufferMemoryBarrier;
            bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            bufferMemoryBarrier.pNext = NULL;
            bufferMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_READ_BIT;
            bufferMemoryBarrier.dstAccessMask = bufferIter->second.ObjectInfo.Buffer.accessFlags;
            bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufferMemoryBarrier.buffer = buffer;
            bufferMemoryBarrier.offset = bufferIter->second.ObjectInfo.Buffer.memoryOffset;
            bufferMemoryBarrier.size = bufferIter->second.ObjectInfo.Buffer.size;

            mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
        }
    }

    // 7) End the cmd buffers, submit, and wait for them
    for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
    {
        VkDevice device = static_cast<VkDevice>(deviceIter->first);

        // Find an existing command buffer
        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];

        mdd(device)->devTable.EndCommandBuffer(commandBuffer);

        // now submit the command buffer
        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = NULL;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = NULL;
        submitInfo.pWaitDstStageMask = NULL;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = NULL;

        VkQueue queue = VK_NULL_HANDLE;
        mdd(device)->devTable.GetDeviceQueue(device, 0, 0, &queue);

        VkFence fence = deviceToFenceMap[device];
        mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, fence);
        mdd(device)->devTable.WaitForFences(device, 1, &fence, VK_TRUE, 10 * 1000 * 1000);
    }

    // 8) destroy the command pools / command buffers and fences
    for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
    {
        VkDevice device = static_cast<VkDevice>(deviceIter->first);

        assert(deviceToCommandBufferMap.find(device) != deviceToCommandBufferMap.end());
        VkCommandBuffer commandBuffer = deviceToCommandBufferMap[device];
        mdd(device)->devTable.ResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        deviceToCommandBufferMap.erase(device);

        // command pools
        assert(deviceToCommandPoolMap.find(device) != deviceToCommandPoolMap.end());
        VkCommandPool commandPool = deviceToCommandPoolMap[device];
        mdd(device)->devTable.ResetCommandPool(device, commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
        mdd(device)->devTable.DestroyCommandPool(device, commandPool, NULL);
        deviceToCommandPoolMap.erase(device);

        // fences
        assert(deviceToFenceMap.find(device) != deviceToFenceMap.end());
        VkFence fence = deviceToFenceMap[device];
        mdd(device)->devTable.DestroyFence(device, fence, NULL);
        deviceToFenceMap.erase(device);
    }
    
    // Now: generate a vkMapMemory to recreate the persistently mapped buffers
    for (TrimObjectInfoMap::iterator iter = s_trimStateTrackerSnapshot.createdDeviceMemorys.begin(); iter != s_trimStateTrackerSnapshot.createdDeviceMemorys.end(); iter++)
    {
        // if the application still has this memory mapped, then we need to make sure the trim trace file leaves it mapped, so let's generate one more call to vkMapBuffer.
        bool bCurrentlyMapped = (iter->second.ObjectInfo.DeviceMemory.mappedAddress != NULL);
        if (bCurrentlyMapped)
        {
            VkDevice device = iter->second.belongsToDevice;
            VkDeviceMemory deviceMemory = static_cast<VkDeviceMemory>(iter->first);
            VkDeviceSize offset = 0;
            VkDeviceSize size = iter->second.ObjectInfo.DeviceMemory.size;
            VkMemoryMapFlags flags = 0;
            void* pData = iter->second.ObjectInfo.DeviceMemory.mappedAddress;

            vktrace_trace_packet_header* pPersistentlyMapMemory = trim_generate_vkMapMemory(
                false,
                device,
                deviceMemory,
                offset,
                size,
                flags,
                &pData
                );
            iter->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket = pPersistentlyMapMemory;
        }
    }

    if (instance != VK_NULL_HANDLE && mid(instance)->LunargDebugReportEnabled)
    {
        mid(instance)->instTable.DebugReportMessageEXT(instance, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT, (uint64_t)instance, __LINE__, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, "trace_trim", "Completed State Snapshot!");
    }

    vktrace_leave_critical_section(&trimStateTrackerLock);
}

void trim_add_Image_call(vktrace_trace_packet_header* pHeader)
{
    if (pHeader != NULL)
    {
        vktrace_enter_critical_section(&trimStateTrackerLock);
        trim_image_calls.push_back(pHeader);
        vktrace_leave_critical_section(&trimStateTrackerLock);
    }
}

#define TRIM_DEFINE_ADD_OBJECT_FUNC(type) \
Trim_ObjectInfo* trim_add_##type##_object(Vk##type var) { \
    vktrace_enter_critical_section(&trimStateTrackerLock); \
    Trim_ObjectInfo& info = s_trimGlobalStateTracker.created##type##s[var]; \
    memset(&info, 0, sizeof(Trim_ObjectInfo)); \
    info.vkObject = (uint64_t)var; \
    vktrace_leave_critical_section(&trimStateTrackerLock); \
    return &info; \
}

#define TRIM_DEFINE_REMOTE_OBJECT_FUNC(type) \
void trim_remove_##type##_object(Vk##type var) { \
    /* make sure the object actually existed before we attempt to remove it. This is for testing and thus only happens in debug builds. */ \
    vktrace_enter_critical_section(&trimStateTrackerLock); \
    assert(s_trimGlobalStateTracker.created##type##s.find(var) != s_trimGlobalStateTracker.created##type##s.end()); \
    s_trimGlobalStateTracker.created##type##s.erase(var); \
    vktrace_leave_critical_section(&trimStateTrackerLock); \
}

#define TRIM_DEFINE_GET_OBJECT_FUNC(type) \
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

#define TRIM_DEFINE_OBJECT_TRACKER_FUNCS(type) \
TRIM_DEFINE_ADD_OBJECT_FUNC(type) \
TRIM_DEFINE_REMOTE_OBJECT_FUNC(type) \
TRIM_DEFINE_GET_OBJECT_FUNC(type)

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

    // Get PhysicalDevice properties, etc
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdPhysicalDevices.begin(); obj != stateTracker.createdPhysicalDevices.end(); obj++)
    {
        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket != nullptr)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceSupportKHRPacket != nullptr)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceSupportKHRPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceSupportKHRPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket != nullptr)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket != nullptr)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket));
        }

        if (obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket != nullptr)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket));
        }
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
    }

    // Image

    // First fill in the memory that the image will be associated with
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdImages.begin(); obj != stateTracker.createdImages.end(); obj++)
    {
        // write map / unmap packets so the memory contents gets set on replay
        if (obj->second.ObjectInfo.Image.pMapMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pMapMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pMapMemoryPacket));
        }

        if (obj->second.ObjectInfo.Image.pUnmapMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.Image.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Image.pUnmapMemoryPacket));
        }
    }

#ifdef TRIM_USE_ORDERED_IMAGE_CREATION
    for (std::list<vktrace_trace_packet_header*>::iterator iter = trim_image_calls.begin(); iter != trim_image_calls.end(); iter++)
    {
        vktrace_write_trace_packet(*iter, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&(*iter));
    }
#else
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
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION

    for (TrimObjectInfoMap::iterator obj = stateTracker.createdImages.begin(); obj != stateTracker.createdImages.end(); obj++)
    {
        VkImageLayout initialLayout = obj->second.ObjectInfo.Image.initialLayout;
        VkImageLayout desiredLayout = obj->second.ObjectInfo.Image.mostRecentLayout;

        // Need to make sure images have the correct VkImageLayout.
        if (desiredLayout != initialLayout && obj->second.ObjectInfo.Image.bIsSwapchainImage == false) // TODO: find the solution for swapchain images
        {
            VkImage image = static_cast<VkImage>(obj->first);
            VkDevice device = obj->second.belongsToDevice;
            uint32_t mipLevels = obj->second.ObjectInfo.Image.mipLevels;
            uint32_t arrayLayers = obj->second.ObjectInfo.Image.arrayLayers;
            VkFormat format = obj->second.ObjectInfo.Image.format;
            VkSharingMode sharingMode = obj->second.ObjectInfo.Image.sharingMode;
            uint32_t queueFamilyIndex = obj->second.ObjectInfo.Image.queueFamilyIndex;
            uint32_t srcAccessMask = (initialLayout == VK_IMAGE_LAYOUT_PREINITIALIZED) ? VK_ACCESS_HOST_WRITE_BIT : 0;
            VkImageAspectFlags aspectMask = trim_getImageAspectFromFormat(format);

            uint32_t srcQueueFamilyIndex = queueFamilyIndex;
            uint32_t dstQueueFamilyIndex = queueFamilyIndex;

            if (sharingMode == VK_SHARING_MODE_CONCURRENT)
            {
                srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            }

            // This current approach is SUPER _NOT_ efficient.
            // We should create a command pool on each device only once.
            // We should only create one Command Buffer in each pool.
            // We should batch all the VkImageMemoryBarrier structs into a single call to VkCmdPipelineBarrier (per-device).
            // We should only end the command buffers once
            // We should only queue submit them once
            // We should only delete them once
            // Instead, this code is doing all of the above, for every single image transition.

            // This means:
            // 0) Need a VkCommandPool
            VkCommandPool tmpCommandPool;
            const VkCommandPoolCreateInfo cmdPoolCreateInfo = {
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                NULL,
                VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT or VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
                0 //demo->graphics_queue_node_index, TODO: Will need to search for this
            };
            vktrace_trace_packet_header* pCreateCommandPoolPacket = trim_generate_vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, &tmpCommandPool);
            vktrace_write_trace_packet(pCreateCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pCreateCommandPoolPacket);

            // 1) Create & begin a command buffer
            VkCommandBuffer tmpCommandBuffer;
            const VkCommandBufferAllocateInfo cmdBufferAllocInfo = {
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                NULL,
                tmpCommandPool,
                VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                1
            };
            vktrace_trace_packet_header* pAllocateCommandBufferPacket = trim_generate_vkAllocateCommandBuffers(true, device, &cmdBufferAllocInfo, &tmpCommandBuffer);
            vktrace_write_trace_packet(pAllocateCommandBufferPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pAllocateCommandBufferPacket);

            VkCommandBufferBeginInfo cmdBufferBeginInfo = {
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                NULL,
                0,
                NULL,
            };

            vktrace_trace_packet_header* pBeginCommandBufferPacket = trim_generate_vkBeginCommandBuffer(tmpCommandBuffer, &cmdBufferBeginInfo);
            vktrace_write_trace_packet(pBeginCommandBufferPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pBeginCommandBufferPacket);

            // 2) Make VkImageMemoryBarrier structs to change the image's layout
            VkImageMemoryBarrier imageMemoryBarrier = {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                NULL,
                srcAccessMask,
                0, // dstAccessMask, determined below
                initialLayout,
                desiredLayout,
                srcQueueFamilyIndex,
                dstQueueFamilyIndex,
                image,
                { aspectMask, 0, mipLevels, 0, arrayLayers } };

            if (desiredLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                /* Make sure anything that was copying from this image has completed */
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            }

            if (desiredLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
                imageMemoryBarrier.dstAccessMask =
                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            }

            if (desiredLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
                imageMemoryBarrier.dstAccessMask =
                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            }

            if (desiredLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                /* Make sure any Copy or CPU writes to image are flushed */
                imageMemoryBarrier.dstAccessMask =
                    VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
            }

            VkImageMemoryBarrier *pmemory_barrier = &imageMemoryBarrier;

            VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

            // 3) Use VkCmdPipelineBarrier to transition the images
            vktrace_trace_packet_header* pCmdPipelineBarrierPacket = trim_generate_vkCmdPipelineBarrier(tmpCommandBuffer, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, pmemory_barrier);
            vktrace_write_trace_packet(pCmdPipelineBarrierPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pCmdPipelineBarrierPacket);

            // 4) VkEndCommandBuffer()
            vktrace_trace_packet_header* pEndCommandBufferPacket = trim_generate_vkEndCommandBuffer(tmpCommandBuffer);
            vktrace_write_trace_packet(pEndCommandBufferPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pEndCommandBufferPacket);
            
            VkQueue trimQueue;
            uint32_t queueIndex = 0; // just using the first queue available, we don't yet verify if this even exists, just assuming.
            mdd(device)->devTable.GetDeviceQueue(device, queueFamilyIndex, queueIndex, &trimQueue);
            
            // 5) vkQueueSubmit()
            VkSubmitInfo submitInfo = {
                VK_STRUCTURE_TYPE_SUBMIT_INFO,
                NULL,
                0,
                NULL,
                NULL,
                1,
                &tmpCommandBuffer,
                0,
                NULL };
            VkFence nullFence = VK_NULL_HANDLE;
            vktrace_trace_packet_header* pQueueSubmitPacket = trim_generate_vkQueueSubmit(trimQueue, 1, &submitInfo, nullFence);
            vktrace_write_trace_packet(pQueueSubmitPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pQueueSubmitPacket);

            // 5a) vkWaitQueueIdle()
            vktrace_trace_packet_header* pQueueWaitIdlePacket = trim_generate_vkQueueWaitIdle(trimQueue);
            vktrace_write_trace_packet(pQueueWaitIdlePacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pQueueWaitIdlePacket);

            // 6) vkResetCommandPool() or vkFreeCommandBuffers()
            vktrace_trace_packet_header* pResetCommandPoolPacket = trim_generate_vkResetCommandPool(device, tmpCommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
            vktrace_write_trace_packet(pResetCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pResetCommandPoolPacket);

            // 7) vkDestroyCommandPool()
            vktrace_trace_packet_header* pDestroyCommandPoolPacket = trim_generate_vkDestroyCommandPool(device, tmpCommandPool, NULL);
            vktrace_write_trace_packet(pDestroyCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pDestroyCommandPoolPacket);
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
        VkBuffer buffer = (VkBuffer)obj->first;
        VkDevice device = obj->second.belongsToDevice;

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

        if (obj->second.ObjectInfo.Buffer.needsStagingBuffer)
        {
            Trim_StagingInfo stagingInfo = trim_bufferToStagedInfoMap[buffer];

            vktrace_trace_packet_header* pHeader = trim_generate_vkCreateBuffer(false, device, &stagingInfo.bufferCreateInfo, NULL, &stagingInfo.buffer);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            pHeader = trim_generate_vkGetBufferMemoryRequirements(false, device, stagingInfo.buffer, &stagingInfo.bufferMemoryRequirements);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            pHeader = trim_generate_vkAllocateMemory(false, device, &stagingInfo.memoryAllocationInfo, NULL, &stagingInfo.memory);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // bind staging buffer to staging memory
            pHeader = trim_generate_vkBindBufferMemory(false, device, stagingInfo.buffer, stagingInfo.memory, 0);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // here's where we map / unmap to insert data into the buffer
            {
                // write map / unmap packets so the memory contents gets set on replay
                if (obj->second.ObjectInfo.Buffer.pMapMemoryPacket != NULL)
                {
                    vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pMapMemoryPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Buffer.pMapMemoryPacket));
                }

                if (obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket != NULL)
                {
                    vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket));
                }
            }

            const VkCommandPoolCreateInfo cmdPoolCreateInfo = {
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                NULL,
                VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT or VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
                0 //demo->graphics_queue_node_index, TODO: Will need to search for this
            };
            vktrace_trace_packet_header* pCreateCommandPoolPacket = trim_generate_vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, &stagingInfo.commandPool);
            vktrace_write_trace_packet(pCreateCommandPoolPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pCreateCommandPoolPacket);

            // create command buffer
            VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
            commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandBufferAllocateInfo.pNext = NULL;
            commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandBufferAllocateInfo.commandBufferCount = 1;
            commandBufferAllocateInfo.commandPool = stagingInfo.commandPool;

            pHeader = trim_generate_vkAllocateCommandBuffers(false, device, &commandBufferAllocateInfo, &stagingInfo.commandBuffer);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            VkCommandBufferBeginInfo commandBufferBeginInfo;
            commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandBufferBeginInfo.pNext = NULL;
            commandBufferBeginInfo.flags = 0;
            commandBufferBeginInfo.pInheritanceInfo = NULL;

            pHeader = trim_generate_vkBeginCommandBuffer(stagingInfo.commandBuffer, &commandBufferBeginInfo);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // issue call to copy buffer
            pHeader = trim_generate_vkCmdCopyBuffer(false, device, stagingInfo.commandBuffer, stagingInfo.buffer, buffer, 1, &stagingInfo.copyRegion);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            pHeader = trim_generate_vkEndCommandBuffer(stagingInfo.commandBuffer);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // create fence
            VkFenceCreateInfo fenceCreateInfo = {};
            fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCreateInfo.pNext = NULL;
            fenceCreateInfo.flags = 0;

            pHeader = trim_generate_vkCreateFence(false, device, &fenceCreateInfo, NULL, &stagingInfo.fence);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // Queue submit the command buffer
            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pNext = NULL;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &stagingInfo.commandBuffer;
            submitInfo.pSignalSemaphores = NULL;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pWaitDstStageMask = NULL;
            submitInfo.pWaitSemaphores = NULL;
            submitInfo.waitSemaphoreCount = 0;

            pHeader = trim_generate_vkQueueSubmit(stagingInfo.queue, 1, &submitInfo, stagingInfo.fence);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // wait for queue to finish
            pHeader = trim_generate_vkWaitForFences(false, device, 1, &stagingInfo.fence, VK_TRUE, 10 * 1000 * 1000);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // delete staging buffer
            pHeader = trim_generate_vkDestroyBuffer(false, device, stagingInfo.buffer, NULL);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // free memory
            pHeader = trim_generate_vkFreeMemory(false, device, stagingInfo.memory, NULL);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);

            // delete command buffer
            pHeader = trim_generate_vkFreeCommandBuffers(false, device, stagingInfo.commandPool, 1, &stagingInfo.commandBuffer);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
        else
        {
            // write map / unmap packets so the memory contents gets set on replay
            if (obj->second.ObjectInfo.Buffer.pMapMemoryPacket != NULL)
            {
                vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pMapMemoryPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Buffer.pMapMemoryPacket));
            }

            if (obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket != NULL)
            {
                vktrace_write_trace_packet(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&(obj->second.ObjectInfo.Buffer.pUnmapMemoryPacket));
            }
        }
    }

    // DeviceMemory
    for (TrimObjectInfoMap::iterator obj = stateTracker.createdDeviceMemorys.begin(); obj != stateTracker.createdDeviceMemorys.end(); obj++)
    {
        if (obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket != NULL)
        {
            vktrace_write_trace_packet(obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(obj->second.ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket));
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
    vktrace_enter_critical_section(&trimCommandBufferPacketLock);
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
    vktrace_leave_critical_section(&trimCommandBufferPacketLock);

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

//===============================================
// Object tracking
//===============================================
void trim_add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader) {
    vktrace_enter_critical_section(&trimCommandBufferPacketLock);
    if (pHeader != NULL)
    {
        s_cmdBufferPackets[commandBuffer].push_back(pHeader);
    }
    vktrace_leave_critical_section(&trimCommandBufferPacketLock);
}

void trim_remove_CommandBuffer_calls(VkCommandBuffer commandBuffer)
{
    vktrace_enter_critical_section(&trimCommandBufferPacketLock);
    std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header*>>::iterator cmdBufferMap = s_cmdBufferPackets.find(commandBuffer);
    if (cmdBufferMap != s_cmdBufferPackets.end())
    {
        for (std::list<vktrace_trace_packet_header*>::iterator packet = cmdBufferMap->second.begin(); packet != cmdBufferMap->second.end(); packet++)
        {
            vktrace_trace_packet_header* pHeader = *packet;
            vktrace_delete_trace_packet(&pHeader);
        }
        cmdBufferMap->second.clear();

        s_cmdBufferPackets.erase(commandBuffer);
    }
    vktrace_leave_critical_section(&trimCommandBufferPacketLock);
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
    vktrace_enter_critical_section(&trimRecordedPacketLock);
    for (std::list<vktrace_trace_packet_header*>::iterator call = trim_recorded_packets.begin(); call != trim_recorded_packets.end(); call++)
    {
        vktrace_write_trace_packet(*call, vktrace_trace_get_trace_file());
    }
    vktrace_leave_critical_section(&trimRecordedPacketLock);
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
    vktrace_delete_critical_section(&trimCommandBufferPacketLock);
}
