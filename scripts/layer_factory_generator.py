#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2020 Valve Corporation
# Copyright (c) 2015-2020 LunarG, Inc.
# Copyright (c) 2015-2020 Google Inc.
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
# Author: Tobin Ehlis <tobine@google.com>
# Author: Mark Lobodzinski <mark@lunarg.com>
#
# This script generates the dispatch portion of a factory layer which intercepts
# all Vulkan  functions. The resultant factory layer allows rapid development of
# layers and interceptors.

import os,re,sys
from generator import *
from common_codegen import *

# LayerFactoryGeneratorOptions - subclass of GeneratorOptions.
#
# Adds options used by LayerFactoryOutputGenerator objects during factory
# layer generation.
#
# Additional members
#   prefixText - list of strings to prefix generated header with
#     (usually a copyright statement + calling convention macros).
#   protectFile - True if multiple inclusion protection should be
#     generated (based on the filename) around the entire header.
#   protectFeature - True if #ifndef..#endif protection should be
#     generated around a feature interface in the header file.
#   genFuncPointers - True if function pointer typedefs should be
#     generated
#   protectProto - If conditional protection should be generated
#     around prototype declarations, set to either '#ifdef'
#     to require opt-in (#ifdef protectProtoStr) or '#ifndef'
#     to require opt-out (#ifndef protectProtoStr). Otherwise
#     set to None.
#   protectProtoStr - #ifdef/#ifndef symbol to use around prototype
#     declarations, if protectProto is set
#   apicall - string to use for the function declaration prefix,
#     such as APICALL on Windows.
#   apientry - string to use for the calling convention macro,
#     in typedefs, such as APIENTRY.
#   apientryp - string to use for the calling convention macro
#     in function pointer typedefs, such as APIENTRYP.
#   indentFuncProto - True if prototype declarations should put each
#     parameter on a separate line
#   indentFuncPointer - True if typedefed function pointers should put each
#     parameter on a separate line
#   alignFuncParam - if nonzero and parameters are being put on a
#     separate line, align parameter names at the specified column
class LayerFactoryGeneratorOptions(GeneratorOptions):
    def __init__(self,
                 conventions = None,
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
                 sortProcedure = regSortFeatures,
                 prefixText = "",
                 genFuncPointers = True,
                 protectFile = True,
                 protectFeature = True,
                 apicall = '',
                 apientry = '',
                 apientryp = '',
                 indentFuncProto = True,
                 indentFuncPointer = False,
                 alignFuncParam = 0,
                 helper_file_type = '',
                 expandEnumerants = True):
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
        self.prefixText      = prefixText
        self.genFuncPointers = genFuncPointers
        self.protectFile     = protectFile
        self.protectFeature  = protectFeature
        self.apicall         = apicall
        self.apientry        = apientry
        self.apientryp       = apientryp
        self.indentFuncProto = indentFuncProto
        self.indentFuncPointer = indentFuncPointer
        self.alignFuncParam  = alignFuncParam

