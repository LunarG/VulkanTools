/*
 * Copyright (c) 2016-2019 Advanced Micro Devices, Inc. All rights reserved.
 * Copyright (C) 2015-2017 LunarG, Inc.
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
 */

#include "vktrace_common.h"
#include "vktrace_pageguard_memorycopy.h"
#include "vktrace_lib_pagestatusarray.h"
#include "vktrace_lib_pageguardmappedmemory.h"
#include "vktrace_lib_pageguardcapture.h"
#include "vktrace_lib_pageguard.h"
#include "vktrace_lib_trim.h"

static const bool PAGEGUARD_PAGEGUARD_ENABLE_DEFAULT = true;

static const VkDeviceSize PAGEGUARD_TARGET_RANGE_SIZE_DEFAULT = 2;  // cover all reasonal mapped memory size, the mapped memory size
                                                                    // may be less than 1 page, so processing for mapped memory
                                                                    // size<1 page is already added,
// other value: 32 * 1024 * 1024 (32M),  64M, this is the size which cause DOOM4 capture very slow.
static const VkDeviceSize PAGEGUARD_PAGEGUARD_TARGET_RANGE_SIZE_MIN = 1;  // already tested: 2,2M,4M,32M,64M, because commonly page
                                                                          // size is 4k, so only range size=2 can cover small size
                                                                          // mapped memory.

static vktrace_sem_id ref_amount_sem_id;  // TODO if vktrace implement cross platform lib or dll load or unload function, this sem
                                          // can be putted in those functions, but now we leave it to process quit.
static bool ref_amount_sem_id_create_success = vktrace_sem_create(&ref_amount_sem_id, 1);
static vktrace_sem_id map_lock_sem_id;
#if defined(PLATFORM_LINUX)
static bool map_lock_sem_id_create_success __attribute__((unused)) = vktrace_sem_create(&map_lock_sem_id, 1);
#else
static bool map_lock_sem_id_create_success = vktrace_sem_create(&map_lock_sem_id, 1);
#endif

void pageguardEnter() {
    // Reference this variable to avoid compiler warnings
    if (!map_lock_sem_id_create_success) {
        vktrace_LogError("Semaphore create failed!");
    }
    vktrace_sem_wait(map_lock_sem_id);
}

void pageguardExit() { vktrace_sem_post(map_lock_sem_id); }

VkDeviceSize& ref_target_range_size() {
    static VkDeviceSize OPTTargetRangeSize = PAGEGUARD_TARGET_RANGE_SIZE_DEFAULT;
    return OPTTargetRangeSize;
}

void set_pageguard_target_range_size(VkDeviceSize newrangesize) {
    VkDeviceSize& refTargetRangeSize = ref_target_range_size();

    refTargetRangeSize = newrangesize;
}

#if defined(WIN32)
LONG WINAPI PageGuardExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo);
#endif

PVOID OPTHandler = nullptr;        // use to remove page guard handler
uint32_t OPTHandlerRefAmount = 0;  // for persistent map and multi-threading environment, map and unmap maybe overlap, we need to
                                   // make sure remove handler after all persistent map has been unmapped.

// return if user using VK_EXT_external_memory_host to disable shadow memory
bool UseMappedExternalHostMemoryExtension() {
    // TODO: read ENV variable to decide return value.
    return false;
}

