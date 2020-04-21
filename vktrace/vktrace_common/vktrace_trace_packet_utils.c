/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * Copyright (C) 2016 Advanced Micro Devices, Inc.
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
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 * Author: David Pinedo <david@lunarg.com>
 **************************************************************************/
#include "vktrace_trace_packet_utils.h"
#include "vktrace_interconnect.h"
#include "vktrace_filelike.h"
#include "vktrace_pageguard_memorycopy.h"

#if defined(WIN32)
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
#include <fcntl.h>
#include <time.h>
#include <sys/utsname.h>
#endif

#if defined(PLATFORM_OSX)
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include "vk_struct_size_helper.c"
#include "vktrace_pageguard_memorycopy.h"

static VKTRACE_CRITICAL_SECTION s_packet_index_lock;
static VKTRACE_CRITICAL_SECTION s_trace_lock;

void vktrace_initialize_trace_packet_utils() {
    vktrace_create_critical_section(&s_packet_index_lock);
    vktrace_create_critical_section(&s_trace_lock);
}

void vktrace_deinitialize_trace_packet_utils() {
    vktrace_delete_critical_section(&s_packet_index_lock);
    vktrace_delete_critical_section(&s_trace_lock);
}

uint64_t vktrace_get_unique_packet_index() {
    // Keep the s_packet_index scope to within this method, to ensure this method is always used to get a unique packet index.
    static uint64_t s_packet_index = 0;

    vktrace_enter_critical_section(&s_packet_index_lock);
    // Store the value then increment within the critical section to avoid race conditions.
    uint64_t index = s_packet_index;
    s_packet_index += 1;
    vktrace_leave_critical_section(&s_packet_index_lock);

    // Return the stored value rather than the actual variable (again to avoid race conditions).
    return index;
}

void vktrace_gen_uuid(uint32_t* pUuid) {
    uint32_t buf[] = {0xABCDEF, 0x12345678, 0xFFFECABC, 0xABCDDEF0};
    vktrace_platform_rand_s(buf, sizeof(buf) / sizeof(uint32_t));

    pUuid[0] = buf[0];
    pUuid[1] = buf[1];
    pUuid[2] = buf[2];
    pUuid[3] = buf[3];
}

#if defined(PLATFORM_LINUX)
uint64_t vktrace_get_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return ((uint64_t)time.tv_sec * 1000000000) + time.tv_nsec;
}
#elif defined(PLATFORM_OSX)
uint64_t vktrace_get_time() {
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);

    return ((uint64_t)mts.tv_sec * 1000000000) + mts.tv_nsec;
}
#elif defined(PLATFORM_WINDOWS)
uint64_t vktrace_get_time() {
    // Should really avoid using RDTSC here since for RDTSC to be
    // accurate, the process needs to stay on the same CPU and the CPU
    // needs to stay at the same clock rate, which isn't always the case
    // with today's power managed CPUs.
    // But if all that is OK, the following one-liner could be used instead
    // of the rest of this function.
    //
    // return __rdtsc();
    //
    LARGE_INTEGER count;
    static LARGE_INTEGER start, freq;
    if (0 == start.QuadPart) {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
    }
    QueryPerformanceCounter(&count);
    // Using a relative (from start) count here postpones overflow as we convert to ns.
    return (uint64_t)(((count.QuadPart - start.QuadPart) * 1000000000) / freq.QuadPart);
}
#else
uint64_t vktrace_get_time() { return 0; }
#endif

uint64_t get_endianess() {
    uint32_t x = 1;
    return *((char*)&x) ? VKTRACE_LITTLE_ENDIAN : VKTRACE_BIG_ENDIAN;
}

// Function to return string describing given endianess
const char* get_endianess_string(uint64_t endianess) {
    if (endianess == VKTRACE_LITTLE_ENDIAN)
        return "VKTRACE_LITTLE_ENDIAN";
    else if (endianess == VKTRACE_BIG_ENDIAN)
        return "VKTRACE_BIG_ENDIAN";
    else
        return ("Unknown");
}

uint64_t get_arch() {
    uint64_t rval = 0;
#if defined(PLATFORM_LINUX)
    struct utsname buf;
    uname(&buf);
    strncpy((char*)&rval, buf.machine, sizeof(uint64_t));
#elif defined(PLATFORM_WINDOWS)
    SYSTEM_INFO systemInfo;
    char* arch;
    GetSystemInfo(&systemInfo);
    switch (systemInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            arch = "x86_64";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            arch = "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            arch = "ia64";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            arch = "x86";
            break;
        default:
            arch = "";
            break;
    }
    strncpy((char*)&rval, arch, sizeof(uint64_t));
#else
    // Other platforms, i.e. OSX, need to be added here
#endif
    return rval;
}

