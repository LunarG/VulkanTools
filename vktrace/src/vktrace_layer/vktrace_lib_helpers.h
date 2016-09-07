/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * All Rights Reserved.
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
 */
#pragma once
#include <unordered_map>
#include "vktrace_vk_vk.h"
#include "vulkan/vk_layer.h"
#include "vktrace_platform.h"

#include "vk_struct_size_helper.h"

// Support for shadowing CPU mapped memory
//TODO better handling of multiple range rather than fixed array
typedef struct _VKAllocInfo {
    VkDeviceSize   totalSize;
    VkDeviceSize   rangeSize;
    VkDeviceSize   rangeOffset;
    BOOL           didFlush;
    VkDeviceMemory handle;
    uint8_t        *pData;
    BOOL           valid;
} VKAllocInfo;

typedef struct _VKMemInfo {
    unsigned int numEntrys;
    VKAllocInfo *pEntrys;
    VKAllocInfo *pLastMapped;
    unsigned int capacity;
} VKMemInfo;

typedef struct _layer_device_data {
    VkLayerDispatchTable devTable;
    bool KHRDeviceSwapchainEnabled;
} layer_device_data;
typedef struct _layer_instance_data {
    VkLayerInstanceDispatchTable instTable;
    bool LunargDebugReportEnabled;
    bool KHRSurfaceEnabled;
    bool KHRXcbSurfaceEnabled;
    bool KHRXlibSurfaceEnabled;
    bool KHRWaylandSurfaceEnabled;
    bool KHRMirSurfaceEnabled;
    bool KHRWin32SurfaceEnabled;
} layer_instance_data;

// defined in manually written file: vktrace_lib_trace.c
extern VKMemInfo g_memInfo;
extern VKTRACE_CRITICAL_SECTION g_memInfoLock;
extern std::unordered_map<void *, layer_device_data *> g_deviceDataMap;
extern std::unordered_map<void *, layer_instance_data *> g_instanceDataMap;

typedef void *dispatch_key;
inline dispatch_key get_dispatch_key(const void* object)
{
    return (dispatch_key) *(VkLayerDispatchTable **) object;
}

layer_instance_data *mid(void *object);
layer_device_data *mdd(void* object);

static void init_mem_info_entrys(VKAllocInfo *ptr, const unsigned int num)
{
    unsigned int i;
    for (i = 0; i < num; i++)
    {
        VKAllocInfo *entry = ptr + i;
        entry->pData = NULL;
        entry->totalSize = 0;
        entry->rangeSize = 0;
        entry->rangeOffset = 0;
        entry->didFlush = FALSE;
        memset(&entry->handle, 0, sizeof(VkDeviceMemory));
        entry->valid = FALSE;
    }
}

// caller must hold the g_memInfoLock
static void init_mem_info()
{
    g_memInfo.numEntrys = 0;
    g_memInfo.capacity = 4096;
    g_memInfo.pLastMapped = NULL;

    g_memInfo.pEntrys = VKTRACE_NEW_ARRAY(VKAllocInfo, g_memInfo.capacity);

    if (g_memInfo.pEntrys == NULL)
        vktrace_LogError("init_mem_info()  malloc failed.");
    else
        init_mem_info_entrys(g_memInfo.pEntrys, g_memInfo.capacity);
}

// caller must hold the g_memInfoLock
static void delete_mem_info()
{
    VKTRACE_DELETE(g_memInfo.pEntrys);
    g_memInfo.pEntrys = NULL;
    g_memInfo.numEntrys = 0;
    g_memInfo.capacity = 0;
    g_memInfo.pLastMapped = NULL;
}