// return if enable pageguard;
// if enable page guard, then check if need to update target range size, page guard only work for those persistent mapped memory
// which >= target range size.
bool getPageGuardEnableFlag() {
    static bool EnablePageGuard = PAGEGUARD_PAGEGUARD_ENABLE_DEFAULT;
    static bool FirstTimeRun = true;
    if (FirstTimeRun) {
        FirstTimeRun = false;
        const char* env_pageguard = vktrace_get_global_var(VKTRACE_PMB_ENABLE_ENV);
        if (env_pageguard) {
            int envvalue;
            if (sscanf(env_pageguard, "%d", &envvalue) == 1) {
                if (envvalue) {
                    EnablePageGuard = true;
                    const char* env_target_range_size = vktrace_get_global_var(_VKTRACE_PMB_TARGET_RANGE_SIZE_ENV);
                    if (env_target_range_size) {
                        VkDeviceSize rangesize;
                        if (sscanf(env_target_range_size, "%" PRIx64, &rangesize) == 1) {
                            if (rangesize > PAGEGUARD_PAGEGUARD_TARGET_RANGE_SIZE_MIN) {
                                set_pageguard_target_range_size(rangesize);
                            }
                        }
                    }
                } else {
                    EnablePageGuard = false;
                }
            }
        }
    }
    return EnablePageGuard;
}

bool getEnableReadProcessFlag(const char* name) {
    bool EnableReadProcessFlag = (vktrace_get_global_var(name) != NULL);
    return EnableReadProcessFlag;
}

bool getEnableReadPMBFlag() {
    static bool EnableReadPMB;
    static bool FirstTimeRun = true;
    if (FirstTimeRun) {
        EnableReadPMB = getEnableReadProcessFlag(VKTRACE_PAGEGUARD_ENABLE_READ_PMB_ENV);
        FirstTimeRun = false;
    }
    return EnableReadPMB;
}

bool getEnableReadPMBPostProcessFlag() {
    static bool EnableReadPMBPostProcess;
    static bool FirstTimeRun = true;
    if (FirstTimeRun) {
        EnableReadPMBPostProcess = getEnableReadProcessFlag(VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS_ENV);
        FirstTimeRun = false;
    }
    return EnableReadPMBPostProcess;
}

bool getEnablePageGuardLazyCopyFlag() {
    static bool EnablePageGuardLazyCopyFlag;
    static bool FirstTimeRun = true;
    if (FirstTimeRun) {
        EnablePageGuardLazyCopyFlag = (vktrace_get_global_var(VKTRACE_PAGEGUARD_ENABLE_LAZY_COPY_ENV) != NULL);
        FirstTimeRun = false;
    }
    return EnablePageGuardLazyCopyFlag;
}

#if defined(PLATFORM_LINUX)
static struct sigaction g_old_sa;
#endif

void setPageGuardExceptionHandler() {
    // Reference this variable to avoid compiler warnings
    if (!ref_amount_sem_id_create_success) {
        vktrace_LogError("Semaphore create failed!");
    }

    vktrace_sem_wait(ref_amount_sem_id);
    if (!OPTHandler) {
#if defined(WIN32)
        OPTHandler = AddVectoredExceptionHandler(1, PageGuardExceptionHandler);
#else
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = PageGuardExceptionHandler;
        if (sigaction(SIGSEGV, &sa, &g_old_sa) == -1) {
            OPTHandler = nullptr;
            vktrace_LogError("Set page guard exception handler failed !");
        } else {
            OPTHandler = (void*)PageGuardExceptionHandler;
        }
#endif
        OPTHandlerRefAmount = 1;
    } else {
        OPTHandlerRefAmount++;
    }
    vktrace_sem_post(ref_amount_sem_id);
}

void removePageGuardExceptionHandler() {
    vktrace_sem_wait(ref_amount_sem_id);
    if (OPTHandler) {
        if (OPTHandlerRefAmount) {
            OPTHandlerRefAmount--;
        }
        if (!OPTHandlerRefAmount) {
#if defined(WIN32)
            RemoveVectoredExceptionHandler(OPTHandler);
#else
            if (sigaction(SIGSEGV, &g_old_sa, NULL) == -1) {
                vktrace_LogError("Remove page guard exception handler failed !");
            }
#endif
            OPTHandler = nullptr;
        }
    }
    vktrace_sem_post(ref_amount_sem_id);
}

uint64_t pageguardGetAdjustedSize(uint64_t size) {
    uint64_t pagesize = pageguardGetSystemPageSize();
    if (size % pagesize) {
        size = size - (size % pagesize) + pagesize;
    }
    return size;
}

