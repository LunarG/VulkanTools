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

#include <unordered_map>
#include <list>
#include <vector>
#include <set>
#include "vktrace_trace_packet_utils.h"

// Create / Destroy all image resources in the order performed by the
// application.
// Enabling this as a pre-processor macro so that we can compare performance and
// file size costs.
// TRUE: Needed on AMD hardware
// FALSE: Use normal object tracking to create only the necessary resources
#define TRIM_USE_ORDERED_IMAGE_CREATION 1

namespace trim {
vktrace_trace_packet_header *copy_packet(vktrace_trace_packet_header *pHeader);
void delete_packet(vktrace_trace_packet_header **ppHeader);

// RenderPasses and VkCmdPipelineBarrier can transition images
// to different layouts, but the change doesn't happen when the API call is made
// but rather when the command buffer is executed. Cache these transitions so
// that they can be applied when needed.
struct ImageTransition {
    VkImage image;
    VkImageLayout initialLayout;
    VkImageLayout finalLayout;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
};

extern VKTRACE_CRITICAL_SECTION trimTransitionMapLock;

// VkCmdPipelineBarrier can transition memory to a different accessMask, but
// the change doesn't happen when the API call is made but rather when the
// command buffer is executed. Cache these transitions so that they can be
// applied when needed.
struct BufferTransition {
    VkBuffer buffer;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
};

struct QueueFamily {
    uint32_t count;
    VkQueue *queues;
};

//-------------------------------------------------------------------------
// Some of the items in this struct are based on what is tracked in the
// 'VkLayer_object_tracker' (struct _OBJTRACK_NODE).
//-------------------------------------------------------------------------
typedef struct _Trim_ObjectInfo {
    uint64_t vkObject;                         // object handle
    bool bReferencedInTrim;                    // True if the object was referenced during the trim
                                               // frames
    VkInstance belongsToInstance;              // owning Instance
    VkPhysicalDevice belongsToPhysicalDevice;  // owning PhysicalDevice
    VkDevice belongsToDevice;                  // owning Device
    union _ObjectInfo {                        // additional object-specific information
        struct _Instance {                     // VkInstance
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            vktrace_trace_packet_header *pEnumeratePhysicalDevicesCountPacket;
            vktrace_trace_packet_header *pEnumeratePhysicalDevicesPacket;
        } Instance;
        struct _PhysicalDevice {  // VkPhysicalDevice
            vktrace_trace_packet_header *pGetPhysicalDevicePropertiesPacket;
            vktrace_trace_packet_header *pGetPhysicalDeviceProperties2KHRPacket;
            vktrace_trace_packet_header *pGetPhysicalDeviceMemoryPropertiesPacket;
            vktrace_trace_packet_header *pGetPhysicalDeviceQueueFamilyPropertiesCountPacket;
            vktrace_trace_packet_header *pGetPhysicalDeviceQueueFamilyPropertiesPacket;
            vktrace_trace_packet_header *pGetPhysicalDeviceQueueFamilyProperties2KHRCountPacket;
            vktrace_trace_packet_header *pGetPhysicalDeviceQueueFamilyProperties2KHRPacket;
            VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
            uint32_t queueFamilyCount;
        } PhysicalDevice;
        struct _SurfaceKHR {  // VkSurfaceKHR
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t queueFamilyCount;
        } SurfaceKHR;
        struct _Device {  // VkDevice
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t queueFamilyCount;
            QueueFamily *pQueueFamilies;
        } Device;
        struct _Queue {  // VkQueue
            vktrace_trace_packet_header *pCreatePacket;
            uint32_t queueFamilyIndex;
            uint32_t queueIndex;
        } Queue;
        struct _CommandPool {  // VkCommandPool
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t numCommandBuffersAllocated[VK_COMMAND_BUFFER_LEVEL_RANGE_SIZE];
            uint32_t queueFamilyIndex;
        } CommandPool;
        struct _SwapchainKHR {  // VkSwapchainKHR
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            vktrace_trace_packet_header *pGetSwapchainImageCountPacket;
            vktrace_trace_packet_header *pGetSwapchainImagesPacket;
        } SwapchainKHR;
        struct _CommandBuffer {  // VkCommandBuffer
            VkCommandPool commandPool;
            VkCommandBufferLevel level;
            VkRenderPass activeRenderPass;
            VkQueue submitQueue;
        } CommandBuffer;
        struct _DeviceMemory {  // VkDeviceMemory
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            VkDeviceSize size;
            uint32_t memoryTypeIndex;
            VkMemoryPropertyFlags propertyFlags;
            void *mappedAddress;
            VkDeviceSize mappedOffset;
            VkDeviceSize mappedSize;
            vktrace_trace_packet_header *pMapMemoryPacket;
            vktrace_trace_packet_header *pUnmapMemoryPacket;
            vktrace_trace_packet_header *pPersistentlyMapMemoryPacket;
        } DeviceMemory;
        struct _Image {  // VkImage
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            bool bIsSwapchainImage;
#if !TRIM_USE_ORDERED_IMAGE_CREATION
            vktrace_trace_packet_header *pGetImageMemoryRequirementsPacket;
#endif  //! TRIM_USE_ORDERED_IMAGE_CREATION
            vktrace_trace_packet_header *pBindImageMemoryPacket;
            vktrace_trace_packet_header *pMapMemoryPacket;
            vktrace_trace_packet_header *pUnmapMemoryPacket;
            VkDeviceMemory memory;
            VkDeviceSize memoryOffset;
            VkDeviceSize memorySize;
            VkFormat format;
            VkImageType imageType;
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
        struct _ImageView {  // VkImageView
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            VkImage image;
        } ImageView;
        struct _Buffer {  // VkBuffer
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            vktrace_trace_packet_header *pBindBufferMemoryPacket;
            vktrace_trace_packet_header *pMapMemoryPacket;
            vktrace_trace_packet_header *pUnmapMemoryPacket;
            uint32_t queueFamilyIndex;
            VkAccessFlags accessFlags;
            VkDeviceMemory memory;
            VkDeviceSize memoryOffset;
            VkDeviceSize size;
            bool needsStagingBuffer;
        } Buffer;
        struct _BufferView {  // VkBufferView
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
        } BufferView;
        struct _Sampler {  // VkSampler
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
        } Sampler;
        struct _DescriptorSetLayout {  // VkDescriptorSetLayout
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t numImages;
            uint32_t numBuffers;
            uint32_t numTexelBufferViews;
            uint32_t bindingCount;
            VkDescriptorSetLayoutBinding *pBindings;
        } DescriptorSetLayout;
        struct _PipelineLayout {  // VkPipelineLayout
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t descriptorSetLayoutCount;
            VkDescriptorSetLayout *pDescriptorSetLayouts;
        } PipelineLayout;
        struct _RenderPass {  // VkRenderPass
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t attachmentCount;
            ImageTransition *pAttachments;
        } RenderPass;
        struct _ShaderModule {  // VkShaderModule
            VkShaderModuleCreateInfo createInfo;
            const VkAllocationCallbacks *pAllocator;
        } ShaderModule;
        struct _PipelineCache {  // VkPipelineCache
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
        } PipelineCache;
        struct _Pipeline {  // VkPipeline
            bool isGraphicsPipeline;
            const VkAllocationCallbacks *pAllocator;
            VkPipelineCache pipelineCache;
            VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
            VkComputePipelineCreateInfo computePipelineCreateInfo;
            uint32_t renderPassVersion;
            uint32_t shaderModuleCreateInfoCount;
            VkShaderModuleCreateInfo *pShaderModuleCreateInfos;
        } Pipeline;
        struct _DescriptorPool {  // VkDescriptorPool
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            VkDescriptorPoolCreateFlags createFlags;
            uint32_t maxSets;
            uint32_t numSets;
        } DescriptorPool;
        struct _DescriptorSet {  // VkDescriptorSet
            VkDescriptorPool descriptorPool;
            VkDescriptorSetLayout layout;
            uint32_t numBindings;           // this is the number of elements allocated in
                                            // each of the two arrays below.
            uint32_t writeDescriptorCount;  // this is the number of descriptor
                                            // sets that will need a write
                                            // update.
            VkWriteDescriptorSet *pWriteDescriptorSets;
            uint32_t copyDescriptorCount;  // this is the number of descriptor
                                           // sets that will need a copy update.
            VkCopyDescriptorSet *pCopyDescriptorSets;
        } DescriptorSet;
        struct _DescriptorUpdateTemplate {  // VkDescriptorSet
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            VkDescriptorUpdateTemplateCreateFlags flags;
            uint32_t descriptorUpdateEntryCount;
            const VkDescriptorUpdateTemplateEntry *pDescriptorUpdateEntries;
            VkDescriptorUpdateTemplateType templateType;
            VkDescriptorSetLayout descriptorSetLayout;
            VkPipelineBindPoint pipelineBindPoint;
            VkPipelineLayout pipelineLayout;
            uint32_t set;
        } DescriptorUpdateTemplate;
        struct _Framebuffer {  // VkFramebuffer
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            uint32_t attachmentCount;
            VkImageView *pAttachments;
        } Framebuffer;
        struct _Semaphore {  // VkSemaphore
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            VkQueue signaledOnQueue;
            VkSwapchainKHR signaledOnSwapChain;  // The member variable is the swapchain object for
                                                 // which the related calls signal the semaphore. If
                                                 // the member variable is not VK_NULL_HANDLE, that
                                                 // mean the semaphore should be signaled after
                                                 // recreation in trimmed trace file.
                                                 //
                                                 // Swapchain related calls like vkAcquireNextImageKHR
                                                 // and vkQueuePresentKHR also wait or signal semaphore,
                                                 // so it's also needed to track the state change caused
                                                 // by them.
        } Semaphore;
        struct _Fence {  // VkFence
            const VkAllocationCallbacks *pAllocator;
            bool signaled;
        } Fence;
        struct _Event {  // VkEvent
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
        } Event;
        struct _QueryPool {  // VkQueryPool
            vktrace_trace_packet_header *pCreatePacket;
            const VkAllocationCallbacks *pAllocator;
            VkCommandBuffer commandBuffer;
            uint32_t size;
            bool *pResultsAvailable;
            VkQueryType queryType;
        } QueryPool;
    } ObjectInfo;
} ObjectInfo;

// typedef std::unordered_map<uint64_t, ObjectInfo> TrimObjectInfoMap;

//-------------------------------------------------------------------------
class StateTracker {
   public:
    StateTracker();
    StateTracker(const StateTracker &other);

    ~StateTracker();

    void clear();

    // The following two maps are used to record the binding relations between
    // command buffer and pipeline.
    //
    // Some title destroy a pipeline object when the command buffer which use
    // it still alive. If trim starting just at the location, the generated
    // trimmed trace file will include invalid calls in the command buffer
    // recreation process. During playback, some of these calls cause crash,
    // (also may cause undefined behaviour by Doc.). The two maps let us know
    // the binding relation between command buffer and pipeline. So we can
    // clears all record calls in bound command buffer if a pipeline is
    // destroyed before the command buffer.

    std::unordered_map<VkCommandBuffer, std::set<VkPipeline>> m_cmdBufferToBindingPipelinesMap;
    std::unordered_map<VkPipeline, std::set<VkCommandBuffer>> m_BindingPipelineTocmdBuffersMap;

    std::unordered_map<VkCommandBuffer, std::list<ImageTransition>> m_cmdBufferToImageTransitionsMap;
    void AddImageTransition(VkCommandBuffer commandBuffer, ImageTransition transition);
    void ClearImageTransitions(VkCommandBuffer commandBuffer);

    std::unordered_map<VkCommandBuffer, std::list<BufferTransition>> m_cmdBufferToBufferTransitionsMap;
    void AddBufferTransition(VkCommandBuffer commandBuffer, BufferTransition transition);
    void ClearBufferTransitions(VkCommandBuffer commandBuffer);

    void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header *pHeader);
    void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer);

