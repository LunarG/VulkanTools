#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2023 Valve Corporation
# Copyright (c) 2015-2023 LunarG, Inc.
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
# Author: Charles Giessen <charles@lunarg.com>
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

BLOCKING_API_CALLS = [
    'vkWaitForFences', 'vkWaitSemaphores', 'vkQueuePresentKHR', 'vkDeviceWaitIdle',
    'vkQueueWaitIdle', 'vkAcquireNextImageKHR', 'vkGetQueryPoolResults',
]

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
{{
    ApiDumpInstance::current().outputMutex()->lock();
    ApiDumpInstance::current().initLayerSettings(pCreateInfo, pAllocator);
    dump_function_head(ApiDumpInstance::current(), "vkCreateInstance", "pCreateInfo, pAllocator, pInstance", "VkResult");

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
    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if(result == VK_SUCCESS) {{
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
    }}

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {{
        switch(ApiDumpInstance::current().settings().format())
        {{
            case ApiDumpFormat::Text:
                dump_text_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
            case ApiDumpFormat::Html:
                dump_html_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
            case ApiDumpFormat::Json:
                dump_json_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
                break;
        }}
    }}
    ApiDumpInstance::current().outputMutex()->unlock();
    return result;
}}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{{
    ApiDumpInstance::current().outputMutex()->lock();
    dump_function_head(ApiDumpInstance::current(), "vkCreateDevice", "physicalDevice, pCreateInfo, pAllocator, pDevice", "VkResult");

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
    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if(result == VK_SUCCESS) {{
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }}

    // Output the API dump
    if (ApiDumpInstance::current().shouldDumpOutput()) {{
        switch(ApiDumpInstance::current().settings().format())
        {{
            case ApiDumpFormat::Text:
                dump_text_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
            case ApiDumpFormat::Html:
                dump_html_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
            case ApiDumpFormat::Json:
                dump_json_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
                break;
        }}
    }}
    ApiDumpInstance::current().outputMutex()->unlock();
    return result;
}}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{{
    return util_GetExtensionProperties(0, NULL, pPropertyCount, pProperties);
}}

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
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

EXPORT_FUNCTION VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
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

// Autogen instance functions

@foreach function where('{funcDispatchType}' == 'instance' and '{funcName}' not in ['vkCreateInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties'])
VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    @if('{funcName}' not in BLOCKING_API_CALLS)
    ApiDumpInstance::current().outputMutex()->lock();
    dump_function_head(ApiDumpInstance::current(), "{funcName}", "{funcNamedParams}", "{funcReturn}");
    @end if

    @if('{funcName}' == 'vkGetPhysicalDeviceToolPropertiesEXT')
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
    @end if

    @if('{funcReturn}' != 'void')
    {funcReturn} result = instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    @end if
    @if('{funcReturn}' == 'void')
    instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    @end if
    @if('{funcName}' in BLOCKING_API_CALLS)
    ApiDumpInstance::current().outputMutex()->lock();
    dump_function_head(ApiDumpInstance::current(), "{funcName}", "{funcNamedParams}", "{funcReturn}");
    @end if
    {funcStateTrackingCode}
    @if('{funcName}' == 'vkEnumeratePhysicalDevices')
    if (pPhysicalDeviceCount != nullptr && pPhysicalDevices != nullptr) {{
        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {{
            ApiDumpInstance::current().set_vk_instance(pPhysicalDevices[i], instance);
        }}
    }}
    @end if
    @if('{funcName}' == 'vkDestroyInstance')
    destroy_instance_dispatch_table(get_dispatch_key(instance));
    @end if

    @if('{funcName}' == 'vkGetPhysicalDeviceToolPropertiesEXT')
    if (original_pToolProperties != nullptr) {{
        pToolProperties = original_pToolProperties;
    }}

    (*pToolCount)++;
    @end if

    if (ApiDumpInstance::current().shouldDumpOutput()) {{
        switch(ApiDumpInstance::current().settings().format())
        {{
            @if('{funcReturn}' != 'void')
            case ApiDumpFormat::Text:
                dump_text_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
                break;
            case ApiDumpFormat::Html:
                dump_html_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
                break;
            case ApiDumpFormat::Json:
                dump_json_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
                break;
            @end if
            @if('{funcReturn}' == 'void')
            case ApiDumpFormat::Text:
                dump_text_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
                break;
            case ApiDumpFormat::Html:
                dump_html_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
                break;
            case ApiDumpFormat::Json:
                dump_json_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
                break;
            @end if
        }}
    }}
    ApiDumpInstance::current().outputMutex()->unlock();
    @if('{funcReturn}' != 'void')
    return result;
    @end if
}}
@end function

// Autogen device functions

