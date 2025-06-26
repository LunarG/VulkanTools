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

HANDWRITTEN_FUNCTIONS = ['vkCreateInstance',
                         'vkCreateDevice',
                         'vkEnumerateInstanceExtensionProperties',
                         'vkEnumerateInstanceLayerProperties',
                         'vkEnumerateInstanceVersion',
                         'vkEnumerateDeviceLayerProperties',
                         'vkEnumerateDeviceExtensionProperties',
                         'vkGetInstanceProcAddr',
                         'vkGetDeviceProcAddr']

BLOCKING_API_CALLS = [
    'vkWaitForFences', 'vkWaitSemaphores', 'vkQueuePresentKHR', 'vkDeviceWaitIdle',
    'vkQueueWaitIdle', 'vkAcquireNextImageKHR', 'vkGetQueryPoolResults', 'vkWaitSemaphoresKHR'
]

EXCLUDED_INCLUDE_LIST = [
    'vk_platform',
    'stdint.h',
]

FUNCTION_IMPLEMENTATION_IGNORE_LIST = ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr', 'vkEnumerateInstanceVersion']

class PlatformGuardHelper():
    """Used to elide platform guards together, so redundant #endif then #ifdefs are removed
    Note - be sure to call add_guard(None) when done to add a trailing #endif if needed
    """
    def __init__(self):
        self.current_guard = None

    def add_guard(self, generator, guard):
        if self.current_guard != guard and self.current_guard is not None:
            generator.write(f'#endif  // {self.current_guard}')
        if self.current_guard != guard and guard is not None:
            generator.write(f'#if defined({guard})')
        self.current_guard = guard

# Returns the command's parameters declaration. ex) uint32_t foo, float bar, char* baz
def command_param_declaration_text(command : Command):
    return ', '.join(str.strip(p.cDeclaration) for p in command.params)

