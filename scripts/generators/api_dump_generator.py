#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2025 Valve Corporation
# Copyright (c) 2015-2025 LunarG, Inc.
# Copyright (c) 2015-2016, 2019, 2021 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import re
from base_generator import BaseGenerator
from vulkan_object import Struct, Command, Param, Member

POINTER_TYPES = ['void', 'xcb_connection_t', 'Display', 'SECURITY_ATTRIBUTES', 'ANativeWindow', 'AHardwareBuffer', 'wl_display', 'wl_surface', '_screen_context', '_screen_window', '_screen_buffer']

TRACKED_STATE = {
    'vkAllocateCommandBuffers':
        'if(result == VK_SUCCESS)\n' +
            'ApiDumpInstance::current().addCmdBuffers(\n' +
                'device,\n' +
                'pAllocateInfo->commandPool,\n' +
                'std::vector<VkCommandBuffer>(pCommandBuffers, pCommandBuffers + pAllocateInfo->commandBufferCount),\n' +
                'pAllocateInfo->level\n'
            ');',
    'vkDestroyCommandPool':
        'ApiDumpInstance::current().eraseCmdBufferPool(device, commandPool);'
    ,
    'vkFreeCommandBuffers':
        'ApiDumpInstance::current().eraseCmdBuffers(device, commandPool, std::vector<VkCommandBuffer>(pCommandBuffers, pCommandBuffers + commandBufferCount));'
    ,
}

PARAMETER_STATE = {
    'VkPipelineViewportStateCreateInfo': {
        'VkGraphicsPipelineCreateInfo':
            'ApiDumpInstance::current().setIsDynamicViewport('
            'object.pDynamicState && '
            'std::count('
                'object.pDynamicState->pDynamicStates, '
                'object.pDynamicState->pDynamicStates + object.pDynamicState->dynamicStateCount, '
                'VK_DYNAMIC_STATE_VIEWPORT'
            ') > 0);'
            'ApiDumpInstance::current().setIsDynamicScissor('
            'object.pDynamicState && '
            'std::count('
                'object.pDynamicState->pDynamicStates, '
                'object.pDynamicState->pDynamicStates + object.pDynamicState->dynamicStateCount, '
                'VK_DYNAMIC_STATE_SCISSOR'
            '));'
            'ApiDumpInstance::current().setIsGPLPreRasterOrFragmentShader(checkForGPLPreRasterOrFragmentShader(object));',
    },
    'VkCommandBufferBeginInfo': {
        'vkBeginCommandBuffer':
            'ApiDumpInstance::current().setCmdBuffer(commandBuffer);',
    },
    'VkPhysicalDeviceMemoryProperties': {
        'VkPhysicalDeviceMemoryProperties2':
            'ApiDumpInstance::current().setMemoryHeapCount(object.memoryProperties.memoryHeapCount);',
    },
    'VkDescriptorDataEXT': {
        'VkDescriptorGetInfoEXT':
            'ApiDumpInstance::current().setDescriptorType(object.type);',
    },
    'VkIndirectExecutionSetInfoEXT':{
        'VkIndirectExecutionSetInfoEXT':
            'ApiDumpInstance::current().setIndirectExecutionSetInfoType(object.type);',
    },
    'VkIndirectCommandsTokenDataEXT':{
        'VkIndirectCommandsLayoutTokenEXT':
            'ApiDumpInstance::current().setIndirectCommandsLayoutToken(object.type);',
    }
}

VALIDITY_CHECKS = {
    'VkBufferCreateInfo': {
        'pQueueFamilyIndices': 'object.sharingMode == VK_SHARING_MODE_CONCURRENT',
    },
    'VkCommandBufferBeginInfo': {
        # Tracked state ApiDumpInstance, and inherited cmd_buffer
        'pInheritanceInfo': 'ApiDumpInstance::current().getCmdBufferLevel() == VK_COMMAND_BUFFER_LEVEL_SECONDARY',
    },
    'VkDescriptorSetLayoutBinding': {
        'pImmutableSamplers':
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)',
    },
    'VkImageCreateInfo': {
        'pQueueFamilyIndices': 'object.sharingMode == VK_SHARING_MODE_CONCURRENT',
    },
    'VkPipelineViewportStateCreateInfo': {
        'pViewports': '!ApiDumpInstance::current().getIsDynamicViewport()', # Inherited state variable is_dynamic_viewport
        'pScissors': '!ApiDumpInstance::current().getIsDynamicScissor()',   # Inherited state variable is_dynamic_scissor
    },
    'VkSwapchainCreateInfoKHR': {
        'pQueueFamilyIndices': 'object.imageSharingMode == VK_SHARING_MODE_CONCURRENT',
    },
    'VkWriteDescriptorSet': {
        'pImageInfo':
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)',
        'pBufferInfo':
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC)',
        'pTexelBufferView':
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)',
    },
    'VkDescriptorDataEXT':{
        'pSampler': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_SAMPLER',
        'pCombinedImageSampler': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER',
        'pInputAttachmentImage': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT',
        'pSampledImage': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE',
        'pStorageImage': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE',
        'pUniformTexelBuffer': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER',
        'pStorageTexelBuffer': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER',
        'pUniformBuffer': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER',
        'pStorageBuffer': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER',
        'accelerationStructure': 'ApiDumpInstance::current().getDescriptorType() == VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR',
    },
    'VkPipelineRenderingCreateInfo': {
        'colorAttachmentCount': '!ApiDumpInstance::current().getIsGPLPreRasterOrFragmentShader()',
        'pColorAttachmentFormats': '!ApiDumpInstance::current().getIsGPLPreRasterOrFragmentShader()',
        'depthAttachmentFormat': '!ApiDumpInstance::current().getIsGPLPreRasterOrFragmentShader()',
        'stencilAttachmentFormat': '!ApiDumpInstance::current().getIsGPLPreRasterOrFragmentShader()',
    },
    'VkIndirectExecutionSetInfoEXT':{
        'pPipelineInfo': 'ApiDumpInstance::current().getIndirectExecutionSetInfoType() == VK_INDIRECT_EXECUTION_SET_INFO_TYPE_PIPELINES_EXT',
        'pShaderInfo': 'ApiDumpInstance::current().getIndirectExecutionSetInfoType() == VK_INDIRECT_EXECUTION_SET_INFO_TYPE_SHADER_OBJECTS_EXT',
    },
    'VkIndirectCommandsTokenDataEXT':{
        'pPushConstant': 'ApiDumpInstance::current().getIndirectCommandsLayoutToken() == VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_EXT || ApiDumpInstance::current().getIndirectCommandsLayoutToken() == VK_INDIRECT_COMMANDS_TOKEN_TYPE_SEQUENCE_INDEX_EXT',
        'pVertexBuffer': 'ApiDumpInstance::current().getIndirectCommandsLayoutToken() == VK_INDIRECT_COMMANDS_TOKEN_TYPE_VERTEX_BUFFER_EXT',
        'pIndexBuffer': 'ApiDumpInstance::current().getIndirectCommandsLayoutToken() == VK_INDIRECT_COMMANDS_TOKEN_TYPE_INDEX_BUFFER_EXT',
        'pExecutionSet': 'ApiDumpInstance::current().getIndirectCommandsLayoutToken() == VK_INDIRECT_COMMANDS_TOKEN_TYPE_EXECUTION_SET_EXT',
    },
}

# These types are defined in both video.xml and vk.xml. Because duplicate functions aren't allowed,
# we have to prevent these from generating twice. This is done by removing the types from the non-video
# outputs
DUPLICATE_TYPES_IN_VIDEO_HEADER = ['uint32_t', 'uint16_t', 'uint8_t', 'int32_t', 'int16_t', 'int8_t']

