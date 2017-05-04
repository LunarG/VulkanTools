#!/usr/bin/env python3
#
# Vulkan
#
# Copyright (C) 2015-2016 Valve Corporation
# Copyright (C) 2015-2016 LunarG, Inc.
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
# Author: Jon Ashburn <jon@lunarg.com>
# Author: Tobin Ehlis <tobin@lunarg.com>
# Author: Peter Lohrmann <peterl@valvesoftware.com>
#

import os, sys

from source_line_info import sourcelineinfo

import vulkan

# vulkan.py doesn't include all the extensions (debug_report missing)
headers = []
objects = []
protos = []
proto_exclusions = [ 'CreateWaylandSurfaceKHR', 'CreateMirSurfaceKHR',
                     'GetPhysicalDeviceWaylandPresentationSupportKHR', 'GetPhysicalDeviceMirPresentationSupportKHR',
                     'GetPhysicalDeviceDisplayPropertiesKHR', 'GetPhysicalDeviceDisplayPlanePropertiesKHR',
                     'GetDisplayPlaneSupportedDisplaysKHR', 'GetDisplayModePropertiesKHR',
                     'CreateDisplayModeKHR', 'GetDisplayPlaneCapabilitiesKHR', 'CreateDisplayPlaneSurfaceKHR']

for ext in vulkan.extensions_all:
    headers.extend(ext.headers)
    objects.extend(ext.objects)
    protos.extend(ext.protos)

# Add parameters we need to remap, along with their type, in pairs
additional_remap_fifo = {}
additional_remap_fifo['pImageIndex'] = "uint32_t"