#if defined(PLATFORM_LINUX)
// Keep a map of memory allocations and sizes.
// We need the size when we want to free the memory on Linux.
static std::unordered_map<void*, size_t> allocateMemoryMap;
#endif

// Page guard only works for virtual memory. Real device memory
// sometimes doesn't have a page concept, so we can't use page guard
// to track it (or check dirty bits in /proc/<pid>/pagemap).
// So we allocate virtual memory to return to the app and we
// keep it sync'ed it with real device memory.
void* pageguardAllocateMemory(uint64_t size) {
    void* pMemory = nullptr;
    if (size != 0) {
#if defined(WIN32)
        pMemory = (PBYTE)VirtualAlloc(nullptr, (size_t)pageguardGetAdjustedSize(size), MEM_WRITE_WATCH | MEM_RESERVE | MEM_COMMIT,
                                      PAGE_READWRITE);
#else
        pMemory = mmap(NULL, pageguardGetAdjustedSize(size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (pMemory != nullptr) allocateMemoryMap[pMemory] = pageguardGetAdjustedSize(size);
#endif
    }
    if (pMemory == nullptr) vktrace_LogError("pageguardAllocateMemory(%d) memory allocation failed", size);
    return pMemory;
}

void pageguardFreeMemory(void* pMemory) {
    if (pMemory) {
#if defined(WIN32)
        VirtualFree(pMemory, 0, MEM_RELEASE);
#else
        munmap(pMemory, allocateMemoryMap[pMemory]);
        allocateMemoryMap.erase(pMemory);
#endif
    }
}

uint64_t pageguardGetSystemPageSize() {
#if defined(PLATFORM_LINUX)
    return getpagesize();
#elif defined(WIN32)
    SYSTEM_INFO sSysInfo;
    GetSystemInfo(&sSysInfo);
    return sSysInfo.dwPageSize;
#endif
}

void setFlagTovkFlushMappedMemoryRangesSpecial(PBYTE pOPTPackageData) {
    PageGuardChangedBlockInfo* pChangedInfoArray = (PageGuardChangedBlockInfo*)pOPTPackageData;
    pChangedInfoArray[0].reserve0 = pChangedInfoArray[0].reserve0 | PAGEGUARD_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES;
}

PageGuardCapture& getPageGuardControlInstance() {
    static PageGuardCapture OPTControl;
    return OPTControl;
}

void flushTargetChangedMappedMemory(LPPageGuardMappedMemory TargetMappedMemory, vkFlushMappedMemoryRangesFunc pFunc,
                                    VkMappedMemoryRange* pMemoryRanges) {
    bool newMemoryRangesInside = (pMemoryRanges == nullptr);
    if (newMemoryRangesInside) {
        pMemoryRanges = new VkMappedMemoryRange[1];
        assert(pMemoryRanges);
    }
    pMemoryRanges[0].memory = TargetMappedMemory->getMappedMemory();
    pMemoryRanges[0].offset = TargetMappedMemory->getMappedOffset();
    pMemoryRanges[0].pNext = nullptr;
    pMemoryRanges[0].size = TargetMappedMemory->getMappedSize();
    pMemoryRanges[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    (*pFunc)(TargetMappedMemory->getMappedDevice(), 1, pMemoryRanges);
    if (newMemoryRangesInside) {
        delete[] pMemoryRanges;
    }
}

void flushAllChangedMappedMemory(vkFlushMappedMemoryRangesFunc pFunc) {
    LPPageGuardMappedMemory pMappedMemoryTemp;
    uint64_t amount = getPageGuardControlInstance().getMapMemory().size();
    if (amount) {
        int i = 0;
        VkMappedMemoryRange* pMemoryRanges = new VkMappedMemoryRange[1];  // amount
        for (std::unordered_map<VkDeviceMemory, PageGuardMappedMemory>::iterator it =
                 getPageGuardControlInstance().getMapMemory().begin();
             it != getPageGuardControlInstance().getMapMemory().end(); it++) {
            pMappedMemoryTemp = &(it->second);
            flushTargetChangedMappedMemory(pMappedMemoryTemp, pFunc, pMemoryRanges);
            i++;
        }
        delete[] pMemoryRanges;
    }
}

void resetAllReadFlagAndPageGuard() {
    LPPageGuardMappedMemory pMappedMemoryTemp;
    uint64_t amount = getPageGuardControlInstance().getMapMemory().size();
    if (amount) {
        int i = 0;
        for (std::unordered_map<VkDeviceMemory, PageGuardMappedMemory>::iterator it =
                 getPageGuardControlInstance().getMapMemory().begin();
             it != getPageGuardControlInstance().getMapMemory().end(); it++) {
            pMappedMemoryTemp = &(it->second);
            pMappedMemoryTemp->resetMemoryObjectAllReadFlagAndPageGuard();
            i++;
        }
    }
}

#if defined(WIN32)
LONG WINAPI PageGuardExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    LONG resultCode = EXCEPTION_CONTINUE_SEARCH;
    pageguardEnter();
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION) {
        VkDeviceSize OffsetOfAddr;
        PBYTE pBlock;
        VkDeviceSize BlockSize;
        PBYTE addr = reinterpret_cast<PBYTE>(ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
        bool bWrite = (ExceptionInfo->ExceptionRecord->ExceptionInformation[0] != NULL);

        LPPageGuardMappedMemory pMappedMem =
            getPageGuardControlInstance().findMappedMemoryObject(addr, &OffsetOfAddr, &pBlock, &BlockSize);
        if (pMappedMem != nullptr) {
            // Make sure pageguard is cleared because in multi-thread environment there's possibility
            // that pageguard be re-armed during pageguard be triggered this time to current location.
            // If pageguard be rearmed, the following sync between real mapped memory to shadow
            // mapped memory will cause deadlock.
            DWORD oldProt = 0;
            VirtualProtect(pBlock, static_cast<SIZE_T>(BlockSize), PAGE_READWRITE, &oldProt);
            int64_t index = pMappedMem->getIndexOfChangedBlockByAddr(addr);
            if (index >= 0) {
                if (!getEnableReadPMBFlag() || bWrite) {
                    if ((!pMappedMem->isMappedBlockLoaded(index)) && (getEnablePageGuardLazyCopyFlag())) {
                        // the page never get accessed since the time of the shadow
                        // memory creation in map process. so here we copy the page
                        // from real mapped memory to shadow memory. after the memcpy,
                        // we set the loaded flag, next time we don't do the memcopy
                        // again in this page guard write process.
                        vktrace_pageguard_memcpy(pBlock,
                                                 pMappedMem->getRealMappedDataPointer() + OffsetOfAddr - OffsetOfAddr % BlockSize,
                                                 pMappedMem->getMappedBlockSize(index));
                        pMappedMem->setMappedBlockLoaded(index, true);
                    }

                    pMappedMem->setMappedBlockChanged(index, true, BLOCK_FLAG_ARRAY_CHANGED);
                } else {
#if !defined(PAGEGUARD_ADD_PAGEGUARD_ON_REAL_MAPPED_MEMORY)
                    if ((!pMappedMem->isMappedBlockLoaded(index)) && (getEnablePageGuardLazyCopyFlag())) {
                        // Target app read the page which is never accessed since the
                        // shadow memory creation in map process.
                        // here we only set the loaded flag, we still need to do memcpy
                        // in the following reading acess to the page,that is different
                        // with page guard process when target app write to the page.
                        // the loaded flag is setted here only to make sure the following
                        // write access not to do the memcpy again. because the memcpy
                        // in read process is to capture GPU side change which is needed
                        // by CPU side, it is not to replace initial sync from real
                        // mapped memory to shadow memory in map process.

                        pMappedMem->setMappedBlockLoaded(index, true);
                    }
                    vktrace_pageguard_memcpy(pBlock,
                                             pMappedMem->getRealMappedDataPointer() + OffsetOfAddr - OffsetOfAddr % BlockSize,
                                             pMappedMem->getMappedBlockSize(index));
                    pMappedMem->setMappedBlockChanged(index, true, BLOCK_FLAG_ARRAY_READ);
                    if (getEnableReadPMBPostProcessFlag()) {
                        pMappedMem->setMappedBlockChanged(index, true, BLOCK_FLAG_ARRAY_CHANGED);
                    }

#else
                    pMappedMem->setMappedBlockChanged(index, true, BLOCK_FLAG_ARRAY_CHANGED);
#endif
                }
                resultCode = EXCEPTION_CONTINUE_EXECUTION;
            }
        }
    }
    pageguardExit();
    return resultCode;
}
#else
void PageGuardExceptionHandler(int sig, siginfo_t* si, void* unused) {
    if (sig == SIGSEGV) {
        VkDeviceSize OffsetOfAddr;
        PBYTE pBlock;
        VkDeviceSize BlockSize;
        PBYTE addr = (PBYTE)si->si_addr;
        pageguardEnter();
        LPPageGuardMappedMemory pMappedMem =
            getPageGuardControlInstance().findMappedMemoryObject(addr, &OffsetOfAddr, &pBlock, &BlockSize);
        if (pMappedMem) {
            uint64_t index = pMappedMem->getIndexOfChangedBlockByAddr(addr);
            pMappedMem->setMappedBlockChanged(index, true, BLOCK_FLAG_ARRAY_CHANGED);
            if (mprotect(pMappedMem->getMappedDataPointer() + index * pageguardGetSystemPageSize(),
                         (SIZE_T)pMappedMem->getMappedBlockSize(index), (PROT_READ | PROT_WRITE)) == -1) {
                vktrace_LogError("Clear memory protect on page(%d) failed !", index);
            }
        } else if (g_old_sa.sa_sigaction) {
            g_old_sa.sa_sigaction(sig, si, unused);
        } else {
            vktrace_LogError("Unhandled SIGSEGV on address: 0x%lx !", (long)addr);
            exit(EXIT_FAILURE);
        }
        pageguardExit();
    }
}
#endif

// The function source code is modified from __HOOKED_vkFlushMappedMemoryRanges
// for coherent map, need this function to dump data so simulate target application write data when playback.
VkResult vkFlushMappedMemoryRangesWithoutAPICall(VkDevice device, uint32_t memoryRangeCount,
                                                 const VkMappedMemoryRange* pMemoryRanges) {
    VkResult result = VK_SUCCESS;
    vktrace_trace_packet_header* pHeader;
    uint64_t rangesSize = 0;
    uint64_t dataSize = 0;
    uint32_t iter;
    packet_vkFlushMappedMemoryRanges* pPacket = nullptr;

#if defined(USE_PAGEGUARD_SPEEDUP)
    PBYTE* ppPackageData = new PBYTE[memoryRangeCount];
    getPageGuardControlInstance().vkFlushMappedMemoryRangesPageGuardHandle(
        device, memoryRangeCount, pMemoryRanges, ppPackageData);  // the packet is not needed if no any change on data of all ranges
#endif

    // find out how much memory is in the ranges
    for (iter = 0; iter < memoryRangeCount; iter++) {
        VkMappedMemoryRange* pRange = (VkMappedMemoryRange*)&pMemoryRanges[iter];
        rangesSize += vk_size_vkmappedmemoryrange(pRange);
        dataSize += (size_t)pRange->size;
    }
#if defined(USE_PAGEGUARD_SPEEDUP)
    dataSize = getPageGuardControlInstance().getALLChangedPackageSizeInMappedMemory(device, memoryRangeCount, pMemoryRanges,
                                                                                    ppPackageData);
#endif
    CREATE_TRACE_PACKET(vkFlushMappedMemoryRanges, rangesSize + sizeof(void*) * memoryRangeCount + dataSize);
    pPacket = interpret_body_as_vkFlushMappedMemoryRanges(pHeader);

    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->pMemoryRanges), rangesSize, pMemoryRanges);
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->pMemoryRanges));

    // insert into packet the data that was written by CPU between the vkMapMemory call and here
    // create a temporary local ppData array and add it to the packet (to reserve the space for the array)
    void** ppTmpData = (void**)malloc(memoryRangeCount * sizeof(void*));
    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData), sizeof(void*) * memoryRangeCount, ppTmpData);
    free(ppTmpData);

    // now the actual memory
    vktrace_enter_critical_section(&g_memInfoLock);
    for (iter = 0; iter < memoryRangeCount; iter++) {
        VkMappedMemoryRange* pRange = (VkMappedMemoryRange*)&pMemoryRanges[iter];
        VKAllocInfo* pEntry = find_mem_info_entry(pRange->memory);

        if (pEntry != nullptr) {
            assert(pEntry->handle == pRange->memory);
            assert(pEntry->totalSize >= (pRange->size + pRange->offset));
            assert(pEntry->totalSize >= pRange->size);
#if defined(USE_PAGEGUARD_SPEEDUP)
            if (dataSize > 0) {
                LPPageGuardMappedMemory pOPTMemoryTemp = getPageGuardControlInstance().findMappedMemoryObject(device, pRange);
                VkDeviceSize OPTPackageSizeTemp = 0;
                if (pOPTMemoryTemp) {
                    PBYTE pOPTDataTemp = pOPTMemoryTemp->getChangedDataPackage(&OPTPackageSizeTemp);
                    if (pEntry->props & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
                        setFlagTovkFlushMappedMemoryRangesSpecial(pOPTDataTemp);
                    }
                    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData[iter]), OPTPackageSizeTemp, pOPTDataTemp);
                    pOPTMemoryTemp->clearChangedDataPackage();
                    pOPTMemoryTemp->resetMemoryObjectAllChangedFlagAndPageGuard();
                } else {
                    PBYTE pOPTDataTemp =
                        getPageGuardControlInstance().getChangedDataPackageOutOfMap(ppPackageData, iter, &OPTPackageSizeTemp);
                    if (pEntry->props & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
                        setFlagTovkFlushMappedMemoryRangesSpecial(pOPTDataTemp);
                    }
                    vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData[iter]), OPTPackageSizeTemp, pOPTDataTemp);
                    getPageGuardControlInstance().clearChangedDataPackageOutOfMap(ppPackageData, iter);
                }
            }
