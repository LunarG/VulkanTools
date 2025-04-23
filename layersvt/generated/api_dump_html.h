
/* Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (c) 2015-2017, 2019 Google Inc.
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
 * Author: Joey Bzdek <joey@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#pragma once

#include "api_dump.h"

void dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);


void dump_html_VkClearColorValue(const VkClearColorValue& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClearValue(const VkClearValue& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterResultKHR(const VkPerformanceCounterResultKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkDeviceOrHostAddressConstAMDX(const VkDeviceOrHostAddressConstAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkDeviceOrHostAddressKHR(const VkDeviceOrHostAddressKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceOrHostAddressConstKHR(const VkDeviceOrHostAddressConstKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryDataKHR(const VkAccelerationStructureGeometryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceValueDataINTEL(const VkPerformanceValueDataINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineExecutableStatisticValueKHR(const VkPipelineExecutableStatisticValueKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorDataEXT(const VkDescriptorDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInstanceDataNV(const VkAccelerationStructureMotionInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureOpInputNV(const VkClusterAccelerationStructureOpInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetInfoEXT(const VkIndirectExecutionSetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsTokenDataEXT(const VkIndirectCommandsTokenDataEXT& object, const ApiDumpSettings& settings, int indents);

//=========================== Type Implementations ==========================//

void dump_html_uint64_t(uint64_t object, const ApiDumpSettings& settings, int indents);
void dump_html_size_t(size_t object, const ApiDumpSettings& settings, int indents);
void dump_html_char(char object, const ApiDumpSettings& settings, int indents);
void dump_html_float(float object, const ApiDumpSettings& settings, int indents);
void dump_html_int64_t(int64_t object, const ApiDumpSettings& settings, int indents);
void dump_html_double(double object, const ApiDumpSettings& settings, int indents);

//========================= Basetype Implementations ========================//

void dump_html_VkBool32(VkBool32 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceAddress(VkDeviceAddress object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceSize(VkDeviceSize object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFlags(VkFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSampleMask(VkSampleMask object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFlags64(VkFlags64 object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_MTLDevice_id(MTLDevice_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_MTLCommandQueue_id(MTLCommandQueue_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_MTLBuffer_id(MTLBuffer_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_MTLTexture_id(MTLTexture_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_IOSurfaceRef(IOSurfaceRef object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_MTLSharedEvent_id(MTLSharedEvent_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkRemoteAddressNV(VkRemoteAddressNV object, const ApiDumpSettings& settings, int indents);


#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_ANativeWindow(const ANativeWindow* object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_AHardwareBuffer(const AHardwareBuffer* object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_ANDROID_KHR


#if defined(VK_USE_PLATFORM_METAL_EXT)
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_CAMetalLayer(CAMetalLayer object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_METAL_EXT

//======================= System Type Implementations =======================//

#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_DWORD(const DWORD object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_Display(const Display* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_GGP)
void dump_html_GgpFrameToken(const GgpFrameToken object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_GGP)
void dump_html_GgpStreamDescriptor(const GgpStreamDescriptor object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_HANDLE(const HANDLE object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_HINSTANCE(const HINSTANCE object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_HMONITOR(const HMONITOR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_HWND(const HWND object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_html_IDirectFB(const IDirectFB object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_html_IDirectFBSurface(const IDirectFBSurface object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_LPCWSTR(const LPCWSTR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_SECURITY_ATTRIBUTES(const SECURITY_ATTRIBUTES* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_VisualID(const VisualID object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_Window(const Window object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html__screen_buffer(const _screen_buffer* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html__screen_context(const _screen_context* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html__screen_window(const _screen_window* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_html_wl_display(const wl_display* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_html_wl_surface(const wl_surface* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_xcb_connection_t(const xcb_connection_t* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_xcb_visualid_t(const xcb_visualid_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_xcb_window_t(const xcb_window_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_zx_handle_t(const zx_handle_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA

//========================== Handle Implementations =========================//

void dump_html_VkBuffer(const VkBuffer object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImage(const VkImage object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInstance(const VkInstance object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevice(const VkPhysicalDevice object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDevice(const VkDevice object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueue(const VkQueue object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphore(const VkSemaphore object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBuffer(const VkCommandBuffer object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFence(const VkFence object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceMemory(const VkDeviceMemory object, const ApiDumpSettings& settings, int indents);
void dump_html_VkEvent(const VkEvent object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPool(const VkQueryPool object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferView(const VkBufferView object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageView(const VkImageView object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderModule(const VkShaderModule object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCache(const VkPipelineCache object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineLayout(const VkPipelineLayout object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipeline(const VkPipeline object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPass(const VkRenderPass object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayout(const VkDescriptorSetLayout object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSampler(const VkSampler object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSet(const VkDescriptorSet object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPool(const VkDescriptorPool object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebuffer(const VkFramebuffer object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPool(const VkCommandPool object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrConversion(const VkSamplerYcbcrConversion object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorUpdateTemplate(const VkDescriptorUpdateTemplate object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPrivateDataSlot(const VkPrivateDataSlot object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceKHR(const VkSurfaceKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainKHR(const VkSwapchainKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayKHR(const VkDisplayKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModeKHR(const VkDisplayModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugReportCallbackEXT(const VkDebugReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionKHR(const VkVideoSessionKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionParametersKHR(const VkVideoSessionParametersKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCuModuleNVX(const VkCuModuleNVX object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCuFunctionNVX(const VkCuFunctionNVX object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessengerEXT(const VkDebugUtilsMessengerEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureKHR(const VkAccelerationStructureKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeferredOperationKHR(const VkDeferredOperationKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationCacheEXT(const VkValidationCacheEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureNV(const VkAccelerationStructureNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceConfigurationINTEL(const VkPerformanceConfigurationINTEL object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkCudaModuleNV(const VkCudaModuleNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkCudaFunctionNV(const VkCudaFunctionNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferCollectionFUCHSIA(const VkBufferCollectionFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_html_VkMicromapEXT(const VkMicromapEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowSessionNV(const VkOpticalFlowSessionNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderEXT(const VkShaderEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryKHR(const VkPipelineBinaryKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalComputeQueueNV(const VkExternalComputeQueueNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetEXT(const VkIndirectExecutionSetEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutEXT(const VkIndirectCommandsLayoutEXT object, const ApiDumpSettings& settings, int indents);

//=========================== Enum Implementations ==========================//

void dump_html_VkResult(VkResult object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStructureType(VkStructureType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCacheHeaderVersion(VkPipelineCacheHeaderVersion object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageLayout(VkImageLayout object, const ApiDumpSettings& settings, int indents);
void dump_html_VkObjectType(VkObjectType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVendorId(VkVendorId object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSystemAllocationScope(VkSystemAllocationScope object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInternalAllocationType(VkInternalAllocationType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormat(VkFormat object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageTiling(VkImageTiling object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageType(VkImageType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceType(VkPhysicalDeviceType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryType(VkQueryType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSharingMode(VkSharingMode object, const ApiDumpSettings& settings, int indents);
void dump_html_VkComponentSwizzle(VkComponentSwizzle object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewType(VkImageViewType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBlendFactor(VkBlendFactor object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBlendOp(VkBlendOp object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCompareOp(VkCompareOp object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDynamicState(VkDynamicState object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFrontFace(VkFrontFace object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVertexInputRate(VkVertexInputRate object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPrimitiveTopology(VkPrimitiveTopology object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPolygonMode(VkPolygonMode object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStencilOp(VkStencilOp object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLogicOp(VkLogicOp object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBorderColor(VkBorderColor object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFilter(VkFilter object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerAddressMode(VkSamplerAddressMode object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerMipmapMode(VkSamplerMipmapMode object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorType(VkDescriptorType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentLoadOp(VkAttachmentLoadOp object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentStoreOp(VkAttachmentStoreOp object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBindPoint(VkPipelineBindPoint object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferLevel(VkCommandBufferLevel object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndexType(VkIndexType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassContents(VkSubpassContents object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPointClippingBehavior(VkPointClippingBehavior object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTessellationDomainOrigin(VkTessellationDomainOrigin object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrModelConversion(VkSamplerYcbcrModelConversion object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrRange(VkSamplerYcbcrRange object, const ApiDumpSettings& settings, int indents);
void dump_html_VkChromaLocation(VkChromaLocation object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorUpdateTemplateType(VkDescriptorUpdateTemplateType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDriverId(VkDriverId object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderFloatControlsIndependence(VkShaderFloatControlsIndependence object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerReductionMode(VkSamplerReductionMode object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreType(VkSemaphoreType object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRobustnessBufferBehavior(VkPipelineRobustnessBufferBehavior object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRobustnessImageBehavior(VkPipelineRobustnessImageBehavior object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueGlobalPriority(VkQueueGlobalPriority object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLineRasterizationMode(VkLineRasterizationMode object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentModeKHR(VkPresentModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkColorSpaceKHR(VkColorSpaceKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugReportObjectTypeEXT(VkDebugReportObjectTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRasterizationOrderAMD(VkRasterizationOrderAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryResultStatusKHR(VkQueryResultStatusKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderInfoTypeAMD(VkShaderInfoTypeAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationCheckEXT(VkValidationCheckEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPowerStateEXT(VkDisplayPowerStateEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceEventTypeEXT(VkDeviceEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayEventTypeEXT(VkDisplayEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkViewportCoordinateSwizzleNV(VkViewportCoordinateSwizzleNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDiscardRectangleModeEXT(VkDiscardRectangleModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkConservativeRasterizationModeEXT(VkConservativeRasterizationModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterUnitKHR(VkPerformanceCounterUnitKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterScopeKHR(VkPerformanceCounterScopeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterStorageKHR(VkPerformanceCounterStorageKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBlendOverlapEXT(VkBlendOverlapEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureTypeKHR(VkAccelerationStructureTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildAccelerationStructureModeKHR(VkBuildAccelerationStructureModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryTypeKHR(VkGeometryTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureBuildTypeKHR(VkAccelerationStructureBuildTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyAccelerationStructureModeKHR(VkCopyAccelerationStructureModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureCompatibilityKHR(VkAccelerationStructureCompatibilityKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCoverageModulationModeNV(VkCoverageModulationModeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationCacheHeaderVersionEXT(VkValidationCacheHeaderVersionEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShadingRatePaletteEntryNV(VkShadingRatePaletteEntryNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCoarseSampleOrderTypeNV(VkCoarseSampleOrderTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingShaderGroupTypeKHR(VkRayTracingShaderGroupTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMemoryRequirementsTypeNV(VkAccelerationStructureMemoryRequirementsTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTimeDomainKHR(VkTimeDomainKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryOverallocationBehaviorAMD(VkMemoryOverallocationBehaviorAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceConfigurationTypeINTEL(VkPerformanceConfigurationTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPoolSamplingModeINTEL(VkQueryPoolSamplingModeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceOverrideTypeINTEL(VkPerformanceOverrideTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceParameterTypeINTEL(VkPerformanceParameterTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceValueTypeINTEL(VkPerformanceValueTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFragmentShadingRateCombinerOpKHR(VkFragmentShadingRateCombinerOpKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationFeatureEnableEXT(VkValidationFeatureEnableEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationFeatureDisableEXT(VkValidationFeatureDisableEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkComponentTypeKHR(VkComponentTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkScopeKHR(VkScopeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCoverageReductionModeNV(VkCoverageReductionModeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkProvokingVertexModeEXT(VkProvokingVertexModeEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkFullScreenExclusiveEXT(VkFullScreenExclusiveEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkPipelineExecutableStatisticFormatKHR(VkPipelineExecutableStatisticFormatKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsTokenTypeNV(VkIndirectCommandsTokenTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDepthBiasRepresentationEXT(VkDepthBiasRepresentationEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceMemoryReportEventTypeEXT(VkDeviceMemoryReportEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeTuningModeKHR(VkVideoEncodeTuningModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFragmentShadingRateTypeNV(VkFragmentShadingRateTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFragmentShadingRateNV(VkFragmentShadingRateNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInstanceTypeNV(VkAccelerationStructureMotionInstanceTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultAddressTypeEXT(VkDeviceFaultAddressTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultVendorBinaryHeaderVersionEXT(VkDeviceFaultVendorBinaryHeaderVersionEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderGroupShaderKHR(VkShaderGroupShaderKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceAddressBindingTypeEXT(VkDeviceAddressBindingTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapTypeEXT(VkMicromapTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildMicromapModeEXT(VkBuildMicromapModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMicromapModeEXT(VkCopyMicromapModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpacityMicromapFormatEXT(VkOpacityMicromapFormatEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpacityMicromapSpecialIndexEXT(VkOpacityMicromapSpecialIndexEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkDisplacementMicromapFormatNV(VkDisplacementMicromapFormatNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkRayTracingLssIndexingModeNV(VkRayTracingLssIndexingModeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingLssPrimitiveEndCapsModeNV(VkRayTracingLssPrimitiveEndCapsModeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassMergeStatusEXT(VkSubpassMergeStatusEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDirectDriverLoadingModeLUNARG(VkDirectDriverLoadingModeLUNARG object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowPerformanceLevelNV(VkOpticalFlowPerformanceLevelNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowSessionBindingPointNV(VkOpticalFlowSessionBindingPointNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAntiLagModeAMD(VkAntiLagModeAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAntiLagStageAMD(VkAntiLagStageAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderCodeTypeEXT(VkShaderCodeTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDepthClampModeEXT(VkDepthClampModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingInvocationReorderModeNV(VkRayTracingInvocationReorderModeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCooperativeVectorMatrixLayoutNV(VkCooperativeVectorMatrixLayoutNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLayerSettingTypeEXT(VkLayerSettingTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLatencyMarkerNV(VkLatencyMarkerNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOutOfBandQueueTypeNV(VkOutOfBandQueueTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1PredictionModeKHR(VkVideoEncodeAV1PredictionModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1RateControlGroupKHR(VkVideoEncodeAV1RateControlGroupKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBlockMatchWindowCompareModeQCOM(VkBlockMatchWindowCompareModeQCOM object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCubicFilterWeightsQCOM(VkCubicFilterWeightsQCOM object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLayeredDriverUnderlyingApiMSFT(VkLayeredDriverUnderlyingApiMSFT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplaySurfaceStereoTypeNV(VkDisplaySurfaceStereoTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLayeredApiKHR(VkPhysicalDeviceLayeredApiKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureTypeNV(VkClusterAccelerationStructureTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureOpTypeNV(VkClusterAccelerationStructureOpTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureOpModeNV(VkClusterAccelerationStructureOpModeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureOpTypeNV(VkPartitionedAccelerationStructureOpTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetInfoTypeEXT(VkIndirectExecutionSetInfoTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsTokenTypeEXT(VkIndirectCommandsTokenTypeEXT object, const ApiDumpSettings& settings, int indents);

//========================= Bitmask Implementations =========================//

void dump_html_VkAccessFlagBits(VkAccessFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageAspectFlagBits(VkImageAspectFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatFeatureFlagBits(VkFormatFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCreateFlagBits(VkImageCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSampleCountFlagBits(VkSampleCountFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageUsageFlagBits(VkImageUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInstanceCreateFlagBits(VkInstanceCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryHeapFlagBits(VkMemoryHeapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryPropertyFlagBits(VkMemoryPropertyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFlagBits(VkQueueFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceQueueCreateFlagBits(VkDeviceQueueCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineStageFlagBits(VkPipelineStageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryMapFlagBits(VkMemoryMapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseMemoryBindFlagBits(VkSparseMemoryBindFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageFormatFlagBits(VkSparseImageFormatFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFenceCreateFlagBits(VkFenceCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkEventCreateFlagBits(VkEventCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPipelineStatisticFlagBits(VkQueryPipelineStatisticFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryResultFlagBits(VkQueryResultFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferCreateFlagBits(VkBufferCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferUsageFlagBits(VkBufferUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewCreateFlagBits(VkImageViewCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCacheCreateFlagBits(VkPipelineCacheCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkColorComponentFlagBits(VkColorComponentFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreateFlagBits(VkPipelineCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineShaderStageCreateFlagBits(VkPipelineShaderStageCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderStageFlagBits(VkShaderStageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCullModeFlagBits(VkCullModeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDepthStencilStateCreateFlagBits(VkPipelineDepthStencilStateCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineColorBlendStateCreateFlagBits(VkPipelineColorBlendStateCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineLayoutCreateFlagBits(VkPipelineLayoutCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerCreateFlagBits(VkSamplerCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPoolCreateFlagBits(VkDescriptorPoolCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutCreateFlagBits(VkDescriptorSetLayoutCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentDescriptionFlagBits(VkAttachmentDescriptionFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDependencyFlagBits(VkDependencyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebufferCreateFlagBits(VkFramebufferCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreateFlagBits(VkRenderPassCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDescriptionFlagBits(VkSubpassDescriptionFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPoolCreateFlagBits(VkCommandPoolCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPoolResetFlagBits(VkCommandPoolResetFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferUsageFlagBits(VkCommandBufferUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryControlFlagBits(VkQueryControlFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferResetFlagBits(VkCommandBufferResetFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStencilFaceFlagBits(VkStencilFaceFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubgroupFeatureFlagBits(VkSubgroupFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPeerMemoryFeatureFlagBits(VkPeerMemoryFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryAllocateFlagBits(VkMemoryAllocateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryHandleTypeFlagBits(VkExternalMemoryHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryFeatureFlagBits(VkExternalMemoryFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalFenceHandleTypeFlagBits(VkExternalFenceHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalFenceFeatureFlagBits(VkExternalFenceFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFenceImportFlagBits(VkFenceImportFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreImportFlagBits(VkSemaphoreImportFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalSemaphoreHandleTypeFlagBits(VkExternalSemaphoreHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalSemaphoreFeatureFlagBits(VkExternalSemaphoreFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkResolveModeFlagBits(VkResolveModeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorBindingFlagBits(VkDescriptorBindingFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreWaitFlagBits(VkSemaphoreWaitFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreationFeedbackFlagBits(VkPipelineCreationFeedbackFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkToolPurposeFlagBits(VkToolPurposeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineStageFlagBits2(VkPipelineStageFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccessFlagBits2(VkAccessFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubmitFlagBits(VkSubmitFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingFlagBits(VkRenderingFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatFeatureFlagBits2(VkFormatFeatureFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryUnmapFlagBits(VkMemoryUnmapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreateFlagBits2(VkPipelineCreateFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferUsageFlagBits2(VkBufferUsageFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHostImageCopyFlagBits(VkHostImageCopyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceTransformFlagBitsKHR(VkSurfaceTransformFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCompositeAlphaFlagBitsKHR(VkCompositeAlphaFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainCreateFlagBitsKHR(VkSwapchainCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupPresentModeFlagBitsKHR(VkDeviceGroupPresentModeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlaneAlphaFlagBitsKHR(VkDisplayPlaneAlphaFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugReportFlagBitsEXT(VkDebugReportFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCodecOperationFlagBitsKHR(VkVideoCodecOperationFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoChromaSubsamplingFlagBitsKHR(VkVideoChromaSubsamplingFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoComponentBitDepthFlagBitsKHR(VkVideoComponentBitDepthFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCapabilityFlagBitsKHR(VkVideoCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionCreateFlagBitsKHR(VkVideoSessionCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionParametersCreateFlagBitsKHR(VkVideoSessionParametersCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCodingControlFlagBitsKHR(VkVideoCodingControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeCapabilityFlagBitsKHR(VkVideoDecodeCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeUsageFlagBitsKHR(VkVideoDecodeUsageFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264CapabilityFlagBitsKHR(VkVideoEncodeH264CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264StdFlagBitsKHR(VkVideoEncodeH264StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264RateControlFlagBitsKHR(VkVideoEncodeH264RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265CapabilityFlagBitsKHR(VkVideoEncodeH265CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265StdFlagBitsKHR(VkVideoEncodeH265StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265CtbSizeFlagBitsKHR(VkVideoEncodeH265CtbSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265TransformBlockSizeFlagBitsKHR(VkVideoEncodeH265TransformBlockSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265RateControlFlagBitsKHR(VkVideoEncodeH265RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264PictureLayoutFlagBitsKHR(VkVideoDecodeH264PictureLayoutFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryHandleTypeFlagBitsNV(VkExternalMemoryHandleTypeFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryFeatureFlagBitsNV(VkExternalMemoryFeatureFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkConditionalRenderingFlagBitsEXT(VkConditionalRenderingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceCounterFlagBitsEXT(VkSurfaceCounterFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterDescriptionFlagBitsKHR(VkPerformanceCounterDescriptionFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAcquireProfilingLockFlagBitsKHR(VkAcquireProfilingLockFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessageSeverityFlagBitsEXT(VkDebugUtilsMessageSeverityFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessageTypeFlagBitsEXT(VkDebugUtilsMessageTypeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildAccelerationStructureFlagBitsKHR(VkBuildAccelerationStructureFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryFlagBitsKHR(VkGeometryFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryInstanceFlagBitsKHR(VkGeometryInstanceFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureCreateFlagBitsKHR(VkAccelerationStructureCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCompilerControlFlagBitsAMD(VkPipelineCompilerControlFlagBitsAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderCorePropertiesFlagBitsAMD(VkShaderCorePropertiesFlagBitsAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentScalingFlagBitsEXT(VkPresentScalingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentGravityFlagBitsEXT(VkPresentGravityFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectStateFlagBitsNV(VkIndirectStateFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutUsageFlagBitsNV(VkIndirectCommandsLayoutUsageFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeFlagBitsKHR(VkVideoEncodeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeCapabilityFlagBitsKHR(VkVideoEncodeCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeRateControlModeFlagBitsKHR(VkVideoEncodeRateControlModeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeFeedbackFlagBitsKHR(VkVideoEncodeFeedbackFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeUsageFlagBitsKHR(VkVideoEncodeUsageFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeContentFlagBitsKHR(VkVideoEncodeContentFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceDiagnosticsConfigFlagBitsNV(VkDeviceDiagnosticsConfigFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTileShadingRenderPassFlagBitsQCOM(VkTileShadingRenderPassFlagBitsQCOM object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalObjectTypeFlagBitsEXT(VkExportMetalObjectTypeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkGraphicsPipelineLibraryFlagBitsEXT(VkGraphicsPipelineLibraryFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCompressionFlagBitsEXT(VkImageCompressionFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCompressionFixedRateFlagBitsEXT(VkImageCompressionFixedRateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceAddressBindingFlagBitsEXT(VkDeviceAddressBindingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImageConstraintsInfoFlagBitsFUCHSIA(VkImageConstraintsInfoFlagBitsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_html_VkFrameBoundaryFlagBitsEXT(VkFrameBoundaryFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildMicromapFlagBitsEXT(VkBuildMicromapFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapCreateFlagBitsEXT(VkMicromapCreateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSchedulingControlsFlagBitsARM(VkPhysicalDeviceSchedulingControlsFlagBitsARM object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryDecompressionMethodFlagBitsNV(VkMemoryDecompressionMethodFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowGridSizeFlagBitsNV(VkOpticalFlowGridSizeFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowUsageFlagBitsNV(VkOpticalFlowUsageFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowSessionCreateFlagBitsNV(VkOpticalFlowSessionCreateFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowExecuteFlagBitsNV(VkOpticalFlowExecuteFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderCreateFlagBitsEXT(VkShaderCreateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1CapabilityFlagBitsKHR(VkVideoEncodeAV1CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1StdFlagBitsKHR(VkVideoEncodeAV1StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1SuperblockSizeFlagBitsKHR(VkVideoEncodeAV1SuperblockSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1RateControlFlagBitsKHR(VkVideoEncodeAV1RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureAddressResolutionFlagBitsNV(VkClusterAccelerationStructureAddressResolutionFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureClusterFlagBitsNV(VkClusterAccelerationStructureClusterFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureGeometryFlagBitsNV(VkClusterAccelerationStructureGeometryFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureIndexFormatFlagBitsNV(VkClusterAccelerationStructureIndexFormatFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureInstanceFlagBitsNV(VkPartitionedAccelerationStructureInstanceFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsInputModeFlagBitsEXT(VkIndirectCommandsInputModeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutUsageFlagBitsEXT(VkIndirectCommandsLayoutUsageFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccessFlagBits3KHR(VkAccessFlagBits3KHR object, const ApiDumpSettings& settings, int indents);

//=========================== Flag Implementations ==========================//

void dump_html_VkAccessFlags(VkAccessFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageAspectFlags(VkImageAspectFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatFeatureFlags(VkFormatFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCreateFlags(VkImageCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSampleCountFlags(VkSampleCountFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageUsageFlags(VkImageUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInstanceCreateFlags(VkInstanceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryHeapFlags(VkMemoryHeapFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryPropertyFlags(VkMemoryPropertyFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFlags(VkQueueFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceQueueCreateFlags(VkDeviceQueueCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineStageFlags(VkPipelineStageFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryMapFlags(VkMemoryMapFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseMemoryBindFlags(VkSparseMemoryBindFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageFormatFlags(VkSparseImageFormatFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFenceCreateFlags(VkFenceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkEventCreateFlags(VkEventCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPipelineStatisticFlags(VkQueryPipelineStatisticFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryResultFlags(VkQueryResultFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferCreateFlags(VkBufferCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferUsageFlags(VkBufferUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewCreateFlags(VkImageViewCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCacheCreateFlags(VkPipelineCacheCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkColorComponentFlags(VkColorComponentFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreateFlags(VkPipelineCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineShaderStageCreateFlags(VkPipelineShaderStageCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCullModeFlags(VkCullModeFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDepthStencilStateCreateFlags(VkPipelineDepthStencilStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineColorBlendStateCreateFlags(VkPipelineColorBlendStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineLayoutCreateFlags(VkPipelineLayoutCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderStageFlags(VkShaderStageFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerCreateFlags(VkSamplerCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPoolCreateFlags(VkDescriptorPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutCreateFlags(VkDescriptorSetLayoutCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentDescriptionFlags(VkAttachmentDescriptionFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDependencyFlags(VkDependencyFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebufferCreateFlags(VkFramebufferCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreateFlags(VkRenderPassCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDescriptionFlags(VkSubpassDescriptionFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPoolCreateFlags(VkCommandPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPoolResetFlags(VkCommandPoolResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferUsageFlags(VkCommandBufferUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryControlFlags(VkQueryControlFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferResetFlags(VkCommandBufferResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStencilFaceFlags(VkStencilFaceFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubgroupFeatureFlags(VkSubgroupFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPeerMemoryFeatureFlags(VkPeerMemoryFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryAllocateFlags(VkMemoryAllocateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryHandleTypeFlags(VkExternalMemoryHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryFeatureFlags(VkExternalMemoryFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalFenceHandleTypeFlags(VkExternalFenceHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalFenceFeatureFlags(VkExternalFenceFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFenceImportFlags(VkFenceImportFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreImportFlags(VkSemaphoreImportFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalSemaphoreHandleTypeFlags(VkExternalSemaphoreHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalSemaphoreFeatureFlags(VkExternalSemaphoreFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkResolveModeFlags(VkResolveModeFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorBindingFlags(VkDescriptorBindingFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreWaitFlags(VkSemaphoreWaitFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreationFeedbackFlags(VkPipelineCreationFeedbackFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkToolPurposeFlags(VkToolPurposeFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineStageFlags2(VkPipelineStageFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccessFlags2(VkAccessFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubmitFlags(VkSubmitFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingFlags(VkRenderingFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatFeatureFlags2(VkFormatFeatureFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryUnmapFlags(VkMemoryUnmapFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreateFlags2(VkPipelineCreateFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferUsageFlags2(VkBufferUsageFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHostImageCopyFlags(VkHostImageCopyFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCompositeAlphaFlagsKHR(VkCompositeAlphaFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceTransformFlagsKHR(VkSurfaceTransformFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainCreateFlagsKHR(VkSwapchainCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupPresentModeFlagsKHR(VkDeviceGroupPresentModeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlaneAlphaFlagsKHR(VkDisplayPlaneAlphaFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugReportFlagsEXT(VkDebugReportFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCodecOperationFlagsKHR(VkVideoCodecOperationFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoChromaSubsamplingFlagsKHR(VkVideoChromaSubsamplingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoComponentBitDepthFlagsKHR(VkVideoComponentBitDepthFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCapabilityFlagsKHR(VkVideoCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionCreateFlagsKHR(VkVideoSessionCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionParametersCreateFlagsKHR(VkVideoSessionParametersCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCodingControlFlagsKHR(VkVideoCodingControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeCapabilityFlagsKHR(VkVideoDecodeCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeUsageFlagsKHR(VkVideoDecodeUsageFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264CapabilityFlagsKHR(VkVideoEncodeH264CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264StdFlagsKHR(VkVideoEncodeH264StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264RateControlFlagsKHR(VkVideoEncodeH264RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265CapabilityFlagsKHR(VkVideoEncodeH265CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265StdFlagsKHR(VkVideoEncodeH265StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265CtbSizeFlagsKHR(VkVideoEncodeH265CtbSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265TransformBlockSizeFlagsKHR(VkVideoEncodeH265TransformBlockSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265RateControlFlagsKHR(VkVideoEncodeH265RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264PictureLayoutFlagsKHR(VkVideoDecodeH264PictureLayoutFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryHandleTypeFlagsNV(VkExternalMemoryHandleTypeFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryFeatureFlagsNV(VkExternalMemoryFeatureFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkConditionalRenderingFlagsEXT(VkConditionalRenderingFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceCounterFlagsEXT(VkSurfaceCounterFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterDescriptionFlagsKHR(VkPerformanceCounterDescriptionFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAcquireProfilingLockFlagsKHR(VkAcquireProfilingLockFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessageTypeFlagsEXT(VkDebugUtilsMessageTypeFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessageSeverityFlagsEXT(VkDebugUtilsMessageSeverityFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildAccelerationStructureFlagsKHR(VkBuildAccelerationStructureFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryFlagsKHR(VkGeometryFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryInstanceFlagsKHR(VkGeometryInstanceFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureCreateFlagsKHR(VkAccelerationStructureCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCompilerControlFlagsAMD(VkPipelineCompilerControlFlagsAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderCorePropertiesFlagsAMD(VkShaderCorePropertiesFlagsAMD object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentScalingFlagsEXT(VkPresentScalingFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentGravityFlagsEXT(VkPresentGravityFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectStateFlagsNV(VkIndirectStateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutUsageFlagsNV(VkIndirectCommandsLayoutUsageFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeFlagsKHR(VkVideoEncodeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeCapabilityFlagsKHR(VkVideoEncodeCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeRateControlModeFlagsKHR(VkVideoEncodeRateControlModeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeFeedbackFlagsKHR(VkVideoEncodeFeedbackFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeUsageFlagsKHR(VkVideoEncodeUsageFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeContentFlagsKHR(VkVideoEncodeContentFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceDiagnosticsConfigFlagsNV(VkDeviceDiagnosticsConfigFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTileShadingRenderPassFlagsQCOM(VkTileShadingRenderPassFlagsQCOM object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalObjectTypeFlagsEXT(VkExportMetalObjectTypeFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkGraphicsPipelineLibraryFlagsEXT(VkGraphicsPipelineLibraryFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCompressionFlagsEXT(VkImageCompressionFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCompressionFixedRateFlagsEXT(VkImageCompressionFixedRateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceAddressBindingFlagsEXT(VkDeviceAddressBindingFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImageConstraintsInfoFlagsFUCHSIA(VkImageConstraintsInfoFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_html_VkFrameBoundaryFlagsEXT(VkFrameBoundaryFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildMicromapFlagsEXT(VkBuildMicromapFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapCreateFlagsEXT(VkMicromapCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSchedulingControlsFlagsARM(VkPhysicalDeviceSchedulingControlsFlagsARM object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryDecompressionMethodFlagsNV(VkMemoryDecompressionMethodFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowGridSizeFlagsNV(VkOpticalFlowGridSizeFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowUsageFlagsNV(VkOpticalFlowUsageFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowSessionCreateFlagsNV(VkOpticalFlowSessionCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowExecuteFlagsNV(VkOpticalFlowExecuteFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderCreateFlagsEXT(VkShaderCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1CapabilityFlagsKHR(VkVideoEncodeAV1CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1StdFlagsKHR(VkVideoEncodeAV1StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1SuperblockSizeFlagsKHR(VkVideoEncodeAV1SuperblockSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1RateControlFlagsKHR(VkVideoEncodeAV1RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureAddressResolutionFlagsNV(VkClusterAccelerationStructureAddressResolutionFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureClusterFlagsNV(VkClusterAccelerationStructureClusterFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureGeometryFlagsNV(VkClusterAccelerationStructureGeometryFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureIndexFormatFlagsNV(VkClusterAccelerationStructureIndexFormatFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureInstanceFlagsNV(VkPartitionedAccelerationStructureInstanceFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsInputModeFlagsEXT(VkIndirectCommandsInputModeFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutUsageFlagsEXT(VkIndirectCommandsLayoutUsageFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccessFlags3KHR(VkAccessFlags3KHR object, const ApiDumpSettings& settings, int indents);


void dump_html_VkDeviceCreateFlags(VkDeviceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreCreateFlags(VkSemaphoreCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPoolCreateFlags(VkQueryPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferViewCreateFlags(VkBufferViewCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderModuleCreateFlags(VkShaderModuleCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineVertexInputStateCreateFlags(VkPipelineVertexInputStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineInputAssemblyStateCreateFlags(VkPipelineInputAssemblyStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineTessellationStateCreateFlags(VkPipelineTessellationStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportStateCreateFlags(VkPipelineViewportStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationStateCreateFlags(VkPipelineRasterizationStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineMultisampleStateCreateFlags(VkPipelineMultisampleStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDynamicStateCreateFlags(VkPipelineDynamicStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPoolResetFlags(VkDescriptorPoolResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPoolTrimFlags(VkCommandPoolTrimFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorUpdateTemplateCreateFlags(VkDescriptorUpdateTemplateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPrivateDataSlotCreateFlags(VkPrivateDataSlotCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModeCreateFlagsKHR(VkDisplayModeCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplaySurfaceCreateFlagsKHR(VkDisplaySurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_VkXlibSurfaceCreateFlagsKHR(VkXlibSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_VkXcbSurfaceCreateFlagsKHR(VkXcbSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_html_VkWaylandSurfaceCreateFlagsKHR(VkWaylandSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidSurfaceCreateFlagsKHR(VkAndroidSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkWin32SurfaceCreateFlagsKHR(VkWin32SurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkVideoBeginCodingFlagsKHR(VkVideoBeginCodingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEndCodingFlagsKHR(VkVideoEndCodingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeFlagsKHR(VkVideoDecodeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationStateStreamCreateFlagsEXT(VkPipelineRasterizationStateStreamCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_html_VkStreamDescriptorSurfaceCreateFlagsGGP(VkStreamDescriptorSurfaceCreateFlagsGGP object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_html_VkViSurfaceCreateFlagsNN(VkViSurfaceCreateFlagsNN object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_VI_NN
void dump_html_VkPipelineViewportSwizzleStateCreateFlagsNV(VkPipelineViewportSwizzleStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDiscardRectangleStateCreateFlagsEXT(VkPipelineDiscardRectangleStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationConservativeStateCreateFlagsEXT(VkPipelineRasterizationConservativeStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationDepthClipStateCreateFlagsEXT(VkPipelineRasterizationDepthClipStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_html_VkIOSSurfaceCreateFlagsMVK(VkIOSSurfaceCreateFlagsMVK object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_html_VkMacOSSurfaceCreateFlagsMVK(VkMacOSSurfaceCreateFlagsMVK object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_html_VkDebugUtilsMessengerCallbackDataFlagsEXT(VkDebugUtilsMessengerCallbackDataFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessengerCreateFlagsEXT(VkDebugUtilsMessengerCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCoverageToColorStateCreateFlagsNV(VkPipelineCoverageToColorStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCoverageModulationStateCreateFlagsNV(VkPipelineCoverageModulationStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationCacheCreateFlagsEXT(VkValidationCacheCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImagePipeSurfaceCreateFlagsFUCHSIA(VkImagePipeSurfaceCreateFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkMetalSurfaceCreateFlagsEXT(VkMetalSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkPipelineCoverageReductionStateCreateFlagsNV(VkPipelineCoverageReductionStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHeadlessSurfaceCreateFlagsEXT(VkHeadlessSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceMemoryReportFlagsEXT(VkDeviceMemoryReportFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeRateControlFlagsKHR(VkVideoEncodeRateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInfoFlagsNV(VkAccelerationStructureMotionInfoFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInstanceFlagsNV(VkAccelerationStructureMotionInstanceFlagsNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_html_VkDirectFBSurfaceCreateFlagsEXT(VkDirectFBSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImageFormatConstraintsFlagsFUCHSIA(VkImageFormatConstraintsFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkScreenSurfaceCreateFlagsQNX(VkScreenSurfaceCreateFlagsQNX object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_html_VkDirectDriverLoadingFlagsLUNARG(VkDirectDriverLoadingFlagsLUNARG object, const ApiDumpSettings& settings, int indents);

//======================= Func Pointer Implementations ======================//

void dump_html_PFN_vkAllocationFunction(PFN_vkAllocationFunction object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkFreeFunction(PFN_vkFreeFunction object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkInternalAllocationNotification(PFN_vkInternalAllocationNotification object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkInternalFreeNotification(PFN_vkInternalFreeNotification object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkReallocationFunction(PFN_vkReallocationFunction object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkVoidFunction(PFN_vkVoidFunction object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkDebugReportCallbackEXT(PFN_vkDebugReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkDebugUtilsMessengerCallbackEXT(PFN_vkDebugUtilsMessengerCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkDeviceMemoryReportCallbackEXT(PFN_vkDeviceMemoryReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_html_PFN_vkGetInstanceProcAddrLUNARG(PFN_vkGetInstanceProcAddrLUNARG object, const ApiDumpSettings& settings, int indents);

//========================== Struct Implementations =========================//

void dump_html_VkExtent2D(const VkExtent2D& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExtent3D(const VkExtent3D& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOffset2D(const VkOffset2D& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOffset3D(const VkOffset3D& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRect2D(const VkRect2D& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBaseInStructure(const VkBaseInStructure& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBaseOutStructure(const VkBaseOutStructure& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferMemoryBarrier(const VkBufferMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDispatchIndirectCommand(const VkDispatchIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrawIndexedIndirectCommand(const VkDrawIndexedIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrawIndirectCommand(const VkDrawIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageSubresourceRange(const VkImageSubresourceRange& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageMemoryBarrier(const VkImageMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryBarrier(const VkMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCacheHeaderVersionOne(const VkPipelineCacheHeaderVersionOne& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAllocationCallbacks(const VkAllocationCallbacks& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkApplicationInfo(const VkApplicationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatProperties(const VkFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageFormatProperties(const VkImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInstanceCreateInfo(const VkInstanceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryHeap(const VkMemoryHeap& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryType(const VkMemoryType& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLimits(const VkPhysicalDeviceLimits& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSparseProperties(const VkPhysicalDeviceSparseProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceProperties(const VkPhysicalDeviceProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyProperties(const VkQueueFamilyProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceQueueCreateInfo(const VkDeviceQueueCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceCreateInfo(const VkDeviceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExtensionProperties(const VkExtensionProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLayerProperties(const VkLayerProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubmitInfo(const VkSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMappedMemoryRange(const VkMappedMemoryRange& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryAllocateInfo(const VkMemoryAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryRequirements(const VkMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseMemoryBind(const VkSparseMemoryBind& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseBufferMemoryBindInfo(const VkSparseBufferMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageOpaqueMemoryBindInfo(const VkSparseImageOpaqueMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageSubresource(const VkImageSubresource& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageMemoryBind(const VkSparseImageMemoryBind& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageMemoryBindInfo(const VkSparseImageMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindSparseInfo(const VkBindSparseInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageFormatProperties(const VkSparseImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageMemoryRequirements(const VkSparseImageMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFenceCreateInfo(const VkFenceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreCreateInfo(const VkSemaphoreCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkEventCreateInfo(const VkEventCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPoolCreateInfo(const VkQueryPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferCreateInfo(const VkBufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferViewCreateInfo(const VkBufferViewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCreateInfo(const VkImageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubresourceLayout(const VkSubresourceLayout& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkComponentMapping(const VkComponentMapping& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewCreateInfo(const VkImageViewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderModuleCreateInfo(const VkShaderModuleCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCacheCreateInfo(const VkPipelineCacheCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSpecializationMapEntry(const VkSpecializationMapEntry& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSpecializationInfo(const VkSpecializationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkComputePipelineCreateInfo(const VkComputePipelineCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVertexInputBindingDescription(const VkVertexInputBindingDescription& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVertexInputAttributeDescription(const VkVertexInputAttributeDescription& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineTessellationStateCreateInfo(const VkPipelineTessellationStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkViewport(const VkViewport& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStencilOpState(const VkStencilOpState& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPushConstantRange(const VkPushConstantRange& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerCreateInfo(const VkSamplerCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyDescriptorSet(const VkCopyDescriptorSet& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorBufferInfo(const VkDescriptorBufferInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorImageInfo(const VkDescriptorImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPoolSize(const VkDescriptorPoolSize& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPoolCreateInfo(const VkDescriptorPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetAllocateInfo(const VkDescriptorSetAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteDescriptorSet(const VkWriteDescriptorSet& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentDescription(const VkAttachmentDescription& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentReference(const VkAttachmentReference& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebufferCreateInfo(const VkFramebufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDescription(const VkSubpassDescription& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDependency(const VkSubpassDependency& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreateInfo(const VkRenderPassCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandPoolCreateInfo(const VkCommandPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferAllocateInfo(const VkCommandBufferAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferInheritanceInfo(const VkCommandBufferInheritanceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferBeginInfo(const VkCommandBufferBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferCopy(const VkBufferCopy& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageSubresourceLayers(const VkImageSubresourceLayers& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferImageCopy(const VkBufferImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClearDepthStencilValue(const VkClearDepthStencilValue& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClearAttachment(const VkClearAttachment& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClearRect(const VkClearRect& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageBlit(const VkImageBlit& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCopy(const VkImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageResolve(const VkImageResolve& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassBeginInfo(const VkRenderPassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSubgroupProperties(const VkPhysicalDeviceSubgroupProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindBufferMemoryInfo(const VkBindBufferMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindImageMemoryInfo(const VkBindImageMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevice16BitStorageFeatures(const VkPhysicalDevice16BitStorageFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryDedicatedRequirements(const VkMemoryDedicatedRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryDedicatedAllocateInfo(const VkMemoryDedicatedAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryAllocateFlagsInfo(const VkMemoryAllocateFlagsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupRenderPassBeginInfo(const VkDeviceGroupRenderPassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupCommandBufferBeginInfo(const VkDeviceGroupCommandBufferBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupSubmitInfo(const VkDeviceGroupSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupBindSparseInfo(const VkDeviceGroupBindSparseInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindBufferMemoryDeviceGroupInfo(const VkBindBufferMemoryDeviceGroupInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindImageMemoryDeviceGroupInfo(const VkBindImageMemoryDeviceGroupInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupDeviceCreateInfo(const VkDeviceGroupDeviceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferMemoryRequirementsInfo2(const VkBufferMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageMemoryRequirementsInfo2(const VkImageMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageSparseMemoryRequirementsInfo2(const VkImageSparseMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryRequirements2(const VkMemoryRequirements2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageMemoryRequirements2(const VkSparseImageMemoryRequirements2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFeatures2(const VkPhysicalDeviceFeatures2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceProperties2(const VkPhysicalDeviceProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatProperties2(const VkFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageFormatProperties2(const VkImageFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageFormatInfo2(const VkPhysicalDeviceImageFormatInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyProperties2(const VkQueueFamilyProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMemoryProperties2(const VkPhysicalDeviceMemoryProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSparseImageFormatProperties2(const VkSparseImageFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSparseImageFormatInfo2(const VkPhysicalDeviceSparseImageFormatInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePointClippingProperties(const VkPhysicalDevicePointClippingProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInputAttachmentAspectReference(const VkInputAttachmentAspectReference& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassInputAttachmentAspectCreateInfo(const VkRenderPassInputAttachmentAspectCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewUsageCreateInfo(const VkImageViewUsageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineTessellationDomainOriginStateCreateInfo(const VkPipelineTessellationDomainOriginStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassMultiviewCreateInfo(const VkRenderPassMultiviewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiviewFeatures(const VkPhysicalDeviceMultiviewFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiviewProperties(const VkPhysicalDeviceMultiviewProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVariablePointersFeatures(const VkPhysicalDeviceVariablePointersFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceProtectedMemoryFeatures(const VkPhysicalDeviceProtectedMemoryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceProtectedMemoryProperties(const VkPhysicalDeviceProtectedMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceQueueInfo2(const VkDeviceQueueInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkProtectedSubmitInfo(const VkProtectedSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrConversionCreateInfo(const VkSamplerYcbcrConversionCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrConversionInfo(const VkSamplerYcbcrConversionInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindImagePlaneMemoryInfo(const VkBindImagePlaneMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImagePlaneMemoryRequirementsInfo(const VkImagePlaneMemoryRequirementsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSamplerYcbcrConversionFeatures(const VkPhysicalDeviceSamplerYcbcrConversionFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrConversionImageFormatProperties(const VkSamplerYcbcrConversionImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorUpdateTemplateEntry(const VkDescriptorUpdateTemplateEntry& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorUpdateTemplateCreateInfo(const VkDescriptorUpdateTemplateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryProperties(const VkExternalMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalImageFormatInfo(const VkPhysicalDeviceExternalImageFormatInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalImageFormatProperties(const VkExternalImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalBufferInfo(const VkPhysicalDeviceExternalBufferInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalBufferProperties(const VkExternalBufferProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceIDProperties(const VkPhysicalDeviceIDProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryImageCreateInfo(const VkExternalMemoryImageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryBufferCreateInfo(const VkExternalMemoryBufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExportMemoryAllocateInfo(const VkExportMemoryAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalFenceInfo(const VkPhysicalDeviceExternalFenceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalFenceProperties(const VkExternalFenceProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExportFenceCreateInfo(const VkExportFenceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExportSemaphoreCreateInfo(const VkExportSemaphoreCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalSemaphoreInfo(const VkPhysicalDeviceExternalSemaphoreInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalSemaphoreProperties(const VkExternalSemaphoreProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance3Properties(const VkPhysicalDeviceMaintenance3Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutSupport(const VkDescriptorSetLayoutSupport& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderDrawParametersFeatures(const VkPhysicalDeviceShaderDrawParametersFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan11Features(const VkPhysicalDeviceVulkan11Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan11Properties(const VkPhysicalDeviceVulkan11Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan12Features(const VkPhysicalDeviceVulkan12Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkConformanceVersion(const VkConformanceVersion& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan12Properties(const VkPhysicalDeviceVulkan12Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageFormatListCreateInfo(const VkImageFormatListCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentDescription2(const VkAttachmentDescription2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentReference2(const VkAttachmentReference2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDescription2(const VkSubpassDescription2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDependency2(const VkSubpassDependency2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreateInfo2(const VkRenderPassCreateInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassBeginInfo(const VkSubpassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassEndInfo(const VkSubpassEndInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevice8BitStorageFeatures(const VkPhysicalDevice8BitStorageFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDriverProperties(const VkPhysicalDeviceDriverProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderAtomicInt64Features(const VkPhysicalDeviceShaderAtomicInt64Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderFloat16Int8Features(const VkPhysicalDeviceShaderFloat16Int8Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFloatControlsProperties(const VkPhysicalDeviceFloatControlsProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutBindingFlagsCreateInfo(const VkDescriptorSetLayoutBindingFlagsCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDescriptorIndexingFeatures(const VkPhysicalDeviceDescriptorIndexingFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDescriptorIndexingProperties(const VkPhysicalDeviceDescriptorIndexingProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetVariableDescriptorCountAllocateInfo(const VkDescriptorSetVariableDescriptorCountAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetVariableDescriptorCountLayoutSupport(const VkDescriptorSetVariableDescriptorCountLayoutSupport& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassDescriptionDepthStencilResolve(const VkSubpassDescriptionDepthStencilResolve& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDepthStencilResolveProperties(const VkPhysicalDeviceDepthStencilResolveProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceScalarBlockLayoutFeatures(const VkPhysicalDeviceScalarBlockLayoutFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageStencilUsageCreateInfo(const VkImageStencilUsageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerReductionModeCreateInfo(const VkSamplerReductionModeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSamplerFilterMinmaxProperties(const VkPhysicalDeviceSamplerFilterMinmaxProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkanMemoryModelFeatures(const VkPhysicalDeviceVulkanMemoryModelFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImagelessFramebufferFeatures(const VkPhysicalDeviceImagelessFramebufferFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebufferAttachmentImageInfo(const VkFramebufferAttachmentImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebufferAttachmentsCreateInfo(const VkFramebufferAttachmentsCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassAttachmentBeginInfo(const VkRenderPassAttachmentBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceUniformBufferStandardLayoutFeatures(const VkPhysicalDeviceUniformBufferStandardLayoutFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures(const VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures(const VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentReferenceStencilLayout(const VkAttachmentReferenceStencilLayout& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentDescriptionStencilLayout(const VkAttachmentDescriptionStencilLayout& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceHostQueryResetFeatures(const VkPhysicalDeviceHostQueryResetFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTimelineSemaphoreFeatures(const VkPhysicalDeviceTimelineSemaphoreFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTimelineSemaphoreProperties(const VkPhysicalDeviceTimelineSemaphoreProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreTypeCreateInfo(const VkSemaphoreTypeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTimelineSemaphoreSubmitInfo(const VkTimelineSemaphoreSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreWaitInfo(const VkSemaphoreWaitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreSignalInfo(const VkSemaphoreSignalInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceBufferDeviceAddressFeatures(const VkPhysicalDeviceBufferDeviceAddressFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferDeviceAddressInfo(const VkBufferDeviceAddressInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferOpaqueCaptureAddressCreateInfo(const VkBufferOpaqueCaptureAddressCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryOpaqueCaptureAddressAllocateInfo(const VkMemoryOpaqueCaptureAddressAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceMemoryOpaqueCaptureAddressInfo(const VkDeviceMemoryOpaqueCaptureAddressInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan13Features(const VkPhysicalDeviceVulkan13Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan13Properties(const VkPhysicalDeviceVulkan13Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreationFeedback(const VkPipelineCreationFeedback& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreationFeedbackCreateInfo(const VkPipelineCreationFeedbackCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderTerminateInvocationFeatures(const VkPhysicalDeviceShaderTerminateInvocationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceToolProperties(const VkPhysicalDeviceToolProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures(const VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePrivateDataFeatures(const VkPhysicalDevicePrivateDataFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDevicePrivateDataCreateInfo(const VkDevicePrivateDataCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPrivateDataSlotCreateInfo(const VkPrivateDataSlotCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineCreationCacheControlFeatures(const VkPhysicalDevicePipelineCreationCacheControlFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryBarrier2(const VkMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferMemoryBarrier2(const VkBufferMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageMemoryBarrier2(const VkImageMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDependencyInfo(const VkDependencyInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreSubmitInfo(const VkSemaphoreSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferSubmitInfo(const VkCommandBufferSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubmitInfo2(const VkSubmitInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSynchronization2Features(const VkPhysicalDeviceSynchronization2Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures(const VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageRobustnessFeatures(const VkPhysicalDeviceImageRobustnessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferCopy2(const VkBufferCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyBufferInfo2(const VkCopyBufferInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCopy2(const VkImageCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyImageInfo2(const VkCopyImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferImageCopy2(const VkBufferImageCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyBufferToImageInfo2(const VkCopyBufferToImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyImageToBufferInfo2(const VkCopyImageToBufferInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageBlit2(const VkImageBlit2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBlitImageInfo2(const VkBlitImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageResolve2(const VkImageResolve2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkResolveImageInfo2(const VkResolveImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSubgroupSizeControlFeatures(const VkPhysicalDeviceSubgroupSizeControlFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSubgroupSizeControlProperties(const VkPhysicalDeviceSubgroupSizeControlProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineShaderStageRequiredSubgroupSizeCreateInfo(const VkPipelineShaderStageRequiredSubgroupSizeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceInlineUniformBlockFeatures(const VkPhysicalDeviceInlineUniformBlockFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceInlineUniformBlockProperties(const VkPhysicalDeviceInlineUniformBlockProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteDescriptorSetInlineUniformBlock(const VkWriteDescriptorSetInlineUniformBlock& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorPoolInlineUniformBlockCreateInfo(const VkDescriptorPoolInlineUniformBlockCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTextureCompressionASTCHDRFeatures(const VkPhysicalDeviceTextureCompressionASTCHDRFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingAttachmentInfo(const VkRenderingAttachmentInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingInfo(const VkRenderingInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRenderingCreateInfo(const VkPipelineRenderingCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDynamicRenderingFeatures(const VkPhysicalDeviceDynamicRenderingFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferInheritanceRenderingInfo(const VkCommandBufferInheritanceRenderingInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderIntegerDotProductFeatures(const VkPhysicalDeviceShaderIntegerDotProductFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderIntegerDotProductProperties(const VkPhysicalDeviceShaderIntegerDotProductProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTexelBufferAlignmentProperties(const VkPhysicalDeviceTexelBufferAlignmentProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFormatProperties3(const VkFormatProperties3& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance4Features(const VkPhysicalDeviceMaintenance4Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance4Properties(const VkPhysicalDeviceMaintenance4Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan14Features(const VkPhysicalDeviceVulkan14Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVulkan14Properties(const VkPhysicalDeviceVulkan14Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceQueueGlobalPriorityCreateInfo(const VkDeviceQueueGlobalPriorityCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceGlobalPriorityQueryFeatures(const VkPhysicalDeviceGlobalPriorityQueryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyGlobalPriorityProperties(const VkQueueFamilyGlobalPriorityProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderSubgroupRotateFeatures(const VkPhysicalDeviceShaderSubgroupRotateFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderFloatControls2Features(const VkPhysicalDeviceShaderFloatControls2Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderExpectAssumeFeatures(const VkPhysicalDeviceShaderExpectAssumeFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLineRasterizationFeatures(const VkPhysicalDeviceLineRasterizationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLineRasterizationProperties(const VkPhysicalDeviceLineRasterizationProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationLineStateCreateInfo(const VkPipelineRasterizationLineStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVertexAttributeDivisorProperties(const VkPhysicalDeviceVertexAttributeDivisorProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVertexInputBindingDivisorDescription(const VkVertexInputBindingDivisorDescription& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineVertexInputDivisorStateCreateInfo(const VkPipelineVertexInputDivisorStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVertexAttributeDivisorFeatures(const VkPhysicalDeviceVertexAttributeDivisorFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceIndexTypeUint8Features(const VkPhysicalDeviceIndexTypeUint8Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryMapInfo(const VkMemoryMapInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryUnmapInfo(const VkMemoryUnmapInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance5Features(const VkPhysicalDeviceMaintenance5Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance5Properties(const VkPhysicalDeviceMaintenance5Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingAreaInfo(const VkRenderingAreaInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageSubresource2(const VkImageSubresource2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceImageSubresourceInfo(const VkDeviceImageSubresourceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubresourceLayout2(const VkSubresourceLayout2& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreateFlags2CreateInfo(const VkPipelineCreateFlags2CreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferUsageFlags2CreateInfo(const VkBufferUsageFlags2CreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePushDescriptorProperties(const VkPhysicalDevicePushDescriptorProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDynamicRenderingLocalReadFeatures(const VkPhysicalDeviceDynamicRenderingLocalReadFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingAttachmentLocationInfo(const VkRenderingAttachmentLocationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingInputAttachmentIndexInfo(const VkRenderingInputAttachmentIndexInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance6Features(const VkPhysicalDeviceMaintenance6Features& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance6Properties(const VkPhysicalDeviceMaintenance6Properties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindMemoryStatus(const VkBindMemoryStatus& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindDescriptorSetsInfo(const VkBindDescriptorSetsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPushConstantsInfo(const VkPushConstantsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPushDescriptorSetInfo(const VkPushDescriptorSetInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPushDescriptorSetWithTemplateInfo(const VkPushDescriptorSetWithTemplateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineProtectedAccessFeatures(const VkPhysicalDevicePipelineProtectedAccessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineRobustnessFeatures(const VkPhysicalDevicePipelineRobustnessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineRobustnessProperties(const VkPhysicalDevicePipelineRobustnessProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRobustnessCreateInfo(const VkPipelineRobustnessCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceHostImageCopyFeatures(const VkPhysicalDeviceHostImageCopyFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceHostImageCopyProperties(const VkPhysicalDeviceHostImageCopyProperties& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryToImageCopy(const VkMemoryToImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageToMemoryCopy(const VkImageToMemoryCopy& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMemoryToImageInfo(const VkCopyMemoryToImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyImageToMemoryInfo(const VkCopyImageToMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyImageToImageInfo(const VkCopyImageToImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHostImageLayoutTransitionInfo(const VkHostImageLayoutTransitionInfo& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubresourceHostMemcpySize(const VkSubresourceHostMemcpySize& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHostImageCopyDevicePerformanceQuery(const VkHostImageCopyDevicePerformanceQuery& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceCapabilitiesKHR(const VkSurfaceCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceFormatKHR(const VkSurfaceFormatKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainCreateInfoKHR(const VkSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentInfoKHR(const VkPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageSwapchainCreateInfoKHR(const VkImageSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindImageMemorySwapchainInfoKHR(const VkBindImageMemorySwapchainInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAcquireNextImageInfoKHR(const VkAcquireNextImageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupPresentCapabilitiesKHR(const VkDeviceGroupPresentCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupPresentInfoKHR(const VkDeviceGroupPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceGroupSwapchainCreateInfoKHR(const VkDeviceGroupSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModeParametersKHR(const VkDisplayModeParametersKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModeCreateInfoKHR(const VkDisplayModeCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModePropertiesKHR(const VkDisplayModePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlaneCapabilitiesKHR(const VkDisplayPlaneCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlanePropertiesKHR(const VkDisplayPlanePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPropertiesKHR(const VkDisplayPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplaySurfaceCreateInfoKHR(const VkDisplaySurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPresentInfoKHR(const VkDisplayPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_VkXlibSurfaceCreateInfoKHR(const VkXlibSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_VkXcbSurfaceCreateInfoKHR(const VkXcbSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_html_VkWaylandSurfaceCreateInfoKHR(const VkWaylandSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidSurfaceCreateInfoKHR(const VkAndroidSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkWin32SurfaceCreateInfoKHR(const VkWin32SurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkDebugReportCallbackCreateInfoEXT(const VkDebugReportCallbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationStateRasterizationOrderAMD(const VkPipelineRasterizationStateRasterizationOrderAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugMarkerObjectNameInfoEXT(const VkDebugMarkerObjectNameInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugMarkerObjectTagInfoEXT(const VkDebugMarkerObjectTagInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugMarkerMarkerInfoEXT(const VkDebugMarkerMarkerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyQueryResultStatusPropertiesKHR(const VkQueueFamilyQueryResultStatusPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyVideoPropertiesKHR(const VkQueueFamilyVideoPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoProfileInfoKHR(const VkVideoProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoProfileListInfoKHR(const VkVideoProfileListInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCapabilitiesKHR(const VkVideoCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVideoFormatInfoKHR(const VkPhysicalDeviceVideoFormatInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoFormatPropertiesKHR(const VkVideoFormatPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoPictureResourceInfoKHR(const VkVideoPictureResourceInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoReferenceSlotInfoKHR(const VkVideoReferenceSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionMemoryRequirementsKHR(const VkVideoSessionMemoryRequirementsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindVideoSessionMemoryInfoKHR(const VkBindVideoSessionMemoryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionCreateInfoKHR(const VkVideoSessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionParametersCreateInfoKHR(const VkVideoSessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoSessionParametersUpdateInfoKHR(const VkVideoSessionParametersUpdateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoBeginCodingInfoKHR(const VkVideoBeginCodingInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEndCodingInfoKHR(const VkVideoEndCodingInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoCodingControlInfoKHR(const VkVideoCodingControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeCapabilitiesKHR(const VkVideoDecodeCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeUsageInfoKHR(const VkVideoDecodeUsageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeInfoKHR(const VkVideoDecodeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDedicatedAllocationImageCreateInfoNV(const VkDedicatedAllocationImageCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDedicatedAllocationBufferCreateInfoNV(const VkDedicatedAllocationBufferCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDedicatedAllocationMemoryAllocateInfoNV(const VkDedicatedAllocationMemoryAllocateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTransformFeedbackFeaturesEXT(const VkPhysicalDeviceTransformFeedbackFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTransformFeedbackPropertiesEXT(const VkPhysicalDeviceTransformFeedbackPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationStateStreamCreateInfoEXT(const VkPipelineRasterizationStateStreamCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCuModuleCreateInfoNVX(const VkCuModuleCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCuModuleTexturingModeCreateInfoNVX(const VkCuModuleTexturingModeCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCuFunctionCreateInfoNVX(const VkCuFunctionCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCuLaunchInfoNVX(const VkCuLaunchInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewHandleInfoNVX(const VkImageViewHandleInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewAddressPropertiesNVX(const VkImageViewAddressPropertiesNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264CapabilitiesKHR(const VkVideoEncodeH264CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264QpKHR(const VkVideoEncodeH264QpKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264QualityLevelPropertiesKHR(const VkVideoEncodeH264QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264SessionCreateInfoKHR(const VkVideoEncodeH264SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264SessionParametersAddInfoKHR(const VkVideoEncodeH264SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264SessionParametersCreateInfoKHR(const VkVideoEncodeH264SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264SessionParametersGetInfoKHR(const VkVideoEncodeH264SessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264SessionParametersFeedbackInfoKHR(const VkVideoEncodeH264SessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264NaluSliceInfoKHR(const VkVideoEncodeH264NaluSliceInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264PictureInfoKHR(const VkVideoEncodeH264PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264DpbSlotInfoKHR(const VkVideoEncodeH264DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264ProfileInfoKHR(const VkVideoEncodeH264ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264RateControlInfoKHR(const VkVideoEncodeH264RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264FrameSizeKHR(const VkVideoEncodeH264FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264RateControlLayerInfoKHR(const VkVideoEncodeH264RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264GopRemainingFrameInfoKHR(const VkVideoEncodeH264GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265CapabilitiesKHR(const VkVideoEncodeH265CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265SessionCreateInfoKHR(const VkVideoEncodeH265SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265QpKHR(const VkVideoEncodeH265QpKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265QualityLevelPropertiesKHR(const VkVideoEncodeH265QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265SessionParametersAddInfoKHR(const VkVideoEncodeH265SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265SessionParametersCreateInfoKHR(const VkVideoEncodeH265SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265SessionParametersGetInfoKHR(const VkVideoEncodeH265SessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265SessionParametersFeedbackInfoKHR(const VkVideoEncodeH265SessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265NaluSliceSegmentInfoKHR(const VkVideoEncodeH265NaluSliceSegmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265PictureInfoKHR(const VkVideoEncodeH265PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265DpbSlotInfoKHR(const VkVideoEncodeH265DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265ProfileInfoKHR(const VkVideoEncodeH265ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265RateControlInfoKHR(const VkVideoEncodeH265RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265FrameSizeKHR(const VkVideoEncodeH265FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265RateControlLayerInfoKHR(const VkVideoEncodeH265RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265GopRemainingFrameInfoKHR(const VkVideoEncodeH265GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264ProfileInfoKHR(const VkVideoDecodeH264ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264CapabilitiesKHR(const VkVideoDecodeH264CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264SessionParametersAddInfoKHR(const VkVideoDecodeH264SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264SessionParametersCreateInfoKHR(const VkVideoDecodeH264SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264PictureInfoKHR(const VkVideoDecodeH264PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264DpbSlotInfoKHR(const VkVideoDecodeH264DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTextureLODGatherFormatPropertiesAMD(const VkTextureLODGatherFormatPropertiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderResourceUsageAMD(const VkShaderResourceUsageAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderStatisticsInfoAMD(const VkShaderStatisticsInfoAMD& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_html_VkStreamDescriptorSurfaceCreateInfoGGP(const VkStreamDescriptorSurfaceCreateInfoGGP& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
void dump_html_VkPhysicalDeviceCornerSampledImageFeaturesNV(const VkPhysicalDeviceCornerSampledImageFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalImageFormatPropertiesNV(const VkExternalImageFormatPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryImageCreateInfoNV(const VkExternalMemoryImageCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExportMemoryAllocateInfoNV(const VkExportMemoryAllocateInfoNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkImportMemoryWin32HandleInfoNV(const VkImportMemoryWin32HandleInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkExportMemoryWin32HandleInfoNV(const VkExportMemoryWin32HandleInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkWin32KeyedMutexAcquireReleaseInfoNV(const VkWin32KeyedMutexAcquireReleaseInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkValidationFlagsEXT(const VkValidationFlagsEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_html_VkViSurfaceCreateInfoNN(const VkViSurfaceCreateInfoNN& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_VI_NN
void dump_html_VkImageViewASTCDecodeModeEXT(const VkImageViewASTCDecodeModeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceASTCDecodeFeaturesEXT(const VkPhysicalDeviceASTCDecodeFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkImportMemoryWin32HandleInfoKHR(const VkImportMemoryWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkExportMemoryWin32HandleInfoKHR(const VkExportMemoryWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkMemoryWin32HandlePropertiesKHR(const VkMemoryWin32HandlePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkMemoryGetWin32HandleInfoKHR(const VkMemoryGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkImportMemoryFdInfoKHR(const VkImportMemoryFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryFdPropertiesKHR(const VkMemoryFdPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryGetFdInfoKHR(const VkMemoryGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkWin32KeyedMutexAcquireReleaseInfoKHR(const VkWin32KeyedMutexAcquireReleaseInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkImportSemaphoreWin32HandleInfoKHR(const VkImportSemaphoreWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkExportSemaphoreWin32HandleInfoKHR(const VkExportSemaphoreWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkD3D12FenceSubmitInfoKHR(const VkD3D12FenceSubmitInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkSemaphoreGetWin32HandleInfoKHR(const VkSemaphoreGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkImportSemaphoreFdInfoKHR(const VkImportSemaphoreFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSemaphoreGetFdInfoKHR(const VkSemaphoreGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkConditionalRenderingBeginInfoEXT(const VkConditionalRenderingBeginInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceConditionalRenderingFeaturesEXT(const VkPhysicalDeviceConditionalRenderingFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferInheritanceConditionalRenderingInfoEXT(const VkCommandBufferInheritanceConditionalRenderingInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRectLayerKHR(const VkRectLayerKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentRegionKHR(const VkPresentRegionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentRegionsKHR(const VkPresentRegionsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkViewportWScalingNV(const VkViewportWScalingNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportWScalingStateCreateInfoNV(const VkPipelineViewportWScalingStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceCapabilities2EXT(const VkSurfaceCapabilities2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPowerInfoEXT(const VkDisplayPowerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceEventInfoEXT(const VkDeviceEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayEventInfoEXT(const VkDisplayEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainCounterCreateInfoEXT(const VkSwapchainCounterCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRefreshCycleDurationGOOGLE(const VkRefreshCycleDurationGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPastPresentationTimingGOOGLE(const VkPastPresentationTimingGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentTimeGOOGLE(const VkPresentTimeGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentTimesInfoGOOGLE(const VkPresentTimesInfoGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX(const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMultiviewPerViewAttributesInfoNVX(const VkMultiviewPerViewAttributesInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkViewportSwizzleNV(const VkViewportSwizzleNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportSwizzleStateCreateInfoNV(const VkPipelineViewportSwizzleStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDiscardRectanglePropertiesEXT(const VkPhysicalDeviceDiscardRectanglePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineDiscardRectangleStateCreateInfoEXT(const VkPipelineDiscardRectangleStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceConservativeRasterizationPropertiesEXT(const VkPhysicalDeviceConservativeRasterizationPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationConservativeStateCreateInfoEXT(const VkPipelineRasterizationConservativeStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDepthClipEnableFeaturesEXT(const VkPhysicalDeviceDepthClipEnableFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationDepthClipStateCreateInfoEXT(const VkPipelineRasterizationDepthClipStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkXYColorEXT(const VkXYColorEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHdrMetadataEXT(const VkHdrMetadataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG(const VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSharedPresentSurfaceCapabilitiesKHR(const VkSharedPresentSurfaceCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkImportFenceWin32HandleInfoKHR(const VkImportFenceWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkExportFenceWin32HandleInfoKHR(const VkExportFenceWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkFenceGetWin32HandleInfoKHR(const VkFenceGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkImportFenceFdInfoKHR(const VkImportFenceFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFenceGetFdInfoKHR(const VkFenceGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePerformanceQueryFeaturesKHR(const VkPhysicalDevicePerformanceQueryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePerformanceQueryPropertiesKHR(const VkPhysicalDevicePerformanceQueryPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterKHR(const VkPerformanceCounterKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterDescriptionKHR(const VkPerformanceCounterDescriptionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPoolPerformanceCreateInfoKHR(const VkQueryPoolPerformanceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAcquireProfilingLockInfoKHR(const VkAcquireProfilingLockInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceQuerySubmitInfoKHR(const VkPerformanceQuerySubmitInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSurfaceInfo2KHR(const VkPhysicalDeviceSurfaceInfo2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceCapabilities2KHR(const VkSurfaceCapabilities2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceFormat2KHR(const VkSurfaceFormat2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayProperties2KHR(const VkDisplayProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlaneProperties2KHR(const VkDisplayPlaneProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModeProperties2KHR(const VkDisplayModeProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlaneInfo2KHR(const VkDisplayPlaneInfo2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayPlaneCapabilities2KHR(const VkDisplayPlaneCapabilities2KHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_html_VkIOSSurfaceCreateInfoMVK(const VkIOSSurfaceCreateInfoMVK& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_html_VkMacOSSurfaceCreateInfoMVK(const VkMacOSSurfaceCreateInfoMVK& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_html_VkDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsObjectNameInfoEXT(const VkDebugUtilsObjectNameInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessengerCallbackDataEXT(const VkDebugUtilsMessengerCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsMessengerCreateInfoEXT(const VkDebugUtilsMessengerCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDebugUtilsObjectTagInfoEXT(const VkDebugUtilsObjectTagInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidHardwareBufferUsageANDROID(const VkAndroidHardwareBufferUsageANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidHardwareBufferPropertiesANDROID(const VkAndroidHardwareBufferPropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidHardwareBufferFormatPropertiesANDROID(const VkAndroidHardwareBufferFormatPropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkImportAndroidHardwareBufferInfoANDROID(const VkImportAndroidHardwareBufferInfoANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkMemoryGetAndroidHardwareBufferInfoANDROID(const VkMemoryGetAndroidHardwareBufferInfoANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkExternalFormatANDROID(const VkExternalFormatANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidHardwareBufferFormatProperties2ANDROID(const VkAndroidHardwareBufferFormatProperties2ANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDeviceShaderEnqueueFeaturesAMDX(const VkPhysicalDeviceShaderEnqueueFeaturesAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDeviceShaderEnqueuePropertiesAMDX(const VkPhysicalDeviceShaderEnqueuePropertiesAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkExecutionGraphPipelineScratchSizeAMDX(const VkExecutionGraphPipelineScratchSizeAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkPipelineLibraryCreateInfoKHR(const VkPipelineLibraryCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkExecutionGraphPipelineCreateInfoAMDX(const VkExecutionGraphPipelineCreateInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkDispatchGraphInfoAMDX(const VkDispatchGraphInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkDispatchGraphCountInfoAMDX(const VkDispatchGraphCountInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPipelineShaderStageNodeCreateInfoAMDX(const VkPipelineShaderStageNodeCreateInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkAttachmentSampleCountInfoAMD(const VkAttachmentSampleCountInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderBfloat16FeaturesKHR(const VkPhysicalDeviceShaderBfloat16FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSampleLocationEXT(const VkSampleLocationEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSampleLocationsInfoEXT(const VkSampleLocationsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAttachmentSampleLocationsEXT(const VkAttachmentSampleLocationsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassSampleLocationsEXT(const VkSubpassSampleLocationsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassSampleLocationsBeginInfoEXT(const VkRenderPassSampleLocationsBeginInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineSampleLocationsStateCreateInfoEXT(const VkPipelineSampleLocationsStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSampleLocationsPropertiesEXT(const VkPhysicalDeviceSampleLocationsPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMultisamplePropertiesEXT(const VkMultisamplePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT(const VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT(const VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineColorBlendAdvancedStateCreateInfoEXT(const VkPipelineColorBlendAdvancedStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCoverageToColorStateCreateInfoNV(const VkPipelineCoverageToColorStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureBuildRangeInfoKHR(const VkAccelerationStructureBuildRangeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAabbPositionsKHR(const VkAabbPositionsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryTrianglesDataKHR(const VkAccelerationStructureGeometryTrianglesDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTransformMatrixKHR(const VkTransformMatrixKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryAabbsDataKHR(const VkAccelerationStructureGeometryAabbsDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryInstancesDataKHR(const VkAccelerationStructureGeometryInstancesDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryKHR(const VkAccelerationStructureGeometryKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureBuildGeometryInfoKHR(const VkAccelerationStructureBuildGeometryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureInstanceKHR(const VkAccelerationStructureInstanceKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureCreateInfoKHR(const VkAccelerationStructureCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteDescriptorSetAccelerationStructureKHR(const VkWriteDescriptorSetAccelerationStructureKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceAccelerationStructureFeaturesKHR(const VkPhysicalDeviceAccelerationStructureFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceAccelerationStructurePropertiesKHR(const VkPhysicalDeviceAccelerationStructurePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureDeviceAddressInfoKHR(const VkAccelerationStructureDeviceAddressInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureVersionInfoKHR(const VkAccelerationStructureVersionInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyAccelerationStructureToMemoryInfoKHR(const VkCopyAccelerationStructureToMemoryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMemoryToAccelerationStructureInfoKHR(const VkCopyMemoryToAccelerationStructureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyAccelerationStructureInfoKHR(const VkCopyAccelerationStructureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureBuildSizesInfoKHR(const VkAccelerationStructureBuildSizesInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCoverageModulationStateCreateInfoNV(const VkPipelineCoverageModulationStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderSMBuiltinsPropertiesNV(const VkPhysicalDeviceShaderSMBuiltinsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderSMBuiltinsFeaturesNV(const VkPhysicalDeviceShaderSMBuiltinsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrmFormatModifierPropertiesEXT(const VkDrmFormatModifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrmFormatModifierPropertiesListEXT(const VkDrmFormatModifierPropertiesListEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageDrmFormatModifierInfoEXT(const VkPhysicalDeviceImageDrmFormatModifierInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageDrmFormatModifierListCreateInfoEXT(const VkImageDrmFormatModifierListCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageDrmFormatModifierExplicitCreateInfoEXT(const VkImageDrmFormatModifierExplicitCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageDrmFormatModifierPropertiesEXT(const VkImageDrmFormatModifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrmFormatModifierProperties2EXT(const VkDrmFormatModifierProperties2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrmFormatModifierPropertiesList2EXT(const VkDrmFormatModifierPropertiesList2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationCacheCreateInfoEXT(const VkValidationCacheCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderModuleValidationCacheCreateInfoEXT(const VkShaderModuleValidationCacheCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDevicePortabilitySubsetFeaturesKHR(const VkPhysicalDevicePortabilitySubsetFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDevicePortabilitySubsetPropertiesKHR(const VkPhysicalDevicePortabilitySubsetPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkShadingRatePaletteNV(const VkShadingRatePaletteNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportShadingRateImageStateCreateInfoNV(const VkPipelineViewportShadingRateImageStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShadingRateImageFeaturesNV(const VkPhysicalDeviceShadingRateImageFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShadingRateImagePropertiesNV(const VkPhysicalDeviceShadingRateImagePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCoarseSampleLocationNV(const VkCoarseSampleLocationNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCoarseSampleOrderCustomNV(const VkCoarseSampleOrderCustomNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportCoarseSampleOrderStateCreateInfoNV(const VkPipelineViewportCoarseSampleOrderStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingShaderGroupCreateInfoNV(const VkRayTracingShaderGroupCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingPipelineCreateInfoNV(const VkRayTracingPipelineCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryTrianglesNV(const VkGeometryTrianglesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryAABBNV(const VkGeometryAABBNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryDataNV(const VkGeometryDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeometryNV(const VkGeometryNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureInfoNV(const VkAccelerationStructureInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureCreateInfoNV(const VkAccelerationStructureCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindAccelerationStructureMemoryInfoNV(const VkBindAccelerationStructureMemoryInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteDescriptorSetAccelerationStructureNV(const VkWriteDescriptorSetAccelerationStructureNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMemoryRequirementsInfoNV(const VkAccelerationStructureMemoryRequirementsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingPropertiesNV(const VkPhysicalDeviceRayTracingPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV(const VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRepresentativeFragmentTestStateCreateInfoNV(const VkPipelineRepresentativeFragmentTestStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageViewImageFormatInfoEXT(const VkPhysicalDeviceImageViewImageFormatInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFilterCubicImageViewImageFormatPropertiesEXT(const VkFilterCubicImageViewImageFormatPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImportMemoryHostPointerInfoEXT(const VkImportMemoryHostPointerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryHostPointerPropertiesEXT(const VkMemoryHostPointerPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalMemoryHostPropertiesEXT(const VkPhysicalDeviceExternalMemoryHostPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderClockFeaturesKHR(const VkPhysicalDeviceShaderClockFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCompilerControlCreateInfoAMD(const VkPipelineCompilerControlCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCalibratedTimestampInfoKHR(const VkCalibratedTimestampInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderCorePropertiesAMD(const VkPhysicalDeviceShaderCorePropertiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265ProfileInfoKHR(const VkVideoDecodeH265ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265CapabilitiesKHR(const VkVideoDecodeH265CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265SessionParametersAddInfoKHR(const VkVideoDecodeH265SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265SessionParametersCreateInfoKHR(const VkVideoDecodeH265SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265PictureInfoKHR(const VkVideoDecodeH265PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265DpbSlotInfoKHR(const VkVideoDecodeH265DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceMemoryOverallocationCreateInfoAMD(const VkDeviceMemoryOverallocationCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT(const VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_html_VkPresentFrameTokenGGP(const VkPresentFrameTokenGGP& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
void dump_html_VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR(const VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMeshShaderFeaturesNV(const VkPhysicalDeviceMeshShaderFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMeshShaderPropertiesNV(const VkPhysicalDeviceMeshShaderPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrawMeshTasksIndirectCommandNV(const VkDrawMeshTasksIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR(const VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderImageFootprintFeaturesNV(const VkPhysicalDeviceShaderImageFootprintFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportExclusiveScissorStateCreateInfoNV(const VkPipelineViewportExclusiveScissorStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExclusiveScissorFeaturesNV(const VkPhysicalDeviceExclusiveScissorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyCheckpointPropertiesNV(const VkQueueFamilyCheckpointPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCheckpointDataNV(const VkCheckpointDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueueFamilyCheckpointProperties2NV(const VkQueueFamilyCheckpointProperties2NV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCheckpointData2NV(const VkCheckpointData2NV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL(const VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceValueINTEL(const VkPerformanceValueINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkInitializePerformanceApiInfoINTEL(const VkInitializePerformanceApiInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPoolPerformanceQueryCreateInfoINTEL(const VkQueryPoolPerformanceQueryCreateInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceMarkerInfoINTEL(const VkPerformanceMarkerInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceStreamMarkerInfoINTEL(const VkPerformanceStreamMarkerInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceOverrideInfoINTEL(const VkPerformanceOverrideInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceConfigurationAcquireInfoINTEL(const VkPerformanceConfigurationAcquireInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePCIBusInfoPropertiesEXT(const VkPhysicalDevicePCIBusInfoPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayNativeHdrSurfaceCapabilitiesAMD(const VkDisplayNativeHdrSurfaceCapabilitiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainDisplayNativeHdrCreateInfoAMD(const VkSwapchainDisplayNativeHdrCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImagePipeSurfaceCreateInfoFUCHSIA(const VkImagePipeSurfaceCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkMetalSurfaceCreateInfoEXT(const VkMetalSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkPhysicalDeviceFragmentDensityMapFeaturesEXT(const VkPhysicalDeviceFragmentDensityMapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentDensityMapPropertiesEXT(const VkPhysicalDeviceFragmentDensityMapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassFragmentDensityMapCreateInfoEXT(const VkRenderPassFragmentDensityMapCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingFragmentDensityMapAttachmentInfoEXT(const VkRenderingFragmentDensityMapAttachmentInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFragmentShadingRateAttachmentInfoKHR(const VkFragmentShadingRateAttachmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineFragmentShadingRateStateCreateInfoKHR(const VkPipelineFragmentShadingRateStateCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShadingRateFeaturesKHR(const VkPhysicalDeviceFragmentShadingRateFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShadingRatePropertiesKHR(const VkPhysicalDeviceFragmentShadingRatePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShadingRateKHR(const VkPhysicalDeviceFragmentShadingRateKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderingFragmentShadingRateAttachmentInfoKHR(const VkRenderingFragmentShadingRateAttachmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderCoreProperties2AMD(const VkPhysicalDeviceShaderCoreProperties2AMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCoherentMemoryFeaturesAMD(const VkPhysicalDeviceCoherentMemoryFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT(const VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderQuadControlFeaturesKHR(const VkPhysicalDeviceShaderQuadControlFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMemoryBudgetPropertiesEXT(const VkPhysicalDeviceMemoryBudgetPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMemoryPriorityFeaturesEXT(const VkPhysicalDeviceMemoryPriorityFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryPriorityAllocateInfoEXT(const VkMemoryPriorityAllocateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceProtectedCapabilitiesKHR(const VkSurfaceProtectedCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV(const VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceBufferDeviceAddressFeaturesEXT(const VkPhysicalDeviceBufferDeviceAddressFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferDeviceAddressCreateInfoEXT(const VkBufferDeviceAddressCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkValidationFeaturesEXT(const VkValidationFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePresentWaitFeaturesKHR(const VkPhysicalDevicePresentWaitFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCooperativeMatrixPropertiesNV(const VkCooperativeMatrixPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeMatrixFeaturesNV(const VkPhysicalDeviceCooperativeMatrixFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeMatrixPropertiesNV(const VkPhysicalDeviceCooperativeMatrixPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCoverageReductionModeFeaturesNV(const VkPhysicalDeviceCoverageReductionModeFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCoverageReductionStateCreateInfoNV(const VkPipelineCoverageReductionStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFramebufferMixedSamplesCombinationNV(const VkFramebufferMixedSamplesCombinationNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT(const VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceYcbcrImageArraysFeaturesEXT(const VkPhysicalDeviceYcbcrImageArraysFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceProvokingVertexFeaturesEXT(const VkPhysicalDeviceProvokingVertexFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceProvokingVertexPropertiesEXT(const VkPhysicalDeviceProvokingVertexPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineRasterizationProvokingVertexStateCreateInfoEXT(const VkPipelineRasterizationProvokingVertexStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkSurfaceFullScreenExclusiveInfoEXT(const VkSurfaceFullScreenExclusiveInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkSurfaceCapabilitiesFullScreenExclusiveEXT(const VkSurfaceCapabilitiesFullScreenExclusiveEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_VkSurfaceFullScreenExclusiveWin32InfoEXT(const VkSurfaceFullScreenExclusiveWin32InfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_VkHeadlessSurfaceCreateInfoEXT(const VkHeadlessSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderAtomicFloatFeaturesEXT(const VkPhysicalDeviceShaderAtomicFloatFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExtendedDynamicStateFeaturesEXT(const VkPhysicalDeviceExtendedDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR(const VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineInfoKHR(const VkPipelineInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineExecutablePropertiesKHR(const VkPipelineExecutablePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineExecutableInfoKHR(const VkPipelineExecutableInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineExecutableStatisticKHR(const VkPipelineExecutableStatisticKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineExecutableInternalRepresentationKHR(const VkPipelineExecutableInternalRepresentationKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMapMemoryPlacedFeaturesEXT(const VkPhysicalDeviceMapMemoryPlacedFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMapMemoryPlacedPropertiesEXT(const VkPhysicalDeviceMapMemoryPlacedPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryMapPlacedInfoEXT(const VkMemoryMapPlacedInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT(const VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfacePresentModeEXT(const VkSurfacePresentModeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfacePresentScalingCapabilitiesEXT(const VkSurfacePresentScalingCapabilitiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfacePresentModeCompatibilityEXT(const VkSurfacePresentModeCompatibilityEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT(const VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainPresentFenceInfoEXT(const VkSwapchainPresentFenceInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainPresentModesCreateInfoEXT(const VkSwapchainPresentModesCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainPresentModeInfoEXT(const VkSwapchainPresentModeInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainPresentScalingCreateInfoEXT(const VkSwapchainPresentScalingCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkReleaseSwapchainImagesInfoEXT(const VkReleaseSwapchainImagesInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV(const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV(const VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGraphicsShaderGroupCreateInfoNV(const VkGraphicsShaderGroupCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGraphicsPipelineShaderGroupsCreateInfoNV(const VkGraphicsPipelineShaderGroupsCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindShaderGroupIndirectCommandNV(const VkBindShaderGroupIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindIndexBufferIndirectCommandNV(const VkBindIndexBufferIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindVertexBufferIndirectCommandNV(const VkBindVertexBufferIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSetStateFlagsIndirectCommandNV(const VkSetStateFlagsIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsStreamNV(const VkIndirectCommandsStreamNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutTokenNV(const VkIndirectCommandsLayoutTokenNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutCreateInfoNV(const VkIndirectCommandsLayoutCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeneratedCommandsInfoNV(const VkGeneratedCommandsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeneratedCommandsMemoryRequirementsInfoNV(const VkGeneratedCommandsMemoryRequirementsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceInheritedViewportScissorFeaturesNV(const VkPhysicalDeviceInheritedViewportScissorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferInheritanceViewportScissorInfoNV(const VkCommandBufferInheritanceViewportScissorInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT(const VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassTransformBeginInfoQCOM(const VkRenderPassTransformBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCommandBufferInheritanceRenderPassTransformInfoQCOM(const VkCommandBufferInheritanceRenderPassTransformInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDepthBiasControlFeaturesEXT(const VkPhysicalDeviceDepthBiasControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDepthBiasInfoEXT(const VkDepthBiasInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDepthBiasRepresentationInfoEXT(const VkDepthBiasRepresentationInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDeviceMemoryReportFeaturesEXT(const VkPhysicalDeviceDeviceMemoryReportFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceMemoryReportCallbackDataEXT(const VkDeviceMemoryReportCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceDeviceMemoryReportCreateInfoEXT(const VkDeviceDeviceMemoryReportCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRobustness2FeaturesEXT(const VkPhysicalDeviceRobustness2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRobustness2PropertiesEXT(const VkPhysicalDeviceRobustness2PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerCustomBorderColorCreateInfoEXT(const VkSamplerCustomBorderColorCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCustomBorderColorPropertiesEXT(const VkPhysicalDeviceCustomBorderColorPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCustomBorderColorFeaturesEXT(const VkPhysicalDeviceCustomBorderColorFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePresentBarrierFeaturesNV(const VkPhysicalDevicePresentBarrierFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSurfaceCapabilitiesPresentBarrierNV(const VkSurfaceCapabilitiesPresentBarrierNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainPresentBarrierCreateInfoNV(const VkSwapchainPresentBarrierCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPresentIdKHR(const VkPresentIdKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePresentIdFeaturesKHR(const VkPhysicalDevicePresentIdFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeInfoKHR(const VkVideoEncodeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeCapabilitiesKHR(const VkVideoEncodeCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryPoolVideoEncodeFeedbackCreateInfoKHR(const VkQueryPoolVideoEncodeFeedbackCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeUsageInfoKHR(const VkVideoEncodeUsageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeRateControlLayerInfoKHR(const VkVideoEncodeRateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeRateControlInfoKHR(const VkVideoEncodeRateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR(const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeQualityLevelPropertiesKHR(const VkVideoEncodeQualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeQualityLevelInfoKHR(const VkVideoEncodeQualityLevelInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeSessionParametersGetInfoKHR(const VkVideoEncodeSessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeSessionParametersFeedbackInfoKHR(const VkVideoEncodeSessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDiagnosticsConfigFeaturesNV(const VkPhysicalDeviceDiagnosticsConfigFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceDiagnosticsConfigCreateInfoNV(const VkDeviceDiagnosticsConfigCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkCudaModuleCreateInfoNV(const VkCudaModuleCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkCudaFunctionCreateInfoNV(const VkCudaFunctionCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkCudaLaunchInfoNV(const VkCudaLaunchInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDeviceCudaKernelLaunchFeaturesNV(const VkPhysicalDeviceCudaKernelLaunchFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDeviceCudaKernelLaunchPropertiesNV(const VkPhysicalDeviceCudaKernelLaunchPropertiesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkPhysicalDeviceTileShadingFeaturesQCOM(const VkPhysicalDeviceTileShadingFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTileShadingPropertiesQCOM(const VkPhysicalDeviceTileShadingPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassTileShadingCreateInfoQCOM(const VkRenderPassTileShadingCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerTileBeginInfoQCOM(const VkPerTileBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerTileEndInfoQCOM(const VkPerTileEndInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDispatchTileInfoQCOM(const VkDispatchTileInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkQueryLowLatencySupportNV(const VkQueryLowLatencySupportNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalObjectCreateInfoEXT(const VkExportMetalObjectCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalObjectsInfoEXT(const VkExportMetalObjectsInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalDeviceInfoEXT(const VkExportMetalDeviceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalCommandQueueInfoEXT(const VkExportMetalCommandQueueInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalBufferInfoEXT(const VkExportMetalBufferInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkImportMetalBufferInfoEXT(const VkImportMetalBufferInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalTextureInfoEXT(const VkExportMetalTextureInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkImportMetalTextureInfoEXT(const VkImportMetalTextureInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalIOSurfaceInfoEXT(const VkExportMetalIOSurfaceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkImportMetalIOSurfaceInfoEXT(const VkImportMetalIOSurfaceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkExportMetalSharedEventInfoEXT(const VkExportMetalSharedEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkImportMetalSharedEventInfoEXT(const VkImportMetalSharedEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkPhysicalDeviceDescriptorBufferPropertiesEXT(const VkPhysicalDeviceDescriptorBufferPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT(const VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDescriptorBufferFeaturesEXT(const VkPhysicalDeviceDescriptorBufferFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorAddressInfoEXT(const VkDescriptorAddressInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorBufferBindingInfoEXT(const VkDescriptorBufferBindingInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorBufferBindingPushDescriptorBufferHandleEXT(const VkDescriptorBufferBindingPushDescriptorBufferHandleEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorGetInfoEXT(const VkDescriptorGetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBufferCaptureDescriptorDataInfoEXT(const VkBufferCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCaptureDescriptorDataInfoEXT(const VkImageCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewCaptureDescriptorDataInfoEXT(const VkImageViewCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerCaptureDescriptorDataInfoEXT(const VkSamplerCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpaqueCaptureDescriptorDataCreateInfoEXT(const VkOpaqueCaptureDescriptorDataCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureCaptureDescriptorDataInfoEXT(const VkAccelerationStructureCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT(const VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT(const VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGraphicsPipelineLibraryCreateInfoEXT(const VkGraphicsPipelineLibraryCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD(const VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR(const VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR(const VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV(const VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV(const VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineFragmentShadingRateEnumStateCreateInfoNV(const VkPipelineFragmentShadingRateEnumStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryMotionTrianglesDataNV(const VkAccelerationStructureGeometryMotionTrianglesDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInfoNV(const VkAccelerationStructureMotionInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMatrixMotionInstanceNV(const VkAccelerationStructureMatrixMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSRTDataNV(const VkSRTDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureSRTMotionInstanceNV(const VkAccelerationStructureSRTMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInstanceNV(const VkAccelerationStructureMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingMotionBlurFeaturesNV(const VkPhysicalDeviceRayTracingMotionBlurFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMeshShaderFeaturesEXT(const VkPhysicalDeviceMeshShaderFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMeshShaderPropertiesEXT(const VkPhysicalDeviceMeshShaderPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrawMeshTasksIndirectCommandEXT(const VkDrawMeshTasksIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT(const VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentDensityMap2FeaturesEXT(const VkPhysicalDeviceFragmentDensityMap2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentDensityMap2PropertiesEXT(const VkPhysicalDeviceFragmentDensityMap2PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyCommandTransformInfoQCOM(const VkCopyCommandTransformInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR(const VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageCompressionControlFeaturesEXT(const VkPhysicalDeviceImageCompressionControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCompressionControlEXT(const VkImageCompressionControlEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageCompressionPropertiesEXT(const VkImageCompressionPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT(const VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevice4444FormatsFeaturesEXT(const VkPhysicalDevice4444FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFaultFeaturesEXT(const VkPhysicalDeviceFaultFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultCountsEXT(const VkDeviceFaultCountsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultAddressInfoEXT(const VkDeviceFaultAddressInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultVendorInfoEXT(const VkDeviceFaultVendorInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultInfoEXT(const VkDeviceFaultInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceFaultVendorBinaryHeaderVersionOneEXT(const VkDeviceFaultVendorBinaryHeaderVersionOneEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT(const VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT(const VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_html_VkDirectFBSurfaceCreateInfoEXT(const VkDirectFBSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_html_VkRayTracingShaderGroupCreateInfoKHR(const VkRayTracingShaderGroupCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingPipelineInterfaceCreateInfoKHR(const VkRayTracingPipelineInterfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingPipelineCreateInfoKHR(const VkRayTracingPipelineCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingPipelineFeaturesKHR(const VkPhysicalDeviceRayTracingPipelineFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingPipelinePropertiesKHR(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStridedDeviceAddressRegionKHR(const VkStridedDeviceAddressRegionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTraceRaysIndirectCommandKHR(const VkTraceRaysIndirectCommandKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayQueryFeaturesKHR(const VkPhysicalDeviceRayQueryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT(const VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMutableDescriptorTypeListEXT(const VkMutableDescriptorTypeListEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMutableDescriptorTypeCreateInfoEXT(const VkMutableDescriptorTypeCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT(const VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVertexInputBindingDescription2EXT(const VkVertexInputBindingDescription2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVertexInputAttributeDescription2EXT(const VkVertexInputAttributeDescription2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDrmPropertiesEXT(const VkPhysicalDeviceDrmPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceAddressBindingReportFeaturesEXT(const VkPhysicalDeviceAddressBindingReportFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceAddressBindingCallbackDataEXT(const VkDeviceAddressBindingCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDepthClipControlFeaturesEXT(const VkPhysicalDeviceDepthClipControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportDepthClipControlCreateInfoEXT(const VkPipelineViewportDepthClipControlCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT(const VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT(const VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImportMemoryZirconHandleInfoFUCHSIA(const VkImportMemoryZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkMemoryZirconHandlePropertiesFUCHSIA(const VkMemoryZirconHandlePropertiesFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkMemoryGetZirconHandleInfoFUCHSIA(const VkMemoryGetZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImportSemaphoreZirconHandleInfoFUCHSIA(const VkImportSemaphoreZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkSemaphoreGetZirconHandleInfoFUCHSIA(const VkSemaphoreGetZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferCollectionCreateInfoFUCHSIA(const VkBufferCollectionCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImportMemoryBufferCollectionFUCHSIA(const VkImportMemoryBufferCollectionFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferCollectionImageCreateInfoFUCHSIA(const VkBufferCollectionImageCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferCollectionConstraintsInfoFUCHSIA(const VkBufferCollectionConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferConstraintsInfoFUCHSIA(const VkBufferConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferCollectionBufferCreateInfoFUCHSIA(const VkBufferCollectionBufferCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkSysmemColorSpaceFUCHSIA(const VkSysmemColorSpaceFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkBufferCollectionPropertiesFUCHSIA(const VkBufferCollectionPropertiesFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImageFormatConstraintsInfoFUCHSIA(const VkImageFormatConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_VkImageConstraintsInfoFUCHSIA(const VkImageConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_html_VkSubpassShadingPipelineCreateInfoHUAWEI(const VkSubpassShadingPipelineCreateInfoHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSubpassShadingFeaturesHUAWEI(const VkPhysicalDeviceSubpassShadingFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSubpassShadingPropertiesHUAWEI(const VkPhysicalDeviceSubpassShadingPropertiesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceInvocationMaskFeaturesHUAWEI(const VkPhysicalDeviceInvocationMaskFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryGetRemoteAddressInfoNV(const VkMemoryGetRemoteAddressInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalMemoryRDMAFeaturesNV(const VkPhysicalDeviceExternalMemoryRDMAFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelinePropertiesIdentifierEXT(const VkPipelinePropertiesIdentifierEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelinePropertiesFeaturesEXT(const VkPhysicalDevicePipelinePropertiesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFrameBoundaryFeaturesEXT(const VkPhysicalDeviceFrameBoundaryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkFrameBoundaryEXT(const VkFrameBoundaryEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT(const VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSubpassResolvePerformanceQueryEXT(const VkSubpassResolvePerformanceQueryEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMultisampledRenderToSingleSampledInfoEXT(const VkMultisampledRenderToSingleSampledInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExtendedDynamicState2FeaturesEXT(const VkPhysicalDeviceExtendedDynamicState2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkScreenSurfaceCreateInfoQNX(const VkScreenSurfaceCreateInfoQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_html_VkPhysicalDeviceColorWriteEnableFeaturesEXT(const VkPhysicalDeviceColorWriteEnableFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineColorWriteCreateInfoEXT(const VkPipelineColorWriteCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT(const VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR(const VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTraceRaysIndirectCommand2KHR(const VkTraceRaysIndirectCommand2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageViewMinLodFeaturesEXT(const VkPhysicalDeviceImageViewMinLodFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewMinLodCreateInfoEXT(const VkImageViewMinLodCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiDrawFeaturesEXT(const VkPhysicalDeviceMultiDrawFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiDrawPropertiesEXT(const VkPhysicalDeviceMultiDrawPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMultiDrawInfoEXT(const VkMultiDrawInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMultiDrawIndexedInfoEXT(const VkMultiDrawIndexedInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImage2DViewOf3DFeaturesEXT(const VkPhysicalDeviceImage2DViewOf3DFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderTileImageFeaturesEXT(const VkPhysicalDeviceShaderTileImageFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderTileImagePropertiesEXT(const VkPhysicalDeviceShaderTileImagePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapUsageEXT(const VkMicromapUsageEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapBuildInfoEXT(const VkMicromapBuildInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapCreateInfoEXT(const VkMicromapCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceOpacityMicromapFeaturesEXT(const VkPhysicalDeviceOpacityMicromapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceOpacityMicromapPropertiesEXT(const VkPhysicalDeviceOpacityMicromapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapVersionInfoEXT(const VkMicromapVersionInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMicromapToMemoryInfoEXT(const VkCopyMicromapToMemoryInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMemoryToMicromapInfoEXT(const VkCopyMemoryToMicromapInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMicromapInfoEXT(const VkCopyMicromapInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapBuildSizesInfoEXT(const VkMicromapBuildSizesInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureTrianglesOpacityMicromapEXT(const VkAccelerationStructureTrianglesOpacityMicromapEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMicromapTriangleEXT(const VkMicromapTriangleEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDeviceDisplacementMicromapFeaturesNV(const VkPhysicalDeviceDisplacementMicromapFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDeviceDisplacementMicromapPropertiesNV(const VkPhysicalDeviceDisplacementMicromapPropertiesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkAccelerationStructureTrianglesDisplacementMicromapNV(const VkAccelerationStructureTrianglesDisplacementMicromapNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI(const VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI(const VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI(const VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceBorderColorSwizzleFeaturesEXT(const VkPhysicalDeviceBorderColorSwizzleFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerBorderColorComponentMappingCreateInfoEXT(const VkSamplerBorderColorComponentMappingCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT(const VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderCorePropertiesARM(const VkPhysicalDeviceShaderCorePropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceQueueShaderCoreControlCreateInfoARM(const VkDeviceQueueShaderCoreControlCreateInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSchedulingControlsFeaturesARM(const VkPhysicalDeviceSchedulingControlsFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSchedulingControlsPropertiesARM(const VkPhysicalDeviceSchedulingControlsPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT(const VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewSlicedCreateInfoEXT(const VkImageViewSlicedCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE(const VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetBindingReferenceVALVE(const VkDescriptorSetBindingReferenceVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetLayoutHostMappingInfoVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDepthClampZeroOneFeaturesKHR(const VkPhysicalDeviceDepthClampZeroOneFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT(const VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRenderPassStripedFeaturesARM(const VkPhysicalDeviceRenderPassStripedFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRenderPassStripedPropertiesARM(const VkPhysicalDeviceRenderPassStripedPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassStripeInfoARM(const VkRenderPassStripeInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassStripeBeginInfoARM(const VkRenderPassStripeBeginInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassStripeSubmitInfoARM(const VkRenderPassStripeSubmitInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentDensityMapOffsetFeaturesEXT(const VkPhysicalDeviceFragmentDensityMapOffsetFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceFragmentDensityMapOffsetPropertiesEXT(const VkPhysicalDeviceFragmentDensityMapOffsetPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassFragmentDensityMapOffsetEndInfoEXT(const VkRenderPassFragmentDensityMapOffsetEndInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMemoryIndirectCommandNV(const VkCopyMemoryIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCopyMemoryToImageIndirectCommandNV(const VkCopyMemoryToImageIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCopyMemoryIndirectFeaturesNV(const VkPhysicalDeviceCopyMemoryIndirectFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCopyMemoryIndirectPropertiesNV(const VkPhysicalDeviceCopyMemoryIndirectPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDecompressMemoryRegionNV(const VkDecompressMemoryRegionNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMemoryDecompressionFeaturesNV(const VkPhysicalDeviceMemoryDecompressionFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMemoryDecompressionPropertiesNV(const VkPhysicalDeviceMemoryDecompressionPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV(const VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkComputePipelineIndirectBufferInfoNV(const VkComputePipelineIndirectBufferInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineIndirectDeviceAddressInfoNV(const VkPipelineIndirectDeviceAddressInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindPipelineIndirectCommandNV(const VkBindPipelineIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV(const VkPhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryLinearSweptSpheresDataNV(const VkAccelerationStructureGeometryLinearSweptSpheresDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometrySpheresDataNV(const VkAccelerationStructureGeometrySpheresDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLinearColorAttachmentFeaturesNV(const VkPhysicalDeviceLinearColorAttachmentFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR(const VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT(const VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageViewSampleWeightCreateInfoQCOM(const VkImageViewSampleWeightCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageProcessingFeaturesQCOM(const VkPhysicalDeviceImageProcessingFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageProcessingPropertiesQCOM(const VkPhysicalDeviceImageProcessingPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceNestedCommandBufferFeaturesEXT(const VkPhysicalDeviceNestedCommandBufferFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceNestedCommandBufferPropertiesEXT(const VkPhysicalDeviceNestedCommandBufferPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalMemoryAcquireUnmodifiedEXT(const VkExternalMemoryAcquireUnmodifiedEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExtendedDynamicState3FeaturesEXT(const VkPhysicalDeviceExtendedDynamicState3FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExtendedDynamicState3PropertiesEXT(const VkPhysicalDeviceExtendedDynamicState3PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkColorBlendEquationEXT(const VkColorBlendEquationEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkColorBlendAdvancedEXT(const VkColorBlendAdvancedEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT(const VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreationControlEXT(const VkRenderPassCreationControlEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreationFeedbackInfoEXT(const VkRenderPassCreationFeedbackInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassCreationFeedbackCreateInfoEXT(const VkRenderPassCreationFeedbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassSubpassFeedbackInfoEXT(const VkRenderPassSubpassFeedbackInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRenderPassSubpassFeedbackCreateInfoEXT(const VkRenderPassSubpassFeedbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDirectDriverLoadingInfoLUNARG(const VkDirectDriverLoadingInfoLUNARG& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDirectDriverLoadingListLUNARG(const VkDirectDriverLoadingListLUNARG& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT(const VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT(const VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineShaderStageModuleIdentifierCreateInfoEXT(const VkPipelineShaderStageModuleIdentifierCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderModuleIdentifierEXT(const VkShaderModuleIdentifierEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceOpticalFlowFeaturesNV(const VkPhysicalDeviceOpticalFlowFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceOpticalFlowPropertiesNV(const VkPhysicalDeviceOpticalFlowPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowImageFormatInfoNV(const VkOpticalFlowImageFormatInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowImageFormatPropertiesNV(const VkOpticalFlowImageFormatPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowSessionCreateInfoNV(const VkOpticalFlowSessionCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowSessionCreatePrivateDataInfoNV(const VkOpticalFlowSessionCreatePrivateDataInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOpticalFlowExecuteInfoNV(const VkOpticalFlowExecuteInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLegacyDitheringFeaturesEXT(const VkPhysicalDeviceLegacyDitheringFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkPhysicalDeviceExternalFormatResolveFeaturesANDROID(const VkPhysicalDeviceExternalFormatResolveFeaturesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkPhysicalDeviceExternalFormatResolvePropertiesANDROID(const VkPhysicalDeviceExternalFormatResolvePropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_VkAndroidHardwareBufferFormatResolvePropertiesANDROID(const VkAndroidHardwareBufferFormatResolvePropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
void dump_html_VkPhysicalDeviceAntiLagFeaturesAMD(const VkPhysicalDeviceAntiLagFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAntiLagPresentationInfoAMD(const VkAntiLagPresentationInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAntiLagDataAMD(const VkAntiLagDataAMD& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR(const VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderObjectFeaturesEXT(const VkPhysicalDeviceShaderObjectFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderObjectPropertiesEXT(const VkPhysicalDeviceShaderObjectPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkShaderCreateInfoEXT(const VkShaderCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDepthClampRangeEXT(const VkDepthClampRangeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineBinaryFeaturesKHR(const VkPhysicalDevicePipelineBinaryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineBinaryPropertiesKHR(const VkPhysicalDevicePipelineBinaryPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDevicePipelineBinaryInternalCacheControlKHR(const VkDevicePipelineBinaryInternalCacheControlKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryKeyKHR(const VkPipelineBinaryKeyKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryDataKHR(const VkPipelineBinaryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryKeysAndDataKHR(const VkPipelineBinaryKeysAndDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineCreateInfoKHR(const VkPipelineCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryCreateInfoKHR(const VkPipelineBinaryCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryInfoKHR(const VkPipelineBinaryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkReleaseCapturedPipelineDataInfoKHR(const VkReleaseCapturedPipelineDataInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryDataInfoKHR(const VkPipelineBinaryDataInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineBinaryHandlesInfoKHR(const VkPipelineBinaryHandlesInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTilePropertiesFeaturesQCOM(const VkPhysicalDeviceTilePropertiesFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTilePropertiesQCOM(const VkTilePropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceAmigoProfilingFeaturesSEC(const VkPhysicalDeviceAmigoProfilingFeaturesSEC& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAmigoProfilingSubmitInfoSEC(const VkAmigoProfilingSubmitInfoSEC& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM(const VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV(const VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV(const VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeVectorPropertiesNV(const VkPhysicalDeviceCooperativeVectorPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeVectorFeaturesNV(const VkPhysicalDeviceCooperativeVectorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCooperativeVectorPropertiesNV(const VkCooperativeVectorPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkConvertCooperativeVectorMatrixInfoNV(const VkConvertCooperativeVectorMatrixInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV(const VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV(const VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT(const VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT(const VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLayerSettingEXT(const VkLayerSettingEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLayerSettingsCreateInfoEXT(const VkLayerSettingsCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM(const VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM(const VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT(const VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT(const VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLatencySleepModeInfoNV(const VkLatencySleepModeInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLatencySleepInfoNV(const VkLatencySleepInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSetLatencyMarkerInfoNV(const VkSetLatencyMarkerInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLatencyTimingsFrameReportNV(const VkLatencyTimingsFrameReportNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGetLatencyMarkerInfoNV(const VkGetLatencyMarkerInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLatencySubmissionPresentIdNV(const VkLatencySubmissionPresentIdNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSwapchainLatencyCreateInfoNV(const VkSwapchainLatencyCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkOutOfBandQueueTypeInfoNV(const VkOutOfBandQueueTypeInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkLatencySurfaceCapabilitiesNV(const VkLatencySurfaceCapabilitiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCooperativeMatrixPropertiesKHR(const VkCooperativeMatrixPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeMatrixFeaturesKHR(const VkPhysicalDeviceCooperativeMatrixFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeMatrixPropertiesKHR(const VkPhysicalDeviceCooperativeMatrixPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM(const VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM(const VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR(const VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeAV1ProfileInfoKHR(const VkVideoDecodeAV1ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeAV1CapabilitiesKHR(const VkVideoDecodeAV1CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeAV1SessionParametersCreateInfoKHR(const VkVideoDecodeAV1SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeAV1PictureInfoKHR(const VkVideoDecodeAV1PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeAV1DpbSlotInfoKHR(const VkVideoDecodeAV1DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVideoEncodeAV1FeaturesKHR(const VkPhysicalDeviceVideoEncodeAV1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1CapabilitiesKHR(const VkVideoEncodeAV1CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1QIndexKHR(const VkVideoEncodeAV1QIndexKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1QualityLevelPropertiesKHR(const VkVideoEncodeAV1QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1SessionCreateInfoKHR(const VkVideoEncodeAV1SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1SessionParametersCreateInfoKHR(const VkVideoEncodeAV1SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1PictureInfoKHR(const VkVideoEncodeAV1PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1DpbSlotInfoKHR(const VkVideoEncodeAV1DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1ProfileInfoKHR(const VkVideoEncodeAV1ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1FrameSizeKHR(const VkVideoEncodeAV1FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1GopRemainingFrameInfoKHR(const VkVideoEncodeAV1GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1RateControlInfoKHR(const VkVideoEncodeAV1RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1RateControlLayerInfoKHR(const VkVideoEncodeAV1RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVideoMaintenance1FeaturesKHR(const VkPhysicalDeviceVideoMaintenance1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoInlineQueryInfoKHR(const VkVideoInlineQueryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePerStageDescriptorSetFeaturesNV(const VkPhysicalDevicePerStageDescriptorSetFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageProcessing2FeaturesQCOM(const VkPhysicalDeviceImageProcessing2FeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageProcessing2PropertiesQCOM(const VkPhysicalDeviceImageProcessing2PropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerBlockMatchWindowCreateInfoQCOM(const VkSamplerBlockMatchWindowCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCubicWeightsFeaturesQCOM(const VkPhysicalDeviceCubicWeightsFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerCubicWeightsCreateInfoQCOM(const VkSamplerCubicWeightsCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBlitImageCubicWeightsInfoQCOM(const VkBlitImageCubicWeightsInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceYcbcrDegammaFeaturesQCOM(const VkPhysicalDeviceYcbcrDegammaFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM(const VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCubicClampFeaturesQCOM(const VkPhysicalDeviceCubicClampFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT(const VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkScreenBufferPropertiesQNX(const VkScreenBufferPropertiesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkScreenBufferFormatPropertiesQNX(const VkScreenBufferFormatPropertiesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkImportScreenBufferInfoQNX(const VkImportScreenBufferInfoQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkExternalFormatQNX(const VkExternalFormatQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX(const VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_html_VkPhysicalDeviceLayeredDriverPropertiesMSFT(const VkPhysicalDeviceLayeredDriverPropertiesMSFT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkSetDescriptorBufferOffsetsInfoEXT(const VkSetDescriptorBufferOffsetsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindDescriptorBufferEmbeddedSamplersInfoEXT(const VkBindDescriptorBufferEmbeddedSamplersInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV(const VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTileMemoryHeapFeaturesQCOM(const VkPhysicalDeviceTileMemoryHeapFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceTileMemoryHeapPropertiesQCOM(const VkPhysicalDeviceTileMemoryHeapPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTileMemoryRequirementsQCOM(const VkTileMemoryRequirementsQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTileMemoryBindInfoQCOM(const VkTileMemoryBindInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkTileMemorySizeInfoQCOM(const VkTileMemorySizeInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplaySurfaceStereoCreateInfoNV(const VkDisplaySurfaceStereoCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDisplayModeStereoPropertiesNV(const VkDisplayModeStereoPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeQuantizationMapCapabilitiesKHR(const VkVideoEncodeQuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoFormatQuantizationMapPropertiesKHR(const VkVideoFormatQuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeQuantizationMapInfoKHR(const VkVideoEncodeQuantizationMapInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR(const VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR(const VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH264QuantizationMapCapabilitiesKHR(const VkVideoEncodeH264QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeH265QuantizationMapCapabilitiesKHR(const VkVideoEncodeH265QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoFormatH265QuantizationMapPropertiesKHR(const VkVideoFormatH265QuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoEncodeAV1QuantizationMapCapabilitiesKHR(const VkVideoEncodeAV1QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoFormatAV1QuantizationMapPropertiesKHR(const VkVideoFormatAV1QuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRawAccessChainsFeaturesNV(const VkPhysicalDeviceRawAccessChainsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalComputeQueueDeviceCreateInfoNV(const VkExternalComputeQueueDeviceCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalComputeQueueCreateInfoNV(const VkExternalComputeQueueCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkExternalComputeQueueDataParamsNV(const VkExternalComputeQueueDataParamsNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceExternalComputeQueuePropertiesNV(const VkPhysicalDeviceExternalComputeQueuePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR(const VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCommandBufferInheritanceFeaturesNV(const VkPhysicalDeviceCommandBufferInheritanceFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance7FeaturesKHR(const VkPhysicalDeviceMaintenance7FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance7PropertiesKHR(const VkPhysicalDeviceMaintenance7PropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLayeredApiPropertiesKHR(const VkPhysicalDeviceLayeredApiPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLayeredApiPropertiesListKHR(const VkPhysicalDeviceLayeredApiPropertiesListKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceLayeredApiVulkanPropertiesKHR(const VkPhysicalDeviceLayeredApiVulkanPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV(const VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT(const VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceRayTracingValidationFeaturesNV(const VkPhysicalDeviceRayTracingValidationFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceClusterAccelerationStructureFeaturesNV(const VkPhysicalDeviceClusterAccelerationStructureFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceClusterAccelerationStructurePropertiesNV(const VkPhysicalDeviceClusterAccelerationStructurePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureClustersBottomLevelInputNV(const VkClusterAccelerationStructureClustersBottomLevelInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureTriangleClusterInputNV(const VkClusterAccelerationStructureTriangleClusterInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureMoveObjectsInputNV(const VkClusterAccelerationStructureMoveObjectsInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureInputInfoNV(const VkClusterAccelerationStructureInputInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureCommandsInfoNV(const VkClusterAccelerationStructureCommandsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkStridedDeviceAddressNV(const VkStridedDeviceAddressNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureGeometryIndexAndGeometryFlagsNV(const VkClusterAccelerationStructureGeometryIndexAndGeometryFlagsNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureMoveObjectsInfoNV(const VkClusterAccelerationStructureMoveObjectsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV(const VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureBuildTriangleClusterInfoNV(const VkClusterAccelerationStructureBuildTriangleClusterInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV(const VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureInstantiateClusterInfoNV(const VkClusterAccelerationStructureInstantiateClusterInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkRayTracingPipelineClusterAccelerationStructureCreateInfoNV(const VkRayTracingPipelineClusterAccelerationStructureCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePartitionedAccelerationStructureFeaturesNV(const VkPhysicalDevicePartitionedAccelerationStructureFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePartitionedAccelerationStructurePropertiesNV(const VkPhysicalDevicePartitionedAccelerationStructurePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureFlagsNV(const VkPartitionedAccelerationStructureFlagsNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildPartitionedAccelerationStructureIndirectCommandNV(const VkBuildPartitionedAccelerationStructureIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureWriteInstanceDataNV(const VkPartitionedAccelerationStructureWriteInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureUpdateInstanceDataNV(const VkPartitionedAccelerationStructureUpdateInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureWritePartitionTranslationDataNV(const VkPartitionedAccelerationStructureWritePartitionTranslationDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteDescriptorSetPartitionedAccelerationStructureNV(const VkWriteDescriptorSetPartitionedAccelerationStructureNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPartitionedAccelerationStructureInstancesInputNV(const VkPartitionedAccelerationStructureInstancesInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBuildPartitionedAccelerationStructureInfoNV(const VkBuildPartitionedAccelerationStructureInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT(const VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT(const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeneratedCommandsMemoryRequirementsInfoEXT(const VkGeneratedCommandsMemoryRequirementsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetPipelineInfoEXT(const VkIndirectExecutionSetPipelineInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetShaderLayoutInfoEXT(const VkIndirectExecutionSetShaderLayoutInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetShaderInfoEXT(const VkIndirectExecutionSetShaderInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetCreateInfoEXT(const VkIndirectExecutionSetCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeneratedCommandsInfoEXT(const VkGeneratedCommandsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteIndirectExecutionSetPipelineEXT(const VkWriteIndirectExecutionSetPipelineEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsPushConstantTokenEXT(const VkIndirectCommandsPushConstantTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsVertexBufferTokenEXT(const VkIndirectCommandsVertexBufferTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsIndexBufferTokenEXT(const VkIndirectCommandsIndexBufferTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsExecutionSetTokenEXT(const VkIndirectCommandsExecutionSetTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutTokenEXT(const VkIndirectCommandsLayoutTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsLayoutCreateInfoEXT(const VkIndirectCommandsLayoutCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDrawIndirectCountIndirectCommandEXT(const VkDrawIndirectCountIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindVertexBufferIndirectCommandEXT(const VkBindVertexBufferIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkBindIndexBufferIndirectCommandEXT(const VkBindIndexBufferIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeneratedCommandsPipelineInfoEXT(const VkGeneratedCommandsPipelineInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkGeneratedCommandsShaderInfoEXT(const VkGeneratedCommandsShaderInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkWriteIndirectExecutionSetShaderEXT(const VkWriteIndirectExecutionSetShaderEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceMaintenance8FeaturesKHR(const VkPhysicalDeviceMaintenance8FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkMemoryBarrierAccessFlags3KHR(const VkMemoryBarrierAccessFlags3KHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageAlignmentControlFeaturesMESA(const VkPhysicalDeviceImageAlignmentControlFeaturesMESA& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceImageAlignmentControlPropertiesMESA(const VkPhysicalDeviceImageAlignmentControlPropertiesMESA& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkImageAlignmentControlCreateInfoMESA(const VkImageAlignmentControlCreateInfoMESA& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceDepthClampControlFeaturesEXT(const VkPhysicalDeviceDepthClampControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineViewportDepthClampControlCreateInfoEXT(const VkPipelineViewportDepthClampControlCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceVideoMaintenance2FeaturesKHR(const VkPhysicalDeviceVideoMaintenance2FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH264InlineSessionParametersInfoKHR(const VkVideoDecodeH264InlineSessionParametersInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeH265InlineSessionParametersInfoKHR(const VkVideoDecodeH265InlineSessionParametersInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkVideoDecodeAV1InlineSessionParametersInfoKHR(const VkVideoDecodeAV1InlineSessionParametersInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceHdrVividFeaturesHUAWEI(const VkPhysicalDeviceHdrVividFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkHdrVividDynamicMetadataHUAWEI(const VkHdrVividDynamicMetadataHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkCooperativeMatrixFlexibleDimensionsPropertiesNV(const VkCooperativeMatrixFlexibleDimensionsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeMatrix2FeaturesNV(const VkPhysicalDeviceCooperativeMatrix2FeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDeviceCooperativeMatrix2PropertiesNV(const VkPhysicalDeviceCooperativeMatrix2PropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPhysicalDevicePipelineOpacityMicromapFeaturesARM(const VkPhysicalDevicePipelineOpacityMicromapFeaturesARM& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkImportMemoryMetalHandleInfoEXT(const VkImportMemoryMetalHandleInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkMemoryMetalHandlePropertiesEXT(const VkMemoryMetalHandlePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_VkMemoryGetMetalHandleInfoEXT(const VkMemoryGetMetalHandleInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT(const VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkSetPresentConfigNV(const VkSetPresentConfigNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkPhysicalDevicePresentMeteringFeaturesNV(const VkPhysicalDevicePresentMeteringFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkRenderingEndInfoEXT(const VkRenderingEndInfoEXT& object, const ApiDumpSettings& settings, int indents);

//========================== Union Implementations ==========================//

void dump_html_VkClearColorValue(const VkClearColorValue& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClearValue(const VkClearValue& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceCounterResultKHR(const VkPerformanceCounterResultKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_VkDeviceOrHostAddressConstAMDX(const VkDeviceOrHostAddressConstAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_VkDeviceOrHostAddressKHR(const VkDeviceOrHostAddressKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDeviceOrHostAddressConstKHR(const VkDeviceOrHostAddressConstKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureGeometryDataKHR(const VkAccelerationStructureGeometryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPerformanceValueDataINTEL(const VkPerformanceValueDataINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkPipelineExecutableStatisticValueKHR(const VkPipelineExecutableStatisticValueKHR& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkDescriptorDataEXT(const VkDescriptorDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkAccelerationStructureMotionInstanceDataNV(const VkAccelerationStructureMotionInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkClusterAccelerationStructureOpInputNV(const VkClusterAccelerationStructureOpInputNV& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectExecutionSetInfoEXT(const VkIndirectExecutionSetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_html_VkIndirectCommandsTokenDataEXT(const VkIndirectCommandsTokenDataEXT& object, const ApiDumpSettings& settings, int indents);

//======================== pNext Chain Implementation =======================//

void dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);

//========================= Function Implementations ========================//

void dump_html_vkCreateInstance(ApiDumpInstance& dump_inst, VkResult result, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void dump_html_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator);
void dump_html_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void dump_html_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
void dump_html_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void dump_html_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void dump_html_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void dump_html_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void dump_html_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void dump_html_vkCreateDevice(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
void dump_html_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator);
void dump_html_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_html_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_html_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_html_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_html_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void dump_html_vkQueueSubmit(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
void dump_html_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue);
void dump_html_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkDevice device);
void dump_html_vkAllocateMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
void dump_html_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
void dump_html_vkMapMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void dump_html_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory);
void dump_html_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_html_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_html_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
void dump_html_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_html_vkBindImageMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_html_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
void dump_html_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void dump_html_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void dump_html_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void dump_html_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);
void dump_html_vkCreateFence(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_html_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
void dump_html_vkResetFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences);
void dump_html_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFence fence);
void dump_html_vkWaitForFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
void dump_html_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
void dump_html_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void dump_html_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetEventStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_html_vkSetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_html_vkResetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_html_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
void dump_html_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_html_vkCreateBuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
void dump_html_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateBufferView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void dump_html_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
void dump_html_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void dump_html_vkCreateImageView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
void dump_html_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void dump_html_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void dump_html_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void dump_html_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);
void dump_html_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_html_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_html_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void dump_html_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateSampler(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void dump_html_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void dump_html_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void dump_html_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
void dump_html_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
void dump_html_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
void dump_html_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
void dump_html_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
void dump_html_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void dump_html_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_html_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void dump_html_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void dump_html_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
void dump_html_vkResetCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void dump_html_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void dump_html_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_html_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
void dump_html_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer);
void dump_html_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void dump_html_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_html_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
void dump_html_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_html_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth);
void dump_html_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void dump_html_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void dump_html_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void dump_html_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void dump_html_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void dump_html_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void dump_html_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
void dump_html_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void dump_html_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void dump_html_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void dump_html_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void dump_html_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_html_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_html_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_html_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_html_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
void dump_html_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
void dump_html_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
void dump_html_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_html_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_html_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void dump_html_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void dump_html_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_html_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_html_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void dump_html_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);
void dump_html_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_html_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_html_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_html_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_html_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void dump_html_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void dump_html_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_html_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void dump_html_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_html_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void dump_html_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void dump_html_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void dump_html_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_html_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_html_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_html_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_html_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_html_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_html_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_html_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_html_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_html_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_html_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_html_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_html_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_html_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_html_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_html_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_html_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_html_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void dump_html_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_html_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_html_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_html_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_html_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
void dump_html_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
void dump_html_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void dump_html_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_html_vkCmdDrawIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkCmdDrawIndexedIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkCreateRenderPass2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_html_vkCmdBeginRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_html_vkCmdNextSubpass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_html_vkCmdEndRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_html_vkResetQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_html_vkGetSemaphoreCounterValue(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_html_vkWaitSemaphores(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_html_vkSignalSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_html_vkGetBufferDeviceAddress(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_html_vkGetBufferOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_html_vkGetDeviceMemoryOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_html_vkGetPhysicalDeviceToolProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_html_vkCreatePrivateDataSlot(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_html_vkDestroyPrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_html_vkSetPrivateData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_html_vkGetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_html_vkCmdSetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_html_vkCmdResetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_html_vkCmdWaitEvents2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_html_vkCmdPipelineBarrier2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_html_vkCmdWriteTimestamp2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_html_vkQueueSubmit2(ApiDumpInstance& dump_inst, VkResult result, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_html_vkCmdCopyBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_html_vkCmdCopyImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_html_vkCmdCopyBufferToImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_html_vkCmdCopyImageToBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_html_vkCmdBlitImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_html_vkCmdResolveImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_html_vkCmdBeginRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_html_vkCmdEndRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
void dump_html_vkCmdSetCullMode(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_html_vkCmdSetFrontFace(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_html_vkCmdSetPrimitiveTopology(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_html_vkCmdSetViewportWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_html_vkCmdSetScissorWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_html_vkCmdBindVertexBuffers2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_html_vkCmdSetDepthTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_html_vkCmdSetDepthWriteEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_html_vkCmdSetDepthCompareOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_html_vkCmdSetDepthBoundsTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_html_vkCmdSetStencilTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_html_vkCmdSetStencilOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_html_vkCmdSetRasterizerDiscardEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_html_vkCmdSetDepthBiasEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_html_vkCmdSetPrimitiveRestartEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
void dump_html_vkGetDeviceBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetDeviceImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetDeviceImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_html_vkCmdSetLineStipple(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_html_vkMapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_html_vkUnmapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_html_vkCmdBindIndexBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_html_vkGetRenderingAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_html_vkGetDeviceImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_html_vkGetImageSubresourceLayout2(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_html_vkCmdPushDescriptorSet(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_html_vkCmdPushDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_html_vkCmdSetRenderingAttachmentLocations(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_html_vkCmdSetRenderingInputAttachmentIndices(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_html_vkCmdBindDescriptorSets2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_html_vkCmdPushConstants2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_html_vkCmdPushDescriptorSet2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_html_vkCmdPushDescriptorSetWithTemplate2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_html_vkCopyMemoryToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_html_vkCopyImageToMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_html_vkCopyImageToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_html_vkTransitionImageLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_html_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void dump_html_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
void dump_html_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
void dump_html_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
void dump_html_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
void dump_html_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void dump_html_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
void dump_html_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
void dump_html_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
void dump_html_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
void dump_html_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
void dump_html_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
void dump_html_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
void dump_html_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
void dump_html_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
void dump_html_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
void dump_html_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
void dump_html_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
void dump_html_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_html_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_html_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_html_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_html_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_html_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void dump_html_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
void dump_html_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
void dump_html_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo);
void dump_html_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo);
void dump_html_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_html_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_html_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_html_vkGetPhysicalDeviceVideoCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
void dump_html_vkGetPhysicalDeviceVideoFormatPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
void dump_html_vkCreateVideoSessionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);
void dump_html_vkDestroyVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetVideoSessionMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);
void dump_html_vkBindVideoSessionMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);
void dump_html_vkCreateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);
void dump_html_vkUpdateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);
void dump_html_vkDestroyVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCmdBeginVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo);
void dump_html_vkCmdEndVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo);
void dump_html_vkCmdControlVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo);
void dump_html_vkCmdDecodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo);
void dump_html_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes);
void dump_html_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_html_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_html_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);
void dump_html_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);
void dump_html_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);
void dump_html_vkCreateCuModuleNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);
void dump_html_vkCreateCuFunctionNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);
void dump_html_vkDestroyCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);
void dump_html_vkDestroyCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCmdCuLaunchKernelNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);
void dump_html_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_html_vkGetImageViewHandle64NVX(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_html_vkGetImageViewAddressNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);
void dump_html_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);
void dump_html_vkCmdBeginRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_html_vkCmdEndRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
#if defined(VK_USE_PLATFORM_GGP)
void dump_html_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
void dump_html_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_html_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_html_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_html_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_html_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_html_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_html_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_html_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_html_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_html_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_html_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
void dump_html_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_html_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_html_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_html_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);
void dump_html_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
void dump_html_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_html_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_html_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_html_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);
void dump_html_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_html_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_html_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_html_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_html_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void dump_html_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_html_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_html_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
void dump_html_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
void dump_html_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);
void dump_html_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_html_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_html_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);
void dump_html_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);
void dump_html_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);
void dump_html_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles);
void dump_html_vkCmdSetDiscardRectangleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);
void dump_html_vkCmdSetDiscardRectangleModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);
void dump_html_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);
void dump_html_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_html_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_html_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_html_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_html_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
void dump_html_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo);
void dump_html_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_html_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
void dump_html_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
void dump_html_vkAcquireProfilingLockKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo);
void dump_html_vkReleaseProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device);
void dump_html_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
void dump_html_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
void dump_html_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
void dump_html_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
void dump_html_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
void dump_html_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_html_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_html_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_html_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
void dump_html_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo);
void dump_html_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_html_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_html_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_html_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_html_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_html_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_html_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void dump_html_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
void dump_html_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCreateExecutionGraphPipelinesAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipelineCache                 pipelineCache, uint32_t                                        createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks*    pAllocator, VkPipeline*               pPipelines);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkGetExecutionGraphPipelineScratchSizeAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipeline                                      executionGraph, VkExecutionGraphPipelineScratchSizeAMDX*        pSizeInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkGetExecutionGraphPipelineNodeIndexAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipeline                                      executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX*  pNodeInfo, uint32_t*                                       pNodeIndex);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCmdInitializeGraphScratchMemoryAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkPipeline                                      executionGraph, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCmdDispatchGraphAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCmdDispatchGraphIndirectAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCmdDispatchGraphIndirectCountAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, VkDeviceAddress                                 countInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);
void dump_html_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
void dump_html_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_html_vkCreateAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure);
void dump_html_vkDestroyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCmdBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_html_vkCmdBuildAccelerationStructuresIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts);
void dump_html_vkBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_html_vkCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_html_vkCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_html_vkCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_html_vkWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_html_vkCmdCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_html_vkCmdCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_html_vkCmdCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_html_vkGetAccelerationStructureDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
void dump_html_vkCmdWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_html_vkGetDeviceAccelerationStructureCompatibilityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_html_vkGetAccelerationStructureBuildSizesKHR(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo);
void dump_html_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_html_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_html_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_html_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_html_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);
void dump_html_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);
void dump_html_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);
void dump_html_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);
void dump_html_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);
void dump_html_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_html_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes);
void dump_html_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);
void dump_html_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);
void dump_html_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);
void dump_html_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);
void dump_html_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);
void dump_html_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);
void dump_html_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);
void dump_html_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_html_vkGetRayTracingShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_html_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_html_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);
void dump_html_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_html_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t shader);
void dump_html_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_html_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);
void dump_html_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_html_vkCmdWriteBufferMarker2AMD(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker);
void dump_html_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_html_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_html_vkGetCalibratedTimestampsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_html_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_html_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);
void dump_html_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_html_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkCmdSetExclusiveScissorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables);
void dump_html_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors);
void dump_html_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker);
void dump_html_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);
void dump_html_vkGetQueueCheckpointData2NV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);
void dump_html_vkGetSemaphoreCounterValueKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_html_vkWaitSemaphoresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_html_vkSignalSemaphoreKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_html_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);
void dump_html_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device);
void dump_html_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);
void dump_html_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);
void dump_html_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo);
void dump_html_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);
void dump_html_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceConfigurationINTEL configuration);
void dump_html_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, VkPerformanceConfigurationINTEL configuration);
void dump_html_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);
void dump_html_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_vkGetPhysicalDeviceFragmentShadingRatesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
void dump_html_vkCmdSetFragmentShadingRateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_html_vkCmdSetRenderingAttachmentLocationsKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_html_vkCmdSetRenderingInputAttachmentIndicesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_html_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_html_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_html_vkWaitForPresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);
void dump_html_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
void dump_html_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_html_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_html_vkGetBufferDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_html_vkGetBufferOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_html_vkGetDeviceMemoryOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_html_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_html_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_html_vkCmdSetCullModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_html_vkCmdSetFrontFaceEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_html_vkCmdSetPrimitiveTopologyEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_html_vkCmdSetViewportWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_html_vkCmdSetScissorWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_html_vkCmdBindVertexBuffers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_html_vkCmdSetDepthTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_html_vkCmdSetDepthWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_html_vkCmdSetDepthCompareOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_html_vkCmdSetDepthBoundsTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_html_vkCmdSetStencilTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_html_vkCmdSetStencilOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_html_vkCreateDeferredOperationKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);
void dump_html_vkDestroyDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetDeferredOperationMaxConcurrencyKHR(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, VkDeferredOperationKHR operation);
void dump_html_vkGetDeferredOperationResultKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_html_vkDeferredOperationJoinKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_html_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);
void dump_html_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);
void dump_html_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);
void dump_html_vkCopyMemoryToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_html_vkCopyImageToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_html_vkCopyImageToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_html_vkTransitionImageLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_html_vkGetImageSubresourceLayout2EXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_html_vkMapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_html_vkUnmapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_html_vkReleaseSwapchainImagesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);
void dump_html_vkGetGeneratedCommandsMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkCmdPreprocessGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_html_vkCmdExecuteGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_html_vkCmdBindPipelineShaderGroupNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);
void dump_html_vkCreateIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);
void dump_html_vkDestroyIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCmdSetDepthBias2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo);
void dump_html_vkAcquireDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
void dump_html_vkGetDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
void dump_html_vkCreatePrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_html_vkDestroyPrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_html_vkSetPrivateDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_html_vkGetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_html_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);
void dump_html_vkGetEncodedVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);
void dump_html_vkCmdEncodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCreateCudaModuleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkGetCudaModuleCacheNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCreateCudaFunctionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkDestroyCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkDestroyCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_html_vkCmdCudaLaunchKernelNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_html_vkCmdDispatchTileQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_html_vkCmdBeginPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileBeginInfoQCOM* pPerTileBeginInfo);
void dump_html_vkCmdEndPerTileExecutionQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerTileEndInfoQCOM* pPerTileEndInfo);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_vkExportMetalObjectsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_vkCmdSetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_html_vkCmdResetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_html_vkCmdWaitEvents2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_html_vkCmdPipelineBarrier2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_html_vkCmdWriteTimestamp2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_html_vkQueueSubmit2KHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_html_vkGetDescriptorSetLayoutSizeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);
void dump_html_vkGetDescriptorSetLayoutBindingOffsetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);
void dump_html_vkGetDescriptorEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);
void dump_html_vkCmdBindDescriptorBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos);
void dump_html_vkCmdSetDescriptorBufferOffsetsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets);
void dump_html_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);
void dump_html_vkGetBufferOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_html_vkGetImageOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_html_vkGetImageViewOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_html_vkGetSamplerOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_html_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_html_vkCmdSetFragmentShadingRateEnumNV(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_html_vkCmdDrawMeshTasksEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_html_vkCmdDrawMeshTasksIndirectEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_html_vkCmdDrawMeshTasksIndirectCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_html_vkCmdCopyBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_html_vkCmdCopyImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_html_vkCmdCopyBufferToImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_html_vkCmdCopyImageToBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_html_vkCmdBlitImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_html_vkCmdResolveImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_html_vkGetDeviceFaultInfoEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkAcquireWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_html_vkGetWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_html_vkCreateDirectFBSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_html_vkGetPhysicalDeviceDirectFBPresentationSupportEXT(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_html_vkCmdTraceRaysKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
void dump_html_vkCreateRayTracingPipelinesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_html_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_html_vkCmdTraceRaysIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);
void dump_html_vkGetRayTracingShaderGroupStackSizeKHR(ApiDumpInstance& dump_inst, VkDeviceSize result, VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);
void dump_html_vkCmdSetRayTracingPipelineStackSizeKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);
void dump_html_vkCmdSetVertexInputEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkGetMemoryZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkGetMemoryZirconHandlePropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkImportSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkGetSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkCreateBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkSetBufferCollectionImageConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkSetBufferCollectionBufferConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkDestroyBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_html_vkGetBufferCollectionPropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_html_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);
void dump_html_vkCmdSubpassShadingHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_html_vkCmdBindInvocationMaskHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_html_vkGetMemoryRemoteAddressNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);
void dump_html_vkGetPipelinePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);
void dump_html_vkCmdSetPatchControlPointsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t patchControlPoints);
void dump_html_vkCmdSetRasterizerDiscardEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_html_vkCmdSetDepthBiasEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_html_vkCmdSetLogicOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLogicOp logicOp);
void dump_html_vkCmdSetPrimitiveRestartEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_vkCreateScreenSurfaceQNX(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_vkGetPhysicalDeviceScreenPresentationSupportQNX(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_html_vkCmdSetColorWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables);
void dump_html_vkCmdTraceRaysIndirect2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);
void dump_html_vkCmdDrawMultiEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);
void dump_html_vkCmdDrawMultiIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset);
void dump_html_vkCreateMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap);
void dump_html_vkDestroyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCmdBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_html_vkBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_html_vkCopyMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo);
void dump_html_vkCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_html_vkCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_html_vkWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_html_vkCmdCopyMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo);
void dump_html_vkCmdCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_html_vkCmdCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_html_vkCmdWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_html_vkGetDeviceMicromapCompatibilityEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_html_vkGetMicromapBuildSizesEXT(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo);
void dump_html_vkCmdDrawClusterHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_html_vkCmdDrawClusterIndirectHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_html_vkSetDeviceMemoryPriorityEXT(ApiDumpInstance& dump_inst, VkDevice       device, VkDeviceMemory memory, float          priority);
void dump_html_vkGetDeviceBufferMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetDeviceImageMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkGetDeviceImageSparseMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_html_vkGetDescriptorSetLayoutHostMappingInfoVALVE(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);
void dump_html_vkGetDescriptorSetHostMappingVALVE(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, void** ppData);
void dump_html_vkCmdCopyMemoryIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);
void dump_html_vkCmdCopyMemoryToImageIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources);
void dump_html_vkCmdDecompressMemoryNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions);
void dump_html_vkCmdDecompressMemoryIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);
void dump_html_vkGetPipelineIndirectMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkCmdUpdatePipelineIndirectBufferNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline);
void dump_html_vkGetPipelineIndirectDeviceAddressNV(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo);
void dump_html_vkCmdSetDepthClampEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);
void dump_html_vkCmdSetPolygonModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);
void dump_html_vkCmdSetRasterizationSamplesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples);
void dump_html_vkCmdSetSampleMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask);
void dump_html_vkCmdSetAlphaToCoverageEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);
void dump_html_vkCmdSetAlphaToOneEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);
void dump_html_vkCmdSetLogicOpEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);
void dump_html_vkCmdSetColorBlendEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables);
void dump_html_vkCmdSetColorBlendEquationEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations);
void dump_html_vkCmdSetColorWriteMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks);
void dump_html_vkCmdSetTessellationDomainOriginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);
void dump_html_vkCmdSetRasterizationStreamEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t rasterizationStream);
void dump_html_vkCmdSetConservativeRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);
void dump_html_vkCmdSetExtraPrimitiveOverestimationSizeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);
void dump_html_vkCmdSetDepthClipEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);
void dump_html_vkCmdSetSampleLocationsEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);
void dump_html_vkCmdSetColorBlendAdvancedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced);
void dump_html_vkCmdSetProvokingVertexModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);
void dump_html_vkCmdSetLineRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);
void dump_html_vkCmdSetLineStippleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);
void dump_html_vkCmdSetDepthClipNegativeOneToOneEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);
void dump_html_vkCmdSetViewportWScalingEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);
void dump_html_vkCmdSetViewportSwizzleNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles);
void dump_html_vkCmdSetCoverageToColorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);
void dump_html_vkCmdSetCoverageToColorLocationNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);
void dump_html_vkCmdSetCoverageModulationModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);
void dump_html_vkCmdSetCoverageModulationTableEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);
void dump_html_vkCmdSetCoverageModulationTableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable);
void dump_html_vkCmdSetShadingRateImageEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);
void dump_html_vkCmdSetRepresentativeFragmentTestEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);
void dump_html_vkCmdSetCoverageReductionModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);
void dump_html_vkGetShaderModuleIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_html_vkGetShaderModuleCreateInfoIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_html_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
void dump_html_vkCreateOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);
void dump_html_vkDestroyOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator);
void dump_html_vkBindOpticalFlowSessionImageNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);
void dump_html_vkCmdOpticalFlowExecuteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo);
void dump_html_vkCmdBindIndexBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_html_vkGetRenderingAreaGranularityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_html_vkGetDeviceImageSubresourceLayoutKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_html_vkGetImageSubresourceLayout2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_html_vkAntiLagUpdateAMD(ApiDumpInstance& dump_inst, VkDevice device, const VkAntiLagDataAMD* pData);
void dump_html_vkCreateShadersEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);
void dump_html_vkDestroyShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetShaderBinaryDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);
void dump_html_vkCmdBindShadersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders);
void dump_html_vkCmdSetDepthClampRangeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange);
void dump_html_vkCreatePipelineBinariesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries);
void dump_html_vkDestroyPipelineBinaryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetPipelineKeyKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey);
void dump_html_vkGetPipelineBinaryDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData);
void dump_html_vkReleaseCapturedPipelineDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetFramebufferTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);
void dump_html_vkGetDynamicRenderingTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);
void dump_html_vkGetPhysicalDeviceCooperativeVectorPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeVectorPropertiesNV* pProperties);
void dump_html_vkConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkConvertCooperativeVectorMatrixInfoNV* pInfo);
void dump_html_vkCmdConvertCooperativeVectorMatrixNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t infoCount, const VkConvertCooperativeVectorMatrixInfoNV* pInfos);
void dump_html_vkSetLatencySleepModeNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo);
void dump_html_vkLatencySleepNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo);
void dump_html_vkSetLatencyMarkerNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_html_vkGetLatencyTimingsNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_html_vkQueueNotifyOutOfBandNV(ApiDumpInstance& dump_inst, VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);
void dump_html_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
void dump_html_vkCmdSetAttachmentFeedbackLoopEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_html_vkGetScreenBufferPropertiesQNX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_html_vkCmdSetLineStippleKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_html_vkCmdBindDescriptorSets2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_html_vkCmdPushConstants2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_html_vkCmdPushDescriptorSet2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_html_vkCmdPushDescriptorSetWithTemplate2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_html_vkCmdSetDescriptorBufferOffsets2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);
void dump_html_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);
void dump_html_vkCmdBindTileMemoryQCOM(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkTileMemoryBindInfoQCOM* pTileMemoryBindInfo);
void dump_html_vkCreateExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkExternalComputeQueueCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkExternalComputeQueueNV* pExternalQueue);
void dump_html_vkDestroyExternalComputeQueueNV(ApiDumpInstance& dump_inst, VkDevice device, VkExternalComputeQueueNV externalQueue, const VkAllocationCallbacks* pAllocator);
void dump_html_vkGetExternalComputeQueueDataNV(ApiDumpInstance& dump_inst, VkExternalComputeQueueNV externalQueue, VkExternalComputeQueueDataParamsNV* params, void* pData);
void dump_html_vkGetClusterAccelerationStructureBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkClusterAccelerationStructureInputInfoNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);
void dump_html_vkCmdBuildClusterAccelerationStructureIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer                     commandBuffer, const VkClusterAccelerationStructureCommandsInfoNV*  pCommandInfos);
void dump_html_vkGetPartitionedAccelerationStructuresBuildSizesNV(ApiDumpInstance& dump_inst, VkDevice device, const VkPartitionedAccelerationStructureInstancesInputNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR*                  pSizeInfo);
void dump_html_vkCmdBuildPartitionedAccelerationStructuresNV(ApiDumpInstance& dump_inst, VkCommandBuffer                     commandBuffer, const VkBuildPartitionedAccelerationStructureInfoNV*  pBuildInfo);
void dump_html_vkGetGeneratedCommandsMemoryRequirementsEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_html_vkCmdPreprocessGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer);
void dump_html_vkCmdExecuteGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo);
void dump_html_vkCreateIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout);
void dump_html_vkDestroyIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_html_vkCreateIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet);
void dump_html_vkDestroyIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator);
void dump_html_vkUpdateIndirectExecutionSetPipelineEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites);
void dump_html_vkUpdateIndirectExecutionSetShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites);
void dump_html_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_vkGetMemoryMetalHandleEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetMetalHandleInfoEXT* pGetMetalHandleInfo, void** pHandle);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_vkGetMemoryMetalHandlePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHandle, VkMemoryMetalHandlePropertiesEXT* pMemoryMetalHandleProperties);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_html_vkCmdEndRendering2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingEndInfoEXT*        pRenderingEndInfo);


