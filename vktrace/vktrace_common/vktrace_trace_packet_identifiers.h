/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
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
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 **************************************************************************/
#pragma once

#include "vktrace_common.h"

#define VKTRACE_TRACE_FILE_VERSION_2 0x0002
#define VKTRACE_TRACE_FILE_VERSION_3 0x0003
#define VKTRACE_TRACE_FILE_VERSION_4 0x0004
#define VKTRACE_TRACE_FILE_VERSION_5 0x0005
#define VKTRACE_TRACE_FILE_VERSION_6 0x0006
#define VKTRACE_TRACE_FILE_VERSION_7 0x0007  // Vulkan 1.1
#define VKTRACE_TRACE_FILE_VERSION VKTRACE_TRACE_FILE_VERSION_7

// vkreplay can replay version 6 (the last Vulkan 1.0 format)
#define VKTRACE_TRACE_FILE_VERSION_MINIMUM_COMPATIBLE VKTRACE_TRACE_FILE_VERSION_6

#define VKTRACE_FILE_MAGIC 0xABADD068ADEAFD0C

#define VKTRACE_MAX_TRACER_ID_ARRAY_SIZE 16  // Should be multiple of 8

typedef enum VKTRACE_TRACER_ID {
    VKTRACE_TID_RESERVED = 0,
    VKTRACE_TID_GL_FPS,
    VKTRACE_TID_VULKAN
    // Max enum must be less than VKTRACE_MAX_TRACER_ID_ARRAY_SIZE
} VKTRACE_TRACER_ID;

typedef struct VKTRACE_TRACER_REPLAYER_INFO {
    VKTRACE_TRACER_ID tracerId;
    BOOL needsReplayer;
    const char* const replayerLibraryName;
    const char* const debuggerLibraryname;
} VKTRACE_TRACER_REPLAYER_INFO;

// The index here should match the value of the VKTRACE_TRACER_ID
static const VKTRACE_TRACER_REPLAYER_INFO gs_tracerReplayerInfo[VKTRACE_MAX_TRACER_ID_ARRAY_SIZE] = {
    {VKTRACE_TID_RESERVED, FALSE, "", ""},
    {VKTRACE_TID_GL_FPS, FALSE, "", ""},
    {VKTRACE_TID_VULKAN, TRUE, VKTRACE_LIBRARY_NAME(vulkan_replay), VKTRACE_LIBRARY_NAME(vktraceviewer_vk)},
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
    {VKTRACE_TID_RESERVED, FALSE, "", ""},  // this can be updated as new tracers are added
};

