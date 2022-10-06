#!/usr/bin/python3 -i
#
# Copyright (c) 2020 The Khronos Group Inc.
# Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0


import os,re,sys,string
import xml.etree.ElementTree as etree
import generator as gen
from generator import *
from collections import namedtuple
from common_codegen import *

COMMON_CODEGEN_JSON_GEN = """
/*
** Copyright (c) 2020 The Khronos Group Inc.
** Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
**
** SPDX-License-Identifier: Apache-2.0
*/

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#include "vulkan/vk_layer.h"
#include "vulkan/vulkan.h"
#include "vk_layer_config.h"
#include "vk_layer_table.h"
#include "vk_layer_extension_utils.h"
#include "vk_layer_utils.h"
#include "vulkan_json_data.hpp"
#include "vulkan_json_layer.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string.h>
#include <string>
#include <type_traits>
#include <map>
#include <thread>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <utility>
//============================= API EntryPoints =============================//


// Specifically implemented functions

static int pipelineCacheCreateInfoCount;
static int pipelinePoolSizeCount;
static int commandBufferRequestCount;

@foreach function where('{funcName}' == 'vkCreateInstance')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    // Get the function pointer
    VkLayerInstanceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr != 0);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if(fpCreateInstance == NULL) {{
        return VK_ERROR_INITIALIZATION_FAILED;
    }}

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    {funcReturn} result = fpCreateInstance({funcNamedParams});
    if(result == VK_SUCCESS) {{
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
    }}
    {funcStateTrackingCode}
 
    return result;
}}
@end function

@foreach function where('{funcName}' == 'vkDestroyInstance')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    // Destroy the dispatch table
    dispatch_key key = get_dispatch_key({funcDispatchParam});
    instance_dispatch_table({funcDispatchParam})->DestroyInstance({funcNamedParams});
    destroy_instance_dispatch_table(key);
    {funcStateTrackingCode}
}}
@end function

@foreach function where('{funcName}' == 'vkCreateDevice')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    // Get the function pointer
    VkLayerDeviceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(NULL, "vkCreateDevice");
    if(fpCreateDevice == NULL) {{
        return VK_ERROR_INITIALIZATION_FAILED;
    }}

    {funcStateTrackingCode}

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    {funcReturn} result = fpCreateDevice(physicalDevice, &devCreateInfoCopy, pAllocator, pDevice);
    if(result == VK_SUCCESS) {{
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }}

    return result;
}}
@end function

@foreach function where('{funcName}' == 'vkDestroyDevice')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    // Destroy the dispatch table
    dispatch_key key = get_dispatch_key({funcDispatchParam});
    device_dispatch_table({funcDispatchParam})->DestroyDevice({funcNamedParams});
    destroy_device_dispatch_table(key);
    {funcStateTrackingCode}
}}
@end function

@foreach function where('{funcName}' == 'vkEnumerateInstanceExtensionProperties')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    return util_GetExtensionProperties(0, NULL, pPropertyCount, pProperties);
}}
@end function

@foreach function where('{funcName}' == 'vkEnumerateInstanceLayerProperties')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    static const VkLayerProperties layerProperties[] = {{
        {{
            "VK_LAYER_KHRONOS_json_gen",
            VK_MAKE_VERSION(1, 2, VK_HEADER_VERSION), // specVersion
            VK_MAKE_VERSION(0, 2, 0), // implementationVersion
            "layer: json_gen",
        }}
    }};

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}}
@end function

@foreach function where('{funcName}' == 'vkEnumerateDeviceLayerProperties')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    static const VkLayerProperties layerProperties[] = {{
        {{
            "VK_LAYER_KHRONOS_json_gen",
            VK_MAKE_VERSION(1, 2, VK_HEADER_VERSION),
            VK_MAKE_VERSION(0, 2, 0),
            "layer: json_gen",
        }}
    }};

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}}
@end function

@foreach function where('{funcName}' == 'vkQueuePresentKHR')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}

    return result;
}}
@end function

// Autogen instance functions

@foreach function where('{funcDispatchType}' == 'instance' and '{funcReturn}' != 'void' and '{funcName}' not in ['vkCreateInstance', 'vkDestroyInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    {funcReturn} result = instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    return result;
}}
@end function

@foreach function where('{funcDispatchType}' == 'instance' and '{funcReturn}' == 'void' and '{funcName}' not in ['vkCreateInstance', 'vkDestroyInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
  
}}
@end function

// Autogen device functions

@foreach function where('{funcDispatchType}' == 'device' and '{funcReturn}' != 'void' and '{funcName}' not in ['vkDestroyDevice', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkQueuePresentKHR', 'vkGetDeviceProcAddr', 'vkGetPipelinePropertiesEXT'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    
    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    
    return result;
}}
@end function

@foreach function where('{funcDispatchType}' == 'device' and '{funcReturn}' == 'void' and '{funcName}' not in ['vkDestroyDevice', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkGetDeviceProcAddr'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    
    device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    
}}
@end function

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{{
    @foreach function where('{funcType}' == 'instance'  and '{funcName}' not in [ 'vkEnumerateDeviceExtensionProperties' ])
    if(strcmp(pName, "{funcName}") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>({funcName});
    @end function

    if(instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return NULL;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName)
{{
    @foreach function where('{funcType}' == 'device')
    if(strcmp(pName, "{funcName}") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>({funcName});
    @end function

    if(device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return NULL;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}}
"""