uint64_t get_os() {
    uint64_t rval;
#if defined(ANDROID)
    strncpy((char*)&rval, "Android", sizeof(uint64_t));
#elif defined(PLATFORM_LINUX)
    strncpy((char*)&rval, "Linux", sizeof(uint64_t));
#else
    strncpy((char*)&rval, "Windows", sizeof(uint64_t));
#endif
    return rval;
}

//=============================================================================
// Methods for creating, populating, and writing trace packets

vktrace_trace_packet_header* vktrace_create_trace_packet(uint8_t tracer_id, uint16_t packet_id, uint64_t packet_size,
                                                         uint64_t additional_buffers_size) {
    vktrace_enter_critical_section(&s_trace_lock);
    // Always allocate at least enough space for the packet header
    uint64_t total_packet_size =
        ROUNDUP_TO_8(sizeof(vktrace_trace_packet_header) + ROUNDUP_TO_8(packet_size) + additional_buffers_size);
    void* pMemory = vktrace_malloc((size_t)total_packet_size);
    memset(pMemory, 0, (size_t)total_packet_size);

    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)pMemory;
    pHeader->size = total_packet_size;
    pHeader->global_packet_index = vktrace_get_unique_packet_index();
    pHeader->tracer_id = tracer_id;
    pHeader->thread_id = vktrace_platform_get_thread_id();
    pHeader->packet_id = packet_id;
    if (pHeader->vktrace_begin_time == 0) pHeader->vktrace_begin_time = vktrace_get_time();
    pHeader->entrypoint_begin_time = pHeader->vktrace_begin_time;
    pHeader->entrypoint_end_time = 0;
    pHeader->vktrace_end_time = 0;
    pHeader->next_buffers_offset =
        sizeof(vktrace_trace_packet_header) + ROUNDUP_TO_8(packet_size);  // Initial offset is from start of header to after the
                                                                          // packet body. Make the initial offset to 64-bit aligned
                                                                          // to make it meets the alignement requirement of
                                                                          // instruction vst1.64 which may be used by a 32-bit GPU
                                                                          // driver on ARMv8 platform. Assuming the content of the
                                                                          // buffer has expected alignment of the tracing platform.
    if (total_packet_size > sizeof(vktrace_trace_packet_header)) {
        pHeader->pBody = (uintptr_t)(((char*)pMemory) + sizeof(vktrace_trace_packet_header));
    }
    return pHeader;
}

// Delete packet after vktrace_create_trace_packet being called.
void vktrace_delete_trace_packet(vktrace_trace_packet_header** ppHeader) {
    vktrace_delete_trace_packet_no_lock(ppHeader);

    vktrace_leave_critical_section(&s_trace_lock);
}

void* vktrace_trace_packet_get_new_buffer_address(vktrace_trace_packet_header* pHeader, uint64_t byteCount) {
    void* pBufferStart;
    assert(byteCount > 0);
    assert((byteCount & 0x3) == 0);  // All buffer sizes should be multiple of 4 so structs in packet are kept aligned
    assert(pHeader->size >= pHeader->next_buffers_offset + byteCount);
    if (pHeader->size < pHeader->next_buffers_offset + byteCount || byteCount == 0) {
        // not enough memory left in packet to hold buffer
        // or request is for 0 bytes
        return NULL;
    }

    pBufferStart = (void*)((char*)pHeader + pHeader->next_buffers_offset);
    pHeader->next_buffers_offset += byteCount;
    return pBufferStart;
}

// size is the size of the buffer pointed to by pBuffer.
// If size is not 4 byte aligned, when computing dest buffer addresses, round up
// the size so it is 4 byte aligned. We use the unrounded size when doing the
// actual copy -- this is so that we don't access memory past the end of the buffer.
void vktrace_add_buffer_to_trace_packet(vktrace_trace_packet_header* pHeader, void** ptr_address, uint64_t size,
                                        const void* pBuffer) {
    // Make sure we have a valid pointer.
    assert(ptr_address != NULL);

    if (pBuffer == NULL || size == 0) {
        *ptr_address = NULL;
    } else {
        // set ptr to the location of the added buffer
        *ptr_address = vktrace_trace_packet_get_new_buffer_address(pHeader, ROUNDUP_TO_4(size));

        // address of buffer in packet adding must be 4 byte aligned
        assert(((uint64_t)*ptr_address & 0x3) == 0);

        // copy buffer to the location
        vktrace_pageguard_memcpy(*ptr_address, pBuffer, (size_t)size);
    }
}

void vktrace_finalize_buffer_address(vktrace_trace_packet_header* pHeader, void** ptr_address) {
    assert(ptr_address != NULL);

    if (*ptr_address != NULL) {
        // turn ptr into an offset from the packet body
        uint64_t offset = (uint64_t)*ptr_address - (uint64_t)(pHeader->pBody);
        *ptr_address = (void*)offset;
    }
}

