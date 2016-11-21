#!/usr/bin/env python3
#
# VK
#
# Copyright (c) 2015-2016 Valve Corporation
# Copyright (c) 2015-2016 LunarG, Inc.
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
# Author: Tobin Ehlis <tobin@lunarg.com>
# Author: Courtney Goeltzenleuchter <courtney@lunarg.com>
# Author: Jon Ashburn <jon@lunarg.com>
# Author: Mark Lobodzinski <mark@lunarg.com>
# Author: Mike Stroyan <stroyan@lunarg.com>
# Author: Tony Barbour <tony@LunarG.com>
# Author: Chia-I Wu <olv@lunarg.com>

import sys
import os
import re

import vulkan
import vk_helper
from source_line_info import sourcelineinfo
from collections import defaultdict

def proto_is_global(proto):
    global_function_names = [
        "CreateInstance",
        "EnumerateInstanceLayerProperties",
        "EnumerateInstanceExtensionProperties",
        "EnumerateDeviceLayerProperties",
        "EnumerateDeviceExtensionProperties",
        "CreateXcbSurfaceKHR",
        "GetPhysicalDeviceXcbPresentationSupportKHR",
        "CreateXlibSurfaceKHR",
        "GetPhysicalDeviceXlibPresentationSupportKHR",
        "CreateWaylandSurfaceKHR",
        "GetPhysicalDeviceWaylandPresentationSupportKHR",
        "CreateMirSurfaceKHR",
        "GetPhysicalDeviceMirPresentationSupportKHR",
        "CreateAndroidSurfaceKHR",
        "CreateWin32SurfaceKHR",
        "GetPhysicalDeviceWin32PresentationSupportKHR"
    ]
    if proto.params[0].ty == "VkInstance" or proto.params[0].ty == "VkPhysicalDevice" or proto.name in global_function_names:
       return True
    else:
       return False

def wsi_name(ext_name):
    wsi_prefix = ""
    if 'Xcb' in ext_name:
        wsi_prefix = 'XCB'
    elif 'Xlib' in ext_name:
        wsi_prefix = 'XLIB'
    elif 'Win32' in ext_name:
        wsi_prefix = 'WIN32'
    elif 'Mir' in ext_name:
        wsi_prefix = 'MIR'
    elif 'Wayland' in ext_name:
        wsi_prefix = 'WAYLAND'
    elif 'Android' in ext_name:
        wsi_prefix = 'ANDROID'
    else:
        wsi_prefix = ''
    return wsi_prefix

def wsi_ifdef(ext_name):
    wsi_prefix = wsi_name(ext_name)
    if not wsi_prefix:
        return ''
    else:
        return "#ifdef VK_USE_PLATFORM_%s_KHR" % wsi_prefix

def wsi_endif(ext_name):
    wsi_prefix = wsi_name(ext_name)
    if not wsi_prefix:
        return ''
    else:
        return "#endif  // VK_USE_PLATFORM_%s_KHR" % wsi_prefix

def generate_get_proc_addr_check(name):
    return "    if (!%s || %s[0] != 'v' || %s[1] != 'k')\n" \
           "        return NULL;" % ((name,) * 3)

# Parse complete struct chain and add any new ndo_uses to the dict
def gather_object_uses_in_struct(obj_list, struct_type):
    struct_uses = {}
    if vk_helper.typedef_rev_dict[struct_type] in vk_helper.struct_dict:
        struct_type = vk_helper.typedef_rev_dict[struct_type]
        # Parse elements of this struct param to identify objects and/or arrays of objects
        for m in sorted(vk_helper.struct_dict[struct_type]):
            array_len = "%s" % (str(vk_helper.struct_dict[struct_type][m]['array_size']))
            base_type = vk_helper.struct_dict[struct_type][m]['type']
            mem_name = vk_helper.struct_dict[struct_type][m]['name']
            if array_len != '0':
                mem_name = "%s[%s]" % (mem_name, array_len)
            if base_type in obj_list:
                #if array_len not in ndo_uses:
                #    struct_uses[array_len] = []
                #struct_uses[array_len].append("%s%s,%s" % (name_prefix, struct_name, base_type))
                struct_uses[mem_name] = base_type
            elif vk_helper.is_type(base_type, 'struct'):
                sub_uses = gather_object_uses_in_struct(obj_list, base_type)
                if len(sub_uses) > 0:
                    struct_uses[mem_name] = sub_uses
    return struct_uses

class Subcommand(object):
    def __init__(self, argv):
        self.argv = argv
        self.headers = vulkan.headers
        self.protos = vulkan.protos
        self.no_addr = False
        self.layer_name = ""
        self.lineinfo = sourcelineinfo()
        self.wsi = sys.argv[1]

    def run(self):
        print(self.generate())

    def generate(self):
        copyright = self.generate_copyright()
        header = self.generate_header()
        body = self.generate_body()
        footer = self.generate_footer()

        contents = []
        if copyright:
            contents.append(copyright)
        if header:
            contents.append(header)
        if body:
            contents.append(body)
        if footer:
            contents.append(footer)

        return "\n\n".join(contents)

    def generate_copyright(self):
        return """/* THIS FILE IS GENERATED.  DO NOT EDIT. */

/*
 * Copyright (c) 2015-2016 Valve Corporation
 * Copyright (c) 2015-2016 LunarG, Inc.
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
 * Author: Tobin Ehlis <tobin@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@lunarg.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Mike Stroyan <mike@lunarg.com>
 * Author: Tony Barbour <tony@LunarG.com>
 */"""

    def generate_header(self):
        return "\n".join(["#include <" + h + ">" for h in self.headers])

    def generate_body(self):
        pass

    def generate_footer(self):
        pass

    # Return set of printf '%' qualifier and input to that qualifier
    def _get_printf_params(self, vk_type, name, output_param, cpp=False):
        # TODO : Need ENUM and STRUCT checks here
        if vk_helper.is_type(vk_type, 'enum'):#"_TYPE" in vk_type: # TODO : This should be generic ENUM check
            return ("%s", "string_%s(%s)" % (vk_type.replace('const ', '').strip('*'), name))
        if "char*" == vk_type:
            return ("%s", name)
        if "uint64" in vk_type:
            if '*' in vk_type:
                return ("%lu", "*%s" % name)
            return ("%lu", name)
        if vk_type.strip('*') in vulkan.object_non_dispatch_list:
            if '*' in vk_type:
                return ("0x%p", "HandleCast(*%s)" % name, '*')
            return ("0x%p", "HandleCast(%s)" % name, '')
        if vk_type.strip('*').replace('const ', '') in vulkan.object_dispatch_list:
            if '*' in vk_type:
                return ("0x%p", "HandleCast(*%s)" % name, '*')
            return ("0x%p", "HandleCast(%s)" % name, '')
        if "size" in vk_type.lower() or "mask" in vk_type.lower():
            if '*' in vk_type:
                return ("0x%p", "*%s" % name, '*')
            return ("0x%p", "%s" % name, '')
        if "float" in vk_type:
            if '[' in vk_type: # handle array, current hard-coded to 4 (TODO: Make this dynamic)
                if cpp:
                    return ("[%i, %i, %i, %i]", '"[" << %s[0] << "," << %s[1] << "," << %s[2] << "," << %s[3] << "]"' % (name, name, name, name))
                return ("[%f, %f, %f, %f]", "%s[0], %s[1], %s[2], %s[3]" % (name, name, name, name))
            return ("%f", name)
        if "bool" in vk_type.lower() or 'xcb_randr_crtc_t' in vk_type:
            return ("%u", name)
        if True in [t in vk_type.lower() for t in ["int", "flags", "mask", "xcb_window_t"]]:
            if '[' in vk_type: # handle array, current hard-coded to 4 (TODO: Make this dynamic)
                if cpp:
                    return ("[%i, %i, %i, %i]", "%s[0] << %s[1] << %s[2] << %s[3]" % (name, name, name, name))
                return ("[%i, %i, %i, %i]", "%s[0], %s[1], %s[2], %s[3]" % (name, name, name, name))
            if '*' in vk_type:
                if 'pUserData' == name:
                    return ("%i", "((pUserData == 0) ? 0 : *(pUserData))")
                if 'const' in vk_type.lower():
                    return ("%p", "(void*)(%s)" % name)
                return ("%i", "*(%s)" % name)
            return ("%i", name)
        # TODO : This is special-cased as there's only one "format" param currently and it's nice to expand it
        if "VkFormat" == vk_type:
            return ("0x%p", "HandleCast(&%s)" % name, '&')
        if output_param:
            if 1 == vk_type.count('*'):
                return ("0x%p", "*%s" % name, '*')
            else:
                return ("0x%p", "HandleCast(*%s)" % name, '*')
