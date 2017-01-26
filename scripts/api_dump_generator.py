#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2016 Valve Corporation
# Copyright (c) 2015-2016 LunarG, Inc.
# Copyright (c) 2015-2016 Google Inc.
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
#       iteration over various items within the Vulkan spec, usch as functions, enums, etc.
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
#

import generator as gen
import re
import sys
import xml.etree;

COMMON_CODEGEN = """
/* Copyright (c) 2015-2016 Valve Corporation
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
 * Author: Lenny Komow <lenny@lunarg.com>
 */
 
/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */
 
#include "api_dump_text.h"
#include "api_dump_html.h"

//============================= Dump Functions ==============================//

@foreach function where('{funcReturn}' != 'void')
inline void dump_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
{{
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {{
    case ApiDumpFormat::Text:
        dump_text_{funcName}(dump_inst, result, {funcNamedParams});
        break;
    case ApiDumpFormat::Html:
        dump_html_{funcName}(dump_inst, result, {funcNamedParams});
        break;
    }}
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}}
@end function

@foreach function where('{funcReturn}' == 'void')
inline void dump_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {{
    case ApiDumpFormat::Text:
        dump_text_{funcName}(dump_inst, {funcNamedParams});
        break;
    case ApiDumpFormat::Html:
        dump_html_{funcName}(dump_inst, {funcNamedParams});
        break;
    }}
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}}
@end function

//============================= API EntryPoints =============================//

// Specifically implemented functions

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

    // Output the API dump
    dump_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
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
    
    // Output the API dump
    dump_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
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
    
    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    {funcReturn} result = fpCreateDevice({funcNamedParams});
    if(result == VK_SUCCESS) {{
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }}
    
    // Output the API dump
    dump_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
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
    
    // Output the API dump
    dump_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
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
            VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
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
            VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION),
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
    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    dump_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    ApiDumpInstance::current().nextFrame();
    return result;
}}
@end function

// Autogen instance functions

@foreach function where('{funcType}' == 'instance' and '{funcReturn}' != 'void' and '{funcName}' not in ['vkCreateInstance', 'vkDestroyInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    {funcReturn} result = instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    dump_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    return result;
}}
@end function

@foreach function where('{funcType}' == 'instance' and '{funcReturn}' == 'void' and '{funcName}' not in ['vkCreateInstance', 'vkDestroyInstance', 'vkCreateDevice', 'vkGetInstanceProcAddr', 'vkEnumerateDeviceExtensionProperties', 'vkEnumerateDeviceLayerProperties'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    instance_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    dump_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
}}
@end function

// Autogen device functions

@foreach function where('{funcType}' == 'device' and '{funcReturn}' != 'void' and '{funcName}' not in ['vkDestroyDevice', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkQueuePresentKHR', 'vkGetDeviceProcAddr'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    {funcReturn} result = device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    dump_{funcName}(ApiDumpInstance::current(), result, {funcNamedParams});
    return result;
}}
@end function

@foreach function where('{funcType}' == 'device' and '{funcReturn}' == 'void' and '{funcName}' not in ['vkDestroyDevice', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateInstanceLayerProperties', 'vkGetDeviceProcAddr'])
VK_LAYER_EXPORT VKAPI_ATTR {funcReturn} VKAPI_CALL {funcName}({funcTypedParams})
{{
    device_dispatch_table({funcDispatchParam})->{funcShortName}({funcNamedParams});
    dump_{funcName}(ApiDumpInstance::current(), {funcNamedParams});
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

TEXT_CODEGEN = """
/* Copyright (c) 2015-2016 Valve Corporation
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
 * Author: Lenny Komow <lenny@lunarg.com>
 */
 
/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */
 
#pragma once
 
#include "api_dump.h"

@foreach struct
std::ostream& dump_text_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents);
@end struct
@foreach union
std::ostream& dump_text_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

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