# Comma separate the names of the parameters. ex) foo, bar, baz
def command_param_usage_text(command : Command):
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
        self.get_return_types()
        self.build_alias_map()
        self.generate_copyright()
        if self.filename == 'api_dump_dispatch.cpp':
            self.generate_dispatch_codegen()
        elif self.filename == 'api_dump_implementation.h':
            self.generate_implementation()
        elif self.filename == 'api_dump_video_implementation.h':
            self.generate_implementation(video=True)


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

        self.write('''#include "api_dump_implementation.h"

            // Autogen instance functions
            ''')

        protect = PlatformGuardHelper()
        for command in  [x for x in self.vk.commands.values() if x.instance]:
            if command.name in HANDWRITTEN_FUNCTIONS:
                continue
            protect.add_guard(self, command.protect)
            self.write(f'VKAPI_ATTR {command.returnType} VKAPI_CALL {command.name}({command_param_declaration_text(command)})')
            self.write('{')
            if command.name not in BLOCKING_API_CALLS:
                self.write(f'''
                    std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());
                    dump_function_head(ApiDumpInstance::current(), "{command.name}", "{command_param_usage_text(command)}", "{command.returnType}");
                    if(ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text && ApiDumpInstance::current().shouldDumpOutput()) {{
                        dump_before_pre_dump_formatting<ApiDumpFormat::Text>(ApiDumpInstance::current().settings());
                        dump_params_{command.name}<ApiDumpFormat::Text>(ApiDumpInstance::current(), {command_param_usage_text(command)});
                    }}''')

            if command.name == 'vkGetPhysicalDeviceToolPropertiesEXT':
                self.write('''
                    static const VkPhysicalDeviceToolPropertiesEXT api_dump_layer_tool_props = {
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
                self.write('auto dispatch_key = get_dispatch_key(instance);')

            return_str = f'{command.returnType} result = ' if command.returnType != 'void' else ''
            self.write(f'{return_str}instance_dispatch_table({command.params[0].name})->{command.name[2:]}({command_param_usage_text(command)});')
            if command.name in BLOCKING_API_CALLS:
                self.write('std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());')
                self.write(f'dump_function_head(ApiDumpInstance::current(), "{command.name}", "{command_param_usage_text(command)}", "{command.returnType}");')

            if command.name in TRACKED_STATE:
                self.write(TRACKED_STATE[command.name])

            if command.name == 'vkEnumeratePhysicalDevices':
                self.write('''
                    if (pPhysicalDeviceCount != nullptr && pPhysicalDevices != nullptr) {
                        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {
                            ApiDumpInstance::current().set_vk_instance(pPhysicalDevices[i], instance);
                        }
                    }''')

            if command.name == 'vkDestroyInstance':
                self.write('destroy_instance_dispatch_table(dispatch_key);')

            if command.name == 'vkGetPhysicalDeviceToolPropertiesEXT':
                self.write('if (original_pToolProperties != nullptr) {')
                self.write('pToolProperties = original_pToolProperties;')
                self.write('}\n')
                self.write('(*pToolCount)++;')

            self.write(f'''if (ApiDumpInstance::current().shouldDumpOutput()) {{
                switch(ApiDumpInstance::current().settings().format())
                {{
                    case ApiDumpFormat::Text:
                        dump_{command.name}<ApiDumpFormat::Text>(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{', '.join(p.name for p in command.params)});
                        break;
                    case ApiDumpFormat::Html:
                        dump_{command.name}<ApiDumpFormat::Html>(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{', '.join(p.name for p in command.params)});
                        break;
                    case ApiDumpFormat::Json:
                        dump_{command.name}<ApiDumpFormat::Json>(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{', '.join(p.name for p in command.params)});
                        break;
                }}
            }}''')
            if command.returnType != 'void':
                self.write('return result;')
            self.write('}')
        protect.add_guard(self, None)

        self.write('\n// Autogen device functions\n')

        for command in [x for x in self.vk.commands.values() if x.device and x.name not in HANDWRITTEN_FUNCTIONS]:
            if command.name in ['vkGetDeviceProcAddr']:
                continue

            protect.add_guard(self, command.protect)

            self.write(f'VKAPI_ATTR {command.returnType} VKAPI_CALL {command.name}({command_param_declaration_text(command)})')
            self.write('{')

            if command.name not in BLOCKING_API_CALLS:
                self.write('std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());')
                if command.name in ['vkDebugMarkerSetObjectNameEXT', 'vkSetDebugUtilsObjectNameEXT']:
                    self.write('ApiDumpInstance::current().update_object_name_map(pNameInfo);')
                self.write(f'''
                    dump_function_head(ApiDumpInstance::current(), "{command.name}", "{command_param_usage_text(command)}", "{command.returnType}");
                    if(ApiDumpInstance::current().settings().shouldPreDump() && ApiDumpInstance::current().settings().format() == ApiDumpFormat::Text && ApiDumpInstance::current().shouldDumpOutput()) {{
                        dump_before_pre_dump_formatting<ApiDumpFormat::Text>(ApiDumpInstance::current().settings());
                        dump_params_{command.name}<ApiDumpFormat::Text>(ApiDumpInstance::current(), {command_param_usage_text(command)});
                    }}''')

            return_str = f'{command.returnType} result = ' if command.returnType != 'void' else ''
            self.write(f'{return_str}device_dispatch_table({command.params[0].name})->{command.name[2:]}({command_param_usage_text(command)});')
            if command.name in BLOCKING_API_CALLS:
                self.write('std::lock_guard<std::mutex> lg(ApiDumpInstance::current().outputMutex());')
                self.write(f'dump_function_head(ApiDumpInstance::current(), "{command.name}", "{command_param_usage_text(command)}", "{command.returnType}");')

            if command.name in TRACKED_STATE:
                self.write('' + TRACKED_STATE[command.name])

            if command.name == 'vkDestroyDevice':
                self.write('destroy_device_dispatch_table(get_dispatch_key(device));')

            self.write(f'''if (ApiDumpInstance::current().shouldDumpOutput()) {{
                switch(ApiDumpInstance::current().settings().format())
                {{
                    case ApiDumpFormat::Text:
                        dump_{command.name}<ApiDumpFormat::Text>(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{command_param_usage_text(command)});
                        break;
                    case ApiDumpFormat::Html:
                        dump_{command.name}<ApiDumpFormat::Html>(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{command_param_usage_text(command)});
                        break;
                    case ApiDumpFormat::Json:
                        dump_{command.name}<ApiDumpFormat::Json>(ApiDumpInstance::current(), {"result, " if command.returnType != "void" else ""}{command_param_usage_text(command)});
                        break;
                }}
            }}''')
            if command.name == 'vkQueuePresentKHR':
                self.write('ApiDumpInstance::current().nextFrame();')
            if command.returnType != 'void':
                self.write('return result;')
            self.write('}')
        protect.add_guard(self, None)


        self.write('\nVKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL api_dump_known_instance_functions(VkInstance instance, const char* pName)')
        self.write('{\n')
        for command in self.vk.commands.values():
            if command.name in ['vkEnumerateDeviceExtensionProperties', 'vkEnumerateInstanceVersion'] or (command.device and 'VK_EXT_debug_utils' not in command.extensions):
                continue
            protect.add_guard(self, command.protect)
            self.write(f'if(strcmp(pName, "{command.name}") == 0)')
            self.write(f'return reinterpret_cast<PFN_vkVoidFunction>({command.name});')
        protect.add_guard(self, None)
        self.write('\n    return nullptr;')
        self.write('}')

        self.write('\nVKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL api_dump_known_device_functions(VkDevice device, const char* pName)')
        self.write('{\n')
        for command in [x for x in self.vk.commands.values() if x.device ]:
            protect.add_guard(self, command.protect)
            self.write(f'if(strcmp(pName, "{command.name}") == 0 && (!device || device_dispatch_table(device)->{command.name[2:]}))')
            self.write(f'return reinterpret_cast<PFN_vkVoidFunction>({command.name});')
        protect.add_guard(self, None)

        self.write('\n    return nullptr;')
        self.write('}')

    def generate_implementation(self, video=False):
        self.write('#pragma once\n')

        self.write('\n#include "api_dump.h"')

        if not video:
            self.write('\n#include "api_dump_video_implementation.h"')

        self.write('\n//=========================== Type Implementations ==========================//\n')

        for t in [x for x in self.vk.platformTypes.values() if x.requires in ['vk_platform', 'stdint']]:
            if t.name in self.return_types:
                self.write('template <ApiDumpFormat Format>')
                self.write(f'void dump_return_value_{t.name}(const {t.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_platform_type_contents(t)
                self.write('}')
            if t.name in ['void', 'char'] or (not video and t.name in DUPLICATE_TYPES_IN_VIDEO_HEADER):
                continue
            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{t.name}(const {t.name} &object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write('dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            self.write_platform_type_contents(t)
            self.write('dump_end<Format>(settings, OutputConstruct::value, indents);')
            self.write('}')

        self.write('\n//========================= Basetype Implementations ========================//\n')

        protect = PlatformGuardHelper()
        for basetype in self.vk.baseTypes.values():
            protect.add_guard(self, basetype.protect)
            if basetype.name in self.return_types:
                self.write('template <ApiDumpFormat Format>')
                self.write(f'void dump_return_value_{basetype.name}(const {basetype.name}{"*" if basetype.name in POINTER_TYPES else "&"} object, const ApiDumpSettings& settings, int indents) {{')
                self.write('    dump_value<Format>(settings, object);')
                self.write('}')

            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{basetype.name}(const {basetype.name}{"*" if basetype.name in POINTER_TYPES else "&"} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write('    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            self.write('    dump_value<Format>(settings, object);')
            self.write('    dump_end<Format>(settings, OutputConstruct::value, indents);')
            self.write('}')
        protect.add_guard(self, None)

        self.write('\n//======================= System Type Implementations =======================//\n')

        for sys in self.vk.platformTypes.values():
            if (sys.requires is not None and (sys.requires in EXCLUDED_INCLUDE_LIST or 'vk_video' in sys.requires)) or (video and sys.name in DUPLICATE_TYPES_IN_VIDEO_HEADER):
                continue
            protect.add_guard(self, sys.protect)
            if sys.name in self.return_types:
                self.write('template <ApiDumpFormat Format>')
                self.write(f'void dump_return_value_{sys.name}(const {sys.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_system_type_contents(sys)
                self.write('}')
            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{sys.name}(const {sys.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents{", &object" if sys.name in POINTER_TYPES else ""});')
            self.write_system_type_contents(sys)
            self.write('dump_end<Format>(settings, OutputConstruct::value, indents);')
            self.write('}')
        protect.add_guard(self, None)

        if not video:
            self.write('\n//========================== Handle Implementations =========================//\n')
            for handle in self.vk.handles.values():
                type_erase_handle = 'TYPE_ERASE_HANDLE(object)'
                if handle.dispatchable:
                    type_erase_handle = 'static_cast<void*>(object)'
                protect.add_guard(self, handle.protect)
                self.write(f'''
                    template <ApiDumpFormat Format>
                    void dump_{handle.name}(const {handle.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr) {{
                        dump_handle<Format>({type_erase_handle}, settings, type_name, var_name, indents, address);
                    }}''')
            protect.add_guard(self, None)

        self.write('\n//=========================== Enum Implementations ==========================//\n')
        for enum in self.vk.enums.values():
            protect.add_guard(self, enum.protect)
            if enum.name in self.return_types:
                self.write('template <ApiDumpFormat Format>')
                self.write(f'void dump_return_value_{enum.name}(const {enum.name}& object, const ApiDumpSettings& settings, int indents) {{')
                self.write_enum_contents(enum)
                self.write('}')
            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{enum.name}(const {enum.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write('dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);')
            if enum.name in self.return_types:
                self.write(f'dump_return_value_{enum.name}<Format>(object, settings, indents);')
            else:
                self.write_enum_contents(enum)
            self.write('dump_end<Format>(settings, OutputConstruct::value, indents);')
            self.write('}')
        protect.add_guard(self, None)

        self.write('\n//========================= Bitmask Implementations =========================//\n')
        for bitmask in self.vk.bitmasks.values():
            protect.add_guard(self, bitmask.protect)
            self.write(f'''
                template <ApiDumpFormat Format>
                void dump_{bitmask.name}(const {bitmask.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr) {{
                    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
                    dump_value_start<Format>(settings);
                    settings.stream() << object;
                    bool is_first = true;''')
            for field in bitmask.flags:
                self.write(f'if(object {"==" if  field.zero or field.multiBit else "&"} {field.name}) {{')
                self.write(f'settings.stream() << (is_first ? \" (\" : \" | \") << "{field.name}"; is_first = false;')
                self.write('}')
            self.write('''
                if(!is_first)
                settings.stream() << ")";
                dump_value_end<Format>(settings);
                dump_end<Format>(settings, OutputConstruct::value, indents);
                }''')
        protect.add_guard(self, None)

        self.write('\n//=========================== Flag Implementations ==========================//\n')

        for flag in self.vk.flags.values():
            protect.add_guard(self, flag.protect)
            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{flag.name}(const {flag.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr) {{')
            if flag.bitmaskName is not None:
                self.write(f'dump_{flag.bitmaskName}<Format>(static_cast<{flag.bitmaskName}>(object), settings, type_name, var_name, indents, address);')
            else:
                self.write('dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);')
                self.write('dump_value<Format>(settings, object);')
                self.write('dump_end<Format>(settings, OutputConstruct::value, indents);')
            self.write('}')
        protect.add_guard(self, None)

        self.write('\n//======================= Func Pointer Implementations ======================//\n')

        for funcpointer in self.vk.funcPointers.values():
            self.write('template <ApiDumpFormat Format>')
            self.write(f'''void dump_{funcpointer.name}(const {funcpointer.name} object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr) {{
                dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
                dump_address<Format>(settings, &object);
                dump_end<Format>(settings, OutputConstruct::value, indents);
            }}''')

        self.write('\n//====================== Struct and Union Implementations =====================//\n')

        for struct in self.vk.structs.values():
            protect.add_guard(self, struct.protect)

            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{struct.name}(const {struct.name}& object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents, const void* address = nullptr)')
            self.write('{')
            self.write(f'dump_start<Format>(settings, OutputConstruct::{"api_union" if struct.union else "api_struct"}, type_name, var_name, indents, address);')

            for member in struct.members:
                validity_check = self.get_validity_check(member, struct)
                parameter_state = self.get_parameter_state(member, struct)
                if parameter_state is not None:
                    if validity_check is not None:
                        self.write(f'if ({validity_check})')
                        self.write('' + parameter_state)
                    else:
                        self.write('' + parameter_state)

            self.write('')
            for member in struct.members:
                custom_fullType = get_fulltype(member)
                validity_check = self.get_validity_check(member, struct)
                if member != struct.members[0]:
                    self.write('dump_separate_members<Format>(settings);')
                if validity_check is not None:
                    self.write(f'if({validity_check}) {{')

                self.write_value(member, struct)

                self.write('')
                if validity_check is not None:
                    if not struct.union: # Union's use validity_check to only print the active element
                        self.write(f'''}} else {{
                            if constexpr (Format == ApiDumpFormat::Text || Format == ApiDumpFormat::Html) {{
                                dump_special<Format>("UNUSED", settings, "{custom_fullType}", "{member.name}", indents + 2);
                            }} else if constexpr (Format == ApiDumpFormat::Json) {{
                                dump_json_UNUSED(settings, "{custom_fullType}", "{member.name}", indents + 1);
                            }}''')
                    self.write('}')

            self.write('')
            for member in struct.members:
                if member.pointer and member.name == 'pNext' and struct.name not in ['VkBaseInStructure', 'VkBaseOutStructure']:
                    self.write('if constexpr (Format == ApiDumpFormat::Text) {')
                    self.write(f'    dump_pNext_trampoline<ApiDumpFormat::Text>(object.pNext, settings, "{member.fullType}", "{member.type}", indents < 2 ? indents + 1 : indents);')
                    self.write('}')

            self.write('dump_end<Format>(settings, OutputConstruct::api_struct, indents);')
            self.write('}')
        protect.add_guard(self, None)

        self.write('\n//======================== pNext Chain Implementation =======================//\n')
        if not video:
            self.write('''
                template <ApiDumpFormat Format>
                void dump_pNext_struct_name(const void* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents) {
                    if (object == nullptr) {
                        dump_nullptr<Format>(settings, type_name, var_name, indents);
                        return;
                    }
                    VkBaseInStructure base_struct{};
                    memcpy(&base_struct, object, sizeof(VkBaseInStructure));
                    if (base_struct.sType == VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO || base_struct.sType == VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO) {
                        if (base_struct.pNext != nullptr) {
                            dump_pNext_struct_name<Format>(base_struct.pNext, settings, type_name, var_name, indents);
                        } else {
                            dump_nullptr<Format>(settings, "const void*", "pNext", indents);
                        }
                        return;
                    }
                    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents);
                    switch(base_struct.sType) {''')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                protect.add_guard(self, struct.protect)
                if struct.sType is not None:
                    self.write(f'''
                        case {struct.sType}:
                            dump_string<Format>(settings, "{struct.name}");
                            break;''')
            protect.add_guard(self, None)
            self.write('''
                    default:
                        dump_string<Format>(settings, "NULL");
                        break;
                    }
                    dump_end<Format>(settings, OutputConstruct::value, indents);
                }

                template <ApiDumpFormat Format>
                void dump_pNext_trampoline(const void* object, const ApiDumpSettings& settings, const char* type_name, const char *var_name, int indents) {
                    if (object == NULL) {
                        if constexpr (Format == ApiDumpFormat::Html || Format == ApiDumpFormat::Json) {
                            dump_nullptr<Format>(settings, type_name, var_name, indents);
                        }
                        return;
                    }
                    VkBaseInStructure base_struct{};
                    memcpy(&base_struct, object, sizeof(VkBaseInStructure));
                    switch(base_struct.sType) {''')
            for struct in [ x for x in self.vk.structs.values() if not x.union ]:
                protect.add_guard(self, struct.protect)
                if struct.sType is not None:
                    self.write(f'''
                        case {struct.sType}:
                            dump_{struct.name}<Format>(*reinterpret_cast<const {struct.name}*>(object), settings, "{struct.name}*", "pNext", indents, reinterpret_cast<const {struct.name}*>(object));
                            break;''')
            protect.add_guard(self, None)
            self.write('''
                    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47
                    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48
                        if (base_struct.pNext != nullptr) {
                            dump_pNext_trampoline<Format>(reinterpret_cast<const void*>(base_struct.pNext), settings, type_name, var_name, indents);
                        } else {
                            dump_nullptr<Format>(settings, "const void*", "pNext", indents);
                        }
                        break;
                    default:
                        dump_start<Format>(settings, OutputConstruct::value, "const void*", "pNext", indents);
                        dump_value<Format>(settings, "UNKNOWN (", (int64_t) (base_struct.sType), ")");
                        dump_end<Format>(settings, OutputConstruct::value, indents);
                        }
                    }''')

        self.write('\n//========================== Function Helpers ===============================//\n')

        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            protect.add_guard(self, command.protect)
            self.write(f'''
                template <ApiDumpFormat Format>
                void dump_params_{command.name}(ApiDumpInstance& dump_inst, {command_param_declaration_text(command)}) {{
                    const ApiDumpSettings& settings(dump_inst.settings());
                    if(settings.showParams()) {{
                        dump_pre_params_formatting<Format>(settings);''')
            for param in command.params:
                if param != command.params[0]:
                    self.write('dump_separate_members<Format>(settings);')

                parameter_state = self.get_parameter_state(param, command)
                if parameter_state is not None:
                    self.write('' + parameter_state)

                self.write_value(param, command)

            self.write('''dump_post_params_formatting<Format>(settings);
                    flush(settings);
                }
            }''')
        protect.add_guard(self, None)

        self.write('\n//========================= Function Implementations ========================//\n')
        for command in [x for x in self.vk.commands.values() if x.name not in FUNCTION_IMPLEMENTATION_IGNORE_LIST]:
            protect.add_guard(self, command.protect)
            returnParam = f'{command.returnType} result, ' if command.returnType != 'void' else ''
            self.write('template <ApiDumpFormat Format>')
            self.write(f'void dump_{command.name}(ApiDumpInstance& dump_inst, {returnParam}{command_param_declaration_text(command)}) {{')
            self.write('const ApiDumpSettings& settings(dump_inst.settings());')
            if command.returnType != 'void':
                self.write(f'dump_return_value<Format>(settings, "{command.returnType}", result, dump_return_value_{self.get_unaliased_type(command.returnType)}<Format>);')

            self.write(f'''dump_pre_function_formatting<Format>(settings);
                dump_params_{command.name}<Format>(dump_inst, {command_param_usage_text(command)});
                dump_post_function_formatting<Format>(settings);
                flush(settings);
            }}''')
        protect.add_guard(self, None)
        self.write('\n')

    def build_alias_map(self):
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

    def get_return_types(self):
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

    def write_value(self, var, parent):
        custom_fullType = get_fulltype(var)
        custom_type = get_type(var)
        object_access = 'object.' if isinstance(parent, Struct) else ''
        call_type = self.get_unaliased_type(var.type)
        indent_plus = '(Format == ApiDumpFormat::Json ? 2 : 1)'
        if isinstance(parent, Command):
            indent = '(Format == ApiDumpFormat::Json ? 4 : 1)'
        else:
            indent = 'indents + (Format == ApiDumpFormat::Json ? 2 : 1)'

        if var.name == 'apiVersion':
            call_type = 'api_version'
        elif var.name == 'pNext' and var.fullType in ['void*', 'const void*']:
            call_type = 'pNext'

        array_ptr = f'{object_access}{var.name}'
        if get_is_array(var):
            array_len = get_array_length(var, parent)
            if parent.name == 'VkPipelineMultisampleStateCreateInfo' and var.length == '(rasterizationSamples + 31) / 32':
                array_len = '(object.rasterizationSamples + 31) / 32'

            if parent.name == 'VkShaderModuleCreateInfo' and var.name == 'pCode':
                array_len = f'{object_access}{var.length}'
                self.write('if(settings.showShader()) {')

            if len(var.fixedSizeArray) > 2:
                raise RuntimeError("Unhandled fixed array dimentionality")
            elif len(var.fixedSizeArray) == 2:
                self.write(f'dump_double_array<Format>({array_ptr}, {var.fixedSizeArray[0]},  {var.fixedSizeArray[1]}, settings, "{custom_fullType}", "{var.name}", "{custom_type}", {indent} + {indent_plus}, dump_{call_type}<Format>);')

            elif len(var.fixedSizeArray) == 1 and isinstance(parent, Struct): # Fixed length array's passed as parameters are treated as void*, so only match when printing members
                fixed_array_len = get_fixed_array_length(var.fixedSizeArray[0], var, parent)
                self.write(f'dump_single_array<Format>({array_ptr}, {fixed_array_len}, settings, "{custom_fullType}", "{var.name}", "{custom_type}", {indent} + {indent_plus}, dump_{call_type}<Format>);')

            else:
                if var.fullType.count('*') > 1 and var.type not in ['void', 'char']:
                    self.write(f'dump_pointer_array<Format>(*{array_ptr}, {array_len}, settings, "{custom_fullType}", "{var.name}", "{custom_type}", {indent} + {indent_plus}, dump_{call_type}<Format>);')
                else:
                    self.write(f'dump_pointer_array<Format>({array_ptr}, {array_len}, settings, "{custom_fullType}", "{var.name}", "{custom_type}", {indent} + {indent_plus}, dump_{call_type}<Format>);')

            if parent.name == 'VkShaderModuleCreateInfo' and var.name == 'pCode':
                self.write('} else {')
                self.write(f'dump_special<Format>("SHADER DATA", settings, "{var.fullType}", "{var.name}", {indent});')
                self.write('}')
        else:
            if var.pointer and var.type not in ['void', 'char'] and var.type not in self.vk.funcPointers:
                self.write(f'dump_pointer<Format>({array_ptr}, settings, "{custom_fullType}", "{var.name}", {indent}, dump_{call_type}<Format>);')
            else:
                self.write(f'dump_{call_type}<Format>({array_ptr}, settings, "{custom_fullType}", "{var.name}", {indent});')


    def write_platform_type_contents(self, t):
        cast = ''
        if t.name == 'uint8_t':
            cast = '(uint32_t) '
        if t.name == 'int8_t':
            cast = '(int32_t) '
        self.write(f'dump_value<Format>(settings, {cast}object);')

    def write_system_type_contents(self, sys):
        self.write('dump_address<Format>(settings, &object);')
        if '*' in sys.name:
            self.write('if constexpr (Format == ApiDumpFormat::Json) {')
            self.write('    settings.stream() << "\\n";')
            self.write('}')

    def write_enum_contents(self, enum):
        int_cast = ('' if any(f.negative for f in enum.fields) else 'u') + (f'int{enum.bitWidth}_t')

        protect = PlatformGuardHelper()
        self.write('switch((int64_t) object)')
        self.write('{')
        for field in enum.fields:
            protect.add_guard(self, field.protect)
            self.write(f'''case {field.name}:
                    dump_enum<Format>(settings, "{field.name}", static_cast<{int_cast}>(object));
                    break;''')
        protect.add_guard(self, None)
        self.write('default:')
        self.write(f'    dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<{int_cast}>(object));')
        self.write('}')