#        if vk_helper_api_dump.is_type(vk_type, 'struct') and '*' not in vk_type:
#            return ("%p", "HandleCast(&%s)" % name, '&')
        return ("0x%p", "HandleCast(%s)" % name, '')

    def _gen_create_msg_callback(self):
        r_body = []
        r_body.append('%s' % self.lineinfo.get())
        r_body.append('VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(')
        r_body.append('        VkInstance                                   instance,')
        r_body.append('        const VkDebugReportCallbackCreateInfoEXT*    pCreateInfo,')
        r_body.append('        const VkAllocationCallbacks*                 pAllocator,')
        r_body.append('        VkDebugReportCallbackEXT*                    pCallback)')
        r_body.append('{')
        # Switch to this code section for the new per-instance storage and debug callbacks
        if self.layer_name in ['object_tracker', 'threading', 'unique_objects']:
            r_body.append('    VkLayerInstanceDispatchTable *pInstanceTable = get_dispatch_table(%s_instance_table_map, instance);' % self.layer_name )
            r_body.append('    VkResult result = pInstanceTable->CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);')
            r_body.append('    if (VK_SUCCESS == result) {')
            r_body.append('        layer_data *my_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);')
            r_body.append('        result = layer_create_msg_callback(my_data->report_data,')
            r_body.append('                                           false,')
            r_body.append('                                           pCreateInfo,')
            r_body.append('                                           pAllocator,')
            r_body.append('                                           pCallback);')
            r_body.append('    }')
            r_body.append('    return result;')
        else:
            r_body.append('    VkResult result = instance_dispatch_table(instance)->CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);')
            r_body.append('    if (VK_SUCCESS == result) {')
            r_body.append('        layer_data *my_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);')
            r_body.append('        result = layer_create_msg_callback(my_data->report_data, false, pCreateInfo, pAllocator, pCallback);')
            r_body.append('    }')
            r_body.append('    return result;')
        r_body.append('}')
        return "\n".join(r_body)

    def _gen_destroy_msg_callback(self):
        r_body = []
        r_body.append('%s' % self.lineinfo.get())
        r_body.append('VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT msgCallback, const VkAllocationCallbacks *pAllocator)')
        r_body.append('{')
        # Switch to this code section for the new per-instance storage and debug callbacks
        if self.layer_name in ['object_tracker', 'threading', 'unique_objects']:
            r_body.append('    VkLayerInstanceDispatchTable *pInstanceTable = get_dispatch_table(%s_instance_table_map, instance);' % self.layer_name )
        else:
            r_body.append('    VkLayerInstanceDispatchTable *pInstanceTable = instance_dispatch_table(instance);')
        r_body.append('    pInstanceTable->DestroyDebugReportCallbackEXT(instance, msgCallback, pAllocator);')
        r_body.append('    layer_data *my_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);')
        r_body.append('    layer_destroy_msg_callback(my_data->report_data, msgCallback, pAllocator);')
        r_body.append('}')
        return "\n".join(r_body)

    def _gen_debug_report_msg(self):
        r_body = []
        r_body.append('%s' % self.lineinfo.get())
        r_body.append('VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT    flags, VkDebugReportObjectTypeEXT objType, uint64_t object, size_t location, int32_t msgCode, const char *pLayerPrefix, const char *pMsg)')
        r_body.append('{')
        # Switch to this code section for the new per-instance storage and debug callbacks
        if self.layer_name == 'object_tracker' or self.layer_name == 'threading':
            r_body.append('    VkLayerInstanceDispatchTable *pInstanceTable = get_dispatch_table(%s_instance_table_map, instance);' % self.layer_name )
        else:
            r_body.append('    VkLayerInstanceDispatchTable *pInstanceTable = instance_dispatch_table(instance);')
        r_body.append('    pInstanceTable->DebugReportMessageEXT(instance, flags, objType, object, location, msgCode, pLayerPrefix, pMsg);')
        r_body.append('}')
        return "\n".join(r_body)

    def _gen_layer_get_global_extension_props(self, layer="generic"):
        ggep_body = []
        # generated layers do not provide any global extensions
        ggep_body.append('%s' % self.lineinfo.get())

        ggep_body.append('')
        if self.layer_name == 'object_tracker' or self.layer_name == 'threading':
            ggep_body.append('static const VkExtensionProperties instance_extensions[] = {')
            ggep_body.append('    {')
            ggep_body.append('        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,')
            ggep_body.append('        VK_EXT_DEBUG_REPORT_SPEC_VERSION')
            ggep_body.append('    }')
            ggep_body.append('};')
        ggep_body.append('VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pCount,  VkExtensionProperties* pProperties)')
        ggep_body.append('{')
        if self.layer_name == 'object_tracker' or self.layer_name == 'threading':
          ggep_body.append('    return util_GetExtensionProperties(1, instance_extensions, pCount, pProperties);')
        else:
          ggep_body.append('    return util_GetExtensionProperties(0, NULL, pCount, pProperties);')
        ggep_body.append('}')
        return "\n".join(ggep_body)

    def _gen_layer_get_global_layer_props(self, layer="generic"):
        ggep_body = []
        if layer == 'generic':
            # Do nothing, extension definition part of generic.h
            ggep_body.append('%s' % self.lineinfo.get())
        else:
            layer_name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', layer)
            layer_name = re.sub('([a-z0-9])([A-Z])', r'\1_\2', layer_name).lower()
            ggep_body.append('%s' % self.lineinfo.get())
            ggep_body.append('static const VkLayerProperties globalLayerProps[] = {')
            ggep_body.append('    {')
            ggep_body.append('        "VK_LAYER_LUNARG_%s",' % layer_name)
            ggep_body.append('        VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion')
            ggep_body.append('        VK_MAKE_VERSION(0, 1, 0), // implementationVersion')
            ggep_body.append('        "layer: %s",' % layer)
            ggep_body.append('    }')
            ggep_body.append('};')
        ggep_body.append('')
        ggep_body.append('%s' % self.lineinfo.get())
        ggep_body.append('')
        ggep_body.append('VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t *pCount,  VkLayerProperties* pProperties)')
        ggep_body.append('{')
        ggep_body.append('    return util_GetLayerProperties(ARRAY_SIZE(globalLayerProps), globalLayerProps, pCount, pProperties);')
        ggep_body.append('}')
        return "\n".join(ggep_body)

    def _gen_layer_get_physical_device_layer_props(self, layer="generic"):
        gpdlp_body = []
        if layer == 'generic':
            # Do nothing, extension definition part of generic.h
            gpdlp_body.append('%s' % self.lineinfo.get())
        else:
            gpdlp_body.append('%s' % self.lineinfo.get())
            gpdlp_body.append('static const VkLayerProperties deviceLayerProps[] = {')
            gpdlp_body.append('    {')
            gpdlp_body.append('        "VK_LAYER_LUNARG_%s",' % layer)
            gpdlp_body.append('        VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION),')
            gpdlp_body.append('        VK_MAKE_VERSION(0, 1, 0),')
            gpdlp_body.append('        "layer: %s",' % layer)
            gpdlp_body.append('    }')
            gpdlp_body.append('};')
        gpdlp_body.append('VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount, VkLayerProperties* pProperties)')
        gpdlp_body.append('{')
        gpdlp_body.append('    return util_GetLayerProperties(ARRAY_SIZE(deviceLayerProps), deviceLayerProps, pCount, pProperties);')
        gpdlp_body.append('}')
        gpdlp_body.append('')
        return "\n".join(gpdlp_body)

    def _generate_dispatch_entrypoints(self, qual=""):
        if qual:
            qual += " "

        funcs = []
        intercepted = []
        for proto in self.protos:
            if proto.name == "GetDeviceProcAddr" or proto.name == "GetInstanceProcAddr":
                continue
            else:
                intercept = self.generate_intercept(proto, qual)
                if intercept is None:
                    # fill in default intercept for certain entrypoints
                    if 'CreateDebugReportCallbackEXT' == proto.name:
                        intercept = self._gen_layer_dbg_create_msg_callback()
                    elif 'DestroyDebugReportCallbackEXT' == proto.name:
                        intercept = self._gen_layer_dbg_destroy_msg_callback()
                    elif 'DebugReportMessageEXT' == proto.name:
                        intercept = self._gen_debug_report_msg()
                    elif 'CreateDevice' == proto.name:
                        funcs.append('/* CreateDevice HERE */')
                    elif 'EnumerateInstanceExtensionProperties' == proto.name:
                        intercept = self._gen_layer_get_global_extension_props(self.layer_name)
                    elif 'EnumerateInstanceLayerProperties' == proto.name:
                        intercept = self._gen_layer_get_global_layer_props(self.layer_name)
                    elif 'EnumerateDeviceLayerProperties' == proto.name:
                        intercept = self._gen_layer_get_physical_device_layer_props(self.layer_name)

                if intercept is not None:
                    funcs.append(intercept)
                    if not "KHR" in proto.name:
                        intercepted.append(proto)

        prefix="vk"
        lookups = []
        for proto in intercepted:
            lookups.append("if (!strcmp(name, \"%s\"))" % proto.name)
            lookups.append("    return (PFN_vkVoidFunction) %s%s;" %
                    (prefix, proto.name))

        # add customized layer_intercept_proc
        body = []
        body.append('%s' % self.lineinfo.get())
        body.append("static inline PFN_vkVoidFunction layer_intercept_proc(const char *name)")
        body.append("{")
        body.append(generate_get_proc_addr_check("name"))
        body.append("")
        body.append("    name += 2;")
        body.append("    %s" % "\n    ".join(lookups))
        body.append("")
        body.append("    return NULL;")
        body.append("}")
        # add layer_intercept_instance_proc
        lookups = []
        for proto in self.protos:
            if not proto_is_global(proto):
                continue

            if not proto in intercepted:
                continue
            if proto.name == "CreateInstance":
                continue
            if proto.name == "CreateDevice":
                continue
            lookups.append("if (!strcmp(name, \"%s\"))" % proto.name)
            lookups.append("    return (PFN_vkVoidFunction) %s%s;" % (prefix, proto.name))

        body.append("static inline PFN_vkVoidFunction layer_intercept_instance_proc(const char *name)")
        body.append("{")
        body.append(generate_get_proc_addr_check("name"))
        body.append("")
        body.append("    name += 2;")
        body.append("    %s" % "\n    ".join(lookups))
        body.append("")
        body.append("    return NULL;")
        body.append("}")

        funcs.append("\n".join(body))
        return "\n\n".join(funcs)

    def _generate_extensions(self):
        exts = []
        exts.append('%s' % self.lineinfo.get())
        exts.append(self._gen_create_msg_callback())
        exts.append(self._gen_destroy_msg_callback())
        exts.append(self._gen_debug_report_msg())
        return "\n".join(exts)

    def _generate_layer_gpa_function(self, extensions=[], instance_extensions=[]):
        func_body = []
