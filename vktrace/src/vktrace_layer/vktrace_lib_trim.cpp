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

bool g_trimEnabled = false;
bool g_trimIsPreTrim = false;
bool g_trimIsInTrim = false;
bool g_trimIsPostTrim = false;
uint64_t g_trimFrameCounter = 0;
uint64_t g_trimStartFrame = 0;
uint64_t g_trimEndFrame = UINT64_MAX;

namespace trim
{
    // Tracks the existence of objects from the very beginning of the application
    static StateTracker s_trimGlobalStateTracker;

    // A snapshot of the GlobalStateTracker taken at the start of the trim frames.
    static StateTracker s_trimStateTrackerSnapshot;

    VKTRACE_CRITICAL_SECTION trimRecordedPacketLock;
    VKTRACE_CRITICAL_SECTION trimStateTrackerLock;
    VKTRACE_CRITICAL_SECTION trimCommandBufferPacketLock;

    // Typically an application will have one VkAllocationCallbacks struct and 
    // will pass in that same address as needed, so we'll keep a map to correlate
    // the supplied address to the AllocationCallbacks object.
    static std::unordered_map<const void*, VkAllocationCallbacks> s_trimAllocatorMap;

    // List of all the packets that have been recorded for the frames of interest.
    std::list<vktrace_trace_packet_header*> recorded_packets;

    std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header*>> s_cmdBufferPackets;

    // List of all packets used to create or delete images.
    // We need to recreate them in the same order to ensure they will have the same size requirements as they had a trace-time.
    std::list<vktrace_trace_packet_header*> image_calls;

    void initialize()
    {
        const char *trimFrames = vktrace_get_global_var("VKTRACE_TRIM_FRAMES");
        if (trimFrames != nullptr)
        {
            uint32_t numFrames = 0;
            if (sscanf(trimFrames, "%llu,%lu", &g_trimStartFrame, &numFrames) == 2)
            {
                g_trimEndFrame = g_trimStartFrame + numFrames;
            }
            else
            {
                int matches = sscanf(trimFrames, "%llu-%llu", &g_trimStartFrame, &g_trimEndFrame);
                assert(matches == 2);
            }

            // make sure the start/end frames are in expected order.
            if (g_trimStartFrame <= g_trimEndFrame)
            {
                g_trimEnabled = true;
                g_trimIsPreTrim = (g_trimStartFrame > 0);
                g_trimIsInTrim = (g_trimStartFrame == 0);
            }
        }

        if (g_trimEnabled)
        {
            vktrace_create_critical_section(&trimStateTrackerLock);
            vktrace_create_critical_section(&trimRecordedPacketLock);
            vktrace_create_critical_section(&trimCommandBufferPacketLock);
        }
    }

    void add_Allocator(const VkAllocationCallbacks* pAllocator)
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

    VkAllocationCallbacks* get_Allocator(const VkAllocationCallbacks* pAllocator)
    {
        if (pAllocator == NULL)
        {
            return NULL;
        }

        std::unordered_map<const void*, VkAllocationCallbacks>::iterator iter = s_trimAllocatorMap.find(pAllocator);
        assert(iter != s_trimAllocatorMap.end());
        VkAllocationCallbacks* pStoredAllocator = &(iter->second);
        return pStoredAllocator;
    }

    bool IsMemoryDeviceOnly(VkDevice device, VkDeviceMemory memory)
    {
        ObjectInfo* pInfo = get_DeviceMemory_objectInfo(memory);
        assert(pInfo != NULL);
        if (pInfo == NULL)
            return false;

        // Is device only if it has VK_MEMORY_PROPERTY_DEVICE_LOCAL and not VK_MEMORY_PROPERTY_HOST_VISIBLE
        bool isDeviceOnly = (pInfo->ObjectInfo.DeviceMemory.propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        return isDeviceOnly;
    }

    VkMemoryPropertyFlags LookUpMemoryProperties(VkDevice device, uint32_t memoryTypeIndex)
    {
        assert(memoryTypeIndex < VK_MAX_MEMORY_TYPES);
        if (memoryTypeIndex >= VK_MAX_MEMORY_TYPES)
            return false;

        VkPhysicalDevice physicalDevice = get_Device_objectInfo(device)->belongsToPhysicalDevice;
        assert(physicalDevice != VK_NULL_HANDLE);
        if (physicalDevice == VK_NULL_HANDLE)
            return false;

        ObjectInfo* pInfo = get_PhysicalDevice_objectInfo(physicalDevice);
        assert(pInfo != NULL);
        if (pInfo == NULL)
            return false;

        if (memoryTypeIndex >= pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypeCount)
            return false;

        return pInfo->ObjectInfo.PhysicalDevice.physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;
    }

    uint32_t FindMemoryTypeIndex(VkDevice device, uint32_t memoryTypeBits, VkMemoryPropertyFlags propertyFlags)
    {
        VkPhysicalDevice physicalDevice = get_Device_objectInfo(device)->belongsToPhysicalDevice;
        assert(physicalDevice != VK_NULL_HANDLE);
        if (physicalDevice == VK_NULL_HANDLE)
            return 0;

        ObjectInfo* pInfo = get_PhysicalDevice_objectInfo(physicalDevice);
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

    VkImageAspectFlags getImageAspectFromFormat(VkFormat format)
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

    // Information necessary to create the staged buffer and memory for DEVICE_LOCAL buffers.
    // This holds all the necessary structures so that we can fill them in once when we make
    // the call during trim snapshot, and then reuse them when generating the calls to recreate
    // the buffer into the trace file.
    struct StagingInfo
    {
        VkBuffer buffer = {};
        VkBufferCreateInfo bufferCreateInfo = {};
        VkMemoryRequirements bufferMemoryRequirements = {};

        VkDeviceMemory memory = {};
        VkMemoryAllocateInfo memoryAllocationInfo = {};

        // Region for copying buffers
        VkBufferCopy copyRegion = {};

        // Per-miplevel region for copying images
        std::vector<VkBufferImageCopy> imageCopyRegions;

        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        VkQueue queue = VK_NULL_HANDLE;
    };

    // map to associate trace-time buffer with a staged buffer
    static std::unordered_map<VkBuffer, StagingInfo> s_bufferToStagedInfoMap;

    // map to associate trace-time image with a staged buffer
    static std::unordered_map<VkImage, StagingInfo> s_imageToStagedInfoMap;

    //=============================================================================
    // Associates a Device to a trim-specific CommandPool
    //=============================================================================
    static std::unordered_map<VkDevice, VkCommandPool> s_deviceToCommandPoolMap;

    //=============================================================================
    // Find existing trim-specific CommandPool from the Device, or 
    // create a new one.
    //=============================================================================
    VkCommandPool getCommandPoolFromDevice(VkDevice device, uint32_t queueFamilyIndex = 0)
    {
        assert(device != VK_NULL_HANDLE);

        VkCommandPool commandPool = VK_NULL_HANDLE;
        if (s_deviceToCommandPoolMap.find(device) == s_deviceToCommandPoolMap.end())
        {
            // create a new command pool on the device
            VkCommandPoolCreateInfo cmdPoolCreateInfo;
            cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cmdPoolCreateInfo.pNext = NULL;
            cmdPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
            cmdPoolCreateInfo.flags = 0;

            VkResult result = mdd(device)->devTable.CreateCommandPool(device, &cmdPoolCreateInfo, NULL, &commandPool);
            assert(result == VK_SUCCESS);
            if (result == VK_SUCCESS)
            {
                s_deviceToCommandPoolMap[device] = commandPool;
            }
        }
        else
        {
            commandPool = s_deviceToCommandPoolMap[device];
        }
        return commandPool;
    }

    //=============================================================================
    // Associates a Device to a trim-specific CommandBuffer
    //=============================================================================
    static std::unordered_map<VkDevice, VkCommandBuffer> s_deviceToCommandBufferMap;

    //=============================================================================
    // Find existing trim-specific CommandBuffer from the Device, or 
    // create a new one.
    //=============================================================================
    VkCommandBuffer getCommandBufferFromDevice(VkDevice device, VkCommandPool commandPool = VK_NULL_HANDLE)
    {
        assert(device != VK_NULL_HANDLE);

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        if (s_deviceToCommandBufferMap.find(device) == s_deviceToCommandBufferMap.end())
        {
            if (commandPool == VK_NULL_HANDLE)
            {
                commandPool = getCommandPoolFromDevice(device, 0);
            }

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
                s_deviceToCommandBufferMap[device] = commandBuffer;
            }
        }
        else
        {
            commandBuffer = s_deviceToCommandBufferMap[device];
        }

        return commandBuffer;
    }

    //=============================================================================
    StagingInfo createStagingBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkDeviceSize size)
    {
        StagingInfo stagingInfo = {};

        stagingInfo.commandPool = commandPool;
        stagingInfo.commandBuffer = commandBuffer;
        mdd(device)->devTable.GetDeviceQueue(device, 0, 0, &stagingInfo.queue);

        stagingInfo.bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingInfo.bufferCreateInfo.pNext = NULL;
        stagingInfo.bufferCreateInfo.flags = 0;
        stagingInfo.bufferCreateInfo.size = size;
        stagingInfo.bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        stagingInfo.bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        stagingInfo.bufferCreateInfo.queueFamilyIndexCount = 0;
        stagingInfo.bufferCreateInfo.pQueueFamilyIndices = NULL;

        mdd(device)->devTable.CreateBuffer(device, &stagingInfo.bufferCreateInfo, NULL, &stagingInfo.buffer);

        mdd(device)->devTable.GetBufferMemoryRequirements(device, stagingInfo.buffer, &stagingInfo.bufferMemoryRequirements);

        stagingInfo.memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        stagingInfo.memoryAllocationInfo.pNext = NULL;
        stagingInfo.memoryAllocationInfo.allocationSize = stagingInfo.bufferMemoryRequirements.size;
        stagingInfo.memoryAllocationInfo.memoryTypeIndex = FindMemoryTypeIndex(device, stagingInfo.bufferMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        mdd(device)->devTable.AllocateMemory(device, &stagingInfo.memoryAllocationInfo, nullptr, &stagingInfo.memory);

        mdd(device)->devTable.BindBufferMemory(device, stagingInfo.buffer, stagingInfo.memory, 0);

        return stagingInfo;
    }

    //=============================================================================
    void generateCreateStagingBuffer(VkDevice device, StagingInfo stagingInfo)
    {
        vktrace_trace_packet_header* pHeader = generate::vkCreateBuffer(false, device, &stagingInfo.bufferCreateInfo, NULL, &stagingInfo.buffer);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);

        pHeader = generate::vkGetBufferMemoryRequirements(false, device, stagingInfo.buffer, &stagingInfo.bufferMemoryRequirements);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);

