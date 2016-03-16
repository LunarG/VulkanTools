/*
* Copyright (C) 2016 Advanced Micro Devices, Inc.
*
* Author: Peter Lohrmann <Peter.Lohrmann@amd.com>
*/
#include "vktrace_lib_trim.h"
#include "vktrace_trace_packet_utils.h"
#include "vulkan.h"

#define TRIM_DEFINE_OBJECT_TRACKERS(type) \
ObjectPacketMap g_trim_created##type##s; \
ObjectPacketMap g_trim_referenced##type##s;

TRIM_DEFINE_OBJECT_TRACKERS(Instance);
TRIM_DEFINE_OBJECT_TRACKERS(Device);
TRIM_DEFINE_OBJECT_TRACKERS(CommandPool);
TRIM_DEFINE_OBJECT_TRACKERS(DescriptorPool);
TRIM_DEFINE_OBJECT_TRACKERS(RenderPass);
TRIM_DEFINE_OBJECT_TRACKERS(Pipeline);
TRIM_DEFINE_OBJECT_TRACKERS(Queue);
TRIM_DEFINE_OBJECT_TRACKERS(Semaphore);
TRIM_DEFINE_OBJECT_TRACKERS(DeviceMemory);
TRIM_DEFINE_OBJECT_TRACKERS(Fence);
TRIM_DEFINE_OBJECT_TRACKERS(SwapchainKHR);
TRIM_DEFINE_OBJECT_TRACKERS(Image);
TRIM_DEFINE_OBJECT_TRACKERS(ImageView);
TRIM_DEFINE_OBJECT_TRACKERS(Buffer);
TRIM_DEFINE_OBJECT_TRACKERS(BufferView);
TRIM_DEFINE_OBJECT_TRACKERS(Framebuffer);
TRIM_DEFINE_OBJECT_TRACKERS(Event);
TRIM_DEFINE_OBJECT_TRACKERS(QueryPool);
TRIM_DEFINE_OBJECT_TRACKERS(ShaderModule);
TRIM_DEFINE_OBJECT_TRACKERS(PipelineLayout);
TRIM_DEFINE_OBJECT_TRACKERS(Sampler);
TRIM_DEFINE_OBJECT_TRACKERS(DescriptorSetLayout);
TRIM_DEFINE_OBJECT_TRACKERS(DescriptorSet);

//===============================================
// Write all stored trace packets to the trace file
//===============================================
#define TRIM_WRITE_OBJECT_PACKETS(type) \
    for (ObjectPacketMap::iterator iter = g_trim_created##type##s.begin(); iter != g_trim_created##type##s.end(); iter++) { \
        for (std::list<vktrace_trace_packet_header*>::iterator call = iter->second.begin(); call != iter->second.end(); call++) { \
            vktrace_write_trace_packet(*call, vktrace_trace_get_trace_file()); \
            vktrace_delete_trace_packet(&(*call)); \
        } \
        iter->second.clear(); \
    } \
    g_trim_created##type##s.clear();

void trim_debug_send_all_calls()
{
    TRIM_WRITE_OBJECT_PACKETS(Instance);
    TRIM_WRITE_OBJECT_PACKETS(Device);
    TRIM_WRITE_OBJECT_PACKETS(SwapchainKHR);
    TRIM_WRITE_OBJECT_PACKETS(Queue);
    TRIM_WRITE_OBJECT_PACKETS(DeviceMemory);
    TRIM_WRITE_OBJECT_PACKETS(CommandPool);
    TRIM_WRITE_OBJECT_PACKETS(Image);
    TRIM_WRITE_OBJECT_PACKETS(ImageView);
    TRIM_WRITE_OBJECT_PACKETS(Buffer);
    TRIM_WRITE_OBJECT_PACKETS(BufferView);
    TRIM_WRITE_OBJECT_PACKETS(ShaderModule);
    TRIM_WRITE_OBJECT_PACKETS(Sampler);
    TRIM_WRITE_OBJECT_PACKETS(RenderPass);
    TRIM_WRITE_OBJECT_PACKETS(Framebuffer);
    TRIM_WRITE_OBJECT_PACKETS(DescriptorSetLayout);
    TRIM_WRITE_OBJECT_PACKETS(DescriptorPool);
    TRIM_WRITE_OBJECT_PACKETS(DescriptorSet);
    TRIM_WRITE_OBJECT_PACKETS(PipelineLayout);
    TRIM_WRITE_OBJECT_PACKETS(Pipeline);
    TRIM_WRITE_OBJECT_PACKETS(Semaphore);
    TRIM_WRITE_OBJECT_PACKETS(Fence);
    TRIM_WRITE_OBJECT_PACKETS(Event);
    TRIM_WRITE_OBJECT_PACKETS(QueryPool);
}

