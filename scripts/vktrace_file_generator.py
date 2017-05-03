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

# TODO: Finish generated file conversion, reenable these extensions
# THESE FUNCTIONS WILL BE ADDED IN AFTER THE SCRIPT CONVERSION EFFORT IS COMPLETE
temporary_script_porting_exclusions = ['vkGetPhysicalDeviceFeatures2KHR',
                                       'vkGetPhysicalDeviceProperties2KHR',
                                       'vkGetPhysicalDeviceFormatProperties2KHR',
                                       'vkGetPhysicalDeviceImageFormatProperties2KHR',
                                       'vkGetPhysicalDeviceQueueFamilyProperties2KHR',
                                       'vkGetPhysicalDeviceMemoryProperties2KHR',
                                       'vkGetPhysicalDeviceSparseImageFormatProperties2KHR',
                                       'vkTrimCommandPoolKHR',
                                       'vkCmdPushDescriptorSetKHR',
                                       'vkCreateDescriptorUpdateTemplateKHR',
                                       'vkDestroyDescriptorUpdateTemplateKHR',
                                       'vkUpdateDescriptorSetWithTemplateKHR',
                                       'vkCmdPushDescriptorSetWithTemplateKHR',
                                       'vkGetDeviceGroupPeerMemoryFeaturesKHX',
                                       'vkBindBufferMemory2KHX',
                                       'vkBindImageMemory2KHX',
                                       'vkCmdSetDeviceMaskKHX',
                                       'vkGetDeviceGroupPresentCapabilitiesKHX',
                                       'vkGetDeviceGroupSurfacePresentModesKHX',
                                       'vkAcquireNextImage2KHX',
                                       'vkCmdDispatchBaseKHX',
                                       'vkGetPhysicalDevicePresentRectanglesKHX',
                                       'vkCreateViSurfaceNN',
                                       'vkEnumeratePhysicalDeviceGroupsKHX',
                                       'vkGetPhysicalDeviceExternalBufferPropertiesKHX',
                                       'vkGetMemoryWin32HandleKHX',
                                       'vkGetMemoryWin32HandlePropertiesKHX',
                                       'vkGetMemoryFdKHX',
                                       'vkGetMemoryFdPropertiesKHX',
                                       'vkGetPhysicalDeviceExternalSemaphorePropertiesKHX',
                                       'vkImportSemaphoreWin32HandleKHX',
                                       'vkGetSemaphoreWin32HandleKHX',
                                       'vkImportSemaphoreFdKHX',
                                       'vkGetSemaphoreFdKHX',
                                       'vkCmdProcessCommandsNVX',
                                       'vkCmdReserveSpaceForCommandsNVX',
                                       'vkCreateIndirectCommandsLayoutNVX',
                                       'vkDestroyIndirectCommandsLayoutNVX',
                                       'vkCreateObjectTableNVX',
                                       'vkDestroyObjectTableNVX',
                                       'vkRegisterObjectsNVX',
                                       'vkUnregisterObjectsNVX',
                                       'vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX',
                                       'vkCmdSetViewportWScalingNV',
                                       'vkReleaseDisplayEXT',
                                       'vkAcquireXlibDisplayEXT',
                                       'vkGetRandROutputDisplayEXT',
                                       'vkGetPhysicalDeviceSurfaceCapabilities2EXT',
                                       'vkDisplayPowerControlEXT',
                                       'vkRegisterDeviceEventEXT',
                                       'vkRegisterDisplayEventEXT',
                                       'vkGetSwapchainCounterEXT',
                                       'vkGetRefreshCycleDurationGOOGLE',
                                       'vkGetPastPresentationTimingGOOGLE',
                                       'vkCmdSetDiscardRectangleEXT',
                                       'vkSetHdrMetadataEXT',
                                       'vkCreateIOSSurfaceMVK',
                                       'vkCreateMacOSSurfaceMVK',
                                       ]