        pHeader = generate::vkAllocateMemory(false, device, &stagingInfo.memoryAllocationInfo, NULL, &stagingInfo.memory);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);

        // bind staging buffer to staging memory
        pHeader = generate::vkBindBufferMemory(false, device, stagingInfo.buffer, stagingInfo.memory, 0);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }

    //=============================================================================
    void generateDestroyStagingBuffer(VkDevice device, StagingInfo stagingInfo)
    {
        // delete staging buffer
        vktrace_trace_packet_header* pHeader = generate::vkDestroyBuffer(false, device, stagingInfo.buffer, NULL);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);

        // free memory
        pHeader = generate::vkFreeMemory(false, device, stagingInfo.memory, NULL);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    }

    //=============================================================================
    void transitionImage(VkDevice device, VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, uint32_t queueFamilyIndex, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t layerCount, uint32_t mipLevels)
    {
        // Create a pipeline barrier to make it host readable
        VkImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = NULL;
        imageMemoryBarrier.srcAccessMask = srcAccessMask;
        imageMemoryBarrier.dstAccessMask = dstAccessMask;
        imageMemoryBarrier.oldLayout = oldLayout;
        imageMemoryBarrier.newLayout = newLayout;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = layerCount;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
        imageMemoryBarrier.srcQueueFamilyIndex = queueFamilyIndex;
        imageMemoryBarrier.dstQueueFamilyIndex = queueFamilyIndex;

        mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
    };

    //=============================================================================
    void generateTransitionImage(VkDevice device, VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, uint32_t queueFamilyIndex, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t layerCount, uint32_t mipLevels)
    {
        // Create a pipeline barrier to make it host readable
        VkImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = NULL;
        imageMemoryBarrier.srcAccessMask = srcAccessMask;
        imageMemoryBarrier.dstAccessMask = dstAccessMask;
        imageMemoryBarrier.oldLayout = oldLayout;
        imageMemoryBarrier.newLayout = newLayout;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = layerCount;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
        imageMemoryBarrier.srcQueueFamilyIndex = queueFamilyIndex;
        imageMemoryBarrier.dstQueueFamilyIndex = queueFamilyIndex;

        vktrace_trace_packet_header* pHeader = generate::vkCmdPipelineBarrier(false, device, commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    };

    //=============================================================================
    void transitionBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkBuffer buffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDeviceSize offset, VkDeviceSize size)
    {
        // Create a pipeline barrier to make it host readable
        VkBufferMemoryBarrier bufferMemoryBarrier;
        bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        bufferMemoryBarrier.pNext = NULL;
        bufferMemoryBarrier.srcAccessMask = srcAccessMask;
        bufferMemoryBarrier.dstAccessMask = dstAccessMask;
        bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.buffer = buffer;
        bufferMemoryBarrier.offset = offset;
        bufferMemoryBarrier.size = size;

        mdd(device)->devTable.CmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
    };

    //=============================================================================
    void generateTransitionBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkBuffer buffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDeviceSize offset, VkDeviceSize size)
    {
        // Create a pipeline barrier to make it host readable
        VkBufferMemoryBarrier bufferMemoryBarrier;
        bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        bufferMemoryBarrier.pNext = NULL;
        bufferMemoryBarrier.srcAccessMask = srcAccessMask;
        bufferMemoryBarrier.dstAccessMask = dstAccessMask;
        bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.buffer = buffer;
        bufferMemoryBarrier.offset = offset;
        bufferMemoryBarrier.size = size;

        vktrace_trace_packet_header* pHeader = generate::vkCmdPipelineBarrier(false, device, commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
        vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
        vktrace_delete_trace_packet(&pHeader);
    };

    //=============================================================================
    // \param isMappedOffset If the buffer is currently mapped by the application, this is the offset that was used during that call to vkMapMemory.
    // \param pIsMappedAddress If the buffer is currently mapped by the application, this is the address of the buffer that was returned by vkMapMemory.
    void generateMapUnmap(bool makeCalls, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkDeviceSize isMappedOffset, void* pIsMappedAddress, vktrace_trace_packet_header** ppMapMemoryPacket, vktrace_trace_packet_header** ppUnmapMemoryPacket)
    {
        assert(ppMapMemoryPacket != NULL);
        assert(ppUnmapMemoryPacket != NULL);

        uint32_t flags = 0;

        vktrace_trace_packet_header* pMapMemory = generate::vkMapMemory(
            makeCalls,
            device,
            memory,
            offset,
            size,
            flags,
            &pIsMappedAddress
            );

        if (pIsMappedAddress == NULL)
        {
            // if the mapped address is still NULL, that means the map failed for some reason.
            // Since it failed, we'll just delete the packet and won't create an unmap for it.
            vktrace_delete_trace_packet(&pMapMemory);
        }
        else
        {
            *ppMapMemoryPacket = pMapMemory;

            // By creating the packet for UnmapMemory, we'll be adding the pData buffer to it, which inherently copies it.

            // need to adjust the pointer to ensure it points to the beginning of the image memory, which may NOT be
            // the same as the offset of the mapped address.
            void* bufferAddress = (BYTE*)pIsMappedAddress + (offset - isMappedOffset);

            // Actually unmap the memory if it wasn't already mapped by the application
            *ppUnmapMemoryPacket = generate::vkUnmapMemory(
                makeCalls,
                size,
                bufferAddress,
                device,
                memory);
        }
    }

    //=============================================================================
    // Use this to snapshot the global state tracker at the start of the trim frames.
    //=============================================================================
    void snapshot_state_tracker()
    {
        vktrace_enter_critical_section(&trimStateTrackerLock);
        s_trimStateTrackerSnapshot = s_trimGlobalStateTracker;

        VkInstance instance = VK_NULL_HANDLE;
        if (s_trimStateTrackerSnapshot.createdInstances.size() > 0)
        {
            instance = (VkInstance)s_trimStateTrackerSnapshot.createdInstances.begin()->first;
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
            VkCommandPool commandPool = getCommandPoolFromDevice(device);

            // Find or create an existing command buffer
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

            // Begin the command buffer
            VkCommandBufferBeginInfo commandBufferBeginInfo;
            commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandBufferBeginInfo.pNext = NULL;
            commandBufferBeginInfo.pInheritanceInfo = NULL;
            commandBufferBeginInfo.flags = 0;
            VkResult result = mdd(device)->devTable.BeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
            assert(result == VK_SUCCESS);
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

            uint32_t queueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;
            if (imageIter->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT)
            {
                queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            }

            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

            if (imageIter->second.ObjectInfo.Image.needsStagingBuffer)
            {
                VkCommandPool commandPool = getCommandPoolFromDevice(device);
                StagingInfo stagingInfo = createStagingBuffer(device, commandPool, commandBuffer, imageIter->second.ObjectInfo.Image.memorySize);
                
                // From Docs: srcImage must have a sample count equal to VK_SAMPLE_COUNT_1_BIT
                // From Docs: srcImage must have been created with VK_IMAGE_USAGE_TRANSFER_SRC_BIT usage flag

                // Copy from device_local image to host_visible buffer

                VkImageAspectFlags aspectMask = imageIter->second.ObjectInfo.Image.aspectMask;
                if (aspectMask == (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
                {
                    stagingInfo.imageCopyRegions.reserve(2);

                    // First depth, then stencil
                    VkImageSubresource sub;
                    sub.arrayLayer = 0;
                    sub.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                    sub.mipLevel = 0;
                    {
                        VkSubresourceLayout layout;
                        mdd(device)->devTable.GetImageSubresourceLayout(device, image, &sub, &layout);

                        VkBufferImageCopy copyRegion = {};
                        copyRegion.bufferRowLength = layout.rowPitch;
                        copyRegion.bufferImageHeight = layout.arrayPitch;
                        copyRegion.bufferOffset = layout.offset;
                        copyRegion.imageExtent.depth = 1;
                        copyRegion.imageExtent.width = imageIter->second.ObjectInfo.Image.extent.width;
                        copyRegion.imageExtent.height = imageIter->second.ObjectInfo.Image.extent.height;
                        copyRegion.imageOffset.x = 0;
                        copyRegion.imageOffset.y = 0;
                        copyRegion.imageOffset.z = 0;
                        copyRegion.imageSubresource.aspectMask = sub.aspectMask;
                        copyRegion.imageSubresource.baseArrayLayer = 0;
                        copyRegion.imageSubresource.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
                        copyRegion.imageSubresource.mipLevel = 0;

                        stagingInfo.imageCopyRegions.push_back(copyRegion);
                    }

                    sub.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
                    {
                        VkSubresourceLayout layout;
                        mdd(device)->devTable.GetImageSubresourceLayout(device, image, &sub, &layout);

                        VkBufferImageCopy copyRegion = {};
                        copyRegion.bufferRowLength = layout.rowPitch;
                        copyRegion.bufferImageHeight = layout.arrayPitch;
                        copyRegion.bufferOffset = layout.offset;
                        copyRegion.imageExtent.depth = 1;
                        copyRegion.imageExtent.width = imageIter->second.ObjectInfo.Image.extent.width;
                        copyRegion.imageExtent.height = imageIter->second.ObjectInfo.Image.extent.height;
                        copyRegion.imageOffset.x = 0;
                        copyRegion.imageOffset.y = 0;
                        copyRegion.imageOffset.z = 0;
                        copyRegion.imageSubresource.aspectMask = sub.aspectMask;
                        copyRegion.imageSubresource.baseArrayLayer = 0;
                        copyRegion.imageSubresource.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
                        copyRegion.imageSubresource.mipLevel = 0;

                        stagingInfo.imageCopyRegions.push_back(copyRegion);
                    }
                }
                else
                {
                    VkImageSubresource sub;
                    sub.arrayLayer = 0;
                    sub.aspectMask = aspectMask;
                    sub.mipLevel = 0;

                    // need to make a VkBufferImageCopy for each mip level
                    stagingInfo.imageCopyRegions.reserve(imageIter->second.ObjectInfo.Image.mipLevels);
                    for (uint32_t i = 0; i < imageIter->second.ObjectInfo.Image.mipLevels; i++)
                    {
                        VkSubresourceLayout lay;
                        sub.mipLevel = i;
                        mdd(device)->devTable.GetImageSubresourceLayout(device, image, &sub, &lay);

                        VkBufferImageCopy copyRegion = {};
                        copyRegion.bufferRowLength = 0; //< tightly packed texels
                        copyRegion.bufferImageHeight = 0; //< tightly packed texels
                        copyRegion.bufferOffset = lay.offset;
                        copyRegion.imageExtent.depth = 1;
                        copyRegion.imageExtent.width = (imageIter->second.ObjectInfo.Image.extent.width >> i);
                        copyRegion.imageExtent.height = (imageIter->second.ObjectInfo.Image.extent.height >> i);
                        copyRegion.imageOffset.x = 0;
                        copyRegion.imageOffset.y = 0;
                        copyRegion.imageOffset.z = 0;
                        copyRegion.imageSubresource.aspectMask = aspectMask;
                        copyRegion.imageSubresource.baseArrayLayer = 0;
                        copyRegion.imageSubresource.layerCount = imageIter->second.ObjectInfo.Image.arrayLayers;
                        copyRegion.imageSubresource.mipLevel = i;

                        stagingInfo.imageCopyRegions.push_back(copyRegion);
                    }
                }

                // From docs: srcImageLayout must specify the layout of the image subresources of srcImage specified in pRegions at the time this command is executed on a VkDevice
                // From docs: srcImageLayout must be either of VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL or VK_IMAGE_LAYOUT_GENERAL
                VkImageLayout srcImageLayout = imageIter->second.ObjectInfo.Image.mostRecentLayout;

                // Transition the image so that it's in an optimal transfer source layout.
                transitionImage(device, 
                    commandBuffer, 
                    image, 
                    imageIter->second.ObjectInfo.Image.accessFlags, 
                    imageIter->second.ObjectInfo.Image.accessFlags, 
                    queueFamilyIndex,
                    srcImageLayout,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    aspectMask,
                    imageIter->second.ObjectInfo.Image.arrayLayers, 
                    imageIter->second.ObjectInfo.Image.mipLevels);

                mdd(device)->devTable.CmdCopyImageToBuffer(
                    commandBuffer,
                    image,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    stagingInfo.buffer,
                    stagingInfo.imageCopyRegions.size(),
                    stagingInfo.imageCopyRegions.data());

                // save the staging info for later
                s_imageToStagedInfoMap[image] = stagingInfo;

                // now that the image data is in a host-readable buffer
                // transition image back to it's previous layout
                transitionImage(device,
                    commandBuffer,
                    image,
                    imageIter->second.ObjectInfo.Image.accessFlags,
                    imageIter->second.ObjectInfo.Image.accessFlags,
                    queueFamilyIndex,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    srcImageLayout,
                    aspectMask,
                    imageIter->second.ObjectInfo.Image.arrayLayers,
                    imageIter->second.ObjectInfo.Image.mipLevels);
            }
            else
            {
                // Create a pipeline barrier to make it host readable
                transitionImage(device,
                    commandBuffer,
                    image,
                    imageIter->second.ObjectInfo.Image.accessFlags,
                    VK_ACCESS_HOST_READ_BIT,
                    queueFamilyIndex,
                    imageIter->second.ObjectInfo.Image.mostRecentLayout,
                    imageIter->second.ObjectInfo.Image.mostRecentLayout,
                    imageIter->second.ObjectInfo.Image.aspectMask,
                    imageIter->second.ObjectInfo.Image.arrayLayers,
                    imageIter->second.ObjectInfo.Image.mipLevels);
            }
        }

        // 2b) Transition all buffers into host-readable state.
        for (TrimObjectInfoMap::iterator bufferIter = s_trimStateTrackerSnapshot.createdBuffers.begin(); bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end(); bufferIter++)
        {
            VkDevice device = bufferIter->second.belongsToDevice;
            VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);

            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

            // If the buffer needs a staging buffer, it's because it's on DEVICE_LOCAL memory that is not HOST_VISIBLE.
            // So we have to create another buffer and memory that IS HOST_VISIBLE so that we can copy the data
            // from the DEVICE_LOCAL memory into HOST_VISIBLE memory, then map / unmap the HOST_VISIBLE memory object.
            // The staging info is kept so that we can generate similar calls in the trace file in order to recreate
            // the DEVICE_LOCAL buffer.
            if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer)
            {
                VkCommandPool commandPool = getCommandPoolFromDevice(device);
                StagingInfo stagingInfo = createStagingBuffer(device, commandPool, commandBuffer, bufferIter->second.ObjectInfo.Buffer.size);

                // Copy from device_local buffer to host_visible buffer
                stagingInfo.copyRegion.srcOffset = 0;
                stagingInfo.copyRegion.dstOffset = 0;
                stagingInfo.copyRegion.size = bufferIter->second.ObjectInfo.Buffer.size;

                mdd(device)->devTable.CmdCopyBuffer(
                    commandBuffer,
                    buffer,
                    stagingInfo.buffer,
                    1,
                    &stagingInfo.copyRegion);

                // save the staging info for later
                s_bufferToStagedInfoMap[buffer] = stagingInfo;
            }
            else
            {
                transitionBuffer(device,
                    commandBuffer,
                    buffer,
                    bufferIter->second.ObjectInfo.Buffer.accessFlags,
                    VK_ACCESS_HOST_READ_BIT,
                    0,
                    bufferIter->second.ObjectInfo.Buffer.size);
            }
        }

        // 3) End the cmd buffers and submit them on the right queue.
        for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
        {
            VkDevice device = static_cast<VkDevice>(deviceIter->first);
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

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
            mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            VkResult waitResult = mdd(device)->devTable.QueueWaitIdle(queue);
            assert(waitResult == VK_SUCCESS);
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

            if (imageIter->second.ObjectInfo.Image.needsStagingBuffer)
            {
                // Note that the staged memory object won't be in the state tracker, so we want to swap out the buffer and memory
                // that will be mapped / unmapped.
                StagingInfo staged = s_imageToStagedInfoMap[image];
                VkBuffer buffer = staged.buffer;
                memory = staged.memory;
                offset = 0;

                void* mappedAddress = NULL;

                if (size != 0)
                {
                    generateMapUnmap(true, device, memory, offset, size, 0, mappedAddress, &imageIter->second.ObjectInfo.Image.pMapMemoryPacket, &imageIter->second.ObjectInfo.Image.pUnmapMemoryPacket);
                }
            }
            else
            {
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

                        generateMapUnmap(!bAlreadyMapped, device, memory, offset, size, 0, mappedAddress, &imageIter->second.ObjectInfo.Image.pMapMemoryPacket, &imageIter->second.ObjectInfo.Image.pUnmapMemoryPacket);
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

            if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer)
            {
                // Note that the staged memory object won't be in the state tracker, so we want to swap out the buffer and memory
                // that will be mapped / unmapped.
                StagingInfo staged = s_bufferToStagedInfoMap[buffer];
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

                generateMapUnmap(!bAlreadyMapped, device, memory, offset, size, 0, mappedAddress, &bufferIter->second.ObjectInfo.Buffer.pMapMemoryPacket, &bufferIter->second.ObjectInfo.Buffer.pUnmapMemoryPacket);
            }
        }

        // 5) Begin the cmd buffers again.
        for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
        {
            VkDevice device = static_cast<VkDevice>(deviceIter->first);
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

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

            // only need to restore the images that did NOT need a staging buffer
            if (imageIter->second.ObjectInfo.Image.needsStagingBuffer)
            {
                // delete the staging objects
                StagingInfo staged = s_imageToStagedInfoMap[image];
                mdd(device)->devTable.DestroyBuffer(device, staged.buffer, NULL);
                mdd(device)->devTable.FreeMemory(device, staged.memory, NULL);
            }
            else
            {
                VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

                uint32_t queueFamilyIndex = imageIter->second.ObjectInfo.Image.queueFamilyIndex;
                if (imageIter->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT)
                {
                    queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                }

                transitionImage(device,
                    commandBuffer,
                    image,
                    VK_ACCESS_HOST_READ_BIT,
                    imageIter->second.ObjectInfo.Image.accessFlags,
                    queueFamilyIndex,
                    imageIter->second.ObjectInfo.Image.mostRecentLayout,
                    imageIter->second.ObjectInfo.Image.mostRecentLayout,
                    imageIter->second.ObjectInfo.Image.aspectMask,
                    imageIter->second.ObjectInfo.Image.arrayLayers,
                    imageIter->second.ObjectInfo.Image.mipLevels);
            }
        }

        // 6b) Transition all the buffers back to their previous state.
        for (TrimObjectInfoMap::iterator bufferIter = s_trimStateTrackerSnapshot.createdBuffers.begin(); bufferIter != s_trimStateTrackerSnapshot.createdBuffers.end(); bufferIter++)
        {
            VkDevice device = bufferIter->second.belongsToDevice;
            VkBuffer buffer = static_cast<VkBuffer>(bufferIter->first);
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

            if (bufferIter->second.ObjectInfo.Buffer.needsStagingBuffer)
            {
                // if this buffer had a staging buffer, then we only need to do delete the staging objects
                StagingInfo staged = s_bufferToStagedInfoMap[buffer];
                mdd(device)->devTable.DestroyBuffer(device, staged.buffer, NULL);
                mdd(device)->devTable.FreeMemory(device, staged.memory, NULL);
            }
            else
            {
                transitionBuffer(device, 
                    commandBuffer, 
                    buffer,
                    VK_ACCESS_HOST_READ_BIT,
                    bufferIter->second.ObjectInfo.Buffer.accessFlags,
                    0,
                    bufferIter->second.ObjectInfo.Buffer.size);
            }
        }

        // 7) End the cmd buffers, submit, and wait for them
        for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
        {
            VkDevice device = static_cast<VkDevice>(deviceIter->first);
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

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

            mdd(device)->devTable.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            VkResult waitResult = mdd(device)->devTable.QueueWaitIdle(queue);
            assert(waitResult == VK_SUCCESS);
        }

        // 8) Destroy the command pools / command buffers and fences
        for (TrimObjectInfoMap::iterator deviceIter = s_trimStateTrackerSnapshot.createdDevices.begin(); deviceIter != s_trimStateTrackerSnapshot.createdDevices.end(); deviceIter++)
        {
            VkDevice device = static_cast<VkDevice>(deviceIter->first);
            VkCommandBuffer commandBuffer = getCommandBufferFromDevice(device);

            mdd(device)->devTable.ResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
            s_deviceToCommandBufferMap.erase(device);

            // command pools
            VkCommandPool commandPool = getCommandPoolFromDevice(device);
            mdd(device)->devTable.ResetCommandPool(device, commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
            mdd(device)->devTable.DestroyCommandPool(device, commandPool, NULL);
            s_deviceToCommandPoolMap.erase(device);
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

                vktrace_trace_packet_header* pPersistentlyMapMemory = generate::vkMapMemory(
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

        vktrace_leave_critical_section(&trimStateTrackerLock);
    }

    void add_Image_call(vktrace_trace_packet_header* pHeader)
    {
        if (pHeader != NULL)
        {
            vktrace_enter_critical_section(&trimStateTrackerLock);
            image_calls.push_back(pHeader);
            vktrace_leave_critical_section(&trimStateTrackerLock);
        }
    }

    #define TRIM_DEFINE_ADD_OBJECT_FUNC(type) \
    ObjectInfo* add_##type##_object(Vk##type var) { \
        vktrace_enter_critical_section(&trimStateTrackerLock); \
        ObjectInfo& info = s_trimGlobalStateTracker.created##type##s[var]; \
        memset(&info, 0, sizeof(ObjectInfo)); \
        info.vkObject = (uint64_t)var; \
        vktrace_leave_critical_section(&trimStateTrackerLock); \
        return &info; \
    }

    #define TRIM_DEFINE_REMOVE_OBJECT_FUNC(type) \
    void remove_##type##_objectInfo(Vk##type var) { \
        vktrace_enter_critical_section(&trimStateTrackerLock); \
        assert(s_trimGlobalStateTracker.created##type##s.find(var) != s_trimGlobalStateTracker.created##type##s.end()); \
        s_trimGlobalStateTracker.created##type##s.erase(var); \
        vktrace_leave_critical_section(&trimStateTrackerLock); \
    }

    void delete_packet(vktrace_trace_packet_header** ppHeader)
    {
        if (ppHeader != nullptr && *ppHeader != nullptr)
        {
            vktrace_delete_trace_packet(ppHeader);
        }
    }

    #define TRIM_DEFINE_GET_OBJECT_FUNC(type) \
    ObjectInfo* get_##type##_objectInfo(Vk##type var) { \
        vktrace_enter_critical_section(&trimStateTrackerLock); \
        TrimObjectInfoMap::iterator iter  = s_trimGlobalStateTracker.created##type##s.find(var); \
        ObjectInfo* pResult = NULL; \
        if (iter != s_trimGlobalStateTracker.created##type##s.end()) { \
            pResult = &(iter->second); \
        } \
        vktrace_leave_critical_section(&trimStateTrackerLock); \
        return pResult;\
    }

    #define TRIM_DEFINE_OBJECT_TRACKER_FUNCS(type) \
    TRIM_DEFINE_ADD_OBJECT_FUNC(type) \
    TRIM_DEFINE_REMOVE_OBJECT_FUNC(type) \
    TRIM_DEFINE_GET_OBJECT_FUNC(type)

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
    
    void remove_Instance_object(VkInstance var)
    {
        ObjectInfo* pInfo = get_Instance_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Instance.pCreatePacket);
            delete_packet(&pInfo->ObjectInfo.Instance.pEnumeratePhysicalDevicesCountPacket);
            delete_packet(&pInfo->ObjectInfo.Instance.pEnumeratePhysicalDevicesPacket);
        }
        remove_Instance_objectInfo(var);
    }

    void remove_PhysicalDevice_object(VkPhysicalDevice var)
    {
        ObjectInfo* pInfo = get_PhysicalDevice_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket);
            delete_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceSurfaceSupportKHRPacket);
            delete_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket);
            delete_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesCountPacket);
            delete_packet(&pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceQueueFamilyPropertiesPacket);
        }
        remove_PhysicalDevice_objectInfo(var);
    }

    void remove_Device_object(VkDevice var)
    {
        ObjectInfo* pInfo = get_Device_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Device.pCreatePacket);
        }
        remove_Device_objectInfo(var);
    }

    void remove_SurfaceKHR_object(VkSurfaceKHR var)
    {
        ObjectInfo* pInfo = get_SurfaceKHR_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.SurfaceKHR.pCreatePacket);
        }
        remove_SurfaceKHR_objectInfo(var);
    }

    void remove_Queue_object(VkQueue var)
    {
        ObjectInfo* pInfo = get_Queue_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Queue.pCreatePacket);
        }
        remove_Queue_objectInfo(var);
    }

    void remove_CommandPool_object(VkCommandPool var)
    {
        ObjectInfo* pInfo = get_CommandPool_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.CommandPool.pCreatePacket);
        }
        remove_CommandPool_objectInfo(var);
    }

    void remove_SwapchainKHR_object(VkSwapchainKHR var)
    {
        ObjectInfo* pInfo = get_SwapchainKHR_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.SwapchainKHR.pCreatePacket);
            delete_packet(&pInfo->ObjectInfo.SwapchainKHR.pGetSwapchainImageCountPacket);
            delete_packet(&pInfo->ObjectInfo.SwapchainKHR.pGetSwapchainImagesPacket);
        }
        remove_SwapchainKHR_objectInfo(var);
    }

    void remove_CommandBuffer_object(VkCommandBuffer var)
    {
        remove_CommandBuffer_objectInfo(var);
    }

    void remove_DeviceMemory_object(VkDeviceMemory var)
    {
        ObjectInfo* pInfo = get_DeviceMemory_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.DeviceMemory.pCreatePacket);
            delete_packet(&pInfo->ObjectInfo.DeviceMemory.pMapMemoryPacket);
            delete_packet(&pInfo->ObjectInfo.DeviceMemory.pUnmapMemoryPacket);
            delete_packet(&pInfo->ObjectInfo.DeviceMemory.pPersistentlyMapMemoryPacket);
        }
        remove_DeviceMemory_objectInfo(var);
    }

    void remove_Image_object(VkImage var)
    {
        ObjectInfo* pInfo = get_Image_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Image.pCreatePacket);