    void add_RenderPassCreateInfo(VkRenderPass renderPass, const VkRenderPassCreateInfo *pCreateInfo);
    VkRenderPassCreateInfo *get_RenderPassCreateInfo(VkRenderPass renderPass, uint32_t version);
    uint32_t get_RenderPassVersion(VkRenderPass renderPass);

#if TRIM_USE_ORDERED_IMAGE_CREATION
    void add_Image_call(vktrace_trace_packet_header *pHeader);
#endif  // TRIM_USE_ORDERED_IMAGE_CREATION

    StateTracker &operator=(const StateTracker &other);

    ObjectInfo &add_Instance(VkInstance var);
    ObjectInfo &add_PhysicalDevice(VkPhysicalDevice var);
    ObjectInfo &add_Device(VkDevice var);
    ObjectInfo &add_SurfaceKHR(VkSurfaceKHR var);
    ObjectInfo &add_CommandPool(VkCommandPool var);
    ObjectInfo &add_CommandBuffer(VkCommandBuffer var);
    ObjectInfo &add_DescriptorPool(VkDescriptorPool var);
    ObjectInfo &add_RenderPass(VkRenderPass var);
    ObjectInfo &add_PipelineCache(VkPipelineCache var);
    ObjectInfo &add_Pipeline(VkPipeline var);
    ObjectInfo &add_Queue(VkQueue var);
    ObjectInfo &add_Semaphore(VkSemaphore var);
    ObjectInfo &add_DeviceMemory(VkDeviceMemory var);
    ObjectInfo &add_Fence(VkFence var);
    ObjectInfo &add_SwapchainKHR(VkSwapchainKHR var);
    ObjectInfo &add_Image(VkImage var);
    ObjectInfo &add_ImageView(VkImageView var);
    ObjectInfo &add_Buffer(VkBuffer var);
    ObjectInfo &add_BufferView(VkBufferView var);
    ObjectInfo &add_Framebuffer(VkFramebuffer var);
    ObjectInfo &add_Event(VkEvent var);
    ObjectInfo &add_QueryPool(VkQueryPool var);
    ObjectInfo &add_ShaderModule(VkShaderModule var);
    ObjectInfo &add_PipelineLayout(VkPipelineLayout var);
    ObjectInfo &add_Sampler(VkSampler var);
    ObjectInfo &add_DescriptorSetLayout(VkDescriptorSetLayout var);
    ObjectInfo &add_DescriptorSet(VkDescriptorSet var);
    ObjectInfo &add_DescriptorUpdateTemplate(VkDescriptorUpdateTemplate var);