// caller must hold the g_memInfoLock
static VKAllocInfo * get_mem_info_entry()
{
    unsigned int i;
    VKAllocInfo *entry;
    if (g_memInfo.numEntrys > g_memInfo.capacity)
    {
        vktrace_LogError("get_mem_info_entry() bad internal state numEntrys %u.", g_memInfo.numEntrys);
        return NULL;
    }

    entry = g_memInfo.pEntrys;
    for (i = 0; i < g_memInfo.numEntrys; i++)
    {
        if ((entry + i)->valid == FALSE)
            return entry + i;
    }
    if (g_memInfo.numEntrys == g_memInfo.capacity)
    {  // grow the array 2x
        g_memInfo.capacity *= 2;
        g_memInfo.pEntrys = (VKAllocInfo *) VKTRACE_REALLOC(g_memInfo.pEntrys, g_memInfo.capacity * sizeof(VKAllocInfo));
        if (g_memInfo.pEntrys == NULL)
            vktrace_LogError("get_mem_info_entry() realloc failed.");
        vktrace_LogDebug("realloc memInfo from %u to %u", g_memInfo.capacity /2, g_memInfo.capacity);
        //init the newly added entrys
        init_mem_info_entrys(g_memInfo.pEntrys + g_memInfo.capacity / 2, g_memInfo.capacity / 2);
    }

    assert(g_memInfo.numEntrys < g_memInfo.capacity);
    entry = g_memInfo.pEntrys + g_memInfo.numEntrys;
    g_memInfo.numEntrys++;
    assert(entry->valid == FALSE);
    return entry;
}

// caller must hold the g_memInfoLock
static VKAllocInfo * find_mem_info_entry(const VkDeviceMemory handle)
{
    VKAllocInfo *entry;
    unsigned int i;
    entry = g_memInfo.pEntrys;
    if (g_memInfo.pLastMapped && g_memInfo.pLastMapped->handle == handle && g_memInfo.pLastMapped->valid)
    {
        return g_memInfo.pLastMapped;
    }
    for (i = 0; i < g_memInfo.numEntrys; i++)
    {
        if ((entry + i)->valid && (handle == (entry + i)->handle))
        {
            return entry + i;
        }
    }

    return NULL;
}

static VKAllocInfo * find_mem_info_entry_lock(const VkDeviceMemory handle)
{
    VKAllocInfo *res;
    vktrace_enter_critical_section(&g_memInfoLock);
    res = find_mem_info_entry(handle);
    vktrace_leave_critical_section(&g_memInfoLock);
    return res;
}

static void add_new_handle_to_mem_info(const VkDeviceMemory handle, VkDeviceSize size, void *pData)
{
    VKAllocInfo *entry;

    vktrace_enter_critical_section(&g_memInfoLock);
    if (g_memInfo.capacity == 0)
        init_mem_info();

    entry = get_mem_info_entry();
    if (entry)
    {
        entry->valid = TRUE;
        entry->handle = handle;
        entry->totalSize = size;
        entry->rangeSize = 0;
        entry->rangeOffset = 0;
        entry->didFlush = FALSE;
        entry->pData = (uint8_t *) pData;   // NOTE: VKFreeMemory will free this mem, so no malloc()
    }
    vktrace_leave_critical_section(&g_memInfoLock);
}

static void add_data_to_mem_info(const VkDeviceMemory handle, VkDeviceSize rangeSize, VkDeviceSize rangeOffset, void *pData)
{
    VKAllocInfo *entry;

    vktrace_enter_critical_section(&g_memInfoLock);
    entry = find_mem_info_entry(handle);
    if (entry)
    {
        entry->pData = (uint8_t *)pData;
        if (rangeSize == VK_WHOLE_SIZE)
            entry->rangeSize = entry->totalSize - rangeOffset;
        else
            entry->rangeSize = rangeSize;
        entry->rangeOffset = rangeOffset;
        assert(entry->totalSize >= entry->rangeSize + rangeOffset);
    }
    g_memInfo.pLastMapped = entry;
    vktrace_leave_critical_section(&g_memInfoLock);
}