// Note: Changing the following enum values will break backwards-compatibility with previously generated traces.  New values should
//       be appended, deprecated values should be left in or skipped.
typedef enum _VKTRACE_TRACE_PACKET_ID_VK {
    VKTRACE_TPI_MESSAGE = 0,
    VKTRACE_TPI_MARKER_CHECKPOINT = 1,
    VKTRACE_TPI_MARKER_API_BOUNDARY = 2,
    VKTRACE_TPI_MARKER_API_GROUP_BEGIN = 3,
    VKTRACE_TPI_MARKER_API_GROUP_END = 4,
    VKTRACE_TPI_MARKER_TERMINATE_PROCESS = 5,
    VKTRACE_TPI_PORTABILITY_TABLE = 6,
    VKTRACE_TPI_VK_vkApiVersion = 7,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalImageFormatPropertiesNV = 8,
    VKTRACE_TPI_VK_vkCmdDrawIndirectCountAMD = 9,
    VKTRACE_TPI_VK_vkCmdDrawIndexedIndirectCountAMD = 10,
    VKTRACE_TPI_VK_vkCreateDebugReportCallbackEXT = 11,
    VKTRACE_TPI_VK_vkDestroyDebugReportCallbackEXT = 12,
    VKTRACE_TPI_VK_vkDebugReportMessageEXT = 13,
    VKTRACE_TPI_VK_vkDebugMarkerSetObjectTagEXT = 14,
    VKTRACE_TPI_VK_vkDebugMarkerSetObjectNameEXT = 15,
    VKTRACE_TPI_VK_vkCmdDebugMarkerBeginEXT = 16,
    VKTRACE_TPI_VK_vkCmdDebugMarkerEndEXT = 17,
    VKTRACE_TPI_VK_vkCmdDebugMarkerInsertEXT = 18,
    VKTRACE_TPI_VK_vkCreateInstance = 19,
    VKTRACE_TPI_VK_vkDestroyInstance = 20,
    VKTRACE_TPI_VK_vkEnumeratePhysicalDevices = 21,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceFeatures = 22,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceFormatProperties = 23,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceImageFormatProperties = 24,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceProperties = 25,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceQueueFamilyProperties = 26,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceMemoryProperties = 27,
    VKTRACE_TPI_VK_vkGetInstanceProcAddr = 28,
    VKTRACE_TPI_VK_vkGetDeviceProcAddr = 29,
    VKTRACE_TPI_VK_vkCreateDevice = 30,
    VKTRACE_TPI_VK_vkDestroyDevice = 31,
    VKTRACE_TPI_VK_vkEnumerateInstanceExtensionProperties = 32,
    VKTRACE_TPI_VK_vkEnumerateDeviceExtensionProperties = 33,
    VKTRACE_TPI_VK_vkEnumerateInstanceLayerProperties = 34,
    VKTRACE_TPI_VK_vkEnumerateDeviceLayerProperties = 35,
    VKTRACE_TPI_VK_vkGetDeviceQueue = 36,
    VKTRACE_TPI_VK_vkQueueSubmit = 37,
    VKTRACE_TPI_VK_vkQueueWaitIdle = 38,
    VKTRACE_TPI_VK_vkDeviceWaitIdle = 39,
    VKTRACE_TPI_VK_vkAllocateMemory = 40,
    VKTRACE_TPI_VK_vkFreeMemory = 41,
    VKTRACE_TPI_VK_vkMapMemory = 42,
    VKTRACE_TPI_VK_vkUnmapMemory = 43,
    VKTRACE_TPI_VK_vkFlushMappedMemoryRanges = 44,
    VKTRACE_TPI_VK_vkInvalidateMappedMemoryRanges = 45,
    VKTRACE_TPI_VK_vkGetDeviceMemoryCommitment = 46,
    VKTRACE_TPI_VK_vkBindBufferMemory = 47,
    VKTRACE_TPI_VK_vkBindImageMemory = 48,
    VKTRACE_TPI_VK_vkGetBufferMemoryRequirements = 49,
    VKTRACE_TPI_VK_vkGetImageMemoryRequirements = 50,
    VKTRACE_TPI_VK_vkGetImageSparseMemoryRequirements = 51,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSparseImageFormatProperties = 52,
    VKTRACE_TPI_VK_vkQueueBindSparse = 53,
    VKTRACE_TPI_VK_vkCreateFence = 54,
    VKTRACE_TPI_VK_vkDestroyFence = 55,
    VKTRACE_TPI_VK_vkResetFences = 56,
    VKTRACE_TPI_VK_vkGetFenceStatus = 57,
    VKTRACE_TPI_VK_vkWaitForFences = 58,
    VKTRACE_TPI_VK_vkCreateSemaphore = 59,
    VKTRACE_TPI_VK_vkDestroySemaphore = 60,
    VKTRACE_TPI_VK_vkCreateEvent = 61,
    VKTRACE_TPI_VK_vkDestroyEvent = 62,
    VKTRACE_TPI_VK_vkGetEventStatus = 63,
    VKTRACE_TPI_VK_vkSetEvent = 64,
    VKTRACE_TPI_VK_vkResetEvent = 65,
    VKTRACE_TPI_VK_vkCreateQueryPool = 66,
    VKTRACE_TPI_VK_vkDestroyQueryPool = 67,
    VKTRACE_TPI_VK_vkGetQueryPoolResults = 68,
    VKTRACE_TPI_VK_vkCreateBuffer = 69,
    VKTRACE_TPI_VK_vkDestroyBuffer = 70,
    VKTRACE_TPI_VK_vkCreateBufferView = 71,
    VKTRACE_TPI_VK_vkDestroyBufferView = 72,
    VKTRACE_TPI_VK_vkCreateImage = 73,
    VKTRACE_TPI_VK_vkDestroyImage = 74,
    VKTRACE_TPI_VK_vkGetImageSubresourceLayout = 75,
    VKTRACE_TPI_VK_vkCreateImageView = 76,
    VKTRACE_TPI_VK_vkDestroyImageView = 77,
    VKTRACE_TPI_VK_vkCreateShaderModule = 78,
    VKTRACE_TPI_VK_vkDestroyShaderModule = 79,
    VKTRACE_TPI_VK_vkCreatePipelineCache = 80,
    VKTRACE_TPI_VK_vkDestroyPipelineCache = 81,
    VKTRACE_TPI_VK_vkGetPipelineCacheData = 82,
    VKTRACE_TPI_VK_vkMergePipelineCaches = 83,
    VKTRACE_TPI_VK_vkCreateGraphicsPipelines = 84,
    VKTRACE_TPI_VK_vkCreateComputePipelines = 85,
    VKTRACE_TPI_VK_vkDestroyPipeline = 86,
    VKTRACE_TPI_VK_vkCreatePipelineLayout = 87,
    VKTRACE_TPI_VK_vkDestroyPipelineLayout = 88,
    VKTRACE_TPI_VK_vkCreateSampler = 89,
    VKTRACE_TPI_VK_vkDestroySampler = 90,
    VKTRACE_TPI_VK_vkCreateDescriptorSetLayout = 91,
    VKTRACE_TPI_VK_vkDestroyDescriptorSetLayout = 92,
    VKTRACE_TPI_VK_vkCreateDescriptorPool = 93,
    VKTRACE_TPI_VK_vkDestroyDescriptorPool = 94,
    VKTRACE_TPI_VK_vkResetDescriptorPool = 95,
    VKTRACE_TPI_VK_vkAllocateDescriptorSets = 96,
    VKTRACE_TPI_VK_vkFreeDescriptorSets = 97,
    VKTRACE_TPI_VK_vkUpdateDescriptorSets = 98,
    VKTRACE_TPI_VK_vkCreateFramebuffer = 99,
    VKTRACE_TPI_VK_vkDestroyFramebuffer = 100,
    VKTRACE_TPI_VK_vkCreateRenderPass = 101,
    VKTRACE_TPI_VK_vkDestroyRenderPass = 102,
    VKTRACE_TPI_VK_vkGetRenderAreaGranularity = 103,
    VKTRACE_TPI_VK_vkCreateCommandPool = 104,
    VKTRACE_TPI_VK_vkDestroyCommandPool = 105,
    VKTRACE_TPI_VK_vkResetCommandPool = 106,
    VKTRACE_TPI_VK_vkAllocateCommandBuffers = 107,
    VKTRACE_TPI_VK_vkFreeCommandBuffers = 108,
    VKTRACE_TPI_VK_vkBeginCommandBuffer = 109,
    VKTRACE_TPI_VK_vkEndCommandBuffer = 110,
    VKTRACE_TPI_VK_vkResetCommandBuffer = 111,
    VKTRACE_TPI_VK_vkCmdBindPipeline = 112,
    VKTRACE_TPI_VK_vkCmdSetViewport = 113,
    VKTRACE_TPI_VK_vkCmdSetScissor = 114,
    VKTRACE_TPI_VK_vkCmdSetLineWidth = 115,
    VKTRACE_TPI_VK_vkCmdSetDepthBias = 116,
    VKTRACE_TPI_VK_vkCmdSetBlendConstants = 117,
    VKTRACE_TPI_VK_vkCmdSetDepthBounds = 118,
    VKTRACE_TPI_VK_vkCmdSetStencilCompareMask = 119,
    VKTRACE_TPI_VK_vkCmdSetStencilWriteMask = 120,
    VKTRACE_TPI_VK_vkCmdSetStencilReference = 121,
    VKTRACE_TPI_VK_vkCmdBindDescriptorSets = 122,
    VKTRACE_TPI_VK_vkCmdBindIndexBuffer = 123,
    VKTRACE_TPI_VK_vkCmdBindVertexBuffers = 124,
    VKTRACE_TPI_VK_vkCmdDraw = 125,
    VKTRACE_TPI_VK_vkCmdDrawIndexed = 126,
    VKTRACE_TPI_VK_vkCmdDrawIndirect = 127,
    VKTRACE_TPI_VK_vkCmdDrawIndexedIndirect = 128,
    VKTRACE_TPI_VK_vkCmdDispatch = 129,
    VKTRACE_TPI_VK_vkCmdDispatchIndirect = 130,
    VKTRACE_TPI_VK_vkCmdCopyBuffer = 131,
    VKTRACE_TPI_VK_vkCmdCopyImage = 132,
    VKTRACE_TPI_VK_vkCmdBlitImage = 133,
    VKTRACE_TPI_VK_vkCmdCopyBufferToImage = 134,
    VKTRACE_TPI_VK_vkCmdCopyImageToBuffer = 135,
    VKTRACE_TPI_VK_vkCmdUpdateBuffer = 136,
    VKTRACE_TPI_VK_vkCmdFillBuffer = 137,
    VKTRACE_TPI_VK_vkCmdClearColorImage = 138,
    VKTRACE_TPI_VK_vkCmdClearDepthStencilImage = 139,
    VKTRACE_TPI_VK_vkCmdClearAttachments = 140,
    VKTRACE_TPI_VK_vkCmdResolveImage = 141,
    VKTRACE_TPI_VK_vkCmdSetEvent = 142,
    VKTRACE_TPI_VK_vkCmdResetEvent = 143,
    VKTRACE_TPI_VK_vkCmdWaitEvents = 144,
    VKTRACE_TPI_VK_vkCmdPipelineBarrier = 145,
    VKTRACE_TPI_VK_vkCmdBeginQuery = 146,
    VKTRACE_TPI_VK_vkCmdEndQuery = 147,
    VKTRACE_TPI_VK_vkCmdResetQueryPool = 148,
    VKTRACE_TPI_VK_vkCmdWriteTimestamp = 149,
    VKTRACE_TPI_VK_vkCmdCopyQueryPoolResults = 150,
    VKTRACE_TPI_VK_vkCmdPushConstants = 151,
    VKTRACE_TPI_VK_vkCmdBeginRenderPass = 152,
    VKTRACE_TPI_VK_vkCmdNextSubpass = 153,
    VKTRACE_TPI_VK_vkCmdEndRenderPass = 154,
    VKTRACE_TPI_VK_vkCmdExecuteCommands = 155,
    VKTRACE_TPI_VK_vkDestroySurfaceKHR = 156,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfaceSupportKHR = 157,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = 158,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfaceFormatsKHR = 159,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfacePresentModesKHR = 160,
    VKTRACE_TPI_VK_vkCreateSwapchainKHR = 161,
    VKTRACE_TPI_VK_vkDestroySwapchainKHR = 162,
    VKTRACE_TPI_VK_vkGetSwapchainImagesKHR = 163,
    VKTRACE_TPI_VK_vkAcquireNextImageKHR = 164,
    VKTRACE_TPI_VK_vkQueuePresentKHR = 165,
    VKTRACE_TPI_VK_vkCreateSharedSwapchainsKHR = 166,
    VKTRACE_TPI_VK_vkCreateWin32SurfaceKHR = 167,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceWin32PresentationSupportKHR = 168,
    VKTRACE_TPI_VK_vkCreateXlibSurfaceKHR = 169,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceXlibPresentationSupportKHR = 170,
    VKTRACE_TPI_VK_vkCreateXcbSurfaceKHR = 171,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceXcbPresentationSupportKHR = 172,
    VKTRACE_TPI_VK_vkCreateAndroidSurfaceKHR = 173,
    VKTRACE_TPI_VK_vkGetMemoryWin32HandleNV = 174,
    VKTRACE_TPI_VK_vkCreateDescriptorUpdateTemplateKHR = 175,
    VKTRACE_TPI_VK_vkDestroyDescriptorUpdateTemplateKHR = 176,
    VKTRACE_TPI_VK_vkUpdateDescriptorSetWithTemplateKHR = 177,
    VKTRACE_TPI_VK_vkCmdPushDescriptorSetWithTemplateKHR = 178,
    VKTRACE_TPI_VK_vkCreateWaylandSurfaceKHR = 179,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceWaylandPresentationSupportKHR = 180,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceFeatures2KHR = 181,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceProperties2KHR = 182,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceFormatProperties2KHR = 183,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceImageFormatProperties2KHR = 184,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceQueueFamilyProperties2KHR = 185,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceMemoryProperties2KHR = 186,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSparseImageFormatProperties2KHR = 187,
    VKTRACE_TPI_VK_vkGetImageMemoryRequirements2KHR = 188,
    VKTRACE_TPI_VK_vkGetBufferMemoryRequirements2KHR = 189,
    VKTRACE_TPI_VK_vkGetImageSparseMemoryRequirements2KHR = 190,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfaceCapabilities2KHR = 191,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfaceFormats2KHR = 192,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceDisplayPropertiesKHR = 193,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceDisplayPlanePropertiesKHR = 194,
    VKTRACE_TPI_VK_vkGetDisplayModePropertiesKHR = 195,
    VKTRACE_TPI_VK_vkCreateDisplayModeKHR = 196,
    VKTRACE_TPI_VK_vkGetDisplayPlaneCapabilitiesKHR = 197,
    VKTRACE_TPI_VK_vkCreateDisplayPlaneSurfaceKHR = 198,
    VKTRACE_TPI_VK_vkTrimCommandPoolKHR = 199,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalBufferPropertiesKHR = 200,
    VKTRACE_TPI_VK_vkGetMemoryFdKHR = 201,
    VKTRACE_TPI_VK_vkGetMemoryFdPropertiesKHR = 202,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = 203,
    VKTRACE_TPI_VK_vkImportSemaphoreFdKHR = 204,
    VKTRACE_TPI_VK_vkGetSemaphoreFdKHR = 205,
    VKTRACE_TPI_VK_vkCmdPushDescriptorSetKHR = 206,
    VKTRACE_TPI_VK_vkGetSwapchainStatusKHR = 207,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalFencePropertiesKHR = 208,
    VKTRACE_TPI_VK_vkGetDeviceGroupPeerMemoryFeaturesKHX = 209,
    VKTRACE_TPI_VK_vkBindBufferMemory2KHR = 210,
    VKTRACE_TPI_VK_vkBindImageMemory2KHR = 211,
    VKTRACE_TPI_VK_vkCmdSetDeviceMaskKHX = 212,
    VKTRACE_TPI_VK_vkGetDeviceGroupPresentCapabilitiesKHX = 213,
    VKTRACE_TPI_VK_vkGetDeviceGroupSurfacePresentModesKHX = 214,
    VKTRACE_TPI_VK_vkAcquireNextImage2KHX = 215,
    VKTRACE_TPI_VK_vkCmdDispatchBaseKHX = 216,
    VKTRACE_TPI_VK_vkGetPhysicalDevicePresentRectanglesKHX = 217,
    VKTRACE_TPI_VK_vkEnumeratePhysicalDeviceGroupsKHX = 218,
    VKTRACE_TPI_VK_vkCmdProcessCommandsNVX = 219,
    VKTRACE_TPI_VK_vkCmdReserveSpaceForCommandsNVX = 220,
    VKTRACE_TPI_VK_vkCreateIndirectCommandsLayoutNVX = 221,
    VKTRACE_TPI_VK_vkDestroyIndirectCommandsLayoutNVX = 222,
    VKTRACE_TPI_VK_vkCreateObjectTableNVX = 223,
    VKTRACE_TPI_VK_vkDestroyObjectTableNVX = 224,
    VKTRACE_TPI_VK_vkRegisterObjectsNVX = 225,
    VKTRACE_TPI_VK_vkUnregisterObjectsNVX = 226,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX = 227,
    VKTRACE_TPI_VK_vkCmdSetViewportWScalingNV = 228,
    VKTRACE_TPI_VK_vkReleaseDisplayEXT = 229,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSurfaceCapabilities2EXT = 230,
    VKTRACE_TPI_VK_vkGetRefreshCycleDurationGOOGLE = 231,
    VKTRACE_TPI_VK_vkGetPastPresentationTimingGOOGLE = 232,
    VKTRACE_TPI_VK_vkCmdSetDiscardRectangleEXT = 233,
    VKTRACE_TPI_VK_vkSetHdrMetadataEXT = 234,
    VKTRACE_TPI_VK_vkAcquireXlibDisplayEXT = 235,
    VKTRACE_TPI_VK_vkGetRandROutputDisplayEXT = 236,
    VKTRACE_TPI_VK_vkDisplayPowerControlEXT = 237,
    VKTRACE_TPI_VK_vkRegisterDeviceEventEXT = 238,
    VKTRACE_TPI_VK_vkRegisterDisplayEventEXT = 239,
    VKTRACE_TPI_VK_vkGetSwapchainCounterEXT = 240,
    VKTRACE_TPI_VK_vkCmdSetSampleLocationsEXT = 241,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceMultisamplePropertiesEXT = 242,
    VKTRACE_TPI_VK_vkCreateSamplerYcbcrConversionKHR = 243,
    VKTRACE_TPI_VK_vkDestroySamplerYcbcrConversionKHR = 244,
    VKTRACE_TPI_VK_vkGetDisplayPlaneSupportedDisplaysKHR = 245,
    VKTRACE_TPI_VK_vkImportFenceFdKHR = 246,
    VKTRACE_TPI_VK_vkGetFenceFdKHR = 247,
    VKTRACE_TPI_VK_vkImportFenceWin32HandleKHR = 248,
    VKTRACE_TPI_VK_vkGetFenceWin32HandleKHR = 249,
    VKTRACE_TPI_VK_vkGetMemoryWin32HandleKHR = 250,
    VKTRACE_TPI_VK_vkGetMemoryWin32HandlePropertiesKHR = 251,
    VKTRACE_TPI_VK_vkImportSemaphoreWin32HandleKHR = 252,
    VKTRACE_TPI_VK_vkGetSemaphoreWin32HandleKHR = 253,
    VKTRACE_TPI_VK_vkGetMemoryHostPointerPropertiesEXT = 254,
    VKTRACE_TPI_VK_vkCreateValidationCacheEXT = 255,
    VKTRACE_TPI_VK_vkDestroyValidationCacheEXT = 256,
    VKTRACE_TPI_VK_vkMergeValidationCachesEXT = 257,
    VKTRACE_TPI_VK_vkGetValidationCacheDataEXT = 258,
    VKTRACE_TPI_VK_vkGetShaderInfoAMD = 259,
    VKTRACE_TPI_VK_vkGetDeviceQueue2 = 260,
    VKTRACE_TPI_VK_vkEnumerateInstanceVersion = 261,
    VKTRACE_TPI_VK_vkGetDescriptorSetLayoutSupport = 262,
    VKTRACE_TPI_VK_vkGetDeviceGroupPresentCapabilitiesKHR = 263,
    VKTRACE_TPI_VK_vkGetDeviceGroupSurfacePresentModesKHR = 264,
    VKTRACE_TPI_VK_vkGetPhysicalDevicePresentRectanglesKHR = 265,
    VKTRACE_TPI_VK_vkAcquireNextImage2KHR = 266,
    VKTRACE_TPI_VK_vkBindBufferMemory2 = 267,
    VKTRACE_TPI_VK_vkBindImageMemory2 = 268,
    VKTRACE_TPI_VK_vkGetDeviceGroupPeerMemoryFeatures = 269,
    VKTRACE_TPI_VK_vkCmdSetDeviceMask = 270,
    VKTRACE_TPI_VK_vkCmdDispatchBase = 271,
    VKTRACE_TPI_VK_vkEnumeratePhysicalDeviceGroups = 272,
    VKTRACE_TPI_VK_vkGetImageMemoryRequirements2 = 273,
    VKTRACE_TPI_VK_vkGetBufferMemoryRequirements2 = 274,
    VKTRACE_TPI_VK_vkGetImageSparseMemoryRequirements2 = 275,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceFeatures2 = 276,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceProperties2 = 277,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceFormatProperties2 = 278,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceImageFormatProperties2 = 279,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceQueueFamilyProperties2 = 280,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceMemoryProperties2 = 281,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceSparseImageFormatProperties2 = 282,
    VKTRACE_TPI_VK_vkTrimCommandPool = 283,
    VKTRACE_TPI_VK_vkCreateSamplerYcbcrConversion = 284,
    VKTRACE_TPI_VK_vkDestroySamplerYcbcrConversion = 285,
    VKTRACE_TPI_VK_vkCreateDescriptorUpdateTemplate = 286,
    VKTRACE_TPI_VK_vkDestroyDescriptorUpdateTemplate = 287,
    VKTRACE_TPI_VK_vkUpdateDescriptorSetWithTemplate = 288,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalBufferProperties = 289,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalFenceProperties = 290,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceExternalSemaphoreProperties = 291,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceDisplayProperties2KHR = 292,
    VKTRACE_TPI_VK_vkGetPhysicalDeviceDisplayPlaneProperties2KHR = 293,
    VKTRACE_TPI_VK_vkGetDisplayModeProperties2KHR = 294,
    VKTRACE_TPI_VK_vkGetDisplayPlaneCapabilities2KHR = 295,
    VKTRACE_TPI_VK_vkCmdDrawIndirectCount = 296,
    VKTRACE_TPI_VK_vkCmdDrawIndexedIndirectCount = 297,
    VKTRACE_TPI_VK_vkCreateRenderPass2 = 298,
    VKTRACE_TPI_VK_vkCmdBeginRenderPass2 = 299,
    VKTRACE_TPI_VK_vkCmdNextSubpass2 =300,
    VKTRACE_TPI_VK_vkCmdEndRenderPass2 = 301,
    VKTRACE_TPI_VK_vkResetQueryPool = 302,
    VKTRACE_TPI_VK_vkGetSemaphoreCounterValue = 303,
    VKTRACE_TPI_VK_vkWaitSemaphores = 304,
    VKTRACE_TPI_VK_vkSignalSemaphore = 305,
    VKTRACE_TPI_VK_vkGetBufferDeviceAddress = 306,
    VKTRACE_TPI_VK_vkGetBufferOpaqueCaptureAddress = 307,
    VKTRACE_TPI_VK_vkGetDeviceMemoryOpaqueCaptureAddress = 308,
} VKTRACE_TRACE_PACKET_ID_VK;