@foreach function where('{funcDispatchType}' == 'device' and '{funcName}' not in ['vkGetDeviceProcAddr'])
VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    @if('{funcName}' not in BLOCKING_API_CALLS)
    ApiDumpInstance::current().outputMutex()->lock();
    @if('{funcName}' in ['vkDebugMarkerSetObjectNameEXT', 'vkSetDebugUtilsObjectNameEXT'])
    ApiDumpInstance::current().update_object_name_map(pNameInfo);
    @end if
    dump_function_head(ApiDumpInstance::current(), "{funcName}", "{funcNamedParams}", "{funcReturn}");
    @end if

    @if('{funcReturn}' != 'void')
    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    @end if
    @if('{funcReturn}' == 'void')
    device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    @end if
    @if('{funcName}' in BLOCKING_API_CALLS)
    ApiDumpInstance::current().outputMutex()->lock();
    dump_function_head(ApiDumpInstance::current(), "{funcName}", "{funcNamedParams}", "{funcReturn}");
    @end if
    {funcStateTrackingCode}
    @if('{funcName}' == 'vkDestroyDevice')
    destroy_device_dispatch_table(get_dispatch_key(device));
    @end if

    if (ApiDumpInstance::current().shouldDumpOutput()) {{
        switch(ApiDumpInstance::current().settings().format())
        {{
            @if('{funcReturn}' != 'void')
            case ApiDumpFormat::Text:
                dump_text_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
                break;
            case ApiDumpFormat::Html:
                dump_html_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
                break;
            case ApiDumpFormat::Json:
                dump_json_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
                break;
            @end if
            @if('{funcReturn}' == 'void')
            case ApiDumpFormat::Text:
                dump_text_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
                break;
            case ApiDumpFormat::Html:
                dump_html_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
                break;
            case ApiDumpFormat::Json:
                dump_json_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
                break;
            @end if
        }}
    }}
    ApiDumpInstance::current().outputMutex()->unlock();
    @if('{funcName}' == 'vkQueuePresentKHR')
    ApiDumpInstance::current().nextFrame();
    @end if
    @if('{funcReturn}' != 'void')
    return result;
    @end if
}}
@end function

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL api_dump_known_instance_functions(VkInstance instance, const char* pName)
{{
    @foreach function where('{funcType}' in ['global', 'instance'] and '{funcName}' not in [ 'vkEnumerateDeviceExtensionProperties' ])
    @if('${funcDispatchType}' == 'instance')
    if(strcmp(pName, "{funcName}") == 0 && (!instance || instance_dispatch_table(instance)->{funcShortName}))
    @end if
    @if('${funcDispatchType}' != 'instance')
    if(strcmp(pName, "{funcName}") == 0)
    @end if
        return reinterpret_cast<PFN_vkVoidFunction>({funcName});
    @end function

    return nullptr;
}}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL api_dump_known_device_functions(VkDevice device, const char* pName)
{{
    @foreach function where('{funcType}' == 'device')
    if(strcmp(pName, "{funcName}") == 0 && (!device || device_dispatch_table(device)->{funcShortName}))
        return reinterpret_cast<PFN_vkVoidFunction>({funcName});
    @end function

    return nullptr;
}}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{{
    auto instance_func = api_dump_known_instance_functions(instance, pName);
    if (instance_func) return instance_func;

    // Make sure that device functions queried through GIPA works
    auto device_func = api_dump_known_device_functions(NULL, pName);
    if (device_func) return device_func;

    // Haven't created an instance yet, exit now since there is no instance_dispatch_table
    if(instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return nullptr;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}}

EXPORT_FUNCTION VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName)
{{
    auto device_func = api_dump_known_device_functions(device, pName);
    if (device_func) return device_func;

    // Haven't created a device yet, exit now since there is no device_dispatch_table
    if(device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return nullptr;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}}
"""

TEXT_CODEGEN = """
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
#include "api_dump_video_text.h"
@if(not {isVideoGeneration})
void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents, const char* pnext_type);
void dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);
@end if
@foreach union
void dump_text_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

//=========================== Type Implementations ==========================//

@foreach type where('{etyName}' != 'void')
void dump_text_{etyName}({etyName} object, const ApiDumpSettings& settings, int indents)
{{
    @if('{etyName}' != 'uint8_t' and '{etyName}' != 'int8_t')
    settings.stream() << object;
    @end if
    @if('{etyName}' == 'uint8_t')
    settings.stream() << (uint32_t) object;
    @end if
    @if('{etyName}' == 'int8_t')
    settings.stream() << (int32_t) object;
    @end if
}}
@end type

//========================= Basetype Implementations ========================//

@foreach basetype where(not '{baseName}' in ['ANativeWindow', 'AHardwareBuffer', 'CAMetalLayer'])
void dump_text_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << object;
}}
@end basetype
@foreach basetype where('{baseName}' in ['ANativeWindow', 'AHardwareBuffer'])
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_text_{baseName}(const {baseName}* object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << object;
}}
#endif
@end basetype
@foreach basetype where('{baseName}' in ['CAMetalLayer'])
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_text_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << object;
}}
#endif
@end basetype

//======================= System Type Implementations =======================//

@foreach systype
void dump_text_{sysName}(const {sysType} object, const ApiDumpSettings& settings, int indents)
{{
    @if({sysNeedsPointer} == True)
    if (object == NULL) {{
        settings.stream() << "NULL";
        return;
    }}
    OutputAddress(settings, object);
    @end if
    @if({sysNeedsPointer} == False)
    if (settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";
    @end if
}}
@end systype

//========================== Handle Implementations =========================//

@foreach handle
void dump_text_{hdlName}(const {hdlName} object, const ApiDumpSettings& settings, int indents)
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
}}
@end handle

//=========================== Enum Implementations ==========================//

@foreach enum
void dump_text_{enumName}({enumName} object, const ApiDumpSettings& settings, int indents)
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
    settings.stream() << object << ")";
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
void dump_text_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    bool is_first = true;
    settings.stream() << object;
    @foreach option
        @if('{optMultiValue}' != 'None')
    if(object == {optValue}) {{
        @end if
        @if('{optMultiValue}' == 'None')
    if(object & {optValue}) {{
        @end if
        settings.stream() << (is_first ? \" (\" : \" | \") << "{optName}"; is_first = false;
    }}
    @end option
    if(!is_first)
        settings.stream() << ")";
}}
@end bitmask

//=========================== Flag Implementations ==========================//

@foreach flag where('{flagEnum}' != 'None')
void dump_text_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    dump_text_{flagEnum}(({flagEnum}) object, settings, indents);
}}
@end flag
@foreach flag where('{flagEnum}' == 'None')
void dump_text_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << object;
}}
@end flag

//======================= Func Pointer Implementations ======================//

@foreach funcpointer
void dump_text_{pfnName}({pfnName} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";
}}
@end funcpointer

