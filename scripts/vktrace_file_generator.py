#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2017 The Khronos Group Inc.
# Copyright (c) 2015-2017 Valve Corporation
# Copyright (c) 2015-2017 LunarG, Inc.
# Copyright (c) 2015-2017 Google Inc.
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
# Author: Mark Lobodzinski <mark@lunarg.com>

import os,re,sys
import xml.etree.ElementTree as etree
from generator import *
from collections import namedtuple
from common_codegen import *

approved_ext = [
                'VK_AMD_draw_indirect_count',
                'VK_AMD_gcn_shader',
                'VK_AMD_gpu_shader_half_float',
                'VK_AMD_gpu_shader_int16',
                'VK_AMD_negative_viewport_height',
                'VK_AMD_rasterization_order',
                'VK_AMD_shader_ballot',
                'VK_AMD_shader_explicit_vertex_parameter',
                'VK_AMD_shader_trinary_minmax',
                'VK_AMD_texture_gather_bias_lod',
                'VK_EXT_acquire_xlib_display',
                'VK_EXT_blend_operation_advanced',
                'VK_EXT_debug_marker',
                'VK_EXT_debug_report',
                'VK_EXT_direct_mode_display',
                'VK_EXT_discard_rectangles',
                'VK_EXT_display_control',
                'VK_EXT_display_surface_counter',
                'VK_EXT_hdr_metadata',
                'VK_EXT_shader_subgroup_ballot',
                'VK_EXT_shader_subgroup_vote',
                'VK_EXT_swapchain_colorspace',
                'VK_EXT_validation_flags',
                'VK_GOOGLE_display_timing',
                'VK_IMG_filter_cubic',
                'VK_IMG_format_pvrtc',
                'VK_KHR_16bit_storage',
                'VK_KHR_android_surface',
                'VK_KHR_bind_memory2',
                'VK_KHR_dedicated_allocation',
                'VK_KHR_descriptor_update_template',
                'VK_KHR_display',
                'VK_KHR_display_swapchain',
                'VK_KHR_external_fence',
                'VK_KHR_external_fence_capabilities',
                'VK_KHR_external_fence_fd',
                'VK_KHR_external_fence_win32',
                'VK_KHR_external_memory',
                'VK_KHR_external_memory_capabilities',
                'VK_KHR_external_memory_fd',
                'VK_KHR_external_memory_win32',
                'VK_KHR_external_semaphore',
                'VK_KHR_external_semaphore_capabilities',
                'VK_KHR_external_semaphore_fd',
                'VK_KHR_external_semaphore_win32',
                'VK_KHR_get_memory_requirements2',
                'VK_KHR_get_physical_device_properties2',
                'VK_KHR_get_surface_capabilities2',
                'VK_KHR_image_format_list',
                'VK_KHR_incremental_present',
                'VK_KHR_maintenance1',
                'VK_KHR_maintenance2',
                #'VK_KHR_mir_surface',
                'VK_KHR_push_descriptor',
                'VK_KHR_relaxed_block_layout',
                'VK_KHR_sampler_filter_minmax',
                'VK_KHR_sampler_mirror_clamp_to_edge',
                'VK_KHR_shader_draw_parameters',
                'VK_KHR_shared_presentable_image',
                'VK_KHR_storage_buffer_storage_class',
                'VK_KHR_surface',
                'VK_KHR_swapchain',
                'VK_KHR_variable_pointers',
                'VK_KHR_wayland_surface',
                'VK_KHR_win32_keyed_mutex',
                'VK_KHR_win32_surface',
                'VK_KHR_xcb_surface',
                'VK_KHR_xlib_surface',
                'VK_KHX_device_group',
                'VK_KHX_device_group_creation',
                'VK_KHX_multiview',
                'VK_EXT_depth_range_unrestricted',
                'VK_EXT_external_memory_dma_buf',
                'VK_EXT_external_memory_host',
                'VK_EXT_global_priority',
                'VK_EXT_post_depth_coverage',
                'VK_EXT_queue_family_foreign',
                'VK_EXT_sampler_filter_minmax',
                'VK_EXT_shader_stencil_export',
                'VK_EXT_shader_viewport_index_layer',
                'VK_EXT_validation_cache',
                'VK_AMD_mixed_attachment_samples',
                'VK_AMD_shader_fragment_mask',
                'VK_AMD_shader_image_load_store_lod',
                'VK_AMD_shader_info',
                #'VK_MVK_ios_surface',
                #'VK_MVK_macos_surface',
                #'VK_MVK_moltenvk',
                #'VK_NN_vi_surface',
                'VK_NV_clip_space_w_scaling',
                'VK_NV_dedicated_allocation',
                'VK_NV_external_memory',
                'VK_NV_external_memory_capabilities',
                'VK_NV_external_memory_win32',
                'VK_NV_fill_rectangle',
                'VK_NV_fragment_coverage_to_color',
                'VK_NV_framebuffer_mixed_samples',
                'VK_NV_geometry_shader_passthrough',
                'VK_NV_glsl_shader',
                'VK_NV_sample_mask_override_coverage',
                'VK_NV_viewport_array2',
                'VK_NV_viewport_swizzle',
                'VK_NV_win32_keyed_mutex',
                'VK_NVX_device_generated_commands',
                'VK_NVX_multiview_per_view_attributes',
                'VK_EXT_sample_locations',
                'VK_KHR_sampler_ycbcr_conversion',
                'VK_KHR_get_display_properties2',
                'VK_KHR_memory2',
                'VK_KHR_protected_memory',
                'VK_KHX_subgroup',
                ]

api_exclusions = [
                'EnumerateInstanceVersion'
                ]

# Helper functions

def isSupportedCmd(cmd, cmd_extension_dict):
    extension = cmd_extension_dict[cmd.name]
    if 'VK_VERSION_1_' not in extension and extension not in approved_ext:
        return False

    cmdname = cmd.name[2:]
    if cmdname in api_exclusions:
        return False
    return True

def isInstanceCmd(cmd):
    cmdtarget = cmd.members[0].type
    handle = cmd.members[0].handle
    return handle == None or cmdtarget == "VkInstance" or cmdtarget == 'VkPhysicalDevice'

#
# VkTraceFileOutputGeneratorOptions - subclass of GeneratorOptions.
class VkTraceFileOutputGeneratorOptions(GeneratorOptions):
    def __init__(self,
                 filename = None,
                 directory = '.',
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
                 alignFuncParam = 0,
                 expandEnumerants = True,
                 library_name = '',
                 vktrace_file_type = ''):
        GeneratorOptions.__init__(self, filename, directory, apiname, profile,
                                  versions, emitversions, defaultExtensions,
                                  addExtensions, removeExtensions, emitExtensions, sortProcedure)
        self.prefixText       = prefixText
        self.genFuncPointers  = genFuncPointers
        self.protectFile      = protectFile
        self.protectFeature   = protectFeature
        self.apicall          = apicall
        self.apientry         = apientry
        self.apientryp        = apientryp
        self.alignFuncParam   = alignFuncParam
        self.expandEnumerants = expandEnumerants,
        self.library_name     = library_name
        self.vktrace_file_type = vktrace_file_type