class Subcommand(object):
    def __init__(self, argv):
        self.argv = argv
        self.extensionName = argv
        self.headers = headers
        self.objects = objects
        self.protos = protos
        self.lineinfo = sourcelineinfo()

    def run(self):
        print(self.generate(self.extensionName))

    def generate(self, extensionName):
        copyright = self.generate_copyright()
        header = self.generate_header(extensionName)
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
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Tobin Ehlis <tobin@lunarg.com>
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 */"""

    def generate_header(self, extensionName):
        return "\n".join(["#include <" + h + ">" for h in self.headers])

    def generate_body(self):
        pass

    def generate_footer(self):
        pass

    def _generate_init_funcs(self):
        init_tracer = []

        init_tracer.append('extern VKTRACE_CRITICAL_SECTION g_memInfoLock;')

        init_tracer.append('#ifdef WIN32\n')
        init_tracer.append('BOOL CALLBACK InitTracer(_Inout_ PINIT_ONCE initOnce, _Inout_opt_ PVOID param, _Out_opt_ PVOID *lpContext)\n{')
        init_tracer.append('#elif defined(PLATFORM_LINUX)\n')
        init_tracer.append('void InitTracer(void)\n{')
        init_tracer.append('#endif\n')
        init_tracer.append('#if defined(ANDROID)')
        init_tracer.append('    // On Android, we can use an abstract socket to fit permissions model')
        init_tracer.append('    const char *ipAddr = "localabstract";')
        init_tracer.append('    const char *ipPort = "vktrace";')
        init_tracer.append('    gMessageStream = vktrace_MessageStream_create_port_string(FALSE, ipAddr, ipPort);')
        init_tracer.append('#else')
        init_tracer.append('    const char *ipAddr = vktrace_get_global_var("VKTRACE_LIB_IPADDR");')
        init_tracer.append('    if (ipAddr == NULL)')
        init_tracer.append('        ipAddr = "127.0.0.1";')
        init_tracer.append('    gMessageStream = vktrace_MessageStream_create(FALSE, ipAddr, VKTRACE_BASE_PORT + VKTRACE_TID_VULKAN);')
        init_tracer.append('#endif')
        init_tracer.append('    vktrace_trace_set_trace_file(vktrace_FileLike_create_msg(gMessageStream));')
        init_tracer.append('    vktrace_tracelog_set_tracer_id(VKTRACE_TID_VULKAN);')
        init_tracer.append('    trim::initialize();')
        init_tracer.append('    vktrace_initialize_trace_packet_utils();')
        init_tracer.append('    vktrace_create_critical_section(&g_memInfoLock);')
        init_tracer.append('#ifdef WIN32\n')
        init_tracer.append('    return true;\n}\n')
        init_tracer.append('#elif defined(PLATFORM_LINUX)\n')
        init_tracer.append('    return;\n}\n')
        init_tracer.append('#endif\n')
        return "\n".join(init_tracer)

    # Take a list of params and return a list of dicts w/ ptr param details
    def _get_packet_ptr_param_list(self, params):
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
                           'pName': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pName), ((pName != NULL) ? ROUNDUP_TO_4(strlen(pName) + 1) : 0), pName)',
                                     'finalize_txt': 'default'},
                           'pMarker': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMarker), ((pMarker != NULL) ? ROUNDUP_TO_4(strlen(pMarker) + 1) : 0), pMarker)',
                                       'finalize_txt': 'default'},
                           'pExtName': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pExtName), ((pExtName != NULL) ? ROUNDUP_TO_4(strlen(pExtName) + 1) : 0), pExtName)',
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
                           'VkMemoryAllocateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkMemoryAllocateInfo), pAllocateInfo);\n'
                                                            '    add_alloc_memory_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo->pNext), pAllocateInfo->pNext)',
                                            'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo))'},
#                          'VkGraphicsPipelineCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfos), count*sizeof(VkGraphicsPipelineCreateInfo), pCreateInfos);\n'
#                                                                      '    add_VkGraphicsPipelineCreateInfos_to_trace_packet(pHeader, (VkGraphicsPipelineCreateInfo*)pPacket->pCreateInfos, pCreateInfos, count)',
#                                                      'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfos))'},
#                          'VkComputePipelineCreateInfo': {'add_txt': 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfos), count*sizeof(VkComputePipelineCreateInfo), pCreateInfos);\n'
#                                                                      '    add_VkComputePipelineCreateInfos_to_trace_packet(pHeader, (VkComputePipelineCreateInfo*)pPacket->pCreateInfos, pCreateInfos, count)',
#                                                      'finalize_txt': 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfos))'},
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
            if '*' in p.ty and p.name not in ['pTag', 'pUserData']:
                if 'const' in p.ty.lower() and 'count' in params[params.index(p)-1].name.lower():
                    pp_dict['add_txt'] = 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), %s*sizeof(%s), %s)' % (p.name, params[params.index(p)-1].name, p.ty.strip('*').replace('const ', ''), p.name)
                elif 'pOffsets' == p.name: # TODO : This is a custom case for BindVertexBuffers last param, need to clean this up
                    pp_dict['add_txt'] = 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), %s*sizeof(%s), %s)' % (p.name, params[params.index(p)-2].name, p.ty.strip('*').replace('const ', ''), p.name)
                elif p.ty.strip('*').replace('const ', '') in custom_ptr_dict:
                    pp_dict['add_txt'] = custom_ptr_dict[p.ty.strip('*').replace('const ', '')]['add_txt']
                    pp_dict['finalize_txt'] = custom_ptr_dict[p.ty.strip('*').replace('const ', '')]['finalize_txt']
                elif p.name in custom_ptr_dict:
                    pp_dict['add_txt'] = custom_ptr_dict[p.name]['add_txt']
                    pp_dict['finalize_txt'] = custom_ptr_dict[p.name]['finalize_txt']
                    # TODO : This is custom hack to account for 2 pData items with dataSize param for sizing
                    if 'pData' == p.name and 'dataSize' == params[params.index(p)-1].name:
                        pp_dict['add_txt'] = pp_dict['add_txt'].replace('_dataSize', 'dataSize')
                elif 'void' in p.ty and (p.name == 'pData' or p.name == 'pValues'):
                    pp_dict['add_txt'] = '//TODO FIXME vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), sizeof(%s), %s)' % (p.name, p.ty.strip('*').replace('const ', ''), p.name)
                    pp_dict['finalize_txt'] = '//TODO FIXME vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->%s))' % (p.name)
                else:
                    pp_dict['add_txt'] = 'vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->%s), sizeof(%s), %s)' % (p.name, p.ty.strip('*').replace('const ', ''), p.name)
                if 'finalize_txt' not in pp_dict or 'default' == pp_dict['finalize_txt']:
                    pp_dict['finalize_txt'] = 'vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->%s))' % (p.name)
                pp_dict['index'] = params.index(p)
                ptr_param_list.append(pp_dict)
        return ptr_param_list

    # Take a list of params and return a list of packet size elements
    def _get_packet_size(self, extensionName, params):
        ps = [] # List of elements to be added together to account for packet size for given params
        skip_list = [] # store params that are already accounted for so we don't count them twice
        # Dict of specific params with unique custom sizes
        # TODO: Now using bitfields for all stages, need pSetBindPoints to accommodate that.
        custom_size_dict = {'pSetBindPoints': '(VK_SHADER_STAGE_COMPUTE * sizeof(uint32_t))', # Accounting for largest possible array
                            'VkSwapchainCreateInfoKHR' : 'vk_size_vkswapchaincreateinfokhr(pCreateInfo)',
                            }
        size_func_suffix = ''
        if extensionName.lower() != "vk_version_1_0":
            size_func_suffix = '_%s' % extensionName.lower()
        for p in params:
            #First handle custom cases
            if p.name in ['pCreateInfo', 'pSetLayoutInfoList', 'pBeginInfo', 'pAllocateInfo'] and 'khr' not in p.ty.lower() and 'lunarg' not in p.ty.lower() and 'ext' not in p.ty.lower():
                ps.append('get_struct_chain_size%s((void*)%s)' % (size_func_suffix, p.name))
                skip_list.append(p.name)
            elif p.name in custom_size_dict:
                ps.append(custom_size_dict[p.name])
                skip_list.append(p.name)
            elif p.ty.strip('*').replace('const ', '') in custom_size_dict:
                tmp_ty = p.ty.strip('*').replace('const ', '')
                ps.append(custom_size_dict[tmp_ty])
                skip_list.append(p.name)
            # Skip any params already handled
            if p.name in skip_list:
                continue
            # Now check to identify dynamic arrays which depend on two params
            if 'count' in p.name.lower():
                next_idx = params.index(p)+1
                # If next element is a const *, then multiply count and array type
                if next_idx < len(params) and '*' in params[next_idx].ty and 'const' in params[next_idx].ty.lower():
                    if '*' in p.ty:
                        ps.append('*%s*sizeof(%s)' % (p.name, params[next_idx].ty.strip('*').replace('const ', '')))
                    else:
                        ps.append('%s*sizeof(%s)' % (p.name, params[next_idx].ty.strip('*').replace('const ', '')))
                    skip_list.append(params[next_idx].name)
                if 'bindingCount' == p.name: # TODO : This is custom case for CmdBindVertexBuffers, need to clean it up
                    ps.append('%s*sizeof(%s)' % (p.name, params[next_idx+1].ty.strip('*').replace('const ', '')))
                    skip_list.append(params[next_idx+1].name)
                elif '*' in p.ty: # Not a custom array size we're aware of, but ptr so need to account for its size
                    ps.append('sizeof(%s)' % (p.ty.strip('*').replace('const ', '')))
            elif '*' in p.ty and p.name not in ['pSysMem', 'pReserved']:
                if 'pData' == p.name:
                    if 'dataSize' == params[params.index(p)-1].name:
                        ps.append('dataSize')
                    elif 'counterCount' == params[params.index(p)-1].name:
                        ps.append('sizeof(%s)' % p.ty.strip('*').replace('const ', ''))
                    else:
                        #ps.append('((pDataSize != NULL && pData != NULL) ? *pDataSize : 0)')
                        ps.append('sizeof(void*)')
                elif '**' in p.ty and 'void' in p.ty:
                    ps.append('sizeof(void*)')
                elif 'void' in p.ty:
                    ps.append('sizeof(%s)' % p.name)
                elif 'char' in p.ty:
                    ps.append('((%s != NULL) ? ROUNDUP_TO_4(strlen(%s) + 1) : 0)' % (p.name, p.name))
                elif 'pDataSize' in p.name:
                    ps.append('((pDataSize != NULL) ? sizeof(size_t) : 0)')
                elif 'IMAGE_SUBRESOURCE' in p.ty and 'pSubresource' == p.name:
                    ps.append('((pSubresource != NULL) ? sizeof(VkImage_SUBRESOURCE) : 0)')
                else:
                    ps.append('sizeof(%s)' % (p.ty.strip('*').replace('const ', '')))
        return ps

    # Generate instructions for certain API calls that need special handling when we are recording
    # them during the trim frames. This ensures that we recreate the objects that are referenced
    # during the trim frames.
    def _generate_trim_recording_instructions(self, proto):
        trim_instructions = []
        if 'CmdExecuteCommands' is proto.name:
            trim_instructions.append("            trim::write_packet(pHeader);")
            trim_instructions.append("            trim::mark_CommandBuffer_reference(commandBuffer);")
            trim_instructions.append("            if (pCommandBuffers != nullptr && commandBufferCount > 0)")
            trim_instructions.append("            {")
            trim_instructions.append("                for (uint32_t i = 0; i < commandBufferCount; i++)")
            trim_instructions.append("                {")
            trim_instructions.append("                    trim::mark_CommandBuffer_reference(pCommandBuffers[i]);")
            trim_instructions.append("                }")
            trim_instructions.append("            }")
        else:
            return None
        return "\n".join(trim_instructions)

    # Generate instructions for certain API calls that need to be tracked prior to the trim frames
    # so that we can recreate objects that are used within a trimmed trace file.
    def _generate_trim_statetracking_instructions(self, proto):
        trim_instructions = []
        if 'GetDeviceQueue' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Queue_object(*pQueue);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Queue.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.Queue.queueFamilyIndex = queueFamilyIndex;")
            trim_instructions.append("        info.ObjectInfo.Queue.queueIndex = queueIndex;")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateCommandPool' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_CommandPool_object(*pCommandPool);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.CommandPool.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.CommandPool.queueFamilyIndex = pCreateInfo->queueFamilyIndex;")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.CommandPool.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyCommandPool' is proto.name:
            trim_instructions.append("        trim::remove_CommandPool_object(commandPool);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyCommandPool' is proto.name:
            trim_instructions.append("        trim::remove_CommandPool_object(commandPool);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'FreeCommandBuffers' is proto.name:
            trim_instructions.append("        trim::ObjectInfo* pCBInfo = trim::get_CommandBuffer_objectInfo(pCommandBuffers[0]);")
            trim_instructions.append("        VkCommandBufferLevel level = (pCBInfo == NULL) ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : pCBInfo->ObjectInfo.CommandBuffer.level;")
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_CommandPool_objectInfo(commandPool);")
            trim_instructions.append("        if (pInfo != NULL ) { pInfo->ObjectInfo.CommandPool.numCommandBuffersAllocated[level] -= commandBufferCount; }")
            trim_instructions.append("        for (uint32_t i = 0; i < commandBufferCount; i++)")
            trim_instructions.append("        {")
            trim_instructions.append("            trim::remove_CommandBuffer_object(pCommandBuffers[i]);")
            trim_instructions.append("            trim::remove_CommandBuffer_calls(pCommandBuffers[i]);")
            trim_instructions.append("            trim::ClearImageTransitions(pCommandBuffers[i]);")
            trim_instructions.append("            trim::ClearBufferTransitions(pCommandBuffers[i]);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'QueueBindSparse' is proto.name:
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateSemaphore' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Semaphore_object(*pSemaphore);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Semaphore.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.Semaphore.signaledOnQueue = VK_NULL_HANDLE;")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Semaphore.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroySemaphore' is proto.name:
            trim_instructions.append("        trim::remove_Semaphore_object(semaphore);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateFence' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Fence_object(*pFence);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Fence.signaled = ((pCreateInfo->flags & VK_FENCE_CREATE_SIGNALED_BIT) == VK_FENCE_CREATE_SIGNALED_BIT);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Fence.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'ResetFences' is proto.name:
            trim_instructions.append("        for (uint32_t i = 0; i < fenceCount; i++) {")
            trim_instructions.append("            trim::ObjectInfo* pFenceInfo = trim::get_Fence_objectInfo(pFences[i]);")
            trim_instructions.append("            if (pFenceInfo != NULL && result == VK_SUCCESS) {")
            trim_instructions.append("                // clear the fence")
            trim_instructions.append("                pFenceInfo->ObjectInfo.Fence.signaled = false;")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyFence' is proto.name:
            trim_instructions.append("        trim::remove_Fence_object(fence);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('CmdCopyImage' is proto.name or
              'CmdBlitImage' is proto.name or
              'CmdResolveImage' is proto.name or
              'CmdCopyBufferToImage' is proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(dstImage);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.mostRecentLayout = dstImageLayout;")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('CmdClearColorImage' is proto.name or
              'CmdClearDepthStencilImage' is proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(image);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.mostRecentLayout = imageLayout;")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CmdEndRenderPass' is proto.name:
            trim_instructions.append("        trim::ObjectInfo* pCommandBuffer = trim::get_CommandBuffer_objectInfo(commandBuffer);")
            trim_instructions.append("        if (pCommandBuffer != nullptr)")
            trim_instructions.append('        {')
            trim_instructions.append("            trim::ObjectInfo* pRenderPass = trim::get_RenderPass_objectInfo(pCommandBuffer->ObjectInfo.CommandBuffer.activeRenderPass);")
            trim_instructions.append("            if (pRenderPass != nullptr)")
            trim_instructions.append('            {')
            trim_instructions.append('                for (uint32_t i = 0; i < pRenderPass->ObjectInfo.RenderPass.attachmentCount; i++)')
            trim_instructions.append('                {')
            trim_instructions.append('                    trim::AddImageTransition(commandBuffer, pRenderPass->ObjectInfo.RenderPass.pAttachments[i]);')
            trim_instructions.append('                }')
            trim_instructions.append('            }')
            trim_instructions.append('        }')
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')

        elif ('EndCommandBuffer' is proto.name or
              'CmdBindPipeline' is proto.name or
              'CmdSetViewport' is proto.name or
              'CmdSetScissor' is proto.name or
              'CmdSetLineWidth' is proto.name or
              'CmdSetDepthBias' is proto.name or
              'CmdSetBlendConstants' is proto.name or
              'CmdSetDepthBounds' is proto.name or
              'CmdSetStencilCompareMask' is proto.name or
              'CmdSetStencilWriteMask' is proto.name or
              'CmdSetStencilReference' is proto.name or
              'CmdBindDescriptorSets' is proto.name or
              'CmdBindIndexBuffer' is proto.name or
              'CmdBindVertexBuffers' is proto.name or
              'CmdDraw' is proto.name or
              'CmdDrawIndexed' is proto.name or
              'CmdDrawIndirect' is proto.name or
              'CmdDrawIndexedIndirect' is proto.name or
              'CmdDispatch' is proto.name or
              'CmdDispatchIndirect' is proto.name or
              'CmdCopyBuffer' is proto.name or
              'CmdCopyImageToBuffer' is proto.name or
              'CmdUpdateBuffer' is proto.name or
              'CmdFillBuffer' is proto.name or
              'CmdClearAttachments' is proto.name or
              'CmdSetEvent' is proto.name or
              'CmdResetEvent' is proto.name or
              'CmdNextSubpass' is proto.name or
              'CmdExecuteCommands' is proto.name):
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('ResetCommandBuffer' is proto.name):
            trim_instructions.append("        trim::remove_CommandBuffer_calls(commandBuffer);")
            trim_instructions.append("        trim::ClearImageTransitions(commandBuffer);")
            trim_instructions.append("        trim::ClearBufferTransitions(commandBuffer);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateImageView' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_ImageView_object(*pView);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.ImageView.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        info.ObjectInfo.ImageView.image = pCreateInfo->image;")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.ImageView.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyImageView' is proto.name:
            trim_instructions.append("        trim::remove_ImageView_object(imageView);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('GetImageMemoryRequirements' is proto.name):
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyImage' is proto.name:
            trim_instructions.append("#if TRIM_USE_ORDERED_IMAGE_CREATION")
            trim_instructions.append("        trim::add_Image_call(trim::copy_packet(pHeader));")
            trim_instructions.append("#endif //TRIM_USE_ORDERED_IMAGE_CREATION")
            trim_instructions.append("        trim::remove_Image_object(image);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'BindImageMemory' is proto.name:
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Image_objectInfo(image);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Image.pBindImageMemoryPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("            pInfo->ObjectInfo.Image.memory = memory;")
            trim_instructions.append("            pInfo->ObjectInfo.Image.memoryOffset = memoryOffset;")
            trim_instructions.append("            pInfo->ObjectInfo.Image.needsStagingBuffer = trim::IsMemoryDeviceOnly(memory);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateBufferView' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_BufferView_object(*pView);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.BufferView.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.BufferView.pAllocator = pAllocator;")
            trim_instructions.append("        }")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyBufferView' is proto.name:
            trim_instructions.append("        trim::remove_BufferView_object(bufferView);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateBuffer' is proto.name:
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyBuffer' is proto.name:
            trim_instructions.append("        trim::remove_Buffer_object(buffer);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'BindBufferMemory' is proto.name:
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_Buffer_objectInfo(buffer);")
            trim_instructions.append("        if (pInfo != NULL) {")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.pBindBufferMemoryPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.memory = memory;")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.memoryOffset = memoryOffset;")
            trim_instructions.append("            pInfo->ObjectInfo.Buffer.needsStagingBuffer = trim::IsMemoryDeviceOnly(memory);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateSampler' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Sampler_object(*pSampler);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Sampler.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Sampler.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroySampler' is proto.name:
            trim_instructions.append("        trim::remove_Sampler_object(sampler);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateDescriptorSetLayout' is proto.name:
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyDescriptorSetLayout' is proto.name:
            trim_instructions.append("        trim::remove_DescriptorSetLayout_object(descriptorSetLayout);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'ResetDescriptorPool' is proto.name:
            trim_instructions.append("        trim::ObjectInfo* pPoolInfo = trim::get_DescriptorPool_objectInfo(descriptorPool);")
            trim_instructions.append("        pPoolInfo->ObjectInfo.DescriptorPool.numSets = 0;")
            trim_instructions.append("        trim::reset_DescriptorPool(descriptorPool);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreatePipelineLayout' is proto.name:
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyPipelineLayout' is proto.name:
            trim_instructions.append("        trim::remove_PipelineLayout_object(pipelineLayout);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyPipelineCache' is proto.name:
            trim_instructions.append("        trim::remove_PipelineCache_object(pipelineCache);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroySwapchainKHR' is proto.name:
            trim_instructions.append("        trim::remove_SwapchainKHR_object(swapchain);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroySurfaceKHR' is proto.name:
            trim_instructions.append("        trim::remove_SurfaceKHR_object(surface);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyPipelineCache' is proto.name:
            trim_instructions.append("        trim::remove_PipelineCache_object(pipelineCache);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroySwapchainKHR' is proto.name:
            trim_instructions.append("        trim::remove_SwapchainKHR_object(swapchain);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroySurfaceKHR' is proto.name:
            trim_instructions.append("        trim::remove_SurfaceKHR_object(surface);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::add_recorded_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyRenderPass' is proto.name:
            trim_instructions.append("        trim::remove_RenderPass_object(renderPass);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateShaderModule' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_ShaderModule_object(*pShaderModule);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        trim::StateTracker::copy_VkShaderModuleCreateInfo(&info.ObjectInfo.ShaderModule.createInfo, *pCreateInfo);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.ShaderModule.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyShaderModule' is proto.name:
            trim_instructions.append("        trim::remove_ShaderModule_object(shaderModule);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyPipeline' is proto.name:
            trim_instructions.append("        trim::remove_Pipeline_object(pipeline);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyDescriptorPool' is proto.name:
            trim_instructions.append("        trim::remove_DescriptorPool_object(descriptorPool);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyFramebuffer' is proto.name:
            trim_instructions.append("        trim::remove_Framebuffer_object(framebuffer);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateEvent' is proto.name:
            trim_instructions.append("        trim::ObjectInfo &info = trim::add_Event_object(*pEvent);")
            trim_instructions.append("        info.belongsToDevice = device;")
            trim_instructions.append("        info.ObjectInfo.Event.pCreatePacket = trim::copy_packet(pHeader);")
            trim_instructions.append("        if (pAllocator != NULL) {")
            trim_instructions.append("            info.ObjectInfo.Event.pAllocator = pAllocator;")
            trim_instructions.append("            trim::add_Allocator(pAllocator);")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyEvent' is proto.name:
            trim_instructions.append("        trim::remove_Event_object(event);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CreateQueryPool' is proto.name:
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CmdResetQueryPool' is proto.name:
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_QueryPool_objectInfo(queryPool);")
            trim_instructions.append("        if (pInfo != NULL) {" )
            trim_instructions.append("            for (uint32_t i = firstQuery; (i < pInfo->ObjectInfo.QueryPool.size) && (i < firstQuery + queryCount); i++) {")
            trim_instructions.append("                pInfo->ObjectInfo.QueryPool.pResultsAvailable[i] = false;")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'CmdBeginQuery' is proto.name:
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif ('CmdEndQuery' is proto.name or
              'CmdWriteTimestamp' is proto.name):
            trim_instructions.append("        trim::ObjectInfo* pInfo = trim::get_QueryPool_objectInfo(queryPool);")
            trim_instructions.append("        if (pInfo != NULL) {" )
            trim_instructions.append("            pInfo->ObjectInfo.QueryPool.commandBuffer = commandBuffer;")
            trim_instructions.append("            pInfo->ObjectInfo.QueryPool.pResultsAvailable[query] = true;")
            trim_instructions.append("        }")
            trim_instructions.append("        trim::add_CommandBuffer_call(commandBuffer, trim::copy_packet(pHeader));")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyQueryPool' is proto.name:
            trim_instructions.append("        trim::remove_QueryPool_object(queryPool);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'AcquireNextImageKHR' is proto.name:
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
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'GetPhysicalDeviceMemoryProperties' is proto.name:
            trim_instructions.append("        if (g_trimIsPreTrim)")
            trim_instructions.append("        {")
            trim_instructions.append("            trim::ObjectInfo* pInfo = trim::get_PhysicalDevice_objectInfo(physicalDevice);")
            trim_instructions.append("            if (pInfo != NULL)")
            trim_instructions.append("            {")
            trim_instructions.append("                pInfo->ObjectInfo.PhysicalDevice.pGetPhysicalDeviceMemoryPropertiesPacket = trim::copy_packet(pHeader);")
            trim_instructions.append("            }")
            trim_instructions.append("        }")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        elif 'DestroyDevice' is proto.name:
            trim_instructions.append("        trim::remove_Device_object(device);")
            trim_instructions.append('        if (g_trimIsInTrim)')
            trim_instructions.append('        {')
            trim_instructions.append('            trim::write_packet(pHeader);')
            trim_instructions.append('        }')
            trim_instructions.append('        else')
            trim_instructions.append('        {')
            trim_instructions.append('            vktrace_delete_trace_packet(&pHeader);')
            trim_instructions.append('        }')
        else:
            return None
        return "\n".join(trim_instructions)

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
    def _generate_trace_funcs(self, extensionName):
        func_body = []
        manually_written_hooked_funcs = ['AllocateCommandBuffers',
                                         'AllocateMemory',
                                         'AllocateDescriptorSets',
                                         'BeginCommandBuffer',
                                         'CreateDescriptorPool',
                                         'CreateDevice',
                                         'CreateFramebuffer',
                                         'CreateImage',
                                         'CreateInstance',
                                         'CreatePipelineCache',
                                         'CreateRenderPass',
                                         'GetPipelineCacheData',
                                         'CreateGraphicsPipelines',
                                         'CreateComputePipelines',
                                         'CmdPipelineBarrier',
                                         'CmdWaitEvents',
                                         'CmdBeginRenderPass',
                                         'CmdPushConstants',
                                         'DestroyInstance',
                                         'EnumeratePhysicalDevices',
                                         'FreeMemory',
                                         'FreeDescriptorSets',
                                         'QueueSubmit',
                                         'QueueBindSparse',
                                         'FlushMappedMemoryRanges',
                                         'InvalidateMappedMemoryRanges',
                                         'GetDeviceProcAddr',
                                         'GetInstanceProcAddr',
                                         'EnumerateInstanceExtensionProperties',
                                         'EnumerateDeviceExtensionProperties',
                                         'EnumerateInstanceLayerProperties',
                                         'EnumerateDeviceLayerProperties',
                                         'GetPhysicalDeviceQueueFamilyProperties',
                                         'GetQueryPoolResults',
                                         'MapMemory',
                                         'UnmapMemory',
                                         'UpdateDescriptorSets',
                                         'GetPhysicalDeviceSurfaceCapabilitiesKHR',
                                         'GetPhysicalDeviceSurfaceFormatsKHR',
                                         'GetPhysicalDeviceSurfacePresentModesKHR',
                                         'CreateSwapchainKHR',
                                         'GetSwapchainImagesKHR',
                                         'QueuePresentKHR',
                                         #TODO add Wayland, Mir
                                         'CreateXcbSurfaceKHR',
                                         'CreateXlibSurfaceKHR',
                                         'GetPhysicalDeviceXcbPresentationSupportKHR',
                                         'GetPhysicalDeviceXlibPresentationSupportKHR',
                                         'CreateWin32SurfaceKHR',
                                         'GetPhysicalDeviceWin32PresentationSupportKHR',
                                         'CreateAndroidSurfaceKHR',
                                         ]

        # validate the manually_written_hooked_funcs list
        protoFuncs = [proto.name for proto in self.protos]
        wsi_platform_manual_funcs = ['CreateWin32SurfaceKHR', 'CreateXcbSurfaceKHR', 'CreateXlibSurfaceKHR', 'CreateAndroidSurfaceKHR',
                                     'GetPhysicalDeviceXcbPresentationSupportKHR','GetPhysicalDeviceXlibPresentationSupportKHR', 'GetPhysicalDeviceWin32PresentationSupportKHR']
        for func in manually_written_hooked_funcs:
            if (func not in protoFuncs) and (func not in wsi_platform_manual_funcs):
                sys.exit("Entry '%s' in manually_written_hooked_funcs list is not in the vulkan function prototypes" % func)

        # process each of the entrypoint prototypes
        approved_ext = ['vk_khr_surface', 'vk_khr_swapchain', 'vk_khr_win32_surface', 'vk_khr_xcb_surface', 'vk_ext_debug_report']
        for ext in vulkan.extensions_all:
            if (ext.name.lower() == extensionName.lower()) or ((extensionName.lower() == 'vk_version_1_0') and (ext.name.lower() in approved_ext)):
                for proto in ext.protos:
                    if proto.name in manually_written_hooked_funcs:
                        func_body.append( '// __HOOKED_vk%s is manually written. Look in vktrace_lib_trace.cpp\n' % proto.name)
                    elif proto.name not in proto_exclusions:
                        raw_packet_update_list = [] # non-ptr elements placed directly into packet
                        ptr_packet_update_list = [] # ptr elements to be updated into packet
                        return_txt = ''
                        packet_size = []
                        in_data_size = False # flag when we need to capture local input size variable for in/out size
                        func_body.append('%s' % self.lineinfo.get())
                        func_body.append('VKTRACER_EXPORT VKAPI_ATTR %s VKAPI_CALL __HOOKED_vk%s(' % (proto.ret, proto.name))
                        for p in proto.params: # TODO : For all of the ptr types, check them for NULL and return 0 if NULL
                            func_body.append('    %s,' % p.c())
                            if '*' in p.ty and p.name not in ['pSysMem', 'pReserved']:
                                if 'pDataSize' in p.name:
                                    in_data_size = True;
                            elif 'pfnMsgCallback' == p.name:
                                raw_packet_update_list.append('    PFN_vkDebugReportCallbackEXT* pNonConstCallback = (PFN_vkDebugReportCallbackEXT*)&pPacket->pfnMsgCallback;')
                                raw_packet_update_list.append('    *pNonConstCallback = pfnMsgCallback;')
                            elif '[' in p.ty:
                                raw_packet_update_list.append('    memcpy((void *) pPacket->%s, %s, sizeof(pPacket->%s));' % (p.name, p.name, p.name))
                            else:
                                raw_packet_update_list.append('    pPacket->%s = %s;' % (p.name, p.name))
                        # Get list of packet size modifiers due to ptr params
                        packet_size = self._get_packet_size(extensionName, proto.params)
                        ptr_packet_update_list = self._get_packet_ptr_param_list(proto.params)
                        func_body[-1] = func_body[-1].replace(',', ')')
                        # End of function declaration portion, begin function body
                        func_body.append('{')
                        if 'void' not in proto.ret or '*' in proto.ret:
                            func_body.append('    %s result;' % proto.ret)
                            return_txt = 'result = '
                        func_body.append('    vktrace_trace_packet_header* pHeader;')
                        if in_data_size:
                            func_body.append('    size_t _dataSize;')
                        func_body.append('    packet_vk%s* pPacket = NULL;' % proto.name)
                        if proto.name == "DestroyInstance" or proto.name == "DestroyDevice":
                            func_body.append('    dispatch_key key = get_dispatch_key(%s);' % proto.params[0].name)

                        if (0 == len(packet_size)):
                            func_body.append('    CREATE_TRACE_PACKET(vk%s, 0);' % (proto.name))
                        else:
                            func_body.append('    CREATE_TRACE_PACKET(vk%s, %s);' % (proto.name, ' + '.join(packet_size)))

                        if proto.name == "CreateImage":
                            func_body.append('    VkImageCreateInfo replayCreateInfo = *pCreateInfo;')
                            func_body.append('    VkImageCreateInfo trimCreateInfo = *pCreateInfo;')
                            func_body.append("    if (g_trimEnabled) {")
                            func_body.append("        // need to add TRANSFER_SRC usage to the image so that we can copy out of it.")
                            func_body.append('        trimCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;')
                            func_body.append('        pCreateInfo = &trimCreateInfo;')
                            func_body.append('    }')
                        elif proto.name == "CreateBuffer":
                            func_body.append('    VkBufferCreateInfo replayCreateInfo = *pCreateInfo;')
                            func_body.append('    VkBufferCreateInfo trimCreateInfo = *pCreateInfo;')
                            func_body.append("    if (g_trimEnabled) {")
                            func_body.append("        // need to add TRANSFER_SRC usage to the buffer so that we can copy out of it.")
                            func_body.append('        trimCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;')
                            func_body.append('        pCreateInfo = &trimCreateInfo;')
                            func_body.append('    }')

                        # call down the layer chain and get return value (if there is one)
                        # Note: this logic doesn't work for CreateInstance or CreateDevice but those are handwritten
                        if extensionName == 'vk_lunarg_debug_marker':
                            table_txt = 'mdd(%s)->debugMarkerTable' % proto.params[0].name
                        elif proto.params[0].ty in ['VkInstance', 'VkPhysicalDevice']:
                           table_txt = 'mid(%s)->instTable' % proto.params[0].name
                        else:
                           table_txt = 'mdd(%s)->devTable' % proto.params[0].name
                        func_body.append('    %s%s.%s;' % (return_txt, table_txt, proto.c_call()))
                        func_body.append('    vktrace_set_packet_entrypoint_end_time(pHeader);')

                        if proto.name == "CreateImage":
                            func_body.append("    if (g_trimEnabled) {")
                            func_body.append("        // need to add TRANSFER_DST usage to the image so that we can recreate it.")
                            func_body.append('        replayCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;')
                            func_body.append('        pCreateInfo = &replayCreateInfo;')
                            func_body.append('    }')
                        elif proto.name == "CreateBuffer":
                            func_body.append("    if (g_trimEnabled) {")
                            func_body.append("        // need to add TRANSFER_DST usage to the buffer so that we can recreate it.")
                            func_body.append('        replayCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;')
                            func_body.append('        pCreateInfo = &replayCreateInfo;')
                            func_body.append('    }')

                        if in_data_size:
                            func_body.append('    _dataSize = (pDataSize == NULL || pData == NULL) ? 0 : *pDataSize;')
                        func_body.append('    pPacket = interpret_body_as_vk%s(pHeader);' % proto.name)
                        func_body.append('\n'.join(raw_packet_update_list))
                        for pp_dict in ptr_packet_update_list: #buff_ptr_indices:
                            func_body.append('    %s;' % (pp_dict['add_txt']))
                        if 'void' not in proto.ret or '*' in proto.ret:
                            func_body.append('    pPacket->result = result;')
                        for pp_dict in ptr_packet_update_list:
                            if ('DeviceCreateInfo' not in proto.params[pp_dict['index']].ty):
                                func_body.append('    %s;' % (pp_dict['finalize_txt']))
                        func_body.append('    if (!g_trimEnabled)')
                        func_body.append('    {')
                        # All buffers should be finalized by now, and the trace packet can be finished (which sends it over the socket)
                        func_body.append('        FINISH_TRACE_PACKET();')

                        # Else half of g_trimEnabled conditional
                        # Since packet wasn't sent to trace file, it either needs to be associated with an object, or deleted.
                        func_body.append('    }')
                        func_body.append('    else')
                        func_body.append('    {')
                        func_body.append('        vktrace_finalize_trace_packet(pHeader);')
                        trim_instructions = self._generate_trim_statetracking_instructions(proto)
                        if trim_instructions is not None:
                            func_body.append(trim_instructions)
                        else:
                            func_body.append('        if (g_trimIsInTrim)')
                            func_body.append('        {')
                            intrim_instructions = self._generate_trim_recording_instructions(proto)
                            if intrim_instructions is not None:
                                func_body.append(intrim_instructions)
                            else:
                                func_body.append('            trim::write_packet(pHeader);')
                            func_body.append('        }')
                            func_body.append('        else')
                            func_body.append('        {')
                            func_body.append('            vktrace_delete_trace_packet(&pHeader);')
                            func_body.append('        }')
                        func_body.append('    }')

                        # Clean up instance or device data if needed
                        if proto.name == "DestroyInstance":
                            func_body.append('    g_instanceDataMap.erase(key);')
                        elif proto.name == "DestroyDevice":
                            func_body.append('    g_deviceDataMap.erase(key);')

                        # return result if needed
                        if 'void' not in proto.ret or '*' in proto.ret:
                            func_body.append('    return result;')
                        func_body.append('}\n')
        return "\n".join(func_body)

    def _generate_struct_util_funcs(self):
        lineinfo = self.lineinfo
        pid_enum = []
        pid_enum.append('%s' % lineinfo.get())
        pid_enum.append('//=============================================================================')
        pid_enum.append('static void add_VkApplicationInfo_to_packet(vktrace_trace_packet_header*  pHeader, VkApplicationInfo** ppStruct, const VkApplicationInfo *pInStruct)')
        pid_enum.append('{')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)ppStruct, sizeof(VkApplicationInfo), pInStruct);')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->pApplicationName), (pInStruct->pApplicationName != NULL) ? ROUNDUP_TO_4(strlen(pInStruct->pApplicationName) + 1) : 0, pInStruct->pApplicationName);')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->pEngineName), (pInStruct->pEngineName != NULL) ? ROUNDUP_TO_4(strlen(pInStruct->pEngineName) + 1) : 0, pInStruct->pEngineName);')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)&((*ppStruct)->pApplicationName));')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)&((*ppStruct)->pEngineName));')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)&*ppStruct);')
        pid_enum.append('};\n')
        pid_enum.append('%s' % lineinfo.get())
        pid_enum.append('static void add_VkInstanceCreateInfo_to_packet(vktrace_trace_packet_header* pHeader, VkInstanceCreateInfo** ppStruct, VkInstanceCreateInfo *pInStruct)')
        pid_enum.append('{')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)ppStruct, sizeof(VkInstanceCreateInfo), pInStruct);')
        pid_enum.append('    if (pInStruct->pApplicationInfo) add_VkApplicationInfo_to_packet(pHeader, (VkApplicationInfo**)&((*ppStruct)->pApplicationInfo), pInStruct->pApplicationInfo);')
        # TODO138 : This is an initial pass at getting the extension/layer arrays correct, needs to be validated.
        pid_enum.append('    uint32_t i, siz = 0;')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledLayerNames), pInStruct->enabledLayerCount * sizeof(char*), pInStruct->ppEnabledLayerNames);')
        pid_enum.append('    if (pInStruct->enabledLayerCount > 0) ')
        pid_enum.append('    {')
        pid_enum.append('        for (i = 0; i < pInStruct->enabledLayerCount; i++) {')
        pid_enum.append('            siz = (uint32_t) ROUNDUP_TO_4(1 + strlen(pInStruct->ppEnabledLayerNames[i]));')
        pid_enum.append('            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledLayerNames[i]), siz, pInStruct->ppEnabledLayerNames[i]);')
        pid_enum.append('            vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledLayerNames[i]);')
        pid_enum.append('        }')
        pid_enum.append('    }')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledLayerNames);')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledExtensionNames), pInStruct->enabledExtensionCount * sizeof(char*), pInStruct->ppEnabledExtensionNames);')
        pid_enum.append('    if (pInStruct->enabledExtensionCount > 0) ')
        pid_enum.append('    {')
        pid_enum.append('        for (i = 0; i < pInStruct->enabledExtensionCount; i++) {')
        pid_enum.append('            siz = (uint32_t) ROUNDUP_TO_4(1 + strlen(pInStruct->ppEnabledExtensionNames[i]));')
        pid_enum.append('            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledExtensionNames[i]), siz, pInStruct->ppEnabledExtensionNames[i]);')
        pid_enum.append('            vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledExtensionNames[i]);')
        pid_enum.append('        }')
        pid_enum.append('    }')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledExtensionNames);')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)ppStruct);')
        pid_enum.append('}\n')
        pid_enum.append('%s' % lineinfo.get())
        pid_enum.append('static void add_VkDeviceCreateInfo_to_packet(vktrace_trace_packet_header*  pHeader, VkDeviceCreateInfo** ppStruct, const VkDeviceCreateInfo *pInStruct)')
        pid_enum.append('{')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)ppStruct, sizeof(VkDeviceCreateInfo), pInStruct);')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos, pInStruct->queueCreateInfoCount*sizeof(VkDeviceQueueCreateInfo), pInStruct->pQueueCreateInfos);')
        pid_enum.append('    for (uint32_t i = 0; i < pInStruct->queueCreateInfoCount; i++) {')
        pid_enum.append('        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos[i].pQueuePriorities,')
        pid_enum.append('                                   pInStruct->pQueueCreateInfos[i].queueCount*sizeof(float),')
        pid_enum.append('                                   pInStruct->pQueueCreateInfos[i].pQueuePriorities);')
        pid_enum.append('        vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos[i].pQueuePriorities);')
        pid_enum.append('    }')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos);')
        # TODO138 : This is an initial pass at getting the extension/layer arrays correct, needs to be validated.
        pid_enum.append('    uint32_t i, siz = 0;')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledLayerNames), pInStruct->enabledLayerCount * sizeof(char*), pInStruct->ppEnabledLayerNames);')
        pid_enum.append('    if (pInStruct->enabledLayerCount > 0) ')
        pid_enum.append('    {')
        pid_enum.append('        for (i = 0; i < pInStruct->enabledLayerCount; i++) {')
        pid_enum.append('            siz = (uint32_t) ROUNDUP_TO_4(1 + strlen(pInStruct->ppEnabledLayerNames[i]));')
        pid_enum.append('            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledLayerNames[i]), siz, pInStruct->ppEnabledLayerNames[i]);')
        pid_enum.append('            vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledLayerNames[i]);')
        pid_enum.append('        }')
        pid_enum.append('    }')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledLayerNames);')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledExtensionNames), pInStruct->enabledExtensionCount * sizeof(char*), pInStruct->ppEnabledExtensionNames);')
        pid_enum.append('    if (pInStruct->enabledExtensionCount > 0) ')
        pid_enum.append('    {')
        pid_enum.append('        for (i = 0; i < pInStruct->enabledExtensionCount; i++) {')
        pid_enum.append('            siz = (uint32_t) ROUNDUP_TO_4(1 + strlen(pInStruct->ppEnabledExtensionNames[i]));')
        pid_enum.append('            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledExtensionNames[i]), siz, pInStruct->ppEnabledExtensionNames[i]);')
        pid_enum.append('            vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledExtensionNames[i]);')
        pid_enum.append('        }')
        pid_enum.append('    }')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void **)&(*ppStruct)->ppEnabledExtensionNames);')
        pid_enum.append('    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(*ppStruct)->pEnabledFeatures, sizeof(VkPhysicalDeviceFeatures), pInStruct->pEnabledFeatures);')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->pEnabledFeatures);')
        pid_enum.append('    vktrace_finalize_buffer_address(pHeader, (void**)ppStruct);')
        pid_enum.append('}\n')
        pid_enum.append('%s' % lineinfo.get())
        pid_enum.append('//=============================================================================\n')
        pid_enum.append('static VkInstanceCreateInfo* interpret_VkInstanceCreateInfo(vktrace_trace_packet_header*  pHeader, intptr_t ptr_variable)')
        pid_enum.append('{')
        pid_enum.append('    VkInstanceCreateInfo* pVkInstanceCreateInfo = (VkInstanceCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)ptr_variable);\n')
        pid_enum.append('    uint32_t i;')
        pid_enum.append('    if (pVkInstanceCreateInfo != NULL)')
        pid_enum.append('    {')
        pid_enum.append('        pVkInstanceCreateInfo->pApplicationInfo = (VkApplicationInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->pApplicationInfo);')
        pid_enum.append('        VkApplicationInfo** ppApplicationInfo = (VkApplicationInfo**) &pVkInstanceCreateInfo->pApplicationInfo;')
        pid_enum.append('        if (pVkInstanceCreateInfo->pApplicationInfo)')
        pid_enum.append('        {')
        pid_enum.append('            (*ppApplicationInfo)->pApplicationName = (const char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->pApplicationInfo->pApplicationName);')
        pid_enum.append('            (*ppApplicationInfo)->pEngineName = (const char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->pApplicationInfo->pEngineName);')
        pid_enum.append('        }')
        pid_enum.append('        if (pVkInstanceCreateInfo->enabledLayerCount > 0)')
        pid_enum.append('        {')
        pid_enum.append('            pVkInstanceCreateInfo->ppEnabledLayerNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledLayerNames);')
        pid_enum.append('            for (i = 0; i < pVkInstanceCreateInfo->enabledLayerCount; i++) {')
        pid_enum.append('                char** ppTmp = (char**)&pVkInstanceCreateInfo->ppEnabledLayerNames[i];')
        pid_enum.append('                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledLayerNames[i]);')
        pid_enum.append('            }')
        pid_enum.append('        }')
        pid_enum.append('        if (pVkInstanceCreateInfo->enabledExtensionCount > 0)')
        pid_enum.append('        {')
        pid_enum.append('            pVkInstanceCreateInfo->ppEnabledExtensionNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledExtensionNames);')
        pid_enum.append('            for (i = 0; i < pVkInstanceCreateInfo->enabledExtensionCount; i++) {')
        pid_enum.append('                char** ppTmp = (char**)&pVkInstanceCreateInfo->ppEnabledExtensionNames[i];')
        pid_enum.append('                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledExtensionNames[i]);')
        pid_enum.append('            }')
        pid_enum.append('        }')
        pid_enum.append('    }\n')
        pid_enum.append('    return pVkInstanceCreateInfo;')
        pid_enum.append('}\n')
        pid_enum.append('%s' % lineinfo.get())
        pid_enum.append('static VkDeviceCreateInfo* interpret_VkDeviceCreateInfo(vktrace_trace_packet_header*  pHeader, intptr_t ptr_variable)')
        pid_enum.append('{')
        pid_enum.append('    VkDeviceCreateInfo* pVkDeviceCreateInfo = (VkDeviceCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)ptr_variable);\n')
        pid_enum.append('    uint32_t i;')
        pid_enum.append('    if (pVkDeviceCreateInfo != NULL)')
        pid_enum.append('    {')
        pid_enum.append('        if (pVkDeviceCreateInfo->queueCreateInfoCount > 0)')
        pid_enum.append('        {')
        pid_enum.append('            pVkDeviceCreateInfo->pQueueCreateInfos = (const VkDeviceQueueCreateInfo *)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->pQueueCreateInfos);')
        pid_enum.append('            for (i = 0; i < pVkDeviceCreateInfo->queueCreateInfoCount; i++) {')
        pid_enum.append('                float** ppQueuePriority = (float**)&pVkDeviceCreateInfo->pQueueCreateInfos[i].pQueuePriorities;')
        pid_enum.append('                *ppQueuePriority = (float*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->pQueueCreateInfos[i].pQueuePriorities);')
        pid_enum.append('            }')
        pid_enum.append('        }')
        pid_enum.append('        if (pVkDeviceCreateInfo->enabledLayerCount > 0)')
        pid_enum.append('        {')
        pid_enum.append('            pVkDeviceCreateInfo->ppEnabledLayerNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledLayerNames);')
        pid_enum.append('            for (i = 0; i < pVkDeviceCreateInfo->enabledLayerCount; i++) {')
        pid_enum.append('                char** ppTmp = (char**)&pVkDeviceCreateInfo->ppEnabledLayerNames[i];')
        pid_enum.append('                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledLayerNames[i]);')
        pid_enum.append('            }')
        pid_enum.append('        }')
        pid_enum.append('        if (pVkDeviceCreateInfo->enabledExtensionCount > 0)')
        pid_enum.append('        {')
        pid_enum.append('            pVkDeviceCreateInfo->ppEnabledExtensionNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledExtensionNames);')
        pid_enum.append('            for (i = 0; i < pVkDeviceCreateInfo->enabledExtensionCount; i++) {')
        pid_enum.append('                char** ppTmp = (char**)&pVkDeviceCreateInfo->ppEnabledExtensionNames[i];')
        pid_enum.append('                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledExtensionNames[i]);')
        pid_enum.append('            }')
        pid_enum.append('        }')
        pid_enum.append('        pVkDeviceCreateInfo->pEnabledFeatures = (const VkPhysicalDeviceFeatures*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pVkDeviceCreateInfo->pEnabledFeatures);\n')
        pid_enum.append('    }\n')
        pid_enum.append('    return pVkDeviceCreateInfo;')
        pid_enum.append('}\n')
        pid_enum.append('%s' % lineinfo.get())
        pid_enum.append('static void interpret_VkPipelineShaderStageCreateInfo(vktrace_trace_packet_header*  pHeader, VkPipelineShaderStageCreateInfo* pShader)')
        pid_enum.append('{')
        pid_enum.append('    if (pShader != NULL)')
        pid_enum.append('    {')
        pid_enum.append('        pShader->pName = (const char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pShader->pName);')
        pid_enum.append('        // specialization info')
        pid_enum.append('        pShader->pSpecializationInfo = (const VkSpecializationInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pShader->pSpecializationInfo);')
        pid_enum.append('        if (pShader->pSpecializationInfo != NULL)')
        pid_enum.append('        {')
        pid_enum.append('            VkSpecializationInfo* pInfo = (VkSpecializationInfo*)pShader->pSpecializationInfo;')
        pid_enum.append('            pInfo->pMapEntries = (const VkSpecializationMapEntry*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pShader->pSpecializationInfo->pMapEntries);')
        pid_enum.append('            pInfo->pData = (const void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pShader->pSpecializationInfo->pData);')
        pid_enum.append('        }')
        pid_enum.append('    }')
        pid_enum.append('}\n')
        pid_enum.append('//=============================================================================')
        return "\n".join(pid_enum)

    # Interpret functions used on replay to read in packets and interpret their contents
    #  This code gets generated into vktrace_vk_vk_packets.h file
    def _generate_interp_funcs(self):
        # Custom txt for given function and parameter.  First check if param is NULL, then insert txt if not
        # First some common code used by both CmdWaitEvents & CmdPipelineBarrier
        mem_barrier_interp = ['uint32_t i = 0;\n',
                              'for (i = 0; i < pPacket->memoryBarrierCount; i++) {\n',
                              '    void** ppMB = (void**)&(pPacket->ppMemoryBarriers[i]);\n',
                              '    *ppMB = vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->ppMemoryBarriers[i]);\n',
                              '    //VkMemoryBarrier* pBarr = (VkMemoryBarrier*)pPacket->ppMemoryBarriers[i];\n',
                              '    // TODO : Could fix up the pNext ptrs here if they were finalized and if we cared by switching on Barrier type and remapping\n',
                              '}']
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
                            '*ppSD = (VkSubpassDependency*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pInfo->pDependencies);\n']
        create_gfx_pipe = ['uint32_t i;\n',
                           'uint32_t j;\n',
                           'for (i=0; i<pPacket->createInfoCount; i++) {\n',
                            'if (pPacket->pCreateInfos[i].sType == VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO) {\n',
                            '// need to make a non-const pointer to the pointer so that we can properly change the original pointer to the interpretted one\n',
                            'VkGraphicsPipelineCreateInfo* pNonConst = (VkGraphicsPipelineCreateInfo*)&(pPacket->pCreateInfos[i]);\n',
                            '// shader stages array\n',
                            'pNonConst->pStages = (VkPipelineShaderStageCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfos[i].pStages);\n',
                            'for (j = 0; j < pPacket->pCreateInfos[i].stageCount; j++)\n',
                            '{\n',
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
                              # TODO138 : Just ripped out a bunch of custom code here that was out of date. Need to scrub these function and verify they're correct
        custom_case_dict = { #'CreateFramebuffer' : {'param': 'pCreateInfo', 'txt': ['VkFramebufferCreateInfo* pInfo = (VkFramebufferCreateInfo*)pPacket->pCreateInfo;\n',
                              #                      'pInfo->pColorAttachments = (VkColorAttachmentBindInfo*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pColorAttachments);\n',
                               #                     'pInfo->pDepthStencilAttachment = (VkDepthStencilBindInfo*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pDepthStencilAttachment);\n']},
                             'CreateRenderPass' : {'param': 'pCreateInfo', 'txt': create_rp_interp},
                             'CreatePipelineCache' : {'param': 'pCreateInfo', 'txt': [
                                                       '((VkPipelineCacheCreateInfo *)pPacket->pCreateInfo)->pInitialData = (const void*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pInitialData);\n']},
                             'CreatePipelineLayout' : {'param': 'pCreateInfo', 'txt': ['VkPipelineLayoutCreateInfo* pInfo = (VkPipelineLayoutCreateInfo*)pPacket->pCreateInfo;\n',
                                                       'pInfo->pSetLayouts = (VkDescriptorSetLayout*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pSetLayouts);\n',
                                                       'pInfo->pPushConstantRanges = (VkPushConstantRange*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pPushConstantRanges);\n']},
                             'CreateDescriptorPool' : {'param': 'pCreateInfo', 'txt': ['VkDescriptorPoolCreateInfo* pInfo = (VkDescriptorPoolCreateInfo*)pPacket->pCreateInfo;\n',
                                                       'pInfo->pPoolSizes = (VkDescriptorPoolSize*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pPoolSizes);\n']},
                             'CmdWaitEvents' : {'param': 'ppMemoryBarriers', 'txt': mem_barrier_interp},
                             'CmdPipelineBarrier' : {'param': 'ppMemoryBarriers', 'txt': mem_barrier_interp},
                             'CreateDescriptorSetLayout' : {'param': 'pCreateInfo', 'txt': ['if (pPacket->pCreateInfo->sType == VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO) {\n',
                                                                                         '    VkDescriptorSetLayoutCreateInfo* pNext = (VkDescriptorSetLayoutCreateInfo*)pPacket->pCreateInfo;\n',
                                                                                         '    do\n','    {\n',
                                                                                         '        // need to make a non-const pointer to the pointer so that we can properly change the original pointer to the interpretted one\n',
                                                                                         '        void** ppNextVoidPtr = (void**)&(pNext->pNext);\n',
                                                                                         '        *ppNextVoidPtr = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pNext->pNext);\n',
                                                                                         '        switch(pNext->sType)\n', '        {\n',
                                                                                         '            case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO:\n',
                                                                                         '            {\n' ,
                                                                                         '                unsigned int i = 0;\n',
                                                                                         '                pNext->pBindings = (VkDescriptorSetLayoutBinding*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pNext->pBindings);\n',
                                                                                         '                for (i = 0; i < pNext->bindingCount; i++)\n','                {\n',
                                                                                         '                    VkSampler** ppSamplers = (VkSampler**)&(pNext->pBindings[i].pImmutableSamplers);\n',
                                                                                         '                    *ppSamplers = (VkSampler*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pNext->pBindings[i].pImmutableSamplers);\n',
                                                                                         '                }\n',
                                                                                         '                break;\n',
                                                                                         '            }\n',
                                                                                         '            default:\n',
                                                                                         '            {\n',
                                                                                         '                vktrace_LogError("Encountered an unexpected type in descriptor set layout create list.");\n',
                                                                                         '                pPacket->header = NULL;\n',
                                                                                         '                pNext->pNext = NULL;\n',
                                                                                         '            }\n',
                                                                                         '        }\n',
                                                                                         '        pNext = (VkDescriptorSetLayoutCreateInfo*)pNext->pNext;\n',
                                                                                         '     }  while (NULL != pNext);\n',
                                                                                         '} else {\n',
                                                                                         '     // This is unexpected.\n',
                                                                                         '     vktrace_LogError("CreateDescriptorSetLayout must have pCreateInfo->stype of VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO.");\n',
                                                                                         '     pPacket->header = NULL;\n',
                                                                                         '}']},
                             'BeginCommandBuffer' : {'param': 'pBeginInfo', 'txt': [
                                                                                          'VkCommandBufferBeginInfo* pInfo = (VkCommandBufferBeginInfo*) pPacket->pBeginInfo;\n',
                                                       'pInfo->pInheritanceInfo = (VkCommandBufferInheritanceInfo*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pBeginInfo->pInheritanceInfo);\n']},
                             'AllocateMemory' : {'param': 'pAllocateInfo', 'txt': ['if (pPacket->pAllocateInfo->sType == VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO) {\n',
                                                                                         '    VkMemoryAllocateInfo** ppNext = (VkMemoryAllocateInfo**) &(pPacket->pAllocateInfo->pNext);\n',
                                                                                         '    *ppNext = (VkMemoryAllocateInfo*) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pAllocateInfo->pNext);\n',
                                                                                         '} else {\n',
                                                                                         '    // This is unexpected.\n',
                                                                                         '    vktrace_LogError("AllocateMemory must have AllocInfo stype of VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO.");\n',
                                                                                         '    pPacket->header = NULL;\n',
                                                                                         '}']},
                             'AllocateDescriptorSets' : {'param': 'pAllocateInfo', 'txt':
                                                                               ['VkDescriptorSetLayout **ppDescSetLayout = (VkDescriptorSetLayout **) &pPacket->pAllocateInfo->pSetLayouts;\n'
                                                                                '        *ppDescSetLayout = (VkDescriptorSetLayout *) vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)(pPacket->pAllocateInfo->pSetLayouts));']},
                             'UpdateDescriptorSets' : {'param': 'pDescriptorWrites', 'txt':
                                                                               [ 'uint32_t i;\n',
                                                                                 'for (i = 0; i < pPacket->descriptorWriteCount; i++) {\n',
                                                                                 '    switch (pPacket->pDescriptorWrites[i].descriptorType) {',
                                                                                 '    case VK_DESCRIPTOR_TYPE_SAMPLER:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:',
                                                                                 '        {',
                                                                                 '            VkDescriptorImageInfo** ppImageInfo = (VkDescriptorImageInfo**)&pPacket->pDescriptorWrites[i].pImageInfo;\n',
                                                                                 '            *ppImageInfo = (VkDescriptorImageInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pImageInfo);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:',
                                                                                 '        {',
                                                                                 '            VkBufferView** ppTexelBufferView = (VkBufferView**)&pPacket->pDescriptorWrites[i].pTexelBufferView;\n',
                                                                                 '            *ppTexelBufferView = (VkBufferView*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pTexelBufferView);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:',
                                                                                 '    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:',
                                                                                 '        {',
                                                                                 '            VkDescriptorBufferInfo** ppBufferInfo = (VkDescriptorBufferInfo**)&pPacket->pDescriptorWrites[i].pBufferInfo;\n',
                                                                                 '            *ppBufferInfo = (VkDescriptorBufferInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pDescriptorWrites[i].pBufferInfo);\n',
                                                                                 '        }',
                                                                                 '        break;',
                                                                                 '    default:',
                                                                                 '        break;',
                                                                                 '    }',
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
                                                                              '*ppQueueFamilyIndices = (uint32_t*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pQueueFamilyIndices);\n',
                                                                              'VkImageCreateInfo *pNext = (VkImageCreateInfo*)pPacket->pCreateInfo;\n',
                                                                              'do {\n',
                                                                              '    void** ppNextVoidPtr = (void**)&(pNext->pNext);\n',
                                                                              '    *ppNextVoidPtr = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pNext->pNext);\n',
                                                                              '    pNext = (VkImageCreateInfo*)pNext->pNext;\n',
                                                                              '} while (pNext);']},
                             'CreateBuffer' : {'param': 'pCreateInfo', 'txt': ['uint32_t** ppQueueFamilyIndices = (uint32_t**)&(pPacket->pCreateInfo->pQueueFamilyIndices);\n',
                                                                              '*ppQueueFamilyIndices = (uint32_t*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->pCreateInfo->pQueueFamilyIndices);']},
                             'FlushMappedMemoryRanges' : {'param': 'ppData', 'txt': ['uint32_t i = 0;\n',
                                                                                     'for (i = 0; i < pPacket->memoryRangeCount; i++)\n',
                                                                                     '{\n',
                                                                                     '    pPacket->ppData[i] = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->ppData[i]);\n',
                                                                                     '}']},
                             'InvalidateMappedMemoryRanges' : {'param': 'ppData', 'txt': ['uint32_t i = 0;\n',
                                                                                     'for (i = 0; i < pPacket->memoryRangeCount; i++)\n',
                                                                                     '{\n',
                                                                                     '    pPacket->ppData[i] = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->ppData[i]);\n',
                                                                                     '}']},
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
        if_body.append('typedef struct packet_vkApiVersion {')
        if_body.append('    vktrace_trace_packet_header* header;')
        if_body.append('    uint32_t version;')
        if_body.append('} packet_vkApiVersion;\n')
        if_body.append('static packet_vkApiVersion* interpret_body_as_vkApiVersion(vktrace_trace_packet_header* pHeader)')
        if_body.append('{')
        if_body.append('    packet_vkApiVersion* pPacket = (packet_vkApiVersion*)pHeader->pBody;')
        if_body.append('    pPacket->header = pHeader;')
        if_body.append('    return pPacket;')
        if_body.append('}\n')
        for proto in self.protos:
            if proto.name not in proto_exclusions:
                if 'UnmapMemory' == proto.name:
                    proto.params.append(vulkan.Param("void*", "pData"))
                elif 'FlushMappedMemoryRanges' == proto.name:
                    proto.params.append(vulkan.Param("void**", "ppData"))
                elif 'InvalidateMappedMemoryRanges' == proto.name:
                    proto.params.append(vulkan.Param("void**", "ppData"))
                if_body.append('%s' % self.lineinfo.get())
                if_body.append('typedef struct packet_vk%s {' % proto.name)
                if_body.append('    vktrace_trace_packet_header* header;')
                for p in proto.params:
                    if_body.append('    %s;' % p.c())
                if 'void' != proto.ret:
                    if_body.append('    %s result;' % proto.ret)
                if_body.append('} packet_vk%s;\n' % proto.name)
                if_body.append('static packet_vk%s* interpret_body_as_vk%s(vktrace_trace_packet_header* pHeader)' % (proto.name, proto.name))
                if_body.append('{')
                if_body.append('    packet_vk%s* pPacket = (packet_vk%s*)pHeader->pBody;' % (proto.name, proto.name))
                if_body.append('    pPacket->header = pHeader;')
                for p in proto.params:
                    if '*' in p.ty:
                        if 'DeviceCreateInfo' in p.ty:
                            if_body.append('    pPacket->%s = interpret_VkDeviceCreateInfo(pHeader, (intptr_t)pPacket->%s);' % (p.name, p.name))
                        elif 'InstanceCreateInfo' in p.ty:
                            if_body.append('    pPacket->%s = interpret_VkInstanceCreateInfo(pHeader, (intptr_t)pPacket->%s);' % (p.name, p.name))
                        else:
                            if_body.append('    pPacket->%s = (%s)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pPacket->%s);' % (p.name, p.ty, p.name))
                        # TODO : Generalize this custom code to kill dict data struct above.
                        #  Really the point of this block is to catch params w/ embedded ptrs to structs and chains of structs
                        if proto.name in custom_case_dict and p.name == custom_case_dict[proto.name]['param']:
                            if_body.append('    if (pPacket->%s != NULL)' % custom_case_dict[proto.name]['param'])
                            if_body.append('    {')
                            if_body.append('        %s' % "        ".join(custom_case_dict[proto.name]['txt']))
                            if_body.append('    }')
                if_body.append('    return pPacket;')
                if_body.append('}\n')
        return "\n".join(if_body)

class VktraceTraceC(Subcommand):
    def generate_header(self, extensionName):
        header_txt = []
        header_txt.append('#include "vktrace_platform.h"')
        header_txt.append('#include "vktrace_common.h"')
        header_txt.append('#include "vktrace_lib_helpers.h"')
        header_txt.append('#include "vktrace_lib_trim.h"')
        header_txt.append('#include "vktrace_vk_vk.h"')
        header_txt.append('#include "vktrace_interconnect.h"')
        header_txt.append('#include "vktrace_filelike.h"')
        header_txt.append('#include "vk_struct_size_helper.h"')
        header_txt.append('#ifdef PLATFORM_LINUX')
        header_txt.append('#include <pthread.h>')
        header_txt.append('#endif')
        header_txt.append('#include "vktrace_trace_packet_utils.h"')
        header_txt.append('#include <stdio.h>\n')
        header_txt.append('#include <string.h>\n')
        header_txt.append('#ifdef WIN32')
        header_txt.append('INIT_ONCE gInitOnce = INIT_ONCE_STATIC_INIT;')
        header_txt.append('#elif defined(PLATFORM_LINUX)')
        header_txt.append('pthread_once_t gInitOnce = PTHREAD_ONCE_INIT;')
        header_txt.append('#endif')
        return "\n".join(header_txt)

    def generate_body(self):
        body = [self._generate_init_funcs(),
                self._generate_trace_funcs(self.extensionName)]

        return "\n".join(body)

class VktraceCoreTracePackets(Subcommand):
    def generate_header(self, extensionName):
        header_txt = []
        header_txt.append('#pragma once\n')
        header_txt.append('#include "vulkan/vulkan.h"')
        header_txt.append('#include "vktrace_trace_packet_utils.h"\n')
        return "\n".join(header_txt)

    def generate_body(self):
        body = [self._generate_struct_util_funcs(),
                self._generate_interp_funcs()]

        return "\n".join(body)


def main():

    subcommands = {
            "vktrace-trace-c" : VktraceTraceC,
            "vktrace-core-trace-packets" : VktraceCoreTracePackets,
    }

    if len(sys.argv) < 3 or sys.argv[2] not in subcommands:
        print("Available subcommands are: %s" % " ".join(subcommands))
        exit(1)

    subcmd = subcommands[sys.argv[2]](sys.argv[3])
    subcmd.run()

if __name__ == "__main__":
    main()