@foreach basetype
inline std::ostream& dump_text_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << object;
}}
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
    if(settings.showAddress())
        return settings.stream() << object;
    else
        return settings.stream() << "address";
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
std::ostream& dump_text_{bitName}({bitName} object, const ApiDumpSettings& settings, int indents)
{{
    bool is_first = true;
    //settings.formatNameType(stream, indents, name, type_string) << object;
    settings.stream() << object;
    @foreach option
    if(object & {optValue})
        is_first = dump_text_bitmaskOption("{optName}", settings.stream(), is_first);
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

@foreach struct where('{sctName}' != 'VkShaderModuleCreateInfo')
std::ostream& dump_text_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
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
    dump_text_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_text_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_text_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember})
    dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    
    @if('{memCondition}' != 'None')
    else
        dump_text_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end member
    return settings.stream();
}}
@end struct

@foreach struct where('{sctName}' == 'VkShaderModuleCreateInfo')
std::ostream& dump_text_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
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
    dump_text_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_text_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember} and '{memName}' != 'pCode')
    dump_text_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
    dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID});
    @end if
    @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_text_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_text_{memTypeID});
    else
        dump_text_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
    @end if
    
    @if('{memCondition}' != 'None')
    else
        dump_text_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end member
    return settings.stream();
}}
@end struct

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
    dump_text_value<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 1, dump_text_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' == 'None')
    dump_text_pointer<const {chcBaseType}>(object.{chcName}, settings, "{chcType}", "{chcName}", indents + 1, dump_text_{chcTypeID});
    @end if
    @if({chcPtrLevel} == 1 and '{chcLength}' != 'None')
    dump_text_array<const {chcBaseType}>(object.{chcName}, {chcLength}, settings, "{chcType}", "{chcChildType}", "{chcName}", indents + 1, dump_text_{chcTypeID});
    @end if
    @end choice
    return settings.stream();
}}
@end union

//========================= Function Implementations ========================//

@foreach function where('{funcReturn}' != 'void')
std::ostream& dump_text_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());
    settings.stream() << "Thread " << dump_inst.threadID() << ", Frame " << dump_inst.frameCount() << ":\\n";
    settings.stream() << "{funcName}({funcNamedParams}) returns {funcReturn} ";
    dump_text_{funcReturn}(result, settings, 0) << ":\\n";
    
    if(settings.showParams())
    {{
        @foreach parameter
        @if({prmPtrLevel} == 0)
        dump_text_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_text_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_text_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_text_{prmTypeID});
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";
    
    return settings.stream();
}}
@end function

@foreach function where('{funcReturn}' == 'void')
std::ostream& dump_text_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());
    settings.stream() << "Thread " << dump_inst.threadID() << ", Frame " << dump_inst.frameCount() << ":\\n";
    settings.stream() << "{funcName}({funcNamedParams}) returns {funcReturn}:\\n";
    
    if(settings.showParams())
    {{
        @foreach parameter
        @if({prmPtrLevel} == 0)
        dump_text_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_text_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_text_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_text_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_text_{prmTypeID});
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
/* Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
 * Copyright (c) 2015-2017 Google Inc.
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
 */
 
/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */
 
#pragma once
 
#include "api_dump.h"

@foreach struct
std::ostream& dump_html_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents);
@end struct
@foreach union
std::ostream& dump_html_{unName}(const {unName}& object, const ApiDumpSettings& settings, int indents);
@end union

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

@foreach basetype
inline std::ostream& dump_html_{baseName}({baseName} object, const ApiDumpSettings& settings, int indents)
{{
    return settings.stream() << "<div class='val'>" << object << "</div></summary>";
}}
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
    if(settings.showAddress())
        settings.stream() << object;
    else
        settings.stream() << "address";
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
    if(object & {optValue})
        is_first = dump_html_bitmaskOption("{optName}", settings.stream(), is_first);
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

@foreach struct where('{sctName}' != 'VkShaderModuleCreateInfo')
std::ostream& dump_html_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
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
    dump_html_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_html_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember})
    dump_html_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember})
    dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    
    @if('{memCondition}' != 'None')
    else
        dump_html_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end member
    return settings.stream();
}}
@end struct