//========================== Struct Implementations =========================//

@foreach struct
void dump_text_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        settings.stream() << &object << ":\\n";
    else
        settings.stream() << "address:\\n";

    @foreach member
        @if('{memParameterStorage}' != '' and '{memCondition}' != 'None')
    if({memCondition})
        {memParameterStorage}
        @end if
        @if('{memParameterStorage}' != '' and '{memCondition}' == 'None')
    {memParameterStorage}
        @end if
    @end member

    @foreach member
        @if('{memCondition}' != 'None')
    if({memCondition})
        @end if
        @if({memPtrLevel} == 0)
            @if('{memName}' != 'pNext')
    dump_text_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID});  // AET
            @end if
            @if('{memName}' == 'pNext')
    dump_text_pNext_struct_name(object.{memName}, settings, indents + 1, "{memType}");
            @end if
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_text_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID});
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_text_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}); // AQA
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
            @if('{memLength}'[0].isdigit() or '{memLength}'[0].isupper())
    dump_text_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}); // BQA
            @end if
            @if(not ('{memLength}'[0].isdigit() or '{memLength}'[0].isupper()))
                @if('{memLength}' == 'rasterizationSamples')
    dump_text_array<const {memBaseType}>(object.{memName}, (object.{memLength} + 31) / 32, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}); // BQB
                @end if
                @if('{memLength}' != 'rasterizationSamples')
    dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}); // BQB
                @end if
            @end if
        @end if

        @if('{sctName}' == 'VkShaderModuleCreateInfo')
            @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID}); // CQA
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
}}
@end struct

//========================== Union Implementations ==========================//

@foreach union
void dump_text_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
        settings.stream() << &object << " (Union):\\n";
    else
        settings.stream() << "address (Union):\\n";

    @foreach choice
    @if('{chcCondition}' != 'None')
    if({chcCondition})
    @end if
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
}}
@end union

//======================== pNext Chain Implementation =======================//
@if(not {isVideoGeneration})
void dump_text_pNext_struct_name(const void* object, const ApiDumpSettings& settings, int indents, const char* pnext_type)
{{
    if (object == nullptr) {{
        dump_text_value<const void*>(object, settings, pnext_type, "pNext", indents, dump_text_void);
        return;
    }}

    settings.formatNameType(indents, "pNext", pnext_type);
    switch(reinterpret_cast<const VkBaseInStructure*>(object)->sType) {{
    @foreach struct
        @if({sctStructureTypeIndex} != -1)
        case {sctStructureTypeIndex}:
            settings.stream() << "{sctName}\\n";
            break;
        @end if
    @end struct
        case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47
        case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48
        default:
            settings.stream() << "NULL\\n";
            break;
    }}
}}

void dump_text_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)
{{
    const auto* base_struct = reinterpret_cast<const VkBaseInStructure*>(object);
    switch(base_struct->sType) {{
    @foreach struct
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        dump_text_pNext<const {sctName}>(reinterpret_cast<const {sctName}*>(object), settings, "{sctName}", indents, dump_text_{sctName});
        break;
        @end if
    @end struct

    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47
    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48
        if(base_struct->pNext != nullptr){{
            dump_text_pNext_trampoline(reinterpret_cast<const void*>(base_struct->pNext), settings, indents);
        }} else {{
            settings.formatNameType(indents, "pNext", "const void*");
            settings.stream() << "NULL\\n";
        }}
        break;
    default:
        settings.formatNameType(indents, "pNext", "const void*");
        settings.stream() << "UNKNOWN (" << (int64_t) (base_struct->sType) << ")\\n";
    }}
}}
@end if
//========================= Function Implementations ========================//

@foreach function where('{funcName}' not in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
@if('{funcReturn}' != 'void')
void dump_text_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
@end if
@if('{funcReturn}' == 'void')
void dump_text_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
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
        @if('{prmParameterStorage}' != '')
        {prmParameterStorage}
        @end if
        @if({prmPtrLevel} == 0)
        dump_text_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID}); // MET
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_text_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_text_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_text_{prmTypeID}); // HQA
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";
}}
@end function

"""

# This HTML Codegen is essentially copied from the format above.
# Due to the way some of the functions have been organized, some of the HTML tags
# that are opened are closed in another function. See api_dump.h. This may need refactoring.

HTML_CODEGEN = """
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
#include "api_dump_video_html.h"
@if(not {isVideoGeneration})
void dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);
@end if
@foreach union
void dump_html_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

//=========================== Type Implementations ==========================//

@foreach type where('{etyName}' != 'void')
void dump_html_{etyName}({etyName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    @if('{etyName}' != 'uint8_t' and '{etyName}' != 'int8_t')
    settings.stream() << object;
    @end if
    @if('{etyName}' == 'uint8_t')
    settings.stream() << (uint32_t) object;
    @end if
    @if('{etyName}' == 'int8_t')
    settings.stream() << (int32_t) object;
    @end if
    settings.stream() << "</div></summary>";
}}
@end type

//========================= Basetype Implementations ========================//

@foreach basetype where(not '{baseName}' in ['ANativeWindow', 'AHardwareBuffer', 'CAMetalLayer'])
void dump_html_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
@end basetype
@foreach basetype where('{baseName}' in ['ANativeWindow', 'AHardwareBuffer'])
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_html_{baseName}(const {baseName}* object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
#endif
@end basetype
@foreach basetype where('{baseName}' in ['CAMetalLayer'])
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_html_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
#endif
@end basetype

//======================= System Type Implementations =======================//

@foreach systype
void dump_html_{sysName}(const {sysType} object, const ApiDumpSettings& settings, int indents)
{{
    @if({sysNeedsPointer} == True)
    settings.stream() << "<div class='val'>";
    OutputAddress(settings, object);
    settings.stream() << "</div>";
    @end if
    @if({sysNeedsPointer} == False)
    if (settings.showAddress())
        settings.stream() << "<div class='val'>" << object << "</div></summary>";
    else
        settings.stream() << "<div class='val'>address</div></summary>";
    @end if
}}
@end systype

//========================== Handle Implementations =========================//

@foreach handle
void dump_html_{hdlName}(const {hdlName} object, const ApiDumpSettings& settings, int indents)
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
    settings.stream() << "</div></summary>";
}}
@end handle

