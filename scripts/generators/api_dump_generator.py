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

from base_generator import BaseGenerator
from vulkan_object import Struct, Command

POINTER_TYPES = ['xcb_connection_t', 'Display', 'SECURITY_ATTRIBUTES', 'ANativeWindow', 'AHardwareBuffer', 'wl_display', 'wl_surface', '_screen_context', '_screen_window', '_screen_buffer']

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

# Returns the command's parameters declaration. ex) uint32_t foo, float bar, char* baz
def commandParameterDeclarationText(command):
    return ', '.join(str.strip(p.cDeclaration) for p in command.params)

# Comma separate the names of the parameters. ex) foo, bar, baz
def commandParameterUsageText(command):
    return ', '.join(p.name for p in command.params)

def get_fulltype(var):
    custom_fullType = var.fullType
    if ':' in var.cDeclaration:
        custom_fullType += var.cDeclaration[var.cDeclaration.find(":"):]
    elif var.fixedSizeArray:
        custom_fullType += '[' + ']['.join(var.fixedSizeArray) + ']'
    return custom_fullType

def get_type(var):
    custom_type = var.type
    if var.const:
        if var.cDeclaration.count('*') >1:
            custom_type = 'const ' + custom_type + '*'
        else:
            custom_type = 'const ' + custom_type
    return custom_type

# Not everything that is functionally a pointer should be printed as such
def get_is_value_type(var):
    # Things which we define as 'pointer types' means we want to print as values
    # Must exclude double pointers here since void is in the POINTER_TYPES list
    if var.type in POINTER_TYPES and var.fullType.count('*') < 2:
        return True
    # If the type is a pointer or can be considered a fixed sized array, it isn't a value
    if not var.pointer and len(var.fixedSizeArray) == 0:
        return True
    # Special case string types or fixed length strings as values so we print them with string processing
    if var.fullType == 'const char*' or (len(var.fixedSizeArray) > 0 and var.type == 'char'):
        return True
    # Function poitners are considered poitners, but we want to print them as values
    if 'PFN_' in var.type:
        return True
    return False

def get_is_array(var):
    if var.length is None:
        return False
    # Things which we define as 'pointer types' means we want to print as values
    # Must exclude double pointers here since void is in the POINTER_TYPES list
    if (var.type == 'void' or var.type in POINTER_TYPES) and var.fullType.count('*') < 2:
        return False

    # If the type is a pointer or can be considered a fixed sized array, it isn't a value
    if not var.pointer and len(var.fixedSizeArray) == 0:
        return False

    # Special case string types or fixed length strings as values so we print them with string processing
    if var.fullType == 'const char*' or (len(var.fixedSizeArray) > 0 and var.type == 'char'):
        return False

    # Function poitners are considered poitners, but we want to print them as values
    if 'PFN_' in var.type:
        return False

    return True

def get_array_length(var, parent):
    lengthIsMember = False
    lengthIsPointer = False
    first_length = var.length.split(',')[0]
    for local in [x for x in (parent.params if isinstance(parent, Command) else parent.members)]:
        if local.name == first_length:
            lengthIsMember = True
            if local.pointer:
                lengthIsPointer = True
            break

    # Special case due to the length being non-constant non-local
    if parent.name == 'VkPhysicalDeviceMemoryBudgetPropertiesEXT' and var.name in ['heapBudget', 'heapUsage']:
        return 'ApiDumpInstance::current().getMemoryHeapCount()'

    # If the length is a number or an API Constant, just return it
    if not lengthIsMember:
        return '*'.join(var.length.split(','))

    # While the array might have a fixed size in memory, we want to use the 'real' length. But, if that length hasn't been initialized, we need to cap it to the fixed size length
    if len(var.fixedSizeArray) > 0:
        return f'std::min(object.{var.length}, {"*".join(var.fixedSizeArray)})'

    # If the length is a pointer, we need to dereference it
    deref = '*' if lengthIsPointer else ''

    # If the variable is from a struct, we need to use `object.` to access it.
    if isinstance(parent, Struct):
        return deref + 'object.' + '*'.join(var.length.split(','))
    else:
        return deref + '*'.join(var.length.split(','))