static void rm_handle_from_mem_info(const VkDeviceMemory handle)
{
    VKAllocInfo *entry;

    vktrace_enter_critical_section(&g_memInfoLock);
    entry = find_mem_info_entry(handle);
    if (entry)
    {
        entry->valid = FALSE;
        entry->pData = NULL;
        entry->totalSize = 0;
        entry->rangeSize = 0;
        entry->rangeOffset = 0;
        entry->didFlush = FALSE;
        memset(&entry->handle, 0, sizeof(VkDeviceMemory));

        if (entry == g_memInfo.pLastMapped)
            g_memInfo.pLastMapped = NULL;
        // adjust numEntrys to be last valid entry in list
        do {
            entry =  g_memInfo.pEntrys + g_memInfo.numEntrys - 1;
            if (entry->valid == FALSE)
                g_memInfo.numEntrys--;
        } while ((entry->valid == FALSE) && (g_memInfo.numEntrys > 0));
        if (g_memInfo.numEntrys == 0)
            delete_mem_info();
    }
    vktrace_leave_critical_section(&g_memInfoLock);
}

static void add_alloc_memory_to_trace_packet(vktrace_trace_packet_header* pHeader, void** ppOut, const void* pIn)
{
    return;
}

static void add_VkPipelineShaderStageCreateInfo_to_trace_packet(vktrace_trace_packet_header* pHeader, VkPipelineShaderStageCreateInfo* packetShader, const VkPipelineShaderStageCreateInfo* paramShader)
{
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&packetShader->pName, ROUNDUP_TO_4(strlen(paramShader->pName) + 1), paramShader->pName);
    vktrace_finalize_buffer_address(pHeader, (void**)&packetShader->pName);

    // Specialization info
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&packetShader->pSpecializationInfo, sizeof(VkSpecializationInfo), paramShader->pSpecializationInfo);
    if (packetShader->pSpecializationInfo != NULL)
    {
        if (paramShader->pSpecializationInfo != NULL) {
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&packetShader->pSpecializationInfo->pMapEntries, sizeof(VkSpecializationMapEntry) * paramShader->pSpecializationInfo->mapEntryCount, paramShader->pSpecializationInfo->pMapEntries);
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&packetShader->pSpecializationInfo->pData, paramShader->pSpecializationInfo->dataSize, paramShader->pSpecializationInfo->pData);
            vktrace_finalize_buffer_address(pHeader, (void**)&packetShader->pSpecializationInfo->pMapEntries);
            vktrace_finalize_buffer_address(pHeader, (void**)&packetShader->pSpecializationInfo->pData);
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&packetShader->pSpecializationInfo);
}

static void add_create_ds_layout_to_trace_packet(vktrace_trace_packet_header* pHeader, const VkDescriptorSetLayoutCreateInfo** ppOut, const VkDescriptorSetLayoutCreateInfo* pIn)
{
    uint32_t i;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)(ppOut), sizeof(VkDescriptorSetLayoutCreateInfo), pIn);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppOut)->pBindings), sizeof(VkDescriptorSetLayoutBinding) * pIn->bindingCount, pIn->pBindings);
    for (i = 0; i < pIn->bindingCount; i++) {
        if (pIn->pBindings[i].pImmutableSamplers != NULL &&
                (pIn->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||
                 pIn->pBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)) {
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppOut)->pBindings[i].pImmutableSamplers),
                                               sizeof(VkSampler) * pIn->pBindings[i].descriptorCount,
                                               pIn->pBindings[i].pImmutableSamplers);
            vktrace_finalize_buffer_address(pHeader, (void**)&((*ppOut)->pBindings[i].pImmutableSamplers));
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&((*ppOut)->pBindings));
    vktrace_finalize_buffer_address(pHeader, (void**)(ppOut));
    return;
}