//=========================== Enum Implementations ==========================//

@foreach enum
void dump_html_{enumName}({enumName} object, const ApiDumpSettings& settings, int indents)
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
    settings.stream() << object << ")</div></summary>";
}}
@end enum

//========================= Bitmask Implementations =========================//

@foreach bitmask
void dump_html_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>";
    bool is_first = true;
    settings.stream() << object;
    @foreach option
        @if('{optMultiValue}' != 'None')
    if(object == {optValue}) {{
        @end if
        @if('{optMultiValue}' == 'None')
    if(object & {optValue}) {{
        @end if
        settings.stream() << (is_first ? \" (\" : \" | \") << "{optName}"; is_first = false;
    }}
    @end option
    if(!is_first)
        settings.stream() << ")";
    settings.stream() << "</div></summary>";
}}
@end bitmask

//=========================== Flag Implementations ==========================//

@foreach flag where('{flagEnum}' != 'None')
void dump_html_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    dump_html_{flagEnum}(({flagEnum}) object, settings, indents);
}}
@end flag
@foreach flag where('{flagEnum}' == 'None')
void dump_html_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>"
                             << object << "</div></summary>";
}}
@end flag

//======================= Func Pointer Implementations ======================//

@foreach funcpointer
void dump_html_{pfnName}({pfnName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>";
    if(settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";
    settings.stream() << "</div></summary>";
}}
@end funcpointer

//========================== Struct Implementations =========================//

@foreach struct
void dump_html_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class=\'val\'>";
    if(settings.showAddress())
        settings.stream() << &object << "\\n";
    else
        settings.stream() << "address\\n";
    settings.stream() << "</div></summary>";

    @foreach member
        @if('{memParameterStorage}' != '' and '{memCondition}' != 'None')
    if({memCondition})
        {memParameterStorage}
        @end if
        @if('{memParameterStorage}' != '' and '{memCondition}' == 'None')
    {memParameterStorage}
        @end if
    @end member

    @foreach member
        @if('{memCondition}' != 'None')
    if({memCondition})
        @end if
        @if({memPtrLevel} == 0)
            @if('{memName}' != 'pNext')
    dump_html_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
            @end if
            @if('{memName}' == 'pNext')
    if(object.pNext != nullptr){{
        dump_html_pNext_trampoline(object.{memName}, settings, indents + 1);
    }} else {{
        dump_html_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
    }}
            @end if
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_html_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_html_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}); // ZRR
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
            @if('{memLength}'[0].isdigit() or '{memLength}'[0].isupper())
    dump_html_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}); // ZRS
            @end if
            @if(not ('{memLength}'[0].isdigit() or '{memLength}'[0].isupper()))
                @if('{memLength}' == 'rasterizationSamples')
    dump_html_array<const {memBaseType}>(object.{memName}, (object.{memLength} + 31) / 32, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}); // ZRT
                @end if
                @if('{memLength}' != 'rasterizationSamples')
    dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}); // ZRT
                @end if
            @end if
        @end if
        @if('{sctName}' == 'VkShaderModuleCreateInfo')
            @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID}); // ZRU
    else
        dump_html_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
            @end if
        @end if

        @if('{memCondition}' != 'None')
    else
        dump_html_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
        @end if
    @end member
}}
@end struct

//========================== Union Implementations ==========================//

@foreach union
void dump_html_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
    if(settings.showAddress())
        settings.stream() << &object << " (Union):\\n";
    else
        settings.stream() << "address (Union):\\n";
    settings.stream() << "</div></summary>";

    @foreach choice
    @if('{chcCondition}' != 'None')
    if({chcCondition})
    @end if
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
}}
@end union

//======================== pNext Chain Implementation =======================//
@if(not {isVideoGeneration})
void dump_html_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) (static_cast<const VkBaseInStructure*>(object)->sType)) {{
    @foreach struct
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        dump_html_pNext<const {sctName}>(static_cast<const {sctName}*>(object), settings, "{sctName}", indents, dump_html_{sctName});
        break;
        @end if
    @end struct

    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47
    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48
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
}}
@end if
//========================= Function Implementations ========================//

@foreach function where('{funcName}' not in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
@if('{funcReturn}' != 'void')
void dump_html_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
@end if
@if('{funcReturn}' == 'void')
void dump_html_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
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
        @if('{prmParameterStorage}' != '')
        {prmParameterStorage}
        @end if
        @if({prmPtrLevel} == 0)
        dump_html_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_html_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_html_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_html_{prmTypeID}); // ZRZ
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";

    settings.stream() << "</details>";
}}
@end function
"""

# This JSON Codegen is essentially copied from the HTML section above.

JSON_CODEGEN = """
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
#include "api_dump_video_json.h"
@if(not {isVideoGeneration})
void dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents);
@end if
@foreach union
void dump_json_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

//=========================== Type Implementations ==========================//

@foreach type where('{etyName}' != 'void')
void dump_json_{etyName}({etyName} object, const ApiDumpSettings& settings, int indents)
{{

    @if('{etyName}' != 'uint8_t' and '{etyName}' != 'int8_t')
    settings.stream() << "\\"" << object << "\\"";
    @end if
    @if('{etyName}' == 'uint8_t')
    settings.stream() << "\\"" << (uint32_t) object << "\\"";
    @end if
    @if('{etyName}' == 'int8_t')
    settings.stream() << "\\"" << (int32_t) object << "\\"";
    @end if
}}
@end type

