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
#include <list>
#include <map>
#include <algorithm>
#include <mutex>
#include "vktrace_trace_packet_identifiers.h"

#include "vktrace_lib_trim_generate.h"
#include "vktrace_lib_trim_statetracker.h"
#include "vulkan/vulkan.h"

#if defined(PLATFORM_LINUX)  // VK_USE_PLATFORM_XCB_KHR
#if defined(ANDROID)
// TODO
#else
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <xcb/xcb_keysyms.h>
#include <vector>
#endif  // ANDROID
#endif

// Trim support
// Indicates whether trim support will be utilized during this instance of
// vktrace.
// Only set once based on the VKTRACE_TRIM_FRAMES env var.
extern bool g_trimEnabled;
extern bool g_trimIsPreTrim;
extern bool g_trimIsInTrim;
extern bool g_trimIsPostTrim;
extern uint64_t g_trimFrameCounter;
extern uint64_t g_trimStartFrame;
extern uint64_t g_trimEndFrame;
extern bool g_trimAlreadyFinished;

namespace trim {
void initialize();
void deinitialize();

enum enum_trim_trigger {
    none = 0,
    frameCounter,  // trim trigger base on startFrame and endFrame
    hotKey         // trim trigger base on hotKey
};

// when the funtion first time run, it Check ENV viarable VKTRACE_TRIM_TRIGGER
// to find out user defined trim trigger string and save the result,
// after first time run, the function directly get the saved result without
// checking ENV viarable.
// the feature disabled if return is nullptr, for any other value which should
// specify user defined option for that type of trigger, the feature ia enabled.
char *getTraceTriggerOptionString(enum enum_trim_trigger triggerType);

// check if specified trim trigger enabled
// return:
//  if specified trigger enabled
bool is_trim_trigger_enabled(enum enum_trim_trigger triggerType);

#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
// TODO
#else
// on Linux platform, xcb calls need Connection which is connected to target
// server, because hotkey process is supposed to insert into target application,
// so we need to capture the connection that target app use. the function is
// used to insert into Vulkan call to capture and save the connection.
void set_keyboard_connection(xcb_connection_t *pConnection);
#endif  // ANDROID
#endif

enum enum_key_state {
    Pressed,
    Released,
};

// return if hotkey triggered;
bool is_hotkey_trim_triggered();

VkDeviceSize calculateImageSubResourceSize(VkDevice device, VkImageCreateInfo imageCreateInfo,
                                           const VkAllocationCallbacks *pAllocator, uint32_t subresourceIndex);
bool calculateImageAllSubResourceSize(VkDevice device, VkImageCreateInfo imageCreateInfo, const VkAllocationCallbacks *pAllocator,
                                      std::vector<VkDeviceSize> &subResourceSizes);

void addImageSubResourceSizes(VkImage image, std::vector<VkDeviceSize> subResourceSizes);

bool getImageSubResourceSizes(VkImage image, std::vector<VkDeviceSize> *pSubresourceSizes);

void deleteImageSubResourceSizes(VkImage image);

VkDeviceSize getImageSize(VkImage image);

VkDeviceSize getImageSubResourceOffset(VkImage image, uint32_t mipLevel);

// Use this to snapshot the global state tracker at the start of the trim
// frames.
void snapshot_state_tracker();

void start();
void stop();

// Outputs object-related trace packets to the trace file.
void write_all_referenced_object_calls();
void write_packet(vktrace_trace_packet_header *pHeader);
void add_destroy_device_object_packets(VkDevice device);
void write_destroy_packets();

void add_RenderPassCreateInfo(VkRenderPass renderPass, const VkRenderPassCreateInfo *pCreateInfo);
uint32_t get_RenderPassVersion(VkRenderPass renderPass);

//-----------------------
// the following calls are pass-through to the StateTracker
void add_CommandBuffer_call(VkCommandBuffer commandBuffer, vktrace_trace_packet_header *pHeader);
void remove_CommandBuffer_calls(VkCommandBuffer commandBuffer);

#if TRIM_USE_ORDERED_IMAGE_CREATION
void add_Image_call(vktrace_trace_packet_header *pHeader);
#endif  // TRIM_USE_ORDERED_IMAGE_CREATION

void add_InTrim_call(vktrace_trace_packet_header *pHeader);

void AddImageTransition(VkCommandBuffer commandBuffer, ImageTransition transition);
std::list<ImageTransition> GetImageTransitions(VkCommandBuffer commandBuffer);
void ClearImageTransitions(VkCommandBuffer commandBuffer);

void AddBufferTransition(VkCommandBuffer commandBuffer, BufferTransition transition);
std::list<BufferTransition> GetBufferTransitions(VkCommandBuffer commandBuffer);
void ClearBufferTransitions(VkCommandBuffer commandBuffer);
// The above calls are pass-through to the StateTracker
//-----------------------

void reset_DescriptorPool(VkDescriptorPool descriptorPool);

VkMemoryPropertyFlags LookUpMemoryProperties(VkDevice device, uint32_t memoryTypeIndex);

// check if a memory type on the physical device is only DEVICE_LOCAL and not
// HOST_VISIBLE
bool IsMemoryDeviceOnly(VkDeviceMemory memory);

VkImageAspectFlags getImageAspectFromFormat(VkFormat format);

// Typically an application will have one VkAllocationCallbacks struct and
// will pass in that same address as needed, so we'll keep a map to correlate
// the supplied address to the AllocationCallbacks object
void add_Allocator(const VkAllocationCallbacks *pAllocator);
VkAllocationCallbacks *get_Allocator(const VkAllocationCallbacks *pAllocator);

void mark_Device_reference(VkDevice var);
void mark_CommandBuffer_reference(VkCommandBuffer var);
void mark_CommandPool_reference(VkCommandPool var);
void mark_ShaderModule_reference(VkShaderModule var);
void mark_Pipeline_reference(VkPipeline var);
void mark_PipelineCache_reference(VkPipelineCache var);
void mark_DescriptorPool_reference(VkDescriptorPool var);
void mark_DescriptorSet_reference(VkDescriptorSet var);
void mark_Image_reference(VkImage var);
void mark_ImageView_reference(VkImageView var);
void mark_Framebuffer_reference(VkFramebuffer var);
void mark_QueryPool_reference(VkQueryPool var);
void mark_DeviceMemory_reference(VkDeviceMemory var);
void mark_Buffer_reference(VkBuffer var);
void mark_BufferView_reference(VkBufferView var);

ObjectInfo &add_Instance_object(VkInstance var);
ObjectInfo *get_Instance_objectInfo(VkInstance var);

ObjectInfo &add_PhysicalDevice_object(VkPhysicalDevice var);
ObjectInfo *get_PhysicalDevice_objectInfo(VkPhysicalDevice var);

VkQueue get_DeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex);