POINTER_TYPES = ['void', 'xcb_connection_t', 'Display', 'SECURITY_ATTRIBUTES', 'ANativeWindow', 'AHardwareBuffer']

JSON_INTERCEPT_API = {
    'vkCreateInstance':
        'if (pCreateInfo != nullptr && pCreateInfo->pApplicationInfo != nullptr) {\n'
        '\t\tuint32_t major = VK_API_VERSION_MAJOR(pCreateInfo->pApplicationInfo->apiVersion);\n'
        '\t\tuint32_t minor = VK_API_VERSION_MINOR(pCreateInfo->pApplicationInfo->apiVersion);\n'
        '\t\t// Need extension version for private data slot feature.\n'
        '\t\tif (major <= 1 && minor < 3) {\n'
            '\t\t\tvk_json::s_pipe.m_usePrivateDataExtension = true;\n'
        '\t\t}\n'
        '\t} else { \n'
        '\t\t // If pcreateInfo->pApplicationInfo is nullptr, then it is assumed to be VK1.0\n'
        '\t\t vk_json::s_pipe.m_usePrivateDataExtension = true;\n'
        '\t}\n'
    ,

    'vkCreateShaderModule':
        'vk_json::s_pipe.setShaderModuleInfo(pCreateInfo, pShaderModule);'
    ,
    'vkCreateGraphicsPipelines':
        'vk_json::s_pipe.objResInfo.graphicsPipelineRequestCount += createInfoCount;\n'
		'\tvk_json::s_pipe.dumpGraphicsPipeline(device, createInfoCount, pCreateInfos, pPipelines);'
    ,
    'vkCreateComputePipelines':
        'vk_json::s_pipe.objResInfo.computePipelineRequestCount += createInfoCount;\n'
		'\tvk_json::s_pipe.dumpComputePipeline(device, createInfoCount, pCreateInfos, pPipelines);'
    ,
    'vkCreateDescriptorSetLayout':
        'vk_json::s_pipe.objResInfo.descriptorSetLayoutRequestCount++;\n'
        '\tvk_json::s_pipe.objResInfo.descriptorSetLayoutBindingRequestCount += pCreateInfo->bindingCount;\n'
        '\tvk_json::s_pipe.addLayout(device, *pSetLayout, pCreateInfo);'
    ,
    'vkCreatePipelineLayout':
        'vk_json::s_pipe.objResInfo.pipelineLayoutRequestCount++;\n'
        '\tvk_json::s_pipe.setPipelineLayout(device, pCreateInfo, pPipelineLayout);'
    ,
    'vkCreateRenderPass':
        'vk_json::s_pipe.objResInfo.renderPassRequestCount++;\n'
        '\tvk_json::s_pipe.setRenderPass(device, pCreateInfo, pRenderPass);'
    ,
    'vkCreateRenderPass2':
        'vk_json::s_pipe.setRenderPass2(device, pCreateInfo, pRenderPass);'
    ,
    'vkGetPhysicalDeviceFeatures2KHR':
        'set_layer_supported_features(pFeatures);\n'
        '\tvk_json::s_pipe.setPhysicalDeviceFeatures2(pFeatures);'
    ,
    'vkGetPhysicalDeviceFeatures2':
        'set_layer_supported_features(pFeatures);\n'
        '\tvk_json::s_pipe.setPhysicalDeviceFeatures2(pFeatures);'
    ,
    'vkCreateSampler':
        'vk_json::s_pipe.objResInfo.samplerRequestCount++;\n'
        '\tvk_json::s_pipe.setSamplerInfo(pCreateInfo, pSampler);'
    ,
    'vkCreateDevice':
        '\t// We have seen what the app has requested in vkCreateInstance, and we have assumed that privateData feature is supported.\n'
        '\t// Now, let us actually validate if that is supported.\n'
        '\tbool isPrivateDataExtSupported{ false };\n'

        '\tif (vk_json::s_pipe.m_usePrivateDataExtension == true) {\n'
        '\t   uint32_t count;\n'
        '\t   instance_dispatch_table(physicalDevice)->EnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr);\n'
        '\t   std::vector<VkExtensionProperties> extensions(count);\n'
        '\t   instance_dispatch_table(physicalDevice)->EnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, extensions.data());\n'
        '\t   for (auto& extension : extensions) {\n'
        '\t      if (strncmp(extension.extensionName, "VK_EXT_private_data", strlen("VK_EXT_private_data")) == 0) {\n'
        '\t         isPrivateDataExtSupported = true;\n'
        '\t         break;\n'
        '\t      }\n'
        '\t   }\n'
        '\t}\n'

        '\t// If neither VK 1.3 or VK_EXT_private_data are supported, then we cannot use this layer.\n'
        '\tif (vk_json::s_pipe.m_usePrivateDataExtension == true && isPrivateDataExtSupported == false) {\n'
        '\t    return VK_ERROR_INITIALIZATION_FAILED;\n'
        '\t}\n'

        '\t// Keep a copy of it since we modify this struct to enable some features if they aren\'t present.\n'
        '\tVkDeviceCreateInfo devCreateInfoCopy = *pCreateInfo;\n'
        '\tstd::vector<const char*> extList;\n'
        '\tif (vk_json::s_pipe.m_usePrivateDataExtension) {\n'
        '\t   bool isPrivDataExtSet{false};\n'
        '\t   for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {\n'
        '\t      const char* ptr = const_cast<const char*>(*pCreateInfo->ppEnabledExtensionNames);\n'
        '\t      extList.push_back(ptr);\n'
        '\t      if (strncmp(ptr, "VK_EXT_private_data", strlen("VK_EXT_private_data")) == 0) {\n'
        '\t         isPrivDataExtSet = true;\n'
        '\t      }\n'
        '\t      ptr++;\n'
        '\t   }\n'
        '\t   if (isPrivDataExtSet == false) {\n'
        '\t      extList.push_back("VK_EXT_private_data");\n'
        '\t   }\n'
        '\t   devCreateInfoCopy.enabledExtensionCount = static_cast<uint32_t>(extList.size());\n'
        '\t   devCreateInfoCopy.ppEnabledExtensionNames = extList.data();\n'
        '\t}\n'
        '\tbool isFeatureSet{false};\n'
        '\t// The feature struct needs to be set even when the extension is used.\n'
        '\tVkBaseOutStructure* pnext = (VkBaseOutStructure*)devCreateInfoCopy.pNext;\n'
        '\twhile (pnext != nullptr) {\n'
        '\t   if (pnext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES) {\n'
        '\t      VkPhysicalDeviceVulkan13Features* f = (VkPhysicalDeviceVulkan13Features*)pnext;\n'
        '\t      f->privateData = VK_TRUE;\n'
        '\t      isFeatureSet = true;\n'
        '\t      break;\n'
        '\t   }\n'
        '\t   if (pnext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES) {\n'
        '\t      VkPhysicalDevicePrivateDataFeatures* f = (VkPhysicalDevicePrivateDataFeatures*)pnext;\n'
        '\t      f->privateData = VK_TRUE;\n'
        '\t      isFeatureSet = true;\n'
        '\t      break;\n'
        '\t   }\n'
        '\t   pnext = pnext->pNext;\n'
        '\t} \n'
        '\tVkPhysicalDevicePrivateDataFeatures privDataFeats{};\n'
        '\tprivDataFeats.privateData = VK_TRUE;\n'
        '\tprivDataFeats.pNext = (void*)devCreateInfoCopy.pNext;\n'
        '\tif (isFeatureSet == false) {\n'
        '\t    devCreateInfoCopy.pNext = &privDataFeats;\n'
        '\t}\n'
        '\tvk_json::s_pipe.setDevice(pCreateInfo);\n'
    ,
    'vkAllocateCommandBuffers':
        'if (pAllocateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.commandBufferRequestCount += pAllocateInfo->commandBufferCount;\n'
        '\t}\n'
    ,
    'vkCreateFence':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.fenceRequestCount++;\n'
        '\t}\n'
    ,    
    'vkCreateBuffer':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.bufferRequestCount++;\n'
        '\t}\n'
    ,
    'vkCreateBufferView':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.bufferViewRequestCount++;\n'
        '\t}\n'
    ,    
    'vkCreateImage':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.imageRequestCount++;\n'
        '\t}\n'
    ,
    'vkCreateImageView':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.imageViewRequestCount++;\n'
        '\t\tif (pCreateInfo->subresourceRange.levelCount > vk_json::s_pipe.objResInfo.maxImageViewMipLevels) { \n'
        '\t\t\tvk_json::s_pipe.objResInfo.maxImageViewMipLevels = pCreateInfo->subresourceRange.levelCount;\n'
        '\t\t}\n'
        '\t\tif (pCreateInfo->subresourceRange.layerCount > vk_json::s_pipe.objResInfo.layeredImageViewRequestCount) { \n'
        '\t\t\tvk_json::s_pipe.objResInfo.layeredImageViewRequestCount = pCreateInfo->subresourceRange.layerCount;\n'
        '\t\t}\n'
        '\t\tif (pCreateInfo->subresourceRange.layerCount > vk_json::s_pipe.objResInfo.layeredImageViewRequestCount) { \n'
        '\t\t\tvk_json::s_pipe.objResInfo.layeredImageViewRequestCount = pCreateInfo->subresourceRange.layerCount;\n'
        '\t\t}\n'
        '\t}\n'
    ,
    'vkCreateEvent':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.eventRequestCount++;\n'
        '\t}\n'
    ,
    'vkCreateQueryPool':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.queryPoolRequestCount++;\n'
        '\t}\n'
    ,  
    'vkCreateDescriptorPool':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.descriptorPoolRequestCount++;\n'
        '\t}\n'
    ,  
    'vkAllocateDescriptorSets':
        'if (pAllocateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.descriptorSetRequestCount += pAllocateInfo->descriptorSetCount;\n'
        '\t}\n'
    ,
    'vkCreateFramebuffer':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.framebufferRequestCount++;\n'
        '\t}\n'
    ,
    'vkCreateCommandPool':
        'if (pCreateInfo) {\n'
        '\t\tvk_json::s_pipe.objResInfo.commandPoolRequestCount++;\n'
        '\t}\n'
    ,
    
    
}