//========================= Basetype Implementations ========================//

@foreach basetype where(not '{baseName}' in ['ANativeWindow', 'AHardwareBuffer', 'CAMetalLayer'])
void dump_json_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "\\"" << object << "\\"";
}}
@end basetype
@foreach basetype where('{baseName}' in ['ANativeWindow', 'AHardwareBuffer'])
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void dump_json_{baseName}(const {baseName}* object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "\\"" << object << "\\"";
}}
#endif
@end basetype
@foreach basetype where('{baseName}' in ['CAMetalLayer'])
#if defined(VK_USE_PLATFORM_METAL_EXT)
void dump_json_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "\\"" << object << "\\"";
}}
#endif
@end basetype

//======================= System Type Implementations =======================//

@foreach systype
void dump_json_{sysName}(const {sysType} object, const ApiDumpSettings& settings, int indents)
{{
    @if({sysNeedsPointer} == True)
    OutputAddressJSON(settings, object);
    settings.stream() << "\\n";
    @end if
    @if({sysNeedsPointer} == False)
    if (settings.showAddress())
        settings.stream() << "\\"" << object << "\\"";
    else
        settings.stream() << "\\"address\\"";
    @end if
}}
@end systype

//========================== Handle Implementations =========================//

@foreach handle
void dump_json_{hdlName}(const {hdlName} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress()) {{
        settings.stream() << "\\"" << object << "\\"";
    }} else {{
        settings.stream() << "\\"address\\"";
    }}
}}
@end handle

//=========================== Enum Implementations ==========================//

@foreach enum
void dump_json_{enumName}({enumName} object, const ApiDumpSettings& settings, int indents)
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
}}
@end enum

//========================= Bitmask Implementations =========================//

@foreach bitmask
void dump_json_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    bool is_first = true;
    settings.stream() << '"' << object;
    @foreach option
        @if('{optMultiValue}' != 'None')
    if(object == {optValue}) {{
        @end if
        @if('{optMultiValue}' == 'None')
    if(object & {optValue}) {{
        @end if
        settings.stream() << (is_first ? \" (\" : \" | \") << "{optName}"; is_first = false;
    }}
    @end option
    if(!is_first)
        settings.stream() << ')';
    settings.stream() << "\\"";
}}
@end bitmask

//=========================== Flag Implementations ==========================//

@foreach flag where('{flagEnum}' != 'None')
void dump_json_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    dump_json_{flagEnum}(({flagEnum}) object, settings, indents);
}}
@end flag
@foreach flag where('{flagEnum}' == 'None')
void dump_json_{flagName}({flagName} object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << '"' << object << "\\"";
}}
@end flag

//======================= Func Pointer Implementations ======================//

@foreach funcpointer
void dump_json_{pfnName}({pfnName} object, const ApiDumpSettings& settings, int indents)
{{
    if(settings.showAddress())
       settings.stream() << "\\"" << object << "\\"";
    else
        settings.stream() << "\\"address\\"";
}}
@end funcpointer

//========================== Struct Implementations =========================//

@foreach struct
void dump_json_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << settings.indentation(indents) << "[\\n";

    @foreach member
        @if('{memParameterStorage}' != '' and '{memCondition}' != 'None')
    if({memCondition})
        {memParameterStorage}
        @end if
        @if('{memParameterStorage}' != '' and '{memCondition}' == 'None')
    {memParameterStorage}
        @end if
    @end member

    @foreach member
        @if({memIndex} != 0)
    settings.stream() << ",\\n";
        @end if
        @if('{memCondition}' != 'None')
    if({memCondition})
        @end if
        @if({memPtrLevel} == 0)
            @if('{memName}' != 'pNext')
    dump_json_value<const {memBaseType}>(object.{memName}, NULL, settings, "{memType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID});
            @end if
            @if('{memName}' == 'pNext')
    if(object.pNext != nullptr){{
        dump_json_pNext_trampoline(object.{memName}, settings, indents + 1);
    }} else {{
        dump_json_value<const {memBaseType}>(object.{memName}, object.{memName}, settings, "{memType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID});
    }}
            @end if
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_json_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID});
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_json_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID}); // IQA
        @end if
        @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
            @if('{memLength}'[0].isdigit() or '{memLength}'[0].isupper())
    dump_json_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID}); // JQA
            @end if
            @if(not ('{memLength}'[0].isdigit() or '{memLength}'[0].isupper()))
                @if('{memLength}' == 'rasterizationSamples')
    dump_json_array<const {memBaseType}>(object.{memName}, (object.{memLength} + 31) / 32, settings, "{memType}", "{memChildType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID}); // JQA
                @end if
                @if('{memLength}' != 'rasterizationSamples')
    dump_json_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID}); // JQA
                @end if
            @end if
        @end if
        @if('{sctName}' == 'VkShaderModuleCreateInfo')
            @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_json_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", {memIsStruct}, {memIsUnion}, indents + 1, dump_json_{memTypeID}); // KQA
    else
        dump_json_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
            @end if
        @end if

        @if('{memCondition}' != 'None')
    else
        dump_json_UNUSED(settings, "{memType}", "{memName}", indents + 1);
        @end if
    @end member
    settings.stream() << "\\n" << settings.indentation(indents) << "]";
}}
@end struct