#
# New style of GPA Functions for the new layer_data/layer_logging changes
#
        if self.layer_name in ['object_tracker', 'threading', 'unique_objects']:
            func_body.append("VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* funcName)\n"
                             "{\n"
                             "    PFN_vkVoidFunction addr;\n"
                             "    if (!strcmp(\"vkGetDeviceProcAddr\", funcName)) {\n"
                             "        return (PFN_vkVoidFunction) vkGetDeviceProcAddr;\n"
                             "    }\n\n"
                             "    addr = layer_intercept_proc(funcName);\n"
                             "    if (addr)\n"
                             "        return addr;\n"
                             "    if (device == VK_NULL_HANDLE) {\n"
                             "        return NULL;\n"
                             "    }\n")
            if 0 != len(extensions):
                func_body.append('%s' % self.lineinfo.get())
                func_body.append('    layer_data *my_device_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);')
                for (ext_enable, ext_list) in extensions:
                    extra_space = ""
                    if 0 != len(ext_enable):
                        func_body.append('    if (my_device_data->%s) {' % ext_enable)
                        extra_space = "    "
                    for ext_name in ext_list:
                        func_body.append('    %sif (!strcmp("%s", funcName))\n'
                                         '        %sreturn reinterpret_cast<PFN_vkVoidFunction>(%s);' % (extra_space, ext_name, extra_space, ext_name))
                    if 0 != len(ext_enable):
                        func_body.append('    }\n')
            func_body.append("\n    if (get_dispatch_table(%s_device_table_map, device)->GetDeviceProcAddr == NULL)\n"
                             "        return NULL;\n"
                             "    return get_dispatch_table(%s_device_table_map, device)->GetDeviceProcAddr(device, funcName);\n"
                             "}\n" % (self.layer_name, self.layer_name))
            func_body.append("VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* funcName)\n"
                             "{\n"
                             "    PFN_vkVoidFunction addr;\n"
                             "    if (!strcmp(funcName, \"vkGetInstanceProcAddr\"))\n"
                             "        return (PFN_vkVoidFunction) vkGetInstanceProcAddr;\n"
                             "    if (!strcmp(funcName, \"vkCreateInstance\"))\n"
                             "        return (PFN_vkVoidFunction) vkCreateInstance;\n"
                             "    if (!strcmp(funcName, \"vkCreateDevice\"))\n"
                             "        return (PFN_vkVoidFunction) vkCreateDevice;\n"
                             "    addr = layer_intercept_instance_proc(funcName);\n"
                             "    if (addr) {\n"
                             "        return addr;"
                             "    }\n"
                             "    if (instance == VK_NULL_HANDLE) {\n"
                             "        return NULL;\n"
                             "    }\n"
                             )

            table_declared = False
            if 0 != len(instance_extensions):
                for (ext_enable, ext_list) in instance_extensions:
                    extra_space = ""
                    if 0 != len(ext_enable):
                        if ext_enable == 'msg_callback_get_proc_addr':
                            func_body.append("    layer_data *my_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);\n"
                                     "    addr = debug_report_get_instance_proc_addr(my_data->report_data, funcName);\n"
                                     "    if (addr) {\n"
                                     "        return addr;\n"
                                     "    }\n")
                        else:
                            if table_declared == False:
                                func_body.append("    VkLayerInstanceDispatchTable* pTable = get_dispatch_table(%s_instance_table_map, instance);" % self.layer_name)
                                table_declared = True
                            func_body.append('    if (instanceExtMap.size() != 0 && instanceExtMap[pTable].%s)' % ext_enable)
                            func_body.append('    {')
                            extra_space = "    "
                            for ext_name in ext_list:
                                if wsi_name(ext_name):
                                    func_body.append('%s' % wsi_ifdef(ext_name))
                                func_body.append('    %sif (!strcmp("%s", funcName))\n'
                                                 '            return reinterpret_cast<PFN_vkVoidFunction>(%s);' % (extra_space, ext_name, ext_name))
                                if wsi_name(ext_name):
                                    func_body.append('%s' % wsi_endif(ext_name))
                            if 0 != len(ext_enable):
                               func_body.append('    }\n')

            func_body.append("    if (get_dispatch_table(%s_instance_table_map, instance)->GetInstanceProcAddr == NULL) {\n"
                             "        return NULL;\n"
                             "    }\n"
                             "    return get_dispatch_table(%s_instance_table_map, instance)->GetInstanceProcAddr(instance, funcName);\n"
                             "}\n" % (self.layer_name, self.layer_name))
            return "\n".join(func_body)
        else:
            func_body.append('%s' % self.lineinfo.get())
            func_body.append("VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* funcName)\n"
                             "{\n"
                             "    PFN_vkVoidFunction addr;\n")
            if self.layer_name == 'generic':
                func_body.append("\n"
                             "    if (!strcmp(\"vkGetDeviceProcAddr\", funcName)) {\n"
                             "        return (PFN_vkVoidFunction) vkGetDeviceProcAddr;\n"
                             "    }\n\n"
                             "    addr = layer_intercept_proc(funcName);\n"
                             "    if (addr)\n"
                             "        return addr;")
            else:
                func_body.append("\n"
                             "    loader_platform_thread_once(&initOnce, init%s);\n\n"
                             "    if (!strcmp(\"vkGetDeviceProcAddr\", funcName)) {\n"
                             "        return (PFN_vkVoidFunction) vkGetDeviceProcAddr;\n"
                             "    }\n\n"
                             "    addr = layer_intercept_proc(funcName);\n"
                             "    if (addr)\n"
                             "        return addr;" % self.layer_name)
            func_body.append("    if (device == VK_NULL_HANDLE) {\n"
                             "        return NULL;\n"
                             "    }\n")
            func_body.append('')
            func_body.append('    VkLayerDispatchTable *pDisp =  device_dispatch_table(device);')
            if 0 != len(extensions):
                extra_space = ""
                for (ext_enable, ext_list) in extensions:
                    if 0 != len(ext_enable):
                        func_body.append('    if (deviceExtMap.size() != 0 && deviceExtMap[pDisp].%s)' % ext_enable)
                        func_body.append('    {')
                        extra_space = "    "
                    for ext_name in ext_list:
                        func_body.append('    %sif (!strcmp("%s", funcName))\n'
                                         '            return reinterpret_cast<PFN_vkVoidFunction>(%s);' % (extra_space, ext_name, ext_name))
                    if 0 != len(ext_enable):
                        func_body.append('    }')
            func_body.append('%s' % self.lineinfo.get())
            func_body.append("    {\n"
                             "        if (pDisp->GetDeviceProcAddr == NULL)\n"
                             "            return NULL;\n"
                             "        return pDisp->GetDeviceProcAddr(device, funcName);\n"
                             "    }\n"
                             "}\n")
            func_body.append('%s' % self.lineinfo.get())
            func_body.append("VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* funcName)\n"
                             "{\n"
                             "    PFN_vkVoidFunction addr;\n"
                             "    if (!strcmp(funcName, \"vkGetInstanceProcAddr\"))\n"
                             "        return (PFN_vkVoidFunction) vkGetInstanceProcAddr;\n"
                             "    if (!strcmp(funcName, \"vkCreateInstance\"))\n"
                             "        return (PFN_vkVoidFunction) vkCreateInstance;\n"
                             "    if (!strcmp(funcName, \"vkCreateDevice\"))\n"
                             "        return (PFN_vkVoidFunction) vkCreateDevice;\n"
                             )
            if self.layer_name == 'generic':
                func_body.append("\n"
                             "    addr = layer_intercept_instance_proc(funcName);\n"
                             "    if (addr)\n"
                             "        return addr;")
            else:
                func_body.append(
                             "    loader_platform_thread_once(&initOnce, init%s);\n\n"
                             "    addr = layer_intercept_instance_proc(funcName);\n"
                             "    if (addr)\n"
                             "        return addr;" % self.layer_name)
            func_body.append("    if (instance == VK_NULL_HANDLE) {\n"
                             "        return NULL;\n"
                             "    }\n")
            func_body.append("")
            func_body.append("    VkLayerInstanceDispatchTable* pTable = instance_dispatch_table(instance);\n")
            if 0 != len(instance_extensions):
                extra_space = ""
                for (ext_enable, ext_list) in instance_extensions:
                    if 0 != len(ext_enable):
                        if ext_enable == 'msg_callback_get_proc_addr':
                            func_body.append("    layer_data *my_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);\n"
                                     "    addr = debug_report_get_instance_proc_addr(my_data->report_data, funcName);\n"
                                     "    if (addr) {\n"
                                     "        return addr;\n"
                                     "    }\n")
                        else:
                            func_body.append('    if (instanceExtMap.size() != 0 && instanceExtMap[pTable].%s)' % ext_enable)
                            func_body.append('    {')
                            extra_space = "    "
                            for ext_name in ext_list:
                                if wsi_name(ext_name):
                                    func_body.append('%s' % wsi_ifdef(ext_name))
                                func_body.append('    %sif (!strcmp("%s", funcName))\n'
                                         '            return reinterpret_cast<PFN_vkVoidFunction>(%s);' % (extra_space, ext_name, ext_name))
                                if wsi_name(ext_name):
                                    func_body.append('%s' % wsi_endif(ext_name))
                            if 0 != len(ext_enable):
                                func_body.append('    }\n')

            func_body.append("    if (pTable->GetInstanceProcAddr == NULL)\n"
                             "        return NULL;\n"
                             "    return pTable->GetInstanceProcAddr(instance, funcName);\n"
                             "}\n")
            return "\n".join(func_body)

    def _generate_layer_initialization(self, init_opts=False, prefix='vk', lockname=None, condname=None):
        func_body = ["#include \"vk_dispatch_table_helper.h\""]
        func_body.append('%s' % self.lineinfo.get())
        func_body.append('static void init_%s(layer_data *my_data, const VkAllocationCallbacks *pAllocator)\n'
                         '{\n' % self.layer_name)
        if init_opts:
            func_body.append('%s' % self.lineinfo.get())
            func_body.append('')
            func_body.append('    layer_debug_actions(my_data->report_data, my_data->logging_callback, pAllocator, "lunarg_%s");' % self.layer_name)
            func_body.append('')
        if lockname is not None:
            func_body.append('%s' % self.lineinfo.get())
            func_body.append("    if (!%sLockInitialized)" % lockname)
            func_body.append("    {")
            func_body.append("        // TODO/TBD: Need to delete this mutex sometime.  How???")
            func_body.append("        loader_platform_thread_create_mutex(&%sLock);" % lockname)
            if condname is not None:
                func_body.append("        loader_platform_thread_init_cond(&%sCond);" % condname)
            func_body.append("        %sLockInitialized = 1;" % lockname)
            func_body.append("    }")
        func_body.append("}\n")
        func_body.append('')
        return "\n".join(func_body)