ObjectInfo &add_Device_object(VkDevice var);
ObjectInfo *get_Device_objectInfo(VkDevice var);

ObjectInfo &add_SurfaceKHR_object(VkSurfaceKHR var);
ObjectInfo *get_SurfaceKHR_objectInfo(VkSurfaceKHR var);

ObjectInfo &add_CommandPool_object(VkCommandPool var);
ObjectInfo *get_CommandPool_objectInfo(VkCommandPool var);

ObjectInfo &add_CommandBuffer_object(VkCommandBuffer var);
ObjectInfo *get_CommandBuffer_objectInfo(VkCommandBuffer var);

ObjectInfo &add_DescriptorPool_object(VkDescriptorPool var);
ObjectInfo *get_DescriptorPool_objectInfo(VkDescriptorPool var);

ObjectInfo &add_RenderPass_object(VkRenderPass var);
ObjectInfo *get_RenderPass_objectInfo(VkRenderPass var);

ObjectInfo &add_PipelineCache_object(VkPipelineCache var);
ObjectInfo *get_PipelineCache_objectInfo(VkPipelineCache var);

ObjectInfo &add_Pipeline_object(VkPipeline var);
ObjectInfo *get_Pipeline_objectInfo(VkPipeline var);

ObjectInfo &add_Queue_object(VkQueue var);
ObjectInfo *get_Queue_objectInfo(VkQueue var);

ObjectInfo &add_Semaphore_object(VkSemaphore var);
ObjectInfo *get_Semaphore_objectInfo(VkSemaphore var);

ObjectInfo &add_DeviceMemory_object(VkDeviceMemory var);
ObjectInfo *get_DeviceMemory_objectInfo(VkDeviceMemory var);

ObjectInfo &add_Fence_object(VkFence var);
ObjectInfo *get_Fence_objectInfo(VkFence var);