//========================== Union Implementations ==========================//
@foreach union
void dump_json_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << settings.indentation(indents) << "[\\n";

    @foreach choice
    @if('{chcCondition}' != 'None')
    if({chcCondition})
    @end if
    @if({chcIndex} != 0 and '{chcCondition}' == 'None')
    settings.stream() << ",\\n"; // Only need commas when more than one field is printed
    @end if
    @if({chcPtrLevel} == 0)
    dump_json_value<const {chcBaseType}>(object.{chcName}, NULL, settings, "{chcType}", "{chcName}", {chcIsStruct}, {chcIsUnion}, indents + 2, dump_json_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' == 'None')
    dump_json_pointer<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", {chcIsStruct}, {chcIsUnion}, indents + 2, dump_json_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' != 'None')
    dump_json_array<const {chcBaseType}>(object.{chcName}, {chcLength}, settings, "{chcType}", "{chcChildType}", "{chcName}", {chcIsStruct}, {chcIsUnion}, indents + 2, dump_json_{chcTypeID}); // OQA
    @end if
    @end choice

    settings.stream() << "\\n" << settings.indentation(indents) << "]";
}}
@end union

//======================== pNext Chain Implementation =======================//
@if(not {isVideoGeneration})
void dump_json_pNext_trampoline(const void* object, const ApiDumpSettings& settings, int indents)
{{
    switch((int64_t) (static_cast<const VkBaseInStructure*>(object)->sType)) {{
    @foreach struct
        @if({sctStructureTypeIndex} != -1)
    case {sctStructureTypeIndex}:
        dump_json_pNext<const {sctName}>(static_cast<const {sctName}*>(object), settings, "{sctName}", indents, dump_json_{sctName});
        break;
        @end if
    @end struct

    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: // 47
    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: // 48
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
        settings.stream() << settings.indentation(indents + 1) << "\\"value\\" : \\"UNKNOWN (" << (int64_t) (static_cast<const VkBaseInStructure*>(object)->sType) << ")\\"\\n";
        settings.stream() << settings.indentation(indents) << "}}";
    }}
}}
@end if
//========================= Function Implementations ========================//

