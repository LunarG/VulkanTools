/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once

#include <unordered_map>
#include "vktrace_trace_packet_utils.h"

// Create / Destroy all image resources in the order performed by the application. 
// Enabling this as a pre-processor macro so that we can compare performance and file size costs.
// TRUE: Needed on AMD hardware
// FALSE: Use normal object tracking to create only the necessary resources
#define TRIM_USE_ORDERED_IMAGE_CREATION true

namespace trim
{
    vktrace_trace_packet_header* copy_packet(vktrace_trace_packet_header* pHeader);
    void delete_packet(vktrace_trace_packet_header** ppHeader);

    // RenderPasses and VkCmdPipelineBarrier can transition images 
    // to different layouts, but the change doesn't happen when the API call is made
    // but rather when the command buffer is executed. Cache these transitions so
    // that they can be applied when needed.
    struct ImageTransition
    {
        VkImage image;
        VkImageLayout initialLayout;
        VkImageLayout finalLayout;
        VkAccessFlags srcAccessMask;
        VkAccessFlags dstAccessMask;
    };

    extern VKTRACE_CRITICAL_SECTION trimTransitionMapLock;

    static std::unordered_map<VkCommandBuffer, std::list<ImageTransition>> m_cmdBufferToImageTransitionsMap;
    
    void AddImageTransition(VkCommandBuffer commandBuffer, ImageTransition transition);

    void ClearImageTransitions(VkCommandBuffer commandBuffer);

    // VkCmdPipelineBarrier can transition memory to a different accessMask, but 
    // the change doesn't happen when the API call is made but rather when the 
    // command buffer is executed. Cache these transitions so that they can be 
    // applied when needed.
    struct BufferTransition
    {
        VkBuffer buffer;
        VkAccessFlags srcAccessMask;
        VkAccessFlags dstAccessMask;
    };

    static std::unordered_map<VkCommandBuffer, std::list<BufferTransition>> m_cmdBufferToBufferTransitionsMap;

    void AddBufferTransition(VkCommandBuffer commandBuffer, BufferTransition transition);
    void ClearBufferTransitions(VkCommandBuffer commandBuffer);