class LayerFuncsSubcommand(Subcommand):
    def generate_header(self):
        return '#include <vulkan/vk_layer.h>\n#include "loader.h"'

    def generate_body(self):
        return self._generate_dispatch_entrypoints("static")

class GenericLayerSubcommand(Subcommand):
    def generate_header(self):
        gen_header = []
        gen_header.append('%s' % self.lineinfo.get())
        gen_header.append('#include <stdio.h>')
        gen_header.append('#include <stdlib.h>')
        gen_header.append('#include <string.h>')
        gen_header.append('#include <unordered_map>')
        gen_header.append('#include "vk_loader_platform.h"')
        gen_header.append('#include "vulkan/vk_layer.h"')
        gen_header.append('#include "vk_layer_config.h"')
        gen_header.append('#include "vk_layer_logging.h"')
        gen_header.append('#include "vk_layer_table.h"')
        gen_header.append('#include "vk_layer_extension_utils.h"')
        gen_header.append('#include "vk_layer_utils.h"')
        gen_header.append('')
        gen_header.append('#include "generic.h"')
        gen_header.append('')
        gen_header.append('%s' % self.lineinfo.get())
        gen_header.append('#define LAYER_EXT_ARRAY_SIZE 1')
        gen_header.append('#define LAYER_DEV_EXT_ARRAY_SIZE 1')
        gen_header.append('//static LOADER_PLATFORM_THREAD_ONCE_DECLARATION(initOnce);')
        gen_header.append('static std::unordered_map<void *, layer_data *> layer_data_map;\n')
        gen_header.append('template layer_data *get_my_data_ptr<layer_data>(')
        gen_header.append('        void *data_key,')
        gen_header.append('        std::unordered_map<void *, layer_data *> &data_map);\n')
        gen_header.append('')
        return "\n".join(gen_header)

    def generate_intercept(self, proto, qual):
        if proto.name in [ 'EnumerateInstanceLayerProperties', 'EnumerateInstanceExtensionProperties', 'EnumerateDeviceLayerProperties', 'EnumerateDeviceExtensionProperties' ]:
            # use default version
            return None
        decl = proto.c_func(prefix="vk", attr="VKAPI")
        ret_val = ''
        stmt = ''
        funcs = []
        table = ''
        if proto_is_global(proto):
           table = 'Instance'
        if proto.ret != "void":
            funcs.append('%s' % self.lineinfo.get())
            ret_val = "%s result = " % proto.ret
            stmt = "    return result;\n"
        if proto.name == "CreateDevice":
            funcs.append('%s' % self.lineinfo.get())
            funcs.append('%s%s\n'
                     '{\n'
                     '    layer_data *my_instance_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);\n'
                     '    char str[1024];\n'
                     '    sprintf(str, "At start of Generic layered %s\\n");\n'
                     '    log_msg(my_instance_data->report_data, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT,'
                     '            (uint64_t)physicalDevice, __LINE__, 0, (char *) "generic", "%%s", (char *) str);\n'
                     '    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);\n'
                     '    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;\n'
                     '    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;\n'
                     '    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(NULL, "vkCreateDevice");\n'
                     '    if (fpCreateDevice == NULL) {\n'
                     '        return VK_ERROR_INITIALIZATION_FAILED;\n'
                     '    }\n'
                     '    // Advance the link info for the next element on the chain\n'
                     '    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;\n'
                     '    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);\n'
                     '    if (result != VK_SUCCESS) {\n'
                     '        return result;\n'
                     '    }\n'
                     '    layer_data *my_device_data = get_my_data_ptr(get_dispatch_key(*pDevice), layer_data_map);\n'
                     '    initDeviceTable(*pDevice, fpGetDeviceProcAddr);\n'
                     '    my_device_data->report_data = layer_debug_report_create_device(my_instance_data->report_data, *pDevice);\n'
                     '    createDeviceRegisterExtensions(pCreateInfo, *pDevice);\n'
                     '    sprintf(str, "Completed generic layered %s\\n");\n'
                     '    log_msg(my_device_data->report_data, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT, (uint64_t)physicalDevice, __LINE__, 0, (char *) "generic", "%%s", (char *) str);\n'
                     '    %s'
                     '}' % (qual, decl, proto.name, proto.name, stmt))
        elif proto.name == "DestroyDevice":
            funcs.append('%s' % self.lineinfo.get())
            funcs.append('%s%s\n'
                         '{\n'
                         '    dispatch_key key = get_dispatch_key(device);\n'
                         '    VkLayerDispatchTable *pDisp  =  device_dispatch_table(device);\n'
                         '    pDisp->DestroyDevice(device, pAllocator);\n'
                         '    deviceExtMap.erase(pDisp);\n'
                         '    destroy_device_dispatch_table(key);\n'
                         '}\n' % (qual, decl))
        elif proto.name == "DestroyInstance":
            funcs.append('%s' % self.lineinfo.get())
            funcs.append('%s%s\n'
                         '{\n'
                         '    dispatch_key key = get_dispatch_key(instance);\n'
                         '    VkLayerInstanceDispatchTable *pDisp  =  instance_dispatch_table(instance);\n'
                         '    pDisp->DestroyInstance(instance, pAllocator);\n'
                         '    // Clean up logging callback, if any\n'
                         '    layer_data *my_data = get_my_data_ptr(key, layer_data_map);\n'
                         '    while (my_data->logging_callback.size() > 0) {'
                         '        VkDebugReportCallbackEXT callback = my_data->logging_callback.back();'
                         '        layer_destroy_msg_callback(my_data->report_data, callback, pAllocator);'
                         '        my_data->logging_callback.pop_back();'
                         '    }\n\n'
                         '    layer_debug_report_destroy_instance(my_data->report_data);\n'
                         '    layer_data_map.erase(key);\n'
                         '    instanceExtMap.erase(pDisp);\n'
                         '    destroy_instance_dispatch_table(key);\n'
                         '}\n' % (qual, decl))
        elif proto.name == "CreateInstance":
            funcs.append('%s' % self.lineinfo.get())
            # CreateInstance needs to use the second parm instead of the first to set the correct dispatch table
            funcs.append('%s%s\n'
                         '{\n'
                         '    char str[1024];\n'
                         '    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);\n'
                         '    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;\n'
                         '    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");\n'
                         '    if (fpCreateInstance == NULL) {\n'
                         '        return VK_ERROR_INITIALIZATION_FAILED;\n'
                         '    }\n'
                         '    // Advance the link info for the next element on the chain\n'
                         '    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;\n'
                         '    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);\n'
                         '    if (result != VK_SUCCESS) {\n'
                         '        return result;\n'
                         '    }\n'
                         '    VkLayerInstanceDispatchTable *pTable = initInstanceTable(*pInstance, fpGetInstanceProcAddr);\n'
                         '    createInstanceRegisterExtensions(pCreateInfo, *pInstance);\n'
                         '    layer_data *my_data = get_my_data_ptr(get_dispatch_key(*pInstance), layer_data_map);\n'
                         '    my_data->report_data = debug_report_create_instance(\n'
                         '            pTable,\n'
                         '            *pInstance,\n'
                         '            pCreateInfo->enabledExtensionCount,\n'
                         '            pCreateInfo->ppEnabledExtensionNames);\n'
                         '    init_generic(my_data, pAllocator);\n'
                         '    sprintf(str, "Completed generic layered %s\\n");\n'
                         '    log_msg(my_data->report_data, VK_DEBUG_REPORT_INFORMATION_BIT_EXT, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT, (uint64_t)*pInstance, __LINE__, 0, (char *) "generic", "%%s", (char *) str);\n'
                         '    return result;\n'
                         '}\n' % (qual, decl, proto.name))
        else:
            if wsi_name(proto.name):
                funcs.append('%s' % wsi_ifdef(proto.name))
            funcs.append('%s' % self.lineinfo.get())
            dispatch_param = proto.params[0].name
            # Must use 'instance' table for these APIs, 'device' table otherwise
            table_type = ""
            if proto_is_global(proto):
                table_type = "instance"
            else:
                table_type = "device"
            funcs.append('%s%s\n'
                     '{\n'
                     '    %s%s_dispatch_table(%s)->%s;\n'
                     '%s'
                     '}' % (qual, decl, ret_val, table_type, dispatch_param, proto.c_call(), stmt))
            if wsi_name(proto.name):
                funcs.append('%s' % wsi_endif(proto.name))
        return "\n\n".join(funcs)

    def generate_body(self):
        self.layer_name = "generic"
        instance_extensions=[('msg_callback_get_proc_addr', []),
                     ('wsi_enabled',
                     ['vkGetPhysicalDeviceSurfaceSupportKHR',
                      'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                      'vkGetPhysicalDeviceSurfaceFormatsKHR',
                      'vkGetPhysicalDeviceSurfacePresentModesKHR'])]
        extensions=[('wsi_enabled',
                     ['vkCreateSwapchainKHR',
                      'vkDestroySwapchainKHR', 'vkGetSwapchainImagesKHR',
                      'vkAcquireNextImageKHR', 'vkQueuePresentKHR'])]
        body = [self._generate_layer_initialization(True),
                self._generate_dispatch_entrypoints("VK_LAYER_EXPORT"),
                self._gen_create_msg_callback(),
                self._gen_destroy_msg_callback(),
                self._gen_debug_report_msg(),
                self._generate_layer_gpa_function(extensions, instance_extensions)]

        return "\n\n".join(body)

