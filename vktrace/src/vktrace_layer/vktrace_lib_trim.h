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
void trim_debug_send_all_calls();
void trim_send_all_referenced_object_calls();

// The intent is to store a list of trace packets that are necessary in order to re-create
// the object that is the key of this map.
typedef std::unordered_map< void*, std::list<vktrace_trace_packet_header*>> ObjectPacketMap;

// Declares a map to track the packets related to a Vulkan object 'type'
// along with methods to add a new call to an object, and to mark an object as being referenced.
// Example macro expansion using type = Instance:
// extern ObjectPacketMap g_createdInstances;
// extern ObjectPacketMap g_referencedInstances;
// void trim_add_Instance_call(VkInstance var, vktrace_trace_packet_header* pHeader);
// void trim_mark_Instance_reference(VkInstance var);
#define TRIM_DECLARE_OBJECT_TRACKERS(type) \
extern ObjectPacketMap g_trim_created##type##s; \
extern ObjectPacketMap g_trim_referenced##type##s; \
void trim_add_##type##_call(Vk##type var, vktrace_trace_packet_header* pHeader); \
void trim_mark_##type##_reference(Vk##type var);

TRIM_DECLARE_OBJECT_TRACKERS(Instance);
TRIM_DECLARE_OBJECT_TRACKERS(PhysicalDevice);
TRIM_DECLARE_OBJECT_TRACKERS(Device);
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