#define AddPointerWithCountToTracebuffer(_sName, _sType, _sPtr, _sCount)                 \
    do {                                                                                 \
        void* pSrc = (void*)(((_sName*)pInNext)->_sPtr);                                 \
        void** pDst = (void**)&(((_sName*)*ppOutNext)->_sPtr);                           \
        uint32_t count = ((_sName*)*ppOutNext)->_sCount;                                 \
        vktrace_add_buffer_to_trace_packet(pHeader, pDst, sizeof(_sType) * count, pSrc); \
        vktrace_finalize_buffer_address(pHeader, pDst);                                  \
    } while (0)

void vktrace_add_pnext_structs_to_trace_packet(vktrace_trace_packet_header* pHeader, void* pOut, const void* pIn) {
    void** ppOutNext;
    const void* pInNext;
    if (!pIn) return;
    // Add the pNext chain to trace packet.
    while (((VkApplicationInfo*)pIn)->pNext) {
        ppOutNext = (void**)&(((VkApplicationInfo*)pOut)->pNext);
        pInNext = (void*)((VkApplicationInfo*)pIn)->pNext;
        size_t size = get_struct_size(pInNext);
        if (size > 0) {
            vktrace_add_buffer_to_trace_packet(pHeader, ppOutNext, size, pInNext);
            // TODO: Might be able codegen this switch statement
            switch (((VkApplicationInfo*)*ppOutNext)->sType) {
                case VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO:
                    AddPointerWithCountToTracebuffer(VkDeviceGroupDeviceCreateInfo, VkPhysicalDevice, pPhysicalDevices,
                                                     physicalDeviceCount);
                    break;
                case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
                    AddPointerWithCountToTracebuffer(VkDeviceGroupRenderPassBeginInfo, VkRect2D, pDeviceRenderAreas,
                                                     deviceRenderAreaCount);
                    break;
                case VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO:
                    AddPointerWithCountToTracebuffer(VkDeviceGroupSubmitInfo, uint32_t, pWaitSemaphoreDeviceIndices,
                                                     waitSemaphoreCount);
                    AddPointerWithCountToTracebuffer(VkDeviceGroupSubmitInfo, uint32_t, pCommandBufferDeviceMasks,
                                                     commandBufferCount);
                    AddPointerWithCountToTracebuffer(VkDeviceGroupSubmitInfo, uint32_t, pSignalSemaphoreDeviceIndices,
                                                     signalSemaphoreCount);
                    break;
                case VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO:
                    AddPointerWithCountToTracebuffer(VkBindBufferMemoryDeviceGroupInfo, uint32_t, pDeviceIndices, deviceIndexCount);
                    break;
                case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO:
                    AddPointerWithCountToTracebuffer(VkBindImageMemoryDeviceGroupInfo, uint32_t, pDeviceIndices, deviceIndexCount);
                    AddPointerWithCountToTracebuffer(VkBindImageMemoryDeviceGroupInfo, VkRect2D, pSplitInstanceBindRegions,
                                                     splitInstanceBindRegionCount);
                    break;
                case VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT:
                    AddPointerWithCountToTracebuffer(VkValidationFlagsEXT, VkValidationCheckEXT, pDisabledValidationChecks,
                                                     disabledValidationCheckCount);
                    break;
                case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT:
                    AddPointerWithCountToTracebuffer(VkDescriptorSetVariableDescriptorCountAllocateInfoEXT, uint32_t,
                                                     pDescriptorCounts, descriptorSetCount);
                    break;
                case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_W_SCALING_STATE_CREATE_INFO_NV:
                    AddPointerWithCountToTracebuffer(VkPipelineViewportWScalingStateCreateInfoNV, VkViewportWScalingNV,
                                                     pViewportWScalings, viewportCount);
                    break;
                case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SWIZZLE_STATE_CREATE_INFO_NV:
                    AddPointerWithCountToTracebuffer(VkPipelineViewportSwizzleStateCreateInfoNV, VkViewportSwizzleNV,
                                                     pViewportSwizzles, viewportCount);
                    break;
                case VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT:
                    AddPointerWithCountToTracebuffer(VkPipelineDiscardRectangleStateCreateInfoEXT, VkRect2D, pDiscardRectangles,
                                                     discardRectangleCount);
                    break;
                case VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT:
                    AddPointerWithCountToTracebuffer(VkSampleLocationsInfoEXT, VkSampleLocationEXT, pSampleLocations,
                                                     sampleLocationsCount);
                    break;
                case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
                    AddPointerWithCountToTracebuffer(VkRenderPassSampleLocationsBeginInfoEXT, VkAttachmentSampleLocationsEXT,
                                                     pAttachmentInitialSampleLocations, attachmentInitialSampleLocationsCount);
                    AddPointerWithCountToTracebuffer(VkRenderPassSampleLocationsBeginInfoEXT, VkSubpassSampleLocationsEXT,
                                                     pPostSubpassSampleLocations, postSubpassSampleLocationsCount);
                    break;
                case VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_MODULATION_STATE_CREATE_INFO_NV:
                    AddPointerWithCountToTracebuffer(VkPipelineCoverageModulationStateCreateInfoNV, VkCoverageModulationModeNV,
                                                     pCoverageModulationTable, coverageModulationTableCount);
                    break;
                case VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO_KHR:
                    AddPointerWithCountToTracebuffer(VkImageFormatListCreateInfoKHR, VkFormat, pViewFormats, viewFormatCount);
                    break;
                case VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO:
                    AddPointerWithCountToTracebuffer(VkRenderPassMultiviewCreateInfo, uint32_t, pViewMasks, subpassCount);
                    AddPointerWithCountToTracebuffer(VkRenderPassMultiviewCreateInfo, int32_t, pViewOffsets, dependencyCount);
                    AddPointerWithCountToTracebuffer(VkRenderPassMultiviewCreateInfo, uint32_t, pCorrelationMasks,
                                                     correlationMaskCount);
                    break;
#if defined(WIN32)
                case VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR:
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoKHR, VkDeviceMemory, pAcquireSyncs,
                                                     acquireCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoKHR, uint64_t, pAcquireKeys, acquireCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoKHR, uint32_t, pAcquireTimeouts,
                                                     acquireCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoKHR, VkDeviceMemory, pReleaseSyncs,
                                                     releaseCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoKHR, uint64_t, pReleaseKeys, releaseCount);
                    break;
                case VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV:
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoNV, VkDeviceMemory, pAcquireSyncs,
                                                     acquireCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoNV, uint64_t, pAcquireKeys, acquireCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoNV, uint32_t, pAcquireTimeoutMilliseconds,
                                                     acquireCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoNV, VkDeviceMemory, pReleaseSyncs,
                                                     releaseCount);
                    AddPointerWithCountToTracebuffer(VkWin32KeyedMutexAcquireReleaseInfoNV, uint64_t, pReleaseKeys, releaseCount);
                    break;
#endif
                case VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE:
                    AddPointerWithCountToTracebuffer(VkPresentTimesInfoGOOGLE, VkPresentTimeGOOGLE, pTimes, swapchainCount);
                    break;
                case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT:
                    AddPointerWithCountToTracebuffer(VkDescriptorSetLayoutBindingFlagsCreateInfoEXT, VkDescriptorBindingFlagsEXT,
                                                     pBindingFlags, bindingCount);

                default:
                    // The cases in this switch statement are only those pnext struct types that have
                    // pointers inside them that need to be added. The pnext list may contain
                    // struct types that don't have pointers in them, which we skip and remove.
                    break;
            }
            pOut = *ppOutNext;
            pIn = pInNext;
            vktrace_finalize_buffer_address(pHeader, ppOutNext);
        } else {
            // Skip and remove from chain, must be an unknown type
            ((VkApplicationInfo*)pOut)->pNext = *ppOutNext ? ((VkApplicationInfo*)*ppOutNext)->pNext : NULL;
            pIn = pInNext;  // Should not remove from original struct, just skip
        }
    }
}

