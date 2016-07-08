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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */
#include <stdbool.h>
#include <unordered_map>
#include "vktrace_vk_vk.h"
#include "vulkan/vulkan.h"
#include "vktrace_platform.h"
#include "vk_dispatch_table_helper.h"
#include "vktrace_common.h"
#include "vktrace_lib_helpers.h"

#include "vktrace_interconnect.h"
#include "vktrace_filelike.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_exts.h"
#include <stdio.h>

// declared as extern in vktrace_lib_helpers.h
VKTRACE_CRITICAL_SECTION g_memInfoLock;
VKMemInfo g_memInfo = {0, NULL, NULL, 0};


std::unordered_map<void *, layer_device_data *> g_deviceDataMap;
std::unordered_map<void *, layer_instance_data *> g_instanceDataMap;


layer_instance_data *mid(void *object)
{
    dispatch_key key = get_dispatch_key(object);
    std::unordered_map<void *, layer_instance_data *>::const_iterator got;
    got = g_instanceDataMap.find(key);
    assert(got != g_instanceDataMap.end());
    return got->second;
}

layer_device_data *mdd(void* object)
{
    dispatch_key key = get_dispatch_key(object);
    std::unordered_map<void *, layer_device_data *>::const_iterator got;
    got = g_deviceDataMap.find(key);
    assert(got != g_deviceDataMap.end());
    return got->second;
}

static layer_instance_data *initInstanceData(
                                    VkInstance instance,
                                    const PFN_vkGetInstanceProcAddr gpa,
                                    std::unordered_map<void *, layer_instance_data *> &map)
{
    layer_instance_data *pTable;
    assert(instance);
    dispatch_key key = get_dispatch_key(instance);

    std::unordered_map<void *, layer_instance_data *>::const_iterator it = map.find(key);
    if (it == map.end())
    {
        pTable =  new layer_instance_data();
        map[key] = pTable;
    } else
    {
        return it->second;
    }

    // TODO: Convert to new init method
    layer_init_instance_dispatch_table(instance, &pTable->instTable, gpa);

    return pTable;
}

static layer_device_data *initDeviceData(
        VkDevice device,
        const PFN_vkGetDeviceProcAddr gpa,
        std::unordered_map<void *, layer_device_data *> &map)
{
    layer_device_data *pTable;
    dispatch_key key = get_dispatch_key(device);

    std::unordered_map<void *, layer_device_data *>::const_iterator it = map.find(key);
    if (it == map.end())
    {
        pTable =  new layer_device_data();
        map[key] = pTable;
    } else
    {
        return it->second;
    }

    layer_init_device_dispatch_table(device, &pTable->devTable, gpa);

    return pTable;
}

/*
 * This function will return the pNext pointer of any
 * CreateInfo extensions that are not loader extensions.
 * This is used to skip past the loader extensions prepended
 * to the list during CreateInstance and CreateDevice.
 */
void *strip_create_extensions(const void *pNext)
{
    VkLayerInstanceCreateInfo *create_info = (VkLayerInstanceCreateInfo *) pNext;

    while (create_info && (create_info->sType == VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO ||
           create_info->sType == VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO)) {
        create_info = (VkLayerInstanceCreateInfo *) create_info->pNext;
    }

    return create_info;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkAllocateMemory(
    VkDevice device,
    const VkMemoryAllocateInfo* pAllocateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDeviceMemory* pMemory)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkAllocateMemory* pPacket = NULL;
    CREATE_TRACE_PACKET(vkAllocateMemory, get_struct_chain_size((void*)pAllocateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkDeviceMemory));
    result = mdd(device)->devTable.AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkAllocateMemory(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkMemoryAllocateInfo), pAllocateInfo);
    add_alloc_memory_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo->pNext), pAllocateInfo->pNext);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemory), sizeof(VkDeviceMemory), pMemory);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemory));
    FINISH_TRACE_PACKET();
    // begin custom code
    add_new_handle_to_mem_info(*pMemory, pAllocateInfo->allocationSize, NULL);
    // end custom code
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkMapMemory(
    VkDevice device,
    VkDeviceMemory memory,
    VkDeviceSize offset,
    VkDeviceSize size,
    VkFlags flags,
    void** ppData)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkMapMemory* pPacket = NULL;
    VKAllocInfo *entry;
    CREATE_TRACE_PACKET(vkMapMemory, sizeof(void*));
    result = mdd(device)->devTable.MapMemory(device, memory, offset, size, flags, ppData);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    entry = find_mem_info_entry(memory);

    // For vktrace usage, clamp the memory size to the total size less offset if VK_WHOLE_SIZE is specified.
    if (size == VK_WHOLE_SIZE) {
        size = entry->totalSize - offset;
    }
    pPacket = interpret_body_as_vkMapMemory(pHeader);
    pPacket->device = device;
    pPacket->memory = memory;
    pPacket->offset = offset;
    pPacket->size = size;
    pPacket->flags = flags;
    if (ppData != NULL)
    {
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData), sizeof(void*), *ppData);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData));
        add_data_to_mem_info(memory, size, offset, *ppData);
    }
    pPacket->result = result;
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkUnmapMemory(
    VkDevice device,
    VkDeviceMemory memory)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkUnmapMemory* pPacket;
    VKAllocInfo *entry;
    size_t siz = 0;
    uint64_t trace_begin_time = vktrace_get_time();

    // insert into packet the data that was written by CPU between the vkMapMemory call and here
    // Note must do this prior to the real vkUnMap() or else may get a FAULT
    vktrace_enter_critical_section(&g_memInfoLock);
    entry = find_mem_info_entry(memory);
    if (entry && entry->pData != NULL)
    {
        if (!entry->didFlush)
        {
            // no FlushMapped Memory
            siz = (size_t)entry->rangeSize;
        }
    }
    CREATE_TRACE_PACKET(vkUnmapMemory, siz);
    pHeader->vktrace_begin_time = trace_begin_time;
    pPacket = interpret_body_as_vkUnmapMemory(pHeader);
    if (siz)
    {
        assert(entry->handle == memory);
        vktrace_add_buffer_to_trace_packet(pHeader, (void**) &(pPacket->pData), siz, entry->pData);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pData));
        entry->pData = NULL;
    }
    vktrace_leave_critical_section(&g_memInfoLock);
    pHeader->entrypoint_begin_time = vktrace_get_time();
    mdd(device)->devTable.UnmapMemory(device, memory);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket->device = device;
    pPacket->memory = memory;
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkFreeMemory(
    VkDevice device,
    VkDeviceMemory memory,
    const VkAllocationCallbacks* pAllocator)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkFreeMemory* pPacket = NULL;
    CREATE_TRACE_PACKET(vkFreeMemory, sizeof(VkAllocationCallbacks));
    mdd(device)->devTable.FreeMemory(device, memory, pAllocator);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkFreeMemory(pHeader);
    pPacket->device = device;
    pPacket->memory = memory;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    FINISH_TRACE_PACKET();
    // begin custom code
    rm_handle_from_mem_info(memory);
    // end custom code
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkFlushMappedMemoryRanges(
    VkDevice device,
    uint32_t memoryRangeCount,
    const VkMappedMemoryRange* pMemoryRanges)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    size_t rangesSize = 0;
    size_t dataSize = 0;
    uint32_t iter;
    packet_vkFlushMappedMemoryRanges* pPacket = NULL;
    uint64_t trace_begin_time = vktrace_get_time();

    // find out how much memory is in the ranges
    for (iter = 0; iter < memoryRangeCount; iter++)
    {
        VkMappedMemoryRange* pRange = (VkMappedMemoryRange*)&pMemoryRanges[iter];
        rangesSize += vk_size_vkmappedmemoryrange(pRange);
        dataSize += (size_t)pRange->size;
    }

    CREATE_TRACE_PACKET(vkFlushMappedMemoryRanges, rangesSize + sizeof(void*)*memoryRangeCount + dataSize);
    pHeader->vktrace_begin_time = trace_begin_time;
    pPacket = interpret_body_as_vkFlushMappedMemoryRanges(pHeader);

    vktrace_add_buffer_to_trace_packet(pHeader, (void**) &(pPacket->pMemoryRanges), rangesSize, pMemoryRanges);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemoryRanges));

    // insert into packet the data that was written by CPU between the vkMapMemory call and here
    // create a temporary local ppData array and add it to the packet (to reserve the space for the array)
    void** ppTmpData = (void **) malloc(memoryRangeCount * sizeof(void*));
    vktrace_add_buffer_to_trace_packet(pHeader, (void**) &(pPacket->ppData), sizeof(void*)*memoryRangeCount, ppTmpData);
    free(ppTmpData);

    // now the actual memory
    vktrace_enter_critical_section(&g_memInfoLock);
    for (iter = 0; iter < memoryRangeCount; iter++)
    {
        VkMappedMemoryRange* pRange = (VkMappedMemoryRange*)&pMemoryRanges[iter];
        VKAllocInfo* pEntry = find_mem_info_entry(pRange->memory);

        if (pEntry != NULL)
        {
            assert(pEntry->handle == pRange->memory);
            assert(pEntry->totalSize >= (pRange->size + pRange->offset));
            assert(pEntry->totalSize >= pRange->size);
            assert(pRange->offset >= pEntry->rangeOffset && (pRange->offset + pRange->size) <= (pEntry->rangeOffset + pEntry->rangeSize));
            vktrace_add_buffer_to_trace_packet(pHeader, (void**) &(pPacket->ppData[iter]), pRange->size, pEntry->pData + pRange->offset);
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData[iter]));
            pEntry->didFlush = TRUE;
        }
        else
        {
             vktrace_LogError("Failed to copy app memory into trace packet (idx = %u) on vkFlushedMappedMemoryRanges", pHeader->global_packet_index);
        }
    }
    vktrace_leave_critical_section(&g_memInfoLock);

    // now finalize the ppData array since it is done being updated
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData));

    pHeader->entrypoint_begin_time = vktrace_get_time();
    result = mdd(device)->devTable.FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket->device = device;
    pPacket->memoryRangeCount = memoryRangeCount;
    pPacket->result = result;
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkAllocateCommandBuffers(
    VkDevice device,
    const VkCommandBufferAllocateInfo* pAllocateInfo,
    VkCommandBuffer* pCommandBuffers)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkAllocateCommandBuffers* pPacket = NULL;
    CREATE_TRACE_PACKET(vkAllocateCommandBuffers, get_struct_chain_size((void*)pAllocateInfo) + sizeof(VkCommandBuffer) * pAllocateInfo->commandBufferCount);
    result = mdd(device)->devTable.AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkAllocateCommandBuffers(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkCommandBufferAllocateInfo), pAllocateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCommandBuffers), sizeof(VkCommandBuffer) * pAllocateInfo->commandBufferCount, pCommandBuffers);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCommandBuffers));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkBeginCommandBuffer(
    VkCommandBuffer commandBuffer,
    const VkCommandBufferBeginInfo* pBeginInfo)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkBeginCommandBuffer* pPacket = NULL;
    CREATE_TRACE_PACKET(vkBeginCommandBuffer, get_struct_chain_size((void*)pBeginInfo));
    result = mdd(commandBuffer)->devTable.BeginCommandBuffer(commandBuffer, pBeginInfo);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkBeginCommandBuffer(pHeader);
    pPacket->commandBuffer = commandBuffer;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBeginInfo), sizeof(VkCommandBufferBeginInfo), pBeginInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBeginInfo->pInheritanceInfo), sizeof(VkCommandBufferInheritanceInfo), pBeginInfo->pInheritanceInfo);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBeginInfo->pInheritanceInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBeginInfo));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateDescriptorPool(
    VkDevice device,
    const VkDescriptorPoolCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDescriptorPool* pDescriptorPool)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateDescriptorPool* pPacket = NULL;
    // begin custom code (needs to use get_struct_chain_size)
    CREATE_TRACE_PACKET(vkCreateDescriptorPool,  get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkDescriptorPool));
    // end custom code
    result = mdd(device)->devTable.CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateDescriptorPool(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkDescriptorPoolCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pPoolSizes), pCreateInfo->poolSizeCount * sizeof(VkDescriptorPoolSize), pCreateInfo->pPoolSizes);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorPool), sizeof(VkDescriptorPool), pDescriptorPool);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pPoolSizes));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorPool));
    FINISH_TRACE_PACKET();
    return result;
}