class JSONGenGeneratorOptions(GeneratorOptions):
    def __init__(self,
                 conventions = None,
                 input = None,
                 filename = None,
                 directory = '.',
                 genpath = None,
                 apiname = None,
                 profile = None,
                 versions = '.*',
                 emitversions = '.*',
                 defaultExtensions = None,
                 addExtensions = None,
                 removeExtensions = None,
                 emitExtensions = None,
                 sortProcedure = None,
                 prefixText = "",
                 genFuncPointers = True,
                 protectFile = True,
                 protectFeature = True,
                 protectProto = None,
                 protectProtoStr = None,
                 apicall = '',
                 apientry = '',
                 apientryp = '',
                 indentFuncProto = True,
                 indentFuncPointer = False,
                 alignFuncParam = 0,
                 expandEnumerants = True,
                 ):
        GeneratorOptions.__init__(self,
                 conventions = conventions,
                 filename = filename,
                 directory = directory,
                 genpath = genpath,
                 apiname = apiname,
                 profile = profile,
                 versions = versions,
                 emitversions = emitversions,
                 defaultExtensions = defaultExtensions,
                 addExtensions = addExtensions,
                 removeExtensions = removeExtensions,
                 emitExtensions = emitExtensions,
                 sortProcedure = sortProcedure)
        self.input           = input
        self.prefixText      = prefixText
        self.genFuncPointers = genFuncPointers
        self.protectFile     = protectFile
        self.protectFeature  = protectFeature
        self.protectProto    = protectProto
        self.protectProtoStr = protectProtoStr
        self.apicall         = apicall
        self.apientry        = apientry
        self.apientryp       = apientryp
        self.indentFuncProto = indentFuncProto
        self.indentFuncPointer = indentFuncPointer
        self.alignFuncParam  = alignFuncParam