class ApiDumpSubcommand(Subcommand):
    def generate_header(self):
        header_txt = []
        header_txt.append('%s' % self.lineinfo.get())
        header_txt.append('#include <fstream>')
        header_txt.append('#include <iostream>')
        header_txt.append('#include <string>')
        header_txt.append('#include <string.h>')
        header_txt.append('')
        header_txt.append('#include "vk_loader_platform.h"')
        header_txt.append('#include "vulkan/vk_layer.h"')
        header_txt.append('#include "vk_struct_string_helper_cpp.h"')
        header_txt.append('#include "vk_layer_table.h"')
        header_txt.append('#include "vk_layer_extension_utils.h"')
        header_txt.append('#include "vk_layer_config.h"')
        header_txt.append('#include "vk_layer_utils.h"')
        header_txt.append('#include <unordered_map>')
        header_txt.append('#include "api_dump.h"')
        header_txt.append('')
        header_txt.append('static std::ofstream fileStream;')
        header_txt.append('')
        header_txt.append('#ifdef ANDROID')
        header_txt.append('static std::string fileName = "/sdcard/Android/vk_apidump.txt";')
        header_txt.append('#else')
        header_txt.append('static std::string fileName = "vk_apidump.txt";')
        header_txt.append('#endif')
        header_txt.append('')
        header_txt.append('std::ostream* outputStream = NULL;')
        header_txt.append('void ConfigureOutputStream(bool writeToFile, bool flushAfterWrite)')
        header_txt.append('{')
        header_txt.append('    if(writeToFile)')
        header_txt.append('    {')
        header_txt.append('        if (fileName == "stdout")')
        header_txt.append('        {')
        header_txt.append('            outputStream = &std::cout;')
        header_txt.append('            (*outputStream) << std::endl << "api_dump output filename \'stdout\' specified. Writing to STDOUT instead of a file." << std::endl << std::endl;')
        header_txt.append('        } else {')
        header_txt.append('            fileStream.open(fileName);')
        header_txt.append('            if ((fileStream.rdstate() & fileStream.failbit) != 0) {')
        header_txt.append('                outputStream = &std::cout;')
        header_txt.append('                (*outputStream) << std::endl << "api_dump ERROR: Bad output filename specified: " << fileName << ". Writing to STDOUT instead" << std::endl << std::endl;')
        header_txt.append('            }')
        header_txt.append('            else')
        header_txt.append('                outputStream = &fileStream;')
        header_txt.append('        }')
        header_txt.append('    }')
        header_txt.append('    else')
        header_txt.append('    {')
        header_txt.append('        outputStream = &std::cout;')
        header_txt.append('    }')
        header_txt.append('')
        header_txt.append('    if(flushAfterWrite)')
        header_txt.append('    {')
        header_txt.append('        outputStream->sync_with_stdio(true);')
        header_txt.append('    }')
        header_txt.append('    else')
        header_txt.append('    {')
        header_txt.append('        outputStream->sync_with_stdio(false);')
        header_txt.append('    }')
        header_txt.append('}')
        header_txt.append('')
        header_txt.append('%s' % self.lineinfo.get())
        header_txt.append('static bool g_ApiDumpDetailed = true;')
        header_txt.append('static uint64_t g_frameCounter = 0;')
        header_txt.append('')
        header_txt.append('static LOADER_PLATFORM_THREAD_ONCE_DECLARATION(initOnce);')
        header_txt.append('')
        header_txt.append('static int printLockInitialized = 0;')
        header_txt.append('static loader_platform_thread_mutex printLock;')
        header_txt.append('')
        header_txt.append('%s' % self.lineinfo.get())
        header_txt.append('#define LAYER_EXT_ARRAY_SIZE 1')
        header_txt.append('#define LAYER_DEV_EXT_ARRAY_SIZE 1')
        header_txt.append('#define MAX_TID 513')
        header_txt.append('static loader_platform_thread_id tidMapping[MAX_TID] = {0};')
        header_txt.append('static uint32_t maxTID = 0;')
        header_txt.append('// Map actual TID to an index value and return that index')
        header_txt.append('//  This keeps TIDs in range from 0-MAX_TID and simplifies compares between runs')
        header_txt.append('static uint32_t getTIDIndex() {')
        header_txt.append('    loader_platform_thread_id tid = loader_platform_get_thread_id();')
        header_txt.append('    for (uint32_t i = 0; i < maxTID; i++) {')
        header_txt.append('        if (tid == tidMapping[i])')
        header_txt.append('            return i;')
        header_txt.append('    }')
        header_txt.append("    // Don't yet have mapping, set it and return newly set index")
        header_txt.append('    uint32_t retVal = (uint32_t) maxTID;')
        header_txt.append('    tidMapping[maxTID++] = tid;')
        header_txt.append('    assert(maxTID < MAX_TID);')
        header_txt.append('    return retVal;')
        header_txt.append('}')
        header_txt.append('')
        return "\n".join(header_txt)

    def generate_init(self):
        func_body = []
        func_body.append('%s' % self.lineinfo.get())
        func_body.append('#include "vk_dispatch_table_helper.h"')
        func_body.append('#include "vk_layer_config.h"')
        func_body.append('')
        func_body.append('static void init%s(void)' % self.layer_name)
        func_body.append('{')
        func_body.append('    using namespace StreamControl;')
        func_body.append('')
        func_body.append('    char const*const logName = getLayerOption("lunarg_api_dump.log_filename");')
        func_body.append('    if(logName != NULL)')
        func_body.append('    {')
        func_body.append('        fileName = logName;')
        func_body.append('    }')
        func_body.append('')
        func_body.append('    char const*const detailedStr = getLayerOption("lunarg_api_dump.detailed");')
        func_body.append('    if(detailedStr != NULL)')
        func_body.append('    {')
        func_body.append('        if(strcmp(detailedStr, "TRUE") == 0)')
        func_body.append('        {')
        func_body.append('            g_ApiDumpDetailed = true;')
        func_body.append('        }')
        func_body.append('        else if(strcmp(detailedStr, "FALSE") == 0)')
        func_body.append('        {')
        func_body.append('            g_ApiDumpDetailed = false;')
        func_body.append('        }')
        func_body.append('    }')
        func_body.append('')
        func_body.append('    char const*const writeToFileStr = getLayerOption("lunarg_api_dump.file");')
        func_body.append('    bool writeToFile = false;')
        func_body.append('    if(writeToFileStr != NULL)')
        func_body.append('    {')
        func_body.append('        if(strcmp(writeToFileStr, "TRUE") == 0)')
        func_body.append('        {')
        func_body.append('            writeToFile = true;')
        func_body.append('        }')
        func_body.append('        else if(strcmp(writeToFileStr, "FALSE") == 0)')
        func_body.append('        {')
        func_body.append('            writeToFile = false;')
        func_body.append('        }')
        func_body.append('    }')
        func_body.append('')
        func_body.append('%s' % self.lineinfo.get())
        func_body.append('    char const*const noAddrStr = getLayerOption("lunarg_api_dump.no_addr");')
        func_body.append('    if(noAddrStr != NULL)')
        func_body.append('    {')
        func_body.append('        if(strcmp(noAddrStr, "FALSE") == 0)')
        func_body.append('        {')
        func_body.append('            StreamControl::writeAddress = true;')
        func_body.append('        }')
        func_body.append('        else if(strcmp(noAddrStr, "TRUE") == 0)')
        func_body.append('        {')
        func_body.append('            StreamControl::writeAddress = false;')
        func_body.append('        }')
        func_body.append('    }')
        func_body.append('')
        func_body.append('    char const*const flushAfterWriteStr = getLayerOption("lunarg_api_dump.flush");')
        func_body.append('    bool flushAfterWrite = false;')
        func_body.append('    if(flushAfterWriteStr != NULL)')
        func_body.append('    {')
        func_body.append('        if(strcmp(flushAfterWriteStr, "TRUE") == 0)')
        func_body.append('        {')
        func_body.append('            flushAfterWrite = true;')
        func_body.append('        }')
        func_body.append('        else if(strcmp(flushAfterWriteStr, "FALSE") == 0)')
        func_body.append('        {')
        func_body.append('            flushAfterWrite = false;')
        func_body.append('        }')
        func_body.append('    }')
        func_body.append('')
        func_body.append('%s' % self.lineinfo.get())
        func_body.append('    ConfigureOutputStream(writeToFile, flushAfterWrite);')
        func_body.append('')
        func_body.append('    if (!printLockInitialized)')
        func_body.append('    {')
        func_body.append('        // TODO/TBD: Need to delete this mutex sometime.  How???')
        func_body.append('        loader_platform_thread_create_mutex(&printLock);')
        func_body.append('        printLockInitialized = 1;')
        func_body.append('    }')
        func_body.append('}')
        func_body.append('')
        return "\n".join(func_body)

    def generate_intercept(self, proto, qual):
        if proto.name in [ 'EnumerateInstanceLayerProperties','EnumerateInstanceExtensionProperties','EnumerateDeviceLayerProperties','EnumerateDeviceExtensionProperties']:
            return None
        decl = proto.c_func(prefix="vk", attr="VKAPI")
        ret_val = ''
        stmt = ''
        funcs = []
        sp_param_dict = {} # Store 'index' for struct param to print, or an name of binding "Count" param for array to print
        create_params = 0 # Num of params at end of function that are created and returned as output values
        if 'AllocateDescriptorSets' in proto.name:
            create_params = -1
        elif 'Create' in proto.name or 'Alloc' in proto.name or 'MapMemory' in proto.name:
            create_params = -1
        if proto.ret != "void":
            ret_val = "%s result = " % proto.ret
            stmt = "    return result;\n"
        f_open = 'loader_platform_thread_lock_mutex(&printLock);\n    '
        log_func = '%s\n' % self.lineinfo.get()
        log_func += '    if (StreamControl::writeAddress == true) {'
        log_func += '\n        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vk%s(' % proto.name
        log_func_no_addr = '\n        (*outputStream) << "t{" << getTIDIndex() << "} f{" << g_frameCounter << "} vk%s(' % proto.name
        f_close = '\n    loader_platform_thread_unlock_mutex(&printLock);'
        pindex = 0
        prev_count_name = ''
        for p in proto.params:
            cp = False
            if 0 != create_params:
                # If this is any of the N last params of the func, treat as output
                for y in range(-1, create_params-1, -1):
                    if p.name == proto.params[y].name:
                        cp = True
            (pft, pfi) = self._get_printf_params(p.ty, p.name, cp, cpp=True)
            if p.name == "pSwapchain" or p.name == "pSwapchainImages":
                log_func += '%s = 0x" << nouppercase <<  hex << HandleCast(%s) << dec << ", ' % (p.name, p.name)
            elif p.name == "swapchain":
                log_func += '%s%s = 0x" << nouppercase <<  hex << HandleCast(%s) << dec << ", ' % (cast, p.name, p.name)
            elif p.name == "visual_id":
                log_func += '%s = 0x" << nouppercase <<  hex << %s << dec << ", ' % (p.name, p.name)
            elif '0x' in pft:
                if "*" in cast:
                    log_func += '" << (%s ? \"%s\" : \"\") << "%s = 0x" << nouppercase <<  hex << (%s ? %s : 0) << dec << ", ' % (p.name, cast, p.name, p.name, pfi)
                else:
                    log_func += '%s%s = 0x" << nouppercase <<  hex << %s << dec << ", ' % (cast, p.name, pfi)
            else:
                log_func += '%s = " << %s << ", ' % (p.name, pfi)
            if "%p" == pft:
                log_func_no_addr += '%s = address, ' % (p.name)
            else:
                log_func_no_addr += '%s = " << %s << ", ' % (p.name, pfi)
            if prev_count_name != '' and (prev_count_name.replace('Count', '')[1:] in p.name):
                sp_param_dict[pindex] = prev_count_name
                prev_count_name = ''
            elif vk_helper.is_type(p.ty.strip('*').replace('const ', ''), 'struct'):
                sp_param_dict[pindex] = 'index'
            if p.name.endswith('Count'):
                if '*' in p.ty:
                    prev_count_name = "*%s" % p.name
                else:
                    prev_count_name = p.name
            else:
                prev_count_name = ''
            pindex += 1
        log_func = log_func.strip(', ')
        log_func_no_addr = log_func_no_addr.strip(', ')
        if proto.ret == "VkResult":
            log_func += ') = " << string_VkResult((VkResult)result) << std::endl'
            log_func_no_addr += ') = " << string_VkResult((VkResult)result) << std::endl'
        elif proto.ret == "void*":
            log_func += ') = " << HandleCast(result) << endl'
            log_func_no_addr += ') = " << HandleCast(result) << endl'
        else:
            log_func += ')\\n"'
            log_func_no_addr += ')\\n"'
        log_func += ';'
        log_func_no_addr += ';'
        log_func += '\n    }\n    else {%s\n    }' % log_func_no_addr;
        log_func += '\n%s' % self.lineinfo.get()
        # log_func += '\n// Proto %s has param_dict: %s' % (proto.name, sp_param_dict)
        if len(sp_param_dict) > 0:
            indent = '    '
            log_func += '\n%sif (g_ApiDumpDetailed) {' % indent
            indent += '    '
            i_decl = False
            log_func += '\n%s' % self.lineinfo.get()
            log_func += '\n%sstd::string tmp_str;' % indent
            for sp_index in sp_param_dict:
                # log_func += '\n// sp_index: %s' % str(sp_index)
                if 'index' == sp_param_dict[sp_index]:
                    cis_print_func = 'vk_print_%s' % (proto.params[sp_index].ty.replace('const ', '').strip('*').lower())
                    local_name = proto.params[sp_index].name
                    if '*' not in proto.params[sp_index].ty:
                        local_name = '&%s' % proto.params[sp_index].name
                    log_func += '\n%s' % self.lineinfo.get()
                    log_func += '\n%sif (%s) {' % (indent, local_name)
                    indent += '    '
                    log_func += '\n%stmp_str = %s(%s, "    ");' % (indent, cis_print_func, local_name)
                    log_func += '\n%s(*outputStream) << "   %s (" << %s << ")" << std::endl << tmp_str << std::endl;' % (indent, local_name, local_name)
                    indent = indent[4:]
                    log_func += '\n%s}' % (indent)
                else: # We have a count value stored to iterate over an array
                    print_cast = ''
                    print_func = ''
                    if vk_helper.is_type(proto.params[sp_index].ty.strip('*').replace('const ', ''), 'struct'):
                        print_cast = '&'
                        print_func = 'vk_print_%s' % proto.params[sp_index].ty.replace('const ', '').strip('*').lower()
                    else:
                        print_cast = ''
                        print_func = 'string_convert_helper'
                        #cis_print_func = 'tmp_str = string_convert_helper(HandleCast(%s[i]), "    ");' % proto.params[sp_index].name
                    if not i_decl:
                        log_func += '\n%suint32_t i;' % (indent)
                        i_decl = True
                    log_func += '\n%sif (%s) {' % (indent, proto.params[sp_index].name)
                    indent += '    '
                    log_func += '\n%s' % self.lineinfo.get()
                    log_func += '\n%sfor (i = 0; i < %s; i++) {' % (indent, sp_param_dict[sp_index])
                    indent += '    '
                    if not proto.params[sp_index].ty.strip('*').replace('const ', '') in vk_helper_api_dump.opaque_types:
                        cis_print_func = 'string tmp_str = %s(%s%s[i], "    ");' % (print_func, print_cast, proto.params[sp_index].name)
                        log_func += '\n%sif (StreamControl::writeAddress == true) {' % (indent)
                        indent += '    '
                        log_func += '\n%s%s' % (indent, cis_print_func)
                        log_func += '\n%s(*outputStream) << "   %s[" << i << "]:\\n" << tmp_str << endl;' % (indent, proto.params[sp_index].name)
                    else:
                        log_func += '\n%sif (StreamControl::writeAddress == true) {' % (indent)
                        indent += '    '
                        log_func += '\n%s(*outputStream) << "   %s[" << i << "] = 0x" << nouppercase << hex << HandleCast(%s[i]) << dec << endl;' % (indent, proto.params[sp_index].name, proto.params[sp_index].name)
                    indent = indent[4:]
                    log_func += '\n%s}' % (indent)
                    indent = indent[4:]
                    log_func += '\n%s}' % (indent)
                    indent = indent[4:]
                    log_func += '\n%s}' % (indent)
            indent = indent[4:]
            log_func += '\n%s}' % (indent)
        table_type = ''
        if proto_is_global(proto):
           table_type = 'instance'
        else:
           table_type = 'device'
        dispatch_param = proto.params[0].name

        if proto.name == "CreateInstance":
            dispatch_param = '*' + proto.params[1].name
            funcs.append('%s%s\n'
                     '{\n'
                     '    using namespace StreamControl;\n'
                     '    loader_platform_thread_once(&initOnce, initapi_dump);\n'
                     '    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);\n'
                     '    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;\n'
                     '    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");\n'
                     '    if (fpCreateInstance == NULL) {\n'
                     '        (*outputStream) << "t{" << getTIDIndex() << "} " << g_frameCounter << " vkCreateInstance FAILED TO FIND PROC ADDRESS" << endl;\n'
                     '        return VK_ERROR_INITIALIZATION_FAILED;\n'
                     '    }\n'
                     '    // Advance the link info for the next element on the chain\n'
                     '    assert(chain_info->u.pLayerInfo);\n'
                     '    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;\n'
                     '    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);\n'
                     '    if (result == VK_SUCCESS) {\n'
                     '        initInstanceTable(*pInstance, fpGetInstanceProcAddr);\n'
                     '        createInstanceRegisterExtensions(pCreateInfo, *pInstance);\n'
                     '    }\n'
                     '    %s%s%s\n'
                     '%s'
                     '}\n' % (qual, decl, f_open, log_func, f_close, stmt))

        elif proto.name == "CreateDevice":
            funcs.append('%s\n' % self.lineinfo.get())
            funcs.append('%s%s\n'
                     '{\n'
                     '    using namespace StreamControl;\n'
                     '    %sexplicit_CreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);\n'
                     '    %s%s%s\n'
                     '%s'
                     '}' % (qual, decl, ret_val, f_open, log_func, f_close, stmt))
        elif proto.name == "DestroyDevice":
            funcs.append('%s%s\n'
                 '{\n'
                 '    using namespace StreamControl;\n'
                 '    dispatch_key key = get_dispatch_key(device);\n'
                 '    VkLayerDispatchTable *pDisp  = %s_dispatch_table(%s);\n'
                 '    %spDisp->%s;\n'
                 '    deviceExtMap.erase(pDisp);\n'
                 '    destroy_device_dispatch_table(key);\n'
                 '    %s%s%s\n'
                 '%s'
                 '}' % (qual, decl, table_type, dispatch_param, ret_val, proto.c_call(), f_open, log_func, f_close, stmt))
        elif proto.name == "DestroyInstance":
            funcs.append('%s%s\n'
                 '{\n'
                 '    using namespace StreamControl;\n'
                 '    dispatch_key key = get_dispatch_key(instance);\n'
                 '    VkLayerInstanceDispatchTable *pDisp  = %s_dispatch_table(%s);\n'
                 '    %spDisp->%s;\n'
                 '    instanceExtMap.erase(pDisp);\n'
                 '    destroy_instance_dispatch_table(key);\n'
                 '    %s%s%s\n'
                 '%s'
                 '}' % (qual, decl, table_type, dispatch_param, ret_val, proto.c_call(), f_open, log_func, f_close, stmt))
        elif proto.name == "QueuePresentKHR":
            funcs.append('%s%s\n'
                 '{\n'
                 '    using namespace StreamControl;\n'
                 '    using namespace std;\n'
                 '    VkLayerDispatchTable *pDisp  = %s_dispatch_table(%s);\n'
                 '    %spDisp->%s;\n'
                 '    %s%s%s\n'
                 '    ++g_frameCounter;\n'
                 '%s'
                 '}' % (qual, decl, table_type, dispatch_param, ret_val, proto.c_call(), f_open, log_func, f_close, stmt))
        else:
            if wsi_name(decl):
                funcs.append('%s' % wsi_ifdef(decl))
            funcs.append('%s%s\n'
                     '{\n'
                     '    using namespace StreamControl;\n'
                     '    %s%s_dispatch_table(%s)->%s;\n'
                     '    %s%s%s\n'
                     '%s'
                     '}' % (qual, decl, ret_val, table_type, dispatch_param, proto.c_call(), f_open, log_func, f_close, stmt))
            if wsi_name(decl):
                funcs.append('%s' % wsi_endif(decl))
        return "\n\n".join(funcs)

    def generate_body(self):
        self.layer_name = "api_dump"
        if self.wsi == 'Win32':
            instance_extensions=[('wsi_enabled',
                                  ['vkDestroySurfaceKHR',
                                   'vkGetPhysicalDeviceSurfaceSupportKHR',
                                   'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                   'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                   'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                   'vkCreateWin32SurfaceKHR',
                                   'vkGetPhysicalDeviceWin32PresentationSupportKHR'])]
        elif self.wsi == 'Android':
            instance_extensions=[('wsi_enabled',
                                  ['vkDestroySurfaceKHR',
                                   'vkGetPhysicalDeviceSurfaceSupportKHR',
                                   'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                   'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                   'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                   'vkCreateAndroidSurfaceKHR'])]
        elif self.wsi == 'Xcb':
            instance_extensions=[('wsi_enabled',
                                  ['vkDestroySurfaceKHR',
                                   'vkGetPhysicalDeviceSurfaceSupportKHR',
                                   'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                   'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                   'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                   'vkCreateXcbSurfaceKHR',
                                   'vkGetPhysicalDeviceXcbPresentationSupportKHR'])]
        elif self.wsi == 'Xlib':
            instance_extensions=[('wsi_enabled',
                                  ['vkDestroySurfaceKHR',
                                   'vkGetPhysicalDeviceSurfaceSupportKHR',
                                   'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                   'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                   'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                   'vkCreateXlibSurfaceKHR',
                                   'vkGetPhysicalDeviceXlibPresentationSupportKHR'])]
        elif self.wsi == 'Wayland':
            instance_extensions=[('wsi_enabled',
                                  ['vkDestroySurfaceKHR',
                                   'vkGetPhysicalDeviceSurfaceSupportKHR',
                                   'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                   'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                   'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                   'vkCreateWaylandSurfaceKHR',
                                   'vkGetPhysicalDeviceWaylandPresentationSupportKHR'])]
        elif self.wsi == 'Mir':
            instance_extensions=[('wsi_enabled',
                                  ['vkDestroySurfaceKHR',
                                   'vkGetPhysicalDeviceSurfaceSupportKHR',
                                   'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                   'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                   'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                   'vkCreateMirSurfaceKHR',
                                   'vkGetPhysicalDeviceMirPresentationSupportKHR'])]
        else:
            message(FATAL_ERROR, "Invalid DisplayServer")

        extensions=[('wsi_enabled',
                     ['vkCreateSwapchainKHR',
                      'vkDestroySwapchainKHR', 'vkGetSwapchainImagesKHR',
                      'vkAcquireNextImageKHR', 'vkQueuePresentKHR'])]
        body = [self.generate_init(),
                self._generate_dispatch_entrypoints("VK_LAYER_EXPORT"),
                self._generate_layer_gpa_function(extensions, instance_extensions)]
        return "\n\n".join(body)

def main():

    wsi = {
            "Win32",
            "Android",
            "Xcb",
            "Xlib",
            "Wayland",
            "Mir",
    }

    subcommands = {
            "layer-funcs" : LayerFuncsSubcommand,
            "generic" : GenericLayerSubcommand,
            "api_dump" : ApiDumpSubcommand,
    }

    if len(sys.argv) < 4 or sys.argv[1] not in wsi or sys.argv[2] not in subcommands or not os.path.exists(sys.argv[3]):
        print("Usage: %s <wsi> <subcommand> <input_header> [options]" % sys.argv[0])
        print
        print("Available subcommands are: %s" % " ".join(subcommands))
        print("Available wsi (displayservers) are: %s" % " ".join(wsi))
        exit(1)

    hfp = vk_helper.HeaderFileParser(sys.argv[3])
    hfp.parse()
    vk_helper.enum_val_dict = hfp.get_enum_val_dict()
    vk_helper.enum_type_dict = hfp.get_enum_type_dict()
    vk_helper.struct_dict = hfp.get_struct_dict()
    vk_helper.typedef_fwd_dict = hfp.get_typedef_fwd_dict()
    vk_helper.typedef_rev_dict = hfp.get_typedef_rev_dict()
    vk_helper.types_dict = hfp.get_types_dict()

    subcmd = subcommands[sys.argv[2]](sys.argv[3:])
    subcmd.run()

if __name__ == "__main__":
    main()