    ObjectInfo *get_Instance(VkInstance var);
    ObjectInfo *get_PhysicalDevice(VkPhysicalDevice var);
    ObjectInfo *get_Device(VkDevice var);
    ObjectInfo *get_SurfaceKHR(VkSurfaceKHR var);
    ObjectInfo *get_CommandPool(VkCommandPool var);
    ObjectInfo *get_CommandBuffer(VkCommandBuffer var);
    ObjectInfo *get_DescriptorPool(VkDescriptorPool var);
    ObjectInfo *get_RenderPass(VkRenderPass var);
    ObjectInfo *get_PipelineCache(VkPipelineCache var);
    ObjectInfo *get_Pipeline(VkPipeline var);
    ObjectInfo *get_Queue(VkQueue var);
    ObjectInfo *get_Semaphore(VkSemaphore var);
    ObjectInfo *get_DeviceMemory(VkDeviceMemory var);
    ObjectInfo *get_Fence(VkFence var);
    ObjectInfo *get_SwapchainKHR(VkSwapchainKHR var);
    ObjectInfo *get_Image(VkImage var);
    ObjectInfo *get_ImageView(VkImageView var);
    ObjectInfo *get_Buffer(VkBuffer var);
    ObjectInfo *get_BufferView(VkBufferView var);
    ObjectInfo *get_Framebuffer(VkFramebuffer var);
    ObjectInfo *get_Event(VkEvent var);
    ObjectInfo *get_QueryPool(VkQueryPool var);
    ObjectInfo *get_ShaderModule(VkShaderModule var);
    ObjectInfo *get_PipelineLayout(VkPipelineLayout var);
    ObjectInfo *get_Sampler(VkSampler var);
    ObjectInfo *get_DescriptorSetLayout(VkDescriptorSetLayout var);
    ObjectInfo *get_DescriptorUpdateTemplate(VkDescriptorUpdateTemplate var);
    ObjectInfo *get_DescriptorSet(VkDescriptorSet var);