class JSONGenOutputGenerator(OutputGenerator):

    def __init__(self,
                 errFile = sys.stderr,
                 warnFile = sys.stderr,
                 diagFile = sys.stdout,
                 registryFile = None):
        gen.OutputGenerator.__init__(self, errFile, warnFile, diagFile)
        self.format = None

        self.constants = {}
        self.extensions = set()
        self.extFuncs = {}
        self.extTypes = {}
        self.includes = {}

        self.basetypes = set()
        self.bitmasks = set()
        self.enums = set()
        self.externalTypes = set()
        self.flags = set()
        self.funcPointers = set()
        self.functions = set()
        self.handles = set()
        self.structs = set()
        self.unions = set()
        self.aliases = {}

        self.registryFile = registryFile

        # Used to track duplications (thanks 1.1 spec)
        self.trackedTypes = []

    def beginFile(self, genOpts):
        gen.OutputGenerator.beginFile(self, genOpts)
        self.format = genOpts.input

        if self.registryFile is not None:
            root = xml.etree.ElementTree.parse(self.registryFile)
        else:
            root = self.registry.reg

        for node in root.find('extensions').findall('extension'):
            ext = VulkanExtension(node)
            self.extensions.add(ext)
            for item in ext.vktypes:
                self.extTypes[item] = ext
            for item in ext.vkfuncs:
                self.extFuncs[item] = ext

        for node in self.registry.reg.findall('enums'):
            if node.get('name') == 'API Constants':
                for item in node.findall('enum'):
                    self.constants[item.get('name')] = item.get('value')

        for node in self.registry.reg.find('types').findall('type'):
            if node.get('category') == 'include':
                self.includes[node.get('name')] = ''.join(node.itertext())


    def endFile(self):
        # Find all of the extensions that use the system types
        self.sysTypes = set()
        for node in self.registry.reg.find('types').findall('type'):
            if node.get('category') is None and node.get('requires') in self.includes and node.get('requires') != 'vk_platform':
                for extension in self.extTypes:
                    for structName in self.extTypes[extension].vktypes:
                        for struct in self.structs:
                            if struct.name == structName:
                                for member in struct.members:
                                    if node.get('name') == member.baseType or node.get('name') + '*' == member.baseType:
                                        sysType = VulkanSystemType(node.get('name'), self.extTypes[structName])
                                        if sysType not in self.sysTypes:
                                            self.sysTypes.add(sysType)
                    for funcName in self.extTypes[extension].vkfuncs:
                        for func in self.functions:
                            if func.name == funcName:
                                for param in func.parameters:
                                    if node.get('name') == param.baseType or node.get('name') + '*' == param.baseType:
                                        sysType = VulkanSystemType(node.get('name'), self.extFuncs[funcName])
                                        if sysType not in self.sysTypes:
                                            self.sysTypes.add(sysType)

        # Find every @foreach, @if, and @end
        forIter = re.finditer('(^\\s*\\@foreach\\s+[a-z]+(\\s+where\\(.*\\))?\\s*^)|(\\@foreach [a-z]+(\\s+where\\(.*\\))?\\b)', self.format, flags=re.MULTILINE)
        ifIter = re.finditer('(^\\s*\\@if\\(.*\\)\\s*^)|(\\@if\\(.*\\))', self.format, flags=re.MULTILINE)
        endIter = re.finditer('(^\\s*\\@end\\s+[a-z]+\\s*^)|(\\@end [a-z]+\\b)', self.format, flags=re.MULTILINE)
        try:
            nextFor = next(forIter)
        except StopIteration:
            nextFor = None
        try:
            nextIf = next(ifIter)
        except StopIteration:
            nextIf = None
        try:
            nextEnd = next(endIter)
        except StopIteration:
            nextEnd = None

        # Match the beginnings to the ends
        loops = []
        unassignedControls = []
        depth = 0
        while nextFor is not None or nextFor is not None or nextEnd is not None:
            # If this is a @foreach
            if nextFor is not None and ((nextIf is None or nextFor.start() < nextIf.start()) and nextFor.start() < nextEnd.start()):
                depth += 1
                forType = re.search('(?<=\\s)[a-z]+', self.format[nextFor.start():nextFor.end()])
                text = self.format[forType.start()+nextFor.start():forType.end()+nextFor.start()]
                whereMatch = re.search('(?<=where\\().*(?=\\))', self.format[nextFor.start():nextFor.end()])
                condition = None if whereMatch is None else self.format[whereMatch.start()+nextFor.start():whereMatch.end()+nextFor.start()]
                unassignedControls.append((nextFor.start(), nextFor.end(), text, condition))

                try:
                    nextFor = next(forIter)
                except StopIteration:
                    nextFor = None

            # If this is an @if
            elif nextIf is not None and nextIf.start() < nextEnd.start():
                depth += 1
                condMatch = re.search('(?<=if\\().*(?=\\))', self.format[nextIf.start():nextIf.end()])
                condition = None if condMatch is None else self.format[condMatch.start()+nextIf.start():condMatch.end()+nextIf.start()]
                unassignedControls.append((nextIf.start(), nextIf.end(), 'if', condition))

                try:
                    nextIf = next(ifIter)
                except StopIteration:
                    nextIf = None

            # Else this is an @end
            else:
                depth -= 1
                endType = re.search('(?<=\\s)[a-z]+', self.format[nextEnd.start():nextEnd.end()])
                text = self.format[endType.start()+nextEnd.start():endType.end()+nextEnd.start()]

                start = unassignedControls.pop(-1)
                assert(start[2] == text)

                item = Control(self.format, start[0:2], (nextEnd.start(), nextEnd.end()), text, start[3])
                if len(loops) < 1 or depth < loops[-1][0]:
                    while len(loops) > 0 and depth < loops[-1][0]:
                        item.children.insert(0, loops.pop(-1)[1])
                    loops.append((depth, item))
                else:
                    loops.append((depth, item))

                try:
                    nextEnd = next(endIter)
                except StopIteration:
                    nextEnd = None

        # Expand each loop into its full form
        lastIndex = 0
        for _, loop in loops:
            gen.write(self.format[lastIndex:loop.startPos[0]].format(**{}), file=self.outFile)
            gen.write(self.expand(loop), file=self.outFile)
            lastIndex = loop.endPos[1]
        gen.write(self.format[lastIndex:-1].format(**{}), file=self.outFile)

        gen.OutputGenerator.endFile(self)

    def genCmd(self, cmd, name, alias):
        gen.OutputGenerator.genCmd(self, cmd, name, alias)

        if name == "vkEnumerateInstanceVersion": return # TODO: Create exclusion list or metadata to indicate this

        self.functions.add(VulkanFunction(cmd.elem, self.constants, self.aliases, self.extFuncs))

    # These are actually constants
    def genEnum(self, enuminfo, name, alias):
        gen.OutputGenerator.genEnum(self, enuminfo, name, alias)

    # These are actually enums
    def genGroup(self, groupinfo, groupName, alias):
        gen.OutputGenerator.genGroup(self, groupinfo, groupName, alias)

        if alias is not None:
            trackedName = alias
        else:
            trackedName = groupName
        if trackedName in self.trackedTypes:
            return
        self.trackedTypes.append(trackedName)

        if groupinfo.elem.get('type') == 'bitmask':
            self.bitmasks.add(VulkanBitmask(groupinfo.elem, self.extensions))
        elif groupinfo.elem.get('type') == 'enum':
            self.enums.add(VulkanEnum(groupinfo.elem, self.extensions))

    def genType(self, typeinfo, name, alias):
        gen.OutputGenerator.genType(self, typeinfo, name, alias)

        if alias is not None:
            trackedName = alias
            if typeinfo.elem.get('category') == 'struct':
                self.aliases[name] = alias
        else:
            trackedName = name
        if trackedName in self.trackedTypes:
            return
        self.trackedTypes.append(trackedName)

        if typeinfo.elem.get('category') == 'struct':
            self.structs.add(VulkanStruct(typeinfo.elem, self.constants, self.enums))
        elif typeinfo.elem.get('category') == 'basetype':
            self.basetypes.add(VulkanBasetype(typeinfo.elem))
        elif typeinfo.elem.get('category') is None and typeinfo.elem.get('requires') == 'vk_platform':
            self.externalTypes.add(VulkanExternalType(typeinfo.elem))
        elif typeinfo.elem.get('category') == 'handle':
            self.handles.add(VulkanHandle(typeinfo.elem))
        elif typeinfo.elem.get('category') == 'union':
            self.unions.add(VulkanUnion(typeinfo.elem, self.constants))
        elif typeinfo.elem.get('category') == 'bitmask':
            self.flags.add(VulkanFlags(typeinfo.elem))
        elif typeinfo.elem.get('category') == 'funcpointer':
            self.funcPointers.add(VulkanFunctionPointer(typeinfo.elem))

    def expand(self, loop, parents=[]):
        # Figure out what we're dealing with
        if loop.text == 'if':
            subjects = [ Control.IfDummy() ]
        elif loop.text == 'basetype':
            subjects = self.basetypes
        elif loop.text == 'bitmask':
            subjects = self.bitmasks
        elif loop.text == 'choice':
            subjects = self.findByType([VulkanUnion], parents).choices
        elif loop.text == 'enum':
            subjects = self.enums
        elif loop.text == 'extension':
            subjects = self.extensions
        elif loop.text == 'flag':
            subjects = self.flags
        elif loop.text == 'funcpointer':
            subjects = self.funcPointers
        elif loop.text == 'function':
            subjects = self.functions
        elif loop.text == 'handle':
            subjects = self.handles
        elif loop.text == 'option':
            subjects = self.findByType([VulkanEnum, VulkanBitmask], parents).options
        elif loop.text == 'member':
            subjects = self.findByType([VulkanStruct], parents).members
        elif loop.text == 'parameter':
            subjects = self.findByType([VulkanFunction], parents).parameters
        elif loop.text == 'struct':
            subjects = self.structs
        elif loop.text == 'systype':
            subjects = self.sysTypes
        elif loop.text == 'type':
            subjects = self.externalTypes
        elif loop.text == 'union':
            subjects = self.unions
        else:
            assert(False)

        # Generate the output string
        out = ''
        for item in subjects:

            # Merge the values and the parent values
            values = item.values().copy()
            for parent in parents:
                values.update(parent.values())

            # Check if the condition is met
            if loop.condition is not None:
                cond = eval(loop.condition.format(**values))
                assert(cond == True or cond == False)
                if not cond:
                    continue

            # Check if an ifdef is needed
            if item.name in self.extFuncs:
                ext = self.extFuncs[item.name]
            elif item.name in self.extTypes:
                ext = self.extTypes[item.name]
            elif item in self.sysTypes:
                ext = item.ext
            else:
                ext = None
            if ext is not None and ext.guard is not None:
                out += '#if defined({})\n'.format(ext.guard)

            # Format the string
            lastIndex = loop.startPos[1]
            for child in loop.children:
                out += loop.fullString[lastIndex:child.startPos[0]].format(**values)
                out += self.expand(child, parents=[item]+parents)
                lastIndex = child.endPos[1]
            out += loop.fullString[lastIndex:loop.endPos[0]].format(**values)

            # Close the ifdef
            if ext is not None and ext.guard is not None:
                out += '#endif // {}\n'.format(ext.guard)

        return out

    def findByType(self, types, objects):
        value = None
        for item in objects:
            for ty in types:
                if isinstance(item, ty):
                    value = item
                    break
        assert(value is not None)
        return value