#define VKTRACE_BIG_ENDIAN 1
#define VKTRACE_LITTLE_ENDIAN 0

typedef struct {
    uint8_t id;
    uint8_t is_64_bit;
} vktrace_tracer_info;

// vktrace_trace_file_header and vktrace_trace_packet_header are written and read
// by both host and target CPU, which can be different architectures.
// All fields in these structs are 64-bit aligned.

#if defined(WIN32)
#define ALIGN8 __declspec(align(8))
#else
#define ALIGN8 __attribute__((aligned(8)))
#endif

typedef struct {
    ALIGN8 uint64_t gpu_id;
    ALIGN8 uint64_t gpu_drv_vers;
} struct_gpuinfo;

typedef struct {
    uint16_t trace_file_version;
    uint16_t reserved1[3];
    ALIGN8 uint64_t magic;
    uint32_t uuid[4];
    ALIGN8 uint64_t first_packet_offset;  // Size of header including gpu array below
    ALIGN8 uint64_t tracer_count;         // number of tracers referenced in this trace file
    vktrace_tracer_info
        tracer_id_array[VKTRACE_MAX_TRACER_ID_ARRAY_SIZE];  // array of tracer_ids and values which are referenced in the trace file
    ALIGN8 uint64_t trace_start_time;
    ALIGN8 uint64_t portability_table_valid;

    // Description of platform on which this trace was created
    ALIGN8 uint64_t endianess;
    ALIGN8 uint64_t ptrsize;
    ALIGN8 uint64_t arch;
    ALIGN8 uint64_t os;

    // Reserve some spaece in case more fields need to be added in the future
    ALIGN8 uint64_t reserved2[8];

    // The header ends with number of gpus and a gpu_id/drv_vers pair for each gpu
    ALIGN8 uint64_t n_gpuinfo;
    // A struct_gpuinfo array of length n_gpuinfo follows this
} vktrace_trace_file_header;

