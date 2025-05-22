
/* Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (c) 2015-2016, 2019 Google Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#pragma once

#include "api_dump.h"

void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents, const char* pnext_type);
void dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);


void dump_text_VkClearColorValue(const VkClearColorValue& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClearValue(const VkClearValue& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterResultKHR(const VkPerformanceCounterResultKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkDeviceOrHostAddressConstAMDX(const VkDeviceOrHostAddressConstAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkDeviceOrHostAddressKHR(const VkDeviceOrHostAddressKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceOrHostAddressConstKHR(const VkDeviceOrHostAddressConstKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryDataKHR(const VkAccelerationStructureGeometryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceValueDataINTEL(const VkPerformanceValueDataINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineExecutableStatisticValueKHR(const VkPipelineExecutableStatisticValueKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorDataEXT(const VkDescriptorDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInstanceDataNV(const VkAccelerationStructureMotionInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureOpInputNV(const VkClusterAccelerationStructureOpInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetInfoEXT(const VkIndirectExecutionSetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsTokenDataEXT(const VkIndirectCommandsTokenDataEXT& object, const ApiDumpSettings& settings, int indents);

//=========================== Type Implementations ==========================//

void dump_text_uint64_t(uint64_t object, const ApiDumpSettings& settings, int indents);
void dump_text_size_t(size_t object, const ApiDumpSettings& settings, int indents);
void dump_text_char(char object, const ApiDumpSettings& settings, int indents);
void dump_text_float(float object, const ApiDumpSettings& settings, int indents);
void dump_text_int64_t(int64_t object, const ApiDumpSettings& settings, int indents);
void dump_text_double(double object, const ApiDumpSettings& settings, int indents);

//========================= Basetype Implementations ========================//

void dump_text_VkBool32(VkBool32 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceAddress(VkDeviceAddress object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceSize(VkDeviceSize object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFlags(VkFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSampleMask(VkSampleMask object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFlags64(VkFlags64 object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_MTLDevice_id(MTLDevice_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_MTLCommandQueue_id(MTLCommandQueue_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_MTLBuffer_id(MTLBuffer_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_MTLTexture_id(MTLTexture_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_IOSurfaceRef(IOSurfaceRef object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_MTLSharedEvent_id(MTLSharedEvent_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkRemoteAddressNV(VkRemoteAddressNV object, const ApiDumpSettings& settings, int indents);


#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_ANativeWindow(const ANativeWindow* object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_AHardwareBuffer(const AHardwareBuffer* object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_ANDROID_KHR


#if defined(VK_USE_PLATFORM_METAL_EXT)
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_CAMetalLayer(CAMetalLayer object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_METAL_EXT

//======================= System Type Implementations =======================//

#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_DWORD(const DWORD object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_Display(const Display* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_GgpFrameToken(const GgpFrameToken object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_GgpStreamDescriptor(const GgpStreamDescriptor object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_HANDLE(const HANDLE object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_HINSTANCE(const HINSTANCE object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_HMONITOR(const HMONITOR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_HWND(const HWND object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_IDirectFB(const IDirectFB object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_IDirectFBSurface(const IDirectFBSurface object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_LPCWSTR(const LPCWSTR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_SECURITY_ATTRIBUTES(const SECURITY_ATTRIBUTES* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_VisualID(const VisualID object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_Window(const Window object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text__screen_buffer(const _screen_buffer* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text__screen_context(const _screen_context* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text__screen_window(const _screen_window* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_wl_display(const wl_display* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_wl_surface(const wl_surface* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_xcb_connection_t(const xcb_connection_t* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_xcb_visualid_t(const xcb_visualid_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_xcb_window_t(const xcb_window_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_zx_handle_t(const zx_handle_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA

//========================== Handle Implementations =========================//

void dump_text_VkBuffer(const VkBuffer object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImage(const VkImage object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInstance(const VkInstance object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevice(const VkPhysicalDevice object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDevice(const VkDevice object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueue(const VkQueue object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphore(const VkSemaphore object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBuffer(const VkCommandBuffer object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFence(const VkFence object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceMemory(const VkDeviceMemory object, const ApiDumpSettings& settings, int indents);
void dump_text_VkEvent(const VkEvent object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPool(const VkQueryPool object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferView(const VkBufferView object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageView(const VkImageView object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderModule(const VkShaderModule object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCache(const VkPipelineCache object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineLayout(const VkPipelineLayout object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipeline(const VkPipeline object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPass(const VkRenderPass object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayout(const VkDescriptorSetLayout object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSampler(const VkSampler object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSet(const VkDescriptorSet object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPool(const VkDescriptorPool object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebuffer(const VkFramebuffer object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPool(const VkCommandPool object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrConversion(const VkSamplerYcbcrConversion object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorUpdateTemplate(const VkDescriptorUpdateTemplate object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPrivateDataSlot(const VkPrivateDataSlot object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceKHR(const VkSurfaceKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainKHR(const VkSwapchainKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayKHR(const VkDisplayKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModeKHR(const VkDisplayModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugReportCallbackEXT(const VkDebugReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionKHR(const VkVideoSessionKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionParametersKHR(const VkVideoSessionParametersKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCuModuleNVX(const VkCuModuleNVX object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCuFunctionNVX(const VkCuFunctionNVX object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessengerEXT(const VkDebugUtilsMessengerEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureKHR(const VkAccelerationStructureKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeferredOperationKHR(const VkDeferredOperationKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationCacheEXT(const VkValidationCacheEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureNV(const VkAccelerationStructureNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceConfigurationINTEL(const VkPerformanceConfigurationINTEL object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkCudaModuleNV(const VkCudaModuleNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkCudaFunctionNV(const VkCudaFunctionNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferCollectionFUCHSIA(const VkBufferCollectionFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_text_VkMicromapEXT(const VkMicromapEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowSessionNV(const VkOpticalFlowSessionNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderEXT(const VkShaderEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryKHR(const VkPipelineBinaryKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalComputeQueueNV(const VkExternalComputeQueueNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetEXT(const VkIndirectExecutionSetEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutEXT(const VkIndirectCommandsLayoutEXT object, const ApiDumpSettings& settings, int indents);

//=========================== Enum Implementations ==========================//

void dump_text_VkResult(VkResult object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStructureType(VkStructureType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCacheHeaderVersion(VkPipelineCacheHeaderVersion object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageLayout(VkImageLayout object, const ApiDumpSettings& settings, int indents);
void dump_text_VkObjectType(VkObjectType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVendorId(VkVendorId object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSystemAllocationScope(VkSystemAllocationScope object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInternalAllocationType(VkInternalAllocationType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormat(VkFormat object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageTiling(VkImageTiling object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageType(VkImageType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceType(VkPhysicalDeviceType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryType(VkQueryType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSharingMode(VkSharingMode object, const ApiDumpSettings& settings, int indents);
void dump_text_VkComponentSwizzle(VkComponentSwizzle object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewType(VkImageViewType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBlendFactor(VkBlendFactor object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBlendOp(VkBlendOp object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCompareOp(VkCompareOp object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDynamicState(VkDynamicState object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFrontFace(VkFrontFace object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVertexInputRate(VkVertexInputRate object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPrimitiveTopology(VkPrimitiveTopology object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPolygonMode(VkPolygonMode object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStencilOp(VkStencilOp object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLogicOp(VkLogicOp object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBorderColor(VkBorderColor object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFilter(VkFilter object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerAddressMode(VkSamplerAddressMode object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerMipmapMode(VkSamplerMipmapMode object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorType(VkDescriptorType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentLoadOp(VkAttachmentLoadOp object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentStoreOp(VkAttachmentStoreOp object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBindPoint(VkPipelineBindPoint object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferLevel(VkCommandBufferLevel object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndexType(VkIndexType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassContents(VkSubpassContents object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPointClippingBehavior(VkPointClippingBehavior object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTessellationDomainOrigin(VkTessellationDomainOrigin object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrModelConversion(VkSamplerYcbcrModelConversion object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrRange(VkSamplerYcbcrRange object, const ApiDumpSettings& settings, int indents);
void dump_text_VkChromaLocation(VkChromaLocation object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorUpdateTemplateType(VkDescriptorUpdateTemplateType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDriverId(VkDriverId object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderFloatControlsIndependence(VkShaderFloatControlsIndependence object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerReductionMode(VkSamplerReductionMode object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreType(VkSemaphoreType object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRobustnessBufferBehavior(VkPipelineRobustnessBufferBehavior object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRobustnessImageBehavior(VkPipelineRobustnessImageBehavior object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueGlobalPriority(VkQueueGlobalPriority object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLineRasterizationMode(VkLineRasterizationMode object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentModeKHR(VkPresentModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkColorSpaceKHR(VkColorSpaceKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugReportObjectTypeEXT(VkDebugReportObjectTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRasterizationOrderAMD(VkRasterizationOrderAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryResultStatusKHR(VkQueryResultStatusKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderInfoTypeAMD(VkShaderInfoTypeAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationCheckEXT(VkValidationCheckEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPowerStateEXT(VkDisplayPowerStateEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceEventTypeEXT(VkDeviceEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayEventTypeEXT(VkDisplayEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkViewportCoordinateSwizzleNV(VkViewportCoordinateSwizzleNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDiscardRectangleModeEXT(VkDiscardRectangleModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkConservativeRasterizationModeEXT(VkConservativeRasterizationModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterUnitKHR(VkPerformanceCounterUnitKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterScopeKHR(VkPerformanceCounterScopeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterStorageKHR(VkPerformanceCounterStorageKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBlendOverlapEXT(VkBlendOverlapEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureTypeKHR(VkAccelerationStructureTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildAccelerationStructureModeKHR(VkBuildAccelerationStructureModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryTypeKHR(VkGeometryTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureBuildTypeKHR(VkAccelerationStructureBuildTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyAccelerationStructureModeKHR(VkCopyAccelerationStructureModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureCompatibilityKHR(VkAccelerationStructureCompatibilityKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCoverageModulationModeNV(VkCoverageModulationModeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationCacheHeaderVersionEXT(VkValidationCacheHeaderVersionEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShadingRatePaletteEntryNV(VkShadingRatePaletteEntryNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCoarseSampleOrderTypeNV(VkCoarseSampleOrderTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingShaderGroupTypeKHR(VkRayTracingShaderGroupTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMemoryRequirementsTypeNV(VkAccelerationStructureMemoryRequirementsTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTimeDomainKHR(VkTimeDomainKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryOverallocationBehaviorAMD(VkMemoryOverallocationBehaviorAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceConfigurationTypeINTEL(VkPerformanceConfigurationTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPoolSamplingModeINTEL(VkQueryPoolSamplingModeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceOverrideTypeINTEL(VkPerformanceOverrideTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceParameterTypeINTEL(VkPerformanceParameterTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceValueTypeINTEL(VkPerformanceValueTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFragmentShadingRateCombinerOpKHR(VkFragmentShadingRateCombinerOpKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationFeatureEnableEXT(VkValidationFeatureEnableEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationFeatureDisableEXT(VkValidationFeatureDisableEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkComponentTypeKHR(VkComponentTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkScopeKHR(VkScopeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCoverageReductionModeNV(VkCoverageReductionModeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkProvokingVertexModeEXT(VkProvokingVertexModeEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkFullScreenExclusiveEXT(VkFullScreenExclusiveEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkPipelineExecutableStatisticFormatKHR(VkPipelineExecutableStatisticFormatKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsTokenTypeNV(VkIndirectCommandsTokenTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDepthBiasRepresentationEXT(VkDepthBiasRepresentationEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceMemoryReportEventTypeEXT(VkDeviceMemoryReportEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeTuningModeKHR(VkVideoEncodeTuningModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFragmentShadingRateTypeNV(VkFragmentShadingRateTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFragmentShadingRateNV(VkFragmentShadingRateNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInstanceTypeNV(VkAccelerationStructureMotionInstanceTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultAddressTypeEXT(VkDeviceFaultAddressTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultVendorBinaryHeaderVersionEXT(VkDeviceFaultVendorBinaryHeaderVersionEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderGroupShaderKHR(VkShaderGroupShaderKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceAddressBindingTypeEXT(VkDeviceAddressBindingTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapTypeEXT(VkMicromapTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildMicromapModeEXT(VkBuildMicromapModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMicromapModeEXT(VkCopyMicromapModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpacityMicromapFormatEXT(VkOpacityMicromapFormatEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpacityMicromapSpecialIndexEXT(VkOpacityMicromapSpecialIndexEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkDisplacementMicromapFormatNV(VkDisplacementMicromapFormatNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkRayTracingLssIndexingModeNV(VkRayTracingLssIndexingModeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingLssPrimitiveEndCapsModeNV(VkRayTracingLssPrimitiveEndCapsModeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassMergeStatusEXT(VkSubpassMergeStatusEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDirectDriverLoadingModeLUNARG(VkDirectDriverLoadingModeLUNARG object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowPerformanceLevelNV(VkOpticalFlowPerformanceLevelNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowSessionBindingPointNV(VkOpticalFlowSessionBindingPointNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAntiLagModeAMD(VkAntiLagModeAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAntiLagStageAMD(VkAntiLagStageAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderCodeTypeEXT(VkShaderCodeTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDepthClampModeEXT(VkDepthClampModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingInvocationReorderModeNV(VkRayTracingInvocationReorderModeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCooperativeVectorMatrixLayoutNV(VkCooperativeVectorMatrixLayoutNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLayerSettingTypeEXT(VkLayerSettingTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLatencyMarkerNV(VkLatencyMarkerNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOutOfBandQueueTypeNV(VkOutOfBandQueueTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1PredictionModeKHR(VkVideoEncodeAV1PredictionModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1RateControlGroupKHR(VkVideoEncodeAV1RateControlGroupKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBlockMatchWindowCompareModeQCOM(VkBlockMatchWindowCompareModeQCOM object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCubicFilterWeightsQCOM(VkCubicFilterWeightsQCOM object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLayeredDriverUnderlyingApiMSFT(VkLayeredDriverUnderlyingApiMSFT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplaySurfaceStereoTypeNV(VkDisplaySurfaceStereoTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLayeredApiKHR(VkPhysicalDeviceLayeredApiKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureTypeNV(VkClusterAccelerationStructureTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureOpTypeNV(VkClusterAccelerationStructureOpTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureOpModeNV(VkClusterAccelerationStructureOpModeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureOpTypeNV(VkPartitionedAccelerationStructureOpTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetInfoTypeEXT(VkIndirectExecutionSetInfoTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsTokenTypeEXT(VkIndirectCommandsTokenTypeEXT object, const ApiDumpSettings& settings, int indents);

//========================= Bitmask Implementations =========================//

void dump_text_VkAccessFlagBits(VkAccessFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageAspectFlagBits(VkImageAspectFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormatFeatureFlagBits(VkFormatFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCreateFlagBits(VkImageCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSampleCountFlagBits(VkSampleCountFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageUsageFlagBits(VkImageUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInstanceCreateFlagBits(VkInstanceCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryHeapFlagBits(VkMemoryHeapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryPropertyFlagBits(VkMemoryPropertyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFlagBits(VkQueueFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceQueueCreateFlagBits(VkDeviceQueueCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineStageFlagBits(VkPipelineStageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryMapFlagBits(VkMemoryMapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseMemoryBindFlagBits(VkSparseMemoryBindFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageFormatFlagBits(VkSparseImageFormatFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFenceCreateFlagBits(VkFenceCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkEventCreateFlagBits(VkEventCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPipelineStatisticFlagBits(VkQueryPipelineStatisticFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryResultFlagBits(VkQueryResultFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferCreateFlagBits(VkBufferCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferUsageFlagBits(VkBufferUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewCreateFlagBits(VkImageViewCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCacheCreateFlagBits(VkPipelineCacheCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkColorComponentFlagBits(VkColorComponentFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreateFlagBits(VkPipelineCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineShaderStageCreateFlagBits(VkPipelineShaderStageCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderStageFlagBits(VkShaderStageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCullModeFlagBits(VkCullModeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDepthStencilStateCreateFlagBits(VkPipelineDepthStencilStateCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineColorBlendStateCreateFlagBits(VkPipelineColorBlendStateCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineLayoutCreateFlagBits(VkPipelineLayoutCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerCreateFlagBits(VkSamplerCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPoolCreateFlagBits(VkDescriptorPoolCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutCreateFlagBits(VkDescriptorSetLayoutCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentDescriptionFlagBits(VkAttachmentDescriptionFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDependencyFlagBits(VkDependencyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebufferCreateFlagBits(VkFramebufferCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreateFlagBits(VkRenderPassCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDescriptionFlagBits(VkSubpassDescriptionFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPoolCreateFlagBits(VkCommandPoolCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPoolResetFlagBits(VkCommandPoolResetFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferUsageFlagBits(VkCommandBufferUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryControlFlagBits(VkQueryControlFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferResetFlagBits(VkCommandBufferResetFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStencilFaceFlagBits(VkStencilFaceFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubgroupFeatureFlagBits(VkSubgroupFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPeerMemoryFeatureFlagBits(VkPeerMemoryFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryAllocateFlagBits(VkMemoryAllocateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryHandleTypeFlagBits(VkExternalMemoryHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryFeatureFlagBits(VkExternalMemoryFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalFenceHandleTypeFlagBits(VkExternalFenceHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalFenceFeatureFlagBits(VkExternalFenceFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFenceImportFlagBits(VkFenceImportFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreImportFlagBits(VkSemaphoreImportFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalSemaphoreHandleTypeFlagBits(VkExternalSemaphoreHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalSemaphoreFeatureFlagBits(VkExternalSemaphoreFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkResolveModeFlagBits(VkResolveModeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorBindingFlagBits(VkDescriptorBindingFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreWaitFlagBits(VkSemaphoreWaitFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreationFeedbackFlagBits(VkPipelineCreationFeedbackFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkToolPurposeFlagBits(VkToolPurposeFlagBits object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkPipelineStageFlagBits2;
void dump_text_VkPipelineStageFlagBits2(VkPipelineStageFlagBits2 object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkAccessFlagBits2;
void dump_text_VkAccessFlagBits2(VkAccessFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubmitFlagBits(VkSubmitFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingFlagBits(VkRenderingFlagBits object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkFormatFeatureFlagBits2;
void dump_text_VkFormatFeatureFlagBits2(VkFormatFeatureFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryUnmapFlagBits(VkMemoryUnmapFlagBits object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkPipelineCreateFlagBits2;
void dump_text_VkPipelineCreateFlagBits2(VkPipelineCreateFlagBits2 object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkBufferUsageFlagBits2;
void dump_text_VkBufferUsageFlagBits2(VkBufferUsageFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHostImageCopyFlagBits(VkHostImageCopyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceTransformFlagBitsKHR(VkSurfaceTransformFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCompositeAlphaFlagBitsKHR(VkCompositeAlphaFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainCreateFlagBitsKHR(VkSwapchainCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupPresentModeFlagBitsKHR(VkDeviceGroupPresentModeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlaneAlphaFlagBitsKHR(VkDisplayPlaneAlphaFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugReportFlagBitsEXT(VkDebugReportFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCodecOperationFlagBitsKHR(VkVideoCodecOperationFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoChromaSubsamplingFlagBitsKHR(VkVideoChromaSubsamplingFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoComponentBitDepthFlagBitsKHR(VkVideoComponentBitDepthFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCapabilityFlagBitsKHR(VkVideoCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionCreateFlagBitsKHR(VkVideoSessionCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionParametersCreateFlagBitsKHR(VkVideoSessionParametersCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCodingControlFlagBitsKHR(VkVideoCodingControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeCapabilityFlagBitsKHR(VkVideoDecodeCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeUsageFlagBitsKHR(VkVideoDecodeUsageFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264CapabilityFlagBitsKHR(VkVideoEncodeH264CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264StdFlagBitsKHR(VkVideoEncodeH264StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264RateControlFlagBitsKHR(VkVideoEncodeH264RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265CapabilityFlagBitsKHR(VkVideoEncodeH265CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265StdFlagBitsKHR(VkVideoEncodeH265StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265CtbSizeFlagBitsKHR(VkVideoEncodeH265CtbSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265TransformBlockSizeFlagBitsKHR(VkVideoEncodeH265TransformBlockSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265RateControlFlagBitsKHR(VkVideoEncodeH265RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264PictureLayoutFlagBitsKHR(VkVideoDecodeH264PictureLayoutFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryHandleTypeFlagBitsNV(VkExternalMemoryHandleTypeFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryFeatureFlagBitsNV(VkExternalMemoryFeatureFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkConditionalRenderingFlagBitsEXT(VkConditionalRenderingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceCounterFlagBitsEXT(VkSurfaceCounterFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterDescriptionFlagBitsKHR(VkPerformanceCounterDescriptionFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAcquireProfilingLockFlagBitsKHR(VkAcquireProfilingLockFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessageSeverityFlagBitsEXT(VkDebugUtilsMessageSeverityFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessageTypeFlagBitsEXT(VkDebugUtilsMessageTypeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildAccelerationStructureFlagBitsKHR(VkBuildAccelerationStructureFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryFlagBitsKHR(VkGeometryFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryInstanceFlagBitsKHR(VkGeometryInstanceFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureCreateFlagBitsKHR(VkAccelerationStructureCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCompilerControlFlagBitsAMD(VkPipelineCompilerControlFlagBitsAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderCorePropertiesFlagBitsAMD(VkShaderCorePropertiesFlagBitsAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentScalingFlagBitsEXT(VkPresentScalingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentGravityFlagBitsEXT(VkPresentGravityFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectStateFlagBitsNV(VkIndirectStateFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutUsageFlagBitsNV(VkIndirectCommandsLayoutUsageFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeFlagBitsKHR(VkVideoEncodeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeCapabilityFlagBitsKHR(VkVideoEncodeCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeRateControlModeFlagBitsKHR(VkVideoEncodeRateControlModeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeFeedbackFlagBitsKHR(VkVideoEncodeFeedbackFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeUsageFlagBitsKHR(VkVideoEncodeUsageFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeContentFlagBitsKHR(VkVideoEncodeContentFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceDiagnosticsConfigFlagBitsNV(VkDeviceDiagnosticsConfigFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTileShadingRenderPassFlagBitsQCOM(VkTileShadingRenderPassFlagBitsQCOM object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalObjectTypeFlagBitsEXT(VkExportMetalObjectTypeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkGraphicsPipelineLibraryFlagBitsEXT(VkGraphicsPipelineLibraryFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCompressionFlagBitsEXT(VkImageCompressionFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCompressionFixedRateFlagBitsEXT(VkImageCompressionFixedRateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceAddressBindingFlagBitsEXT(VkDeviceAddressBindingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImageConstraintsInfoFlagBitsFUCHSIA(VkImageConstraintsInfoFlagBitsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_text_VkFrameBoundaryFlagBitsEXT(VkFrameBoundaryFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildMicromapFlagBitsEXT(VkBuildMicromapFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapCreateFlagBitsEXT(VkMicromapCreateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkPhysicalDeviceSchedulingControlsFlagBitsARM;
void dump_text_VkPhysicalDeviceSchedulingControlsFlagBitsARM(VkPhysicalDeviceSchedulingControlsFlagBitsARM object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkMemoryDecompressionMethodFlagBitsNV;
void dump_text_VkMemoryDecompressionMethodFlagBitsNV(VkMemoryDecompressionMethodFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowGridSizeFlagBitsNV(VkOpticalFlowGridSizeFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowUsageFlagBitsNV(VkOpticalFlowUsageFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowSessionCreateFlagBitsNV(VkOpticalFlowSessionCreateFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowExecuteFlagBitsNV(VkOpticalFlowExecuteFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderCreateFlagBitsEXT(VkShaderCreateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1CapabilityFlagBitsKHR(VkVideoEncodeAV1CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1StdFlagBitsKHR(VkVideoEncodeAV1StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1SuperblockSizeFlagBitsKHR(VkVideoEncodeAV1SuperblockSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1RateControlFlagBitsKHR(VkVideoEncodeAV1RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureAddressResolutionFlagBitsNV(VkClusterAccelerationStructureAddressResolutionFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureClusterFlagBitsNV(VkClusterAccelerationStructureClusterFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureGeometryFlagBitsNV(VkClusterAccelerationStructureGeometryFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureIndexFormatFlagBitsNV(VkClusterAccelerationStructureIndexFormatFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureInstanceFlagBitsNV(VkPartitionedAccelerationStructureInstanceFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsInputModeFlagBitsEXT(VkIndirectCommandsInputModeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutUsageFlagBitsEXT(VkIndirectCommandsLayoutUsageFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
// 64 bit bitmasks don't have an enum of bit values.
typedef VkFlags64 VkAccessFlagBits3KHR;
void dump_text_VkAccessFlagBits3KHR(VkAccessFlagBits3KHR object, const ApiDumpSettings& settings, int indents);

//=========================== Flag Implementations ==========================//

void dump_text_VkAccessFlags(VkAccessFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageAspectFlags(VkImageAspectFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormatFeatureFlags(VkFormatFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCreateFlags(VkImageCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSampleCountFlags(VkSampleCountFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageUsageFlags(VkImageUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInstanceCreateFlags(VkInstanceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryHeapFlags(VkMemoryHeapFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryPropertyFlags(VkMemoryPropertyFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFlags(VkQueueFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceQueueCreateFlags(VkDeviceQueueCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineStageFlags(VkPipelineStageFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryMapFlags(VkMemoryMapFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseMemoryBindFlags(VkSparseMemoryBindFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageFormatFlags(VkSparseImageFormatFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFenceCreateFlags(VkFenceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkEventCreateFlags(VkEventCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPipelineStatisticFlags(VkQueryPipelineStatisticFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryResultFlags(VkQueryResultFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferCreateFlags(VkBufferCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferUsageFlags(VkBufferUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewCreateFlags(VkImageViewCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCacheCreateFlags(VkPipelineCacheCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkColorComponentFlags(VkColorComponentFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreateFlags(VkPipelineCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineShaderStageCreateFlags(VkPipelineShaderStageCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCullModeFlags(VkCullModeFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDepthStencilStateCreateFlags(VkPipelineDepthStencilStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineColorBlendStateCreateFlags(VkPipelineColorBlendStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineLayoutCreateFlags(VkPipelineLayoutCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderStageFlags(VkShaderStageFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerCreateFlags(VkSamplerCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPoolCreateFlags(VkDescriptorPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutCreateFlags(VkDescriptorSetLayoutCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentDescriptionFlags(VkAttachmentDescriptionFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDependencyFlags(VkDependencyFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebufferCreateFlags(VkFramebufferCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreateFlags(VkRenderPassCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDescriptionFlags(VkSubpassDescriptionFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPoolCreateFlags(VkCommandPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPoolResetFlags(VkCommandPoolResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferUsageFlags(VkCommandBufferUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryControlFlags(VkQueryControlFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferResetFlags(VkCommandBufferResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStencilFaceFlags(VkStencilFaceFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubgroupFeatureFlags(VkSubgroupFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPeerMemoryFeatureFlags(VkPeerMemoryFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryAllocateFlags(VkMemoryAllocateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryHandleTypeFlags(VkExternalMemoryHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryFeatureFlags(VkExternalMemoryFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalFenceHandleTypeFlags(VkExternalFenceHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalFenceFeatureFlags(VkExternalFenceFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFenceImportFlags(VkFenceImportFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreImportFlags(VkSemaphoreImportFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalSemaphoreHandleTypeFlags(VkExternalSemaphoreHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalSemaphoreFeatureFlags(VkExternalSemaphoreFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkResolveModeFlags(VkResolveModeFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorBindingFlags(VkDescriptorBindingFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreWaitFlags(VkSemaphoreWaitFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreationFeedbackFlags(VkPipelineCreationFeedbackFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkToolPurposeFlags(VkToolPurposeFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineStageFlags2(VkPipelineStageFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccessFlags2(VkAccessFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubmitFlags(VkSubmitFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingFlags(VkRenderingFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormatFeatureFlags2(VkFormatFeatureFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryUnmapFlags(VkMemoryUnmapFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreateFlags2(VkPipelineCreateFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferUsageFlags2(VkBufferUsageFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHostImageCopyFlags(VkHostImageCopyFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCompositeAlphaFlagsKHR(VkCompositeAlphaFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceTransformFlagsKHR(VkSurfaceTransformFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainCreateFlagsKHR(VkSwapchainCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupPresentModeFlagsKHR(VkDeviceGroupPresentModeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlaneAlphaFlagsKHR(VkDisplayPlaneAlphaFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugReportFlagsEXT(VkDebugReportFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCodecOperationFlagsKHR(VkVideoCodecOperationFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoChromaSubsamplingFlagsKHR(VkVideoChromaSubsamplingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoComponentBitDepthFlagsKHR(VkVideoComponentBitDepthFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCapabilityFlagsKHR(VkVideoCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionCreateFlagsKHR(VkVideoSessionCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionParametersCreateFlagsKHR(VkVideoSessionParametersCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCodingControlFlagsKHR(VkVideoCodingControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeCapabilityFlagsKHR(VkVideoDecodeCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeUsageFlagsKHR(VkVideoDecodeUsageFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264CapabilityFlagsKHR(VkVideoEncodeH264CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264StdFlagsKHR(VkVideoEncodeH264StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264RateControlFlagsKHR(VkVideoEncodeH264RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265CapabilityFlagsKHR(VkVideoEncodeH265CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265StdFlagsKHR(VkVideoEncodeH265StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265CtbSizeFlagsKHR(VkVideoEncodeH265CtbSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265TransformBlockSizeFlagsKHR(VkVideoEncodeH265TransformBlockSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265RateControlFlagsKHR(VkVideoEncodeH265RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264PictureLayoutFlagsKHR(VkVideoDecodeH264PictureLayoutFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryHandleTypeFlagsNV(VkExternalMemoryHandleTypeFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryFeatureFlagsNV(VkExternalMemoryFeatureFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkConditionalRenderingFlagsEXT(VkConditionalRenderingFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceCounterFlagsEXT(VkSurfaceCounterFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterDescriptionFlagsKHR(VkPerformanceCounterDescriptionFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAcquireProfilingLockFlagsKHR(VkAcquireProfilingLockFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessageTypeFlagsEXT(VkDebugUtilsMessageTypeFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessageSeverityFlagsEXT(VkDebugUtilsMessageSeverityFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildAccelerationStructureFlagsKHR(VkBuildAccelerationStructureFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryFlagsKHR(VkGeometryFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryInstanceFlagsKHR(VkGeometryInstanceFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureCreateFlagsKHR(VkAccelerationStructureCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCompilerControlFlagsAMD(VkPipelineCompilerControlFlagsAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderCorePropertiesFlagsAMD(VkShaderCorePropertiesFlagsAMD object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentScalingFlagsEXT(VkPresentScalingFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentGravityFlagsEXT(VkPresentGravityFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectStateFlagsNV(VkIndirectStateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutUsageFlagsNV(VkIndirectCommandsLayoutUsageFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeFlagsKHR(VkVideoEncodeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeCapabilityFlagsKHR(VkVideoEncodeCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeRateControlModeFlagsKHR(VkVideoEncodeRateControlModeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeFeedbackFlagsKHR(VkVideoEncodeFeedbackFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeUsageFlagsKHR(VkVideoEncodeUsageFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeContentFlagsKHR(VkVideoEncodeContentFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceDiagnosticsConfigFlagsNV(VkDeviceDiagnosticsConfigFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTileShadingRenderPassFlagsQCOM(VkTileShadingRenderPassFlagsQCOM object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalObjectTypeFlagsEXT(VkExportMetalObjectTypeFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkGraphicsPipelineLibraryFlagsEXT(VkGraphicsPipelineLibraryFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCompressionFlagsEXT(VkImageCompressionFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCompressionFixedRateFlagsEXT(VkImageCompressionFixedRateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceAddressBindingFlagsEXT(VkDeviceAddressBindingFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImageConstraintsInfoFlagsFUCHSIA(VkImageConstraintsInfoFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_text_VkFrameBoundaryFlagsEXT(VkFrameBoundaryFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildMicromapFlagsEXT(VkBuildMicromapFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapCreateFlagsEXT(VkMicromapCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSchedulingControlsFlagsARM(VkPhysicalDeviceSchedulingControlsFlagsARM object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryDecompressionMethodFlagsNV(VkMemoryDecompressionMethodFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowGridSizeFlagsNV(VkOpticalFlowGridSizeFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowUsageFlagsNV(VkOpticalFlowUsageFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowSessionCreateFlagsNV(VkOpticalFlowSessionCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowExecuteFlagsNV(VkOpticalFlowExecuteFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderCreateFlagsEXT(VkShaderCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1CapabilityFlagsKHR(VkVideoEncodeAV1CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1StdFlagsKHR(VkVideoEncodeAV1StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1SuperblockSizeFlagsKHR(VkVideoEncodeAV1SuperblockSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1RateControlFlagsKHR(VkVideoEncodeAV1RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureAddressResolutionFlagsNV(VkClusterAccelerationStructureAddressResolutionFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureClusterFlagsNV(VkClusterAccelerationStructureClusterFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureGeometryFlagsNV(VkClusterAccelerationStructureGeometryFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureIndexFormatFlagsNV(VkClusterAccelerationStructureIndexFormatFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureInstanceFlagsNV(VkPartitionedAccelerationStructureInstanceFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsInputModeFlagsEXT(VkIndirectCommandsInputModeFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutUsageFlagsEXT(VkIndirectCommandsLayoutUsageFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccessFlags3KHR(VkAccessFlags3KHR object, const ApiDumpSettings& settings, int indents);


void dump_text_VkDeviceCreateFlags(VkDeviceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreCreateFlags(VkSemaphoreCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPoolCreateFlags(VkQueryPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferViewCreateFlags(VkBufferViewCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderModuleCreateFlags(VkShaderModuleCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineVertexInputStateCreateFlags(VkPipelineVertexInputStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineInputAssemblyStateCreateFlags(VkPipelineInputAssemblyStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineTessellationStateCreateFlags(VkPipelineTessellationStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportStateCreateFlags(VkPipelineViewportStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationStateCreateFlags(VkPipelineRasterizationStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineMultisampleStateCreateFlags(VkPipelineMultisampleStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDynamicStateCreateFlags(VkPipelineDynamicStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPoolResetFlags(VkDescriptorPoolResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPoolTrimFlags(VkCommandPoolTrimFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorUpdateTemplateCreateFlags(VkDescriptorUpdateTemplateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPrivateDataSlotCreateFlags(VkPrivateDataSlotCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModeCreateFlagsKHR(VkDisplayModeCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplaySurfaceCreateFlagsKHR(VkDisplaySurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_VkXlibSurfaceCreateFlagsKHR(VkXlibSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_VkXcbSurfaceCreateFlagsKHR(VkXcbSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_VkWaylandSurfaceCreateFlagsKHR(VkWaylandSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidSurfaceCreateFlagsKHR(VkAndroidSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkWin32SurfaceCreateFlagsKHR(VkWin32SurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkVideoBeginCodingFlagsKHR(VkVideoBeginCodingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEndCodingFlagsKHR(VkVideoEndCodingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeFlagsKHR(VkVideoDecodeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationStateStreamCreateFlagsEXT(VkPipelineRasterizationStateStreamCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_VkStreamDescriptorSurfaceCreateFlagsGGP(VkStreamDescriptorSurfaceCreateFlagsGGP object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_text_VkViSurfaceCreateFlagsNN(VkViSurfaceCreateFlagsNN object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_VI_NN
void dump_text_VkPipelineViewportSwizzleStateCreateFlagsNV(VkPipelineViewportSwizzleStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDiscardRectangleStateCreateFlagsEXT(VkPipelineDiscardRectangleStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationConservativeStateCreateFlagsEXT(VkPipelineRasterizationConservativeStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationDepthClipStateCreateFlagsEXT(VkPipelineRasterizationDepthClipStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_text_VkIOSSurfaceCreateFlagsMVK(VkIOSSurfaceCreateFlagsMVK object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_text_VkMacOSSurfaceCreateFlagsMVK(VkMacOSSurfaceCreateFlagsMVK object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_text_VkDebugUtilsMessengerCallbackDataFlagsEXT(VkDebugUtilsMessengerCallbackDataFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessengerCreateFlagsEXT(VkDebugUtilsMessengerCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCoverageToColorStateCreateFlagsNV(VkPipelineCoverageToColorStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCoverageModulationStateCreateFlagsNV(VkPipelineCoverageModulationStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationCacheCreateFlagsEXT(VkValidationCacheCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImagePipeSurfaceCreateFlagsFUCHSIA(VkImagePipeSurfaceCreateFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkMetalSurfaceCreateFlagsEXT(VkMetalSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkPipelineCoverageReductionStateCreateFlagsNV(VkPipelineCoverageReductionStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHeadlessSurfaceCreateFlagsEXT(VkHeadlessSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceMemoryReportFlagsEXT(VkDeviceMemoryReportFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeRateControlFlagsKHR(VkVideoEncodeRateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInfoFlagsNV(VkAccelerationStructureMotionInfoFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInstanceFlagsNV(VkAccelerationStructureMotionInstanceFlagsNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_VkDirectFBSurfaceCreateFlagsEXT(VkDirectFBSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImageFormatConstraintsFlagsFUCHSIA(VkImageFormatConstraintsFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkScreenSurfaceCreateFlagsQNX(VkScreenSurfaceCreateFlagsQNX object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_VkDirectDriverLoadingFlagsLUNARG(VkDirectDriverLoadingFlagsLUNARG object, const ApiDumpSettings& settings, int indents);

//======================= Func Pointer Implementations ======================//

void dump_text_PFN_vkAllocationFunction(PFN_vkAllocationFunction object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkFreeFunction(PFN_vkFreeFunction object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkInternalAllocationNotification(PFN_vkInternalAllocationNotification object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkInternalFreeNotification(PFN_vkInternalFreeNotification object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkReallocationFunction(PFN_vkReallocationFunction object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkVoidFunction(PFN_vkVoidFunction object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkDebugReportCallbackEXT(PFN_vkDebugReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkDebugUtilsMessengerCallbackEXT(PFN_vkDebugUtilsMessengerCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkDeviceMemoryReportCallbackEXT(PFN_vkDeviceMemoryReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_text_PFN_vkGetInstanceProcAddrLUNARG(PFN_vkGetInstanceProcAddrLUNARG object, const ApiDumpSettings& settings, int indents);

//========================== Struct Implementations =========================//

void dump_text_VkExtent2D(const VkExtent2D& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExtent3D(const VkExtent3D& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOffset2D(const VkOffset2D& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOffset3D(const VkOffset3D& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRect2D(const VkRect2D& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBaseInStructure(const VkBaseInStructure& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBaseOutStructure(const VkBaseOutStructure& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferMemoryBarrier(const VkBufferMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDispatchIndirectCommand(const VkDispatchIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrawIndexedIndirectCommand(const VkDrawIndexedIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrawIndirectCommand(const VkDrawIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageSubresourceRange(const VkImageSubresourceRange& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageMemoryBarrier(const VkImageMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryBarrier(const VkMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCacheHeaderVersionOne(const VkPipelineCacheHeaderVersionOne& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAllocationCallbacks(const VkAllocationCallbacks& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkApplicationInfo(const VkApplicationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormatProperties(const VkFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageFormatProperties(const VkImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInstanceCreateInfo(const VkInstanceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryHeap(const VkMemoryHeap& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryType(const VkMemoryType& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLimits(const VkPhysicalDeviceLimits& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSparseProperties(const VkPhysicalDeviceSparseProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceProperties(const VkPhysicalDeviceProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyProperties(const VkQueueFamilyProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceQueueCreateInfo(const VkDeviceQueueCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceCreateInfo(const VkDeviceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExtensionProperties(const VkExtensionProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLayerProperties(const VkLayerProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubmitInfo(const VkSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMappedMemoryRange(const VkMappedMemoryRange& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryAllocateInfo(const VkMemoryAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryRequirements(const VkMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseMemoryBind(const VkSparseMemoryBind& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseBufferMemoryBindInfo(const VkSparseBufferMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageOpaqueMemoryBindInfo(const VkSparseImageOpaqueMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageSubresource(const VkImageSubresource& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageMemoryBind(const VkSparseImageMemoryBind& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageMemoryBindInfo(const VkSparseImageMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindSparseInfo(const VkBindSparseInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageFormatProperties(const VkSparseImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageMemoryRequirements(const VkSparseImageMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFenceCreateInfo(const VkFenceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreCreateInfo(const VkSemaphoreCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkEventCreateInfo(const VkEventCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPoolCreateInfo(const VkQueryPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferCreateInfo(const VkBufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferViewCreateInfo(const VkBufferViewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCreateInfo(const VkImageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubresourceLayout(const VkSubresourceLayout& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkComponentMapping(const VkComponentMapping& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewCreateInfo(const VkImageViewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderModuleCreateInfo(const VkShaderModuleCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCacheCreateInfo(const VkPipelineCacheCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSpecializationMapEntry(const VkSpecializationMapEntry& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSpecializationInfo(const VkSpecializationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkComputePipelineCreateInfo(const VkComputePipelineCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVertexInputBindingDescription(const VkVertexInputBindingDescription& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVertexInputAttributeDescription(const VkVertexInputAttributeDescription& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineTessellationStateCreateInfo(const VkPipelineTessellationStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkViewport(const VkViewport& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStencilOpState(const VkStencilOpState& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPushConstantRange(const VkPushConstantRange& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerCreateInfo(const VkSamplerCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyDescriptorSet(const VkCopyDescriptorSet& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorBufferInfo(const VkDescriptorBufferInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorImageInfo(const VkDescriptorImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPoolSize(const VkDescriptorPoolSize& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPoolCreateInfo(const VkDescriptorPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetAllocateInfo(const VkDescriptorSetAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteDescriptorSet(const VkWriteDescriptorSet& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentDescription(const VkAttachmentDescription& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentReference(const VkAttachmentReference& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebufferCreateInfo(const VkFramebufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDescription(const VkSubpassDescription& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDependency(const VkSubpassDependency& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreateInfo(const VkRenderPassCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandPoolCreateInfo(const VkCommandPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferAllocateInfo(const VkCommandBufferAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferInheritanceInfo(const VkCommandBufferInheritanceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferBeginInfo(const VkCommandBufferBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferCopy(const VkBufferCopy& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageSubresourceLayers(const VkImageSubresourceLayers& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferImageCopy(const VkBufferImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClearDepthStencilValue(const VkClearDepthStencilValue& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClearAttachment(const VkClearAttachment& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClearRect(const VkClearRect& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageBlit(const VkImageBlit& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCopy(const VkImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageResolve(const VkImageResolve& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassBeginInfo(const VkRenderPassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSubgroupProperties(const VkPhysicalDeviceSubgroupProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindBufferMemoryInfo(const VkBindBufferMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindImageMemoryInfo(const VkBindImageMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevice16BitStorageFeatures(const VkPhysicalDevice16BitStorageFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryDedicatedRequirements(const VkMemoryDedicatedRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryDedicatedAllocateInfo(const VkMemoryDedicatedAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryAllocateFlagsInfo(const VkMemoryAllocateFlagsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupRenderPassBeginInfo(const VkDeviceGroupRenderPassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupCommandBufferBeginInfo(const VkDeviceGroupCommandBufferBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupSubmitInfo(const VkDeviceGroupSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupBindSparseInfo(const VkDeviceGroupBindSparseInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindBufferMemoryDeviceGroupInfo(const VkBindBufferMemoryDeviceGroupInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindImageMemoryDeviceGroupInfo(const VkBindImageMemoryDeviceGroupInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupDeviceCreateInfo(const VkDeviceGroupDeviceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferMemoryRequirementsInfo2(const VkBufferMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageMemoryRequirementsInfo2(const VkImageMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageSparseMemoryRequirementsInfo2(const VkImageSparseMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryRequirements2(const VkMemoryRequirements2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageMemoryRequirements2(const VkSparseImageMemoryRequirements2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFeatures2(const VkPhysicalDeviceFeatures2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceProperties2(const VkPhysicalDeviceProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormatProperties2(const VkFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageFormatProperties2(const VkImageFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageFormatInfo2(const VkPhysicalDeviceImageFormatInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyProperties2(const VkQueueFamilyProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMemoryProperties2(const VkPhysicalDeviceMemoryProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSparseImageFormatProperties2(const VkSparseImageFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSparseImageFormatInfo2(const VkPhysicalDeviceSparseImageFormatInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePointClippingProperties(const VkPhysicalDevicePointClippingProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInputAttachmentAspectReference(const VkInputAttachmentAspectReference& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassInputAttachmentAspectCreateInfo(const VkRenderPassInputAttachmentAspectCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewUsageCreateInfo(const VkImageViewUsageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineTessellationDomainOriginStateCreateInfo(const VkPipelineTessellationDomainOriginStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassMultiviewCreateInfo(const VkRenderPassMultiviewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiviewFeatures(const VkPhysicalDeviceMultiviewFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiviewProperties(const VkPhysicalDeviceMultiviewProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVariablePointersFeatures(const VkPhysicalDeviceVariablePointersFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceProtectedMemoryFeatures(const VkPhysicalDeviceProtectedMemoryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceProtectedMemoryProperties(const VkPhysicalDeviceProtectedMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceQueueInfo2(const VkDeviceQueueInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkProtectedSubmitInfo(const VkProtectedSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrConversionCreateInfo(const VkSamplerYcbcrConversionCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrConversionInfo(const VkSamplerYcbcrConversionInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindImagePlaneMemoryInfo(const VkBindImagePlaneMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImagePlaneMemoryRequirementsInfo(const VkImagePlaneMemoryRequirementsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSamplerYcbcrConversionFeatures(const VkPhysicalDeviceSamplerYcbcrConversionFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrConversionImageFormatProperties(const VkSamplerYcbcrConversionImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorUpdateTemplateEntry(const VkDescriptorUpdateTemplateEntry& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorUpdateTemplateCreateInfo(const VkDescriptorUpdateTemplateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryProperties(const VkExternalMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalImageFormatInfo(const VkPhysicalDeviceExternalImageFormatInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalImageFormatProperties(const VkExternalImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalBufferInfo(const VkPhysicalDeviceExternalBufferInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalBufferProperties(const VkExternalBufferProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceIDProperties(const VkPhysicalDeviceIDProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryImageCreateInfo(const VkExternalMemoryImageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryBufferCreateInfo(const VkExternalMemoryBufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExportMemoryAllocateInfo(const VkExportMemoryAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalFenceInfo(const VkPhysicalDeviceExternalFenceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalFenceProperties(const VkExternalFenceProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExportFenceCreateInfo(const VkExportFenceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExportSemaphoreCreateInfo(const VkExportSemaphoreCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalSemaphoreInfo(const VkPhysicalDeviceExternalSemaphoreInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalSemaphoreProperties(const VkExternalSemaphoreProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance3Properties(const VkPhysicalDeviceMaintenance3Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutSupport(const VkDescriptorSetLayoutSupport& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderDrawParametersFeatures(const VkPhysicalDeviceShaderDrawParametersFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan11Features(const VkPhysicalDeviceVulkan11Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan11Properties(const VkPhysicalDeviceVulkan11Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan12Features(const VkPhysicalDeviceVulkan12Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkConformanceVersion(const VkConformanceVersion& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan12Properties(const VkPhysicalDeviceVulkan12Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageFormatListCreateInfo(const VkImageFormatListCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentDescription2(const VkAttachmentDescription2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentReference2(const VkAttachmentReference2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDescription2(const VkSubpassDescription2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDependency2(const VkSubpassDependency2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreateInfo2(const VkRenderPassCreateInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassBeginInfo(const VkSubpassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassEndInfo(const VkSubpassEndInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevice8BitStorageFeatures(const VkPhysicalDevice8BitStorageFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDriverProperties(const VkPhysicalDeviceDriverProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderAtomicInt64Features(const VkPhysicalDeviceShaderAtomicInt64Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderFloat16Int8Features(const VkPhysicalDeviceShaderFloat16Int8Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFloatControlsProperties(const VkPhysicalDeviceFloatControlsProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutBindingFlagsCreateInfo(const VkDescriptorSetLayoutBindingFlagsCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDescriptorIndexingFeatures(const VkPhysicalDeviceDescriptorIndexingFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDescriptorIndexingProperties(const VkPhysicalDeviceDescriptorIndexingProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetVariableDescriptorCountAllocateInfo(const VkDescriptorSetVariableDescriptorCountAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetVariableDescriptorCountLayoutSupport(const VkDescriptorSetVariableDescriptorCountLayoutSupport& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassDescriptionDepthStencilResolve(const VkSubpassDescriptionDepthStencilResolve& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDepthStencilResolveProperties(const VkPhysicalDeviceDepthStencilResolveProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceScalarBlockLayoutFeatures(const VkPhysicalDeviceScalarBlockLayoutFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageStencilUsageCreateInfo(const VkImageStencilUsageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerReductionModeCreateInfo(const VkSamplerReductionModeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSamplerFilterMinmaxProperties(const VkPhysicalDeviceSamplerFilterMinmaxProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkanMemoryModelFeatures(const VkPhysicalDeviceVulkanMemoryModelFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImagelessFramebufferFeatures(const VkPhysicalDeviceImagelessFramebufferFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebufferAttachmentImageInfo(const VkFramebufferAttachmentImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebufferAttachmentsCreateInfo(const VkFramebufferAttachmentsCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassAttachmentBeginInfo(const VkRenderPassAttachmentBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceUniformBufferStandardLayoutFeatures(const VkPhysicalDeviceUniformBufferStandardLayoutFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures(const VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures(const VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentReferenceStencilLayout(const VkAttachmentReferenceStencilLayout& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentDescriptionStencilLayout(const VkAttachmentDescriptionStencilLayout& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceHostQueryResetFeatures(const VkPhysicalDeviceHostQueryResetFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTimelineSemaphoreFeatures(const VkPhysicalDeviceTimelineSemaphoreFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTimelineSemaphoreProperties(const VkPhysicalDeviceTimelineSemaphoreProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreTypeCreateInfo(const VkSemaphoreTypeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTimelineSemaphoreSubmitInfo(const VkTimelineSemaphoreSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreWaitInfo(const VkSemaphoreWaitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreSignalInfo(const VkSemaphoreSignalInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceBufferDeviceAddressFeatures(const VkPhysicalDeviceBufferDeviceAddressFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferDeviceAddressInfo(const VkBufferDeviceAddressInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferOpaqueCaptureAddressCreateInfo(const VkBufferOpaqueCaptureAddressCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryOpaqueCaptureAddressAllocateInfo(const VkMemoryOpaqueCaptureAddressAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceMemoryOpaqueCaptureAddressInfo(const VkDeviceMemoryOpaqueCaptureAddressInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan13Features(const VkPhysicalDeviceVulkan13Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan13Properties(const VkPhysicalDeviceVulkan13Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreationFeedback(const VkPipelineCreationFeedback& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreationFeedbackCreateInfo(const VkPipelineCreationFeedbackCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderTerminateInvocationFeatures(const VkPhysicalDeviceShaderTerminateInvocationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceToolProperties(const VkPhysicalDeviceToolProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures(const VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePrivateDataFeatures(const VkPhysicalDevicePrivateDataFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDevicePrivateDataCreateInfo(const VkDevicePrivateDataCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPrivateDataSlotCreateInfo(const VkPrivateDataSlotCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineCreationCacheControlFeatures(const VkPhysicalDevicePipelineCreationCacheControlFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryBarrier2(const VkMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferMemoryBarrier2(const VkBufferMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageMemoryBarrier2(const VkImageMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDependencyInfo(const VkDependencyInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreSubmitInfo(const VkSemaphoreSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferSubmitInfo(const VkCommandBufferSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubmitInfo2(const VkSubmitInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSynchronization2Features(const VkPhysicalDeviceSynchronization2Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures(const VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageRobustnessFeatures(const VkPhysicalDeviceImageRobustnessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferCopy2(const VkBufferCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyBufferInfo2(const VkCopyBufferInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCopy2(const VkImageCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyImageInfo2(const VkCopyImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferImageCopy2(const VkBufferImageCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyBufferToImageInfo2(const VkCopyBufferToImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyImageToBufferInfo2(const VkCopyImageToBufferInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageBlit2(const VkImageBlit2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBlitImageInfo2(const VkBlitImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageResolve2(const VkImageResolve2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkResolveImageInfo2(const VkResolveImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSubgroupSizeControlFeatures(const VkPhysicalDeviceSubgroupSizeControlFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSubgroupSizeControlProperties(const VkPhysicalDeviceSubgroupSizeControlProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineShaderStageRequiredSubgroupSizeCreateInfo(const VkPipelineShaderStageRequiredSubgroupSizeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceInlineUniformBlockFeatures(const VkPhysicalDeviceInlineUniformBlockFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceInlineUniformBlockProperties(const VkPhysicalDeviceInlineUniformBlockProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteDescriptorSetInlineUniformBlock(const VkWriteDescriptorSetInlineUniformBlock& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorPoolInlineUniformBlockCreateInfo(const VkDescriptorPoolInlineUniformBlockCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTextureCompressionASTCHDRFeatures(const VkPhysicalDeviceTextureCompressionASTCHDRFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingAttachmentInfo(const VkRenderingAttachmentInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingInfo(const VkRenderingInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRenderingCreateInfo(const VkPipelineRenderingCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDynamicRenderingFeatures(const VkPhysicalDeviceDynamicRenderingFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferInheritanceRenderingInfo(const VkCommandBufferInheritanceRenderingInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderIntegerDotProductFeatures(const VkPhysicalDeviceShaderIntegerDotProductFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderIntegerDotProductProperties(const VkPhysicalDeviceShaderIntegerDotProductProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTexelBufferAlignmentProperties(const VkPhysicalDeviceTexelBufferAlignmentProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFormatProperties3(const VkFormatProperties3& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance4Features(const VkPhysicalDeviceMaintenance4Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance4Properties(const VkPhysicalDeviceMaintenance4Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan14Features(const VkPhysicalDeviceVulkan14Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVulkan14Properties(const VkPhysicalDeviceVulkan14Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceQueueGlobalPriorityCreateInfo(const VkDeviceQueueGlobalPriorityCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceGlobalPriorityQueryFeatures(const VkPhysicalDeviceGlobalPriorityQueryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyGlobalPriorityProperties(const VkQueueFamilyGlobalPriorityProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderSubgroupRotateFeatures(const VkPhysicalDeviceShaderSubgroupRotateFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderFloatControls2Features(const VkPhysicalDeviceShaderFloatControls2Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderExpectAssumeFeatures(const VkPhysicalDeviceShaderExpectAssumeFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLineRasterizationFeatures(const VkPhysicalDeviceLineRasterizationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLineRasterizationProperties(const VkPhysicalDeviceLineRasterizationProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationLineStateCreateInfo(const VkPipelineRasterizationLineStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVertexAttributeDivisorProperties(const VkPhysicalDeviceVertexAttributeDivisorProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVertexInputBindingDivisorDescription(const VkVertexInputBindingDivisorDescription& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineVertexInputDivisorStateCreateInfo(const VkPipelineVertexInputDivisorStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVertexAttributeDivisorFeatures(const VkPhysicalDeviceVertexAttributeDivisorFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceIndexTypeUint8Features(const VkPhysicalDeviceIndexTypeUint8Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryMapInfo(const VkMemoryMapInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryUnmapInfo(const VkMemoryUnmapInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance5Features(const VkPhysicalDeviceMaintenance5Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance5Properties(const VkPhysicalDeviceMaintenance5Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingAreaInfo(const VkRenderingAreaInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageSubresource2(const VkImageSubresource2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceImageSubresourceInfo(const VkDeviceImageSubresourceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubresourceLayout2(const VkSubresourceLayout2& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreateFlags2CreateInfo(const VkPipelineCreateFlags2CreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferUsageFlags2CreateInfo(const VkBufferUsageFlags2CreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePushDescriptorProperties(const VkPhysicalDevicePushDescriptorProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDynamicRenderingLocalReadFeatures(const VkPhysicalDeviceDynamicRenderingLocalReadFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingAttachmentLocationInfo(const VkRenderingAttachmentLocationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingInputAttachmentIndexInfo(const VkRenderingInputAttachmentIndexInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance6Features(const VkPhysicalDeviceMaintenance6Features& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance6Properties(const VkPhysicalDeviceMaintenance6Properties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindMemoryStatus(const VkBindMemoryStatus& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindDescriptorSetsInfo(const VkBindDescriptorSetsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPushConstantsInfo(const VkPushConstantsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPushDescriptorSetInfo(const VkPushDescriptorSetInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPushDescriptorSetWithTemplateInfo(const VkPushDescriptorSetWithTemplateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineProtectedAccessFeatures(const VkPhysicalDevicePipelineProtectedAccessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineRobustnessFeatures(const VkPhysicalDevicePipelineRobustnessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineRobustnessProperties(const VkPhysicalDevicePipelineRobustnessProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRobustnessCreateInfo(const VkPipelineRobustnessCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceHostImageCopyFeatures(const VkPhysicalDeviceHostImageCopyFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceHostImageCopyProperties(const VkPhysicalDeviceHostImageCopyProperties& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryToImageCopy(const VkMemoryToImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageToMemoryCopy(const VkImageToMemoryCopy& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMemoryToImageInfo(const VkCopyMemoryToImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyImageToMemoryInfo(const VkCopyImageToMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyImageToImageInfo(const VkCopyImageToImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHostImageLayoutTransitionInfo(const VkHostImageLayoutTransitionInfo& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubresourceHostMemcpySize(const VkSubresourceHostMemcpySize& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHostImageCopyDevicePerformanceQuery(const VkHostImageCopyDevicePerformanceQuery& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceCapabilitiesKHR(const VkSurfaceCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceFormatKHR(const VkSurfaceFormatKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainCreateInfoKHR(const VkSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentInfoKHR(const VkPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageSwapchainCreateInfoKHR(const VkImageSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindImageMemorySwapchainInfoKHR(const VkBindImageMemorySwapchainInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAcquireNextImageInfoKHR(const VkAcquireNextImageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupPresentCapabilitiesKHR(const VkDeviceGroupPresentCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupPresentInfoKHR(const VkDeviceGroupPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceGroupSwapchainCreateInfoKHR(const VkDeviceGroupSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModeParametersKHR(const VkDisplayModeParametersKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModeCreateInfoKHR(const VkDisplayModeCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModePropertiesKHR(const VkDisplayModePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlaneCapabilitiesKHR(const VkDisplayPlaneCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlanePropertiesKHR(const VkDisplayPlanePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPropertiesKHR(const VkDisplayPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplaySurfaceCreateInfoKHR(const VkDisplaySurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPresentInfoKHR(const VkDisplayPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_VkXlibSurfaceCreateInfoKHR(const VkXlibSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_VkXcbSurfaceCreateInfoKHR(const VkXcbSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_VkWaylandSurfaceCreateInfoKHR(const VkWaylandSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidSurfaceCreateInfoKHR(const VkAndroidSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkWin32SurfaceCreateInfoKHR(const VkWin32SurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkDebugReportCallbackCreateInfoEXT(const VkDebugReportCallbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationStateRasterizationOrderAMD(const VkPipelineRasterizationStateRasterizationOrderAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugMarkerObjectNameInfoEXT(const VkDebugMarkerObjectNameInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugMarkerObjectTagInfoEXT(const VkDebugMarkerObjectTagInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugMarkerMarkerInfoEXT(const VkDebugMarkerMarkerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyQueryResultStatusPropertiesKHR(const VkQueueFamilyQueryResultStatusPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyVideoPropertiesKHR(const VkQueueFamilyVideoPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoProfileInfoKHR(const VkVideoProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoProfileListInfoKHR(const VkVideoProfileListInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCapabilitiesKHR(const VkVideoCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVideoFormatInfoKHR(const VkPhysicalDeviceVideoFormatInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoFormatPropertiesKHR(const VkVideoFormatPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoPictureResourceInfoKHR(const VkVideoPictureResourceInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoReferenceSlotInfoKHR(const VkVideoReferenceSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionMemoryRequirementsKHR(const VkVideoSessionMemoryRequirementsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindVideoSessionMemoryInfoKHR(const VkBindVideoSessionMemoryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionCreateInfoKHR(const VkVideoSessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionParametersCreateInfoKHR(const VkVideoSessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoSessionParametersUpdateInfoKHR(const VkVideoSessionParametersUpdateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoBeginCodingInfoKHR(const VkVideoBeginCodingInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEndCodingInfoKHR(const VkVideoEndCodingInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoCodingControlInfoKHR(const VkVideoCodingControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeCapabilitiesKHR(const VkVideoDecodeCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeUsageInfoKHR(const VkVideoDecodeUsageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeInfoKHR(const VkVideoDecodeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDedicatedAllocationImageCreateInfoNV(const VkDedicatedAllocationImageCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDedicatedAllocationBufferCreateInfoNV(const VkDedicatedAllocationBufferCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDedicatedAllocationMemoryAllocateInfoNV(const VkDedicatedAllocationMemoryAllocateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTransformFeedbackFeaturesEXT(const VkPhysicalDeviceTransformFeedbackFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTransformFeedbackPropertiesEXT(const VkPhysicalDeviceTransformFeedbackPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationStateStreamCreateInfoEXT(const VkPipelineRasterizationStateStreamCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCuModuleCreateInfoNVX(const VkCuModuleCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCuModuleTexturingModeCreateInfoNVX(const VkCuModuleTexturingModeCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCuFunctionCreateInfoNVX(const VkCuFunctionCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCuLaunchInfoNVX(const VkCuLaunchInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewHandleInfoNVX(const VkImageViewHandleInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewAddressPropertiesNVX(const VkImageViewAddressPropertiesNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264CapabilitiesKHR(const VkVideoEncodeH264CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264QpKHR(const VkVideoEncodeH264QpKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264QualityLevelPropertiesKHR(const VkVideoEncodeH264QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264SessionCreateInfoKHR(const VkVideoEncodeH264SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264SessionParametersAddInfoKHR(const VkVideoEncodeH264SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264SessionParametersCreateInfoKHR(const VkVideoEncodeH264SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264SessionParametersGetInfoKHR(const VkVideoEncodeH264SessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264SessionParametersFeedbackInfoKHR(const VkVideoEncodeH264SessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264NaluSliceInfoKHR(const VkVideoEncodeH264NaluSliceInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264PictureInfoKHR(const VkVideoEncodeH264PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264DpbSlotInfoKHR(const VkVideoEncodeH264DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264ProfileInfoKHR(const VkVideoEncodeH264ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264RateControlInfoKHR(const VkVideoEncodeH264RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264FrameSizeKHR(const VkVideoEncodeH264FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264RateControlLayerInfoKHR(const VkVideoEncodeH264RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264GopRemainingFrameInfoKHR(const VkVideoEncodeH264GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265CapabilitiesKHR(const VkVideoEncodeH265CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265SessionCreateInfoKHR(const VkVideoEncodeH265SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265QpKHR(const VkVideoEncodeH265QpKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265QualityLevelPropertiesKHR(const VkVideoEncodeH265QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265SessionParametersAddInfoKHR(const VkVideoEncodeH265SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265SessionParametersCreateInfoKHR(const VkVideoEncodeH265SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265SessionParametersGetInfoKHR(const VkVideoEncodeH265SessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265SessionParametersFeedbackInfoKHR(const VkVideoEncodeH265SessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265NaluSliceSegmentInfoKHR(const VkVideoEncodeH265NaluSliceSegmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265PictureInfoKHR(const VkVideoEncodeH265PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265DpbSlotInfoKHR(const VkVideoEncodeH265DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265ProfileInfoKHR(const VkVideoEncodeH265ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265RateControlInfoKHR(const VkVideoEncodeH265RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265FrameSizeKHR(const VkVideoEncodeH265FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265RateControlLayerInfoKHR(const VkVideoEncodeH265RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265GopRemainingFrameInfoKHR(const VkVideoEncodeH265GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264ProfileInfoKHR(const VkVideoDecodeH264ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264CapabilitiesKHR(const VkVideoDecodeH264CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264SessionParametersAddInfoKHR(const VkVideoDecodeH264SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264SessionParametersCreateInfoKHR(const VkVideoDecodeH264SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264PictureInfoKHR(const VkVideoDecodeH264PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264DpbSlotInfoKHR(const VkVideoDecodeH264DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTextureLODGatherFormatPropertiesAMD(const VkTextureLODGatherFormatPropertiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderResourceUsageAMD(const VkShaderResourceUsageAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderStatisticsInfoAMD(const VkShaderStatisticsInfoAMD& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_VkStreamDescriptorSurfaceCreateInfoGGP(const VkStreamDescriptorSurfaceCreateInfoGGP& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
void dump_text_VkPhysicalDeviceCornerSampledImageFeaturesNV(const VkPhysicalDeviceCornerSampledImageFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalImageFormatPropertiesNV(const VkExternalImageFormatPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryImageCreateInfoNV(const VkExternalMemoryImageCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExportMemoryAllocateInfoNV(const VkExportMemoryAllocateInfoNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkImportMemoryWin32HandleInfoNV(const VkImportMemoryWin32HandleInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkExportMemoryWin32HandleInfoNV(const VkExportMemoryWin32HandleInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkWin32KeyedMutexAcquireReleaseInfoNV(const VkWin32KeyedMutexAcquireReleaseInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkValidationFlagsEXT(const VkValidationFlagsEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_text_VkViSurfaceCreateInfoNN(const VkViSurfaceCreateInfoNN& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_VI_NN
void dump_text_VkImageViewASTCDecodeModeEXT(const VkImageViewASTCDecodeModeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceASTCDecodeFeaturesEXT(const VkPhysicalDeviceASTCDecodeFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkImportMemoryWin32HandleInfoKHR(const VkImportMemoryWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkExportMemoryWin32HandleInfoKHR(const VkExportMemoryWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkMemoryWin32HandlePropertiesKHR(const VkMemoryWin32HandlePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkMemoryGetWin32HandleInfoKHR(const VkMemoryGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkImportMemoryFdInfoKHR(const VkImportMemoryFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryFdPropertiesKHR(const VkMemoryFdPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryGetFdInfoKHR(const VkMemoryGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkWin32KeyedMutexAcquireReleaseInfoKHR(const VkWin32KeyedMutexAcquireReleaseInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkImportSemaphoreWin32HandleInfoKHR(const VkImportSemaphoreWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkExportSemaphoreWin32HandleInfoKHR(const VkExportSemaphoreWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkD3D12FenceSubmitInfoKHR(const VkD3D12FenceSubmitInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkSemaphoreGetWin32HandleInfoKHR(const VkSemaphoreGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkImportSemaphoreFdInfoKHR(const VkImportSemaphoreFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSemaphoreGetFdInfoKHR(const VkSemaphoreGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkConditionalRenderingBeginInfoEXT(const VkConditionalRenderingBeginInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceConditionalRenderingFeaturesEXT(const VkPhysicalDeviceConditionalRenderingFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferInheritanceConditionalRenderingInfoEXT(const VkCommandBufferInheritanceConditionalRenderingInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRectLayerKHR(const VkRectLayerKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentRegionKHR(const VkPresentRegionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentRegionsKHR(const VkPresentRegionsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkViewportWScalingNV(const VkViewportWScalingNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportWScalingStateCreateInfoNV(const VkPipelineViewportWScalingStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceCapabilities2EXT(const VkSurfaceCapabilities2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPowerInfoEXT(const VkDisplayPowerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceEventInfoEXT(const VkDeviceEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayEventInfoEXT(const VkDisplayEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainCounterCreateInfoEXT(const VkSwapchainCounterCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRefreshCycleDurationGOOGLE(const VkRefreshCycleDurationGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPastPresentationTimingGOOGLE(const VkPastPresentationTimingGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentTimeGOOGLE(const VkPresentTimeGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentTimesInfoGOOGLE(const VkPresentTimesInfoGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX(const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMultiviewPerViewAttributesInfoNVX(const VkMultiviewPerViewAttributesInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkViewportSwizzleNV(const VkViewportSwizzleNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportSwizzleStateCreateInfoNV(const VkPipelineViewportSwizzleStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDiscardRectanglePropertiesEXT(const VkPhysicalDeviceDiscardRectanglePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineDiscardRectangleStateCreateInfoEXT(const VkPipelineDiscardRectangleStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceConservativeRasterizationPropertiesEXT(const VkPhysicalDeviceConservativeRasterizationPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationConservativeStateCreateInfoEXT(const VkPipelineRasterizationConservativeStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDepthClipEnableFeaturesEXT(const VkPhysicalDeviceDepthClipEnableFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationDepthClipStateCreateInfoEXT(const VkPipelineRasterizationDepthClipStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkXYColorEXT(const VkXYColorEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHdrMetadataEXT(const VkHdrMetadataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG(const VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSharedPresentSurfaceCapabilitiesKHR(const VkSharedPresentSurfaceCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkImportFenceWin32HandleInfoKHR(const VkImportFenceWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkExportFenceWin32HandleInfoKHR(const VkExportFenceWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkFenceGetWin32HandleInfoKHR(const VkFenceGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkImportFenceFdInfoKHR(const VkImportFenceFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFenceGetFdInfoKHR(const VkFenceGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePerformanceQueryFeaturesKHR(const VkPhysicalDevicePerformanceQueryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePerformanceQueryPropertiesKHR(const VkPhysicalDevicePerformanceQueryPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterKHR(const VkPerformanceCounterKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterDescriptionKHR(const VkPerformanceCounterDescriptionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPoolPerformanceCreateInfoKHR(const VkQueryPoolPerformanceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAcquireProfilingLockInfoKHR(const VkAcquireProfilingLockInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceQuerySubmitInfoKHR(const VkPerformanceQuerySubmitInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSurfaceInfo2KHR(const VkPhysicalDeviceSurfaceInfo2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceCapabilities2KHR(const VkSurfaceCapabilities2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceFormat2KHR(const VkSurfaceFormat2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayProperties2KHR(const VkDisplayProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlaneProperties2KHR(const VkDisplayPlaneProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModeProperties2KHR(const VkDisplayModeProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlaneInfo2KHR(const VkDisplayPlaneInfo2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayPlaneCapabilities2KHR(const VkDisplayPlaneCapabilities2KHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_text_VkIOSSurfaceCreateInfoMVK(const VkIOSSurfaceCreateInfoMVK& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_text_VkMacOSSurfaceCreateInfoMVK(const VkMacOSSurfaceCreateInfoMVK& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_text_VkDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsObjectNameInfoEXT(const VkDebugUtilsObjectNameInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessengerCallbackDataEXT(const VkDebugUtilsMessengerCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsMessengerCreateInfoEXT(const VkDebugUtilsMessengerCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDebugUtilsObjectTagInfoEXT(const VkDebugUtilsObjectTagInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidHardwareBufferUsageANDROID(const VkAndroidHardwareBufferUsageANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidHardwareBufferPropertiesANDROID(const VkAndroidHardwareBufferPropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidHardwareBufferFormatPropertiesANDROID(const VkAndroidHardwareBufferFormatPropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkImportAndroidHardwareBufferInfoANDROID(const VkImportAndroidHardwareBufferInfoANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkMemoryGetAndroidHardwareBufferInfoANDROID(const VkMemoryGetAndroidHardwareBufferInfoANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkExternalFormatANDROID(const VkExternalFormatANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidHardwareBufferFormatProperties2ANDROID(const VkAndroidHardwareBufferFormatProperties2ANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDeviceShaderEnqueueFeaturesAMDX(const VkPhysicalDeviceShaderEnqueueFeaturesAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDeviceShaderEnqueuePropertiesAMDX(const VkPhysicalDeviceShaderEnqueuePropertiesAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkExecutionGraphPipelineScratchSizeAMDX(const VkExecutionGraphPipelineScratchSizeAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkPipelineLibraryCreateInfoKHR(const VkPipelineLibraryCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkExecutionGraphPipelineCreateInfoAMDX(const VkExecutionGraphPipelineCreateInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkDispatchGraphInfoAMDX(const VkDispatchGraphInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkDispatchGraphCountInfoAMDX(const VkDispatchGraphCountInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPipelineShaderStageNodeCreateInfoAMDX(const VkPipelineShaderStageNodeCreateInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkAttachmentSampleCountInfoAMD(const VkAttachmentSampleCountInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderBfloat16FeaturesKHR(const VkPhysicalDeviceShaderBfloat16FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSampleLocationEXT(const VkSampleLocationEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSampleLocationsInfoEXT(const VkSampleLocationsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAttachmentSampleLocationsEXT(const VkAttachmentSampleLocationsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassSampleLocationsEXT(const VkSubpassSampleLocationsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassSampleLocationsBeginInfoEXT(const VkRenderPassSampleLocationsBeginInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineSampleLocationsStateCreateInfoEXT(const VkPipelineSampleLocationsStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSampleLocationsPropertiesEXT(const VkPhysicalDeviceSampleLocationsPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMultisamplePropertiesEXT(const VkMultisamplePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT(const VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT(const VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineColorBlendAdvancedStateCreateInfoEXT(const VkPipelineColorBlendAdvancedStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCoverageToColorStateCreateInfoNV(const VkPipelineCoverageToColorStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureBuildRangeInfoKHR(const VkAccelerationStructureBuildRangeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAabbPositionsKHR(const VkAabbPositionsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryTrianglesDataKHR(const VkAccelerationStructureGeometryTrianglesDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTransformMatrixKHR(const VkTransformMatrixKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryAabbsDataKHR(const VkAccelerationStructureGeometryAabbsDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryInstancesDataKHR(const VkAccelerationStructureGeometryInstancesDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryKHR(const VkAccelerationStructureGeometryKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureBuildGeometryInfoKHR(const VkAccelerationStructureBuildGeometryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureInstanceKHR(const VkAccelerationStructureInstanceKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureCreateInfoKHR(const VkAccelerationStructureCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteDescriptorSetAccelerationStructureKHR(const VkWriteDescriptorSetAccelerationStructureKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceAccelerationStructureFeaturesKHR(const VkPhysicalDeviceAccelerationStructureFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceAccelerationStructurePropertiesKHR(const VkPhysicalDeviceAccelerationStructurePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureDeviceAddressInfoKHR(const VkAccelerationStructureDeviceAddressInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureVersionInfoKHR(const VkAccelerationStructureVersionInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyAccelerationStructureToMemoryInfoKHR(const VkCopyAccelerationStructureToMemoryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMemoryToAccelerationStructureInfoKHR(const VkCopyMemoryToAccelerationStructureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyAccelerationStructureInfoKHR(const VkCopyAccelerationStructureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureBuildSizesInfoKHR(const VkAccelerationStructureBuildSizesInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCoverageModulationStateCreateInfoNV(const VkPipelineCoverageModulationStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderSMBuiltinsPropertiesNV(const VkPhysicalDeviceShaderSMBuiltinsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderSMBuiltinsFeaturesNV(const VkPhysicalDeviceShaderSMBuiltinsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrmFormatModifierPropertiesEXT(const VkDrmFormatModifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrmFormatModifierPropertiesListEXT(const VkDrmFormatModifierPropertiesListEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageDrmFormatModifierInfoEXT(const VkPhysicalDeviceImageDrmFormatModifierInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageDrmFormatModifierListCreateInfoEXT(const VkImageDrmFormatModifierListCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageDrmFormatModifierExplicitCreateInfoEXT(const VkImageDrmFormatModifierExplicitCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageDrmFormatModifierPropertiesEXT(const VkImageDrmFormatModifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrmFormatModifierProperties2EXT(const VkDrmFormatModifierProperties2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrmFormatModifierPropertiesList2EXT(const VkDrmFormatModifierPropertiesList2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationCacheCreateInfoEXT(const VkValidationCacheCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderModuleValidationCacheCreateInfoEXT(const VkShaderModuleValidationCacheCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDevicePortabilitySubsetFeaturesKHR(const VkPhysicalDevicePortabilitySubsetFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDevicePortabilitySubsetPropertiesKHR(const VkPhysicalDevicePortabilitySubsetPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkShadingRatePaletteNV(const VkShadingRatePaletteNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportShadingRateImageStateCreateInfoNV(const VkPipelineViewportShadingRateImageStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShadingRateImageFeaturesNV(const VkPhysicalDeviceShadingRateImageFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShadingRateImagePropertiesNV(const VkPhysicalDeviceShadingRateImagePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCoarseSampleLocationNV(const VkCoarseSampleLocationNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCoarseSampleOrderCustomNV(const VkCoarseSampleOrderCustomNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportCoarseSampleOrderStateCreateInfoNV(const VkPipelineViewportCoarseSampleOrderStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingShaderGroupCreateInfoNV(const VkRayTracingShaderGroupCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingPipelineCreateInfoNV(const VkRayTracingPipelineCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryTrianglesNV(const VkGeometryTrianglesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryAABBNV(const VkGeometryAABBNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryDataNV(const VkGeometryDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeometryNV(const VkGeometryNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureInfoNV(const VkAccelerationStructureInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureCreateInfoNV(const VkAccelerationStructureCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindAccelerationStructureMemoryInfoNV(const VkBindAccelerationStructureMemoryInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteDescriptorSetAccelerationStructureNV(const VkWriteDescriptorSetAccelerationStructureNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMemoryRequirementsInfoNV(const VkAccelerationStructureMemoryRequirementsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingPropertiesNV(const VkPhysicalDeviceRayTracingPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV(const VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRepresentativeFragmentTestStateCreateInfoNV(const VkPipelineRepresentativeFragmentTestStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageViewImageFormatInfoEXT(const VkPhysicalDeviceImageViewImageFormatInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFilterCubicImageViewImageFormatPropertiesEXT(const VkFilterCubicImageViewImageFormatPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImportMemoryHostPointerInfoEXT(const VkImportMemoryHostPointerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryHostPointerPropertiesEXT(const VkMemoryHostPointerPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalMemoryHostPropertiesEXT(const VkPhysicalDeviceExternalMemoryHostPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderClockFeaturesKHR(const VkPhysicalDeviceShaderClockFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCompilerControlCreateInfoAMD(const VkPipelineCompilerControlCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCalibratedTimestampInfoKHR(const VkCalibratedTimestampInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderCorePropertiesAMD(const VkPhysicalDeviceShaderCorePropertiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265ProfileInfoKHR(const VkVideoDecodeH265ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265CapabilitiesKHR(const VkVideoDecodeH265CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265SessionParametersAddInfoKHR(const VkVideoDecodeH265SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265SessionParametersCreateInfoKHR(const VkVideoDecodeH265SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265PictureInfoKHR(const VkVideoDecodeH265PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265DpbSlotInfoKHR(const VkVideoDecodeH265DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceMemoryOverallocationCreateInfoAMD(const VkDeviceMemoryOverallocationCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT(const VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_VkPresentFrameTokenGGP(const VkPresentFrameTokenGGP& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
void dump_text_VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR(const VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMeshShaderFeaturesNV(const VkPhysicalDeviceMeshShaderFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMeshShaderPropertiesNV(const VkPhysicalDeviceMeshShaderPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrawMeshTasksIndirectCommandNV(const VkDrawMeshTasksIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR(const VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderImageFootprintFeaturesNV(const VkPhysicalDeviceShaderImageFootprintFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportExclusiveScissorStateCreateInfoNV(const VkPipelineViewportExclusiveScissorStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExclusiveScissorFeaturesNV(const VkPhysicalDeviceExclusiveScissorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyCheckpointPropertiesNV(const VkQueueFamilyCheckpointPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCheckpointDataNV(const VkCheckpointDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueueFamilyCheckpointProperties2NV(const VkQueueFamilyCheckpointProperties2NV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCheckpointData2NV(const VkCheckpointData2NV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL(const VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceValueINTEL(const VkPerformanceValueINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkInitializePerformanceApiInfoINTEL(const VkInitializePerformanceApiInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPoolPerformanceQueryCreateInfoINTEL(const VkQueryPoolPerformanceQueryCreateInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceMarkerInfoINTEL(const VkPerformanceMarkerInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceStreamMarkerInfoINTEL(const VkPerformanceStreamMarkerInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceOverrideInfoINTEL(const VkPerformanceOverrideInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceConfigurationAcquireInfoINTEL(const VkPerformanceConfigurationAcquireInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePCIBusInfoPropertiesEXT(const VkPhysicalDevicePCIBusInfoPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayNativeHdrSurfaceCapabilitiesAMD(const VkDisplayNativeHdrSurfaceCapabilitiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainDisplayNativeHdrCreateInfoAMD(const VkSwapchainDisplayNativeHdrCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImagePipeSurfaceCreateInfoFUCHSIA(const VkImagePipeSurfaceCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkMetalSurfaceCreateInfoEXT(const VkMetalSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkPhysicalDeviceFragmentDensityMapFeaturesEXT(const VkPhysicalDeviceFragmentDensityMapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentDensityMapPropertiesEXT(const VkPhysicalDeviceFragmentDensityMapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassFragmentDensityMapCreateInfoEXT(const VkRenderPassFragmentDensityMapCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingFragmentDensityMapAttachmentInfoEXT(const VkRenderingFragmentDensityMapAttachmentInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFragmentShadingRateAttachmentInfoKHR(const VkFragmentShadingRateAttachmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineFragmentShadingRateStateCreateInfoKHR(const VkPipelineFragmentShadingRateStateCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShadingRateFeaturesKHR(const VkPhysicalDeviceFragmentShadingRateFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShadingRatePropertiesKHR(const VkPhysicalDeviceFragmentShadingRatePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShadingRateKHR(const VkPhysicalDeviceFragmentShadingRateKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderingFragmentShadingRateAttachmentInfoKHR(const VkRenderingFragmentShadingRateAttachmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderCoreProperties2AMD(const VkPhysicalDeviceShaderCoreProperties2AMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCoherentMemoryFeaturesAMD(const VkPhysicalDeviceCoherentMemoryFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT(const VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderQuadControlFeaturesKHR(const VkPhysicalDeviceShaderQuadControlFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMemoryBudgetPropertiesEXT(const VkPhysicalDeviceMemoryBudgetPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMemoryPriorityFeaturesEXT(const VkPhysicalDeviceMemoryPriorityFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryPriorityAllocateInfoEXT(const VkMemoryPriorityAllocateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceProtectedCapabilitiesKHR(const VkSurfaceProtectedCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV(const VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceBufferDeviceAddressFeaturesEXT(const VkPhysicalDeviceBufferDeviceAddressFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferDeviceAddressCreateInfoEXT(const VkBufferDeviceAddressCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkValidationFeaturesEXT(const VkValidationFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePresentWaitFeaturesKHR(const VkPhysicalDevicePresentWaitFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCooperativeMatrixPropertiesNV(const VkCooperativeMatrixPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeMatrixFeaturesNV(const VkPhysicalDeviceCooperativeMatrixFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeMatrixPropertiesNV(const VkPhysicalDeviceCooperativeMatrixPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCoverageReductionModeFeaturesNV(const VkPhysicalDeviceCoverageReductionModeFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCoverageReductionStateCreateInfoNV(const VkPipelineCoverageReductionStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFramebufferMixedSamplesCombinationNV(const VkFramebufferMixedSamplesCombinationNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT(const VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceYcbcrImageArraysFeaturesEXT(const VkPhysicalDeviceYcbcrImageArraysFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceProvokingVertexFeaturesEXT(const VkPhysicalDeviceProvokingVertexFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceProvokingVertexPropertiesEXT(const VkPhysicalDeviceProvokingVertexPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineRasterizationProvokingVertexStateCreateInfoEXT(const VkPipelineRasterizationProvokingVertexStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkSurfaceFullScreenExclusiveInfoEXT(const VkSurfaceFullScreenExclusiveInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkSurfaceCapabilitiesFullScreenExclusiveEXT(const VkSurfaceCapabilitiesFullScreenExclusiveEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_VkSurfaceFullScreenExclusiveWin32InfoEXT(const VkSurfaceFullScreenExclusiveWin32InfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_VkHeadlessSurfaceCreateInfoEXT(const VkHeadlessSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderAtomicFloatFeaturesEXT(const VkPhysicalDeviceShaderAtomicFloatFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExtendedDynamicStateFeaturesEXT(const VkPhysicalDeviceExtendedDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR(const VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineInfoKHR(const VkPipelineInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineExecutablePropertiesKHR(const VkPipelineExecutablePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineExecutableInfoKHR(const VkPipelineExecutableInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineExecutableStatisticKHR(const VkPipelineExecutableStatisticKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineExecutableInternalRepresentationKHR(const VkPipelineExecutableInternalRepresentationKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMapMemoryPlacedFeaturesEXT(const VkPhysicalDeviceMapMemoryPlacedFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMapMemoryPlacedPropertiesEXT(const VkPhysicalDeviceMapMemoryPlacedPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryMapPlacedInfoEXT(const VkMemoryMapPlacedInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT(const VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfacePresentModeEXT(const VkSurfacePresentModeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfacePresentScalingCapabilitiesEXT(const VkSurfacePresentScalingCapabilitiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfacePresentModeCompatibilityEXT(const VkSurfacePresentModeCompatibilityEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT(const VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainPresentFenceInfoEXT(const VkSwapchainPresentFenceInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainPresentModesCreateInfoEXT(const VkSwapchainPresentModesCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainPresentModeInfoEXT(const VkSwapchainPresentModeInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainPresentScalingCreateInfoEXT(const VkSwapchainPresentScalingCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkReleaseSwapchainImagesInfoEXT(const VkReleaseSwapchainImagesInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV(const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV(const VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGraphicsShaderGroupCreateInfoNV(const VkGraphicsShaderGroupCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGraphicsPipelineShaderGroupsCreateInfoNV(const VkGraphicsPipelineShaderGroupsCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindShaderGroupIndirectCommandNV(const VkBindShaderGroupIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindIndexBufferIndirectCommandNV(const VkBindIndexBufferIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindVertexBufferIndirectCommandNV(const VkBindVertexBufferIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSetStateFlagsIndirectCommandNV(const VkSetStateFlagsIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsStreamNV(const VkIndirectCommandsStreamNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutTokenNV(const VkIndirectCommandsLayoutTokenNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutCreateInfoNV(const VkIndirectCommandsLayoutCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeneratedCommandsInfoNV(const VkGeneratedCommandsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeneratedCommandsMemoryRequirementsInfoNV(const VkGeneratedCommandsMemoryRequirementsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceInheritedViewportScissorFeaturesNV(const VkPhysicalDeviceInheritedViewportScissorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferInheritanceViewportScissorInfoNV(const VkCommandBufferInheritanceViewportScissorInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT(const VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassTransformBeginInfoQCOM(const VkRenderPassTransformBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCommandBufferInheritanceRenderPassTransformInfoQCOM(const VkCommandBufferInheritanceRenderPassTransformInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDepthBiasControlFeaturesEXT(const VkPhysicalDeviceDepthBiasControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDepthBiasInfoEXT(const VkDepthBiasInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDepthBiasRepresentationInfoEXT(const VkDepthBiasRepresentationInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDeviceMemoryReportFeaturesEXT(const VkPhysicalDeviceDeviceMemoryReportFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceMemoryReportCallbackDataEXT(const VkDeviceMemoryReportCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceDeviceMemoryReportCreateInfoEXT(const VkDeviceDeviceMemoryReportCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRobustness2FeaturesEXT(const VkPhysicalDeviceRobustness2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRobustness2PropertiesEXT(const VkPhysicalDeviceRobustness2PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerCustomBorderColorCreateInfoEXT(const VkSamplerCustomBorderColorCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCustomBorderColorPropertiesEXT(const VkPhysicalDeviceCustomBorderColorPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCustomBorderColorFeaturesEXT(const VkPhysicalDeviceCustomBorderColorFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePresentBarrierFeaturesNV(const VkPhysicalDevicePresentBarrierFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSurfaceCapabilitiesPresentBarrierNV(const VkSurfaceCapabilitiesPresentBarrierNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainPresentBarrierCreateInfoNV(const VkSwapchainPresentBarrierCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPresentIdKHR(const VkPresentIdKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePresentIdFeaturesKHR(const VkPhysicalDevicePresentIdFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeInfoKHR(const VkVideoEncodeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeCapabilitiesKHR(const VkVideoEncodeCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryPoolVideoEncodeFeedbackCreateInfoKHR(const VkQueryPoolVideoEncodeFeedbackCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeUsageInfoKHR(const VkVideoEncodeUsageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeRateControlLayerInfoKHR(const VkVideoEncodeRateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeRateControlInfoKHR(const VkVideoEncodeRateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR(const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeQualityLevelPropertiesKHR(const VkVideoEncodeQualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeQualityLevelInfoKHR(const VkVideoEncodeQualityLevelInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeSessionParametersGetInfoKHR(const VkVideoEncodeSessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeSessionParametersFeedbackInfoKHR(const VkVideoEncodeSessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDiagnosticsConfigFeaturesNV(const VkPhysicalDeviceDiagnosticsConfigFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceDiagnosticsConfigCreateInfoNV(const VkDeviceDiagnosticsConfigCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkCudaModuleCreateInfoNV(const VkCudaModuleCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkCudaFunctionCreateInfoNV(const VkCudaFunctionCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkCudaLaunchInfoNV(const VkCudaLaunchInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDeviceCudaKernelLaunchFeaturesNV(const VkPhysicalDeviceCudaKernelLaunchFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDeviceCudaKernelLaunchPropertiesNV(const VkPhysicalDeviceCudaKernelLaunchPropertiesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkPhysicalDeviceTileShadingFeaturesQCOM(const VkPhysicalDeviceTileShadingFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTileShadingPropertiesQCOM(const VkPhysicalDeviceTileShadingPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassTileShadingCreateInfoQCOM(const VkRenderPassTileShadingCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerTileBeginInfoQCOM(const VkPerTileBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerTileEndInfoQCOM(const VkPerTileEndInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDispatchTileInfoQCOM(const VkDispatchTileInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkQueryLowLatencySupportNV(const VkQueryLowLatencySupportNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalObjectCreateInfoEXT(const VkExportMetalObjectCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalObjectsInfoEXT(const VkExportMetalObjectsInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalDeviceInfoEXT(const VkExportMetalDeviceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalCommandQueueInfoEXT(const VkExportMetalCommandQueueInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalBufferInfoEXT(const VkExportMetalBufferInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkImportMetalBufferInfoEXT(const VkImportMetalBufferInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalTextureInfoEXT(const VkExportMetalTextureInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkImportMetalTextureInfoEXT(const VkImportMetalTextureInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalIOSurfaceInfoEXT(const VkExportMetalIOSurfaceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkImportMetalIOSurfaceInfoEXT(const VkImportMetalIOSurfaceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkExportMetalSharedEventInfoEXT(const VkExportMetalSharedEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkImportMetalSharedEventInfoEXT(const VkImportMetalSharedEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkPhysicalDeviceDescriptorBufferPropertiesEXT(const VkPhysicalDeviceDescriptorBufferPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT(const VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDescriptorBufferFeaturesEXT(const VkPhysicalDeviceDescriptorBufferFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorAddressInfoEXT(const VkDescriptorAddressInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorBufferBindingInfoEXT(const VkDescriptorBufferBindingInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorBufferBindingPushDescriptorBufferHandleEXT(const VkDescriptorBufferBindingPushDescriptorBufferHandleEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorGetInfoEXT(const VkDescriptorGetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBufferCaptureDescriptorDataInfoEXT(const VkBufferCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCaptureDescriptorDataInfoEXT(const VkImageCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewCaptureDescriptorDataInfoEXT(const VkImageViewCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerCaptureDescriptorDataInfoEXT(const VkSamplerCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpaqueCaptureDescriptorDataCreateInfoEXT(const VkOpaqueCaptureDescriptorDataCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureCaptureDescriptorDataInfoEXT(const VkAccelerationStructureCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT(const VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT(const VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGraphicsPipelineLibraryCreateInfoEXT(const VkGraphicsPipelineLibraryCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD(const VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR(const VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR(const VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV(const VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV(const VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineFragmentShadingRateEnumStateCreateInfoNV(const VkPipelineFragmentShadingRateEnumStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryMotionTrianglesDataNV(const VkAccelerationStructureGeometryMotionTrianglesDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInfoNV(const VkAccelerationStructureMotionInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMatrixMotionInstanceNV(const VkAccelerationStructureMatrixMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSRTDataNV(const VkSRTDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureSRTMotionInstanceNV(const VkAccelerationStructureSRTMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInstanceNV(const VkAccelerationStructureMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingMotionBlurFeaturesNV(const VkPhysicalDeviceRayTracingMotionBlurFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMeshShaderFeaturesEXT(const VkPhysicalDeviceMeshShaderFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMeshShaderPropertiesEXT(const VkPhysicalDeviceMeshShaderPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrawMeshTasksIndirectCommandEXT(const VkDrawMeshTasksIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT(const VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentDensityMap2FeaturesEXT(const VkPhysicalDeviceFragmentDensityMap2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentDensityMap2PropertiesEXT(const VkPhysicalDeviceFragmentDensityMap2PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyCommandTransformInfoQCOM(const VkCopyCommandTransformInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR(const VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageCompressionControlFeaturesEXT(const VkPhysicalDeviceImageCompressionControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCompressionControlEXT(const VkImageCompressionControlEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageCompressionPropertiesEXT(const VkImageCompressionPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT(const VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevice4444FormatsFeaturesEXT(const VkPhysicalDevice4444FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFaultFeaturesEXT(const VkPhysicalDeviceFaultFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultCountsEXT(const VkDeviceFaultCountsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultAddressInfoEXT(const VkDeviceFaultAddressInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultVendorInfoEXT(const VkDeviceFaultVendorInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultInfoEXT(const VkDeviceFaultInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceFaultVendorBinaryHeaderVersionOneEXT(const VkDeviceFaultVendorBinaryHeaderVersionOneEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT(const VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT(const VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_VkDirectFBSurfaceCreateInfoEXT(const VkDirectFBSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_text_VkRayTracingShaderGroupCreateInfoKHR(const VkRayTracingShaderGroupCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingPipelineInterfaceCreateInfoKHR(const VkRayTracingPipelineInterfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingPipelineCreateInfoKHR(const VkRayTracingPipelineCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingPipelineFeaturesKHR(const VkPhysicalDeviceRayTracingPipelineFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingPipelinePropertiesKHR(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStridedDeviceAddressRegionKHR(const VkStridedDeviceAddressRegionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTraceRaysIndirectCommandKHR(const VkTraceRaysIndirectCommandKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayQueryFeaturesKHR(const VkPhysicalDeviceRayQueryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT(const VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMutableDescriptorTypeListEXT(const VkMutableDescriptorTypeListEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMutableDescriptorTypeCreateInfoEXT(const VkMutableDescriptorTypeCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT(const VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVertexInputBindingDescription2EXT(const VkVertexInputBindingDescription2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVertexInputAttributeDescription2EXT(const VkVertexInputAttributeDescription2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDrmPropertiesEXT(const VkPhysicalDeviceDrmPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceAddressBindingReportFeaturesEXT(const VkPhysicalDeviceAddressBindingReportFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceAddressBindingCallbackDataEXT(const VkDeviceAddressBindingCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDepthClipControlFeaturesEXT(const VkPhysicalDeviceDepthClipControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportDepthClipControlCreateInfoEXT(const VkPipelineViewportDepthClipControlCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT(const VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT(const VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImportMemoryZirconHandleInfoFUCHSIA(const VkImportMemoryZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkMemoryZirconHandlePropertiesFUCHSIA(const VkMemoryZirconHandlePropertiesFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkMemoryGetZirconHandleInfoFUCHSIA(const VkMemoryGetZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImportSemaphoreZirconHandleInfoFUCHSIA(const VkImportSemaphoreZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkSemaphoreGetZirconHandleInfoFUCHSIA(const VkSemaphoreGetZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferCollectionCreateInfoFUCHSIA(const VkBufferCollectionCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImportMemoryBufferCollectionFUCHSIA(const VkImportMemoryBufferCollectionFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferCollectionImageCreateInfoFUCHSIA(const VkBufferCollectionImageCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferCollectionConstraintsInfoFUCHSIA(const VkBufferCollectionConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferConstraintsInfoFUCHSIA(const VkBufferConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferCollectionBufferCreateInfoFUCHSIA(const VkBufferCollectionBufferCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkSysmemColorSpaceFUCHSIA(const VkSysmemColorSpaceFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkBufferCollectionPropertiesFUCHSIA(const VkBufferCollectionPropertiesFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImageFormatConstraintsInfoFUCHSIA(const VkImageFormatConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_VkImageConstraintsInfoFUCHSIA(const VkImageConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_text_VkSubpassShadingPipelineCreateInfoHUAWEI(const VkSubpassShadingPipelineCreateInfoHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSubpassShadingFeaturesHUAWEI(const VkPhysicalDeviceSubpassShadingFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSubpassShadingPropertiesHUAWEI(const VkPhysicalDeviceSubpassShadingPropertiesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceInvocationMaskFeaturesHUAWEI(const VkPhysicalDeviceInvocationMaskFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryGetRemoteAddressInfoNV(const VkMemoryGetRemoteAddressInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalMemoryRDMAFeaturesNV(const VkPhysicalDeviceExternalMemoryRDMAFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelinePropertiesIdentifierEXT(const VkPipelinePropertiesIdentifierEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelinePropertiesFeaturesEXT(const VkPhysicalDevicePipelinePropertiesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFrameBoundaryFeaturesEXT(const VkPhysicalDeviceFrameBoundaryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkFrameBoundaryEXT(const VkFrameBoundaryEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT(const VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSubpassResolvePerformanceQueryEXT(const VkSubpassResolvePerformanceQueryEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMultisampledRenderToSingleSampledInfoEXT(const VkMultisampledRenderToSingleSampledInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExtendedDynamicState2FeaturesEXT(const VkPhysicalDeviceExtendedDynamicState2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkScreenSurfaceCreateInfoQNX(const VkScreenSurfaceCreateInfoQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_VkPhysicalDeviceColorWriteEnableFeaturesEXT(const VkPhysicalDeviceColorWriteEnableFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineColorWriteCreateInfoEXT(const VkPipelineColorWriteCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT(const VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR(const VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTraceRaysIndirectCommand2KHR(const VkTraceRaysIndirectCommand2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageViewMinLodFeaturesEXT(const VkPhysicalDeviceImageViewMinLodFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewMinLodCreateInfoEXT(const VkImageViewMinLodCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiDrawFeaturesEXT(const VkPhysicalDeviceMultiDrawFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiDrawPropertiesEXT(const VkPhysicalDeviceMultiDrawPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMultiDrawInfoEXT(const VkMultiDrawInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMultiDrawIndexedInfoEXT(const VkMultiDrawIndexedInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImage2DViewOf3DFeaturesEXT(const VkPhysicalDeviceImage2DViewOf3DFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderTileImageFeaturesEXT(const VkPhysicalDeviceShaderTileImageFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderTileImagePropertiesEXT(const VkPhysicalDeviceShaderTileImagePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapUsageEXT(const VkMicromapUsageEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapBuildInfoEXT(const VkMicromapBuildInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapCreateInfoEXT(const VkMicromapCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceOpacityMicromapFeaturesEXT(const VkPhysicalDeviceOpacityMicromapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceOpacityMicromapPropertiesEXT(const VkPhysicalDeviceOpacityMicromapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapVersionInfoEXT(const VkMicromapVersionInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMicromapToMemoryInfoEXT(const VkCopyMicromapToMemoryInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMemoryToMicromapInfoEXT(const VkCopyMemoryToMicromapInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMicromapInfoEXT(const VkCopyMicromapInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapBuildSizesInfoEXT(const VkMicromapBuildSizesInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureTrianglesOpacityMicromapEXT(const VkAccelerationStructureTrianglesOpacityMicromapEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMicromapTriangleEXT(const VkMicromapTriangleEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDeviceDisplacementMicromapFeaturesNV(const VkPhysicalDeviceDisplacementMicromapFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDeviceDisplacementMicromapPropertiesNV(const VkPhysicalDeviceDisplacementMicromapPropertiesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkAccelerationStructureTrianglesDisplacementMicromapNV(const VkAccelerationStructureTrianglesDisplacementMicromapNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI(const VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI(const VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI(const VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceBorderColorSwizzleFeaturesEXT(const VkPhysicalDeviceBorderColorSwizzleFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerBorderColorComponentMappingCreateInfoEXT(const VkSamplerBorderColorComponentMappingCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT(const VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderCorePropertiesARM(const VkPhysicalDeviceShaderCorePropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceQueueShaderCoreControlCreateInfoARM(const VkDeviceQueueShaderCoreControlCreateInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSchedulingControlsFeaturesARM(const VkPhysicalDeviceSchedulingControlsFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSchedulingControlsPropertiesARM(const VkPhysicalDeviceSchedulingControlsPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT(const VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewSlicedCreateInfoEXT(const VkImageViewSlicedCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE(const VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetBindingReferenceVALVE(const VkDescriptorSetBindingReferenceVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetLayoutHostMappingInfoVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDepthClampZeroOneFeaturesKHR(const VkPhysicalDeviceDepthClampZeroOneFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT(const VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRenderPassStripedFeaturesARM(const VkPhysicalDeviceRenderPassStripedFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRenderPassStripedPropertiesARM(const VkPhysicalDeviceRenderPassStripedPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassStripeInfoARM(const VkRenderPassStripeInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassStripeBeginInfoARM(const VkRenderPassStripeBeginInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassStripeSubmitInfoARM(const VkRenderPassStripeSubmitInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentDensityMapOffsetFeaturesEXT(const VkPhysicalDeviceFragmentDensityMapOffsetFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceFragmentDensityMapOffsetPropertiesEXT(const VkPhysicalDeviceFragmentDensityMapOffsetPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassFragmentDensityMapOffsetEndInfoEXT(const VkRenderPassFragmentDensityMapOffsetEndInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMemoryIndirectCommandNV(const VkCopyMemoryIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCopyMemoryToImageIndirectCommandNV(const VkCopyMemoryToImageIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCopyMemoryIndirectFeaturesNV(const VkPhysicalDeviceCopyMemoryIndirectFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCopyMemoryIndirectPropertiesNV(const VkPhysicalDeviceCopyMemoryIndirectPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDecompressMemoryRegionNV(const VkDecompressMemoryRegionNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMemoryDecompressionFeaturesNV(const VkPhysicalDeviceMemoryDecompressionFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMemoryDecompressionPropertiesNV(const VkPhysicalDeviceMemoryDecompressionPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV(const VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkComputePipelineIndirectBufferInfoNV(const VkComputePipelineIndirectBufferInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineIndirectDeviceAddressInfoNV(const VkPipelineIndirectDeviceAddressInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindPipelineIndirectCommandNV(const VkBindPipelineIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV(const VkPhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryLinearSweptSpheresDataNV(const VkAccelerationStructureGeometryLinearSweptSpheresDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometrySpheresDataNV(const VkAccelerationStructureGeometrySpheresDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLinearColorAttachmentFeaturesNV(const VkPhysicalDeviceLinearColorAttachmentFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR(const VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT(const VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageViewSampleWeightCreateInfoQCOM(const VkImageViewSampleWeightCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageProcessingFeaturesQCOM(const VkPhysicalDeviceImageProcessingFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageProcessingPropertiesQCOM(const VkPhysicalDeviceImageProcessingPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceNestedCommandBufferFeaturesEXT(const VkPhysicalDeviceNestedCommandBufferFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceNestedCommandBufferPropertiesEXT(const VkPhysicalDeviceNestedCommandBufferPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalMemoryAcquireUnmodifiedEXT(const VkExternalMemoryAcquireUnmodifiedEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExtendedDynamicState3FeaturesEXT(const VkPhysicalDeviceExtendedDynamicState3FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExtendedDynamicState3PropertiesEXT(const VkPhysicalDeviceExtendedDynamicState3PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkColorBlendEquationEXT(const VkColorBlendEquationEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkColorBlendAdvancedEXT(const VkColorBlendAdvancedEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT(const VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreationControlEXT(const VkRenderPassCreationControlEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreationFeedbackInfoEXT(const VkRenderPassCreationFeedbackInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassCreationFeedbackCreateInfoEXT(const VkRenderPassCreationFeedbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassSubpassFeedbackInfoEXT(const VkRenderPassSubpassFeedbackInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRenderPassSubpassFeedbackCreateInfoEXT(const VkRenderPassSubpassFeedbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDirectDriverLoadingInfoLUNARG(const VkDirectDriverLoadingInfoLUNARG& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDirectDriverLoadingListLUNARG(const VkDirectDriverLoadingListLUNARG& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT(const VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT(const VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineShaderStageModuleIdentifierCreateInfoEXT(const VkPipelineShaderStageModuleIdentifierCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderModuleIdentifierEXT(const VkShaderModuleIdentifierEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceOpticalFlowFeaturesNV(const VkPhysicalDeviceOpticalFlowFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceOpticalFlowPropertiesNV(const VkPhysicalDeviceOpticalFlowPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowImageFormatInfoNV(const VkOpticalFlowImageFormatInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowImageFormatPropertiesNV(const VkOpticalFlowImageFormatPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowSessionCreateInfoNV(const VkOpticalFlowSessionCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowSessionCreatePrivateDataInfoNV(const VkOpticalFlowSessionCreatePrivateDataInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOpticalFlowExecuteInfoNV(const VkOpticalFlowExecuteInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLegacyDitheringFeaturesEXT(const VkPhysicalDeviceLegacyDitheringFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkPhysicalDeviceExternalFormatResolveFeaturesANDROID(const VkPhysicalDeviceExternalFormatResolveFeaturesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkPhysicalDeviceExternalFormatResolvePropertiesANDROID(const VkPhysicalDeviceExternalFormatResolvePropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_VkAndroidHardwareBufferFormatResolvePropertiesANDROID(const VkAndroidHardwareBufferFormatResolvePropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
void dump_text_VkPhysicalDeviceAntiLagFeaturesAMD(const VkPhysicalDeviceAntiLagFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAntiLagPresentationInfoAMD(const VkAntiLagPresentationInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAntiLagDataAMD(const VkAntiLagDataAMD& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR(const VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderObjectFeaturesEXT(const VkPhysicalDeviceShaderObjectFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderObjectPropertiesEXT(const VkPhysicalDeviceShaderObjectPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkShaderCreateInfoEXT(const VkShaderCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDepthClampRangeEXT(const VkDepthClampRangeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineBinaryFeaturesKHR(const VkPhysicalDevicePipelineBinaryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineBinaryPropertiesKHR(const VkPhysicalDevicePipelineBinaryPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDevicePipelineBinaryInternalCacheControlKHR(const VkDevicePipelineBinaryInternalCacheControlKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryKeyKHR(const VkPipelineBinaryKeyKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryDataKHR(const VkPipelineBinaryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryKeysAndDataKHR(const VkPipelineBinaryKeysAndDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineCreateInfoKHR(const VkPipelineCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryCreateInfoKHR(const VkPipelineBinaryCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryInfoKHR(const VkPipelineBinaryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkReleaseCapturedPipelineDataInfoKHR(const VkReleaseCapturedPipelineDataInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryDataInfoKHR(const VkPipelineBinaryDataInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineBinaryHandlesInfoKHR(const VkPipelineBinaryHandlesInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTilePropertiesFeaturesQCOM(const VkPhysicalDeviceTilePropertiesFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTilePropertiesQCOM(const VkTilePropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceAmigoProfilingFeaturesSEC(const VkPhysicalDeviceAmigoProfilingFeaturesSEC& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAmigoProfilingSubmitInfoSEC(const VkAmigoProfilingSubmitInfoSEC& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM(const VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV(const VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV(const VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeVectorPropertiesNV(const VkPhysicalDeviceCooperativeVectorPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeVectorFeaturesNV(const VkPhysicalDeviceCooperativeVectorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCooperativeVectorPropertiesNV(const VkCooperativeVectorPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkConvertCooperativeVectorMatrixInfoNV(const VkConvertCooperativeVectorMatrixInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV(const VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV(const VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT(const VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT(const VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLayerSettingEXT(const VkLayerSettingEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLayerSettingsCreateInfoEXT(const VkLayerSettingsCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM(const VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM(const VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT(const VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT(const VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLatencySleepModeInfoNV(const VkLatencySleepModeInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLatencySleepInfoNV(const VkLatencySleepInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSetLatencyMarkerInfoNV(const VkSetLatencyMarkerInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLatencyTimingsFrameReportNV(const VkLatencyTimingsFrameReportNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGetLatencyMarkerInfoNV(const VkGetLatencyMarkerInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLatencySubmissionPresentIdNV(const VkLatencySubmissionPresentIdNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSwapchainLatencyCreateInfoNV(const VkSwapchainLatencyCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkOutOfBandQueueTypeInfoNV(const VkOutOfBandQueueTypeInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkLatencySurfaceCapabilitiesNV(const VkLatencySurfaceCapabilitiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCooperativeMatrixPropertiesKHR(const VkCooperativeMatrixPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeMatrixFeaturesKHR(const VkPhysicalDeviceCooperativeMatrixFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeMatrixPropertiesKHR(const VkPhysicalDeviceCooperativeMatrixPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM(const VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM(const VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR(const VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeAV1ProfileInfoKHR(const VkVideoDecodeAV1ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeAV1CapabilitiesKHR(const VkVideoDecodeAV1CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeAV1SessionParametersCreateInfoKHR(const VkVideoDecodeAV1SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeAV1PictureInfoKHR(const VkVideoDecodeAV1PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeAV1DpbSlotInfoKHR(const VkVideoDecodeAV1DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVideoEncodeAV1FeaturesKHR(const VkPhysicalDeviceVideoEncodeAV1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1CapabilitiesKHR(const VkVideoEncodeAV1CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1QIndexKHR(const VkVideoEncodeAV1QIndexKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1QualityLevelPropertiesKHR(const VkVideoEncodeAV1QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1SessionCreateInfoKHR(const VkVideoEncodeAV1SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1SessionParametersCreateInfoKHR(const VkVideoEncodeAV1SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1PictureInfoKHR(const VkVideoEncodeAV1PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1DpbSlotInfoKHR(const VkVideoEncodeAV1DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1ProfileInfoKHR(const VkVideoEncodeAV1ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1FrameSizeKHR(const VkVideoEncodeAV1FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1GopRemainingFrameInfoKHR(const VkVideoEncodeAV1GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1RateControlInfoKHR(const VkVideoEncodeAV1RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1RateControlLayerInfoKHR(const VkVideoEncodeAV1RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVideoMaintenance1FeaturesKHR(const VkPhysicalDeviceVideoMaintenance1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoInlineQueryInfoKHR(const VkVideoInlineQueryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePerStageDescriptorSetFeaturesNV(const VkPhysicalDevicePerStageDescriptorSetFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageProcessing2FeaturesQCOM(const VkPhysicalDeviceImageProcessing2FeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageProcessing2PropertiesQCOM(const VkPhysicalDeviceImageProcessing2PropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerBlockMatchWindowCreateInfoQCOM(const VkSamplerBlockMatchWindowCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCubicWeightsFeaturesQCOM(const VkPhysicalDeviceCubicWeightsFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerCubicWeightsCreateInfoQCOM(const VkSamplerCubicWeightsCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBlitImageCubicWeightsInfoQCOM(const VkBlitImageCubicWeightsInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceYcbcrDegammaFeaturesQCOM(const VkPhysicalDeviceYcbcrDegammaFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM(const VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCubicClampFeaturesQCOM(const VkPhysicalDeviceCubicClampFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT(const VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkScreenBufferPropertiesQNX(const VkScreenBufferPropertiesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkScreenBufferFormatPropertiesQNX(const VkScreenBufferFormatPropertiesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkImportScreenBufferInfoQNX(const VkImportScreenBufferInfoQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkExternalFormatQNX(const VkExternalFormatQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX(const VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_VkPhysicalDeviceLayeredDriverPropertiesMSFT(const VkPhysicalDeviceLayeredDriverPropertiesMSFT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkSetDescriptorBufferOffsetsInfoEXT(const VkSetDescriptorBufferOffsetsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindDescriptorBufferEmbeddedSamplersInfoEXT(const VkBindDescriptorBufferEmbeddedSamplersInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV(const VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTileMemoryHeapFeaturesQCOM(const VkPhysicalDeviceTileMemoryHeapFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceTileMemoryHeapPropertiesQCOM(const VkPhysicalDeviceTileMemoryHeapPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTileMemoryRequirementsQCOM(const VkTileMemoryRequirementsQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTileMemoryBindInfoQCOM(const VkTileMemoryBindInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkTileMemorySizeInfoQCOM(const VkTileMemorySizeInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplaySurfaceStereoCreateInfoNV(const VkDisplaySurfaceStereoCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDisplayModeStereoPropertiesNV(const VkDisplayModeStereoPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeQuantizationMapCapabilitiesKHR(const VkVideoEncodeQuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoFormatQuantizationMapPropertiesKHR(const VkVideoFormatQuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeQuantizationMapInfoKHR(const VkVideoEncodeQuantizationMapInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR(const VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR(const VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH264QuantizationMapCapabilitiesKHR(const VkVideoEncodeH264QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeH265QuantizationMapCapabilitiesKHR(const VkVideoEncodeH265QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoFormatH265QuantizationMapPropertiesKHR(const VkVideoFormatH265QuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoEncodeAV1QuantizationMapCapabilitiesKHR(const VkVideoEncodeAV1QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoFormatAV1QuantizationMapPropertiesKHR(const VkVideoFormatAV1QuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRawAccessChainsFeaturesNV(const VkPhysicalDeviceRawAccessChainsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalComputeQueueDeviceCreateInfoNV(const VkExternalComputeQueueDeviceCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalComputeQueueCreateInfoNV(const VkExternalComputeQueueCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkExternalComputeQueueDataParamsNV(const VkExternalComputeQueueDataParamsNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceExternalComputeQueuePropertiesNV(const VkPhysicalDeviceExternalComputeQueuePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR(const VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCommandBufferInheritanceFeaturesNV(const VkPhysicalDeviceCommandBufferInheritanceFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance7FeaturesKHR(const VkPhysicalDeviceMaintenance7FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance7PropertiesKHR(const VkPhysicalDeviceMaintenance7PropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLayeredApiPropertiesKHR(const VkPhysicalDeviceLayeredApiPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLayeredApiPropertiesListKHR(const VkPhysicalDeviceLayeredApiPropertiesListKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceLayeredApiVulkanPropertiesKHR(const VkPhysicalDeviceLayeredApiVulkanPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV(const VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT(const VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceRayTracingValidationFeaturesNV(const VkPhysicalDeviceRayTracingValidationFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceClusterAccelerationStructureFeaturesNV(const VkPhysicalDeviceClusterAccelerationStructureFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceClusterAccelerationStructurePropertiesNV(const VkPhysicalDeviceClusterAccelerationStructurePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureClustersBottomLevelInputNV(const VkClusterAccelerationStructureClustersBottomLevelInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureTriangleClusterInputNV(const VkClusterAccelerationStructureTriangleClusterInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureMoveObjectsInputNV(const VkClusterAccelerationStructureMoveObjectsInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureInputInfoNV(const VkClusterAccelerationStructureInputInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureCommandsInfoNV(const VkClusterAccelerationStructureCommandsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkStridedDeviceAddressNV(const VkStridedDeviceAddressNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureGeometryIndexAndGeometryFlagsNV(const VkClusterAccelerationStructureGeometryIndexAndGeometryFlagsNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureMoveObjectsInfoNV(const VkClusterAccelerationStructureMoveObjectsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV(const VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureBuildTriangleClusterInfoNV(const VkClusterAccelerationStructureBuildTriangleClusterInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV(const VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureInstantiateClusterInfoNV(const VkClusterAccelerationStructureInstantiateClusterInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkRayTracingPipelineClusterAccelerationStructureCreateInfoNV(const VkRayTracingPipelineClusterAccelerationStructureCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePartitionedAccelerationStructureFeaturesNV(const VkPhysicalDevicePartitionedAccelerationStructureFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePartitionedAccelerationStructurePropertiesNV(const VkPhysicalDevicePartitionedAccelerationStructurePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureFlagsNV(const VkPartitionedAccelerationStructureFlagsNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildPartitionedAccelerationStructureIndirectCommandNV(const VkBuildPartitionedAccelerationStructureIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureWriteInstanceDataNV(const VkPartitionedAccelerationStructureWriteInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureUpdateInstanceDataNV(const VkPartitionedAccelerationStructureUpdateInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureWritePartitionTranslationDataNV(const VkPartitionedAccelerationStructureWritePartitionTranslationDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteDescriptorSetPartitionedAccelerationStructureNV(const VkWriteDescriptorSetPartitionedAccelerationStructureNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPartitionedAccelerationStructureInstancesInputNV(const VkPartitionedAccelerationStructureInstancesInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBuildPartitionedAccelerationStructureInfoNV(const VkBuildPartitionedAccelerationStructureInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT(const VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT(const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeneratedCommandsMemoryRequirementsInfoEXT(const VkGeneratedCommandsMemoryRequirementsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetPipelineInfoEXT(const VkIndirectExecutionSetPipelineInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetShaderLayoutInfoEXT(const VkIndirectExecutionSetShaderLayoutInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetShaderInfoEXT(const VkIndirectExecutionSetShaderInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetCreateInfoEXT(const VkIndirectExecutionSetCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeneratedCommandsInfoEXT(const VkGeneratedCommandsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteIndirectExecutionSetPipelineEXT(const VkWriteIndirectExecutionSetPipelineEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsPushConstantTokenEXT(const VkIndirectCommandsPushConstantTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsVertexBufferTokenEXT(const VkIndirectCommandsVertexBufferTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsIndexBufferTokenEXT(const VkIndirectCommandsIndexBufferTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsExecutionSetTokenEXT(const VkIndirectCommandsExecutionSetTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutTokenEXT(const VkIndirectCommandsLayoutTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsLayoutCreateInfoEXT(const VkIndirectCommandsLayoutCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDrawIndirectCountIndirectCommandEXT(const VkDrawIndirectCountIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindVertexBufferIndirectCommandEXT(const VkBindVertexBufferIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkBindIndexBufferIndirectCommandEXT(const VkBindIndexBufferIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeneratedCommandsPipelineInfoEXT(const VkGeneratedCommandsPipelineInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkGeneratedCommandsShaderInfoEXT(const VkGeneratedCommandsShaderInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkWriteIndirectExecutionSetShaderEXT(const VkWriteIndirectExecutionSetShaderEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceMaintenance8FeaturesKHR(const VkPhysicalDeviceMaintenance8FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkMemoryBarrierAccessFlags3KHR(const VkMemoryBarrierAccessFlags3KHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageAlignmentControlFeaturesMESA(const VkPhysicalDeviceImageAlignmentControlFeaturesMESA& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceImageAlignmentControlPropertiesMESA(const VkPhysicalDeviceImageAlignmentControlPropertiesMESA& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkImageAlignmentControlCreateInfoMESA(const VkImageAlignmentControlCreateInfoMESA& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceDepthClampControlFeaturesEXT(const VkPhysicalDeviceDepthClampControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineViewportDepthClampControlCreateInfoEXT(const VkPipelineViewportDepthClampControlCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceVideoMaintenance2FeaturesKHR(const VkPhysicalDeviceVideoMaintenance2FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH264InlineSessionParametersInfoKHR(const VkVideoDecodeH264InlineSessionParametersInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeH265InlineSessionParametersInfoKHR(const VkVideoDecodeH265InlineSessionParametersInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkVideoDecodeAV1InlineSessionParametersInfoKHR(const VkVideoDecodeAV1InlineSessionParametersInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceHdrVividFeaturesHUAWEI(const VkPhysicalDeviceHdrVividFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkHdrVividDynamicMetadataHUAWEI(const VkHdrVividDynamicMetadataHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkCooperativeMatrixFlexibleDimensionsPropertiesNV(const VkCooperativeMatrixFlexibleDimensionsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeMatrix2FeaturesNV(const VkPhysicalDeviceCooperativeMatrix2FeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDeviceCooperativeMatrix2PropertiesNV(const VkPhysicalDeviceCooperativeMatrix2PropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPhysicalDevicePipelineOpacityMicromapFeaturesARM(const VkPhysicalDevicePipelineOpacityMicromapFeaturesARM& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkImportMemoryMetalHandleInfoEXT(const VkImportMemoryMetalHandleInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkMemoryMetalHandlePropertiesEXT(const VkMemoryMetalHandlePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_VkMemoryGetMetalHandleInfoEXT(const VkMemoryGetMetalHandleInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT(const VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkSetPresentConfigNV(const VkSetPresentConfigNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkPhysicalDevicePresentMeteringFeaturesNV(const VkPhysicalDevicePresentMeteringFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkRenderingEndInfoEXT(const VkRenderingEndInfoEXT& object, const ApiDumpSettings& settings, int indents);

//========================== Union Implementations ==========================//

void dump_text_VkClearColorValue(const VkClearColorValue& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClearValue(const VkClearValue& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceCounterResultKHR(const VkPerformanceCounterResultKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_VkDeviceOrHostAddressConstAMDX(const VkDeviceOrHostAddressConstAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_VkDeviceOrHostAddressKHR(const VkDeviceOrHostAddressKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDeviceOrHostAddressConstKHR(const VkDeviceOrHostAddressConstKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureGeometryDataKHR(const VkAccelerationStructureGeometryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPerformanceValueDataINTEL(const VkPerformanceValueDataINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkPipelineExecutableStatisticValueKHR(const VkPipelineExecutableStatisticValueKHR& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkDescriptorDataEXT(const VkDescriptorDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkAccelerationStructureMotionInstanceDataNV(const VkAccelerationStructureMotionInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkClusterAccelerationStructureOpInputNV(const VkClusterAccelerationStructureOpInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectExecutionSetInfoEXT(const VkIndirectExecutionSetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_text_VkIndirectCommandsTokenDataEXT(const VkIndirectCommandsTokenDataEXT& object, const ApiDumpSettings& settings, int indents);

//======================== pNext Chain Implementation =======================//

void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents, const char* pnext_type);

void dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);

//========================= Function Helpers ================================//

void dump_text_params_vkCreateInstance(ApiDumpInstance& dump_inst, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void dump_text_params_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void dump_text_params_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
void dump_text_params_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void dump_text_params_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void dump_text_params_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void dump_text_params_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void dump_text_params_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void dump_text_params_vkCreateDevice(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
void dump_text_params_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_text_params_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_text_params_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_text_params_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_text_params_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void dump_text_params_vkQueueSubmit(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
void dump_text_params_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_text_params_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkDevice device);
void dump_text_params_vkAllocateMemory(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
void dump_text_params_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkMapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void dump_text_params_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory);
void dump_text_params_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_text_params_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_text_params_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
void dump_text_params_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_text_params_vkBindImageMemory(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_text_params_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
void dump_text_params_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void dump_text_params_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void dump_text_params_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void dump_text_params_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);
void dump_text_params_vkCreateFence(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_text_params_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkResetFences(ApiDumpInstance& dump_inst, VkDevice device, uint32_t fenceCount, const VkFence* pFences);
void dump_text_params_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence);
void dump_text_params_vkWaitForFences(ApiDumpInstance& dump_inst, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
void dump_text_params_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
void dump_text_params_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateEvent(ApiDumpInstance& dump_inst, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void dump_text_params_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetEventStatus(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event);
void dump_text_params_vkSetEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event);
void dump_text_params_vkResetEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event);
void dump_text_params_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
void dump_text_params_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_text_params_vkCreateBuffer(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
void dump_text_params_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateBufferView(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void dump_text_params_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateImage(ApiDumpInstance& dump_inst, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
void dump_text_params_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void dump_text_params_vkCreateImageView(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
void dump_text_params_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void dump_text_params_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void dump_text_params_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void dump_text_params_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);
void dump_text_params_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_params_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_params_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void dump_text_params_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateSampler(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void dump_text_params_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void dump_text_params_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void dump_text_params_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
void dump_text_params_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
void dump_text_params_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
void dump_text_params_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
void dump_text_params_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void dump_text_params_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_text_params_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void dump_text_params_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void dump_text_params_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkResetCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void dump_text_params_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void dump_text_params_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_text_params_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
void dump_text_params_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void dump_text_params_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_text_params_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
void dump_text_params_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_text_params_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth);
void dump_text_params_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void dump_text_params_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void dump_text_params_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void dump_text_params_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void dump_text_params_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void dump_text_params_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void dump_text_params_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
void dump_text_params_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void dump_text_params_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void dump_text_params_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void dump_text_params_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void dump_text_params_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_params_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_params_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_params_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_text_params_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
void dump_text_params_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
void dump_text_params_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
void dump_text_params_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_text_params_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_text_params_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void dump_text_params_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void dump_text_params_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_text_params_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_text_params_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void dump_text_params_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);
void dump_text_params_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_text_params_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_text_params_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_text_params_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_text_params_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void dump_text_params_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void dump_text_params_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_text_params_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void dump_text_params_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_text_params_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void dump_text_params_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void dump_text_params_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void dump_text_params_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_text_params_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_text_params_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_text_params_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_text_params_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_text_params_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_params_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_text_params_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_params_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_text_params_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_text_params_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_text_params_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_text_params_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_text_params_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_text_params_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_text_params_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_text_params_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void dump_text_params_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_text_params_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_text_params_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_text_params_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
void dump_text_params_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
void dump_text_params_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void dump_text_params_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_text_params_vkCmdDrawIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkCmdDrawIndexedIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkCreateRenderPass2(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_text_params_vkCmdBeginRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_text_params_vkCmdNextSubpass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_params_vkCmdEndRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_params_vkResetQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_text_params_vkGetSemaphoreCounterValue(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_text_params_vkWaitSemaphores(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_text_params_vkSignalSemaphore(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_text_params_vkGetBufferDeviceAddress(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_params_vkGetBufferOpaqueCaptureAddress(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_params_vkGetDeviceMemoryOpaqueCaptureAddress(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_text_params_vkGetPhysicalDeviceToolProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_text_params_vkCreatePrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_text_params_vkDestroyPrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkSetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_text_params_vkGetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_text_params_vkCmdSetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_params_vkCmdResetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_text_params_vkCmdWaitEvents2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_text_params_vkCmdPipelineBarrier2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_params_vkCmdWriteTimestamp2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_text_params_vkQueueSubmit2(ApiDumpInstance& dump_inst, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_text_params_vkCmdCopyBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_text_params_vkCmdCopyImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_text_params_vkCmdCopyBufferToImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_text_params_vkCmdCopyImageToBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_text_params_vkCmdBlitImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_text_params_vkCmdResolveImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_text_params_vkCmdBeginRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_text_params_vkCmdEndRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
void dump_text_params_vkCmdSetCullMode(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_text_params_vkCmdSetFrontFace(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_text_params_vkCmdSetPrimitiveTopology(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_text_params_vkCmdSetViewportWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_text_params_vkCmdSetScissorWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_text_params_vkCmdBindVertexBuffers2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_text_params_vkCmdSetDepthTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_text_params_vkCmdSetDepthWriteEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_text_params_vkCmdSetDepthCompareOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_text_params_vkCmdSetDepthBoundsTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_text_params_vkCmdSetStencilTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_text_params_vkCmdSetStencilOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_text_params_vkCmdSetRasterizerDiscardEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_text_params_vkCmdSetDepthBiasEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_text_params_vkCmdSetPrimitiveRestartEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
void dump_text_params_vkGetDeviceBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetDeviceImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetDeviceImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_params_vkCmdSetLineStipple(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_text_params_vkMapMemory2(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_text_params_vkUnmapMemory2(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_text_params_vkCmdBindIndexBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_text_params_vkGetRenderingAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_text_params_vkGetDeviceImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_text_params_vkGetImageSubresourceLayout2(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_text_params_vkCmdPushDescriptorSet(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_text_params_vkCmdPushDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_text_params_vkCmdSetRenderingAttachmentLocations(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_text_params_vkCmdSetRenderingInputAttachmentIndices(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_text_params_vkCmdBindDescriptorSets2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_text_params_vkCmdPushConstants2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_text_params_vkCmdPushDescriptorSet2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_text_params_vkCmdPushDescriptorSetWithTemplate2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_text_params_vkCopyMemoryToImage(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_text_params_vkCopyImageToMemory(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_text_params_vkCopyImageToImage(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_text_params_vkTransitionImageLayout(ApiDumpInstance& dump_inst, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_text_params_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void dump_text_params_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
void dump_text_params_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
void dump_text_params_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
void dump_text_params_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
void dump_text_params_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void dump_text_params_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
void dump_text_params_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
void dump_text_params_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
void dump_text_params_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
void dump_text_params_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
void dump_text_params_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
void dump_text_params_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
void dump_text_params_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
void dump_text_params_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
void dump_text_params_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
void dump_text_params_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
void dump_text_params_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
void dump_text_params_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_text_params_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_params_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_params_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_params_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_params_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_params_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_params_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_params_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_params_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void dump_text_params_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
void dump_text_params_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo);
void dump_text_params_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo);
void dump_text_params_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_text_params_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_text_params_vkGetPhysicalDeviceVideoCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
void dump_text_params_vkGetPhysicalDeviceVideoFormatPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
void dump_text_params_vkCreateVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);
void dump_text_params_vkDestroyVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetVideoSessionMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);
void dump_text_params_vkBindVideoSessionMemoryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);
void dump_text_params_vkCreateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);
void dump_text_params_vkUpdateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);
void dump_text_params_vkDestroyVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCmdBeginVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo);
void dump_text_params_vkCmdEndVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo);
void dump_text_params_vkCmdControlVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo);
void dump_text_params_vkCmdDecodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo);
void dump_text_params_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes);
void dump_text_params_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_text_params_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_text_params_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);
void dump_text_params_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);
void dump_text_params_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);
void dump_text_params_vkCreateCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);
void dump_text_params_vkCreateCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);
void dump_text_params_vkDestroyCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkDestroyCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCmdCuLaunchKernelNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);
void dump_text_params_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_text_params_vkGetImageViewHandle64NVX(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_text_params_vkGetImageViewAddressNVX(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);
void dump_text_params_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);
void dump_text_params_vkCmdBeginRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_text_params_vkCmdEndRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_params_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
void dump_text_params_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_params_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_text_params_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_text_params_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_text_params_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_text_params_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_text_params_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_text_params_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_text_params_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_text_params_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_text_params_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_text_params_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
void dump_text_params_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_text_params_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_text_params_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_params_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_text_params_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);
void dump_text_params_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_params_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
void dump_text_params_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_text_params_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_text_params_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_text_params_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);
void dump_text_params_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_text_params_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_text_params_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void dump_text_params_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_text_params_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_text_params_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
void dump_text_params_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
void dump_text_params_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);
void dump_text_params_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_text_params_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_text_params_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);
void dump_text_params_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);
void dump_text_params_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);
void dump_text_params_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles);
void dump_text_params_vkCmdSetDiscardRectangleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);
void dump_text_params_vkCmdSetDiscardRectangleModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);
void dump_text_params_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);
void dump_text_params_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_text_params_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_text_params_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_params_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_params_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain);
void dump_text_params_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_params_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo);
void dump_text_params_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_text_params_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
void dump_text_params_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
void dump_text_params_vkAcquireProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo);
void dump_text_params_vkReleaseProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device);
void dump_text_params_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
void dump_text_params_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
void dump_text_params_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
void dump_text_params_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
void dump_text_params_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
void dump_text_params_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_text_params_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_text_params_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_text_params_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
void dump_text_params_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo);
void dump_text_params_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_params_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_text_params_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_params_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_params_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_params_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void dump_text_params_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_params_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_params_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCreateExecutionGraphPipelinesAMDX(ApiDumpInstance& dump_inst, VkDevice                                        device, VkPipelineCache                 pipelineCache, uint32_t                                        createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks*    pAllocator, VkPipeline*               pPipelines);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkGetExecutionGraphPipelineScratchSizeAMDX(ApiDumpInstance& dump_inst, VkDevice                                        device, VkPipeline                                      executionGraph, VkExecutionGraphPipelineScratchSizeAMDX*        pSizeInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkGetExecutionGraphPipelineNodeIndexAMDX(ApiDumpInstance& dump_inst, VkDevice                                        device, VkPipeline                                      executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX*  pNodeInfo, uint32_t*                                       pNodeIndex);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCmdInitializeGraphScratchMemoryAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkPipeline                                      executionGraph, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCmdDispatchGraphAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCmdDispatchGraphIndirectAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCmdDispatchGraphIndirectCountAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, VkDeviceAddress                                 countInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_params_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);
void dump_text_params_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
void dump_text_params_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_params_vkCreateAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure);
void dump_text_params_vkDestroyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCmdBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_text_params_vkCmdBuildAccelerationStructuresIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts);
void dump_text_params_vkBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_text_params_vkCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_text_params_vkCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_text_params_vkCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_text_params_vkWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_text_params_vkCmdCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_text_params_vkCmdCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_text_params_vkCmdCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_text_params_vkGetAccelerationStructureDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
void dump_text_params_vkCmdWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_text_params_vkGetDeviceAccelerationStructureCompatibilityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_text_params_vkGetAccelerationStructureBuildSizesKHR(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo);
void dump_text_params_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_text_params_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_text_params_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_text_params_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);
void dump_text_params_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);
void dump_text_params_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);
void dump_text_params_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);
void dump_text_params_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_text_params_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes);
void dump_text_params_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);
void dump_text_params_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);
void dump_text_params_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);
void dump_text_params_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);
void dump_text_params_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);
void dump_text_params_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);
void dump_text_params_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);
void dump_text_params_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_params_vkGetRayTracingShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_text_params_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_text_params_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);
void dump_text_params_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_text_params_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t shader);
void dump_text_params_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_text_params_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);
void dump_text_params_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_text_params_vkCmdWriteBufferMarker2AMD(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker);
void dump_text_params_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_text_params_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_text_params_vkGetCalibratedTimestampsKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_text_params_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_text_params_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);
void dump_text_params_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_params_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkCmdSetExclusiveScissorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables);
void dump_text_params_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors);
void dump_text_params_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker);
void dump_text_params_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);
void dump_text_params_vkGetQueueCheckpointData2NV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);
void dump_text_params_vkGetSemaphoreCounterValueKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_text_params_vkWaitSemaphoresKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_text_params_vkSignalSemaphoreKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_text_params_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);
void dump_text_params_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device);
void dump_text_params_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);
void dump_text_params_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);
void dump_text_params_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo);
void dump_text_params_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);
void dump_text_params_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkDevice device, VkPerformanceConfigurationINTEL configuration);
void dump_text_params_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkQueue queue, VkPerformanceConfigurationINTEL configuration);
void dump_text_params_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);
void dump_text_params_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_params_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_params_vkGetPhysicalDeviceFragmentShadingRatesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
void dump_text_params_vkCmdSetFragmentShadingRateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_text_params_vkCmdSetRenderingAttachmentLocationsKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_text_params_vkCmdSetRenderingInputAttachmentIndicesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_text_params_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_params_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_text_params_vkWaitForPresentKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);
void dump_text_params_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
void dump_text_params_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_params_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_text_params_vkGetBufferDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_params_vkGetBufferOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_params_vkGetDeviceMemoryOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_text_params_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_text_params_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_text_params_vkCmdSetCullModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_text_params_vkCmdSetFrontFaceEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_text_params_vkCmdSetPrimitiveTopologyEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_text_params_vkCmdSetViewportWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_text_params_vkCmdSetScissorWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_text_params_vkCmdBindVertexBuffers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_text_params_vkCmdSetDepthTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_text_params_vkCmdSetDepthWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_text_params_vkCmdSetDepthCompareOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_text_params_vkCmdSetDepthBoundsTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_text_params_vkCmdSetStencilTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_text_params_vkCmdSetStencilOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_text_params_vkCreateDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);
void dump_text_params_vkDestroyDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetDeferredOperationMaxConcurrencyKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation);
void dump_text_params_vkGetDeferredOperationResultKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation);
void dump_text_params_vkDeferredOperationJoinKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation);
void dump_text_params_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);
void dump_text_params_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);
void dump_text_params_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);
void dump_text_params_vkCopyMemoryToImageEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_text_params_vkCopyImageToMemoryEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_text_params_vkCopyImageToImageEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_text_params_vkTransitionImageLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_text_params_vkGetImageSubresourceLayout2EXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_text_params_vkMapMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_text_params_vkUnmapMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_text_params_vkReleaseSwapchainImagesEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);
void dump_text_params_vkGetGeneratedCommandsMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkCmdPreprocessGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_text_params_vkCmdExecuteGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_text_params_vkCmdBindPipelineShaderGroupNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);
void dump_text_params_vkCreateIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);
void dump_text_params_vkDestroyIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCmdSetDepthBias2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo);
void dump_text_params_vkAcquireDrmDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
void dump_text_params_vkGetDrmDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
void dump_text_params_vkCreatePrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_text_params_vkDestroyPrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkSetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_text_params_vkGetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_text_params_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);
void dump_text_params_vkGetEncodedVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);
void dump_text_params_vkCmdEncodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCreateCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkGetCudaModuleCacheNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCreateCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkDestroyCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkDestroyCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_params_vkCmdCudaLaunchKernelNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_params_vkCmdDispatchTileQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkCmdBeginPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileBeginInfoQCOM* pPerTileBeginInfo);
void dump_text_params_vkCmdEndPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileEndInfoQCOM* pPerTileEndInfo);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_params_vkExportMetalObjectsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_params_vkCmdSetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_params_vkCmdResetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_text_params_vkCmdWaitEvents2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_text_params_vkCmdPipelineBarrier2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_params_vkCmdWriteTimestamp2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_text_params_vkQueueSubmit2KHR(ApiDumpInstance& dump_inst, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_text_params_vkGetDescriptorSetLayoutSizeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);
void dump_text_params_vkGetDescriptorSetLayoutBindingOffsetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);
void dump_text_params_vkGetDescriptorEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);
void dump_text_params_vkCmdBindDescriptorBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos);
void dump_text_params_vkCmdSetDescriptorBufferOffsetsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets);
void dump_text_params_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);
void dump_text_params_vkGetBufferOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_params_vkGetImageOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_params_vkGetImageViewOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_params_vkGetSamplerOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_params_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_params_vkCmdSetFragmentShadingRateEnumNV(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_text_params_vkCmdDrawMeshTasksEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_params_vkCmdDrawMeshTasksIndirectEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_params_vkCmdDrawMeshTasksIndirectCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_params_vkCmdCopyBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_text_params_vkCmdCopyImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_text_params_vkCmdCopyBufferToImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_text_params_vkCmdCopyImageToBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_text_params_vkCmdBlitImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_text_params_vkCmdResolveImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_text_params_vkGetDeviceFaultInfoEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkAcquireWinrtDisplayNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_params_vkGetWinrtDisplayNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_params_vkCreateDirectFBSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_params_vkGetPhysicalDeviceDirectFBPresentationSupportEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_text_params_vkCmdTraceRaysKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
void dump_text_params_vkCreateRayTracingPipelinesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_params_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_text_params_vkCmdTraceRaysIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);
void dump_text_params_vkGetRayTracingShaderGroupStackSizeKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);
void dump_text_params_vkCmdSetRayTracingPipelineStackSizeKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);
void dump_text_params_vkCmdSetVertexInputEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkGetMemoryZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkGetMemoryZirconHandlePropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkImportSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkGetSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkCreateBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkSetBufferCollectionImageConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkSetBufferCollectionBufferConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkDestroyBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_params_vkGetBufferCollectionPropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_text_params_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);
void dump_text_params_vkCmdSubpassShadingHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_params_vkCmdBindInvocationMaskHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_text_params_vkGetMemoryRemoteAddressNV(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);
void dump_text_params_vkGetPipelinePropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);
void dump_text_params_vkCmdSetPatchControlPointsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t patchControlPoints);
void dump_text_params_vkCmdSetRasterizerDiscardEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_text_params_vkCmdSetDepthBiasEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_text_params_vkCmdSetLogicOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLogicOp logicOp);
void dump_text_params_vkCmdSetPrimitiveRestartEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_params_vkCreateScreenSurfaceQNX(ApiDumpInstance& dump_inst, VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_params_vkGetPhysicalDeviceScreenPresentationSupportQNX(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_params_vkCmdSetColorWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables);
void dump_text_params_vkCmdTraceRaysIndirect2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);
void dump_text_params_vkCmdDrawMultiEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);
void dump_text_params_vkCmdDrawMultiIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset);
void dump_text_params_vkCreateMicromapEXT(ApiDumpInstance& dump_inst, VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap);
void dump_text_params_vkDestroyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCmdBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_text_params_vkBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_text_params_vkCopyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo);
void dump_text_params_vkCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_text_params_vkCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_text_params_vkWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_text_params_vkCmdCopyMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo);
void dump_text_params_vkCmdCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_text_params_vkCmdCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_text_params_vkCmdWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_text_params_vkGetDeviceMicromapCompatibilityEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_text_params_vkGetMicromapBuildSizesEXT(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo);
void dump_text_params_vkCmdDrawClusterHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_params_vkCmdDrawClusterIndirectHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_text_params_vkSetDeviceMemoryPriorityEXT(ApiDumpInstance& dump_inst, VkDevice       device, VkDeviceMemory memory, float          priority);
void dump_text_params_vkGetDeviceBufferMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetDeviceImageMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkGetDeviceImageSparseMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_params_vkGetDescriptorSetLayoutHostMappingInfoVALVE(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);
void dump_text_params_vkGetDescriptorSetHostMappingVALVE(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, void** ppData);
void dump_text_params_vkCmdCopyMemoryIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);
void dump_text_params_vkCmdCopyMemoryToImageIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources);
void dump_text_params_vkCmdDecompressMemoryNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions);
void dump_text_params_vkCmdDecompressMemoryIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);
void dump_text_params_vkGetPipelineIndirectMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkCmdUpdatePipelineIndirectBufferNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline);
void dump_text_params_vkGetPipelineIndirectDeviceAddressNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo);
void dump_text_params_vkCmdSetDepthClampEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);
void dump_text_params_vkCmdSetPolygonModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);
void dump_text_params_vkCmdSetRasterizationSamplesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples);
void dump_text_params_vkCmdSetSampleMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask);
void dump_text_params_vkCmdSetAlphaToCoverageEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);
void dump_text_params_vkCmdSetAlphaToOneEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);
void dump_text_params_vkCmdSetLogicOpEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);
void dump_text_params_vkCmdSetColorBlendEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables);
void dump_text_params_vkCmdSetColorBlendEquationEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations);
void dump_text_params_vkCmdSetColorWriteMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks);
void dump_text_params_vkCmdSetTessellationDomainOriginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);
void dump_text_params_vkCmdSetRasterizationStreamEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t rasterizationStream);
void dump_text_params_vkCmdSetConservativeRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);
void dump_text_params_vkCmdSetExtraPrimitiveOverestimationSizeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);
void dump_text_params_vkCmdSetDepthClipEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);
void dump_text_params_vkCmdSetSampleLocationsEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);
void dump_text_params_vkCmdSetColorBlendAdvancedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced);
void dump_text_params_vkCmdSetProvokingVertexModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);
void dump_text_params_vkCmdSetLineRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);
void dump_text_params_vkCmdSetLineStippleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);
void dump_text_params_vkCmdSetDepthClipNegativeOneToOneEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);
void dump_text_params_vkCmdSetViewportWScalingEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);
void dump_text_params_vkCmdSetViewportSwizzleNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles);
void dump_text_params_vkCmdSetCoverageToColorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);
void dump_text_params_vkCmdSetCoverageToColorLocationNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);
void dump_text_params_vkCmdSetCoverageModulationModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);
void dump_text_params_vkCmdSetCoverageModulationTableEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);
void dump_text_params_vkCmdSetCoverageModulationTableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable);
void dump_text_params_vkCmdSetShadingRateImageEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);
void dump_text_params_vkCmdSetRepresentativeFragmentTestEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);
void dump_text_params_vkCmdSetCoverageReductionModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);
void dump_text_params_vkGetShaderModuleIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_text_params_vkGetShaderModuleCreateInfoIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_text_params_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
void dump_text_params_vkCreateOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);
void dump_text_params_vkDestroyOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkBindOpticalFlowSessionImageNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);
void dump_text_params_vkCmdOpticalFlowExecuteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo);
void dump_text_params_vkCmdBindIndexBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_text_params_vkGetRenderingAreaGranularityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_text_params_vkGetDeviceImageSubresourceLayoutKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_text_params_vkGetImageSubresourceLayout2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_text_params_vkAntiLagUpdateAMD(ApiDumpInstance& dump_inst, VkDevice device, const VkAntiLagDataAMD* pData);
void dump_text_params_vkCreateShadersEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);
void dump_text_params_vkDestroyShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetShaderBinaryDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);
void dump_text_params_vkCmdBindShadersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders);
void dump_text_params_vkCmdSetDepthClampRangeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange);
void dump_text_params_vkCreatePipelineBinariesKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries);
void dump_text_params_vkDestroyPipelineBinaryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetPipelineKeyKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey);
void dump_text_params_vkGetPipelineBinaryDataKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData);
void dump_text_params_vkReleaseCapturedPipelineDataKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetFramebufferTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);
void dump_text_params_vkGetDynamicRenderingTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);
void dump_text_params_vkGetPhysicalDeviceCooperativeVectorPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeVectorPropertiesNV* pProperties);
void dump_text_params_vkConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkDevice device, const VkConvertCooperativeVectorMatrixInfoNV* pInfo);
void dump_text_params_vkCmdConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkConvertCooperativeVectorMatrixInfoNV* pInfos);
void dump_text_params_vkSetLatencySleepModeNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo);
void dump_text_params_vkLatencySleepNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo);
void dump_text_params_vkSetLatencyMarkerNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_text_params_vkGetLatencyTimingsNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_text_params_vkQueueNotifyOutOfBandNV(ApiDumpInstance& dump_inst, VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);
void dump_text_params_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
void dump_text_params_vkCmdSetAttachmentFeedbackLoopEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_params_vkGetScreenBufferPropertiesQNX(ApiDumpInstance& dump_inst, VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_params_vkCmdSetLineStippleKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_text_params_vkCmdBindDescriptorSets2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_text_params_vkCmdPushConstants2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_text_params_vkCmdPushDescriptorSet2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_text_params_vkCmdPushDescriptorSetWithTemplate2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_text_params_vkCmdSetDescriptorBufferOffsets2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);
void dump_text_params_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);
void dump_text_params_vkCmdBindTileMemoryQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkTileMemoryBindInfoQCOM* pTileMemoryBindInfo);
void dump_text_params_vkCreateExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, const VkExternalComputeQueueCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkExternalComputeQueueNV* pExternalQueue);
void dump_text_params_vkDestroyExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, VkExternalComputeQueueNV externalQueue, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkGetExternalComputeQueueDataNV(ApiDumpInstance& dump_inst, VkExternalComputeQueueNV externalQueue, VkExternalComputeQueueDataParamsNV* params, void* pData);
void dump_text_params_vkGetClusterAccelerationStructureBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkClusterAccelerationStructureInputInfoNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_text_params_vkCmdBuildClusterAccelerationStructureIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer                     commandBuffer, const VkClusterAccelerationStructureCommandsInfoNV*  pCommandInfos);
void dump_text_params_vkGetPartitionedAccelerationStructuresBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPartitionedAccelerationStructureInstancesInputNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR*                  pSizeInfo);
void dump_text_params_vkCmdBuildPartitionedAccelerationStructuresNV(ApiDumpInstance& dump_inst, VkCommandBuffer                     commandBuffer, const VkBuildPartitionedAccelerationStructureInfoNV*  pBuildInfo);
void dump_text_params_vkGetGeneratedCommandsMemoryRequirementsEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_params_vkCmdPreprocessGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer);
void dump_text_params_vkCmdExecuteGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo);
void dump_text_params_vkCreateIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout);
void dump_text_params_vkDestroyIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkCreateIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet);
void dump_text_params_vkDestroyIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator);
void dump_text_params_vkUpdateIndirectExecutionSetPipelineEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites);
void dump_text_params_vkUpdateIndirectExecutionSetShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites);
void dump_text_params_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_params_vkGetMemoryMetalHandleEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetMetalHandleInfoEXT* pGetMetalHandleInfo, void** pHandle);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_params_vkGetMemoryMetalHandlePropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHandle, VkMemoryMetalHandlePropertiesEXT* pMemoryMetalHandleProperties);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_params_vkCmdEndRendering2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingEndInfoEXT*        pRenderingEndInfo);

//========================= Function Implementations ========================//

void dump_text_vkCreateInstance(ApiDumpInstance& dump_inst, VkResult result, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void dump_text_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator);
void dump_text_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void dump_text_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
void dump_text_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void dump_text_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void dump_text_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void dump_text_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void dump_text_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void dump_text_vkCreateDevice(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
void dump_text_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator);
void dump_text_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_text_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_text_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_text_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_text_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void dump_text_vkQueueSubmit(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
void dump_text_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue);
void dump_text_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkDevice device);
void dump_text_vkAllocateMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
void dump_text_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
void dump_text_vkMapMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void dump_text_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory);
void dump_text_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_text_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_text_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
void dump_text_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_text_vkBindImageMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_text_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
void dump_text_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void dump_text_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void dump_text_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void dump_text_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);
void dump_text_vkCreateFence(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_text_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
void dump_text_vkResetFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences);
void dump_text_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFence fence);
void dump_text_vkWaitForFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
void dump_text_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
void dump_text_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void dump_text_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetEventStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_text_vkSetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_text_vkResetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_text_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
void dump_text_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_text_vkCreateBuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
void dump_text_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateBufferView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void dump_text_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
void dump_text_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void dump_text_vkCreateImageView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
void dump_text_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void dump_text_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void dump_text_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void dump_text_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);
void dump_text_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void dump_text_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateSampler(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void dump_text_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void dump_text_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void dump_text_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
void dump_text_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
void dump_text_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
void dump_text_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
void dump_text_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
void dump_text_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void dump_text_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_text_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void dump_text_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void dump_text_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
void dump_text_vkResetCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void dump_text_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void dump_text_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_text_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
void dump_text_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer);
void dump_text_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void dump_text_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_text_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
void dump_text_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_text_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth);
void dump_text_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void dump_text_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void dump_text_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void dump_text_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void dump_text_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void dump_text_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void dump_text_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
void dump_text_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void dump_text_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void dump_text_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void dump_text_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void dump_text_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_text_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
void dump_text_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
void dump_text_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
void dump_text_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_text_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_text_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void dump_text_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void dump_text_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_text_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_text_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void dump_text_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);
void dump_text_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_text_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_text_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_text_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_text_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void dump_text_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void dump_text_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_text_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void dump_text_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_text_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void dump_text_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void dump_text_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void dump_text_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_text_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_text_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_text_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_text_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_text_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_text_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_text_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_text_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_text_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_text_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_text_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_text_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_text_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_text_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void dump_text_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_text_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_text_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_text_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_text_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
void dump_text_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
void dump_text_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void dump_text_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_text_vkCmdDrawIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkCmdDrawIndexedIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkCreateRenderPass2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_text_vkCmdBeginRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_text_vkCmdNextSubpass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_vkCmdEndRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_vkResetQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_text_vkGetSemaphoreCounterValue(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_text_vkWaitSemaphores(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_text_vkSignalSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_text_vkGetBufferDeviceAddress(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_vkGetBufferOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_vkGetDeviceMemoryOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_text_vkGetPhysicalDeviceToolProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_text_vkCreatePrivateDataSlot(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_text_vkDestroyPrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_text_vkSetPrivateData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_text_vkGetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_text_vkCmdSetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_vkCmdResetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_text_vkCmdWaitEvents2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_text_vkCmdPipelineBarrier2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_vkCmdWriteTimestamp2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_text_vkQueueSubmit2(ApiDumpInstance& dump_inst, VkResult result, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_text_vkCmdCopyBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_text_vkCmdCopyImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_text_vkCmdCopyBufferToImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_text_vkCmdCopyImageToBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_text_vkCmdBlitImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_text_vkCmdResolveImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_text_vkCmdBeginRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_text_vkCmdEndRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
void dump_text_vkCmdSetCullMode(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_text_vkCmdSetFrontFace(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_text_vkCmdSetPrimitiveTopology(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_text_vkCmdSetViewportWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_text_vkCmdSetScissorWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_text_vkCmdBindVertexBuffers2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_text_vkCmdSetDepthTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_text_vkCmdSetDepthWriteEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_text_vkCmdSetDepthCompareOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_text_vkCmdSetDepthBoundsTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_text_vkCmdSetStencilTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_text_vkCmdSetStencilOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_text_vkCmdSetRasterizerDiscardEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_text_vkCmdSetDepthBiasEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_text_vkCmdSetPrimitiveRestartEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
void dump_text_vkGetDeviceBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetDeviceImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetDeviceImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_vkCmdSetLineStipple(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_text_vkMapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_text_vkUnmapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_text_vkCmdBindIndexBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_text_vkGetRenderingAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_text_vkGetDeviceImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_text_vkGetImageSubresourceLayout2(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_text_vkCmdPushDescriptorSet(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_text_vkCmdPushDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_text_vkCmdSetRenderingAttachmentLocations(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_text_vkCmdSetRenderingInputAttachmentIndices(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_text_vkCmdBindDescriptorSets2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_text_vkCmdPushConstants2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_text_vkCmdPushDescriptorSet2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_text_vkCmdPushDescriptorSetWithTemplate2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_text_vkCopyMemoryToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_text_vkCopyImageToMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_text_vkCopyImageToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_text_vkTransitionImageLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_text_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void dump_text_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
void dump_text_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
void dump_text_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
void dump_text_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
void dump_text_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void dump_text_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
void dump_text_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
void dump_text_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
void dump_text_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
void dump_text_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
void dump_text_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
void dump_text_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
void dump_text_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
void dump_text_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
void dump_text_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
void dump_text_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
void dump_text_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
void dump_text_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_text_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_text_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_text_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_text_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void dump_text_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
void dump_text_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
void dump_text_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo);
void dump_text_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo);
void dump_text_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_text_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_text_vkGetPhysicalDeviceVideoCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
void dump_text_vkGetPhysicalDeviceVideoFormatPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
void dump_text_vkCreateVideoSessionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);
void dump_text_vkDestroyVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetVideoSessionMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);
void dump_text_vkBindVideoSessionMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);
void dump_text_vkCreateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);
void dump_text_vkUpdateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);
void dump_text_vkDestroyVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCmdBeginVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo);
void dump_text_vkCmdEndVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo);
void dump_text_vkCmdControlVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo);
void dump_text_vkCmdDecodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo);
void dump_text_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes);
void dump_text_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_text_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_text_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);
void dump_text_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);
void dump_text_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);
void dump_text_vkCreateCuModuleNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);
void dump_text_vkCreateCuFunctionNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);
void dump_text_vkDestroyCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);
void dump_text_vkDestroyCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCmdCuLaunchKernelNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);
void dump_text_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_text_vkGetImageViewHandle64NVX(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_text_vkGetImageViewAddressNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);
void dump_text_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);
void dump_text_vkCmdBeginRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_text_vkCmdEndRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
#if defined(VK_USE_PLATFORM_GGP)
void dump_text_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
void dump_text_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_text_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_text_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_text_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_text_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_text_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_text_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_text_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_text_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_text_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_text_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
void dump_text_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_text_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_text_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_text_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);
void dump_text_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
void dump_text_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_text_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_text_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_text_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);
void dump_text_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_text_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_text_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_text_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void dump_text_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_text_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_text_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
void dump_text_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
void dump_text_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);
void dump_text_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_text_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_text_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);
void dump_text_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);
void dump_text_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);
void dump_text_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles);
void dump_text_vkCmdSetDiscardRectangleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);
void dump_text_vkCmdSetDiscardRectangleModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);
void dump_text_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);
void dump_text_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_text_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_text_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_text_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
void dump_text_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo);
void dump_text_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_text_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
void dump_text_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
void dump_text_vkAcquireProfilingLockKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo);
void dump_text_vkReleaseProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device);
void dump_text_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
void dump_text_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
void dump_text_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
void dump_text_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
void dump_text_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
void dump_text_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_text_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_text_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_text_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
void dump_text_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo);
void dump_text_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_text_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_text_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void dump_text_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
void dump_text_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCreateExecutionGraphPipelinesAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipelineCache                 pipelineCache, uint32_t                                        createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks*    pAllocator, VkPipeline*               pPipelines);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkGetExecutionGraphPipelineScratchSizeAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipeline                                      executionGraph, VkExecutionGraphPipelineScratchSizeAMDX*        pSizeInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkGetExecutionGraphPipelineNodeIndexAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipeline                                      executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX*  pNodeInfo, uint32_t*                                       pNodeIndex);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCmdInitializeGraphScratchMemoryAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkPipeline                                      executionGraph, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCmdDispatchGraphAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCmdDispatchGraphIndirectAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCmdDispatchGraphIndirectCountAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, VkDeviceAddress                                 countInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);
void dump_text_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
void dump_text_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_vkCreateAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure);
void dump_text_vkDestroyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCmdBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_text_vkCmdBuildAccelerationStructuresIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts);
void dump_text_vkBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_text_vkCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_text_vkCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_text_vkCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_text_vkWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_text_vkCmdCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_text_vkCmdCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_text_vkCmdCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_text_vkGetAccelerationStructureDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
void dump_text_vkCmdWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_text_vkGetDeviceAccelerationStructureCompatibilityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_text_vkGetAccelerationStructureBuildSizesKHR(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo);
void dump_text_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_text_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_text_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_text_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_text_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);
void dump_text_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);
void dump_text_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);
void dump_text_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);
void dump_text_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);
void dump_text_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_text_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes);
void dump_text_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);
void dump_text_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);
void dump_text_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);
void dump_text_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);
void dump_text_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);
void dump_text_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);
void dump_text_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);
void dump_text_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_vkGetRayTracingShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_text_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_text_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);
void dump_text_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_text_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t shader);
void dump_text_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_text_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);
void dump_text_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_text_vkCmdWriteBufferMarker2AMD(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker);
void dump_text_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_text_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_text_vkGetCalibratedTimestampsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_text_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_text_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);
void dump_text_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkCmdSetExclusiveScissorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables);
void dump_text_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors);
void dump_text_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker);
void dump_text_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);
void dump_text_vkGetQueueCheckpointData2NV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);
void dump_text_vkGetSemaphoreCounterValueKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_text_vkWaitSemaphoresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_text_vkSignalSemaphoreKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_text_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);
void dump_text_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device);
void dump_text_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);
void dump_text_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);
void dump_text_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo);
void dump_text_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);
void dump_text_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceConfigurationINTEL configuration);
void dump_text_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, VkPerformanceConfigurationINTEL configuration);
void dump_text_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);
void dump_text_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_vkGetPhysicalDeviceFragmentShadingRatesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
void dump_text_vkCmdSetFragmentShadingRateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_text_vkCmdSetRenderingAttachmentLocationsKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_text_vkCmdSetRenderingInputAttachmentIndicesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_text_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_text_vkWaitForPresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);
void dump_text_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
void dump_text_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_text_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_text_vkGetBufferDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_vkGetBufferOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_text_vkGetDeviceMemoryOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_text_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_text_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_text_vkCmdSetCullModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_text_vkCmdSetFrontFaceEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_text_vkCmdSetPrimitiveTopologyEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_text_vkCmdSetViewportWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_text_vkCmdSetScissorWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_text_vkCmdBindVertexBuffers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_text_vkCmdSetDepthTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_text_vkCmdSetDepthWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_text_vkCmdSetDepthCompareOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_text_vkCmdSetDepthBoundsTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_text_vkCmdSetStencilTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_text_vkCmdSetStencilOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_text_vkCreateDeferredOperationKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);
void dump_text_vkDestroyDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetDeferredOperationMaxConcurrencyKHR(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, VkDeferredOperationKHR operation);
void dump_text_vkGetDeferredOperationResultKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_text_vkDeferredOperationJoinKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_text_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);
void dump_text_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);
void dump_text_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);
void dump_text_vkCopyMemoryToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_text_vkCopyImageToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_text_vkCopyImageToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_text_vkTransitionImageLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_text_vkGetImageSubresourceLayout2EXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_text_vkMapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_text_vkUnmapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_text_vkReleaseSwapchainImagesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);
void dump_text_vkGetGeneratedCommandsMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkCmdPreprocessGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_text_vkCmdExecuteGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_text_vkCmdBindPipelineShaderGroupNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);
void dump_text_vkCreateIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);
void dump_text_vkDestroyIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCmdSetDepthBias2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo);
void dump_text_vkAcquireDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
void dump_text_vkGetDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
void dump_text_vkCreatePrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_text_vkDestroyPrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_text_vkSetPrivateDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_text_vkGetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_text_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);
void dump_text_vkGetEncodedVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);
void dump_text_vkCmdEncodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCreateCudaModuleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkGetCudaModuleCacheNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCreateCudaFunctionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkDestroyCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkDestroyCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_text_vkCmdCudaLaunchKernelNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_text_vkCmdDispatchTileQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_vkCmdBeginPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileBeginInfoQCOM* pPerTileBeginInfo);
void dump_text_vkCmdEndPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileEndInfoQCOM* pPerTileEndInfo);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_vkExportMetalObjectsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_vkCmdSetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_vkCmdResetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_text_vkCmdWaitEvents2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_text_vkCmdPipelineBarrier2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_text_vkCmdWriteTimestamp2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_text_vkQueueSubmit2KHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_text_vkGetDescriptorSetLayoutSizeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);
void dump_text_vkGetDescriptorSetLayoutBindingOffsetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);
void dump_text_vkGetDescriptorEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);
void dump_text_vkCmdBindDescriptorBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos);
void dump_text_vkCmdSetDescriptorBufferOffsetsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets);
void dump_text_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);
void dump_text_vkGetBufferOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_vkGetImageOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_vkGetImageViewOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_vkGetSamplerOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_text_vkCmdSetFragmentShadingRateEnumNV(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_text_vkCmdDrawMeshTasksEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_vkCmdDrawMeshTasksIndirectEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_text_vkCmdDrawMeshTasksIndirectCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_text_vkCmdCopyBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_text_vkCmdCopyImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_text_vkCmdCopyBufferToImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_text_vkCmdCopyImageToBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_text_vkCmdBlitImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_text_vkCmdResolveImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_text_vkGetDeviceFaultInfoEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkAcquireWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_text_vkGetWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_vkCreateDirectFBSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_text_vkGetPhysicalDeviceDirectFBPresentationSupportEXT(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_text_vkCmdTraceRaysKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
void dump_text_vkCreateRayTracingPipelinesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_text_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_text_vkCmdTraceRaysIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);
void dump_text_vkGetRayTracingShaderGroupStackSizeKHR(ApiDumpInstance& dump_inst, VkDeviceSize result, VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);
void dump_text_vkCmdSetRayTracingPipelineStackSizeKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);
void dump_text_vkCmdSetVertexInputEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkGetMemoryZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkGetMemoryZirconHandlePropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkImportSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkGetSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkCreateBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkSetBufferCollectionImageConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkSetBufferCollectionBufferConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkDestroyBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_text_vkGetBufferCollectionPropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_text_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);
void dump_text_vkCmdSubpassShadingHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_text_vkCmdBindInvocationMaskHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_text_vkGetMemoryRemoteAddressNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);
void dump_text_vkGetPipelinePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);
void dump_text_vkCmdSetPatchControlPointsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t patchControlPoints);
void dump_text_vkCmdSetRasterizerDiscardEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_text_vkCmdSetDepthBiasEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_text_vkCmdSetLogicOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLogicOp logicOp);
void dump_text_vkCmdSetPrimitiveRestartEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_vkCreateScreenSurfaceQNX(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_vkGetPhysicalDeviceScreenPresentationSupportQNX(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_vkCmdSetColorWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables);
void dump_text_vkCmdTraceRaysIndirect2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);
void dump_text_vkCmdDrawMultiEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);
void dump_text_vkCmdDrawMultiIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset);
void dump_text_vkCreateMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap);
void dump_text_vkDestroyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCmdBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_text_vkBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_text_vkCopyMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo);
void dump_text_vkCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_text_vkCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_text_vkWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_text_vkCmdCopyMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo);
void dump_text_vkCmdCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_text_vkCmdCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_text_vkCmdWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_text_vkGetDeviceMicromapCompatibilityEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_text_vkGetMicromapBuildSizesEXT(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo);
void dump_text_vkCmdDrawClusterHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_text_vkCmdDrawClusterIndirectHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_text_vkSetDeviceMemoryPriorityEXT(ApiDumpInstance& dump_inst, VkDevice       device, VkDeviceMemory memory, float          priority);
void dump_text_vkGetDeviceBufferMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetDeviceImageMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkGetDeviceImageSparseMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_text_vkGetDescriptorSetLayoutHostMappingInfoVALVE(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);
void dump_text_vkGetDescriptorSetHostMappingVALVE(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, void** ppData);
void dump_text_vkCmdCopyMemoryIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);
void dump_text_vkCmdCopyMemoryToImageIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources);
void dump_text_vkCmdDecompressMemoryNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions);
void dump_text_vkCmdDecompressMemoryIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);
void dump_text_vkGetPipelineIndirectMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkCmdUpdatePipelineIndirectBufferNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline);
void dump_text_vkGetPipelineIndirectDeviceAddressNV(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo);
void dump_text_vkCmdSetDepthClampEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);
void dump_text_vkCmdSetPolygonModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);
void dump_text_vkCmdSetRasterizationSamplesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples);
void dump_text_vkCmdSetSampleMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask);
void dump_text_vkCmdSetAlphaToCoverageEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);
void dump_text_vkCmdSetAlphaToOneEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);
void dump_text_vkCmdSetLogicOpEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);
void dump_text_vkCmdSetColorBlendEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables);
void dump_text_vkCmdSetColorBlendEquationEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations);
void dump_text_vkCmdSetColorWriteMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks);
void dump_text_vkCmdSetTessellationDomainOriginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);
void dump_text_vkCmdSetRasterizationStreamEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t rasterizationStream);
void dump_text_vkCmdSetConservativeRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);
void dump_text_vkCmdSetExtraPrimitiveOverestimationSizeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);
void dump_text_vkCmdSetDepthClipEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);
void dump_text_vkCmdSetSampleLocationsEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);
void dump_text_vkCmdSetColorBlendAdvancedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced);
void dump_text_vkCmdSetProvokingVertexModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);
void dump_text_vkCmdSetLineRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);
void dump_text_vkCmdSetLineStippleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);
void dump_text_vkCmdSetDepthClipNegativeOneToOneEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);
void dump_text_vkCmdSetViewportWScalingEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);
void dump_text_vkCmdSetViewportSwizzleNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles);
void dump_text_vkCmdSetCoverageToColorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);
void dump_text_vkCmdSetCoverageToColorLocationNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);
void dump_text_vkCmdSetCoverageModulationModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);
void dump_text_vkCmdSetCoverageModulationTableEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);
void dump_text_vkCmdSetCoverageModulationTableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable);
void dump_text_vkCmdSetShadingRateImageEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);
void dump_text_vkCmdSetRepresentativeFragmentTestEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);
void dump_text_vkCmdSetCoverageReductionModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);
void dump_text_vkGetShaderModuleIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_text_vkGetShaderModuleCreateInfoIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_text_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
void dump_text_vkCreateOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);
void dump_text_vkDestroyOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator);
void dump_text_vkBindOpticalFlowSessionImageNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);
void dump_text_vkCmdOpticalFlowExecuteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo);
void dump_text_vkCmdBindIndexBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_text_vkGetRenderingAreaGranularityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_text_vkGetDeviceImageSubresourceLayoutKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_text_vkGetImageSubresourceLayout2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_text_vkAntiLagUpdateAMD(ApiDumpInstance& dump_inst, VkDevice device, const VkAntiLagDataAMD* pData);
void dump_text_vkCreateShadersEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);
void dump_text_vkDestroyShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetShaderBinaryDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);
void dump_text_vkCmdBindShadersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders);
void dump_text_vkCmdSetDepthClampRangeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange);
void dump_text_vkCreatePipelineBinariesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries);
void dump_text_vkDestroyPipelineBinaryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetPipelineKeyKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey);
void dump_text_vkGetPipelineBinaryDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData);
void dump_text_vkReleaseCapturedPipelineDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetFramebufferTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);
void dump_text_vkGetDynamicRenderingTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);
void dump_text_vkGetPhysicalDeviceCooperativeVectorPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeVectorPropertiesNV* pProperties);
void dump_text_vkConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkConvertCooperativeVectorMatrixInfoNV* pInfo);
void dump_text_vkCmdConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkConvertCooperativeVectorMatrixInfoNV* pInfos);
void dump_text_vkSetLatencySleepModeNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo);
void dump_text_vkLatencySleepNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo);
void dump_text_vkSetLatencyMarkerNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_text_vkGetLatencyTimingsNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_text_vkQueueNotifyOutOfBandNV(ApiDumpInstance& dump_inst, VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);
void dump_text_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
void dump_text_vkCmdSetAttachmentFeedbackLoopEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_text_vkGetScreenBufferPropertiesQNX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_text_vkCmdSetLineStippleKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_text_vkCmdBindDescriptorSets2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_text_vkCmdPushConstants2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_text_vkCmdPushDescriptorSet2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_text_vkCmdPushDescriptorSetWithTemplate2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_text_vkCmdSetDescriptorBufferOffsets2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);
void dump_text_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);
void dump_text_vkCmdBindTileMemoryQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkTileMemoryBindInfoQCOM* pTileMemoryBindInfo);
void dump_text_vkCreateExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkExternalComputeQueueCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkExternalComputeQueueNV* pExternalQueue);
void dump_text_vkDestroyExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, VkExternalComputeQueueNV externalQueue, const VkAllocationCallbacks* pAllocator);
void dump_text_vkGetExternalComputeQueueDataNV(ApiDumpInstance& dump_inst, VkExternalComputeQueueNV externalQueue, VkExternalComputeQueueDataParamsNV* params, void* pData);
void dump_text_vkGetClusterAccelerationStructureBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkClusterAccelerationStructureInputInfoNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_text_vkCmdBuildClusterAccelerationStructureIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer                     commandBuffer, const VkClusterAccelerationStructureCommandsInfoNV*  pCommandInfos);
void dump_text_vkGetPartitionedAccelerationStructuresBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPartitionedAccelerationStructureInstancesInputNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR*                  pSizeInfo);
void dump_text_vkCmdBuildPartitionedAccelerationStructuresNV(ApiDumpInstance& dump_inst, VkCommandBuffer                     commandBuffer, const VkBuildPartitionedAccelerationStructureInfoNV*  pBuildInfo);
void dump_text_vkGetGeneratedCommandsMemoryRequirementsEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_text_vkCmdPreprocessGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer);
void dump_text_vkCmdExecuteGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo);
void dump_text_vkCreateIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout);
void dump_text_vkDestroyIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_text_vkCreateIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet);
void dump_text_vkDestroyIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator);
void dump_text_vkUpdateIndirectExecutionSetPipelineEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites);
void dump_text_vkUpdateIndirectExecutionSetShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites);
void dump_text_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_vkGetMemoryMetalHandleEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetMetalHandleInfoEXT* pGetMetalHandleInfo, void** pHandle);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_vkGetMemoryMetalHandlePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHandle, VkMemoryMetalHandlePropertiesEXT* pMemoryMetalHandleProperties);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_text_vkCmdEndRendering2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingEndInfoEXT*        pRenderingEndInfo);