    std::set<VkCommandBuffer> *get_BoundCommandBuffers(VkPipeline var, bool createFlag = true);
    std::set<VkPipeline> *get_BindingPipelines(VkCommandBuffer var, bool createFlag = true);

    void remove_Instance(const VkInstance var);
    void remove_PhysicalDevice(const VkPhysicalDevice var);
    void remove_Device(const VkDevice var);
    void remove_SurfaceKHR(const VkSurfaceKHR var);
    void remove_CommandPool(const VkCommandPool var);
    void remove_CommandBuffer(const VkCommandBuffer var);
    void remove_DescriptorPool(const VkDescriptorPool var);
    void remove_RenderPass(const VkRenderPass var);
    void remove_PipelineCache(const VkPipelineCache var);
    void remove_Pipeline(const VkPipeline var);
    void remove_Queue(const VkQueue var);
    void remove_Semaphore(const VkSemaphore var);
    void remove_DeviceMemory(const VkDeviceMemory var);
    void remove_Fence(const VkFence var);
    void remove_SwapchainKHR(const VkSwapchainKHR var);
    void remove_Image(const VkImage var);
    void remove_ImageView(const VkImageView var);
    void remove_Buffer(const VkBuffer var);
    void remove_BufferView(const VkBufferView var);
    void remove_Framebuffer(const VkFramebuffer var);
    void remove_Event(const VkEvent var);
    void remove_QueryPool(const VkQueryPool var);
    void remove_ShaderModule(const VkShaderModule var);
    void remove_PipelineLayout(const VkPipelineLayout var);
    void remove_Sampler(const VkSampler var);
    void remove_DescriptorSetLayout(const VkDescriptorSetLayout var);
    void remove_DescriptorUpdateTemplate(const VkDescriptorUpdateTemplate var);
    void remove_DescriptorSet(const VkDescriptorSet var);