#if !TRIM_USE_ORDERED_IMAGE_CREATION
            delete_packet(&pInfo->ObjectInfo.Image.pGetImageMemoryRequirementsPacket);
            delete_packet(&pInfo->ObjectInfo.Image.pBindImageMemoryPacket);
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION
            delete_packet(&pInfo->ObjectInfo.Image.pMapMemoryPacket);
            delete_packet(&pInfo->ObjectInfo.Image.pUnmapMemoryPacket);
    }
        remove_Image_objectInfo(var);
    }

    void remove_ImageView_object(VkImageView var)
    {
        ObjectInfo* pInfo = get_ImageView_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.ImageView.pCreatePacket);
        }
        remove_ImageView_objectInfo(var);
    }

    void remove_Buffer_object(VkBuffer var)
    {
        ObjectInfo* pInfo = get_Buffer_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Buffer.pCreatePacket);
            delete_packet(&pInfo->ObjectInfo.Buffer.pBindBufferMemoryPacket);
            delete_packet(&pInfo->ObjectInfo.Buffer.pMapMemoryPacket);
            delete_packet(&pInfo->ObjectInfo.Buffer.pUnmapMemoryPacket);
        }
        remove_Buffer_objectInfo(var);
    }

    void remove_BufferView_object(VkBufferView var)
    {
        ObjectInfo* pInfo = get_BufferView_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.BufferView.pCreatePacket);
        }
        remove_BufferView_objectInfo(var);
    }

    void remove_Sampler_object(VkSampler var)
    {
        ObjectInfo* pInfo = get_Sampler_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Sampler.pCreatePacket);
        }
        remove_Sampler_objectInfo(var);
    }

    void remove_DescriptorSetLayout_object(VkDescriptorSetLayout var)
    {
        ObjectInfo* pInfo = get_DescriptorSetLayout_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.DescriptorSetLayout.pCreatePacket);

            if (pInfo->ObjectInfo.DescriptorSetLayout.pBindings != nullptr)
            {
                delete[] pInfo->ObjectInfo.DescriptorSetLayout.pBindings;
                pInfo->ObjectInfo.DescriptorSetLayout.pBindings = nullptr;
            }
        }
        remove_DescriptorSetLayout_objectInfo(var);
    }

    void remove_PipelineLayout_object(VkPipelineLayout var)
    {
        ObjectInfo* pInfo = get_PipelineLayout_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.PipelineLayout.pCreatePacket);
        }
        remove_PipelineLayout_objectInfo(var);
    }

    void remove_RenderPass_object(VkRenderPass var)
    {
        ObjectInfo* pInfo = get_RenderPass_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.RenderPass.pCreatePacket);
        }
        remove_RenderPass_objectInfo(var);
    }

    void remove_ShaderModule_object(VkShaderModule var)
    {
        ObjectInfo* pInfo = get_ShaderModule_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.ShaderModule.pCreatePacket);
        }
        remove_ShaderModule_objectInfo(var);
    }

    void remove_PipelineCache_object(VkPipelineCache var)
    {
        ObjectInfo* pInfo = get_PipelineCache_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.PipelineCache.pCreatePacket);
        }
        remove_PipelineCache_objectInfo(var);
    }

    void remove_Pipeline_object(VkPipeline var)
    {
        ObjectInfo* pInfo = get_Pipeline_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Pipeline.pCreatePacket);
        }
        remove_Pipeline_objectInfo(var);
    }

    void remove_DescriptorPool_object(VkDescriptorPool var)
    {
        ObjectInfo* pInfo = get_DescriptorPool_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.DescriptorPool.pCreatePacket);
        }
        remove_DescriptorPool_objectInfo(var);
    }

    void remove_DescriptorSet_object(VkDescriptorSet var)
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
        remove_DescriptorSet_objectInfo(var);
    }

    void remove_Framebuffer_object(VkFramebuffer var)
    {
        ObjectInfo* pInfo = get_Framebuffer_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Framebuffer.pCreatePacket);
        }
        remove_Framebuffer_objectInfo(var);
    }

    void remove_Semaphore_object(VkSemaphore var)
    {
        ObjectInfo* pInfo = get_Semaphore_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Semaphore.pCreatePacket);
        }
        remove_Semaphore_objectInfo(var);
    }

    void remove_Fence_object(VkFence var)
    {
        remove_Fence_objectInfo(var);
    }

    void remove_Event_object(VkEvent var)
    {
        ObjectInfo* pInfo = get_Event_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.Event.pCreatePacket);
        }
        remove_Event_objectInfo(var);
    }

    void remove_QueryPool_object(VkQueryPool var)
    {
        ObjectInfo* pInfo = get_QueryPool_objectInfo(var);
        if (pInfo != nullptr)
        {
            delete_packet(&pInfo->ObjectInfo.QueryPool.pCreatePacket);
        }
        remove_QueryPool_objectInfo(var);
    }