class Control:

    class IfDummy:

        def __init__(self):
            self.name = 'ifdummy'

        def values(self):
            return {}

    def __init__(self, fullString, start, end, text, condition):
        self.fullString = fullString
        self.startPos = start
        self.endPos = end
        self.text = text
        self.condition = condition
        self.children = []

# Base class for VulkanStruct.Member and VulkanStruct.Parameter
class VulkanVariable:

    def __init__(self, rootNode, constants, aliases, parentName):
        # Set basic properties
        self.name = rootNode.find('name').text      # Variable name
        self.typeID = rootNode.find('type').text    # Typename, dereferenced and converted to a useable C++ token
        if aliases is not None and self.typeID in aliases:
            self.typeID = aliases[self.typeID]
        self.baseType = self.typeID                 # Type, dereferenced to the non-pointer type
        self.childType = None                       # Type, dereferenced to the non-pointer type (None if it isn't a pointer)
        self.arrayLength = None                     # Length of the array, or None if it isn't an array

        # Get the text of the variable type and name, but not the comment
        self.text = ''
        for node in rootNode.itertext():
            comment = rootNode.find('comment')
            if comment is not None and comment.text == node:
                continue
            self.text += node

        typeMatch = re.search('.+?(?=' + self.name + ')', self.text)
        self.type = typeMatch.string[typeMatch.start():typeMatch.end()]
        self.type = ' '.join(self.type.split())
        bracketMatch = re.search('(?<=\\[)[a-zA-Z0-9_]+(?=\\])', self.text)
        if bracketMatch is not None:
            matchText = bracketMatch.string[bracketMatch.start():bracketMatch.end()]
            self.childType = self.type
            self.type += '[' + matchText + ']'
            if matchText in constants:
                self.arrayLength = constants[matchText]
            else:
                self.arrayLength = matchText

        self.lengthMember = False
        lengthString = rootNode.get('len')
        lengths = []
        if lengthString is not None:
            lengths = re.split(',', lengthString)
            lengths = list(filter(('null-terminated').__ne__, lengths))
        #assert(len(lengths) <= 1)
        if self.arrayLength is None and len(lengths) > 0:
            self.childType = '*'.join(self.type.split('*')[0:-1])
            self.arrayLength = lengths[0]
            self.lengthMember = True
        if self.arrayLength is not None and self.arrayLength.startswith('latexmath'):
            code = self.arrayLength[10:len(self.arrayLength)]
            code = re.sub('\\[', '', code)
            code = re.sub('\\]', '', code)
            code = re.sub('\\\\(lceil|rceil)', '', code)
            code = re.sub('{|}', '', code)
            code = re.sub('\\\\mathit', '', code)
            code = re.sub('\\\\over', '/', code)
            code = re.sub('\\\\textrm', '', code)
            self.arrayLength = code

        # Dereference if necessary and handle members of variables
        if self.arrayLength is not None:
            self.arrayLength = re.sub('::', '->', self.arrayLength)
            sections = self.arrayLength.split('->')
            if sections[-1][0] == 'p' and sections[0][1].isupper():
                self.arrayLength = '*' + self.arrayLength

        self.pointerLevels = len(re.findall('\\*|\\[', self.text))
        if self.typeID == 'char' and self.pointerLevels > 0:
            self.baseType += '*'
            self.pointerLevels -= 1
            self.typeID = 'cstring'
        elif self.typeID in POINTER_TYPES:
            self.baseType += '*'
            self.pointerLevels -= 1
        assert(self.pointerLevels >= 0)

        self.inheritedConditions = ''

