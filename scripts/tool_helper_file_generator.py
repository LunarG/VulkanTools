#!/usr/bin/python3 -i
#
# Copyright (c) 2015-2018 The Khronos Group Inc.
# Copyright (c) 2015-2018 Valve Corporation
# Copyright (c) 2015-2018 LunarG, Inc.
# Copyright (c) 2015-2018 Google Inc.
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
# Author: Tobin Ehlis <tobine@google.com>

import os,re,sys
import xml.etree.ElementTree as etree
from generator import *
from collections import namedtuple
from common_codegen import *

#
# ToolHelperFileOutputGeneratorOptions - subclass of GeneratorOptions.
class ToolHelperFileOutputGeneratorOptions(GeneratorOptions):
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
                 protectProto = None,
                 protectProtoStr = None,
                 apicall = '',
                 apientry = '',
                 apientryp = '',
                 alignFuncParam = 0,
                 library_name = '',
                 helper_file_type = ''):
        GeneratorOptions.__init__(self, filename, directory, apiname, profile,
                                  versions, emitversions, defaultExtensions,
                                  addExtensions, removeExtensions, emitExtensions, sortProcedure)
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
        self.helper_file_type = helper_file_type
#
# ToolHelperFileOutputGenerator - subclass of OutputGenerator. Outputs Vulkan helper files
class ToolHelperFileOutputGenerator(OutputGenerator):
    """Generate helper file based on XML element attributes"""
    def __init__(self,
                 errFile = sys.stderr,
                 warnFile = sys.stderr,
                 diagFile = sys.stdout):
        OutputGenerator.__init__(self, errFile, warnFile, diagFile)
        # Internal state - accumulators for different inner block text
        self.struct_size_h_output = ''                    # string built up of struct size header output
        self.struct_size_c_output = ''                    # string built up of struct size source output
        # Internal state - accumulators for different inner block text
        self.structNames = []                             # List of Vulkan struct typenames
        self.structTypes = dict()                         # Map of Vulkan struct typename to required VkStructureType
        self.structMembers = []                           # List of StructMemberData records for all Vulkan structs
        self.object_types = []                            # List of all handle types
        self.debug_report_object_types = []               # Handy copy of debug_report_object_type enum data
        self.core_object_types = []                       # Handy copy of core_object_type enum data
        self.device_extension_info = dict()               # Dict of device extension name defines and ifdef values
        self.instance_extension_info = dict()             # Dict of instance extension name defines and ifdef values

        # Named tuples to store struct and command data
        self.StructType = namedtuple('StructType', ['name', 'value'])
        self.CommandParam = namedtuple('CommandParam', ['type', 'name', 'ispointer', 'isstaticarray', 'isconst', 'iscount', 'len', 'extstructs', 'cdecl'])
        self.StructMemberData = namedtuple('StructMemberData', ['name', 'members', 'ifdef_protect'])

        self.custom_construct_params = {
            # safe_VkGraphicsPipelineCreateInfo needs to know if subpass has color and\or depth\stencil attachments to use its pointers
            'VkGraphicsPipelineCreateInfo' :
                ', const bool uses_color_attachment, const bool uses_depthstencil_attachment',
            # safe_VkPipelineViewportStateCreateInfo needs to know if viewport and scissor is dynamic to use its pointers
            'VkPipelineViewportStateCreateInfo' :
                ', const bool is_dynamic_viewports, const bool is_dynamic_scissors',
        }
    #
    # Called once at the beginning of each run
    def beginFile(self, genOpts):
        OutputGenerator.beginFile(self, genOpts)
        # User-supplied prefix text, if any (list of strings)
        self.helper_file_type = genOpts.helper_file_type
        self.library_name = genOpts.library_name
        # File Comment
        file_comment = '// *** THIS FILE IS GENERATED - DO NOT EDIT ***\n'
        file_comment += '// See helper_file_generator.py for modifications\n'
        write(file_comment, file=self.outFile)
        # Copyright Notice
        copyright = ''
        copyright += '\n'
        copyright += '/***************************************************************************\n'
        copyright += ' *\n'
        copyright += ' * Copyright (c) 2015-2018 The Khronos Group Inc.\n'
        copyright += ' * Copyright (c) 2015-2018 Valve Corporation\n'
        copyright += ' * Copyright (c) 2015-2018 LunarG, Inc.\n'
        copyright += ' * Copyright (c) 2015-2018 Google Inc.\n'
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
        copyright += ' * Author: Courtney Goeltzenleuchter <courtneygo@google.com>\n'
        copyright += ' * Author: Tobin Ehlis <tobine@google.com>\n'
        copyright += ' * Author: Chris Forbes <chrisforbes@google.com>\n'
        copyright += ' * Author: John Zulauf<jzulauf@lunarg.com>\n'
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
    # Override parent class to be notified of the beginning of an extension
    def beginFeature(self, interface, emit):
        # Start processing in superclass
        OutputGenerator.beginFeature(self, interface, emit)
        self.featureExtraProtect = GetFeatureProtect(interface)
        if 'VK_VERSION_1' in self.featureName:
            return
        nameElem = interface[0][1]
        name = nameElem.get('name')
        if 'EXTENSION_NAME' not in name:
            print("Error in vk.xml file -- extension name is not available")
        if interface.get('type') == 'instance':
            self.instance_extension_info[name] = self.featureExtraProtect
        else:
            self.device_extension_info[name] = self.featureExtraProtect
    #
    # Override parent class to be notified of the end of an extension
    def endFeature(self):
        # Finish processing in superclass
        OutputGenerator.endFeature(self)
    #
    # Grab group (e.g. C "enum" type) info to output for enum-string conversion helper
    def genGroup(self, groupinfo, groupName, alias):
        OutputGenerator.genGroup(self, groupinfo, groupName, alias)
        groupElem = groupinfo.elem
        # For enum_string_header
        if self.helper_file_type == 'enum_string_header':
            value_list = []
            for elem in groupElem.findall('enum'):
                if elem.get('supported') != 'disabled':
                    item_name = elem.get('name')
                    value_list.append(item_name)
        elif self.helper_file_type == 'object_types_header':
            if groupName == 'VkDebugReportObjectTypeEXT':
                for elem in groupElem.findall('enum'):
                    if elem.get('supported') != 'disabled':
                        item_name = elem.get('name')
                        self.debug_report_object_types.append(item_name)
            elif groupName == 'VkObjectType':
                for elem in groupElem.findall('enum'):
                    if elem.get('supported') != 'disabled':
                        item_name = elem.get('name')
                        self.core_object_types.append(item_name)

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
            self.genStruct(typeinfo, name, alias)
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
        if (paramname.tail is not None) and ('[' in paramname.tail):
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
            # Process VkStructureType
            if type == 'VkStructureType':
                # Extract the required struct type value from the comments
                # embedded in the original text defining the 'typeinfo' element
                rawXml = etree.tostring(typeinfo.elem).decode('ascii')
                result = re.search(r'VK_STRUCTURE_TYPE_\w+', rawXml)
                if result:
                    value = result.group(0)
                    # Store the required type value
                    self.structTypes[typeName] = self.StructType(name=name, value=value)
            # Store pointer/array/string info
            isstaticarray = self.paramIsStaticArray(member)
            membersInfo.append(self.CommandParam(type=type,
                                                 name=name,
                                                 ispointer=self.paramIsPointer(member),
                                                 isstaticarray=isstaticarray,
                                                 isconst=True if 'const' in cdecl else False,
                                                 iscount=True if name in lens else False,
                                                 len=self.getLen(member),
                                                 extstructs=self.registry.validextensionstructs[typeName] if name == 'pNext' else None,
                                                 cdecl=cdecl))
        self.structMembers.append(self.StructMemberData(name=typeName, members=membersInfo, ifdef_protect=self.featureExtraProtect))
    #
    # struct_size_header: build function prototypes for header file
    def GenerateStructSizeHeader(self):
        outstring = ''
        outstring += 'size_t get_struct_chain_size(const void* struct_ptr);\n'
        outstring += 'size_t get_struct_size(const void* struct_ptr);\n'
        for item in self.structMembers:
            lower_case_name = item.name.lower()
            if item.ifdef_protect != None:
                outstring += '#ifdef %s\n' % item.ifdef_protect
            outstring += 'size_t vk_size_%s(const %s* struct_ptr);\n' % (item.name.lower(), item.name)
            if item.ifdef_protect != None:
                outstring += '#endif // %s\n' % item.ifdef_protect
        outstring += '#ifdef __cplusplus\n'
        outstring += '}\n'
        outstring += '#endif'
        return outstring
    #
    # Combine struct size helper header file preamble with body text and return
    def GenerateStructSizeHelperHeader(self):
        struct_size_helper_header = '\n'
        struct_size_helper_header += '#ifdef __cplusplus\n'
        struct_size_helper_header += 'extern "C" {\n'
        struct_size_helper_header += '#endif\n'
        struct_size_helper_header += '\n'
        struct_size_helper_header += '#include <stdio.h>\n'
        struct_size_helper_header += '#include <stdlib.h>\n'
        struct_size_helper_header += '#include <vulkan/vulkan.h>\n'
        struct_size_helper_header += '\n'
        struct_size_helper_header += '// Function Prototypes\n'
        struct_size_helper_header += self.GenerateStructSizeHeader()
        return struct_size_helper_header
    #
    # Helper function for declaring a counter variable only once
    def DeclareCounter(self, string_var, declare_flag):
        if declare_flag == False:
            string_var += '        uint32_t i = 0;\n'
            declare_flag = True
        return string_var, declare_flag
    #
    # Build the header of the get_struct_chain_size function
    def GenerateChainSizePreamble(self):
        preamble = '\nsize_t get_struct_chain_size(const void* struct_ptr) {\n'
        preamble += '    // Use VkApplicationInfo as struct until actual type is resolved\n'
        preamble += '    VkApplicationInfo* pNext = (VkApplicationInfo*)struct_ptr;\n'
        preamble += '    size_t struct_size = 0;\n'
        preamble += '    while (pNext) {\n'
        preamble += '        switch (pNext->sType) {\n'
        return preamble
    #
    # Build the footer of the get_struct_chain_size function
    def GenerateChainSizePostamble(self):
        postamble  = '            default:\n'
        postamble += '                struct_size += 0;\n'
        postamble += '                break;'
        postamble += '        }\n'
        postamble += '        pNext = (VkApplicationInfo*)pNext->pNext;\n'
        postamble += '    }\n'
        postamble += '    return struct_size;\n'
        postamble += '}\n'
        return postamble
    #
    # Build the header of the get_struct_size function
    def GenerateStructSizePreamble(self):
        preamble = '\nsize_t get_struct_size(const void* struct_ptr) {\n'
        preamble += '    switch (((VkApplicationInfo*)struct_ptr)->sType) {\n'
        return preamble
    #
    # Build the footer of the get_struct_size function
    def GenerateStructSizePostamble(self):
        postamble  = '    default:\n'
        postamble += '        return(0);\n'
        postamble += '    }\n'
        postamble += '}'
        return postamble
    #
    # struct_size_helper source -- create bodies of struct size helper functions
    def GenerateStructSizeSource(self):
        # Construct the bodies of the struct size functions, get_struct_chain_size(),
        # and get_struct_size() simultaneously
        struct_size_funcs = ''
        chain_size  = self.GenerateChainSizePreamble()
        struct_size  = self.GenerateStructSizePreamble()
        for item in self.structMembers:
            struct_size_funcs += '\n'
            lower_case_name = item.name.lower()
            if item.ifdef_protect != None:
                struct_size_funcs += '#ifdef %s\n' % item.ifdef_protect
                struct_size += '#ifdef %s\n' % item.ifdef_protect
                chain_size += '#ifdef %s\n' % item.ifdef_protect
            if item.name in self.structTypes:
                chain_size += '            case %s: {\n' % self.structTypes[item.name].value
                chain_size += '                struct_size += vk_size_%s((%s*)pNext);\n' % (item.name.lower(), item.name)
                chain_size += '                break;\n'
                chain_size += '            }\n'
                struct_size += '    case %s: \n' % self.structTypes[item.name].value
                struct_size += '        return vk_size_%s((%s*)struct_ptr);\n' % (item.name.lower(), item.name)
            struct_size_funcs += 'size_t vk_size_%s(const %s* struct_ptr) { \n' % (item.name.lower(), item.name)
            struct_size_funcs += '    size_t struct_size = 0;\n'
            struct_size_funcs += '    if (struct_ptr) {\n'
            struct_size_funcs += '        struct_size = sizeof(%s);\n' % item.name
            counter_declared = False
            for member in item.members:
                vulkan_type = next((i for i, v in enumerate(self.structMembers) if v[0] == member.type), None)
                if member.ispointer == True:
                    if vulkan_type is not None:
                        # If this is another Vulkan structure call generated size function
                        if member.len is not None:
                            struct_size_funcs, counter_declared = self.DeclareCounter(struct_size_funcs, counter_declared)
                            struct_size_funcs += '        for (i = 0; i < struct_ptr->%s; i++) {\n' % member.len
                            struct_size_funcs += '            struct_size += vk_size_%s(&struct_ptr->%s[i]);\n' % (member.type.lower(), member.name)
                            struct_size_funcs += '        }\n'
                        else:
                            struct_size_funcs += '        struct_size += vk_size_%s(struct_ptr->%s);\n' % (member.type.lower(), member.name)
                    else:
                        if member.type == 'char':
                            # Deal with sizes of character strings
                            if member.len is not None:
                                struct_size_funcs, counter_declared = self.DeclareCounter(struct_size_funcs, counter_declared)
                                struct_size_funcs += '        for (i = 0; i < struct_ptr->%s; i++) {\n' % member.len
                                struct_size_funcs += '            struct_size += (sizeof(char*) + ROUNDUP_TO_4((sizeof(char) * (1 + strlen(struct_ptr->%s[i])))));\n' % (member.name)
                                struct_size_funcs += '        }\n'
                            else:
                                struct_size_funcs += '        struct_size += (struct_ptr->%s != NULL) ? ROUNDUP_TO_4(sizeof(char)*(1+strlen(struct_ptr->%s))) : 0;\n' % (member.name, member.name)
                        else:
                            if member.len is not None:
                                # Avoid using 'sizeof(void)', which generates compile-time warnings/errors
                                checked_type = member.type
                                if checked_type == 'void':
                                    checked_type = 'void*'
                                struct_size_funcs += '        struct_size += (struct_ptr->%s ) * sizeof(%s);\n' % (member.len, checked_type)
            struct_size_funcs += '    }\n'
            struct_size_funcs += '    return struct_size;\n'
            struct_size_funcs += '}\n'
            if item.ifdef_protect != None:
                struct_size_funcs += '#endif // %s\n' % item.ifdef_protect
                struct_size += '#endif // %s\n' % item.ifdef_protect
                chain_size += '#endif // %s\n' % item.ifdef_protect
        chain_size += self.GenerateChainSizePostamble()
        struct_size += self.GenerateStructSizePostamble()
        return_value = struct_size_funcs + chain_size + struct_size;
        return return_value
    #
    # Combine struct size helper source file preamble with body text and return
    def GenerateStructSizeHelperSource(self):
        struct_size_helper_source = '\n'
        struct_size_helper_source += '#include "vk_struct_size_helper.h"\n'
        struct_size_helper_source += '#include <string.h>\n'
        struct_size_helper_source += '#include <assert.h>\n'
        struct_size_helper_source += '\n'
        struct_size_helper_source += '#define ROUNDUP_TO_4(_len) ((((_len) + 3) >> 2) << 2)\n\n'
        struct_size_helper_source += '// Function Definitions\n'
        struct_size_helper_source += self.GenerateStructSizeSource()
        return struct_size_helper_source

    #
    # Create a helper file and return it as a string
    def OutputDestFile(self):
        if self.helper_file_type == 'struct_size_header':
            return self.GenerateStructSizeHelperHeader()
        elif self.helper_file_type == 'struct_size_source':
            return self.GenerateStructSizeHelperSource()
        else:
            return 'Bad Tools Helper File Generator Option %s' % self.helper_file_type