VkLayerDeviceCreateInfo *get_chain_info(const VkDeviceCreateInfo *pCreateInfo, VkLayerFunction func)
{
    VkLayerDeviceCreateInfo *chain_info = (VkLayerDeviceCreateInfo *) pCreateInfo->pNext;
    while (chain_info && !(chain_info->sType == VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO
           && chain_info->function == func)) {
        chain_info = (VkLayerDeviceCreateInfo *) chain_info->pNext;
    }
    assert(chain_info != NULL);
    return chain_info;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateDevice(
    VkPhysicalDevice physicalDevice,
    const VkDeviceCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDevice* pDevice)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateDevice* pPacket = NULL;
    uint32_t i;

    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr);
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    assert(fpGetDeviceProcAddr);
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(NULL, "vkCreateDevice");
    if (fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if (result != VK_SUCCESS) {
        return result;
    }
 
    initDeviceData(*pDevice, fpGetDeviceProcAddr, g_deviceDataMap);
    // Setup device dispatch table for extensions
    ext_init_create_device(mdd(*pDevice), *pDevice, fpGetDeviceProcAddr, pCreateInfo->enabledExtensionCount, pCreateInfo->ppEnabledExtensionNames);

    // remove the loader extended createInfo structure
    VkDeviceCreateInfo localCreateInfo;
    memcpy(&localCreateInfo, pCreateInfo, sizeof(localCreateInfo));
    for (i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
        char **ppName = (char **) &localCreateInfo.ppEnabledExtensionNames[i];
        *ppName = (char *) pCreateInfo->ppEnabledExtensionNames[i];
    }
    for (i = 0; i < pCreateInfo->enabledLayerCount; i++) {
        char **ppName = (char **) &localCreateInfo.ppEnabledLayerNames[i];
        *ppName = (char *) pCreateInfo->ppEnabledLayerNames[i];
    }
    localCreateInfo.pNext = strip_create_extensions(pCreateInfo->pNext);

    CREATE_TRACE_PACKET(vkCreateDevice, get_struct_chain_size((void*)&localCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkDevice));
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateDevice(pHeader);
    pPacket->physicalDevice = physicalDevice;
    add_VkDeviceCreateInfo_to_packet(pHeader, (VkDeviceCreateInfo**) &(pPacket->pCreateInfo), &localCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDevice), sizeof(VkDevice), pDevice);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDevice));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateFramebuffer(
    VkDevice device,
    const VkFramebufferCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkFramebuffer* pFramebuffer)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateFramebuffer* pPacket = NULL;
    // begin custom code
    uint32_t attachmentCount = (pCreateInfo != NULL && pCreateInfo->pAttachments != NULL) ? pCreateInfo->attachmentCount : 0;
    CREATE_TRACE_PACKET(vkCreateFramebuffer, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkFramebuffer));
    // end custom code
    result = mdd(device)->devTable.CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateFramebuffer(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkFramebufferCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pAttachments), attachmentCount * sizeof(VkImageView), pCreateInfo->pAttachments);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pFramebuffer), sizeof(VkFramebuffer), pFramebuffer);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pAttachments));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pFramebuffer));
    FINISH_TRACE_PACKET();
    return result;
}