api_exclusions = ['CreateWaylandSurfaceKHR',
                  'CreateMirSurfaceKHR',
                  'GetPhysicalDeviceWaylandPresentationSupportKHR',
                  'GetPhysicalDeviceMirPresentationSupportKHR',
                  'GetPhysicalDeviceDisplayPropertiesKHR',
                  'GetPhysicalDeviceDisplayPlanePropertiesKHR',
                  'GetDisplayPlaneSupportedDisplaysKHR',
                  'GetDisplayModePropertiesKHR',
                  'CreateDisplayModeKHR',
                  'GetDisplayPlaneCapabilitiesKHR',
                  'CreateDisplayPlaneSurfaceKHR',
                  ]

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
                 sortProcedure = regSortFeatures,
                 prefixText = "",
                 genFuncPointers = True,
                 protectFile = True,
                 protectFeature = True,
                 protectProto = None,
                 protectProtoStr = None,
                 apicall = '',
                 apientry = '',
                 apientryp = '',
                 alignFuncParam = 0,
                 library_name = '',
                 vktrace_file_type = ''):
        GeneratorOptions.__init__(self, filename, directory, apiname, profile,
                                  versions, emitversions, defaultExtensions,
                                  addExtensions, removeExtensions, sortProcedure)
        self.prefixText       = prefixText
        self.genFuncPointers  = genFuncPointers
        self.protectFile      = protectFile
        self.protectFeature   = protectFeature
        self.protectProto     = protectProto
        self.protectProtoStr  = protectProtoStr
        self.apicall          = apicall
        self.apientry         = apientry
        self.apientryp        = apientryp
        self.alignFuncParam   = alignFuncParam
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
        self.CommandParam = namedtuple('CommandParam', ['type', 'name', 'ispointer', 'isstaticarray', 'isconst', 'iscount', 'len', 'cdecl', 'feature_protect'])
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
        copyright += ' * Author: Tobin Ehlis <tobin@lunarg.com>\n'
        copyright += ' * Author: Peter Lohrmann <peterl@valvesoftware.com>\n'
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
    def genType(self, typeinfo, name):
        OutputGenerator.genType(self, typeinfo, name)
        typeElem = typeinfo.elem
        # If the type is a struct type, traverse the imbedded <member> tags generating a structure.
        # Otherwise, emit the tag text.
        category = typeElem.get('category')
        if category == 'handle':
            self.object_types.append(name)
        elif (category == 'struct' or category == 'union'):
            self.structNames.append(name)
            self.genStruct(typeinfo, name)
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
    def genCmd(self, cmdinfo, cmdname):
        # Add struct-member type information to command parameter information
        OutputGenerator.genCmd(self, cmdinfo, cmdname)
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
            membersInfo.append(self.CommandParam(type=type,
                                                 name=name,
                                                 ispointer=ispointer,
                                                 isstaticarray=isstaticarray,
                                                 isconst=isconst,
                                                 iscount=iscount,
                                                 len=len,
                                                 cdecl=cdecl,
                                                 feature_protect=self.featureExtraProtect))
        self.cmdMembers.append(self.CmdMemberData(name=cmdname, members=membersInfo))
        self.cmd_info_data.append(self.CmdInfoData(name=cmdname, cmdinfo=cmdinfo))
        self.cmd_extension_names.append(self.cmd_extension_data(name=cmdname, extension_name=self.current_feature_name))
        self.cmd_feature_protect.append(self.CmdExtraProtect(name=cmdname, extra_protect=self.featureExtraProtect))
    #
    # Generate local ready-access data describing Vulkan structures and unions from the XML metadata
    def genStruct(self, typeinfo, typeName):
        OutputGenerator.genStruct(self, typeinfo, typeName)
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
            membersInfo.append(self.CommandParam(type=type,
                                                 name=name,
                                                 ispointer=self.paramIsPointer(member),
                                                 isstaticarray=isstaticarray,
                                                 isconst=True if 'const' in cdecl else False,
                                                 iscount=True if name in lens else False,
                                                 len=self.getLen(member),
                                                 cdecl=cdecl,
                                                 feature_protect=self.featureExtraProtect))
        self.structMembers.append(self.StructMemberData(name=typeName, members=membersInfo, ifdef_protect=self.featureExtraProtect))
    #
    def beginFeature(self, interface, emit):
        # Start processing in superclass
        OutputGenerator.beginFeature(self, interface, emit)
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
    # Construct vkreplay func pointer header file
    def GenerateReplayFuncptrHeader(self):
        replay_funcptr_header  = '\n'
        replay_funcptr_header += 'struct vkFuncs {'
        replay_funcptr_header += '    void init_funcs(void * libHandle);'
        replay_funcptr_header += '    void *m_libHandle;\n'
        cmd_member_dict = dict(self.cmdMembers)
        cmd_info_dict = dict(self.cmd_info_data)
        cmd_protect_dict = dict(self.cmd_feature_protect)
        for api in self.cmdMembers:
            cmdname = api.name
            cmdinfo = cmd_info_dict[api.name]
            protect = cmd_protect_dict[cmdname]
            decl = self.makeCDecls(cmdinfo.elem)[1]
            typedef = decl.replace('VKAPI_PTR *PFN_', 'VKAPI_PTR *type_')
            if protect is not None:
                replay_funcptr_header += '#ifdef %s\n' % protect
            replay_funcptr_header += '    %s\n' % typedef
            replay_funcptr_header += '    type_%s real_%s;\n' % (cmdname, cmdname)
            if protect is not None:
                replay_funcptr_header += '#endif // %s\n' % protect
            replay_funcptr_header += '\n'
        replay_funcptr_header += '};\n'
        return replay_funcptr_header
    #
    # Construct vkreplay replay gen source file
    def GenerateReplayGenSource(self):
        cmd_member_dict = dict(self.cmdMembers)
        cmd_info_dict = dict(self.cmd_info_data)
        cmd_protect_dict = dict(self.cmd_feature_protect)
        cmd_extension_dict = dict(self.cmd_extension_names)

        wsi_platform_manual_funcs = ['CreateWin32SurfaceKHR',
                                     'CreateXcbSurfaceKHR',
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
                                 'GetPhysicalDeviceMemoryProperties',
                                 'GetPhysicalDeviceQueueFamilyProperties',
                                 'GetPhysicalDeviceSurfaceSupportKHR',
                                 'GetPhysicalDeviceSurfaceCapabilitiesKHR',
                                 'GetPhysicalDeviceSurfaceFormatsKHR',
                                 'GetPhysicalDeviceSurfacePresentModesKHR',
                                 'CreateSwapchainKHR',
                                 'GetSwapchainImagesKHR',
                                 'CreateXcbSurfaceKHR',
                                 'CreateXlibSurfaceKHR',
                                 'GetPhysicalDeviceXcbPresentationSupportKHR',
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
                                 'GetBufferMemoryRequirements',
                                 ]
        # Map APIs to functions if body is fully custom
        custom_body_dict = {'CreateInstance': self.GenReplayCreateInstance,
                            'GetPhysicalDeviceXcbPresentationSupportKHR': self.GenReplayGetPhysicalDeviceXcbPresentationSupportKHR,
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
        replay_gen_source += 'void vkFuncs::init_funcs(void * handle) {\n'
        replay_gen_source += '    m_libHandle = handle;\n'

        for api in self.cmdMembers:
            # TEMPORARY EXTENSION WORKAROUND
            if api.name in temporary_script_porting_exclusions:
                continue
            cmdname = api.name
            protect = cmd_protect_dict[cmdname]
            if protect is not None:
                replay_gen_source += '#ifdef %s\n' % protect
            if 'DebugReport' not in cmdname:
                replay_gen_source += '    real_%s = (type_%s)(vktrace_platform_get_library_entrypoint(handle, "%s"));\n' % (cmdname, cmdname, cmdname)
            else: # These func ptrs get assigned at GetProcAddr time
                replay_gen_source += '    real_%s = (type_%s)NULL;\n' % (cmdname, cmdname)
            if protect is not None:
                replay_gen_source += '#endif // %s\n' % protect
        replay_gen_source += '}\n\n'
        replay_gen_source += 'vktrace_replay::VKTRACE_REPLAY_RESULT vkReplay::replay(vktrace_trace_packet_header *packet) { \n'
        replay_gen_source += '    vktrace_replay::VKTRACE_REPLAY_RESULT returnValue = vktrace_replay::VKTRACE_REPLAY_SUCCESS;\n'
        replay_gen_source += '    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;\n'
        replay_gen_source += '    switch (packet->packet_id) {\n'
        replay_gen_source += '        case VKTRACE_TPI_VK_vkApiVersion: {\n'
        replay_gen_source += '            packet_vkApiVersion* pPacket = (packet_vkApiVersion*)(packet->pBody);\n'
        replay_gen_source += '            if (VK_VERSION_MAJOR(pPacket->version) != 1 || VK_VERSION_MINOR (pPacket->version) != 0) {\n'
        replay_gen_source += '                vktrace_LogError("Trace file is from Vulkan version 0x%x (%u.%u.%u), but the vktrace plugin only supports version 0x%x (%u.%u.%u).", pPacket->version, (pPacket->version & 0xFFC00000) >> 22, (pPacket->version & 0x003FF000) >> 12, (pPacket->version & 0x00000FFF), VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), ((VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION)) & 0xFFC00000) >> 22, ((VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION)) & 0x003FF000) >> 12, ((VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION)) & 0x00000FFF));\n'
        replay_gen_source += '                returnValue = vktrace_replay::VKTRACE_REPLAY_ERROR;\n'
        replay_gen_source += '            }\n'
        replay_gen_source += '            break;\n'
        replay_gen_source += '        }\n'

        for api in self.cmdMembers:
            cmdname = api.name
            vk_cmdname = cmdname
            # TEMPORARY EXTENSION WORKAROUND
            if api.name in temporary_script_porting_exclusions:
                continue
            # Strip off 'vk' from command name
            cmdname = cmdname[2:]
            if cmdname in api_exclusions:
                continue
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
                    replay_gen_source += '            if (m_vkFuncs.real_vkDestroyDebugReportCallbackEXT != NULL) {\n'
                    replay_gen_source += '                m_vkFuncs.real_vkDestroyDebugReportCallbackEXT(remappedinstance, m_dbgMsgCallbackObj, pPacket->pAllocator);\n'
                    replay_gen_source += '            }\n'
                # TODO: need a better way to indicate which extensions should be mapped to which Get*ProcAddr
                elif cmdname == 'GetInstanceProcAddr':
                    for command in self.cmdMembers:
                        # TEMPORARY EXTENSION WORKAROUND
                        if api.name in temporary_script_porting_exclusions:
                            continue
                        if cmd_extension_dict[command.name] != 'VK_VERSION_1_0' and command.name not in api_exclusions:
                            gipa_params = cmd_member_dict[vk_cmdname]
                            gipa_protect = cmd_protect_dict[command.name]
                            if gipa_protect is not None:
                                replay_gen_source += '#ifdef %s\n' % gipa_protect
                            if (gipa_params[0].type == 'VkInstance'):
                                replay_gen_source += '            if (strcmp(pPacket->pName, "%s") == 0) {\n' % (command.name)
                                replay_gen_source += '               m_vkFuncs.real_%s = (PFN_%s)vk%s(remappedinstance, pPacket->pName);\n' % (command.name, command.name, cmdname)
                                replay_gen_source += '            }\n'
                            if gipa_protect is not None:
                                replay_gen_source += '#endif // %s\n' % gipa_protect
                elif cmdname == 'GetDeviceProcAddr':
                    for command in self.cmdMembers:
                        # TEMPORARY EXTENSION WORKAROUND
                        if api.name in temporary_script_porting_exclusions:
                            continue
                        if cmd_extension_dict[command.name] != 'VK_VERSION_1_0' and command.name not in api_exclusions:
                            gdpa_params = cmd_member_dict[vk_cmdname]
                            gdpa_protect = cmd_protect_dict[command.name]
                            if gdpa_protect is not None:
                                replay_gen_source += '#ifdef %s\n' % gdpa_protect
                            if gdpa_params[0].type != 'VkInstance' and gdpa_params[0].type != 'VkPhysicalDevice':
                                replay_gen_source += '            if (strcmp(pPacket->pName, "%s") == 0) {\n' % (command.name)
                                replay_gen_source += '               m_vkFuncs.real_%s = (PFN_%s)vk%s(remappeddevice, pPacket->pName);\n' % (command.name, command.name, cmdname)
                                replay_gen_source += '            }\n'
                            if gdpa_protect is not None:
                                replay_gen_source += '#endif // %s\n' % gdpa_protect
                elif cmdname == 'GetPhysicalDeviceMemoryProperties':
                    replay_gen_source += '            VkPhysicalDeviceMemoryProperties memProperties = *(pPacket->pMemoryProperties);\n'
                elif cmdname == 'GetImageMemoryRequirements':
                    replay_gen_source += '            VkMemoryRequirements memReqs = *(pPacket->pMemoryRequirements);\n'
                # Build the call to the "real_" entrypoint
                rr_string = '            '
                if ret_value:
                    if cmdinfo.elem.find('proto/type').text != 'VkResult':
                        ret_value = False
                    else:
                        rr_string = '            replayResult = '
                rr_string += 'm_vkFuncs.real_vk%s(' % cmdname
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
                get_ext_layers_proto = ['EnumerateInstanceExtensionProperties', 'EnumerateDeviceExtensionProperties','EnumerateInstanceLayerProperties', 'EnumerateDeviceLayerProperties']
                if 'DestroyDevice' in cmdname:
                    replay_gen_source += '            if (replayResult == VK_SUCCESS) {\n'
                    replay_gen_source += '                m_pCBDump = NULL;\n'
                    replay_gen_source += '                m_pDSDump = NULL;\n'
                    #TODO138 : disabling snapshot
                    #replay_gen_source += '                m_pVktraceSnapshotPrint = NULL;\n'
                    replay_gen_source += '                m_objMapper.rm_from_devices_map(pPacket->device);\n'
                    replay_gen_source += '                m_display->m_initedVK = false;\n'
                    replay_gen_source += '            }\n'
                elif cmdname in get_ext_layers_proto:
                    replay_gen_source += '            if (replayResult == VK_ERROR_LAYER_NOT_PRESENT || replayResult == VK_INCOMPLETE) {\n'
                    replay_gen_source += '                replayResult = VK_SUCCESS;\n'
                    replay_gen_source += '            }\n'
                elif 'DestroySwapchainKHR' in cmdname:
                    replay_gen_source += '            if (replayResult == VK_SUCCESS) {\n'
                    replay_gen_source += '                m_objMapper.rm_from_swapchainkhrs_map(pPacket->swapchain);\n'
                    replay_gen_source += '            }\n'
                elif 'AcquireNextImageKHR' in cmdname:
                    replay_gen_source += '            m_objMapper.add_to_pImageIndex_map(*(pPacket->pImageIndex), local_pImageIndex);\n'
                elif 'DestroyInstance' in cmdname:
                    replay_gen_source += '            if (replayResult == VK_SUCCESS) {\n'
                    replay_gen_source += '                // TODO need to handle multiple instances and only clearing maps within an instance.\n'
                    replay_gen_source += '                // TODO this only works with a single instance used at any given time.\n'
                    replay_gen_source += '                m_objMapper.clear_all_map_handles();\n'
                    replay_gen_source += '            }\n'
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
        for api in self.cmdMembers:
            if api.name[2:] in api_exclusions:
                continue
            # TEMPORARY EXTENSION WORKAROUND
            if api.name in temporary_script_porting_exclusions:
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
            if api.name[2:] in api_exclusions:
                continue
            # TEMPORARY EXTENSION WORKAROUND
            if api.name in temporary_script_porting_exclusions:
                continue
            trace_pkt_id_hdr += '    case VKTRACE_TPI_VK_%s: {\n' % api.name
            func_str = 'vk%s(' % api.name
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
        for api in self.cmdMembers:
            if api.name[2:] in api_exclusions:
                continue
            # TEMPORARY EXTENSION WORKAROUND
            if api.name in temporary_script_porting_exclusions:
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
        if "VkMemoryRequirements" in vk_type:
            return ("%p {size=%\" PRIu64 \", alignment=%\" PRIu64 \", memoryTypeBits=%0x08X}", "%s, (%s == NULL)?0:%s->size, (%s == NULL)?0:%s->alignment, (%s == NULL)?0:%s->memoryTypeBits" % (name, name, name, name, name, name, name), "")
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
                return ("%s",  "(*%s == VK_TRUE) ? \"VK_TRUE\" : \"VK_FALSE\"" % (name), "*")
            return ("%s", "(%s == VK_TRUE) ? \"VK_TRUE\" : \"VK_FALSE\"" %(name), deref)
        if "VkFence" in vk_type:
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
    # Create a vktrace file and return it as a string
    def OutputDestFile(self):
        if self.vktrace_file_type == 'vkreplay_objmapper_header':
            return self.GenerateReplayObjmapperHeader()
        elif self.vktrace_file_type == 'vkreplay_funcptr_header':
            return self.GenerateReplayFuncptrHeader()
        elif self.vktrace_file_type == 'vkreplay_replay_gen_source':
            return self.GenerateReplayGenSource()
        elif self.vktrace_file_type == 'vktrace_packet_id_header':
            return self.GenerateTracePacketIdHeader()
        else:
            return 'Bad VkTrace File Generator Option %s' % self.vktrace_file_type