# LayerFactoryOutputGenerator - subclass of OutputGenerator.
# Generates a LayerFactory layer that intercepts all API entrypoints
#  This is intended to be used as a starting point for creating custom layers
#
# ---- methods ----
# LayerFactoryOutputGenerator(errFile, warnFile, diagFile) - args as for
#   OutputGenerator. Defines additional internal state.
# ---- methods overriding base class ----
# beginFile(genOpts)
# endFile()
# beginFeature(interface, emit)
# endFeature()
# genType(typeinfo,name)
# genStruct(typeinfo,name)
# genGroup(groupinfo,name)
# genEnum(enuminfo, name)
# genCmd(cmdinfo)
class LayerFactoryOutputGenerator(OutputGenerator):
    """Generate specified API interfaces in a specific style, such as a C header"""
    # This is an ordered list of sections in the header file.
    TYPE_SECTIONS = ['include', 'define', 'basetype', 'handle', 'enum',
                     'group', 'bitmask', 'funcpointer', 'struct']
    ALL_SECTIONS = TYPE_SECTIONS + ['command']




    inline_custom_source_preamble = """
// This file is ***GENERATED***.  Do Not Edit.
// See layer_factory_generator.py for modifications.

/* Copyright (c) 2015-2022 The Khronos Group Inc.
 * Copyright (c) 2015-2022 Valve Corporation
 * Copyright (c) 2015-2022 LunarG, Inc.
 * Copyright (c) 2015-2022 Google Inc.
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
 * Author: Tobin Ehlis <tobine@google.com>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

#include <string.h>
#include <mutex>

#include <vulkan/vulkan.h>

#if defined(WIN32)
/* Windows-specific common code: */
// WinBase.h defines CreateSemaphore and synchapi.h defines CreateEvent
//  undefine them to avoid conflicts with VkLayerDispatchTable struct members.
#ifdef CreateSemaphore
#undef CreateSemaphore
#endif
#ifdef CreateEvent
#undef CreateEvent
#endif
#endif

#define VALIDATION_ERROR_MAP_IMPL

#include "vk_dispatch_table_helper.h"
#include "vk_layer_data.h"
#include "vk_layer_extension_utils.h"
#include "vk_layer_logging.h"
#include "vk_extension_helper.h"
#include "vk_layer_utils.h"

class layer_factory;
std::vector<layer_factory *> global_interceptor_list;
debug_report_data *vlf_report_data = VK_NULL_HANDLE;

#include "layer_factory.h"

struct instance_layer_data {
    VkLayerInstanceDispatchTable dispatch_table;
    VkInstance instance = VK_NULL_HANDLE;
    debug_report_data *report_data = nullptr;
    std::vector<VkDebugReportCallbackEXT> logging_callback;
    std::vector<VkDebugUtilsMessengerEXT> logging_messenger;
    InstanceExtensions extensions;
};

struct device_layer_data {
    debug_report_data *report_data = nullptr;
    VkLayerDispatchTable dispatch_table;
    DeviceExtensions extensions = {};
    VkDevice device = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    instance_layer_data *instance_data = nullptr;
};

static std::unordered_map<void *, device_layer_data *> device_layer_data_map;
static std::unordered_map<void *, instance_layer_data *> instance_layer_data_map;

#include "interceptor_objects.h"

using mutex_t = std::mutex;
using lock_guard_t = std::lock_guard<mutex_t>;
using unique_lock_t = std::unique_lock<mutex_t>;

namespace vulkan_layer_factory {

using std::unordered_map;

static mutex_t global_lock;

static const VkLayerProperties global_layer = {
    "VK_LAYER_LUNARG_layer_factory", VK_LAYER_API_VERSION, 1, "LunarG Layer Factory Layer",
};

static const VkExtensionProperties instance_extensions[] = {{VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_SPEC_VERSION}};

extern const std::unordered_map<std::string, void*> name_to_funcptr_map;


// Manually written functions

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(VkDevice device, const char *funcName) {
    assert(device);
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    const auto &item = name_to_funcptr_map.find(funcName);
    if (item != name_to_funcptr_map.end()) {
        return reinterpret_cast<PFN_vkVoidFunction>(item->second);
    }
    auto &table = device_data->dispatch_table;
    if (!table.GetDeviceProcAddr) return nullptr;
    return table.GetDeviceProcAddr(device, funcName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(VkInstance instance, const char *funcName) {
    instance_layer_data *instance_data;
    const auto &item = name_to_funcptr_map.find(funcName);
    if (item != name_to_funcptr_map.end()) {
        return reinterpret_cast<PFN_vkVoidFunction>(item->second);
    }
    instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    auto &table = instance_data->dispatch_table;
    if (!table.GetInstanceProcAddr) return nullptr;
    return table.GetInstanceProcAddr(instance, funcName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetPhysicalDeviceProcAddr(VkInstance instance, const char *funcName) {
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    auto &table = instance_data->dispatch_table;
    if (!table.GetPhysicalDeviceProcAddr) return nullptr;
    return table.GetPhysicalDeviceProcAddr(instance, funcName);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &global_layer, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount,
                                                              VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &global_layer, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount,
                                                                    VkExtensionProperties *pProperties) {
    if (pLayerName && !strcmp(pLayerName, global_layer.layerName))
        return util_GetExtensionProperties(1, instance_extensions, pCount, pProperties);

    return VK_ERROR_LAYER_NOT_PRESENT;
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char *pLayerName,
                                                                  uint32_t *pCount, VkExtensionProperties *pProperties) {
    if (pLayerName && !strcmp(pLayerName, global_layer.layerName)) return util_GetExtensionProperties(0, NULL, pCount, pProperties);

    assert(physicalDevice);

    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    return instance_data->dispatch_table.EnumerateDeviceExtensionProperties(physicalDevice, NULL, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                              VkInstance *pInstance) {
    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance)fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if (fpCreateInstance == NULL) return VK_ERROR_INITIALIZATION_FAILED;
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    // Init dispatch array and call registration functions
    for (auto intercept : global_interceptor_list) {
        intercept->PreCallCreateInstance(pCreateInfo, pAllocator, pInstance);
    }

    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);

    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(*pInstance), instance_layer_data_map);
    instance_data->instance = *pInstance;
    layer_init_instance_dispatch_table(*pInstance, &instance_data->dispatch_table, fpGetInstanceProcAddr);
    instance_data->report_data = new debug_report_data{};
    instance_data->extensions.InitFromInstanceCreateInfo((pCreateInfo->pApplicationInfo ? pCreateInfo->pApplicationInfo->apiVersion : VK_API_VERSION_1_0), pCreateInfo);
    layer_debug_report_actions(instance_data->report_data, pAllocator, "lunarg_layer_factory");
    layer_debug_messenger_actions(instance_data->report_data, pAllocator, "lunarg_layer_factory");
    vlf_report_data = instance_data->report_data;

    for (auto intercept : global_interceptor_list) {
        intercept->PostCallCreateInstance(pCreateInfo, pAllocator, pInstance, result);
    }

    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(instance);
    instance_layer_data *instance_data = GetLayerDataPtr(key, instance_layer_data_map);
    for (auto intercept : global_interceptor_list) {
        intercept->PreCallDestroyInstance(instance, pAllocator);
    }

    instance_data->dispatch_table.DestroyInstance(instance, pAllocator);

    lock_guard_t lock(global_lock);
    for (auto intercept : global_interceptor_list) {
        intercept->PostCallDestroyInstance(instance, pAllocator);
    }
    // Clean up logging callback, if any
    while (instance_data->logging_messenger.size() > 0) {
        VkDebugUtilsMessengerEXT messenger = instance_data->logging_messenger.back();
        layer_destroy_callback(instance_data->report_data, messenger, pAllocator);
        instance_data->logging_messenger.pop_back();
    }
    while (instance_data->logging_callback.size() > 0) {
        VkDebugReportCallbackEXT callback = instance_data->logging_callback.back();
        layer_destroy_callback(instance_data->report_data, callback, pAllocator);
        instance_data->logging_callback.pop_back();
    }
    layer_debug_utils_destroy_instance(instance_data->report_data);
    FreeLayerDataPtr(key, instance_layer_data_map);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDevice(VkPhysicalDevice gpu, const VkDeviceCreateInfo *pCreateInfo,
                                            const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(gpu), instance_layer_data_map);

    unique_lock_t lock(global_lock);
    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice)fpGetInstanceProcAddr(instance_data->instance, "vkCreateDevice");
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    for (auto intercept : global_interceptor_list) {
        intercept->PreCallCreateDevice(gpu, pCreateInfo, pAllocator, pDevice);
    }
    lock.unlock();

    VkResult result = fpCreateDevice(gpu, pCreateInfo, pAllocator, pDevice);

    lock.lock();
    for (auto intercept : global_interceptor_list) {
        intercept->PostCallCreateDevice(gpu, pCreateInfo, pAllocator, pDevice, result);
    }
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(*pDevice), device_layer_data_map);
    device_data->instance_data = instance_data;
    layer_init_device_dispatch_table(*pDevice, &device_data->dispatch_table, fpGetDeviceProcAddr);
    device_data->device = *pDevice;
    device_data->physical_device = gpu;
    device_data->report_data = instance_data->report_data;
    VkPhysicalDeviceProperties physical_device_properties{};
    instance_data->dispatch_table.GetPhysicalDeviceProperties(gpu, &physical_device_properties);
    device_data->extensions.InitFromDeviceCreateInfo(&instance_data->extensions, physical_device_properties.apiVersion, pCreateInfo);
    lock.unlock();

    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(device);
    device_layer_data *device_data = GetLayerDataPtr(key, device_layer_data_map);

    unique_lock_t lock(global_lock);
    for (auto intercept : global_interceptor_list) {
        intercept->PreCallDestroyDevice(device, pAllocator);
    }
    lock.unlock();

    device_data->dispatch_table.DestroyDevice(device, pAllocator);

    lock.lock();
    for (auto intercept : global_interceptor_list) {
        intercept->PostCallDestroyDevice(device, pAllocator);
    }

    FreeLayerDataPtr(key, device_layer_data_map);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDebugReportCallbackEXT(VkInstance instance,
                                                            const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                                            const VkAllocationCallbacks *pAllocator,
                                                            VkDebugReportCallbackEXT *pCallback) {
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    for (auto intercept : global_interceptor_list) {
        intercept->PreCallCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
    }
    VkResult result = instance_data->dispatch_table.CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
    result = layer_create_report_callback(instance_data->report_data, false, pCreateInfo, pAllocator, pCallback);
    for (auto intercept : global_interceptor_list) {
        intercept->PostCallCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback, result);
    }
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                                         const VkAllocationCallbacks *pAllocator) {
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    for (auto intercept : global_interceptor_list) {
        intercept->PreCallDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
    }
    instance_data->dispatch_table.DestroyDebugReportCallbackEXT(instance, callback, pAllocator);
    layer_destroy_callback(instance_data->report_data, callback, pAllocator);
    for (auto intercept : global_interceptor_list) {
        intercept->PostCallDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
    }
}
"""

    inline_custom_source_postamble = """
// loader-layer interface v0, just wrappers since there is only a layer

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount,
                                                                                      VkExtensionProperties *pProperties) {
    return vulkan_layer_factory::EnumerateInstanceExtensionProperties(pLayerName, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t *pCount,
                                                                                  VkLayerProperties *pProperties) {
    return vulkan_layer_factory::EnumerateInstanceLayerProperties(pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount,
                                                                                VkLayerProperties *pProperties) {
    // the layer command handles VK_NULL_HANDLE just fine internally
    assert(physicalDevice == VK_NULL_HANDLE);
    return vulkan_layer_factory::EnumerateDeviceLayerProperties(VK_NULL_HANDLE, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice,
                                                                                    const char *pLayerName, uint32_t *pCount,
                                                                                    VkExtensionProperties *pProperties) {
    // the layer command handles VK_NULL_HANDLE just fine internally
    assert(physicalDevice == VK_NULL_HANDLE);
    return vulkan_layer_factory::EnumerateDeviceExtensionProperties(VK_NULL_HANDLE, pLayerName, pCount, pProperties);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice dev, const char *funcName) {
    return vulkan_layer_factory::GetDeviceProcAddr(dev, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char *funcName) {
    return vulkan_layer_factory::GetInstanceProcAddr(instance, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_layerGetPhysicalDeviceProcAddr(VkInstance instance,
                                                                                           const char *funcName) {
    return vulkan_layer_factory::GetPhysicalDeviceProcAddr(instance, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkNegotiateLoaderLayerInterfaceVersion(VkNegotiateLayerInterface *pVersionStruct) {
    assert(pVersionStruct != NULL);
    assert(pVersionStruct->sType == LAYER_NEGOTIATE_INTERFACE_STRUCT);

    // Fill in the function pointers if our version is at least capable of having the structure contain them.
    if (pVersionStruct->loaderLayerInterfaceVersion >= 2) {
        pVersionStruct->pfnGetInstanceProcAddr = vkGetInstanceProcAddr;
        pVersionStruct->pfnGetDeviceProcAddr = vkGetDeviceProcAddr;
        pVersionStruct->pfnGetPhysicalDeviceProcAddr = vk_layerGetPhysicalDeviceProcAddr;
    }

    return VK_SUCCESS;
}"""





    def __init__(self,
                 errFile = sys.stderr,
                 warnFile = sys.stderr,
                 diagFile = sys.stdout):
        OutputGenerator.__init__(self, errFile, warnFile, diagFile)
        # Internal state - accumulators for different inner block text
        self.sections = dict([(section, []) for section in self.ALL_SECTIONS])
        self.intercepts = []
        self.layer_factory = ''                     # String containing base layer factory class definition

    # Check if the parameter passed in is a pointer to an array
    def paramIsArray(self, param):
        return param.attrib.get('len') is not None

    # Check if the parameter passed in is a pointer
    def paramIsPointer(self, param):
        ispointer = False
        for elem in param:
            if elem.tag == 'type' and elem.tail is not None and '*' in elem.tail:
                ispointer = True
        return ispointer

    # Check if an object is a non-dispatchable handle
    def isHandleTypeNonDispatchable(self, handletype):
        handle = self.registry.tree.find("types/type/[name='" + handletype + "'][@category='handle']")
        if handle is not None and handle.find('type').text == 'VK_DEFINE_NON_DISPATCHABLE_HANDLE':
            return True
        else:
            return False

    # Check if an object is a dispatchable handle
    def isHandleTypeDispatchable(self, handletype):
        handle = self.registry.tree.find("types/type/[name='" + handletype + "'][@category='handle']")
        if handle is not None and handle.find('type').text == 'VK_DEFINE_HANDLE':
            return True
        else:
            return False

    def beginFile(self, genOpts):
        OutputGenerator.beginFile(self, genOpts)
        # Multiple inclusion protection & C++ namespace.
        self.header = False
        if (self.genOpts.filename and 'h' == self.genOpts.filename[-1]):
            self.header = True
            write('#pragma once', file=self.outFile)
            self.newline()
        # User-supplied prefix text, if any (list of strings)
        if self.header:
            if (genOpts.prefixText):
                for s in genOpts.prefixText:
                    write(s, file=self.outFile)
            write('#include "vulkan/vk_layer.h"', file=self.outFile)
            write('#include <unordered_map>\n', file=self.outFile)
            write('class layer_factory;', file=self.outFile)
            write('extern std::vector<layer_factory *> global_interceptor_list;', file=self.outFile)
            write('extern debug_report_data *vlf_report_data;\n', file=self.outFile)
            write('namespace vulkan_layer_factory {\n', file=self.outFile)
        else:
            write(self.inline_custom_source_preamble, file=self.outFile)

        # Initialize Enum Section
        self.layer_factory += '// Layer Factory base class definition\n'
        self.layer_factory += 'class layer_factory {\n'
        self.layer_factory += '    public:\n'
        self.layer_factory += '        layer_factory() {\n'
        self.layer_factory += '            global_interceptor_list.emplace_back(this);\n'
        self.layer_factory += '        };\n'
        self.layer_factory += '\n'
        self.layer_factory += '        std::string layer_name = "VLF";\n'
        self.layer_factory += '\n'
        self.layer_factory += '        bool log_msg(const debug_report_data *debug_data, VkFlags msg_flags, VkObjectType object_type,\n'
        self.layer_factory += '                                   uint64_t src_object, const std::string &vuid_text, const char *format, ...) {\n'
        self.layer_factory += '            if (!debug_data) return false;\n'
        self.layer_factory += '            VkFlags local_severity = 0;\n'
        self.layer_factory += '            VkFlags local_type = 0;\n'
        self.layer_factory += '            DebugReportFlagsToAnnotFlags(msg_flags, true, &local_severity, &local_type);\n'
        self.layer_factory += '            if (!debug_data || !(debug_data->active_severities & local_severity) || !(debug_data->active_types & local_type)) {\n'
        self.layer_factory += '                // Message is not wanted\n'
        self.layer_factory += '                return false;\n'
        self.layer_factory += '            }\n'
        self.layer_factory += '        \n'
        self.layer_factory += '            va_list argptr;\n'
        self.layer_factory += '            va_start(argptr, format);\n'
        self.layer_factory += '            char *str;\n'
        self.layer_factory += '            if (-1 == vasprintf(&str, format, argptr)) {\n'
        self.layer_factory += '                // On failure, glibc vasprintf leaves str undefined\n'
        self.layer_factory += '                str = nullptr;\n'
        self.layer_factory += '            }\n'
        self.layer_factory += '            va_end(argptr);\n'
        self.layer_factory += '        \n'
        self.layer_factory += '            VulkanTypedHandle null_handle{};\n'
        self.layer_factory += '            LogObjectList objlist(null_handle);\n'
        self.layer_factory += '            va_end(argptr);\n'
        self.layer_factory += '            return LogMsgLocked(debug_data, msg_flags, objlist, vuid_text, str);\n'
        self.layer_factory += '        }\n'
        self.layer_factory += '\n'
        self.layer_factory += '        // Pre/post hook point declarations\n'
        self.layer_factory += '        bool Information(const std::string &message) {\n'
        self.layer_factory += '            return log_msg(vlf_report_data, kInformationBit, VK_OBJECT_TYPE_UNKNOWN, 0,\n'
        self.layer_factory += '                           layer_name.c_str(), "%s", message.c_str());\n'
        self.layer_factory += '        }\n'
        self.layer_factory += '\n'
        self.layer_factory += '        bool PerformanceWarning(const std::string &message) {\n'
        self.layer_factory += '            return log_msg(vlf_report_data, kPerformanceWarningBit, VK_OBJECT_TYPE_UNKNOWN,\n'
        self.layer_factory += '                           0, layer_name.c_str(), "%s", message.c_str());\n'
        self.layer_factory += '        }\n'
        self.layer_factory += '\n'
        self.layer_factory += '        bool Warning(const std::string &message) {\n'
        self.layer_factory += '            return log_msg(vlf_report_data, kWarningBit, VK_OBJECT_TYPE_UNKNOWN, 0,\n'
        self.layer_factory += '                           layer_name.c_str(), "%s", message.c_str());\n'
        self.layer_factory += '        }\n'
        self.layer_factory += '\n'
        self.layer_factory += '        bool Error(const std::string &message) {\n'
        self.layer_factory += '            return log_msg(vlf_report_data, kDebugBit, VK_OBJECT_TYPE_UNKNOWN, 0,\n'
        self.layer_factory += '                           layer_name.c_str(), "%s", message.c_str());\n'
        self.layer_factory += '        }\n'
        self.layer_factory += '\n'
        self.layer_factory += '        void Breakpoint(void) {\n'
        self.layer_factory += '#ifdef WIN32\n'
        self.layer_factory += '            DebugBreak();\n'
        self.layer_factory += '#else\n'
        self.layer_factory += '            raise(SIGTRAP);\n'
        self.layer_factory += '#endif\n'
        self.layer_factory += '        }\n'
        self.layer_factory += '\n'
        self.layer_factory += '        virtual void PreCallApiFunction(const char *api_name) {};\n'
        self.layer_factory += '        virtual void PostCallApiFunction(const char *api_name) {};\n'
        self.layer_factory += '        virtual void PreCallApiFunction(const char *api_name, VkResult result) {};\n'
        self.layer_factory += '        virtual void PostCallApiFunction(const char *api_name, VkResult result) {};\n'
        self.layer_factory += '\n'
        self.layer_factory += '        // Pre/post hook point declarations\n'
    #
    def endFile(self):
        # Finish C++ namespace and multiple inclusion protection
        self.newline()
        if not self.header:
            # Record intercepted procedures
            write('// Map of all APIs to be intercepted by this layer', file=self.outFile)
            write('const std::unordered_map<std::string, void*> name_to_funcptr_map = {', file=self.outFile)
            write('\n'.join(self.intercepts), file=self.outFile)
            write('};\n', file=self.outFile)
            self.newline()
        write('} // namespace vulkan_layer_factory', file=self.outFile)
        if self.header:
            self.newline()
            # Output Layer Factory Class Definitions
            self.layer_factory += '};\n'
            write(self.layer_factory, file=self.outFile)
        else:
            write(self.inline_custom_source_postamble, file=self.outFile)
        # Finish processing in superclass
        OutputGenerator.endFile(self)

    def beginFeature(self, interface, emit):
        # Start processing in superclass
        OutputGenerator.beginFeature(self, interface, emit)
        # Get feature extra protect
        self.featureExtraProtect = GetFeatureProtect(interface)
        # Accumulate includes, defines, types, enums, function pointer typedefs, end function prototypes separately for this
        # feature. They're only printed in endFeature().
        self.sections = dict([(section, []) for section in self.ALL_SECTIONS])

    def endFeature(self):
        # Actually write the interface to the output file.
        if (self.emit):
            self.newline()
            # If type declarations are needed by other features based on this one, it may be necessary to suppress the ExtraProtect,
            # or move it below the 'for section...' loop.
            if (self.featureExtraProtect is not None):
                write('#ifdef', self.featureExtraProtect, file=self.outFile)
            for section in self.TYPE_SECTIONS:
                contents = self.sections[section]
                if contents:
                    write('\n'.join(contents), file=self.outFile)
                    self.newline()
            if (self.sections['command']):
                write('\n'.join(self.sections['command']), end=u'', file=self.outFile)
                self.newline()
            if (self.featureExtraProtect is not None):
                write('#endif /*', self.featureExtraProtect, '*/', file=self.outFile)
        # Finish processing in superclass
        OutputGenerator.endFeature(self)
    #
    # Append a definition to the specified section
    def appendSection(self, section, text):
        self.sections[section].append(text)
    #
    # Type generation
    def genType(self, typeinfo, name, alias):
        pass
    #
    # Struct (e.g. C "struct" type) generation. This is a special case of the <type> tag where the contents are
    # interpreted as a set of <member> tags instead of freeform C type declarations. The <member> tags are just like <param>
    # tags - they are a declaration of a struct or union member. Only simple member declarations are supported (no nested
    # structs etc.)
    def genStruct(self, typeinfo, typeName):
        OutputGenerator.genStruct(self, typeinfo, typeName)
        body = 'typedef ' + typeinfo.elem.get('category') + ' ' + typeName + ' {\n'
        # paramdecl = self.makeCParamDecl(typeinfo.elem, self.genOpts.alignFuncParam)
        for member in typeinfo.elem.findall('.//member'):
            body += self.makeCParamDecl(member, self.genOpts.alignFuncParam)
            body += ';\n'
        body += '} ' + typeName + ';\n'
        self.appendSection('struct', body)
    #
    # Group (e.g. C "enum" type) generation. These are concatenated together with other types.
    def genGroup(self, groupinfo, groupName, alias):
        pass
    # Enumerant generation
    # <enum> tags may specify their values in several ways, but are usually just integers.
    def genEnum(self, enuminfo, name, alias):
        pass
    #
    # Customize Cdecl for layer factory base class
    def BaseClassCdecl(self, elem, name):
        raw = self.makeCDecls(elem)[1]
        # Change initial keyword
        result = raw.replace("typedef", "virtual")
        # Remove first set of parenthesis
        result = result.replace("(", "", 1)
        result = result.replace(")", "", 1)
        # Add default implementation: This map contains the default function definitions for the return types of Vulkan Commands.
        # If any new return types are required, they'll need to be added to this dict.
        return_map = {
            'PFN_vkVoidFunction': ' { return nullptr; };',
            'uint32_t': ' { return 0; };',
            'uint64_t': ' { return 0; };',
            'VkBool32': ' { return VK_TRUE; };',
            'VkDeviceAddress': '{ return 0; };',
            'VkResult': ' { return VK_SUCCESS; };',
            'VkDeviceSize': ' { return 0; };',
            'void': ' {};',
            }
        return_type = result.split(" ")[1]
        default_def = return_map[return_type]
        result = result.replace(';', default_def, 1)
        pre_call = result.replace("VKAPI_PTR *PFN_vk", "PreCall")
        pre_call_function = '{ PreCallApiFunction("%s");' % name
        pre_call = pre_call.replace("{", pre_call_function)
        post_call = pre_call.replace("PreCall", "PostCall")
        if return_type == 'VkResult':
            post_call = post_call.replace(')', ', VkResult result)', 1)
            post_call = post_call.replace(');', ', result);', 1);
        return '        %s\n        %s\n' % (pre_call, post_call)
    #
    # Command generation
    def genCmd(self, cmdinfo, name, alias):
        ignore_functions = [
        'vkEnumerateInstanceVersion'
		]

        if name in ignore_functions:
            return

        if self.header: # In the header declare all intercepts
            self.appendSection('command', '')
            self.appendSection('command', self.makeCDecls(cmdinfo.elem)[0])
            if (self.featureExtraProtect is not None):
                self.intercepts += [ '#ifdef %s' % self.featureExtraProtect ]
                self.layer_factory += '#ifdef %s\n' % self.featureExtraProtect
            # Update base class with virtual function declarations
            self.layer_factory += self.BaseClassCdecl(cmdinfo.elem, name)
            # Update function intercepts
            self.intercepts += [ '    {"%s", (void*)%s},' % (name,name[2:]) ]
            if (self.featureExtraProtect is not None):
                self.intercepts += [ '#endif' ]
                self.layer_factory += '#endif\n'
            return

        manual_functions = [
            # Include functions here to be interecpted w/ manually implemented function bodies
            'vkGetDeviceProcAddr',
            'vkGetInstanceProcAddr',
            'vkGetPhysicalDeviceProcAddr',
            'vkCreateDevice',
            'vkDestroyDevice',
            'vkCreateInstance',
            'vkDestroyInstance',
            'vkCreateDebugReportCallbackEXT',
            'vkDestroyDebugReportCallbackEXT',
            'vkEnumerateInstanceLayerProperties',
            'vkEnumerateInstanceExtensionProperties',
            'vkEnumerateDeviceLayerProperties',
            'vkEnumerateDeviceExtensionProperties',
        ]
        if name in manual_functions:
            ####decls = self.makeCDecls(cmdinfo.elem)
            ####self.appendSection('command', '')
            ####self.appendSection('command', '// Declare only')
            ####self.appendSection('command', decls[0])
            self.intercepts += [ '    {"%s", (void*)%s},' % (name,name[2:]) ]
            return
        # Record that the function will be intercepted
        if (self.featureExtraProtect is not None):
            self.intercepts += [ '#ifdef %s' % self.featureExtraProtect ]
        self.intercepts += [ '    {"%s", (void*)%s},' % (name,name[2:]) ]
        if (self.featureExtraProtect is not None):
            self.intercepts += [ '#endif' ]
        OutputGenerator.genCmd(self, cmdinfo, name, alias)
        #
        decls = self.makeCDecls(cmdinfo.elem)
        self.appendSection('command', '')
        self.appendSection('command', '%s {' % decls[0][:-1])
        # Setup common to call wrappers. First parameter is always dispatchable
        dispatchable_type = cmdinfo.elem.find('param/type').text
        dispatchable_name = cmdinfo.elem.find('param/name').text
        # Default to device
        device_or_instance = 'device'
        dispatch_table_name = 'VkLayerDispatchTable'
        # Set to instance as necessary
        if dispatchable_type in ["VkPhysicalDevice", "VkInstance"] or name == 'vkCreateInstance':
            device_or_instance = 'instance'
            dispatch_table_name = 'VkLayerInstanceDispatchTable'
        self.appendSection('command', '    %s_layer_data *%s_data = GetLayerDataPtr(get_dispatch_key(%s), %s_layer_data_map);' % (device_or_instance, device_or_instance, dispatchable_name, device_or_instance))
        api_function_name = cmdinfo.elem.attrib.get('name')
        params = cmdinfo.elem.findall('param/name')
        paramstext = ', '.join([str(param.text) for param in params])
        API = api_function_name.replace('vk','%s_data->dispatch_table.' % (device_or_instance),1)

        # Generate pre-call object processing source code
        self.appendSection('command', '    for (auto intercept : global_interceptor_list) {')
        self.appendSection('command', '        intercept->PreCall%s(%s);' % (api_function_name[2:], paramstext))
        self.appendSection('command', '    }')

        # Declare result variable, if any.
        resulttype = cmdinfo.elem.find('proto/type')
        if (resulttype is not None and resulttype.text == 'void'):
          resulttype = None
        if (resulttype is not None):
            assignresult = resulttype.text + ' result = '
        else:
            assignresult = ''

        self.appendSection('command', '    ' + assignresult + API + '(' + paramstext + ');')

        # Generate post-call object processing source code
        returnParam = ''
        if (resulttype is not None and resulttype.text == 'VkResult'):
            returnParam = ', result'
        self.appendSection('command', '    for (auto intercept : global_interceptor_list) {')
        self.appendSection('command', '        intercept->PostCall%s(%s%s);' % (api_function_name[2:], paramstext, returnParam))
        self.appendSection('command', '    }')

        # Return result variable, if any.
        if (resulttype is not None):
            self.appendSection('command', '    return result;')
        self.appendSection('command', '}')
    #
    # Override makeProtoName to drop the "vk" prefix
    def makeProtoName(self, name, tail):
        return self.genOpts.apientry + name[2:] + tail
