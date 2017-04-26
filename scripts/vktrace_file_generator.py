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

        # Named tuples to store struct and command data
        self.StructType = namedtuple('StructType', ['name', 'value'])
        self.CommandParam = namedtuple('CommandParam', ['type', 'name', 'ispointer', 'isstaticarray', 'isconst', 'iscount', 'len', 'extstructs', 'cdecl'])
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
                                                 extstructs=member.attrib.get('validextensionstructs') if name == 'pNext' else None,
                                                 cdecl=cdecl))
        self.structMembers.append(self.StructMemberData(name=typeName, members=membersInfo, ifdef_protect=self.featureExtraProtect))
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
        replay_objmapper_header += 'class objMemory {\n'
        replay_objmapper_header += 'public:\n'
        replay_objmapper_header += '    objMemory() : m_numAllocations(0), m_pMemReqs(NULL) {}\n'
        replay_objmapper_header += '    ~objMemory() { free(m_pMemReqs);}\n'
        replay_objmapper_header += '    void setCount(const uint32_t num) {\n'
        replay_objmapper_header += '        m_numAllocations = num;\n'
        replay_objmapper_header += '    }\n\n'
        replay_objmapper_header += '    void setReqs(const VkMemoryRequirements *pReqs, const uint32_t num) {\n'
        replay_objmapper_header += '        if (m_numAllocations != num && m_numAllocations != 0) {\n'
        replay_objmapper_header += '            vktrace_LogError("objMemory::setReqs, internal mismatch on number of allocations.");\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        if (m_pMemReqs == NULL && pReqs != NULL) {\n'
        replay_objmapper_header += '            m_pMemReqs = (VkMemoryRequirements *) vktrace_malloc(num * sizeof(VkMemoryRequirements));\n'
        replay_objmapper_header += '            if (m_pMemReqs == NULL) {\n'
        replay_objmapper_header += '                vktrace_LogError("objMemory::setReqs out of memory.");\n'
        replay_objmapper_header += '                return;\n'
        replay_objmapper_header += '            }\n'
        replay_objmapper_header += '            memcpy(m_pMemReqs, pReqs, num * sizeof(VkMemoryRequirements));\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '    }\n\n'
        replay_objmapper_header += 'private:\n'
        replay_objmapper_header += '    uint32_t m_numAllocations;\n'
        replay_objmapper_header += '    VkMemoryRequirements *m_pMemReqs;\n'
        replay_objmapper_header += '};\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += 'class gpuMemory {\n'
        replay_objmapper_header += 'public:\n'
        replay_objmapper_header += '    gpuMemory() : m_pendingAlloc(false) {m_allocInfo.allocationSize = 0;}\n'
        replay_objmapper_header += '    ~gpuMemory() {}\n'
        replay_objmapper_header += '// memory mapping functions for app writes into mapped memory\n'
        replay_objmapper_header += '    bool isPendingAlloc()\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        return m_pendingAlloc;\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    void setAllocInfo(const VkMemoryAllocateInfo *info, const bool pending)\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        m_pendingAlloc = pending;\n'
        replay_objmapper_header += '        m_allocInfo = *info;\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    void setMemoryDataAddr(void *pBuf)\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        if (m_mapRange.empty())\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            return;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        MapRange mr = m_mapRange.back();\n'
        replay_objmapper_header += '        if (mr.pData != NULL)\n'
        replay_objmapper_header += '            vktrace_LogWarning("gpuMemory::setMemoryDataAddr() data already mapped overwrite old mapping.");\n'
        replay_objmapper_header += '        else if (pBuf == NULL)\n'
        replay_objmapper_header += '            vktrace_LogWarning("gpuMemory::setMemoryDataAddr() adding NULL pointer.");\n'
        replay_objmapper_header += '        mr.pData = (uint8_t *) pBuf;\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    void copyMappingDataPageGuard(const void* pSrcData)\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        if (m_mapRange.empty())\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            return;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        MapRange mr = m_mapRange.back();\n'
        replay_objmapper_header += '        if (!pSrcData || !mr.pData)\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            if (!pSrcData)\n'
        replay_objmapper_header += '                vktrace_LogError("gpuMemory::copyMappingData() null src pointer.");\n'
        replay_objmapper_header += '            else\n'
        replay_objmapper_header += '                vktrace_LogError("gpuMemory::copyMappingData() null dest pointer totalSize=%u.", m_allocInfo.allocationSize);\n'
        replay_objmapper_header += '            m_mapRange.pop_back();\n'
        replay_objmapper_header += '            return;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '        PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)pSrcData;\n'
        replay_objmapper_header += '        if (pChangedInfoArray[0].length)\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            PBYTE pChangedData = (PBYTE)(pSrcData)+sizeof(PageGuardChangedBlockInfo)*(pChangedInfoArray[0].offset + 1);\n'
        replay_objmapper_header += '            DWORD CurrentOffset = 0;\n'
        replay_objmapper_header += '            for (DWORD i = 0; i < pChangedInfoArray[0].offset; i++)\n'
        replay_objmapper_header += '            {\n'
        replay_objmapper_header += '                if ((size_t)pChangedInfoArray[i + 1].length)\n'
        replay_objmapper_header += '                {\n'
        replay_objmapper_header += '                    memcpy(mr.pData +  (size_t)pChangedInfoArray[i + 1].offset, pChangedData + CurrentOffset, (size_t)pChangedInfoArray[i + 1].length);\n'
        replay_objmapper_header += '                }\n'
        replay_objmapper_header += '                CurrentOffset += pChangedInfoArray[i + 1].length;\n'
        replay_objmapper_header += '            }\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    void setMemoryMapRange(void *pBuf, const size_t size, const size_t offset, const bool pending)\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        MapRange mr;\n'
        replay_objmapper_header += '        mr.pData = (uint8_t *) pBuf;\n'
        replay_objmapper_header += '        if (size == 0)\n'
        replay_objmapper_header += '            mr.size = (size_t)m_allocInfo.allocationSize - offset;\n'
        replay_objmapper_header += '        else\n'
        replay_objmapper_header += '            mr.size = size;\n'
        replay_objmapper_header += '        mr.offset = offset;\n'
        replay_objmapper_header += '        mr.pending = pending;\n'
        replay_objmapper_header += '        m_mapRange.push_back(mr);\n'
        replay_objmapper_header += '        assert((size_t)m_allocInfo.allocationSize >= (size + offset));\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    void copyMappingData(const void* pSrcData, bool entire_map, size_t size, size_t offset)\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        if (m_mapRange.empty())\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            return;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        MapRange mr = m_mapRange.back();\n'
        replay_objmapper_header += '        if (!pSrcData || !mr.pData)\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            if (!pSrcData)\n'
        replay_objmapper_header += '                vktrace_LogError("gpuMemory::copyMappingData() null src pointer.");\n'
        replay_objmapper_header += '            else\n'
        replay_objmapper_header += '                vktrace_LogError("gpuMemory::copyMappingData() null dest pointer totalSize=%u.", m_allocInfo.allocationSize);\n'
        replay_objmapper_header += '            m_mapRange.pop_back();\n'
        replay_objmapper_header += '            return;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        if (entire_map)\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            size = mr.size;\n'
        replay_objmapper_header += '            offset = 0;   // pointer to mapped buffer is from offset 0\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        else\n'
        replay_objmapper_header += '        {\n'
        replay_objmapper_header += '            assert(offset >= mr.offset);\n'
        replay_objmapper_header += '            assert(size <= mr.size && (size + offset) <= (size_t)m_allocInfo.allocationSize);\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        memcpy(mr.pData + offset, pSrcData, size);\n'
        replay_objmapper_header += '        if (!mr.pending && entire_map)\n'
        replay_objmapper_header += '            m_mapRange.pop_back();\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    size_t getMemoryMapSize()\n'
        replay_objmapper_header += '    {\n'
        replay_objmapper_header += '        return (!m_mapRange.empty()) ? m_mapRange.back().size : 0;\n'
        replay_objmapper_header += '    }\n\n'
        replay_objmapper_header += 'private:\n'
        replay_objmapper_header += '    bool m_pendingAlloc;\n'
        replay_objmapper_header += '    struct MapRange {\n'
        replay_objmapper_header += '        bool pending;\n'
        replay_objmapper_header += '        size_t size;\n'
        replay_objmapper_header += '        size_t offset;\n'
        replay_objmapper_header += '        uint8_t* pData;\n'
        replay_objmapper_header += '    };\n'
        replay_objmapper_header += '    std::vector<MapRange> m_mapRange;\n'
        replay_objmapper_header += '    VkMemoryAllocateInfo m_allocInfo;\n'
        replay_objmapper_header += '};\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += 'typedef struct _imageObj {\n'
        replay_objmapper_header += '     objMemory imageMem;\n'
        replay_objmapper_header += '     VkImage replayImage;\n'
        replay_objmapper_header += ' } imageObj;\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += 'typedef struct _bufferObj {\n'
        replay_objmapper_header += '     objMemory bufferMem;\n'
        replay_objmapper_header += '     VkBuffer replayBuffer;\n'
        replay_objmapper_header += ' } bufferObj;\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += 'typedef struct _devicememoryObj {\n'
        replay_objmapper_header += '     gpuMemory *pGpuMem;\n'
        replay_objmapper_header += '     VkDeviceMemory replayDeviceMemory;\n'
        replay_objmapper_header += ' } devicememoryObj;\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += 'class vkReplayObjMapper {\n'
        replay_objmapper_header += 'public:\n'
        replay_objmapper_header += '    vkReplayObjMapper() {}\n'
        replay_objmapper_header += '    ~vkReplayObjMapper() {}\n'
        replay_objmapper_header += '\n'
        replay_objmapper_header += '    bool m_adjustForGPU; // true if replay adjusts behavior based on GPU\n'
        replay_objmapper_header += '    void init_objMemCount(const uint64_t handle, const VkDebugReportObjectTypeEXT objectType, const uint32_t &num) {\n'
        replay_objmapper_header += '        switch (objectType) {\n'
        replay_objmapper_header += '            case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT: {\n'
        replay_objmapper_header += '                std::map<VkBuffer, bufferObj>::iterator it = m_buffers.find((VkBuffer) handle);\n'
        replay_objmapper_header += '                if (it != m_buffers.end()) {\n'
        replay_objmapper_header += '                    objMemory obj = it->second.bufferMem;\n'
        replay_objmapper_header += '                    obj.setCount(num);\n'
        replay_objmapper_header += '                    return;\n'
        replay_objmapper_header += '                }\n'
        replay_objmapper_header += '                break;\n'
        replay_objmapper_header += '            }\n'
        replay_objmapper_header += '            case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT: {\n'
        replay_objmapper_header += '                std::map<VkImage, imageObj>::iterator it = m_images.find((VkImage) handle);\n'
        replay_objmapper_header += '                if (it != m_images.end()) {\n'
        replay_objmapper_header += '                    objMemory obj = it->second.imageMem;\n'
        replay_objmapper_header += '                    obj.setCount(num);\n'
        replay_objmapper_header += '                    return;\n'
        replay_objmapper_header += '                }\n'
        replay_objmapper_header += '                break;\n'
        replay_objmapper_header += '            }\n'
        replay_objmapper_header += '            default:\n'
        replay_objmapper_header += '                break;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        return;\n'
        replay_objmapper_header += '    }\n\n'
        replay_objmapper_header += '    void init_objMemReqs(const uint64_t handle, const VkDebugReportObjectTypeEXT objectType, const VkMemoryRequirements *pMemReqs, const unsigned int num)\n    {\n'
        replay_objmapper_header += '        switch (objectType) {\n'
        replay_objmapper_header += '            case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT: { \n'
        replay_objmapper_header += '                std::map<VkBuffer, bufferObj>::iterator it = m_buffers.find((VkBuffer) handle);\n'
        replay_objmapper_header += '                if (it != m_buffers.end()) {\n'
        replay_objmapper_header += '                    objMemory obj = it->second.bufferMem;\n'
        replay_objmapper_header += '                    obj.setReqs(pMemReqs, num);\n'
        replay_objmapper_header += '                    return;\n'
        replay_objmapper_header += '                }\n'
        replay_objmapper_header += '                break;\n'
        replay_objmapper_header += '            }\n'
        replay_objmapper_header += '            case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT: {\n'
        replay_objmapper_header += '                std::map<VkImage, imageObj>::iterator it = m_images.find((VkImage) handle);\n'
        replay_objmapper_header += '                if (it != m_images.end()) {\n'
        replay_objmapper_header += '                    objMemory obj = it->second.imageMem;\n'
        replay_objmapper_header += '                    obj.setReqs(pMemReqs, num);\n'
        replay_objmapper_header += '                    return;\n'
        replay_objmapper_header += '                }\n'
        replay_objmapper_header += '                break;\n'
        replay_objmapper_header += '            }\n'
        replay_objmapper_header += '            default:\n'
        replay_objmapper_header += '                break;\n'
        replay_objmapper_header += '        }\n'
        replay_objmapper_header += '        return;\n'
        replay_objmapper_header += '    }\n'
        replay_objmapper_header += '\n'

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
    # Create a vktrace file and return it as a string
    def OutputDestFile(self):
        if self.vktrace_file_type == 'vkreplay_objmapper_header':
            return self.GenerateReplayObjmapperHeader()
        else:
            return 'Bad VkTrace File Generator Option %s' % self.vktrace_file_type