VkLayerInstanceCreateInfo *get_chain_info(const VkInstanceCreateInfo *pCreateInfo, VkLayerFunction func)
{
    VkLayerInstanceCreateInfo *chain_info = (VkLayerInstanceCreateInfo *) pCreateInfo->pNext;
    while (chain_info && ((chain_info->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO)
           || (chain_info->function != func))) {
        chain_info = (VkLayerInstanceCreateInfo *) chain_info->pNext;
    }
    assert(chain_info != NULL);
    return chain_info;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateInstance(
    const VkInstanceCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkInstance* pInstance)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateInstance* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint32_t i;
    uint64_t vktraceStartTime = vktrace_get_time();
    SEND_ENTRYPOINT_ID(vkCreateInstance);
    startTime = vktrace_get_time();

    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(*pInstance, "vkCreateInstance");
    if (fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result != VK_SUCCESS) {
        return result;
    }
    endTime = vktrace_get_time();

    initInstanceData(*pInstance, fpGetInstanceProcAddr, g_instanceDataMap);
    ext_init_create_instance(mid(*pInstance), *pInstance, pCreateInfo->enabledExtensionCount, pCreateInfo->ppEnabledExtensionNames);

    // remove the loader extended createInfo structure
    VkInstanceCreateInfo localCreateInfo;
    memcpy(&localCreateInfo, pCreateInfo, sizeof(localCreateInfo));
    for (i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
        char **ppName = (char **) &localCreateInfo.ppEnabledExtensionNames[i];
        *ppName = (char *) pCreateInfo->ppEnabledExtensionNames[i];
    }
    for (i = 0; i < pCreateInfo->enabledLayerCount; i++) {
        char **ppName = (char **) &localCreateInfo.ppEnabledLayerNames[i];
        *ppName = (char *) pCreateInfo->ppEnabledLayerNames[i];
    }
    //localCreateInfo.pNext = strip_create_extensions(pCreateInfo->pNext);
    // The pNext pointer isn't getting marshalled into the trace buffer properly anyway, so
    // set it to NULL so that replay does not trip over it.
    localCreateInfo.pNext = NULL;
    CREATE_TRACE_PACKET(vkCreateInstance, sizeof(VkInstance) + get_struct_chain_size((void*)&localCreateInfo) + sizeof(VkAllocationCallbacks));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkCreateInstance(pHeader);

    add_VkInstanceCreateInfo_to_packet(pHeader, (VkInstanceCreateInfo**)&(pPacket->pCreateInfo), (VkInstanceCreateInfo*) &localCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pInstance), sizeof(VkInstance), pInstance);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pInstance));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateRenderPass(
    VkDevice device,
    const VkRenderPassCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkRenderPass* pRenderPass)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateRenderPass* pPacket = NULL;
    // begin custom code (get_struct_chain_size)
    uint32_t attachmentCount = (pCreateInfo != NULL && (pCreateInfo->pAttachments != NULL)) ? pCreateInfo->attachmentCount : 0;
    uint32_t dependencyCount = (pCreateInfo != NULL && (pCreateInfo->pDependencies != NULL)) ? pCreateInfo->dependencyCount : 0;
    uint32_t subpassCount = (pCreateInfo != NULL && (pCreateInfo->pSubpasses != NULL)) ? pCreateInfo->subpassCount : 0;
    CREATE_TRACE_PACKET(vkCreateRenderPass, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkRenderPass));
    // end custom code
    result = mdd(device)->devTable.CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateRenderPass(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkRenderPassCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pAttachments), attachmentCount * sizeof(VkAttachmentDescription), pCreateInfo->pAttachments);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pDependencies), dependencyCount * sizeof(VkSubpassDependency), pCreateInfo->pDependencies);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pSubpasses), subpassCount * sizeof(VkSubpassDescription), pCreateInfo->pSubpasses);
    uint32_t i;
    for (i=0; i < pPacket->pCreateInfo->subpassCount; i++) {
        VkSubpassDescription *pSubpass = (VkSubpassDescription *) &pPacket->pCreateInfo->pSubpasses[i];
        const VkSubpassDescription *pSp = &pCreateInfo->pSubpasses[i];
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pSubpass->pInputAttachments), pSubpass->inputAttachmentCount * sizeof(VkAttachmentReference), pSp->pInputAttachments);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pSubpass->pInputAttachments));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pSubpass->pColorAttachments), pSubpass->colorAttachmentCount * sizeof(VkAttachmentReference), pSp->pColorAttachments);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pSubpass->pColorAttachments));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pSubpass->pResolveAttachments), pSubpass->colorAttachmentCount * sizeof(VkAttachmentReference), pSp->pResolveAttachments);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pSubpass->pResolveAttachments));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pSubpass->pDepthStencilAttachment), 1 * sizeof(VkAttachmentReference), pSp->pDepthStencilAttachment);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pSubpass->pDepthStencilAttachment));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pSubpass->pPreserveAttachments), pSubpass->preserveAttachmentCount * sizeof(VkAttachmentReference), pSp->pPreserveAttachments);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pSubpass->pPreserveAttachments));
    }
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pRenderPass), sizeof(VkRenderPass), pRenderPass);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pAttachments));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pDependencies));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pSubpasses));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pRenderPass));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkEnumerateDeviceExtensionProperties(
    VkPhysicalDevice physicalDevice,
    const char* pLayerName,
    uint32_t* pPropertyCount,
    VkExtensionProperties* pProperties)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkEnumerateDeviceExtensionProperties* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    // Only call down chain if querying ICD rather than layer device extensions
    if (pLayerName == NULL)
        result = mid(physicalDevice)->instTable.EnumerateDeviceExtensionProperties(physicalDevice, NULL, pPropertyCount, pProperties);
    else
    {
        *pPropertyCount = 0;
        return VK_SUCCESS;
    }
    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkEnumerateDeviceExtensionProperties, ((pLayerName != NULL) ? strlen(pLayerName) + 1 : 0) + sizeof(uint32_t) + (*pPropertyCount * sizeof(VkExtensionProperties)));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkEnumerateDeviceExtensionProperties(pHeader);
    pPacket->physicalDevice = physicalDevice;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pLayerName), ((pLayerName != NULL) ? strlen(pLayerName) + 1 : 0), pLayerName);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPropertyCount), sizeof(uint32_t), pPropertyCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pProperties), *pPropertyCount * sizeof(VkExtensionProperties), pProperties);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pLayerName));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPropertyCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pProperties));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkEnumerateDeviceLayerProperties(
    VkPhysicalDevice physicalDevice,
    uint32_t* pPropertyCount,
    VkLayerProperties* pProperties)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkEnumerateDeviceLayerProperties* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    result = mid(physicalDevice)->instTable.EnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkEnumerateDeviceLayerProperties, sizeof(uint32_t) + (*pPropertyCount * sizeof(VkLayerProperties)));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkEnumerateDeviceLayerProperties(pHeader);
    pPacket->physicalDevice = physicalDevice;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPropertyCount), sizeof(uint32_t), pPropertyCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pProperties), *pPropertyCount * sizeof(VkLayerProperties), pProperties);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPropertyCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pProperties));
    FINISH_TRACE_PACKET();
    return result;
}
// TODO : This should be pretty easy to fit into codegen. Don't need to make the call prior to creating packet
//  Just need to account for "count" number of queue properties
VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkGetPhysicalDeviceQueueFamilyProperties(
    VkPhysicalDevice physicalDevice,
    uint32_t* pQueueFamilyPropertyCount,
    VkQueueFamilyProperties* pQueueFamilyProperties)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkGetPhysicalDeviceQueueFamilyProperties* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    mid(physicalDevice)->instTable.GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceQueueFamilyProperties, sizeof(uint32_t) + *pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetPhysicalDeviceQueueFamilyProperties(pHeader);
    pPacket->physicalDevice = physicalDevice;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pQueueFamilyPropertyCount), sizeof(uint32_t), pQueueFamilyPropertyCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pQueueFamilyProperties), *pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties), pQueueFamilyProperties);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pQueueFamilyPropertyCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pQueueFamilyProperties));
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkEnumeratePhysicalDevices(
    VkInstance instance,
    uint32_t* pPhysicalDeviceCount,
    VkPhysicalDevice* pPhysicalDevices)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkEnumeratePhysicalDevices* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    //TODO make sure can handle being called twice with pPD == 0
    SEND_ENTRYPOINT_ID(vkEnumeratePhysicalDevices);
    startTime = vktrace_get_time();
    result = mid(instance)->instTable.EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkEnumeratePhysicalDevices, sizeof(uint32_t) + ((pPhysicalDevices && pPhysicalDeviceCount) ? *pPhysicalDeviceCount * sizeof(VkPhysicalDevice) : 0));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkEnumeratePhysicalDevices(pHeader);
    pPacket->instance = instance;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPhysicalDeviceCount), sizeof(uint32_t), pPhysicalDeviceCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPhysicalDevices), *pPhysicalDeviceCount*sizeof(VkPhysicalDevice), pPhysicalDevices);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPhysicalDeviceCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPhysicalDevices));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetQueryPoolResults(
    VkDevice device,
    VkQueryPool queryPool,
    uint32_t firstQuery,
    uint32_t queryCount,
    size_t dataSize,
    void* pData,
    VkDeviceSize stride,
    VkQueryResultFlags flags)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkGetQueryPoolResults* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    result = mdd(device)->devTable.GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkGetQueryPoolResults, dataSize);
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetQueryPoolResults(pHeader);
    pPacket->device = device;
    pPacket->queryPool = queryPool;
    pPacket->firstQuery = firstQuery;
    pPacket->queryCount = queryCount;
    pPacket->dataSize = dataSize;
    pPacket->stride = stride;
    pPacket->flags = flags;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pData), dataSize, pData);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pData));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkAllocateDescriptorSets(
    VkDevice device,
    const VkDescriptorSetAllocateInfo* pAllocateInfo,
    VkDescriptorSet* pDescriptorSets)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkAllocateDescriptorSets* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    SEND_ENTRYPOINT_ID(vkAllocateDescriptorSets);
    startTime = vktrace_get_time();
    result = mdd(device)->devTable.AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
    endTime = vktrace_get_time();
    CREATE_TRACE_PACKET(vkAllocateDescriptorSets, vk_size_vkdescriptorsetallocateinfo(pAllocateInfo) + (pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSet)));
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkAllocateDescriptorSets(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo), sizeof(VkDescriptorSetAllocateInfo), pAllocateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocateInfo->pSetLayouts), pPacket->pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSetLayout), pAllocateInfo->pSetLayouts);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorSets), pPacket->pAllocateInfo->descriptorSetCount * sizeof(VkDescriptorSet), pDescriptorSets);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo->pSetLayouts));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorSets));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocateInfo));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkUpdateDescriptorSets(
VkDevice device,
        uint32_t descriptorWriteCount,
        const VkWriteDescriptorSet* pDescriptorWrites,
        uint32_t descriptorCopyCount,
        const VkCopyDescriptorSet* pDescriptorCopies);
