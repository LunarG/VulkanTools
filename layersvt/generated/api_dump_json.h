
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
 * Author: David Pinedo <david@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#pragma once

#include "api_dump.h"

void dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);


void dump_json_VkClearColorValue(const VkClearColorValue& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkClearValue(const VkClearValue& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterResultKHR(const VkPerformanceCounterResultKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkDeviceOrHostAddressConstAMDX(const VkDeviceOrHostAddressConstAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkDeviceOrHostAddressKHR(const VkDeviceOrHostAddressKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceOrHostAddressConstKHR(const VkDeviceOrHostAddressConstKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryDataKHR(const VkAccelerationStructureGeometryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceValueDataINTEL(const VkPerformanceValueDataINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineExecutableStatisticValueKHR(const VkPipelineExecutableStatisticValueKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorDataEXT(const VkDescriptorDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInstanceDataNV(const VkAccelerationStructureMotionInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetInfoEXT(const VkIndirectExecutionSetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsTokenDataEXT(const VkIndirectCommandsTokenDataEXT& object, const ApiDumpSettings& settings, int indents);

//=========================== Type Implementations ==========================//

void dump_json_uint64_t(uint64_t object, const ApiDumpSettings& settings, int indents);
void dump_json_size_t(size_t object, const ApiDumpSettings& settings, int indents);
void dump_json_char(char object, const ApiDumpSettings& settings, int indents);
void dump_json_float(float object, const ApiDumpSettings& settings, int indents);
void dump_json_int64_t(int64_t object, const ApiDumpSettings& settings, int indents);
void dump_json_double(double object, const ApiDumpSettings& settings, int indents);

//========================= Basetype Implementations ========================//

void dump_json_VkBool32(VkBool32 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceAddress(VkDeviceAddress object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceSize(VkDeviceSize object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFlags(VkFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSampleMask(VkSampleMask object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFlags64(VkFlags64 object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_MTLDevice_id(MTLDevice_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_MTLCommandQueue_id(MTLCommandQueue_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_MTLBuffer_id(MTLBuffer_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_MTLTexture_id(MTLTexture_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_IOSurfaceRef(IOSurfaceRef object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_MTLSharedEvent_id(MTLSharedEvent_id object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_VkRemoteAddressNV(VkRemoteAddressNV object, const ApiDumpSettings& settings, int indents);


#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_ANativeWindow(const ANativeWindow* object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_AHardwareBuffer(const AHardwareBuffer* object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_ANDROID_KHR


#if defined(VK_USE_PLATFORM_METAL_EXT)
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_CAMetalLayer(CAMetalLayer object, const ApiDumpSettings& settings, int indents);
#endif
#endif // VK_USE_PLATFORM_METAL_EXT

//======================= System Type Implementations =======================//

#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_DWORD(const DWORD object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_Display(const Display* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_GgpFrameToken(const GgpFrameToken object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_GgpStreamDescriptor(const GgpStreamDescriptor object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_HANDLE(const HANDLE object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_HINSTANCE(const HINSTANCE object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_HMONITOR(const HMONITOR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_HWND(const HWND object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_IDirectFB(const IDirectFB object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_IDirectFBSurface(const IDirectFBSurface object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_LPCWSTR(const LPCWSTR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_SECURITY_ATTRIBUTES(const SECURITY_ATTRIBUTES* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_VisualID(const VisualID object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_Window(const Window object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json__screen_buffer(const _screen_buffer* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json__screen_context(const _screen_context* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json__screen_window(const _screen_window* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_wl_display(const wl_display* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_wl_surface(const wl_surface* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_xcb_connection_t(const xcb_connection_t* object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_xcb_visualid_t(const xcb_visualid_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_xcb_window_t(const xcb_window_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_zx_handle_t(const zx_handle_t object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA

//========================== Handle Implementations =========================//

void dump_json_VkBuffer(const VkBuffer object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImage(const VkImage object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInstance(const VkInstance object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevice(const VkPhysicalDevice object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDevice(const VkDevice object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueue(const VkQueue object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphore(const VkSemaphore object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBuffer(const VkCommandBuffer object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFence(const VkFence object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceMemory(const VkDeviceMemory object, const ApiDumpSettings& settings, int indents);
void dump_json_VkEvent(const VkEvent object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPool(const VkQueryPool object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferView(const VkBufferView object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageView(const VkImageView object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderModule(const VkShaderModule object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCache(const VkPipelineCache object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineLayout(const VkPipelineLayout object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipeline(const VkPipeline object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPass(const VkRenderPass object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayout(const VkDescriptorSetLayout object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSampler(const VkSampler object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSet(const VkDescriptorSet object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPool(const VkDescriptorPool object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebuffer(const VkFramebuffer object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPool(const VkCommandPool object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrConversion(const VkSamplerYcbcrConversion object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorUpdateTemplate(const VkDescriptorUpdateTemplate object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPrivateDataSlot(const VkPrivateDataSlot object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceKHR(const VkSurfaceKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainKHR(const VkSwapchainKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayKHR(const VkDisplayKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModeKHR(const VkDisplayModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugReportCallbackEXT(const VkDebugReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionKHR(const VkVideoSessionKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionParametersKHR(const VkVideoSessionParametersKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCuModuleNVX(const VkCuModuleNVX object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCuFunctionNVX(const VkCuFunctionNVX object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessengerEXT(const VkDebugUtilsMessengerEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureKHR(const VkAccelerationStructureKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeferredOperationKHR(const VkDeferredOperationKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationCacheEXT(const VkValidationCacheEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureNV(const VkAccelerationStructureNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceConfigurationINTEL(const VkPerformanceConfigurationINTEL object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutNV(const VkIndirectCommandsLayoutNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCudaModuleNV(const VkCudaModuleNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCudaFunctionNV(const VkCudaFunctionNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferCollectionFUCHSIA(const VkBufferCollectionFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_VkMicromapEXT(const VkMicromapEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowSessionNV(const VkOpticalFlowSessionNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderEXT(const VkShaderEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryKHR(const VkPipelineBinaryKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetEXT(const VkIndirectExecutionSetEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutEXT(const VkIndirectCommandsLayoutEXT object, const ApiDumpSettings& settings, int indents);

//=========================== Enum Implementations ==========================//

void dump_json_VkResult(VkResult object, const ApiDumpSettings& settings, int indents);
void dump_json_VkStructureType(VkStructureType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCacheHeaderVersion(VkPipelineCacheHeaderVersion object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageLayout(VkImageLayout object, const ApiDumpSettings& settings, int indents);
void dump_json_VkObjectType(VkObjectType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVendorId(VkVendorId object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSystemAllocationScope(VkSystemAllocationScope object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInternalAllocationType(VkInternalAllocationType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormat(VkFormat object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageTiling(VkImageTiling object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageType(VkImageType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceType(VkPhysicalDeviceType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryType(VkQueryType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSharingMode(VkSharingMode object, const ApiDumpSettings& settings, int indents);
void dump_json_VkComponentSwizzle(VkComponentSwizzle object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewType(VkImageViewType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBlendFactor(VkBlendFactor object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBlendOp(VkBlendOp object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCompareOp(VkCompareOp object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDynamicState(VkDynamicState object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFrontFace(VkFrontFace object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVertexInputRate(VkVertexInputRate object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPrimitiveTopology(VkPrimitiveTopology object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPolygonMode(VkPolygonMode object, const ApiDumpSettings& settings, int indents);
void dump_json_VkStencilOp(VkStencilOp object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLogicOp(VkLogicOp object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBorderColor(VkBorderColor object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFilter(VkFilter object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerAddressMode(VkSamplerAddressMode object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerMipmapMode(VkSamplerMipmapMode object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorType(VkDescriptorType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentLoadOp(VkAttachmentLoadOp object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentStoreOp(VkAttachmentStoreOp object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBindPoint(VkPipelineBindPoint object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferLevel(VkCommandBufferLevel object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndexType(VkIndexType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassContents(VkSubpassContents object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPointClippingBehavior(VkPointClippingBehavior object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTessellationDomainOrigin(VkTessellationDomainOrigin object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrModelConversion(VkSamplerYcbcrModelConversion object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrRange(VkSamplerYcbcrRange object, const ApiDumpSettings& settings, int indents);
void dump_json_VkChromaLocation(VkChromaLocation object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorUpdateTemplateType(VkDescriptorUpdateTemplateType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDriverId(VkDriverId object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderFloatControlsIndependence(VkShaderFloatControlsIndependence object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerReductionMode(VkSamplerReductionMode object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreType(VkSemaphoreType object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRobustnessBufferBehavior(VkPipelineRobustnessBufferBehavior object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRobustnessImageBehavior(VkPipelineRobustnessImageBehavior object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueGlobalPriority(VkQueueGlobalPriority object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLineRasterizationMode(VkLineRasterizationMode object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentModeKHR(VkPresentModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkColorSpaceKHR(VkColorSpaceKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugReportObjectTypeEXT(VkDebugReportObjectTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRasterizationOrderAMD(VkRasterizationOrderAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryResultStatusKHR(VkQueryResultStatusKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderInfoTypeAMD(VkShaderInfoTypeAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationCheckEXT(VkValidationCheckEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPowerStateEXT(VkDisplayPowerStateEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceEventTypeEXT(VkDeviceEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayEventTypeEXT(VkDisplayEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkViewportCoordinateSwizzleNV(VkViewportCoordinateSwizzleNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDiscardRectangleModeEXT(VkDiscardRectangleModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkConservativeRasterizationModeEXT(VkConservativeRasterizationModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterUnitKHR(VkPerformanceCounterUnitKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterScopeKHR(VkPerformanceCounterScopeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterStorageKHR(VkPerformanceCounterStorageKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBlendOverlapEXT(VkBlendOverlapEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureTypeKHR(VkAccelerationStructureTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBuildAccelerationStructureModeKHR(VkBuildAccelerationStructureModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryTypeKHR(VkGeometryTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureBuildTypeKHR(VkAccelerationStructureBuildTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyAccelerationStructureModeKHR(VkCopyAccelerationStructureModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureCompatibilityKHR(VkAccelerationStructureCompatibilityKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCoverageModulationModeNV(VkCoverageModulationModeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationCacheHeaderVersionEXT(VkValidationCacheHeaderVersionEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShadingRatePaletteEntryNV(VkShadingRatePaletteEntryNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCoarseSampleOrderTypeNV(VkCoarseSampleOrderTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRayTracingShaderGroupTypeKHR(VkRayTracingShaderGroupTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMemoryRequirementsTypeNV(VkAccelerationStructureMemoryRequirementsTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTimeDomainKHR(VkTimeDomainKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryOverallocationBehaviorAMD(VkMemoryOverallocationBehaviorAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceConfigurationTypeINTEL(VkPerformanceConfigurationTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPoolSamplingModeINTEL(VkQueryPoolSamplingModeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceOverrideTypeINTEL(VkPerformanceOverrideTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceParameterTypeINTEL(VkPerformanceParameterTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceValueTypeINTEL(VkPerformanceValueTypeINTEL object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFragmentShadingRateCombinerOpKHR(VkFragmentShadingRateCombinerOpKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationFeatureEnableEXT(VkValidationFeatureEnableEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationFeatureDisableEXT(VkValidationFeatureDisableEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkComponentTypeKHR(VkComponentTypeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkScopeKHR(VkScopeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCoverageReductionModeNV(VkCoverageReductionModeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkProvokingVertexModeEXT(VkProvokingVertexModeEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkFullScreenExclusiveEXT(VkFullScreenExclusiveEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkPipelineExecutableStatisticFormatKHR(VkPipelineExecutableStatisticFormatKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsTokenTypeNV(VkIndirectCommandsTokenTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDepthBiasRepresentationEXT(VkDepthBiasRepresentationEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceMemoryReportEventTypeEXT(VkDeviceMemoryReportEventTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeTuningModeKHR(VkVideoEncodeTuningModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFragmentShadingRateTypeNV(VkFragmentShadingRateTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFragmentShadingRateNV(VkFragmentShadingRateNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInstanceTypeNV(VkAccelerationStructureMotionInstanceTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultAddressTypeEXT(VkDeviceFaultAddressTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultVendorBinaryHeaderVersionEXT(VkDeviceFaultVendorBinaryHeaderVersionEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderGroupShaderKHR(VkShaderGroupShaderKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceAddressBindingTypeEXT(VkDeviceAddressBindingTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapTypeEXT(VkMicromapTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBuildMicromapModeEXT(VkBuildMicromapModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMicromapModeEXT(VkCopyMicromapModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpacityMicromapFormatEXT(VkOpacityMicromapFormatEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpacityMicromapSpecialIndexEXT(VkOpacityMicromapSpecialIndexEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkDisplacementMicromapFormatNV(VkDisplacementMicromapFormatNV object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkSubpassMergeStatusEXT(VkSubpassMergeStatusEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDirectDriverLoadingModeLUNARG(VkDirectDriverLoadingModeLUNARG object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowPerformanceLevelNV(VkOpticalFlowPerformanceLevelNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowSessionBindingPointNV(VkOpticalFlowSessionBindingPointNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAntiLagModeAMD(VkAntiLagModeAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAntiLagStageAMD(VkAntiLagStageAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderCodeTypeEXT(VkShaderCodeTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDepthClampModeEXT(VkDepthClampModeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRayTracingInvocationReorderModeNV(VkRayTracingInvocationReorderModeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLayerSettingTypeEXT(VkLayerSettingTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLatencyMarkerNV(VkLatencyMarkerNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOutOfBandQueueTypeNV(VkOutOfBandQueueTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1PredictionModeKHR(VkVideoEncodeAV1PredictionModeKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1RateControlGroupKHR(VkVideoEncodeAV1RateControlGroupKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBlockMatchWindowCompareModeQCOM(VkBlockMatchWindowCompareModeQCOM object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCubicFilterWeightsQCOM(VkCubicFilterWeightsQCOM object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLayeredDriverUnderlyingApiMSFT(VkLayeredDriverUnderlyingApiMSFT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplaySurfaceStereoTypeNV(VkDisplaySurfaceStereoTypeNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLayeredApiKHR(VkPhysicalDeviceLayeredApiKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetInfoTypeEXT(VkIndirectExecutionSetInfoTypeEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsTokenTypeEXT(VkIndirectCommandsTokenTypeEXT object, const ApiDumpSettings& settings, int indents);

//========================= Bitmask Implementations =========================//

void dump_json_VkAccessFlagBits(VkAccessFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageAspectFlagBits(VkImageAspectFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatFeatureFlagBits(VkFormatFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCreateFlagBits(VkImageCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSampleCountFlagBits(VkSampleCountFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageUsageFlagBits(VkImageUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInstanceCreateFlagBits(VkInstanceCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryHeapFlagBits(VkMemoryHeapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryPropertyFlagBits(VkMemoryPropertyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFlagBits(VkQueueFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceQueueCreateFlagBits(VkDeviceQueueCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineStageFlagBits(VkPipelineStageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryMapFlagBits(VkMemoryMapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseMemoryBindFlagBits(VkSparseMemoryBindFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageFormatFlagBits(VkSparseImageFormatFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFenceCreateFlagBits(VkFenceCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkEventCreateFlagBits(VkEventCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPipelineStatisticFlagBits(VkQueryPipelineStatisticFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryResultFlagBits(VkQueryResultFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferCreateFlagBits(VkBufferCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferUsageFlagBits(VkBufferUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewCreateFlagBits(VkImageViewCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCacheCreateFlagBits(VkPipelineCacheCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkColorComponentFlagBits(VkColorComponentFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreateFlagBits(VkPipelineCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineShaderStageCreateFlagBits(VkPipelineShaderStageCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderStageFlagBits(VkShaderStageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCullModeFlagBits(VkCullModeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDepthStencilStateCreateFlagBits(VkPipelineDepthStencilStateCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineColorBlendStateCreateFlagBits(VkPipelineColorBlendStateCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineLayoutCreateFlagBits(VkPipelineLayoutCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerCreateFlagBits(VkSamplerCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPoolCreateFlagBits(VkDescriptorPoolCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutCreateFlagBits(VkDescriptorSetLayoutCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentDescriptionFlagBits(VkAttachmentDescriptionFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDependencyFlagBits(VkDependencyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebufferCreateFlagBits(VkFramebufferCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreateFlagBits(VkRenderPassCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDescriptionFlagBits(VkSubpassDescriptionFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPoolCreateFlagBits(VkCommandPoolCreateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPoolResetFlagBits(VkCommandPoolResetFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferUsageFlagBits(VkCommandBufferUsageFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryControlFlagBits(VkQueryControlFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferResetFlagBits(VkCommandBufferResetFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkStencilFaceFlagBits(VkStencilFaceFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubgroupFeatureFlagBits(VkSubgroupFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPeerMemoryFeatureFlagBits(VkPeerMemoryFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryAllocateFlagBits(VkMemoryAllocateFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryHandleTypeFlagBits(VkExternalMemoryHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryFeatureFlagBits(VkExternalMemoryFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalFenceHandleTypeFlagBits(VkExternalFenceHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalFenceFeatureFlagBits(VkExternalFenceFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFenceImportFlagBits(VkFenceImportFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreImportFlagBits(VkSemaphoreImportFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalSemaphoreHandleTypeFlagBits(VkExternalSemaphoreHandleTypeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalSemaphoreFeatureFlagBits(VkExternalSemaphoreFeatureFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkResolveModeFlagBits(VkResolveModeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorBindingFlagBits(VkDescriptorBindingFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreWaitFlagBits(VkSemaphoreWaitFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreationFeedbackFlagBits(VkPipelineCreationFeedbackFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkToolPurposeFlagBits(VkToolPurposeFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineStageFlagBits2(VkPipelineStageFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccessFlagBits2(VkAccessFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubmitFlagBits(VkSubmitFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingFlagBits(VkRenderingFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatFeatureFlagBits2(VkFormatFeatureFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryUnmapFlagBits(VkMemoryUnmapFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreateFlagBits2(VkPipelineCreateFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferUsageFlagBits2(VkBufferUsageFlagBits2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHostImageCopyFlagBits(VkHostImageCopyFlagBits object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceTransformFlagBitsKHR(VkSurfaceTransformFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCompositeAlphaFlagBitsKHR(VkCompositeAlphaFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainCreateFlagBitsKHR(VkSwapchainCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupPresentModeFlagBitsKHR(VkDeviceGroupPresentModeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlaneAlphaFlagBitsKHR(VkDisplayPlaneAlphaFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugReportFlagBitsEXT(VkDebugReportFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCodecOperationFlagBitsKHR(VkVideoCodecOperationFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoChromaSubsamplingFlagBitsKHR(VkVideoChromaSubsamplingFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoComponentBitDepthFlagBitsKHR(VkVideoComponentBitDepthFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCapabilityFlagBitsKHR(VkVideoCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionCreateFlagBitsKHR(VkVideoSessionCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionParametersCreateFlagBitsKHR(VkVideoSessionParametersCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCodingControlFlagBitsKHR(VkVideoCodingControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeCapabilityFlagBitsKHR(VkVideoDecodeCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeUsageFlagBitsKHR(VkVideoDecodeUsageFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264CapabilityFlagBitsKHR(VkVideoEncodeH264CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264StdFlagBitsKHR(VkVideoEncodeH264StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264RateControlFlagBitsKHR(VkVideoEncodeH264RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265CapabilityFlagBitsKHR(VkVideoEncodeH265CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265StdFlagBitsKHR(VkVideoEncodeH265StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265CtbSizeFlagBitsKHR(VkVideoEncodeH265CtbSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265TransformBlockSizeFlagBitsKHR(VkVideoEncodeH265TransformBlockSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265RateControlFlagBitsKHR(VkVideoEncodeH265RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264PictureLayoutFlagBitsKHR(VkVideoDecodeH264PictureLayoutFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryHandleTypeFlagBitsNV(VkExternalMemoryHandleTypeFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryFeatureFlagBitsNV(VkExternalMemoryFeatureFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkConditionalRenderingFlagBitsEXT(VkConditionalRenderingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceCounterFlagBitsEXT(VkSurfaceCounterFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterDescriptionFlagBitsKHR(VkPerformanceCounterDescriptionFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAcquireProfilingLockFlagBitsKHR(VkAcquireProfilingLockFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessageSeverityFlagBitsEXT(VkDebugUtilsMessageSeverityFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessageTypeFlagBitsEXT(VkDebugUtilsMessageTypeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBuildAccelerationStructureFlagBitsKHR(VkBuildAccelerationStructureFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryFlagBitsKHR(VkGeometryFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryInstanceFlagBitsKHR(VkGeometryInstanceFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureCreateFlagBitsKHR(VkAccelerationStructureCreateFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCompilerControlFlagBitsAMD(VkPipelineCompilerControlFlagBitsAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderCorePropertiesFlagBitsAMD(VkShaderCorePropertiesFlagBitsAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentScalingFlagBitsEXT(VkPresentScalingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentGravityFlagBitsEXT(VkPresentGravityFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectStateFlagBitsNV(VkIndirectStateFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutUsageFlagBitsNV(VkIndirectCommandsLayoutUsageFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeFlagBitsKHR(VkVideoEncodeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeCapabilityFlagBitsKHR(VkVideoEncodeCapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeRateControlModeFlagBitsKHR(VkVideoEncodeRateControlModeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeFeedbackFlagBitsKHR(VkVideoEncodeFeedbackFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeUsageFlagBitsKHR(VkVideoEncodeUsageFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeContentFlagBitsKHR(VkVideoEncodeContentFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceDiagnosticsConfigFlagBitsNV(VkDeviceDiagnosticsConfigFlagBitsNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalObjectTypeFlagBitsEXT(VkExportMetalObjectTypeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_VkGraphicsPipelineLibraryFlagBitsEXT(VkGraphicsPipelineLibraryFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCompressionFlagBitsEXT(VkImageCompressionFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCompressionFixedRateFlagBitsEXT(VkImageCompressionFixedRateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceAddressBindingFlagBitsEXT(VkDeviceAddressBindingFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImageConstraintsInfoFlagBitsFUCHSIA(VkImageConstraintsInfoFlagBitsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_VkFrameBoundaryFlagBitsEXT(VkFrameBoundaryFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBuildMicromapFlagBitsEXT(VkBuildMicromapFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapCreateFlagBitsEXT(VkMicromapCreateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSchedulingControlsFlagBitsARM(VkPhysicalDeviceSchedulingControlsFlagBitsARM object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryDecompressionMethodFlagBitsNV(VkMemoryDecompressionMethodFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowGridSizeFlagBitsNV(VkOpticalFlowGridSizeFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowUsageFlagBitsNV(VkOpticalFlowUsageFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowSessionCreateFlagBitsNV(VkOpticalFlowSessionCreateFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowExecuteFlagBitsNV(VkOpticalFlowExecuteFlagBitsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderCreateFlagBitsEXT(VkShaderCreateFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1CapabilityFlagBitsKHR(VkVideoEncodeAV1CapabilityFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1StdFlagBitsKHR(VkVideoEncodeAV1StdFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1SuperblockSizeFlagBitsKHR(VkVideoEncodeAV1SuperblockSizeFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1RateControlFlagBitsKHR(VkVideoEncodeAV1RateControlFlagBitsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsInputModeFlagBitsEXT(VkIndirectCommandsInputModeFlagBitsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutUsageFlagBitsEXT(VkIndirectCommandsLayoutUsageFlagBitsEXT object, const ApiDumpSettings& settings, int indents);

//=========================== Flag Implementations ==========================//

void dump_json_VkAccessFlags(VkAccessFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageAspectFlags(VkImageAspectFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatFeatureFlags(VkFormatFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCreateFlags(VkImageCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSampleCountFlags(VkSampleCountFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageUsageFlags(VkImageUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInstanceCreateFlags(VkInstanceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryHeapFlags(VkMemoryHeapFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryPropertyFlags(VkMemoryPropertyFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFlags(VkQueueFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceQueueCreateFlags(VkDeviceQueueCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineStageFlags(VkPipelineStageFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryMapFlags(VkMemoryMapFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseMemoryBindFlags(VkSparseMemoryBindFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageFormatFlags(VkSparseImageFormatFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFenceCreateFlags(VkFenceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkEventCreateFlags(VkEventCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPipelineStatisticFlags(VkQueryPipelineStatisticFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryResultFlags(VkQueryResultFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferCreateFlags(VkBufferCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferUsageFlags(VkBufferUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewCreateFlags(VkImageViewCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCacheCreateFlags(VkPipelineCacheCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkColorComponentFlags(VkColorComponentFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreateFlags(VkPipelineCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineShaderStageCreateFlags(VkPipelineShaderStageCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCullModeFlags(VkCullModeFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDepthStencilStateCreateFlags(VkPipelineDepthStencilStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineColorBlendStateCreateFlags(VkPipelineColorBlendStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineLayoutCreateFlags(VkPipelineLayoutCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderStageFlags(VkShaderStageFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerCreateFlags(VkSamplerCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPoolCreateFlags(VkDescriptorPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutCreateFlags(VkDescriptorSetLayoutCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentDescriptionFlags(VkAttachmentDescriptionFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDependencyFlags(VkDependencyFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebufferCreateFlags(VkFramebufferCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreateFlags(VkRenderPassCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDescriptionFlags(VkSubpassDescriptionFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPoolCreateFlags(VkCommandPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPoolResetFlags(VkCommandPoolResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferUsageFlags(VkCommandBufferUsageFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryControlFlags(VkQueryControlFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferResetFlags(VkCommandBufferResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkStencilFaceFlags(VkStencilFaceFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubgroupFeatureFlags(VkSubgroupFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPeerMemoryFeatureFlags(VkPeerMemoryFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryAllocateFlags(VkMemoryAllocateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryHandleTypeFlags(VkExternalMemoryHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryFeatureFlags(VkExternalMemoryFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalFenceHandleTypeFlags(VkExternalFenceHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalFenceFeatureFlags(VkExternalFenceFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFenceImportFlags(VkFenceImportFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreImportFlags(VkSemaphoreImportFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalSemaphoreHandleTypeFlags(VkExternalSemaphoreHandleTypeFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalSemaphoreFeatureFlags(VkExternalSemaphoreFeatureFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkResolveModeFlags(VkResolveModeFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorBindingFlags(VkDescriptorBindingFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreWaitFlags(VkSemaphoreWaitFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreationFeedbackFlags(VkPipelineCreationFeedbackFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkToolPurposeFlags(VkToolPurposeFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineStageFlags2(VkPipelineStageFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccessFlags2(VkAccessFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubmitFlags(VkSubmitFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingFlags(VkRenderingFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatFeatureFlags2(VkFormatFeatureFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryUnmapFlags(VkMemoryUnmapFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreateFlags2(VkPipelineCreateFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferUsageFlags2(VkBufferUsageFlags2 object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHostImageCopyFlags(VkHostImageCopyFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCompositeAlphaFlagsKHR(VkCompositeAlphaFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceTransformFlagsKHR(VkSurfaceTransformFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainCreateFlagsKHR(VkSwapchainCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupPresentModeFlagsKHR(VkDeviceGroupPresentModeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlaneAlphaFlagsKHR(VkDisplayPlaneAlphaFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugReportFlagsEXT(VkDebugReportFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCodecOperationFlagsKHR(VkVideoCodecOperationFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoChromaSubsamplingFlagsKHR(VkVideoChromaSubsamplingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoComponentBitDepthFlagsKHR(VkVideoComponentBitDepthFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCapabilityFlagsKHR(VkVideoCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionCreateFlagsKHR(VkVideoSessionCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionParametersCreateFlagsKHR(VkVideoSessionParametersCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCodingControlFlagsKHR(VkVideoCodingControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeCapabilityFlagsKHR(VkVideoDecodeCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeUsageFlagsKHR(VkVideoDecodeUsageFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264CapabilityFlagsKHR(VkVideoEncodeH264CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264StdFlagsKHR(VkVideoEncodeH264StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264RateControlFlagsKHR(VkVideoEncodeH264RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265CapabilityFlagsKHR(VkVideoEncodeH265CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265StdFlagsKHR(VkVideoEncodeH265StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265CtbSizeFlagsKHR(VkVideoEncodeH265CtbSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265TransformBlockSizeFlagsKHR(VkVideoEncodeH265TransformBlockSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265RateControlFlagsKHR(VkVideoEncodeH265RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264PictureLayoutFlagsKHR(VkVideoDecodeH264PictureLayoutFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryHandleTypeFlagsNV(VkExternalMemoryHandleTypeFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryFeatureFlagsNV(VkExternalMemoryFeatureFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkConditionalRenderingFlagsEXT(VkConditionalRenderingFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceCounterFlagsEXT(VkSurfaceCounterFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterDescriptionFlagsKHR(VkPerformanceCounterDescriptionFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAcquireProfilingLockFlagsKHR(VkAcquireProfilingLockFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessageTypeFlagsEXT(VkDebugUtilsMessageTypeFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessageSeverityFlagsEXT(VkDebugUtilsMessageSeverityFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBuildAccelerationStructureFlagsKHR(VkBuildAccelerationStructureFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryFlagsKHR(VkGeometryFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryInstanceFlagsKHR(VkGeometryInstanceFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureCreateFlagsKHR(VkAccelerationStructureCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCompilerControlFlagsAMD(VkPipelineCompilerControlFlagsAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderCorePropertiesFlagsAMD(VkShaderCorePropertiesFlagsAMD object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentScalingFlagsEXT(VkPresentScalingFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentGravityFlagsEXT(VkPresentGravityFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectStateFlagsNV(VkIndirectStateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutUsageFlagsNV(VkIndirectCommandsLayoutUsageFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeFlagsKHR(VkVideoEncodeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeCapabilityFlagsKHR(VkVideoEncodeCapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeRateControlModeFlagsKHR(VkVideoEncodeRateControlModeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeFeedbackFlagsKHR(VkVideoEncodeFeedbackFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeUsageFlagsKHR(VkVideoEncodeUsageFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeContentFlagsKHR(VkVideoEncodeContentFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceDiagnosticsConfigFlagsNV(VkDeviceDiagnosticsConfigFlagsNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalObjectTypeFlagsEXT(VkExportMetalObjectTypeFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_VkGraphicsPipelineLibraryFlagsEXT(VkGraphicsPipelineLibraryFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCompressionFlagsEXT(VkImageCompressionFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCompressionFixedRateFlagsEXT(VkImageCompressionFixedRateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceAddressBindingFlagsEXT(VkDeviceAddressBindingFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImageConstraintsInfoFlagsFUCHSIA(VkImageConstraintsInfoFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_VkFrameBoundaryFlagsEXT(VkFrameBoundaryFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBuildMicromapFlagsEXT(VkBuildMicromapFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapCreateFlagsEXT(VkMicromapCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSchedulingControlsFlagsARM(VkPhysicalDeviceSchedulingControlsFlagsARM object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryDecompressionMethodFlagsNV(VkMemoryDecompressionMethodFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowGridSizeFlagsNV(VkOpticalFlowGridSizeFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowUsageFlagsNV(VkOpticalFlowUsageFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowSessionCreateFlagsNV(VkOpticalFlowSessionCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowExecuteFlagsNV(VkOpticalFlowExecuteFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderCreateFlagsEXT(VkShaderCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1CapabilityFlagsKHR(VkVideoEncodeAV1CapabilityFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1StdFlagsKHR(VkVideoEncodeAV1StdFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1SuperblockSizeFlagsKHR(VkVideoEncodeAV1SuperblockSizeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1RateControlFlagsKHR(VkVideoEncodeAV1RateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsInputModeFlagsEXT(VkIndirectCommandsInputModeFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutUsageFlagsEXT(VkIndirectCommandsLayoutUsageFlagsEXT object, const ApiDumpSettings& settings, int indents);


void dump_json_VkDeviceCreateFlags(VkDeviceCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreCreateFlags(VkSemaphoreCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPoolCreateFlags(VkQueryPoolCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferViewCreateFlags(VkBufferViewCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderModuleCreateFlags(VkShaderModuleCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineVertexInputStateCreateFlags(VkPipelineVertexInputStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineInputAssemblyStateCreateFlags(VkPipelineInputAssemblyStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineTessellationStateCreateFlags(VkPipelineTessellationStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportStateCreateFlags(VkPipelineViewportStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationStateCreateFlags(VkPipelineRasterizationStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineMultisampleStateCreateFlags(VkPipelineMultisampleStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDynamicStateCreateFlags(VkPipelineDynamicStateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPoolResetFlags(VkDescriptorPoolResetFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPoolTrimFlags(VkCommandPoolTrimFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorUpdateTemplateCreateFlags(VkDescriptorUpdateTemplateCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPrivateDataSlotCreateFlags(VkPrivateDataSlotCreateFlags object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModeCreateFlagsKHR(VkDisplayModeCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplaySurfaceCreateFlagsKHR(VkDisplaySurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_VkXlibSurfaceCreateFlagsKHR(VkXlibSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_VkXcbSurfaceCreateFlagsKHR(VkXcbSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_VkWaylandSurfaceCreateFlagsKHR(VkWaylandSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidSurfaceCreateFlagsKHR(VkAndroidSurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkWin32SurfaceCreateFlagsKHR(VkWin32SurfaceCreateFlagsKHR object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkVideoBeginCodingFlagsKHR(VkVideoBeginCodingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEndCodingFlagsKHR(VkVideoEndCodingFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeFlagsKHR(VkVideoDecodeFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationStateStreamCreateFlagsEXT(VkPipelineRasterizationStateStreamCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_VkStreamDescriptorSurfaceCreateFlagsGGP(VkStreamDescriptorSurfaceCreateFlagsGGP object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_json_VkViSurfaceCreateFlagsNN(VkViSurfaceCreateFlagsNN object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_VI_NN
void dump_json_VkPipelineViewportSwizzleStateCreateFlagsNV(VkPipelineViewportSwizzleStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDiscardRectangleStateCreateFlagsEXT(VkPipelineDiscardRectangleStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationConservativeStateCreateFlagsEXT(VkPipelineRasterizationConservativeStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationDepthClipStateCreateFlagsEXT(VkPipelineRasterizationDepthClipStateCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_json_VkIOSSurfaceCreateFlagsMVK(VkIOSSurfaceCreateFlagsMVK object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_json_VkMacOSSurfaceCreateFlagsMVK(VkMacOSSurfaceCreateFlagsMVK object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_json_VkDebugUtilsMessengerCallbackDataFlagsEXT(VkDebugUtilsMessengerCallbackDataFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessengerCreateFlagsEXT(VkDebugUtilsMessengerCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCoverageToColorStateCreateFlagsNV(VkPipelineCoverageToColorStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCoverageModulationStateCreateFlagsNV(VkPipelineCoverageModulationStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationCacheCreateFlagsEXT(VkValidationCacheCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImagePipeSurfaceCreateFlagsFUCHSIA(VkImagePipeSurfaceCreateFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkMetalSurfaceCreateFlagsEXT(VkMetalSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_VkPipelineCoverageReductionStateCreateFlagsNV(VkPipelineCoverageReductionStateCreateFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHeadlessSurfaceCreateFlagsEXT(VkHeadlessSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceMemoryReportFlagsEXT(VkDeviceMemoryReportFlagsEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeRateControlFlagsKHR(VkVideoEncodeRateControlFlagsKHR object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInfoFlagsNV(VkAccelerationStructureMotionInfoFlagsNV object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInstanceFlagsNV(VkAccelerationStructureMotionInstanceFlagsNV object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_VkDirectFBSurfaceCreateFlagsEXT(VkDirectFBSurfaceCreateFlagsEXT object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImageFormatConstraintsFlagsFUCHSIA(VkImageFormatConstraintsFlagsFUCHSIA object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkScreenSurfaceCreateFlagsQNX(VkScreenSurfaceCreateFlagsQNX object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_VkDirectDriverLoadingFlagsLUNARG(VkDirectDriverLoadingFlagsLUNARG object, const ApiDumpSettings& settings, int indents);

//======================= Func Pointer Implementations ======================//

void dump_json_PFN_vkAllocationFunction(PFN_vkAllocationFunction object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkFreeFunction(PFN_vkFreeFunction object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkInternalAllocationNotification(PFN_vkInternalAllocationNotification object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkInternalFreeNotification(PFN_vkInternalFreeNotification object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkReallocationFunction(PFN_vkReallocationFunction object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkVoidFunction(PFN_vkVoidFunction object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkDebugReportCallbackEXT(PFN_vkDebugReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkDebugUtilsMessengerCallbackEXT(PFN_vkDebugUtilsMessengerCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkDeviceMemoryReportCallbackEXT(PFN_vkDeviceMemoryReportCallbackEXT object, const ApiDumpSettings& settings, int indents);
void dump_json_PFN_vkGetInstanceProcAddrLUNARG(PFN_vkGetInstanceProcAddrLUNARG object, const ApiDumpSettings& settings, int indents);

//========================== Struct Implementations =========================//

void dump_json_VkExtent2D(const VkExtent2D& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExtent3D(const VkExtent3D& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOffset2D(const VkOffset2D& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOffset3D(const VkOffset3D& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRect2D(const VkRect2D& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBaseInStructure(const VkBaseInStructure& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBaseOutStructure(const VkBaseOutStructure& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferMemoryBarrier(const VkBufferMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDispatchIndirectCommand(const VkDispatchIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrawIndexedIndirectCommand(const VkDrawIndexedIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrawIndirectCommand(const VkDrawIndirectCommand& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageSubresourceRange(const VkImageSubresourceRange& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageMemoryBarrier(const VkImageMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryBarrier(const VkMemoryBarrier& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCacheHeaderVersionOne(const VkPipelineCacheHeaderVersionOne& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAllocationCallbacks(const VkAllocationCallbacks& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkApplicationInfo(const VkApplicationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatProperties(const VkFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageFormatProperties(const VkImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInstanceCreateInfo(const VkInstanceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryHeap(const VkMemoryHeap& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryType(const VkMemoryType& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLimits(const VkPhysicalDeviceLimits& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSparseProperties(const VkPhysicalDeviceSparseProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceProperties(const VkPhysicalDeviceProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyProperties(const VkQueueFamilyProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceQueueCreateInfo(const VkDeviceQueueCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceCreateInfo(const VkDeviceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExtensionProperties(const VkExtensionProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLayerProperties(const VkLayerProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubmitInfo(const VkSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMappedMemoryRange(const VkMappedMemoryRange& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryAllocateInfo(const VkMemoryAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryRequirements(const VkMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseMemoryBind(const VkSparseMemoryBind& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseBufferMemoryBindInfo(const VkSparseBufferMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageOpaqueMemoryBindInfo(const VkSparseImageOpaqueMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageSubresource(const VkImageSubresource& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageMemoryBind(const VkSparseImageMemoryBind& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageMemoryBindInfo(const VkSparseImageMemoryBindInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindSparseInfo(const VkBindSparseInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageFormatProperties(const VkSparseImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageMemoryRequirements(const VkSparseImageMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFenceCreateInfo(const VkFenceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreCreateInfo(const VkSemaphoreCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkEventCreateInfo(const VkEventCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPoolCreateInfo(const VkQueryPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferCreateInfo(const VkBufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferViewCreateInfo(const VkBufferViewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCreateInfo(const VkImageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubresourceLayout(const VkSubresourceLayout& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkComponentMapping(const VkComponentMapping& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewCreateInfo(const VkImageViewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderModuleCreateInfo(const VkShaderModuleCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCacheCreateInfo(const VkPipelineCacheCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSpecializationMapEntry(const VkSpecializationMapEntry& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSpecializationInfo(const VkSpecializationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkComputePipelineCreateInfo(const VkComputePipelineCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVertexInputBindingDescription(const VkVertexInputBindingDescription& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVertexInputAttributeDescription(const VkVertexInputAttributeDescription& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineTessellationStateCreateInfo(const VkPipelineTessellationStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkViewport(const VkViewport& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkStencilOpState(const VkStencilOpState& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPushConstantRange(const VkPushConstantRange& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerCreateInfo(const VkSamplerCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyDescriptorSet(const VkCopyDescriptorSet& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorBufferInfo(const VkDescriptorBufferInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorImageInfo(const VkDescriptorImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPoolSize(const VkDescriptorPoolSize& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPoolCreateInfo(const VkDescriptorPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetAllocateInfo(const VkDescriptorSetAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkWriteDescriptorSet(const VkWriteDescriptorSet& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentDescription(const VkAttachmentDescription& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentReference(const VkAttachmentReference& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebufferCreateInfo(const VkFramebufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDescription(const VkSubpassDescription& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDependency(const VkSubpassDependency& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreateInfo(const VkRenderPassCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandPoolCreateInfo(const VkCommandPoolCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferAllocateInfo(const VkCommandBufferAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferInheritanceInfo(const VkCommandBufferInheritanceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferBeginInfo(const VkCommandBufferBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferCopy(const VkBufferCopy& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageSubresourceLayers(const VkImageSubresourceLayers& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferImageCopy(const VkBufferImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkClearDepthStencilValue(const VkClearDepthStencilValue& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkClearAttachment(const VkClearAttachment& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkClearRect(const VkClearRect& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageBlit(const VkImageBlit& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCopy(const VkImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageResolve(const VkImageResolve& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassBeginInfo(const VkRenderPassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSubgroupProperties(const VkPhysicalDeviceSubgroupProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindBufferMemoryInfo(const VkBindBufferMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindImageMemoryInfo(const VkBindImageMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevice16BitStorageFeatures(const VkPhysicalDevice16BitStorageFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryDedicatedRequirements(const VkMemoryDedicatedRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryDedicatedAllocateInfo(const VkMemoryDedicatedAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryAllocateFlagsInfo(const VkMemoryAllocateFlagsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupRenderPassBeginInfo(const VkDeviceGroupRenderPassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupCommandBufferBeginInfo(const VkDeviceGroupCommandBufferBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupSubmitInfo(const VkDeviceGroupSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupBindSparseInfo(const VkDeviceGroupBindSparseInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindBufferMemoryDeviceGroupInfo(const VkBindBufferMemoryDeviceGroupInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindImageMemoryDeviceGroupInfo(const VkBindImageMemoryDeviceGroupInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupDeviceCreateInfo(const VkDeviceGroupDeviceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferMemoryRequirementsInfo2(const VkBufferMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageMemoryRequirementsInfo2(const VkImageMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageSparseMemoryRequirementsInfo2(const VkImageSparseMemoryRequirementsInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryRequirements2(const VkMemoryRequirements2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageMemoryRequirements2(const VkSparseImageMemoryRequirements2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFeatures2(const VkPhysicalDeviceFeatures2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceProperties2(const VkPhysicalDeviceProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatProperties2(const VkFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageFormatProperties2(const VkImageFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageFormatInfo2(const VkPhysicalDeviceImageFormatInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyProperties2(const VkQueueFamilyProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMemoryProperties2(const VkPhysicalDeviceMemoryProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSparseImageFormatProperties2(const VkSparseImageFormatProperties2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSparseImageFormatInfo2(const VkPhysicalDeviceSparseImageFormatInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePointClippingProperties(const VkPhysicalDevicePointClippingProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInputAttachmentAspectReference(const VkInputAttachmentAspectReference& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassInputAttachmentAspectCreateInfo(const VkRenderPassInputAttachmentAspectCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewUsageCreateInfo(const VkImageViewUsageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineTessellationDomainOriginStateCreateInfo(const VkPipelineTessellationDomainOriginStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassMultiviewCreateInfo(const VkRenderPassMultiviewCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiviewFeatures(const VkPhysicalDeviceMultiviewFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiviewProperties(const VkPhysicalDeviceMultiviewProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVariablePointersFeatures(const VkPhysicalDeviceVariablePointersFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceProtectedMemoryFeatures(const VkPhysicalDeviceProtectedMemoryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceProtectedMemoryProperties(const VkPhysicalDeviceProtectedMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceQueueInfo2(const VkDeviceQueueInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkProtectedSubmitInfo(const VkProtectedSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrConversionCreateInfo(const VkSamplerYcbcrConversionCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrConversionInfo(const VkSamplerYcbcrConversionInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindImagePlaneMemoryInfo(const VkBindImagePlaneMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImagePlaneMemoryRequirementsInfo(const VkImagePlaneMemoryRequirementsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSamplerYcbcrConversionFeatures(const VkPhysicalDeviceSamplerYcbcrConversionFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrConversionImageFormatProperties(const VkSamplerYcbcrConversionImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorUpdateTemplateEntry(const VkDescriptorUpdateTemplateEntry& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorUpdateTemplateCreateInfo(const VkDescriptorUpdateTemplateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryProperties(const VkExternalMemoryProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExternalImageFormatInfo(const VkPhysicalDeviceExternalImageFormatInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalImageFormatProperties(const VkExternalImageFormatProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExternalBufferInfo(const VkPhysicalDeviceExternalBufferInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalBufferProperties(const VkExternalBufferProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceIDProperties(const VkPhysicalDeviceIDProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryImageCreateInfo(const VkExternalMemoryImageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryBufferCreateInfo(const VkExternalMemoryBufferCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExportMemoryAllocateInfo(const VkExportMemoryAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExternalFenceInfo(const VkPhysicalDeviceExternalFenceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalFenceProperties(const VkExternalFenceProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExportFenceCreateInfo(const VkExportFenceCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExportSemaphoreCreateInfo(const VkExportSemaphoreCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExternalSemaphoreInfo(const VkPhysicalDeviceExternalSemaphoreInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalSemaphoreProperties(const VkExternalSemaphoreProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance3Properties(const VkPhysicalDeviceMaintenance3Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutSupport(const VkDescriptorSetLayoutSupport& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderDrawParametersFeatures(const VkPhysicalDeviceShaderDrawParametersFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan11Features(const VkPhysicalDeviceVulkan11Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan11Properties(const VkPhysicalDeviceVulkan11Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan12Features(const VkPhysicalDeviceVulkan12Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkConformanceVersion(const VkConformanceVersion& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan12Properties(const VkPhysicalDeviceVulkan12Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageFormatListCreateInfo(const VkImageFormatListCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentDescription2(const VkAttachmentDescription2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentReference2(const VkAttachmentReference2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDescription2(const VkSubpassDescription2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDependency2(const VkSubpassDependency2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreateInfo2(const VkRenderPassCreateInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassBeginInfo(const VkSubpassBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassEndInfo(const VkSubpassEndInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevice8BitStorageFeatures(const VkPhysicalDevice8BitStorageFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDriverProperties(const VkPhysicalDeviceDriverProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderAtomicInt64Features(const VkPhysicalDeviceShaderAtomicInt64Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderFloat16Int8Features(const VkPhysicalDeviceShaderFloat16Int8Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFloatControlsProperties(const VkPhysicalDeviceFloatControlsProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutBindingFlagsCreateInfo(const VkDescriptorSetLayoutBindingFlagsCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDescriptorIndexingFeatures(const VkPhysicalDeviceDescriptorIndexingFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDescriptorIndexingProperties(const VkPhysicalDeviceDescriptorIndexingProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetVariableDescriptorCountAllocateInfo(const VkDescriptorSetVariableDescriptorCountAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetVariableDescriptorCountLayoutSupport(const VkDescriptorSetVariableDescriptorCountLayoutSupport& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassDescriptionDepthStencilResolve(const VkSubpassDescriptionDepthStencilResolve& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDepthStencilResolveProperties(const VkPhysicalDeviceDepthStencilResolveProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceScalarBlockLayoutFeatures(const VkPhysicalDeviceScalarBlockLayoutFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageStencilUsageCreateInfo(const VkImageStencilUsageCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerReductionModeCreateInfo(const VkSamplerReductionModeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSamplerFilterMinmaxProperties(const VkPhysicalDeviceSamplerFilterMinmaxProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkanMemoryModelFeatures(const VkPhysicalDeviceVulkanMemoryModelFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImagelessFramebufferFeatures(const VkPhysicalDeviceImagelessFramebufferFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebufferAttachmentImageInfo(const VkFramebufferAttachmentImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebufferAttachmentsCreateInfo(const VkFramebufferAttachmentsCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassAttachmentBeginInfo(const VkRenderPassAttachmentBeginInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceUniformBufferStandardLayoutFeatures(const VkPhysicalDeviceUniformBufferStandardLayoutFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures(const VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures(const VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentReferenceStencilLayout(const VkAttachmentReferenceStencilLayout& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentDescriptionStencilLayout(const VkAttachmentDescriptionStencilLayout& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceHostQueryResetFeatures(const VkPhysicalDeviceHostQueryResetFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTimelineSemaphoreFeatures(const VkPhysicalDeviceTimelineSemaphoreFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTimelineSemaphoreProperties(const VkPhysicalDeviceTimelineSemaphoreProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreTypeCreateInfo(const VkSemaphoreTypeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTimelineSemaphoreSubmitInfo(const VkTimelineSemaphoreSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreWaitInfo(const VkSemaphoreWaitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreSignalInfo(const VkSemaphoreSignalInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceBufferDeviceAddressFeatures(const VkPhysicalDeviceBufferDeviceAddressFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferDeviceAddressInfo(const VkBufferDeviceAddressInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferOpaqueCaptureAddressCreateInfo(const VkBufferOpaqueCaptureAddressCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryOpaqueCaptureAddressAllocateInfo(const VkMemoryOpaqueCaptureAddressAllocateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceMemoryOpaqueCaptureAddressInfo(const VkDeviceMemoryOpaqueCaptureAddressInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan13Features(const VkPhysicalDeviceVulkan13Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan13Properties(const VkPhysicalDeviceVulkan13Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreationFeedback(const VkPipelineCreationFeedback& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreationFeedbackCreateInfo(const VkPipelineCreationFeedbackCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderTerminateInvocationFeatures(const VkPhysicalDeviceShaderTerminateInvocationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceToolProperties(const VkPhysicalDeviceToolProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures(const VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePrivateDataFeatures(const VkPhysicalDevicePrivateDataFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDevicePrivateDataCreateInfo(const VkDevicePrivateDataCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPrivateDataSlotCreateInfo(const VkPrivateDataSlotCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineCreationCacheControlFeatures(const VkPhysicalDevicePipelineCreationCacheControlFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryBarrier2(const VkMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferMemoryBarrier2(const VkBufferMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageMemoryBarrier2(const VkImageMemoryBarrier2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDependencyInfo(const VkDependencyInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreSubmitInfo(const VkSemaphoreSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferSubmitInfo(const VkCommandBufferSubmitInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubmitInfo2(const VkSubmitInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSynchronization2Features(const VkPhysicalDeviceSynchronization2Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures(const VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageRobustnessFeatures(const VkPhysicalDeviceImageRobustnessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferCopy2(const VkBufferCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyBufferInfo2(const VkCopyBufferInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCopy2(const VkImageCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyImageInfo2(const VkCopyImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferImageCopy2(const VkBufferImageCopy2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyBufferToImageInfo2(const VkCopyBufferToImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyImageToBufferInfo2(const VkCopyImageToBufferInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageBlit2(const VkImageBlit2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBlitImageInfo2(const VkBlitImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageResolve2(const VkImageResolve2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkResolveImageInfo2(const VkResolveImageInfo2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSubgroupSizeControlFeatures(const VkPhysicalDeviceSubgroupSizeControlFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSubgroupSizeControlProperties(const VkPhysicalDeviceSubgroupSizeControlProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineShaderStageRequiredSubgroupSizeCreateInfo(const VkPipelineShaderStageRequiredSubgroupSizeCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceInlineUniformBlockFeatures(const VkPhysicalDeviceInlineUniformBlockFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceInlineUniformBlockProperties(const VkPhysicalDeviceInlineUniformBlockProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkWriteDescriptorSetInlineUniformBlock(const VkWriteDescriptorSetInlineUniformBlock& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorPoolInlineUniformBlockCreateInfo(const VkDescriptorPoolInlineUniformBlockCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTextureCompressionASTCHDRFeatures(const VkPhysicalDeviceTextureCompressionASTCHDRFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingAttachmentInfo(const VkRenderingAttachmentInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingInfo(const VkRenderingInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRenderingCreateInfo(const VkPipelineRenderingCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDynamicRenderingFeatures(const VkPhysicalDeviceDynamicRenderingFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferInheritanceRenderingInfo(const VkCommandBufferInheritanceRenderingInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderIntegerDotProductFeatures(const VkPhysicalDeviceShaderIntegerDotProductFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderIntegerDotProductProperties(const VkPhysicalDeviceShaderIntegerDotProductProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTexelBufferAlignmentProperties(const VkPhysicalDeviceTexelBufferAlignmentProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFormatProperties3(const VkFormatProperties3& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance4Features(const VkPhysicalDeviceMaintenance4Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance4Properties(const VkPhysicalDeviceMaintenance4Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceBufferMemoryRequirements(const VkDeviceBufferMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceImageMemoryRequirements(const VkDeviceImageMemoryRequirements& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan14Features(const VkPhysicalDeviceVulkan14Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVulkan14Properties(const VkPhysicalDeviceVulkan14Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceQueueGlobalPriorityCreateInfo(const VkDeviceQueueGlobalPriorityCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceGlobalPriorityQueryFeatures(const VkPhysicalDeviceGlobalPriorityQueryFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyGlobalPriorityProperties(const VkQueueFamilyGlobalPriorityProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderSubgroupRotateFeatures(const VkPhysicalDeviceShaderSubgroupRotateFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderFloatControls2Features(const VkPhysicalDeviceShaderFloatControls2Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderExpectAssumeFeatures(const VkPhysicalDeviceShaderExpectAssumeFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLineRasterizationFeatures(const VkPhysicalDeviceLineRasterizationFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLineRasterizationProperties(const VkPhysicalDeviceLineRasterizationProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationLineStateCreateInfo(const VkPipelineRasterizationLineStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVertexAttributeDivisorProperties(const VkPhysicalDeviceVertexAttributeDivisorProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVertexInputBindingDivisorDescription(const VkVertexInputBindingDivisorDescription& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineVertexInputDivisorStateCreateInfo(const VkPipelineVertexInputDivisorStateCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVertexAttributeDivisorFeatures(const VkPhysicalDeviceVertexAttributeDivisorFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceIndexTypeUint8Features(const VkPhysicalDeviceIndexTypeUint8Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryMapInfo(const VkMemoryMapInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryUnmapInfo(const VkMemoryUnmapInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance5Features(const VkPhysicalDeviceMaintenance5Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance5Properties(const VkPhysicalDeviceMaintenance5Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingAreaInfo(const VkRenderingAreaInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageSubresource2(const VkImageSubresource2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceImageSubresourceInfo(const VkDeviceImageSubresourceInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubresourceLayout2(const VkSubresourceLayout2& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreateFlags2CreateInfo(const VkPipelineCreateFlags2CreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferUsageFlags2CreateInfo(const VkBufferUsageFlags2CreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePushDescriptorProperties(const VkPhysicalDevicePushDescriptorProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDynamicRenderingLocalReadFeatures(const VkPhysicalDeviceDynamicRenderingLocalReadFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingAttachmentLocationInfo(const VkRenderingAttachmentLocationInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingInputAttachmentIndexInfo(const VkRenderingInputAttachmentIndexInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance6Features(const VkPhysicalDeviceMaintenance6Features& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance6Properties(const VkPhysicalDeviceMaintenance6Properties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindMemoryStatus(const VkBindMemoryStatus& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindDescriptorSetsInfo(const VkBindDescriptorSetsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPushConstantsInfo(const VkPushConstantsInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPushDescriptorSetInfo(const VkPushDescriptorSetInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPushDescriptorSetWithTemplateInfo(const VkPushDescriptorSetWithTemplateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineProtectedAccessFeatures(const VkPhysicalDevicePipelineProtectedAccessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineRobustnessFeatures(const VkPhysicalDevicePipelineRobustnessFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineRobustnessProperties(const VkPhysicalDevicePipelineRobustnessProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRobustnessCreateInfo(const VkPipelineRobustnessCreateInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceHostImageCopyFeatures(const VkPhysicalDeviceHostImageCopyFeatures& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceHostImageCopyProperties(const VkPhysicalDeviceHostImageCopyProperties& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryToImageCopy(const VkMemoryToImageCopy& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageToMemoryCopy(const VkImageToMemoryCopy& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMemoryToImageInfo(const VkCopyMemoryToImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyImageToMemoryInfo(const VkCopyImageToMemoryInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyImageToImageInfo(const VkCopyImageToImageInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHostImageLayoutTransitionInfo(const VkHostImageLayoutTransitionInfo& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubresourceHostMemcpySize(const VkSubresourceHostMemcpySize& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHostImageCopyDevicePerformanceQuery(const VkHostImageCopyDevicePerformanceQuery& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceCapabilitiesKHR(const VkSurfaceCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceFormatKHR(const VkSurfaceFormatKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainCreateInfoKHR(const VkSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentInfoKHR(const VkPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageSwapchainCreateInfoKHR(const VkImageSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindImageMemorySwapchainInfoKHR(const VkBindImageMemorySwapchainInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAcquireNextImageInfoKHR(const VkAcquireNextImageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupPresentCapabilitiesKHR(const VkDeviceGroupPresentCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupPresentInfoKHR(const VkDeviceGroupPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceGroupSwapchainCreateInfoKHR(const VkDeviceGroupSwapchainCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModeParametersKHR(const VkDisplayModeParametersKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModeCreateInfoKHR(const VkDisplayModeCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModePropertiesKHR(const VkDisplayModePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlaneCapabilitiesKHR(const VkDisplayPlaneCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlanePropertiesKHR(const VkDisplayPlanePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPropertiesKHR(const VkDisplayPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplaySurfaceCreateInfoKHR(const VkDisplaySurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPresentInfoKHR(const VkDisplayPresentInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_VkXlibSurfaceCreateInfoKHR(const VkXlibSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_VkXcbSurfaceCreateInfoKHR(const VkXcbSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_VkWaylandSurfaceCreateInfoKHR(const VkWaylandSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidSurfaceCreateInfoKHR(const VkAndroidSurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkWin32SurfaceCreateInfoKHR(const VkWin32SurfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkDebugReportCallbackCreateInfoEXT(const VkDebugReportCallbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationStateRasterizationOrderAMD(const VkPipelineRasterizationStateRasterizationOrderAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugMarkerObjectNameInfoEXT(const VkDebugMarkerObjectNameInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugMarkerObjectTagInfoEXT(const VkDebugMarkerObjectTagInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugMarkerMarkerInfoEXT(const VkDebugMarkerMarkerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyQueryResultStatusPropertiesKHR(const VkQueueFamilyQueryResultStatusPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyVideoPropertiesKHR(const VkQueueFamilyVideoPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoProfileInfoKHR(const VkVideoProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoProfileListInfoKHR(const VkVideoProfileListInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCapabilitiesKHR(const VkVideoCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVideoFormatInfoKHR(const VkPhysicalDeviceVideoFormatInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoFormatPropertiesKHR(const VkVideoFormatPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoPictureResourceInfoKHR(const VkVideoPictureResourceInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoReferenceSlotInfoKHR(const VkVideoReferenceSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionMemoryRequirementsKHR(const VkVideoSessionMemoryRequirementsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindVideoSessionMemoryInfoKHR(const VkBindVideoSessionMemoryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionCreateInfoKHR(const VkVideoSessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionParametersCreateInfoKHR(const VkVideoSessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoSessionParametersUpdateInfoKHR(const VkVideoSessionParametersUpdateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoBeginCodingInfoKHR(const VkVideoBeginCodingInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEndCodingInfoKHR(const VkVideoEndCodingInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoCodingControlInfoKHR(const VkVideoCodingControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeCapabilitiesKHR(const VkVideoDecodeCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeUsageInfoKHR(const VkVideoDecodeUsageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeInfoKHR(const VkVideoDecodeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDedicatedAllocationImageCreateInfoNV(const VkDedicatedAllocationImageCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDedicatedAllocationBufferCreateInfoNV(const VkDedicatedAllocationBufferCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDedicatedAllocationMemoryAllocateInfoNV(const VkDedicatedAllocationMemoryAllocateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTransformFeedbackFeaturesEXT(const VkPhysicalDeviceTransformFeedbackFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTransformFeedbackPropertiesEXT(const VkPhysicalDeviceTransformFeedbackPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationStateStreamCreateInfoEXT(const VkPipelineRasterizationStateStreamCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCuModuleCreateInfoNVX(const VkCuModuleCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCuModuleTexturingModeCreateInfoNVX(const VkCuModuleTexturingModeCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCuFunctionCreateInfoNVX(const VkCuFunctionCreateInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCuLaunchInfoNVX(const VkCuLaunchInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewHandleInfoNVX(const VkImageViewHandleInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewAddressPropertiesNVX(const VkImageViewAddressPropertiesNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264CapabilitiesKHR(const VkVideoEncodeH264CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264QpKHR(const VkVideoEncodeH264QpKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264QualityLevelPropertiesKHR(const VkVideoEncodeH264QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264SessionCreateInfoKHR(const VkVideoEncodeH264SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264SessionParametersAddInfoKHR(const VkVideoEncodeH264SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264SessionParametersCreateInfoKHR(const VkVideoEncodeH264SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264SessionParametersGetInfoKHR(const VkVideoEncodeH264SessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264SessionParametersFeedbackInfoKHR(const VkVideoEncodeH264SessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264NaluSliceInfoKHR(const VkVideoEncodeH264NaluSliceInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264PictureInfoKHR(const VkVideoEncodeH264PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264DpbSlotInfoKHR(const VkVideoEncodeH264DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264ProfileInfoKHR(const VkVideoEncodeH264ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264RateControlInfoKHR(const VkVideoEncodeH264RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264FrameSizeKHR(const VkVideoEncodeH264FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264RateControlLayerInfoKHR(const VkVideoEncodeH264RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264GopRemainingFrameInfoKHR(const VkVideoEncodeH264GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265CapabilitiesKHR(const VkVideoEncodeH265CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265SessionCreateInfoKHR(const VkVideoEncodeH265SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265QpKHR(const VkVideoEncodeH265QpKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265QualityLevelPropertiesKHR(const VkVideoEncodeH265QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265SessionParametersAddInfoKHR(const VkVideoEncodeH265SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265SessionParametersCreateInfoKHR(const VkVideoEncodeH265SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265SessionParametersGetInfoKHR(const VkVideoEncodeH265SessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265SessionParametersFeedbackInfoKHR(const VkVideoEncodeH265SessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265NaluSliceSegmentInfoKHR(const VkVideoEncodeH265NaluSliceSegmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265PictureInfoKHR(const VkVideoEncodeH265PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265DpbSlotInfoKHR(const VkVideoEncodeH265DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265ProfileInfoKHR(const VkVideoEncodeH265ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265RateControlInfoKHR(const VkVideoEncodeH265RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265FrameSizeKHR(const VkVideoEncodeH265FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265RateControlLayerInfoKHR(const VkVideoEncodeH265RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265GopRemainingFrameInfoKHR(const VkVideoEncodeH265GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264ProfileInfoKHR(const VkVideoDecodeH264ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264CapabilitiesKHR(const VkVideoDecodeH264CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264SessionParametersAddInfoKHR(const VkVideoDecodeH264SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264SessionParametersCreateInfoKHR(const VkVideoDecodeH264SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264PictureInfoKHR(const VkVideoDecodeH264PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH264DpbSlotInfoKHR(const VkVideoDecodeH264DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTextureLODGatherFormatPropertiesAMD(const VkTextureLODGatherFormatPropertiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderResourceUsageAMD(const VkShaderResourceUsageAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderStatisticsInfoAMD(const VkShaderStatisticsInfoAMD& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_VkStreamDescriptorSurfaceCreateInfoGGP(const VkStreamDescriptorSurfaceCreateInfoGGP& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
void dump_json_VkPhysicalDeviceCornerSampledImageFeaturesNV(const VkPhysicalDeviceCornerSampledImageFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalImageFormatPropertiesNV(const VkExternalImageFormatPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryImageCreateInfoNV(const VkExternalMemoryImageCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExportMemoryAllocateInfoNV(const VkExportMemoryAllocateInfoNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkImportMemoryWin32HandleInfoNV(const VkImportMemoryWin32HandleInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkExportMemoryWin32HandleInfoNV(const VkExportMemoryWin32HandleInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkWin32KeyedMutexAcquireReleaseInfoNV(const VkWin32KeyedMutexAcquireReleaseInfoNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkValidationFlagsEXT(const VkValidationFlagsEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_json_VkViSurfaceCreateInfoNN(const VkViSurfaceCreateInfoNN& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_VI_NN
void dump_json_VkImageViewASTCDecodeModeEXT(const VkImageViewASTCDecodeModeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceASTCDecodeFeaturesEXT(const VkPhysicalDeviceASTCDecodeFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkImportMemoryWin32HandleInfoKHR(const VkImportMemoryWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkExportMemoryWin32HandleInfoKHR(const VkExportMemoryWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkMemoryWin32HandlePropertiesKHR(const VkMemoryWin32HandlePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkMemoryGetWin32HandleInfoKHR(const VkMemoryGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkImportMemoryFdInfoKHR(const VkImportMemoryFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryFdPropertiesKHR(const VkMemoryFdPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryGetFdInfoKHR(const VkMemoryGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkWin32KeyedMutexAcquireReleaseInfoKHR(const VkWin32KeyedMutexAcquireReleaseInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkImportSemaphoreWin32HandleInfoKHR(const VkImportSemaphoreWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkExportSemaphoreWin32HandleInfoKHR(const VkExportSemaphoreWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkD3D12FenceSubmitInfoKHR(const VkD3D12FenceSubmitInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkSemaphoreGetWin32HandleInfoKHR(const VkSemaphoreGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkImportSemaphoreFdInfoKHR(const VkImportSemaphoreFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSemaphoreGetFdInfoKHR(const VkSemaphoreGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkConditionalRenderingBeginInfoEXT(const VkConditionalRenderingBeginInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceConditionalRenderingFeaturesEXT(const VkPhysicalDeviceConditionalRenderingFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferInheritanceConditionalRenderingInfoEXT(const VkCommandBufferInheritanceConditionalRenderingInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRectLayerKHR(const VkRectLayerKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentRegionKHR(const VkPresentRegionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentRegionsKHR(const VkPresentRegionsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkViewportWScalingNV(const VkViewportWScalingNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportWScalingStateCreateInfoNV(const VkPipelineViewportWScalingStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceCapabilities2EXT(const VkSurfaceCapabilities2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPowerInfoEXT(const VkDisplayPowerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceEventInfoEXT(const VkDeviceEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayEventInfoEXT(const VkDisplayEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainCounterCreateInfoEXT(const VkSwapchainCounterCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRefreshCycleDurationGOOGLE(const VkRefreshCycleDurationGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPastPresentationTimingGOOGLE(const VkPastPresentationTimingGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentTimeGOOGLE(const VkPresentTimeGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentTimesInfoGOOGLE(const VkPresentTimesInfoGOOGLE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX(const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMultiviewPerViewAttributesInfoNVX(const VkMultiviewPerViewAttributesInfoNVX& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkViewportSwizzleNV(const VkViewportSwizzleNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportSwizzleStateCreateInfoNV(const VkPipelineViewportSwizzleStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDiscardRectanglePropertiesEXT(const VkPhysicalDeviceDiscardRectanglePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineDiscardRectangleStateCreateInfoEXT(const VkPipelineDiscardRectangleStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceConservativeRasterizationPropertiesEXT(const VkPhysicalDeviceConservativeRasterizationPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationConservativeStateCreateInfoEXT(const VkPipelineRasterizationConservativeStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDepthClipEnableFeaturesEXT(const VkPhysicalDeviceDepthClipEnableFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationDepthClipStateCreateInfoEXT(const VkPipelineRasterizationDepthClipStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkXYColorEXT(const VkXYColorEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHdrMetadataEXT(const VkHdrMetadataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG(const VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSharedPresentSurfaceCapabilitiesKHR(const VkSharedPresentSurfaceCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkImportFenceWin32HandleInfoKHR(const VkImportFenceWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkExportFenceWin32HandleInfoKHR(const VkExportFenceWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkFenceGetWin32HandleInfoKHR(const VkFenceGetWin32HandleInfoKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkImportFenceFdInfoKHR(const VkImportFenceFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFenceGetFdInfoKHR(const VkFenceGetFdInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePerformanceQueryFeaturesKHR(const VkPhysicalDevicePerformanceQueryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePerformanceQueryPropertiesKHR(const VkPhysicalDevicePerformanceQueryPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterKHR(const VkPerformanceCounterKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterDescriptionKHR(const VkPerformanceCounterDescriptionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPoolPerformanceCreateInfoKHR(const VkQueryPoolPerformanceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAcquireProfilingLockInfoKHR(const VkAcquireProfilingLockInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceQuerySubmitInfoKHR(const VkPerformanceQuerySubmitInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSurfaceInfo2KHR(const VkPhysicalDeviceSurfaceInfo2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceCapabilities2KHR(const VkSurfaceCapabilities2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceFormat2KHR(const VkSurfaceFormat2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayProperties2KHR(const VkDisplayProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlaneProperties2KHR(const VkDisplayPlaneProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModeProperties2KHR(const VkDisplayModeProperties2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlaneInfo2KHR(const VkDisplayPlaneInfo2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayPlaneCapabilities2KHR(const VkDisplayPlaneCapabilities2KHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_json_VkIOSSurfaceCreateInfoMVK(const VkIOSSurfaceCreateInfoMVK& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_json_VkMacOSSurfaceCreateInfoMVK(const VkMacOSSurfaceCreateInfoMVK& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_json_VkDebugUtilsLabelEXT(const VkDebugUtilsLabelEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsObjectNameInfoEXT(const VkDebugUtilsObjectNameInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessengerCallbackDataEXT(const VkDebugUtilsMessengerCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsMessengerCreateInfoEXT(const VkDebugUtilsMessengerCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDebugUtilsObjectTagInfoEXT(const VkDebugUtilsObjectTagInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidHardwareBufferUsageANDROID(const VkAndroidHardwareBufferUsageANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidHardwareBufferPropertiesANDROID(const VkAndroidHardwareBufferPropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidHardwareBufferFormatPropertiesANDROID(const VkAndroidHardwareBufferFormatPropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkImportAndroidHardwareBufferInfoANDROID(const VkImportAndroidHardwareBufferInfoANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkMemoryGetAndroidHardwareBufferInfoANDROID(const VkMemoryGetAndroidHardwareBufferInfoANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkExternalFormatANDROID(const VkExternalFormatANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidHardwareBufferFormatProperties2ANDROID(const VkAndroidHardwareBufferFormatProperties2ANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPhysicalDeviceShaderEnqueueFeaturesAMDX(const VkPhysicalDeviceShaderEnqueueFeaturesAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPhysicalDeviceShaderEnqueuePropertiesAMDX(const VkPhysicalDeviceShaderEnqueuePropertiesAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkExecutionGraphPipelineScratchSizeAMDX(const VkExecutionGraphPipelineScratchSizeAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkPipelineLibraryCreateInfoKHR(const VkPipelineLibraryCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkExecutionGraphPipelineCreateInfoAMDX(const VkExecutionGraphPipelineCreateInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkDispatchGraphInfoAMDX(const VkDispatchGraphInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkDispatchGraphCountInfoAMDX(const VkDispatchGraphCountInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPipelineShaderStageNodeCreateInfoAMDX(const VkPipelineShaderStageNodeCreateInfoAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkAttachmentSampleCountInfoAMD(const VkAttachmentSampleCountInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSampleLocationEXT(const VkSampleLocationEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSampleLocationsInfoEXT(const VkSampleLocationsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAttachmentSampleLocationsEXT(const VkAttachmentSampleLocationsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassSampleLocationsEXT(const VkSubpassSampleLocationsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassSampleLocationsBeginInfoEXT(const VkRenderPassSampleLocationsBeginInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineSampleLocationsStateCreateInfoEXT(const VkPipelineSampleLocationsStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSampleLocationsPropertiesEXT(const VkPhysicalDeviceSampleLocationsPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMultisamplePropertiesEXT(const VkMultisamplePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT(const VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT(const VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineColorBlendAdvancedStateCreateInfoEXT(const VkPipelineColorBlendAdvancedStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCoverageToColorStateCreateInfoNV(const VkPipelineCoverageToColorStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureBuildRangeInfoKHR(const VkAccelerationStructureBuildRangeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAabbPositionsKHR(const VkAabbPositionsKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryTrianglesDataKHR(const VkAccelerationStructureGeometryTrianglesDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTransformMatrixKHR(const VkTransformMatrixKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryAabbsDataKHR(const VkAccelerationStructureGeometryAabbsDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryInstancesDataKHR(const VkAccelerationStructureGeometryInstancesDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryKHR(const VkAccelerationStructureGeometryKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureBuildGeometryInfoKHR(const VkAccelerationStructureBuildGeometryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureInstanceKHR(const VkAccelerationStructureInstanceKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureCreateInfoKHR(const VkAccelerationStructureCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkWriteDescriptorSetAccelerationStructureKHR(const VkWriteDescriptorSetAccelerationStructureKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceAccelerationStructureFeaturesKHR(const VkPhysicalDeviceAccelerationStructureFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceAccelerationStructurePropertiesKHR(const VkPhysicalDeviceAccelerationStructurePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureDeviceAddressInfoKHR(const VkAccelerationStructureDeviceAddressInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureVersionInfoKHR(const VkAccelerationStructureVersionInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyAccelerationStructureToMemoryInfoKHR(const VkCopyAccelerationStructureToMemoryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMemoryToAccelerationStructureInfoKHR(const VkCopyMemoryToAccelerationStructureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyAccelerationStructureInfoKHR(const VkCopyAccelerationStructureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureBuildSizesInfoKHR(const VkAccelerationStructureBuildSizesInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCoverageModulationStateCreateInfoNV(const VkPipelineCoverageModulationStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderSMBuiltinsPropertiesNV(const VkPhysicalDeviceShaderSMBuiltinsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderSMBuiltinsFeaturesNV(const VkPhysicalDeviceShaderSMBuiltinsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrmFormatModifierPropertiesEXT(const VkDrmFormatModifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrmFormatModifierPropertiesListEXT(const VkDrmFormatModifierPropertiesListEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageDrmFormatModifierInfoEXT(const VkPhysicalDeviceImageDrmFormatModifierInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageDrmFormatModifierListCreateInfoEXT(const VkImageDrmFormatModifierListCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageDrmFormatModifierExplicitCreateInfoEXT(const VkImageDrmFormatModifierExplicitCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageDrmFormatModifierPropertiesEXT(const VkImageDrmFormatModifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrmFormatModifierProperties2EXT(const VkDrmFormatModifierProperties2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrmFormatModifierPropertiesList2EXT(const VkDrmFormatModifierPropertiesList2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationCacheCreateInfoEXT(const VkValidationCacheCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderModuleValidationCacheCreateInfoEXT(const VkShaderModuleValidationCacheCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPhysicalDevicePortabilitySubsetFeaturesKHR(const VkPhysicalDevicePortabilitySubsetFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPhysicalDevicePortabilitySubsetPropertiesKHR(const VkPhysicalDevicePortabilitySubsetPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkShadingRatePaletteNV(const VkShadingRatePaletteNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportShadingRateImageStateCreateInfoNV(const VkPipelineViewportShadingRateImageStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShadingRateImageFeaturesNV(const VkPhysicalDeviceShadingRateImageFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShadingRateImagePropertiesNV(const VkPhysicalDeviceShadingRateImagePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCoarseSampleLocationNV(const VkCoarseSampleLocationNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCoarseSampleOrderCustomNV(const VkCoarseSampleOrderCustomNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportCoarseSampleOrderStateCreateInfoNV(const VkPipelineViewportCoarseSampleOrderStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRayTracingShaderGroupCreateInfoNV(const VkRayTracingShaderGroupCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRayTracingPipelineCreateInfoNV(const VkRayTracingPipelineCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryTrianglesNV(const VkGeometryTrianglesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryAABBNV(const VkGeometryAABBNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryDataNV(const VkGeometryDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeometryNV(const VkGeometryNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureInfoNV(const VkAccelerationStructureInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureCreateInfoNV(const VkAccelerationStructureCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindAccelerationStructureMemoryInfoNV(const VkBindAccelerationStructureMemoryInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkWriteDescriptorSetAccelerationStructureNV(const VkWriteDescriptorSetAccelerationStructureNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMemoryRequirementsInfoNV(const VkAccelerationStructureMemoryRequirementsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingPropertiesNV(const VkPhysicalDeviceRayTracingPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV(const VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRepresentativeFragmentTestStateCreateInfoNV(const VkPipelineRepresentativeFragmentTestStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageViewImageFormatInfoEXT(const VkPhysicalDeviceImageViewImageFormatInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFilterCubicImageViewImageFormatPropertiesEXT(const VkFilterCubicImageViewImageFormatPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImportMemoryHostPointerInfoEXT(const VkImportMemoryHostPointerInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryHostPointerPropertiesEXT(const VkMemoryHostPointerPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExternalMemoryHostPropertiesEXT(const VkPhysicalDeviceExternalMemoryHostPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderClockFeaturesKHR(const VkPhysicalDeviceShaderClockFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCompilerControlCreateInfoAMD(const VkPipelineCompilerControlCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCalibratedTimestampInfoKHR(const VkCalibratedTimestampInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderCorePropertiesAMD(const VkPhysicalDeviceShaderCorePropertiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH265ProfileInfoKHR(const VkVideoDecodeH265ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH265CapabilitiesKHR(const VkVideoDecodeH265CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH265SessionParametersAddInfoKHR(const VkVideoDecodeH265SessionParametersAddInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH265SessionParametersCreateInfoKHR(const VkVideoDecodeH265SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH265PictureInfoKHR(const VkVideoDecodeH265PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeH265DpbSlotInfoKHR(const VkVideoDecodeH265DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceMemoryOverallocationCreateInfoAMD(const VkDeviceMemoryOverallocationCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT(const VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_VkPresentFrameTokenGGP(const VkPresentFrameTokenGGP& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_GGP
void dump_json_VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR(const VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMeshShaderFeaturesNV(const VkPhysicalDeviceMeshShaderFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMeshShaderPropertiesNV(const VkPhysicalDeviceMeshShaderPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrawMeshTasksIndirectCommandNV(const VkDrawMeshTasksIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR(const VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderImageFootprintFeaturesNV(const VkPhysicalDeviceShaderImageFootprintFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportExclusiveScissorStateCreateInfoNV(const VkPipelineViewportExclusiveScissorStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExclusiveScissorFeaturesNV(const VkPhysicalDeviceExclusiveScissorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyCheckpointPropertiesNV(const VkQueueFamilyCheckpointPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCheckpointDataNV(const VkCheckpointDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueueFamilyCheckpointProperties2NV(const VkQueueFamilyCheckpointProperties2NV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCheckpointData2NV(const VkCheckpointData2NV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL(const VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceValueINTEL(const VkPerformanceValueINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkInitializePerformanceApiInfoINTEL(const VkInitializePerformanceApiInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPoolPerformanceQueryCreateInfoINTEL(const VkQueryPoolPerformanceQueryCreateInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceMarkerInfoINTEL(const VkPerformanceMarkerInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceStreamMarkerInfoINTEL(const VkPerformanceStreamMarkerInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceOverrideInfoINTEL(const VkPerformanceOverrideInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceConfigurationAcquireInfoINTEL(const VkPerformanceConfigurationAcquireInfoINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePCIBusInfoPropertiesEXT(const VkPhysicalDevicePCIBusInfoPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayNativeHdrSurfaceCapabilitiesAMD(const VkDisplayNativeHdrSurfaceCapabilitiesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainDisplayNativeHdrCreateInfoAMD(const VkSwapchainDisplayNativeHdrCreateInfoAMD& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImagePipeSurfaceCreateInfoFUCHSIA(const VkImagePipeSurfaceCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkMetalSurfaceCreateInfoEXT(const VkMetalSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_VkPhysicalDeviceFragmentDensityMapFeaturesEXT(const VkPhysicalDeviceFragmentDensityMapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentDensityMapPropertiesEXT(const VkPhysicalDeviceFragmentDensityMapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassFragmentDensityMapCreateInfoEXT(const VkRenderPassFragmentDensityMapCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingFragmentDensityMapAttachmentInfoEXT(const VkRenderingFragmentDensityMapAttachmentInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFragmentShadingRateAttachmentInfoKHR(const VkFragmentShadingRateAttachmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineFragmentShadingRateStateCreateInfoKHR(const VkPipelineFragmentShadingRateStateCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShadingRateFeaturesKHR(const VkPhysicalDeviceFragmentShadingRateFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShadingRatePropertiesKHR(const VkPhysicalDeviceFragmentShadingRatePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShadingRateKHR(const VkPhysicalDeviceFragmentShadingRateKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderingFragmentShadingRateAttachmentInfoKHR(const VkRenderingFragmentShadingRateAttachmentInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderCoreProperties2AMD(const VkPhysicalDeviceShaderCoreProperties2AMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCoherentMemoryFeaturesAMD(const VkPhysicalDeviceCoherentMemoryFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT(const VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderQuadControlFeaturesKHR(const VkPhysicalDeviceShaderQuadControlFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMemoryBudgetPropertiesEXT(const VkPhysicalDeviceMemoryBudgetPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMemoryPriorityFeaturesEXT(const VkPhysicalDeviceMemoryPriorityFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryPriorityAllocateInfoEXT(const VkMemoryPriorityAllocateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceProtectedCapabilitiesKHR(const VkSurfaceProtectedCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV(const VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceBufferDeviceAddressFeaturesEXT(const VkPhysicalDeviceBufferDeviceAddressFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferDeviceAddressCreateInfoEXT(const VkBufferDeviceAddressCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkValidationFeaturesEXT(const VkValidationFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePresentWaitFeaturesKHR(const VkPhysicalDevicePresentWaitFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCooperativeMatrixPropertiesNV(const VkCooperativeMatrixPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCooperativeMatrixFeaturesNV(const VkPhysicalDeviceCooperativeMatrixFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCooperativeMatrixPropertiesNV(const VkPhysicalDeviceCooperativeMatrixPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCoverageReductionModeFeaturesNV(const VkPhysicalDeviceCoverageReductionModeFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCoverageReductionStateCreateInfoNV(const VkPipelineCoverageReductionStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFramebufferMixedSamplesCombinationNV(const VkFramebufferMixedSamplesCombinationNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT(const VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceYcbcrImageArraysFeaturesEXT(const VkPhysicalDeviceYcbcrImageArraysFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceProvokingVertexFeaturesEXT(const VkPhysicalDeviceProvokingVertexFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceProvokingVertexPropertiesEXT(const VkPhysicalDeviceProvokingVertexPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineRasterizationProvokingVertexStateCreateInfoEXT(const VkPipelineRasterizationProvokingVertexStateCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkSurfaceFullScreenExclusiveInfoEXT(const VkSurfaceFullScreenExclusiveInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkSurfaceCapabilitiesFullScreenExclusiveEXT(const VkSurfaceCapabilitiesFullScreenExclusiveEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_VkSurfaceFullScreenExclusiveWin32InfoEXT(const VkSurfaceFullScreenExclusiveWin32InfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_VkHeadlessSurfaceCreateInfoEXT(const VkHeadlessSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderAtomicFloatFeaturesEXT(const VkPhysicalDeviceShaderAtomicFloatFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExtendedDynamicStateFeaturesEXT(const VkPhysicalDeviceExtendedDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR(const VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineInfoKHR(const VkPipelineInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineExecutablePropertiesKHR(const VkPipelineExecutablePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineExecutableInfoKHR(const VkPipelineExecutableInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineExecutableStatisticKHR(const VkPipelineExecutableStatisticKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineExecutableInternalRepresentationKHR(const VkPipelineExecutableInternalRepresentationKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMapMemoryPlacedFeaturesEXT(const VkPhysicalDeviceMapMemoryPlacedFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMapMemoryPlacedPropertiesEXT(const VkPhysicalDeviceMapMemoryPlacedPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryMapPlacedInfoEXT(const VkMemoryMapPlacedInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT(const VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfacePresentModeEXT(const VkSurfacePresentModeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfacePresentScalingCapabilitiesEXT(const VkSurfacePresentScalingCapabilitiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfacePresentModeCompatibilityEXT(const VkSurfacePresentModeCompatibilityEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT(const VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainPresentFenceInfoEXT(const VkSwapchainPresentFenceInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainPresentModesCreateInfoEXT(const VkSwapchainPresentModesCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainPresentModeInfoEXT(const VkSwapchainPresentModeInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainPresentScalingCreateInfoEXT(const VkSwapchainPresentScalingCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkReleaseSwapchainImagesInfoEXT(const VkReleaseSwapchainImagesInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV(const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV(const VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGraphicsShaderGroupCreateInfoNV(const VkGraphicsShaderGroupCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGraphicsPipelineShaderGroupsCreateInfoNV(const VkGraphicsPipelineShaderGroupsCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindShaderGroupIndirectCommandNV(const VkBindShaderGroupIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindIndexBufferIndirectCommandNV(const VkBindIndexBufferIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindVertexBufferIndirectCommandNV(const VkBindVertexBufferIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSetStateFlagsIndirectCommandNV(const VkSetStateFlagsIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsStreamNV(const VkIndirectCommandsStreamNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutTokenNV(const VkIndirectCommandsLayoutTokenNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutCreateInfoNV(const VkIndirectCommandsLayoutCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeneratedCommandsInfoNV(const VkGeneratedCommandsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeneratedCommandsMemoryRequirementsInfoNV(const VkGeneratedCommandsMemoryRequirementsInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceInheritedViewportScissorFeaturesNV(const VkPhysicalDeviceInheritedViewportScissorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferInheritanceViewportScissorInfoNV(const VkCommandBufferInheritanceViewportScissorInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT(const VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassTransformBeginInfoQCOM(const VkRenderPassTransformBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCommandBufferInheritanceRenderPassTransformInfoQCOM(const VkCommandBufferInheritanceRenderPassTransformInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDepthBiasControlFeaturesEXT(const VkPhysicalDeviceDepthBiasControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDepthBiasInfoEXT(const VkDepthBiasInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDepthBiasRepresentationInfoEXT(const VkDepthBiasRepresentationInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDeviceMemoryReportFeaturesEXT(const VkPhysicalDeviceDeviceMemoryReportFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceMemoryReportCallbackDataEXT(const VkDeviceMemoryReportCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceDeviceMemoryReportCreateInfoEXT(const VkDeviceDeviceMemoryReportCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRobustness2FeaturesEXT(const VkPhysicalDeviceRobustness2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRobustness2PropertiesEXT(const VkPhysicalDeviceRobustness2PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerCustomBorderColorCreateInfoEXT(const VkSamplerCustomBorderColorCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCustomBorderColorPropertiesEXT(const VkPhysicalDeviceCustomBorderColorPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCustomBorderColorFeaturesEXT(const VkPhysicalDeviceCustomBorderColorFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePresentBarrierFeaturesNV(const VkPhysicalDevicePresentBarrierFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSurfaceCapabilitiesPresentBarrierNV(const VkSurfaceCapabilitiesPresentBarrierNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainPresentBarrierCreateInfoNV(const VkSwapchainPresentBarrierCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPresentIdKHR(const VkPresentIdKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePresentIdFeaturesKHR(const VkPhysicalDevicePresentIdFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeInfoKHR(const VkVideoEncodeInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeCapabilitiesKHR(const VkVideoEncodeCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryPoolVideoEncodeFeedbackCreateInfoKHR(const VkQueryPoolVideoEncodeFeedbackCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeUsageInfoKHR(const VkVideoEncodeUsageInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeRateControlLayerInfoKHR(const VkVideoEncodeRateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeRateControlInfoKHR(const VkVideoEncodeRateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR(const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeQualityLevelPropertiesKHR(const VkVideoEncodeQualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeQualityLevelInfoKHR(const VkVideoEncodeQualityLevelInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeSessionParametersGetInfoKHR(const VkVideoEncodeSessionParametersGetInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeSessionParametersFeedbackInfoKHR(const VkVideoEncodeSessionParametersFeedbackInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDiagnosticsConfigFeaturesNV(const VkPhysicalDeviceDiagnosticsConfigFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceDiagnosticsConfigCreateInfoNV(const VkDeviceDiagnosticsConfigCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCudaModuleCreateInfoNV(const VkCudaModuleCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCudaFunctionCreateInfoNV(const VkCudaFunctionCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCudaLaunchInfoNV(const VkCudaLaunchInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCudaKernelLaunchFeaturesNV(const VkPhysicalDeviceCudaKernelLaunchFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCudaKernelLaunchPropertiesNV(const VkPhysicalDeviceCudaKernelLaunchPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkQueryLowLatencySupportNV(const VkQueryLowLatencySupportNV& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalObjectCreateInfoEXT(const VkExportMetalObjectCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalObjectsInfoEXT(const VkExportMetalObjectsInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalDeviceInfoEXT(const VkExportMetalDeviceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalCommandQueueInfoEXT(const VkExportMetalCommandQueueInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalBufferInfoEXT(const VkExportMetalBufferInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkImportMetalBufferInfoEXT(const VkImportMetalBufferInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalTextureInfoEXT(const VkExportMetalTextureInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkImportMetalTextureInfoEXT(const VkImportMetalTextureInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalIOSurfaceInfoEXT(const VkExportMetalIOSurfaceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkImportMetalIOSurfaceInfoEXT(const VkImportMetalIOSurfaceInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkExportMetalSharedEventInfoEXT(const VkExportMetalSharedEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_VkImportMetalSharedEventInfoEXT(const VkImportMetalSharedEventInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_VkPhysicalDeviceDescriptorBufferPropertiesEXT(const VkPhysicalDeviceDescriptorBufferPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT(const VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDescriptorBufferFeaturesEXT(const VkPhysicalDeviceDescriptorBufferFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorAddressInfoEXT(const VkDescriptorAddressInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorBufferBindingInfoEXT(const VkDescriptorBufferBindingInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorBufferBindingPushDescriptorBufferHandleEXT(const VkDescriptorBufferBindingPushDescriptorBufferHandleEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorGetInfoEXT(const VkDescriptorGetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBufferCaptureDescriptorDataInfoEXT(const VkBufferCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCaptureDescriptorDataInfoEXT(const VkImageCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewCaptureDescriptorDataInfoEXT(const VkImageViewCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerCaptureDescriptorDataInfoEXT(const VkSamplerCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpaqueCaptureDescriptorDataCreateInfoEXT(const VkOpaqueCaptureDescriptorDataCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureCaptureDescriptorDataInfoEXT(const VkAccelerationStructureCaptureDescriptorDataInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT(const VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT(const VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGraphicsPipelineLibraryCreateInfoEXT(const VkGraphicsPipelineLibraryCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD(const VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR(const VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR(const VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV(const VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV(const VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineFragmentShadingRateEnumStateCreateInfoNV(const VkPipelineFragmentShadingRateEnumStateCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryMotionTrianglesDataNV(const VkAccelerationStructureGeometryMotionTrianglesDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInfoNV(const VkAccelerationStructureMotionInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMatrixMotionInstanceNV(const VkAccelerationStructureMatrixMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSRTDataNV(const VkSRTDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureSRTMotionInstanceNV(const VkAccelerationStructureSRTMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInstanceNV(const VkAccelerationStructureMotionInstanceNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingMotionBlurFeaturesNV(const VkPhysicalDeviceRayTracingMotionBlurFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMeshShaderFeaturesEXT(const VkPhysicalDeviceMeshShaderFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMeshShaderPropertiesEXT(const VkPhysicalDeviceMeshShaderPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrawMeshTasksIndirectCommandEXT(const VkDrawMeshTasksIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT(const VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentDensityMap2FeaturesEXT(const VkPhysicalDeviceFragmentDensityMap2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentDensityMap2PropertiesEXT(const VkPhysicalDeviceFragmentDensityMap2PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyCommandTransformInfoQCOM(const VkCopyCommandTransformInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR(const VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageCompressionControlFeaturesEXT(const VkPhysicalDeviceImageCompressionControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCompressionControlEXT(const VkImageCompressionControlEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageCompressionPropertiesEXT(const VkImageCompressionPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT(const VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevice4444FormatsFeaturesEXT(const VkPhysicalDevice4444FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFaultFeaturesEXT(const VkPhysicalDeviceFaultFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultCountsEXT(const VkDeviceFaultCountsEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultAddressInfoEXT(const VkDeviceFaultAddressInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultVendorInfoEXT(const VkDeviceFaultVendorInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultInfoEXT(const VkDeviceFaultInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceFaultVendorBinaryHeaderVersionOneEXT(const VkDeviceFaultVendorBinaryHeaderVersionOneEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT(const VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT(const VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_VkDirectFBSurfaceCreateInfoEXT(const VkDirectFBSurfaceCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_json_VkRayTracingShaderGroupCreateInfoKHR(const VkRayTracingShaderGroupCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRayTracingPipelineInterfaceCreateInfoKHR(const VkRayTracingPipelineInterfaceCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRayTracingPipelineCreateInfoKHR(const VkRayTracingPipelineCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingPipelineFeaturesKHR(const VkPhysicalDeviceRayTracingPipelineFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingPipelinePropertiesKHR(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkStridedDeviceAddressRegionKHR(const VkStridedDeviceAddressRegionKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTraceRaysIndirectCommandKHR(const VkTraceRaysIndirectCommandKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayQueryFeaturesKHR(const VkPhysicalDeviceRayQueryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT(const VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMutableDescriptorTypeListEXT(const VkMutableDescriptorTypeListEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMutableDescriptorTypeCreateInfoEXT(const VkMutableDescriptorTypeCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT(const VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVertexInputBindingDescription2EXT(const VkVertexInputBindingDescription2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVertexInputAttributeDescription2EXT(const VkVertexInputAttributeDescription2EXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDrmPropertiesEXT(const VkPhysicalDeviceDrmPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceAddressBindingReportFeaturesEXT(const VkPhysicalDeviceAddressBindingReportFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceAddressBindingCallbackDataEXT(const VkDeviceAddressBindingCallbackDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDepthClipControlFeaturesEXT(const VkPhysicalDeviceDepthClipControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportDepthClipControlCreateInfoEXT(const VkPipelineViewportDepthClipControlCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT(const VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT(const VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImportMemoryZirconHandleInfoFUCHSIA(const VkImportMemoryZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkMemoryZirconHandlePropertiesFUCHSIA(const VkMemoryZirconHandlePropertiesFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkMemoryGetZirconHandleInfoFUCHSIA(const VkMemoryGetZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImportSemaphoreZirconHandleInfoFUCHSIA(const VkImportSemaphoreZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkSemaphoreGetZirconHandleInfoFUCHSIA(const VkSemaphoreGetZirconHandleInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferCollectionCreateInfoFUCHSIA(const VkBufferCollectionCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImportMemoryBufferCollectionFUCHSIA(const VkImportMemoryBufferCollectionFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferCollectionImageCreateInfoFUCHSIA(const VkBufferCollectionImageCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferCollectionConstraintsInfoFUCHSIA(const VkBufferCollectionConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferConstraintsInfoFUCHSIA(const VkBufferConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferCollectionBufferCreateInfoFUCHSIA(const VkBufferCollectionBufferCreateInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkSysmemColorSpaceFUCHSIA(const VkSysmemColorSpaceFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkBufferCollectionPropertiesFUCHSIA(const VkBufferCollectionPropertiesFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImageFormatConstraintsInfoFUCHSIA(const VkImageFormatConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_VkImageConstraintsInfoFUCHSIA(const VkImageConstraintsInfoFUCHSIA& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_VkSubpassShadingPipelineCreateInfoHUAWEI(const VkSubpassShadingPipelineCreateInfoHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSubpassShadingFeaturesHUAWEI(const VkPhysicalDeviceSubpassShadingFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSubpassShadingPropertiesHUAWEI(const VkPhysicalDeviceSubpassShadingPropertiesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceInvocationMaskFeaturesHUAWEI(const VkPhysicalDeviceInvocationMaskFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMemoryGetRemoteAddressInfoNV(const VkMemoryGetRemoteAddressInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExternalMemoryRDMAFeaturesNV(const VkPhysicalDeviceExternalMemoryRDMAFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelinePropertiesIdentifierEXT(const VkPipelinePropertiesIdentifierEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelinePropertiesFeaturesEXT(const VkPhysicalDevicePipelinePropertiesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFrameBoundaryFeaturesEXT(const VkPhysicalDeviceFrameBoundaryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkFrameBoundaryEXT(const VkFrameBoundaryEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT(const VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassResolvePerformanceQueryEXT(const VkSubpassResolvePerformanceQueryEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMultisampledRenderToSingleSampledInfoEXT(const VkMultisampledRenderToSingleSampledInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExtendedDynamicState2FeaturesEXT(const VkPhysicalDeviceExtendedDynamicState2FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkScreenSurfaceCreateInfoQNX(const VkScreenSurfaceCreateInfoQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_VkPhysicalDeviceColorWriteEnableFeaturesEXT(const VkPhysicalDeviceColorWriteEnableFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineColorWriteCreateInfoEXT(const VkPipelineColorWriteCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT(const VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR(const VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTraceRaysIndirectCommand2KHR(const VkTraceRaysIndirectCommand2KHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageViewMinLodFeaturesEXT(const VkPhysicalDeviceImageViewMinLodFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewMinLodCreateInfoEXT(const VkImageViewMinLodCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiDrawFeaturesEXT(const VkPhysicalDeviceMultiDrawFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiDrawPropertiesEXT(const VkPhysicalDeviceMultiDrawPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMultiDrawInfoEXT(const VkMultiDrawInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMultiDrawIndexedInfoEXT(const VkMultiDrawIndexedInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImage2DViewOf3DFeaturesEXT(const VkPhysicalDeviceImage2DViewOf3DFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderTileImageFeaturesEXT(const VkPhysicalDeviceShaderTileImageFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderTileImagePropertiesEXT(const VkPhysicalDeviceShaderTileImagePropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapUsageEXT(const VkMicromapUsageEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapBuildInfoEXT(const VkMicromapBuildInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapCreateInfoEXT(const VkMicromapCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceOpacityMicromapFeaturesEXT(const VkPhysicalDeviceOpacityMicromapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceOpacityMicromapPropertiesEXT(const VkPhysicalDeviceOpacityMicromapPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapVersionInfoEXT(const VkMicromapVersionInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMicromapToMemoryInfoEXT(const VkCopyMicromapToMemoryInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMemoryToMicromapInfoEXT(const VkCopyMemoryToMicromapInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMicromapInfoEXT(const VkCopyMicromapInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapBuildSizesInfoEXT(const VkMicromapBuildSizesInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureTrianglesOpacityMicromapEXT(const VkAccelerationStructureTrianglesOpacityMicromapEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMicromapTriangleEXT(const VkMicromapTriangleEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPhysicalDeviceDisplacementMicromapFeaturesNV(const VkPhysicalDeviceDisplacementMicromapFeaturesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkPhysicalDeviceDisplacementMicromapPropertiesNV(const VkPhysicalDeviceDisplacementMicromapPropertiesNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkAccelerationStructureTrianglesDisplacementMicromapNV(const VkAccelerationStructureTrianglesDisplacementMicromapNV& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI(const VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI(const VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI(const VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceBorderColorSwizzleFeaturesEXT(const VkPhysicalDeviceBorderColorSwizzleFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerBorderColorComponentMappingCreateInfoEXT(const VkSamplerBorderColorComponentMappingCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT(const VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderCorePropertiesARM(const VkPhysicalDeviceShaderCorePropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceQueueShaderCoreControlCreateInfoARM(const VkDeviceQueueShaderCoreControlCreateInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSchedulingControlsFeaturesARM(const VkPhysicalDeviceSchedulingControlsFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSchedulingControlsPropertiesARM(const VkPhysicalDeviceSchedulingControlsPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT(const VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewSlicedCreateInfoEXT(const VkImageViewSlicedCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE(const VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetBindingReferenceVALVE(const VkDescriptorSetBindingReferenceVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorSetLayoutHostMappingInfoVALVE(const VkDescriptorSetLayoutHostMappingInfoVALVE& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDepthClampZeroOneFeaturesEXT(const VkPhysicalDeviceDepthClampZeroOneFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT(const VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRenderPassStripedFeaturesARM(const VkPhysicalDeviceRenderPassStripedFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRenderPassStripedPropertiesARM(const VkPhysicalDeviceRenderPassStripedPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassStripeInfoARM(const VkRenderPassStripeInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassStripeBeginInfoARM(const VkRenderPassStripeBeginInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassStripeSubmitInfoARM(const VkRenderPassStripeSubmitInfoARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM(const VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM(const VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSubpassFragmentDensityMapOffsetEndInfoQCOM(const VkSubpassFragmentDensityMapOffsetEndInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMemoryIndirectCommandNV(const VkCopyMemoryIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCopyMemoryToImageIndirectCommandNV(const VkCopyMemoryToImageIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCopyMemoryIndirectFeaturesNV(const VkPhysicalDeviceCopyMemoryIndirectFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCopyMemoryIndirectPropertiesNV(const VkPhysicalDeviceCopyMemoryIndirectPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDecompressMemoryRegionNV(const VkDecompressMemoryRegionNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMemoryDecompressionFeaturesNV(const VkPhysicalDeviceMemoryDecompressionFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMemoryDecompressionPropertiesNV(const VkPhysicalDeviceMemoryDecompressionPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV(const VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkComputePipelineIndirectBufferInfoNV(const VkComputePipelineIndirectBufferInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineIndirectDeviceAddressInfoNV(const VkPipelineIndirectDeviceAddressInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindPipelineIndirectCommandNV(const VkBindPipelineIndirectCommandNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLinearColorAttachmentFeaturesNV(const VkPhysicalDeviceLinearColorAttachmentFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR(const VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT(const VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageViewSampleWeightCreateInfoQCOM(const VkImageViewSampleWeightCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageProcessingFeaturesQCOM(const VkPhysicalDeviceImageProcessingFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageProcessingPropertiesQCOM(const VkPhysicalDeviceImageProcessingPropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceNestedCommandBufferFeaturesEXT(const VkPhysicalDeviceNestedCommandBufferFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceNestedCommandBufferPropertiesEXT(const VkPhysicalDeviceNestedCommandBufferPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkExternalMemoryAcquireUnmodifiedEXT(const VkExternalMemoryAcquireUnmodifiedEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExtendedDynamicState3FeaturesEXT(const VkPhysicalDeviceExtendedDynamicState3FeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExtendedDynamicState3PropertiesEXT(const VkPhysicalDeviceExtendedDynamicState3PropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkColorBlendEquationEXT(const VkColorBlendEquationEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkColorBlendAdvancedEXT(const VkColorBlendAdvancedEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT(const VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreationControlEXT(const VkRenderPassCreationControlEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreationFeedbackInfoEXT(const VkRenderPassCreationFeedbackInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassCreationFeedbackCreateInfoEXT(const VkRenderPassCreationFeedbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassSubpassFeedbackInfoEXT(const VkRenderPassSubpassFeedbackInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkRenderPassSubpassFeedbackCreateInfoEXT(const VkRenderPassSubpassFeedbackCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDirectDriverLoadingInfoLUNARG(const VkDirectDriverLoadingInfoLUNARG& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDirectDriverLoadingListLUNARG(const VkDirectDriverLoadingListLUNARG& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT(const VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT(const VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineShaderStageModuleIdentifierCreateInfoEXT(const VkPipelineShaderStageModuleIdentifierCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderModuleIdentifierEXT(const VkShaderModuleIdentifierEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceOpticalFlowFeaturesNV(const VkPhysicalDeviceOpticalFlowFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceOpticalFlowPropertiesNV(const VkPhysicalDeviceOpticalFlowPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowImageFormatInfoNV(const VkOpticalFlowImageFormatInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowImageFormatPropertiesNV(const VkOpticalFlowImageFormatPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowSessionCreateInfoNV(const VkOpticalFlowSessionCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowSessionCreatePrivateDataInfoNV(const VkOpticalFlowSessionCreatePrivateDataInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOpticalFlowExecuteInfoNV(const VkOpticalFlowExecuteInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLegacyDitheringFeaturesEXT(const VkPhysicalDeviceLegacyDitheringFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkPhysicalDeviceExternalFormatResolveFeaturesANDROID(const VkPhysicalDeviceExternalFormatResolveFeaturesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkPhysicalDeviceExternalFormatResolvePropertiesANDROID(const VkPhysicalDeviceExternalFormatResolvePropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_VkAndroidHardwareBufferFormatResolvePropertiesANDROID(const VkAndroidHardwareBufferFormatResolvePropertiesANDROID& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_ANDROID_KHR
void dump_json_VkPhysicalDeviceAntiLagFeaturesAMD(const VkPhysicalDeviceAntiLagFeaturesAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAntiLagPresentationInfoAMD(const VkAntiLagPresentationInfoAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAntiLagDataAMD(const VkAntiLagDataAMD& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR(const VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderObjectFeaturesEXT(const VkPhysicalDeviceShaderObjectFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderObjectPropertiesEXT(const VkPhysicalDeviceShaderObjectPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkShaderCreateInfoEXT(const VkShaderCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDepthClampRangeEXT(const VkDepthClampRangeEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineBinaryFeaturesKHR(const VkPhysicalDevicePipelineBinaryFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineBinaryPropertiesKHR(const VkPhysicalDevicePipelineBinaryPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDevicePipelineBinaryInternalCacheControlKHR(const VkDevicePipelineBinaryInternalCacheControlKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryKeyKHR(const VkPipelineBinaryKeyKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryDataKHR(const VkPipelineBinaryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryKeysAndDataKHR(const VkPipelineBinaryKeysAndDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineCreateInfoKHR(const VkPipelineCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryCreateInfoKHR(const VkPipelineBinaryCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryInfoKHR(const VkPipelineBinaryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkReleaseCapturedPipelineDataInfoKHR(const VkReleaseCapturedPipelineDataInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryDataInfoKHR(const VkPipelineBinaryDataInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineBinaryHandlesInfoKHR(const VkPipelineBinaryHandlesInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceTilePropertiesFeaturesQCOM(const VkPhysicalDeviceTilePropertiesFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkTilePropertiesQCOM(const VkTilePropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceAmigoProfilingFeaturesSEC(const VkPhysicalDeviceAmigoProfilingFeaturesSEC& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAmigoProfilingSubmitInfoSEC(const VkAmigoProfilingSubmitInfoSEC& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM(const VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV(const VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV(const VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV(const VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV(const VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT(const VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT(const VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLayerSettingEXT(const VkLayerSettingEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLayerSettingsCreateInfoEXT(const VkLayerSettingsCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM(const VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM(const VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT(const VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT(const VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLatencySleepModeInfoNV(const VkLatencySleepModeInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLatencySleepInfoNV(const VkLatencySleepInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSetLatencyMarkerInfoNV(const VkSetLatencyMarkerInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLatencyTimingsFrameReportNV(const VkLatencyTimingsFrameReportNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGetLatencyMarkerInfoNV(const VkGetLatencyMarkerInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLatencySubmissionPresentIdNV(const VkLatencySubmissionPresentIdNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSwapchainLatencyCreateInfoNV(const VkSwapchainLatencyCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkOutOfBandQueueTypeInfoNV(const VkOutOfBandQueueTypeInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkLatencySurfaceCapabilitiesNV(const VkLatencySurfaceCapabilitiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCooperativeMatrixPropertiesKHR(const VkCooperativeMatrixPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCooperativeMatrixFeaturesKHR(const VkPhysicalDeviceCooperativeMatrixFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCooperativeMatrixPropertiesKHR(const VkPhysicalDeviceCooperativeMatrixPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM(const VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM(const VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR(const VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeAV1ProfileInfoKHR(const VkVideoDecodeAV1ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeAV1CapabilitiesKHR(const VkVideoDecodeAV1CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeAV1SessionParametersCreateInfoKHR(const VkVideoDecodeAV1SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeAV1PictureInfoKHR(const VkVideoDecodeAV1PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoDecodeAV1DpbSlotInfoKHR(const VkVideoDecodeAV1DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVideoEncodeAV1FeaturesKHR(const VkPhysicalDeviceVideoEncodeAV1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1CapabilitiesKHR(const VkVideoEncodeAV1CapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1QIndexKHR(const VkVideoEncodeAV1QIndexKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1QualityLevelPropertiesKHR(const VkVideoEncodeAV1QualityLevelPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1SessionCreateInfoKHR(const VkVideoEncodeAV1SessionCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1SessionParametersCreateInfoKHR(const VkVideoEncodeAV1SessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1PictureInfoKHR(const VkVideoEncodeAV1PictureInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1DpbSlotInfoKHR(const VkVideoEncodeAV1DpbSlotInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1ProfileInfoKHR(const VkVideoEncodeAV1ProfileInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1FrameSizeKHR(const VkVideoEncodeAV1FrameSizeKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1GopRemainingFrameInfoKHR(const VkVideoEncodeAV1GopRemainingFrameInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1RateControlInfoKHR(const VkVideoEncodeAV1RateControlInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1RateControlLayerInfoKHR(const VkVideoEncodeAV1RateControlLayerInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVideoMaintenance1FeaturesKHR(const VkPhysicalDeviceVideoMaintenance1FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoInlineQueryInfoKHR(const VkVideoInlineQueryInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDevicePerStageDescriptorSetFeaturesNV(const VkPhysicalDevicePerStageDescriptorSetFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageProcessing2FeaturesQCOM(const VkPhysicalDeviceImageProcessing2FeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageProcessing2PropertiesQCOM(const VkPhysicalDeviceImageProcessing2PropertiesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerBlockMatchWindowCreateInfoQCOM(const VkSamplerBlockMatchWindowCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCubicWeightsFeaturesQCOM(const VkPhysicalDeviceCubicWeightsFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerCubicWeightsCreateInfoQCOM(const VkSamplerCubicWeightsCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBlitImageCubicWeightsInfoQCOM(const VkBlitImageCubicWeightsInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceYcbcrDegammaFeaturesQCOM(const VkPhysicalDeviceYcbcrDegammaFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM(const VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCubicClampFeaturesQCOM(const VkPhysicalDeviceCubicClampFeaturesQCOM& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT(const VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkScreenBufferPropertiesQNX(const VkScreenBufferPropertiesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkScreenBufferFormatPropertiesQNX(const VkScreenBufferFormatPropertiesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkImportScreenBufferInfoQNX(const VkImportScreenBufferInfoQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkExternalFormatQNX(const VkExternalFormatQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX(const VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_VkPhysicalDeviceLayeredDriverPropertiesMSFT(const VkPhysicalDeviceLayeredDriverPropertiesMSFT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkSetDescriptorBufferOffsetsInfoEXT(const VkSetDescriptorBufferOffsetsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindDescriptorBufferEmbeddedSamplersInfoEXT(const VkBindDescriptorBufferEmbeddedSamplersInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV(const VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplaySurfaceStereoCreateInfoNV(const VkDisplaySurfaceStereoCreateInfoNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDisplayModeStereoPropertiesNV(const VkDisplayModeStereoPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeQuantizationMapCapabilitiesKHR(const VkVideoEncodeQuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoFormatQuantizationMapPropertiesKHR(const VkVideoFormatQuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeQuantizationMapInfoKHR(const VkVideoEncodeQuantizationMapInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR(const VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR(const VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH264QuantizationMapCapabilitiesKHR(const VkVideoEncodeH264QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeH265QuantizationMapCapabilitiesKHR(const VkVideoEncodeH265QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoFormatH265QuantizationMapPropertiesKHR(const VkVideoFormatH265QuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoEncodeAV1QuantizationMapCapabilitiesKHR(const VkVideoEncodeAV1QuantizationMapCapabilitiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkVideoFormatAV1QuantizationMapPropertiesKHR(const VkVideoFormatAV1QuantizationMapPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRawAccessChainsFeaturesNV(const VkPhysicalDeviceRawAccessChainsFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR(const VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCommandBufferInheritanceFeaturesNV(const VkPhysicalDeviceCommandBufferInheritanceFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance7FeaturesKHR(const VkPhysicalDeviceMaintenance7FeaturesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceMaintenance7PropertiesKHR(const VkPhysicalDeviceMaintenance7PropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLayeredApiPropertiesKHR(const VkPhysicalDeviceLayeredApiPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLayeredApiPropertiesListKHR(const VkPhysicalDeviceLayeredApiPropertiesListKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceLayeredApiVulkanPropertiesKHR(const VkPhysicalDeviceLayeredApiVulkanPropertiesKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV(const VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT(const VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceRayTracingValidationFeaturesNV(const VkPhysicalDeviceRayTracingValidationFeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT(const VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT(const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeneratedCommandsMemoryRequirementsInfoEXT(const VkGeneratedCommandsMemoryRequirementsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetPipelineInfoEXT(const VkIndirectExecutionSetPipelineInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetShaderLayoutInfoEXT(const VkIndirectExecutionSetShaderLayoutInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetShaderInfoEXT(const VkIndirectExecutionSetShaderInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetCreateInfoEXT(const VkIndirectExecutionSetCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeneratedCommandsInfoEXT(const VkGeneratedCommandsInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkWriteIndirectExecutionSetPipelineEXT(const VkWriteIndirectExecutionSetPipelineEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsPushConstantTokenEXT(const VkIndirectCommandsPushConstantTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsVertexBufferTokenEXT(const VkIndirectCommandsVertexBufferTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsIndexBufferTokenEXT(const VkIndirectCommandsIndexBufferTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsExecutionSetTokenEXT(const VkIndirectCommandsExecutionSetTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutTokenEXT(const VkIndirectCommandsLayoutTokenEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsLayoutCreateInfoEXT(const VkIndirectCommandsLayoutCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDrawIndirectCountIndirectCommandEXT(const VkDrawIndirectCountIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindVertexBufferIndirectCommandEXT(const VkBindVertexBufferIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkBindIndexBufferIndirectCommandEXT(const VkBindIndexBufferIndirectCommandEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeneratedCommandsPipelineInfoEXT(const VkGeneratedCommandsPipelineInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkGeneratedCommandsShaderInfoEXT(const VkGeneratedCommandsShaderInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkWriteIndirectExecutionSetShaderEXT(const VkWriteIndirectExecutionSetShaderEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageAlignmentControlFeaturesMESA(const VkPhysicalDeviceImageAlignmentControlFeaturesMESA& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceImageAlignmentControlPropertiesMESA(const VkPhysicalDeviceImageAlignmentControlPropertiesMESA& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkImageAlignmentControlCreateInfoMESA(const VkImageAlignmentControlCreateInfoMESA& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceDepthClampControlFeaturesEXT(const VkPhysicalDeviceDepthClampControlFeaturesEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineViewportDepthClampControlCreateInfoEXT(const VkPipelineViewportDepthClampControlCreateInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceHdrVividFeaturesHUAWEI(const VkPhysicalDeviceHdrVividFeaturesHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkHdrVividDynamicMetadataHUAWEI(const VkHdrVividDynamicMetadataHUAWEI& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkCooperativeMatrixFlexibleDimensionsPropertiesNV(const VkCooperativeMatrixFlexibleDimensionsPropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCooperativeMatrix2FeaturesNV(const VkPhysicalDeviceCooperativeMatrix2FeaturesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceCooperativeMatrix2PropertiesNV(const VkPhysicalDeviceCooperativeMatrix2PropertiesNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT(const VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT& object, const ApiDumpSettings& settings, int indents);

//========================== Union Implementations ==========================//

void dump_json_VkClearColorValue(const VkClearColorValue& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkClearValue(const VkClearValue& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceCounterResultKHR(const VkPerformanceCounterResultKHR& object, const ApiDumpSettings& settings, int indents);
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_VkDeviceOrHostAddressConstAMDX(const VkDeviceOrHostAddressConstAMDX& object, const ApiDumpSettings& settings, int indents);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_VkDeviceOrHostAddressKHR(const VkDeviceOrHostAddressKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDeviceOrHostAddressConstKHR(const VkDeviceOrHostAddressConstKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureGeometryDataKHR(const VkAccelerationStructureGeometryDataKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPerformanceValueDataINTEL(const VkPerformanceValueDataINTEL& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkPipelineExecutableStatisticValueKHR(const VkPipelineExecutableStatisticValueKHR& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkDescriptorDataEXT(const VkDescriptorDataEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkAccelerationStructureMotionInstanceDataNV(const VkAccelerationStructureMotionInstanceDataNV& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectExecutionSetInfoEXT(const VkIndirectExecutionSetInfoEXT& object, const ApiDumpSettings& settings, int indents);
void dump_json_VkIndirectCommandsTokenDataEXT(const VkIndirectCommandsTokenDataEXT& object, const ApiDumpSettings& settings, int indents);

//======================== pNext Chain Implementation =======================//

void dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);

//========================= Function Implementations ========================//

void dump_json_vkCreateInstance(ApiDumpInstance& dump_inst, VkResult result, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
void dump_json_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator);
void dump_json_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
void dump_json_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
void dump_json_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
void dump_json_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);
void dump_json_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
void dump_json_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
void dump_json_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
void dump_json_vkCreateDevice(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
void dump_json_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator);
void dump_json_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_json_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
void dump_json_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_json_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);
void dump_json_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
void dump_json_vkQueueSubmit(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
void dump_json_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue);
void dump_json_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkDevice device);
void dump_json_vkAllocateMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
void dump_json_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
void dump_json_vkMapMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
void dump_json_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory);
void dump_json_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_json_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
void dump_json_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);
void dump_json_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_json_vkBindImageMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
void dump_json_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
void dump_json_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
void dump_json_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);
void dump_json_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);
void dump_json_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence);
void dump_json_vkCreateFence(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
void dump_json_vkResetFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences);
void dump_json_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFence fence);
void dump_json_vkWaitForFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
void dump_json_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
void dump_json_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent);
void dump_json_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetEventStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_json_vkSetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_json_vkResetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event);
void dump_json_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);
void dump_json_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_json_vkCreateBuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
void dump_json_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateBufferView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView);
void dump_json_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
void dump_json_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
void dump_json_vkCreateImageView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
void dump_json_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
void dump_json_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);
void dump_json_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);
void dump_json_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches);
void dump_json_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
void dump_json_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateSampler(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
void dump_json_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
void dump_json_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
void dump_json_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
void dump_json_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
void dump_json_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
void dump_json_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
void dump_json_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
void dump_json_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
void dump_json_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);
void dump_json_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
void dump_json_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
void dump_json_vkResetCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);
void dump_json_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
void dump_json_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_json_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
void dump_json_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer);
void dump_json_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
void dump_json_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
void dump_json_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth);
void dump_json_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
void dump_json_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void dump_json_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);
void dump_json_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);
void dump_json_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);
void dump_json_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);
void dump_json_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
void dump_json_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
void dump_json_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void dump_json_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
void dump_json_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void dump_json_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_json_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
void dump_json_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
void dump_json_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
void dump_json_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_json_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
void dump_json_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void dump_json_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void dump_json_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_json_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void dump_json_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void dump_json_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);
void dump_json_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_json_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);
void dump_json_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_json_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
void dump_json_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
void dump_json_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void dump_json_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
void dump_json_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
void dump_json_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void dump_json_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void dump_json_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void dump_json_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
void dump_json_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_json_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_json_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_json_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_json_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_json_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_json_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_json_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_json_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_json_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_json_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_json_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_json_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_json_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);
void dump_json_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_json_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_json_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_json_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_json_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
void dump_json_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
void dump_json_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
void dump_json_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_json_vkCmdDrawIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirectCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCreateRenderPass2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_vkCmdBeginRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_json_vkCmdNextSubpass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_json_vkCmdEndRenderPass2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_json_vkResetQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_vkGetSemaphoreCounterValue(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_json_vkWaitSemaphores(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_json_vkSignalSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_json_vkGetBufferDeviceAddress(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetBufferOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetDeviceMemoryOpaqueCaptureAddress(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_json_vkGetPhysicalDeviceToolProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_json_vkCreatePrivateDataSlot(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_json_vkDestroyPrivateDataSlot(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_json_vkSetPrivateData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_json_vkGetPrivateData(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_json_vkCmdSetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_json_vkCmdResetEvent2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_json_vkCmdWaitEvents2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_json_vkCmdPipelineBarrier2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_json_vkCmdWriteTimestamp2(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_json_vkQueueSubmit2(ApiDumpInstance& dump_inst, VkResult result, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_json_vkCmdCopyBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_json_vkCmdCopyImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_json_vkCmdCopyBufferToImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_json_vkCmdCopyImageToBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_json_vkCmdBlitImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_json_vkCmdResolveImage2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_json_vkCmdBeginRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_json_vkCmdEndRendering(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
void dump_json_vkCmdSetCullMode(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_json_vkCmdSetFrontFace(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_json_vkCmdSetPrimitiveTopology(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_json_vkCmdSetViewportWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_vkCmdSetScissorWithCount(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_vkCmdBindVertexBuffers2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_json_vkCmdSetDepthTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_json_vkCmdSetDepthWriteEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_json_vkCmdSetDepthCompareOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_json_vkCmdSetDepthBoundsTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_json_vkCmdSetStencilTestEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_json_vkCmdSetStencilOp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_json_vkCmdSetRasterizerDiscardEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_json_vkCmdSetDepthBiasEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_json_vkCmdSetPrimitiveRestartEnable(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
void dump_json_vkGetDeviceBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkCmdSetLineStipple(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_vkMapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_json_vkUnmapMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_json_vkCmdBindIndexBuffer2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_json_vkGetRenderingAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_json_vkGetDeviceImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_json_vkGetImageSubresourceLayout2(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_vkCmdPushDescriptorSet(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_json_vkCmdPushDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_json_vkCmdSetRenderingAttachmentLocations(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_json_vkCmdSetRenderingInputAttachmentIndices(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_json_vkCmdBindDescriptorSets2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_json_vkCmdPushConstants2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_json_vkCmdPushDescriptorSet2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_json_vkCmdPushDescriptorSetWithTemplate2(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_json_vkCopyMemoryToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_json_vkCopyImageToMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_json_vkCopyImageToImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_json_vkTransitionImageLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_json_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
void dump_json_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
void dump_json_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
void dump_json_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
void dump_json_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
void dump_json_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
void dump_json_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
void dump_json_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);
void dump_json_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);
void dump_json_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);
void dump_json_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);
void dump_json_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);
void dump_json_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);
void dump_json_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);
void dump_json_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);
void dump_json_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);
void dump_json_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);
void dump_json_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);
void dump_json_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_json_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
void dump_json_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_XCB_KHR)
void dump_json_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif // VK_USE_PLATFORM_XCB_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
void dump_json_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void dump_json_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
void dump_json_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
void dump_json_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo);
void dump_json_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo);
void dump_json_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_json_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void dump_json_vkGetPhysicalDeviceVideoCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);
void dump_json_vkGetPhysicalDeviceVideoFormatPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);
void dump_json_vkCreateVideoSessionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);
void dump_json_vkDestroyVideoSessionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetVideoSessionMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);
void dump_json_vkBindVideoSessionMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);
void dump_json_vkCreateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);
void dump_json_vkUpdateVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);
void dump_json_vkDestroyVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdBeginVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo);
void dump_json_vkCmdEndVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo);
void dump_json_vkCmdControlVideoCodingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo);
void dump_json_vkCmdDecodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo);
void dump_json_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes);
void dump_json_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_json_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets);
void dump_json_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);
void dump_json_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);
void dump_json_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);
void dump_json_vkCreateCuModuleNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);
void dump_json_vkCreateCuFunctionNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);
void dump_json_vkDestroyCuModuleNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator);
void dump_json_vkDestroyCuFunctionNVX(ApiDumpInstance& dump_inst, VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdCuLaunchKernelNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo);
void dump_json_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_json_vkGetImageViewHandle64NVX(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo);
void dump_json_vkGetImageViewAddressNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);
void dump_json_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);
void dump_json_vkCmdBeginRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo);
void dump_json_vkCmdEndRenderingKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer);
#if defined(VK_USE_PLATFORM_GGP)
void dump_json_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_GGP
void dump_json_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);
void dump_json_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);
void dump_json_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);
void dump_json_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);
void dump_json_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);
void dump_json_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);
void dump_json_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);
void dump_json_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);
void dump_json_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void dump_json_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
#if defined(VK_USE_PLATFORM_VI_NN)
void dump_json_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_VI_NN
void dump_json_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);
void dump_json_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
void dump_json_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);
void dump_json_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
void dump_json_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void dump_json_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
void dump_json_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);
void dump_json_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);
void dump_json_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator);
void dump_json_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData);
void dump_json_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void dump_json_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_json_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
void dump_json_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
void dump_json_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);
void dump_json_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo);
void dump_json_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
void dump_json_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);
void dump_json_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);
void dump_json_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);
void dump_json_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles);
void dump_json_vkCmdSetDiscardRectangleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);
void dump_json_vkCmdSetDiscardRectangleModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);
void dump_json_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata);
void dump_json_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
void dump_json_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo);
void dump_json_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_json_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo);
void dump_json_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
void dump_json_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo);
void dump_json_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);
void dump_json_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);
void dump_json_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
void dump_json_vkAcquireProfilingLockKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo);
void dump_json_vkReleaseProfilingLockKHR(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);
void dump_json_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);
void dump_json_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);
void dump_json_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);
void dump_json_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);
void dump_json_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);
#if defined(VK_USE_PLATFORM_IOS_MVK)
void dump_json_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_MACOS_MVK)
void dump_json_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_MACOS_MVK
void dump_json_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);
void dump_json_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo);
void dump_json_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue);
void dump_json_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo);
void dump_json_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void dump_json_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);
void dump_json_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCreateExecutionGraphPipelinesAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipelineCache                 pipelineCache, uint32_t                                        createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks*    pAllocator, VkPipeline*               pPipelines);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkGetExecutionGraphPipelineScratchSizeAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipeline                                      executionGraph, VkExecutionGraphPipelineScratchSizeAMDX*        pSizeInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkGetExecutionGraphPipelineNodeIndexAMDX(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                        device, VkPipeline                                      executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX*  pNodeInfo, uint32_t*                                       pNodeIndex);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdInitializeGraphScratchMemoryAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkPipeline                                      executionGraph, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdDispatchGraphAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdDispatchGraphIndirectAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, const VkDispatchGraphCountInfoAMDX*             pCountInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
#if defined(VK_ENABLE_BETA_EXTENSIONS)
void dump_json_vkCmdDispatchGraphIndirectCountAMDX(ApiDumpInstance& dump_inst, VkCommandBuffer                                 commandBuffer, VkDeviceAddress                                 scratch, VkDeviceSize                                    scratchSize, VkDeviceAddress                                 countInfo);
#endif // VK_ENABLE_BETA_EXTENSIONS
void dump_json_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo);
void dump_json_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);
void dump_json_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkCreateAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, const VkAccelerationStructureCreateInfoKHR*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkAccelerationStructureKHR*                        pAccelerationStructure);
void dump_json_vkDestroyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_json_vkCmdBuildAccelerationStructuresIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts);
void dump_json_vkBuildAccelerationStructuresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos);
void dump_json_vkCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_json_vkCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_json_vkCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_json_vkWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_json_vkCmdCopyAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo);
void dump_json_vkCmdCopyAccelerationStructureToMemoryKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);
void dump_json_vkCmdCopyMemoryToAccelerationStructureKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);
void dump_json_vkGetAccelerationStructureDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo);
void dump_json_vkCmdWriteAccelerationStructuresPropertiesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_vkGetDeviceAccelerationStructureCompatibilityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_json_vkGetAccelerationStructureBuildSizesKHR(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkAccelerationStructureBuildGeometryInfoKHR*  pBuildInfo, const uint32_t*  pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR*           pSizeInfo);
void dump_json_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);
void dump_json_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator);
void dump_json_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos);
void dump_json_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos);
void dump_json_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);
void dump_json_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);
void dump_json_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator);
void dump_json_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches);
void dump_json_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);
void dump_json_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_json_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes);
void dump_json_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders);
void dump_json_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);
void dump_json_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);
void dump_json_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos);
void dump_json_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);
void dump_json_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);
void dump_json_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);
void dump_json_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkGetRayTracingShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);
void dump_json_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t shader);
void dump_json_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
void dump_json_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);
void dump_json_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);
void dump_json_vkCmdWriteBufferMarker2AMD(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker);
void dump_json_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_json_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);
void dump_json_vkGetCalibratedTimestampsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_json_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);
void dump_json_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);
void dump_json_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdSetExclusiveScissorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables);
void dump_json_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors);
void dump_json_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker);
void dump_json_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);
void dump_json_vkGetQueueCheckpointData2NV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);
void dump_json_vkGetSemaphoreCounterValueKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSemaphore semaphore, uint64_t* pValue);
void dump_json_vkWaitSemaphoresKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);
void dump_json_vkSignalSemaphoreKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo);
void dump_json_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo);
void dump_json_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device);
void dump_json_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo);
void dump_json_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);
void dump_json_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo);
void dump_json_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);
void dump_json_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceConfigurationINTEL configuration);
void dump_json_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, VkPerformanceConfigurationINTEL configuration);
void dump_json_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);
void dump_json_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_vkGetPhysicalDeviceFragmentShadingRatesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
void dump_json_vkCmdSetFragmentShadingRateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_json_vkCmdSetRenderingAttachmentLocationsKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo);
void dump_json_vkCmdSetRenderingInputAttachmentIndicesKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
void dump_json_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);
void dump_json_vkWaitForPresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);
void dump_json_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
void dump_json_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);
#endif // VK_USE_PLATFORM_WIN32_KHR
void dump_json_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
void dump_json_vkGetBufferDeviceAddressKHR(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetBufferOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkBufferDeviceAddressInfo* pInfo);
void dump_json_vkGetDeviceMemoryOpaqueCaptureAddressKHR(ApiDumpInstance& dump_inst, uint64_t result, VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);
void dump_json_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
void dump_json_vkCmdSetCullModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);
void dump_json_vkCmdSetFrontFaceEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkFrontFace frontFace);
void dump_json_vkCmdSetPrimitiveTopologyEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);
void dump_json_vkCmdSetViewportWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports);
void dump_json_vkCmdSetScissorWithCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors);
void dump_json_vkCmdBindVertexBuffers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);
void dump_json_vkCmdSetDepthTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);
void dump_json_vkCmdSetDepthWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);
void dump_json_vkCmdSetDepthCompareOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);
void dump_json_vkCmdSetDepthBoundsTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);
void dump_json_vkCmdSetStencilTestEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);
void dump_json_vkCmdSetStencilOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);
void dump_json_vkCreateDeferredOperationKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);
void dump_json_vkDestroyDeferredOperationKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetDeferredOperationMaxConcurrencyKHR(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_vkGetDeferredOperationResultKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_vkDeferredOperationJoinKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR operation);
void dump_json_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);
void dump_json_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);
void dump_json_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);
void dump_json_vkCopyMemoryToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyMemoryToImageInfo*    pCopyMemoryToImageInfo);
void dump_json_vkCopyImageToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToMemoryInfo*    pCopyImageToMemoryInfo);
void dump_json_vkCopyImageToImageEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCopyImageToImageInfo*    pCopyImageToImageInfo);
void dump_json_vkTransitionImageLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo*    pTransitions);
void dump_json_vkGetImageSubresourceLayout2EXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_vkMapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData);
void dump_json_vkUnmapMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo);
void dump_json_vkReleaseSwapchainImagesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);
void dump_json_vkGetGeneratedCommandsMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdPreprocessGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_json_vkCmdExecuteGeneratedCommandsNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);
void dump_json_vkCmdBindPipelineShaderGroupNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);
void dump_json_vkCreateIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);
void dump_json_vkDestroyIndirectCommandsLayoutNV(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdSetDepthBias2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT*         pDepthBiasInfo);
void dump_json_vkAcquireDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);
void dump_json_vkGetDrmDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);
void dump_json_vkCreatePrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);
void dump_json_vkDestroyPrivateDataSlotEXT(ApiDumpInstance& dump_inst, VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator);
void dump_json_vkSetPrivateDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);
void dump_json_vkGetPrivateDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);
void dump_json_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);
void dump_json_vkGetEncodedVideoSessionParametersKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);
void dump_json_vkCmdEncodeVideoKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo);
void dump_json_vkCreateCudaModuleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);
void dump_json_vkGetCudaModuleCacheNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);
void dump_json_vkCreateCudaFunctionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);
void dump_json_vkDestroyCudaModuleNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator);
void dump_json_vkDestroyCudaFunctionNV(ApiDumpInstance& dump_inst, VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdCudaLaunchKernelNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo);
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_vkExportMetalObjectsEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);
#endif // VK_USE_PLATFORM_METAL_EXT
void dump_json_vkCmdSetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo);
void dump_json_vkCmdResetEvent2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask);
void dump_json_vkCmdWaitEvents2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos);
void dump_json_vkCmdPipelineBarrier2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo);
void dump_json_vkCmdWriteTimestamp2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query);
void dump_json_vkQueueSubmit2KHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue                           queue, uint32_t                            submitCount, const VkSubmitInfo2*              pSubmits, VkFence           fence);
void dump_json_vkGetDescriptorSetLayoutSizeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);
void dump_json_vkGetDescriptorSetLayoutBindingOffsetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);
void dump_json_vkGetDescriptorEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);
void dump_json_vkCmdBindDescriptorBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos);
void dump_json_vkCmdSetDescriptorBufferOffsetsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets);
void dump_json_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);
void dump_json_vkGetBufferOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetImageOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetImageViewOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetSamplerOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);
void dump_json_vkCmdSetFragmentShadingRateEnumNV(ApiDumpInstance& dump_inst, VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]);
void dump_json_vkCmdDrawMeshTasksEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkCmdDrawMeshTasksIndirectEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void dump_json_vkCmdDrawMeshTasksIndirectCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
void dump_json_vkCmdCopyBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo);
void dump_json_vkCmdCopyImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo);
void dump_json_vkCmdCopyBufferToImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);
void dump_json_vkCmdCopyImageToBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);
void dump_json_vkCmdBlitImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo);
void dump_json_vkCmdResolveImage2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo);
void dump_json_vkGetDeviceFaultInfoEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkAcquireWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
void dump_json_vkGetWinrtDisplayNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_vkCreateDirectFBSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT)
void dump_json_vkGetPhysicalDeviceDirectFBPresentationSupportEXT(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);
#endif // VK_USE_PLATFORM_DIRECTFB_EXT
void dump_json_vkCmdTraceRaysKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);
void dump_json_vkCreateRayTracingPipelinesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
void dump_json_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);
void dump_json_vkCmdTraceRaysIndirectKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);
void dump_json_vkGetRayTracingShaderGroupStackSizeKHR(ApiDumpInstance& dump_inst, VkDeviceSize result, VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);
void dump_json_vkCmdSetRayTracingPipelineStackSizeKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);
void dump_json_vkCmdSetVertexInputEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetMemoryZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetMemoryZirconHandlePropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkImportSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetSemaphoreZirconHandleFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkCreateBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkSetBufferCollectionImageConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkSetBufferCollectionBufferConstraintsFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkDestroyBufferCollectionFUCHSIA(ApiDumpInstance& dump_inst, VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator);
#endif // VK_USE_PLATFORM_FUCHSIA
#if defined(VK_USE_PLATFORM_FUCHSIA)
void dump_json_vkGetBufferCollectionPropertiesFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);
#endif // VK_USE_PLATFORM_FUCHSIA
void dump_json_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);
void dump_json_vkCmdSubpassShadingHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer);
void dump_json_vkCmdBindInvocationMaskHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);
void dump_json_vkGetMemoryRemoteAddressNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);
void dump_json_vkGetPipelinePropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);
void dump_json_vkCmdSetPatchControlPointsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t patchControlPoints);
void dump_json_vkCmdSetRasterizerDiscardEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);
void dump_json_vkCmdSetDepthBiasEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);
void dump_json_vkCmdSetLogicOpEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLogicOp logicOp);
void dump_json_vkCmdSetPrimitiveRestartEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_vkCreateScreenSurfaceQNX(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif // VK_USE_PLATFORM_SCREEN_QNX
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_vkGetPhysicalDeviceScreenPresentationSupportQNX(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_vkCmdSetColorWriteEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables);
void dump_json_vkCmdTraceRaysIndirect2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);
void dump_json_vkCmdDrawMultiEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);
void dump_json_vkCmdDrawMultiIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset);
void dump_json_vkCreateMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, const VkMicromapCreateInfoEXT*        pCreateInfo, const VkAllocationCallbacks*       pAllocator, VkMicromapEXT*                        pMicromap);
void dump_json_vkDestroyMicromapEXT(ApiDumpInstance& dump_inst, VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCmdBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_json_vkBuildMicromapsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice                                           device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos);
void dump_json_vkCopyMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo);
void dump_json_vkCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_json_vkCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_json_vkWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType  queryType, size_t       dataSize, void* pData, size_t stride);
void dump_json_vkCmdCopyMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo);
void dump_json_vkCmdCopyMicromapToMemoryEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo);
void dump_json_vkCmdCopyMemoryToMicromapEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo);
void dump_json_vkCmdWriteMicromapsPropertiesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);
void dump_json_vkGetDeviceMicromapCompatibilityEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);
void dump_json_vkGetMicromapBuildSizesEXT(ApiDumpInstance& dump_inst, VkDevice                                            device, VkAccelerationStructureBuildTypeKHR                 buildType, const VkMicromapBuildInfoEXT*  pBuildInfo, VkMicromapBuildSizesInfoEXT*           pSizeInfo);
void dump_json_vkCmdDrawClusterHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
void dump_json_vkCmdDrawClusterIndirectHUAWEI(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);
void dump_json_vkSetDeviceMemoryPriorityEXT(ApiDumpInstance& dump_inst, VkDevice       device, VkDeviceMemory memory, float          priority);
void dump_json_vkGetDeviceBufferMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkGetDeviceImageSparseMemoryRequirementsKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);
void dump_json_vkGetDescriptorSetLayoutHostMappingInfoVALVE(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);
void dump_json_vkGetDescriptorSetHostMappingVALVE(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, void** ppData);
void dump_json_vkCmdCopyMemoryIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);
void dump_json_vkCmdCopyMemoryToImageIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources);
void dump_json_vkCmdDecompressMemoryNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions);
void dump_json_vkCmdDecompressMemoryIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);
void dump_json_vkGetPipelineIndirectMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdUpdatePipelineIndirectBufferNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline);
void dump_json_vkGetPipelineIndirectDeviceAddressNV(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo);
void dump_json_vkCmdSetDepthClampEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);
void dump_json_vkCmdSetPolygonModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);
void dump_json_vkCmdSetRasterizationSamplesEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples);
void dump_json_vkCmdSetSampleMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask);
void dump_json_vkCmdSetAlphaToCoverageEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);
void dump_json_vkCmdSetAlphaToOneEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);
void dump_json_vkCmdSetLogicOpEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);
void dump_json_vkCmdSetColorBlendEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables);
void dump_json_vkCmdSetColorBlendEquationEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations);
void dump_json_vkCmdSetColorWriteMaskEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks);
void dump_json_vkCmdSetTessellationDomainOriginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);
void dump_json_vkCmdSetRasterizationStreamEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t rasterizationStream);
void dump_json_vkCmdSetConservativeRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);
void dump_json_vkCmdSetExtraPrimitiveOverestimationSizeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);
void dump_json_vkCmdSetDepthClipEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);
void dump_json_vkCmdSetSampleLocationsEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);
void dump_json_vkCmdSetColorBlendAdvancedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced);
void dump_json_vkCmdSetProvokingVertexModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);
void dump_json_vkCmdSetLineRasterizationModeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);
void dump_json_vkCmdSetLineStippleEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);
void dump_json_vkCmdSetDepthClipNegativeOneToOneEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);
void dump_json_vkCmdSetViewportWScalingEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);
void dump_json_vkCmdSetViewportSwizzleNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles);
void dump_json_vkCmdSetCoverageToColorEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);
void dump_json_vkCmdSetCoverageToColorLocationNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);
void dump_json_vkCmdSetCoverageModulationModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);
void dump_json_vkCmdSetCoverageModulationTableEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);
void dump_json_vkCmdSetCoverageModulationTableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable);
void dump_json_vkCmdSetShadingRateImageEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);
void dump_json_vkCmdSetRepresentativeFragmentTestEnableNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);
void dump_json_vkCmdSetCoverageReductionModeNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);
void dump_json_vkGetShaderModuleIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_json_vkGetShaderModuleCreateInfoIdentifierEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);
void dump_json_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
void dump_json_vkCreateOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);
void dump_json_vkDestroyOpticalFlowSessionNV(ApiDumpInstance& dump_inst, VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator);
void dump_json_vkBindOpticalFlowSessionImageNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);
void dump_json_vkCmdOpticalFlowExecuteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo);
void dump_json_vkCmdBindIndexBuffer2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
void dump_json_vkGetRenderingAreaGranularityKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity);
void dump_json_vkGetDeviceImageSubresourceLayoutKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout);
void dump_json_vkGetImageSubresourceLayout2KHR(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout);
void dump_json_vkAntiLagUpdateAMD(ApiDumpInstance& dump_inst, VkDevice device, const VkAntiLagDataAMD* pData);
void dump_json_vkCreateShadersEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);
void dump_json_vkDestroyShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetShaderBinaryDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);
void dump_json_vkCmdBindShadersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders);
void dump_json_vkCmdSetDepthClampRangeEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange);
void dump_json_vkCreatePipelineBinariesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries);
void dump_json_vkDestroyPipelineBinaryKHR(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetPipelineKeyKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey);
void dump_json_vkGetPipelineBinaryDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData);
void dump_json_vkReleaseCapturedPipelineDataKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator);
void dump_json_vkGetFramebufferTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);
void dump_json_vkGetDynamicRenderingTilePropertiesQCOM(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);
void dump_json_vkSetLatencySleepModeNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo);
void dump_json_vkLatencySleepNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo);
void dump_json_vkSetLatencyMarkerNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_json_vkGetLatencyTimingsNV(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);
void dump_json_vkQueueNotifyOutOfBandNV(ApiDumpInstance& dump_inst, VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);
void dump_json_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);
void dump_json_vkCmdSetAttachmentFeedbackLoopEnableEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);
#if defined(VK_USE_PLATFORM_SCREEN_QNX)
void dump_json_vkGetScreenBufferPropertiesQNX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);
#endif // VK_USE_PLATFORM_SCREEN_QNX
void dump_json_vkCmdSetLineStippleKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);
void dump_json_vkCmdBindDescriptorSets2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo*   pBindDescriptorSetsInfo);
void dump_json_vkCmdPushConstants2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushConstantsInfo*        pPushConstantsInfo);
void dump_json_vkCmdPushDescriptorSet2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo*    pPushDescriptorSetInfo);
void dump_json_vkCmdPushDescriptorSetWithTemplate2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);
void dump_json_vkCmdSetDescriptorBufferOffsets2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);
void dump_json_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);
void dump_json_vkGetGeneratedCommandsMemoryRequirementsEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements);
void dump_json_vkCmdPreprocessGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer);
void dump_json_vkCmdExecuteGeneratedCommandsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo);
void dump_json_vkCreateIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout);
void dump_json_vkDestroyIndirectCommandsLayoutEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator);
void dump_json_vkCreateIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet);
void dump_json_vkDestroyIndirectExecutionSetEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator);
void dump_json_vkUpdateIndirectExecutionSetPipelineEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites);
void dump_json_vkUpdateIndirectExecutionSetShaderEXT(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites);
void dump_json_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties);


