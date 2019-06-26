/*
 * Copyright (c) 2016-2019 Advanced Micro Devices, Inc. All rights reserved.
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

// OPT: Optimization by using page-guard for speed up capture
//     The speed is extremely slow when use vktrace to capture DOOM4. It took over half a day and 900G of trace for a capture from
//     beginning to the game menu.
//     The reason that caused such slow capture is DOOM updates a big mapped memory(over 67M) frequently, vktrace copies this memory
//     block to harddrive when DOOM calls vkFlushmappedMemory to update it every time.
//     Here we use page guard to record which page of big memory block has been changed and only save those changed pages, it make
//     the capture time reduce to round 15 minutes, the trace file size is round 40G,
//     The Playback time for these trace file is round 7 minutes(on Win10/AMDFury/32GRam/I5 system).

#include "vktrace_pageguard_memorycopy.h"
#include "vktrace_lib_pagestatusarray.h"
#include "vktrace_lib_pageguardmappedmemory.h"
#include "vktrace_lib_pageguardcapture.h"
#include "vktrace_lib_pageguard.h"

VkDevice &PageGuardMappedMemory::getMappedDevice() { return MappedDevice; }

VkDeviceMemory &PageGuardMappedMemory::getMappedMemory() { return MappedMemory; }

VkDeviceSize &PageGuardMappedMemory::getMappedOffset() { return MappedOffset; }

PBYTE &PageGuardMappedMemory::getRealMappedDataPointer() { return pRealMappedData; }

PBYTE &PageGuardMappedMemory::getMappedDataPointer() { return pMappedData; }

VkDeviceSize &PageGuardMappedMemory::getMappedSize() { return MappedSize; }

PageGuardMappedMemory::PageGuardMappedMemory()
    : MappedDevice(nullptr),
      MappedMemory((VkDeviceMemory) nullptr),
      MappedOffset(0),
      pMappedData(nullptr),
      pRealMappedData(nullptr),
      pChangedDataPackage(nullptr),
      MappedSize(0),
      PageGuardSize(pageguardGetSystemPageSize()),
      pPageStatus(nullptr),
      BlockConflictError(false),
      PageSizeLeft(0),
      StartingAddressOffset(0),
      PageGuardAmount(0) {}

PageGuardMappedMemory::~PageGuardMappedMemory() {}

// return true if currently shadow memory is being used and valid.
bool PageGuardMappedMemory::isUseCopyForRealMappedMemory() {
    bool useCopyForRealMappedMemory = false;
    if (pRealMappedData != nullptr) {
        if (pRealMappedData != pMappedData) {
            useCopyForRealMappedMemory = true;
        }
    }
    return useCopyForRealMappedMemory;
}

bool PageGuardMappedMemory::getChangedRangeByIndex(uint64_t index, PBYTE *pAddress, VkDeviceSize *pBlockSize) {
    bool isValidResult = false;
    if (index < PageGuardAmount) {
        isValidResult = true;
        if (pAddress) {
            *pAddress = pMappedData + getMappedBlockOffset(index);
        }
        if (pBlockSize) {
            *pBlockSize = getMappedBlockSize(index);
        }
    }
    return isValidResult;
}

// if return value ==-1, mean addr is out of page guard.
int64_t PageGuardMappedMemory::getIndexOfChangedBlockByAddr(PBYTE addr) {
    int64_t addrOffset = addr - pMappedData;
    int64_t indexOfChangedBlockByAddr = -1;
    if ((addrOffset >= 0) && ((VkDeviceSize)addrOffset < MappedSize)) {
        indexOfChangedBlockByAddr = (addrOffset + StartingAddressOffset) / PageGuardSize;
    }
    return indexOfChangedBlockByAddr;
}

void PageGuardMappedMemory::setMappedBlockChanged(uint64_t index, bool changed, int which) {
    if (index < PageGuardAmount) {
        switch (which) {
            case BLOCK_FLAG_ARRAY_CHANGED:
                pPageStatus->setBlockChangedArray(index, changed);
                break;

            case BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT:
                pPageStatus->setBlockChangedArraySnapshot(index, changed);
                break;

            case BLOCK_FLAG_ARRAY_READ_SNAPSHOT:
                pPageStatus->setBlockReadArraySnapshot(index, changed);
                break;

            default:  // BLOCK_FLAG_ARRAY_READ
                pPageStatus->setBlockReadArray(index, changed);
                break;
        }
    }
}

bool PageGuardMappedMemory::isMappedBlockChanged(uint64_t index, int which) {
    bool mappedBlockChanged = false;
    if (index < PageGuardAmount) {
        switch (which) {
            case BLOCK_FLAG_ARRAY_CHANGED:
                mappedBlockChanged = pPageStatus->getBlockChangedArray(index);
                break;

            case BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT:
                mappedBlockChanged = pPageStatus->getBlockChangedArraySnapshot(index);
                break;

            case BLOCK_FLAG_ARRAY_READ_SNAPSHOT:
                mappedBlockChanged = pPageStatus->getBlockReadArraySnapshot(index);
                break;

            case BLOCK_FLAG_ARRAY_READ:
                mappedBlockChanged = pPageStatus->getBlockReadArray(index);
                break;

            default:
                assert(false);
                break;
        }
    }
    return mappedBlockChanged;
}

bool PageGuardMappedMemory::isMappedBlockLoaded(uint64_t index) { return pPageStatus->getBlockFirstTimeLoadArray(index); }

void PageGuardMappedMemory::setMappedBlockLoaded(uint64_t index, bool bLoaded) {
    pPageStatus->setBlockFirstTimeLoadArray(index, bLoaded);
}

uint64_t PageGuardMappedMemory::getMappedBlockSize(uint64_t index) {
    uint64_t mappedBlockSize = PageGuardSize;
    if (index == 0) {
        if (PageGuardAmount == 1) {
            if (PageSizeLeft == 0) {
                mappedBlockSize = PageGuardSize - StartingAddressOffset;
            } else {
                mappedBlockSize = PageSizeLeft - StartingAddressOffset;
            }
        } else {
            mappedBlockSize = PageGuardSize - StartingAddressOffset;
        }
    } else {
        if ((index + 1) == PageGuardAmount) {
            if (PageSizeLeft) {
                mappedBlockSize = PageSizeLeft;
            }
        }
    }
    return mappedBlockSize;
}

uint64_t PageGuardMappedMemory::getMappedBlockOffset(uint64_t index) {
    uint64_t mappedBlockOffset = 0;
    if (index < PageGuardAmount) {
        if (index != 0) {
            mappedBlockOffset = index * PageGuardSize - StartingAddressOffset;
        }
    }
    return mappedBlockOffset;
}

bool PageGuardMappedMemory::isNoMappedBlockChanged() {
    bool noMappedBlockChanged = true;
    for (uint64_t i = 0; i < PageGuardAmount; i++) {
        if (isMappedBlockChanged(i, BLOCK_FLAG_ARRAY_CHANGED)) {
            noMappedBlockChanged = false;
            break;
        }
    }
    return noMappedBlockChanged;
}

#if defined(WIN32)
uint64_t PageGuardMappedMemory::getWriteWatchForPage(DWORD dwFlags, void *pgAddr) {
    uint64_t pageSize = PageGuardSize;
    PVOID addresses[1];
    ULONG granularity;
    ULONG_PTR count = 1;
    UINT returncode;

    returncode = GetWriteWatch(WRITE_WATCH_FLAG_RESET, pgAddr, static_cast<size_t>(pageSize), &addresses[0], &count, &granularity);
    if (returncode != 0) {
        vktrace_LogError("GetWriteWatch fail, the error code is: %d", returncode);
    }
    return count;
}
#endif

void PageGuardMappedMemory::resetMemoryObjectAllChangedFlagAndPageGuard() {
    for (uint64_t i = 0; i < PageGuardAmount; i++) {
        if (isMappedBlockChanged(i, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT)) {
#if defined(WIN32)
            if (false == UseMappedExternalHostMemoryExtension()) {
                // We reset pageguard only when using pageguard. If using
                // writewatch + external host memory extension, we don't need
                // the following process.
                uint64_t pageSize = PageGuardSize;
                uint64_t pmask = ~(pageSize - 1);
                ULONG_PTR count = 1;
                DWORD oldProt;
                void *pgAddr = reinterpret_cast<void *>(pMappedData + (i * PageGuardSize));
                VirtualProtect(pgAddr, static_cast<SIZE_T>(getMappedBlockSize(i)), PAGE_READWRITE | PAGE_GUARD, &oldProt);
                count = getWriteWatchForPage(WRITE_WATCH_FLAG_RESET, pgAddr);
                if (count == 1) {
                    // Page was modified after we copied it, so mark the page as changed.
                    VirtualProtect(pgAddr, static_cast<SIZE_T>(getMappedBlockSize(i)), PAGE_READWRITE, &oldProt);
                    setMappedBlockChanged(i, true, BLOCK_FLAG_ARRAY_CHANGED);

                    // for one page, there are two ways that trigger page guard and then the page
                    // need to be rearmed: write and read, we also use two flags for the page
                    // to record page guard is triggered by write (dirty) or by read, then base on
                    // the two flags, we rearm page guard in different functions.
                    // here if GetWriteWatch detect dirty page, we set it's a dirty page, also
                    // need to clear another read flag to avoid dead lock: if two flags
                    // all set to true, and if already rearm page guard by read related process,
                    // then when write the dirty page (because it's marked as dirty page), copy
                    // the dirty page will trigger unexpected page guard, cause a deadlock.
                    setMappedBlockChanged(i, false, BLOCK_FLAG_ARRAY_READ);
                }
            }
#else
            if (mprotect(pMappedData + i * PageGuardSize, (SIZE_T)getMappedBlockSize(i), PROT_READ) == -1) {
                vktrace_LogError("Set memory protect on page(%d) failed !", i);
            }
#endif
            setMappedBlockChanged(i, false, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT);
        }
    }
}

void PageGuardMappedMemory::resetMemoryObjectAllReadFlagAndPageGuard() {
    backupBlockReadArraySnapshot();
    for (uint64_t i = 0; i < PageGuardAmount; i++) {
        if (isMappedBlockChanged(i, BLOCK_FLAG_ARRAY_READ_SNAPSHOT)) {
#if defined(WIN32)
            DWORD oldProt;
            VirtualProtect(pMappedData + i * PageGuardSize, (SIZE_T)getMappedBlockSize(i), PAGE_READWRITE | PAGE_GUARD, &oldProt);
#else
            if (mprotect(pMappedData + i * PageGuardSize, (SIZE_T)getMappedBlockSize(i), PROT_READ) == -1) {
                vktrace_LogError("Set memory protect on page(%d) failed !", i);
            }
#endif
            setMappedBlockChanged(i, false, BLOCK_FLAG_ARRAY_READ_SNAPSHOT);
        }
    }
}

bool PageGuardMappedMemory::setAllPageGuardAndFlag(bool bSetPageGuard, bool bSetBlockChanged) {
    // Note that we don't call GetWriteWatch from this method to check write counts on pages.
    // This is because this method is only called during vkMapMemory and vkUnMapMemory, so
    // we don't need to be concerned with missing a write on mapped memory, since the memory
    // is either being created or destroyed.  If this method were to be called from some other
    // api call besides vkMapMemory or vkUnmapMemory, this method would have to be modified to
    // check write counts of pages.

    bool setSuccessfully = true;
#if defined(WIN32)
    DWORD dwMemSetting = bSetPageGuard ? (PAGE_READWRITE | PAGE_GUARD) : PAGE_READWRITE;
#else
    int prot = bSetPageGuard ? PROT_READ : (PROT_READ | PROT_WRITE);
#endif

    for (uint64_t i = 0; i < PageGuardAmount; i++) {
#if defined(WIN32)
        if (UseMappedExternalHostMemoryExtension()) {
            uint64_t pageSize = PageGuardSize;
            uint64_t pmask = ~(pageSize - 1);
            void *pgAddr = reinterpret_cast<void *>(pMappedData + getMappedBlockOffset(i));
            getWriteWatchForPage(WRITE_WATCH_FLAG_RESET, pgAddr);
        } else {
            DWORD oldProt, dwErr;
            if (!VirtualProtect(pMappedData + getMappedBlockOffset(i), (SIZE_T)getMappedBlockSize(i), dwMemSetting, &oldProt)) {
                dwErr = GetLastError();
                setSuccessfully = false;
            }
        }
#else
        if (mprotect(pMappedData + i * PageGuardSize, (SIZE_T)getMappedBlockSize(i), prot) == -1) {
            vktrace_LogError("Set memory protect(%d) on page(%d) failed !", prot, i);
            setSuccessfully = false;
        }
#endif
        setMappedBlockChanged(i, bSetBlockChanged, BLOCK_FLAG_ARRAY_CHANGED);
    }
    return setSuccessfully;
}
VkMemoryPropertyFlags PageGuardMappedMemory::getMemoryPropertyFlags(std::unordered_map<VkDevice, VkPhysicalDevice> &mapDevice,
                                                                    VkDevice device, uint32_t memoryTypeIndex) {
    VkPhysicalDevice physicalDevice = mapDevice[device];
    VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
    mid(physicalDevice)->instTable.GetPhysicalDeviceMemoryProperties(physicalDevice, &PhysicalDeviceMemoryProperties);
    assert(memoryTypeIndex < PhysicalDeviceMemoryProperties.memoryTypeCount);
    return PhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;
}

// VkDeviceSize *pMemoryWholeSize, return the memory object size. if pMemorySize == nullptr, ignore it;
// VkMemoryPropertyFlags *pMemoryPropertyFlags, return MemoryPropertyFlags of the memory object, if pMemoryPropertyFlags == nullptr,
// ignore it;
bool PageGuardMappedMemory::getMemoryProperty(std::unordered_map<VkDeviceMemory, VkMemoryAllocateInfo> &mapMemoryCreateInfo,
                                              std::unordered_map<VkDevice, VkPhysicalDevice> &mapDevice,
                                              VkDeviceSize *pMemoryWholeSize, VkMemoryPropertyFlags *pMemoryPropertyFlags) {
    bool getMemoryPropertySuccess = false;
    auto findAllocateInfo = mapMemoryCreateInfo.find(getMappedMemory());
    VkMemoryAllocateInfo memoryAllocateInfo;
    if (findAllocateInfo != mapMemoryCreateInfo.end()) {
        getMemoryPropertySuccess = true;
        memoryAllocateInfo = findAllocateInfo->second;
        if (pMemoryWholeSize) {
            *pMemoryWholeSize = memoryAllocateInfo.allocationSize;
        }
        if (pMemoryPropertyFlags) {
            *pMemoryPropertyFlags =
                PageGuardMappedMemory::getMemoryPropertyFlags(mapDevice, getMappedDevice(), memoryAllocateInfo.memoryTypeIndex);
        }
    } else {
        assert(false);
    }
    return getMemoryPropertySuccess;
}

bool PageGuardMappedMemory::vkMapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset,
                                                       VkDeviceSize size, VkFlags flags, void **ppData, void *pExternalHostMemory) {
    bool handleSuccessfully = true;
    MappedDevice = device;
    MappedMemory = memory;
    MappedOffset = offset;

    // Whatever the shadow memory for pageguard/write-watch enabled or not,
    // pRealMappedData always point to the mapped memory which is returned
    // by vkMapMemory, even if we are using external host memory extension
    // for current memory object.
    pRealMappedData = (PBYTE)*ppData;

    if (false == UseMappedExternalHostMemoryExtension()) {
        pMappedData = (PBYTE)pageguardAllocateMemory(size);
#if !defined(WIN32)
        // the memcpy is only for other plaforms, for win32, here we only create shadow memory,
        // but we do not sync the content with real mapped memory for the shadow memory,
        // we leave this work to page guard handler, and only sync those pages which are truly
        // accessed.
        // for non-win32 platforms, so far we haven't found similiar page guard handler, so need
        // to keep this memcpy.
        vktrace_pageguard_memcpy(pMappedData, pRealMappedData, size);
#else
        if (!getEnablePageGuardLazyCopyFlag()) {
            vktrace_pageguard_memcpy(pMappedData, pRealMappedData, size);
        }
#endif
        *ppData = pMappedData;
    } else {
        pMappedData = reinterpret_cast<PBYTE>(*ppData);
        StartingAddressOffset = (pMappedData-pExternalHostMemory)% PageGuardSize;
    }

    MappedSize = size;
    bool setPageGuard = !UseMappedExternalHostMemoryExtension();
    if (setPageGuard) {
        setPageGuardExceptionHandler();
    }
    PageSizeLeft = (size + StartingAddressOffset) % PageGuardSize;
    PageGuardAmount = (size + StartingAddressOffset) / PageGuardSize;
    if (PageSizeLeft != 0) {
        PageGuardAmount++;
    }
    pPageStatus = new PageStatusArray(PageGuardAmount);
    assert(pPageStatus);

    if (!setAllPageGuardAndFlag(setPageGuard, false)) {
        handleSuccessfully = false;
    }

    return handleSuccessfully;
}

void PageGuardMappedMemory::vkUnmapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, void **MappedData) {
    if ((memory == MappedMemory) && (device == MappedDevice)) {
        setAllPageGuardAndFlag(false, false);
        if (!UseMappedExternalHostMemoryExtension()) {
            removePageGuardExceptionHandler();
        }
        clearChangedDataPackage();
        if (!UseMappedExternalHostMemoryExtension()) {
            if (MappedData == nullptr) {
                pageguardFreeMemory(pMappedData);
            } else {
                *MappedData = pMappedData;
            }
            pRealMappedData = nullptr;
            pMappedData = nullptr;
        } else {
            pRealMappedData = nullptr;
            pMappedData = nullptr;
            if (MappedData != nullptr) {
                *MappedData = nullptr;
            }
        }
        delete pPageStatus;
        pPageStatus = nullptr;
        MappedMemory = (VkDeviceMemory) nullptr;
        MappedSize = 0;
    }
}

void PageGuardMappedMemory::SyncRealMappedMemoryToMemoryCopyHandle(VkDevice device, VkDeviceMemory memory) {
    if ((memory == MappedMemory) && (device == MappedDevice) && isUseCopyForRealMappedMemory()) {
        bool isBlockChanged = !isNoMappedBlockChanged();
        setAllPageGuardAndFlag(false, isBlockChanged);
        vktrace_pageguard_memcpy(pMappedData, pRealMappedData, MappedSize);
        setAllPageGuardAndFlag(true, isBlockChanged);
    }
}

void PageGuardMappedMemory::backupBlockChangedArraySnapshot() {
    if (UseMappedExternalHostMemoryExtension()) {
#if defined(WIN32)
        // If using the external host memory extension, we only use write
        // watch to capture the changed page, so here we get the information
        // about which page is changed from write watch, not from pageguard.
        uint64_t pageSize = PageGuardSize;
        PVOID **pAddressArray = reinterpret_cast<PVOID **>(pageguardAllocateMemory(PageGuardAmount * sizeof(PVOID *)));
        ULONG_PTR Count = PageGuardAmount;
        ULONG Granularity;
        UINT rval;
        uint64_t pageIndexCalculated = 0;
        rval = GetWriteWatch(WRITE_WATCH_FLAG_RESET, pMappedData, static_cast<size_t>(MappedSize),
                             reinterpret_cast<PVOID *>(pAddressArray), &Count, &Granularity);
        assert(rval == 0);
        pPageStatus->clearActiveChangesArray();
        for (ULONG i = 0; i < Count; i++) {
            pageIndexCalculated = (reinterpret_cast<PBYTE>(pAddressArray[i]) - pMappedData + StartingAddressOffset) / PageGuardSize;
            assert(pageIndexCalculated < PageGuardAmount);
            setMappedBlockChanged(pageIndexCalculated, true, BLOCK_FLAG_ARRAY_CHANGED);
        }
        pageguardFreeMemory(reinterpret_cast<PVOID *>(pAddressArray));
#endif
    }
    pPageStatus->backupChangedArray();
}

void PageGuardMappedMemory::backupBlockReadArraySnapshot() { pPageStatus->backupReadArray(); }

size_t PageGuardMappedMemory::getChangedBlockAmount(int useWhich) {
    size_t dwAmount = 0;
    for (size_t i = 0; i < PageGuardAmount; i++) {
        if (isMappedBlockChanged(i, useWhich)) {
            dwAmount++;
        }
    }
    return dwAmount;
}

// is RangeLimit cover or partly cover Range
bool PageGuardMappedMemory::isRangeIncluded(VkDeviceSize RangeOffsetLimit, VkDeviceSize RangeSizeLimit, VkDeviceSize RangeOffset,
                                            VkDeviceSize RangeSize) {
    bool rangeIncluded = false;
    if ((RangeOffsetLimit <= RangeOffset) && ((RangeOffsetLimit + RangeSizeLimit) > RangeOffset)) {
        rangeIncluded = true;
    } else {
        if ((RangeOffsetLimit < (RangeOffset + RangeSize)) && ((RangeOffsetLimit + RangeSizeLimit) >= (RangeOffset + RangeSize))) {
            rangeIncluded = true;
        }
    }
    return rangeIncluded;
}

// for output,
// if pData!=nullptr,the pData + Offset is head addr of an array of PageGuardChangedBlockInfo, the [0] is block amount, size (size
// for all changed blocks which amount is block amount),then block1 offset,block1 size....,
//               the block? offset is  this changed block offset to mapped memory head addr,the array followed by changed blocks
//               data
//
// if pData==nullptr, only get size
// uint64_t *pdwSaveSize, the size of all changed blocks
// uint64_t *pInfoSize, the size of array of PageGuardChangedBlockInfo
// VkDeviceSize RangeOffset, RangeSize, only consider the block which is in the range which start from RangeOffset and size is
// RangeSize, if RangeOffset<0, consider whole mapped memory
// return the amount of changed blocks.
uint64_t PageGuardMappedMemory::getChangedBlockInfo(VkDeviceSize RangeOffset, VkDeviceSize RangeSize, uint64_t *pdwSaveSize,
                                                    uint64_t *pInfoSize, PBYTE pData, uint64_t DataOffset, int useWhich) {
    uint64_t dwAmount = getChangedBlockAmount(useWhich), dwIndex = 0, offset = 0;
    uint64_t infosize = sizeof(PageGuardChangedBlockInfo) * (dwAmount + 1), SaveSize = 0, CurrentBlockSize = 0;
    PBYTE pChangedData;
    PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)(pData ? (pData + DataOffset) : nullptr);
    void *srcAddr;

    if (pInfoSize) {
        *pInfoSize = infosize;
    }
    for (uint64_t i = 0; i < PageGuardAmount; i++) {
        CurrentBlockSize = getMappedBlockSize(i);
        offset = getMappedBlockOffset(i);
        if (isMappedBlockChanged(i, useWhich)) {
            if (pChangedInfoArray) {
                pChangedInfoArray[dwIndex + 1].offset = (uint32_t)offset;
                pChangedInfoArray[dwIndex + 1].length = (uint32_t)CurrentBlockSize;
                pChangedInfoArray[dwIndex + 1].reserve0 = 0;
                pChangedInfoArray[dwIndex + 1].reserve1 = 0;
                pChangedData = pData + DataOffset + infosize + SaveSize;

                srcAddr = (void *)((uint64_t)(pMappedData + offset));
#if defined(WIN32)
                if (!UseMappedExternalHostMemoryExtension()) {
                    // We are about to copy from mapped memory to a temporary buffer.
                    // If another thread were to change this mapped memory after the
                    // copy but before the VirtualProtect we'll be doing later to
                    // re-arm PAGE_GUARD exceptions for this page, we would not see
                    // the change to mapped memory. So we call GetWriteWatch to reset the
                    // write count on this page, and then we'll call it again after the
                    // the VirtualProtect to see if it was written to between the copy
                    // and the VirtualProtect.

                    uint64_t pageSize = PageGuardSize;
                    uint64_t pmask = ~(pageSize - 1);
                    assert((((SIZE_T)(srcAddr)) & (~pmask)) == 0);
                    getWriteWatchForPage(WRITE_WATCH_FLAG_RESET, srcAddr);
                }
#else
                // Disable writes to the page before we copy from it.
                // If it is modified by another thread while copying, we'll get
                // another signal and mark it dirty, and we will copy it again.
                if (mprotect(srcAddr, CurrentBlockSize, PROT_READ) == -1) {
                    vktrace_LogError("Set memory protect on page failed!");
                }
#endif
                vktrace_pageguard_memcpy(pChangedData, srcAddr, CurrentBlockSize);
            }
            SaveSize += CurrentBlockSize;
            dwIndex++;
        }
    }
    if (pChangedInfoArray) {
        pChangedInfoArray[0].offset = (uint32_t)dwAmount;
        pChangedInfoArray[0].length = (uint32_t)SaveSize;
    }
    if (pdwSaveSize) {
        *pdwSaveSize = SaveSize;
    }
    return dwAmount;
}

// return: if memory already changed;
//        evenif no change to mmeory, it will still allocate memory for info array which only include one
//        PageGuardChangedBlockInfo,its  offset and length are all 0;
// VkDeviceSize       *pChangedSize, the size of all changed data block
// VkDeviceSize       *pDataPackageSize, size for ChangedDataPackage
// PBYTE              *ppChangedDataPackage, is an array of PageGuardChangedBlockInfo + all changed data, the [0] offset is blocks
// amount, length is size amount of these blocks(not include this info array),then block1 offset,block1 size....
//                     allocate needed memory in the method, ppChangedDataPackage point to returned pointer, if
//                     ppChangedDataPackage==null, only calculate size
bool PageGuardMappedMemory::vkFlushMappedMemoryRangePageGuardHandle(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset,
                                                                    VkDeviceSize size, VkDeviceSize *pChangedSize,
                                                                    VkDeviceSize *pDataPackageSize, PBYTE *ppChangedDataPackage) {
    bool handleSuccessfully = false;
    uint64_t dwSaveSize, InfoSize;

    backupBlockChangedArraySnapshot();
    getChangedBlockInfo(offset, size, &dwSaveSize, &InfoSize, nullptr, 0,
                        BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT);  // get the info size and size of changed blocks
    if ((dwSaveSize != 0)) {
        handleSuccessfully = true;
    }
    if (pChangedSize) {
        *pChangedSize = dwSaveSize;
    }
    if (pDataPackageSize) {
        *pDataPackageSize = dwSaveSize + InfoSize;
    }
    pChangedDataPackage = (PBYTE)pageguardAllocateMemory(dwSaveSize + InfoSize);
    getChangedBlockInfo(offset, size, &dwSaveSize, &InfoSize, pChangedDataPackage, 0, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT);

    // if use copy of real mapped memory, need copy back to real mapped memory
    if (!UseMappedExternalHostMemoryExtension()) {
        PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)pChangedDataPackage;
        if (pChangedInfoArray[0].length) {
            PBYTE pChangedData = (PBYTE)pChangedDataPackage + sizeof(PageGuardChangedBlockInfo) * (pChangedInfoArray[0].offset + 1);
            size_t CurrentOffset = 0;
            for (size_t i = 0; i < pChangedInfoArray[0].offset; i++) {
                vktrace_pageguard_memcpy(pRealMappedData + pChangedInfoArray[i + 1].offset, pChangedData + CurrentOffset,
                                         (size_t)pChangedInfoArray[i + 1].length);
                CurrentOffset += pChangedInfoArray[i + 1].length;
            }
        }
    }

    if (ppChangedDataPackage) {
        // regist the changed package
        *ppChangedDataPackage = pChangedDataPackage;
    }
    return handleSuccessfully;
}

void PageGuardMappedMemory::clearChangedDataPackage() {
    if (pChangedDataPackage) {
        pageguardFreeMemory(pChangedDataPackage);
        pChangedDataPackage = nullptr;
    }
}

// get ptr and size of OPTChangedDataPackage;
PBYTE PageGuardMappedMemory::getChangedDataPackage(VkDeviceSize *pSize) {
    PBYTE pResultDataPackage = nullptr;
    if (pChangedDataPackage) {
        pResultDataPackage = pChangedDataPackage;
        PageGuardChangedBlockInfo *pChangedInfoArray = reinterpret_cast<PageGuardChangedBlockInfo *>(pChangedDataPackage);
        if (pSize) {
            *pSize = sizeof(PageGuardChangedBlockInfo) * (pChangedInfoArray[0].offset + 1) + pChangedInfoArray[0].length;
        }
    }
    return pResultDataPackage;
}