// Manually written because it needs to use get_struct_chain_size and allocate some extra pointers (why?)
// Also since it needs to app the array of pointers and sub-buffers (see comments in function)
VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkUpdateDescriptorSets(
    VkDevice device,
    uint32_t descriptorWriteCount,
    const VkWriteDescriptorSet* pDescriptorWrites,
    uint32_t descriptorCopyCount,
    const VkCopyDescriptorSet* pDescriptorCopies )
{
    vktrace_trace_packet_header* pHeader;
    packet_vkUpdateDescriptorSets* pPacket = NULL;
    // begin custom code
    size_t arrayByteCount = 0;
    size_t i;

    for (i = 0; i < descriptorWriteCount; i++)
    {
        arrayByteCount += get_struct_chain_size(&pDescriptorWrites[i]);
    }

    for (i = 0; i < descriptorCopyCount; i++)
    {
        arrayByteCount += get_struct_chain_size(&pDescriptorCopies[i]);
    }

    CREATE_TRACE_PACKET(vkUpdateDescriptorSets, arrayByteCount);
    // end custom code

    mdd(device)->devTable.UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkUpdateDescriptorSets(pHeader);
    pPacket->device = device;
    pPacket->descriptorWriteCount = descriptorWriteCount;
    // begin custom code
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites), descriptorWriteCount * sizeof(VkWriteDescriptorSet), pDescriptorWrites);
    for (i = 0; i < descriptorWriteCount; i++)
    {
        switch (pPacket->pDescriptorWrites[i].descriptorType) {
        case VK_DESCRIPTOR_TYPE_SAMPLER:
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
        case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pImageInfo),
                                                   pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo),
                                                   pDescriptorWrites[i].pImageInfo);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pImageInfo));
            }
            break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
            {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pTexelBufferView),
                                                   pDescriptorWrites[i].descriptorCount * sizeof(VkBufferView),
                                                   pDescriptorWrites[i].pTexelBufferView);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pTexelBufferView));
            }
            break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
            {
                vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pBufferInfo),
                                                   pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorBufferInfo),
                                                   pDescriptorWrites[i].pBufferInfo);
                vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites[i].pBufferInfo));
            }
            break;
        default:
            break;
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorWrites));

    pPacket->descriptorCopyCount = descriptorCopyCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorCopies), descriptorCopyCount * sizeof(VkCopyDescriptorSet), pDescriptorCopies);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorCopies));
    // end custom code
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkQueueSubmit(
    VkQueue queue,
    uint32_t submitCount,
    const VkSubmitInfo* pSubmits,
    VkFence fence)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkQueueSubmit* pPacket = NULL;
    size_t arrayByteCount = 0;
    uint32_t i = 0;
    for (i=0; i<submitCount; ++i) {
        arrayByteCount += vk_size_vksubmitinfo(&pSubmits[i]);
    }
    CREATE_TRACE_PACKET(vkQueueSubmit, arrayByteCount);
    result = mdd(queue)->devTable.QueueSubmit(queue, submitCount, pSubmits, fence);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkQueueSubmit(pHeader);
    pPacket->queue = queue;
    pPacket->submitCount = submitCount;
    pPacket->fence = fence;
    pPacket->result = result;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits), submitCount*sizeof(VkSubmitInfo), pSubmits);
    for (i=0; i<submitCount; ++i) {
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pCommandBuffers), pPacket->pSubmits[i].commandBufferCount * sizeof(VkCommandBuffer), pSubmits[i].pCommandBuffers);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pCommandBuffers));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pWaitSemaphores), pPacket->pSubmits[i].waitSemaphoreCount * sizeof(VkSemaphore), pSubmits[i].pWaitSemaphores);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pWaitSemaphores));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pSignalSemaphores), pPacket->pSubmits[i].signalSemaphoreCount * sizeof(VkSemaphore), pSubmits[i].pSignalSemaphores);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pSignalSemaphores));
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSubmits[i].pWaitDstStageMask), sizeof(VkPipelineStageFlags), pSubmits[i].pWaitDstStageMask);
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits[i].pWaitDstStageMask));
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSubmits));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkCmdWaitEvents(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkCmdWaitEvents* pPacket = NULL;
    size_t customSize;
    customSize = (eventCount * sizeof(VkEvent)) + (memoryBarrierCount * sizeof(VkMemoryBarrier)) +
            (bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier)) +
            (imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier));
    CREATE_TRACE_PACKET(vkCmdWaitEvents, customSize);
    mdd(commandBuffer)->devTable.CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask,
                                    memoryBarrierCount, pMemoryBarriers,
                                    bufferMemoryBarrierCount, pBufferMemoryBarriers,
                                    imageMemoryBarrierCount, pImageMemoryBarriers);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdWaitEvents(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->eventCount = eventCount;
    pPacket->srcStageMask = srcStageMask;
    pPacket->dstStageMask = dstStageMask;
    pPacket->memoryBarrierCount = memoryBarrierCount;
    pPacket->bufferMemoryBarrierCount = bufferMemoryBarrierCount;
    pPacket->imageMemoryBarrierCount = imageMemoryBarrierCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pEvents), eventCount * sizeof(VkEvent), pEvents);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pEvents));
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemoryBarriers), memoryBarrierCount * sizeof(VkMemoryBarrier), pMemoryBarriers);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBufferMemoryBarriers), bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier), pBufferMemoryBarriers);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pImageMemoryBarriers), imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier), pImageMemoryBarriers);

    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemoryBarriers));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBufferMemoryBarriers));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pImageMemoryBarriers));
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkCmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkCmdPipelineBarrier* pPacket = NULL;
    size_t customSize;
    customSize = (memoryBarrierCount * sizeof(VkMemoryBarrier)) +
            (bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier)) +
            (imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier));
    CREATE_TRACE_PACKET(vkCmdPipelineBarrier, customSize);
    mdd(commandBuffer)->devTable.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdPipelineBarrier(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->srcStageMask = srcStageMask;
    pPacket->dstStageMask = dstStageMask;
    pPacket->dependencyFlags = dependencyFlags;
    pPacket->memoryBarrierCount = memoryBarrierCount;
    pPacket->bufferMemoryBarrierCount = bufferMemoryBarrierCount;
    pPacket->imageMemoryBarrierCount = imageMemoryBarrierCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemoryBarriers), memoryBarrierCount * sizeof(VkMemoryBarrier), pMemoryBarriers);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pBufferMemoryBarriers), bufferMemoryBarrierCount * sizeof(VkBufferMemoryBarrier), pBufferMemoryBarriers);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pImageMemoryBarriers), imageMemoryBarrierCount * sizeof(VkImageMemoryBarrier), pImageMemoryBarriers);

    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemoryBarriers));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pBufferMemoryBarriers));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pImageMemoryBarriers));
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkCmdPushConstants(
    VkCommandBuffer commandBuffer,
    VkPipelineLayout layout,
    VkShaderStageFlags stageFlags,
    uint32_t offset,
    uint32_t size,
    const void* pValues)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkCmdPushConstants* pPacket = NULL;
    CREATE_TRACE_PACKET(vkCmdPushConstants, size);
    mdd(commandBuffer)->devTable.CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdPushConstants(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->layout = layout;
    pPacket->stageFlags = stageFlags;
    pPacket->offset = offset;
    pPacket->size = size;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pValues), size, pValues);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pValues));
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetPipelineCacheData(
    VkDevice device,
    VkPipelineCache pipelineCache,
    size_t* pDataSize,
    void* pData)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkGetPipelineCacheData* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    result = mdd(device)->devTable.GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
    endTime = vktrace_get_time();
    assert(pDataSize);
    CREATE_TRACE_PACKET(vkGetPipelineCacheData, sizeof(size_t)  + *pDataSize );
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkGetPipelineCacheData(pHeader);
    pPacket->device = device;
    pPacket->pipelineCache = pipelineCache;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDataSize), sizeof(size_t), pDataSize);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pData), *pDataSize, pData);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDataSize));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pData));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateGraphicsPipelines(
    VkDevice device,
    VkPipelineCache pipelineCache,
    uint32_t createInfoCount,
    const VkGraphicsPipelineCreateInfo* pCreateInfos,
    const VkAllocationCallbacks* pAllocator,
    VkPipeline* pPipelines)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateGraphicsPipelines* pPacket = NULL;
    size_t total_size = 0;
    uint32_t i;
    for (i = 0; i < createInfoCount; i++) {
        total_size += get_struct_chain_size((void*)&pCreateInfos[i]);
    }
    CREATE_TRACE_PACKET(vkCreateGraphicsPipelines, total_size + sizeof(VkAllocationCallbacks) + createInfoCount*sizeof(VkPipeline));
    result = mdd(device)->devTable.CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateGraphicsPipelines(pHeader);
    pPacket->device = device;
    pPacket->pipelineCache = pipelineCache;
    pPacket->createInfoCount = createInfoCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfos), createInfoCount*sizeof(VkGraphicsPipelineCreateInfo), pCreateInfos);
    add_VkGraphicsPipelineCreateInfos_to_trace_packet(pHeader, (VkGraphicsPipelineCreateInfo*)pPacket->pCreateInfos, pCreateInfos, createInfoCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPipelines), createInfoCount*sizeof(VkPipeline), pPipelines);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfos));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPipelines));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateComputePipelines(
    VkDevice device,
    VkPipelineCache pipelineCache,
    uint32_t createInfoCount,
    const VkComputePipelineCreateInfo* pCreateInfos,
    const VkAllocationCallbacks* pAllocator,
    VkPipeline* pPipelines)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateComputePipelines* pPacket = NULL;
    uint32_t i;
    size_t total_size;

    total_size = createInfoCount*sizeof(VkComputePipelineCreateInfo) + sizeof(VkAllocationCallbacks) + createInfoCount*sizeof(VkPipeline);
    for (i=0; i < createInfoCount; i++) {
        total_size += (strlen(pCreateInfos[i].stage.pName)+1);
        if (pCreateInfos[i].stage.pSpecializationInfo) {
            total_size += sizeof(VkSpecializationInfo);
            if (pCreateInfos[i].stage.pSpecializationInfo->mapEntryCount > 0 && pCreateInfos[i].stage.pSpecializationInfo->pMapEntries)
                total_size += pCreateInfos[i].stage.pSpecializationInfo->mapEntryCount * sizeof(VkSpecializationMapEntry);
            if (pCreateInfos[i].stage.pSpecializationInfo->dataSize > 0 && pCreateInfos[i].stage.pSpecializationInfo->pData)
                total_size += pCreateInfos[i].stage.pSpecializationInfo->dataSize;
        }
    }
    CREATE_TRACE_PACKET(vkCreateComputePipelines, total_size);
    result = mdd(device)->devTable.CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreateComputePipelines(pHeader);
    pPacket->device = device;
    pPacket->pipelineCache = pipelineCache;
    pPacket->createInfoCount = createInfoCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfos), createInfoCount*sizeof(VkComputePipelineCreateInfo), pCreateInfos);
    add_VkComputePipelineCreateInfos_to_trace_packet(pHeader, (VkComputePipelineCreateInfo*)pPacket->pCreateInfos, pCreateInfos, createInfoCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPipelines), createInfoCount*sizeof(VkPipeline), pPipelines);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfos));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPipelines));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreatePipelineCache(
    VkDevice device,
    const VkPipelineCacheCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkPipelineCache* pPipelineCache)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreatePipelineCache* pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreatePipelineCache, get_struct_chain_size((void*)pCreateInfo) + sizeof(VkAllocationCallbacks) + sizeof(VkPipelineCache));
    result = mdd(device)->devTable.CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCreatePipelineCache(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkPipelineCacheCreateInfo), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pInitialData), pPacket->pCreateInfo->initialDataSize, pCreateInfo->pInitialData);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPipelineCache), sizeof(VkPipelineCache), pPipelineCache);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pInitialData));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPipelineCache));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR void VKAPI_CALL __HOOKED_vkCmdBeginRenderPass(
    VkCommandBuffer commandBuffer,
    const VkRenderPassBeginInfo* pRenderPassBegin,
    VkSubpassContents contents)
{
    vktrace_trace_packet_header* pHeader;
    packet_vkCmdBeginRenderPass* pPacket = NULL;
    size_t clearValueSize = sizeof(VkClearValue) * pRenderPassBegin->clearValueCount;
    CREATE_TRACE_PACKET(vkCmdBeginRenderPass, sizeof(VkRenderPassBeginInfo) + clearValueSize);
    mdd(commandBuffer)->devTable.CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkCmdBeginRenderPass(pHeader);
    pPacket->commandBuffer = commandBuffer;
    pPacket->contents = contents;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pRenderPassBegin), sizeof(VkRenderPassBeginInfo), pRenderPassBegin);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pRenderPassBegin->pClearValues), clearValueSize, pRenderPassBegin->pClearValues);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pRenderPassBegin->pClearValues));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pRenderPassBegin));
    FINISH_TRACE_PACKET();
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkFreeDescriptorSets(
    VkDevice device,
    VkDescriptorPool descriptorPool,
    uint32_t descriptorSetCount,
    const VkDescriptorSet* pDescriptorSets)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkFreeDescriptorSets* pPacket = NULL;
    CREATE_TRACE_PACKET(vkFreeDescriptorSets, descriptorSetCount*sizeof(VkDescriptorSet));
    result = mdd(device)->devTable.FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkFreeDescriptorSets(pHeader);
    pPacket->device = device;
    pPacket->descriptorPool = descriptorPool;
    pPacket->descriptorSetCount = descriptorSetCount;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pDescriptorSets), descriptorSetCount*sizeof(VkDescriptorSet), pDescriptorSets);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pDescriptorSets));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface,
    VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkGetPhysicalDeviceSurfaceCapabilitiesKHR* pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfaceCapabilitiesKHR, sizeof(VkSurfaceCapabilitiesKHR));
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurfaceCapabilities), sizeof(VkSurfaceCapabilitiesKHR), pSurfaceCapabilities);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurfaceCapabilities));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface,
    uint32_t* pSurfaceFormatCount,
    VkSurfaceFormatKHR* pSurfaceFormats)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    size_t _dataSize;
    packet_vkGetPhysicalDeviceSurfaceFormatsKHR* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    endTime = vktrace_get_time();
    _dataSize = (pSurfaceFormatCount == NULL || pSurfaceFormats == NULL) ? 0 : (*pSurfaceFormatCount *sizeof(VkSurfaceFormatKHR));
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfaceFormatsKHR, sizeof(uint32_t) + _dataSize);
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfaceFormatsKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurfaceFormatCount), sizeof(uint32_t), pSurfaceFormatCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurfaceFormats), _dataSize, pSurfaceFormats);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurfaceFormatCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurfaceFormats));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR surface,
    uint32_t* pPresentModeCount,
    VkPresentModeKHR* pPresentModes)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    size_t _dataSize;
    packet_vkGetPhysicalDeviceSurfacePresentModesKHR* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    endTime = vktrace_get_time();
    _dataSize = (pPresentModeCount == NULL || pPresentModes == NULL) ? 0 : (*pPresentModeCount *sizeof(VkPresentModeKHR));
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfacePresentModesKHR, sizeof(uint32_t) + _dataSize);
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfacePresentModesKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->surface = surface;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentModeCount), sizeof(uint32_t), pPresentModeCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentModes), _dataSize, pPresentModes);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentModeCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentModes));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateSwapchainKHR(
    VkDevice device,
    const VkSwapchainCreateInfoKHR* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkSwapchainKHR* pSwapchain)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateSwapchainKHR* pPacket = NULL;
    CREATE_TRACE_PACKET(vkCreateSwapchainKHR, vk_size_vkswapchaincreateinfokhr(pCreateInfo) + sizeof(VkSwapchainKHR) + sizeof(VkAllocationCallbacks));
    result = mdd(device)->devTable.CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    pPacket = interpret_body_as_vkCreateSwapchainKHR(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkSwapchainCreateInfoKHR), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSwapchain), sizeof(VkSwapchainKHR), pSwapchain);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices), pCreateInfo->queueFamilyIndexCount * sizeof(uint32_t), pCreateInfo->pQueueFamilyIndices);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo->pQueueFamilyIndices));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSwapchain));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetSwapchainImagesKHR(
    VkDevice device,
    VkSwapchainKHR swapchain,
    uint32_t* pSwapchainImageCount,
    VkImage* pSwapchainImages)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    size_t _dataSize;
    packet_vkGetSwapchainImagesKHR* pPacket = NULL;
    uint64_t startTime;
    uint64_t endTime;
    uint64_t vktraceStartTime = vktrace_get_time();
    startTime = vktrace_get_time();
    result = mdd(device)->devTable.GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    endTime = vktrace_get_time();
    _dataSize = (pSwapchainImageCount == NULL || pSwapchainImages == NULL) ? 0 : (*pSwapchainImageCount *sizeof(VkImage));
    CREATE_TRACE_PACKET(vkGetSwapchainImagesKHR, sizeof(uint32_t) + _dataSize);
    pHeader->vktrace_begin_time = vktraceStartTime;
    pHeader->entrypoint_begin_time = startTime;
    pHeader->entrypoint_end_time = endTime;
    pPacket = interpret_body_as_vkGetSwapchainImagesKHR(pHeader);
    pPacket->device = device;
    pPacket->swapchain = swapchain;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSwapchainImageCount), sizeof(uint32_t), pSwapchainImageCount);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSwapchainImages), _dataSize, pSwapchainImages);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSwapchainImageCount));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSwapchainImages));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkQueuePresentKHR(
    VkQueue queue,
    const VkPresentInfoKHR* pPresentInfo)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkQueuePresentKHR* pPacket = NULL;
    size_t swapchainSize = pPresentInfo->swapchainCount * sizeof(VkSwapchainKHR);
    size_t indexSize = pPresentInfo->swapchainCount * sizeof(uint32_t);
    size_t semaSize = pPresentInfo->waitSemaphoreCount * sizeof(VkSemaphore);
    size_t resultsSize = pPresentInfo->swapchainCount * sizeof(VkResult);
    size_t totalSize = sizeof(VkPresentInfoKHR) + swapchainSize + indexSize + semaSize;
    if (pPresentInfo->pResults != NULL) {
        totalSize += resultsSize;
    }
    CREATE_TRACE_PACKET(vkQueuePresentKHR, totalSize);
    result = mdd(queue)->devTable.QueuePresentKHR(queue, pPresentInfo);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkQueuePresentKHR(pHeader);
    pPacket->queue = queue;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentInfo), sizeof(VkPresentInfoKHR), pPresentInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentInfo->pSwapchains), swapchainSize, pPresentInfo->pSwapchains);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentInfo->pImageIndices), indexSize, pPresentInfo->pImageIndices);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentInfo->pWaitSemaphores), semaSize, pPresentInfo->pWaitSemaphores);
    if (pPresentInfo->pResults != NULL) {
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pPresentInfo->pResults), resultsSize, pPresentInfo->pResults);
    }
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentInfo->pImageIndices));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentInfo->pSwapchains));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentInfo->pWaitSemaphores));
    if (pPresentInfo->pResults != NULL) {
        vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentInfo->pResults));
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pPresentInfo));
    FINISH_TRACE_PACKET();
    return result;
}

