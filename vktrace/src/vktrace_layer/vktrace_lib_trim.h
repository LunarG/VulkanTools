/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#pragma once
#include <unordered_map>
#include <list>
#include "vktrace_trace_packet_identifiers.h"
#include "vulkan.h"

// Outputs object-related trace packets to the trace file.
void trim_write_all_referenced_object_calls();
void trim_add_recorded_packet(vktrace_trace_packet_header* pHeader);
void trim_write_recorded_packets();
void trim_write_destroy_packets();
void trim_delete_all_packets();

// some of the items in this struct are based on what is tracked in the 'VkLayer_object_tracker' (struct _OBJTRACK_NODE).
typedef struct _Trim_ObjectInfo
{
    uint64_t vkObject;                               // object handle
    VkDebugReportObjectTypeEXT vkObjectType;         // object type
    bool bReferencedInTrim;                          // True if the object was referenced during the trim frames
    VkInstance belongsToInstance;                    // owning Instance
    VkPhysicalDevice belongsToPhysicalDevice;        // owning PhysicalDevice
    VkDevice belongsToDevice;                        // owning Device
    union _ObjectInfo {                              // additional object-specific information
        struct _Instance {              // VkInstance
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
            vktrace_trace_packet_header* pEnumeratePhysicalDevicesCountPacket;
            vktrace_trace_packet_header* pEnumeratePhysicalDevicesPacket;
        } Instance;
        struct _PhysicalDevice {        // VkPhysicalDevice

        } PhysicalDevice;
        struct _SurfaceKHR {            // VkSurfaceKHR
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } SurfaceKHR;
        struct _Device {                // VkDevice
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } Device;
        struct _Queue {                 // VkQueue
            vktrace_trace_packet_header* pCreatePacket;
        } Queue;
        struct _CommandPool {           // VkCommandPool
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
            uint32_t numCommandBuffersAllocated[VK_COMMAND_BUFFER_LEVEL_RANGE_SIZE];
        } CommandPool;
        struct _SwapchainKHR {           // VkSwapchainKHR
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
            vktrace_trace_packet_header* pGetSwapchainImageCountPacket;
            vktrace_trace_packet_header* pGetSwapchainImagesPacket;
        } SwapchainKHR;
        struct _CommandBuffer {         // VkCommandBuffer
            VkCommandPool commandPool;
            VkCommandBufferLevel level;
        } CommandBuffer;
        struct _DeviceMemory {          // VkDeviceMemory
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
            VkDeviceSize size;
            void* mappedAddress;
            VkDeviceSize mappedOffset;
            VkDeviceSize mappedSize;
        } DeviceMemory;
        struct _Image {                 // VkImage
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
            bool bIsSwapchainImage;
            vktrace_trace_packet_header* pBindImageMemoryPacket;
            VkDeviceMemory memory;
            VkDeviceSize memoryOffset;
        } Image;
        struct _ImageView {                 // VkImageView
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } ImageView;
        struct _Buffer {                 // VkBuffer
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
            vktrace_trace_packet_header* pBindBufferMemoryPacket;
            VkDeviceMemory memory;
            VkDeviceSize memoryOffset;
        } Buffer;
        struct _BufferView {                 // VkBufferView
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } BufferView;
        struct _Sampler {                 // VkSampler
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } Sampler;
        struct _DescriptorSetLayout {     // VkDescriptorSetLayout
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } DescriptorSetLayout;
        struct _PipelineLayout {     // VkPipelineLayout
            vktrace_trace_packet_header* pCreatePacket;
            VkAllocationCallbacks allocator;
        } PipelineLayout;
        struct _DescriptorPool {        // VkDescriptorPool
            VkDescriptorPoolCreateFlags createInfo_flags;
            uint32_t createInfo_maxSets;
            uint32_t createInfo_poolSizeCount;
            VkDescriptorPoolSize* createInfo_pPoolSizes;
        } DescriptorPool;
    } ObjectInfo;
} Trim_ObjectInfo;

typedef std::unordered_map<void*, Trim_ObjectInfo> TrimObjectInfoMap;

#define TRIM_DECLARE_OBJECT_TRACKERS(type) \
TrimObjectInfoMap created##type##s; 

#define TRIM_DECLARE_OBJECT_TRACKER_FUNCS(type) \
Trim_ObjectInfo* trim_add_##type##_object(Vk##type var); \
void trim_remove_##type##_object(Vk##type var); \
Trim_ObjectInfo* trim_get_##type##_objectInfo(Vk##type var); \
void trim_add_##type##_call(Vk##type var, vktrace_trace_packet_header* pHeader); \
void trim_mark_##type##_reference(Vk##type var);

typedef struct _Trim_StateTracker
{
    TRIM_DECLARE_OBJECT_TRACKERS(Instance);
    TRIM_DECLARE_OBJECT_TRACKERS(PhysicalDevice);
    TRIM_DECLARE_OBJECT_TRACKERS(Device);
    TRIM_DECLARE_OBJECT_TRACKERS(SurfaceKHR);
    TRIM_DECLARE_OBJECT_TRACKERS(CommandPool);
    TRIM_DECLARE_OBJECT_TRACKERS(CommandBuffer);
    TRIM_DECLARE_OBJECT_TRACKERS(DescriptorPool);
    TRIM_DECLARE_OBJECT_TRACKERS(RenderPass);
    TRIM_DECLARE_OBJECT_TRACKERS(Pipeline);
    TRIM_DECLARE_OBJECT_TRACKERS(Queue);
    TRIM_DECLARE_OBJECT_TRACKERS(Semaphore);
    TRIM_DECLARE_OBJECT_TRACKERS(DeviceMemory);
    TRIM_DECLARE_OBJECT_TRACKERS(Fence);
    TRIM_DECLARE_OBJECT_TRACKERS(SwapchainKHR);
    TRIM_DECLARE_OBJECT_TRACKERS(Image);
    TRIM_DECLARE_OBJECT_TRACKERS(ImageView);
    TRIM_DECLARE_OBJECT_TRACKERS(Buffer);
    TRIM_DECLARE_OBJECT_TRACKERS(BufferView);
    TRIM_DECLARE_OBJECT_TRACKERS(Framebuffer);
    TRIM_DECLARE_OBJECT_TRACKERS(Event);
    TRIM_DECLARE_OBJECT_TRACKERS(QueryPool);
    TRIM_DECLARE_OBJECT_TRACKERS(ShaderModule);
    TRIM_DECLARE_OBJECT_TRACKERS(PipelineLayout);
    TRIM_DECLARE_OBJECT_TRACKERS(Sampler);
    TRIM_DECLARE_OBJECT_TRACKERS(DescriptorSetLayout);
    TRIM_DECLARE_OBJECT_TRACKERS(DescriptorSet);
} Trim_StateTracker;


TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Instance);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(PhysicalDevice);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(Device);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(SurfaceKHR);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(CommandPool);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(CommandBuffer);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(DescriptorPool);
TRIM_DECLARE_OBJECT_TRACKER_FUNCS(RenderPass);
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

extern Trim_StateTracker g_trimGlobalStateTracker;
