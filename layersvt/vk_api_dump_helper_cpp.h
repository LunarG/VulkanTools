/* THIS FILE IS GENERATED.  DO NOT EDIT. */

/*
 * Vulkan
 *
 * Copyright (c) 2015-2016 The Khronos Group Inc.
 * Copyright (c) 2015-2016 Valve Corporation.
 * Copyright (c) 2015-2016 LunarG, Inc.
 * Copyright (c) 2015-2016 Google Inc.
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
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 * Author: Tobin Ehlis <tobin@lunarg.com>
 */
//#includes, #defines, globals and such...
#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include "vk_enum_string_helper.h"
namespace StreamControl
{
bool writeAddress = true;
template <typename T>
std::ostream& operator<< (std::ostream &out, T const* pointer)
{
    if(writeAddress)
    {
        out.operator<<(pointer);
    }
    else
    {
        std::operator<<(out, "address");
    }
    return out;
}
template <typename HandleType> uint64_t HandleCast(HandleType * handle)
{
    return reinterpret_cast<uint64_t>(handle);
}
uint64_t HandleCast(uint64_t handle)
{
    return handle;
}
std::ostream& operator<<(std::ostream &out, char const*const s)
{
    return std::operator<<(out, s);
}
}

std::string dynamic_display(const void* pStruct, const std::string prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #914
std::string vk_print_vkallocationcallbacks(const VkAllocationCallbacks* pStruct, const std::string prefix);
#ifdef VK_USE_PLATFORM_ANDROID_KHR
std::string vk_print_vkandroidsurfacecreateinfokhr(const VkAndroidSurfaceCreateInfoKHR* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_ANDROID_KHR
std::string vk_print_vkapplicationinfo(const VkApplicationInfo* pStruct, const std::string prefix);
std::string vk_print_vkattachmentdescription(const VkAttachmentDescription* pStruct, const std::string prefix);
std::string vk_print_vkattachmentreference(const VkAttachmentReference* pStruct, const std::string prefix);
std::string vk_print_vkbindsparseinfo(const VkBindSparseInfo* pStruct, const std::string prefix);
std::string vk_print_vkbuffercopy(const VkBufferCopy* pStruct, const std::string prefix);
std::string vk_print_vkbuffercreateinfo(const VkBufferCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkbufferimagecopy(const VkBufferImageCopy* pStruct, const std::string prefix);
std::string vk_print_vkbuffermemorybarrier(const VkBufferMemoryBarrier* pStruct, const std::string prefix);
std::string vk_print_vkbufferviewcreateinfo(const VkBufferViewCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkclearattachment(const VkClearAttachment* pStruct, const std::string prefix);
std::string vk_print_vkclearcolorvalue(const VkClearColorValue* pStruct, const std::string prefix);
std::string vk_print_vkcleardepthstencilvalue(const VkClearDepthStencilValue* pStruct, const std::string prefix);
std::string vk_print_vkclearrect(const VkClearRect* pStruct, const std::string prefix);
std::string vk_print_vkclearvalue(const VkClearValue* pStruct, const std::string prefix);
std::string vk_print_vkcommandbufferallocateinfo(const VkCommandBufferAllocateInfo* pStruct, const std::string prefix);
std::string vk_print_vkcommandbufferbegininfo(const VkCommandBufferBeginInfo* pStruct, const std::string prefix);
std::string vk_print_vkcommandbufferinheritanceinfo(const VkCommandBufferInheritanceInfo* pStruct, const std::string prefix);
std::string vk_print_vkcommandpoolcreateinfo(const VkCommandPoolCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkcomponentmapping(const VkComponentMapping* pStruct, const std::string prefix);
std::string vk_print_vkcomputepipelinecreateinfo(const VkComputePipelineCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkcopydescriptorset(const VkCopyDescriptorSet* pStruct, const std::string prefix);
std::string vk_print_vkdebugmarkermarkerinfoext(const VkDebugMarkerMarkerInfoEXT* pStruct, const std::string prefix);
std::string vk_print_vkdebugmarkerobjectnameinfoext(const VkDebugMarkerObjectNameInfoEXT* pStruct, const std::string prefix);
std::string vk_print_vkdebugmarkerobjecttaginfoext(const VkDebugMarkerObjectTagInfoEXT* pStruct, const std::string prefix);
std::string vk_print_vkdebugreportcallbackcreateinfoext(const VkDebugReportCallbackCreateInfoEXT* pStruct, const std::string prefix);
std::string vk_print_vkdedicatedallocationbuffercreateinfonv(const VkDedicatedAllocationBufferCreateInfoNV* pStruct, const std::string prefix);
std::string vk_print_vkdedicatedallocationimagecreateinfonv(const VkDedicatedAllocationImageCreateInfoNV* pStruct, const std::string prefix);
std::string vk_print_vkdedicatedallocationmemoryallocateinfonv(const VkDedicatedAllocationMemoryAllocateInfoNV* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorbufferinfo(const VkDescriptorBufferInfo* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorimageinfo(const VkDescriptorImageInfo* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorpoolcreateinfo(const VkDescriptorPoolCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorpoolsize(const VkDescriptorPoolSize* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorsetallocateinfo(const VkDescriptorSetAllocateInfo* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorsetlayoutbinding(const VkDescriptorSetLayoutBinding* pStruct, const std::string prefix);
std::string vk_print_vkdescriptorsetlayoutcreateinfo(const VkDescriptorSetLayoutCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkdevicecreateinfo(const VkDeviceCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkdevicequeuecreateinfo(const VkDeviceQueueCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkdispatchindirectcommand(const VkDispatchIndirectCommand* pStruct, const std::string prefix);
std::string vk_print_vkdisplaymodecreateinfokhr(const VkDisplayModeCreateInfoKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplaymodeparameterskhr(const VkDisplayModeParametersKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplaymodepropertieskhr(const VkDisplayModePropertiesKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplayplanecapabilitieskhr(const VkDisplayPlaneCapabilitiesKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplayplanepropertieskhr(const VkDisplayPlanePropertiesKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplaypresentinfokhr(const VkDisplayPresentInfoKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplaypropertieskhr(const VkDisplayPropertiesKHR* pStruct, const std::string prefix);
std::string vk_print_vkdisplaysurfacecreateinfokhr(const VkDisplaySurfaceCreateInfoKHR* pStruct, const std::string prefix);
std::string vk_print_vkdrawindexedindirectcommand(const VkDrawIndexedIndirectCommand* pStruct, const std::string prefix);
std::string vk_print_vkdrawindirectcommand(const VkDrawIndirectCommand* pStruct, const std::string prefix);
std::string vk_print_vkeventcreateinfo(const VkEventCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkexportmemoryallocateinfonv(const VkExportMemoryAllocateInfoNV* pStruct, const std::string prefix);
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkexportmemorywin32handleinfonv(const VkExportMemoryWin32HandleInfoNV* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkextensionproperties(const VkExtensionProperties* pStruct, const std::string prefix);
std::string vk_print_vkextent2d(const VkExtent2D* pStruct, const std::string prefix);
std::string vk_print_vkextent3d(const VkExtent3D* pStruct, const std::string prefix);
std::string vk_print_vkexternalimageformatpropertiesnv(const VkExternalImageFormatPropertiesNV* pStruct, const std::string prefix);
std::string vk_print_vkexternalmemoryimagecreateinfonv(const VkExternalMemoryImageCreateInfoNV* pStruct, const std::string prefix);
std::string vk_print_vkfencecreateinfo(const VkFenceCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkformatproperties(const VkFormatProperties* pStruct, const std::string prefix);
std::string vk_print_vkframebuffercreateinfo(const VkFramebufferCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkgraphicspipelinecreateinfo(const VkGraphicsPipelineCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkimageblit(const VkImageBlit* pStruct, const std::string prefix);
std::string vk_print_vkimagecopy(const VkImageCopy* pStruct, const std::string prefix);
std::string vk_print_vkimagecreateinfo(const VkImageCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkimageformatproperties(const VkImageFormatProperties* pStruct, const std::string prefix);
std::string vk_print_vkimagememorybarrier(const VkImageMemoryBarrier* pStruct, const std::string prefix);
std::string vk_print_vkimageresolve(const VkImageResolve* pStruct, const std::string prefix);
std::string vk_print_vkimagesubresource(const VkImageSubresource* pStruct, const std::string prefix);
std::string vk_print_vkimagesubresourcelayers(const VkImageSubresourceLayers* pStruct, const std::string prefix);
std::string vk_print_vkimagesubresourcerange(const VkImageSubresourceRange* pStruct, const std::string prefix);
std::string vk_print_vkimageviewcreateinfo(const VkImageViewCreateInfo* pStruct, const std::string prefix);
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkimportmemorywin32handleinfonv(const VkImportMemoryWin32HandleInfoNV* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkinstancecreateinfo(const VkInstanceCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vklayerproperties(const VkLayerProperties* pStruct, const std::string prefix);
std::string vk_print_vkmappedmemoryrange(const VkMappedMemoryRange* pStruct, const std::string prefix);
std::string vk_print_vkmemoryallocateinfo(const VkMemoryAllocateInfo* pStruct, const std::string prefix);
std::string vk_print_vkmemorybarrier(const VkMemoryBarrier* pStruct, const std::string prefix);
std::string vk_print_vkmemoryheap(const VkMemoryHeap* pStruct, const std::string prefix);
std::string vk_print_vkmemoryrequirements(const VkMemoryRequirements* pStruct, const std::string prefix);
std::string vk_print_vkmemorytype(const VkMemoryType* pStruct, const std::string prefix);
#ifdef VK_USE_PLATFORM_MIR_KHR
std::string vk_print_vkmirsurfacecreateinfokhr(const VkMirSurfaceCreateInfoKHR* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_MIR_KHR
std::string vk_print_vkoffset2d(const VkOffset2D* pStruct, const std::string prefix);
std::string vk_print_vkoffset3d(const VkOffset3D* pStruct, const std::string prefix);
std::string vk_print_vkphysicaldevicefeatures(const VkPhysicalDeviceFeatures* pStruct, const std::string prefix);
std::string vk_print_vkphysicaldevicelimits(const VkPhysicalDeviceLimits* pStruct, const std::string prefix);
std::string vk_print_vkphysicaldevicememoryproperties(const VkPhysicalDeviceMemoryProperties* pStruct, const std::string prefix);
std::string vk_print_vkphysicaldeviceproperties(const VkPhysicalDeviceProperties* pStruct, const std::string prefix);
std::string vk_print_vkphysicaldevicesparseproperties(const VkPhysicalDeviceSparseProperties* pStruct, const std::string prefix);
std::string vk_print_vkpipelinecachecreateinfo(const VkPipelineCacheCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinecolorblendattachmentstate(const VkPipelineColorBlendAttachmentState* pStruct, const std::string prefix);
std::string vk_print_vkpipelinecolorblendstatecreateinfo(const VkPipelineColorBlendStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinedepthstencilstatecreateinfo(const VkPipelineDepthStencilStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinedynamicstatecreateinfo(const VkPipelineDynamicStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelineinputassemblystatecreateinfo(const VkPipelineInputAssemblyStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinelayoutcreateinfo(const VkPipelineLayoutCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinemultisamplestatecreateinfo(const VkPipelineMultisampleStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinerasterizationstatecreateinfo(const VkPipelineRasterizationStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinerasterizationstaterasterizationorderamd(const VkPipelineRasterizationStateRasterizationOrderAMD* pStruct, const std::string prefix);
std::string vk_print_vkpipelineshaderstagecreateinfo(const VkPipelineShaderStageCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinetessellationstatecreateinfo(const VkPipelineTessellationStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelinevertexinputstatecreateinfo(const VkPipelineVertexInputStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpipelineviewportstatecreateinfo(const VkPipelineViewportStateCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkpresentinfokhr(const VkPresentInfoKHR* pStruct, const std::string prefix);
std::string vk_print_vkpushconstantrange(const VkPushConstantRange* pStruct, const std::string prefix);
std::string vk_print_vkquerypoolcreateinfo(const VkQueryPoolCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkqueuefamilyproperties(const VkQueueFamilyProperties* pStruct, const std::string prefix);
std::string vk_print_vkrect2d(const VkRect2D* pStruct, const std::string prefix);
std::string vk_print_vkrenderpassbegininfo(const VkRenderPassBeginInfo* pStruct, const std::string prefix);
std::string vk_print_vkrenderpasscreateinfo(const VkRenderPassCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vksamplercreateinfo(const VkSamplerCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vksemaphorecreateinfo(const VkSemaphoreCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vkshadermodulecreateinfo(const VkShaderModuleCreateInfo* pStruct, const std::string prefix);
std::string vk_print_vksparsebuffermemorybindinfo(const VkSparseBufferMemoryBindInfo* pStruct, const std::string prefix);
std::string vk_print_vksparseimageformatproperties(const VkSparseImageFormatProperties* pStruct, const std::string prefix);
std::string vk_print_vksparseimagememorybind(const VkSparseImageMemoryBind* pStruct, const std::string prefix);
std::string vk_print_vksparseimagememorybindinfo(const VkSparseImageMemoryBindInfo* pStruct, const std::string prefix);
std::string vk_print_vksparseimagememoryrequirements(const VkSparseImageMemoryRequirements* pStruct, const std::string prefix);
std::string vk_print_vksparseimageopaquememorybindinfo(const VkSparseImageOpaqueMemoryBindInfo* pStruct, const std::string prefix);
std::string vk_print_vksparsememorybind(const VkSparseMemoryBind* pStruct, const std::string prefix);
std::string vk_print_vkspecializationinfo(const VkSpecializationInfo* pStruct, const std::string prefix);
std::string vk_print_vkspecializationmapentry(const VkSpecializationMapEntry* pStruct, const std::string prefix);
std::string vk_print_vkstencilopstate(const VkStencilOpState* pStruct, const std::string prefix);
std::string vk_print_vksubmitinfo(const VkSubmitInfo* pStruct, const std::string prefix);
std::string vk_print_vksubpassdependency(const VkSubpassDependency* pStruct, const std::string prefix);
std::string vk_print_vksubpassdescription(const VkSubpassDescription* pStruct, const std::string prefix);
std::string vk_print_vksubresourcelayout(const VkSubresourceLayout* pStruct, const std::string prefix);
std::string vk_print_vksurfacecapabilitieskhr(const VkSurfaceCapabilitiesKHR* pStruct, const std::string prefix);
std::string vk_print_vksurfaceformatkhr(const VkSurfaceFormatKHR* pStruct, const std::string prefix);
std::string vk_print_vkswapchaincreateinfokhr(const VkSwapchainCreateInfoKHR* pStruct, const std::string prefix);
std::string vk_print_vkvertexinputattributedescription(const VkVertexInputAttributeDescription* pStruct, const std::string prefix);
std::string vk_print_vkvertexinputbindingdescription(const VkVertexInputBindingDescription* pStruct, const std::string prefix);
std::string vk_print_vkviewport(const VkViewport* pStruct, const std::string prefix);
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
std::string vk_print_vkwaylandsurfacecreateinfokhr(const VkWaylandSurfaceCreateInfoKHR* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_WAYLAND_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkwin32keyedmutexacquirereleaseinfonv(const VkWin32KeyedMutexAcquireReleaseInfoNV* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_WIN32_KHR
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkwin32surfacecreateinfokhr(const VkWin32SurfaceCreateInfoKHR* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkwritedescriptorset(const VkWriteDescriptorSet* pStruct, const std::string prefix);
#ifdef VK_USE_PLATFORM_XCB_KHR
std::string vk_print_vkxcbsurfacecreateinfokhr(const VkXcbSurfaceCreateInfoKHR* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_XCB_KHR
#ifdef VK_USE_PLATFORM_XLIB_KHR
std::string vk_print_vkxlibsurfacecreateinfokhr(const VkXlibSurfaceCreateInfoKHR* pStruct, const std::string prefix);
#endif //VK_USE_PLATFORM_XLIB_KHR


// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #924
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkallocationcallbacks(const VkAllocationCallbacks* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pUserData) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->pfnAllocation << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->pfnReallocation << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->pfnFree << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->pfnInternalAllocation << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->pfnInternalFree << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "pUserData = " + ss[0].str() + "\n" + prefix + "pfnAllocation = " + ss[1].str() + "\n" + prefix + "pfnReallocation = " + ss[2].str() + "\n" + prefix + "pfnFree = " + ss[3].str() + "\n" + prefix + "pfnInternalAllocation = " + ss[4].str() + "\n" + prefix + "pfnInternalFree = " + ss[5].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_ANDROID_KHR
std::string vk_print_vkandroidsurfacecreateinfokhr(const VkAndroidSurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->window) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "window = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_ANDROID_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkapplicationinfo(const VkApplicationInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1111
    if (pStruct->pApplicationName != NULL) {
        ss[1] << pStruct->pApplicationName;
     } else {
        ss[1] << "";
     }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->applicationVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1111
    if (pStruct->pEngineName != NULL) {
        ss[3] << pStruct->pEngineName;
     } else {
        ss[3] << "";
     }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[4] << pStruct->engineVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->apiVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "pApplicationName = " + ss[1].str() + "\n" + prefix + "applicationVersion = " + ss[2].str() + "\n" + prefix + "pEngineName = " + ss[3].str() + "\n" + prefix + "engineVersion = " + ss[4].str() + "\n" + prefix + "apiVersion = " + ss[5].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkattachmentdescription(const VkAttachmentDescription* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "flags = " + ss[0].str() + "\n" + prefix + "format = " + string_VkFormat(pStruct->format) + "\n" + prefix + "samples = " + string_VkSampleCountFlagBits(pStruct->samples) + "\n" + prefix + "loadOp = " + string_VkAttachmentLoadOp(pStruct->loadOp) + "\n" + prefix + "storeOp = " + string_VkAttachmentStoreOp(pStruct->storeOp) + "\n" + prefix + "stencilLoadOp = " + string_VkAttachmentLoadOp(pStruct->stencilLoadOp) + "\n" + prefix + "stencilStoreOp = " + string_VkAttachmentStoreOp(pStruct->stencilStoreOp) + "\n" + prefix + "initialLayout = " + string_VkImageLayout(pStruct->initialLayout) + "\n" + prefix + "finalLayout = " + string_VkImageLayout(pStruct->finalLayout) + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkattachmentreference(const VkAttachmentReference* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->attachment;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "attachment = " + ss[0].str() + "\n" + prefix + "layout = " + string_VkImageLayout(pStruct->layout) + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkbindsparseinfo(const VkBindSparseInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[11];
    string stp_strs[6];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pWaitSemaphores) {
        for (uint32_t i = 0; i < pStruct->waitSemaphoreCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitSemaphores[i]) << dec;
            stp_strs[1] += " " + prefix + "pWaitSemaphores[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pBufferBinds) {
        for (uint32_t i = 0; i < pStruct->bufferBindCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksparsebuffermemorybindinfo(&pStruct->pBufferBinds[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[2] += " " + prefix + "pBufferBinds[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    if (pStruct->pImageOpaqueBinds) {
        for (uint32_t i = 0; i < pStruct->imageOpaqueBindCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksparseimageopaquememorybindinfo(&pStruct->pImageOpaqueBinds[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[3] += " " + prefix + "pImageOpaqueBinds[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[3].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[4] = "";
    if (pStruct->pImageBinds) {
        for (uint32_t i = 0; i < pStruct->imageBindCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksparseimagememorybindinfo(&pStruct->pImageBinds[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[4] += " " + prefix + "pImageBinds[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[4].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[5] = "";
    if (pStruct->pSignalSemaphores) {
        for (uint32_t i = 0; i < pStruct->signalSemaphoreCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pSignalSemaphores[i]) << dec;
            stp_strs[5] += " " + prefix + "pSignalSemaphores[" + index_ss.str() + "] = " + ss[5].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[5].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->waitSemaphoreCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitSemaphores) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->bufferBindCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pBufferBinds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->imageOpaqueBindCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[6] << "0x" << hex << nouppercase << HandleCast(pStruct->pImageOpaqueBinds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->imageBindCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[8] << "0x" << hex << nouppercase << HandleCast(pStruct->pImageBinds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[9] << "0x" << hex << nouppercase << pStruct->signalSemaphoreCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[10] << "0x" << hex << nouppercase << HandleCast(pStruct->pSignalSemaphores) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "waitSemaphoreCount = " + ss[1].str() + "\n" + prefix + "pWaitSemaphores = " + ss[2].str() + "\n" + prefix + "bufferBindCount = " + ss[3].str() + "\n" + prefix + "pBufferBinds = " + ss[4].str() + "\n" + prefix + "imageOpaqueBindCount = " + ss[5].str() + "\n" + prefix + "pImageOpaqueBinds = " + ss[6].str() + "\n" + prefix + "imageBindCount = " + ss[7].str() + "\n" + prefix + "pImageBinds = " + ss[8].str() + "\n" + prefix + "signalSemaphoreCount = " + ss[9].str() + "\n" + prefix + "pSignalSemaphores = " + ss[10].str() + "\n" + stp_strs[5] + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkbuffercopy(const VkBufferCopy* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->srcOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->dstOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "srcOffset = " + ss[0].str() + "\n" + prefix + "dstOffset = " + ss[1].str() + "\n" + prefix + "size = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkbuffercreateinfo(const VkBufferCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->sharingMode == VK_SHARING_MODE_CONCURRENT) {
        if (pStruct->pQueueFamilyIndices) {
            for (uint32_t i = 0; i < pStruct->queueFamilyIndexCount; i++) {
                index_ss.str("");
                index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
                ss[1] << "0x" << hex << nouppercase << pStruct->pQueueFamilyIndices[i] << dec;
                stp_strs[1] += " " + prefix + "pQueueFamilyIndices[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
                ss[1].str("");
            }
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->usage << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->queueFamilyIndexCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pQueueFamilyIndices) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "size = " + ss[2].str() + "\n" + prefix + "usage = " + ss[3].str() + "\n" + prefix + "sharingMode = " + string_VkSharingMode(pStruct->sharingMode) + "\n" + prefix + "queueFamilyIndexCount = " + ss[4].str() + "\n" + prefix + "pQueueFamilyIndices = " + ss[5].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkbufferimagecopy(const VkBufferImageCopy* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->imageSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "imageSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset3d(&pStruct->imageOffset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "imageOffset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->imageExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "imageExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->bufferOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->bufferRowLength << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->bufferImageHeight << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageOffset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[5] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "bufferOffset = " + ss[0].str() + "\n" + prefix + "bufferRowLength = " + ss[1].str() + "\n" + prefix + "bufferImageHeight = " + ss[2].str() + "\n" + prefix + "imageSubresource = " + ss[3].str() + "\n" + prefix + "imageOffset = " + ss[4].str() + "\n" + prefix + "imageExtent = " + ss[5].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkbuffermemorybarrier(const VkBufferMemoryBarrier* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->srcAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->dstAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->srcQueueFamilyIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->dstQueueFamilyIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->buffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "srcAccessMask = " + ss[1].str() + "\n" + prefix + "dstAccessMask = " + ss[2].str() + "\n" + prefix + "srcQueueFamilyIndex = " + ss[3].str() + "\n" + prefix + "dstQueueFamilyIndex = " + ss[4].str() + "\n" + prefix + "buffer = " + ss[5].str() + "\n" + prefix + "offset = " + ss[6].str() + "\n" + prefix + "size = " + ss[7].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkbufferviewcreateinfo(const VkBufferViewCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->buffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->range << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "buffer = " + ss[2].str() + "\n" + prefix + "format = " + string_VkFormat(pStruct->format) + "\n" + prefix + "offset = " + ss[3].str() + "\n" + prefix + "range = " + ss[4].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkclearattachment(const VkClearAttachment* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkclearvalue(&pStruct->clearValue, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "clearValue:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->aspectMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->colorAttachment;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->clearValue) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "aspectMask = " + ss[0].str() + "\n" + prefix + "colorAttachment = " + ss[1].str() + "\n" + prefix + "clearValue = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkclearcolorvalue(const VkClearColorValue* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[0] = "";
    stringstream index_ss;
    for (uint32_t i = 0; i < 4; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[0] << pStruct->float32[i];
        stp_strs[0] += " " + prefix + "float32[" + index_ss.str() + "] = " + ss[0].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[0].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    for (uint32_t i = 0; i < 4; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[1] << pStruct->int32[i];
        stp_strs[1] += " " + prefix + "int32[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[1].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    for (uint32_t i = 0; i < 4; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[2] << pStruct->uint32[i];
        stp_strs[2] += " " + prefix + "uint32[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[2].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->float32) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->int32) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->uint32) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "float32 = " + ss[0].str() + "\n" + prefix + "int32 = " + ss[1].str() + "\n" + prefix + "uint32 = " + ss[2].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcleardepthstencilvalue(const VkClearDepthStencilValue* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->depth;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->stencil;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "depth = " + ss[0].str() + "\n" + prefix + "stencil = " + ss[1].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkclearrect(const VkClearRect* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkrect2d(&pStruct->rect, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "rect:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->rect) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->baseArrayLayer;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->layerCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "rect = " + ss[0].str() + "\n" + prefix + "baseArrayLayer = " + ss[1].str() + "\n" + prefix + "layerCount = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkclearvalue(const VkClearValue* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkclearcolorvalue(&pStruct->color, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "color:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkcleardepthstencilvalue(&pStruct->depthStencil, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "depthStencil:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->color) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->depthStencil) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "color = " + ss[0].str() + "\n" + prefix + "depthStencil = " + ss[1].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcommandbufferallocateinfo(const VkCommandBufferAllocateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->commandPool) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->commandBufferCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "commandPool = " + ss[1].str() + "\n" + prefix + "level = " + string_VkCommandBufferLevel(pStruct->level) + "\n" + prefix + "commandBufferCount = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcommandbufferbegininfo(const VkCommandBufferBeginInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pInheritanceInfo) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkcommandbufferinheritanceinfo(pStruct->pInheritanceInfo, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[1] = " " + prefix + "pInheritanceInfo:\n" + tmp_str;
        ss[1].str("");
    }
    else
        stp_strs[1] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pInheritanceInfo) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "pInheritanceInfo = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcommandbufferinheritanceinfo(const VkCommandBufferInheritanceInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->renderPass) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->subpass;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->framebuffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[4].str(pStruct->occlusionQueryEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->queryFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->pipelineStatistics << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "renderPass = " + ss[1].str() + "\n" + prefix + "subpass = " + ss[2].str() + "\n" + prefix + "framebuffer = " + ss[3].str() + "\n" + prefix + "occlusionQueryEnable = " + ss[4].str() + "\n" + prefix + "queryFlags = " + ss[5].str() + "\n" + prefix + "pipelineStatistics = " + ss[6].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcommandpoolcreateinfo(const VkCommandPoolCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->queueFamilyIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "queueFamilyIndex = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcomponentmapping(const VkComponentMapping* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "r = " + string_VkComponentSwizzle(pStruct->r) + "\n" + prefix + "g = " + string_VkComponentSwizzle(pStruct->g) + "\n" + prefix + "b = " + string_VkComponentSwizzle(pStruct->b) + "\n" + prefix + "a = " + string_VkComponentSwizzle(pStruct->a) + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcomputepipelinecreateinfo(const VkComputePipelineCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkpipelineshaderstagecreateinfo(&pStruct->stage, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "stage:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->stage) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->layout) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->basePipelineHandle) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->basePipelineIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "stage = " + ss[2].str() + "\n" + prefix + "layout = " + ss[3].str() + "\n" + prefix + "basePipelineHandle = " + ss[4].str() + "\n" + prefix + "basePipelineIndex = " + ss[5].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkcopydescriptorset(const VkCopyDescriptorSet* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->srcSet) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->srcBinding;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->srcArrayElement;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->dstSet) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->dstBinding;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[6] << pStruct->dstArrayElement;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->descriptorCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "srcSet = " + ss[1].str() + "\n" + prefix + "srcBinding = " + ss[2].str() + "\n" + prefix + "srcArrayElement = " + ss[3].str() + "\n" + prefix + "dstSet = " + ss[4].str() + "\n" + prefix + "dstBinding = " + ss[5].str() + "\n" + prefix + "dstArrayElement = " + ss[6].str() + "\n" + prefix + "descriptorCount = " + ss[7].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdebugmarkermarkerinfoext(const VkDebugMarkerMarkerInfoEXT* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    for (uint32_t i = 0; i < 4; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[1] << pStruct->color[i];
        stp_strs[1] += " " + prefix + "color[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[1].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1111
    if (pStruct->pMarkerName != NULL) {
        ss[1] << pStruct->pMarkerName;
     } else {
        ss[1] << "";
     }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->color) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "pMarkerName = " + ss[1].str() + "\n" + prefix + "color = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdebugmarkerobjectnameinfoext(const VkDebugMarkerObjectNameInfoEXT* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->object << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1111
    if (pStruct->pObjectName != NULL) {
        ss[2] << pStruct->pObjectName;
     } else {
        ss[2] << "";
     }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "objectType = " + string_VkDebugReportObjectTypeEXT(pStruct->objectType) + "\n" + prefix + "object = " + ss[1].str() + "\n" + prefix + "pObjectName = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdebugmarkerobjecttaginfoext(const VkDebugMarkerObjectTagInfoEXT* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->object << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->tagName;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->tagSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pTag) << dec;
    else
        ss[4].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "objectType = " + string_VkDebugReportObjectTypeEXT(pStruct->objectType) + "\n" + prefix + "object = " + ss[1].str() + "\n" + prefix + "tagName = " + ss[2].str() + "\n" + prefix + "tagSize = " + ss[3].str() + "\n" + prefix + "pTag = " + ss[4].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdebugreportcallbackcreateinfoext(const VkDebugReportCallbackCreateInfoEXT* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->pfnCallback << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pUserData) << dec;
    else
        ss[3].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "pfnCallback = " + ss[2].str() + "\n" + prefix + "pUserData = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdedicatedallocationbuffercreateinfonv(const VkDedicatedAllocationBufferCreateInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[1].str(pStruct->dedicatedAllocation ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "dedicatedAllocation = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdedicatedallocationimagecreateinfonv(const VkDedicatedAllocationImageCreateInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[1].str(pStruct->dedicatedAllocation ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "dedicatedAllocation = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdedicatedallocationmemoryallocateinfonv(const VkDedicatedAllocationMemoryAllocateInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->image) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->buffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "image = " + ss[1].str() + "\n" + prefix + "buffer = " + ss[2].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorbufferinfo(const VkDescriptorBufferInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->buffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->range << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "buffer = " + ss[0].str() + "\n" + prefix + "offset = " + ss[1].str() + "\n" + prefix + "range = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorimageinfo(const VkDescriptorImageInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->sampler) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->imageView) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sampler = " + ss[0].str() + "\n" + prefix + "imageView = " + ss[1].str() + "\n" + prefix + "imageLayout = " + string_VkImageLayout(pStruct->imageLayout) + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorpoolcreateinfo(const VkDescriptorPoolCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pPoolSizes) {
        for (uint32_t i = 0; i < pStruct->poolSizeCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkdescriptorpoolsize(&pStruct->pPoolSizes[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pPoolSizes[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->maxSets;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->poolSizeCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pPoolSizes) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "maxSets = " + ss[2].str() + "\n" + prefix + "poolSizeCount = " + ss[3].str() + "\n" + prefix + "pPoolSizes = " + ss[4].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorpoolsize(const VkDescriptorPoolSize* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->descriptorCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "type = " + string_VkDescriptorType(pStruct->type) + "\n" + prefix + "descriptorCount = " + ss[0].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorsetallocateinfo(const VkDescriptorSetAllocateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    stringstream index_ss;
    if (pStruct->pSetLayouts) {
        for (uint32_t i = 0; i < pStruct->descriptorSetCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pSetLayouts[i]) << dec;
            stp_strs[2] += " " + prefix + "pSetLayouts[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->descriptorPool) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->descriptorSetCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pSetLayouts) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "descriptorPool = " + ss[1].str() + "\n" + prefix + "descriptorSetCount = " + ss[2].str() + "\n" + prefix + "pSetLayouts = " + ss[3].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorsetlayoutbinding(const VkDescriptorSetLayoutBinding* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[0] = "";
    stringstream index_ss;
    if (pStruct->pImmutableSamplers) {
        for (uint32_t i = 0; i < pStruct->descriptorCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pImmutableSamplers[i]) << dec;
            stp_strs[0] += " " + prefix + "pImmutableSamplers[" + index_ss.str() + "] = " + ss[0].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[0].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->binding;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->descriptorCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->stageFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pImmutableSamplers) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "binding = " + ss[0].str() + "\n" + prefix + "descriptorType = " + string_VkDescriptorType(pStruct->descriptorType) + "\n" + prefix + "descriptorCount = " + ss[1].str() + "\n" + prefix + "stageFlags = " + ss[2].str() + "\n" + prefix + "pImmutableSamplers = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdescriptorsetlayoutcreateinfo(const VkDescriptorSetLayoutCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pBindings) {
        for (uint32_t i = 0; i < pStruct->bindingCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkdescriptorsetlayoutbinding(&pStruct->pBindings[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pBindings[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->bindingCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pBindings) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "bindingCount = " + ss[2].str() + "\n" + prefix + "pBindings = " + ss[3].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdevicecreateinfo(const VkDeviceCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[9];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pQueueCreateInfos) {
        for (uint32_t i = 0; i < pStruct->queueCreateInfoCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkdevicequeuecreateinfo(&pStruct->pQueueCreateInfos[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pQueueCreateInfos[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pEnabledFeatures) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkphysicaldevicefeatures(pStruct->pEnabledFeatures, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[4] = " " + prefix + "pEnabledFeatures:\n" + tmp_str;
        ss[4].str("");
    }
    else
        stp_strs[4] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->queueCreateInfoCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pQueueCreateInfos) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->enabledLayerCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->ppEnabledLayerNames) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->enabledExtensionCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->ppEnabledExtensionNames) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[8] << "0x" << hex << nouppercase << HandleCast(pStruct->pEnabledFeatures) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "queueCreateInfoCount = " + ss[2].str() + "\n" + prefix + "pQueueCreateInfos = " + ss[3].str() + "\n" + prefix + "enabledLayerCount = " + ss[4].str() + "\n" + prefix + "ppEnabledLayerNames = " + ss[5].str() + "\n" + prefix + "enabledExtensionCount = " + ss[6].str() + "\n" + prefix + "ppEnabledExtensionNames = " + ss[7].str() + "\n" + prefix + "pEnabledFeatures = " + ss[8].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdevicequeuecreateinfo(const VkDeviceQueueCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pQueuePriorities) {
        for (uint32_t i = 0; i < pStruct->queueCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
            ss[1] << "0x" << hex << nouppercase << pStruct->pQueuePriorities[i] << dec;
            stp_strs[1] += " " + prefix + "pQueuePriorities[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->queueFamilyIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->queueCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pQueuePriorities) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "queueFamilyIndex = " + ss[2].str() + "\n" + prefix + "queueCount = " + ss[3].str() + "\n" + prefix + "pQueuePriorities = " + ss[4].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdispatchindirectcommand(const VkDispatchIndirectCommand* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->x;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->y;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->z;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "x = " + ss[0].str() + "\n" + prefix + "y = " + ss[1].str() + "\n" + prefix + "z = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplaymodecreateinfokhr(const VkDisplayModeCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkdisplaymodeparameterskhr(&pStruct->parameters, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "parameters:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->parameters) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "parameters = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplaymodeparameterskhr(const VkDisplayModeParametersKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->visibleRegion, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "visibleRegion:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->visibleRegion) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->refreshRate;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "visibleRegion = " + ss[0].str() + "\n" + prefix + "refreshRate = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplaymodepropertieskhr(const VkDisplayModePropertiesKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkdisplaymodeparameterskhr(&pStruct->parameters, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "parameters:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->displayMode) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->parameters) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "displayMode = " + ss[0].str() + "\n" + prefix + "parameters = " + ss[1].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplayplanecapabilitieskhr(const VkDisplayPlaneCapabilitiesKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[9];
    string stp_strs[8];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset2d(&pStruct->minSrcPosition, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "minSrcPosition:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset2d(&pStruct->maxSrcPosition, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "maxSrcPosition:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->minSrcExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "minSrcExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->maxSrcExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[3] = " " + prefix + "maxSrcExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset2d(&pStruct->minDstPosition, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[4] = " " + prefix + "minDstPosition:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset2d(&pStruct->maxDstPosition, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[5] = " " + prefix + "maxDstPosition:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->minDstExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[6] = " " + prefix + "minDstExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->maxDstExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[7] = " " + prefix + "maxDstExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->supportedAlpha << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->minSrcPosition) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->maxSrcPosition) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->minSrcExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->maxSrcExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[5] << "0x" << hex << nouppercase << HandleCast(&pStruct->minDstPosition) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[6] << "0x" << hex << nouppercase << HandleCast(&pStruct->maxDstPosition) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[7] << "0x" << hex << nouppercase << HandleCast(&pStruct->minDstExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[8] << "0x" << hex << nouppercase << HandleCast(&pStruct->maxDstExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "supportedAlpha = " + ss[0].str() + "\n" + prefix + "minSrcPosition = " + ss[1].str() + "\n" + prefix + "maxSrcPosition = " + ss[2].str() + "\n" + prefix + "minSrcExtent = " + ss[3].str() + "\n" + prefix + "maxSrcExtent = " + ss[4].str() + "\n" + prefix + "minDstPosition = " + ss[5].str() + "\n" + prefix + "maxDstPosition = " + ss[6].str() + "\n" + prefix + "minDstExtent = " + ss[7].str() + "\n" + prefix + "maxDstExtent = " + ss[8].str() + "\n" + stp_strs[7] + stp_strs[6] + stp_strs[5] + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplayplanepropertieskhr(const VkDisplayPlanePropertiesKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->currentDisplay) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->currentStackIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "currentDisplay = " + ss[0].str() + "\n" + prefix + "currentStackIndex = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplaypresentinfokhr(const VkDisplayPresentInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkrect2d(&pStruct->srcRect, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "srcRect:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkrect2d(&pStruct->dstRect, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "dstRect:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcRect) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstRect) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[3].str(pStruct->persistent ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "srcRect = " + ss[1].str() + "\n" + prefix + "dstRect = " + ss[2].str() + "\n" + prefix + "persistent = " + ss[3].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplaypropertieskhr(const VkDisplayPropertiesKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->physicalDimensions, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "physicalDimensions:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->physicalResolution, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "physicalResolution:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->display) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1111
    if (pStruct->displayName != NULL) {
        ss[1] << pStruct->displayName;
     } else {
        ss[1] << "";
     }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->physicalDimensions) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->physicalResolution) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->supportedTransforms << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[5].str(pStruct->planeReorderPossible ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[6].str(pStruct->persistentContent ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "display = " + ss[0].str() + "\n" + prefix + "displayName = " + ss[1].str() + "\n" + prefix + "physicalDimensions = " + ss[2].str() + "\n" + prefix + "physicalResolution = " + ss[3].str() + "\n" + prefix + "supportedTransforms = " + ss[4].str() + "\n" + prefix + "planeReorderPossible = " + ss[5].str() + "\n" + prefix + "persistentContent = " + ss[6].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdisplaysurfacecreateinfokhr(const VkDisplaySurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->imageExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "imageExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->displayMode) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->planeIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->planeStackIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->globalAlpha;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[6] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "displayMode = " + ss[2].str() + "\n" + prefix + "planeIndex = " + ss[3].str() + "\n" + prefix + "planeStackIndex = " + ss[4].str() + "\n" + prefix + "transform = " + string_VkSurfaceTransformFlagBitsKHR(pStruct->transform) + "\n" + prefix + "globalAlpha = " + ss[5].str() + "\n" + prefix + "alphaMode = " + string_VkDisplayPlaneAlphaFlagBitsKHR(pStruct->alphaMode) + "\n" + prefix + "imageExtent = " + ss[6].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdrawindexedindirectcommand(const VkDrawIndexedIndirectCommand* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->indexCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->instanceCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->firstIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->vertexOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[4] << pStruct->firstInstance;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "indexCount = " + ss[0].str() + "\n" + prefix + "instanceCount = " + ss[1].str() + "\n" + prefix + "firstIndex = " + ss[2].str() + "\n" + prefix + "vertexOffset = " + ss[3].str() + "\n" + prefix + "firstInstance = " + ss[4].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkdrawindirectcommand(const VkDrawIndirectCommand* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->vertexCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->instanceCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->firstVertex;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->firstInstance;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "vertexCount = " + ss[0].str() + "\n" + prefix + "instanceCount = " + ss[1].str() + "\n" + prefix + "firstVertex = " + ss[2].str() + "\n" + prefix + "firstInstance = " + ss[3].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkeventcreateinfo(const VkEventCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkexportmemoryallocateinfonv(const VkExportMemoryAllocateInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->handleTypes << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "handleTypes = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkexportmemorywin32handleinfonv(const VkExportMemoryWin32HandleInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pAttributes) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->dwAccess;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "pAttributes = " + ss[1].str() + "\n" + prefix + "dwAccess = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_WIN32_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkextensionproperties(const VkExtensionProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1093
    ss[0] << pStruct->extensionName;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->specVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "extensionName = " + ss[0].str() + "\n" + prefix + "specVersion = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkextent2d(const VkExtent2D* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->width;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->height;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "width = " + ss[0].str() + "\n" + prefix + "height = " + ss[1].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkextent3d(const VkExtent3D* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->width;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->height;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->depth;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "width = " + ss[0].str() + "\n" + prefix + "height = " + ss[1].str() + "\n" + prefix + "depth = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkexternalimageformatpropertiesnv(const VkExternalImageFormatPropertiesNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimageformatproperties(&pStruct->imageFormatProperties, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "imageFormatProperties:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageFormatProperties) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->externalMemoryFeatures << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->exportFromImportedHandleTypes << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->compatibleHandleTypes << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "imageFormatProperties = " + ss[0].str() + "\n" + prefix + "externalMemoryFeatures = " + ss[1].str() + "\n" + prefix + "exportFromImportedHandleTypes = " + ss[2].str() + "\n" + prefix + "compatibleHandleTypes = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkexternalmemoryimagecreateinfonv(const VkExternalMemoryImageCreateInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->handleTypes << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "handleTypes = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkfencecreateinfo(const VkFenceCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkformatproperties(const VkFormatProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->linearTilingFeatures << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->optimalTilingFeatures << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->bufferFeatures << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "linearTilingFeatures = " + ss[0].str() + "\n" + prefix + "optimalTilingFeatures = " + ss[1].str() + "\n" + prefix + "bufferFeatures = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkframebuffercreateinfo(const VkFramebufferCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    stringstream index_ss;
    if (pStruct->pAttachments) {
        for (uint32_t i = 0; i < pStruct->attachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pAttachments[i]) << dec;
            stp_strs[2] += " " + prefix + "pAttachments[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->renderPass) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->attachmentCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->width;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[6] << pStruct->height;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[7] << pStruct->layers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "renderPass = " + ss[2].str() + "\n" + prefix + "attachmentCount = " + ss[3].str() + "\n" + prefix + "pAttachments = " + ss[4].str() + "\n" + prefix + "width = " + ss[5].str() + "\n" + prefix + "height = " + ss[6].str() + "\n" + prefix + "layers = " + ss[7].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkgraphicspipelinecreateinfo(const VkGraphicsPipelineCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[18];
    string stp_strs[14];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pStages) {
        for (uint32_t i = 0; i < pStruct->stageCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkpipelineshaderstagecreateinfo(&pStruct->pStages[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pStages[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pVertexInputState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinevertexinputstatecreateinfo(pStruct->pVertexInputState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[2] = " " + prefix + "pVertexInputState:\n" + tmp_str;
        ss[2].str("");
    }
    else
        stp_strs[2] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pInputAssemblyState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelineinputassemblystatecreateinfo(pStruct->pInputAssemblyState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[3] = " " + prefix + "pInputAssemblyState:\n" + tmp_str;
        ss[3].str("");
    }
    else
        stp_strs[3] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pTessellationState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinetessellationstatecreateinfo(pStruct->pTessellationState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[4] = " " + prefix + "pTessellationState:\n" + tmp_str;
        ss[4].str("");
    }
    else
        stp_strs[4] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pViewportState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelineviewportstatecreateinfo(pStruct->pViewportState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[5] = " " + prefix + "pViewportState:\n" + tmp_str;
        ss[5].str("");
    }
    else
        stp_strs[5] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pRasterizationState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinerasterizationstatecreateinfo(pStruct->pRasterizationState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[6] = " " + prefix + "pRasterizationState:\n" + tmp_str;
        ss[6].str("");
    }
    else
        stp_strs[6] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pMultisampleState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinemultisamplestatecreateinfo(pStruct->pMultisampleState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[7] = " " + prefix + "pMultisampleState:\n" + tmp_str;
        ss[7].str("");
    }
    else
        stp_strs[7] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pDepthStencilState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinedepthstencilstatecreateinfo(pStruct->pDepthStencilState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[8] = " " + prefix + "pDepthStencilState:\n" + tmp_str;
        ss[8].str("");
    }
    else
        stp_strs[8] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pColorBlendState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinecolorblendstatecreateinfo(pStruct->pColorBlendState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[9] = " " + prefix + "pColorBlendState:\n" + tmp_str;
        ss[9].str("");
    }
    else
        stp_strs[9] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pDynamicState) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkpipelinedynamicstatecreateinfo(pStruct->pDynamicState, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[10] = " " + prefix + "pDynamicState:\n" + tmp_str;
        ss[10].str("");
    }
    else
        stp_strs[10] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->stageCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pStages) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pVertexInputState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pInputAssemblyState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[6] << "0x" << hex << nouppercase << HandleCast(pStruct->pTessellationState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->pViewportState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[8] << "0x" << hex << nouppercase << HandleCast(pStruct->pRasterizationState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[9] << "0x" << hex << nouppercase << HandleCast(pStruct->pMultisampleState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[10] << "0x" << hex << nouppercase << HandleCast(pStruct->pDepthStencilState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[11] << "0x" << hex << nouppercase << HandleCast(pStruct->pColorBlendState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[12] << "0x" << hex << nouppercase << HandleCast(pStruct->pDynamicState) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[13] << "0x" << hex << nouppercase << HandleCast(pStruct->layout) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[14] << "0x" << hex << nouppercase << HandleCast(pStruct->renderPass) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[15] << pStruct->subpass;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[16] << "0x" << hex << nouppercase << HandleCast(pStruct->basePipelineHandle) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[17] << "0x" << hex << nouppercase << pStruct->basePipelineIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "stageCount = " + ss[2].str() + "\n" + prefix + "pStages = " + ss[3].str() + "\n" + prefix + "pVertexInputState = " + ss[4].str() + "\n" + prefix + "pInputAssemblyState = " + ss[5].str() + "\n" + prefix + "pTessellationState = " + ss[6].str() + "\n" + prefix + "pViewportState = " + ss[7].str() + "\n" + prefix + "pRasterizationState = " + ss[8].str() + "\n" + prefix + "pMultisampleState = " + ss[9].str() + "\n" + prefix + "pDepthStencilState = " + ss[10].str() + "\n" + prefix + "pColorBlendState = " + ss[11].str() + "\n" + prefix + "pDynamicState = " + ss[12].str() + "\n" + prefix + "layout = " + ss[13].str() + "\n" + prefix + "renderPass = " + ss[14].str() + "\n" + prefix + "subpass = " + ss[15].str() + "\n" + prefix + "basePipelineHandle = " + ss[16].str() + "\n" + prefix + "basePipelineIndex = " + ss[17].str() + "\n" + stp_strs[13] + stp_strs[12] + stp_strs[11] + stp_strs[10] + stp_strs[9] + stp_strs[8] + stp_strs[7] + stp_strs[6] + stp_strs[5] + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimageblit(const VkImageBlit* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->srcSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "srcSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    for (uint32_t i = 0; i < 2; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
        tmp_str = vk_print_vkoffset3d(&pStruct->srcOffsets[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
        stp_strs[1] += " " + prefix + "srcOffsets[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[1].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->dstSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "dstSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    for (uint32_t i = 0; i < 2; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
        tmp_str = vk_print_vkoffset3d(&pStruct->dstOffsets[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
        stp_strs[3] += " " + prefix + "dstOffsets[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[3].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcOffsets) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstOffsets) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "srcSubresource = " + ss[0].str() + "\n" + prefix + "srcOffsets = " + ss[1].str() + "\n" + prefix + "dstSubresource = " + ss[2].str() + "\n" + prefix + "dstOffsets = " + ss[3].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimagecopy(const VkImageCopy* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->srcSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "srcSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset3d(&pStruct->srcOffset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "srcOffset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->dstSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "dstSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset3d(&pStruct->dstOffset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[3] = " " + prefix + "dstOffset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->extent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[4] = " " + prefix + "extent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcOffset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstOffset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->extent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "srcSubresource = " + ss[0].str() + "\n" + prefix + "srcOffset = " + ss[1].str() + "\n" + prefix + "dstSubresource = " + ss[2].str() + "\n" + prefix + "dstOffset = " + ss[3].str() + "\n" + prefix + "extent = " + ss[4].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimagecreateinfo(const VkImageCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->extent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "extent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    stringstream index_ss;
    if (pStruct->sharingMode == VK_SHARING_MODE_CONCURRENT) {
        if (pStruct->pQueueFamilyIndices) {
            for (uint32_t i = 0; i < pStruct->queueFamilyIndexCount; i++) {
                index_ss.str("");
                index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
                ss[2] << "0x" << hex << nouppercase << pStruct->pQueueFamilyIndices[i] << dec;
                stp_strs[2] += " " + prefix + "pQueueFamilyIndices[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
                ss[2].str("");
            }
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->extent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->mipLevels;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[4] << pStruct->arrayLayers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->usage << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->queueFamilyIndexCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->pQueueFamilyIndices) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "imageType = " + string_VkImageType(pStruct->imageType) + "\n" + prefix + "format = " + string_VkFormat(pStruct->format) + "\n" + prefix + "extent = " + ss[2].str() + "\n" + prefix + "mipLevels = " + ss[3].str() + "\n" + prefix + "arrayLayers = " + ss[4].str() + "\n" + prefix + "samples = " + string_VkSampleCountFlagBits(pStruct->samples) + "\n" + prefix + "tiling = " + string_VkImageTiling(pStruct->tiling) + "\n" + prefix + "usage = " + ss[5].str() + "\n" + prefix + "sharingMode = " + string_VkSharingMode(pStruct->sharingMode) + "\n" + prefix + "queueFamilyIndexCount = " + ss[6].str() + "\n" + prefix + "pQueueFamilyIndices = " + ss[7].str() + "\n" + prefix + "initialLayout = " + string_VkImageLayout(pStruct->initialLayout) + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimageformatproperties(const VkImageFormatProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->maxExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "maxExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->maxExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->maxMipLevels;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->maxArrayLayers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->sampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->maxResourceSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "maxExtent = " + ss[0].str() + "\n" + prefix + "maxMipLevels = " + ss[1].str() + "\n" + prefix + "maxArrayLayers = " + ss[2].str() + "\n" + prefix + "sampleCounts = " + ss[3].str() + "\n" + prefix + "maxResourceSize = " + ss[4].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimagememorybarrier(const VkImageMemoryBarrier* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcerange(&pStruct->subresourceRange, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "subresourceRange:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->srcAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->dstAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->srcQueueFamilyIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->dstQueueFamilyIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->image) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[6] << "0x" << hex << nouppercase << HandleCast(&pStruct->subresourceRange) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "srcAccessMask = " + ss[1].str() + "\n" + prefix + "dstAccessMask = " + ss[2].str() + "\n" + prefix + "oldLayout = " + string_VkImageLayout(pStruct->oldLayout) + "\n" + prefix + "newLayout = " + string_VkImageLayout(pStruct->newLayout) + "\n" + prefix + "srcQueueFamilyIndex = " + ss[3].str() + "\n" + prefix + "dstQueueFamilyIndex = " + ss[4].str() + "\n" + prefix + "image = " + ss[5].str() + "\n" + prefix + "subresourceRange = " + ss[6].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimageresolve(const VkImageResolve* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->srcSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "srcSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset3d(&pStruct->srcOffset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "srcOffset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcelayers(&pStruct->dstSubresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "dstSubresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset3d(&pStruct->dstOffset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[3] = " " + prefix + "dstOffset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->extent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[4] = " " + prefix + "extent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->srcOffset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstSubresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->dstOffset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->extent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "srcSubresource = " + ss[0].str() + "\n" + prefix + "srcOffset = " + ss[1].str() + "\n" + prefix + "dstSubresource = " + ss[2].str() + "\n" + prefix + "dstOffset = " + ss[3].str() + "\n" + prefix + "extent = " + ss[4].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimagesubresource(const VkImageSubresource* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->aspectMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->mipLevel;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->arrayLayer;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "aspectMask = " + ss[0].str() + "\n" + prefix + "mipLevel = " + ss[1].str() + "\n" + prefix + "arrayLayer = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimagesubresourcelayers(const VkImageSubresourceLayers* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->aspectMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->mipLevel;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->baseArrayLayer;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->layerCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "aspectMask = " + ss[0].str() + "\n" + prefix + "mipLevel = " + ss[1].str() + "\n" + prefix + "baseArrayLayer = " + ss[2].str() + "\n" + prefix + "layerCount = " + ss[3].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimagesubresourcerange(const VkImageSubresourceRange* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->aspectMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->baseMipLevel;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->levelCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->baseArrayLayer;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->layerCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "aspectMask = " + ss[0].str() + "\n" + prefix + "baseMipLevel = " + ss[1].str() + "\n" + prefix + "levelCount = " + ss[2].str() + "\n" + prefix + "baseArrayLayer = " + ss[3].str() + "\n" + prefix + "layerCount = " + ss[4].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkimageviewcreateinfo(const VkImageViewCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkcomponentmapping(&pStruct->components, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "components:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresourcerange(&pStruct->subresourceRange, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[3] = " " + prefix + "subresourceRange:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->image) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->components) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->subresourceRange) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "image = " + ss[2].str() + "\n" + prefix + "viewType = " + string_VkImageViewType(pStruct->viewType) + "\n" + prefix + "format = " + string_VkFormat(pStruct->format) + "\n" + prefix + "components = " + ss[3].str() + "\n" + prefix + "subresourceRange = " + ss[4].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkimportmemorywin32handleinfonv(const VkImportMemoryWin32HandleInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->handleType << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->handle << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "handleType = " + ss[1].str() + "\n" + prefix + "handle = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_WIN32_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkinstancecreateinfo(const VkInstanceCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pApplicationInfo) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkapplicationinfo(pStruct->pApplicationInfo, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[1] = " " + prefix + "pApplicationInfo:\n" + tmp_str;
        ss[1].str("");
    }
    else
        stp_strs[1] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pApplicationInfo) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->enabledLayerCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->ppEnabledLayerNames) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->enabledExtensionCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[6] << "0x" << hex << nouppercase << HandleCast(pStruct->ppEnabledExtensionNames) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "pApplicationInfo = " + ss[2].str() + "\n" + prefix + "enabledLayerCount = " + ss[3].str() + "\n" + prefix + "ppEnabledLayerNames = " + ss[4].str() + "\n" + prefix + "enabledExtensionCount = " + ss[5].str() + "\n" + prefix + "ppEnabledExtensionNames = " + ss[6].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vklayerproperties(const VkLayerProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1093
    ss[0] << pStruct->layerName;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->specVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->implementationVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1093
    ss[3] << pStruct->description;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "layerName = " + ss[0].str() + "\n" + prefix + "specVersion = " + ss[1].str() + "\n" + prefix + "implementationVersion = " + ss[2].str() + "\n" + prefix + "description = " + ss[3].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkmappedmemoryrange(const VkMappedMemoryRange* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->memory) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "memory = " + ss[1].str() + "\n" + prefix + "offset = " + ss[2].str() + "\n" + prefix + "size = " + ss[3].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkmemoryallocateinfo(const VkMemoryAllocateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->allocationSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->memoryTypeIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "allocationSize = " + ss[1].str() + "\n" + prefix + "memoryTypeIndex = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkmemorybarrier(const VkMemoryBarrier* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->srcAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->dstAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "srcAccessMask = " + ss[1].str() + "\n" + prefix + "dstAccessMask = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkmemoryheap(const VkMemoryHeap* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "size = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkmemoryrequirements(const VkMemoryRequirements* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->alignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->memoryTypeBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "size = " + ss[0].str() + "\n" + prefix + "alignment = " + ss[1].str() + "\n" + prefix + "memoryTypeBits = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkmemorytype(const VkMemoryType* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->propertyFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->heapIndex << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "propertyFlags = " + ss[0].str() + "\n" + prefix + "heapIndex = " + ss[1].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_MIR_KHR
std::string vk_print_vkmirsurfacecreateinfokhr(const VkMirSurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->connection) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->mirSurface) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "connection = " + ss[2].str() + "\n" + prefix + "mirSurface = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_MIR_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkoffset2d(const VkOffset2D* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->x;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->y;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "x = " + ss[0].str() + "\n" + prefix + "y = " + ss[1].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkoffset3d(const VkOffset3D* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->x;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->y;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->z;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "x = " + ss[0].str() + "\n" + prefix + "y = " + ss[1].str() + "\n" + prefix + "z = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkphysicaldevicefeatures(const VkPhysicalDeviceFeatures* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[55];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[0].str(pStruct->robustBufferAccess ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[1].str(pStruct->fullDrawIndexUint32 ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->imageCubeArray ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[3].str(pStruct->independentBlend ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[4].str(pStruct->geometryShader ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[5].str(pStruct->tessellationShader ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[6].str(pStruct->sampleRateShading ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[7].str(pStruct->dualSrcBlend ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[8].str(pStruct->logicOp ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[9].str(pStruct->multiDrawIndirect ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[10].str(pStruct->drawIndirectFirstInstance ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[11].str(pStruct->depthClamp ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[12].str(pStruct->depthBiasClamp ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[13].str(pStruct->fillModeNonSolid ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[14].str(pStruct->depthBounds ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[15].str(pStruct->wideLines ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[16].str(pStruct->largePoints ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[17].str(pStruct->alphaToOne ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[18].str(pStruct->multiViewport ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[19].str(pStruct->samplerAnisotropy ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[20].str(pStruct->textureCompressionETC2 ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[21].str(pStruct->textureCompressionASTC_LDR ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[22].str(pStruct->textureCompressionBC ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[23].str(pStruct->occlusionQueryPrecise ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[24].str(pStruct->pipelineStatisticsQuery ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[25].str(pStruct->vertexPipelineStoresAndAtomics ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[26].str(pStruct->fragmentStoresAndAtomics ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[27].str(pStruct->shaderTessellationAndGeometryPointSize ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[28].str(pStruct->shaderImageGatherExtended ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[29].str(pStruct->shaderStorageImageExtendedFormats ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[30].str(pStruct->shaderStorageImageMultisample ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[31].str(pStruct->shaderStorageImageReadWithoutFormat ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[32].str(pStruct->shaderStorageImageWriteWithoutFormat ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[33].str(pStruct->shaderUniformBufferArrayDynamicIndexing ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[34].str(pStruct->shaderSampledImageArrayDynamicIndexing ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[35].str(pStruct->shaderStorageBufferArrayDynamicIndexing ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[36].str(pStruct->shaderStorageImageArrayDynamicIndexing ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[37].str(pStruct->shaderClipDistance ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[38].str(pStruct->shaderCullDistance ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[39].str(pStruct->shaderFloat64 ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[40].str(pStruct->shaderInt64 ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[41].str(pStruct->shaderInt16 ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[42].str(pStruct->shaderResourceResidency ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[43].str(pStruct->shaderResourceMinLod ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[44].str(pStruct->sparseBinding ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[45].str(pStruct->sparseResidencyBuffer ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[46].str(pStruct->sparseResidencyImage2D ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[47].str(pStruct->sparseResidencyImage3D ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[48].str(pStruct->sparseResidency2Samples ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[49].str(pStruct->sparseResidency4Samples ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[50].str(pStruct->sparseResidency8Samples ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[51].str(pStruct->sparseResidency16Samples ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[52].str(pStruct->sparseResidencyAliased ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[53].str(pStruct->variableMultisampleRate ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[54].str(pStruct->inheritedQueries ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "robustBufferAccess = " + ss[0].str() + "\n" + prefix + "fullDrawIndexUint32 = " + ss[1].str() + "\n" + prefix + "imageCubeArray = " + ss[2].str() + "\n" + prefix + "independentBlend = " + ss[3].str() + "\n" + prefix + "geometryShader = " + ss[4].str() + "\n" + prefix + "tessellationShader = " + ss[5].str() + "\n" + prefix + "sampleRateShading = " + ss[6].str() + "\n" + prefix + "dualSrcBlend = " + ss[7].str() + "\n" + prefix + "logicOp = " + ss[8].str() + "\n" + prefix + "multiDrawIndirect = " + ss[9].str() + "\n" + prefix + "drawIndirectFirstInstance = " + ss[10].str() + "\n" + prefix + "depthClamp = " + ss[11].str() + "\n" + prefix + "depthBiasClamp = " + ss[12].str() + "\n" + prefix + "fillModeNonSolid = " + ss[13].str() + "\n" + prefix + "depthBounds = " + ss[14].str() + "\n" + prefix + "wideLines = " + ss[15].str() + "\n" + prefix + "largePoints = " + ss[16].str() + "\n" + prefix + "alphaToOne = " + ss[17].str() + "\n" + prefix + "multiViewport = " + ss[18].str() + "\n" + prefix + "samplerAnisotropy = " + ss[19].str() + "\n" + prefix + "textureCompressionETC2 = " + ss[20].str() + "\n" + prefix + "textureCompressionASTC_LDR = " + ss[21].str() + "\n" + prefix + "textureCompressionBC = " + ss[22].str() + "\n" + prefix + "occlusionQueryPrecise = " + ss[23].str() + "\n" + prefix + "pipelineStatisticsQuery = " + ss[24].str() + "\n" + prefix + "vertexPipelineStoresAndAtomics = " + ss[25].str() + "\n" + prefix + "fragmentStoresAndAtomics = " + ss[26].str() + "\n" + prefix + "shaderTessellationAndGeometryPointSize = " + ss[27].str() + "\n" + prefix + "shaderImageGatherExtended = " + ss[28].str() + "\n" + prefix + "shaderStorageImageExtendedFormats = " + ss[29].str() + "\n" + prefix + "shaderStorageImageMultisample = " + ss[30].str() + "\n" + prefix + "shaderStorageImageReadWithoutFormat = " + ss[31].str() + "\n" + prefix + "shaderStorageImageWriteWithoutFormat = " + ss[32].str() + "\n" + prefix + "shaderUniformBufferArrayDynamicIndexing = " + ss[33].str() + "\n" + prefix + "shaderSampledImageArrayDynamicIndexing = " + ss[34].str() + "\n" + prefix + "shaderStorageBufferArrayDynamicIndexing = " + ss[35].str() + "\n" + prefix + "shaderStorageImageArrayDynamicIndexing = " + ss[36].str() + "\n" + prefix + "shaderClipDistance = " + ss[37].str() + "\n" + prefix + "shaderCullDistance = " + ss[38].str() + "\n" + prefix + "shaderFloat64 = " + ss[39].str() + "\n" + prefix + "shaderInt64 = " + ss[40].str() + "\n" + prefix + "shaderInt16 = " + ss[41].str() + "\n" + prefix + "shaderResourceResidency = " + ss[42].str() + "\n" + prefix + "shaderResourceMinLod = " + ss[43].str() + "\n" + prefix + "sparseBinding = " + ss[44].str() + "\n" + prefix + "sparseResidencyBuffer = " + ss[45].str() + "\n" + prefix + "sparseResidencyImage2D = " + ss[46].str() + "\n" + prefix + "sparseResidencyImage3D = " + ss[47].str() + "\n" + prefix + "sparseResidency2Samples = " + ss[48].str() + "\n" + prefix + "sparseResidency4Samples = " + ss[49].str() + "\n" + prefix + "sparseResidency8Samples = " + ss[50].str() + "\n" + prefix + "sparseResidency16Samples = " + ss[51].str() + "\n" + prefix + "sparseResidencyAliased = " + ss[52].str() + "\n" + prefix + "variableMultisampleRate = " + ss[53].str() + "\n" + prefix + "inheritedQueries = " + ss[54].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkphysicaldevicelimits(const VkPhysicalDeviceLimits* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[106];
    string stp_strs[6];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[0] = "";
    stringstream index_ss;
    for (uint32_t i = 0; i < 3; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[0] << pStruct->maxComputeWorkGroupCount[i];
        stp_strs[0] += " " + prefix + "maxComputeWorkGroupCount[" + index_ss.str() + "] = " + ss[0].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[0].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    for (uint32_t i = 0; i < 3; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[1] << pStruct->maxComputeWorkGroupSize[i];
        stp_strs[1] += " " + prefix + "maxComputeWorkGroupSize[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[1].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    for (uint32_t i = 0; i < 2; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[2] << pStruct->maxViewportDimensions[i];
        stp_strs[2] += " " + prefix + "maxViewportDimensions[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[2].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    for (uint32_t i = 0; i < 2; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[3] << pStruct->viewportBoundsRange[i];
        stp_strs[3] += " " + prefix + "viewportBoundsRange[" + index_ss.str() + "] = " + ss[3].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[3].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[4] = "";
    for (uint32_t i = 0; i < 2; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[4] << pStruct->pointSizeRange[i];
        stp_strs[4] += " " + prefix + "pointSizeRange[" + index_ss.str() + "] = " + ss[4].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[4].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[5] = "";
    for (uint32_t i = 0; i < 2; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[5] << pStruct->lineWidthRange[i];
        stp_strs[5] += " " + prefix + "lineWidthRange[" + index_ss.str() + "] = " + ss[5].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[5].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->maxImageDimension1D;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->maxImageDimension2D;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->maxImageDimension3D;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->maxImageDimensionCube;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[4] << pStruct->maxImageArrayLayers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->maxTexelBufferElements << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->maxUniformBufferRange << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->maxStorageBufferRange << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[8] << "0x" << hex << nouppercase << pStruct->maxPushConstantsSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[9] << "0x" << hex << nouppercase << pStruct->maxMemoryAllocationCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[10] << "0x" << hex << nouppercase << pStruct->maxSamplerAllocationCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[11] << "0x" << hex << nouppercase << pStruct->bufferImageGranularity << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[12] << "0x" << hex << nouppercase << pStruct->sparseAddressSpaceSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[13] << pStruct->maxBoundDescriptorSets;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[14] << pStruct->maxPerStageDescriptorSamplers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[15] << "0x" << hex << nouppercase << pStruct->maxPerStageDescriptorUniformBuffers << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[16] << "0x" << hex << nouppercase << pStruct->maxPerStageDescriptorStorageBuffers << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[17] << pStruct->maxPerStageDescriptorSampledImages;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[18] << pStruct->maxPerStageDescriptorStorageImages;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[19] << pStruct->maxPerStageDescriptorInputAttachments;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[20] << pStruct->maxPerStageResources;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[21] << pStruct->maxDescriptorSetSamplers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[22] << "0x" << hex << nouppercase << pStruct->maxDescriptorSetUniformBuffers << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[23] << "0x" << hex << nouppercase << pStruct->maxDescriptorSetUniformBuffersDynamic << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[24] << "0x" << hex << nouppercase << pStruct->maxDescriptorSetStorageBuffers << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[25] << "0x" << hex << nouppercase << pStruct->maxDescriptorSetStorageBuffersDynamic << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[26] << pStruct->maxDescriptorSetSampledImages;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[27] << pStruct->maxDescriptorSetStorageImages;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[28] << pStruct->maxDescriptorSetInputAttachments;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[29] << pStruct->maxVertexInputAttributes;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[30] << pStruct->maxVertexInputBindings;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[31] << "0x" << hex << nouppercase << pStruct->maxVertexInputAttributeOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[32] << pStruct->maxVertexInputBindingStride;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[33] << pStruct->maxVertexOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[34] << pStruct->maxTessellationGenerationLevel;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[35] << "0x" << hex << nouppercase << pStruct->maxTessellationPatchSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[36] << pStruct->maxTessellationControlPerVertexInputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[37] << pStruct->maxTessellationControlPerVertexOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[38] << pStruct->maxTessellationControlPerPatchOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[39] << pStruct->maxTessellationControlTotalOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[40] << pStruct->maxTessellationEvaluationInputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[41] << pStruct->maxTessellationEvaluationOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[42] << pStruct->maxGeometryShaderInvocations;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[43] << pStruct->maxGeometryInputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[44] << pStruct->maxGeometryOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[45] << pStruct->maxGeometryOutputVertices;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[46] << pStruct->maxGeometryTotalOutputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[47] << pStruct->maxFragmentInputComponents;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[48] << pStruct->maxFragmentOutputAttachments;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[49] << pStruct->maxFragmentDualSrcAttachments;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[50] << pStruct->maxFragmentCombinedOutputResources;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[51] << "0x" << hex << nouppercase << pStruct->maxComputeSharedMemorySize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[52] << "0x" << hex << nouppercase << HandleCast(pStruct->maxComputeWorkGroupCount) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[53] << pStruct->maxComputeWorkGroupInvocations;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[54] << "0x" << hex << nouppercase << HandleCast(pStruct->maxComputeWorkGroupSize) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[55] << "0x" << hex << nouppercase << pStruct->subPixelPrecisionBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[56] << "0x" << hex << nouppercase << pStruct->subTexelPrecisionBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[57] << "0x" << hex << nouppercase << pStruct->mipmapPrecisionBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[58] << "0x" << hex << nouppercase << pStruct->maxDrawIndexedIndexValue << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[59] << "0x" << hex << nouppercase << pStruct->maxDrawIndirectCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[60] << pStruct->maxSamplerLodBias;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[61] << pStruct->maxSamplerAnisotropy;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[62] << pStruct->maxViewports;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[63] << "0x" << hex << nouppercase << HandleCast(pStruct->maxViewportDimensions) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[64] << "0x" << hex << nouppercase << HandleCast(pStruct->viewportBoundsRange) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[65] << "0x" << hex << nouppercase << pStruct->viewportSubPixelBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[66] << "0x" << hex << nouppercase << pStruct->minMemoryMapAlignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[67] << "0x" << hex << nouppercase << pStruct->minTexelBufferOffsetAlignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[68] << "0x" << hex << nouppercase << pStruct->minUniformBufferOffsetAlignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[69] << "0x" << hex << nouppercase << pStruct->minStorageBufferOffsetAlignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[70] << "0x" << hex << nouppercase << pStruct->minTexelOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[71] << "0x" << hex << nouppercase << pStruct->maxTexelOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[72] << "0x" << hex << nouppercase << pStruct->minTexelGatherOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[73] << "0x" << hex << nouppercase << pStruct->maxTexelGatherOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[74] << pStruct->minInterpolationOffset;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[75] << pStruct->maxInterpolationOffset;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[76] << "0x" << hex << nouppercase << pStruct->subPixelInterpolationOffsetBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[77] << "0x" << hex << nouppercase << pStruct->maxFramebufferWidth << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[78] << "0x" << hex << nouppercase << pStruct->maxFramebufferHeight << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[79] << "0x" << hex << nouppercase << pStruct->maxFramebufferLayers << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[80] << "0x" << hex << nouppercase << pStruct->framebufferColorSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[81] << "0x" << hex << nouppercase << pStruct->framebufferDepthSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[82] << "0x" << hex << nouppercase << pStruct->framebufferStencilSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[83] << "0x" << hex << nouppercase << pStruct->framebufferNoAttachmentsSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[84] << pStruct->maxColorAttachments;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[85] << "0x" << hex << nouppercase << pStruct->sampledImageColorSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[86] << "0x" << hex << nouppercase << pStruct->sampledImageIntegerSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[87] << "0x" << hex << nouppercase << pStruct->sampledImageDepthSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[88] << "0x" << hex << nouppercase << pStruct->sampledImageStencilSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[89] << "0x" << hex << nouppercase << pStruct->storageImageSampleCounts << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[90] << "0x" << hex << nouppercase << pStruct->maxSampleMaskWords << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[91].str(pStruct->timestampComputeAndGraphics ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[92] << pStruct->timestampPeriod;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[93] << pStruct->maxClipDistances;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[94] << pStruct->maxCullDistances;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[95] << pStruct->maxCombinedClipAndCullDistances;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[96] << pStruct->discreteQueuePriorities;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[97] << "0x" << hex << nouppercase << HandleCast(pStruct->pointSizeRange) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[98] << "0x" << hex << nouppercase << HandleCast(pStruct->lineWidthRange) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[99] << pStruct->pointSizeGranularity;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[100] << pStruct->lineWidthGranularity;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[101].str(pStruct->strictLines ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[102].str(pStruct->standardSampleLocations ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[103] << "0x" << hex << nouppercase << pStruct->optimalBufferCopyOffsetAlignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[104] << "0x" << hex << nouppercase << pStruct->optimalBufferCopyRowPitchAlignment << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[105] << "0x" << hex << nouppercase << pStruct->nonCoherentAtomSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "maxImageDimension1D = " + ss[0].str() + "\n" + prefix + "maxImageDimension2D = " + ss[1].str() + "\n" + prefix + "maxImageDimension3D = " + ss[2].str() + "\n" + prefix + "maxImageDimensionCube = " + ss[3].str() + "\n" + prefix + "maxImageArrayLayers = " + ss[4].str() + "\n" + prefix + "maxTexelBufferElements = " + ss[5].str() + "\n" + prefix + "maxUniformBufferRange = " + ss[6].str() + "\n" + prefix + "maxStorageBufferRange = " + ss[7].str() + "\n" + prefix + "maxPushConstantsSize = " + ss[8].str() + "\n" + prefix + "maxMemoryAllocationCount = " + ss[9].str() + "\n" + prefix + "maxSamplerAllocationCount = " + ss[10].str() + "\n" + prefix + "bufferImageGranularity = " + ss[11].str() + "\n" + prefix + "sparseAddressSpaceSize = " + ss[12].str() + "\n" + prefix + "maxBoundDescriptorSets = " + ss[13].str() + "\n" + prefix + "maxPerStageDescriptorSamplers = " + ss[14].str() + "\n" + prefix + "maxPerStageDescriptorUniformBuffers = " + ss[15].str() + "\n" + prefix + "maxPerStageDescriptorStorageBuffers = " + ss[16].str() + "\n" + prefix + "maxPerStageDescriptorSampledImages = " + ss[17].str() + "\n" + prefix + "maxPerStageDescriptorStorageImages = " + ss[18].str() + "\n" + prefix + "maxPerStageDescriptorInputAttachments = " + ss[19].str() + "\n" + prefix + "maxPerStageResources = " + ss[20].str() + "\n" + prefix + "maxDescriptorSetSamplers = " + ss[21].str() + "\n" + prefix + "maxDescriptorSetUniformBuffers = " + ss[22].str() + "\n" + prefix + "maxDescriptorSetUniformBuffersDynamic = " + ss[23].str() + "\n" + prefix + "maxDescriptorSetStorageBuffers = " + ss[24].str() + "\n" + prefix + "maxDescriptorSetStorageBuffersDynamic = " + ss[25].str() + "\n" + prefix + "maxDescriptorSetSampledImages = " + ss[26].str() + "\n" + prefix + "maxDescriptorSetStorageImages = " + ss[27].str() + "\n" + prefix + "maxDescriptorSetInputAttachments = " + ss[28].str() + "\n" + prefix + "maxVertexInputAttributes = " + ss[29].str() + "\n" + prefix + "maxVertexInputBindings = " + ss[30].str() + "\n" + prefix + "maxVertexInputAttributeOffset = " + ss[31].str() + "\n" + prefix + "maxVertexInputBindingStride = " + ss[32].str() + "\n" + prefix + "maxVertexOutputComponents = " + ss[33].str() + "\n" + prefix + "maxTessellationGenerationLevel = " + ss[34].str() + "\n" + prefix + "maxTessellationPatchSize = " + ss[35].str() + "\n" + prefix + "maxTessellationControlPerVertexInputComponents = " + ss[36].str() + "\n" + prefix + "maxTessellationControlPerVertexOutputComponents = " + ss[37].str() + "\n" + prefix + "maxTessellationControlPerPatchOutputComponents = " + ss[38].str() + "\n" + prefix + "maxTessellationControlTotalOutputComponents = " + ss[39].str() + "\n" + prefix + "maxTessellationEvaluationInputComponents = " + ss[40].str() + "\n" + prefix + "maxTessellationEvaluationOutputComponents = " + ss[41].str() + "\n" + prefix + "maxGeometryShaderInvocations = " + ss[42].str() + "\n" + prefix + "maxGeometryInputComponents = " + ss[43].str() + "\n" + prefix + "maxGeometryOutputComponents = " + ss[44].str() + "\n" + prefix + "maxGeometryOutputVertices = " + ss[45].str() + "\n" + prefix + "maxGeometryTotalOutputComponents = " + ss[46].str() + "\n" + prefix + "maxFragmentInputComponents = " + ss[47].str() + "\n" + prefix + "maxFragmentOutputAttachments = " + ss[48].str() + "\n" + prefix + "maxFragmentDualSrcAttachments = " + ss[49].str() + "\n" + prefix + "maxFragmentCombinedOutputResources = " + ss[50].str() + "\n" + prefix + "maxComputeSharedMemorySize = " + ss[51].str() + "\n" + prefix + "maxComputeWorkGroupCount = " + ss[52].str() + "\n" + prefix + "maxComputeWorkGroupInvocations = " + ss[53].str() + "\n" + prefix + "maxComputeWorkGroupSize = " + ss[54].str() + "\n" + prefix + "subPixelPrecisionBits = " + ss[55].str() + "\n" + prefix + "subTexelPrecisionBits = " + ss[56].str() + "\n" + prefix + "mipmapPrecisionBits = " + ss[57].str() + "\n" + prefix + "maxDrawIndexedIndexValue = " + ss[58].str() + "\n" + prefix + "maxDrawIndirectCount = " + ss[59].str() + "\n" + prefix + "maxSamplerLodBias = " + ss[60].str() + "\n" + prefix + "maxSamplerAnisotropy = " + ss[61].str() + "\n" + prefix + "maxViewports = " + ss[62].str() + "\n" + prefix + "maxViewportDimensions = " + ss[63].str() + "\n" + prefix + "viewportBoundsRange = " + ss[64].str() + "\n" + prefix + "viewportSubPixelBits = " + ss[65].str() + "\n" + prefix + "minMemoryMapAlignment = " + ss[66].str() + "\n" + prefix + "minTexelBufferOffsetAlignment = " + ss[67].str() + "\n" + prefix + "minUniformBufferOffsetAlignment = " + ss[68].str() + "\n" + prefix + "minStorageBufferOffsetAlignment = " + ss[69].str() + "\n" + prefix + "minTexelOffset = " + ss[70].str() + "\n" + prefix + "maxTexelOffset = " + ss[71].str() + "\n" + prefix + "minTexelGatherOffset = " + ss[72].str() + "\n" + prefix + "maxTexelGatherOffset = " + ss[73].str() + "\n" + prefix + "minInterpolationOffset = " + ss[74].str() + "\n" + prefix + "maxInterpolationOffset = " + ss[75].str() + "\n" + prefix + "subPixelInterpolationOffsetBits = " + ss[76].str() + "\n" + prefix + "maxFramebufferWidth = " + ss[77].str() + "\n" + prefix + "maxFramebufferHeight = " + ss[78].str() + "\n" + prefix + "maxFramebufferLayers = " + ss[79].str() + "\n" + prefix + "framebufferColorSampleCounts = " + ss[80].str() + "\n" + prefix + "framebufferDepthSampleCounts = " + ss[81].str() + "\n" + prefix + "framebufferStencilSampleCounts = " + ss[82].str() + "\n" + prefix + "framebufferNoAttachmentsSampleCounts = " + ss[83].str() + "\n" + prefix + "maxColorAttachments = " + ss[84].str() + "\n" + prefix + "sampledImageColorSampleCounts = " + ss[85].str() + "\n" + prefix + "sampledImageIntegerSampleCounts = " + ss[86].str() + "\n" + prefix + "sampledImageDepthSampleCounts = " + ss[87].str() + "\n" + prefix + "sampledImageStencilSampleCounts = " + ss[88].str() + "\n" + prefix + "storageImageSampleCounts = " + ss[89].str() + "\n" + prefix + "maxSampleMaskWords = " + ss[90].str() + "\n" + prefix + "timestampComputeAndGraphics = " + ss[91].str() + "\n" + prefix + "timestampPeriod = " + ss[92].str() + "\n" + prefix + "maxClipDistances = " + ss[93].str() + "\n" + prefix + "maxCullDistances = " + ss[94].str() + "\n" + prefix + "maxCombinedClipAndCullDistances = " + ss[95].str() + "\n" + prefix + "discreteQueuePriorities = " + ss[96].str() + "\n" + prefix + "pointSizeRange = " + ss[97].str() + "\n" + prefix + "lineWidthRange = " + ss[98].str() + "\n" + prefix + "pointSizeGranularity = " + ss[99].str() + "\n" + prefix + "lineWidthGranularity = " + ss[100].str() + "\n" + prefix + "strictLines = " + ss[101].str() + "\n" + prefix + "standardSampleLocations = " + ss[102].str() + "\n" + prefix + "optimalBufferCopyOffsetAlignment = " + ss[103].str() + "\n" + prefix + "optimalBufferCopyRowPitchAlignment = " + ss[104].str() + "\n" + prefix + "nonCoherentAtomSize = " + ss[105].str() + "\n" + stp_strs[5] + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkphysicaldevicememoryproperties(const VkPhysicalDeviceMemoryProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[0] = "";
    stringstream index_ss;
    for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
        tmp_str = vk_print_vkmemorytype(&pStruct->memoryTypes[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
        stp_strs[0] += " " + prefix + "memoryTypes[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[0].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    for (uint32_t i = 0; i < VK_MAX_MEMORY_HEAPS; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
        tmp_str = vk_print_vkmemoryheap(&pStruct->memoryHeaps[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
        stp_strs[1] += " " + prefix + "memoryHeaps[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[1].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->memoryTypeCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->memoryTypes) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->memoryHeapCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->memoryHeaps) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "memoryTypeCount = " + ss[0].str() + "\n" + prefix + "memoryTypes = " + ss[1].str() + "\n" + prefix + "memoryHeapCount = " + ss[2].str() + "\n" + prefix + "memoryHeaps = " + ss[3].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkphysicaldeviceproperties(const VkPhysicalDeviceProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    for (uint32_t i = 0; i < VK_UUID_SIZE; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[1] << "0x" << hex << nouppercase << static_cast<const unsigned>(pStruct->pipelineCacheUUID[i]) << dec;
        stp_strs[1] += " " + prefix + "pipelineCacheUUID[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[1].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkphysicaldevicelimits(&pStruct->limits, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "limits:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkphysicaldevicesparseproperties(&pStruct->sparseProperties, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[3] = " " + prefix + "sparseProperties:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->apiVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->driverVersion;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->vendorID;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->deviceID;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1093
    ss[4] << pStruct->deviceName;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pipelineCacheUUID) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[6] << "0x" << hex << nouppercase << HandleCast(&pStruct->limits) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[7] << "0x" << hex << nouppercase << HandleCast(&pStruct->sparseProperties) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "apiVersion = " + ss[0].str() + "\n" + prefix + "driverVersion = " + ss[1].str() + "\n" + prefix + "vendorID = " + ss[2].str() + "\n" + prefix + "deviceID = " + ss[3].str() + "\n" + prefix + "deviceType = " + string_VkPhysicalDeviceType(pStruct->deviceType) + "\n" + prefix + "deviceName = " + ss[4].str() + "\n" + prefix + "pipelineCacheUUID = " + ss[5].str() + "\n" + prefix + "limits = " + ss[6].str() + "\n" + prefix + "sparseProperties = " + ss[7].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkphysicaldevicesparseproperties(const VkPhysicalDeviceSparseProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[0].str(pStruct->residencyStandard2DBlockShape ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[1].str(pStruct->residencyStandard2DMultisampleBlockShape ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->residencyStandard3DBlockShape ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[3].str(pStruct->residencyAlignedMipSize ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[4].str(pStruct->residencyNonResidentStrict ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "residencyStandard2DBlockShape = " + ss[0].str() + "\n" + prefix + "residencyStandard2DMultisampleBlockShape = " + ss[1].str() + "\n" + prefix + "residencyStandard3DBlockShape = " + ss[2].str() + "\n" + prefix + "residencyAlignedMipSize = " + ss[3].str() + "\n" + prefix + "residencyNonResidentStrict = " + ss[4].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinecachecreateinfo(const VkPipelineCacheCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->initialDataSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pInitialData) << dec;
    else
        ss[3].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "initialDataSize = " + ss[2].str() + "\n" + prefix + "pInitialData = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinecolorblendattachmentstate(const VkPipelineColorBlendAttachmentState* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[0].str(pStruct->blendEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->colorWriteMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "blendEnable = " + ss[0].str() + "\n" + prefix + "srcColorBlendFactor = " + string_VkBlendFactor(pStruct->srcColorBlendFactor) + "\n" + prefix + "dstColorBlendFactor = " + string_VkBlendFactor(pStruct->dstColorBlendFactor) + "\n" + prefix + "colorBlendOp = " + string_VkBlendOp(pStruct->colorBlendOp) + "\n" + prefix + "srcAlphaBlendFactor = " + string_VkBlendFactor(pStruct->srcAlphaBlendFactor) + "\n" + prefix + "dstAlphaBlendFactor = " + string_VkBlendFactor(pStruct->dstAlphaBlendFactor) + "\n" + prefix + "alphaBlendOp = " + string_VkBlendOp(pStruct->alphaBlendOp) + "\n" + prefix + "colorWriteMask = " + ss[1].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinecolorblendstatecreateinfo(const VkPipelineColorBlendStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pAttachments) {
        for (uint32_t i = 0; i < pStruct->attachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkpipelinecolorblendattachmentstate(&pStruct->pAttachments[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pAttachments[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #966
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    for (uint32_t i = 0; i < 4; i++) {
        index_ss.str("");
        index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
        ss[2] << pStruct->blendConstants[i];
        stp_strs[2] += " " + prefix + "blendConstants[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
        ss[2].str("");
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->logicOpEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->attachmentCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->blendConstants) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "logicOpEnable = " + ss[2].str() + "\n" + prefix + "logicOp = " + string_VkLogicOp(pStruct->logicOp) + "\n" + prefix + "attachmentCount = " + ss[3].str() + "\n" + prefix + "pAttachments = " + ss[4].str() + "\n" + prefix + "blendConstants = " + ss[5].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinedepthstencilstatecreateinfo(const VkPipelineDepthStencilStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[10];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkstencilopstate(&pStruct->front, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "front:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkstencilopstate(&pStruct->back, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "back:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->depthTestEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[3].str(pStruct->depthWriteEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[4].str(pStruct->depthBoundsTestEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[5].str(pStruct->stencilTestEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[6] << "0x" << hex << nouppercase << HandleCast(&pStruct->front) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[7] << "0x" << hex << nouppercase << HandleCast(&pStruct->back) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[8] << pStruct->minDepthBounds;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[9] << pStruct->maxDepthBounds;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "depthTestEnable = " + ss[2].str() + "\n" + prefix + "depthWriteEnable = " + ss[3].str() + "\n" + prefix + "depthCompareOp = " + string_VkCompareOp(pStruct->depthCompareOp) + "\n" + prefix + "depthBoundsTestEnable = " + ss[4].str() + "\n" + prefix + "stencilTestEnable = " + ss[5].str() + "\n" + prefix + "front = " + ss[6].str() + "\n" + prefix + "back = " + ss[7].str() + "\n" + prefix + "minDepthBounds = " + ss[8].str() + "\n" + prefix + "maxDepthBounds = " + ss[9].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinedynamicstatecreateinfo(const VkPipelineDynamicStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pDynamicStates) {
        for (uint32_t i = 0; i < pStruct->dynamicStateCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1003
            ss[1] << string_VkDynamicState(pStruct->pDynamicStates[i]);
            stp_strs[1] += " " + prefix + "pDynamicStates[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->dynamicStateCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1136
    if (pStruct->pDynamicStates)
        ss[3] << "0x" << hex << nouppercase << pStruct->pDynamicStates << dec << " (See individual array values below)";
    else
        ss[3].str("NULL");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "dynamicStateCount = " + ss[2].str() + "\n" + prefix + "pDynamicStates = " + ss[3].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelineinputassemblystatecreateinfo(const VkPipelineInputAssemblyStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->primitiveRestartEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "topology = " + string_VkPrimitiveTopology(pStruct->topology) + "\n" + prefix + "primitiveRestartEnable = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinelayoutcreateinfo(const VkPipelineLayoutCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pSetLayouts) {
        for (uint32_t i = 0; i < pStruct->setLayoutCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pSetLayouts[i]) << dec;
            stp_strs[1] += " " + prefix + "pSetLayouts[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pPushConstantRanges) {
        for (uint32_t i = 0; i < pStruct->pushConstantRangeCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkpushconstantrange(&pStruct->pPushConstantRanges[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[2] += " " + prefix + "pPushConstantRanges[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->setLayoutCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pSetLayouts) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->pushConstantRangeCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pPushConstantRanges) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "setLayoutCount = " + ss[2].str() + "\n" + prefix + "pSetLayouts = " + ss[3].str() + "\n" + prefix + "pushConstantRangeCount = " + ss[4].str() + "\n" + prefix + "pPushConstantRanges = " + ss[5].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinemultisamplestatecreateinfo(const VkPipelineMultisampleStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->sampleShadingEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->minSampleShading;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pSampleMask) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[5].str(pStruct->alphaToCoverageEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[6].str(pStruct->alphaToOneEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "rasterizationSamples = " + string_VkSampleCountFlagBits(pStruct->rasterizationSamples) + "\n" + prefix + "sampleShadingEnable = " + ss[2].str() + "\n" + prefix + "minSampleShading = " + ss[3].str() + "\n" + prefix + "pSampleMask = " + ss[4].str() + "\n" + prefix + "alphaToCoverageEnable = " + ss[5].str() + "\n" + prefix + "alphaToOneEnable = " + ss[6].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinerasterizationstatecreateinfo(const VkPipelineRasterizationStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[10];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[2].str(pStruct->depthClampEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[3].str(pStruct->rasterizerDiscardEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->cullMode << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[5].str(pStruct->depthBiasEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[6] << pStruct->depthBiasConstantFactor;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[7] << pStruct->depthBiasClamp;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[8] << pStruct->depthBiasSlopeFactor;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[9] << pStruct->lineWidth;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "depthClampEnable = " + ss[2].str() + "\n" + prefix + "rasterizerDiscardEnable = " + ss[3].str() + "\n" + prefix + "polygonMode = " + string_VkPolygonMode(pStruct->polygonMode) + "\n" + prefix + "cullMode = " + ss[4].str() + "\n" + prefix + "frontFace = " + string_VkFrontFace(pStruct->frontFace) + "\n" + prefix + "depthBiasEnable = " + ss[5].str() + "\n" + prefix + "depthBiasConstantFactor = " + ss[6].str() + "\n" + prefix + "depthBiasClamp = " + ss[7].str() + "\n" + prefix + "depthBiasSlopeFactor = " + ss[8].str() + "\n" + prefix + "lineWidth = " + ss[9].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinerasterizationstaterasterizationorderamd(const VkPipelineRasterizationStateRasterizationOrderAMD* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[1];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "rasterizationOrder = " + string_VkRasterizationOrderAMD(pStruct->rasterizationOrder) + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelineshaderstagecreateinfo(const VkPipelineShaderStageCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pSpecializationInfo) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkspecializationinfo(pStruct->pSpecializationInfo, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[2] = " " + prefix + "pSpecializationInfo:\n" + tmp_str;
        ss[2].str("");
    }
    else
        stp_strs[2] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->module) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1111
    if (pStruct->pName != NULL) {
        ss[3] << pStruct->pName;
     } else {
        ss[3] << "";
     }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pSpecializationInfo) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "stage = " + string_VkShaderStageFlagBits(pStruct->stage) + "\n" + prefix + "module = " + ss[2].str() + "\n" + prefix + "pName = " + ss[3].str() + "\n" + prefix + "pSpecializationInfo = " + ss[4].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinetessellationstatecreateinfo(const VkPipelineTessellationStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->patchControlPoints;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "patchControlPoints = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelinevertexinputstatecreateinfo(const VkPipelineVertexInputStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pVertexBindingDescriptions) {
        for (uint32_t i = 0; i < pStruct->vertexBindingDescriptionCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkvertexinputbindingdescription(&pStruct->pVertexBindingDescriptions[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pVertexBindingDescriptions[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pVertexAttributeDescriptions) {
        for (uint32_t i = 0; i < pStruct->vertexAttributeDescriptionCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkvertexinputattributedescription(&pStruct->pVertexAttributeDescriptions[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[2] += " " + prefix + "pVertexAttributeDescriptions[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->vertexBindingDescriptionCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pVertexBindingDescriptions) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->vertexAttributeDescriptionCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pVertexAttributeDescriptions) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "vertexBindingDescriptionCount = " + ss[2].str() + "\n" + prefix + "pVertexBindingDescriptions = " + ss[3].str() + "\n" + prefix + "vertexAttributeDescriptionCount = " + ss[4].str() + "\n" + prefix + "pVertexAttributeDescriptions = " + ss[5].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpipelineviewportstatecreateinfo(const VkPipelineViewportStateCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pViewports) {
        for (uint32_t i = 0; i < pStruct->viewportCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkviewport(&pStruct->pViewports[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pViewports[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pScissors) {
        for (uint32_t i = 0; i < pStruct->scissorCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkrect2d(&pStruct->pScissors[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[2] += " " + prefix + "pScissors[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->viewportCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pViewports) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->scissorCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pScissors) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "viewportCount = " + ss[2].str() + "\n" + prefix + "pViewports = " + ss[3].str() + "\n" + prefix + "scissorCount = " + ss[4].str() + "\n" + prefix + "pScissors = " + ss[5].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpresentinfokhr(const VkPresentInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pWaitSemaphores) {
        for (uint32_t i = 0; i < pStruct->waitSemaphoreCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitSemaphores[i]) << dec;
            stp_strs[1] += " " + prefix + "pWaitSemaphores[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pSwapchains) {
        for (uint32_t i = 0; i < pStruct->swapchainCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pSwapchains[i]) << dec;
            stp_strs[2] += " " + prefix + "pSwapchains[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    if (pStruct->pImageIndices) {
        for (uint32_t i = 0; i < pStruct->swapchainCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
            ss[3] << "0x" << hex << nouppercase << pStruct->pImageIndices[i] << dec;
            stp_strs[3] += " " + prefix + "pImageIndices[" + index_ss.str() + "] = " + ss[3].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[3].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->waitSemaphoreCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitSemaphores) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->swapchainCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pSwapchains) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pImageIndices) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1136
    if (pStruct->pResults)
        ss[6] << "0x" << hex << nouppercase << pStruct->pResults << dec << " (See individual array values below)";
    else
        ss[6].str("NULL");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "waitSemaphoreCount = " + ss[1].str() + "\n" + prefix + "pWaitSemaphores = " + ss[2].str() + "\n" + prefix + "swapchainCount = " + ss[3].str() + "\n" + prefix + "pSwapchains = " + ss[4].str() + "\n" + prefix + "pImageIndices = " + ss[5].str() + "\n" + prefix + "pResults = " + ss[6].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkpushconstantrange(const VkPushConstantRange* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->stageFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "stageFlags = " + ss[0].str() + "\n" + prefix + "offset = " + ss[1].str() + "\n" + prefix + "size = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkquerypoolcreateinfo(const VkQueryPoolCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->queryCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->pipelineStatistics << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "queryType = " + string_VkQueryType(pStruct->queryType) + "\n" + prefix + "queryCount = " + ss[2].str() + "\n" + prefix + "pipelineStatistics = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkqueuefamilyproperties(const VkQueueFamilyProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->minImageTransferGranularity, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "minImageTransferGranularity:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->queueFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->queueCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->timestampValidBits << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->minImageTransferGranularity) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "queueFlags = " + ss[0].str() + "\n" + prefix + "queueCount = " + ss[1].str() + "\n" + prefix + "timestampValidBits = " + ss[2].str() + "\n" + prefix + "minImageTransferGranularity = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkrect2d(const VkRect2D* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset2d(&pStruct->offset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "offset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->extent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "extent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->offset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->extent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "offset = " + ss[0].str() + "\n" + prefix + "extent = " + ss[1].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkrenderpassbegininfo(const VkRenderPassBeginInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkrect2d(&pStruct->renderArea, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[3] = " " + prefix + "renderArea:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[4] = "";
    stringstream index_ss;
    if (pStruct->pClearValues) {
        for (uint32_t i = 0; i < pStruct->clearValueCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkclearvalue(&pStruct->pClearValues[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[4] += " " + prefix + "pClearValues[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[4].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->renderPass) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->framebuffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->renderArea) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->clearValueCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pClearValues) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "renderPass = " + ss[1].str() + "\n" + prefix + "framebuffer = " + ss[2].str() + "\n" + prefix + "renderArea = " + ss[3].str() + "\n" + prefix + "clearValueCount = " + ss[4].str() + "\n" + prefix + "pClearValues = " + ss[5].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkrenderpasscreateinfo(const VkRenderPassCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pAttachments) {
        for (uint32_t i = 0; i < pStruct->attachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkattachmentdescription(&pStruct->pAttachments[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pAttachments[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pSubpasses) {
        for (uint32_t i = 0; i < pStruct->subpassCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksubpassdescription(&pStruct->pSubpasses[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[2] += " " + prefix + "pSubpasses[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    if (pStruct->pDependencies) {
        for (uint32_t i = 0; i < pStruct->dependencyCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksubpassdependency(&pStruct->pDependencies[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[3] += " " + prefix + "pDependencies[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[3].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->attachmentCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->subpassCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pSubpasses) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->dependencyCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->pDependencies) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "attachmentCount = " + ss[2].str() + "\n" + prefix + "pAttachments = " + ss[3].str() + "\n" + prefix + "subpassCount = " + ss[4].str() + "\n" + prefix + "pSubpasses = " + ss[5].str() + "\n" + prefix + "dependencyCount = " + ss[6].str() + "\n" + prefix + "pDependencies = " + ss[7].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksamplercreateinfo(const VkSamplerCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[9];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->mipLodBias;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[3].str(pStruct->anisotropyEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[4] << pStruct->maxAnisotropy;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[5].str(pStruct->compareEnable ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[6] << pStruct->minLod;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[7] << pStruct->maxLod;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[8].str(pStruct->unnormalizedCoordinates ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "magFilter = " + string_VkFilter(pStruct->magFilter) + "\n" + prefix + "minFilter = " + string_VkFilter(pStruct->minFilter) + "\n" + prefix + "mipmapMode = " + string_VkSamplerMipmapMode(pStruct->mipmapMode) + "\n" + prefix + "addressModeU = " + string_VkSamplerAddressMode(pStruct->addressModeU) + "\n" + prefix + "addressModeV = " + string_VkSamplerAddressMode(pStruct->addressModeV) + "\n" + prefix + "addressModeW = " + string_VkSamplerAddressMode(pStruct->addressModeW) + "\n" + prefix + "mipLodBias = " + ss[2].str() + "\n" + prefix + "anisotropyEnable = " + ss[3].str() + "\n" + prefix + "maxAnisotropy = " + ss[4].str() + "\n" + prefix + "compareEnable = " + ss[5].str() + "\n" + prefix + "compareOp = " + string_VkCompareOp(pStruct->compareOp) + "\n" + prefix + "minLod = " + ss[6].str() + "\n" + prefix + "maxLod = " + ss[7].str() + "\n" + prefix + "borderColor = " + string_VkBorderColor(pStruct->borderColor) + "\n" + prefix + "unnormalizedCoordinates = " + ss[8].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksemaphorecreateinfo(const VkSemaphoreCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkshadermodulecreateinfo(const VkShaderModuleCreateInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->codeSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pCode) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "codeSize = " + ss[2].str() + "\n" + prefix + "pCode = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparsebuffermemorybindinfo(const VkSparseBufferMemoryBindInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pBinds) {
        for (uint32_t i = 0; i < pStruct->bindCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksparsememorybind(&pStruct->pBinds[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pBinds[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->buffer) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->bindCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pBinds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "buffer = " + ss[0].str() + "\n" + prefix + "bindCount = " + ss[1].str() + "\n" + prefix + "pBinds = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparseimageformatproperties(const VkSparseImageFormatProperties* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->imageGranularity, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "imageGranularity:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->aspectMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageGranularity) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "aspectMask = " + ss[0].str() + "\n" + prefix + "imageGranularity = " + ss[1].str() + "\n" + prefix + "flags = " + ss[2].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparseimagememorybind(const VkSparseImageMemoryBind* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkimagesubresource(&pStruct->subresource, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "subresource:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkoffset3d(&pStruct->offset, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "offset:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent3d(&pStruct->extent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "extent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->subresource) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[1] << "0x" << hex << nouppercase << HandleCast(&pStruct->offset) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->extent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->memory) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->memoryOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "subresource = " + ss[0].str() + "\n" + prefix + "offset = " + ss[1].str() + "\n" + prefix + "extent = " + ss[2].str() + "\n" + prefix + "memory = " + ss[3].str() + "\n" + prefix + "memoryOffset = " + ss[4].str() + "\n" + prefix + "flags = " + ss[5].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparseimagememorybindinfo(const VkSparseImageMemoryBindInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pBinds) {
        for (uint32_t i = 0; i < pStruct->bindCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksparseimagememorybind(&pStruct->pBinds[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pBinds[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->image) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->bindCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pBinds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "image = " + ss[0].str() + "\n" + prefix + "bindCount = " + ss[1].str() + "\n" + prefix + "pBinds = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparseimagememoryrequirements(const VkSparseImageMemoryRequirements* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vksparseimageformatproperties(&pStruct->formatProperties, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "formatProperties:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[0] << "0x" << hex << nouppercase << HandleCast(&pStruct->formatProperties) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->imageMipTailFirstLod;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->imageMipTailSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->imageMipTailOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->imageMipTailStride << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "formatProperties = " + ss[0].str() + "\n" + prefix + "imageMipTailFirstLod = " + ss[1].str() + "\n" + prefix + "imageMipTailSize = " + ss[2].str() + "\n" + prefix + "imageMipTailOffset = " + ss[3].str() + "\n" + prefix + "imageMipTailStride = " + ss[4].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparseimageopaquememorybindinfo(const VkSparseImageOpaqueMemoryBindInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
    string stp_strs[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pBinds) {
        for (uint32_t i = 0; i < pStruct->bindCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vksparsememorybind(&pStruct->pBinds[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pBinds[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->image) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->bindCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pBinds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "image = " + ss[0].str() + "\n" + prefix + "bindCount = " + ss[1].str() + "\n" + prefix + "pBinds = " + ss[2].str() + "\n" + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksparsememorybind(const VkSparseMemoryBind* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->resourceOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->memory) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->memoryOffset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "resourceOffset = " + ss[0].str() + "\n" + prefix + "size = " + ss[1].str() + "\n" + prefix + "memory = " + ss[2].str() + "\n" + prefix + "memoryOffset = " + ss[3].str() + "\n" + prefix + "flags = " + ss[4].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkspecializationinfo(const VkSpecializationInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[0] = "";
    stringstream index_ss;
    if (pStruct->pMapEntries) {
        for (uint32_t i = 0; i < pStruct->mapEntryCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkspecializationmapentry(&pStruct->pMapEntries[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[0] += " " + prefix + "pMapEntries[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[0].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->mapEntryCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pMapEntries) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->dataSize << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pData) << dec;
    else
        ss[3].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "mapEntryCount = " + ss[0].str() + "\n" + prefix + "pMapEntries = " + ss[1].str() + "\n" + prefix + "dataSize = " + ss[2].str() + "\n" + prefix + "pData = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkspecializationmapentry(const VkSpecializationMapEntry* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->constantID;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "constantID = " + ss[0].str() + "\n" + prefix + "offset = " + ss[1].str() + "\n" + prefix + "size = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkstencilopstate(const VkStencilOpState* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->compareMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->writeMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->reference;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "failOp = " + string_VkStencilOp(pStruct->failOp) + "\n" + prefix + "passOp = " + string_VkStencilOp(pStruct->passOp) + "\n" + prefix + "depthFailOp = " + string_VkStencilOp(pStruct->depthFailOp) + "\n" + prefix + "compareOp = " + string_VkCompareOp(pStruct->compareOp) + "\n" + prefix + "compareMask = " + ss[0].str() + "\n" + prefix + "writeMask = " + ss[1].str() + "\n" + prefix + "reference = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksubmitinfo(const VkSubmitInfo* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[4];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pWaitSemaphores) {
        for (uint32_t i = 0; i < pStruct->waitSemaphoreCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitSemaphores[i]) << dec;
            stp_strs[1] += " " + prefix + "pWaitSemaphores[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pCommandBuffers) {
        for (uint32_t i = 0; i < pStruct->commandBufferCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pCommandBuffers[i]) << dec;
            stp_strs[2] += " " + prefix + "pCommandBuffers[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    if (pStruct->pSignalSemaphores) {
        for (uint32_t i = 0; i < pStruct->signalSemaphoreCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pSignalSemaphores[i]) << dec;
            stp_strs[3] += " " + prefix + "pSignalSemaphores[" + index_ss.str() + "] = " + ss[3].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[3].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->waitSemaphoreCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitSemaphores) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pWaitDstStageMask) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->commandBufferCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pCommandBuffers) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->signalSemaphoreCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->pSignalSemaphores) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "waitSemaphoreCount = " + ss[1].str() + "\n" + prefix + "pWaitSemaphores = " + ss[2].str() + "\n" + prefix + "pWaitDstStageMask = " + ss[3].str() + "\n" + prefix + "commandBufferCount = " + ss[4].str() + "\n" + prefix + "pCommandBuffers = " + ss[5].str() + "\n" + prefix + "signalSemaphoreCount = " + ss[6].str() + "\n" + prefix + "pSignalSemaphores = " + ss[7].str() + "\n" + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksubpassdependency(const VkSubpassDependency* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[7];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->srcSubpass;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->dstSubpass;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->srcStageMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->dstStageMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->srcAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->dstAccessMask << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->dependencyFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "srcSubpass = " + ss[0].str() + "\n" + prefix + "dstSubpass = " + ss[1].str() + "\n" + prefix + "srcStageMask = " + ss[2].str() + "\n" + prefix + "dstStageMask = " + ss[3].str() + "\n" + prefix + "srcAccessMask = " + ss[4].str() + "\n" + prefix + "dstAccessMask = " + ss[5].str() + "\n" + prefix + "dependencyFlags = " + ss[6].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksubpassdescription(const VkSubpassDescription* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[9];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[0] = "";
    stringstream index_ss;
    if (pStruct->pInputAttachments) {
        for (uint32_t i = 0; i < pStruct->inputAttachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkattachmentreference(&pStruct->pInputAttachments[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[0] += " " + prefix + "pInputAttachments[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[0].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    if (pStruct->pColorAttachments) {
        for (uint32_t i = 0; i < pStruct->colorAttachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkattachmentreference(&pStruct->pColorAttachments[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[1] += " " + prefix + "pColorAttachments[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pResolveAttachments) {
        for (uint32_t i = 0; i < pStruct->colorAttachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
            tmp_str = vk_print_vkattachmentreference(&pStruct->pResolveAttachments[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
            stp_strs[2] += " " + prefix + "pResolveAttachments[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pDepthStencilAttachment) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1054
        tmp_str = vk_print_vkattachmentreference(pStruct->pDepthStencilAttachment, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[3] = " " + prefix + "pDepthStencilAttachment:\n" + tmp_str;
        ss[3].str("");
    }
    else
        stp_strs[3] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[4] = "";
    if (pStruct->pPreserveAttachments) {
        for (uint32_t i = 0; i < pStruct->preserveAttachmentCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
            ss[4] << "0x" << hex << nouppercase << pStruct->pPreserveAttachments[i] << dec;
            stp_strs[4] += " " + prefix + "pPreserveAttachments[" + index_ss.str() + "] = " + ss[4].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[4].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->inputAttachmentCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pInputAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->colorAttachmentCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pColorAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pResolveAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[6] << "0x" << hex << nouppercase << HandleCast(pStruct->pDepthStencilAttachment) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->preserveAttachmentCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[8] << "0x" << hex << nouppercase << HandleCast(pStruct->pPreserveAttachments) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "flags = " + ss[0].str() + "\n" + prefix + "pipelineBindPoint = " + string_VkPipelineBindPoint(pStruct->pipelineBindPoint) + "\n" + prefix + "inputAttachmentCount = " + ss[1].str() + "\n" + prefix + "pInputAttachments = " + ss[2].str() + "\n" + prefix + "colorAttachmentCount = " + ss[3].str() + "\n" + prefix + "pColorAttachments = " + ss[4].str() + "\n" + prefix + "pResolveAttachments = " + ss[5].str() + "\n" + prefix + "pDepthStencilAttachment = " + ss[6].str() + "\n" + prefix + "preserveAttachmentCount = " + ss[7].str() + "\n" + prefix + "pPreserveAttachments = " + ss[8].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksubresourcelayout(const VkSubresourceLayout* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->size << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->rowPitch << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->arrayPitch << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->depthPitch << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "offset = " + ss[0].str() + "\n" + prefix + "size = " + ss[1].str() + "\n" + prefix + "rowPitch = " + ss[2].str() + "\n" + prefix + "arrayPitch = " + ss[3].str() + "\n" + prefix + "depthPitch = " + ss[4].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksurfacecapabilitieskhr(const VkSurfaceCapabilitiesKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[9];
    string stp_strs[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->currentExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[0] = " " + prefix + "currentExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->minImageExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[1] = " " + prefix + "minImageExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->maxImageExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "maxImageExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[0] << "0x" << hex << nouppercase << pStruct->minImageCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->maxImageCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[2] << "0x" << hex << nouppercase << HandleCast(&pStruct->currentExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[3] << "0x" << hex << nouppercase << HandleCast(&pStruct->minImageExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->maxImageExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->maxImageArrayLayers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->supportedTransforms << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->supportedCompositeAlpha << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[8] << "0x" << hex << nouppercase << pStruct->supportedUsageFlags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "minImageCount = " + ss[0].str() + "\n" + prefix + "maxImageCount = " + ss[1].str() + "\n" + prefix + "currentExtent = " + ss[2].str() + "\n" + prefix + "minImageExtent = " + ss[3].str() + "\n" + prefix + "maxImageExtent = " + ss[4].str() + "\n" + prefix + "maxImageArrayLayers = " + ss[5].str() + "\n" + prefix + "supportedTransforms = " + ss[6].str() + "\n" + prefix + "currentTransform = " + string_VkSurfaceTransformFlagBitsKHR(pStruct->currentTransform) + "\n" + prefix + "supportedCompositeAlpha = " + ss[7].str() + "\n" + prefix + "supportedUsageFlags = " + ss[8].str() + "\n" + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vksurfaceformatkhr(const VkSurfaceFormatKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "format = " + string_VkFormat(pStruct->format) + "\n" + prefix + "colorSpace = " + string_VkColorSpaceKHR(pStruct->colorSpace) + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkswapchaincreateinfokhr(const VkSwapchainCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[11];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1064
    tmp_str = vk_print_vkextent2d(&pStruct->imageExtent, extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1071
    stp_strs[2] = " " + prefix + "imageExtent:" + "\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    stringstream index_ss;
    if (pStruct->imageSharingMode == VK_SHARING_MODE_CONCURRENT) {
        if (pStruct->pQueueFamilyIndices) {
            for (uint32_t i = 0; i < pStruct->queueFamilyIndexCount; i++) {
                index_ss.str("");
                index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
                ss[3] << "0x" << hex << nouppercase << pStruct->pQueueFamilyIndices[i] << dec;
                stp_strs[3] += " " + prefix + "pQueueFamilyIndices[" + index_ss.str() + "] = " + ss[3].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
                ss[3].str("");
            }
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->surface) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[3] << "0x" << hex << nouppercase << pStruct->minImageCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1088
    ss[4] << "0x" << hex << nouppercase << HandleCast(&pStruct->imageExtent) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->imageArrayLayers;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[6] << "0x" << hex << nouppercase << pStruct->imageUsage << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[7] << "0x" << hex << nouppercase << pStruct->queueFamilyIndexCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[8] << "0x" << hex << nouppercase << HandleCast(pStruct->pQueueFamilyIndices) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1099
    ss[9].str(pStruct->clipped ? "TRUE" : "FALSE");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[10] << "0x" << hex << nouppercase << HandleCast(pStruct->oldSwapchain) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "surface = " + ss[2].str() + "\n" + prefix + "minImageCount = " + ss[3].str() + "\n" + prefix + "imageFormat = " + string_VkFormat(pStruct->imageFormat) + "\n" + prefix + "imageColorSpace = " + string_VkColorSpaceKHR(pStruct->imageColorSpace) + "\n" + prefix + "imageExtent = " + ss[4].str() + "\n" + prefix + "imageArrayLayers = " + ss[5].str() + "\n" + prefix + "imageUsage = " + ss[6].str() + "\n" + prefix + "imageSharingMode = " + string_VkSharingMode(pStruct->imageSharingMode) + "\n" + prefix + "queueFamilyIndexCount = " + ss[7].str() + "\n" + prefix + "pQueueFamilyIndices = " + ss[8].str() + "\n" + prefix + "preTransform = " + string_VkSurfaceTransformFlagBitsKHR(pStruct->preTransform) + "\n" + prefix + "compositeAlpha = " + string_VkCompositeAlphaFlagBitsKHR(pStruct->compositeAlpha) + "\n" + prefix + "presentMode = " + string_VkPresentModeKHR(pStruct->presentMode) + "\n" + prefix + "clipped = " + ss[9].str() + "\n" + prefix + "oldSwapchain = " + ss[10].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkvertexinputattributedescription(const VkVertexInputAttributeDescription* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[3];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->location;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->binding;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[2] << "0x" << hex << nouppercase << pStruct->offset << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "location = " + ss[0].str() + "\n" + prefix + "binding = " + ss[1].str() + "\n" + prefix + "format = " + string_VkFormat(pStruct->format) + "\n" + prefix + "offset = " + ss[2].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkvertexinputbindingdescription(const VkVertexInputBindingDescription* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[2];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->binding;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->stride;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "binding = " + ss[0].str() + "\n" + prefix + "stride = " + ss[1].str() + "\n" + prefix + "inputRate = " + string_VkVertexInputRate(pStruct->inputRate) + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkviewport(const VkViewport* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[6];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[0] << pStruct->x;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[1] << pStruct->y;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->width;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->height;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[4] << pStruct->minDepth;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[5] << pStruct->maxDepth;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "x = " + ss[0].str() + "\n" + prefix + "y = " + ss[1].str() + "\n" + prefix + "width = " + ss[2].str() + "\n" + prefix + "height = " + ss[3].str() + "\n" + prefix + "minDepth = " + ss[4].str() + "\n" + prefix + "maxDepth = " + ss[5].str() + "\n";
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
std::string vk_print_vkwaylandsurfacecreateinfokhr(const VkWaylandSurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->display) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->surface) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "display = " + ss[2].str() + "\n" + prefix + "surface = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_WAYLAND_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkwin32keyedmutexacquirereleaseinfonv(const VkWin32KeyedMutexAcquireReleaseInfoNV* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[6];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[1] = "";
    stringstream index_ss;
    if (pStruct->pAcquireSyncs) {
        for (uint32_t i = 0; i < pStruct->acquireCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->pAcquireSyncs[i]) << dec;
            stp_strs[1] += " " + prefix + "pAcquireSyncs[" + index_ss.str() + "] = " + ss[1].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[1].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    if (pStruct->pAcquireKeys) {
        for (uint32_t i = 0; i < pStruct->acquireCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
            ss[2] << "0x" << hex << nouppercase << pStruct->pAcquireKeys[i] << dec;
            stp_strs[2] += " " + prefix + "pAcquireKeys[" + index_ss.str() + "] = " + ss[2].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[2].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    if (pStruct->pAcquireTimeoutMilliseconds) {
        for (uint32_t i = 0; i < pStruct->acquireCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
            ss[3] << "0x" << hex << nouppercase << pStruct->pAcquireTimeoutMilliseconds[i] << dec;
            stp_strs[3] += " " + prefix + "pAcquireTimeoutMilliseconds[" + index_ss.str() + "] = " + ss[3].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[3].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[4] = "";
    if (pStruct->pReleaseSyncs) {
        for (uint32_t i = 0; i < pStruct->releaseCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
            ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pReleaseSyncs[i]) << dec;
            stp_strs[4] += " " + prefix + "pReleaseSyncs[" + index_ss.str() + "] = " + ss[4].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[4].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[5] = "";
    if (pStruct->pReleaseKeys) {
        for (uint32_t i = 0; i < pStruct->releaseCount; i++) {
            index_ss.str("");
            index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1018
            ss[5] << "0x" << hex << nouppercase << pStruct->pReleaseKeys[i] << dec;
            stp_strs[5] += " " + prefix + "pReleaseKeys[" + index_ss.str() + "] = " + ss[5].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
            ss[5].str("");
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->acquireCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->pAcquireSyncs) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->pAcquireKeys) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pAcquireTimeoutMilliseconds) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[5] << "0x" << hex << nouppercase << pStruct->releaseCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[6] << "0x" << hex << nouppercase << HandleCast(pStruct->pReleaseSyncs) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->pReleaseKeys) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "acquireCount = " + ss[1].str() + "\n" + prefix + "pAcquireSyncs = " + ss[2].str() + "\n" + prefix + "pAcquireKeys = " + ss[3].str() + "\n" + prefix + "pAcquireTimeoutMilliseconds = " + ss[4].str() + "\n" + prefix + "releaseCount = " + ss[5].str() + "\n" + prefix + "pReleaseSyncs = " + ss[6].str() + "\n" + prefix + "pReleaseKeys = " + ss[7].str() + "\n" + stp_strs[5] + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_WIN32_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_WIN32_KHR
std::string vk_print_vkwin32surfacecreateinfokhr(const VkWin32SurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->hinstance) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[3] << "0x" << hex << nouppercase << HandleCast(pStruct->hwnd) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "hinstance = " + ss[2].str() + "\n" + prefix + "hwnd = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_WIN32_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
std::string vk_print_vkwritedescriptorset(const VkWriteDescriptorSet* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[8];
    string stp_strs[5];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[2] = "";
    stringstream index_ss;
    if ((pStruct->descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)                ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)          ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE))           {
        if (pStruct->pImageInfo) {
            for (uint32_t i = 0; i < pStruct->descriptorCount; i++) {
                index_ss.str("");
                index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
                tmp_str = vk_print_vkdescriptorimageinfo(&pStruct->pImageInfo[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
                stp_strs[2] += " " + prefix + "pImageInfo[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
                ss[2].str("");
            }
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[3] = "";
    if ((pStruct->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)         ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)         ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC))  {
        if (pStruct->pBufferInfo) {
            for (uint32_t i = 0; i < pStruct->descriptorCount; i++) {
                index_ss.str("");
                index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
                tmp_str = vk_print_vkdescriptorbufferinfo(&pStruct->pBufferInfo[i], extra_indent);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1011
                stp_strs[3] += " " + prefix + "pBufferInfo[" + index_ss.str() + "]:\n" + tmp_str;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
                ss[3].str("");
            }
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #961
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #963
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #968
    stp_strs[4] = "";
    if ((pStruct->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER) ||
        (pStruct->descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER))  {
        if (pStruct->pTexelBufferView) {
            for (uint32_t i = 0; i < pStruct->descriptorCount; i++) {
                index_ss.str("");
                index_ss << i;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1008
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1014
                ss[4] << "0x" << hex << nouppercase << HandleCast(pStruct->pTexelBufferView[i]) << dec;
                stp_strs[4] += " " + prefix + "pTexelBufferView[" + index_ss.str() + "] = " + ss[4].str() + "\n";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1030
                ss[4].str("");
            }
        }
    }
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1085
    ss[1] << "0x" << hex << nouppercase << HandleCast(pStruct->dstSet) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[2] << pStruct->dstBinding;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->dstArrayElement;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[4] << "0x" << hex << nouppercase << pStruct->descriptorCount << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[5] << "0x" << hex << nouppercase << HandleCast(pStruct->pImageInfo) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[6] << "0x" << hex << nouppercase << HandleCast(pStruct->pBufferInfo) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1096
    ss[7] << "0x" << hex << nouppercase << HandleCast(pStruct->pTexelBufferView) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "dstSet = " + ss[1].str() + "\n" + prefix + "dstBinding = " + ss[2].str() + "\n" + prefix + "dstArrayElement = " + ss[3].str() + "\n" + prefix + "descriptorCount = " + ss[4].str() + "\n" + prefix + "descriptorType = " + string_VkDescriptorType(pStruct->descriptorType) + "\n" + prefix + "pImageInfo = " + ss[5].str() + "\n" + prefix + "pBufferInfo = " + ss[6].str() + "\n" + prefix + "pTexelBufferView = " + ss[7].str() + "\n" + stp_strs[4] + stp_strs[3] + stp_strs[2] + stp_strs[1] + stp_strs[0];
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_XCB_KHR
std::string vk_print_vkxcbsurfacecreateinfokhr(const VkXcbSurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->connection) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->window;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "connection = " + ss[2].str() + "\n" + prefix + "window = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_XCB_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #936
#ifdef VK_USE_PLATFORM_XLIB_KHR
std::string vk_print_vkxlibsurfacecreateinfokhr(const VkXlibSurfaceCreateInfoKHR* pStruct, const std::string prefix)
{
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #942
    using namespace StreamControl;
    using namespace std;
    string final_str;
    string tmp_str;
    string extra_indent = "  " + prefix;
    stringstream ss[4];
    string stp_strs[1];
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1042
    if (pStruct->pNext) {
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1046
        tmp_str = dynamic_display((void*)pStruct->pNext, prefix);
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1056
        stp_strs[0] = " " + prefix + "pNext:\n" + tmp_str;
        ss[0].str("");
    }
    else
        stp_strs[0] = "";
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1105
    if (StreamControl::writeAddress)
        ss[0] << "0x" << hex << nouppercase << HandleCast(pStruct->pNext) << dec;
    else
        ss[0].str("address");
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1126: NB: Edit here to choose hex vs dec output by variable name
    ss[1] << "0x" << hex << nouppercase << pStruct->flags << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1120
    ss[2] << "0x" << hex << nouppercase << HandleCast(pStruct->dpy) << dec;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1129: NB Edit this section to choose hex vs dec output by variable name
    ss[3] << pStruct->window;
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1150
    final_str = prefix + "sType = " + string_VkStructureType(pStruct->sType) + "\n" + prefix + "pNext = " + ss[0].str() + "\n" + prefix + "flags = " + ss[1].str() + "\n" + prefix + "dpy = " + ss[2].str() + "\n" + prefix + "window = " + ss[3].str() + "\n" + stp_strs[0];
    return final_str;
}
#endif //VK_USE_PLATFORM_XLIB_KHR
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1158
std::string string_convert_helper(const void* toString, const std::string prefix)
{
    using namespace StreamControl;
    using namespace std;
    stringstream ss;
    ss << toString;
    string final_str = prefix + ss.str();
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1167
std::string string_convert_helper(const uint64_t toString, const std::string prefix)
{
    using namespace StreamControl;
    using namespace std;
    stringstream ss;
    ss << toString;
    string final_str = prefix + ss.str();
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1177
std::string string_convert_helper(VkSurfaceFormatKHR toString, const std::string prefix)
{
    using namespace std;
    string final_str = prefix + "format = " + string_VkFormat(toString.format) + "format = " + string_VkColorSpaceKHR(toString.colorSpace);
    return final_str;
}
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1184
std::string dynamic_display(const void* pStruct, const std::string prefix)
{
    using namespace std;
    // Cast to APP_INFO ptr initially just to pull sType off struct
    if (pStruct == NULL) {

        return string();
    }

    VkStructureType sType = ((VkApplicationInfo*)pStruct)->sType;
    string indent = "    ";
    indent += prefix;
    switch (sType)
    {
        case VK_STRUCTURE_TYPE_APPLICATION_INFO:
        {
            return vk_print_vkapplicationinfo((VkApplicationInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_BIND_SPARSE_INFO:
        {
            return vk_print_vkbindsparseinfo((VkBindSparseInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO:
        {
            return vk_print_vkbuffercreateinfo((VkBufferCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER:
        {
            return vk_print_vkbuffermemorybarrier((VkBufferMemoryBarrier*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO:
        {
            return vk_print_vkbufferviewcreateinfo((VkBufferViewCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO:
        {
            return vk_print_vkcommandbufferallocateinfo((VkCommandBufferAllocateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO:
        {
            return vk_print_vkcommandbufferbegininfo((VkCommandBufferBeginInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO:
        {
            return vk_print_vkcommandbufferinheritanceinfo((VkCommandBufferInheritanceInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO:
        {
            return vk_print_vkcommandpoolcreateinfo((VkCommandPoolCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO:
        {
            return vk_print_vkcomputepipelinecreateinfo((VkComputePipelineCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET:
        {
            return vk_print_vkcopydescriptorset((VkCopyDescriptorSet*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO:
        {
            return vk_print_vkdescriptorpoolcreateinfo((VkDescriptorPoolCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO:
        {
            return vk_print_vkdescriptorsetallocateinfo((VkDescriptorSetAllocateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO:
        {
            return vk_print_vkdescriptorsetlayoutcreateinfo((VkDescriptorSetLayoutCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO:
        {
            return vk_print_vkdevicecreateinfo((VkDeviceCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO:
        {
            return vk_print_vkdevicequeuecreateinfo((VkDeviceQueueCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_EVENT_CREATE_INFO:
        {
            return vk_print_vkeventcreateinfo((VkEventCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_FENCE_CREATE_INFO:
        {
            return vk_print_vkfencecreateinfo((VkFenceCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO:
        {
            return vk_print_vkframebuffercreateinfo((VkFramebufferCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO:
        {
            return vk_print_vkgraphicspipelinecreateinfo((VkGraphicsPipelineCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO:
        {
            return vk_print_vkimagecreateinfo((VkImageCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER:
        {
            return vk_print_vkimagememorybarrier((VkImageMemoryBarrier*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO:
        {
            return vk_print_vkimageviewcreateinfo((VkImageViewCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO:
        {
            return vk_print_vkinstancecreateinfo((VkInstanceCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE:
        {
            return vk_print_vkmappedmemoryrange((VkMappedMemoryRange*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO:
        {
            return vk_print_vkmemoryallocateinfo((VkMemoryAllocateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_MEMORY_BARRIER:
        {
            return vk_print_vkmemorybarrier((VkMemoryBarrier*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO:
        {
            return vk_print_vkpipelinecachecreateinfo((VkPipelineCacheCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinecolorblendstatecreateinfo((VkPipelineColorBlendStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinedepthstencilstatecreateinfo((VkPipelineDepthStencilStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinedynamicstatecreateinfo((VkPipelineDynamicStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelineinputassemblystatecreateinfo((VkPipelineInputAssemblyStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO:
        {
            return vk_print_vkpipelinelayoutcreateinfo((VkPipelineLayoutCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinemultisamplestatecreateinfo((VkPipelineMultisampleStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinerasterizationstatecreateinfo((VkPipelineRasterizationStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO:
        {
            return vk_print_vkpipelineshaderstagecreateinfo((VkPipelineShaderStageCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinetessellationstatecreateinfo((VkPipelineTessellationStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelinevertexinputstatecreateinfo((VkPipelineVertexInputStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO:
        {
            return vk_print_vkpipelineviewportstatecreateinfo((VkPipelineViewportStateCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO:
        {
            return vk_print_vkquerypoolcreateinfo((VkQueryPoolCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO:
        {
            return vk_print_vkrenderpassbegininfo((VkRenderPassBeginInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO:
        {
            return vk_print_vkrenderpasscreateinfo((VkRenderPassCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO:
        {
            return vk_print_vksamplercreateinfo((VkSamplerCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO:
        {
            return vk_print_vksemaphorecreateinfo((VkSemaphoreCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO:
        {
            return vk_print_vkshadermodulecreateinfo((VkShaderModuleCreateInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_SUBMIT_INFO:
        {
            return vk_print_vksubmitinfo((VkSubmitInfo*)pStruct, indent);
        }
        break;
        case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET:
        {
            return vk_print_vkwritedescriptorset((VkWriteDescriptorSet*)pStruct, indent);
        }
        break;
        default:
        return string();
// CODEGEN : file /home/lenny/Documents/VulkanTools/vk_helper_api_dump.py line #1210
    }
}