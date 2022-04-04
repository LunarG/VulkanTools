#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2016, 2019, 2021 Valve Corporation
# Copyright (c) 2015-2016, 2019, 2021 LunarG, Inc.
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
# Author: Lenny Komow <lenny@lunarg.com>
#
# The API dump layer works by passing custom format strings to the ApiDumpGenerator. These format
# strings are C++ code, with 3-ish exceptions:
#   * Anything beginning with @ will be expanded by the ApiDumpGenerator. These are used to allow
#       iteration over various items within the Vulkan spec, such as functions, enums, etc.
#   * Anything surrounded by { and } will be substituted when the ApiDumpGenerator expands the @
#       directives. This gives a way to get things like data types or names for anything that can
#       be iterated over in an @ directive.
#   * Curly braces must be doubled like {{ for a single curly brace to appear in the output code.
#
# The API dump uses separate format strings for each output file, but passes them to a common
# generator. This allows greater flexibility, as changing the output codegen means just changing
# the corresponding format string.
#
# Currently, the API dump layer generates the following files from the following strings:
#   * api_dump.cpp: COMMON_CODEGEN - Provides all entrypoints for functions and dispatches the calls
#       to the proper back end
#   * api_dump_text.h: TEXT_CODEGEN - Provides the back end for dumping to a text file
#   * api_dump_html.h: HTML_CODEGEN - Provides the back end for dumping to a html document
#   * api_dump_json.h: JSON_CODEGEN - Provides the back end for dumping to a JSON file
#

import os,re,sys,string
import xml.etree.ElementTree as etree
import generator as gen
from generator import *
from collections import namedtuple
from common_codegen import *