/* TODO these can probably be moved into code gen */
#ifdef VK_USE_PLATFORM_WIN32_KHR
VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateWin32SurfaceKHR(
    VkInstance                                  instance,
    const VkWin32SurfaceCreateInfoKHR*          pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateWin32SurfaceKHR* pPacket = NULL;
    // don't bother with copying the actual win32 hinstance, hwnd into the trace packet, vkreplay has to use it's own anyway
    CREATE_TRACE_PACKET(vkCreateWin32SurfaceKHR, sizeof(VkSurfaceKHR) + sizeof(VkAllocationCallbacks) + sizeof(VkWin32SurfaceCreateInfoKHR));
    result = mid(instance)->instTable.CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    pPacket = interpret_body_as_vkCreateWin32SurfaceKHR(pHeader);
    pPacket->instance = instance;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkWin32SurfaceCreateInfoKHR), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurface), sizeof(VkSurfaceKHR), pSurface);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurface));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL __HOOKED_vkGetPhysicalDeviceWin32PresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex)
{
    vktrace_trace_packet_header* pHeader;
    VkBool32 result;
    packet_vkGetPhysicalDeviceWin32PresentationSupportKHR* pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceWin32PresentationSupportKHR, 0);
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
    pPacket = interpret_body_as_vkGetPhysicalDeviceWin32PresentationSupportKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->queueFamilyIndex = queueFamilyIndex;
    pPacket->result = result;
    FINISH_TRACE_PACKET();
    return result;
}
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateXcbSurfaceKHR(
    VkInstance                                  instance,
    const VkXcbSurfaceCreateInfoKHR*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateXcbSurfaceKHR* pPacket = NULL;
    // don't bother with copying the actual xcb window and connection into the trace packet, vkreplay has to use it's own anyway
    CREATE_TRACE_PACKET(vkCreateXcbSurfaceKHR, sizeof(VkSurfaceKHR) + sizeof(VkAllocationCallbacks) + sizeof(VkXcbSurfaceCreateInfoKHR));
    result = mid(instance)->instTable.CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    pPacket = interpret_body_as_vkCreateXcbSurfaceKHR(pHeader);
    pPacket->instance = instance;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkXcbSurfaceCreateInfoKHR), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurface), sizeof(VkSurfaceKHR), pSurface);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurface));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL __HOOKED_vkGetPhysicalDeviceXcbPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    xcb_connection_t*                           connection,
    xcb_visualid_t                              visual_id)
{
    vktrace_trace_packet_header* pHeader;
    VkBool32 result;
    packet_vkGetPhysicalDeviceXcbPresentationSupportKHR* pPacket = NULL;
    // don't bother with copying the actual xcb visual_id and connection into the trace packet, vkreplay has to use it's own anyway
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceXcbPresentationSupportKHR, 0);
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
    pPacket = interpret_body_as_vkGetPhysicalDeviceXcbPresentationSupportKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->connection = connection;
    pPacket->queueFamilyIndex = queueFamilyIndex;
    pPacket->visual_id = visual_id;
    pPacket->result = result;
    FINISH_TRACE_PACKET();
    return result;
}
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkCreateXlibSurfaceKHR(
    VkInstance                                  instance,
    const VkXlibSurfaceCreateInfoKHR*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkCreateXlibSurfaceKHR* pPacket = NULL;
    // don't bother with copying the actual xlib window and connection into the trace packet, vkreplay has to use it's own anyway
    CREATE_TRACE_PACKET(vkCreateXlibSurfaceKHR, sizeof(VkSurfaceKHR) + sizeof(VkAllocationCallbacks) + sizeof(VkXlibSurfaceCreateInfoKHR));
    result = mid(instance)->instTable.CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    pPacket = interpret_body_as_vkCreateXlibSurfaceKHR(pHeader);
    pPacket->instance = instance;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pCreateInfo), sizeof(VkXlibSurfaceCreateInfoKHR), pCreateInfo);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pAllocator), sizeof(VkAllocationCallbacks), NULL);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurface), sizeof(VkSurfaceKHR), pSurface);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pCreateInfo));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pAllocator));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurface));
    FINISH_TRACE_PACKET();
    return result;
}