void vktrace_set_packet_entrypoint_end_time(vktrace_trace_packet_header* pHeader) {
    pHeader->entrypoint_end_time = vktrace_get_time();
}

void vktrace_finalize_trace_packet(vktrace_trace_packet_header* pHeader) {
    if (pHeader->entrypoint_end_time == 0) {
        vktrace_set_packet_entrypoint_end_time(pHeader);
    }
    pHeader->vktrace_end_time = vktrace_get_time();
}

void vktrace_write_trace_packet(const vktrace_trace_packet_header* pHeader, FileLike* pFile) {
    BOOL res = vktrace_FileLike_WriteRaw(pFile, pHeader, (size_t)pHeader->size);
    if (!res && pHeader->packet_id != VKTRACE_TPI_MARKER_TERMINATE_PROCESS) {
        // We don't retry on failure because vktrace_FileLike_WriteRaw already retried and gave up.
        vktrace_LogWarning("Failed to write trace packet.");
        exit(1);
    }
}

//=============================================================================
// Methods for Reading and interpretting trace packets

vktrace_trace_packet_header* vktrace_read_trace_packet(FileLike* pFile) {
    // read size
    // allocate space
    // offset to after size
    // read the rest of the packet
    uint64_t total_packet_size = 0;

    if (vktrace_FileLike_ReadRaw(pFile, &total_packet_size, sizeof(uint64_t)) == FALSE) {
        return NULL;
    }

    // allocate space
    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)vktrace_malloc((size_t)total_packet_size);

    if (pHeader != NULL) {
        pHeader->size = total_packet_size;
        if (vktrace_FileLike_ReadRaw(pFile, (char*)pHeader + sizeof(uint64_t), (size_t)total_packet_size - sizeof(uint64_t)) ==
            FALSE) {
            vktrace_LogError("Failed to read trace packet with size of %ju from %s source.", (intmax_t)total_packet_size,
                             FILELIKE_MODE_NAME(pFile->mMode));
            return NULL;
        }

        pHeader->pBody = (uintptr_t)pHeader + sizeof(vktrace_trace_packet_header);
    } else {
        vktrace_LogError("Malloc failed in vktrace_read_trace_packet of size %u.", total_packet_size);
    }

    return pHeader;
}

