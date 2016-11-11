/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once
#include <list>
#include "vktrace_trace_packet_identifiers.h"

#include "vktrace_lib_trim_generate.h"
#include "vktrace_lib_trim_statetracker.h"
#include "vulkan.h"

// Trim support
// Indicates whether trim support will be utilized during this instance of vktrace.
// Only set once based on the VKTRACE_TRIM_FRAMES env var.
extern bool g_trimEnabled;
extern bool g_trimIsPreTrim;
extern bool g_trimIsInTrim;
extern bool g_trimIsPostTrim;
extern uint64_t g_trimFrameCounter;
extern uint64_t g_trimStartFrame;
extern uint64_t g_trimEndFrame;

namespace trim
{
    void initialize();
    void deinitialize();

    // Use this to snapshot the global state tracker at the start of the trim frames.
    void snapshot_state_tracker();

    // Outputs object-related trace packets to the trace file.
    void write_all_referenced_object_calls();
    void add_recorded_packet(vktrace_trace_packet_header* pHeader);
    void write_recorded_packets();
    void write_destroy_packets();
    void delete_all_packets();

    //-----------------------
    // the follow calls are pass-through to the StateTracker
    void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader);
    void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer);

#if TRIM_USE_ORDERED_IMAGE_CREATION
    void add_Image_call(vktrace_trace_packet_header* pHeader);