#
# VkTraceFileOutputGenerator - subclass of OutputGenerator. Outputs Vulkan VkTrace files
class VkTraceFileOutputGenerator(OutputGenerator):
    """Generate VkTrace file based on XML element attributes"""
    def __init__(self,
                 errFile = sys.stderr,
                 warnFile = sys.stderr,
                 diagFile = sys.stdout):
        OutputGenerator.__init__(self, errFile, warnFile, diagFile)
        # Internal state - accumulators for different inner block text
        self.structNames = []                             # List of Vulkan struct typenames
        self.structMembers = []                           # List of StructMemberData records for all Vulkan structs
        self.object_types = []                            # List of all handle types
        self.debug_report_object_types = []               # Handy copy of debug_report_object_type enum data
        self.CmdInfoData = namedtuple('CmdInfoData', ['name', 'cmdinfo'])
        self.cmd_feature_protect = []  # Save ifdef's for each command
        self.cmd_extension_data = namedtuple('cmd_extension_data', ['name', 'extension_name'])
        self.cmd_extension_names = []    # Save extension name for each command
        self.cmd_info_data = []        # Save the cmdinfo data for wrapping the handles when processing is complete
        self.cmdMembers = []
        self.current_feature_name = None
        # Named tuples to store struct and command data
        self.CmdMemberData = namedtuple('CmdMemberData', ['name', 'members'])
        self.CmdExtraProtect = namedtuple('CmdExtraProtect', ['name', 'extra_protect'])
        self.StructType = namedtuple('StructType', ['name', 'value'])
        self.CommandParam = namedtuple('CommandParam', ['type', 'name', 'ispointer', 'isstaticarray', 'isconst', 'iscount', 'len', 'cdecl', 'feature_protect', 'handle'])
        self.StructMemberData = namedtuple('StructMemberData', ['name', 'members', 'ifdef_protect'])
    #
    # Called once at the beginning of each run
    def beginFile(self, genOpts):
        OutputGenerator.beginFile(self, genOpts)
        # User-supplied prefix text, if any (list of strings)
        self.vktrace_file_type = genOpts.vktrace_file_type
        self.library_name = genOpts.library_name
        # File Comment
        file_comment = '// *** THIS FILE IS GENERATED - DO NOT EDIT ***\n'
        file_comment += '// See vktrace_file_generator.py for modifications\n'
        write(file_comment, file=self.outFile)
        # Copyright Notice
        copyright = ''
        copyright += '\n'
        copyright += '/***************************************************************************\n'
        copyright += ' *\n'
        copyright += ' * Copyright (c) 2015-2017 The Khronos Group Inc.\n'
        copyright += ' * Copyright (c) 2015-2017 Valve Corporation\n'
        copyright += ' * Copyright (c) 2015-2017 LunarG, Inc.\n'
        copyright += ' * Copyright (c) 2015-2017 Google Inc.\n'
        copyright += ' *\n'
        copyright += ' * Licensed under the Apache License, Version 2.0 (the "License");\n'
        copyright += ' * you may not use this file except in compliance with the License.\n'
        copyright += ' * You may obtain a copy of the License at\n'
        copyright += ' *\n'
        copyright += ' *     http://www.apache.org/licenses/LICENSE-2.0\n'
        copyright += ' *\n'
        copyright += ' * Unless required by applicable law or agreed to in writing, software\n'
        copyright += ' * distributed under the License is distributed on an "AS IS" BASIS,\n'
        copyright += ' * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n'
        copyright += ' * See the License for the specific language governing permissions and\n'
        copyright += ' * limitations under the License.\n'
        copyright += ' *\n'
        copyright += ' * Author: Mark Lobodzinski <mark@lunarg.com>\n'
        copyright += ' * Author: Jon Ashburn <jon@lunarg.com>\n'
        copyright += ' * Author: Tobin Ehlis <tobin@lunarg.com>\n'
        copyright += ' * Author: Peter Lohrmann <peterl@valvesoftware.com>\n'
        copyright += ' * Author: David Pinedo <david@lunarg.com>\n'
        copyright += ' *\n'
        copyright += ' ****************************************************************************/\n'
        write(copyright, file=self.outFile)
    #
    # Write generated file content to output file
    def endFile(self):
        dest_file = ''
        dest_file += self.OutputDestFile()
        # Remove blank lines at EOF
        if dest_file.endswith('\n'):
            dest_file = dest_file[:-1]
        write(dest_file, file=self.outFile);
        # Finish processing in superclass
        OutputGenerator.endFile(self)
    #
    # Called for each type -- if the type is a struct/union, grab the metadata
    def genType(self, typeinfo, name, alias):
        OutputGenerator.genType(self, typeinfo, name, alias)
        typeElem = typeinfo.elem
        # If the type is a struct type, traverse the imbedded <member> tags generating a structure.
        # Otherwise, emit the tag text.
        category = typeElem.get('category')
        if category == 'handle':
            self.object_types.append(name)
        elif (category == 'struct' or category == 'union'):
            self.structNames.append(name)
            self.genStruct(typeinfo, name, alias) # TODO: This is bad. Might confuse parent class
    #
    # Check if the parameter passed in is a pointer
    def paramIsPointer(self, param):
        ispointer = False
        for elem in param:
            if ((elem.tag is not 'type') and (elem.tail is not None)) and '*' in elem.tail:
                ispointer = True
        return ispointer
    #
    # Check if the parameter passed in is a static array
    def paramIsStaticArray(self, param):
        isstaticarray = 0
        paramname = param.find('name')
        if (paramname is not None) and (paramname.tail is not None) and ('[' in paramname.tail):
            isstaticarray = paramname.tail.count('[')
        return isstaticarray
    #
    # Retrieve the type and name for a parameter
    def getTypeNameTuple(self, param):
        type = ''
        name = ''
        for elem in param:
            if elem.tag == 'type':
                type = noneStr(elem.text)
            elif elem.tag == 'name':
                name = noneStr(elem.text)
        return (type, name)
    # Extract length values from latexmath.  Currently an inflexible solution that looks for specific
    # patterns that are found in vk.xml.  Will need to be updated when new patterns are introduced.
    def parseLateXMath(self, source):
        name = 'ERROR'
        decoratedName = 'ERROR'
        if 'mathit' in source:
            # Matches expressions similar to 'latexmath:[\lceil{\mathit{rasterizationSamples} \over 32}\rceil]'
            match = re.match(r'latexmath\s*\:\s*\[\s*\\l(\w+)\s*\{\s*\\mathit\s*\{\s*(\w+)\s*\}\s*\\over\s*(\d+)\s*\}\s*\\r(\w+)\s*\]', source)
            if not match or match.group(1) != match.group(4):
                raise 'Unrecognized latexmath expression'
            name = match.group(2)
            # Need to add 1 for ceiling function; otherwise, the allocated packet
            # size will be less than needed during capture for some title which use
            # this in VkPipelineMultisampleStateCreateInfo. based on ceiling function
            # definition,it is '{0}%{1}?{0}/{1} + 1:{0}/{1}'.format(*match.group(2, 3)),
            # its value <= '{}/{} + 1'.
            if match.group(1) == 'ceil':
                decoratedName = '{}/{} + 1'.format(*match.group(2, 3))
            else:
                decoratedName = '{}/{}'.format(*match.group(2, 3))
        else:
            # Matches expressions similar to 'latexmath : [dataSize \over 4]'
            match = re.match(r'latexmath\s*\:\s*\[\s*(\w+)\s*\\over\s*(\d+)\s*\]', source)
            name = match.group(1)
            decoratedName = '{}/{}'.format(*match.group(1, 2))
        return name, decoratedName
    #
    # Retrieve the value of the len tag
    def getLen(self, param):
        result = None
        len = param.attrib.get('len')
        if len and len != 'null-terminated':
            # For string arrays, 'len' can look like 'count,null-terminated', indicating that we
            # have a null terminated array of strings.  We strip the null-terminated from the
            # 'len' field and only return the parameter specifying the string count
            if 'null-terminated' in len:
                result = len.split(',')[0]
            else:
                result = len
            if 'latexmath' in len:
                param_type, param_name = self.getTypeNameTuple(param)
                len_name, result = self.parseLateXMath(len)
            # Spec has now notation for len attributes, using :: instead of platform specific pointer symbol
            result = str(result).replace('::', '->')
        return result
    #
    # Check if a structure is or contains a dispatchable (dispatchable = True) or
    # non-dispatchable (dispatchable = False) handle
    def TypeContainsObjectHandle(self, handle_type, dispatchable):
        if dispatchable:
            type_key = 'VK_DEFINE_HANDLE'
        else:
            type_key = 'VK_DEFINE_NON_DISPATCHABLE_HANDLE'
        handle = self.registry.tree.find("types/type/[name='" + handle_type + "'][@category='handle']")
        if handle is not None and handle.find('type').text == type_key:
            return True
        # if handle_type is a struct, search its members
        if handle_type in self.structNames:
            member_index = next((i for i, v in enumerate(self.structMembers) if v[0] == handle_type), None)
            if member_index is not None:
                for item in self.structMembers[member_index].members:
                    handle = self.registry.tree.find("types/type/[name='" + item.type + "'][@category='handle']")
                    if handle is not None and handle.find('type').text == type_key:
                        return True
        return False
    #
    # Capture command parameter info needed
    def genCmd(self, cmdinfo, cmdname, alias):
        if "GetPhysicalDeviceSurfacePresentModes" in cmdname:
           stop="here"
        # Add struct-member type information to command parameter information
        OutputGenerator.genCmd(self, cmdinfo, cmdname, alias)
        members = cmdinfo.elem.findall('.//param')
        # Iterate over members once to get length parameters for arrays
        lens = set()
        for member in members:
            len = self.getLen(member)
            if len:
                lens.add(len)
        # Generate member info
        membersInfo = []
        constains_extension_structs = False
        for member in members:
            # Get type and name of member
            info = self.getTypeNameTuple(member)
            type = info[0]
            name = info[1]
            cdecl = self.makeCParamDecl(member, 0)
            # Check for parameter name in lens set
            iscount = True if name in lens else False
            len = self.getLen(member)
            isconst = True if 'const' in cdecl else False
            ispointer = self.paramIsPointer(member)
            isstaticarray = self.paramIsStaticArray(member)
            handle = self.registry.tree.find("types/type/[name='" + type + "'][@category='handle']")
            membersInfo.append(self.CommandParam(type=type,
                                                 name=name,
                                                 ispointer=ispointer,
                                                 isstaticarray=isstaticarray,
                                                 isconst=isconst,
                                                 iscount=iscount,
                                                 len=len,
                                                 cdecl=cdecl,
                                                 feature_protect=self.featureExtraProtect,
                                                 handle=handle))
        self.cmdMembers.append(self.CmdMemberData(name=cmdname, members=membersInfo))
        self.cmd_info_data.append(self.CmdInfoData(name=cmdname, cmdinfo=cmdinfo))
        self.cmd_extension_names.append(self.cmd_extension_data(name=cmdname, extension_name=self.current_feature_name))
        self.cmd_feature_protect.append(self.CmdExtraProtect(name=cmdname, extra_protect=self.featureExtraProtect))
    #
    # Generate local ready-access data describing Vulkan structures and unions from the XML metadata
    def genStruct(self, typeinfo, typeName, alias):
        OutputGenerator.genStruct(self, typeinfo, typeName, alias)
        members = typeinfo.elem.findall('.//member')
        # Iterate over members once to get length parameters for arrays
        lens = set()
        for member in members:
            len = self.getLen(member)
            if len:
                lens.add(len)
        # Generate member info
        membersInfo = []
        for member in members:
            # Get the member's type and name
            info = self.getTypeNameTuple(member)
            type = info[0]
            name = info[1]
            cdecl = self.makeCParamDecl(member, 1)
            # Store pointer/array/string info
            isstaticarray = self.paramIsStaticArray(member)
            handle = self.registry.tree.find("types/type/[name='" + type + "'][@category='handle']")
            membersInfo.append(self.CommandParam(type=type,
                                                 name=name,
                                                 ispointer=self.paramIsPointer(member),
                                                 isstaticarray=isstaticarray,
                                                 isconst=True if 'const' in cdecl else False,
                                                 iscount=True if name in lens else False,
                                                 len=self.getLen(member),
                                                 cdecl=cdecl,
                                                 feature_protect=self.featureExtraProtect,
                                                 handle=handle))
        self.structMembers.append(self.StructMemberData(name=typeName, members=membersInfo, ifdef_protect=self.featureExtraProtect))
    #
    def beginFeature(self, interface, emit):
        # Start processing in superclass
        OutputGenerator.beginFeature(self, interface, emit)
        self.featureExtraProtect = GetFeatureProtect(interface)
        self.current_feature_name = self.featureName
    #
    # Enum_string_header: Create a routine to convert an enumerated value into a string
    def GenerateEnumStringConversion(self, groupName, value_list):
        outstring = '\n'
        outstring += 'static inline const char* string_%s(%s input_value)\n' % (groupName, groupName)
        outstring += '{\n'
        outstring += '    switch ((%s)input_value)\n' % groupName
        outstring += '    {\n'
        for item in value_list:
            outstring += '        case %s:\n' % item
            outstring += '            return "%s";\n' % item
        outstring += '        default:\n'
        outstring += '            return "Unhandled %s";\n' % groupName
        outstring += '    }\n'
        outstring += '}\n'
        return outstring
    #
    # Construct vkreplay objmapper header file as a string and return
    def GenerateReplayObjmapperHeader(self):
        replay_objmapper_header  = '\n'
        replay_objmapper_header += '#pragma once\n\n'
        replay_objmapper_header += '#include <set>\n'
        replay_objmapper_header += '#include <map>\n'
        replay_objmapper_header += '#include <list>\n'
        replay_objmapper_header += '#include <vector>\n'
        replay_objmapper_header += '#include <string>\n'
        replay_objmapper_header += '#include "vulkan/vulkan.h"\n'
        replay_objmapper_header += '#include "vktrace_pageguard_memorycopy.h"\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '#include "vkreplay_objmapper_class_defs.h"\n\n'

        # TODO: This is kinda kludgy -- why this outlier?
        additional_remap_fifo = {}
        additional_remap_fifo['pImageIndex'] = "uint32_t"

        # Output function to clear object maps
        replay_objmapper_header += '    void clear_all_map_handles() {\n'
        for item in self.object_types:
            mangled_name = 'm_' + item[2:].lower() + 's'
            replay_objmapper_header += '        %s.clear();\n' % mangled_name
        for item in additional_remap_fifo:
            replay_objmapper_header += '        m_%s.clear();\n' % item
        replay_objmapper_header += '    }\n'

        remapped_objects = ['VkImage', 'VkBuffer', 'VkDeviceMemory']
        for item in self.object_types:
            map_name = item[2:].lower() + 's'
            mangled_name = 'm_' + map_name
            if item in remapped_objects:
                obj_name = item[2:].lower() + 'Obj'
            else:
                obj_name = item
            replay_objmapper_header += '    std::map<%s, %s> %s;\n' % (item, obj_name, mangled_name)
            replay_objmapper_header += '    void add_to_%s_map(%s pTraceVal, %s pReplayVal) {\n' % (map_name, item, obj_name)
            replay_objmapper_header += '        %s[pTraceVal] = pReplayVal;\n' % mangled_name
            replay_objmapper_header += '    }\n\n'

            replay_objmapper_header += '    void rm_from_%s_map(const %s& key) {\n' % (map_name, item)
            replay_objmapper_header += '         %s.erase(key);\n' % mangled_name
            replay_objmapper_header += '    }\n\n'

            replay_objmapper_header += '    %s remap_%s(const %s& value) {\n' % (item, map_name, item)
            replay_objmapper_header += '        if (value == 0) { return 0; }\n'
            if item in remapped_objects:
                replay_objmapper_header += '        std::map<%s, %s>::const_iterator q = %s.find(value);\n' % (item, obj_name, mangled_name)
                if item == 'VkDeviceMemory':
                    replay_objmapper_header += '        if (q == %s.end()) { vktrace_LogError("Failed to remap %s."); return VK_NULL_HANDLE; }\n' % (mangled_name, item)
                else:
                    replay_objmapper_header += '        if (q == %s.end()) return VK_NULL_HANDLE;\n' % mangled_name
                replay_objmapper_header += '        return q->second.replay%s;\n' % item[2:]
            else:
                replay_objmapper_header += '        std::map<%s, %s>::const_iterator q = %s.find(value);\n' % (item, obj_name, mangled_name)
                replay_objmapper_header += '        if (q == %s.end()) { vktrace_LogError("Failed to remap %s."); return VK_NULL_HANDLE; }\n' % (mangled_name, item)
                replay_objmapper_header += '        return q->second;\n'
            replay_objmapper_header += '    }\n\n'
        for item in additional_remap_fifo:
            replay_objmapper_header += '    std::list<%s> m_%s;\n' % (additional_remap_fifo[item], item)
            replay_objmapper_header += '    void add_to_%s_map(%s traceVal, %s replayVal) {\n' % (item, additional_remap_fifo[item], additional_remap_fifo[item])
            replay_objmapper_header += '        m_%s.push_back(replayVal);\n' % item
            replay_objmapper_header += '    }\n\n'
            replay_objmapper_header += '    %s remap_%s(const %s& value) {\n' % (additional_remap_fifo[item], item, additional_remap_fifo[item])
            replay_objmapper_header += '        if (m_%s.size() == 0) {\n' % (item)
            replay_objmapper_header += '            vktrace_LogError("Failed to remap %s.");\n' % (item)
            replay_objmapper_header += '            return UINT32_MAX;\n'
            replay_objmapper_header += '        }\n'
            replay_objmapper_header += '        %s result = m_%s.front();\n' % (additional_remap_fifo[item], item)
            replay_objmapper_header += '        m_%s.pop_front();\n' % (item)
            replay_objmapper_header += '        return result;\n'
            replay_objmapper_header += '    }\n'
        replay_objmapper_header += '};'
        return replay_objmapper_header
    #
    # Construct vkreplay replay gen source file
    def GenerateReplayGenSource(self):
        cmd_member_dict = dict(self.cmdMembers)
        cmd_info_dict = dict(self.cmd_info_data)
        cmd_protect_dict = dict(self.cmd_feature_protect)
        cmd_extension_dict = dict(self.cmd_extension_names)

        wsi_platform_manual_funcs = ['CreateWin32SurfaceKHR',
                                     'CreateXcbSurfaceKHR',
                                     'CreateWaylandSurfaceKHR',
                                     'CreateXlibSurfaceKHR',
                                     'CreateAndroidSurfaceKHR']
        manually_replay_funcs = ['AllocateMemory',
                                 'BeginCommandBuffer',
                                 'CreateDescriptorSetLayout',
                                 'CreateDevice',
                                 'CreateBuffer',
                                 'CreateImage',
                                 'CreateCommandPool',
                                 'CreateFramebuffer',
                                 'GetPipelineCacheData',
                                 'CreateGraphicsPipelines',
                                 'CreateComputePipelines',
                                 'CreatePipelineLayout',
                                 'CreateRenderPass',
                                 'CmdBeginRenderPass',
                                 'CmdBindDescriptorSets',
                                 'CmdBindVertexBuffers',
                                 'CmdPipelineBarrier',
                                 'QueuePresentKHR',
                                 'CmdWaitEvents',
                                 'DestroyBuffer',
                                 'DestroyImage',
                                 'EnumeratePhysicalDevices',
                                 'FreeMemory',
                                 'FreeDescriptorSets',
                                 'FlushMappedMemoryRanges',
                                 'InvalidateMappedMemoryRanges',
                                 'GetPhysicalDeviceProperties',
                                 'GetPhysicalDeviceProperties2KHR',
                                 'GetPhysicalDeviceMemoryProperties',
                                 'GetPhysicalDeviceMemoryProperties2KHR',
                                 'GetPhysicalDeviceQueueFamilyProperties',
                                 'GetPhysicalDeviceQueueFamilyProperties2KHR',
                                 'GetPhysicalDeviceSparseImageFormatProperties',
                                 'GetPhysicalDeviceSparseImageFormatProperties2KHR',
                                 'GetPhysicalDeviceSurfaceSupportKHR',
                                 'GetPhysicalDeviceSurfaceCapabilitiesKHR',
                                 'GetPhysicalDeviceSurfaceFormatsKHR',
                                 'GetPhysicalDeviceSurfacePresentModesKHR',
                                 'CreateSwapchainKHR',
                                 'DestroySwapchainKHR',
                                 'GetSwapchainImagesKHR',
                                 'CreateXcbSurfaceKHR',
                                 'CreateWaylandSurfaceKHR',
                                 'CreateXlibSurfaceKHR',
                                 'GetPhysicalDeviceXcbPresentationSupportKHR',
                                 'GetPhysicalDeviceWaylandPresentationSupportKHR',
                                 'GetPhysicalDeviceXlibPresentationSupportKHR',
                                 'CreateWin32SurfaceKHR',
                                 'GetPhysicalDeviceWin32PresentationSupportKHR',
                                 'CreateAndroidSurfaceKHR',
                                 'MapMemory',
                                 'QueueSubmit',
                                 'QueueBindSparse',
                                 'UnmapMemory',
                                 'UpdateDescriptorSets',
                                 'WaitForFences',
                                 'CreateDebugReportCallbackEXT',
                                 'DestroyDebugReportCallbackEXT',
                                 'AllocateCommandBuffers',
                                 'GetImageMemoryRequirements',
                                 'GetImageMemoryRequirements2KHR',
                                 'GetBufferMemoryRequirements',
                                 'GetBufferMemoryRequirements2KHR',
                                 'CreateDescriptorUpdateTemplateKHR',
                                 'CreateDescriptorUpdateTemplate',
                                 'DestroyDescriptorUpdateTemplateKHR',
                                 'DestroyDescriptorUpdateTemplate',
                                 'UpdateDescriptorSetWithTemplateKHR',
                                 'UpdateDescriptorSetWithTemplate',
                                 'CmdPushDescriptorSetKHR',
                                 'CmdPushDescriptorSetWithTemplateKHR',
                                 'BindBufferMemory',
                                 'BindImageMemory',
                                 # VK_EXT_display_control
                                 'RegisterDeviceEventEXT',
                                 'RegisterDisplayEventEXT',
                                 # VK_NVX_device_generated_commands
                                 'CreateObjectTableNVX',
                                 'CmdProcessCommandsNVX',
                                 'CreateIndirectCommandsLayoutNVX',
                                 'BindBufferMemory2KHR',
                                 'BindImageMemory2KHR',
                                 'GetDisplayPlaneSupportedDisplaysKHR',
                                 'EnumerateDeviceExtensionProperties'
                                 ]
        # Map APIs to functions if body is fully custom
        custom_body_dict = {'CreateInstance': self.GenReplayCreateInstance,
                            'GetPhysicalDeviceXcbPresentationSupportKHR': self.GenReplayGetPhysicalDeviceXcbPresentationSupportKHR,
                            'GetPhysicalDeviceWaylandPresentationSupportKHR': self.GenReplayGetPhysicalDeviceWaylandPresentationSupportKHR,
                            'GetPhysicalDeviceXlibPresentationSupportKHR': self.GenReplayGetPhysicalDeviceXlibPresentationSupportKHR,
                            'GetPhysicalDeviceWin32PresentationSupportKHR': self.GenReplayGetPhysicalDeviceWin32PresentationSupportKHR }
        # Special cases for functions that use do-while loops
        do_while_dict = {'GetFenceStatus': 'replayResult != pPacket->result  && pPacket->result == VK_SUCCESS',
                         'GetEventStatus': '(pPacket->result == VK_EVENT_SET || pPacket->result == VK_EVENT_RESET) && replayResult != pPacket->result',
                         'GetQueryPoolResults': 'pPacket->result == VK_SUCCESS && replayResult != pPacket->result'}

        replay_gen_source  = '\n'
        replay_gen_source += '#include "vkreplay_vkreplay.h"\n'
        replay_gen_source += '#include "vkreplay.h"\n'
        replay_gen_source += '#include "vkreplay_main.h"\n'
        replay_gen_source += '#include <algorithm>\n'
        replay_gen_source += '#include <queue>\n'
        replay_gen_source += '\n'
        replay_gen_source += 'extern "C" {\n'
        replay_gen_source += '#include "vktrace_vk_vk_packets.h"\n'
        replay_gen_source += '#include "vktrace_vk_packet_id.h"\n\n'
        replay_gen_source += 'void vkReplay::init_funcs(void * handle) {\n'
        replay_gen_source += '    m_libHandle = handle;\n'

        for api in self.cmdMembers:
            cmdname = api.name[2:]
            if not isSupportedCmd(api, cmd_extension_dict):
                continue;
            temp_exclude = ['CreateInstance', 'EnumerateInstanceExtensionProperties', 'EnumerateInstanceLayerProperties', 'EnumerateInstanceVersion']
            if cmdname in temp_exclude: # TODO verify this needs to be here
                continue
            protect = cmd_protect_dict[api.name]
            if protect is not None:
                replay_gen_source += '#ifdef %s\n' % protect
            disp_table = ""
            if isInstanceCmd(api):
                disp_table = "m_vkFuncs"
            else:
                disp_table = "m_vkDeviceFuncs"
            if 'DebugReport' not in cmdname:
                replay_gen_source += '    %s.%s = (PFN_vk%s)(vktrace_platform_get_library_entrypoint(handle, "vk%s"));\n' % (disp_table, cmdname, cmdname, cmdname)
            else: # These func ptrs get assigned at GetProcAddr time
                replay_gen_source += '    %s.%s = (PFN_vk%s)NULL;\n' % (disp_table, cmdname, cmdname)
            if protect is not None:
                replay_gen_source += '#endif // %s\n' % protect
        replay_gen_source += '}\n\n'
        replay_gen_source += 'vktrace_replay::VKTRACE_REPLAY_RESULT vkReplay::replay(vktrace_trace_packet_header *packet) { \n'
        replay_gen_source += '    vktrace_replay::VKTRACE_REPLAY_RESULT returnValue = vktrace_replay::VKTRACE_REPLAY_SUCCESS;\n'
        replay_gen_source += '    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;\n'
        replay_gen_source += '    switch (packet->packet_id) {\n'
        replay_gen_source += '        case VKTRACE_TPI_VK_vkApiVersion:\n'
        replay_gen_source += '            // Ignore api version packets\n'
        replay_gen_source += '            break;\n'

        for api in self.cmdMembers:
            if not isSupportedCmd(api, cmd_extension_dict):
                continue

            vk_cmdname = api.name
            # Strip off 'vk' from command name
            cmdname = api.name[2:]

            cmdinfo = cmd_info_dict[vk_cmdname]
            protect = cmd_protect_dict[vk_cmdname]
            if protect is not None:
                replay_gen_source += '#ifdef %s\n' % protect
            # TODO : How to handle void* return of GetProcAddr?
            # TODO : Make sure vkDestroy object functions really do clean up the object maps
            # Handle return values, if any
            ret_value = True
            resulttype = cmdinfo.elem.find('proto/type')
            if resulttype != None and resulttype.text == 'void' or cmdname in custom_body_dict:
              ret_value = False
            create_func = True if True in [create_txt in cmdname for create_txt in ['Create', 'Allocate', 'Acquire', 'GetDeviceQueue']] else False
            create_view = True if True in [create_txt in cmdname for create_txt in ['CreateBufferView', 'CreateImageView']] else False
            params = cmd_member_dict[vk_cmdname]
            replay_gen_source += '        case VKTRACE_TPI_VK_vk%s: { \n' % cmdname
            replay_gen_source += '            packet_vk%s* pPacket = (packet_vk%s*)(packet->pBody);\n' % (cmdname, cmdname)

            if cmdname in manually_replay_funcs:
                if ret_value == True:
                    replay_gen_source += '            replayResult = manually_replay_vk%s(pPacket);\n' % cmdname
                else:
                    replay_gen_source += '            manually_replay_vk%s(pPacket);\n' % cmdname
            elif cmdname in custom_body_dict:
                replay_gen_source += custom_body_dict[cmdname]()
            else:
                if create_view:
                    replay_gen_source += '            %s createInfo;\n' % (params[1].type.strip('*').replace('const ', ''))
                    replay_gen_source += '            memcpy(&createInfo, pPacket->pCreateInfo, sizeof(%s))\n;' % (params[1].type.strip('*').replace('const ', ''))
                    if 'CreateComputePipeline' == cmdname:
                        replay_gen_source += '            createInfo.cs.shader = m_objMapper.remap_shaders(pPacket->pCreateInfo->cs.shader);\n'
                        replay_gen_source += '            if (createInfo.cs.shader == VK_NULL_HANDLE && pPacket->pCreateInfo->cs.shader != VK_NULL_HANDLE) {\n'
                        replay_gen_source += '                vktrace_LogError("Error detected in vkCreateComputePipelines() due to invalid remapped VkShader.");\n'
                        replay_gen_source += '                return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        replay_gen_source += '            }\n'
                    elif 'CreateBufferView' == cmdname:
                        replay_gen_source += '            createInfo.buffer = m_objMapper.remap_buffers(pPacket->pCreateInfo->buffer);\n'
                        replay_gen_source += '            if (createInfo.buffer == VK_NULL_HANDLE && pPacket->pCreateInfo->buffer != VK_NULL_HANDLE) {\n'
                        replay_gen_source += '                vktrace_LogError("Error detected in vkCreateBufferView() due to invalid remapped VkBuffer.");\n'
                        replay_gen_source += '                return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        replay_gen_source += '            }\n'
                    else:
                        replay_gen_source += '            createInfo.image = m_objMapper.remap_images(pPacket->pCreateInfo->image);\n'
                        replay_gen_source += '            if (createInfo.image == VK_NULL_HANDLE && pPacket->pCreateInfo->image != VK_NULL_HANDLE) {\n'
                        replay_gen_source += '                vktrace_LogError("Error detected in vkCreateImageView() due to invalid remapped VkImage.");\n'
                        replay_gen_source += '                return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        replay_gen_source += '            }\n'
                    replay_gen_source += '            %s local_%s;\n' % (params[-1].type.strip('*').replace('const ', ''), params[-1].name)
                elif create_func: # Declare local var to store created handle into
                    if 'AllocateDescriptorSets' == cmdname:
                        p_ty = params[-1].type.strip('*').replace('const ', '')
                        replay_gen_source += '            %s* local_%s = (%s*)malloc(pPacket->pAllocateInfo->descriptorSetCount * sizeof(%s));\n' % (p_ty, params[-1].name, p_ty, p_ty)
                        replay_gen_source += '            VkDescriptorSetLayout* local_pSetLayouts = (VkDescriptorSetLayout*)malloc(pPacket->pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSetLayout));\n'
                        replay_gen_source += '            VkDescriptorSetAllocateInfo local_AllocInfo, *local_pAllocateInfo = &local_AllocInfo;\n'
                        replay_gen_source += '            VkDescriptorPool local_descPool;\n'
                        replay_gen_source += '            local_descPool = m_objMapper.remap_descriptorpools(pPacket->pAllocateInfo->descriptorPool);\n'
                        replay_gen_source += '            if (local_descPool == VK_NULL_HANDLE) {\n'
                        replay_gen_source += '                vktrace_LogError("Error detected in vkAllocateDescriptorSets() due to invalid remapped VkDescriptorPool.");\n'
                        replay_gen_source += '                return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        replay_gen_source += '            }\n'
                        replay_gen_source += '            for (uint32_t i = 0; i < pPacket->pAllocateInfo->descriptorSetCount; i++) {\n'
                        replay_gen_source += '                local_pSetLayouts[i] = m_objMapper.remap_descriptorsetlayouts(pPacket->%s->pSetLayouts[i]);\n' % (params[-2].name)
                        replay_gen_source += '                if (local_pSetLayouts[i] == VK_NULL_HANDLE) { \n'
                        replay_gen_source += '                    vktrace_LogError("Error detected in vkAllocateDescriptorSets() due to invalid remapped VkDescriptorSetLayout.");\n'
                        replay_gen_source += '                    return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        replay_gen_source += '                }\n'
                        replay_gen_source += '            }\n'
                        replay_gen_source += '            memcpy(local_pAllocateInfo, pPacket->pAllocateInfo, sizeof(VkDescriptorSetAllocateInfo));\n'
                        replay_gen_source += '            local_pAllocateInfo->pSetLayouts = local_pSetLayouts;\n'
                        replay_gen_source += '            local_pAllocateInfo->descriptorPool = local_descPool;\n'
                    else:
                        replay_gen_source += '            %s local_%s;\n' % (params[-1].type.strip('*').replace('const ', ''), params[-1].name)
                elif cmdname == 'ResetFences':
                    replay_gen_source += '            VkFence* fences = VKTRACE_NEW_ARRAY(VkFence, pPacket->fenceCount);\n'
                    replay_gen_source += '            for (uint32_t i = 0; i < pPacket->fenceCount; i++) {\n'
                    replay_gen_source += '                fences[i] = m_objMapper.remap_fences(pPacket->%s[i]);\n' % (params[-1].name)
                    replay_gen_source += '                if (fences[i] == VK_NULL_HANDLE) {\n'
                    replay_gen_source += '                    vktrace_LogError("Error detected in vkResetFences() due to invalid remapped VkFence.");\n'
                    replay_gen_source += '                    return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                    replay_gen_source += '                }\n'
                    replay_gen_source += '            }\n'
                elif cmdname in do_while_dict:
                    replay_gen_source += '            do {\n'
                last_name = ''
                for p in params:
                    if p.name is not '':
                        if create_func or create_view:
                            if p.name != params[-1].name:
                                replay_gen_source += self.RemapPacketParam(cmdname, p, last_name)
                        else:
                            replay_gen_source += self.RemapPacketParam(cmdname, p, last_name)
                        last_name = p.name

                if cmdname == 'DestroyInstance':
                    replay_gen_source += '            if (m_vkFuncs.DestroyDebugReportCallbackEXT != NULL) {\n'
                    replay_gen_source += '                m_vkFuncs.DestroyDebugReportCallbackEXT(remappedinstance, m_dbgMsgCallbackObj, pPacket->pAllocator);\n'
                    replay_gen_source += '            }\n'
                # TODO: need a better way to indicate which extensions should be mapped to which Get*ProcAddr
                elif cmdname == 'GetInstanceProcAddr':
                    for command in self.cmdMembers:
                        if not isInstanceCmd(command):
                            continue
                        extension = cmd_extension_dict[command.name]
                        if 'VK_VERSION_' not in extension and extension not in approved_ext:
                            continue
                        command_name_novk = command.name[2:]
                        if command_name_novk in api_exclusions:
                            continue
                        if 'VK_VERSION_' not in cmd_extension_dict[command.name] and command.name not in api_exclusions:
                            gipa_params = cmd_member_dict[vk_cmdname]
                            gipa_protect = cmd_protect_dict[command.name]
                            if gipa_protect is not None:
                                replay_gen_source += '#ifdef %s\n' % gipa_protect
                            replay_gen_source += '            if (strcmp(pPacket->pName, "%s") == 0) {\n' % (command.name)
                            replay_gen_source += '               m_vkFuncs.%s = (PFN_%s)vk%s(remappedinstance, pPacket->pName);\n' % (command_name_novk, command.name, cmdname)
                            replay_gen_source += '            }\n'
                            if gipa_protect is not None:
                                replay_gen_source += '#endif // %s\n' % gipa_protect
                elif cmdname == 'GetDeviceProcAddr':
                    for command in self.cmdMembers:
                        if isInstanceCmd(command):
                            continue
                        extension = cmd_extension_dict[command.name]
                        if 'VK_VERSION_' not in extension and extension not in approved_ext:
                            continue
                        command_name_novk = command.name[2:]
                        if command_name_novk in api_exclusions:
                            continue
                        if 'VK_VERSION_' not in cmd_extension_dict[command.name] and command.name not in api_exclusions:
                            gdpa_params = cmd_member_dict[vk_cmdname]
                            gdpa_protect = cmd_protect_dict[command.name]
                            if gdpa_protect is not None:
                                replay_gen_source += '#ifdef %s\n' % gdpa_protect
                            replay_gen_source += '            if (strcmp(pPacket->pName, "%s") == 0) {\n' % (command.name)
                            replay_gen_source += '               m_vkDeviceFuncs.%s = (PFN_%s)vk%s(remappeddevice, pPacket->pName);\n' % (command_name_novk, command.name, cmdname)
                            replay_gen_source += '            }\n'
                            if gdpa_protect is not None:
                                replay_gen_source += '#endif // %s\n' % gdpa_protect
                elif cmdname == 'GetPhysicalDeviceMemoryProperties':
                    replay_gen_source += '            VkPhysicalDeviceMemoryProperties memProperties = *(pPacket->pMemoryProperties);\n'
                elif cmdname == 'GetImageMemoryRequirements':
                    replay_gen_source += '            VkMemoryRequirements memReqs = *(pPacket->pMemoryRequirements);\n'
                elif 'SparseMemoryRequirements2' in cmdname:
                    replay_gen_source += '            vktrace_interpret_pnext_pointers(pPacket->header, (void *)pPacket->pInfo);\n'
                # Build the call to the "real_" entrypoint
                rr_string = '            '
                if ret_value:
                    if cmdinfo.elem.find('proto/type').text != 'VkResult':
                        ret_value = False
                    else:
                        rr_string = '            replayResult = '
                if cmdname == "EnumerateInstanceExtensionProperties" or cmdname == "EnumerateInstanceLayerProperties" or cmdname == "EnumerateInstanceVersion":
                    rr_string += 'vk%s(' % cmdname # TODO figure out if we need this case
                elif isInstanceCmd(api):
                    rr_string += 'm_vkFuncs.%s(' % cmdname
                else:
                    rr_string += 'm_vkDeviceFuncs.%s(' % cmdname
                for p in params:
                    if p.name is not '':
                        # For last param of Create funcs, pass address of param
                        if create_func:
                            if cmdname == 'AllocateDescriptorSets' and ((p.name == params[-2].name) or (p.name == params[-1].name)):
                                rr_string += 'local_%s, ' % p.name
                            elif p.name == params[-1].name:
                                rr_string += '&local_%s, ' % p.name
                            else:
                                rr_string += '%s, ' % self.GetPacketParam(cmdname, p.type, p.name)
                        else:
                            rr_string += '%s, ' % self.GetPacketParam(cmdname, p.type, p.name)
                rr_string = '%s);' % rr_string[:-2]
                if create_view:
                    rr_list = rr_string.split(', ')
                    rr_list[-3] = '&createInfo'
                    rr_list[-2] = 'NULL'
                    rr_list[-1] = '&local_%s);\n' % params[-1].name
                    rr_string = ', '.join(rr_list)
                    # This is a sneaky shortcut to use generic create code below to add_to_map
                    create_func = True
                elif cmdname == 'AllocateDescriptorSets':
                    rr_string = rr_string.replace('pPacket->pSetLayouts', 'pLocalDescSetLayouts')
                elif cmdname == 'ResetFences':
                   rr_string = rr_string.replace('pPacket->pFences', 'fences')
                # Insert the real_*(..) call
                replay_gen_source += '%s\n' % rr_string
                # Handle return values or anything that needs to happen after the real_*(..) call
                if 'DestroyDevice' in cmdname:
                    replay_gen_source += '            m_pCBDump = NULL;\n'
                    replay_gen_source += '            m_pDSDump = NULL;\n'
                    #TODO138 : disabling snapshot
                    #replay_gen_source += '            m_pVktraceSnapshotPrint = NULL;\n'
                    replay_gen_source += '            m_objMapper.rm_from_devices_map(pPacket->device);\n'
                    replay_gen_source += '            m_display->m_initedVK = false;\n'
                elif 'DestroySwapchainKHR' in cmdname:
                    replay_gen_source += '            m_objMapper.rm_from_swapchainkhrs_map(pPacket->swapchain);\n'
                elif 'AcquireNextImage' in cmdname:
                    replay_gen_source += '            if (replayResult == VK_SUCCESS) {\n'
                    replay_gen_source += '                m_objMapper.add_to_pImageIndex_map(*(pPacket->pImageIndex), local_pImageIndex);\n'
                    replay_gen_source += '            }\n'
                elif 'DestroyInstance' in cmdname:
                    replay_gen_source += '            // TODO need to handle multiple instances and only clearing maps within an instance.\n'
                    replay_gen_source += '            // TODO this only works with a single instance used at any given time.\n'
                    replay_gen_source += '            m_objMapper.clear_all_map_handles();\n'
                elif 'MergePipelineCaches' in cmdname:
                    replay_gen_source += '            delete[] remappedpSrcCaches;\n'
                elif 'FreeCommandBuffers' in cmdname:
                    replay_gen_source += '            delete[] remappedpCommandBuffers;\n'
                elif 'CmdExecuteCommands' in cmdname:
                    replay_gen_source += '            delete[] remappedpCommandBuffers;\n'
                elif 'AllocateDescriptorSets' in cmdname:
                    replay_gen_source += '            if (replayResult == VK_SUCCESS) {\n'
                    replay_gen_source += '                for (uint32_t i = 0; i < pPacket->pAllocateInfo->descriptorSetCount; i++) {\n'
                    replay_gen_source += '                    m_objMapper.add_to_descriptorsets_map(pPacket->%s[i], local_%s[i]);\n' % (params[-1].name, params[-1].name)
                    replay_gen_source += '                }\n'
                    replay_gen_source += '            }\n'
                    replay_gen_source += '            free(local_pSetLayouts);\n'
                    replay_gen_source += '            free(local_pDescriptorSets);\n'
                elif cmdname == 'GetImageMemoryRequirements':
                    replay_gen_source += '            if (memReqs.size != pPacket->pMemoryRequirements->size) {\n'
                    replay_gen_source += '                vktrace_LogError("Image memory size requirements differ: trace image %p needed %u bytes; replay image %p needed %u bytes.", pPacket->image, memReqs.size, remappedimage, pPacket->pMemoryRequirements->size);\n'
                    replay_gen_source += '            }\n'
                elif cmdname == 'GetPhysicalDeviceMemoryProperties':
                    replay_gen_source += '            if (memcmp(&memProperties, pPacket->pMemoryProperties, sizeof(VkPhysicalDeviceMemoryProperties)) != 0) {\n'
                    replay_gen_source += '                vktrace_LogError("Physical Device Memory properties differ. Memory heaps may not match as expected.");\n'
                    replay_gen_source += '            }\n'
                elif cmdname == 'ResetFences':
                    replay_gen_source += '            VKTRACE_DELETE(fences);\n'
                elif create_func: # Save handle mapping if create successful
                    if ret_value:
                        replay_gen_source += '            if (replayResult == VK_SUCCESS) {\n'
                    clean_type = params[-1].type.strip('*').replace('const ', '')
                    replay_gen_source += '                m_objMapper.add_to_%ss_map(*(pPacket->%s), local_%s);\n' % (clean_type.lower()[2:], params[-1].name, params[-1].name)
                    if 'AllocateMemory' == cmdname:
                        replay_gen_source += '                m_objMapper.add_entry_to_mapData(local_%s, pPacket->pAllocateInfo->allocationSize);\n' % (params[-1].name)
                    if ret_value:
                        replay_gen_source += '            }\n'
                elif cmdname in do_while_dict:
                    replay_gen_source += '            } while (%s);\n' % do_while_dict[cmdname]
                    replay_gen_source += '            if (pPacket->result != VK_NOT_READY || replayResult != VK_SUCCESS)\n'
            if ret_value:
                replay_gen_source += '            CHECK_RETURN_VALUE(vk%s);\n' % cmdname
            replay_gen_source += '            break;\n'
            replay_gen_source += '        }\n'
            if protect is not None:
                replay_gen_source += '#endif // %s\n' % protect
        replay_gen_source += '        default:\n'
        replay_gen_source += '            vktrace_LogWarning("Unrecognized packet_id %u, skipping.", packet->packet_id);\n'
        replay_gen_source += '            returnValue = vktrace_replay::VKTRACE_REPLAY_INVALID_ID;\n'
        replay_gen_source += '            break;\n'
        replay_gen_source += '    }\n'
        replay_gen_source += '    return returnValue;\n'
        replay_gen_source += '}\n'
        replay_gen_source += '}\n'
        return replay_gen_source
    #
    # Parameter remapping utility function
    def RemapPacketParam(self, funcName, param, lastName):
        param_exclude_list = ['pDescriptorSets', 'pFences']
        cleanParamType = param.type.strip('*').replace('const ', '')
        for obj in self.object_types:
            if obj == cleanParamType and param.name not in param_exclude_list:
                objectTypeRemapParam = ''
                if 'VkDynamicStateObject' == cleanParamType:
                    objectTypeRemapParam = ', pPacket->stateBindPoint'
                elif 'object' == param.name:
                    objectTypeRemapParam = ', pPacket->objType'
                elif 'srcObject' == param.name and 'Callback' in funcName:
                    objectTypeRemapParam = ', pPacket->objType'
                pArray = ''
                if param.ispointer:
                    if param.isconst == False:
                        result = '        %s remapped%s = m_objMapper.remap_%ss(*pPacket->%s%s);\n' % (cleanParamType, param.name, param.name.lower(), param.name, objectTypeRemapParam)
                        result += '        if (pPacket->%s != VK_NULL_HANDLE && remapped%s == VK_NULL_HANDLE) {\n' % (param.name, param.name)
                        result += '            vktrace_LogError("Error detected in %s() due to invalid remapped %s.");\n' % (funcName, cleanParamType)
                        result += '            return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        result += '        }\n'
                        return result
                    else:
                        if lastName == '':
                            return '            // pPacket->%s should have been remapped with special case code' % (param.name)
                        pArray = '[pPacket->%s]' % lastName
                        result = '            %s *remapped%s = new %s%s;\n' % (cleanParamType, param.name, cleanParamType, pArray)
                        result += '            for (uint32_t i = 0; i < pPacket->%s; i++) {\n' % lastName
                        result += '                remapped%s[i] = m_objMapper.remap_%ss(pPacket->%s[i]%s);\n' % (param.name, cleanParamType.lower()[2:], param.name, objectTypeRemapParam)
                        result += '                if (pPacket->%s[i] != VK_NULL_HANDLE && remapped%s[i] == VK_NULL_HANDLE) {\n' % (param.name, param.name)
                        result += '                    vktrace_LogError("Error detected in %s() due to invalid remapped %s.");\n' % (funcName, cleanParamType)
                        result += '                    return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                        result += '                }\n'
                        result += '            }\n'
                        return result
                result = '            %s remapped%s = m_objMapper.remap_%ss(pPacket->%s%s);\n' % (param.type, param.name, cleanParamType.lower()[2:], param.name, objectTypeRemapParam)
                result += '            if (pPacket->%s != VK_NULL_HANDLE && remapped%s == VK_NULL_HANDLE) {\n' % (param.name, param.name)
                result += '                vktrace_LogError("Error detected in %s() due to invalid remapped %s.");\n' % (funcName, cleanParamType)
                result += '                return vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
                result += '            }\n'
                return result
        return '            // No need to remap %s\n' % (param.name)
    #
    # Return correct remapping prefix
    def GetPacketParam(self, funcName, paramType, paramName):
        # List of types that require remapping
        param_exclude_list = ['pDescriptorSets', 'pFences']
        cleanParamType = paramType.strip('*').replace('const ', '')
        for obj in self.object_types:
            if obj == cleanParamType and paramName not in param_exclude_list:
                return 'remapped%s' % (paramName)
        return 'pPacket->%s' % (paramName)
    #
    # Dump source for CreateInstance
    def GenReplayCreateInstance(self):
        cb_body = []
        cb_body.append('            replayResult = manually_replay_vkCreateInstance(pPacket);')
        cb_body.append('            CHECK_RETURN_VALUE(vkCreateInstance);')
        cb_body.append('            if (replayResult == VK_SUCCESS) {')
        cb_body.append('                VkInstance remappedInstance = m_objMapper.remap_instances(*pPacket->pInstance);')
        cb_body.append('                if (remappedInstance == VK_NULL_HANDLE) {')
        cb_body.append('                    vktrace_LogError("Error detected in vkCreateInstance() due to invalid remapped VkInstance.");')
        cb_body.append('                    returnValue = vktrace_replay::VKTRACE_REPLAY_ERROR;')
        cb_body.append('                    break;')
        cb_body.append('                }')
        cb_body.append('                VkFlags reportFlags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;')
        cb_body.append('                PFN_vkCreateDebugReportCallbackEXT callback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(remappedInstance, "vkCreateDebugReportCallbackEXT");')
        cb_body.append('                if (callback != NULL) {')
        cb_body.append('                    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;')
        cb_body.append('                    memset(&dbgCreateInfo, 0, sizeof(dbgCreateInfo));')
        cb_body.append('                    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;')
        cb_body.append('                    dbgCreateInfo.flags = reportFlags;')
        cb_body.append('                    dbgCreateInfo.pfnCallback = g_fpDbgMsgCallback;')
        cb_body.append('                    dbgCreateInfo.pUserData = NULL;')
        cb_body.append('                    if (callback(remappedInstance, &dbgCreateInfo, NULL, &m_dbgMsgCallbackObj) != VK_SUCCESS) {')
        cb_body.append('                        vktrace_LogWarning("Failed to register vulkan callback for replayer error handling.");')
        cb_body.append('                        returnValue = vktrace_replay::VKTRACE_REPLAY_ERROR;')
        cb_body.append('                        break;')
        cb_body.append('                    }')
        cb_body.append('                }')
        cb_body.append('            }')
        return "\n".join(cb_body)
    #
    # These are customized because they are the only entry points returning VkBool32
    def GenReplayGetPhysicalDeviceXcbPresentationSupportKHR (self):
        cb_body = []
        cb_body.append('            VkBool32 rval = manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR(pPacket);')
        cb_body.append('            if (rval != pPacket->result) {')
        cb_body.append('                vktrace_LogError("Return value %d from API call (vkGetPhysicalDeviceXcbPresentationSupportKHR) does not match return value from trace file %d.",')
        cb_body.append('                                 rval, pPacket->result);')
        cb_body.append('                returnValue = vktrace_replay::VKTRACE_REPLAY_BAD_RETURN;')
        cb_body.append('            }')
        return "\n".join(cb_body)

    def GenReplayGetPhysicalDeviceWaylandPresentationSupportKHR (self):
        cb_body = []
        cb_body.append('            VkBool32 rval = manually_replay_vkGetPhysicalDeviceWaylandPresentationSupportKHR(pPacket);')
        cb_body.append('            if (rval != pPacket->result) {')
        cb_body.append('                vktrace_LogError("Return value %d from API call (vkGetPhysicalDeviceWaylandPresentationSupportKHR) does not match return value from trace file %d.",')
        cb_body.append('                                 rval, pPacket->result);')
        cb_body.append('                returnValue = vktrace_replay::VKTRACE_REPLAY_BAD_RETURN;')
        cb_body.append('            }')
        return "\n".join(cb_body)

    def GenReplayGetPhysicalDeviceXlibPresentationSupportKHR (self):
        cb_body = []
        cb_body.append('            VkBool32 rval = manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR(pPacket);')
        cb_body.append('            if (rval != pPacket->result) {')
        cb_body.append('                vktrace_LogError("Return value %d from API call (vkGetPhysicalDeviceXlibPresentationSupportKHR) does not match return value from trace file %d.",')
        cb_body.append('                                 rval, pPacket->result);')
        cb_body.append('                returnValue = vktrace_replay::VKTRACE_REPLAY_BAD_RETURN;')
        cb_body.append('            }')
        return "\n".join(cb_body)

    def GenReplayGetPhysicalDeviceWin32PresentationSupportKHR (self):
        cb_body = []
        cb_body.append('            VkBool32 rval = manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR(pPacket);')
        cb_body.append('            if (rval != pPacket->result) {')
        cb_body.append('                vktrace_LogError("Return value %d from API call (vkGetPhysicalDeviceWin32PresentationSupportKHR) does not match return value from trace file %d.",')
        cb_body.append('                                 rval, pPacket->result);')
        cb_body.append('                returnValue = vktrace_replay::VKTRACE_REPLAY_BAD_RETURN;')
        cb_body.append('            }')
        return "\n".join(cb_body)
    #
    # Construct vktrace packet id header file
    def GenerateTracePacketIdHeader(self):
        trace_pkt_id_hdr  = '#pragma once\n'
        trace_pkt_id_hdr += '#include "vktrace_vk_vk_packets.h"\n'
        trace_pkt_id_hdr += '#include "vktrace_trace_packet_utils.h"\n'
        trace_pkt_id_hdr += '#include "vktrace_trace_packet_identifiers.h"\n'
        trace_pkt_id_hdr += '#include "vktrace_interconnect.h"\n'
        trace_pkt_id_hdr += '#include <inttypes.h>\n'
        trace_pkt_id_hdr += '#include "vk_enum_string_helper.h"\n'
        trace_pkt_id_hdr += '#ifndef _WIN32\n'
        trace_pkt_id_hdr += '#pragma GCC diagnostic ignored "-Wwrite-strings"\n'
        trace_pkt_id_hdr += '#endif\n'
        trace_pkt_id_hdr += '#ifndef _WIN32\n'
        trace_pkt_id_hdr += '#pragma GCC diagnostic warning "-Wwrite-strings"\n'
        trace_pkt_id_hdr += '#endif\n'
        trace_pkt_id_hdr += '#if defined(WIN32)\n'
        trace_pkt_id_hdr += '#define snprintf _snprintf\n'
        trace_pkt_id_hdr += '#endif\n'
        trace_pkt_id_hdr += '#if defined(WIN32)\n'
        trace_pkt_id_hdr += '#define VK_SIZE_T_SPECIFIER "%Iu"\n'
        trace_pkt_id_hdr += '#else\n'
        trace_pkt_id_hdr += '#define VK_SIZE_T_SPECIFIER "%zu"\n'
        trace_pkt_id_hdr += '#endif\n'
        trace_pkt_id_hdr += '#define SEND_ENTRYPOINT_ID(entrypoint) ;\n'
        trace_pkt_id_hdr += '#define SEND_ENTRYPOINT_PARAMS(entrypoint, ...) ;\n'
        trace_pkt_id_hdr += '#define CREATE_TRACE_PACKET(entrypoint, buffer_bytes_needed) \\\n'
        trace_pkt_id_hdr += '    pHeader = vktrace_create_trace_packet(VKTRACE_TID_VULKAN, VKTRACE_TPI_VK_##entrypoint, sizeof(packet_##entrypoint), buffer_bytes_needed);\n\n'
        trace_pkt_id_hdr += '#define FINISH_TRACE_PACKET() \\\n'
        trace_pkt_id_hdr += '    vktrace_finalize_trace_packet(pHeader); \\\n'
        trace_pkt_id_hdr += '    vktrace_write_trace_packet(pHeader, vktrace_trace_get_trace_file()); \\\n'
        trace_pkt_id_hdr += '    vktrace_delete_trace_packet(&pHeader);\n'
        trace_pkt_id_hdr += '\n'
        trace_pkt_id_hdr += '// Include trace packet identifier definitions\n'
        trace_pkt_id_hdr += '#include "vktrace_trace_packet_identifiers.h"\n\n'
        #
        # Construct packet id name helper function
        trace_pkt_id_hdr += 'static const char *vktrace_vk_packet_id_name(const VKTRACE_TRACE_PACKET_ID_VK id) {\n'
        trace_pkt_id_hdr += '    switch(id) {\n'
        trace_pkt_id_hdr += '        case VKTRACE_TPI_VK_vkApiVersion: {\n'
        trace_pkt_id_hdr += '            return "vkApiVersion";\n'
        trace_pkt_id_hdr += '        }\n'
        cmd_extension_dict = dict(self.cmd_extension_names)
        for api in self.cmdMembers:
            if not isSupportedCmd(api, cmd_extension_dict):
                continue
            trace_pkt_id_hdr += '        case VKTRACE_TPI_VK_%s: {\n' % api.name
            trace_pkt_id_hdr += '            return "%s";\n' % api.name
            trace_pkt_id_hdr += '        };\n'
        trace_pkt_id_hdr += '        default:\n'
        trace_pkt_id_hdr += '            return NULL;\n'
        trace_pkt_id_hdr += '    }\n'
        trace_pkt_id_hdr += '}\n'
        trace_pkt_id_hdr += '\n'
        #
        # Construct packet id stringify helper function
        trace_pkt_id_hdr += 'static const char *vktrace_stringify_vk_packet_id(const VKTRACE_TRACE_PACKET_ID_VK id, const vktrace_trace_packet_header* pHeader) {\n'
        trace_pkt_id_hdr += '    static char str[1024];\n'
        trace_pkt_id_hdr += '    switch(id) {\n'
        trace_pkt_id_hdr += '        case VKTRACE_TPI_VK_vkApiVersion: {\n'
        trace_pkt_id_hdr += '            packet_vkApiVersion* pPacket = (packet_vkApiVersion*)(pHeader->pBody);\n'
        trace_pkt_id_hdr += '            snprintf(str, 1024, "vkApiVersion = 0x%x", pPacket->version);\n'
        trace_pkt_id_hdr += '            return str;\n'
        trace_pkt_id_hdr += '        }\n'
        cmd_member_dict = dict(self.cmdMembers)
        for api in self.cmdMembers:
            extension = cmd_extension_dict[api.name]
            if 'VK_VERSION_' not in extension and extension not in approved_ext:
                continue
            if api.name[2:] in api_exclusions:
                continue
            trace_pkt_id_hdr += '    case VKTRACE_TPI_VK_%s: {\n' % api.name
            func_str = '%s(' % api.name
            print_vals = ''
            create_func = True if True in [create_txt in api.name for create_txt in ['Create', 'Allocate', 'MapMemory', 'GetSwapchainImages']] else False
            params = cmd_member_dict[api.name]
            for p in params:
                if p.name == '':
                    continue
                last_param = False
                if (p.name == params[-1].name):
                    last_param = True
                if last_param and create_func: # Last param of create func
                    (pft, pfi, ptr) = self.GetPrintfParams(p, True)
                else:
                    (pft, pfi, ptr) = self.GetPrintfParams(p, False)
                if last_param == True:
                    func_str += '%s%s = %s)' % (ptr, p.name, pft)
                    print_vals += ', %s' % (pfi)
                else:
                    func_str += '%s%s = %s, ' % (ptr, p.name, pft)
                    print_vals += ', %s' % (pfi)
            trace_pkt_id_hdr += '            packet_%s* pPacket = (packet_%s*)(pHeader->pBody);\n' % (api.name, api.name)
            trace_pkt_id_hdr += '            snprintf(str, 1024, "%s"%s);\n' % (func_str, print_vals)
            trace_pkt_id_hdr += '            return str;\n'
            trace_pkt_id_hdr += '        }\n'
        trace_pkt_id_hdr += '        default:\n'
        trace_pkt_id_hdr += '            return NULL;\n'
        trace_pkt_id_hdr += '    }\n'
        trace_pkt_id_hdr += '}\n'
        trace_pkt_id_hdr += '\n'
        # Include interpret_trace_packet_vk function
        trace_pkt_id_hdr += self.GenerateInterpFunc()
        return trace_pkt_id_hdr
    #
    # Creates the shared interpret_trace_packet function
    def GenerateInterpFunc(self):
        interp_func_body = ''
        interp_func_body += 'static vktrace_trace_packet_header* interpret_trace_packet_vk(vktrace_trace_packet_header* pHeader) { \n'
        interp_func_body += '    if (pHeader == NULL) { \n'
        interp_func_body += '        return NULL;\n'
        interp_func_body += '    }\n'
        interp_func_body += '    switch (pHeader->packet_id) { \n'
        interp_func_body += '        case VKTRACE_TPI_VK_vkApiVersion: {\n'
        interp_func_body += '            return interpret_body_as_vkApiVersion(pHeader)->header;\n'
        interp_func_body += '        }\n'
        cmd_extension_dict = dict(self.cmd_extension_names)
        for api in self.cmdMembers:
            extension = cmd_extension_dict[api.name]
            if 'VK_VERSION_' not in extension and extension not in approved_ext:
                continue
            if api.name[2:] in api_exclusions:
                continue
            interp_func_body += '        case VKTRACE_TPI_VK_%s: {\n' % api.name
            interp_func_body += '            return interpret_body_as_%s(pHeader)->header;\n        }\n' % api.name
        interp_func_body += '        default:\n'
        interp_func_body += '            return NULL;\n'
        interp_func_body += '    }\n'
        interp_func_body += '    return NULL;\n'
        interp_func_body += '}\n'
        return interp_func_body
        #
        # Return set of printf '%' qualifier, input to that qualifier, and any dereference
    def GetPrintfParams(self, param, output_param):
        vk_type = param.type
        name = 'pPacket->%s' % param.name
        deref = ""
        # TODO : Need ENUM and STRUCT checks here
        if "VkImageLayout" in vk_type or "VkImageAspectMask" in vk_type:
            return ("%s", "string_%s(%s)" % (vk_type.replace('const ', '').strip('*'), name), deref)
        if "VkMappedMemoryRange" in vk_type:
            return ("%p [0]={memory=%\" PRIx64 \", offset=%\" PRIu64 \", size=%\" PRIu64 \"}", "%s, (%s == NULL)?0:(uint64_t)(%s->memory), (%s == NULL)?0:%s->offset, (%s == NULL)?0:%s->size" % (name, name, name, name, name, name, name), "")
        if "VkImageMemoryBarrier" in vk_type:
            return ("%p [0]={srcAccessMask=%u, dstAccessMask=%u, oldLayout=%s, newLayout=%s, srcQueueFamilyIndex=%u, dstQueueFamilyIndex=%u, image=%\" PRIx64 \", subresourceRange=%\" PRIx64 \"}", "%s, (%s == NULL)?0:%s->srcAccessMask, (%s == NULL)?0:%s->dstAccessMask, (%s == NULL)?NULL:string_VkImageLayout(%s->oldLayout), (%s == NULL)?NULL:string_VkImageLayout(%s->newLayout), (%s == NULL)?0:%s->srcQueueFamilyIndex, (%s == NULL)?0:%s->dstQueueFamilyIndex, (%s == NULL)?0:(uint64_t)(%s->image), (%s == NULL)?0:(uint64_t)&%s->subresourceRange" % (name, name, name, name, name, name, name, name, name, name, name, name, name, name, name, name, name), "")
        if "VkBufferMemoryBarrier" in vk_type:
            return ("%p [0]={srcAccessMask=%u, dstAccessMask=%u, srcQueueFamilyIndex=%u, dstQueueFamilyIndex=%u, buffer=%\" PRIx64 \", offset=%\" PRIu64 \", size=%\" PRIu64 \"}", "%s, (%s == NULL)?0:%s->srcAccessMask, (%s == NULL)?0:%s->dstAccessMask, (%s == NULL)?0:%s->srcQueueFamilyIndex, (%s == NULL)?0:%s->dstQueueFamilyIndex, (%s == NULL)?0:(uint64_t)%s->buffer, (%s == NULL)?0:%s->offset, (%s == NULL)?0:%s->size" % (name, name, name, name, name, name, name, name, name, name, name, name, name, name, name), "")
        if "VkSubmitInfo" in vk_type:
            return ("%p [0]={... waitSemaphoreCount=%u, pWaitSemaphores[0]=%\" PRIx64 \", cmdBufferCount=%u, pCmdBuffers[0]=%\" PRIx64 \", signalSemaphoreCount=%u, pSignalSemaphores[0]=%\" PRIx64 \" ...}", "%s, (%s == NULL)?0:%s->waitSemaphoreCount, (%s == NULL)?0:(%s->pWaitSemaphores == NULL)?0:(uint64_t)%s->pWaitSemaphores[0], (%s == NULL)?0:%s->commandBufferCount, (%s == NULL)?0:(%s->pCommandBuffers == NULL)?0:(uint64_t)%s->pCommandBuffers[0], (%s == NULL)?0:%s->signalSemaphoreCount, (%s == NULL)?0:(%s->pSignalSemaphores == NULL)?0:(uint64_t)%s->pSignalSemaphores[0]" % (name, name, name, name, name, name, name, name, name, name, name, name, name, name, name, name), "")
        if "VkPresentInfoKHR" in vk_type:
            return ("%p {... waitSemaphoreCount=%u, pWaitSemaphores[0]=%\" PRIx64 \", swapchainCount=%u, pSwapchains[0]=%\" PRIx64 \", pImageIndices[0]=%\" PRIu64 \" ...}", "%s, (%s == NULL)?0:%s->waitSemaphoreCount, (%s == NULL)?0:(%s->pWaitSemaphores == NULL)?0:(uint64_t)%s->pWaitSemaphores[0], (%s == NULL)?0:%s->swapchainCount, (%s == NULL)?0:(%s->pSwapchains == NULL)?0:(uint64_t)%s->pSwapchains[0], (%s == NULL)?0:(%s->pImageIndices == NULL)?0:(uint64_t)%s->pImageIndices[0]" % (name, name, name, name, name, name, name, name, name, name, name, name, name, name), "")
        if "VkFenceCreateInfo" in vk_type:
            return ("%p { flags=%s }", "%s, (%s == NULL)?\"0\":(%s->flags == VK_FENCE_CREATE_SIGNALED_BIT)?\"VK_FENCE_CREATE_SIGNALED_BIT\":\"0\"" % (name, name, name), "")
        if "VkBufferCopy" in vk_type:
            return ("%p [0]={srcOffset=%\" PRIu64 \", dstOffset=%\" PRIu64 \", size=%\" PRIu64 \"}", "%s, (%s == NULL)?0:%s->srcOffset, (%s == NULL)?0:%s->dstOffset, (%s == NULL)?0:%s->size" % (name, name, name, name, name, name, name), "")
        if "VkMemoryRequirements2" in vk_type:
            return ("%p {size=%\" PRIu64 \", alignment=%\" PRIu64 \", memoryTypeBits=%0x08X}", "%s, (%s == NULL)?0:%s->memoryRequirements.size, (%s == NULL)?0:%s->memoryRequirements.alignment, (%s == NULL)?0:%s->memoryRequirements.memoryTypeBits" % (name, name, name, name, name, name, name), "")
        elif "VkMemoryRequirements" in vk_type:
            return ("%p {size=%\" PRIu64 \", alignment=%\" PRIu64 \", memoryTypeBits=%0x08X}", "%s, (%s == NULL)?0:%s->size, (%s == NULL)?0:%s->alignment, (%s == NULL)?0:%s->memoryTypeBits" % (name, name, name, name, name, name, name), "")
        if "VkFenceGet" in vk_type:
            return ("%p {fence=%\" PRIx64 \", handleType=%\" PRIx64 \"}", "%s, (%s == NULL)?0:(uint64_t)%s->fence, (%s == NULL)?0:(uint64_t)%s->handleType" % (name, name, name, name, name), "")
        if "VkClearColor" in vk_type:
            return ("%p", "(void*)&%s" % name, deref)
        if "_type" in vk_type.lower(): # TODO : This should be generic ENUM check
            return ("%s", "string_%s(%s)" % (vk_type.replace('const ', '').strip('*'), name), deref)
        if "char*" in vk_type:
            return ("\\\"%s\\\"", name, "*")
        if "VkDeviceSize" in vk_type:
            if param.ispointer:
                return ("%\" PRIu64 \"",  "(%s == NULL) ? 0 : *(%s)" % (name, name), "*")
            return ("%\" PRIu64 \"", name, deref)
        if "uint64_t" in vk_type:
            if param.ispointer:
                return ("%\" PRIu64 \"",  "(%s == NULL) ? 0 : *(%s)" % (name, name), "*")
            return ("%\" PRIu64 \"", name, deref)
        if "uint32_t" in vk_type:
            if param.ispointer:
                return ("%u",  "(%s == NULL) ? 0 : *(%s)" % (name, name), "*")
            return ("%u", name, deref)
        if "xcb_visualid_t" in vk_type:
            if param.ispointer:
                return ("%u",  "(%s == NULL) ? 0 : *(%s)" % (name, name), "*")
            return ("%u", name, deref)
        if "VisualID" in vk_type:
            return ("%\" PRIu64 \"", "(uint64_t)%s" % name, deref)
        if "VkBool32" in vk_type:
            if param.ispointer:
                return ("%s",  "(%s && *%s == VK_TRUE) ? \"VK_TRUE\" : \"VK_FALSE\"" % (name, name), "*")
            return ("%s", "(%s == VK_TRUE) ? \"VK_TRUE\" : \"VK_FALSE\"" %(name), deref)
        if "VkFence" == vk_type:
            if param.ispointer:
                return ("%p {%\" PRIx64 \"}", "(void*)%s, (%s == NULL) ? 0 : (uint64_t)*(%s)" % (name, name, name), "*")
            return ("%p", "(void*)%s" %(name), deref)
        if "size_t" in vk_type:
            if param.ispointer:
                return ("\" VK_SIZE_T_SPECIFIER \"", "(%s == NULL) ? 0 : *(%s)" % (name, name), "*")
            return ("\" VK_SIZE_T_SPECIFIER \"", name, deref)
        if "float" in vk_type:
            if param.isstaticarray > 0: # handle array, current hard-coded to 4 (TODO: Make this dynamic)
                return ("[%f, %f, %f, %f]", "%s[0], %s[1], %s[2], %s[3]" % (name, name, name, name), deref)
            return ("%f", name, deref)
        if "bool" in vk_type or 'xcb_randr_crtc_t' in vk_type:
            return ("%u", name, deref)
        if "RROutput" in vk_type:
            return ("%u", "(uint32_t)(%s)" % name, "")
        if "PointerProperties" in vk_type and "int" != vk_type:
            return ("%p", "(void*)(%s)" % name, deref)
        if True in [t in vk_type.lower() for t in ["int", "flags", "mask", "xcb_window_t"]]:
            if param.isstaticarray > 0: # handle array, current hard-coded to 4 (TODO: Make this dynamic)
                return ("[%i, %i, %i, %i]", "%s[0], %s[1], %s[2], %s[3]" % (name, name, name, name), deref)
            if param.ispointer:
                return ("%i", "(%s == NULL) ? 0 : *(%s)" % (name, name), "*")
            return ("%i", name, deref)
        if output_param:
            return ("%p {%\" PRIX64 \"}", "(void*)%s, (%s == NULL) ? 0 : (uint64_t)*(%s)" % (name, name, name), deref)
        return ("%p", "(void*)(%s)" % name, deref)
    #
    # Construct vktrace vk header file
    def GenerateTraceVkHeader(self):
        trace_vk_hdr  = '#include "vktrace_vk_vk_packets.h"\n'
        trace_vk_hdr += '#include "vktrace_vk_packet_id.h"\n'
        trace_vk_hdr += '#include "vulkan/vk_layer.h"\n'
        trace_vk_hdr += '\n'
        trace_vk_hdr += 'void InitTracer(void);\n'
        trace_vk_hdr += '\n'
        trace_vk_hdr += '#ifdef WIN32\n'
        trace_vk_hdr += 'BOOL CALLBACK InitTracer(_Inout_ PINIT_ONCE initOnce, _Inout_opt_ PVOID param, _Out_opt_ PVOID *lpContext);\n'
        trace_vk_hdr += 'extern INIT_ONCE gInitOnce;\n'
        trace_vk_hdr += '#elif defined(PLATFORM_LINUX)\n'
        trace_vk_hdr += 'void InitTracer(void);\n'
        trace_vk_hdr += 'extern pthread_once_t gInitOnce;\n'
        trace_vk_hdr += '#endif\n'
        trace_vk_hdr += '\n'
        trace_vk_hdr += '#ifdef __cplusplus\n'
        trace_vk_hdr += 'extern"C" {\n'
        trace_vk_hdr += '#endif\n'
        trace_vk_hdr += '// Hooked function prototypes\n'
        trace_vk_hdr += '\n'
        cmd_protect_dict = dict(self.cmd_feature_protect)
        cmd_info_dict = dict(self.cmd_info_data)
        cmd_extension_dict = dict(self.cmd_extension_names)
        for api in self.cmdMembers:
            extension = cmd_extension_dict[api.name]
            if 'VK_VERSION_' not in extension and extension not in approved_ext:
                continue
            cmdinfo = cmd_info_dict[api.name]
            cdecl = self.makeCDecls(cmdinfo.elem)[0]
            protect = cmd_protect_dict[api.name]
            if protect is not None:
                trace_vk_hdr += '#ifdef %s\n' % protect
            if api.name[2:] not in api_exclusions:
                trace_vk_hdr += 'VKTRACER_EXPORT %s\n' % cdecl.replace('VKAPI_CALL vk', 'VKAPI_CALL __HOOKED_vk')
            # LoaderLayerInterface V0
            if api.name in [ 'vkGetInstanceProcAddr', 'vkGetDeviceProcAddr']:
                trace_vk_hdr += 'VK_LAYER_EXPORT %s\n' % cdecl.replace('VKAPI_CALL vk', 'VKAPI_CALL VK_LAYER_LUNARG_vktrace')
            if api.name in [ 'vkEnumerateInstanceLayerProperties', 'vkEnumerateInstanceExtensionProperties', 'vkEnumerateDeviceLayerProperties', 'vkEnumerateDeviceExtensionProperties']:
                trace_vk_hdr += 'VK_LAYER_EXPORT %s\n' % cdecl
            if protect is not None:
                trace_vk_hdr += '#endif // %s\n' % protect
        trace_vk_hdr += '#ifdef __cplusplus\n'
        trace_vk_hdr += '}\n'
        trace_vk_hdr += '#endif\n'
        return trace_vk_hdr

    # Take a list of params and return a list of dicts w/ ptr param details
    def GetPacketPtrParamList(self, params):
        ptr_param_list = []
        # TODO : This is a slightly nicer way to handle custom cases than initial code, however
        #   this can still be further generalized to eliminate more custom code
        #   big case to handle is when ptrs to structs have embedded data that needs to be accounted for in packet
        custom_ptr_dict = {'VkDeviceCreateInfo': {'add_txt': 'add_VkDeviceCreateInfo_to_packet(pHeader, (VkDeviceCreateInfo**) &(pPacket->pCreateInfo), pCreateInfo)',
                                                  'finalize_txt': ''},
                           'VkApplicationInfo': {'add_txt': 'add_VkApplicationInfo_to_packet(pHeader, (VkApplicationInfo**)&(pPacket->pApplicationInfo), pApplicationInfo)',
                                                 'finalize_txt': ''},
                           'VkPhysicalDevice': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pGpus), *pGpuCount*sizeof(VkPhysicalDevice), pGpus)',
                                                'finalize_txt': 'default'},
                           'VkImageCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkImageCreateInfo), pCreateInfo);\n'
                                                            '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices), sizeof(uint32_t) * pCreateInfo->queueFamilyIndexCount, pCreateInfo->pQueueFamilyIndices)',
                                               'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices));\n'
                                                               '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'VkBufferCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkBufferCreateInfo), pCreateInfo);\n'
                                                            '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices), sizeof(uint32_t) * pCreateInfo->queueFamilyIndexCount, pCreateInfo->pQueueFamilyIndices)',
                                               'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices));\n'
                                                               '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'pDataSize': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDataSize), sizeof(size_t), &_dataSize)',
                                         'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDataSize))'},
                           'pData': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pData), _dataSize, pData)',
                                     'finalize_txt': 'default'},
                           'pName': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pName), ((pName != NULL) ? strlen(pName) + 1 : 0), pName)',
                                     'finalize_txt': 'default'},
                           'pMarker': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMarker), ((pMarker != NULL) ? strlen(pMarker) + 1 : 0), pMarker)',
                                       'finalize_txt': 'default'},
                           'pExtName': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pExtName), ((pExtName != NULL) ? strlen(pExtName) + 1 : 0), pExtName)',
                                        'finalize_txt': 'default'},
                           'pDescriptorSets': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorSets), customSize, pDescriptorSets)',
                                               'finalize_txt': 'default'},
                           'pSparseMemoryRequirements': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSparseMemoryRequirements), (*pSparseMemoryRequirementCount) * sizeof(VkSparseImageMemoryRequirements), pSparseMemoryRequirements)',
                                               'finalize_txt': 'default'},
                           'pAllocator': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL)',
                                          'finalize_txt': 'default'},
                           'VkSparseImageFormatProperties': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pProperties), (*pPropertyCount) * sizeof(VkSparseImageFormatProperties), pProperties)',
                                               'finalize_txt': 'default'},
                           'VkSparseMemoryBindInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBindInfo), numBindings * sizeof(VkSparseMemoryBindInfo), pBindInfo)',
                                               'finalize_txt': 'default'},
                           'VkSparseImageMemoryBindInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBindInfo), numBindings * sizeof(VkSparseImageMemoryBindInfo), pBindInfo)',
                                               'finalize_txt': 'default'},
                           'VkFramebufferCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkFramebufferCreateInfo), pCreateInfo);\n'
                                                                  '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pColorAttachments), colorCount * sizeof(VkColorAttachmentBindInfo), pCreateInfo->pColorAttachments);\n'
                                                                  '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pDepthStencilAttachment), dsSize, pCreateInfo->pDepthStencilAttachment)',
                                                  'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pColorAttachments));\n'
                                                                  '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pDepthStencilAttachment));\n'
                                                                  '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'VkRenderPassCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkRenderPassCreateInfo), pCreateInfo);\n'
                                                                 '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pColorFormats), colorCount * sizeof(VkFormat), pCreateInfo->pColorFormats);\n'
                                                                 '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pColorLayouts), colorCount * sizeof(VkImageLayout), pCreateInfo->pColorLayouts);\n'
                                                                 '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pColorLoadOps), colorCount * sizeof(VkAttachmentLoadOp), pCreateInfo->pColorLoadOps);\n'
                                                                 '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pColorStoreOps), colorCount * sizeof(VkAttachmentStoreOp), pCreateInfo->pColorStoreOps);\n'
                                                                 '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pColorLoadClearValues), colorCount * sizeof(VkClearColor), pCreateInfo->pColorLoadClearValues)',
                                                 'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pColorFormats));\n'
                                                                 '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pColorLayouts));\n'
                                                                 '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pColorLoadOps));\n'
                                                                 '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pColorStoreOps));\n'
                                                                 '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pColorLoadClearValues));\n'
                                                                 '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'VkPipelineLayoutCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkPipelineLayoutCreateInfo), pCreateInfo);\n'
                                                                     '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pSetLayouts), pCreateInfo->setLayoutCount * sizeof(VkDescriptorSetLayout), pCreateInfo->pSetLayouts);'
                                                                     '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pPushConstantRanges), pCreateInfo->pushConstantRangeCount * sizeof(VkPushConstantRange), pCreateInfo->pPushConstantRanges);',
                                                     'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pSetLayouts));\n'
                                                                     'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pPushConstantRanges));\n'
                                                                     'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'VkDescriptorPoolCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkDescriptorPoolCreateInfo), pCreateInfo);\n'
                                                                     '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pPoolSizes), pCreateInfo->poolSizeCount * sizeof(VkDescriptorPoolSize), pCreateInfo->pPoolSizes)',
                                                     'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pPoolSizes));\n'
                                                                     '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'VkDescriptorSetLayoutCreateInfo': {'add_txt': 'add_create_ds_layout_to_trace_packet(pHeader, &pPacket->pCreateInfo, pCreateInfo)',
                                                          'finalize_txt': '// pCreateInfo finalized in add_create_ds_layout_to_trace_packet'},
                           'VkSwapchainCreateInfoKHR': {'add_txt':      'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkSwapchainCreateInfoKHR), pCreateInfo);\n'
                                                                        '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices), pPacket->pCreateInfo->queueFamilyIndexCount * sizeof(uint32_t), pCreateInfo->pQueueFamilyIndices)',
                                                        'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices));\n'
                                                                        '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                           'VkShaderModuleCreateInfo': {'add_txt':      'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkShaderModuleCreateInfo), pCreateInfo);\n'
                                                                        '    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pCode), pPacket->pCreateInfo->codeSize, pCreateInfo->pCode)',
                                                        'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pCode));\n'
                                                                        '    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo))'},
                          }

        for p in params:
            pp_dict = {}
            if p.ispointer and p.name not in ['pTag', 'pUserData']:
                if p.len is not None:
                    multiplier = ''
                    if (p.type != 'void'):
                        multiplier = ' * sizeof(%s)' % p.type
                    if p.len[0] == 'p': # Count parameter is itself a pointer
                        pp_dict['add_txt'] = 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), (*%s)%s, %s)' % (p.name, p.len, multiplier, p.name)
                    else:
                        pp_dict['add_txt'] = 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), %s%s, %s)' % (p.name, p.len, multiplier, p.name)
                elif p.type in custom_ptr_dict:
                    pp_dict['add_txt'] = custom_ptr_dict[p.type.strip('*').replace('const ', '')]['add_txt']
                    pp_dict['finalize_txt'] = custom_ptr_dict[p.type.strip('*').replace('const ', '')]['finalize_txt']
                elif p.name in custom_ptr_dict:
                    pp_dict['add_txt'] = custom_ptr_dict[p.name]['add_txt']
                    pp_dict['finalize_txt'] = custom_ptr_dict[p.name]['finalize_txt']
                    # TODO : This is custom hack to account for 2 pData items with dataSize param for sizing
                    if 'pData' == p.name and 'dataSize' == params[params.index(p)-1].name:
                        pp_dict['add_txt'] = pp_dict['add_txt'].replace('_dataSize', 'dataSize')
                elif 'void' in p.type and (p.name == 'pData' or p.name == 'pValues' or p.name == 'pHostPointer'):
                    pp_dict['add_txt'] = '//TODO FIXME vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), sizeof(%s), %s)' % (p.name, p.type.strip('*').replace('const ', ''), p.name)
                    pp_dict['finalize_txt'] = '//TODO FIXME vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->%s))' % (p.name)
                else:
                    pp_dict['add_txt'] = 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), sizeof(%s), %s)' % (p.name, p.type.strip('*').replace('const ', ''), p.name)
                if 'finalize_txt' not in pp_dict or 'default' == pp_dict['finalize_txt']:
                    pp_dict['finalize_txt'] = 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->%s))' % (p.name)
                pp_dict['index'] = params.index(p)
                ptr_param_list.append(pp_dict)
        return ptr_param_list

    # Take a list of params and return a list of packet size elements
    def GetPacketSize(self, params):
        ps = [] # List of elements to be added together to account for packet size for given params
        skip_list = [] # store params that are already accounted for so we don't count them twice
        # Dict of specific params with unique custom sizes
        # TODO: Now using bitfields for all stages, need pSetBindPoints to accommodate that.
        custom_size_dict = {'VkSwapchainCreateInfoKHR' : 'vk_size_vkswapchaincreateinfokhr',
                            }
        for p in params:
            # First handle custom cases
            if ((p.name in ['pCreateInfo', 'pBeginInfo', 'pAllocateInfo','pReserveSpaceInfo','pLimits','pExternalBufferInfo','pExternalBufferProperties','pGetFdInfo','pMemoryFdProperties','pExternalSemaphoreProperties','pExternalSemaphoreInfo','pImportSemaphoreFdInfo','pExternalFenceInfo','pExternalFenceProperties','pSurfaceInfo', 'pTagInfo','pNameInfo','pMarkerInfo','pDeviceGroupPresentCapabilities','pAcquireInfo','pPhysicalDeviceGroupProperties','pDisplayPowerInfo','pDeviceEventInfo','pDisplayEventInfo','pMetadata','pRenderPassBegin','pPresentInfo'])
                or (p.name in ['pFeatures'] and 'nvx' in p.type.lower())
                or (p.name in ['pInfo'] and 'void' not in p.type.lower())
                or (p.name in ['pFeatures', 'pProperties', 'pFormatProperties','pImageFormatInfo','pImageFormatProperties','pQueueFamilyProperties','pMemoryProperties','pFormatInfo','pSurfaceFormats','pMemoryRequirements','pSparseMemoryRequirements','pSurfaceCapabilities'] and '2khr' in p.type.lower())
                or (p.name in ['pSurfaceCapabilities'] and '2ext' in p.type.lower())):
                ps.append('get_struct_chain_size((void*)%s)' % (p.name))
                skip_list.append(p.name)
            elif p.name in custom_size_dict:
                ps.append('%s(%s)' % (custom_size_dict[p.name], p.name))
                skip_list.append(p.name)
            if p.type in custom_size_dict:
                ps.append('%s(%s)' % (custom_size_dict[p.type], p.name))
                skip_list.append(p.name)
            # Skip any params already handled
            if p.name in skip_list:
                continue
            # Now check to identify dynamic arrays which depend on two params
            if p.len:
                if p.ispointer:
                    if p.type == 'void':
                        if p.len[0] == 'p':
                            ps.append('(*%s)' % p.len)
                        else:
                            ps.append('%s' % p.len)
                    else:
                        if p.len[0] == 'p':
                            ps.append('(*%s) * sizeof(%s)' % (p.len, p.type))
                        else:
                            ps.append('%s * sizeof(%s)' % (p.len, p.type))
                    skip_list.append(p.name)
            elif p.ispointer:
                if 'pData' == p.name:
                    if 'dataSize' == params[params.index(p)-1].name:
                        ps.append('dataSize')
                    elif 'counterCount' == params[params.index(p)-1].name:
                        ps.append('sizeof(%s)' % p.type.strip('*').replace('const ', ''))
                    else:
                        #ps.append('((pDataSize != NULL && pData != NULL) ? *pDataSize : 0)')
                        ps.append('sizeof(void*)')
                elif '**' in p.cdecl and 'void' in p.type:
                    ps.append('sizeof(void*)')
                elif 'void' in p.type:
                    ps.append('sizeof(%s)' % p.name)
                elif 'char' in p.type:
                    ps.append('((%s != NULL) ? ROUNDUP_TO_4(strlen(%s) + 1) : 0)' % (p.name, p.name))
                elif 'pDataSize' in p.name:
                    ps.append('((pDataSize != NULL) ? sizeof(size_t) : 0)')
                else:
                    ps.append('sizeof(%s)' % (p.type.strip('*').replace('const ', '')))
        return ps

    # Generate instructions for certain API calls that need special handling when we are recording
    # them during the trim frames. This ensures that we recreate the objects that are referenced
    # during the trim frames.
    def GenerateTrimRecordingInstructions(self, proto):
        trim_instructions = []
        if 'CmdExecuteCommands' is proto.name:
            trim_instructions.append("            trim::write_packet(pHeader);")
            trim_instructions.append("            trim::mark_CommandBuffer_reference(commandBuffer);")
            trim_instructions.append("            if (pCommandBuffers != nullptr && commandBufferCount > 0) {")
            trim_instructions.append("                for (uint32_t i = 0; i < commandBufferCount; i++) {")
            trim_instructions.append("                    trim::mark_CommandBuffer_reference(pCommandBuffers[i]);")
            trim_instructions.append("                }")
            trim_instructions.append("            }")
        else:
            return None
        return "\n".join(trim_instructions)

    # Generate instructions for certain API calls that need to be tracked prior to the trim frames
    # so that we can recreate objects that are used within a trimmed trace file.
    def GenerateTrimStateTrackingInstructions(self, proto):
        trim_instructions = []
        if 'vkGetDeviceQueue' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Queue_object(*pQueue);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Queue.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.Queue.queueFamilyIndex = queueFamilyIndex;")
            trim_instructions.append("        info.ObjectInfo.Queue.queueIndex = queueIndex;")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateCommandPool' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_CommandPool_object(*pCommandPool);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.CommandPool.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.CommandPool.queueFamilyIndex = pCreateInfo->queueFamilyIndex;")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.CommandPool.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyCommandPool' == proto.name:
            trim_instructions.append("        trim::remove_CommandPool_object(commandPool);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyCommandPool' == proto.name:
            trim_instructions.append("        trim::remove_CommandPool_object(commandPool);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkFreeCommandBuffers' == proto.name:
            trim_instructions.append("        trim::ObjectInfo* pCBInfo = trim::get_CommandBuffer_objectInfo(pCommandBuffers[0]);")
            trim_instructions.append("        VkCommandBufferLevel level = (pCBInfo == NULL) ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : pCBInfo->ObjectInfo.CommandBuffer.level;")
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_CommandPool_objectInfo(commandPool);")
            trim_instructions.append("        if (pInfo != NULL ) { pInfo->ObjectInfo.CommandPool.numCommandBuffersAllocated[level] -= commandBufferCount; }")
            trim_instructions.append("        for (uint32_t i = 0; i < commandBufferCount; i++) {")
            trim_instructions.append("            trim::remove_CommandBuffer_object(pCommandBuffers[i]);")
            trim_instructions.append("            trim::remove_CommandBuffer_calls(pCommandBuffers[i]);")
            trim_instructions.append("            trim::ClearImageTransitions(pCommandBuffers[i]);")
            trim_instructions.append("            trim::ClearBufferTransitions(pCommandBuffers[i]);")
            trim_instructions.append('        }')
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkQueueBindSparse' == proto.name:
            trim_instructions.append("        if (result == VK_SUCCESS) {")
            trim_instructions.append("            trim::ObjectInfo* pFenceInfo = trim::get_Fence_objectInfo(fence);")
            trim_instructions.append("            if (pFenceInfo != NULL) {")
            trim_instructions.append("                pFenceInfo->ObjectInfo.Fence.pendingOnQueue = queue;")
            trim_instructions.append("            }")
            trim_instructions.append("            if (pBindInfo != NULL && pBindInfo->pWaitSemaphores != NULL) {")
            trim_instructions.append("                for (uint32_t i = 0; i < pBindInfo->waitSemaphoreCount; i++) {")
            trim_instructions.append("                    trim::ObjectInfo* pInfo = trim::get_Semaphore_objectInfo(pBindInfo->pWaitSemaphores[i]);")
            trim_instructions.append("                    if (pInfo != NULL) {")
            trim_instructions.append("                        pInfo->ObjectInfo.Semaphore.signaledOnQueue = VK_NULL_HANDLE;")
            trim_instructions.append("                    }")
            trim_instructions.append("                }")
            trim_instructions.append("            }")
            trim_instructions.append("            if (pBindInfo != NULL && pBindInfo->pSignalSemaphores != NULL) {")
            trim_instructions.append("                for (uint32_t i = 0; i < pBindInfo->signalSemaphoreCount; i++) {")
            trim_instructions.append("                    trim::ObjectInfo* pInfo = trim::get_Semaphore_objectInfo(pBindInfo->pSignalSemaphores[i]);")
            trim_instructions.append("                    if (pInfo != NULL) {")
            trim_instructions.append("                        pInfo->ObjectInfo.Semaphore.signaledOnQueue = queue;")
            trim_instructions.append("                    }")
            trim_instructions.append("                }")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateSemaphore' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Semaphore_object(*pSemaphore);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Semaphore.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.Semaphore.signaledOnQueue = VK_NULL_HANDLE;")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Semaphore.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroySemaphore' == proto.name:
            trim_instructions.append("        trim::remove_Semaphore_object(semaphore);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateFence' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Fence_object(*pFence);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Fence.signaled = ((pCreateInfo->flags & VK_FENCE_CREATE_SIGNALED_BIT) == VK_FENCE_CREATE_SIGNALED_BIT);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Fence.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkResetFences' == proto.name:
            trim_instructions.append("        for (uint32_t i = 0; i < fenceCount; i++) {")
            trim_instructions.append("            trim::ObjectInfo* pFenceInfo = trim::get_Fence_objectInfo(pFences[i]);")
            trim_instructions.append("            if (pFenceInfo != NULL && result == VK_SUCCESS) {")
            trim_instructions.append("                // clear the fence")
            trim_instructions.append("                pFenceInfo->ObjectInfo.Fence.signaled = false;")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyFence' == proto.name:
            trim_instructions.append("        trim::remove_Fence_object(fence);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('vkCmdCopyImage' == proto.name or
              'vkCmdBlitImage' == proto.name or
              'vkCmdResolveImage' == proto.name or
              'vkCmdCopyBufferToImage' == proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(dstImage);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.mostRecentLayout = dstImageLayout;")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('vkCmdClearColorImage' == proto.name or
              'vkCmdClearDepthStencilImage' == proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(image);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.mostRecentLayout = imageLayout;")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCmdEndRenderPass' == proto.name:
            trim_instructions.append("        trim::ObjectInfo* pCommandBuffer = trim::get_CommandBuffer_objectInfo(commandBuffer);")
            trim_instructions.append("        if (pCommandBuffer != nullptr) {")
            trim_instructions.append("            trim::ObjectInfo* pRenderPass = trim::get_RenderPass_objectInfo(pCommandBuffer->ObjectInfo.CommandBuffer.activeRenderPass);")
            trim_instructions.append("            if (pRenderPass != nullptr) {")
            trim_instructions.append('                for (uint32_t i = 0; i < pRenderPass->ObjectInfo.RenderPass.attachmentCount; i++) {')
            trim_instructions.append('                    trim::AddImageTransition(commandBuffer, pRenderPass->ObjectInfo.RenderPass.pAttachments[i]);')
            trim_instructions.append('                }')
            trim_instructions.append('            }')
            trim_instructions.append('        }')
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')

        elif ('vkEndCommandBuffer' == proto.name or
              'vkCmdBindPipeline' == proto.name or
              'vkCmdSetViewport' == proto.name or
              'vkCmdSetScissor' == proto.name or
              'vkCmdSetLineWidth' == proto.name or
              'vkCmdSetDepthBias' == proto.name or
              'vkCmdSetBlendConstants' == proto.name or
              'vkCmdSetDepthBounds' == proto.name or
              'vkCmdSetStencilCompareMask' == proto.name or
              'vkCmdSetStencilWriteMask' == proto.name or
              'vkCmdSetStencilReference' == proto.name or
              'vkCmdBindDescriptorSets' == proto.name or
              'vkCmdBindIndexBuffer' == proto.name or
              'vkCmdBindVertexBuffers' == proto.name or
              'vkCmdDraw' == proto.name or
              'vkCmdDrawIndexed' == proto.name or
              'vkCmdDrawIndirect' == proto.name or
              'vkCmdDrawIndexedIndirect' == proto.name or
              'vkCmdDispatch' == proto.name or
              'vkCmdDispatchIndirect' == proto.name or
              'vkCmdCopyBuffer' == proto.name or
              'vkCmdCopyImageToBuffer' == proto.name or
              'vkCmdUpdateBuffer' == proto.name or
              'vkCmdFillBuffer' == proto.name or
              'vkCmdClearAttachments' == proto.name or
              'vkCmdSetEvent' == proto.name or
              'vkCmdResetEvent' == proto.name or
              'vkCmdNextSubpass' == proto.name or
              'vkCmdExecuteCommands' == proto.name):
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('vkResetCommandBuffer' == proto.name):
            trim_instructions.append("        trim::remove_CommandBuffer_calls(commandBuffer);")
            trim_instructions.append("        trim::ClearImageTransitions(commandBuffer);")
            trim_instructions.append("        trim::ClearBufferTransitions(commandBuffer);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateImageView' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_ImageView_object(*pView);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.ImageView.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.ImageView.image = pCreateInfo->image;")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.ImageView.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyImageView' == proto.name:
            trim_instructions.append("        trim::remove_ImageView_object(imageView);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('vkGetImageMemoryRequirements' == proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(image);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.memorySize = pMemoryRequirements->size;")
            trim_instructions.append("        }")
            trim_instructions.append("#if TRIM_USE_ORDERED_IMAGE_CREATION")
            trim_instructions.append("        trim::add_Image_call(trim::copy_packet(pHeader));")
            trim_instructions.append("#else")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.pGetImageMemoryRequirementsPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        }")
            trim_instructions.append("#endif //TRIM_USE_ORDERED_IMAGE_CREATION")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyImage' == proto.name:
            trim_instructions.append("#if TRIM_USE_ORDERED_IMAGE_CREATION")
            trim_instructions.append("        trim::add_Image_call(trim::copy_packet(pHeader));")
            trim_instructions.append("#endif //TRIM_USE_ORDERED_IMAGE_CREATION")
            trim_instructions.append("        trim::remove_Image_object(image);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkBindImageMemory' == proto.name:
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(image);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            if (pInfo->ObjectInfo.Image.memorySize == 0) {")
            trim_instructions.append("                // trim get image memory size through target title call")
            trim_instructions.append("                // vkGetImageMemoryRequirements for the image, but so")
            trim_instructions.append("                // far the title doesn't call vkGetImageMemoryRequirements,")
            trim_instructions.append("                // so here we call it for the image.")
            trim_instructions.append("                VkMemoryRequirements MemoryRequirements;")
            trim_instructions.append("                mdd(device)->devTable.GetImageMemoryRequirements(device,image,&MemoryRequirements);")
            trim_instructions.append("                pInfo->ObjectInfo.Image.memorySize = MemoryRequirements.size;")
            trim_instructions.append("            }")
            trim_instructions.append("")
            trim_instructions.append("            pInfo->ObjectInfo.Image.pBindImageMemoryPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("            pInfo->ObjectInfo.Image.memory = memory;")
            trim_instructions.append("            pInfo->ObjectInfo.Image.memoryOffset = memoryOffset;")
            trim_instructions.append("            pInfo->ObjectInfo.Image.needsStagingBuffer = pInfo->ObjectInfo.Image.needsStagingBuffer || trim::IsMemoryDeviceOnly(memory);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateBufferView' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_BufferView_object(*pView);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.BufferView.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.BufferView.pAllocator = pAllocator;")
            trim_instructions.append("        }")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyBufferView' == proto.name:
            trim_instructions.append("        trim::remove_BufferView_object(bufferView);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateBuffer' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Buffer_object(*pBuffer);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Buffer.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.Buffer.size = pCreateInfo->size;")
            trim_instructions.append("        if (pCreateInfo->queueFamilyIndexCount > 0) { info.ObjectInfo.Buffer.queueFamilyIndex = pCreateInfo->pQueueFamilyIndices[0]; }")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Buffer.pAllocator = pAllocator;")
            trim_instructions.append("        }")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyBuffer' == proto.name:
            trim_instructions.append("        trim::remove_Buffer_object(buffer);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkBindBufferMemory' == proto.name:
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Buffer_objectInfo(buffer);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.pBindBufferMemoryPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.memory = memory;")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.memoryOffset = memoryOffset;")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.needsStagingBuffer = trim::IsMemoryDeviceOnly(memory);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateSampler' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Sampler_object(*pSampler);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Sampler.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Sampler.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroySampler' == proto.name:
            trim_instructions.append("        trim::remove_Sampler_object(sampler);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateDescriptorSetLayout' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_DescriptorSetLayout_object(*pSetLayout);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.DescriptorSetLayout.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.DescriptorSetLayout.bindingCount = pCreateInfo->bindingCount;")
            trim_instructions.append("        info.ObjectInfo.DescriptorSetLayout.pBindings = (pCreateInfo->bindingCount == 0) ? nullptr : new VkDescriptorSetLayoutBinding[pCreateInfo->bindingCount];")
            trim_instructions.append("        for (uint32_t i = 0; i < pCreateInfo->bindingCount; i++ ) {")
            trim_instructions.append("            info.ObjectInfo.DescriptorSetLayout.pBindings[i] = pCreateInfo->pBindings[i];")
            trim_instructions.append("            if (pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT) {")
            trim_instructions.append("                    info.ObjectInfo.DescriptorSetLayout.numImages++;")
            trim_instructions.append("            }")
            trim_instructions.append("            if (pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC) {")
            trim_instructions.append("                    info.ObjectInfo.DescriptorSetLayout.numBuffers++;")
            trim_instructions.append("            }")
            trim_instructions.append("            if (pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER ||")
            trim_instructions.append("                pCreateInfo->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER) {")
            trim_instructions.append("                    info.ObjectInfo.DescriptorSetLayout.numTexelBufferViews++;")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.DescriptorSetLayout.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyDescriptorSetLayout' == proto.name:
            trim_instructions.append("        trim::remove_DescriptorSetLayout_object(descriptorSetLayout);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkResetDescriptorPool' == proto.name:
            trim_instructions.append("        trim::ObjectInfo* pPoolInfo = trim::get_DescriptorPool_objectInfo(descriptorPool);")
            trim_instructions.append("        pPoolInfo->ObjectInfo.DescriptorPool.numSets = 0;")
            trim_instructions.append("        trim::reset_DescriptorPool(descriptorPool);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreatePipelineLayout' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_PipelineLayout_object(*pPipelineLayout);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.PipelineLayout.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.PipelineLayout.descriptorSetLayoutCount = pCreateInfo->setLayoutCount;")
            trim_instructions.append("        info.ObjectInfo.PipelineLayout.pDescriptorSetLayouts = (pCreateInfo->setLayoutCount == 0) ? nullptr : new VkDescriptorSetLayout[pCreateInfo->setLayoutCount];")
            trim_instructions.append("        memcpy(info.ObjectInfo.PipelineLayout.pDescriptorSetLayouts, pCreateInfo->pSetLayouts, pCreateInfo->setLayoutCount * sizeof( VkDescriptorSetLayout ));")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.PipelineLayout.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyPipelineLayout' == proto.name:
            trim_instructions.append("        trim::remove_PipelineLayout_object(pipelineLayout);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyPipelineCache' == proto.name:
            trim_instructions.append("        trim::remove_PipelineCache_object(pipelineCache);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroySwapchainKHR' == proto.name:
            trim_instructions.append("        trim::remove_SwapchainKHR_object(swapchain);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroySurfaceKHR' == proto.name:
            trim_instructions.append("        trim::remove_SurfaceKHR_object(surface);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyPipelineCache' == proto.name:
            trim_instructions.append("        trim::remove_PipelineCache_object(pipelineCache);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroySwapchainKHR' == proto.name:
            trim_instructions.append("        trim::remove_SwapchainKHR_object(swapchain);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroySurfaceKHR' == proto.name:
            trim_instructions.append("        trim::remove_SurfaceKHR_object(surface);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyRenderPass' == proto.name:
            trim_instructions.append("        trim::remove_RenderPass_object(renderPass);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateShaderModule' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_ShaderModule_object(*pShaderModule);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        trim::StateTracker::copy_VkShaderModuleCreateInfo(&info.ObjectInfo.ShaderModule.createInfo, *pCreateInfo);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.ShaderModule.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyShaderModule' == proto.name:
            trim_instructions.append("        trim::remove_ShaderModule_object(shaderModule);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyPipeline' == proto.name:
            trim_instructions.append("        trim::remove_Pipeline_object(pipeline);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyDescriptorPool' == proto.name:
            trim_instructions.append("        trim::remove_DescriptorPool_object(descriptorPool);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyFramebuffer' == proto.name:
            trim_instructions.append("        trim::remove_Framebuffer_object(framebuffer);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateEvent' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Event_object(*pEvent);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Event.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Event.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyEvent' == proto.name:
            trim_instructions.append("        trim::remove_Event_object(event);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCreateQueryPool' == proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_QueryPool_object(*pQueryPool);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.QueryPool.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pCreateInfo != nullptr) {")
            trim_instructions.append("            info.ObjectInfo.QueryPool.queryType = pCreateInfo->queryType;")
            trim_instructions.append("            info.ObjectInfo.QueryPool.size = pCreateInfo->queryCount;")
            trim_instructions.append("            info.ObjectInfo.QueryPool.pResultsAvailable = new bool[pCreateInfo->queryCount];")
            trim_instructions.append("            for (uint32_t i = 0; i < pCreateInfo->queryCount; i++) {")
            trim_instructions.append("                info.ObjectInfo.QueryPool.pResultsAvailable[i] = false;")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.QueryPool.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCmdResetQueryPool' == proto.name:
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_QueryPool_objectInfo(queryPool);")
            trim_instructions.append("        if (pInfo != NULL) {" )
            trim_instructions.append("            for (uint32_t i = firstQuery; (i < pInfo->ObjectInfo.QueryPool.size) && (i < firstQuery + queryCount); i++) {")
            trim_instructions.append("                pInfo->ObjectInfo.QueryPool.pResultsAvailable[i] = false;")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkCmdBeginQuery' == proto.name:
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('vkCmdEndQuery' == proto.name or
              'vkCmdWriteTimestamp' == proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_QueryPool_objectInfo(queryPool);")
            trim_instructions.append("        if (pInfo != NULL) {" )
            trim_instructions.append("            pInfo->ObjectInfo.QueryPool.commandBuffer = commandBuffer;")
            trim_instructions.append("            pInfo->ObjectInfo.QueryPool.pResultsAvailable[query] = true;")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyQueryPool' == proto.name:
            trim_instructions.append("        trim::remove_QueryPool_object(queryPool);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkAcquireNextImageKHR' == proto.name:
            trim_instructions.append("        if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR ) {")
            trim_instructions.append("            if (semaphore != VK_NULL_HANDLE) {")
            trim_instructions.append("                trim::ObjectInfo* pInfo = trim::get_Semaphore_objectInfo(semaphore);")
            trim_instructions.append("                if (pInfo != NULL) {")
            trim_instructions.append("                    // TODO Find another way to signal this semaphore since it's not on a queue.")
            trim_instructions.append("                    pInfo->ObjectInfo.Semaphore.signaledOnQueue = VK_NULL_HANDLE;")
            trim_instructions.append("                }")
            trim_instructions.append("            }")
            trim_instructions.append("            if (fence != VK_NULL_HANDLE) {")
            trim_instructions.append("                trim::ObjectInfo* pFenceInfo = trim::get_Fence_objectInfo(fence);")
            trim_instructions.append("                if (pFenceInfo != NULL) {")
            trim_instructions.append("                    pFenceInfo->ObjectInfo.Fence.signaled = true;")
            trim_instructions.append("                }")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkGetPhysicalDeviceMemoryProperties' == proto.name:
            trim_instructions.append("        if (g_trimIsPreTrim) {")
            trim_instructions.append("            trim::ObjectInfo* pInfo = trim::get_PhysicalDevice_objectInfo(physicalDevice);")
            trim_instructions.append("            if (pInfo != NULL) {")
            trim_instructions.append("                pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'vkDestroyDevice' == proto.name:
            trim_instructions.append("        trim::remove_Device_object(device);")
            trim_instructions.append('        if (g_trimIsInTrim) {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        } else {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        else:
            return None
        return "\n".join(trim_instructions)




    #
    # Construct the vktrace vk source file
    def GenerateTraceVkSource(self):


        trace_vk_src  = ''


        trace_vk_src += '#include "vktrace_platform.h"\n'
        trace_vk_src += '#include "vktrace_common.h"\n'
        trace_vk_src += '#include "vktrace_lib_helpers.h"\n'
        trace_vk_src += '#include "vktrace_lib_trim.h"\n'
        trace_vk_src += '#include "vktrace_vk_vk.h"\n'
        trace_vk_src += '#include "vktrace_interconnect.h"\n'
        trace_vk_src += '#include "vktrace_filelike.h"\n'
        trace_vk_src += '#include "vk_struct_size_helper.h"\n'
        trace_vk_src += '#ifdef PLATFORM_LINUX\n'
        trace_vk_src += '#include <pthread.h>\n'
        trace_vk_src += '#endif\n'
        trace_vk_src += '#include "vktrace_trace_packet_utils.h"\n'
        trace_vk_src += '#include <stdio.h>\n'
        trace_vk_src += '#include <string.h>\n'
        trace_vk_src += '\n'
        trace_vk_src += '#ifdef WIN32\n'
        trace_vk_src += 'INIT_ONCE gInitOnce = INIT_ONCE_STATIC_INIT;\n'
        trace_vk_src += '#elif defined(PLATFORM_LINUX)\n'
        trace_vk_src += 'pthread_once_t gInitOnce = PTHREAD_ONCE_INIT;\n'
        trace_vk_src += '#endif\n'
        trace_vk_src += '\n'
        trace_vk_src += 'extern VKTRACE_CRITICAL_SECTION g_memInfoLock;\n'
        trace_vk_src += '\n'
        trace_vk_src += '#ifdef WIN32\n'
        trace_vk_src += 'BOOL CALLBACK InitTracer(_Inout_ PINIT_ONCE initOnce, _Inout_opt_ PVOID param, _Out_opt_ PVOID *lpContext) {\n'
        trace_vk_src += '#elif defined(PLATFORM_LINUX)\n'
        trace_vk_src += 'void InitTracer(void) {\n'
        trace_vk_src += '#endif\n\n'
        trace_vk_src += '#if defined(ANDROID)\n'
        trace_vk_src += '    // On Android, we can use an abstract socket to fit permissions model\n'
        trace_vk_src += '    const char *ipAddr = "localabstract";\n'
        trace_vk_src += '    const char *ipPort = "vktrace";\n'
        trace_vk_src += '    gMessageStream = vktrace_MessageStream_create_port_string(FALSE, ipAddr, ipPort);\n'
        trace_vk_src += '#else\n'
        trace_vk_src += '    const char *ipAddr = vktrace_get_global_var("VKTRACE_LIB_IPADDR");\n'
        trace_vk_src += '    if (ipAddr == NULL)\n'
        trace_vk_src += '        ipAddr = "127.0.0.1";\n'
        trace_vk_src += '    gMessageStream = vktrace_MessageStream_create(FALSE, ipAddr, VKTRACE_BASE_PORT + VKTRACE_TID_VULKAN);\n'
        trace_vk_src += '#endif\n'
        trace_vk_src += '    vktrace_trace_set_trace_file(vktrace_FileLike_create_msg(gMessageStream));\n'
        trace_vk_src += '    vktrace_tracelog_set_tracer_id(VKTRACE_TID_VULKAN);\n'
        trace_vk_src += '    trim::initialize();\n'
        trace_vk_src += '    vktrace_initialize_trace_packet_utils();\n'
        trace_vk_src += '    vktrace_create_critical_section(&g_memInfoLock);\n'
        trace_vk_src += '#ifdef WIN32\n'
        trace_vk_src += '    return true;\n}\n'
        trace_vk_src += '#elif defined(PLATFORM_LINUX)\n'
        trace_vk_src += '    return;\n}\n'
        trace_vk_src += '#endif\n'
        trace_vk_src += '\n'

        # Generate functions used to trace API calls and store the input and result data into a packet
        # Here's the general flow of code insertion w/ option items flagged w/ "?"
        # Result decl?
        # Packet struct decl
        # ?Special case : setup call to function first and do custom API call time tracking
        # CREATE_PACKET
        # call real entrypoint and get return value (if there is one)
        # Assign packet values
        # FINISH packet
        # return result if needed

        manually_written_hooked_funcs = ['vkAllocateCommandBuffers',
                                         'vkAllocateMemory',
                                         'vkAllocateDescriptorSets',
                                         'vkBeginCommandBuffer',
                                         'vkCreateDescriptorPool',
                                         'vkGetPhysicalDeviceProperties',
                                         'vkGetPhysicalDeviceProperties2KHR',
                                         'vkCreateDevice',
                                         'vkCreateFramebuffer',
                                         'vkCreateImage',
                                         'vkCreateBuffer',
                                         'vkCreateInstance',
                                         'vkCreatePipelineCache',
                                         'vkCreateRenderPass',
                                         'vkGetPipelineCacheData',
                                         'vkCreateGraphicsPipelines',
                                         'vkCreateComputePipelines',
                                         'vkCmdPipelineBarrier',
                                         'vkCmdWaitEvents',
                                         'vkCmdBeginRenderPass',
                                         'vkCmdPushConstants',
                                         'vkDestroyInstance',
                                         'vkEnumeratePhysicalDevices',
                                         'vkFreeMemory',
                                         'vkFreeDescriptorSets',
                                         'vkQueueSubmit',
                                         'vkQueueBindSparse',
                                         'vkFlushMappedMemoryRanges',
                                         'vkInvalidateMappedMemoryRanges',
                                         'vkGetDeviceProcAddr',
                                         'vkGetInstanceProcAddr',
                                         'vkEnumerateInstanceExtensionProperties',
                                         'vkEnumerateDeviceExtensionProperties',
                                         'vkEnumerateInstanceLayerProperties',
                                         'vkEnumerateDeviceLayerProperties',
                                         'vkGetPhysicalDeviceQueueFamilyProperties',
                                         'vkGetPhysicalDeviceQueueFamilyProperties2KHR',
                                         'vkGetQueryPoolResults',
                                         'vkMapMemory',
                                         'vkUnmapMemory',
                                         'vkUpdateDescriptorSets',
                                         'vkGetPhysicalDeviceSurfaceCapabilitiesKHR',
                                         'vkGetPhysicalDeviceSurfaceFormatsKHR',
                                         'vkGetPhysicalDeviceSurfacePresentModesKHR',
                                         'vkCreateSwapchainKHR',
                                         'vkGetSwapchainImagesKHR',
                                         'vkQueuePresentKHR',
                                         #TODO add Mir
                                         'vkCreateXcbSurfaceKHR',
                                         'vkCreateWaylandSurfaceKHR',
                                         'vkCreateXlibSurfaceKHR',
                                         'vkGetPhysicalDeviceXcbPresentationSupportKHR',
                                         'vkGetPhysicalDeviceWaylandPresentationSupportKHR',
                                         'vkGetPhysicalDeviceXlibPresentationSupportKHR',
                                         'vkCreateWin32SurfaceKHR',
                                         'vkGetPhysicalDeviceWin32PresentationSupportKHR',
                                         'vkCreateAndroidSurfaceKHR',
                                         'vkCreateDescriptorUpdateTemplateKHR',
                                         'vkCreateDescriptorUpdateTemplate',
                                         'vkDestroyDescriptorUpdateTemplateKHR',
                                         'vkDestroyDescriptorUpdateTemplate',
                                         'vkUpdateDescriptorSetWithTemplateKHR',
                                         'vkUpdateDescriptorSetWithTemplate',
                                         'vkCmdPushDescriptorSetKHR',
                                         'vkCmdPushDescriptorSetWithTemplateKHR',
                                         'vkAcquireXlibDisplayEXT',
                                         'vkGetRandROutputDisplayEXT',
                                         'vkCreateObjectTableNVX',
                                         'vkCmdProcessCommandsNVX',
                                         'vkCreateIndirectCommandsLayoutNVX',
                                         # TODO: VK_EXT_display_control
                                         ]

        # Validate the manually_written_hooked_funcs list
        protoFuncs = [proto.name for proto in self.cmdMembers]
        wsi_platform_manual_funcs = ['vkCreateWin32SurfaceKHR',
                                     'vkCreateXcbSurfaceKHR',
                                     'vkCreateWaylandSurfaceKHR',
                                     'vkCreateXlibSurfaceKHR',
                                     'vkCreateAndroidSurfaceKHR',
                                     'vkGetPhysicalDeviceXcbPresentationSupportKHR',
                                     'vkGetPhysicalDeviceWaylandPresentationSupportKHR',
                                     'vkGetPhysicalDeviceXlibPresentationSupportKHR',
                                     'vkGetPhysicalDeviceWin32PresentationSupportKHR']

        for func in manually_written_hooked_funcs:
            if (func not in protoFuncs) and (func not in wsi_platform_manual_funcs):
                sys.exit("Entry '%s' in manually_written_hooked_funcs list is not in the vulkan function prototypes" % func)
        # Process each of the entrypoint prototypes
        cmd_extension_dict = dict(self.cmd_extension_names)
        cmd_protect_dict = dict(self.cmd_feature_protect)
        cmd_info_dict = dict(self.cmd_info_data)
        cmd_protect_dict = dict(self.cmd_feature_protect)
        for proto in self.cmdMembers:
            extension = cmd_extension_dict[proto.name]
            cmdinfo = cmd_info_dict[proto.name]
            if 'VK_VERSION_' not in extension and extension not in approved_ext or proto.name[2:] in api_exclusions:
                trace_vk_src += '// TODO: Add support for __HOOKED_%s: Skipping for now.\n' % proto.name
                continue

            if proto.name in manually_written_hooked_funcs:
                trace_vk_src += '// __HOOKED_%s is manually written. Look in vktrace_lib_trace.cpp. Stub for proc mapping function.\n' % proto.name

            protect = cmd_protect_dict[proto.name]
            if protect is not None:
                trace_vk_src += '#ifdef %s\n' % protect

            resulttype = cmdinfo.elem.find('proto/type')
            resulttype = resulttype.text if not None else ''
            raw_packet_update_list = [] # Non-ptr elements placed directly into packet
            ptr_packet_update_list = [] # Ptr elements to be updated into packet
            return_txt = ''
            packet_size = []
            in_data_size = False # Flag when we need to capture local input size variable for in/out size
            trace_vk_src += 'VKTRACER_EXPORT VKAPI_ATTR %s VKAPI_CALL __HOOKED_%s(\n' % (resulttype, proto.name)
            for p in proto.members: # TODO : For all of the ptr types, check them for NULL and return 0 if NULL
                if p.name == '':
                    continue
                trace_vk_src += '%s,\n' % p.cdecl
                if p.ispointer and p.name not in ['pSysMem', 'pReserved']:
                    if 'pDataSize' in p.name:
                        in_data_size = True;
                elif 'pfnMsgCallback' == p.name:
                    raw_packet_update_list.append('    PFN_vkDebugReportCallbackEXT* pNonConstCallback = (PFN_vkDebugReportCallbackEXT*)&pPacket->pfnMsgCallback;')
                    raw_packet_update_list.append('    *pNonConstCallback = pfnMsgCallback;')
                elif p.isstaticarray:
                    raw_packet_update_list.append('    memcpy((void *) pPacket->%s, %s, sizeof(pPacket->%s));' % (p.name, p.name, p.name))
                else:
                    raw_packet_update_list.append('    pPacket->%s = %s;' % (p.name, p.name))
            trace_vk_src = trace_vk_src[:-2] + ')'

            if proto.name in manually_written_hooked_funcs:
                # Just declare function for manually written entrypoints. Declaration needed for proc mapping
                trace_vk_src += ';\n';
            else:
                # Get list of packet size modifiers due to ptr params
                packet_size = self.GetPacketSize(proto.members)
                ptr_packet_update_list = self.GetPacketPtrParamList(proto.members)
                # End of function declaration portion, begin function body
                trace_vk_src += ' {\n'
                if 'void' not in resulttype or '*' in resulttype:
                    trace_vk_src += '    %s result;\n' % resulttype
                    return_txt = 'result = '
                trace_vk_src += '    vktrace_trace_packet_header* pHeader;\n'
                if in_data_size:
                    trace_vk_src += '    size_t _dataSize;\n'
                trace_vk_src += '    packet_%s* pPacket = NULL;\n' % proto.name
                if proto.name == 'vkDestroyInstance' or proto.name == 'vkDestroyDevice':
                    trace_vk_src += '    dispatch_key key = get_dispatch_key(%s);\n' % proto.members[0].name
                if (0 == len(packet_size)):
                    trace_vk_src += '    CREATE_TRACE_PACKET(%s, 0);\n' % (proto.name)
                else:
                    trace_vk_src += '    CREATE_TRACE_PACKET(%s, %s);\n' % (proto.name, ' + '.join(packet_size))
                if proto.name == 'vkCreateImage':
                    trace_vk_src += '    VkImageCreateInfo replayCreateInfo = *pCreateInfo;\n'
                    trace_vk_src += '    VkImageCreateInfo trimCreateInfo = *pCreateInfo;\n'
                    trace_vk_src += '    if (g_trimEnabled) {\n'
                    trace_vk_src += '        // need to add TRANSFER_SRC usage to the image so that we can copy out of it.\n'
                    trace_vk_src += '        trimCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;\n'
                    trace_vk_src += '        pCreateInfo = &trimCreateInfo;\n'
                    trace_vk_src += '    }\n'
                elif proto.name == 'vkCreateBuffer':
                    trace_vk_src += '    VkBufferCreateInfo replayCreateInfo = *pCreateInfo;\n'
                    trace_vk_src += '    VkBufferCreateInfo trimCreateInfo = *pCreateInfo;\n'
                    trace_vk_src += '    if (g_trimEnabled) {\n'
                    trace_vk_src += '        // need to add TRANSFER_SRC usage to the buffer so that we can copy out of it.\n'
                    trace_vk_src += '        trimCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;\n'
                    trace_vk_src += '        pCreateInfo = &trimCreateInfo;\n'
                    trace_vk_src += '    }\n'
                # Call down the layer chain and get return value (if there is one)
                # Note: this logic doesn't work for CreateInstance or CreateDevice but those are handwritten
                #if extension == 'VK_EXT_debug_marker':
                #    table_txt = 'mdd(%s)->debugMarkerTable' % proto.members[0].name
                if proto.members[0].type in ['VkInstance', 'VkPhysicalDevice']:
                    table_txt = 'mid(%s)->instTable' % proto.members[0].name
                else:
                    table_txt = 'mdd(%s)->devTable' % proto.members[0].name
                paramstext = '%s' % proto.members[0].name
                for param in proto.members[1:]:
                    if param.name != '':
                        paramstext += ', %s' % param.name
                c_call = proto.name[2:] + '(' + paramstext + ')'
                trace_vk_src += '    %s%s.%s;\n' % (return_txt, table_txt, c_call)
                trace_vk_src += '    vktrace_set_packet_entrypoint_end_time(pHeader);\n'
                if proto.name == 'vkCreateImage':
                    trace_vk_src += '    if (g_trimEnabled) {\n'
                    trace_vk_src += '        // need to add TRANSFER_DST usage to the image so that we can recreate it.\n'
                    trace_vk_src += '        replayCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;\n'
                    trace_vk_src += '        pCreateInfo = &replayCreateInfo;\n'
                    trace_vk_src += '    }\n'
                elif proto.name == 'vkCreateBuffer':
                    trace_vk_src += '    if (g_trimEnabled) {\n'
                    trace_vk_src += '        // need to add TRANSFER_DST usage to the buffer so that we can recreate it.\n'
                    trace_vk_src += '        replayCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;\n'
                    trace_vk_src += '        pCreateInfo = &replayCreateInfo;\n'
                    trace_vk_src += '    }\n'
                if in_data_size:
                    trace_vk_src += '    _dataSize = (pDataSize == NULL || pData == NULL) ? 0 : *pDataSize;\n'
                trace_vk_src += '    pPacket = interpret_body_as_%s(pHeader);\n' % proto.name
                trace_vk_src += '\n'.join(raw_packet_update_list)
                trace_vk_src += '\n'
                for pp_dict in ptr_packet_update_list: # buff_ptr_indices:
                    trace_vk_src += '    %s;\n' % (pp_dict['add_txt'])
                    if '(pPacket->pCreateInfo)' in pp_dict['add_txt']:
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pCreateInfo, (void *)pCreateInfo);\n'
                    if '(pPacket->pBeginInfo)' in pp_dict['add_txt']:
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pBeginInfo, (void *)pBeginInfo);\n'
                    if '(pPacket->pAllocateInfo)' in pp_dict['add_txt']:
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pAllocateInfo, (void *)pAllocateInfo);\n'
                    if '(pPacket->pReserveSpaceInfo)' in pp_dict['add_txt']:
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pReserveSpaceInfo, (void *)pReserveSpaceInfo);\n'
                    if '(pPacket->pLimits)' in pp_dict['add_txt']:
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pLimits, (void *)pLimits);\n'
                    if ('(pPacket->pFeatures)' in pp_dict['add_txt'] and ('KHR' in pp_dict['add_txt'] or ('NVX' in pp_dict['add_txt']))):
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pFeatures, (void *)pFeatures);\n'
                    if ('(pPacket->pSurfaceInfo)' in pp_dict['add_txt'] and ('2KHR' in pp_dict['add_txt'])):
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pSurfaceInfo, (void *)pSurfaceInfo);\n'
                    if ('(pPacket->pInfo)' in pp_dict['add_txt'] and ('2KHR' in pp_dict['add_txt'])):
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pInfo, (void *)pInfo);\n'
                    elif ('(pPacket->pInfo)' in pp_dict['add_txt'] and ('RequirementsInfo2' in pp_dict['add_txt'])):
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pInfo, (void *)pInfo);\n'
                    if ('(pPacket->pMemoryRequirements)' in pp_dict['add_txt'] and ('2KHR' in pp_dict['add_txt'])):
                        trace_vk_src += '    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void *)pPacket->pMemoryRequirements, (void *)pMemoryRequirements);\n'
                if 'void' not in resulttype or '*' in resulttype:
                    trace_vk_src += '    pPacket->result = result;\n'
                for pp_dict in ptr_packet_update_list:
                    if ('DeviceCreateInfo' not in proto.members[pp_dict['index']].type):
                        trace_vk_src += '    %s;\n' % (pp_dict['finalize_txt'])
                trace_vk_src += '    if (!g_trimEnabled) {\n'
                # All buffers should be finalized by now, and the trace packet can be finished (which sends it over the socket)
                trace_vk_src += '        FINISH_TRACE_PACKET();\n'

                # Else half of g_trimEnabled conditional
                # Since packet wasn't sent to trace file, it either needs to be associated with an object, or deleted.
                trace_vk_src += '    } else {\n'
                trace_vk_src += '        vktrace_finalize_trace_packet(pHeader);\n'
                trim_instructions = self.GenerateTrimStateTrackingInstructions(proto)
                if trim_instructions is not None:
                    trace_vk_src += trim_instructions
                else:
                    trace_vk_src += '        if (g_trimIsInTrim) {\n'
                    intrim_instructions = self.GenerateTrimRecordingInstructions(proto)
                    if intrim_instructions is not None:
                        trace_vk_src += intrim_instructions
                    else:
                        trace_vk_src += '            trim::write_packet(pHeader);\n'
                    trace_vk_src += '        } else {\n'
                    trace_vk_src += '            vktrace_delete_trace_packet(&pHeader);\n'
                    trace_vk_src += '        }\n'
                trace_vk_src += '    }\n'

                # Clean up instance or device data if needed
                if proto.name == "vkDestroyInstance":
                    trace_vk_src += '    g_instanceDataMap.erase(key);\n'
                elif proto.name == "vkDestroyDevice":
                    trace_vk_src += '    g_deviceDataMap.erase(key);\n'

                # Return result if needed
                if 'void' not in resulttype or '*' in resulttype:
                    trace_vk_src += '    return result;\n'
                trace_vk_src += '}\n'

            # Close off protect preprocessor if necessary
            if protect is not None:
                trace_vk_src += '#endif // %s\n' % protect

        # Add name to intercept proc mapping functions
        trace_vk_src += 'PFN_vkVoidFunction layer_intercept_instance_proc(const char* name) {\n'
        trace_vk_src += '    if (!name || name[0] != \'v\' || name[1] != \'k\') return NULL;\n'
        trace_vk_src += '    name += 2;\n'

        for cmd in self.cmdMembers:
            cmdname = cmd.name[2:]
            if not isSupportedCmd(cmd, cmd_extension_dict) or not isInstanceCmd(cmd):
                continue
            protect = cmd_protect_dict[cmd.name]
            if protect is not None:
                trace_vk_src += '#ifdef %s\n' % protect
            trace_vk_src += '   if (!strcmp(name, "%s")) return (PFN_vkVoidFunction)__HOOKED_vk%s;\n' % (cmdname, cmdname)
            if protect is not None:
                trace_vk_src += '#endif // %s\n' % protect

        trace_vk_src += '    return NULL;\n'
        trace_vk_src += '}\n\n'

        trace_vk_src += 'PFN_vkVoidFunction layer_intercept_proc(const char* name) {\n'
        trace_vk_src += '    if (!name || name[0] != \'v\' || name[1] != \'k\') return NULL;\n'
        trace_vk_src += '    name += 2;\n'

        for cmd in self.cmdMembers:
            cmdname = cmd.name[2:]
            if not isSupportedCmd(cmd, cmd_extension_dict) or isInstanceCmd(cmd):
                continue
            protect = cmd_protect_dict[cmd.name]
            if protect is not None:
                trace_vk_src += '#ifdef %s\n' % protect
            trace_vk_src += '   if (!strcmp(name, "%s")) return (PFN_vkVoidFunction)__HOOKED_vk%s;\n' % (cmdname, cmdname)
            if protect is not None:
                trace_vk_src += '#endif // %s\n' % protect

        trace_vk_src += '    return NULL;\n'
        trace_vk_src += '}\n\n'

        return trace_vk_src
    #
    # Construct vktrace packets header file
    def GenerateTraceVkPacketsHeader(self):
        trace_pkt_hdr  = ''
        trace_pkt_hdr += '#pragma once\n'
        trace_pkt_hdr += '#include "vulkan/vulkan.h"\n'
        trace_pkt_hdr += '#include "vktrace_trace_packet_utils.h"\n'
        trace_pkt_hdr += '\n'

        # Custom txt for given function and parameter.  First check if param is NULL, then insert txt if not
        # First some common code used by both CmdWaitEvents & CmdPipelineBarrier
        create_rp_interp = ['VkRenderPassCreateInfo* pInfo = (VkRenderPassCreateInfo*)pPacket->pCreateInfo;\n',
                            'uint32_t i = 0;\n',
                            'VkAttachmentDescription **ppAD = (VkAttachmentDescription **)&(pInfo->pAttachments);\n',
                            '*ppAD = (VkAttachmentDescription*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pAttachments);\n',
                            'VkSubpassDescription** ppSP = (VkSubpassDescription**)&(pInfo->pSubpasses);\n',
                            '*ppSP = (VkSubpassDescription*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pSubpasses);\n',
                            'for (i=0; i<pInfo->subpassCount; i++) {\n',
                            '    VkAttachmentReference** pAR = (VkAttachmentReference**)&(pInfo->pSubpasses[i].pInputAttachments);\n',
                            '    *pAR = (VkAttachmentReference*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pSubpasses[i].pInputAttachments);\n',
                            '    pAR = (VkAttachmentReference**)&(pInfo->pSubpasses[i].pColorAttachments);\n',
                            '    *pAR = (VkAttachmentReference*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pSubpasses[i].pColorAttachments);\n',
                            '    pAR = (VkAttachmentReference**)&(pInfo->pSubpasses[i].pResolveAttachments);\n',
                            '    *pAR = (VkAttachmentReference*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pSubpasses[i].pResolveAttachments);\n',
                            '    pAR = (VkAttachmentReference**)&(pInfo->pSubpasses[i].pDepthStencilAttachment);\n',
                            '    *pAR = (VkAttachmentReference*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pSubpasses[i].pDepthStencilAttachment);\n',
                            '    pAR = (VkAttachmentReference**)&(pInfo->pSubpasses[i].pPreserveAttachments);\n',
                            '    *pAR = (VkAttachmentReference*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pSubpasses[i].pPreserveAttachments);\n',
                            '}\n',
                            'VkSubpassDependency** ppSD = (VkSubpassDependency**)&(pInfo->pDependencies);\n',
                            '*ppSD = (VkSubpassDependency*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pDependencies);\n',
                            ]
        create_gfx_pipe = ['uint32_t i;\n',
                           'uint32_t j;\n',
                           'for (i=0; i<pPacket->createInfoCount; i++) {\n',
                            'if (pPacket->pCreateInfos[i].sType == VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO) {\n',
                            '// need to make a non-const pointer to the pointer so that we can properly change the original pointer to the interpretted one\n',
                            'VkGraphicsPipelineCreateInfo* pNonConst = (VkGraphicsPipelineCreateInfo*)&(pPacket->pCreateInfos[i]);\n',
                            '// shader stages array\n',
                            'pNonConst->pStages = (VkPipelineShaderStageCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pStages);\n',
                            'for (j = 0; j < pPacket->pCreateInfos[i].stageCount; j++) {\n',
                            '    interpret_VkPipelineShaderStageCreateInfo(pHeader, (VkPipelineShaderStageCreateInfo*)&pPacket->pCreateInfos[i].pStages[j]);\n',
                            '}\n',
                            '// Vertex Input State\n',
                            'pNonConst->pVertexInputState = (VkPipelineVertexInputStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pVertexInputState);\n',
                            'VkPipelineVertexInputStateCreateInfo* pNonConstVIState = (VkPipelineVertexInputStateCreateInfo*)pNonConst->pVertexInputState;\n',
                            'if (pNonConstVIState) {\n',
                            '    pNonConstVIState->pVertexBindingDescriptions = (const VkVertexInputBindingDescription*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pVertexInputState->pVertexBindingDescriptions);\n',
                            '    pNonConstVIState->pVertexAttributeDescriptions = (const VkVertexInputAttributeDescription*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pVertexInputState->pVertexAttributeDescriptions);\n',
                            '}\n',
                            '// Input Assembly State\n',
                            'pNonConst->pInputAssemblyState = (const VkPipelineInputAssemblyStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pInputAssemblyState);\n',
                            '// Tesselation State\n',
                            'pNonConst->pTessellationState = (const VkPipelineTessellationStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pTessellationState);\n',
                            '// Viewport State\n',
                            'pNonConst->pViewportState = (const VkPipelineViewportStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pViewportState);\n',
                            '// Raster State\n',
                            'pNonConst->pRasterizationState = (const VkPipelineRasterizationStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pRasterizationState);\n',


                            '// MultiSample State\n',
                            'pNonConst->pMultisampleState = (const VkPipelineMultisampleStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pMultisampleState);\n',
                            '// DepthStencil State\n',
                            'pNonConst->pDepthStencilState = (const VkPipelineDepthStencilStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pDepthStencilState);\n',
                            '// DynamicState State\n',
                            'pNonConst->pDynamicState = (const VkPipelineDynamicStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pDynamicState);\n',
                            'VkPipelineDynamicStateCreateInfo* pNonConstDyState = (VkPipelineDynamicStateCreateInfo*)pNonConst->pDynamicState;\n',
                            'if (pNonConstDyState) {\n',
                            '    pNonConstDyState->pDynamicStates = (const VkDynamicState*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pDynamicState->pDynamicStates);\n',
                            '}\n',

                            '// ColorBuffer State\n',
                            'pNonConst->pColorBlendState = (const VkPipelineColorBlendStateCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pColorBlendState);\n',
                            'VkPipelineColorBlendStateCreateInfo* pNonConstCbState = (VkPipelineColorBlendStateCreateInfo*)pNonConst->pColorBlendState;\n',
                            'if (pNonConstCbState)\n',
                            '    pNonConstCbState->pAttachments = (const VkPipelineColorBlendAttachmentState*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pColorBlendState->pAttachments);\n',
                            '} else {\n',
                            '    // This is unexpected.\n',
                            '    vktrace_LogError("CreateGraphicsPipelines must have CreateInfo stype of VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO.");\n',
                            '    pPacket->header = NULL;\n',
                            '}\n',
                            '}\n']

        # TODO : This code is now too large and complex, need to make codegen smarter for pointers embedded in struct params to handle those cases automatically
        custom_case_dict = { 'CreateRenderPass' : {'param': 'pCreateInfo', 'txt': create_rp_interp},
                             'CreatePipelineCache' : {'param': 'pCreateInfo', 'txt': [
                                                       '((VkPipelineCacheCreateInfo *)pPacket->pCreateInfo)->pInitialData = (const void*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pInitialData);']},
                             'CreatePipelineLayout' : {'param': 'pCreateInfo', 'txt': ['VkPipelineLayoutCreateInfo* pInfo = (VkPipelineLayoutCreateInfo*)pPacket->pCreateInfo;\n',
                                                       'pInfo->pSetLayouts = (VkDescriptorSetLayout*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pSetLayouts);\n',
                                                       'pInfo->pPushConstantRanges = (VkPushConstantRange*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pPushConstantRanges);']},
                             'CreateDescriptorPool' : {'param': 'pCreateInfo', 'txt': ['VkDescriptorPoolCreateInfo* pInfo = (VkDescriptorPoolCreateInfo*)pPacket->pCreateInfo;\n',
                                                       'pInfo->pPoolSizes = (VkDescriptorPoolSize*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pPoolSizes);']},
                             'BeginCommandBuffer' : {'param': 'pBeginInfo', 'txt': [
                                                                                         'VkCommandBufferBeginInfo* pInfo = (VkCommandBufferBeginInfo*) pPacket->pBeginInfo;\n',
                                                       'pInfo->pInheritanceInfo = (VkCommandBufferInheritanceInfo*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pBeginInfo->pInheritanceInfo);\n']},
                             'AllocateDescriptorSets' : {'param': 'pAllocateInfo', 'txt':
                                                                               ['VkDescriptorSetLayout **ppDescSetLayout = (VkDescriptorSetLayout **) &pPacket->pAllocateInfo->pSetLayouts;\n'
                                                                                '        *ppDescSetLayout = (VkDescriptorSetLayout *) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pAllocateInfo->pSetLayouts));']},
                             'CmdPushDescriptorSetKHR' : {'param': 'pDescriptorWrites', 'txt':
                                                                               [ 'uint32_t i;\n',
                                                                                 'for (i = 0; i < pPacket->descriptorWriteCount; i++) {\n',
                                                                                 '    vktrace_interpret_pnext_pointers(pPacket->header, (void *)&pPacket->pDescriptorWrites[i]);\n',
                                                                                 '    switch (pPacket->pDescriptorWrites[i].descriptorType) {\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_SAMPLER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {\n',
                                                                                 '            VkDescriptorImageInfo** ppImageInfo = (VkDescriptorImageInfo**)&pPacket->pDescriptorWrites[i].pImageInfo;\n',
                                                                                 '            *ppImageInfo = (VkDescriptorImageInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pImageInfo);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {\n',
                                                                                 '            VkBufferView** ppTexelBufferView = (VkBufferView**)&pPacket->pDescriptorWrites[i].pTexelBufferView;\n',
                                                                                 '            *ppTexelBufferView = (VkBufferView*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pTexelBufferView);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {\n',
                                                                                 '            VkDescriptorBufferInfo** ppBufferInfo = (VkDescriptorBufferInfo**)&pPacket->pDescriptorWrites[i].pBufferInfo;\n',
                                                                                 '            *ppBufferInfo = (VkDescriptorBufferInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pBufferInfo);\n',
                                                                                 '        }\n',
                                                                                 '        break;\n',
                                                                                 '    default:\n',
                                                                                 '        break;\n',
                                                                                 '    }\n',
                                                                                 '}'
                                                                               ]},
                             'UpdateDescriptorSets' : {'param': 'pDescriptorWrites', 'txt':
                                                                               [ 'uint32_t i;\n',
                                                                                 'for (i = 0; i < pPacket->descriptorWriteCount; i++) {\n',
                                                                                 '    vktrace_interpret_pnext_pointers(pPacket->header, (void *)&pPacket->pDescriptorWrites[i]);\n',
                                                                                 '    switch (pPacket->pDescriptorWrites[i].descriptorType) {\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_SAMPLER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {\n',
                                                                                 '            VkDescriptorImageInfo** ppImageInfo = (VkDescriptorImageInfo**)&pPacket->pDescriptorWrites[i].pImageInfo;\n',
                                                                                 '            *ppImageInfo = (VkDescriptorImageInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pImageInfo);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {\n',
                                                                                 '            VkBufferView** ppTexelBufferView = (VkBufferView**)&pPacket->pDescriptorWrites[i].pTexelBufferView;\n',
                                                                                 '            *ppTexelBufferView = (VkBufferView*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pTexelBufferView);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:\n',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {\n',
                                                                                 '            VkDescriptorBufferInfo** ppBufferInfo = (VkDescriptorBufferInfo**)&pPacket->pDescriptorWrites[i].pBufferInfo;\n',
                                                                                 '            *ppBufferInfo = (VkDescriptorBufferInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pBufferInfo);\n',
                                                                                 '        }\n',
                                                                                 '        break;\n',
                                                                                 '    default:\n',
                                                                                 '        break;\n',
                                                                                 '    }\n',
                                                                                 '}\n',
                                                                                 'for (i = 0; i < pPacket->descriptorCopyCount; i++) {\n',
                                                                                 '    vktrace_interpret_pnext_pointers(pPacket->header, (void *)&pPacket->pDescriptorCopies[i]);\n',
                                                                                 '}'
                                                                               ]},
                             'QueueSubmit' : {'param': 'pSubmits', 'txt':
                                                                               [ 'uint32_t i;\n',
                                                                                 'for (i = 0; i < pPacket->submitCount; i++) {\n',
                                                                                 '   VkCommandBuffer** ppCBs = (VkCommandBuffer**)&pPacket->pSubmits[i].pCommandBuffers;\n',
                                                                                 '   *ppCBs = (VkCommandBuffer*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pSubmits[i].pCommandBuffers);\n',
                                                                                 '   VkSemaphore** ppSems = (VkSemaphore**)&pPacket->pSubmits[i].pWaitSemaphores;\n',
                                                                                 '   *ppSems = (VkSemaphore*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pSubmits[i].pWaitSemaphores);\n',
                                                                                 '   ppSems = (VkSemaphore**)&pPacket->pSubmits[i].pSignalSemaphores;\n',
                                                                                 '   *ppSems = (VkSemaphore*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pSubmits[i].pSignalSemaphores);\n',
                                                                                 '   VkPipelineStageFlags** ppStageMask = (VkPipelineStageFlags**)&pPacket->pSubmits[i].pWaitDstStageMask;\n',
                                                                                 '   *ppStageMask = (VkPipelineStageFlags*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pSubmits[i].pWaitDstStageMask);\n',
                                                                                 '   vktrace_interpret_pnext_pointers(pHeader, (void *)&pPacket->pSubmits[i]);\n'
                                                                                 '}'
                                                                               ]},
                             'CreateGraphicsPipelines' : {'param': 'pCreateInfos', 'txt': create_gfx_pipe},
                             'CreateComputePipeline' : {'param': 'pCreateInfo', 'txt': ['interpret_VkPipelineShaderStageCreateInfo(pHeader, (VkPipelineShaderStageCreateInfo*)(&pPacket->pCreateInfo->cs));']},
                             'CreateFramebuffer' : {'param': 'pCreateInfo', 'txt': ['VkImageView** ppAV = (VkImageView**)&(pPacket->pCreateInfo->pAttachments);\n',
                                                                                    '*ppAV = (VkImageView*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pCreateInfo->pAttachments));']},
                             'CmdBeginRenderPass' : {'param': 'pRenderPassBegin', 'txt': ['VkClearValue** ppCV = (VkClearValue**)&(pPacket->pRenderPassBegin->pClearValues);\n',
                                                                                          '*ppCV = (VkClearValue*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pRenderPassBegin->pClearValues));']},
                             'CreateShaderModule' : {'param': 'pCreateInfo', 'txt': ['void** ppCode = (void**)&(pPacket->pCreateInfo->pCode);\n',
                                                                                     '*ppCode = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pCode);']},
                             'CreateImage' : {'param': 'pCreateInfo', 'txt': ['uint32_t** ppQueueFamilyIndices = (uint32_t**)&(pPacket->pCreateInfo->pQueueFamilyIndices);\n',
                                                                              '*ppQueueFamilyIndices = (uint32_t*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pQueueFamilyIndices);',
                                                                              ]},
                             'CreateBuffer' : {'param': 'pCreateInfo', 'txt': ['uint32_t** ppQueueFamilyIndices = (uint32_t**)&(pPacket->pCreateInfo->pQueueFamilyIndices);\n',
                                                                              '*ppQueueFamilyIndices = (uint32_t*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pQueueFamilyIndices);',
                                                                              ]},
                             'QueuePresentKHR' : {'param': 'pPresentInfo', 'txt': ['VkSwapchainKHR **ppSC = (VkSwapchainKHR **)& pPacket->pPresentInfo->pSwapchains;\n',
                                                                                   '*ppSC = (VkSwapchainKHR*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pPresentInfo->pSwapchains));\n',
                                                                                   'VkSemaphore **ppS = (VkSemaphore **) &pPacket->pPresentInfo->pWaitSemaphores;\n',
                                                                                   '*ppS = (VkSemaphore *) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pPresentInfo->pWaitSemaphores));\n',
                                                                                   'uint32_t **ppII = (uint32_t **) &pPacket->pPresentInfo->pImageIndices;\n',
                                                                                   '*ppII = (uint32_t*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pPresentInfo->pImageIndices));\n',
                                                                                   'if (pPacket->pPresentInfo->pResults != NULL) {\n',
                                                                                   '    VkResult **ppR = (VkResult **) &pPacket->pPresentInfo->pResults;\n',
                                                                                   '    *ppR = (VkResult*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pPresentInfo->pResults));\n',
                                                                                   '}']},
                             'CreateSwapchainKHR' : {'param': 'pCreateInfo', 'txt': ['uint32_t **ppQFI = (uint32_t**)&pPacket->pCreateInfo->pQueueFamilyIndices;\n',
                                                     '(*ppQFI) = (uint32_t*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pCreateInfo->pQueueFamilyIndices));']},

        }
        if_body = []
        trace_pkt_hdr += 'typedef struct packet_vkApiVersion {\n'
        trace_pkt_hdr += '    vktrace_trace_packet_header* header;\n'
        trace_pkt_hdr += '    uint32_t version;\n'
        trace_pkt_hdr += '} packet_vkApiVersion;\n\n'
        trace_pkt_hdr += 'static packet_vkApiVersion* interpret_body_as_vkApiVersion(vktrace_trace_packet_header* pHeader) {\n'
        trace_pkt_hdr += '    packet_vkApiVersion* pPacket = (packet_vkApiVersion*)pHeader->pBody;\n'
        trace_pkt_hdr += '    pPacket->header = pHeader;\n'
        trace_pkt_hdr += '    return pPacket;\n'
        trace_pkt_hdr += '}\n\n'
        cmd_info_dict = dict(self.cmd_info_data)
        cmd_protect_dict = dict(self.cmd_feature_protect)
        cmd_extension_dict = dict(self.cmd_extension_names)
        for proto in self.cmdMembers:
            extension = cmd_extension_dict[proto.name]
            if 'VK_VERSION_' not in extension and extension not in approved_ext:
                continue
            novk_name = proto.name[2:]
            if novk_name not in api_exclusions:
                cmdinfo = cmd_info_dict[proto.name]
                protect = cmd_protect_dict[proto.name]
                # TODO: Enable ifdef protections for extension?
                #if protect is not None:
                #    trace_pkt_hdr += '#ifdef %s\n' % protect
                trace_pkt_hdr += 'typedef struct packet_%s {\n' % proto.name
                trace_pkt_hdr += '    vktrace_trace_packet_header* header;\n'
                for p in proto.members:
                    if p.name != '':
                        trace_pkt_hdr += '%s;\n' % p.cdecl
                if 'UnmapMemory' in proto.name:
                    trace_pkt_hdr += '    void* pData;\n'
                elif 'FlushMappedMemoryRanges' in proto.name or 'InvalidateMappedMemoryRanges' in proto.name:
                    trace_pkt_hdr += '    void** ppData;\n'
                resulttype = cmdinfo.elem.find('proto/type')
                if resulttype != None and resulttype.text != 'void':
                    trace_pkt_hdr += '    %s result;\n' % resulttype.text
                trace_pkt_hdr += '} packet_%s;\n\n' % proto.name
                trace_pkt_hdr += 'static packet_%s* interpret_body_as_%s(vktrace_trace_packet_header* pHeader) {\n' % (proto.name, proto.name)
                trace_pkt_hdr += '    packet_%s* pPacket = (packet_%s*)pHeader->pBody;\n' % (proto.name, proto.name)
                trace_pkt_hdr += '    pPacket->header = pHeader;\n'
                for p in proto.members:
                    if p.name != '' and p.ispointer:
                        if 'DeviceCreateInfo' in p.type:
                            trace_pkt_hdr += '    pPacket->%s = interpret_VkDeviceCreateInfo(pHeader, (intptr_t)pPacket->%s);\n' % (p.name, p.name)
                        elif 'InstanceCreateInfo' in p.type:
                            trace_pkt_hdr += '    pPacket->%s = interpret_VkInstanceCreateInfo(pHeader, (intptr_t)pPacket->%s);\n' % (p.name, p.name)
                        else:
                            cast = p.cdecl[4:].rsplit(' ', 1)[0].rstrip()
                            trace_pkt_hdr += '    pPacket->%s = (%s)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->%s);\n' % (p.name, cast, p.name)
                        # TODO : Generalize this custom code to kill dict data struct above.
                        #  Really the point of this block is to catch params w/ embedded ptrs to structs and chains of structs
                        if novk_name in custom_case_dict and p.name == custom_case_dict[novk_name]['param']:
                            trace_pkt_hdr += '    if (pPacket->%s != NULL) {\n' % custom_case_dict[novk_name]['param']
                            trace_pkt_hdr += '        %s\n' % "        ".join(custom_case_dict[novk_name]['txt'])
                            trace_pkt_hdr += '    }\n'
                        if ((p.name in ['pCreateInfo', 'pBeginInfo', 'pAllocateInfo','pReserveSpaceInfo','pLimits','pExternalBufferInfo','pExternalBufferProperties',
                                        'pGetFdInfo','pMemoryFdProperties','pExternalSemaphoreProperties','pExternalSemaphoreInfo','pImportSemaphoreFdInfo',
                                        'pExternalFenceInfo','pExternalFenceProperties','pSurfaceInfo','pTagInfo','pNameInfo','pMarkerInfo',
                                        'pDeviceGroupPresentCapabilities','pAcquireInfo','pPhysicalDeviceGroupProperties','pDisplayPowerInfo','pDeviceEventInfo',
                                        'pDisplayEventInfo','pMetadata','pRenderPassBegin','pPresentInfo'])
                            or (p.name in ['pFeatures'] and 'nvx' in p.type.lower())
                            or (p.name in ['pFeatures', 'pProperties','pFormatProperties','pImageFormatInfo','pImageFormatProperties','pQueueFamilyProperties',
                                           'pMemoryProperties','pFormatInfo','pSurfaceFormats','pMemoryRequirements','pInfo',
                                           'pSparseMemoryRequirements','pSurfaceCapabilities'] and '2khr' in p.type.lower())
                            or (p.name in ['pSurfaceCapabilities'] and '2ext' in p.type.lower())):
                            trace_pkt_hdr += '    if (pPacket->%s != NULL) {\n' % p.name
                            trace_pkt_hdr += '        vktrace_interpret_pnext_pointers(pHeader, (void *)pPacket->%s);\n' % p.name
                            trace_pkt_hdr += '    }\n'
                if 'UnmapMemory' in proto.name:
                    trace_pkt_hdr += '    pPacket->pData = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pData);\n'
                elif 'FlushMappedMemoryRanges' in proto.name or 'InvalidateMappedMemoryRanges' in proto.name:
                            trace_pkt_hdr += '    pPacket->ppData = (void**)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->ppData);\n'
                            trace_pkt_hdr += '    if (pPacket->ppData != NULL) {\n'
                            trace_pkt_hdr += '        uint32_t i = 0;\n'
                            trace_pkt_hdr += '        for (i = 0; i < pPacket->memoryRangeCount; i++) {\n'
                            trace_pkt_hdr += '            pPacket->ppData[i] = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->ppData[i]);\n'
                            trace_pkt_hdr += '        }\n'
                            trace_pkt_hdr += '    }\n'
                trace_pkt_hdr += '    return pPacket;\n'
                trace_pkt_hdr += '}\n'
                # TODO: Enable ifdef protections for extension?
                #if protect is not None:
                #   trace_pkt_hdr += '#endif // %s\n' % protect
                trace_pkt_hdr += '\n'
        return trace_pkt_hdr

    #
    # Create a vktrace file and return it as a string
    def OutputDestFile(self):
        if self.vktrace_file_type == 'vkreplay_objmapper_header':
            return self.GenerateReplayObjmapperHeader()
        elif self.vktrace_file_type == 'vkreplay_replay_gen_source':
            return self.GenerateReplayGenSource()
        elif self.vktrace_file_type == 'vktrace_packet_id_header':
            return self.GenerateTracePacketIdHeader()
        elif self.vktrace_file_type == 'vktrace_vk_header':
            return self.GenerateTraceVkHeader()
        elif self.vktrace_file_type == 'vktrace_vk_source':
            return self.GenerateTraceVkSource()
        elif self.vktrace_file_type == 'vktrace_vk_packets_header':
            return self.GenerateTraceVkPacketsHeader()
        else:
            return 'Bad VkTrace File Generator Option %s' % self.vktrace_file_type