    static void copy_VkRenderPassCreateInfo(VkRenderPassCreateInfo *pDst, const VkRenderPassCreateInfo &src);

    static void copy_VkShaderModuleCreateInfo(VkShaderModuleCreateInfo *pDst, const VkShaderModuleCreateInfo &src);

    static void delete_VkShaderModuleCreateInfo(VkShaderModuleCreateInfo *pModule);

    static void copy_VkGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo *pDst, const VkGraphicsPipelineCreateInfo &src);
    static void copy_VkComputePipelineCreateInfo(VkComputePipelineCreateInfo *pDst, const VkComputePipelineCreateInfo &src);
    static void copy_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo *pDstStage,
                                                     const VkPipelineShaderStageCreateInfo &srcStage);
    static void delete_VkPipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo *pStage);

    // Map relating a command buffer object to all the calls that have been
    // made on that command buffer since it was started or last reset.
    std::unordered_map<VkCommandBuffer, std::list<vktrace_trace_packet_header *>> m_cmdBufferPackets;

    // Map to keep track of older RenderPass versions so that we can recreate
    // pipelines.
    std::unordered_map<VkRenderPass, std::vector<VkRenderPassCreateInfo *>> m_renderPassVersions;

    // List of all packets used to create or delete images.
    // We need to recreate them in the same order to ensure they will have the
    // same size requirements as they had a trace-time.
    std::list<vktrace_trace_packet_header *> m_image_calls;

    std::unordered_map<VkInstance, ObjectInfo> createdInstances;
    std::unordered_map<VkPhysicalDevice, ObjectInfo> createdPhysicalDevices;
    std::unordered_map<VkDevice, ObjectInfo> createdDevices;
    std::unordered_map<VkSurfaceKHR, ObjectInfo> createdSurfaceKHRs;
    std::unordered_map<VkCommandPool, ObjectInfo> createdCommandPools;
    std::unordered_map<VkCommandBuffer, ObjectInfo> createdCommandBuffers;
    std::unordered_map<VkDescriptorPool, ObjectInfo> createdDescriptorPools;
    std::unordered_map<VkRenderPass, ObjectInfo> createdRenderPasss;
    std::unordered_map<VkPipelineCache, ObjectInfo> createdPipelineCaches;
    std::unordered_map<VkPipeline, ObjectInfo> createdPipelines;
    std::unordered_map<VkQueue, ObjectInfo> createdQueues;
    std::unordered_map<VkSemaphore, ObjectInfo> createdSemaphores;
    std::unordered_map<VkDeviceMemory, ObjectInfo> createdDeviceMemorys;
    std::unordered_map<VkFence, ObjectInfo> createdFences;
    std::unordered_map<VkSwapchainKHR, ObjectInfo> createdSwapchainKHRs;
    std::unordered_map<VkImage, ObjectInfo> createdImages;
    std::unordered_map<VkImageView, ObjectInfo> createdImageViews;
    std::unordered_map<VkBuffer, ObjectInfo> createdBuffers;
    std::unordered_map<VkBufferView, ObjectInfo> createdBufferViews;
    std::unordered_map<VkFramebuffer, ObjectInfo> createdFramebuffers;
    std::unordered_map<VkEvent, ObjectInfo> createdEvents;
    std::unordered_map<VkQueryPool, ObjectInfo> createdQueryPools;
    std::unordered_map<VkShaderModule, ObjectInfo> createdShaderModules;
    std::unordered_map<VkPipelineLayout, ObjectInfo> createdPipelineLayouts;
    std::unordered_map<VkSampler, ObjectInfo> createdSamplers;
    std::unordered_map<VkDescriptorSetLayout, ObjectInfo> createdDescriptorSetLayouts;
    std::unordered_map<VkDescriptorUpdateTemplate, ObjectInfo> createdDescriptorUpdateTemplates;
    std::unordered_map<VkDescriptorSet, ObjectInfo> createdDescriptorSets;
};
}