#endif //TRIM_USE_ORDERED_IMAGE_CREATION
    // The above calls are pass-through to the StateTracker
    //-----------------------

    void reset_DescriptorPool(VkDescriptorPool descriptorPool);

    VkMemoryPropertyFlags LookUpMemoryProperties(VkDevice device, uint32_t memoryTypeIndex);

    // check if a memory type on the physical device is only DEVICE_LOCAL and not HOST_VISIBLE
    bool IsMemoryDeviceOnly(VkDevice device, VkDeviceMemory memory);

    VkImageAspectFlags getImageAspectFromFormat(VkFormat format);

    // Typically an application will have one VkAllocationCallbacks struct and 
    // will pass in that same address as needed, so we'll keep a map to correlate
    // the supplied address to the AllocationCallbacks object
    void add_Allocator(const VkAllocationCallbacks* pAllocator);
    VkAllocationCallbacks* get_Allocator(const VkAllocationCallbacks* pAllocator);

    void mark_CommandBuffer_reference(VkCommandBuffer commandbuffer);

    ObjectInfo* add_Instance_object(VkInstance var);
    ObjectInfo* get_Instance_objectInfo(VkInstance var);

    ObjectInfo* add_PhysicalDevice_object(VkPhysicalDevice var);
    ObjectInfo* get_PhysicalDevice_objectInfo(VkPhysicalDevice var);

    ObjectInfo* add_Device_object(VkDevice var);
    ObjectInfo* get_Device_objectInfo(VkDevice var);
    
    ObjectInfo* add_SurfaceKHR_object(VkSurfaceKHR var);
    ObjectInfo* get_SurfaceKHR_objectInfo(VkSurfaceKHR var);
    
    ObjectInfo* add_CommandPool_object(VkCommandPool var);
    ObjectInfo* get_CommandPool_objectInfo(VkCommandPool var);

    ObjectInfo* add_CommandBuffer_object(VkCommandBuffer var);
    ObjectInfo* get_CommandBuffer_objectInfo(VkCommandBuffer var);

    ObjectInfo* add_DescriptorPool_object(VkDescriptorPool var);
    ObjectInfo* get_DescriptorPool_objectInfo(VkDescriptorPool var);

    ObjectInfo* add_RenderPass_object(VkRenderPass var);
    ObjectInfo* get_RenderPass_objectInfo(VkRenderPass var);

    ObjectInfo* add_PipelineCache_object(VkPipelineCache var);
    ObjectInfo* get_PipelineCache_objectInfo(VkPipelineCache var);

    ObjectInfo* add_Pipeline_object(VkPipeline var);
    ObjectInfo* get_Pipeline_objectInfo(VkPipeline var);

    ObjectInfo* add_Queue_object(VkQueue var);
    ObjectInfo* get_Queue_objectInfo(VkQueue var);

    ObjectInfo* add_Semaphore_object(VkSemaphore var);
    ObjectInfo* get_Semaphore_objectInfo(VkSemaphore var);

    ObjectInfo* add_DeviceMemory_object(VkDeviceMemory var);
    ObjectInfo* get_DeviceMemory_objectInfo(VkDeviceMemory var);

    ObjectInfo* add_Fence_object(VkFence var);
    ObjectInfo* get_Fence_objectInfo(VkFence var);

    ObjectInfo* add_SwapchainKHR_object(VkSwapchainKHR var);
    ObjectInfo* get_SwapchainKHR_objectInfo(VkSwapchainKHR var);

    ObjectInfo* add_Image_object(VkImage var);
    ObjectInfo* get_Image_objectInfo(VkImage var);

    ObjectInfo* add_ImageView_object(VkImageView var);
    ObjectInfo* get_ImageView_objectInfo(VkImageView var);

    ObjectInfo* add_Buffer_object(VkBuffer var);
    ObjectInfo* get_Buffer_objectInfo(VkBuffer var);

    ObjectInfo* add_BufferView_object(VkBufferView var);
    ObjectInfo* get_BufferView_objectInfo(VkBufferView var);

    ObjectInfo* add_Framebuffer_object(VkFramebuffer var);
    ObjectInfo* get_Framebuffer_objectInfo(VkFramebuffer var);

    ObjectInfo* add_Event_object(VkEvent var);
    ObjectInfo* get_Event_objectInfo(VkEvent var);

    ObjectInfo* add_QueryPool_object(VkQueryPool var);
    ObjectInfo* get_QueryPool_objectInfo(VkQueryPool var);

    ObjectInfo* add_ShaderModule_object(VkShaderModule var);
    ObjectInfo* get_ShaderModule_objectInfo(VkShaderModule var);

    ObjectInfo* add_PipelineLayout_object(VkPipelineLayout var);
    ObjectInfo* get_PipelineLayout_objectInfo(VkPipelineLayout var);

    ObjectInfo* add_Sampler_object(VkSampler var);
    ObjectInfo* get_Sampler_objectInfo(VkSampler var);

    ObjectInfo* add_DescriptorSetLayout_object(VkDescriptorSetLayout var);
    ObjectInfo* get_DescriptorSetLayout_objectInfo(VkDescriptorSetLayout var);

    ObjectInfo* add_DescriptorSet_object(VkDescriptorSet var);
    ObjectInfo* get_DescriptorSet_objectInfo(VkDescriptorSet var);

    void remove_Instance_object(VkInstance var);
    void remove_PhysicalDevice_object(VkPhysicalDevice var);
    void remove_Device_object(VkDevice var);
    void remove_SurfaceKHR_object(VkSurfaceKHR var);
    void remove_CommandPool_object(VkCommandPool var);
    void remove_CommandBuffer_object(VkCommandBuffer var);
    void remove_DescriptorPool_object(VkDescriptorPool var);
    void remove_RenderPass_object(VkRenderPass var);
    void remove_PipelineCache_object(VkPipelineCache var);
    void remove_Pipeline_object(VkPipeline var);
    void remove_Queue_object(VkQueue var);
    void remove_Semaphore_object(VkSemaphore var);
    void remove_DeviceMemory_object(VkDeviceMemory var);
    void remove_Fence_object(VkFence var);
    void remove_SwapchainKHR_object(VkSwapchainKHR var);
    void remove_Image_object(VkImage var);
    void remove_ImageView_object(VkImageView var);
    void remove_Buffer_object(VkBuffer var);
    void remove_BufferView_object(VkBufferView var);
    void remove_Framebuffer_object(VkFramebuffer var);
    void remove_Event_object(VkEvent var);
    void remove_QueryPool_object(VkQueryPool var);
    void remove_ShaderModule_object(VkShaderModule var);
    void remove_PipelineLayout_object(VkPipelineLayout var);
    void remove_Sampler_object(VkSampler var);
    void remove_DescriptorSetLayout_object(VkDescriptorSetLayout var);
    void remove_DescriptorSet_object(VkDescriptorSet var);
} // namespace trim