//===============================================
// Write trace packets only for referenced objects to the trace file
// TODO: Currently unused and incomplete
//===============================================
void trim_send_all_referenced_object_calls()
{
    assert(!"Not Fully Implemented");
    // first create instances
    for (ObjectPacketMap::iterator iter = g_trim_referencedInstances.begin(); iter != g_trim_createdInstances.end(); iter++)
    {
        for (std::list<vktrace_trace_packet_header*>::iterator call = iter->second.begin(); call != iter->second.end(); call++)
        {
            vktrace_write_trace_packet(*call, vktrace_trace_get_trace_file());
            vktrace_delete_trace_packet(&(*call));
        }
        iter->second.clear();
    }
    g_trim_createdInstances.clear();
}

//===============================================
#define TRIM_ADD_OBJECT_CALL(type) \
void trim_add_##type##_call(Vk##type var, vktrace_trace_packet_header* pHeader) { \
    ObjectPacketMap::iterator iter = g_trim_created##type##s.find(var); \
    if (iter == g_trim_created##type##s.end()) \
    { \
        std::list<vktrace_trace_packet_header*> packets; \
        packets.push_back(pHeader); \
        g_trim_created##type##s.insert_or_assign(var, packets); \
    } else { \
        iter->second.push_back(pHeader); \
    } \
}

//===============================================
#define TRIM_MARK_OBJECT_REFERENCE(type) \
void trim_mark_##type##_reference(Vk##type var) { \
    ObjectPacketMap::iterator referenced = g_trim_referenced##type##s.find(var);\
    if (referenced != g_trim_referenced##type##s.end())\
    {\
        /* // it hasn't been referenced yet. */ \
        ObjectPacketMap::iterator existing = g_trim_created##type##s.find(var); \
        assert(existing != g_trim_created##type##s.end()); \
        /* // add it to the referenced list */ \
        g_trim_referenced##type##s.insert_or_assign(existing->first, existing->second); \
    } \
}

//===============================================
// Object tracking
//===============================================
TRIM_ADD_OBJECT_CALL(Instance);
TRIM_MARK_OBJECT_REFERENCE(Instance);

TRIM_ADD_OBJECT_CALL(Device)
TRIM_MARK_OBJECT_REFERENCE(Device)

TRIM_ADD_OBJECT_CALL(CommandPool)
TRIM_MARK_OBJECT_REFERENCE(CommandPool)

TRIM_ADD_OBJECT_CALL(DescriptorPool)
TRIM_MARK_OBJECT_REFERENCE(DescriptorPool)

TRIM_ADD_OBJECT_CALL(DescriptorSet)
TRIM_MARK_OBJECT_REFERENCE(DescriptorSet)

TRIM_ADD_OBJECT_CALL(RenderPass)
TRIM_MARK_OBJECT_REFERENCE(RenderPass)

TRIM_ADD_OBJECT_CALL(Pipeline)
TRIM_MARK_OBJECT_REFERENCE(Pipeline)

TRIM_ADD_OBJECT_CALL(Queue)
TRIM_MARK_OBJECT_REFERENCE(Queue)

TRIM_ADD_OBJECT_CALL(Semaphore)
TRIM_MARK_OBJECT_REFERENCE(Semaphore)

TRIM_ADD_OBJECT_CALL(DeviceMemory)
TRIM_MARK_OBJECT_REFERENCE(DeviceMemory)

TRIM_ADD_OBJECT_CALL(Fence)
TRIM_MARK_OBJECT_REFERENCE(Fence)

TRIM_ADD_OBJECT_CALL(SwapchainKHR)
TRIM_MARK_OBJECT_REFERENCE(SwapchainKHR)

TRIM_ADD_OBJECT_CALL(Image)
TRIM_MARK_OBJECT_REFERENCE(Image)

TRIM_ADD_OBJECT_CALL(ImageView)
TRIM_MARK_OBJECT_REFERENCE(ImageView)

TRIM_ADD_OBJECT_CALL(Buffer)
TRIM_MARK_OBJECT_REFERENCE(Buffer)

TRIM_ADD_OBJECT_CALL(BufferView)
TRIM_MARK_OBJECT_REFERENCE(BufferView)

TRIM_ADD_OBJECT_CALL(Framebuffer)
TRIM_MARK_OBJECT_REFERENCE(Framebuffer)

TRIM_ADD_OBJECT_CALL(Event)
TRIM_MARK_OBJECT_REFERENCE(Event)

TRIM_ADD_OBJECT_CALL(QueryPool)
TRIM_MARK_OBJECT_REFERENCE(QueryPool)

TRIM_ADD_OBJECT_CALL(ShaderModule)
TRIM_MARK_OBJECT_REFERENCE(ShaderModule)

TRIM_ADD_OBJECT_CALL(PipelineLayout)
TRIM_MARK_OBJECT_REFERENCE(PipelineLayout)

TRIM_ADD_OBJECT_CALL(Sampler)
TRIM_MARK_OBJECT_REFERENCE(Sampler)

TRIM_ADD_OBJECT_CALL(DescriptorSetLayout)
TRIM_MARK_OBJECT_REFERENCE(DescriptorSetLayout)