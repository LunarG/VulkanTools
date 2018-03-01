/*
 *
 * Copyright (C) 2015-2017 Valve Corporation
 * Copyright (C) 2015-2017 LunarG, Inc.
 * All Rights Reserved
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
 *
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Tobin Ehlis <tobin@lunarg.com>
 */

#pragma once

#include <set>
#include <map>
#include <vector>
#include <string>
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
#include <android_native_app_glue.h>
#else
#include <xcb/xcb.h>
#endif  // ANDROID
#endif
#include "vktrace_multiplatform.h"
#include "vkreplay_window.h"
#include "vkreplay_factory.h"
#include "vktrace_trace_packet_identifiers.h"
#include <unordered_map>

extern "C" {
#include "vktrace_vk_vk_packets.h"

// TODO138 : Need to add packets files for new wsi headers
}

#include "vulkan/vulkan.h"

#include "vk_layer_dispatch_table.h"
#include "vk_dispatch_table_helper.h"

#include "vkreplay_vkdisplay.h"
#include "vkreplay_vk_objmapper.h"

#define CHECK_RETURN_VALUE(entrypoint) returnValue = handle_replay_errors(#entrypoint, replayResult, pPacket->result, returnValue);

extern vkreplayer_settings* g_pReplaySettings;

class vkReplay {
   public:
    ~vkReplay();
    vkReplay(vkreplayer_settings* pReplaySettings, vktrace_trace_file_header* pFileHeader);

    int init(vktrace_replay::ReplayDisplay& disp);
    vkDisplay* get_display() { return m_display; }
    vktrace_replay::VKTRACE_REPLAY_RESULT replay(vktrace_trace_packet_header* packet);
    vktrace_replay::VKTRACE_REPLAY_RESULT handle_replay_errors(const char* entrypointName, const VkResult resCall,
                                                               const VkResult resTrace,
                                                               const vktrace_replay::VKTRACE_REPLAY_RESULT resIn);

    void push_validation_msg(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObjectHandle, size_t location,
                             int32_t msgCode, const char* pLayerPrefix, const char* pMsg, const void* pUserData);
    vktrace_replay::VKTRACE_REPLAY_RESULT pop_validation_msgs();
    int dump_validation_data();
    int get_frame_number() { return m_frameNumber; }
    void reset_frame_number(int frameNumber) { m_frameNumber = frameNumber > 0 ? frameNumber : 0; }

   private:
    void init_funcs(void* handle);
    void* m_libHandle;
    VkLayerInstanceDispatchTable m_vkFuncs;
    VkLayerDispatchTable m_vkDeviceFuncs;
    vkReplayObjMapper m_objMapper;
    void (*m_pDSDump)(char*);
    void (*m_pCBDump)(char*);
    // VKTRACESNAPSHOT_PRINT_OBJECTS m_pVktraceSnapshotPrint;
    vkDisplay* m_display;

    int m_frameNumber;
    vktrace_trace_file_header* m_pFileHeader;
    struct_gpuinfo* m_pGpuinfo;

    // Replay platform description
    uint64_t m_replay_endianess;
    uint64_t m_replay_ptrsize;
    uint64_t m_replay_arch;
    uint64_t m_replay_os;
    uint64_t m_replay_gpu;
    uint64_t m_replay_drv_vers;

    // Result of comparing trace platform with replay platform
    // -1: Not initialized. 0: No match. 1: Match.
    int m_platformMatch;

    struct ValidationMsg {
        VkFlags msgFlags;
        VkDebugReportObjectTypeEXT objType;
        uint64_t srcObjectHandle;
        size_t location;
        int32_t msgCode;
        char layerPrefix[256];
        char msg[256];
        void* pUserData;
    };

    VkDebugReportCallbackEXT m_dbgMsgCallbackObj;