@foreach struct where('{sctName}' == 'VkShaderModuleCreateInfo')
std::ostream& dump_html_{sctName}(const {sctName}& object, const ApiDumpSettings& settings, int indents)
{{
    settings.stream() << "<div class='val'>";
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
    dump_html_value<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' == 'None')
    dump_html_pointer<const {memBaseType}>(object.{memName}, settings, "{memType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and not {memLengthIsMember} and '{memName}' != 'pCode')
    dump_html_array<const {memBaseType}>(object.{memName}, {memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if({memPtrLevel} == 1 and '{memLength}' != 'None' and {memLengthIsMember} and '{memName}' != 'pCode')
    dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID});
    @end if
    @if('{memName}' == 'pCode')
    if(settings.showShader())
        dump_html_array<const {memBaseType}>(object.{memName}, object.{memLength}, settings, "{memType}", "{memChildType}", "{memName}", indents + 1, dump_html_{memTypeID});
    else
        dump_html_special("SHADER DATA", settings, "{memType}", "{memName}", indents + 1);
    @end if
    
    @if('{memCondition}' != 'None')
    else
        dump_html_special("UNUSED", settings, "{memType}", "{memName}", indents + 1);
    @end if
    @end member
    return settings.stream();
}}
@end struct

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
    dump_html_array<const {chcBaseType}>(object.{chcName}, {chcLength}, settings, "{chcType}", "{chcChildType}", "{chcName}", indents + 1, dump_html_{chcTypeID});
    @end if
    @end choice
    return settings.stream();
}}
@end union

//========================= Function Implementations ========================//

uint64_t next_frame = 0;

@foreach function where('{funcReturn}' != 'void')
std::ostream& dump_html_{funcName}(ApiDumpInstance& dump_inst, {funcReturn} result, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());
    uint64_t current_frame = dump_inst.frameCount();
    if (current_frame == next_frame) {{
        if (next_frame > 0) {{
            settings.stream() << "</details>";
        }}
        settings.stream() << "<details class='frm'><summary>Frame " << current_frame << "</summary>";
        next_frame++;
    }}
    settings.stream() << "<div class='thd'>Thread " << dump_inst.threadID() << ":</div>";
    settings.stream() << "<details class='fn'><summary>";
    dump_html_nametype(settings.stream(), "{funcName}({funcNamedParams})", "{funcReturn}");
    dump_html_{funcReturn}(result, settings, 0);
    settings.stream() << "</summary>";
    
    if(settings.showParams())
    {{
        @foreach parameter
        @if({prmPtrLevel} == 0)
        dump_html_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_html_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_html_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";
    
    return settings.stream() << "</details>";
}}
@end function