ObjectInfo &add_SwapchainKHR_object(VkSwapchainKHR var);
ObjectInfo *get_SwapchainKHR_objectInfo(VkSwapchainKHR var);

ObjectInfo &add_Image_object(VkImage var);
ObjectInfo *get_Image_objectInfo(VkImage var);

ObjectInfo &add_ImageView_object(VkImageView var);
ObjectInfo *get_ImageView_objectInfo(VkImageView var);

ObjectInfo &add_Buffer_object(VkBuffer var);
ObjectInfo *get_Buffer_objectInfo(VkBuffer var);

ObjectInfo &add_BufferView_object(VkBufferView var);
ObjectInfo *get_BufferView_objectInfo(VkBufferView var);

ObjectInfo &add_Framebuffer_object(VkFramebuffer var);
ObjectInfo *get_Framebuffer_objectInfo(VkFramebuffer var);

ObjectInfo &add_Event_object(VkEvent var);
ObjectInfo *get_Event_objectInfo(VkEvent var);

ObjectInfo &add_QueryPool_object(VkQueryPool var);
ObjectInfo *get_QueryPool_objectInfo(VkQueryPool var);

ObjectInfo &add_ShaderModule_object(VkShaderModule var);
ObjectInfo *get_ShaderModule_objectInfo(VkShaderModule var);

ObjectInfo &add_PipelineLayout_object(VkPipelineLayout var);
ObjectInfo *get_PipelineLayout_objectInfo(VkPipelineLayout var);

ObjectInfo &add_Sampler_object(VkSampler var);
ObjectInfo *get_Sampler_objectInfo(VkSampler var);

ObjectInfo &add_DescriptorSetLayout_object(VkDescriptorSetLayout var);
ObjectInfo *get_DescriptorSetLayout_objectInfo(VkDescriptorSetLayout var);

ObjectInfo &add_DescriptorUpdateTemplate_object(VkDescriptorUpdateTemplate var);
ObjectInfo *get_DescriptorUpdateTemplate_objectInfo(VkDescriptorUpdateTemplate var);

ObjectInfo &add_DescriptorSet_object(VkDescriptorSet var);
ObjectInfo *get_DescriptorSet_objectInfo(VkDescriptorSet var);

void remove_Instance_object(const VkInstance var);
void remove_PhysicalDevice_object(const VkPhysicalDevice var);
void remove_Device_object(const VkDevice var);
void remove_SurfaceKHR_object(const VkSurfaceKHR var);
void remove_CommandPool_object(const VkCommandPool var);
void remove_CommandBuffer_object(const VkCommandBuffer var);
void remove_DescriptorPool_object(const VkDescriptorPool var);
void remove_RenderPass_object(const VkRenderPass var);
void remove_PipelineCache_object(const VkPipelineCache var);
void remove_Pipeline_object(const VkPipeline var);
void remove_Queue_object(const VkQueue var);
void remove_Semaphore_object(const VkSemaphore var);
void remove_DeviceMemory_object(const VkDeviceMemory var);
void remove_Fence_object(const VkFence var);
void remove_SwapchainKHR_object(const VkSwapchainKHR var);
void remove_Image_object(const VkImage var);
void remove_ImageView_object(const VkImageView var);
void remove_Buffer_object(const VkBuffer var);
void remove_BufferView_object(const VkBufferView var);
void remove_Framebuffer_object(const VkFramebuffer var);
void remove_Event_object(const VkEvent var);
void remove_QueryPool_object(const VkQueryPool var);
void remove_ShaderModule_object(const VkShaderModule var);
void remove_PipelineLayout_object(const VkPipelineLayout var);
void remove_Sampler_object(const VkSampler var);
void remove_DescriptorSetLayout_object(const VkDescriptorSetLayout var);
void remove_DescriptorUpdateTemplate_object(VkDescriptorUpdateTemplate var);
void remove_DescriptorSet_object(const VkDescriptorSet var);

void add_binding_Pipeline_to_CommandBuffer(VkCommandBuffer commandBuffer, VkPipeline pipeLine);
void clear_binding_Pipelines_from_CommandBuffer(VkCommandBuffer commandBuffer);
void add_CommandBuffer_to_binding_Pipeline(VkCommandBuffer commandBuffer, VkPipeline pipeLine);
void clear_CommandBuffer_calls_by_binding_Pipeline(VkPipeline pipeLine);

}  // namespace trim