// Delete packet after vktrace_read_trace_packet being called.
void vktrace_delete_trace_packet_no_lock(vktrace_trace_packet_header** ppHeader) {
    if (ppHeader == NULL) return;
    if (*ppHeader == NULL) return;

    VKTRACE_DELETE(*ppHeader);
    *ppHeader = NULL;
}

void* vktrace_trace_packet_interpret_buffer_pointer(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable) {
    // the pointer variable actually contains a byte offset from the packet body to the start of the buffer.
    uint64_t offset = ptr_variable;
    void* buffer_location;

    // if the offset is 0, then we know the pointer to the buffer was NULL, so no buffer exists and we return NULL.
    if (offset == 0) return NULL;

    buffer_location = (char*)(pHeader->pBody) + offset;
    return buffer_location;
}

void add_VkApplicationInfo_to_packet(vktrace_trace_packet_header* pHeader, VkApplicationInfo** ppStruct,
                                     const VkApplicationInfo* pInStruct) {
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)ppStruct, sizeof(VkApplicationInfo), pInStruct);
    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void**)ppStruct, (void*)pInStruct);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->pApplicationName),
                                       (pInStruct->pApplicationName != NULL) ? strlen(pInStruct->pApplicationName) + 1 : 0,
                                       pInStruct->pApplicationName);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->pEngineName),
                                       (pInStruct->pEngineName != NULL) ? strlen(pInStruct->pEngineName) + 1 : 0,
                                       pInStruct->pEngineName);
    vktrace_finalize_buffer_address(pHeader, (void**)&((*ppStruct)->pApplicationName));
    vktrace_finalize_buffer_address(pHeader, (void**)&((*ppStruct)->pEngineName));
    vktrace_finalize_buffer_address(pHeader, (void**)&*ppStruct);
}

void add_VkInstanceCreateInfo_to_packet(vktrace_trace_packet_header* pHeader, VkInstanceCreateInfo** ppStruct,
                                        VkInstanceCreateInfo* pInStruct) {
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)ppStruct, sizeof(VkInstanceCreateInfo), pInStruct);
    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void*)*ppStruct, (void*)pInStruct);
    if (pInStruct->pApplicationInfo)
        add_VkApplicationInfo_to_packet(pHeader, (VkApplicationInfo**)&((*ppStruct)->pApplicationInfo),
                                        pInStruct->pApplicationInfo);
    uint32_t i, siz = 0;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledLayerNames),
                                       pInStruct->enabledLayerCount * sizeof(char*), pInStruct->ppEnabledLayerNames);
    if (pInStruct->enabledLayerCount > 0) {
        for (i = 0; i < pInStruct->enabledLayerCount; i++) {
            siz = (uint32_t)(1 + strlen(pInStruct->ppEnabledLayerNames[i]));
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledLayerNames[i]), siz,
                                               pInStruct->ppEnabledLayerNames[i]);
            vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledLayerNames[i]);
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledLayerNames);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledExtensionNames),
                                       pInStruct->enabledExtensionCount * sizeof(char*), pInStruct->ppEnabledExtensionNames);
    if (pInStruct->enabledExtensionCount > 0) {
        for (i = 0; i < pInStruct->enabledExtensionCount; i++) {
            siz = (uint32_t)(1 + strlen(pInStruct->ppEnabledExtensionNames[i]));
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledExtensionNames[i]), siz,
                                               pInStruct->ppEnabledExtensionNames[i]);
            vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledExtensionNames[i]);
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledExtensionNames);
    vktrace_finalize_buffer_address(pHeader, (void**)ppStruct);
}