@foreach function where('{funcReturn}' == 'void')
std::ostream& dump_html_{funcName}(ApiDumpInstance& dump_inst, {funcTypedParams})
{{
    const ApiDumpSettings& settings(dump_inst.settings());
    uint64_t current_frame = dump_inst.frameCount();
    if (current_frame == next_frame) {{
        if (next_frame > 0) {{
            settings.stream() << "</details>";
        }}
        settings.stream() << "<details class='frm'><summary>Frame " << current_frame << "</summary>";
        next_frame++;
    }}
    settings.stream() << "<div class='thd'>Thread " << dump_inst.threadID() << ":</div>";
    settings.stream() << "<details class='fn'><summary>";
    dump_html_nametype(settings.stream(), "{funcName}({funcNamedParams})", "{funcReturn}");
    settings.stream() << "</summary>";
    
    if(settings.showParams())
    {{
        @foreach parameter
        @if({prmPtrLevel} == 0)
        dump_html_value<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' == 'None')
        dump_html_pointer<const {prmBaseType}>({prmName}, settings, "{prmType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @if({prmPtrLevel} == 1 and '{prmLength}' != 'None')
        dump_html_array<const {prmBaseType}>({prmName}, {prmLength}, settings, "{prmType}", "{prmChildType}", "{prmName}", 1, dump_html_{prmTypeID});
        @end if
        @end parameter
    }}
    settings.shouldFlush() ? settings.stream() << std::endl : settings.stream() << "\\n";
    
    return settings.stream() << "</details>";
}}
@end function
"""

POINTER_TYPES = ['void', 'xcb_connection_t', 'Display', 'SECURITY_ATTRIBUTES', 'ANativeWindow']
VALIDITY_CHECKS = {
    'VkBufferCreateInfo': {
        'pQueueFamilyIndices': 'object.sharingMode == VK_SHARING_MODE_CONCURRENT',
    },
    'VkCommandBufferBeginInfo': {
        'pInheritanceInfo': 'false',    # No easy way to tell if this is a primary command buffer
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
        'pViewports': 'false',      # No easy way to check if viewport state is dynamic
        'pScissors': 'false',       # No easy way to check if scissor state is dynamic
    },
    'VkSwapchainCreateInfoKHR': {
        'pQueueFamilyIndices': 'object.imageSharingMode == VK_SHARING_MODE_CONCURRENT',
    },
    'VkWriteDescriptorSet': {
        'pImageInfo':
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE) || ' +
            '(object.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)',
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

class ApiDumpGeneratorOptions(gen.GeneratorOptions):
    
    def __init__(self,
                 input = None,
                 filename = None,
                 directory = '.',
                 apiname = None,
                 profile = None,
                 versions = '.*',
                 emitversions = '.*',
                 defaultExtensions = None,
                 addExtensions = None,
                 removeExtensions = None,
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
                 alignFuncParam = 0):
        gen.GeneratorOptions.__init__(self, filename, directory, apiname, profile,
            versions, emitversions, defaultExtensions,
            addExtensions, removeExtensions, sortProcedure)
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


class ApiDumpOutputGenerator(gen.OutputGenerator):
    
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
        
        self.registryFile = registryFile
        
    def beginFile(self, genOpts):
        gen.OutputGenerator.beginFile(self, genOpts)
        self.format = genOpts.input

        if self.registryFile != None:
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
            if node.get('category') == None and node.get('requires') in self.includes and node.get('requires') != 'vk_platform':
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
        while nextFor != None or nextFor != None or nextEnd != None:
            # If this is a @foreach
            if nextFor != None and ((nextIf == None or nextFor.start() < nextIf.start()) and nextFor.start() < nextEnd.start()):
                depth += 1
                forType = re.search('(?<=\\s)[a-z]+', self.format[nextFor.start():nextFor.end()])
                text = self.format[forType.start()+nextFor.start():forType.end()+nextFor.start()]
                whereMatch = re.search('(?<=where\\().*(?=\\))', self.format[nextFor.start():nextFor.end()])
                condition = None if whereMatch == None else self.format[whereMatch.start()+nextFor.start():whereMatch.end()+nextFor.start()]
                unassignedControls.append((nextFor.start(), nextFor.end(), text, condition))
                
                try:
                    nextFor = next(forIter)
                except StopIteration:
                    nextFor = None
            
            # If this is an @if    
            elif nextIf != None and nextIf.start() < nextEnd.start():
                depth += 1
                condMatch = re.search('(?<=if\\().*(?=\\))', self.format[nextIf.start():nextIf.end()])
                condition = None if condMatch == None else self.format[condMatch.start()+nextIf.start():condMatch.end()+nextIf.start()]
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
        
    def genCmd(self, cmd, name):
        gen.OutputGenerator.genCmd(self, cmd, name)
        self.functions.add(VulkanFunction(cmd.elem, self.constants))
        
    # These are actually constants
    def genEnum(self, enuminfo, name):
        gen.OutputGenerator.genEnum(self, enuminfo, name)
    
    # These are actually enums
    def genGroup(self, groupinfo, groupName):
        gen.OutputGenerator.genGroup(self, groupinfo, groupName)
        
        if groupinfo.elem.get('type') == 'bitmask':
            self.bitmasks.add(VulkanBitmask(groupinfo.elem, self.extensions))
        elif groupinfo.elem.get('type') == 'enum':
            self.enums.add(VulkanEnum(groupinfo.elem, self.extensions))

    def genType(self, typeinfo, name):
        gen.OutputGenerator.genType(self, typeinfo, name)
        
        if typeinfo.elem.get('category') == 'struct':
            self.structs.add(VulkanStruct(typeinfo.elem, self.constants))
        elif typeinfo.elem.get('category') == 'basetype':
            self.basetypes.add(VulkanBasetype(typeinfo.elem))
        elif typeinfo.elem.get('category') == None and typeinfo.elem.get('requires') == 'vk_platform':
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
            if loop.condition != None:
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
            if ext != None and ext.guard != None:
                out += '#if defined({})\n'.format(ext.guard)
                    
            # Format the string
            lastIndex = loop.startPos[1]
            for child in loop.children:
                out += loop.fullString[lastIndex:child.startPos[0]].format(**values)
                out += self.expand(child, parents=[item]+parents)
                lastIndex = child.endPos[1]
            out += loop.fullString[lastIndex:loop.endPos[0]].format(**values)
            
            # Close the ifdef
            if ext != None and ext.guard != None:
                out += '#endif // {}\n'.format(ext.guard)
            
        return out
        
    def findByType(self, types, objects):
        value = None
        for item in objects:
            for ty in types:
                if isinstance(item, ty):
                    value = item
                    break
        assert(value != None)
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
    
    def __init__(self, rootNode, constants):
        # Set basic properties
        self.name = rootNode.find('name').text      # Variable name
        self.typeID = rootNode.find('type').text    # Typename, dereferenced and converted to a useable C++ token
        self.baseType = self.typeID                     # Type, dereferenced to the non-pointer type
        self.childType = None                       # Type, dereferenced to the non-pointer type (None if it isn't a pointer)
        self.arrayLength = None                     # Length of the array, or None if it isn't an array
        
        typeMatch = re.search('.+(?=' + self.name + ')', ''.join(rootNode.itertext()))
        self.type = typeMatch.string[typeMatch.start():typeMatch.end()]
        self.type = ' '.join(self.type.split())
        bracketMatch = re.search('(?<=\\[)[a-zA-Z0-9_]+(?=\\])', ''.join(rootNode.itertext()))
        if bracketMatch != None:
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
        if lengthString != None:
            lengths = re.split(',', lengthString)
            lengths = list(filter(('null-terminated').__ne__, lengths))
        assert(len(lengths) <= 1)
        if self.arrayLength == None and len(lengths) > 0:
            self.childType = '*'.join(self.type.split('*')[0:-1])
            self.arrayLength = lengths[0]
            self.lengthMember = True
        if self.arrayLength != None and self.arrayLength.startswith('latexmath'):
            code = self.arrayLength[10:len(self.arrayLength)]
            code = re.sub('\\[', '', code)
            code = re.sub('\\]', '', code)
            code = re.sub('\\\\(lceil|rceil)', '', code)
            code = re.sub('{|}', '', code)
            code = re.sub('\\\\mathit', '', code)
            code = re.sub('\\\\over', '/', code)
            self.arrayLength = code
            
        # Dereference if necessary and handle members of variables
        if self.arrayLength != None:
            self.arrayLength = re.sub('::', '->', self.arrayLength)
            sections = self.arrayLength.split('->')
            if sections[-1][0] == 'p' and sections[0][1].isupper():
                self.arrayLength = '*' + self.arrayLength
        
        self.pointerLevels = len(re.findall('\\*|\\[', ''.join(rootNode.itertext())))
        if self.typeID == 'char' and self.pointerLevels > 0:
            self.baseType += '*'
            self.pointerLevels -= 1
            self.typeID = 'cstring'
        elif self.typeID in POINTER_TYPES:
            self.baseType += '*'
            self.pointerLevels -= 1
        assert(self.pointerLevels >= 0)
        
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
            if childName == None or (childValue == None and childBitpos == None):
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
        
        
class VulkanEnum:
    
    class Option:
        
        def __init__(self, name, value, bitpos, comment):
            self.name = name
            self.comment = comment
            
            if value == 0 or value == None:
                value = 1 << int(bitpos)
            self.value = value
            
        def values(self):
            return {
                'optName': self.name,
                'optValue': self.value,
                'optComment': self.comment,
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
            if childName == None or (childValue == None and childBitpos == None):
                continue
                
            self.options.append(VulkanEnum.Option(childName, childValue, childBitpos, childComment))
            
        for ext in extensions:
            if self.name in ext.enumValues:
                childName, childValue = ext.enumValues[self.name]
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
        self.guard = rootNode.get('protect')
        self.supported = rootNode.get('supported')
        
        self.vktypes = []
        for ty in rootNode.find('require').findall('type'):
            self.vktypes.append(ty.get('name'))
        self.vkfuncs = []
        for func in rootNode.find('require').findall('command'):
            self.vkfuncs.append(func.get('name'))
            
        self.constants = {}
        self.enumValues = {}
        for enum in rootNode.find('require').findall('enum'):
            base = enum.get('extends')
            name = enum.get('name')
            value = enum.get('value')
            bitpos = enum.get('bitpos')
            offset = enum.get('offset')
            
            if value == None and bitpos != None:
                value = 1 << int(bitpos)
            
            if offset != None:
                offset = int(offset)
            if base != None and offset != None:
                enumValue = 1000000000 + 1000*(self.number - 1) + offset
                if enum.get('dir') == '-':
                    enumValue = -enumValue;
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
        
        def __init__(self, rootNode, constants):
            VulkanVariable.__init__(self, rootNode, constants)
            self.text = ''.join(rootNode.itertext())

        def values(self):
            return {
                'prmName': self.name,
                'prmBaseType': self.baseType,
                'prmTypeID': self.typeID,
                'prmType': self.type,
                'prmChildType': self.childType,
                'prmPtrLevel': self.pointerLevels,
                'prmLength': self.arrayLength,
            }
    
    def __init__(self, rootNode, constants):
        self.name = rootNode.find('proto').find('name').text
        self.returnType = rootNode.find('proto').find('type').text

        self.parameters = []
        self.namedParams = ''
        self.typedParams = ''
        for node in rootNode.findall('param'):
            self.parameters.append(VulkanFunction.Parameter(node, constants))
            self.namedParams += self.parameters[-1].name + ', '
            self.typedParams += self.parameters[-1].text + ', '
        if len(self.parameters) > 0:
            self.namedParams = self.namedParams[0:-2]
            self.typedParams = self.typedParams[0:-2]
            
        if self.parameters[0].type in ['VkInstance', 'VkPhysicalDevice'] or self.name == 'vkCreateInstance':
            self.type = 'instance'
        else:
            self.type = 'device' 
            
    def values(self):
        return {
            'funcName': self.name,
            'funcShortName': self.name[2:len(self.name)],
            'funcType': self.type,
            'funcReturn': self.returnType,
            'funcNamedParams': self.namedParams,
            'funcTypedParams': self.typedParams,
            'funcDispatchParam': self.parameters[0].name
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
        
        def __init__(self, rootNode, constants, parentName):
            VulkanVariable.__init__(self, rootNode, constants)
            
            # Search for a member condition
            self.condition = None
            if rootNode.get('noautovalidity') == 'true' and parentName in VALIDITY_CHECKS and self.name in VALIDITY_CHECKS[parentName]:
                self.condition = VALIDITY_CHECKS[parentName][self.name]
            
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
            }
            
    
    def __init__(self, rootNode, constants):
        self.name = rootNode.get('name')
        self.members = []
        for node in rootNode.findall('member'):
            self.members.append(VulkanStruct.Member(node, constants, self.name))
            
    def values(self):
        return {
            'sctName': self.name,
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
        
        def __init__(self, rootNode, constants):
            VulkanVariable.__init__(self, rootNode, constants)
            
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
            self.choices.append(VulkanUnion.Choice(node, constants))
        
    def values(self):
        return {
            'unName': self.name,
        }
            