    //-------------------------------------------------------------------------
    // Some of the items in this struct are based on what is tracked in the 'VkLayer_object_tracker' (struct _OBJTRACK_NODE).
    //-------------------------------------------------------------------------
    typedef struct _Trim_ObjectInfo
    {
        uint64_t vkObject;                               // object handle
        bool bReferencedInTrim;                          // True if the object was referenced during the trim frames
        VkInstance belongsToInstance;                    // owning Instance
        VkPhysicalDevice belongsToPhysicalDevice;        // owning PhysicalDevice
        VkDevice belongsToDevice;                        // owning Device
        union _ObjectInfo {                              // additional object-specific information
            struct _Instance {              // VkInstance
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                vktrace_trace_packet_header* pEnumeratePhysicalDevicesCountPacket;
                vktrace_trace_packet_header* pEnumeratePhysicalDevicesPacket;
            } Instance;
            struct _PhysicalDevice {        // VkPhysicalDevice
                vktrace_trace_packet_header* pGetPhysicalDeviceSurfaceCapabilitiesKHRPacket;
                vktrace_trace_packet_header* pGetPhysicalDeviceSurfaceSupportKHRPacket;
                vktrace_trace_packet_header* pGetPhysicalDeviceMemoryPropertiesPacket;
                vktrace_trace_packet_header* pGetPhysicalDeviceQueueFamilyPropertiesCountPacket;
                vktrace_trace_packet_header* pGetPhysicalDeviceQueueFamilyPropertiesPacket;
                VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
            } PhysicalDevice;
            struct _SurfaceKHR {            // VkSurfaceKHR
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } SurfaceKHR;
            struct _Device {                // VkDevice
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } Device;
            struct _Queue {                 // VkQueue
                vktrace_trace_packet_header* pCreatePacket;
            } Queue;
            struct _CommandPool {           // VkCommandPool
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                uint32_t numCommandBuffersAllocated[VK_COMMAND_BUFFER_LEVEL_RANGE_SIZE];
            } CommandPool;
            struct _SwapchainKHR {           // VkSwapchainKHR
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                vktrace_trace_packet_header* pGetSwapchainImageCountPacket;
                vktrace_trace_packet_header* pGetSwapchainImagesPacket;
            } SwapchainKHR;
            struct _CommandBuffer {         // VkCommandBuffer
                VkCommandPool commandPool;
                VkCommandBufferLevel level;
                VkRenderPass activeRenderPass;
            } CommandBuffer;
            struct _DeviceMemory {          // VkDeviceMemory
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                VkDeviceSize size;
                uint32_t memoryTypeIndex;
                VkMemoryPropertyFlags propertyFlags;
                void* mappedAddress;
                VkDeviceSize mappedOffset;
                VkDeviceSize mappedSize;
                vktrace_trace_packet_header* pMapMemoryPacket;
                vktrace_trace_packet_header* pUnmapMemoryPacket;
                vktrace_trace_packet_header* pPersistentlyMapMemoryPacket;
            } DeviceMemory;
            struct _Image {                 // VkImage
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                bool bIsSwapchainImage;
#if !TRIM_USE_ORDERED_IMAGE_CREATION
                vktrace_trace_packet_header* pGetImageMemoryRequirementsPacket;
#endif //!TRIM_USE_ORDERED_IMAGE_CREATION
                vktrace_trace_packet_header* pBindImageMemoryPacket;
                vktrace_trace_packet_header* pMapMemoryPacket;
                vktrace_trace_packet_header* pUnmapMemoryPacket;
                VkDeviceMemory memory;
                VkDeviceSize memoryOffset;
                VkDeviceSize memorySize;
                VkFormat format;
                VkExtent3D extent;
                uint32_t mipLevels;
                uint32_t arrayLayers;
                VkSharingMode sharingMode;
                uint32_t queueFamilyIndex;
                VkAccessFlags accessFlags;
                VkImageAspectFlags aspectMask;
                VkImageLayout initialLayout;
                VkImageLayout mostRecentLayout;
                bool needsStagingBuffer;
            } Image;
            struct _ImageView {             // VkImageView
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                VkImage image;
            } ImageView;
            struct _Buffer {                // VkBuffer
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                vktrace_trace_packet_header* pBindBufferMemoryPacket;
                vktrace_trace_packet_header* pMapMemoryPacket;
                vktrace_trace_packet_header* pUnmapMemoryPacket;
                uint32_t queueFamilyIndex;
                VkAccessFlags accessFlags;
                VkDeviceMemory memory;
                VkDeviceSize memoryOffset;
                VkDeviceSize size;
                bool needsStagingBuffer;
            } Buffer;
            struct _BufferView {            // VkBufferView
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } BufferView;
            struct _Sampler {               // VkSampler
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } Sampler;
            struct _DescriptorSetLayout {   // VkDescriptorSetLayout
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                uint32_t numImages;
                uint32_t numBuffers;
                uint32_t numTexelBufferViews;
                uint32_t bindingCount;
                VkDescriptorSetLayoutBinding* pBindings;
            } DescriptorSetLayout;
            struct _PipelineLayout {        // VkPipelineLayout
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                uint32_t descriptorSetLayoutCount;
                VkDescriptorSetLayout* pDescriptorSetLayouts;
            } PipelineLayout;
            struct _RenderPass {            // VkRenderPass
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                uint32_t attachmentCount;
                ImageTransition* pAttachments;
            } RenderPass;
            struct _ShaderModule {          // VkShaderModule
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } ShaderModule;
            struct _PipelineCache {         // VkPipelineCache
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } PipelineCache;
            struct _Pipeline {              // VkPipeline
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
                VkComputePipelineCreateInfo computePipelineCreateInfo;
            } Pipeline;
            struct _DescriptorPool {        // VkDescriptorPool
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                VkDescriptorPoolCreateFlags createFlags;
                uint32_t maxSets;
                uint32_t numSets;
            } DescriptorPool;
            struct _DescriptorSet {         // VkDescriptorSet
                VkDescriptorPool descriptorPool;
                VkDescriptorSetLayout layout;
                uint32_t numBindings; // this is the number of elements allocated in each of the two arrays below.
                uint32_t writeDescriptorCount; // this is the number of descriptor sets that will need a write update.
                VkWriteDescriptorSet* pWriteDescriptorSets;
                uint32_t copyDescriptorCount;  // this is the number of descriptor sets that will need a copy update.
                VkCopyDescriptorSet* pCopyDescriptorSets;
            } DescriptorSet;
            struct _Framebuffer {           // VkFramebuffer
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                uint32_t attachmentCount;
                VkImageView* pAttachments;
            } Framebuffer;
            struct _Semaphore {           // VkSemaphore
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                VkQueue signaledOnQueue;
            } Semaphore;
            struct _Fence {           // VkFence
                const VkAllocationCallbacks* pAllocator;
                bool signaled;
            } Fence;
            struct _Event {           // VkEvent
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
            } Event;
            struct _QueryPool {           // VkQueryPool
                vktrace_trace_packet_header* pCreatePacket;
                const VkAllocationCallbacks* pAllocator;
                VkCommandBuffer commandBuffer;
                uint32_t size;
                bool* pResultsAvailable;
                VkQueryType queryType;
            } QueryPool;
        } ObjectInfo;
    } ObjectInfo;