@foreach function where(not '{funcName}' in ['vkGetDeviceProcAddr', 'vkGetInstanceProcAddr'])
@if('{funcReturn}' != 'void')
void dump_json_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
@end if
@if('{funcReturn}' == 'void')
void dump_json_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
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
        settings.stream() << settings.indentation(3) << "\\\"args\\\" :\\n";
        settings.stream() << settings.indentation(3) << "[\\n";

        @foreach parameter
        @if({prmIndex} != 0)
        settings.stream() << ",\\n";
        @end if
        @if('{prmParameterStorage}' != '')
        {prmParameterStorage}
        @end if
        @if({prmPtrLevel} == 0)
        dump_json_value<const {prmBaseType}>({prmName}, NULL, settings, "{prmType}", "{prmName}", {prmIsStruct}, {prmIsUnion}, 4, dump_json_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_json_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", {prmIsStruct}, {prmIsUnion}, 4, dump_json_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_json_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", {prmIsStruct}, {prmIsUnion}, 4, dump_json_{prmTypeID}); // PQA
        @end if
        @end parameter

        settings.stream() << "\\n" << settings.indentation(3) << "]\\n";
    }}
    settings.stream() << settings.indentation(2) << "}}";
    if (settings.shouldFlush()) settings.stream().flush();
}}
@end function
"""

POINTER_TYPES = ['void', 'xcb_connection_t', 'Display', 'SECURITY_ATTRIBUTES', 'ANativeWindow', 'AHardwareBuffer', 'wl_display', '_screen_context', '_screen_window', '_screen_buffer']

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
    }
}

# These types are defined in both video.xml and vk.xml. Because duplicate functions aren't allowed,
# we have to prevent these from generating twice. This is done by removing the types from the non-video
# outputs
DUPLICATE_TYPES_IN_VIDEO_HEADER = ['uint32_t', 'uint16_t', 'uint8_t', 'int32_t', 'int16_t', 'int8_t']

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
                 isVideoGeneration = False,
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
        self.isVideoGeneration = isVideoGeneration


class ApiDumpOutputGenerator(OutputGenerator):

    def __init__(self,
                 errFile = sys.stderr,
                 warnFile = sys.stderr,
                 diagFile = sys.stdout,
                 registryFile = None):
        gen.OutputGenerator.__init__(self, errFile, warnFile, diagFile)
        self.format = None
        self.isVideoGeneration = False

        self.constants = {}
        self.extensions = {}
        self.extFuncs = {}
        self.extTypes = {}
        self.includes = {}

        self.sysTypes = {}
        self.basetypes = {}
        self.bitmasks = {}
        self.enums = {}
        self.externalTypes = {}
        self.flags = {}
        self.funcPointers = {}
        self.functions = {}
        self.handles = {}
        self.structs = {}
        self.unions = {}
        self.aliases = {}

        self.registryFile = registryFile

    def beginFile(self, genOpts):
        gen.OutputGenerator.beginFile(self, genOpts)
        self.format = genOpts.input
        self.isVideoGeneration = genOpts.isVideoGeneration

        if self.registryFile is not None:
            root = xml.etree.ElementTree.parse(self.registryFile)
        else:
            root = self.registry.reg

        for node in root.find('extensions').findall('extension'):
            if 'vulkan' in node.get('supported'): # dont print unsupported extensions
                ext = VulkanExtension(node)
                self.extensions[ext.name] = ext
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
        # Find all 'system' types and put it in a set
        sysTypeNames = set()
        for node in self.registry.reg.find('types').findall('type'):
            if node.get('category') is None and node.get('requires') in self.includes and node.get('requires') not in ['vk_platform', 'stdint'] \
                and "vk_video" not in node.get('requires'):
                sysTypeNames.add(node.get('name'))

        # Look through the set of sysTypeName to find all of the extensions that use the system types, then add it to sysTypes
        self.sysTypes = {}
        for sysTypeName in sysTypeNames:
            for extension in self.extTypes.values():
                for typeName in extension.vktypes:
                    if typeName not in self.structs:
                        continue # not all types are structures nor will be enabled
                    for member in self.structs[typeName].members:
                        if sysTypeName == member.baseType or sysTypeName + '*' == member.baseType:
                            self.sysTypes[sysTypeName] = VulkanSystemType(sysTypeName, extension)
                for functionName in extension.vkfuncs:
                    if functionName not in self.functions:
                        continue # not all functions will be enabled
                    for param in self.functions[functionName].parameters:
                        if sysTypeName == param.baseType or sysTypeName + '*' == param.baseType:
                            self.sysTypes[sysTypeName] = VulkanSystemType(sysTypeName, extension)


        # Fill in the is_struct and is_union member variables of the Variable class so that json output can be correct

        for value in self.functions.values():
            for variable in value.parameters:
                if variable.typeID in self.structs:
                    variable.is_struct = True
                if variable.typeID in self.unions:
                    variable.is_union = True
        for value in self.structs.values():
            for variable in value.members:
                if variable.typeID in self.structs:
                    variable.is_struct = True
                if variable.typeID in self.unions:
                    variable.is_union = True
        for value in self.unions.values():
            for variable in value.choices:
                if variable.typeID in self.structs:
                    variable.is_struct = True
                if variable.typeID in self.unions:
                    variable.is_union = True

        # Replace any types that are aliases with the non-aliased type
        for struct in self.structs.values():
            for member in struct.members:
                if member.typeID in self.aliases:
                    member.typeID = self.aliases[member.typeID]


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

        self.functions[cmd.elem.get('name')] = VulkanFunction(cmd.elem, self.constants, self.aliases, self.extFuncs)

    # These are actually constants
    def genEnum(self, enuminfo, name, alias):
        gen.OutputGenerator.genEnum(self, enuminfo, name, alias)

    # These are actually enums
    def genGroup(self, groupinfo, groupName, alias):
        gen.OutputGenerator.genGroup(self, groupinfo, groupName, alias)

        if alias is not None:
            self.aliases[groupName] = alias
            return

        if groupinfo.elem.get('type') == 'bitmask':
            self.bitmasks[groupinfo.elem.get('name')] = VulkanBitmask(groupinfo.elem, self.extensions)

        elif groupinfo.elem.get('type') == 'enum':
            self.enums[groupinfo.elem.get('name')] = VulkanEnum(groupinfo.elem, self.extensions)

    def genType(self, typeinfo, name, alias):
        gen.OutputGenerator.genType(self, typeinfo, name, alias)

        if alias is not None:
            self.aliases[name] = alias
            return

        if typeinfo.elem.get('category') == 'struct':
            self.structs[typeinfo.elem.get('name')] = VulkanStruct(typeinfo.elem, self.constants, self.enums)
        elif typeinfo.elem.get('category') == 'basetype':
            self.basetypes[typeinfo.elem.get('name')] = VulkanBasetype(typeinfo.elem)
        elif typeinfo.elem.get('category') is None and typeinfo.elem.get('requires') in ['vk_platform', 'stdint']:
            # only add these types if we are generating the video headers
            if typeinfo.elem.get('name') in DUPLICATE_TYPES_IN_VIDEO_HEADER:
                if self.isVideoGeneration:
                    self.externalTypes[typeinfo.elem.get('name')] = VulkanExternalType(typeinfo.elem)
            else:
                self.externalTypes[typeinfo.elem.get('name')] = VulkanExternalType(typeinfo.elem)
        elif typeinfo.elem.get('category') == 'handle':
            self.handles[typeinfo.elem.get('name')] = VulkanHandle(typeinfo.elem)
        elif typeinfo.elem.get('category') == 'union':
            self.unions[typeinfo.elem.get('name')] = VulkanUnion(typeinfo.elem, self.constants)
        elif typeinfo.elem.get('category') == 'bitmask':
            self.flags[typeinfo.elem.get('name')] = VulkanFlags(typeinfo.elem)
        elif typeinfo.elem.get('category') == 'funcpointer':
            func = VulkanFunctionPointer(typeinfo.elem)
            self.funcPointers[func.name] = func

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
        # turn subjects into a set if it is a dict
        if type(subjects) is dict:
            subjects = subjects.values()

        for item in subjects:

            # Merge the values and the parent values
            values = item.values().copy()
            values.update({'isVideoGeneration' : str(self.isVideoGeneration)})
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
            elif item.name in self.sysTypes:
                ext = self.sysTypes[item.name].ext
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

        # Need to get the 'full type', do this by making a list out of the text, remove the name, then put it back together
        # We must add spaces around the brackets so they are separate list elements, which is necessary to prune array length declarations
        text_list = self.text.replace('[', ' [ ').replace(']', ' ] ').split()
        if self.name in text_list:
            text_list = text_list[0:text_list.index(self.name)] # remove all elements after the name
        self.type = ' '.join(text_list)
        bracketMatch = re.search('(?<=\\[)[ a-zA-Z0-9_]+(?=\\])', self.text)
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

        self.parameterStorage = ''
        if self.typeID in PARAMETER_STATE and parentName in PARAMETER_STATE[self.typeID]:
            self.parameterStorage = PARAMETER_STATE[self.typeID][parentName]

        self.is_struct = False
        self.is_union = False

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

        for ext in extensions.values():
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

        for ext in extensions.values():
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
        # video.xml extensions dont have numbers - just use None in that case
        self.number = int(rootNode.get('number')) if rootNode.get('number') else None
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

        def __init__(self, rootNode, constants, aliases, parentName, index):
            VulkanVariable.__init__(self, rootNode, constants, aliases, parentName)
            self.index = index

        def values(self):
            return {
                'prmName': self.name,
                'prmBaseType': self.baseType,
                'prmTypeID': self.typeID,
                'prmType': self.type,
                'prmChildType': self.childType,
                'prmPtrLevel': self.pointerLevels,
                'prmLength': self.arrayLength,
                'prmParameterStorage': self.parameterStorage,
                'prmIndex': self.index,
                'prmIsStruct': 'true' if self.is_struct else 'false',
                'prmIsUnion': 'true' if self.is_union else 'false'
            }

    def __init__(self, rootNode, constants, aliases, extensions):
        self.name = rootNode.find('proto').find('name').text
        self.returnType = rootNode.find('proto').find('type').text

        self.parameters = []
        index = 0
        for node in rootNode.findall('param'):
            self.parameters.append(VulkanFunction.Parameter(node, constants, aliases, self.name, index))
            index = index + 1

        self.namedParams = ', '.join(p.name for p in self.parameters)
        self.typedParams = ', '.join(p.text for p in self.parameters)

        self.dispatchType = 'global'
        if self.parameters[0].type in ['VkInstance', 'VkPhysicalDevice']:
            self.dispatchType = 'instance'
        elif self.parameters[0].type in ['VkDevice', 'VkCommandBuffer', 'VkQueue']:
            self.dispatchType = 'device'

        if self.name in extensions and extensions[self.name].type == 'instance':
            self.type = 'instance'
        else:
            self.type = self.dispatchType

        self.stateTrackingCode = ''
        if self.name in TRACKED_STATE:
            self.stateTrackingCode = TRACKED_STATE[self.name]

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

class VulkanStruct:

    class Member(VulkanVariable):

        def __init__(self, rootNode, constants, parentName, index):
            VulkanVariable.__init__(self, rootNode, constants, None, parentName)

            # Search for a member condition
            self.condition = None
            if rootNode.get('noautovalidity') == 'true' and parentName in VALIDITY_CHECKS and self.name in VALIDITY_CHECKS[parentName]:
                self.condition = VALIDITY_CHECKS[parentName][self.name]
            self.structValues = rootNode.get('values')
            self.index = index

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
                'memParameterStorage': self.parameterStorage,
                'memIndex' : self.index,
                'memIsStruct': 'true' if self.is_struct else 'false',
                'memIsUnion': 'true' if self.is_union else 'false',
            }


    def __init__(self, rootNode, constants, enums):
        self.name = rootNode.get('name')
        self.structExtends = rootNode.get('structextends')
        self.members = []
        index = 0
        for node in rootNode.findall('member'):
            self.members.append(VulkanStruct.Member(node, constants, self.name, index))
            index = index + 1

        self.structureIndex = -1

        for member in self.members:
            if(member.structValues is not None):
                for opt in enums['VkStructureType'].options:
                    if(member.structValues  == opt.name):
                        self.structureIndex = opt.value
                        break

        # The xml doesn't contain the relevant information here since the struct contains 'fixed' length arrays.
        # Thus we have to fix up the variable such that the length member corresponds to the runtime length, not compile time.
        if self.name in ['VkPhysicalDeviceMemoryProperties','VkPhysicalDeviceGroupProperties', 'VkPhysicalDeviceMemoryBudgetPropertiesEXT', 'VkQueueFamilyGlobalPriorityPropertiesKHR']:
            for member in self.members:
                if member.name == 'memoryTypes':
                    member.lengthMember = True
                    member.arrayLength = 'memoryTypeCount'
                if member.name == 'memoryHeaps':
                    member.lengthMember = True
                    member.arrayLength = 'memoryHeapCount'
                if member.name == 'physicalDevices':
                    member.lengthMember = True
                    member.arrayLength = 'physicalDeviceCount'
                if member.name == 'heapBudget':
                    member.lengthMember = True
                    member.arrayLength = 'ApiDumpInstance::current().getMemoryHeapCount()'
                if member.name == 'heapUsage':
                    member.lengthMember = True
                    member.arrayLength = 'ApiDumpInstance::current().getMemoryHeapCount()'
                if member.name == 'priorities':
                    member.lengthMember = True
                    member.arrayLength = 'priorityCount'


    def values(self):
        return {
            'sctName': self.name,
            'sctStructureTypeIndex': self.structureIndex,
        }

class VulkanSystemType:

    def __init__(self, name, ext):
        self.name = name
        if name in POINTER_TYPES:
            self.type = self.name + '*'
            self.needsPointer = True
        else:
            self.needsPointer = False
            self.type = self.name
        self.ext = ext

    def __hash__(self):
        return hash(self.name) | hash(self.type)

    def values(self):
        return {
            'sysName': self.name,
            'sysType': self.type,
            'sysNeedsPointer': self.needsPointer,
        }

class VulkanUnion:

    class Choice(VulkanVariable):

        def __init__(self, rootNode, constants, parentName, index):
            VulkanVariable.__init__(self, rootNode, constants, None, parentName)
            self.index = index

             # Search for a member condition
            self.condition = None
            if parentName in VALIDITY_CHECKS and self.name in VALIDITY_CHECKS[parentName]:
                self.condition = VALIDITY_CHECKS[parentName][self.name]

        def values(self):
            return {
                'chcName': self.name,
                'chcBaseType': self.baseType,
                'chcTypeID': self.typeID,
                'chcType': self.type,
                'chcChildType': self.childType,
                'chcPtrLevel': self.pointerLevels,
                'chcLength': self.arrayLength,
                'chcCondition': self.condition,
                #'chcLengthIsMember': self.lengthMember,
                'chcIndex': self.index,
                'chcIsStruct': 'true' if self.is_struct else 'false',
                'chcIsUnion': 'true' if self.is_union else 'false',
            }

    def __init__(self, rootNode, constants):
        self.name = rootNode.get('name')
        self.choices = []
        index = 0
        for node in rootNode.findall('member'):
            self.choices.append(VulkanUnion.Choice(node, constants, self.name, index))
            index = index + 1

    def values(self):
        return {
            'unName': self.name,
        }