void add_VkDeviceCreateInfo_to_packet(vktrace_trace_packet_header* pHeader, VkDeviceCreateInfo** ppStruct,
                                      const VkDeviceCreateInfo* pInStruct) {
    uint32_t i, siz = 0;
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)ppStruct, sizeof(VkDeviceCreateInfo), pInStruct);
    vktrace_add_pnext_structs_to_trace_packet(pHeader, (void*)*ppStruct, (void*)pInStruct);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos,
                                       pInStruct->queueCreateInfoCount * sizeof(VkDeviceQueueCreateInfo),
                                       pInStruct->pQueueCreateInfos);
    for (i = 0; i < pInStruct->queueCreateInfoCount; i++) {
        vktrace_add_pnext_structs_to_trace_packet(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos[i].pQueuePriorities,
                                                  (void*)&pInStruct->pQueueCreateInfos[i]);
        vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos[i].pQueuePriorities,
                                           pInStruct->pQueueCreateInfos[i].queueCount * sizeof(float),
                                           pInStruct->pQueueCreateInfos[i].pQueuePriorities);
        vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos[i].pQueuePriorities);
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->pQueueCreateInfos);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledLayerNames),
                                       pInStruct->enabledLayerCount * sizeof(char*), pInStruct->ppEnabledLayerNames);
    if (pInStruct->enabledLayerCount > 0) {
        for (i = 0; i < pInStruct->enabledLayerCount; i++) {
            siz = (uint32_t)(1 + strlen(pInStruct->ppEnabledLayerNames[i]));
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledLayerNames[i]), siz,
                                               pInStruct->ppEnabledLayerNames[i]);
            vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledLayerNames[i]);
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledLayerNames);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&((*ppStruct)->ppEnabledExtensionNames),
                                       pInStruct->enabledExtensionCount * sizeof(char*), pInStruct->ppEnabledExtensionNames);
    if (pInStruct->enabledExtensionCount > 0) {
        for (i = 0; i < pInStruct->enabledExtensionCount; i++) {
            siz = (uint32_t)(1 + strlen(pInStruct->ppEnabledExtensionNames[i]));
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)(&(*ppStruct)->ppEnabledExtensionNames[i]), siz,
                                               pInStruct->ppEnabledExtensionNames[i]);
            vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledExtensionNames[i]);
        }
    }
    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->ppEnabledExtensionNames);
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(*ppStruct)->pEnabledFeatures, sizeof(VkPhysicalDeviceFeatures),
                                       pInStruct->pEnabledFeatures);
    vktrace_finalize_buffer_address(pHeader, (void**)&(*ppStruct)->pEnabledFeatures);
    vktrace_finalize_buffer_address(pHeader, (void**)ppStruct);
}

VkInstanceCreateInfo* interpret_VkInstanceCreateInfo(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable) {
    VkInstanceCreateInfo* pVkInstanceCreateInfo =
        (VkInstanceCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)ptr_variable);
    uint32_t i;
    if (pVkInstanceCreateInfo != NULL) {
        pVkInstanceCreateInfo->pApplicationInfo = (VkApplicationInfo*)vktrace_trace_packet_interpret_buffer_pointer(
            pHeader, (intptr_t)pVkInstanceCreateInfo->pApplicationInfo);
        VkApplicationInfo** ppApplicationInfo = (VkApplicationInfo**)&pVkInstanceCreateInfo->pApplicationInfo;
        if (pVkInstanceCreateInfo->pApplicationInfo) {
            (*ppApplicationInfo)->pApplicationName = (const char*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkInstanceCreateInfo->pApplicationInfo->pApplicationName);
            (*ppApplicationInfo)->pEngineName = (const char*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkInstanceCreateInfo->pApplicationInfo->pEngineName);
        }
        if (pVkInstanceCreateInfo->enabledLayerCount > 0) {
            pVkInstanceCreateInfo->ppEnabledLayerNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledLayerNames);
            for (i = 0; i < pVkInstanceCreateInfo->enabledLayerCount; i++) {
                char** ppTmp = (char**)&pVkInstanceCreateInfo->ppEnabledLayerNames[i];
                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledLayerNames[i]);
            }
        }
        if (pVkInstanceCreateInfo->enabledExtensionCount > 0) {
            pVkInstanceCreateInfo->ppEnabledExtensionNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledExtensionNames);
            for (i = 0; i < pVkInstanceCreateInfo->enabledExtensionCount; i++) {
                char** ppTmp = (char**)&pVkInstanceCreateInfo->ppEnabledExtensionNames[i];
                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)pVkInstanceCreateInfo->ppEnabledExtensionNames[i]);
            }
        }
    }
    return pVkInstanceCreateInfo;
}

VkDeviceCreateInfo* interpret_VkDeviceCreateInfo(vktrace_trace_packet_header* pHeader, intptr_t ptr_variable) {
    VkDeviceCreateInfo* pVkDeviceCreateInfo =
        (VkDeviceCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)ptr_variable);
    uint32_t i;
    if (pVkDeviceCreateInfo != NULL) {
        if (pVkDeviceCreateInfo->queueCreateInfoCount > 0) {
            pVkDeviceCreateInfo->pQueueCreateInfos = (const VkDeviceQueueCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkDeviceCreateInfo->pQueueCreateInfos);
            for (i = 0; i < pVkDeviceCreateInfo->queueCreateInfoCount; i++) {
                float** ppQueuePriority = (float**)&pVkDeviceCreateInfo->pQueueCreateInfos[i].pQueuePriorities;
                *ppQueuePriority = (float*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)pVkDeviceCreateInfo->pQueueCreateInfos[i].pQueuePriorities);
            }
        }
        if (pVkDeviceCreateInfo->enabledLayerCount > 0) {
            pVkDeviceCreateInfo->ppEnabledLayerNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledLayerNames);
            for (i = 0; i < pVkDeviceCreateInfo->enabledLayerCount; i++) {
                char** ppTmp = (char**)&pVkDeviceCreateInfo->ppEnabledLayerNames[i];
                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledLayerNames[i]);
            }
        }
        if (pVkDeviceCreateInfo->enabledExtensionCount > 0) {
            pVkDeviceCreateInfo->ppEnabledExtensionNames = (const char* const*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledExtensionNames);
            for (i = 0; i < pVkDeviceCreateInfo->enabledExtensionCount; i++) {
                char** ppTmp = (char**)&pVkDeviceCreateInfo->ppEnabledExtensionNames[i];
                *ppTmp = (char*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)pVkDeviceCreateInfo->ppEnabledExtensionNames[i]);
            }
        }
        pVkDeviceCreateInfo->pEnabledFeatures = (const VkPhysicalDeviceFeatures*)vktrace_trace_packet_interpret_buffer_pointer(
            pHeader, (intptr_t)pVkDeviceCreateInfo->pEnabledFeatures);
    }
    return pVkDeviceCreateInfo;
}