    std::vector<struct ValidationMsg> m_validationMsgs;
    std::vector<int> m_screenshotFrames;
    VkResult manually_replay_vkCreateInstance(packet_vkCreateInstance* pPacket);
    VkResult manually_replay_vkCreateDevice(packet_vkCreateDevice* pPacket);
    VkResult manually_replay_vkCreateBuffer(packet_vkCreateBuffer* pPacket);
    VkResult manually_replay_vkCreateImage(packet_vkCreateImage* pPacket);
    VkResult manually_replay_vkCreateCommandPool(packet_vkCreateCommandPool* pPacket);
    void manually_replay_vkDestroyBuffer(packet_vkDestroyBuffer* pPacket);
    void manually_replay_vkDestroyImage(packet_vkDestroyImage* pPacket);
    VkResult manually_replay_vkEnumeratePhysicalDevices(packet_vkEnumeratePhysicalDevices* pPacket);
    // TODO138 : Many new functions in API now that we need to assess if manual code needed
    // VkResult manually_replay_vkGetPhysicalDeviceInfo(packet_vkGetPhysicalDeviceInfo* pPacket);
    // VkResult manually_replay_vkGetGlobalExtensionInfo(packet_vkGetGlobalExtensionInfo* pPacket);
    // VkResult manually_replay_vkGetPhysicalDeviceExtensionInfo(packet_vkGetPhysicalDeviceExtensionInfo* pPacket);
    VkResult manually_replay_vkQueueSubmit(packet_vkQueueSubmit* pPacket);
    VkResult manually_replay_vkQueueBindSparse(packet_vkQueueBindSparse* pPacket);
    // VkResult manually_replay_vkGetObjectInfo(packet_vkGetObjectInfo* pPacket);
    // VkResult manually_replay_vkGetImageSubresourceInfo(packet_vkGetImageSubresourceInfo* pPacket);
    void manually_replay_vkUpdateDescriptorSets(packet_vkUpdateDescriptorSets* pPacket);
    VkResult manually_replay_vkCreateDescriptorSetLayout(packet_vkCreateDescriptorSetLayout* pPacket);
    void manually_replay_vkDestroyDescriptorSetLayout(packet_vkDestroyDescriptorSetLayout* pPacket);
    VkResult manually_replay_vkAllocateDescriptorSets(packet_vkAllocateDescriptorSets* pPacket);
    VkResult manually_replay_vkFreeDescriptorSets(packet_vkFreeDescriptorSets* pPacket);
    void manually_replay_vkCmdBindDescriptorSets(packet_vkCmdBindDescriptorSets* pPacket);
    void manually_replay_vkCmdBindVertexBuffers(packet_vkCmdBindVertexBuffers* pPacket);
    VkResult manually_replay_vkGetPipelineCacheData(packet_vkGetPipelineCacheData* pPacket);
    VkResult manually_replay_vkCreateGraphicsPipelines(packet_vkCreateGraphicsPipelines* pPacket);
    VkResult manually_replay_vkCreateComputePipelines(packet_vkCreateComputePipelines* pPacket);
    VkResult manually_replay_vkCreatePipelineLayout(packet_vkCreatePipelineLayout* pPacket);
    void manually_replay_vkCmdWaitEvents(packet_vkCmdWaitEvents* pPacket);
    void manually_replay_vkCmdPipelineBarrier(packet_vkCmdPipelineBarrier* pPacket);
    VkResult manually_replay_vkCreateFramebuffer(packet_vkCreateFramebuffer* pPacket);
    VkResult manually_replay_vkCreateRenderPass(packet_vkCreateRenderPass* pPacket);
    void manually_replay_vkCmdBeginRenderPass(packet_vkCmdBeginRenderPass* pPacket);
    VkResult manually_replay_vkBeginCommandBuffer(packet_vkBeginCommandBuffer* pPacket);
    VkResult manually_replay_vkAllocateCommandBuffers(packet_vkAllocateCommandBuffers* pPacket);
    VkResult manually_replay_vkWaitForFences(packet_vkWaitForFences* pPacket);
    VkResult manually_replay_vkAllocateMemory(packet_vkAllocateMemory* pPacket);
    void manually_replay_vkFreeMemory(packet_vkFreeMemory* pPacket);
    VkResult manually_replay_vkMapMemory(packet_vkMapMemory* pPacket);
    void manually_replay_vkUnmapMemory(packet_vkUnmapMemory* pPacket);
    VkResult manually_replay_vkFlushMappedMemoryRanges(packet_vkFlushMappedMemoryRanges* pPacket);
    VkResult manually_replay_vkInvalidateMappedMemoryRanges(packet_vkInvalidateMappedMemoryRanges* pPacket);
    void manually_replay_vkGetPhysicalDeviceMemoryProperties(packet_vkGetPhysicalDeviceMemoryProperties* pPacket);
    void manually_replay_vkGetPhysicalDeviceMemoryProperties2KHR(packet_vkGetPhysicalDeviceMemoryProperties2KHR* pPacket);
    void manually_replay_vkGetPhysicalDeviceQueueFamilyProperties(packet_vkGetPhysicalDeviceQueueFamilyProperties* pPacket);
    void manually_replay_vkGetPhysicalDeviceQueueFamilyProperties2KHR(packet_vkGetPhysicalDeviceQueueFamilyProperties2KHR* pPacket);
    void manually_replay_vkGetPhysicalDeviceSparseImageFormatProperties(
        packet_vkGetPhysicalDeviceSparseImageFormatProperties* pPacket);
    void manually_replay_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(
        packet_vkGetPhysicalDeviceSparseImageFormatProperties2KHR* pPacket);
    void manually_replay_vkGetImageMemoryRequirements(packet_vkGetImageMemoryRequirements* pPacket);
    void manually_replay_vkGetImageMemoryRequirements2KHR(packet_vkGetImageMemoryRequirements2KHR* pPacket);
    void manually_replay_vkGetBufferMemoryRequirements(packet_vkGetBufferMemoryRequirements* pPacket);
    void manually_replay_vkGetBufferMemoryRequirements2KHR(packet_vkGetBufferMemoryRequirements2KHR* pPacket);
    void manually_replay_vkGetPhysicalDeviceProperties(packet_vkGetPhysicalDeviceProperties* pPacket);
    void manually_replay_vkGetPhysicalDeviceProperties2KHR(packet_vkGetPhysicalDeviceProperties2KHR* pPacket);
    VkResult manually_replay_vkGetPhysicalDeviceSurfaceSupportKHR(packet_vkGetPhysicalDeviceSurfaceSupportKHR* pPacket);
    VkResult manually_replay_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(packet_vkGetPhysicalDeviceSurfaceCapabilitiesKHR* pPacket);
    VkResult manually_replay_vkGetPhysicalDeviceSurfaceFormatsKHR(packet_vkGetPhysicalDeviceSurfaceFormatsKHR* pPacket);
    VkResult manually_replay_vkGetPhysicalDeviceSurfacePresentModesKHR(packet_vkGetPhysicalDeviceSurfacePresentModesKHR* pPacket);
    VkResult manually_replay_vkCreateSwapchainKHR(packet_vkCreateSwapchainKHR* pPacket);
    void manually_replay_vkDestroySwapchainKHR(packet_vkDestroySwapchainKHR* pPacket);
    VkResult manually_replay_vkGetSwapchainImagesKHR(packet_vkGetSwapchainImagesKHR* pPacket);
    VkResult manually_replay_vkQueuePresentKHR(packet_vkQueuePresentKHR* pPacket);
    VkResult manually_replay_vkCreateXcbSurfaceKHR(packet_vkCreateXcbSurfaceKHR* pPacket);
    VkBool32 manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR(
        packet_vkGetPhysicalDeviceXcbPresentationSupportKHR* pPacket);
    VkResult manually_replay_vkCreateXlibSurfaceKHR(packet_vkCreateXlibSurfaceKHR* pPacket);
    VkBool32 manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR(
        packet_vkGetPhysicalDeviceXlibPresentationSupportKHR* pPacket);
    VkResult manually_replay_vkCreateWaylandSurfaceKHR(packet_vkCreateWaylandSurfaceKHR* pPacket);
    VkBool32 manually_replay_vkGetPhysicalDeviceWaylandPresentationSupportKHR(
        packet_vkGetPhysicalDeviceWaylandPresentationSupportKHR* pPacket);
    VkResult manually_replay_vkCreateWin32SurfaceKHR(packet_vkCreateWin32SurfaceKHR* pPacket);
    VkBool32 manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR(
        packet_vkGetPhysicalDeviceWin32PresentationSupportKHR* pPacket);
    VkResult manually_replay_vkCreateAndroidSurfaceKHR(packet_vkCreateAndroidSurfaceKHR* pPacket);
    VkResult manually_replay_vkCreateDebugReportCallbackEXT(packet_vkCreateDebugReportCallbackEXT* pPacket);
    void manually_replay_vkDestroyDebugReportCallbackEXT(packet_vkDestroyDebugReportCallbackEXT* pPacket);
    VkResult manually_replay_vkCreateDescriptorUpdateTemplateKHR(packet_vkCreateDescriptorUpdateTemplateKHR* pPacket);
    void manually_replay_vkDestroyDescriptorUpdateTemplateKHR(packet_vkDestroyDescriptorUpdateTemplateKHR* pPacket);
    void manually_replay_vkUpdateDescriptorSetWithTemplateKHR(packet_vkUpdateDescriptorSetWithTemplateKHR* pPacket);
    void manually_replay_vkCmdPushDescriptorSetWithTemplateKHR(packet_vkCmdPushDescriptorSetWithTemplateKHR* pPacket);
    VkResult manually_replay_vkBindBufferMemory(packet_vkBindBufferMemory* pPacket);
    VkResult manually_replay_vkBindImageMemory(packet_vkBindImageMemory *pPacket);
    VkResult manually_replay_vkRegisterDeviceEventEXT(packet_vkRegisterDeviceEventEXT *pPacket);
    VkResult manually_replay_vkRegisterDisplayEventEXT(packet_vkRegisterDisplayEventEXT *pPacket);
    VkResult manually_replay_vkCreateObjectTableNVX(packet_vkCreateObjectTableNVX *pPacket);
    void manually_replay_vkCmdProcessCommandsNVX(packet_vkCmdProcessCommandsNVX *pPacket);
    VkResult manually_replay_vkCreateIndirectCommandsLayoutNVX(packet_vkCreateIndirectCommandsLayoutNVX *pPacket);
    VkResult manually_replay_vkBindBufferMemory2KHR(packet_vkBindBufferMemory2KHR* pPacket);
    VkResult manually_replay_vkBindImageMemory2KHR(packet_vkBindImageMemory2KHR* pPacket);
    VkResult manually_replay_vkGetDisplayPlaneSupportedDisplaysKHR(packet_vkGetDisplayPlaneSupportedDisplaysKHR* pPacket);
    VkResult manually_replay_vkEnumerateDeviceExtensionProperties(packet_vkEnumerateDeviceExtensionProperties* pPacket);

