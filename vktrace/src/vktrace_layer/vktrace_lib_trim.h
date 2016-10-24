/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once
#include <list>
#include "vktrace_trace_packet_identifiers.h"

    // Create / Destroy all image resources in the order performed by the application. 
    // Enabling this as a pre-processor macro so that we can compare performance and file size costs.
    // TRUE: Needed on AMD hardware
    // FALSE: Use normal object tracking to create only the necessary resources
#define TRIM_USE_ORDERED_IMAGE_CREATION TRUE

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

    // Use this to snapshot the global state tracker at the start of the trim frames.
    void snapshot_state_tracker();

    // Outputs object-related trace packets to the trace file.
    void write_all_referenced_object_calls();
    void add_recorded_packet(vktrace_trace_packet_header* pHeader);
    void write_recorded_packets();
    void write_destroy_packets();
    void delete_all_packets();

    void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header* pHeader);
    void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer);

    void reset_DescriptorPool(VkDescriptorPool descriptorPool);

    VkMemoryPropertyFlags LookUpMemoryProperties(VkDevice device, uint32_t memoryTypeIndex);

    // check if a memory type on the physical device is only DEVICE_LOCAL and not HOST_VISIBLE
    bool IsMemoryDeviceOnly(VkDevice device, VkDeviceMemory memory);

    VkImageAspectFlags getImageAspectFromFormat(VkFormat format);

    void delete_packet(vktrace_trace_packet_header** ppHeader);

#if TRIM_USE_ORDERED_IMAGE_CREATION
    void add_Image_call(vktrace_trace_packet_header* pHeader);
#endif //TRIM_USE_ORDERED_IMAGE_CREATION

    // Typically an application will have one VkAllocationCallbacks struct and 
    // will pass in that same address as needed, so we'll keep a map to correlate
    // the supplied address to the AllocationCallbacks object
    void add_Allocator(const VkAllocationCallbacks* pAllocator);
    VkAllocationCallbacks* get_Allocator(const VkAllocationCallbacks* pAllocator);

#define TRIM_DECLARE_OBJECT_TRACKER_FUNCS(type) \
    ObjectInfo* add_##type##_object(Vk##type var); \
    void remove_##type##_object(Vk##type var); \
    ObjectInfo* get_##type##_objectInfo(Vk##type var);

    void mark_CommandBuffer_reference(VkCommandBuffer commandbuffer);

    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Instance);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(PhysicalDevice);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Device);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(SurfaceKHR);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(CommandPool);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(CommandBuffer);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(DescriptorPool);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(RenderPass);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(PipelineCache);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Pipeline);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Queue);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Semaphore);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(DeviceMemory);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Fence);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(SwapchainKHR);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Image);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(ImageView);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Buffer);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(BufferView);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Framebuffer);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Event);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(QueryPool);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(ShaderModule);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(PipelineLayout);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Sampler);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(DescriptorSetLayout);
    TRIM_DECLARE_OBJECT_TRACKER_FUNCS(DescriptorSet);

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