class VulkanBasetype:

    def __init__(self, rootNode):
        self.name = rootNode.get('name')
        self.type = rootNode.get('type')

    def values(self):
        return {
            'baseName': self.name,
            'baseType': self.type,
        }

class VulkanBitmask:

    def __init__(self, rootNode, extensions):
        self.name = rootNode.get('name')
        self.type = rootNode.get('type')

        # Read each value that the enum contains
        self.options = []
        for child in rootNode:
            childName = child.get('name')
            childValue = child.get('value')
            childBitpos = child.get('bitpos')
            childComment = child.get('comment')
            if childName is None or (childValue is None and childBitpos is None):
                continue

            self.options.append(VulkanEnum.Option(childName, childValue, childBitpos, childComment))

        for ext in extensions:
            if self.name in ext.enumValues:
                childName, childValue = ext.enumValues[self.name]
                self.options.append(VulkanEnum.Option(childName, childValue, None, None))

    def values(self):
        return {
            'bitName': self.name,
            'bitType': self.type,
        }

def isPow2(num):
    return num != 0 and ((num & (num - 1)) == 0)

def StrToInt(s):
    try:
        return int(s)
    except ValueError:
        return int(s,16)

class VulkanEnum:

    class Option:

        def __init__(self, name, value, bitpos, comment):
            self.name = name
            self.comment = comment
            self.multiValue = None
            
            if value is not None:         

                self.multiValue = not isPow2(StrToInt(value))

            if value == 0 or value is None:
                value = 1 << int(bitpos)

            self.value = value

        def values(self):
            return {
                'optName': self.name,
                'optValue': self.value,
                'optComment': self.comment,
                'optMultiValue': self.multiValue,
            }

    def __init__(self, rootNode, extensions):
        self.name = rootNode.get('name')
        self.type = rootNode.get('type')

        # Read each value that the enum contains
        self.options = []
        for child in rootNode:
            childName = child.get('name')
            childValue = child.get('value')
            childBitpos = child.get('bitpos')
            childComment = child.get('comment')
            childExtends = child.get('extends')
            childOffset = child.get('offset')
            childExtNum = child.get('extnumber')

            if childName is None:
                continue
            if (childValue is None and childBitpos is None and childOffset is None):
                continue

            if childExtends is not None and childExtNum is not None and childOffset is not None:
                enumNegative = False
                extNum = int(childExtNum)
                extOffset = int(childOffset)
                extBase      = 1000000000
                extBlockSize = 1000
                childValue = extBase + (extNum - 1) * extBlockSize + extOffset
                if ('dir' in child.keys()):
                    childValue = -childValue
               
            # Check for duplicates
            duplicate = False
            for o in self.options:
                if o.values()['optName'] == childName:
                    duplicate = True
                    break
            if duplicate:
                continue

            self.options.append(VulkanEnum.Option(childName, childValue, childBitpos, childComment))

        for ext in extensions:
            if self.name in ext.enumValues:
                childName, childValue = ext.enumValues[self.name]
                duplicate = False
                # Check for duplicates
                for o in self.options:
                    if o.values()['optName'] == childName:
                        duplicate = True
                        break
                if duplicate:
                    continue
                self.options.append(VulkanEnum.Option(childName, childValue, None, None))

    def values(self):
        return {
            'enumName': self.name,
            'enumType': self.type,
        }