    void process_screenshot_list(const char* list) {
        std::string spec(list), word;
        size_t start = 0, comma = 0;

        while (start < spec.size()) {
            comma = spec.find(',', start);

            if (comma == std::string::npos)
                word = std::string(spec, start);
            else
                word = std::string(spec, start, comma - start);

            m_screenshotFrames.push_back(atoi(word.c_str()));
            if (comma == std::string::npos) break;

            start = comma + 1;
        }
    }

    struct QueueFamilyProperties {
        uint32_t count;
        VkQueueFamilyProperties* queueFamilyProperties;
    };

    // Map VkPhysicalDevice to QueueFamilyPropeties (and ultimately queue indices)
    std::unordered_map<VkPhysicalDevice, struct QueueFamilyProperties> traceQueueFamilyProperties;
    std::unordered_map<VkPhysicalDevice, struct QueueFamilyProperties> replayQueueFamilyProperties;

    // Map VkDevice to a VkPhysicalDevice
    std::unordered_map<VkDevice, VkPhysicalDevice> tracePhysicalDevices;
    std::unordered_map<VkDevice, VkPhysicalDevice> replayPhysicalDevices;

    // Map VkBuffer to VkDevice, so we can search for the VkDevice used to create a buffer
    std::unordered_map<VkBuffer, VkDevice> traceBufferToDevice;
    std::unordered_map<VkBuffer, VkDevice> replayBufferToDevice;