void interpret_VkPipelineShaderStageCreateInfo(vktrace_trace_packet_header* pHeader, VkPipelineShaderStageCreateInfo* pShader) {
    if (pShader != NULL) {
        pShader->pName = (const char*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pShader->pName);
        // specialization info
        pShader->pSpecializationInfo = (const VkSpecializationInfo*)vktrace_trace_packet_interpret_buffer_pointer(
            pHeader, (intptr_t)pShader->pSpecializationInfo);
        if (pShader->pSpecializationInfo != NULL) {
            VkSpecializationInfo* pInfo = (VkSpecializationInfo*)pShader->pSpecializationInfo;
            pInfo->pMapEntries = (const VkSpecializationMapEntry*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pShader->pSpecializationInfo->pMapEntries);
            pInfo->pData =
                (const void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pShader->pSpecializationInfo->pData);
        }
    }
}

VkDeviceGroupDeviceCreateInfo* interpret_VkDeviceGroupDeviceCreateInfo(vktrace_trace_packet_header* pHeader,
                                                                       intptr_t ptr_variable) {
    VkDeviceGroupDeviceCreateInfo* pCreateInfo =
        (VkDeviceGroupDeviceCreateInfo*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)ptr_variable);
    uint32_t i;
    if (pCreateInfo != NULL) {
        if (pCreateInfo->physicalDeviceCount > 0) {
            pCreateInfo->pPhysicalDevices = (const VkPhysicalDevice*)vktrace_trace_packet_interpret_buffer_pointer(
                pHeader, (intptr_t)pCreateInfo->pPhysicalDevices);
            for (i = 0; i < pCreateInfo->physicalDeviceCount; i++) {
                VkPhysicalDevice** ppTmp = (VkPhysicalDevice**)&pCreateInfo->pPhysicalDevices[i];
                *ppTmp = (VkPhysicalDevice*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)pCreateInfo->pPhysicalDevices[i]);
            }
        }
    }
    return pCreateInfo;
}
#define InterpretPointerInPNext(_sName, _sPtrType, _sPtr)                                                        \
    do {                                                                                                         \
        _sName* struct_ptr_cur = (_sName*)(((VkApplicationInfo*)struct_ptr)->pNext);                             \
        struct_ptr_cur->_sPtr =                                                                                  \
            (_sPtrType*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)struct_ptr_cur->_sPtr); \
    } while (0)