# Short list of all of the functions that are 'global', meaning they can be queried from vkGetInstanceProcAddr(NULL, "<func_name>")
GLOBAL_FUNCTION_NAMES = ['vkEnumerateInstanceLayerProperties', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceVersion', 'vkCreateInstance']

BLOCKING_API_CALLS = [
    'vkWaitForFences', 'vkWaitSemaphores', 'vkQueuePresentKHR', 'vkDeviceWaitIdle',
    'vkQueueWaitIdle', 'vkAcquireNextImageKHR', 'vkGetQueryPoolResults', 'vkWaitSemaphoresKHR'
]

EXCLUDED_INCLUDE_LIST = [
    'vk_platform',
    'stdint.h',
]

FUNCTION_IMPLEMENTATION_IGNORE_LIST = ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr', 'vkEnumerateInstanceVersion']

def shouldSkipSystemType(sys, video):
    return (sys.requires is not None and (sys.requires in EXCLUDED_INCLUDE_LIST or 'vk_video' in sys.requires)) or \
            sys.name == 'int' or (video and sys.name in DUPLICATE_TYPES_IN_VIDEO_HEADER)

# Returns the command's parameters declaration. ex) uint32_t foo, float bar, char* baz
def commandParameterDeclarationText(command):
    return ', '.join(str.strip(p.cDeclaration) for p in command.params)

# Comma separate the names of the parameters. ex) foo, bar, baz
def commandParameterUsageText(command):
    return ', '.join(p.name for p in command.params)


class VariableTypeInfo:
    def __init__(self):
        self.type: str
        self.templateType: str
        self.childType : str
        self.id : str
        self.pointerLevels : int
        self.arrayLength : None | str = None
        self.lengthIsMember : bool = False
        self.maxArrayLength : None | str = None
        self.isStruct: str = "false"
        self.isUnion : str = "false"
        self.parameter_state: None | str = None
        self.validity_check: None | str = None

class ApiDumpGenerator(BaseGenerator):
    def __init__(self):
        BaseGenerator.__init__(self)
        self.aliases = {}

    def generate(self):
        self.buildAliasMap()
        self.generate_copyright()
        if self.filename == 'api_dump.cpp':
            self.generate_dispatch_codegen()
        elif self.filename == 'api_dump_text.h':
            self.generate_header(output_format='text')
        elif self.filename == 'api_dump_text.cpp':
            self.generate_text_implementation(video=False)
        elif self.filename == 'api_dump_html.h':
            self.generate_header(output_format='html')
        elif self.filename == 'api_dump_html.cpp':
            self.generate_html_implementation(video=False)
        elif self.filename == 'api_dump_json.h':
            self.generate_header(output_format='json')
        elif self.filename == 'api_dump_json.cpp':
            self.generate_json_implementation(video=False)
        elif self.filename == 'api_dump_video_text.h':
            self.generate_text_implementation(video=True)
        elif self.filename == 'api_dump_video_html.h':
            self.generate_html_implementation(video=True)
        elif self.filename == 'api_dump_video_json.h':
            self.generate_json_implementation(video=True)

    def generate_copyright(self):
        self.write('''
/* Copyright (c) 2015-2025 Valve Corporation
 * Copyright (c) 2015-2025 LunarG, Inc.
 * Copyright (c) 2015-2017, 2019, 2021 Google Inc.
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
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */
''')

    def generate_dispatch_codegen(self):

        self.write('''#include "api_dump_text.h"
#include "api_dump_html.h"
#include "api_dump_json.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// Specifically implemented functions

#if defined(__GNUC__) && __GNUC__ >= 4
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590)
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#else
#define EXPORT_FUNCTION
#endif

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    ApiDumpInstance::current().initLayerSettings(pCreateInfo, pAllocator);
    if (ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text) {
        dump_function_head(ApiDumpInstance::current(), "vkCreateInstance", "pCreateInfo, pAllocator, pInstance");
        if (ApiDumpInstance::current().shouldDumpOutput()) {
            dump_text_params_vkCreateInstance(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);
        }
        dump_return_preamble(ApiDumpInstance::current(), "VkResult");
    } else {
        dump_function_head(ApiDumpInstance::current(), "vkCreateInstance", "pCreateInfo, pAllocator, pInstance", "VkResult");
    }

    // Get the function pointer
    VkLayerInstanceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr != 0);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if(fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if(result == VK_SUCCESS) {
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
    }

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {
        switch(ApiDumpInstance::current().settings().format())
        {
            case ApiDumpFormat::Text:
                dump_text_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
            case ApiDumpFormat::Html:
                dump_html_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
            case ApiDumpFormat::Json:
                dump_json_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
        }
    }
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    if (ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text) {
        dump_function_head(ApiDumpInstance::current(), "vkCreateDevice", "physicalDevice, pCreateInfo, pAllocator, pDevice");
        if (ApiDumpInstance::current().shouldDumpOutput()) {
            dump_text_params_vkCreateDevice(ApiDumpInstance::current(), physicalDevice, pCreateInfo, pAllocator, pDevice);
        }
        dump_return_preamble(ApiDumpInstance::current(), "VkResult");
    } else {
        dump_function_head(ApiDumpInstance::current(), "vkCreateDevice", "physicalDevice, pCreateInfo, pAllocator, pDevice", "VkResult");
    }

    // Get the function pointer
    VkLayerDeviceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance vk_instance = ApiDumpInstance::current().get_vk_instance(physicalDevice);
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(vk_instance, "vkCreateDevice");
    if(fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if(result == VK_SUCCESS) {
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {
        switch(ApiDumpInstance::current().settings().format())
        {
            case ApiDumpFormat::Text:
                dump_text_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
            case ApiDumpFormat::Html:
                dump_html_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
            case ApiDumpFormat::Json:
                dump_json_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
        }
    }
    return result;
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    return util_GetExtensionProperties(0, NULL, pPropertyCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    static const VkLayerProperties layerProperties[] = {
        {
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 4, VK_HEADER_VERSION), // specVersion
            VK_MAKE_VERSION(0, 2, 0), // implementationVersion
            "layer: api_dump",
        }
    };

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    static const VkLayerProperties layerProperties[] = {
        {
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 4, VK_HEADER_VERSION),
            VK_MAKE_VERSION(0, 2, 0),
            "layer: api_dump",
        }
    };

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}

// Autogen instance functions
''')


        for command in  [x for x in self.vk.commands.values() if x.instance]:
            if command.name in ['vkCreateInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkEnumerateInstanceVersion']:
                continue
            if command.protect:
                self.write(f'#if defined({command.protect})')
            self.write(f'VKAPI_ATTR {command.returnType} VKAPI_CALL {command.name}({commandParameterDeclarationText(command)})')
            self.write('{')
            if command.name not in BLOCKING_API_CALLS:
                self.write(f'''    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
    if(ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text) {{
        dump_function_head(ApiDumpInstance::current(), "{command.name}", "{commandParameterUsageText(command)}");
        if (ApiDumpInstance::current().shouldDumpOutput()) {{
            dump_text_params_{command.name}(ApiDumpInstance::current(), {commandParameterUsageText(command)});
        }}
        dump_return_preamble(ApiDumpInstance::current(), "{command.returnType}");
    }} else {{
        dump_function_head(ApiDumpInstance::current(), "{command.name}", "{commandParameterUsageText(command)}", "{command.returnType}");
    }}''')

            if command.name == 'vkGetPhysicalDeviceToolPropertiesEXT':
                self.write('''    static const VkPhysicalDeviceToolPropertiesEXT api_dump_layer_tool_props = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT,
        nullptr,
        "API Dump Layer",
        "2",
        VK_TOOL_PURPOSE_PROFILING_BIT_EXT | VK_TOOL_PURPOSE_TRACING_BIT_EXT,
        "The VK_LAYER_LUNARG_api_dump utility layer prints API calls, parameters, and values to the identified output stream.",
        "VK_LAYER_LUNARG_api_dump"};

    auto original_pToolProperties = pToolProperties;
    if (pToolProperties != nullptr) {
        *pToolProperties = api_dump_layer_tool_props;
        pToolProperties = ((*pToolCount > 1) ? &pToolProperties[1] : nullptr);
        (*pToolCount)--;
    }''')
            if command.name == 'vkDestroyInstance':
                self.write('    auto dispatch_key = get_dispatch_key(instance);')

            return_str = f'{command.returnType} result = ' if command.returnType != 'void' else ''
            self.write(f'    {return_str}instance_dispatch_table({command.params[0].name})->{command.name[2:]}({commandParameterUsageText(command)});')
            if command.name in BLOCKING_API_CALLS:
                self.write('    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());')
                self.write(f'    dump_function_head(ApiDumpInstance::current(), "{command.name}", "{commandParameterUsageText(command)}", "{command.returnType}");')

            self.write('    ')
            if command.name in TRACKED_STATE:
                self.write(TRACKED_STATE[command.name])

            if command.name == 'vkEnumeratePhysicalDevices':
                self.write('    if (pPhysicalDeviceCount != nullptr && pPhysicalDevices != nullptr) {')
                self.write('        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {')
                self.write('            ApiDumpInstance::current().set_vk_instance(pPhysicalDevices[i], instance);')
                self.write('        }')
                self.write('    }')

            if command.name == 'vkDestroyInstance':
                self.write('    destroy_instance_dispatch_table(dispatch_key);')

            if command.name == 'vkGetPhysicalDeviceToolPropertiesEXT':
                self.write('    if (original_pToolProperties != nullptr) {')
                self.write('        pToolProperties = original_pToolProperties;')
                self.write('    }\n')
                self.write('    (*pToolCount)++;')

            self.write(f'''    if (ApiDumpInstance::current().shouldDumpOutput()) {{
        switch(ApiDumpInstance::current().settings().format())
        {{
            case ApiDumpFormat::Text:
                dump_text_{command.name}(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{', '.join(p.name for p in command.params)});
                break;
            case ApiDumpFormat::Html:
                dump_html_{command.name}(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{', '.join(p.name for p in command.params)});
                break;
            case ApiDumpFormat::Json:
                dump_json_{command.name}(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{', '.join(p.name for p in command.params)});
                break;
        }}
    }}''')
            if command.returnType != 'void':
                self.write('    return result;')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n// Autogen device functions\n')

        for command in [x for x in self.vk.commands.values() if x.device]:
            if command.name in ['vkGetDeviceProcAddr']:
                continue

            if command.protect:
                self.write(f'#if defined({command.protect})')

            self.write(f'VKAPI_ATTR {command.returnType} VKAPI_CALL {command.name}({commandParameterDeclarationText(command)})')
            self.write('{')

            if command.name not in BLOCKING_API_CALLS:
                self.write('    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());')
                if command.name in ['vkDebugMarkerSetObjectNameEXT', 'vkSetDebugUtilsObjectNameEXT']:
                    self.write('    ApiDumpInstance::current().update_object_name_map(pNameInfo);')
                self.write('    if(ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text) {')
                self.write(f'        dump_function_head(ApiDumpInstance::current(), "{command.name}", "{commandParameterUsageText(command)}");')
                self.write('        if (ApiDumpInstance::current().shouldDumpOutput()) {')
                self.write(f'            dump_text_params_{command.name}(ApiDumpInstance::current(), {commandParameterUsageText(command)});')
                self.write('        }')
                self.write(f'        dump_return_preamble(ApiDumpInstance::current(), "{command.returnType}");\n')
                self.write('    } else {')
                self.write(f'        dump_function_head(ApiDumpInstance::current(), "{command.name}", "{commandParameterUsageText(command)}", "{command.returnType}");')
                self.write('    }')

            return_str = f'{command.returnType} result = ' if command.returnType != 'void' else ''
            self.write(f'    {return_str}device_dispatch_table({command.params[0].name})->{command.name[2:]}({commandParameterUsageText(command)});')
            if command.name in BLOCKING_API_CALLS:
                self.write('    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());')
                self.write(f'    dump_function_head(ApiDumpInstance::current(), "{command.name}", "{commandParameterUsageText(command)}", "{command.returnType}");')

            if command.name in TRACKED_STATE:
                self.write('    ' + TRACKED_STATE[command.name])
            else:
                self.write('    ')

            if command.name == 'vkDestroyDevice':
                self.write('    destroy_device_dispatch_table(get_dispatch_key(device));')

            self.write(f'''    if (ApiDumpInstance::current().shouldDumpOutput()) {{
        switch(ApiDumpInstance::current().settings().format())
        {{
            case ApiDumpFormat::Text:
                dump_text_{command.name}(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{commandParameterUsageText(command)});
                break;
            case ApiDumpFormat::Html:
                dump_html_{command.name}(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{commandParameterUsageText(command)});
                break;
            case ApiDumpFormat::Json:
                dump_json_{command.name}(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{commandParameterUsageText(command)});
                break;
        }}
    }}''')
            if command.name == 'vkQueuePresentKHR':
                self.write('    ApiDumpInstance::current().nextFrame();')
            if command.returnType != 'void':
                self.write('    return result;')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\nVKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL api_dump_known_instance_functions(VkInstance instance, const char* pName)')
        self.write('{\n')
        for command in self.vk.commands.values():
            if command.name in ['vkEnumerateDeviceExtensionProperties', 'vkEnumerateInstanceVersion'] or (command.device and 'VK_EXT_debug_utils' not in command.extensions):
                continue
            if command.protect:
                self.write(f'#if defined({command.protect})')
            # checking for instance functions seems to have always been broken
            # if command.params[0].type in ['VkInstance', 'VkPhysicalDevice']:
            #     self.write(f'    if(strcmp(pName, "{command.name}") == 0 && (!instance || instance_dispatch_table(instance)->{command.name[2:]}))')
            # else:
            self.write(f'    if(strcmp(pName, "{command.name}") == 0)')
            self.write(f'        return reinterpret_cast<PFN_vkVoidFunction>({command.name});')
            if command.protect:
                self.write(f'#endif // {command.protect}')
        self.write('\n    return nullptr;')
        self.write('}')

        self.write('\nVKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL api_dump_known_device_functions(VkDevice device, const char* pName)')
        self.write('{\n')
        for command in [x for x in self.vk.commands.values() if x.device ]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            self.write(f'    if(strcmp(pName, "{command.name}") == 0 && (!device || device_dispatch_table(device)->{command.name[2:]}))')
            self.write(f'        return reinterpret_cast<PFN_vkVoidFunction>({command.name});')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n    return nullptr;')
        self.write('}')

        self.write('''
EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{
    auto instance_func = api_dump_known_instance_functions(instance, pName);
    if (instance_func) return instance_func;

    // Make sure that device functions queried through GIPA works
    auto device_func = api_dump_known_device_functions(NULL, pName);
    if (device_func) return device_func;

    // Haven't created an instance yet, exit now since there is no instance_dispatch_table
    if(instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return nullptr;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName)
{
    auto device_func = api_dump_known_device_functions(device, pName);
    if (device_func) return device_func;

    // Haven't created a device yet, exit now since there is no device_dispatch_table
    if(device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return nullptr;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}''')

    def generate_header(self, output_format):
        self.write('#pragma once\n')

        self.write('#include "api_dump.h"')

        self.write('\n//========================= Function Helpers ================================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            self.write(f'void dump_{output_format}_params_{command.name}(ApiDumpInstance& dump_inst, {commandParameterDeclarationText(command)});')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n//========================= Function Implementations ========================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            if command.returnType != 'void':
                self.write(f'void dump_{output_format}_{command.name}(ApiDumpInstance& dump_inst, {command.returnType} result, {commandParameterDeclarationText(command)});')
            else:
                self.write(f'void dump_{output_format}_{command.name}(ApiDumpInstance& dump_inst, {commandParameterDeclarationText(command)});')
            if command.protect:
                self.write(f'#endif // {command.protect}')
        self.write('\n')

    def generate_type_implementation(self, video, output_format):
        self.write('\n//=========================== Type Implementations ==========================//\n')

        for t in [x for x in self.vk.platformTypes.values() if x.requires in ['vk_platform', 'stdint']]:
            if t.name in ['void'] or (not video and t.name in DUPLICATE_TYPES_IN_VIDEO_HEADER):
                continue
            self.write(f'void dump_{output_format}_{t.name}({t.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            cast = ''
            if t.name == 'uint8_t':
                cast = '(uint32_t) '
            if t.name == 'int8_t':
                cast = '(int32_t) '

            if output_format == 'text':
                self.write(f'    settings.stream() << {cast}object;')

            if output_format == 'html':
                self.write('    settings.stream() << "<div class=\'val\'>";')
                self.write(f'    settings.stream() << {cast}object;')
                self.write('    settings.stream() << "</div></summary>";')

            if output_format == 'json':
                self.write('')
                self.write(f'    settings.stream() << "\\"" << {cast}object << "\\"";')

            self.write('}')

    def generate_basetype_implementation(self, output_format):
        self.write('\n//========================= Basetype Implementations ========================//\n')

        for basetype in self.vk.baseTypes.values():
            if basetype.protect:
                self.write(f'#if defined({basetype.protect})')
            if basetype.name in ['ANativeWindow', 'AHardwareBuffer']:
                self.write(f'void dump_{output_format}_{basetype.name}(const {basetype.name}* object, const ApiDumpSettings& settings, int indents)')
            else:
                self.write(f'void dump_{output_format}_{basetype.name}({basetype.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            if output_format == 'text':
                self.write('    settings.stream() << object;')
            if output_format == 'html':
                self.write('    settings.stream() << "<div class=\'val\'>" << object << "</div></summary>";')
            if output_format == 'json':
                self.write('    settings.stream() << "\\"" << object << "\\"";')
            self.write('}')
            if basetype.protect:
                self.write(f'#endif // {basetype.protect}')


    def generate_text_implementation(self, video=False):
        if video:
            self.write('#pragma once\n')

        self.write('#include "api_dump_text.h"')
        if not video:
            self.write('\n#include "api_dump_video_text.h"')

        self.generate_type_implementation(video, 'text')
        self.generate_basetype_implementation('text')

        self.write('\n//======================= System Type Implementations =======================//\n')
        sortedSystemTypes = dict(sorted(self.vk.platformTypes.items()))
        for sys in sortedSystemTypes.values():
            if shouldSkipSystemType(sys, video):
                continue
            if sys.protect:
                self.write(f'#if defined({sys.protect})')
            self.write(f'void dump_text_{sys.name}(const {sys.name}{"*" if sys.name in POINTER_TYPES else ""} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            if sys.name in POINTER_TYPES:
                self.write('''    if (object == NULL) {
        settings.stream() << "NULL";
        return;
    }
    OutputAddress(settings, object);''')
            else:
                self.write('''    if (settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";''')
            self.write('}')
            if sys.protect:
                self.write(f'#endif // {sys.protect}')

        self.write('\n//========================== Handle Implementations =========================//\n')
        for handle in self.vk.handles.values():
            if handle.protect:
                self.write(f'#if defined({handle.protect})')
            self.write(f'''void dump_text_{handle.name}(const {handle.name} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress()) {{
        settings.stream() << object;

        std::unordered_map<uint64_t, std::string>::const_iterator it = ApiDumpInstance::current().object_name_map.find((uint64_t) object);
        if (it != ApiDumpInstance::current().object_name_map.end()) {{
            settings.stream() << " [" << it->second << "]";
        }}
    }} else {{
        settings.stream() << "address";
    }}
}}''')
            if handle.protect:
                self.write(f'#endif // {handle.protect}')

        self.write('\n//=========================== Enum Implementations ==========================//\n')
        for enum in self.vk.enums.values():
            if enum.protect:
                self.write(f'#if defined({enum.protect})')
            self.write(f'void dump_text_{enum.name}({enum.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    switch((int64_t) object)')
            self.write('    {')
            for field in enum.fields:
                self.write(f'    case {field.valueStr}:')
                self.write(f'        settings.stream() << "{field.name} (";')
                self.write('        break;')
            self.write('    default:')
            self.write('        settings.stream() << "UNKNOWN (";')
            self.write('    }')
            self.write('    settings.stream() << object << ")";')
            self.write('}')
            if enum.protect:
                self.write(f'#endif // {enum.protect}')
        self.write('\n//========================= Bitmask Implementations =========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            if bitmask.bitWidth == 64:
                self.write('// 64 bit bitmasks don\'t have an enum of bit values.')
                self.write(f'typedef VkFlags64 {bitmask.name};')
            self.write(f'void dump_text_{bitmask.name}({bitmask.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    bool is_first = true;')
            self.write('    settings.stream() << object;')
            for field in bitmask.flags:
                self.write(f'    if(object {"==" if  field.zero or field.multiBit else "&"} {field.valueStr if field.multiBit else field.value}) {{')
                self.write(f'        settings.stream() << (is_first ? \" (\" : \" | \") << "{field.name}"; is_first = false;')
                self.write('    }')
            self.write('    if(!is_first)')
            self.write('        settings.stream() << ")";')
            self.write('}')
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n//=========================== Flag Implementations ==========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            self.write(f'void dump_text_{bitmask.flagName}({bitmask.flagName} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write(f'    dump_text_{bitmask.name}(({bitmask.name}) object, settings, indents);')
            self.write('}')
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n')
        for flag in self.vk.flags.values():
            if flag.bitmaskName is not None:
                continue
            if flag.protect:
                self.write(f'#if defined({flag.protect})')
            self.write(f'void dump_text_{flag.name}({flag.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << object;')
            self.write('}')
            if flag.protect:
                self.write(f'#endif // {flag.protect}')

        self.write('\n//======================= Func Pointer Implementations ======================//\n')

        for funcpointer in self.vk.funcPointers.values():
            self.write(f'void dump_text_{funcpointer.name}({funcpointer.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    if(settings.showAddress())')
            self.write('        settings.stream() << object;')
            self.write('    else')
            self.write('        settings.stream() << "address";')
            self.write('}')

        self.write('\n//======================== Union Forward Declarations =======================//\n')

        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_text_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, int indents);')
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Declarations =======================//\n')
        if not video:
            self.write('void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents, const char* pnext_type);')
            self.write('void dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);')

        self.write('\n//========================== Struct Implementations =========================//\n')
        for struct in [ x for x in self.vk.structs.values() if not x.union ]:
            if struct.protect:
                self.write(f'#if defined({struct.protect})')
            self.write(f'void dump_text_{struct.name}(const {struct.name}& object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    if(settings.showAddress())')
            self.write('        settings.stream() << &object << ":\\n";')
            self.write('    else')
            self.write('        settings.stream() << "address:\\n";')
            for member in struct.members:
                typeInfo = self.fillin_variable_typeinfo(member, struct)
                if typeInfo.parameter_state:
                    if typeInfo.validity_check:
                        self.write(f'    if ({typeInfo.validity_check})')
                        self.write('        ' + typeInfo.parameter_state)
                    else:
                        self.write('    ' + typeInfo.parameter_state)

            self.write('')
            for member in struct.members:
                typeInfo = self.fillin_variable_typeinfo(member, struct)

                if typeInfo.validity_check is not None:
                    self.write(f'    if({typeInfo.validity_check})')

                if typeInfo.pointerLevels == 0:
                    if member.name != 'pNext':
                        if member.name == 'apiVersion':
                            self.write(f'    dump_text_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, OutputApiVersionTEXT);')
                        else:
                            self.write(f'    dump_text_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')
                    else:
                        self.write(f'    dump_text_pNext_struct_name(object.{member.name}, settings, indents + 1, "{typeInfo.type}");')

                elif typeInfo.pointerLevels > 0:
                    if typeInfo.arrayLength is None:
                        self.write(f'    dump_text_pointer<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

                    else:
                        if not typeInfo.lengthIsMember:
                            self.write(f'    dump_text_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

                        elif typeInfo.lengthIsMember and member.name != 'pCode':
                            if typeInfo.arrayLength[0].isdigit() or typeInfo.arrayLength[0].isupper():
                                self.write(f'    dump_text_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

                            elif not typeInfo.arrayLength[0].isdigit() or typeInfo.arrayLength[0].isupper():
                                if typeInfo.arrayLength == 'rasterizationSamples':
                                    self.write(f'    dump_text_array<const {typeInfo.templateType}>(object.{member.name}, (object.{typeInfo.arrayLength} + 31) / 32, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')
                                else:
                                    if typeInfo.maxArrayLength is not None:
                                        self.write(f'    dump_text_array<const {typeInfo.templateType}>(object.{member.name}, std::min(object.{typeInfo.arrayLength}, {typeInfo.maxArrayLength}), settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')
                                    else:
                                        self.write(f'    dump_text_array<const {typeInfo.templateType}>(object.{member.name}, object.{typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

                self.write('')
                if struct.name == 'VkShaderModuleCreateInfo':
                    if member.name == 'pCode':
                        self.write('    if(settings.showShader())')
                        self.write(f'        dump_text_array<const {member.type}>(object.{member.name}, object.{typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')
                        self.write('    else')
                        self.write(f'        dump_text_special("SHADER DATA", settings, "{typeInfo.type}", "{member.name}", indents + 1);')
                self.write('')
                if typeInfo.validity_check is not None:
                    self.write('    else')
                    self.write(f'        dump_text_special("UNUSED", settings, "{typeInfo.type}", "{member.name}", indents + 1);')


            self.write('')
            for member in struct.members:
                if member.pointer and member.name == 'pNext' and struct.name not in ['VkBaseInStructure', 'VkBaseOutStructure']:
                    self.write('    if(object.pNext != nullptr){')
                    self.write('        dump_text_pNext_trampoline(object.pNext, settings, indents < 2 ? indents + 1 : indents);')
                    self.write('    }')

            self.write('}')
            if struct.protect:
                self.write(f'#endif // {struct.protect}')

        self.write('\n//========================== Union Implementations ==========================//\n')

        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_text_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, int indents)')
            self.write('''{
    if(settings.showAddress())
        settings.stream() << &object << " (Union):\\n";
    else
        settings.stream() << "address (Union):\\n";''')
            for member in union.members:
                typeInfo = self.fillin_variable_typeinfo(member, union)

                if typeInfo.validity_check is not None:
                    self.write(f'    if({typeInfo.validity_check})')

                if typeInfo.pointerLevels == 0:
                    self.write(f'    dump_text_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is None:
                    self.write(f'    dump_text_pointer<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is not None:
                    self.write(f'    dump_text_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_text_{typeInfo.id});')

            self.write('}')
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Implementation =======================//\n')
        if not video:
            self.write('void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents, const char* pnext_type)')
            self.write('{')
            self.write('    if (object == nullptr) {')
            self.write('        dump_text_value<const void*>(object, settings, pnext_type, "pNext", indents, dump_text_void);')
            self.write('        return;')
            self.write('    }\n')

            self.write('    settings.formatNameType(indents, "pNext", pnext_type);')
            self.write('    switch(reinterpret_cast<const VkBaseInStructure*>(object)->sType) {')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                if struct.protect:
                    self.write(f'#if defined({struct.protect})')
                if struct.sType is not None:
                    self.write(f'        case {struct.sType}:')
                    self.write(f'            settings.stream() << "{struct.name}\\n";')
                    self.write('            break;')
                if struct.protect:
                    self.write(f'#endif // {struct.protect}')
            self.write('        case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47')
            self.write('        case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48')
            self.write('        default:')
            self.write('            settings.stream() << "NULL\\n";')
            self.write('            break;')
            self.write('    }')
            self.write('}\n')

            self.write('void dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    VkBaseInStructure base_struct{};')
            self.write('    memcpy(&base_struct, object, sizeof(VkBaseInStructure));')
            self.write('    switch(base_struct.sType) {')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                if struct.protect:
                    self.write(f'#if defined({struct.protect})')
                if struct.sType is not None:
                    self.write(f'    case {struct.sType}:')
                    self.write(f'        dump_text_pNext<const {struct.name}>(reinterpret_cast<const {struct.name}*>(object), settings, "{struct.name}", indents, dump_text_{struct.name});')
                    self.write('        break;')
                if struct.protect:
                    self.write(f'#endif // {struct.protect}')
            self.write('')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48')
            self.write('        if(base_struct.pNext != nullptr){')
            self.write('            dump_text_pNext_trampoline(reinterpret_cast<const void*>(base_struct.pNext), settings, indents);')
            self.write('        } else {')
            self.write('            settings.formatNameType(indents, "pNext", "const void*");')
            self.write('            settings.stream() << "NULL\\n";')
            self.write('        }')
            self.write('        break;')
            self.write('    default:')
            self.write('        settings.formatNameType(indents, "pNext", "const void*");')
            self.write('        settings.stream() << "UNKNOWN (" << (int64_t) (base_struct.sType) << ")\\n";')
            self.write('    }')
            self.write('}')

        self.write('\n//========================== Function Helpers ===============================//\n')

        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            self.write(f'void dump_text_params_{command.name}(ApiDumpInstance& dump_inst, {commandParameterDeclarationText(command)})')
            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());')
            self.write('    if(settings.showParams())')
            self.write('    {')
            self.write('        settings.stream() << "\\n";')
            for param in command.params:
                typeInfo = self.fillin_variable_typeinfo(param, command)

                if typeInfo.parameter_state is not None:
                    self.write('        ' + typeInfo.parameter_state)

                if typeInfo.pointerLevels == 0:
                    self.write(f'        dump_text_value<const {typeInfo.templateType}>({param.name}, settings, "{typeInfo.type}", "{param.name}", 1, dump_text_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is None:
                    self.write(f'        dump_text_pointer<const {typeInfo.templateType}>({param.name}, settings, "{typeInfo.type}", "{param.name}", 1, dump_text_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is not None:
                    self.write(f'        dump_text_array<const {typeInfo.templateType}>({param.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{param.name}", 1, dump_text_{typeInfo.id});')

            self.write('        if (settings.shouldFlush()) settings.stream().flush();')
            self.write('    }')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n//========================= Function Implementations ========================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            returnParam = f'{command.returnType} result, ' if command.returnType != 'void' else ''
            self.write(f'void dump_text_{command.name}(ApiDumpInstance& dump_inst, {returnParam}{commandParameterDeclarationText(command)})')

            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());')

            if command.returnType != 'void':
                self.write('    settings.stream() << " ";')
                self.write(f'    dump_text_{command.returnType}(result, settings, 0);')
            self.write('    settings.stream() << ":";')
            self.write(f'    dump_text_params_{command.name}(dump_inst, {commandParameterUsageText(command)});')
            self.write('    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')
        self.write('\n')

    def generate_html_implementation(self, video=False):

        if video:
            self.write('#pragma once\n')

        self.write('#include "api_dump_html.h"')
        if not video:
            self.write('\n#include "api_dump_video_html.h"')

        self.generate_type_implementation(video, 'html')
        self.generate_basetype_implementation('html')

        self.write('\n//======================= System Type Implementations =======================//\n')
        sortedSystemTypes = dict(sorted(self.vk.platformTypes.items()))
        for sys in sortedSystemTypes.values():
            if shouldSkipSystemType(sys, video):
                continue
            if sys.protect:
                self.write(f'#if defined({sys.protect})')
            self.write(f'void dump_html_{sys.name}(const {sys.name}{"*" if sys.name in POINTER_TYPES else ""} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            if sys.name in POINTER_TYPES:
                self.write('''    settings.stream() << "<div class=\'val\'>";
    OutputAddress(settings, object);
    settings.stream() << "</div>";''')
            else:
                self.write('''    if (settings.showAddress())
        settings.stream() << "<div class='val'>" << object << "</div></summary>";
    else
        settings.stream() << "<div class='val'>address</div></summary>";''')
            self.write('}')
            if sys.protect:
                self.write(f'#endif // {sys.protect}')
        self.write('\n//========================== Handle Implementations =========================//\n')
        for handle in self.vk.handles.values():
            if handle.protect:
                self.write(f'#if defined({handle.protect})')
            self.write(f'''void dump_html_{handle.name}(const {handle.name} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>";
    if(settings.showAddress()) {{
        settings.stream() << object;

        std::unordered_map<uint64_t, std::string>::const_iterator it = ApiDumpInstance::current().object_name_map.find((uint64_t) object);
        if (it != ApiDumpInstance::current().object_name_map.end()) {{
            settings.stream() << "</div><div class=\'val\'>[" << it->second << "]";
        }}
    }} else {{
        settings.stream() << "address";
    }}
    settings.stream() << "</div></summary>";
}}''')
            if handle.protect:
                self.write(f'#endif // {handle.protect}')

        self.write('\n//=========================== Enum Implementations ==========================//\n')
        for enum in self.vk.enums.values():
            if enum.protect:
                self.write(f'#if defined({enum.protect})')
            self.write(f'void dump_html_{enum.name}({enum.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << "<div class=\'val\'>";')
            self.write('    switch((int64_t) object)')
            self.write('    {')
            for field in enum.fields:
                self.write(f'    case {field.valueStr}:')
                self.write(f'        settings.stream() << "{field.name} (";')
                self.write('        break;')

            self.write('    default:')
            self.write('        settings.stream() << "UNKNOWN (";')
            self.write('    }')
            self.write('    settings.stream() << object << ")</div></summary>";')
            self.write('}')
            if enum.protect:
                self.write(f'#endif // {enum.protect}')

        self.write('\n//========================= Bitmask Implementations =========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            self.write(f'void dump_html_{bitmask.name}({bitmask.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << "<div class=\'val\'>";')
            self.write('    bool is_first = true;')
            self.write('    settings.stream() << object;')
            for field in bitmask.flags:
                self.write(f'    if(object {"==" if  field.zero or field.multiBit else "&"} {field.valueStr if field.multiBit else field.value}) {{')
                self.write(f'        settings.stream() << (is_first ? \" (\" : \" | \") << "{field.name}"; is_first = false;')
                self.write('    }')
            self.write('    if(!is_first)')
            self.write('        settings.stream() << ")";')
            self.write('    settings.stream() << "</div></summary>";')
            self.write('}')
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n//=========================== Flag Implementations ==========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            self.write(f'void dump_html_{bitmask.flagName}({bitmask.flagName} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write(f'    dump_html_{bitmask.name}(({bitmask.name}) object, settings, indents);')
            self.write('}')
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n')
        for flag in self.vk.flags.values():
            if flag.bitmaskName is not None:
                continue
            if flag.protect:
                self.write(f'#if defined({flag.protect})')
            self.write(f'void dump_html_{flag.name}({flag.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << "<div class=\'val\'>"')
            self.write('                             << object << "</div></summary>";')
            self.write('}')
            if flag.protect:
                self.write(f'#endif // {flag.protect}')

        self.write('\n//======================= Func Pointer Implementations ======================//\n')

        for funcpointer in self.vk.funcPointers.values():
            self.write(f'void dump_html_{funcpointer.name}({funcpointer.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << "<div class=\'val\'>";')
            self.write('    if(settings.showAddress())')
            self.write('        settings.stream() << object;')
            self.write('    else')
            self.write('        settings.stream() << "address";')
            self.write('    settings.stream() << "</div></summary>";')
            self.write('}')

        self.write('\n//======================== Union Forward Declarations =======================//\n')

        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_html_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, int indents);')
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Declarations =======================//\n')
        if not video:
            self.write('void dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);')

        self.write('\n//========================== Struct Implementations =========================//\n')
        for struct in [ x for x in self.vk.structs.values() if not x.union ]:
            if struct.protect:
                self.write(f'#if defined({struct.protect})')
            self.write(f'void dump_html_{struct.name}(const {struct.name}& object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << "<div class=\'val\'>";')
            self.write('    if(settings.showAddress())')
            self.write('        settings.stream() << &object << "\\n";')
            self.write('    else')
            self.write('        settings.stream() << "address\\n";')
            self.write('    settings.stream() << "</div></summary>";')
            for member in struct.members:
                typeInfo = self.fillin_variable_typeinfo(member, struct)
                if typeInfo.parameter_state:
                    if typeInfo.validity_check:
                        self.write(f'    if ({typeInfo.validity_check})')
                        self.write('        ' + typeInfo.parameter_state)
                    else:
                        self.write('    ' + typeInfo.parameter_state)

            self.write('')
            for member in struct.members:
                typeInfo = self.fillin_variable_typeinfo(member, struct)

                if typeInfo.validity_check is not None:
                    self.write(f'    if({typeInfo.validity_check})')

                if typeInfo.pointerLevels == 0:
                    if member.name != 'pNext':
                        if member.name == 'apiVersion':
                            self.write(f'    dump_html_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, OutputApiVersionHTML);')
                        else:
                            self.write(f'    dump_html_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')
                    else:
                        self.write('    if(object.pNext != nullptr){')
                        self.write(f'        dump_html_pNext_trampoline(object.{member.name}, settings, indents + 1);')
                        self.write('    } else {')
                        self.write(f'        dump_html_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')
                        self.write('    }')

                elif typeInfo.pointerLevels > 0:
                    if typeInfo.arrayLength is None:
                        self.write(f'    dump_html_pointer<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

                    else:
                        if not typeInfo.lengthIsMember:
                            self.write(f'    dump_html_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

                        elif typeInfo.lengthIsMember and member.name != 'pCode':
                            if typeInfo.arrayLength[0].isdigit() or typeInfo.arrayLength[0].isupper():
                                self.write(f'    dump_html_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

                            elif not typeInfo.arrayLength[0].isdigit() or typeInfo.arrayLength[0].isupper():
                                if typeInfo.arrayLength == 'rasterizationSamples':
                                    self.write(f'    dump_html_array<const {typeInfo.templateType}>(object.{member.name}, (object.{typeInfo.arrayLength} + 31) / 32, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')
                                else:
                                    if typeInfo.maxArrayLength is not None:
                                        self.write(f'    dump_html_array<const {typeInfo.templateType}>(object.{member.name}, std::min(object.{typeInfo.arrayLength}, {typeInfo.maxArrayLength}), settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')
                                    else:
                                        self.write(f'    dump_html_array<const {typeInfo.templateType}>(object.{member.name}, object.{typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

                self.write('')
                if struct.name == 'VkShaderModuleCreateInfo':
                    if member.name == 'pCode':
                        self.write('    if(settings.showShader())')
                        self.write(f'        dump_html_array<const {typeInfo.templateType}>(object.{member.name}, object.{typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')
                        self.write('    else')
                        self.write(f'        dump_html_special("SHADER DATA", settings, "{typeInfo.type}", "{member.name}", indents + 1);')

                self.write('')
                if typeInfo.validity_check is not None:
                    self.write('    else')
                    self.write(f'        dump_html_special("UNUSED", settings, "{typeInfo.type}", "{member.name}", indents + 1);')

            self.write('}')
            if struct.protect:
                self.write(f'#endif // {struct.protect}')


        self.write('\n//========================== Union Implementations ==========================//\n')
        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_html_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, int indents)')
            self.write('''{
    settings.stream() << "<div class='val'>";
    if(settings.showAddress())
        settings.stream() << &object << " (Union):\\n";
    else
        settings.stream() << "address (Union):\\n";
    settings.stream() << "</div></summary>";''')
            for member in union.members:
                typeInfo = self.fillin_variable_typeinfo(member, union)

                if typeInfo.validity_check is not None:
                    self.write(f'    if({typeInfo.validity_check})')

                if typeInfo.pointerLevels == 0:
                    self.write(f'    dump_html_value<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is None:
                    self.write(f'    dump_html_pointer<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is not None:
                    self.write(f'    dump_html_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", indents + 1, dump_html_{typeInfo.id});')

            self.write('}')
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Implementation =======================//\n')
        if not video:
            self.write('void dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    VkBaseInStructure base_struct{};')
            self.write('    memcpy(&base_struct, object, sizeof(VkBaseInStructure));')
            self.write('    switch(base_struct.sType) {')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                if struct.protect:
                    self.write(f'#if defined({struct.protect})')
                if struct.sType is not None:
                    self.write(f'    case {struct.sType}:')
                    self.write(f'        dump_html_pNext<const {struct.name}>(reinterpret_cast<const {struct.name}*>(object), settings, "{struct.name}", indents, dump_html_{struct.name});')
                    self.write('        break;')
                if struct.protect:
                    self.write(f'#endif // {struct.protect}')
            self.write('')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48')
            self.write('        if(base_struct.pNext != nullptr){')
            self.write('            dump_html_pNext_trampoline(reinterpret_cast<const void*>(base_struct.pNext), settings, indents);')
            self.write('        } else {')
            self.write('            settings.stream() << "<details class=\'data\'><summary>";')
            self.write('            dump_html_nametype(settings.stream(), settings.showType(), "pNext", "const void*");')
            self.write('            settings.stream() << "<div class=\'val\'> NULL</div></summary></details>";')
            self.write('        }')
            self.write('        break;')
            self.write('    default:')
            self.write('        settings.stream() << "<details class=\'data\'><summary>";')
            self.write('        dump_html_nametype(settings.stream(), settings.showType(), "pNext", "const void*");')
            self.write('        settings.stream() << "<div class=\'val\'>UNKNOWN (" << (int64_t) (base_struct.sType) <<")</div></summary></details>";')
            self.write('    }')
            self.write('}')

        self.write('\n//========================= Function Helpers ================================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            self.write(f'void dump_html_params_{command.name}(ApiDumpInstance& dump_inst, {commandParameterDeclarationText(command)})')
            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());\n')

            self.write('    if(settings.showParams())')
            self.write('    {')
            for param in command.params:
                typeInfo = self.fillin_variable_typeinfo(param, command)

                if typeInfo.parameter_state is not None:
                    self.write('        ' + typeInfo.parameter_state)

                if typeInfo.pointerLevels == 0:
                    self.write(f'        dump_html_value<const {typeInfo.templateType}>({param.name}, settings, "{typeInfo.type}", "{param.name}", 1, dump_html_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is None:
                    self.write(f'        dump_html_pointer<const {typeInfo.templateType}>({param.name}, settings, "{typeInfo.type}", "{param.name}", 1, dump_html_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is not None:
                    self.write(f'        dump_html_array<const {typeInfo.templateType}>({param.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{param.name}", 1, dump_html_{typeInfo.id});')

            self.write('    }')
            self.write('    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n//========================= Function Implementations ========================//\n')

        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            returnParam = f'{command.returnType} result, ' if command.returnType != 'void' else ''
            self.write(f'void dump_html_{command.name}(ApiDumpInstance& dump_inst, {returnParam}{commandParameterDeclarationText(command)})')

            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());')

            if command.returnType != 'void':
                self.write(f'    dump_html_{command.returnType}(result, settings, 0);')

            self.write(f'    dump_html_params_{command.name}(dump_inst, {commandParameterUsageText(command)});\n')
            self.write('    settings.stream() << "</details>";')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')
        self.write('\n')

    def generate_json_implementation(self, video=False):
        if video:
            self.write('#pragma once\n')

        self.write('#include "api_dump_json.h"')
        if not video:
            self.write('\n#include "api_dump_video_json.h"')

        self.generate_type_implementation(video, 'json')
        self.generate_basetype_implementation('json')

        self.write('\n//======================= System Type Implementations =======================//\n')
        sortedSystemTypes = dict(sorted(self.vk.platformTypes.items()))
        for sys in sortedSystemTypes.values():
            if shouldSkipSystemType(sys, video):
                continue
            if sys.protect:
                self.write(f'#if defined({sys.protect})')
            self.write(f'void dump_json_{sys.name}(const {sys.name}{"*" if sys.name in POINTER_TYPES else ""} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            if sys.name in POINTER_TYPES:
                self.write('    OutputAddressJSON(settings, object);')
                self.write('    settings.stream() << "\\n";')
            else:
                self.write('''    if (settings.showAddress())
        settings.stream() << "\\"" << object << "\\"";
    else
        settings.stream() << "\\"address\\"";''')
            self.write('}')
            if sys.protect:
                self.write(f'#endif // {sys.protect}')

        self.write('\n//========================== Handle Implementations =========================//\n')
        for handle in self.vk.handles.values():
            if handle.protect:
                self.write(f'#if defined({handle.protect})')
            self.write(f'''void dump_json_{handle.name}(const {handle.name} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress()) {{
        settings.stream() << "\\"" << object << "\\"";
    }} else {{
        settings.stream() << "\\"address\\"";
    }}
}}''')
            if handle.protect:
                self.write(f'#endif // {handle.protect}')

        self.write('\n//=========================== Enum Implementations ==========================//\n')
        for enum in self.vk.enums.values():
            if enum.protect:
                self.write(f'#if defined({enum.protect})')
            self.write(f'void dump_json_{enum.name}({enum.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    switch((int64_t) object)')
            self.write('    {')
            for field in enum.fields:
                self.write(f'    case {field.valueStr}:')
                self.write(f'        settings.stream() << "\\"{field.name}\\"";')
                self.write('        break;')

            self.write('    default:')
            self.write('        settings.stream() << "\\"UNKNOWN (" << object << ")\\"";')
            self.write('    }')
            self.write('}')
            if enum.protect:
                self.write(f'#endif // {enum.protect}')

        self.write('\n//========================= Bitmask Implementations =========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            self.write(f'void dump_json_{bitmask.name}({bitmask.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    bool is_first = true;')
            self.write('    settings.stream() << \'"\' << object;')
            for field in bitmask.flags:
                self.write(f'    if(object {"==" if  field.zero or field.multiBit else "&"} {field.valueStr if field.multiBit else field.value}) {{')
                self.write(f'        settings.stream() << (is_first ? \" (\" : \" | \") << "{field.name}"; is_first = false;')
                self.write('    }')
            self.write('    if(!is_first)')
            self.write('        settings.stream() << \')\';')
            self.write('    settings.stream() << "\\"";')
            self.write('}')
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n//=========================== Flag Implementations ==========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            self.write(f'void dump_json_{bitmask.flagName}({bitmask.flagName} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write(f'    dump_json_{bitmask.name}(({bitmask.name}) object, settings, indents);')
            self.write('}')
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n')
        for flag in self.vk.flags.values():
            if flag.bitmaskName is not None:
                continue
            if flag.protect:
                self.write(f'#if defined({flag.protect})')
            self.write(f'void dump_json_{flag.name}({flag.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << \'"\' << object << "\\"";')
            self.write('}')
            if flag.protect:
                self.write(f'#endif // {flag.protect}')

        self.write('\n//======================= Func Pointer Implementations ======================//\n')
        for funcpointer in self.vk.funcPointers.values():
            self.write(f'void dump_json_{funcpointer.name}({funcpointer.name} object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    if(settings.showAddress())')
            self.write('       settings.stream() << "\\"" << object << "\\"";')
            self.write('    else')
            self.write('        settings.stream() << "\\"address\\"";')
            self.write('}')

        self.write('\n//======================== Union Forward Declarations =======================//\n')

        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_json_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, int indents);')
            if union.protect:
                self.write(f'#endif // {union.protect}')


        self.write('\n//======================== pNext Chain Declarations =======================//\n')
        if not video:
            self.write('void dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);')

        self.write('\n//========================== Struct Implementations =========================//\n')
        for struct in [ x for x in self.vk.structs.values() if not x.union ]:
            if struct.protect:
                self.write(f'#if defined({struct.protect})')
            self.write(f'void dump_json_{struct.name}(const {struct.name}& object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << settings.indentation(indents) << "[\\n";')
            self.write('')
            for member in struct.members:
                typeInfo = self.fillin_variable_typeinfo(member, struct)
                if typeInfo.parameter_state:
                    if typeInfo.validity_check:
                        self.write(f'    if ({typeInfo.validity_check})')
                        self.write('        ' + typeInfo.parameter_state)
                    else:
                        self.write('    ' + typeInfo.parameter_state)

            self.write('')
            for member in struct.members:
                typeInfo = self.fillin_variable_typeinfo(member, struct)

                if member != struct.members[0]:
                    self.write('    settings.stream() << ",\\n";')

                if typeInfo.validity_check is not None:
                    self.write(f'    if({typeInfo.validity_check})')

                if typeInfo.pointerLevels == 0:
                    if member.name != 'pNext':
                        if member.name == 'apiVersion':
                            self.write(f'    dump_json_value<const {typeInfo.templateType}>(object.{member.name}, NULL, settings, "{typeInfo.type}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, OutputApiVersionJSON);')
                        else:
                            self.write(f'    dump_json_value<const {typeInfo.templateType}>(object.{member.name}, NULL, settings, "{typeInfo.type}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')
                    else:
                        self.write('    if(object.pNext != nullptr){')
                        self.write(f'        dump_json_pNext_trampoline(object.{member.name}, settings, indents + 1);')
                        self.write('    } else {')
                        self.write(f'        dump_json_value<const {typeInfo.templateType}>(object.{member.name}, object.{member.name}, settings, "{typeInfo.type}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')
                        self.write('    }')

                elif typeInfo.pointerLevels > 0:
                    if typeInfo.arrayLength is None:
                        self.write(f'    dump_json_pointer<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')

                    else:
                        if not typeInfo.lengthIsMember:
                            self.write(f'    dump_json_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')

                        elif typeInfo.lengthIsMember and member.name != 'pCode':
                            if typeInfo.arrayLength[0].isdigit() or typeInfo.arrayLength[0].isupper():
                                self.write(f'    dump_json_array<const {typeInfo.templateType}>(object.{member.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')

                            elif not typeInfo.arrayLength[0].isdigit() or typeInfo.arrayLength[0].isupper():
                                if typeInfo.arrayLength == 'rasterizationSamples':
                                    self.write(f'    dump_json_array<const {typeInfo.templateType}>(object.{member.name}, (object.{typeInfo.arrayLength} + 31) / 32, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')
                                else:
                                    if typeInfo.maxArrayLength is not None:
                                        self.write(f'    dump_json_array<const {typeInfo.templateType}>(object.{member.name}, std::min(object.{typeInfo.arrayLength}, {typeInfo.maxArrayLength}), settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')
                                    else:
                                        self.write(f'    dump_json_array<const {typeInfo.templateType}>(object.{member.name}, object.{typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')

                self.write('')
                if struct.name == 'VkShaderModuleCreateInfo':
                    if member.name == 'pCode':
                        self.write('    if(settings.showShader())')
                        self.write(f'        dump_json_array<const {member.type}>(object.{member.name}, object.{typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 1, dump_json_{typeInfo.id});')
                        self.write('    else')
                        self.write(f'        dump_json_special("SHADER DATA", settings, "{typeInfo.type}", "{member.name}", indents + 1);')

                self.write('')
                if typeInfo.validity_check is not None:
                    self.write('    else')
                    self.write(f'        dump_json_UNUSED(settings, "{typeInfo.type}", "{member.name}", indents + 1);')

            self.write('    settings.stream() << "\\n" << settings.indentation(indents) << "]";')
            self.write('}')
            if struct.protect:
                self.write(f'#endif // {struct.protect}')

        self.write('\n//========================== Union Implementations ==========================//\n')
        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_json_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    settings.stream() << settings.indentation(indents) << "[\\n";')
            for member in union.members:
                typeInfo = self.fillin_variable_typeinfo(member, union)

                if member != union.members[0] and typeInfo.validity_check is None:
                    self.write('    settings.stream() << ",\\n"; // Only need commas when more than one field is printed')

                if typeInfo.validity_check is not None:
                    self.write(f'    if({typeInfo.validity_check})')

                if typeInfo.pointerLevels == 0:
                    self.write(f'    dump_json_value<const {typeInfo.templateType}>(object.{member.name}, NULL, settings, "{typeInfo.type}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 2, dump_json_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is None:
                    self.write(f'    dump_json_pointer<const {typeInfo.templateType}>(object.{member.name}, settings, "{typeInfo.type}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 2, dump_json_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is not None:
                    self.write(f'    dump_json_array<const {typeInfo.templateType}>(object.{member.name}, {member.length}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{member.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, indents + 2, dump_json_{typeInfo.id});')

            self.write('    settings.stream() << "\\n" << settings.indentation(indents) << "]";')
            self.write('}')
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Implementation =======================//\n')
        if not video:
            self.write('void dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)')
            self.write('{')
            self.write('    VkBaseInStructure base_struct{};')
            self.write('    memcpy(&base_struct, object, sizeof(VkBaseInStructure));')
            self.write('    switch(base_struct.sType) {')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                if struct.protect:
                    self.write(f'#if defined({struct.protect})')
                if struct.sType is not None:
                    self.write(f'    case {struct.sType}:')
                    self.write(f'        dump_json_pNext<const {struct.name}>(reinterpret_cast<const {struct.name}*>(object), settings, "{struct.name}", indents, dump_json_{struct.name});')
                    self.write('        break;')
                if struct.protect:
                    self.write(f'#endif // {struct.protect}')
            self.write('')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48')
            self.write('        if(base_struct.pNext != nullptr){')
            self.write('            dump_json_pNext_trampoline(reinterpret_cast<const void*>(base_struct.pNext), settings, indents);')
            self.write('        } else {')
            self.write('            settings.stream() << settings.indentation(indents) << "{\\n";')
            self.write('            settings.stream() << settings.indentation(indents + 1) << "\\"type\\" : \\"const void*\\",\\n";')
            self.write('            settings.stream() << settings.indentation(indents + 1) << "\\"name\\" : \\"pNext\\",\\n";')
            self.write('            settings.stream() << settings.indentation(indents + 1) << "\\"value\\" : \\"NULL\\"\\n";')
            self.write('            settings.stream() << settings.indentation(indents) << "}";')
            self.write('        }')
            self.write('        break;')
            self.write('    default:')
            self.write('        settings.stream() << settings.indentation(indents) << "{\\n";')
            self.write('        settings.stream() << settings.indentation(indents + 1) << "\\"type\\" : \\"const void*\\",\\n";')
            self.write('        settings.stream() << settings.indentation(indents + 1) << "\\"name\\" : \\"pNext\\",\\n";')
            self.write('        settings.stream() << settings.indentation(indents + 1) << "\\"value\\" : \\"UNKNOWN (" << (int64_t) (base_struct.sType) << ")\\"\\n";')
            self.write('        settings.stream() << settings.indentation(indents) << "}";')
            self.write('    }')
            self.write('}')

        self.write('\n//========================= Function Helpers ================================//\n')

        self.write('// Display parameter values\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')

            self.write(f'void dump_json_params_{command.name}(ApiDumpInstance& dump_inst, {commandParameterDeclarationText(command)})')
            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());\n')

            self.write('    if(settings.showParams())')
            self.write('    {')
            self.write('        settings.stream() << settings.indentation(3) << "\\\"args\\\" :\\n";')
            self.write('        settings.stream() << settings.indentation(3) << "[\\n";')

            for param in command.params:
                if param != command.params[0]:
                    self.write('        settings.stream() << ",\\n";')

                typeInfo = self.fillin_variable_typeinfo(param, command)

                if typeInfo.parameter_state is not None:
                    self.write('        ' + typeInfo.parameter_state)

                if typeInfo.pointerLevels == 0:
                    self.write(f'        dump_json_value<const {typeInfo.templateType}>({param.name}, NULL, settings, "{typeInfo.type}", "{param.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, 4, dump_json_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is None:
                    self.write(f'        dump_json_pointer<const {typeInfo.templateType}>({param.name}, settings, "{typeInfo.type}", "{param.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, 4, dump_json_{typeInfo.id});')

                elif typeInfo.pointerLevels == 1 and typeInfo.arrayLength is not None:
                    self.write(f'        dump_json_array<const {typeInfo.templateType}>({param.name}, {typeInfo.arrayLength}, settings, "{typeInfo.type}", "{typeInfo.childType}", "{param.name}", {typeInfo.isStruct}, {typeInfo.isUnion}, 4, dump_json_{typeInfo.id});')

            self.write('        settings.stream() << "\\n" << settings.indentation(3) << "]\\n";')
            self.write('    }')
            self.write('    if (settings.shouldFlush()) settings.stream().flush();')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n//========================= Function Implementations ========================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            returnParam = f'{command.returnType} result, ' if command.returnType != 'void' else ''
            self.write(f'void dump_json_{command.name}(ApiDumpInstance& dump_inst, {returnParam}{commandParameterDeclarationText(command)})')

            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());')

            if command.returnType != 'void':
                self.write('    settings.stream() << settings.indentation(3) << "\\\"returnValue\\\" : ";')
                self.write(f'    dump_json_{command.returnType}(result, settings, 0);')
                self.write('    if(settings.showParams())')
                self.write('        settings.stream() << ",";')
                self.write('    settings.stream() << "\\n";')

            self.write(f'    dump_json_params_{command.name}(dump_inst, {commandParameterUsageText(command)});')
            self.write('    settings.stream() << settings.indentation(2) << "}";')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')
        self.write('\n')

    def buildAliasMap(self):
        for handle in self.vk.handles.values():
            for alias in handle.aliases:
                self.aliases[alias] = handle.name

        for command in self.vk.commands.values():
            if command.alias:
                self.aliases[command.alias] = command.name
            for param in command.params:
                if param.alias:
                    self.aliases[param.alias] = param.name

        for struct in self.vk.structs.values():
            for alias in struct.aliases:
                self.aliases[alias] = struct.name

        for enum in self.vk.enums.values():
            for alias in enum.aliases:
                self.aliases[alias] = enum.name

        for bitmask in self.vk.bitmasks.values():
            for alias in bitmask.aliases:
                self.aliases[alias] = bitmask.name

        for flag in self.vk.flags.values():
            for alias in flag.aliases:
                self.aliases[alias] = flag.name



    def fillin_variable_typeinfo(self, variable : Param | Member, parent : Struct | Command):
        typeInfo = VariableTypeInfo()
        typeInfo.id = variable.type
        if typeInfo.id in self.aliases:
            typeInfo.id = self.aliases[typeInfo.id]
        typeInfo.templateType = typeInfo.id

        text = variable.cDeclaration
        text_list = text.replace('[', ' [ ').replace(']', ' ] ').split()
        if variable.name in text_list:
            text_list = text_list[0:text_list.index(variable.name)] # remove all elements after the name
        typeInfo.type = ' '.join(text_list)
        bracketMatches = re.findall('(?<=\\[)[ a-zA-Z0-9_]+(?=\\])', text)
        for bracketMatch in bracketMatches:
            typeInfo.childType = typeInfo.type
            typeInfo.type += '[' + bracketMatch + ']'
            if typeInfo.arrayLength is None:
                typeInfo.arrayLength = bracketMatch
            else:
                typeInfo.arrayLength = typeInfo.arrayLength + '*' + bracketMatch

        lengthString = variable.length
        if lengthString is not None and lengthString == '(rasterizationSamples + 31) / 32':
            lengthString = 'rasterizationSamples'

        lengths = []
        if lengthString is not None:
            lengths = re.split(',', lengthString)
            lengths = list(filter(('null-terminated').__ne__, lengths))
        if typeInfo.arrayLength is None and len(lengths) > 0:
            typeInfo.childType = '*'.join(typeInfo.type.split('*')[0:-1])
            typeInfo.arrayLength = lengths[0]
            typeInfo.lengthIsMember = True

        # Dereference if necessary and handle members of variables
        if typeInfo.arrayLength is not None:
            typeInfo.arrayLength = re.sub('::', '->', typeInfo.arrayLength)
            sections = typeInfo.arrayLength.split('->')
            if sections[-1][0] == 'p' and sections[0][1].isupper():
                typeInfo.arrayLength = '*' + typeInfo.arrayLength

        typeInfo.pointerLevels = len(re.findall('\\*|\\[', text))
        if typeInfo.id == 'char' and typeInfo.pointerLevels > 0:
            typeInfo.templateType += '*'
            typeInfo.pointerLevels -= 1
            typeInfo.id = 'cstring'
        elif typeInfo.id in POINTER_TYPES:
            typeInfo.templateType += '*'
            typeInfo.pointerLevels -= 1
        assert(typeInfo.pointerLevels >= 0)

        if typeInfo.id in PARAMETER_STATE and parent.name in PARAMETER_STATE[typeInfo.id]:
            typeInfo.parameter_state = PARAMETER_STATE[typeInfo.id][parent.name]

        if parent.name in VALIDITY_CHECKS and variable.name in VALIDITY_CHECKS[parent.name]:
            if variable.noAutoValidity or (isinstance(parent, Struct) and parent.union):
                typeInfo.validity_check = VALIDITY_CHECKS[parent.name][variable.name]

        typeInfo.isStruct = "true" if variable.type in self.vk.structs and not self.vk.structs[variable.type].union else "false"
        typeInfo.isUnion = "true" if variable.type in self.vk.structs and self.vk.structs[variable.type].union else "false"

        if parent.name in ['VkPhysicalDeviceMemoryProperties','VkPhysicalDeviceGroupProperties', 'VkPhysicalDeviceMemoryBudgetPropertiesEXT', 'VkQueueFamilyGlobalPriorityPropertiesKHR']:
            if variable.name == 'memoryTypes':
                typeInfo.lengthIsMember = True
                typeInfo.arrayLength = 'memoryTypeCount'
                typeInfo.maxArrayLength = 'VK_MAX_MEMORY_TYPES'
            if variable.name == 'memoryHeaps':
                typeInfo.lengthIsMember = True
                typeInfo.arrayLength = 'memoryHeapCount'
                typeInfo.maxArrayLength = 'VK_MAX_MEMORY_HEAPS'
            if variable.name == 'physicalDevices':
                typeInfo.lengthIsMember = True
                typeInfo.arrayLength = 'physicalDeviceCount'
            if variable.name == 'heapBudget':
                typeInfo.lengthIsMember = True
                typeInfo.arrayLength = 'ApiDumpInstance::current().getMemoryHeapCount()'
            if variable.name == 'heapUsage':
                typeInfo.lengthIsMember = True
                typeInfo.arrayLength = 'ApiDumpInstance::current().getMemoryHeapCount()'
            if variable.name == 'priorities':
                typeInfo.lengthIsMember = True
                typeInfo.arrayLength = 'priorityCount'

        return typeInfo