class VulkanExtension:

    def __init__(self, rootNode):
        self.name = rootNode.get('name')
        self.number = int(rootNode.get('number'))
        self.type = rootNode.get('type')
        self.dependency = rootNode.get('requires')
        self.guard = GetFeatureProtect(rootNode)
        self.supported = rootNode.get('supported')

        self.vktypes = []
        self.vkfuncs = []
        self.constants = {}
        self.enumValues = {}

        for req in rootNode.findall('require'):
            for ty in req.findall('type'):
                self.vktypes.append(ty.get('name'))

            for func in req.findall('command'):
                self.vkfuncs.append(func.get('name'))

            for enum in req.findall('enum'):
                base = enum.get('extends')
                name = enum.get('name')
                value = enum.get('value')
                bitpos = enum.get('bitpos')
                offset = enum.get('offset')

                if value is None and bitpos is not None:
                    value = 1 << int(bitpos)

                if offset is not None:
                    offset = int(offset)
                if base is not None and offset is not None:
                    enumValue = 1000000000 + 1000*(self.number - 1) + offset
                    if enum.get('dir') == '-':
                        enumValue = -enumValue
                    self.enumValues[base] = (name, enumValue)
                else:
                    self.constants[name] = value

    def values(self):
        return {
            'extName': self.name,
            'extNumber': self.number,
            'extType': self.type,
            'extDependency': self.dependency,
            'extGuard': self.guard,
            'extSupported': self.supported,
        }

class VulkanExternalType:

    def __init__(self, rootNode):
        self.name = rootNode.get('name')
        self.dependency = rootNode.get('requires')

    def values(self):
        return {
            'etyName': self.name,
            'etyDependency': self.dependency,
        }

class VulkanFlags:

    def __init__(self, rootNode):
        self.name = rootNode.get('name')
        self.type = rootNode.get('type')
        self.enum = rootNode.get('requires')

    def values(self):
        return {
            'flagName': self.name,
            'flagType': self.type,
            'flagEnum': self.enum,
        }