#define TRIM_MARK_OBJECT_REFERENCE(type) \
    void mark_##type##_reference(Vk##type var) { \
        vktrace_enter_critical_section(&trimStateTrackerLock);\
        TrimObjectInfoMap::iterator iter = s_trimStateTrackerSnapshot.created##type##s.find(var);\
        if (iter != s_trimStateTrackerSnapshot.created##type##s.end()) {\
            ObjectInfo* info = &iter->second;\
            if (info != nullptr) {\
                info->bReferencedInTrim = true; \
            }\
        }\
        vktrace_leave_critical_section(&trimStateTrackerLock);\
    }

#define TRIM_MARK_OBJECT_REFERENCE_WITH_DEVICE_DEPENDENCY(type) \
    void mark_##type##_reference(Vk##type var) { \
        vktrace_enter_critical_section(&trimStateTrackerLock);\
        TrimObjectInfoMap::iterator iter = s_trimStateTrackerSnapshot.created##type##s.find(var);\
        if (iter != s_trimStateTrackerSnapshot.created##type##s.end()) {\
            ObjectInfo* info = &iter->second;\
            if (info != nullptr) {\
                info->bReferencedInTrim = true; \
                mark_Device_reference((VkDevice)info->belongsToDevice); \
            }\
        }\
        vktrace_leave_critical_section(&trimStateTrackerLock);\
    }

    TRIM_MARK_OBJECT_REFERENCE(Instance);
    TRIM_MARK_OBJECT_REFERENCE(PhysicalDevice);
    TRIM_MARK_OBJECT_REFERENCE(Device);

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

    //=============================================================================
    // Recreate all objects
    //=============================================================================
    void write_all_referenced_object_calls()
    {
        // write the referenced objects from the snapshot
        StateTracker& stateTracker = s_trimStateTrackerSnapshot;

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

                    vktrace_trace_packet_header* pHeader = generate::vkAllocateCommandBuffers(false, poolObj->second.belongsToDevice, &allocateInfo, pCommandBuffers);
                    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&(pHeader));

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
            // only update the memory for images that don't need a staging buffer
            if (obj->second.ObjectInfo.Image.needsStagingBuffer == false)
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
        }

    #ifdef TRIM_USE_ORDERED_IMAGE_CREATION
        for (std::list<vktrace_trace_packet_header*>::iterator iter = image_calls.begin(); iter != image_calls.end(); iter++)
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
            VkImage image = static_cast<VkImage>(obj->first);
            VkDevice device = obj->second.belongsToDevice;

            if (obj->second.ObjectInfo.Image.mostRecentLayout == VK_IMAGE_LAYOUT_UNDEFINED)
            {
                // If the current layout is UNDEFINED, that means the app hasn't used it yet, or it doesn't care what is currently
                // in the image and so it will be discarded when the app uses it next. As such, there's no point in us trying to
                // recreate this image.
                continue;
            }

            uint32_t queueFamilyIndex = obj->second.ObjectInfo.Image.queueFamilyIndex;
            if (obj->second.ObjectInfo.Image.sharingMode == VK_SHARING_MODE_CONCURRENT)
            {
                queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            }

            if (obj->second.ObjectInfo.Image.needsStagingBuffer)
            {
                // make a staging buffer and copy the data into the image (similar to what we do for buffers)
                StagingInfo stagingInfo = s_imageToStagedInfoMap[image];
                stagingInfo.bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

                // generate packets needed to create a staging buffer
                generateCreateStagingBuffer(device, stagingInfo);

                // here's where we map / unmap to insert data into the buffer
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

                const VkCommandPoolCreateInfo cmdPoolCreateInfo = {
                    VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                    NULL,
                    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT or VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
                    0 //demo->graphics_queue_node_index, TODO: Will need to search for this
                };
                vktrace_trace_packet_header* pCreateCommandPoolPacket = generate::vkCreateCommandPool(true, device, &cmdPoolCreateInfo, NULL, &stagingInfo.commandPool);
                vktrace_write_trace_packet(pCreateCommandPoolPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pCreateCommandPoolPacket);

                // create command buffer
                VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
                commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                commandBufferAllocateInfo.pNext = NULL;
                commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                commandBufferAllocateInfo.commandBufferCount = 1;
                commandBufferAllocateInfo.commandPool = stagingInfo.commandPool;

                vktrace_trace_packet_header* pHeader = generate::vkAllocateCommandBuffers(false, device, &commandBufferAllocateInfo, &stagingInfo.commandBuffer);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                VkCommandBufferBeginInfo commandBufferBeginInfo;
                commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                commandBufferBeginInfo.pNext = NULL;
                commandBufferBeginInfo.flags = 0;
                commandBufferBeginInfo.pInheritanceInfo = NULL;

                pHeader = generate::vkBeginCommandBuffer(false, device, stagingInfo.commandBuffer, &commandBufferBeginInfo);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
                
                // Transition image to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                generateTransitionImage(device,
                    stagingInfo.commandBuffer,
                    image,
                    0,
                    VK_ACCESS_TRANSFER_WRITE_BIT,
                    queueFamilyIndex,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    obj->second.ObjectInfo.Image.aspectMask,
                    obj->second.ObjectInfo.Image.arrayLayers,
                    obj->second.ObjectInfo.Image.mipLevels);

                // issue call to copy buffer
                pHeader = generate::vkCmdCopyBufferToImage(false, device, stagingInfo.commandBuffer, stagingInfo.buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(stagingInfo.imageCopyRegions.size()), stagingInfo.imageCopyRegions.data());
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // transition image to final layout
                generateTransitionImage(device,
                    stagingInfo.commandBuffer,
                    image,
                    VK_ACCESS_TRANSFER_WRITE_BIT,
                    obj->second.ObjectInfo.Image.accessFlags,
                    queueFamilyIndex,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    obj->second.ObjectInfo.Image.mostRecentLayout,
                    obj->second.ObjectInfo.Image.aspectMask,
                    obj->second.ObjectInfo.Image.arrayLayers,
                    obj->second.ObjectInfo.Image.mipLevels);

                pHeader = generate::vkEndCommandBuffer(false, device, stagingInfo.commandBuffer);
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

                pHeader = generate::vkQueueSubmit(false, device, stagingInfo.queue, 1, &submitInfo, VK_NULL_HANDLE);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // wait for queue to finish
                pHeader = generate::vkQueueWaitIdle(false, device, stagingInfo.queue);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // delete staging buffer
                generateDestroyStagingBuffer(device, stagingInfo);

                // delete command buffer
                pHeader = generate::vkFreeCommandBuffers(false, device, stagingInfo.commandPool, 1, &stagingInfo.commandBuffer);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }
            else
            {
                VkImageLayout initialLayout = obj->second.ObjectInfo.Image.initialLayout;
                VkImageLayout desiredLayout = obj->second.ObjectInfo.Image.mostRecentLayout;

                // Need to make sure images have the correct VkImageLayout.
                if (obj->second.ObjectInfo.Image.bIsSwapchainImage == false)
                {
                    VkDevice device = obj->second.belongsToDevice;
                    uint32_t mipLevels = obj->second.ObjectInfo.Image.mipLevels;
                    uint32_t arrayLayers = obj->second.ObjectInfo.Image.arrayLayers;
                    VkFormat format = obj->second.ObjectInfo.Image.format;
                    VkSharingMode sharingMode = obj->second.ObjectInfo.Image.sharingMode;
                    uint32_t srcAccessMask = (initialLayout == VK_IMAGE_LAYOUT_PREINITIALIZED) ? VK_ACCESS_HOST_WRITE_BIT : 0;
                    VkImageAspectFlags aspectMask = getImageAspectFromFormat(format);

                    uint32_t srcQueueFamilyIndex = queueFamilyIndex;
                    uint32_t dstQueueFamilyIndex = queueFamilyIndex;

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
                    vktrace_trace_packet_header* pCreateCommandPoolPacket = generate::vkCreateCommandPool(true, device, &cmdPoolCreateInfo, NULL, &tmpCommandPool);
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
                    vktrace_trace_packet_header* pAllocateCommandBufferPacket = generate::vkAllocateCommandBuffers(true, device, &cmdBufferAllocInfo, &tmpCommandBuffer);
                    vktrace_write_trace_packet(pAllocateCommandBufferPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pAllocateCommandBufferPacket);

                    VkCommandBufferBeginInfo cmdBufferBeginInfo = {
                        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                        NULL,
                        0,
                        NULL,
                    };

                    vktrace_trace_packet_header* pBeginCommandBufferPacket = generate::vkBeginCommandBuffer(false, device, tmpCommandBuffer, &cmdBufferBeginInfo);
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
                    vktrace_trace_packet_header* pCmdPipelineBarrierPacket = generate::vkCmdPipelineBarrier(false, device, tmpCommandBuffer, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, pmemory_barrier);
                    vktrace_write_trace_packet(pCmdPipelineBarrierPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pCmdPipelineBarrierPacket);

                    // 4) VkEndCommandBuffer()
                    vktrace_trace_packet_header* pEndCommandBufferPacket = generate::vkEndCommandBuffer(false, device, tmpCommandBuffer);
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
                    vktrace_trace_packet_header* pQueueSubmitPacket = generate::vkQueueSubmit(false, device, trimQueue, 1, &submitInfo, nullFence);
                    vktrace_write_trace_packet(pQueueSubmitPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pQueueSubmitPacket);

                    // 5a) vkWaitQueueIdle()
                    vktrace_trace_packet_header* pQueueWaitIdlePacket = generate::vkQueueWaitIdle(false, device, trimQueue);
                    vktrace_write_trace_packet(pQueueWaitIdlePacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pQueueWaitIdlePacket);

                    // 6) vkResetCommandPool() or vkFreeCommandBuffers()
                    vktrace_trace_packet_header* pResetCommandPoolPacket = generate::vkResetCommandPool(true, device, tmpCommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
                    vktrace_write_trace_packet(pResetCommandPoolPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pResetCommandPoolPacket);

                    // 7) vkDestroyCommandPool()
                    vktrace_trace_packet_header* pDestroyCommandPoolPacket = generate::vkDestroyCommandPool(true, device, tmpCommandPool, NULL);
                    vktrace_write_trace_packet(pDestroyCommandPoolPacket, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pDestroyCommandPoolPacket);
                }
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
                StagingInfo stagingInfo = s_bufferToStagedInfoMap[buffer];
                stagingInfo.bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

                // Generate packets to create the staging buffer
                generateCreateStagingBuffer(device, stagingInfo);

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
                vktrace_trace_packet_header* pCreateCommandPoolPacket = generate::vkCreateCommandPool(true, device, &cmdPoolCreateInfo, NULL, &stagingInfo.commandPool);
                vktrace_write_trace_packet(pCreateCommandPoolPacket, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pCreateCommandPoolPacket);

                // create command buffer
                VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
                commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                commandBufferAllocateInfo.pNext = NULL;
                commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                commandBufferAllocateInfo.commandBufferCount = 1;
                commandBufferAllocateInfo.commandPool = stagingInfo.commandPool;

                vktrace_trace_packet_header* pHeader = generate::vkAllocateCommandBuffers(false, device, &commandBufferAllocateInfo, &stagingInfo.commandBuffer);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                VkCommandBufferBeginInfo commandBufferBeginInfo;
                commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                commandBufferBeginInfo.pNext = NULL;
                commandBufferBeginInfo.flags = 0;
                commandBufferBeginInfo.pInheritanceInfo = NULL;

                pHeader = generate::vkBeginCommandBuffer(false, device, stagingInfo.commandBuffer, &commandBufferBeginInfo);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
                
                // Transition Buffer to be writeable
                generateTransitionBuffer(device,
                    stagingInfo.commandBuffer,
                    buffer,
                    0,
                    VK_ACCESS_TRANSFER_WRITE_BIT,
                    0,
                    obj->second.ObjectInfo.Buffer.size);

                // issue call to copy buffer
                stagingInfo.copyRegion.dstOffset = 0;
                stagingInfo.copyRegion.srcOffset = 0;
                pHeader = generate::vkCmdCopyBuffer(false, device, stagingInfo.commandBuffer, stagingInfo.buffer, buffer, 1, &stagingInfo.copyRegion);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // transition buffer to final access mask
                generateTransitionBuffer(device,
                    stagingInfo.commandBuffer,
                    buffer,
                    VK_ACCESS_TRANSFER_WRITE_BIT,
                    obj->second.ObjectInfo.Buffer.accessFlags,
                    0,
                    obj->second.ObjectInfo.Buffer.size);

                pHeader = generate::vkEndCommandBuffer(false, device, stagingInfo.commandBuffer);
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

                pHeader = generate::vkQueueSubmit(false, device, stagingInfo.queue, 1, &submitInfo, VK_NULL_HANDLE);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // wait for queue to finish
                pHeader = generate::vkQueueWaitIdle(false, device, stagingInfo.queue);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);

                // delete staging buffer
                generateDestroyStagingBuffer(device, stagingInfo);

                // delete command buffer
                pHeader = generate::vkFreeCommandBuffers(false, device, stagingInfo.commandPool, 1, &stagingInfo.commandBuffer);
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
                uint64_t vktraceStartTime = vktrace_get_time();
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

                VkDevice device = poolObj->second.belongsToDevice;

                vktrace_trace_packet_header* pHeader = generate::vkAllocateDescriptorSets(false, device, &allocateInfo, pDescriptorSets);
                pHeader->vktrace_begin_time = vktraceStartTime;
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&(pHeader));

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

                    vktrace_trace_packet_header* pHeader = generate::vkUpdateDescriptorSets(false, setObj->second.belongsToDevice, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
                    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pHeader);

                    // need to clean up the structs created by trim
                    for (uint32_t i = 0; i < descriptorWriteCount; i++)
                    {
                        if (pDescriptorWrites[i].pImageInfo != nullptr)
                        {
                            free((void*)pDescriptorWrites[i].pImageInfo);
                            pDescriptorWrites[i].pImageInfo = nullptr;
                        }
                        if (pDescriptorWrites[i].pTexelBufferView != nullptr)
                        {
                            free((void*)pDescriptorWrites[i].pTexelBufferView);
                            pDescriptorWrites[i].pTexelBufferView = nullptr;
                        }
                        if (pDescriptorWrites[i].pBufferInfo != nullptr)
                        {
                            free((void*)pDescriptorWrites[i].pBufferInfo);
                            pDescriptorWrites[i].pBufferInfo = nullptr;
                        }
                    }

                    // special to trim code
                    free(pDescriptorWrites);
                    free(pDescriptorCopies);
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
            VkDevice device = obj->second.belongsToDevice;
            VkFence fence = static_cast<VkFence>(obj->first);
            VkAllocationCallbacks* pAllocator = get_Allocator(obj->second.ObjectInfo.Fence.pAllocator);

            VkFenceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            createInfo.flags = (obj->second.ObjectInfo.Fence.signaled) ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

            vktrace_trace_packet_header* pCreateFence = generate::vkCreateFence(false, device, &createInfo, pAllocator, &fence);
            vktrace_write_trace_packet(pCreateFence, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(pCreateFence));
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

            if (cmdBuffer->second.bReferencedInTrim)
            {
                // write the packets
                for (std::list<vktrace_trace_packet_header*>::iterator packet = packets.begin(); packet != packets.end(); packet++)
                {
                    vktrace_trace_packet_header* pHeader = *packet;
                    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pHeader);
                }
            }
            else
            {
                // just delete the packets
                for (std::list<vktrace_trace_packet_header*>::iterator packet = packets.begin(); packet != packets.end(); packet++)
                {
                    vktrace_trace_packet_header* pHeader = *packet;
                    vktrace_delete_trace_packet(&pHeader);
                }
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

                vktrace_trace_packet_header* pHeader = generate::vkQueueSubmit(false, obj->second.belongsToDevice, queue, 1, &submit_info, VK_NULL_HANDLE);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }
        }

        VKTRACE_DELETE(pSignalSemaphores);
    }

    //===============================================
    // Object tracking
    //===============================================
    void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader) {
        vktrace_enter_critical_section(&trimCommandBufferPacketLock);
        if (pHeader != NULL)
        {
            s_cmdBufferPackets[commandBuffer].push_back(pHeader);
        }
        vktrace_leave_critical_section(&trimCommandBufferPacketLock);
    }

    void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer)
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

    void reset_DescriptorPool(VkDescriptorPool descriptorPool)
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
    // Packet Recording for frames of interest
    //===============================================
    void add_recorded_packet(vktrace_trace_packet_header* pHeader)
    {
        vktrace_enter_critical_section(&trimRecordedPacketLock);
        recorded_packets.push_back(pHeader);
        vktrace_leave_critical_section(&trimRecordedPacketLock);
    }

    void write_recorded_packets()
    {
        vktrace_enter_critical_section(&trimRecordedPacketLock);
        for (std::list<vktrace_trace_packet_header*>::iterator call = recorded_packets.begin(); call != recorded_packets.end(); call++)
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
    void write_destroy_packets()
    {
        // Make sure all queues have completed before trying to delete anything
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdQueues.begin(); obj != s_trimGlobalStateTracker.createdQueues.end(); obj++)
        {
            VkQueue queue = static_cast<VkQueue>(obj->first);
            VkDevice device = obj->second.belongsToDevice;
            vktrace_trace_packet_header* pHeader = generate::vkQueueWaitIdle(false, device, queue);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // QueryPool
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdQueryPools.begin(); obj != s_trimGlobalStateTracker.createdQueryPools.end(); obj++)
        {
            VkQueryPool queryPool = static_cast<VkQueryPool>(obj->first);
            VkAllocationCallbacks* pAllocator = get_Allocator(obj->second.ObjectInfo.QueryPool.pAllocator);

            vktrace_trace_packet_header* pHeader = generate::vkDestroyQueryPool(false, obj->second.belongsToDevice, queryPool, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Event
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdEvents.begin(); obj != s_trimGlobalStateTracker.createdEvents.end(); obj++)
        {
            VkEvent event = static_cast<VkEvent>(obj->first);
            VkAllocationCallbacks* pAllocator = get_Allocator(obj->second.ObjectInfo.Event.pAllocator);

            vktrace_trace_packet_header* pHeader = generate::vkDestroyEvent(false, obj->second.belongsToDevice, event, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Fence
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdFences.begin(); obj != s_trimGlobalStateTracker.createdFences.end(); obj++)
        {
            VkFence fence = static_cast<VkFence>(obj->first);
            VkAllocationCallbacks* pAllocator = get_Allocator(obj->second.ObjectInfo.Fence.pAllocator);

            vktrace_trace_packet_header* pHeader = generate::vkDestroyFence(false, obj->second.belongsToDevice, fence, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Semaphore
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSemaphores.begin(); obj != s_trimGlobalStateTracker.createdSemaphores.end(); obj++)
        {
            VkSemaphore semaphore = static_cast<VkSemaphore>(obj->first);
            VkAllocationCallbacks* pAllocator = get_Allocator(obj->second.ObjectInfo.Semaphore.pAllocator);

            vktrace_trace_packet_header* pHeader = generate::vkDestroySemaphore(false, obj->second.belongsToDevice, semaphore, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Framebuffer
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdFramebuffers.begin(); obj != s_trimGlobalStateTracker.createdFramebuffers.end(); obj++)
        {
            VkFramebuffer framebuffer = static_cast<VkFramebuffer>(obj->first);
            VkAllocationCallbacks* pAllocator = get_Allocator(obj->second.ObjectInfo.Framebuffer.pAllocator);

            vktrace_trace_packet_header* pHeader = generate::vkDestroyFramebuffer(false, obj->second.belongsToDevice, framebuffer, pAllocator);
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // DescriptorPool
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDescriptorPools.begin(); obj != s_trimGlobalStateTracker.createdDescriptorPools.end(); obj++)
        {
            // Free the associated DescriptorSets
            VkDescriptorPool descriptorPool = static_cast<VkDescriptorPool>(obj->first);

            // We can always call ResetDescriptorPool, but can only use FreeDescriptorSets if the pool was created with VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT.
            {
                vktrace_trace_packet_header* pHeader = generate::vkResetDescriptorPool(false, obj->second.belongsToDevice, descriptorPool, 0);
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }

            // Now destroy the DescriptorPool
            {
                vktrace_trace_packet_header* pHeader = generate::vkDestroyDescriptorPool(false, obj->second.belongsToDevice, descriptorPool, get_Allocator(obj->second.ObjectInfo.DescriptorPool.pAllocator));
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }
        }

        // Pipeline
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdPipelines.begin(); obj != s_trimGlobalStateTracker.createdPipelines.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyPipeline(false, obj->second.belongsToDevice, (VkPipeline)obj->first, get_Allocator(obj->second.ObjectInfo.Pipeline.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // PipelineCache
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdPipelineCaches.begin(); obj != s_trimGlobalStateTracker.createdPipelineCaches.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyPipelineCache(false, obj->second.belongsToDevice, (VkPipelineCache)obj->first, get_Allocator(obj->second.ObjectInfo.PipelineCache.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // ShaderModule
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdShaderModules.begin(); obj != s_trimGlobalStateTracker.createdShaderModules.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyShaderModule(false, obj->second.belongsToDevice, (VkShaderModule)obj->first, get_Allocator(obj->second.ObjectInfo.ShaderModule.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // RenderPass
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdRenderPasss.begin(); obj != s_trimGlobalStateTracker.createdRenderPasss.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyRenderPass(false, obj->second.belongsToDevice, (VkRenderPass)obj->first, get_Allocator(obj->second.ObjectInfo.RenderPass.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // PipelineLayout
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdPipelineLayouts.begin(); obj != s_trimGlobalStateTracker.createdPipelineLayouts.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyPipelineLayout(false, obj->second.belongsToDevice, (VkPipelineLayout)obj->first, get_Allocator(obj->second.ObjectInfo.PipelineLayout.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // DescriptorSetLayout
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDescriptorSetLayouts.begin(); obj != s_trimGlobalStateTracker.createdDescriptorSetLayouts.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyDescriptorSetLayout(false, obj->second.belongsToDevice, (VkDescriptorSetLayout)obj->first, get_Allocator(obj->second.ObjectInfo.DescriptorSetLayout.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Sampler
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSamplers.begin(); obj != s_trimGlobalStateTracker.createdSamplers.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroySampler(false, obj->second.belongsToDevice, (VkSampler)obj->first, get_Allocator(obj->second.ObjectInfo.Sampler.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Buffer
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdBuffers.begin(); obj != s_trimGlobalStateTracker.createdBuffers.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyBuffer(false, obj->second.belongsToDevice, (VkBuffer)obj->first, get_Allocator(obj->second.ObjectInfo.Buffer.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // BufferView
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdBufferViews.begin(); obj != s_trimGlobalStateTracker.createdBufferViews.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyBufferView(false, obj->second.belongsToDevice, (VkBufferView)obj->first, get_Allocator(obj->second.ObjectInfo.BufferView.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Image
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdImages.begin(); obj != s_trimGlobalStateTracker.createdImages.end(); obj++)
        {
            if (obj->second.ObjectInfo.Image.bIsSwapchainImage == false)
            {
                vktrace_trace_packet_header* pHeader = generate::vkDestroyImage(false, obj->second.belongsToDevice, (VkImage)obj->first, get_Allocator(obj->second.ObjectInfo.Image.pAllocator));
                vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                vktrace_delete_trace_packet(&pHeader);
            }
        }

        // ImageView
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdImageViews.begin(); obj != s_trimGlobalStateTracker.createdImageViews.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyImageView(false, obj->second.belongsToDevice, (VkImageView)obj->first, get_Allocator(obj->second.ObjectInfo.ImageView.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // DeviceMemory
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDeviceMemorys.begin(); obj != s_trimGlobalStateTracker.createdDeviceMemorys.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkFreeMemory(false, obj->second.belongsToDevice, (VkDeviceMemory)obj->first, get_Allocator(obj->second.ObjectInfo.DeviceMemory.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // SwapchainKHR
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSwapchainKHRs.begin(); obj != s_trimGlobalStateTracker.createdSwapchainKHRs.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroySwapchainKHR(false, obj->second.belongsToDevice, (VkSwapchainKHR)obj->first, get_Allocator(obj->second.ObjectInfo.SwapchainKHR.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
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
                    uint64_t vktrace_begin_time = vktrace_get_time();

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

                    vktrace_trace_packet_header* pHeader = generate::vkFreeCommandBuffers(false, obj->second.belongsToDevice, (VkCommandPool)obj->first, commandBufferCount, pCommandBuffers);
                    pHeader->vktrace_begin_time = vktrace_begin_time;
                    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
                    vktrace_delete_trace_packet(&pHeader);

                    delete[] pCommandBuffers;
                }
            }

            // Destroy the CommandPool
            vktrace_trace_packet_header* pHeader = generate::vkDestroyCommandPool(false, obj->second.belongsToDevice, (VkCommandPool)obj->first, get_Allocator(obj->second.ObjectInfo.CommandPool.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Device
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdDevices.begin(); obj != s_trimGlobalStateTracker.createdDevices.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyDevice(false, (VkDevice)obj->first, get_Allocator(obj->second.ObjectInfo.Device.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // SurfaceKHR
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdSurfaceKHRs.begin(); obj != s_trimGlobalStateTracker.createdSurfaceKHRs.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroySurfaceKHR(false, obj->second.belongsToInstance, (VkSurfaceKHR)obj->first, get_Allocator(obj->second.ObjectInfo.SurfaceKHR.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }

        // Instance
        for (TrimObjectInfoMap::iterator obj = s_trimGlobalStateTracker.createdInstances.begin(); obj != s_trimGlobalStateTracker.createdInstances.end(); obj++)
        {
            vktrace_trace_packet_header* pHeader = generate::vkDestroyInstance(false, (VkInstance)obj->first, get_Allocator(obj->second.ObjectInfo.Instance.pAllocator));
            vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&pHeader);
        }
    }

    //===============================================
    // Delete all the created packets
    //===============================================
    #define TRIM_DELETE_ALL_PACKETS(type) \
        s_trimGlobalStateTracker.created##type##s.clear();

    void delete_all_packets()
    {
        // delete all recorded packets
        for (std::list<vktrace_trace_packet_header*>::iterator call = recorded_packets.begin(); call != recorded_packets.end(); call++)
        {
            vktrace_delete_trace_packet(&(*call));
        }
        recorded_packets.clear();

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
} // namespace trim