COMMON_CODEGEN = """
/* Copyright (c) 2015-2016, 2021 Valve Corporation
 * Copyright (c) 2015-2016, 2021 LunarG, Inc.
 * Copyright (c) 2015-2016, 2021 Google Inc.
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

#include "api_dump_text.h"
#include "api_dump_html.h"
#include "api_dump_json.h"

//============================= Dump Functions ==============================//

@foreach function where(not '{funcName}' in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr', 'vkDebugMarkerSetObjectNameEXT','vkSetDebugUtilsObjectNameEXT'])
inline void dump_head_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    if (!dump_inst.shouldDumpOutput()) return ;
    dump_inst.outputMutex()->lock();
    switch(dump_inst.settings().format())
    {{
    case ApiDumpFormat::Text:
        dump_text_head_{funcName}(dump_inst, {funcNamedParams});
        break;
    case ApiDumpFormat::Html:
        dump_html_head_{funcName}(dump_inst, {funcNamedParams});
        break;
    case ApiDumpFormat::Json:
        dump_json_head_{funcName}(dump_inst, {funcNamedParams});
        break;
    }}
    //Keep lock
}}
@end function

@foreach function where('{funcReturn}' != 'void' and not '{funcName}' in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr', 'vkDebugMarkerSetObjectNameEXT','vkSetDebugUtilsObjectNameEXT'])
inline void dump_body_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
{{
    if (!dump_inst.shouldDumpOutput()) return;

    //Lock is already held
    switch(dump_inst.settings().format())
    {{
    case ApiDumpFormat::Text:
        dump_text_body_{funcName}(dump_inst, result, {funcNamedParams});
        break;
    case ApiDumpFormat::Html:
        dump_html_body_{funcName}(dump_inst, result, {funcNamedParams});
        break;
    case ApiDumpFormat::Json:
        dump_json_body_{funcName}(dump_inst, result, {funcNamedParams});
        break;
    }}
    dump_inst.outputMutex()->unlock();
}}
@end function

@foreach function where('{funcReturn}' == 'void')
inline void dump_body_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    if (!dump_inst.shouldDumpOutput()) return ;
    //Lock is already held
    switch(dump_inst.settings().format())
    {{
    case ApiDumpFormat::Text:
        dump_text_body_{funcName}(dump_inst, {funcNamedParams});
        break;
    case ApiDumpFormat::Html:
        dump_html_body_{funcName}(dump_inst, {funcNamedParams});
        break;
    case ApiDumpFormat::Json:
        dump_json_body_{funcName}(dump_inst, {funcNamedParams});
        break;
    }}
    dump_inst.outputMutex()->unlock();
}}
@end function


@foreach function where('{funcName}' == 'vkDebugMarkerSetObjectNameEXT')
inline void dump_head_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    dump_inst.outputMutex()->lock();

    if (pNameInfo->pObjectName)
    {{
        dump_inst.object_name_map.insert(std::make_pair<uint64_t, std::string>((uint64_t &&)pNameInfo->object, pNameInfo->pObjectName));
    }}
    else
    {{
        dump_inst.object_name_map.erase(pNameInfo->object);
    }}

    if (dump_inst.shouldDumpOutput()) {{
        switch(dump_inst.settings().format())
        {{
        case ApiDumpFormat::Text:
            dump_text_head_{funcName}(dump_inst, {funcNamedParams});
            break;
        case ApiDumpFormat::Html:
            dump_html_head_{funcName}(dump_inst, {funcNamedParams});
            break;
        case ApiDumpFormat::Json:
            dump_json_head_{funcName}(dump_inst, {funcNamedParams});
            break;
        }}
    }}

    //Keep lock
}}
@end function

@foreach function where('{funcName}' == 'vkDebugMarkerSetObjectNameEXT')
inline void dump_body_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
{{
    //Lock is already held
    if (dump_inst.shouldDumpOutput()) {{
        switch(dump_inst.settings().format())
        {{
        case ApiDumpFormat::Text:
            dump_text_body_{funcName}(dump_inst, result, {funcNamedParams});
            break;
        case ApiDumpFormat::Html:
            dump_html_body_{funcName}(dump_inst, result, {funcNamedParams});
            break;
        case ApiDumpFormat::Json:
            dump_json_body_{funcName}(dump_inst, result, {funcNamedParams});
            break;
        }}
    }}

    dump_inst.outputMutex()->unlock();
}}
@end function

@foreach function where('{funcName}' == 'vkSetDebugUtilsObjectNameEXT')
inline void dump_head_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    dump_inst.outputMutex()->lock();
    if (pNameInfo->pObjectName)
    {{
        dump_inst.object_name_map.insert(std::make_pair<uint64_t, std::string>((uint64_t &&)pNameInfo->objectHandle, pNameInfo->pObjectName));
    }}
    else
    {{
        dump_inst.object_name_map.erase(pNameInfo->objectHandle);
    }}
    if (dump_inst.shouldDumpOutput()) {{
        switch(dump_inst.settings().format())
        {{
        case ApiDumpFormat::Text:
            dump_text_head_{funcName}(dump_inst, {funcNamedParams});
            break;
        case ApiDumpFormat::Html:
            dump_html_head_{funcName}(dump_inst, {funcNamedParams});
            break;
        case ApiDumpFormat::Json:
            dump_json_head_{funcName}(dump_inst, {funcNamedParams});
            break;
        }}
    }}
    //Keep lock
}}
@end function

@foreach function where('{funcName}' == 'vkSetDebugUtilsObjectNameEXT')
inline void dump_body_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
{{
    //Lock is already held
    if (dump_inst.shouldDumpOutput()) {{
        switch(dump_inst.settings().format())
        {{
        case ApiDumpFormat::Text:
            dump_text_body_{funcName}(dump_inst, result, {funcNamedParams});
            break;
        case ApiDumpFormat::Html:
            dump_html_body_{funcName}(dump_inst, result, {funcNamedParams});
            break;
        case ApiDumpFormat::Json:
            dump_json_body_{funcName}(dump_inst, result, {funcNamedParams});
            break;
        }}
    }}
    dump_inst.outputMutex()->unlock();
}}
@end function

//============================= API EntryPoints =============================//

// Specifically implemented functions

@foreach function where('{funcName}' == 'vkCreateInstance')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});

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
    // Output the API dump
    dump_body_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    return result;
}}
@end function

@foreach function where('{funcName}' == 'vkDestroyInstance')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
    // Destroy the dispatch table
    dispatch_key key = get_dispatch_key({funcDispatchParam});
    instance_dispatch_table({funcDispatchParam})->DestroyInstance({funcNamedParams});
    destroy_instance_dispatch_table(key);
    {funcStateTrackingCode}
    // Output the API dump
    dump_body_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
}}
@end function

@foreach function where('{funcName}' == 'vkCreateDevice')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});

    // Get the function pointer
    VkLayerDeviceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    VkInstance vk_instance = ApiDumpInstance::current().get_vk_instance(physicalDevice);
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(vk_instance, "vkCreateDevice");
    if(fpCreateDevice == NULL) {{
        return VK_ERROR_INITIALIZATION_FAILED;
    }}

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    {funcReturn} result = fpCreateDevice({funcNamedParams});
    if(result == VK_SUCCESS) {{
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }}
    {funcStateTrackingCode}
    // Output the API dump
    dump_body_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    return result;
}}
@end function

@foreach function where('{funcName}' == 'vkDestroyDevice')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});

    // Destroy the dispatch table
    dispatch_key key = get_dispatch_key({funcDispatchParam});
    device_dispatch_table({funcDispatchParam})->DestroyDevice({funcNamedParams});
    destroy_device_dispatch_table(key);
    {funcStateTrackingCode}
    // Output the API dump
    dump_body_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
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
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 2, VK_HEADER_VERSION), // specVersion
            VK_MAKE_VERSION(0, 2, 0), // implementationVersion
            "layer: api_dump",
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
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 2, VK_HEADER_VERSION),
            VK_MAKE_VERSION(0, 2, 0),
            "layer: api_dump",
        }}
    }};

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}}
@end function

@foreach function where('{funcName}' == 'vkQueuePresentKHR')
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    ApiDumpInstance::current().outputMutex()->lock();
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});

    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    dump_body_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});

    ApiDumpInstance::current().nextFrame();
    ApiDumpInstance::current().outputMutex()->unlock();
    return result;
}}
@end function

// Autogen instance functions

@foreach function where('{funcDispatchType}' == 'instance' and '{funcReturn}' != 'void' and '{funcName}' not in ['vkCreateInstance', 'vkDestroyInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties','vkGetPhysicalDeviceToolPropertiesEXT'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
    {funcReturn} result = instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    @if('{funcName}' == 'vkEnumeratePhysicalDevices')
    if (pPhysicalDeviceCount != nullptr && pPhysicalDevices != nullptr) {{
        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {{
            ApiDumpInstance::current().set_vk_instance(pPhysicalDevices[i], instance);
        }}
    }}
    @end if
    dump_body_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    return result;
}}
@end function

@foreach function where('{funcDispatchType}' == 'instance' and '{funcReturn}' == 'void' and '{funcName}' not in ['vkCreateInstance', 'vkDestroyInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
    instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    dump_body_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
}}
@end function

// Autogen device functions

@foreach function where('{funcDispatchType}' == 'device' and '{funcReturn}' != 'void' and '{funcName}' not in ['vkDestroyDevice', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkQueuePresentKHR', 'vkGetDeviceProcAddr'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    dump_body_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    return result;
}}
@end function

@foreach function where('{funcDispatchType}' == 'device' and '{funcReturn}' == 'void' and '{funcName}' not in ['vkDestroyDevice', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkGetDeviceProcAddr'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    dump_head_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
    device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    {funcStateTrackingCode}
    dump_body_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
}}
@end function

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t *pToolCount, VkPhysicalDeviceToolPropertiesEXT *pToolProperties)
{{
    dump_head_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance::current(), physicalDevice, pToolCount, pToolProperties);
    static const VkPhysicalDeviceToolPropertiesEXT api_dump_layer_tool_props = {{
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT,
        nullptr,
        "API Dump Layer",
        "2",
        VK_TOOL_PURPOSE_PROFILING_BIT_EXT | VK_TOOL_PURPOSE_TRACING_BIT_EXT,
        "The VK_LAYER_LUNARG_api_dump utility layer prints API calls, parameters, and values to the identified output stream.",
        "VK_LAYER_LUNARG_api_dump"}};

    auto original_pToolProperties = pToolProperties;
    if (pToolProperties != nullptr) {{
        *pToolProperties = api_dump_layer_tool_props;
        pToolProperties = ((*pToolCount > 1) ? &pToolProperties[1] : nullptr);
        (*pToolCount)--;
    }}

    VkLayerInstanceDispatchTable *pInstanceTable = instance_dispatch_table(physicalDevice);
    VkResult result = pInstanceTable->GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);

    if (original_pToolProperties != nullptr) {{
        pToolProperties = original_pToolProperties;
    }}

    (*pToolCount)++;
    dump_body_vkGetPhysicalDeviceToolPropertiesEXT(ApiDumpInstance::current(), result, physicalDevice, pToolCount, pToolProperties);
    return result;
}}

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

TEXT_CODEGEN = """
/* Copyright (c) 2015-2016, 2019 Valve Corporation
 * Copyright (c) 2015-2016, 2019 LunarG, Inc.
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

@foreach struct
std::ostream& dump_text_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents{sctConditionVars});
@end struct
@foreach union
std::ostream& dump_text_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

//============================= typedefs ==============================//

// Functions for dumping typedef types that the codegen scripting can't handle
#if defined(VK_ENABLE_BETA_EXTENSIONS)
std::ostream& dump_text_VkAccelerationStructureTypeKHR(VkAccelerationStructureTypeKHR object, const ApiDumpSettings& settings, int indents);
std::ostream& dump_text_VkAccelerationStructureTypeNV(VkAccelerationStructureTypeNV object, const ApiDumpSettings& settings, int indents)
{{
    return dump_text_VkAccelerationStructureTypeKHR(object, settings, indents);
}}
std::ostream& dump_text_VkBuildAccelerationStructureFlagsKHR(VkBuildAccelerationStructureFlagsKHR object, const ApiDumpSettings& settings, int indents);
inline std::ostream& dump_text_VkBuildAccelerationStructureFlagsNV(VkBuildAccelerationStructureFlagsNV object, const ApiDumpSettings& settings, int indents)
{{
    return dump_text_VkBuildAccelerationStructureFlagsKHR(object, settings, indents);
}}
#endif // VK_ENABLE_BETA_EXTENSIONS


//======================== pNext Chain Implementation =======================//

std::ostream& dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) (static_cast<const VkBaseInStructure*>(object)->sType)) {{
    @foreach struct where('{sctName}' not in ['VkPipelineViewportStateCreateInfo', 'VkCommandBufferBeginInfo'])
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        dump_text_pNext<const {sctName}>(static_cast<const {sctName}*>(object), settings, "{sctName}", indents, dump_text_{sctName});
        break;
        @end if
    @end struct

    case 47: // VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO
    case 48: // VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO
        if(static_cast<const VkBaseInStructure*>(object)->pNext != nullptr){{
            dump_text_pNext_trampoline(static_cast<const void*>(static_cast<const VkBaseInStructure*>(object)->pNext), settings, indents);
        }} else {{
            settings.formatNameType(settings.stream(), indents, "pNext", "const void*");
            settings.stream() << "NULL\\n";
        }}
        break;
    default:
        settings.formatNameType(settings.stream(), indents, "pNext", "const void*");
        settings.stream() << "UNKNOWN (" << (int64_t) (static_cast<const VkBaseInStructure*>(object)->sType) << ")\\n";
    }}
    return settings.stream();
}}

inline std::ostream& dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents, bool is_dynamic_viewport, bool is_dynamic_scissor)
{{
    dump_text_pNext<const VkPipelineViewportStateCreateInfo>(static_cast<const VkPipelineViewportStateCreateInfo*>(object), settings, "VkPipelineViewportStateCreateInfo", indents, dump_text_VkPipelineViewportStateCreateInfo, is_dynamic_viewport, is_dynamic_scissor);
    return settings.stream();
}}

inline std::ostream& dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents, VkCommandBuffer cmd_buffer)
{{
    dump_text_pNext<const VkCommandBufferBeginInfo>(static_cast<const VkCommandBufferBeginInfo*>(object), settings, "VkCommandBufferBeginInfo", indents, dump_text_VkCommandBufferBeginInfo, cmd_buffer);
    return settings.stream();
}}

std::ostream& dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) (static_cast<const VkBaseInStructure*>(object)->sType)) {{
    @foreach struct where('{sctName}' not in ['VkPipelineViewportStateCreateInfo', 'VkCommandBufferBeginInfo'])
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        settings.formatNameType(settings.stream(), indents, "pNext", "const void*");
        settings.stream() << "{sctName}\\n";
        break;
        @end if
    @end struct

    case 47: // VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO
    case 48: // VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO
        settings.formatNameType(settings.stream(), indents, "pNext", "const void*");
        settings.stream() << "NULL\\n";
        break;
    default:
        settings.formatNameType(settings.stream(), indents, "pNext", "const void*");
        settings.stream() << "UNKNOWN (" << (int64_t) (static_cast<const VkBaseInStructure*>(object)->sType) << ")\\n";
    }}
    return settings.stream();
}}

//=========================== Type Implementations ==========================//

@foreach type where('{etyName}' != 'void')
inline std::ostream& dump_text_{etyName}({etyName} object, const ApiDumpSettings& settings, int indents)
{{
    @if('{etyName}' != 'uint8_t')
    return settings.stream() << object;
    @end if
    @if('{etyName}' == 'uint8_t')
    return settings.stream() << (uint32_t) object;
    @end if
}}
@end type

//========================= Basetype Implementations ========================//

@foreach basetype where(not '{baseName}' in ['ANativeWindow', 'AHardwareBuffer', 'CAMetalLayer'])
inline std::ostream& dump_text_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << object;
}}
@end basetype
@foreach basetype where('{baseName}' in ['ANativeWindow', 'AHardwareBuffer'])
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline std::ostream& dump_text_{baseName}(const {baseName}* object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << object;
}}
#endif
@end basetype
@foreach basetype where('{baseName}' in ['CAMetalLayer'])
#if defined(VK_USE_PLATFORM_METAL_EXT)
inline std::ostream& dump_text_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << object;
}}
#endif
@end basetype

//======================= System Type Implementations =======================//

@foreach systype
inline std::ostream& dump_text_{sysName}(const {sysType} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << object;
}}
@end systype

//========================== Handle Implementations =========================//

@foreach handle
inline std::ostream& dump_text_{hdlName}(const {hdlName} object, const ApiDumpSettings& settings, int indents)
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

    return settings.stream();
}}
@end handle

//=========================== Enum Implementations ==========================//

@foreach enum
std::ostream& dump_text_{enumName}({enumName} object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) object)
    {{
    @foreach option
    case {optValue}:
        settings.stream() << "{optName} (";
        break;
    @end option
    default:
        settings.stream() << "UNKNOWN (";
    }}
    return settings.stream() << object << ")";
}}
@end enum

//========================= Bitmask Implementations =========================//

@foreach bitmask
@if('{bitWidth}' == '64')
// 64 bit bitmasks don't have an enum of bit values.
// NOTE: Because all of the api_dump_*.h files are only included in api_dump.cpp, this typedef
// only needs to be generated by the first .h file.
typedef VkFlags64 {bitName};
@end if
std::ostream& dump_text_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    bool is_first = true;
    //settings.formatNameType(stream, indents, name, type_string) << object;
    settings.stream() << object;
    @foreach option
        @if('{optMultiValue}' != 'None')
            if(object == {optValue})
                is_first = dump_text_bitmaskOption("{optName}", settings.stream(), is_first);
        @end if
        @if('{optMultiValue}' == 'None')
            if(object & {optValue})
                is_first = dump_text_bitmaskOption("{optName}", settings.stream(), is_first);
        @end if

    @end option
    if(!is_first)
        settings.stream() << ")";
    return settings.stream();
}}
@end bitmask

//=========================== Flag Implementations ==========================//

@foreach flag where('{flagEnum}' != 'None')
inline std::ostream& dump_text_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    return dump_text_{flagEnum}(({flagEnum}) object, settings, indents);
}}
@end flag
@foreach flag where('{flagEnum}' == 'None')
inline std::ostream& dump_text_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << object;
}}
@end flag

//======================= Func Pointer Implementations ======================//

@foreach funcpointer
inline std::ostream& dump_text_{pfnName}({pfnName} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        return settings.stream() << object;
    else
        return settings.stream() << "address";
}}
@end funcpointer

//========================== Struct Implementations =========================//

@foreach struct where('{sctName}' not in ['VkPhysicalDeviceMemoryProperties','VkPhysicalDeviceGroupProperties'])
std::ostream& dump_text_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents{sctConditionVars})
{{
    if(settings.showAddress())
        settings.stream() << &object << ":\\n";
    else
        settings.stream() << "address:\\n";

    @foreach member
    @if('{memCondition}' != 'None')
    if({memCondition})
    @end if

    @if({memPtrLevel} == 0)
        @if('{memName}' != 'pNext')
    dump_text_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions});  // AET
        @end if
        @if('{memName}' == 'pNext')
    if(object.pNext != nullptr){{
        dump_text_pNext_struct_name(object.{memName}, settings, indents + 1);
    }} else {{
        dump_text_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions}); // BET
    }}
        @end if
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_text_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_text_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions}); // AQA
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
    @if('{memLength}'[0].isdigit() or '{memLength}'[0].isupper())
    dump_text_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions}); // BQA
    @end if
    @if(not ('{memLength}'[0].isdigit() or '{memLength}'[0].isupper()))
    @if('{memLength}' == 'rasterizationSamples')
    dump_text_array<const {memBaseType}>(object.{memName}, (object.{memLength} + 31) / 32, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions}); // BQB
    @end if
    @if('{memLength}' != 'rasterizationSamples')
    dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions}); // BQB
    @end if
    @end if
    @end if

    @if('{sctName}' == 'VkShaderModuleCreateInfo')
    @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}{memInheritedConditions}); // CQA
    else
        dump_text_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end if

    @if('{memCondition}' != 'None')
    else
        dump_text_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end member

    @foreach member
    @if({memPtrLevel} == 0)
        @if('{memName}' == 'pNext')
    if(object.pNext != nullptr){{
        dump_text_pNext_trampoline(object.{memName}, settings, indents < 2 ? indents + 1 : indents);
    }}
        @end if
    @end if
    @end member
    return settings.stream();
}}
@end struct

std::ostream& dump_text_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties& object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        settings.stream() << &object << ":\\n";
    else
        settings.stream() << "address:\\n";

    dump_text_value<const uint32_t>(object.memoryTypeCount, settings, "uint32_t", "memoryTypeCount", indents + 1, dump_text_uint32_t); // FET
    dump_text_array<const VkMemoryType>(object.memoryTypes, object.memoryTypeCount, settings, "VkMemoryType[VK_MAX_MEMORY_TYPES]", "VkMemoryType", "memoryTypes", indents + 1, dump_text_VkMemoryType); // DQA
    dump_text_value<const uint32_t>(object.memoryHeapCount, settings, "uint32_t", "memoryHeapCount", indents + 1, dump_text_uint32_t); // GET
    dump_text_array<const VkMemoryHeap>(object.memoryHeaps, object.memoryHeapCount, settings, "VkMemoryHeap[VK_MAX_MEMORY_HEAPS]", "VkMemoryHeap", "memoryHeaps", indents + 1, dump_text_VkMemoryHeap); // EQA
    return settings.stream();
}}

std::ostream& dump_text_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties& object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        settings.stream() << &object << ":\\n";
    else
        settings.stream() << "address:\\n";
    dump_text_value<const VkStructureType>(object.sType, settings, "VkStructureType", "sType", indents + 1, dump_text_VkStructureType); // HET
    dump_text_value<const void*>(object.pNext, settings, "void*", "pNext", indents + 1, dump_text_void); // IET
    dump_text_value<const uint32_t>(object.physicalDeviceCount, settings, "uint32_t", "physicalDeviceCount", indents + 1, dump_text_uint32_t); // JET
    dump_text_array<const VkPhysicalDevice>(object.physicalDevices, object.physicalDeviceCount, settings, "VkPhysicalDevice[VK_MAX_DEVICE_GROUP_SIZE]", "VkPhysicalDevice", "physicalDevices", indents + 1, dump_text_VkPhysicalDevice); // FQA
    dump_text_value<const VkBool32>(object.subsetAllocation, settings, "VkBool32", "subsetAllocation", indents + 1, dump_text_VkBool32); // KET
    return settings.stream();
}}

//========================== Union Implementations ==========================//

@foreach union
std::ostream& dump_text_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        settings.stream() << &object << " (Union):\\n";
    else
        settings.stream() << "address (Union):\\n";

    @foreach choice
    @if({chcPtrLevel} == 0)
    dump_text_value<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 1, dump_text_{chcTypeID}); // LET
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' == 'None')
    dump_text_pointer<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 1, dump_text_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' != 'None')
    dump_text_array<const {chcBaseType}>(object.{chcName}, {chcLength}, settings, "{chcType}", "{chcChildType}", "{chcName}", indents + 1, dump_text_{chcTypeID}); // GQA
    @end if
    @end choice
    return settings.stream();
}}
@end union

//========================= Function Implementations ========================//

@foreach function where('{funcName}' not in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
std::ostream& dump_text_head_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());
    if (settings.showThreadAndFrame()) {{
        settings.stream() << "Thread " << dump_inst.threadID() << ", Frame " << dump_inst.frameCount();
    }}
    if(settings.showTimestamp() && settings.showThreadAndFrame()) {{
        settings.stream() << ", ";
    }}
    if (settings.showTimestamp()) {{
        settings.stream() << "Time " << dump_inst.current_time_since_start().count() << " us";
    }}
    if (settings.showTimestamp() || settings.showThreadAndFrame()) {{
        settings.stream() << ":\\n";
    }}
    settings.stream() << "{funcName}({funcNamedParams}) returns {funcReturn}";

    return settings.shouldFlush() ? settings.stream() << std::flush : settings.stream();
}}
@end function

@foreach function where('{funcName}' not in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
@if('{funcReturn}' != 'void')
std::ostream& dump_text_body_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
@end if
@if('{funcReturn}' == 'void')
std::ostream& dump_text_body_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
@end if
{{
    const ApiDumpSettings& settings(dump_inst.settings());

    @if('{funcReturn}' != 'void')
    settings.stream() << " ";
    dump_text_{funcReturn}(result, settings, 0);
    @end if
    settings.stream() << ":\\n";
    if(settings.showParams())
    {{
        @foreach parameter
        @if({prmPtrLevel} == 0)
        dump_text_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID}{prmInheritedConditions}); // MET
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_text_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID}{prmInheritedConditions});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_text_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_text_{prmTypeID}{prmInheritedConditions}); // HQA
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";

    return settings.stream();
}}
@end function

"""

# This HTML Codegen is essentially copied from the format above.
# Due to the way some of the functions have been organized, some of the HTML tags
# that are opened are closed in another function. See api_dump.h. This may need refactoring.

HTML_CODEGEN = """
/* Copyright (c) 2015-2017, 2019 Valve Corporation
 * Copyright (c) 2015-2017, 2019 LunarG, Inc.
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

@foreach struct
std::ostream& dump_html_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents{sctConditionVars});
@end struct
@foreach union
std::ostream& dump_html_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

//============================= typedefs ==============================//

// Functions for dumping typedef types that the codegen scripting can't handle
#if defined(VK_ENABLE_BETA_EXTENSIONS)
std::ostream& dump_html_VkAccelerationStructureTypeKHR(VkAccelerationStructureTypeKHR object, const ApiDumpSettings& settings, int indents);
std::ostream& dump_html_VkAccelerationStructureTypeNV(VkAccelerationStructureTypeNV object, const ApiDumpSettings& settings, int indents)
{{
    return dump_html_VkAccelerationStructureTypeKHR(object, settings, indents);
}}
std::ostream& dump_html_VkBuildAccelerationStructureFlagsKHR(VkBuildAccelerationStructureFlagsKHR object, const ApiDumpSettings& settings, int indents);
inline std::ostream& dump_html_VkBuildAccelerationStructureFlagsNV(VkBuildAccelerationStructureFlagsNV object, const ApiDumpSettings& settings, int indents)
{{
    return dump_html_VkBuildAccelerationStructureFlagsKHR(object, settings, indents);
}}
#endif // VK_ENABLE_BETA_EXTENSIONS


//======================== pNext Chain Implementation =======================//

std::ostream& dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) (static_cast<const VkBaseInStructure*>(object)->sType)) {{
    @foreach struct where('{sctName}' not in ['VkPipelineViewportStateCreateInfo', 'VkCommandBufferBeginInfo'])
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        dump_html_pNext<const {sctName}>(static_cast<const {sctName}*>(object), settings, "{sctName}", indents, dump_html_{sctName});
        break;
        @end if
    @end struct

    case 47: // VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO
    case 48: // VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO
        if(static_cast<const VkBaseInStructure*>(object)->pNext != nullptr){{
            dump_html_pNext_trampoline(static_cast<const void*>(static_cast<const VkBaseInStructure*>(object)->pNext), settings, indents);
        }} else {{
            settings.stream() << "<details class='data'><summary>";
            dump_html_nametype(settings.stream(), settings.showType(), "pNext", "const void*");
            settings.stream() << "<div class='val'> NULL</div></summary></details>";
        }}
        break;
    default:
        settings.stream() << "<details class='data'><summary>";
        dump_html_nametype(settings.stream(), settings.showType(), "pNext", "const void*");
        settings.stream() << "<div class='val'>UNKNOWN (" << (int64_t) (static_cast<const VkBaseInStructure*>(object)->sType) <<")</div></summary></details>";
    }}
    return settings.stream();
}}

inline std::ostream& dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents, bool is_dynamic_viewport, bool is_dynamic_scissor)
{{
    dump_html_pNext<const VkPipelineViewportStateCreateInfo>(static_cast<const VkPipelineViewportStateCreateInfo*>(object), settings, "VkPipelineViewportStateCreateInfo", indents, dump_html_VkPipelineViewportStateCreateInfo, is_dynamic_viewport, is_dynamic_scissor);
    return settings.stream();
}}

inline std::ostream& dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents, VkCommandBuffer cmd_buffer)
{{
    dump_html_pNext<const VkCommandBufferBeginInfo>(static_cast<const VkCommandBufferBeginInfo*>(object), settings, "VkCommandBufferBeginInfo", indents, dump_html_VkCommandBufferBeginInfo, cmd_buffer);
    return settings.stream();
}}

//=========================== Type Implementations ==========================//

@foreach type where('{etyName}' != 'void')
inline std::ostream& dump_html_{etyName}({etyName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    @if('{etyName}' != 'uint8_t')
    settings.stream() << object;
    @end if
    @if('{etyName}' == 'uint8_t')
    settings.stream() << (uint32_t) object;
    @end if
    return settings.stream() << "</div></summary>";
}}
@end type

//========================= Basetype Implementations ========================//

@foreach basetype where(not '{baseName}' in ['ANativeWindow', 'AHardwareBuffer', 'CAMetalLayer'])
inline std::ostream& dump_html_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
@end basetype
@foreach basetype where('{baseName}' in ['ANativeWindow', 'AHardwareBuffer'])
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline std::ostream& dump_html_{baseName}(const {baseName}* object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
#endif
@end basetype
@foreach basetype where('{baseName}' in ['CAMetalLayer'])
#if defined(VK_USE_PLATFORM_METAL_EXT)
inline std::ostream& dump_html_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
#endif
@end basetype

//======================= System Type Implementations =======================//

@foreach systype
inline std::ostream& dump_html_{sysName}(const {sysType} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
@end systype

//========================== Handle Implementations =========================//

@foreach handle
inline std::ostream& dump_html_{hdlName}(const {hdlName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    if(settings.showAddress()) {{
        settings.stream() << object;

        std::unordered_map<uint64_t, std::string>::const_iterator it = ApiDumpInstance::current().object_name_map.find((uint64_t) object);
        if (it != ApiDumpInstance::current().object_name_map.end()) {{
            settings.stream() << "</div><div class='val'>[" << it->second << "]";
        }}
    }} else {{
        settings.stream() << "address";
    }}
    return settings.stream() << "</div></summary>";
}}
@end handle

//=========================== Enum Implementations ==========================//

@foreach enum
std::ostream& dump_html_{enumName}({enumName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    switch((int64_t) object)
    {{
    @foreach option
    case {optValue}:
        settings.stream() << "{optName} (";
        break;
    @end option
    default:
        settings.stream() << "UNKNOWN (";
    }}
    return settings.stream() << object << ")</div></summary>";
}}
@end enum

//========================= Bitmask Implementations =========================//

@foreach bitmask
std::ostream& dump_html_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>";
    bool is_first = true;
    settings.stream() << object;
    @foreach option
        @if('{optMultiValue}' != 'None')
            if(object == {optValue})
                is_first = dump_html_bitmaskOption("{optName}", settings.stream(), is_first);
        @end if
        @if('{optMultiValue}' == 'None')
            if(object & {optValue})
                is_first = dump_html_bitmaskOption("{optName}", settings.stream(), is_first);
        @end if
    @end option
    if(!is_first)
        settings.stream() << ")";
    return settings.stream() << "</div></summary>";
}}
@end bitmask

//=========================== Flag Implementations ==========================//

@foreach flag where('{flagEnum}' != 'None')
inline std::ostream& dump_html_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    return dump_html_{flagEnum}(({flagEnum}) object, settings, indents);
}}
@end flag
@foreach flag where('{flagEnum}' == 'None')
inline std::ostream& dump_html_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "<div class=\'val\'>"
                             << object << "</div></summary>";
}}
@end flag

//======================= Func Pointer Implementations ======================//

@foreach funcpointer
inline std::ostream& dump_html_{pfnName}({pfnName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>";
    if(settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";
    return settings.stream() << "</div></summary>";
}}
@end funcpointer

//========================== Struct Implementations =========================//

@foreach struct where('{sctName}' not in ['VkPhysicalDeviceMemoryProperties' ,'VkPhysicalDeviceGroupProperties'])
std::ostream& dump_html_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents{sctConditionVars})
{{
    settings.stream() << "<div class=\'val\'>";
    if(settings.showAddress())
        settings.stream() << &object << "\\n";
    else
        settings.stream() << "address\\n";
    settings.stream() << "</div></summary>";

    @foreach member
    @if('{memCondition}' != 'None')
    if({memCondition})
    @end if

    @if({memPtrLevel} == 0)
        @if('{memName}' != 'pNext')
    dump_html_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions});
        @end if
        @if('{memName}' == 'pNext')
    if(object.pNext != nullptr){{
        dump_html_pNext_trampoline(object.{memName}, settings, indents + 1);
    }} else {{
        dump_html_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions});
    }}
        @end if
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_html_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_html_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions}); // ZRR
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
    @if('{memLength}'[0].isdigit() or '{memLength}'[0].isupper())
    dump_html_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions}); // ZRS
    @end if
    @if(not ('{memLength}'[0].isdigit() or '{memLength}'[0].isupper()))
    @if('{memLength}' == 'rasterizationSamples')
    dump_html_array<const {memBaseType}>(object.{memName}, (object.{memLength} + 31) / 32, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions}); // ZRT
    @end if
    @if('{memLength}' != 'rasterizationSamples')
    dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions}); // ZRT
    @end if
    @end if
    @end if
    @if('{sctName}' == 'VkShaderModuleCreateInfo')
    @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}{memInheritedConditions}); // ZRU
    else
        dump_html_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end if

    @if('{memCondition}' != 'None')
    else
        dump_html_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end member
    return settings.stream();
}}
@end struct

std::ostream& dump_html_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    if(settings.showAddress())
        settings.stream() << &object << "\\n";
    else
        settings.stream() << "address\\n";
    settings.stream() << "</div></summary>";
    dump_html_value<const uint32_t>(object.memoryTypeCount, settings, "uint32_t", "memoryTypeCount", indents + 1, dump_html_uint32_t);
    dump_html_array<const VkMemoryType>(object.memoryTypes, object.memoryTypeCount, settings, "VkMemoryType[VK_MAX_MEMORY_TYPES]", "VkMemoryType", "memoryTypes", indents + 1, dump_html_VkMemoryType); // ZRV
    dump_html_value<const uint32_t>(object.memoryHeapCount, settings, "uint32_t", "memoryHeapCount", indents + 1, dump_html_uint32_t);
    dump_html_array<const VkMemoryHeap>(object.memoryHeaps, object.memoryHeapCount, settings, "VkMemoryHeap[VK_MAX_MEMORY_HEAPS]", "VkMemoryHeap", "memoryHeaps", indents + 1, dump_html_VkMemoryHeap); // ZRW
    return settings.stream();
}}

std::ostream& dump_html_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    if(settings.showAddress())
        settings.stream() << &object << "\\n";
    else
        settings.stream() << "address\\n";
    settings.stream() << "</div></summary>";
    dump_html_value<const VkStructureType>(object.sType, settings, "VkStructureType", "sType", indents + 1, dump_html_VkStructureType);
    dump_html_value<const void*>(object.pNext, settings, "void*", "pNext", indents + 1, dump_html_void);
    dump_html_value<const uint32_t>(object.physicalDeviceCount, settings, "uint32_t", "physicalDeviceCount", indents + 1, dump_html_uint32_t);
    dump_html_array<const VkPhysicalDevice>(object.physicalDevices, object.physicalDeviceCount, settings, "VkPhysicalDevice[VK_MAX_DEVICE_GROUP_SIZE]", "VkPhysicalDevice", "physicalDevices", indents + 1, dump_html_VkPhysicalDevice); // ZRX
    dump_html_value<const VkBool32>(object.subsetAllocation, settings, "VkBool32", "subsetAllocation", indents + 1, dump_html_VkBool32);
    return settings.stream();
}}

//========================== Union Implementations ==========================//

@foreach union
std::ostream& dump_html_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    if(settings.showAddress())
        settings.stream() << &object << " (Union):\\n";
    else
        settings.stream() << "address (Union):\\n";
    settings.stream() << "</div></summary>";

    @foreach choice
    @if({chcPtrLevel} == 0)
    dump_html_value<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 1, dump_html_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' == 'None')
    dump_html_pointer<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 1, dump_html_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' != 'None')
    dump_html_array<const {chcBaseType}>(object.{chcName}, {chcLength}, settings, "{chcType}", "{chcChildType}", "{chcName}", indents + 1, dump_html_{chcTypeID}); // ZRY
    @end if
    @end choice
    return settings.stream();
}}
@end union

//========================= Function Implementations ========================//

@foreach function where('{funcName}' not in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
std::ostream& dump_html_head_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());
    if (settings.showThreadAndFrame()){{
        settings.stream() << "<div class='thd'>Thread: " << dump_inst.threadID() << "</div>";
    }}
    if(settings.showTimestamp())
        settings.stream() << "<div class='time'>Time: " << dump_inst.current_time_since_start().count() << " us</div>";
    settings.stream() << "<details class='fn'><summary>";
    dump_html_nametype(settings.stream(), settings.showType(), "{funcName}({funcNamedParams})", "{funcReturn}");

    return settings.shouldFlush() ? settings.stream() << std::flush : settings.stream();
}}
@end function

@foreach function where('{funcName}' not in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
@if('{funcReturn}' != 'void')
std::ostream& dump_html_body_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
@end if
@if('{funcReturn}' == 'void')
std::ostream& dump_html_body_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
@end if
{{
    const ApiDumpSettings& settings(dump_inst.settings());

    @if('{funcReturn}' != 'void')
    dump_html_{funcReturn}(result, settings, 0);
    @end if
    settings.stream() << "</summary>";

    if(settings.showParams())
    {{
        @foreach parameter
        @if({prmPtrLevel} == 0)
        dump_html_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID}{prmInheritedConditions});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_html_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID}{prmInheritedConditions});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_html_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_html_{prmTypeID}{prmInheritedConditions}); // ZRZ
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";

    return settings.stream() << "</details>";
}}
@end function
"""

# This JSON Codegen is essentially copied from the HTML section above.

JSON_CODEGEN = """
/* Copyright (c) 2015-2019, 2019 Valve Corporation
 * Copyright (c) 2015-2019, 2019 LunarG, Inc.
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

@foreach struct
std::ostream& dump_json_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents{sctConditionVars});
@end struct
@foreach union
std::ostream& dump_json_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

//============================= typedefs ==============================//

// Functions for dumping typedef types that the codegen scripting can't handle
#if defined(VK_ENABLE_BETA_EXTENSIONS)
std::ostream& dump_json_VkAccelerationStructureTypeKHR(VkAccelerationStructureTypeKHR object, const ApiDumpSettings& settings, int indents);
std::ostream& dump_json_VkAccelerationStructureTypeNV(VkAccelerationStructureTypeNV object, const ApiDumpSettings& settings, int indents)
{{
    return dump_json_VkAccelerationStructureTypeKHR(object, settings, indents);
}}
std::ostream& dump_json_VkBuildAccelerationStructureFlagsKHR(VkBuildAccelerationStructureFlagsKHR object, const ApiDumpSettings& settings, int indents);
inline std::ostream& dump_json_VkBuildAccelerationStructureFlagsNV(VkBuildAccelerationStructureFlagsNV object, const ApiDumpSettings& settings, int indents)
{{
    return dump_json_VkBuildAccelerationStructureFlagsKHR(object, settings, indents);
}}
#endif // VK_ENABLE_BETA_EXTENSIONS


//======================== pNext Chain Implementation =======================//

std::ostream& dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) (static_cast<const VkBaseInStructure*>(object)->sType)) {{
    @foreach struct where('{sctName}' not in ['VkPipelineViewportStateCreateInfo', 'VkCommandBufferBeginInfo'])
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        dump_json_pNext<const {sctName}>(static_cast<const {sctName}*>(object), settings, "{sctName}", indents, dump_json_{sctName});
        break;
        @end if
    @end struct

    case 47: // VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO
    case 48: // VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO
        if(static_cast<const VkBaseInStructure*>(object)->pNext != nullptr){{
            dump_json_pNext_trampoline(static_cast<const void*>(static_cast<const VkBaseInStructure*>(object)->pNext), settings, indents);
        }} else {{
            settings.stream() << settings.indentation(indents) << "{{\\n";
            settings.stream() << settings.indentation(indents + 1) << "\\"type\\" : \\"const void*\\",\\n";
            settings.stream() << settings.indentation(indents + 1) << "\\"name\\" : \\"pNext\\",\\n";
            settings.stream() << settings.indentation(indents + 1) << "\\"value\\" : \\"NULL\\"\\n";
            settings.stream() << settings.indentation(indents) << "}}";
        }}
        break;
    default:
        settings.stream() << settings.indentation(indents) << "{{\\n";
        settings.stream() << settings.indentation(indents + 1) << "\\"type\\" : \\"const void*\\",\\n";
        settings.stream() << settings.indentation(indents + 1) << "\\"name\\" : \\"pNext\\",\\n";
        settings.stream() << settings.indentation(indents + 1) << "\\"value\\" : \\"UNKNOWN (\\"" << (int64_t) (static_cast<const VkBaseInStructure*>(object)->sType) << "\\")\\n";
        settings.stream() << settings.indentation(indents) << "}}";
    }}
    return settings.stream();
}}

inline std::ostream& dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents, bool is_dynamic_viewport, bool is_dynamic_scissor)
{{
    dump_json_pNext<const VkPipelineViewportStateCreateInfo>(static_cast<const VkPipelineViewportStateCreateInfo*>(object), settings, "VkPipelineViewportStateCreateInfo", indents, dump_json_VkPipelineViewportStateCreateInfo, is_dynamic_viewport, is_dynamic_scissor);
    return settings.stream();
}}

inline std::ostream& dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents, VkCommandBuffer cmd_buffer)
{{
    dump_json_pNext<const VkCommandBufferBeginInfo>(static_cast<const VkCommandBufferBeginInfo*>(object), settings, "VkCommandBufferBeginInfo", indents, dump_json_VkCommandBufferBeginInfo, cmd_buffer);
    return settings.stream();
}}

//=========================== Type Implementations ==========================//

@foreach type where('{etyName}' != 'void')
inline std::ostream& dump_json_{etyName}({etyName} object, const ApiDumpSettings& settings, int indents)
{{

    //settings.stream() << settings.indentation(indents);
    @if('{etyName}' != 'uint8_t')
    return settings.stream() << "\\"" << object << "\\"";
    @end if
    @if('{etyName}' == 'uint8_t')
    return settings.stream() << "\\"" << (uint32_t) object << "\\"";
    @end if
}}
@end type

//========================= Basetype Implementations ========================//

@foreach basetype where(not '{baseName}' in ['ANativeWindow', 'AHardwareBuffer', 'CAMetalLayer'])
inline std::ostream& dump_json_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "\\"" << object << "\\"";
}}
@end basetype
@foreach basetype where('{baseName}' in ['ANativeWindow', 'AHardwareBuffer'])
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline std::ostream& dump_json_{baseName}(const {baseName}* object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "\\"" << object << "\\"";
}}
#endif
@end basetype
@foreach basetype where('{baseName}' in ['CAMetalLayer'])
#if defined(VK_USE_PLATFORM_METAL_EXT)
inline std::ostream& dump_json_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "\\"" << object << "\\"";
}}
#endif
@end basetype

//======================= System Type Implementations =======================//

@foreach systype
inline std::ostream& dump_json_{sysName}(const {sysType} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "\\"" << object << "\\"";
}}
@end systype

//========================== Handle Implementations =========================//

@foreach handle
inline std::ostream& dump_json_{hdlName}(const {hdlName} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress()) {{
        return settings.stream() << "\\"" << object << "\\"";
    }} else {{
        return settings.stream() << "\\"address\\"";
    }}
    return settings.stream();
}}
@end handle

//=========================== Enum Implementations ==========================//

@foreach enum
std::ostream& dump_json_{enumName}({enumName} object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) object)
    {{
    @foreach option
    case {optValue}:
        settings.stream() << "\\"{optName}\\"";
        break;
    @end option
    default:
        settings.stream() << "\\"UNKNOWN (" << object << ")\\"";
    }}
    return settings.stream();
}}
@end enum

//========================= Bitmask Implementations =========================//

@foreach bitmask
std::ostream& dump_json_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    bool is_first = true;
    settings.stream() << '"' << object;
    if (object)
        settings.stream() << ' ';
    @foreach option
        @if('{optMultiValue}' != 'None')
            if(object == {optValue})
                is_first = dump_json_bitmaskOption("{optName}", settings.stream(), is_first);
        @end if
        @if('{optMultiValue}' == 'None')
            if(object & {optValue})
                is_first = dump_json_bitmaskOption("{optName}", settings.stream(), is_first);
        @end if
    @end option
    if(!is_first)
        settings.stream() << ')';
    return settings.stream() << "\\"";
}}
@end bitmask

//=========================== Flag Implementations ==========================//

@foreach flag where('{flagEnum}' != 'None')
inline std::ostream& dump_json_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    return dump_json_{flagEnum}(({flagEnum}) object, settings, indents);
}}
@end flag
@foreach flag where('{flagEnum}' == 'None')
inline std::ostream& dump_json_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << '"' << object << "\\"";
}}
@end flag

//======================= Func Pointer Implementations ======================//

@foreach funcpointer
inline std::ostream& dump_json_{pfnName}({pfnName} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
       settings.stream() << "\\"" << object << "\\"";
    else
        settings.stream() << "\\"address\\"";
    return settings.stream();
}}
@end funcpointer

//========================== Struct Implementations =========================//

@foreach struct where('{sctName}' not in ['VkPhysicalDeviceMemoryProperties' ,'VkPhysicalDeviceGroupProperties'])
std::ostream& dump_json_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents{sctConditionVars})
{{
    settings.stream() << settings.indentation(indents) << "[\\n";

    bool needMemberComma = false;
    @foreach member
    if (needMemberComma) settings.stream() << ",\\n";
    @if('{memCondition}' != 'None')
    if({memCondition})
    @end if

    @if({memPtrLevel} == 0)
        @if('{memName}' != 'pNext')
    dump_json_value<const {memBaseType}>(object.{memName}, NULL, settings, "{memType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions});
        @end if
        @if('{memName}' == 'pNext')
    if(object.pNext != nullptr){{
        dump_json_pNext_trampoline(object.{memName}, settings, indents + 1);
    }} else {{
        dump_json_value<const {memBaseType}>(object.{memName}, object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions});
    }}
        @end if
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_json_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_json_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions}); // IQA
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
    @if('{memLength}'[0].isdigit() or '{memLength}'[0].isupper())
    dump_json_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions}); // JQA
    @end if
    @if(not ('{memLength}'[0].isdigit() or '{memLength}'[0].isupper()))
    @if('{memLength}' == 'rasterizationSamples')
    dump_json_array<const {memBaseType}>(object.{memName}, (object.{memLength} + 31) / 32, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions}); // JQA
    @end if
    @if('{memLength}' != 'rasterizationSamples')
    dump_json_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions}); // JQA
    @end if
    @end if
    @end if
    @if('{sctName}' == 'VkShaderModuleCreateInfo')
    @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_json_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_json_{memTypeID}{memInheritedConditions}); // KQA
    else
        dump_json_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end if

    @if('{memCondition}' != 'None')
    else
    {{
        settings.stream() << settings.indentation(indents+1) << "{{\\n";
        settings.stream() << settings.indentation(indents+2) << "\\"type\\" : \\"{memType}\\",\\n";
        settings.stream() << settings.indentation(indents+2) << "\\"name\\" : \\"{memName}\\",\\n";
        settings.stream() << settings.indentation(indents+2) << "\\"address\\" : \\"UNUSED\\",\\n";
        settings.stream() << settings.indentation(indents+2) << "\\"value\\" : \\"UNUSED\\"\\n";
        settings.stream() << settings.indentation(indents+1) << "}}";
    }}
    @end if
    needMemberComma = true;
    @end member
    settings.stream() << "\\n" << settings.indentation(indents) << "]";
    return settings.stream();
}}
@end struct

bool is_struct(const char *t)
{{
    char *tm = (char*)t;
    size_t tmlen;
    if (strncmp(tm, "const ", 6) == 0) tm = tm + 6;
    tmlen = strcspn(tm, "[*");
@foreach struct
    if (strncmp("{sctName}", tm, tmlen) == 0 && strlen("{sctName}") == tmlen) return true;
@end struct
    return false;
}}

std::ostream& dump_json_VkPhysicalDeviceMemoryProperties(const VkPhysicalDeviceMemoryProperties& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << settings.indentation(indents) << "[\\n";

    dump_json_value<const uint32_t>(object.memoryTypeCount, NULL, settings, "uint32_t", "memoryTypeCount", indents + 1, dump_json_uint32_t);
    settings.stream() << ",\\n";
    dump_json_array<const VkMemoryType>(object.memoryTypes, object.memoryTypeCount, settings, "VkMemoryType[VK_MAX_MEMORY_TYPES]", "VkMemoryType", "memoryTypes", indents + 1, dump_json_VkMemoryType); // LQA
    settings.stream() << ",\\n";
    dump_json_value<const uint32_t>(object.memoryHeapCount, NULL, settings, "uint32_t", "memoryHeapCount", indents + 1, dump_json_uint32_t);
    settings.stream() << ",\\n";
    dump_json_array<const VkMemoryHeap>(object.memoryHeaps, object.memoryHeapCount, settings, "VkMemoryHeap[VK_MAX_MEMORY_HEAPS]", "VkMemoryHeap", "memoryHeaps", indents + 1, dump_json_VkMemoryHeap); // MQA
    settings.stream() << "\\n" << settings.indentation(indents) << "]";
    return settings.stream();
}}

std::ostream& dump_json_VkPhysicalDeviceGroupProperties(const VkPhysicalDeviceGroupProperties& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << settings.indentation(indents) << "[\\n";

    dump_json_value<const VkStructureType>(object.sType, NULL, settings, "VkStructureType", "sType", indents + 1, dump_json_VkStructureType);
    settings.stream() << ",\\n";
    dump_json_value<const void*>(object.pNext, object.pNext, settings, "void*", "pNext", indents + 1, dump_json_void);
    settings.stream() << ",\\n";
    dump_json_value<const uint32_t>(object.physicalDeviceCount, NULL, settings, "uint32_t", "physicalDeviceCount", indents + 1, dump_json_uint32_t);
    settings.stream() << ",\\n";
    dump_json_array<const VkPhysicalDevice>(object.physicalDevices, object.physicalDeviceCount, settings, "VkPhysicalDevice[VK_MAX_DEVICE_GROUP_SIZE]", "VkPhysicalDevice", "physicalDevices", indents + 1, dump_json_VkPhysicalDevice); // NQA
    settings.stream() << ",\\n";
    dump_json_value<const VkBool32>(object.subsetAllocation, NULL, settings, "VkBool32", "subsetAllocation", indents + 1, dump_json_VkBool32);
    settings.stream() << "\\n" << settings.indentation(indents) << "]";
    return settings.stream();
}}

//========================== Union Implementations ==========================//
@foreach union
std::ostream& dump_json_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << settings.indentation(indents) << "[\\n";

    bool needChoiceComma = false;
    @foreach choice
    if (needChoiceComma) settings.stream() << ",\\n";

    @if({chcPtrLevel} == 0)
    dump_json_value<const {chcBaseType}>(object.{chcName}, NULL, settings, "{chcType}", "{chcName}", indents + 2, dump_json_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' == 'None')
    dump_json_pointer<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 2, dump_json_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' != 'None')
    dump_json_array<const {chcBaseType}>(object.{chcName}, {chcLength}, settings, "{chcType}", "{chcChildType}", "{chcName}", indents + 2, dump_json_{chcTypeID}); // OQA
    @end if
    needChoiceComma = true;
    @end choice

    settings.stream() << "\\n" << settings.indentation(indents) << "]";
    return settings.stream();
}}
@end union

bool is_union(const char *t)
{{
    char *tm = (char*)t;
    size_t tmlen;
    if (strncmp(tm, "const ", 6) == 0) tm = tm + 6;
    tmlen = strcspn(tm, "[*");
@foreach union
    if (strncmp("{unName}", tm, tmlen) == 0 && strlen("{unName}") == tmlen) return true;
@end union
    return false;
}}

//========================= Function Implementations ========================//

static bool needFuncComma = false;

@foreach function where(not '{funcName}' in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
std::ostream& dump_json_head_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());

    if(dump_inst.firstFunctionCallOnFrame())
        needFuncComma = false;

    if (needFuncComma) settings.stream() << ",\\n";

    // Display apicall name
    settings.stream() << settings.indentation(2) << "{{\\n";
    settings.stream() << settings.indentation(3) << "\\\"name\\\" : \\\"{funcName}\\\",\\n";

    // Display thread info
    if (settings.showThreadAndFrame()){{
        settings.stream() << settings.indentation(3) << "\\\"thread\\\" : \\\"Thread " << dump_inst.threadID() << "\\\",\\n";
    }}

    // Display elapsed time
    if(settings.showTimestamp()) {{
        settings.stream() << settings.indentation(3) << "\\\"time\\\" : \\\""<< dump_inst.current_time_since_start().count() << " us\\\",\\n";
    }}

    // Display return value
    settings.stream() << settings.indentation(3) << "\\\"returnType\\\" : " << "\\\"{funcReturn}\\\",\\n";

    return settings.shouldFlush() ? settings.stream() << std::flush : settings.stream();
}}
@end function

@foreach function where(not '{funcName}' in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
@if('{funcReturn}' != 'void')
std::ostream& dump_json_body_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
@end if
@if('{funcReturn}' == 'void')
std::ostream& dump_json_body_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
@end if
{{
    const ApiDumpSettings& settings(dump_inst.settings());

    @if('{funcReturn}' != 'void')
    settings.stream() << settings.indentation(3) << "\\\"returnValue\\\" : ";
    dump_json_{funcReturn}(result, settings, 0);
    if(settings.showParams())
        settings.stream() << ",";
    settings.stream() << "\\n";
    @end if

    // Display parameter values
    if(settings.showParams())
    {{
        bool needParameterComma = false;

        settings.stream() << settings.indentation(3) << "\\\"args\\\" :\\n";
        settings.stream() << settings.indentation(3) << "[\\n";

        @foreach parameter
        if (needParameterComma) settings.stream() << ",\\n";
        @if({prmPtrLevel} == 0)
        dump_json_value<const {prmBaseType}>({prmName}, NULL, settings, "{prmType}", "{prmName}", 4, dump_json_{prmTypeID}{prmInheritedConditions});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_json_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 4, dump_json_{prmTypeID}{prmInheritedConditions});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_json_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 4, dump_json_{prmTypeID}{prmInheritedConditions}); // PQA
        @end if
        needParameterComma = true;
        @end parameter

        settings.stream() << "\\n" << settings.indentation(3) << "]\\n";
    }}
    settings.stream() << settings.indentation(2) << "}}";
    needFuncComma = true;
    if (settings.shouldFlush()) settings.stream().flush();
    return settings.stream();
}}
@end function
"""

POINTER_TYPES = ['void', 'xcb_connection_t', 'Display', 'SECURITY_ATTRIBUTES', 'ANativeWindow', 'AHardwareBuffer']

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

INHERITED_STATE = {
    'VkPipelineViewportStateCreateInfo': {
        'VkGraphicsPipelineCreateInfo': [
            {
                'name': 'is_dynamic_viewport',
                'type': 'bool',
                'expr':
                    'object.pDynamicState && ' +
                    'std::count(' +
                        'object.pDynamicState->pDynamicStates, ' +
                        'object.pDynamicState->pDynamicStates + object.pDynamicState->dynamicStateCount, ' +
                        'VK_DYNAMIC_STATE_VIEWPORT' +
                    ')',
             },
             {
                'name':'is_dynamic_scissor',
                'type': 'bool',
                'expr':
                    'object.pDynamicState && ' +
                    'std::count(' +
                        'object.pDynamicState->pDynamicStates, ' +
                        'object.pDynamicState->pDynamicStates + object.pDynamicState->dynamicStateCount, ' +
                        'VK_DYNAMIC_STATE_SCISSOR' +
                    ')',
            },
        ],
    },
    'VkCommandBufferBeginInfo': {
        'vkBeginCommandBuffer': [
            {
                'name': 'cmd_buffer',
                'type': 'VkCommandBuffer',
                'expr': 'commandBuffer',
            },
        ],
    },
}

VALIDITY_CHECKS = {
    'VkBufferCreateInfo': {
        'pQueueFamilyIndices': 'object.sharingMode == VK_SHARING_MODE_CONCURRENT',
    },
    'VkCommandBufferBeginInfo': {
        # Tracked state ApiDumpInstance, and inherited cmd_buffer
        'pInheritanceInfo': 'ApiDumpInstance::current().getCmdBufferLevel(cmd_buffer) == VK_COMMAND_BUFFER_LEVEL_SECONDARY',
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
        'pViewports': '!is_dynamic_viewport', # Inherited state variable is_dynamic_viewport
        'pScissors': '!is_dynamic_scissor',   # Inherited state variable is_dynamic_scissor
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
}

class ApiDumpGeneratorOptions(GeneratorOptions):
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


class ApiDumpOutputGenerator(OutputGenerator):

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
        if lengthString is not None:
            if "ename:" in lengthString or "latexmath:" in lengthString:
                lengthString = rootNode.get('altlen')
                # Original form would result in object.(rasterizationSamples + 31)
                # We fix it correctly inside the template. Just set it to `rasterizationSamples` to make it easy
                # to find this situation in the code gen.
                if lengthString == '(rasterizationSamples + 31) / 32':
                    lengthString = 'rasterizationSamples'
        lengths = []
        if lengthString is not None:
            lengths = re.split(',', lengthString)
            lengths = list(filter(('null-terminated').__ne__, lengths))
        if self.arrayLength is None and len(lengths) > 0:
            self.childType = '*'.join(self.type.split('*')[0:-1])
            self.arrayLength = lengths[0]
            self.lengthMember = True

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
        if self.typeID in INHERITED_STATE and parentName in INHERITED_STATE[self.typeID]:
            for states in INHERITED_STATE[self.typeID][parentName]:
                self.inheritedConditions += ', ' + states['expr']

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
        self.width = int(rootNode.get('bitwidth', '32'))

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
            'bitWidth': self.width,
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
        # Some flags definitions set 'bitvalues' instead of 'requires'
        if self.enum is None:
            self.enum = rootNode.get('bitvalues')

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
        if self.name in TRACKED_STATE:
            self.stateTrackingCode = TRACKED_STATE[self.name]

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
            if rootNode.get('noautovalidity') == 'true' and parentName in VALIDITY_CHECKS and self.name in VALIDITY_CHECKS[parentName]:
                self.condition = VALIDITY_CHECKS[parentName][self.name]
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
        if self.name in INHERITED_STATE:
            for parent, states in INHERITED_STATE[self.name].items():
                for state in states:
                    self.conditionVars += ', ' + state['type'] + ' ' + state['name']

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