typedef struct {
    ALIGN8 uint64_t size;  // total size, including extra data, needed to get to the next packet_header
    ALIGN8 uint64_t global_packet_index;
    uint8_t tracer_id;   // TODO: need to uniquely identify tracers in a way that is known by the replayer
    uint16_t packet_id;  // VKTRACE_TRACE_PACKET_ID_VK
    uint32_t thread_id;
    ALIGN8 uint64_t vktrace_begin_time;  // start of measuring vktrace's overhead related to this packet
    ALIGN8 uint64_t entrypoint_begin_time;
    ALIGN8 uint64_t entrypoint_end_time;
    ALIGN8 uint64_t vktrace_end_time;     // end of measuring vktrace's overhead related to this packet
    ALIGN8 uint64_t next_buffers_offset;  // used for tracking the addition of buffers to the trace packet
    ALIGN8 uintptr_t pBody;               // points to the body of the packet
} vktrace_trace_packet_header;

typedef struct {
    vktrace_trace_packet_header* pHeader;
    VktraceLogLevel type;
    uint32_t length;
    char* message;
} vktrace_trace_packet_message;

typedef struct {
    vktrace_trace_packet_header* pHeader;
    unsigned int length;
    char* label;
} vktrace_trace_packet_marker_checkpoint;

typedef vktrace_trace_packet_marker_checkpoint vktrace_trace_packet_marker_api_boundary;
typedef vktrace_trace_packet_marker_checkpoint vktrace_trace_packet_marker_api_group_begin;
typedef vktrace_trace_packet_marker_checkpoint vktrace_trace_packet_marker_api_group_end;

typedef VKTRACE_TRACER_ID(VKTRACER_CDECL* funcptr_VKTRACE_GetTracerId)();