def get_fixed_array_length(fixed_length, var, parent):
    lengthIsMember = False
    for local in [x for x in (parent.params if isinstance(parent, Command) else parent.members)]:
        if local.name == var.length:
            lengthIsMember = True
            break

    # Special case due to the length being non-constant non-local
    if parent.name == 'VkPhysicalDeviceMemoryBudgetPropertiesEXT' and var.name in ['heapBudget', 'heapUsage']:
        return f'std::min(ApiDumpInstance::current().getMemoryHeapCount(), {fixed_length})'

    if lengthIsMember:
        object_access = 'object.' if isinstance(parent, Struct) else ''
        return f'std::min({object_access}{var.length}, {fixed_length})'
    else:
        return fixed_length

class ApiDumpGenerator(BaseGenerator):
    def __init__(self):
        BaseGenerator.__init__(self)
        self.aliases = {}
        self.return_types = set()

    def generate(self):
        self.getReturnTypes()
        self.buildAliasMap()
        self.generate_copyright()
        if self.filename == 'api_dump.cpp':
            self.generate_dispatch_codegen()
        elif self.filename == 'api_dump_text.h':
            self.generate_header(output_format='text')
        elif self.filename == 'api_dump_text.cpp':
            self.generate_implementation('text', video=False)
        elif self.filename == 'api_dump_html.h':
            self.generate_header(output_format='html')
        elif self.filename == 'api_dump_html.cpp':
            self.generate_implementation('html', video=False)
        elif self.filename == 'api_dump_json.h':
            self.generate_header(output_format='json')
        elif self.filename == 'api_dump_json.cpp':
            self.generate_implementation('json', video=False)
        elif self.filename == 'api_dump_video_text.h':
            self.generate_implementation('text', video=True)
        elif self.filename == 'api_dump_video_html.h':
            self.generate_implementation('html', video=True)
        elif self.filename == 'api_dump_video_json.h':
            self.generate_implementation('json', video=True)

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

    def generate_implementation(self, output_format, video=False):
        if video:
            self.write('#pragma once\n')

        self.write(f'#include "api_dump_{output_format}.h"')
        if not video:
            self.write(f'\n#include "api_dump_video_{output_format}.h"')

        self.write('\n//=========================== Type Implementations ==========================//\n')

        for t in [x for x in self.vk.platformTypes.values() if x.requires in ['vk_platform', 'stdint']]:
            if t.name in self.return_types:
                self.write(f'void dump_{output_format}_return_value_{t.name}(const {t.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_platform_type_contents(output_format, t)
                self.write('}')
            if t.name in ['void', 'char'] or (not video and t.name in DUPLICATE_TYPES_IN_VIDEO_HEADER):
                continue
            self.write(f'void dump_{output_format}_{t.name}(const {t.name} &object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            if t.name in self.return_types:
                self.write(f'    dump_{output_format}_return_value_{t.name}(object, settings, indents);')
            else:
                self.write_platform_type_contents(output_format, t)
            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            if '*' not in t.name:
                self.write_pointer_overload(output_format, t.name)


        self.write('\n//========================= Basetype Implementations ========================//\n')

        for basetype in self.vk.baseTypes.values():
            if basetype.name in self.return_types:
                self.write(f'void dump_{output_format}_return_value_{basetype.name}(const {basetype.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_basetype_contents(output_format)
                self.write('}')

            if basetype.protect:
                self.write(f'#if defined({basetype.protect})')
            self.write(f'void dump_{output_format}_{basetype.name}(const {basetype.name}{"*" if basetype.name in POINTER_TYPES else "&"} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            if basetype.name in self.return_types:
                self.write(f'    dump_{output_format}_return_value_{basetype.name}(object, settings, indents);')
            else:
                self.write_basetype_contents(output_format)
            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            self.write_pointer_overload(output_format, basetype.name)
            if basetype.protect:
                self.write(f'#endif // {basetype.protect}')

        self.write('\n//======================= System Type Implementations =======================//\n')

        sortedSystemTypes = dict(sorted(self.vk.platformTypes.items()))
        for sys in sortedSystemTypes.values():
            if (sys.requires is not None and (sys.requires in EXCLUDED_INCLUDE_LIST or 'vk_video' in sys.requires)) or (video and sys.name in DUPLICATE_TYPES_IN_VIDEO_HEADER):
                continue
            if sys.protect:
                self.write(f'#if defined({sys.protect})')
            if sys.name in self.return_types:
                self.write(f'void dump_{output_format}_return_value_{sys.name}(const {sys.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_system_type_contents(output_format, sys)
                self.write('}')
            self.write(f'void dump_{output_format}_{sys.name}(const {sys.name}{"*" if sys.name in POINTER_TYPES else "&"} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents{", object" if sys.name in POINTER_TYPES else ""});')
            if sys.name in self.return_types:
                self.write(f'    dump_{output_format}_return_value_{sys.name}(object, settings, indents);')
            else:
                self.write_system_type_contents(output_format, sys)
            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            if sys.name not in POINTER_TYPES:
                self.write_pointer_overload(output_format, sys.name)
            if sys.protect:
                self.write(f'#endif // {sys.protect}')

        if not video:
            self.write('\n//========================== Handle Implementations =========================//\n')
            for handle in self.vk.handles.values():
                if handle.protect:
                    self.write(f'#if defined({handle.protect})')
                self.write(f'void dump_{output_format}_{handle.name}(const {handle.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
                self.write('{')
                self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
                if output_format in ['text', 'html']:
                    self.write('    if(settings.showAddress()) {')
                    self.write('        std::unordered_map<uint64_t, std::string>::const_iterator it = ApiDumpInstance::current().object_name_map.find((uint64_t) object);')
                    self.write('        if (it != ApiDumpInstance::current().object_name_map.end()) {')
                    self.write(f'            dump_{output_format}_value(settings, object, " [", it->second, "]");')
                    self.write('        } else {')
                    self.write(f'            dump_{output_format}_value(settings, object);')
                    self.write('        }')
                    self.write('    } else {')
                    self.write(f'       dump_{output_format}_value(settings, "address");')
                    self.write('    }')
                if output_format == 'json':
                    self.write('    dump_json_handle(settings, object);')
                self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
                self.write('}')
                self.write_pointer_overload(output_format, handle.name)
                if handle.protect:
                    self.write(f'#endif // {handle.protect}')

        self.write('\n//=========================== Enum Implementations ==========================//\n')
        for enum in self.vk.enums.values():
            if enum.protect:
                self.write(f'#if defined({enum.protect})')
            if enum.name in self.return_types:
                self.write(f'void dump_{output_format}_return_value_{enum.name}(const {enum.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_enum_contents(output_format, enum)
                self.write('}')
            self.write(f'void dump_{output_format}_{enum.name}(const {enum.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            if enum.name in self.return_types:
                self.write(f'    dump_{output_format}_return_value_{enum.name}(object, settings, indents);')
            else:
                self.write_enum_contents(output_format, enum)
            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            self.write_pointer_overload(output_format, enum.name)
            if enum.protect:
                self.write(f'#endif // {enum.protect}')

        self.write('\n//========================= Bitmask Implementations =========================//\n')
        for bitmask in self.vk.bitmasks.values():
            if bitmask.protect:
                self.write(f'#if defined({bitmask.protect})')
            if output_format == 'text':
                if bitmask.bitWidth == 64:
                    self.write('// 64 bit bitmasks don\'t have an enum of bit values.')
                    self.write(f'typedef VkFlags64 {bitmask.name};')
            self.write(f'void dump_{output_format}_{bitmask.name}(const {bitmask.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            self.write_bitmask_constents(output_format, bitmask)
            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            self.write_pointer_overload(output_format, bitmask.name)
            if bitmask.protect:
                self.write(f'#endif // {bitmask.protect}')

        self.write('\n//=========================== Flag Implementations ==========================//\n')

        for flag in self.vk.flags.values():
            if flag.protect:
                self.write(f'#if defined({flag.protect})')
            self.write(f'void dump_{output_format}_{flag.name}(const {flag.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr) {{')
            if flag.bitmaskName is not None:
                self.write(f'    dump_{output_format}_{flag.bitmaskName}(static_cast<{flag.bitmaskName}>(object), settings, type_name, var_name, indents, address);')
            else:
                self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
                self.write(f'    dump_{output_format}_value(settings, object);')
                self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            self.write_pointer_overload(output_format, flag.name)
            if flag.protect:
                self.write(f'#endif // {flag.protect}')

        self.write('\n//======================= Func Pointer Implementations ======================//\n')

        for funcpointer in self.vk.funcPointers.values():
            self.write(f'void dump_{output_format}_{funcpointer.name}(const {funcpointer.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            self.write(f'    dump_{output_format}_handle(settings, object);')
            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('}')
            self.write_pointer_overload(output_format, funcpointer.name)

        self.write('\n//======================== Union Forward Declarations =======================//\n')

        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_{output_format}_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr);')
            self.write(f'void dump_{output_format}_{union.name}(const {union.name}* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents);')
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Declarations =======================//\n')
        if not video:
            if output_format == 'text':
                self.write(f'void dump_{output_format}_pNext_struct_name(const void* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents);')
            self.write(f'void dump_{output_format}_pNext_trampoline(const void* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents);')

        self.write('\n//========================== Struct Implementations =========================//\n')

        for struct in [ x for x in self.vk.structs.values() if not x.union ]:
            if struct.protect:
                self.write(f'#if defined({struct.protect})')

            # Rare recursive structure, needs pointer type declaration to work properly
            if struct.name in ['VkBaseInStructure', 'VkBaseOutStructure']:
                self.write(f'void dump_{output_format}_{struct.name}(const {struct.name}* object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents);')

            self.write(f'void dump_{output_format}_{struct.name}(const {struct.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);')

            for member in struct.members:
                validity_check = self.get_validity_check(member, struct)
                parameter_state = self.get_parameter_state(member, struct)
                if parameter_state is not None:
                    if validity_check is not None:
                        self.write(f'    if ({validity_check})')
                        self.write('        ' + parameter_state)
                    else:
                        self.write('    ' + parameter_state)

            self.write('')
            json_indent = '2' if output_format == 'json' else '1'
            for member in struct.members:
                custom_fullType = get_fulltype(member)
                validity_check = self.get_validity_check(member, struct)
                if output_format == 'json':
                    if member != struct.members[0]:
                        self.write('    dump_json_comma_and_newline(settings);')

                if validity_check is not None:
                    self.write(f'    if({validity_check}) {{')

                self.write_value(output_format, member, struct)

                self.write('')
                if validity_check is not None:
                    self.write('    } else {')
                    if output_format in ['text', 'html']:
                        self.write(f'        dump_{output_format}_special("UNUSED", settings, "{custom_fullType}", "{member.name}", indents + {json_indent});')
                    if output_format == 'json':
                        self.write(f'        dump_{output_format}_UNUSED(settings, "{custom_fullType}", "{member.name}", indents + {json_indent});')
                    self.write('    }')

            self.write('')
            if output_format == 'text':
                for member in struct.members:
                    if member.pointer and member.name == 'pNext' and struct.name not in ['VkBaseInStructure', 'VkBaseOutStructure']:
                        self.write(f'    dump_text_pNext_trampoline(object.pNext, settings, "{member.fullType}", "{member.type}", indents < 2 ? indents + {json_indent} : indents);')

            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::api_struct, indents);')
            self.write('}')
            self.write_pointer_overload(output_format, struct.name)
            if struct.protect:
                self.write(f'#endif // {struct.protect}')

        self.write('\n//========================== Union Implementations ==========================//\n')

        for union in [ x for x in self.vk.structs.values() if x.union ]:
            if union.protect:
                self.write(f'#if defined({union.protect})')
            self.write(f'void dump_{output_format}_{union.name}(const {union.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address)')
            self.write('{')
            self.write(f'    dump_{output_format}_start(settings, OutputConstruct::api_union, type_name, var_name, indents, address);')

            for member in union.members:
                validity_check = self.get_validity_check(member, union)
                if output_format == 'json':
                    if member != union.members[0] and validity_check is None:
                        self.write('    dump_json_comma_and_newline(settings);')

                if validity_check is not None:
                    self.write(f'    if({validity_check}) {{')

                self.write_value(output_format, member, union)

                if validity_check is not None:
                    self.write('    }')

            self.write(f'    dump_{output_format}_end(settings, OutputConstruct::api_union, indents);')

            self.write('}')
            self.write_pointer_overload(output_format, union.name)
            if union.protect:
                self.write(f'#endif // {union.protect}')

        self.write('\n//======================== pNext Chain Implementation =======================//\n')
        if not video:
            if output_format == 'text':
                self.write('void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents)')
                self.write('{')
                self.write('    if (object == nullptr) {')
                self.write('        dump_text_nullptr(settings, type_name, var_name, indents);')
                self.write('        return;')
                self.write('    }\n')
                self.write('    switch(reinterpret_cast<const VkBaseInStructure*>(object)->sType) {')
                for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                    if struct.protect:
                        self.write(f'#if defined({struct.protect})')
                    if struct.sType is not None:
                        self.write(f'        case {struct.sType}:')
                        self.write(f'            dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents);')
                        self.write(f'            dump_{output_format}_value(settings, "{struct.name}");')
                        self.write(f'            dump_{output_format}_end(settings, OutputConstruct::value, indents);')
                        self.write('            break;')
                    if struct.protect:
                        self.write(f'#endif // {struct.protect}')
                self.write('        case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47')
                self.write('        case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48')
                self.write('        default:')
                self.write(f'            dump_{output_format}_start(settings, OutputConstruct::value, type_name, var_name, indents);')
                self.write(f'            dump_{output_format}_value(settings, "NULL");')
                self.write(f'            dump_{output_format}_end(settings, OutputConstruct::value, indents);')
                self.write('            break;')
                self.write('    }')
                self.write('}\n')

            self.write(f'void dump_{output_format}_pNext_trampoline(const void* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents)')
            self.write('{')
            self.write('    if (object == NULL) {')
            if output_format in ['html', 'json']:
                self.write(f'        dump_{output_format}_nullptr(settings, type_name, var_name, indents);')
            self.write('        return;')
            self.write('    }')
            self.write('    VkBaseInStructure base_struct{};')
            self.write('    memcpy(&base_struct, object, sizeof(VkBaseInStructure));')
            self.write('    switch(base_struct.sType) {')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                if struct.protect:
                    self.write(f'#if defined({struct.protect})')
                if struct.sType is not None:
                    self.write(f'    case {struct.sType}:')
                    self.write(f'        dump_{output_format}_{struct.name}(*reinterpret_cast<const {struct.name}*>(object), settings, "{struct.name}{"*" if output_format == "json" else ""}", "pNext", indents, reinterpret_cast<const {struct.name}*>(object));')
                    self.write('        break;')
                if struct.protect:
                    self.write(f'#endif // {struct.protect}')
            self.write('')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47')
            self.write('    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48')
            self.write('        if(base_struct.pNext != nullptr){')
            self.write(f'            dump_{output_format}_pNext_trampoline(reinterpret_cast<const void*>(base_struct.pNext), settings, type_name, var_name, indents);')
            self.write('        } else {')
            self.write(f'            dump_{output_format}_nullptr(settings, "const void*", "pNext", indents);')
            self.write('        }')
            self.write('        break;')
            self.write('    default:')
            self.write(f'        dump_{output_format}_start(settings, OutputConstruct::value, "const void*", "pNext", indents);')
            self.write(f'        dump_{output_format}_value(settings, "UNKNOWN (", (int64_t) (base_struct.sType), ")");')
            self.write(f'        dump_{output_format}_end(settings, OutputConstruct::value, indents);')
            self.write('    }')
            self.write('}')

        self.write('\n//========================== Function Helpers ===============================//\n')

        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            self.write(f'void dump_{output_format}_params_{command.name}(ApiDumpInstance& dump_inst, {commandParameterDeclarationText(command)})')
            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());')
            self.write('    if(settings.showParams())')
            self.write('    {')
            if output_format == 'json':
                self.write('        settings.stream() << settings.indentation(3) << "\\\"args\\\" :\\n";')
                self.write('        settings.stream() << settings.indentation(3) << "[\\n";')
            for param in command.params:
                if output_format == 'json':
                    if param != command.params[0]:
                        self.write('        dump_json_comma_and_newline(settings);')

                parameter_state = self.get_parameter_state(param, command)
                if parameter_state is not None:
                    self.write('        ' + parameter_state)

                self.write_value(output_format, param, command)

            if output_format == 'json':
                self.write('        settings.stream() << "\\n" << settings.indentation(3) << "]\\n";')
            if output_format in ['text', 'html']:
                self.write('        settings.stream() << "\\n";')
            self.write('        flush(settings);')
            self.write('    }')
            self.write('}')
            if command.protect:
                self.write(f'#endif // {command.protect}')

        self.write('\n//========================= Function Implementations ========================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            if command.protect:
                self.write(f'#if defined({command.protect})')
            returnParam = f'{command.returnType} result, ' if command.returnType != 'void' else ''
            self.write(f'void dump_{output_format}_{command.name}(ApiDumpInstance& dump_inst, {returnParam}{commandParameterDeclarationText(command)})')

            self.write('{')
            self.write('    const ApiDumpSettings& settings(dump_inst.settings());')
            if command.returnType != 'void':
                call_type = self.get_unaliased_type(command.returnType)
                if output_format == 'text':
                    self.write('    settings.stream() << " ";')
                if output_format == 'json':
                    self.write('    settings.stream() << settings.indentation(3) << "\\\"returnValue\\\" :";')
                self.write(f'    dump_{output_format}_return_value_{call_type}(result, settings, 0);')
                if output_format == 'json':
                    self.write('    if(settings.showParams())')
                    self.write('        settings.stream() << ",";')
                    self.write('    settings.stream() << "\\n";')
            if output_format == 'text':
                self.write('    settings.stream() << ":\\n";')
            if output_format == 'html':
                self.write('    settings.stream() << "</summary>";')

            self.write(f'    dump_{output_format}_params_{command.name}(dump_inst, {commandParameterUsageText(command)});')
            if output_format == 'html':
                self.write('\n    settings.stream() << "</details>";')
            if output_format == 'json':
                self.write('    settings.stream() << settings.indentation(2) << "}";')
            self.write('    flush(settings);')
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

    def getReturnTypes(self):
        for command in self.vk.commands.values():
            if command.returnType != 'void' :
                self.return_types.add(command.returnType)

    def get_unaliased_type(self, type_to_check):
        if type_to_check in self.aliases and self.aliases[type_to_check] is not None:
            return self.aliases[type_to_check]
        return type_to_check

    def get_parameter_state(self, var, parent):
        typeID = self.get_unaliased_type(var.type)
        if typeID in PARAMETER_STATE and parent.name in PARAMETER_STATE[typeID]:
            return PARAMETER_STATE[typeID][parent.name]
        return None

    def get_validity_check(self, var, parent):
        if parent.name in VALIDITY_CHECKS and var.name in VALIDITY_CHECKS[parent.name]:
            if var.noAutoValidity or (isinstance(parent, Struct) and parent.union):
                return VALIDITY_CHECKS[parent.name][var.name]
        return None

    def get_kind_of_value(self,var):
        if var.type in self.vk.structs:
            return 'union' if self.vk.structs[var.type].union else 'struct'
        return 'value'

    def write_value(self, output_format, var, parent):
        custom_fullType = get_fulltype(var)
        custom_type = get_type(var)
        object_access = 'object.' if isinstance(parent, Struct) else ''
        call_type = self.get_unaliased_type(var.type)
        indent_plus = '2' if output_format == 'json' else '1'
        pi = ''
        if isinstance(parent, Command):
            pi = '    '
            indent = 4 if output_format == 'json' else 1
        else:
            indent = f'indents + {"2" if output_format == "json" else "1"}'

        if var.name == 'apiVersion':
            call_type = 'api_version'

        if get_is_array(var):
            array_len = get_array_length(var, parent)
            array_ptr = f'{object_access}{var.name}'
            if parent.name == 'VkPipelineMultisampleStateCreateInfo' and var.length == '(rasterizationSamples + 31) / 32':
                array_len = '(object.rasterizationSamples + 31) / 32'

            if parent.name == 'VkShaderModuleCreateInfo' and var.name == 'pCode':
                array_len = f'{object_access}{var.length}'
                self.write('    if(settings.showShader()) {')
                pi = '    ' + pi

            not_null_check = f'{object_access}{var.name} != NULL' if var.pointer else 'true'
            if var.pointer:
                self.write(pi + f'    if ({array_len} > 0 && {not_null_check}) {{')
                pi = '    ' + pi

            self.write(pi + f'    dump_{output_format}_array_start({array_ptr}, {array_len}, settings, "{custom_fullType}", "{var.name}", {indent});')

            if len(var.fixedSizeArray) == 2:
                self.write(pi + f'    for (size_t i = 0; i < {var.fixedSizeArray[0]}; ++i) {{')
                self.write(pi + f'      for (size_t j = 0; j < {var.fixedSizeArray[1]}; ++j) {{')
                self.write(pi + '        std::stringstream stream;')
                self.write(pi + f'       stream << "{var.name if output_format != "json" else ""}" << "[" << i << "][" << j << "]";')
                self.write(pi + '        std::string indexName = stream.str();')
                self.write(pi + f'        dump_{output_format}_{call_type}({object_access}{var.name}[i][j], settings, "{custom_type}", indexName.c_str(), {indent} + {indent_plus});')
                if output_format == 'json':
                    self.write(pi + f'        if (i < {var.fixedSizeArray[1]} - 1 && j < {var.fixedSizeArray[0]} - 1) settings.stream() << \',\';')
                    self.write(pi + '    settings.stream() << "\\n";')
                self.write(pi + '      }')
                self.write(pi + '    }')
            elif len(var.fixedSizeArray) == 1:
                fixed_array_len = get_fixed_array_length(var.fixedSizeArray[0], var, parent)
                self.write(pi + f'    for (size_t i = 0; i < {fixed_array_len}; ++i) {{')
                self.write(pi + '        std::stringstream stream;')
                self.write(pi + f'        stream << "{var.name if output_format != "json" else ""}" <<"[" << i << "]";')
                self.write(pi + '        std::string indexName = stream.str();')
                self.write(pi + f'        dump_{output_format}_{call_type}({object_access}{var.name}[i], settings, "{custom_type}", indexName.c_str(), {indent} + {indent_plus});')
                if output_format == 'json':
                    self.write(pi + f'          if (i < {fixed_array_len} - 1) settings.stream() << \',\';')
                    self.write(pi + '    settings.stream() << "\\n";')
                self.write(pi + '    }')
            else:
                elem = f'{object_access}{var.name}[i]'
                if var.fullType.count('*') > 1 and var.type != 'void' and var.type != 'char':
                    elem = f'*{object_access}{var.name}[i]'

                self.write(pi + f'    for (size_t i = 0; i < {array_len}; ++i) {{')
                self.write(pi + '        std::stringstream stream;')
                self.write(pi + f'        stream << "{var.name if output_format != "json" else ""}" << "[" << i << "]";')
                self.write(pi + '        std::string indexName = stream.str();')
                self.write(pi + f'        dump_{output_format}_{call_type}({elem}, settings, "{custom_type}", indexName.c_str(), {indent} + {indent_plus});')
                if output_format == 'json':
                    self.write(pi + f'        if (i < {array_len} - 1) settings.stream() << \',\';')
                    self.write(pi + '        settings.stream() << "\\n";')
                self.write(pi + '    }')

            self.write(pi + f'    dump_{output_format}_array_end({array_ptr}, {array_len}, settings, {indent});')
            if var.pointer:
                pi = pi[4:]
                self.write(pi + '    } else {')
                self.write(pi + f'        dump_{output_format}_nullptr(settings, "{custom_fullType}", "{var.name}", {indent});')
                self.write(pi + '    }')
            if parent.name == 'VkShaderModuleCreateInfo' and var.name == 'pCode':
                self.write('    } else {')
                self.write(f'        dump_{output_format}_special("SHADER DATA", settings, "{var.fullType}", "{var.name}", {indent});')
                self.write('    }')
        else:
            if var.name == 'pNext' and var.fullType in ['void*', 'const void*']:
                if output_format == 'text':
                    self.write(f'    dump_{output_format}_pNext_struct_name(object.{var.name}, settings, "{var.fullType}", "{var.name}", {indent});')
                elif output_format in ['html', 'json']:
                    self.write(f'    dump_{output_format}_pNext_trampoline(object.{var.name}, settings, "{var.fullType}", "{var.name}", {indent});')
            else:
                self.write(pi + f'    dump_{output_format}_{call_type}({object_access}{var.name}, settings, "{custom_fullType}", "{var.name}", {indent});')


    def write_platform_type_contents(self, output_format, t):
        cast = ''
        if t.name == 'uint8_t':
            cast = '(uint32_t) '
        if t.name == 'int8_t':
            cast = '(int32_t) '
        self.write(f'    dump_{output_format}_value(settings, {cast}object);')

    def write_basetype_contents(self, output_format):
        self.write(f'    dump_{output_format}_value(settings, object);')

    def write_system_type_contents(self, output_format, sys):
        if '*' in sys.name:
            self.write(f'    dump_{output_format}_address(settings, &object);')
            if output_format == 'json':
                self.write('    settings.stream() << "\\n";')
        else:
            self.write(f'    dump_{output_format}_handle(settings, object);')

    def write_enum_contents(self, output_format, enum):
        self.write('    switch((int64_t) object)')
        self.write('    {')
        for field in enum.fields:
            self.write(f'    case {field.valueStr}:')
            if output_format in ['text', 'html']:
                self.write(f'        dump_{output_format}_value(settings, "{field.name} (", object, ")");')
            if output_format == 'json':
                self.write(f'        dump_{output_format}_value(settings, "{field.name}");')
            self.write('        break;')
        self.write('    default:')
        self.write(f'        dump_{output_format}_value(settings, "UNKNOWN (", object, ")");')
        self.write('    }')

    def write_bitmask_constents(self, output_format, bitmask):
        self.write(f'    dump_{output_format}_value_start(settings);')
        self.write('    settings.stream() << object;')
        self.write('    bool is_first = true;')
        for field in bitmask.flags:
            self.write(f'    if(object {"==" if  field.zero or field.multiBit else "&"} {field.valueStr if field.multiBit else field.value}) {{')
            self.write(f'        settings.stream() << (is_first ? \" (\" : \" | \") << "{field.name}"; is_first = false;')
            self.write('    }')
        self.write('    if(!is_first)')
        self.write('        settings.stream() << ")";')
        self.write(f'    dump_{output_format}_value_end(settings);')

    def write_pointer_overload(self, output_format, object_type):
        # Workaround for some types needing a double pointer overload function
        const = 'const '
        double_ptr = ''
        if object_type in ['AHardwareBuffer', 'ANativeWindow']:
            const = ''
            double_ptr = '*'

        self.write(f'void dump_{output_format}_{object_type}({const}{object_type}{double_ptr}* object, const ApiDumpSettings &settings, const char *type_string, const char *name, int indents){{')
        self.write('    if (object == NULL) {')
        self.write(f'        dump_{output_format}_nullptr(settings, type_string, name, indents);')
        self.write('    } else {')
        self.write(f'        dump_{output_format}_{object_type}(*object, settings, type_string, name, indents, object);')
        self.write('    }')
        self.write('}')