    // Map VkImage to VkDevice, so we can search for the VkDevice used to create an image
    std::unordered_map<VkImage, VkDevice> traceImageToDevice;
    std::unordered_map<VkImage, VkDevice> replayImageToDevice;

    // Map VkSwapchainKHR to vector of VkImage, so we can unmap swapchain images at vkDestroySwapchainKHR
    std::unordered_map<VkSwapchainKHR, std::vector<VkImage>> traceSwapchainToImages;

    // Map VkPhysicalDevice to VkPhysicalDeviceMemoryProperites
    std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceMemoryProperties> traceMemoryProperties;
    std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceMemoryProperties> replayMemoryProperties;

    // Map VkImage to VkMemoryRequirements
    std::unordered_map<VkImage, VkMemoryRequirements> replayGetImageMemoryRequirements;

    // Map VkBuffer to VkMemoryRequirements
    std::unordered_map<VkBuffer, VkMemoryRequirements> replayGetBufferMemoryRequirements;

    // Map device to extension property count, for device extension property queries
    std::unordered_map<VkPhysicalDevice, uint32_t> replayDeviceExtensionPropertyCount;

    bool getMemoryTypeIdx(VkDevice traceDevice, VkDevice replayDevice, uint32_t traceIdx, VkMemoryRequirements* memRequirements,
                          uint32_t* pReplayIdx);

    bool getQueueFamilyIdx(VkPhysicalDevice tracePhysicalDevice, VkPhysicalDevice replayPhysicalDevice, uint32_t traceIdx,
                           uint32_t* pReplayIdx);
    bool getQueueFamilyIdx(VkDevice traceDevice, VkDevice replayDevice, uint32_t traceIdx, uint32_t* pReplayIdx);

    void remapHandlesInDescriptorSetWithTemplateData(VkDescriptorUpdateTemplateKHR remappedDescriptorUpdateTemplate, char* pData);
};