    typedef std::unordered_map<void*, ObjectInfo> TrimObjectInfoMap;

    //-------------------------------------------------------------------------
    class StateTracker
    {
    public:
        StateTracker();
        StateTracker(const StateTracker& other);

        ~StateTracker();

        void clear();

        void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader);
        void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer);

#if TRIM_USE_ORDERED_IMAGE_CREATION
        void add_Image_call(vktrace_trace_packet_header* pHeader);
#endif //TRIM_USE_ORDERED_IMAGE_CREATION

        StateTracker& operator= (const StateTracker& other);

        ObjectInfo* add_Instance(VkInstance var);
        ObjectInfo* add_PhysicalDevice(VkPhysicalDevice var);
        ObjectInfo* add_Device(VkDevice var);
        ObjectInfo* add_SurfaceKHR(VkSurfaceKHR var);
        ObjectInfo* add_CommandPool(VkCommandPool var);
        ObjectInfo* add_CommandBuffer(VkCommandBuffer var);
        ObjectInfo* add_DescriptorPool(VkDescriptorPool var);
        ObjectInfo* add_RenderPass(VkRenderPass var);
        ObjectInfo* add_PipelineCache(VkPipelineCache var);
        ObjectInfo* add_Pipeline(VkPipeline var);
        ObjectInfo* add_Queue(VkQueue var);
        ObjectInfo* add_Semaphore(VkSemaphore var);
        ObjectInfo* add_DeviceMemory(VkDeviceMemory var);
        ObjectInfo* add_Fence(VkFence var);
        ObjectInfo* add_SwapchainKHR(VkSwapchainKHR var);
        ObjectInfo* add_Image(VkImage var);
        ObjectInfo* add_ImageView(VkImageView var);
        ObjectInfo* add_Buffer(VkBuffer var);
        ObjectInfo* add_BufferView(VkBufferView var);
        ObjectInfo* add_Framebuffer(VkFramebuffer var);
        ObjectInfo* add_Event(VkEvent var);
        ObjectInfo* add_QueryPool(VkQueryPool var);
        ObjectInfo* add_ShaderModule(VkShaderModule var);
        ObjectInfo* add_PipelineLayout(VkPipelineLayout var);
        ObjectInfo* add_Sampler(VkSampler var);
        ObjectInfo* add_DescriptorSetLayout(VkDescriptorSetLayout var);
        ObjectInfo* add_DescriptorSet(VkDescriptorSet var);

        ObjectInfo* get_Instance(VkInstance var);
        ObjectInfo* get_PhysicalDevice(VkPhysicalDevice var);
        ObjectInfo* get_Device(VkDevice var);
        ObjectInfo* get_SurfaceKHR(VkSurfaceKHR var);
        ObjectInfo* get_CommandPool(VkCommandPool var);
        ObjectInfo* get_CommandBuffer(VkCommandBuffer var);
        ObjectInfo* get_DescriptorPool(VkDescriptorPool var);
        ObjectInfo* get_RenderPass(VkRenderPass var);
        ObjectInfo* get_PipelineCache(VkPipelineCache var);
        ObjectInfo* get_Pipeline(VkPipeline var);
        ObjectInfo* get_Queue(VkQueue var);
        ObjectInfo* get_Semaphore(VkSemaphore var);
        ObjectInfo* get_DeviceMemory(VkDeviceMemory var);
        ObjectInfo* get_Fence(VkFence var);
        ObjectInfo* get_SwapchainKHR(VkSwapchainKHR var);
        ObjectInfo* get_Image(VkImage var);
        ObjectInfo* get_ImageView(VkImageView var);
        ObjectInfo* get_Buffer(VkBuffer var);
        ObjectInfo* get_BufferView(VkBufferView var);
        ObjectInfo* get_Framebuffer(VkFramebuffer var);
        ObjectInfo* get_Event(VkEvent var);
        ObjectInfo* get_QueryPool(VkQueryPool var);
        ObjectInfo* get_ShaderModule(VkShaderModule var);
        ObjectInfo* get_PipelineLayout(VkPipelineLayout var);
        ObjectInfo* get_Sampler(VkSampler var);
        ObjectInfo* get_DescriptorSetLayout(VkDescriptorSetLayout var);
        ObjectInfo* get_DescriptorSet(VkDescriptorSet var);

        void remove_Instance(VkInstance var);
        void remove_PhysicalDevice(VkPhysicalDevice var);
        void remove_Device(VkDevice var);
        void remove_SurfaceKHR(VkSurfaceKHR var);
        void remove_CommandPool(VkCommandPool var);
        void remove_CommandBuffer(VkCommandBuffer var);
        void remove_DescriptorPool(VkDescriptorPool var);
        void remove_RenderPass(VkRenderPass var);
        void remove_PipelineCache(VkPipelineCache var);
        void remove_Pipeline(VkPipeline var);
        void remove_Queue(VkQueue var);
        void remove_Semaphore(VkSemaphore var);
        void remove_DeviceMemory(VkDeviceMemory var);
        void remove_Fence(VkFence var);
        void remove_SwapchainKHR(VkSwapchainKHR var);
        void remove_Image(VkImage var);
        void remove_ImageView(VkImageView var);
        void remove_Buffer(VkBuffer var);
        void remove_BufferView(VkBufferView var);
        void remove_Framebuffer(VkFramebuffer var);
        void remove_Event(VkEvent var);
        void remove_QueryPool(VkQueryPool var);
        void remove_ShaderModule(VkShaderModule var);
        void remove_PipelineLayout(VkPipelineLayout var);
        void remove_Sampler(VkSampler var);
        void remove_DescriptorSetLayout(VkDescriptorSetLayout var);
        void remove_DescriptorSet(VkDescriptorSet var);

        static void copy_VkGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo* pDst, const VkGraphicsPipelineCreateInfo& src);
        static void copy_VkComputePipelineCreateInfo(VkComputePipelineCreateInfo* pDst, const VkComputePipelineCreateInfo& src);
        static void copy_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo* pDstStage, const VkPipelineShaderStageCreateInfo& srcStage);
        static void delete_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo* pStage);

        // Map relating a command buffer object to all the calls that have been
        // made on that command buffer since it was started or last reset.
        std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header*>> m_cmdBufferPackets;

        // List of all packets used to create or delete images.
        // We need to recreate them in the same order to ensure they will have the same size requirements as they had a trace-time.
        std::list<vktrace_trace_packet_header*> m_image_calls;
        
        TrimObjectInfoMap createdInstances;
        TrimObjectInfoMap createdPhysicalDevices;
        TrimObjectInfoMap createdDevices;
        TrimObjectInfoMap createdSurfaceKHRs;
        TrimObjectInfoMap createdCommandPools;
        TrimObjectInfoMap createdCommandBuffers;
        TrimObjectInfoMap createdDescriptorPools;
        TrimObjectInfoMap createdRenderPasss;
        TrimObjectInfoMap createdPipelineCaches;
        TrimObjectInfoMap createdPipelines;
        TrimObjectInfoMap createdQueues;
        TrimObjectInfoMap createdSemaphores;
        TrimObjectInfoMap createdDeviceMemorys;
        TrimObjectInfoMap createdFences;
        TrimObjectInfoMap createdSwapchainKHRs;
        TrimObjectInfoMap createdImages;
        TrimObjectInfoMap createdImageViews;
        TrimObjectInfoMap createdBuffers;
        TrimObjectInfoMap createdBufferViews;
        TrimObjectInfoMap createdFramebuffers;
        TrimObjectInfoMap createdEvents;
        TrimObjectInfoMap createdQueryPools;
        TrimObjectInfoMap createdShaderModules;
        TrimObjectInfoMap createdPipelineLayouts;
        TrimObjectInfoMap createdSamplers;
        TrimObjectInfoMap createdDescriptorSetLayouts;
        TrimObjectInfoMap createdDescriptorSets;
    };
}