void vkreplay_interpret_pnext_pointers(vktrace_trace_packet_header* pHeader, void* struct_ptr) {
    uint32_t i;
    if (!struct_ptr) return;

    while (((VkApplicationInfo*)struct_ptr)->pNext) {
        // Convert the pNext pointer
        VkApplicationInfo* pNext = (VkApplicationInfo*)((VkApplicationInfo*)struct_ptr)->pNext;
        ((VkApplicationInfo*)struct_ptr)->pNext = (void*)vktrace_trace_packet_interpret_buffer_pointer(pHeader, (intptr_t)pNext);

        // Convert pointers in pNext structures
        switch (((VkApplicationInfo*)(((VkApplicationInfo*)struct_ptr)->pNext))->sType) {
            case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO: {
                VkDescriptorSetLayoutCreateInfo* struct_ptr_cur =
                    (VkDescriptorSetLayoutCreateInfo*)(((VkApplicationInfo*)struct_ptr)->pNext);
                struct_ptr_cur->pBindings = (VkDescriptorSetLayoutBinding*)vktrace_trace_packet_interpret_buffer_pointer(
                    pHeader, (intptr_t)struct_ptr_cur->pBindings);
                for (i = 0; i < struct_ptr_cur->bindingCount; i++) {
                    VkSampler** ppSamplers = (VkSampler**)&(struct_ptr_cur->pBindings[i].pImmutableSamplers);
                    *ppSamplers = (VkSampler*)vktrace_trace_packet_interpret_buffer_pointer(
                        pHeader, (intptr_t)struct_ptr_cur->pBindings[i].pImmutableSamplers);
                }
            } break;
            case VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO:
                InterpretPointerInPNext(VkDeviceGroupDeviceCreateInfo, VkPhysicalDevice, pPhysicalDevices);
                break;
            case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
                InterpretPointerInPNext(VkDeviceGroupRenderPassBeginInfo, VkRect2D, pDeviceRenderAreas);
                break;
            case VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO:
                InterpretPointerInPNext(VkDeviceGroupSubmitInfo, uint32_t, pWaitSemaphoreDeviceIndices);
                InterpretPointerInPNext(VkDeviceGroupSubmitInfo, uint32_t, pCommandBufferDeviceMasks);
                InterpretPointerInPNext(VkDeviceGroupSubmitInfo, uint32_t, pSignalSemaphoreDeviceIndices);
                break;
            case VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO:
                InterpretPointerInPNext(VkBindBufferMemoryDeviceGroupInfo, uint32_t, pDeviceIndices);
                break;
            case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO:
                InterpretPointerInPNext(VkBindImageMemoryDeviceGroupInfo, uint32_t, pDeviceIndices);
                InterpretPointerInPNext(VkBindImageMemoryDeviceGroupInfo, VkRect2D, pSplitInstanceBindRegions);
                break;
            case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT:
                InterpretPointerInPNext(VkDescriptorSetVariableDescriptorCountAllocateInfoEXT, uint32_t, pDescriptorCounts);
                break;
            case VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT:
                InterpretPointerInPNext(VkValidationFlagsEXT, VkValidationCheckEXT, pDisabledValidationChecks);
                break;
            case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_W_SCALING_STATE_CREATE_INFO_NV:
                InterpretPointerInPNext(VkPipelineViewportWScalingStateCreateInfoNV, VkViewportWScalingNV, pViewportWScalings);
                break;
            case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SWIZZLE_STATE_CREATE_INFO_NV:
                InterpretPointerInPNext(VkPipelineViewportSwizzleStateCreateInfoNV, VkViewportSwizzleNV, pViewportSwizzles);
                break;
            case VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT:
                InterpretPointerInPNext(VkPipelineDiscardRectangleStateCreateInfoEXT, VkRect2D, pDiscardRectangles);
                break;
            case VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT:
                InterpretPointerInPNext(VkSampleLocationsInfoEXT, VkSampleLocationEXT, pSampleLocations);
                break;
            case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
                InterpretPointerInPNext(VkRenderPassSampleLocationsBeginInfoEXT, VkAttachmentSampleLocationsEXT,
                                        pAttachmentInitialSampleLocations);
                InterpretPointerInPNext(VkRenderPassSampleLocationsBeginInfoEXT, VkSubpassSampleLocationsEXT,
                                        pPostSubpassSampleLocations);
                break;
            case VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_MODULATION_STATE_CREATE_INFO_NV:
                InterpretPointerInPNext(VkPipelineCoverageModulationStateCreateInfoNV, float, pCoverageModulationTable);
                break;
            case VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO_KHR:
                InterpretPointerInPNext(VkImageFormatListCreateInfoKHR, VkFormat, pViewFormats);
                break;
            case VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO:
                InterpretPointerInPNext(VkRenderPassMultiviewCreateInfo, uint32_t, pViewMasks);
                InterpretPointerInPNext(VkRenderPassMultiviewCreateInfo, int32_t, pViewOffsets);
                InterpretPointerInPNext(VkRenderPassMultiviewCreateInfo, uint32_t, pCorrelationMasks);
                break;
#if defined(WIN32)
            case VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR:
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoKHR, VkDeviceMemory, pAcquireSyncs);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoKHR, uint64_t, pAcquireKeys);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoKHR, uint32_t, pAcquireTimeouts);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoKHR, VkDeviceMemory, pReleaseSyncs);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoKHR, uint64_t, pReleaseKeys);
                break;
            case VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV:
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoNV, VkDeviceMemory, pAcquireSyncs);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoNV, uint64_t, pAcquireKeys);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoNV, uint32_t, pAcquireTimeoutMilliseconds);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoNV, VkDeviceMemory, pReleaseSyncs);
                InterpretPointerInPNext(VkWin32KeyedMutexAcquireReleaseInfoNV, uint64_t, pReleaseKeys);
                break;
#endif
            case VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE:
                InterpretPointerInPNext(VkPresentTimesInfoGOOGLE, VkPresentTimeGOOGLE, pTimes);
                break;
            case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT:
                InterpretPointerInPNext(VkDescriptorSetLayoutBindingFlagsCreateInfoEXT, VkDescriptorBindingFlagsEXT, pBindingFlags);
                break;
            default:
                // The cases in this switch statement are only those pnext struct types that have
                // pointers inside them that need to be interpreted. The pnext list may contain
                // struct types that don't have pointers in them, which we simply skip over.
                break;
        }
        struct_ptr = (VkApplicationInfo*)((VkApplicationInfo*)struct_ptr)->pNext;
    }
}