VKTRACER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL __HOOKED_vkGetPhysicalDeviceXlibPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    Display*                                    dpy,
    VisualID                                    visualID)
{
    vktrace_trace_packet_header* pHeader;
    VkBool32 result;
    packet_vkGetPhysicalDeviceXlibPresentationSupportKHR* pPacket = NULL;
    // don't bother with copying the actual xlib visual_id and connection into the trace packet, vkreplay has to use it's own anyway
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceXlibPresentationSupportKHR, 0);
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
    pPacket = interpret_body_as_vkGetPhysicalDeviceXlibPresentationSupportKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->dpy = dpy;
    pPacket->queueFamilyIndex = queueFamilyIndex;
    pPacket->visualID = visualID;
    pPacket->result = result;
    FINISH_TRACE_PACKET();
    return result;
}
#endif
//TODO Wayland and Mir support

/* TODO: Probably want to make this manual to get the result of the boolean and then check it on replay
VKTRACER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL __HOOKED_vkGetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice physicalDevice,
    uint32_t queueFamilyIndex,
    const VkSurfaceDescriptionKHR* pSurfaceDescription,
    VkBool32* pSupported)
{
    vktrace_trace_packet_header* pHeader;
    VkResult result;
    packet_vkGetPhysicalDeviceSurfaceSupportKHR* pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetPhysicalDeviceSurfaceSupportKHR, sizeof(VkSurfaceDescriptionKHR) + sizeof(VkBool32));
    result = mid(physicalDevice)->instTable.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, pSurfaceDescription, pSupported);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkGetPhysicalDeviceSurfaceSupportKHR(pHeader);
    pPacket->physicalDevice = physicalDevice;
    pPacket->queueFamilyIndex = queueFamilyIndex;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSurfaceDescription), sizeof(VkSurfaceDescriptionKHR), pSurfaceDescription);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pSupported), sizeof(VkBool32), pSupported);
    pPacket->result = result;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSurfaceDescription));
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pSupported));
    FINISH_TRACE_PACKET();
    return result;
}
*/