static void add_VkGraphicsPipelineCreateInfos_to_trace_packet(vktrace_trace_packet_header* pHeader, VkGraphicsPipelineCreateInfo* pPacket, const VkGraphicsPipelineCreateInfo* pParam, uint32_t count)
{
    if (pParam != NULL)
    {
        uint32_t i;
        uint32_t j;

        for (i = 0; i < count; i++) {
            // shader stages array
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pStages), sizeof(VkPipelineShaderStageCreateInfo) * pParam[i].stageCount, pParam[i].pStages);
            for (j = 0; j < pParam[i].stageCount; j++)
            {
                add_VkPipelineShaderStageCreateInfo_to_trace_packet(pHeader, (VkPipelineShaderStageCreateInfo*)&pPacket->pStages[j], &pParam->pStages[j]);
            }
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pStages));

            // Vertex Input State
            if (pParam[i].pVertexInputState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pVertexInputState), sizeof(VkPipelineVertexInputStateCreateInfo), pParam[i].pVertexInputState);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pVertexInputState->pVertexBindingDescriptions), pParam[i].pVertexInputState->vertexBindingDescriptionCount * sizeof(VkVertexInputBindingDescription), pParam[i].pVertexInputState->pVertexBindingDescriptions);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pVertexInputState->pVertexBindingDescriptions));
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pVertexInputState->pVertexAttributeDescriptions), pParam[i].pVertexInputState->vertexAttributeDescriptionCount * sizeof(VkVertexInputAttributeDescription), pParam[i].pVertexInputState->pVertexAttributeDescriptions);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pVertexInputState->pVertexAttributeDescriptions));
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pVertexInputState));
            }
            // Input Assembly State
            if (pParam[i].pInputAssemblyState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pInputAssemblyState), sizeof(VkPipelineInputAssemblyStateCreateInfo), pParam[i].pInputAssemblyState);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pInputAssemblyState));
            }
            // Tesselation State
            if (pParam[i].pTessellationState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pTessellationState), sizeof(VkPipelineTessellationStateCreateInfo), pParam[i].pTessellationState);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pTessellationState));
            }
            // Viewport State
            if (pParam[i].pViewportState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pViewportState), sizeof(VkPipelineViewportStateCreateInfo), pParam[i].pViewportState);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pViewportState->pViewports), sizeof(VkViewport) * pParam[i].pViewportState->viewportCount,
                                                   pParam[i].pViewportState->pViewports);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pViewportState->pViewports));
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pViewportState->pScissors), sizeof(VkRect2D) * pParam[i].pViewportState->scissorCount,
                                                   pParam[i].pViewportState->pScissors);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pViewportState->pScissors));
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pViewportState));
            }

            // Raster State
            if (pParam[i].pRasterizationState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pRasterizationState), sizeof(VkPipelineRasterizationStateCreateInfo), pParam[i].pRasterizationState);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pRasterizationState));
            }
            // MultiSample State
            if (pParam[i].pMultisampleState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMultisampleState), sizeof(VkPipelineMultisampleStateCreateInfo), pParam[i].pMultisampleState);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMultisampleState->pSampleMask), sizeof(VkSampleMask), pParam[i].pMultisampleState->pSampleMask);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMultisampleState->pSampleMask));
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMultisampleState));
            }

            // DepthStencil State
            if (pParam[i].pDepthStencilState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDepthStencilState), sizeof(VkPipelineDepthStencilStateCreateInfo), pParam[i].pDepthStencilState);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDepthStencilState));
            }

            // ColorBlend State
            if (pParam[i].pColorBlendState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pColorBlendState), sizeof(VkPipelineColorBlendStateCreateInfo), pParam[i].pColorBlendState);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pColorBlendState->pAttachments), pParam[i].pColorBlendState->attachmentCount * sizeof(VkPipelineColorBlendAttachmentState), pParam[i].pColorBlendState->pAttachments);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pColorBlendState->pAttachments));
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pColorBlendState));
            }

            // DynamicState
            if (pParam[i].pDynamicState) {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDynamicState), sizeof(VkPipelineDynamicStateCreateInfo), pParam[i].pDynamicState);
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDynamicState->pDynamicStates), pParam[i].pDynamicState->dynamicStateCount * sizeof(VkDynamicState), pParam[i].pDynamicState->pDynamicStates);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDynamicState->pDynamicStates));
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDynamicState));
            }
        }
    }
    return;
}

static void add_VkComputePipelineCreateInfos_to_trace_packet(vktrace_trace_packet_header* pHeader, VkComputePipelineCreateInfo* pPacket, const VkComputePipelineCreateInfo* pParam, uint32_t count)
{
    if (pParam != NULL)
    {
        uint32_t i;

        for (i = 0; i < count; i++) {
            // shader stage
            add_VkPipelineShaderStageCreateInfo_to_trace_packet(pHeader, (VkPipelineShaderStageCreateInfo*)&pPacket->stage, &pParam[i].stage);
        }
    }
    return;
}