#else
            vktrace_add_buffer_to_trace_packet(pHeader, (void**)&(pPacket->ppData[iter]), pRange->size,
                                               pEntry->pData + pRange->offset);
#endif
            vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData[iter]));
            pEntry->didFlush = true;
        } else {
            vktrace_LogError("Failed to copy app memory into trace packet (idx = %u) on vkFlushedMappedMemoryRanges",
                             pHeader->global_packet_index);
        }
    }
#if defined(USE_PAGEGUARD_SPEEDUP)
    delete[] ppPackageData;
#endif
    vktrace_leave_critical_section(&g_memInfoLock);

    // now finalize the ppData array since it is done being updated
    vktrace_finalize_buffer_address(pHeader, (void**)&(pPacket->ppData));

    // result = mdd(device)->devTable.FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    vktrace_set_packet_entrypoint_end_time(pHeader);
    pPacket->device = device;
    pPacket->memoryRangeCount = memoryRangeCount;
    pPacket->result = result;

    if (!g_trimEnabled) {
        // trim not enabled, send packet as usual
        FINISH_TRACE_PACKET();
    } else {
        vktrace_finalize_trace_packet(pHeader);
        if (g_trimIsInTrim) {
            // Currently tracing the frame, so need to track references & store packet to write post-tracing.
            trim::write_packet(pHeader);
        } else {
            vktrace_delete_trace_packet(&pHeader);
        }
    }
    return result;
}