static inline PFN_vkVoidFunction layer_intercept_proc(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;

    if (!strcmp(name, "CreateDevice"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateDevice;
    if (!strcmp(name, "DestroyDevice"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyDevice;
    if (!strcmp(name, "GetDeviceQueue"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetDeviceQueue;
    if (!strcmp(name, "QueueSubmit"))
        return (PFN_vkVoidFunction) __HOOKED_vkQueueSubmit;
    if (!strcmp(name, "QueueWaitIdle"))
        return (PFN_vkVoidFunction) __HOOKED_vkQueueWaitIdle;
    if (!strcmp(name, "DeviceWaitIdle"))
        return (PFN_vkVoidFunction) __HOOKED_vkDeviceWaitIdle;
    if (!strcmp(name, "AllocateMemory"))
        return (PFN_vkVoidFunction) __HOOKED_vkAllocateMemory;
    if (!strcmp(name, "FreeMemory"))
        return (PFN_vkVoidFunction) __HOOKED_vkFreeMemory;
    if (!strcmp(name, "MapMemory"))
        return (PFN_vkVoidFunction) __HOOKED_vkMapMemory;
    if (!strcmp(name, "UnmapMemory"))
        return (PFN_vkVoidFunction) __HOOKED_vkUnmapMemory;
    if (!strcmp(name, "FlushMappedMemoryRanges"))
        return (PFN_vkVoidFunction) __HOOKED_vkFlushMappedMemoryRanges;
    if (!strcmp(name, "InvalidateMappedMemoryRanges"))
        return (PFN_vkVoidFunction) __HOOKED_vkInvalidateMappedMemoryRanges;
    if (!strcmp(name, "GetDeviceMemoryCommitment"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetDeviceMemoryCommitment;
    if (!strcmp(name, "BindBufferMemory"))
        return (PFN_vkVoidFunction) __HOOKED_vkBindBufferMemory;
    if (!strcmp(name, "BindImageMemory"))
        return (PFN_vkVoidFunction) __HOOKED_vkBindImageMemory;
    if (!strcmp(name, "GetBufferMemoryRequirements"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetBufferMemoryRequirements;
    if (!strcmp(name, "GetImageMemoryRequirements"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetImageMemoryRequirements;
    if (!strcmp(name, "GetImageSparseMemoryRequirements"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetImageSparseMemoryRequirements;
    if (!strcmp(name, "GetPhysicalDeviceSparseImageFormatProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceSparseImageFormatProperties;
    if (!strcmp(name, "QueueBindSparse"))
        return (PFN_vkVoidFunction) __HOOKED_vkQueueBindSparse;
    if (!strcmp(name, "CreateFence"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateFence;
    if (!strcmp(name, "DestroyFence"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyFence;
    if (!strcmp(name, "ResetFences"))
        return (PFN_vkVoidFunction) __HOOKED_vkResetFences;
    if (!strcmp(name, "GetFenceStatus"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetFenceStatus;
    if (!strcmp(name, "WaitForFences"))
        return (PFN_vkVoidFunction) __HOOKED_vkWaitForFences;
    if (!strcmp(name, "CreateSemaphore"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateSemaphore;
    if (!strcmp(name, "DestroySemaphore"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroySemaphore;
    if (!strcmp(name, "CreateEvent"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateEvent;
    if (!strcmp(name, "DestroyEvent"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyEvent;
    if (!strcmp(name, "GetEventStatus"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetEventStatus;
    if (!strcmp(name, "SetEvent"))
        return (PFN_vkVoidFunction) __HOOKED_vkSetEvent;
    if (!strcmp(name, "ResetEvent"))
        return (PFN_vkVoidFunction) __HOOKED_vkResetEvent;
    if (!strcmp(name, "CreateQueryPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateQueryPool;
    if (!strcmp(name, "DestroyQueryPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyQueryPool;
    if (!strcmp(name, "GetQueryPoolResults"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetQueryPoolResults;
    if (!strcmp(name, "CreateBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateBuffer;
    if (!strcmp(name, "DestroyBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyBuffer;
    if (!strcmp(name, "CreateBufferView"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateBufferView;
    if (!strcmp(name, "DestroyBufferView"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyBufferView;
    if (!strcmp(name, "CreateImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateImage;
    if (!strcmp(name, "DestroyImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyImage;
    if (!strcmp(name, "GetImageSubresourceLayout"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetImageSubresourceLayout;
    if (!strcmp(name, "CreateImageView"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateImageView;
    if (!strcmp(name, "DestroyImageView"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyImageView;
    if (!strcmp(name, "CreateShaderModule"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateShaderModule;
    if (!strcmp(name, "DestroyShaderModule"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyShaderModule;
    if (!strcmp(name, "CreatePipelineCache"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreatePipelineCache;
    if (!strcmp(name, "DestroyPipelineCache"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyPipelineCache;
    if (!strcmp(name, "GetPipelineCacheData"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPipelineCacheData;
    if (!strcmp(name, "MergePipelineCaches"))
        return (PFN_vkVoidFunction) __HOOKED_vkMergePipelineCaches;
    if (!strcmp(name, "CreateGraphicsPipelines"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateGraphicsPipelines;
    if (!strcmp(name, "CreateComputePipelines"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateComputePipelines;
    if (!strcmp(name, "DestroyPipeline"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyPipeline;
    if (!strcmp(name, "CreatePipelineLayout"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreatePipelineLayout;
    if (!strcmp(name, "DestroyPipelineLayout"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyPipelineLayout;
    if (!strcmp(name, "CreateSampler"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateSampler;
    if (!strcmp(name, "DestroySampler"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroySampler;
    if (!strcmp(name, "CreateDescriptorSetLayout"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateDescriptorSetLayout;
    if (!strcmp(name, "DestroyDescriptorSetLayout"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyDescriptorSetLayout;
    if (!strcmp(name, "CreateDescriptorPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateDescriptorPool;
    if (!strcmp(name, "DestroyDescriptorPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyDescriptorPool;
    if (!strcmp(name, "ResetDescriptorPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkResetDescriptorPool;
    if (!strcmp(name, "AllocateDescriptorSets"))
        return (PFN_vkVoidFunction) __HOOKED_vkAllocateDescriptorSets;
    if (!strcmp(name, "FreeDescriptorSets"))
        return (PFN_vkVoidFunction) __HOOKED_vkFreeDescriptorSets;
    if (!strcmp(name, "UpdateDescriptorSets"))
        return (PFN_vkVoidFunction) __HOOKED_vkUpdateDescriptorSets;
    if (!strcmp(name, "CreateCommandPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateCommandPool;
    if (!strcmp(name, "DestroyCommandPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyCommandPool;
    if (!strcmp(name, "ResetCommandPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkResetCommandPool;
    if (!strcmp(name, "AllocateCommandBuffers"))
        return (PFN_vkVoidFunction) __HOOKED_vkAllocateCommandBuffers;
    if (!strcmp(name, "FreeCommandBuffers"))
        return (PFN_vkVoidFunction) __HOOKED_vkFreeCommandBuffers;
    if (!strcmp(name, "BeginCommandBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkBeginCommandBuffer;
    if (!strcmp(name, "EndCommandBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkEndCommandBuffer;
    if (!strcmp(name, "ResetCommandBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkResetCommandBuffer;
    if (!strcmp(name, "CmdBindPipeline"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBindPipeline;
    if (!strcmp(name, "CmdSetViewport"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetViewport;
    if (!strcmp(name, "CmdSetScissor"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetScissor;
    if (!strcmp(name, "CmdSetLineWidth"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetLineWidth;
    if (!strcmp(name, "CmdSetDepthBias"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetDepthBias;
    if (!strcmp(name, "CmdSetBlendConstants"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetBlendConstants;
    if (!strcmp(name, "CmdSetDepthBounds"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetDepthBounds;
    if (!strcmp(name, "CmdSetStencilCompareMask"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetStencilCompareMask;
    if (!strcmp(name, "CmdSetStencilWriteMask"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetStencilWriteMask;
    if (!strcmp(name, "CmdSetStencilReference"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetStencilReference;
    if (!strcmp(name, "CmdBindDescriptorSets"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBindDescriptorSets;
    if (!strcmp(name, "CmdBindIndexBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBindIndexBuffer;
    if (!strcmp(name, "CmdBindVertexBuffers"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBindVertexBuffers;
    if (!strcmp(name, "CmdDraw"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdDraw;
    if (!strcmp(name, "CmdDrawIndexed"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdDrawIndexed;
    if (!strcmp(name, "CmdDrawIndirect"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdDrawIndirect;
    if (!strcmp(name, "CmdDrawIndexedIndirect"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdDrawIndexedIndirect;
    if (!strcmp(name, "CmdDispatch"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdDispatch;
    if (!strcmp(name, "CmdDispatchIndirect"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdDispatchIndirect;
    if (!strcmp(name, "CmdCopyBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdCopyBuffer;
    if (!strcmp(name, "CmdCopyImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdCopyImage;
    if (!strcmp(name, "CmdBlitImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBlitImage;
    if (!strcmp(name, "CmdCopyBufferToImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdCopyBufferToImage;
    if (!strcmp(name, "CmdCopyImageToBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdCopyImageToBuffer;
    if (!strcmp(name, "CmdUpdateBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdUpdateBuffer;
    if (!strcmp(name, "CmdFillBuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdFillBuffer;
    if (!strcmp(name, "CmdClearColorImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdClearColorImage;
    if (!strcmp(name, "CmdClearDepthStencilImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdClearDepthStencilImage;
    if (!strcmp(name, "CmdClearAttachments"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdClearAttachments;
    if (!strcmp(name, "CmdResolveImage"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdResolveImage;
    if (!strcmp(name, "CmdSetEvent"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdSetEvent;
    if (!strcmp(name, "CmdResetEvent"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdResetEvent;
    if (!strcmp(name, "CmdWaitEvents"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdWaitEvents;
    if (!strcmp(name, "CmdPipelineBarrier"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdPipelineBarrier;
    if (!strcmp(name, "CmdBeginQuery"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBeginQuery;
    if (!strcmp(name, "CmdEndQuery"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdEndQuery;
    if (!strcmp(name, "CmdResetQueryPool"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdResetQueryPool;
    if (!strcmp(name, "CmdWriteTimestamp"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdWriteTimestamp;
    if (!strcmp(name, "CmdCopyQueryPoolResults"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdCopyQueryPoolResults;
    if (!strcmp(name, "CreateFramebuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateFramebuffer;
    if (!strcmp(name, "DestroyFramebuffer"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyFramebuffer;
    if (!strcmp(name, "CreateRenderPass"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateRenderPass;
    if (!strcmp(name, "DestroyRenderPass"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyRenderPass;
    if (!strcmp(name, "GetRenderAreaGranularity"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetRenderAreaGranularity;
    if (!strcmp(name, "CmdBeginRenderPass"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdBeginRenderPass;
    if (!strcmp(name, "CmdNextSubpass"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdNextSubpass;
    if (!strcmp(name, "CmdPushConstants"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdPushConstants;
    if (!strcmp(name, "CmdEndRenderPass"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdEndRenderPass;
    if (!strcmp(name, "CmdExecuteCommands"))
        return (PFN_vkVoidFunction) __HOOKED_vkCmdExecuteCommands;

    return NULL;
}

static inline PFN_vkVoidFunction layer_intercept_instance_proc(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;
    if (!strcmp(name, "CreateDevice"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateDevice;
    if (!strcmp(name, "CreateInstance"))
        return (PFN_vkVoidFunction) __HOOKED_vkCreateInstance;
    if (!strcmp(name, "DestroyInstance"))
        return (PFN_vkVoidFunction) __HOOKED_vkDestroyInstance;
    if (!strcmp(name, "EnumeratePhysicalDevices"))
        return (PFN_vkVoidFunction) __HOOKED_vkEnumeratePhysicalDevices;
    if (!strcmp(name, "GetPhysicalDeviceFeatures"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceFeatures;
    if (!strcmp(name, "GetPhysicalDeviceFormatProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceImageFormatProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceImageFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceSparseImageFormatProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceSparseImageFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceProperties;
    if (!strcmp(name, "GetPhysicalDeviceQueueFamilyProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceQueueFamilyProperties;
    if (!strcmp(name, "GetPhysicalDeviceMemoryProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceMemoryProperties;
    if (!strcmp(name, "EnumerateDeviceLayerProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkEnumerateDeviceLayerProperties;
    if (!strcmp(name, "EnumerateDeviceExtensionProperties"))
        return (PFN_vkVoidFunction) __HOOKED_vkEnumerateDeviceExtensionProperties;

    return NULL;
}

/**
 * Want trace packets created for GetDeviceProcAddr that is app initiated
 * but not for loader initiated calls to GDPA. Thus need two versions of GDPA.
 */
VKTRACER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vktraceGetDeviceProcAddr(VkDevice device, const char* funcName)
{
    vktrace_trace_packet_header *pHeader;
    PFN_vkVoidFunction addr;
    packet_vkGetDeviceProcAddr* pPacket = NULL;
    CREATE_TRACE_PACKET(vkGetDeviceProcAddr, ((funcName != NULL) ? strlen(funcName) + 1 : 0));
    addr = __HOOKED_vkGetDeviceProcAddr(device, funcName);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkGetDeviceProcAddr(pHeader);
    pPacket->device = device;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pName), ((funcName != NULL) ? strlen(funcName) + 1 : 0), funcName);
    pPacket->result = addr;
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pName));
    FINISH_TRACE_PACKET();
    return addr;
}

/* GDPA with no trace packet creation */
VKTRACER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL __HOOKED_vkGetDeviceProcAddr(VkDevice device, const char* funcName)
{
    if (!strcmp("vkGetDeviceProcAddr", funcName)) {
        if (gMessageStream != NULL) {
            return (PFN_vkVoidFunction) vktraceGetDeviceProcAddr;
        } else {
            return (PFN_vkVoidFunction) __HOOKED_vkGetDeviceProcAddr;
        }
    }

    layer_device_data  *devData = mdd(device);
    if (gMessageStream != NULL) {

        PFN_vkVoidFunction addr;
        addr = layer_intercept_proc(funcName);
        if (addr)
            return addr;

        if (devData->KHRDeviceSwapchainEnabled)
        {
            if (!strcmp("vkCreateSwapchainKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateSwapchainKHR;
            if (!strcmp("vkDestroySwapchainKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkDestroySwapchainKHR;
            if (!strcmp("vkGetSwapchainImagesKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetSwapchainImagesKHR;
            if (!strcmp("vkAcquireNextImageKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkAcquireNextImageKHR;
            if (!strcmp("vkQueuePresentKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkQueuePresentKHR;
        }
    }

    if (device == VK_NULL_HANDLE) {
        return NULL;
    }

    VkLayerDispatchTable *pDisp =  &devData->devTable;
    if (pDisp->GetDeviceProcAddr == NULL)
        return NULL;
    return pDisp->GetDeviceProcAddr(device, funcName);
}

/**
 * Want trace packets created for GetInstanceProcAddr that is app initiated
 * but not for loader initiated calls to GIPA. Thus need two versions of GIPA.
 */
VKTRACER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vktraceGetInstanceProcAddr(VkInstance instance, const char* funcName)
{
    vktrace_trace_packet_header* pHeader;
    PFN_vkVoidFunction addr;
    packet_vkGetInstanceProcAddr* pPacket = NULL;
    assert(strcmp("vkGetInstanceProcAddr", funcName));
    CREATE_TRACE_PACKET(vkGetInstanceProcAddr, ((funcName != NULL) ? strlen(funcName) + 1 : 0));
    addr = __HOOKED_vkGetInstanceProcAddr(instance, funcName);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket = interpret_body_as_vkGetInstanceProcAddr(pHeader);
    pPacket->instance = instance;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**) &(pPacket->pName), ((funcName != NULL) ? strlen(funcName) + 1 : 0), funcName);
    pPacket->result = addr;
    vktrace_finalize_buffer_address(pHeader, (void**) &(pPacket->pName));
    FINISH_TRACE_PACKET();
    return addr;
}

/* GIPA with no trace packet creation */
VKTRACER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL __HOOKED_vkGetInstanceProcAddr(VkInstance instance, const char* funcName)
{
    PFN_vkVoidFunction addr;
    layer_instance_data  *instData;

    vktrace_platform_thread_once(&gInitOnce, InitTracer);
    if (!strcmp("vkGetInstanceProcAddr", funcName)) {
        if (gMessageStream != NULL) {
            return (PFN_vkVoidFunction) vktraceGetInstanceProcAddr;
        } else {
            return (PFN_vkVoidFunction) __HOOKED_vkGetInstanceProcAddr;
        }
    }

    if (gMessageStream != NULL) {
        addr = layer_intercept_instance_proc(funcName);
        if (addr)
            return addr;

        if (instance == VK_NULL_HANDLE) {
            return NULL;
        }

        instData = mid(instance);
        if (instData->LunargDebugReportEnabled)
        {
            if (!strcmp("vkCreateDebugReportCallbackEXT", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateDebugReportCallbackEXT;
            if (!strcmp("vkDestroyDebugReportCallbackEXT", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkDestroyDebugReportCallbackEXT;

        }
        if (instData->KHRSurfaceEnabled)
        {
            if (!strcmp("vkGetPhysicalDeviceSurfaceSupportKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceSurfaceSupportKHR;
            if (!strcmp("vkDestroySurfaceKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkDestroySurfaceKHR;
            if (!strcmp("vkGetPhysicalDeviceSurfaceCapabilitiesKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
            if (!strcmp("vkGetPhysicalDeviceSurfaceFormatsKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceSurfaceFormatsKHR;
            if (!strcmp("vkGetPhysicalDeviceSurfacePresentModesKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceSurfacePresentModesKHR;
        }
#ifdef VK_USE_PLATFORM_XLIB_KHR
        if (instData->KHRXlibSurfaceEnabled)
        {
            if (!strcmp("vkCreateXlibSurfaceKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateXlibSurfaceKHR;
            if (!strcmp("vkGetPhysicalDeviceXlibPresentationSupportKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceXlibPresentationSupportKHR;
        }
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
        if (instData->KHRXcbSurfaceEnabled)
        {
            if (!strcmp("vkCreateXcbSurfaceKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateXcbSurfaceKHR;
            if (!strcmp("vkGetPhysicalDeviceXcbPresentationSupportKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceXcbPresentationSupportKHR;
        }
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
        if (instData->KHRWaylandSurfaceEnabled)
        {
            if (!strcmp("vkCreateWaylandSurfaceKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateWaylandSurfaceKHR;
            if (!strcmp("vkGetPhysicalDeviceWaylandPresentationSupportKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceWaylandPresentationSupportKHR;
        }
#endif
#ifdef VK_USE_PLATFORM_MIR_KHR
        if (instData->KHRMirSurfaceEnabled)
        {
            if (!strcmp("vkCreateMirSurfaceKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateMirSurfaceKHR;
            if (!strcmp("vkGetPhysicalDeviceMirPresentationSupportKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceMirPresentationSupportKHR;
        }
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
        if (instData->KHRWin32SurfaceEnabled)
        {
            if (!strcmp("vkCreateWin32SurfaceKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkCreateWin32SurfaceKHR;
            if (!strcmp("vkGetPhysicalDeviceWin32PresentationSupportKHR", funcName))
                return (PFN_vkVoidFunction) __HOOKED_vkGetPhysicalDeviceWin32PresentationSupportKHR;
        }
#endif
    } else {
        if (instance == VK_NULL_HANDLE) {
            return NULL;
        }
        instData = mid(instance);
    }
    VkLayerInstanceDispatchTable* pTable = &instData->instTable;
    if (pTable->GetInstanceProcAddr == NULL)
        return NULL;

    return pTable->GetInstanceProcAddr(instance, funcName);
}