class VulkanFunction:

    class Parameter(VulkanVariable):

        def __init__(self, rootNode, constants, aliases, parentName):
            VulkanVariable.__init__(self, rootNode, constants, aliases, parentName)

        def values(self):
            return {
                'prmName': self.name,
                'prmBaseType': self.baseType,
                'prmTypeID': self.typeID,
                'prmType': self.type,
                'prmChildType': self.childType,
                'prmPtrLevel': self.pointerLevels,
                'prmLength': self.arrayLength,
                'prmInheritedConditions': self.inheritedConditions,
            }

    def __init__(self, rootNode, constants, aliases, extensions):
        self.name = rootNode.find('proto').find('name').text
        self.returnType = rootNode.find('proto').find('type').text

        self.parameters = []
        self.namedParams = ''
        self.typedParams = ''
        for node in rootNode.findall('param'):
            self.parameters.append(VulkanFunction.Parameter(node, constants, aliases, self.name))
            self.namedParams += self.parameters[-1].name + ', '
            self.typedParams += self.parameters[-1].text + ', '
        if len(self.parameters) > 0:
            self.namedParams = self.namedParams[0:-2]
            self.typedParams = self.typedParams[0:-2]

        if self.parameters[0].type in ['VkInstance', 'VkPhysicalDevice'] or self.name == 'vkCreateInstance':
            self.dispatchType = 'instance'
        else:
            self.dispatchType = 'device'

        if self.name in extensions and extensions[self.name].type == 'instance':    
            self.type = 'instance'
        else:
            self.type = self.dispatchType

        self.stateTrackingCode = ''
        if self.name in JSON_INTERCEPT_API:
            self.stateTrackingCode = JSON_INTERCEPT_API[self.name]

        self.safeToPrint = True
        for param in self.parameters:
            if param.pointerLevels == 1 and param.type.find("const") == -1:
                self.safeToPrint = False

    def values(self):
        return {
            'funcName': self.name,
            'funcShortName': self.name[2:len(self.name)],
            'funcType': self.type,
            'funcReturn': self.returnType,
            'funcNamedParams': self.namedParams,
            'funcTypedParams': self.typedParams,
            'funcDispatchParam': self.parameters[0].name,
            'funcDispatchType' : self.dispatchType, 
            'funcStateTrackingCode': self.stateTrackingCode,
            'funcSafeToPrint': self.safeToPrint,
        }

class VulkanFunctionPointer:

    def __init__(self, rootNode):
        self.name = rootNode.get('name')

    def values(self):
        return {
            'pfnName': self.name,
        }

class VulkanHandle:

    def __init__(self, rootNode):
        self.name = rootNode.get('name')
        self.type = rootNode.get('type')
        self.parent = rootNode.get('parent')

    def values(self):
        return {
            'hdlName': self.name,
            'hdlType': self.type,
            'hdlParent': self.parent,
        }

def str_VkStructureTypeToEnum(structureType):
    return structureType.title().replace('_', '').replace('VkStructureType','Vk')

class VulkanStruct:

    class Member(VulkanVariable):

        def __init__(self, rootNode, constants, parentName):
            VulkanVariable.__init__(self, rootNode, constants, None, parentName)

            # Search for a member condition
            self.condition = None
            self.structValues = rootNode.get('values')

        def values(self):
            return {
                'memName': self.name,
                'memBaseType': self.baseType,
                'memTypeID': self.typeID,
                'memType': self.type,
                'memChildType': self.childType,
                'memPtrLevel': self.pointerLevels,
                'memLength': self.arrayLength,
                'memLengthIsMember': self.lengthMember,
                'memCondition': self.condition,
                'memInheritedConditions': self.inheritedConditions,
            }


    def __init__(self, rootNode, constants, enums):
        self.name = rootNode.get('name')
        self.structExtends = rootNode.get('structextends')
        self.members = []
        for node in rootNode.findall('member'):
            self.members.append(VulkanStruct.Member(node, constants, self.name))
        self.conditionVars = ''

        self.structureIndex = -1
        
        if(self.structExtends is not None):
            for member in self.members:
                if(member.structValues is not None):
                    for enum in enums:
                        if(enum.name == 'VkStructureType'):
                            for opt in enum.options:
                                if(member.structValues  == opt.name):
                                    self.structureIndex = opt.value
              
    def values(self):
        return {
            'sctName': self.name,
            'sctConditionVars': self.conditionVars,
            'sctStructureTypeIndex': self.structureIndex,
        }

class VulkanSystemType:

    def __init__(self, name, ext):
        self.name = name
        self.type = self.name if name not in POINTER_TYPES else self.name + '*'
        self.ext = ext

    def __eq__(self, that):
        return self.name == that.name and self.type == that.type

    def __hash__(self):
        return hash(self.name) | hash(self.type)

    def values(self):
        return {
            'sysName': self.name,
            'sysType': self.type,
        }

class VulkanUnion:

    class Choice(VulkanVariable):

        def __init__(self, rootNode, constants, parentName):
            VulkanVariable.__init__(self, rootNode, constants, None, parentName)

        def values(self):
            return {
                'chcName': self.name,
                'chcBaseType': self.baseType,
                'chcTypeID': self.typeID,
                'chcType': self.type,
                'chcChildType': self.childType,
                'chcPtrLevel': self.pointerLevels,
                'chcLength': self.arrayLength,
                #'chcLengthIsMember': self.lengthMember,
            }

    def __init__(self, rootNode, constants):
        self.name = rootNode.get('name')
        self.choices = []
        for node in rootNode.findall('member'):
            self.choices.append(VulkanUnion.Choice(node, constants, self.name))

    def values(self):
        return {
            'unName': self.name,
        }
