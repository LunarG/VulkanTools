/*
* Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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

//OPT: Optimization by using page-guard for speed up capture
//     The speed is extremely slow when use vktrace to capture DOOM4. It took over half a day and 900G of trace for a capture from beginning to the game menu.
//     The reason that caused such slow capture is DOOM updates a big mapped memory(over 67M) frequently, vktrace copies this memory block to harddrive when DOOM calls vkFlushmappedMemory to update it every time.
//     Here we use page guard to record which page of big memory block has been changed and only save those changed pages, it make the capture time reduce to round 15 minutes, the trace file size is round 40G, 
//     The Playback time for these trace file is round 7 minutes(on Win10/AMDFury/32GRam/I5 system).

#include "vktrace_pageguard_memorycopy.h"
#include "vktrace_lib_pagestatusarray.h"
#include "vktrace_lib_pageguardmappedmemory.h"
#include "vktrace_lib_pageguardcapture.h"
#include "vktrace_lib_pageguard.h"

VkDevice& PageGuardMappedMemory::getMappedDevice()
{
    return MappedDevice;
}

VkDeviceMemory& PageGuardMappedMemory::getMappedMemory()
{
    return MappedMemory;
}

VkDeviceSize& PageGuardMappedMemory::getMappedOffset()
{
    return MappedOffset;
}

PBYTE& PageGuardMappedMemory::getRealMappedDataPointer()
{
    return pRealMappedData;
}

VkDeviceSize& PageGuardMappedMemory::getMappedSize()
{
    return MappedSize;
}

PageGuardMappedMemory::PageGuardMappedMemory()
    :MappedDevice(nullptr),
    MappedMemory((VkDeviceMemory)nullptr),
    pMappedData(nullptr),
    pRealMappedData(nullptr),
    pChangedDataPackage(nullptr),
    MappedSize(0),
    PageGuardSize(pageguardGetSystemPageSize()),
    #if defined(PLATFORM_LINUX)
    clearRefsFd(-1),
    #endif
    pPageStatus(nullptr),
    BlockConflictError(false),
    PageSizeLeft(0),
    PageGuardAmount(0)
{
}

PageGuardMappedMemory::~PageGuardMappedMemory()
{
}

bool PageGuardMappedMemory::isUseCopyForRealMappedMemory()
{
    bool useCopyForRealMappedMemory = false;
    if (pRealMappedData)
    {
        useCopyForRealMappedMemory = true;
    }
    return useCopyForRealMappedMemory;
}

bool PageGuardMappedMemory::getChangedRangeByIndex(uint64_t index, PBYTE *pAddress, VkDeviceSize *pBlockSize)
{
    bool isValidResult = false;
    if (index < PageGuardAmount)
    {
        isValidResult = true;
        if ((index + 1) == PageGuardAmount)
        {
            if (pAddress)
            {
                *pAddress = pMappedData + index*PageGuardSize;
            }
            if (pBlockSize)
            {
                *pBlockSize = (SIZE_T)(PageSizeLeft ? PageSizeLeft : PageGuardSize);
            }
        }
        else
        {
            if (pAddress)
            {
                *pAddress = pMappedData + index*PageGuardSize;
            }
            if (pBlockSize)
            {
                *pBlockSize = (SIZE_T)PageGuardSize;
            }
        }
    }
    return isValidResult;
}

//if return value ==-1, mean addr is out of page guard.
uint64_t PageGuardMappedMemory::getIndexOfChangedBlockByAddr(PBYTE addr)
{
    int64_t addrOffset = addr - pMappedData;
    uint64_t indexOfChangedBlockByAddr = -1;
    if ((addrOffset >= 0) && ((VkDeviceSize)addrOffset < MappedSize))
    {
        indexOfChangedBlockByAddr = addrOffset / PageGuardSize;
    }
    return indexOfChangedBlockByAddr;
}

void PageGuardMappedMemory::setMappedBlockChanged(uint64_t index, bool changed, int which)
{
    if (index < PageGuardAmount)
    {
        switch (which)
        {
        case BLOCK_FLAG_ARRAY_CHANGED:
            pPageStatus->setBlockChangedArray(index, changed);
            break;

        case BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT:
            pPageStatus->setBlockChangedArraySnapshot(index, changed);
            break;

        case BLOCK_FLAG_ARRAY_READ_SNAPSHOT:
            pPageStatus->setBlockReadArraySnapshot(index, changed);
            break;

        default://BLOCK_FLAG_ARRAY_READ
            pPageStatus->setBlockReadArray(index, changed);
            break;
        }
    }
}

bool PageGuardMappedMemory::isMappedBlockChanged(uint64_t index, int which)
{
    bool mappedBlockChanged = false;
    if (index < PageGuardAmount)
    {
        switch (which)
        {
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

uint64_t PageGuardMappedMemory::getMappedBlockSize(uint64_t index)
{
    uint64_t mappedBlockSize = PageGuardSize;
    if ((index + 1) == PageGuardAmount)
    {
        if (PageSizeLeft)
        {
            mappedBlockSize = PageSizeLeft;
        }
    }
    return mappedBlockSize;
}

uint64_t PageGuardMappedMemory::getMappedBlockOffset(uint64_t index)
{
    uint64_t mappedBlockOffset = 0;
    if (index < PageGuardAmount)
    {
        mappedBlockOffset = index*PageGuardSize;
    }
    return mappedBlockOffset;
}

bool PageGuardMappedMemory::isNoMappedBlockChanged()
{
    bool noMappedBlockChanged = true;
    for (uint64_t i = 0; i < PageGuardAmount; i++)
    {
        if (isMappedBlockChanged(i, BLOCK_FLAG_ARRAY_CHANGED))
        {
            noMappedBlockChanged = false;
            break;
        }
    }
    return noMappedBlockChanged;
}

void PageGuardMappedMemory::resetMemoryObjectAllChangedFlagAndPageGuard()
{
    for (uint64_t i = 0; i < PageGuardAmount; i++)
    {
        if (isMappedBlockChanged(i, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT))
        {
            #if defined(WIN32)
            DWORD oldProt;
            VirtualProtect(pMappedData + i*PageGuardSize, (SIZE_T)getMappedBlockSize(i), PAGE_READWRITE | PAGE_GUARD, &oldProt);
            #endif
            setMappedBlockChanged(i, false, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT);
        }
    }
}

void PageGuardMappedMemory::resetMemoryObjectAllReadFlagAndPageGuard()
{
    backupBlockReadArraySnapshot();
    for (uint64_t i = 0; i < PageGuardAmount; i++)
    {
        if (isMappedBlockChanged(i, BLOCK_FLAG_ARRAY_READ_SNAPSHOT))
        {
            #if defined(WIN32)
            DWORD oldProt;
            VirtualProtect(pMappedData + i*PageGuardSize, (SIZE_T)getMappedBlockSize(i), PAGE_READWRITE | PAGE_GUARD, &oldProt);
            #endif
            setMappedBlockChanged(i, false, BLOCK_FLAG_ARRAY_READ_SNAPSHOT);
        }
    }
}

bool PageGuardMappedMemory::setAllPageGuardAndFlag(bool bSetPageGuard, bool bSetBlockChanged)
{
    bool setSuccessfully = true;
    #if defined(WIN32)
    DWORD dwMemSetting = bSetPageGuard ? (PAGE_READWRITE | PAGE_GUARD) : PAGE_READWRITE;
    #endif

    for (uint64_t i = 0; i < PageGuardAmount; i++)
    {
        #if defined(WIN32)
        DWORD oldProt, dwErr;
        if (!VirtualProtect(pMappedData + i*PageGuardSize, (SIZE_T)getMappedBlockSize(i), dwMemSetting, &oldProt))
        {
            dwErr = GetLastError();
            setSuccessfully = false;
        }
        #endif
        setMappedBlockChanged(i, bSetBlockChanged, BLOCK_FLAG_ARRAY_CHANGED);
    }
    return setSuccessfully;
}

bool PageGuardMappedMemory::vkMapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkFlags flags, void** ppData)
{
    bool handleSuccessfully = true;
    MappedDevice = device;
    MappedMemory = memory;
    MappedOffset = offset;
#ifndef PAGEGUARD_ADD_PAGEGUARD_ON_REAL_MAPPED_MEMORY
    pRealMappedData = (PBYTE)*ppData;
    pMappedData = (PBYTE)pageguardAllocateMemory(size);
    vktrace_pageguard_memcpy(pMappedData, pRealMappedData, size);
    *ppData = pMappedData;
#else
    pMappedData = (PBYTE)*ppData;
#endif
    MappedSize = size;

#ifdef WIN32
    setPageGuardExceptionHandler();
#endif

    PageSizeLeft = size % PageGuardSize;
    PageGuardAmount = size / PageGuardSize;
    if (PageSizeLeft != 0)
    {
        PageGuardAmount++;
    }
    pPageStatus = new PageStatusArray(PageGuardAmount);
    assert(pPageStatus);
    if (!setAllPageGuardAndFlag(true, false))
    {
        handleSuccessfully = false;
    }
    return handleSuccessfully;
}

void PageGuardMappedMemory::vkUnmapMemoryPageGuardHandle(VkDevice device, VkDeviceMemory memory, void** MappedData)
{
    if ((memory == MappedMemory) && (device == MappedDevice))
    {
        setAllPageGuardAndFlag(false, false);
#ifdef WIN32
        removePageGuardExceptionHandler();
#endif
        clearChangedDataPackage();
#ifndef PAGEGUARD_ADD_PAGEGUARD_ON_REAL_MAPPED_MEMORY
        if (MappedData == nullptr)
        {
            vktrace_pageguard_memcpy(pRealMappedData, pMappedData, MappedSize);
            pageguardFreeMemory(pMappedData);
        }
        else
        {
            vktrace_pageguard_memcpy(pRealMappedData, pMappedData, MappedSize);
            *MappedData = pMappedData;
        }
        pRealMappedData = nullptr;
        pMappedData = nullptr;
#else
        pMappedData = nullptr;
        if (MappedData != nullptr)
        {
            *MappedData = nullptr;
        }
#endif
        delete pPageStatus;
        pPageStatus = nullptr;
        MappedMemory =(VkDeviceMemory)nullptr;
        MappedSize = 0;
    }
}

void PageGuardMappedMemory::backupBlockChangedArraySnapshot()
{
    pPageStatus->backupChangedArray();
}

void PageGuardMappedMemory::backupBlockReadArraySnapshot()
{
    pPageStatus->backupReadArray();
}

DWORD PageGuardMappedMemory::getChangedBlockAmount(int useWhich)
{
    DWORD dwAmount = 0;
    for (uint64_t i = 0; i < PageGuardAmount; i++)
    {
        if (isMappedBlockChanged(i, useWhich))
        {
            dwAmount++;
        }
    }
    return dwAmount;
}

//is RangeLimit cover or partly cover Range
bool PageGuardMappedMemory::isRangeIncluded(VkDeviceSize RangeOffsetLimit, VkDeviceSize RangeSizeLimit, VkDeviceSize RangeOffset, VkDeviceSize RangeSize)
{
    bool rangeIncluded = false;
    if ((RangeOffsetLimit <= RangeOffset) &&
        ((RangeOffsetLimit + RangeSizeLimit)>RangeOffset))
    {
        rangeIncluded = true;
    }
    else
    {
        if ((RangeOffsetLimit<(RangeOffset + RangeSize)) &&
            ((RangeOffsetLimit + RangeSizeLimit) >= (RangeOffset + RangeSize)))
        {
            rangeIncluded = true;
        }

    }
    return rangeIncluded;
}

//for output,
//if pData!=nullptr,the pData + Offset is head addr of an array of PageGuardChangedBlockInfo, the [0] is block amount, size (size for all changed blocks which amount is block amount),then block1 offset,block1 size...., 
//               the block? offset is  this changed block offset to mapped memory head addr,the array followed by changed blocks data
//
//if pData==nullptr, only get size
//DWORD *pdwSaveSize, the size of all changed blocks
//DWORD *pInfoSize, the size of array of PageGuardChangedBlockInfo
//VkDeviceSize RangeOffset, RangeSize, only consider the block which is in the range which start from RangeOffset and size is RangeSize, if RangeOffset<0, consider whole mapped memory
//return the amount of changed blocks.
DWORD PageGuardMappedMemory::getChangedBlockInfo(VkDeviceSize RangeOffset, VkDeviceSize RangeSize, DWORD *pdwSaveSize, DWORD *pInfoSize, PBYTE pData, DWORD DataOffset, int useWhich)
{
    DWORD dwAmount = getChangedBlockAmount(useWhich), dwIndex = 0, offset = 0;
    DWORD infosize = sizeof(PageGuardChangedBlockInfo)*(dwAmount + 1), SaveSize = 0, CurrentBlockSize = 0;
    PBYTE pChangedData;
    PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)(pData ? (pData + DataOffset) : nullptr);

    if (pInfoSize)
    {
        *pInfoSize = infosize;
    }
    for (uint64_t i = 0; i < PageGuardAmount; i++)
    {
        CurrentBlockSize = getMappedBlockSize(i);
        offset = getMappedBlockOffset(i);
        if (isMappedBlockChanged(i, useWhich))
        {
            if (pChangedInfoArray)
            {
                pChangedInfoArray[dwIndex + 1].offset = offset;
                pChangedInfoArray[dwIndex + 1].length = CurrentBlockSize;
                pChangedInfoArray[dwIndex + 1].reserve0 = 0;
                pChangedInfoArray[dwIndex + 1].reserve1 = 0;
                pChangedData = pData + DataOffset + infosize + SaveSize;
                vktrace_pageguard_memcpy(pChangedData, pMappedData + offset, CurrentBlockSize);
            }
            SaveSize += CurrentBlockSize;
            dwIndex++;
        }
    }
    if (pChangedInfoArray)
    {
        pChangedInfoArray[0].offset = dwAmount;
        pChangedInfoArray[0].length = SaveSize;
    }
    if (pdwSaveSize)
    {
        *pdwSaveSize = SaveSize;
    }
    return dwAmount;
}

//return: if memory already changed;
//        evenif no change to mmeory, it will still allocate memory for info array which only include one PageGuardChangedBlockInfo,its  offset and length are all 0;
// VkDeviceSize       *pChangedSize, the size of all changed data block
// VkDeviceSize       *pDataPackageSize, size for ChangedDataPackage
// PBYTE              *ppChangedDataPackage, is an array of PageGuardChangedBlockInfo + all changed data, the [0] offset is blocks amount, length is size amount of these blocks(not include this info array),then block1 offset,block1 size....
//                     allocate needed memory in the method, ppChangedDataPackage point to returned pointer, if ppChangedDataPackage==null, only calculate size
bool PageGuardMappedMemory::vkFlushMappedMemoryRangePageGuardHandle(
    VkDevice           device,
    VkDeviceMemory     memory,
    VkDeviceSize       offset,
    VkDeviceSize       size,
    VkDeviceSize       *pChangedSize,
    VkDeviceSize       *pDataPackageSize,
    PBYTE              *ppChangedDataPackage)
{
    bool handleSuccessfully = false;
    DWORD dwSaveSize, InfoSize;

    backupBlockChangedArraySnapshot();
    getChangedBlockInfo(offset, size, &dwSaveSize, &InfoSize, nullptr, 0, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT); //get the info size and size of changed blocks
    if ((dwSaveSize != 0))
    {
        handleSuccessfully = true;
    }
    if (pChangedSize)
    {
        *pChangedSize = dwSaveSize;
    }
    if (pDataPackageSize)
    {
        *pDataPackageSize = dwSaveSize + InfoSize;
    }
    pChangedDataPackage = (PBYTE)pageguardAllocateMemory(dwSaveSize + InfoSize);
    getChangedBlockInfo(offset, size, &dwSaveSize, &InfoSize, pChangedDataPackage, 0, BLOCK_FLAG_ARRAY_CHANGED_SNAPSHOT);

    //if use copy of real mapped memory, need copy back to real mapped memory
#ifndef PAGEGUARD_ADD_PAGEGUARD_ON_REAL_MAPPED_MEMORY
    PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)pChangedDataPackage;
    if (pChangedInfoArray[0].length)
    {
        PBYTE pChangedData = (PBYTE)pChangedDataPackage + sizeof(PageGuardChangedBlockInfo)*(pChangedInfoArray[0].offset + 1);
        DWORD CurrentOffset = 0;
        for (DWORD i = 0; i < pChangedInfoArray[0].offset; i++)
        {
            vktrace_pageguard_memcpy(pRealMappedData + pChangedInfoArray[i + 1].offset, pChangedData + CurrentOffset, (size_t)pChangedInfoArray[i + 1].length);
            CurrentOffset += pChangedInfoArray[i + 1].length;
        }
    }
#endif

    if (ppChangedDataPackage)
    {
        //regist the changed package
        *ppChangedDataPackage = pChangedDataPackage;
    }
    return handleSuccessfully;
}

void PageGuardMappedMemory::clearChangedDataPackage()
{
    if (pChangedDataPackage)
    {
        pageguardFreeMemory(pChangedDataPackage);
        pChangedDataPackage = nullptr;
    }
}

//get ptr and size of OPTChangedDataPackage;
PBYTE PageGuardMappedMemory::getChangedDataPackage(VkDeviceSize  *pSize)
{
    PBYTE pResultDataPackage = nullptr;
    if (pChangedDataPackage)
    {
        pResultDataPackage = pChangedDataPackage;
        PageGuardChangedBlockInfo *pChangedInfoArray = reinterpret_cast<PageGuardChangedBlockInfo *>(pChangedDataPackage);
        if (pSize)
        {
            *pSize = sizeof(PageGuardChangedBlockInfo)*(pChangedInfoArray[0].offset + 1) + pChangedInfoArray[0].length;
        }
    }
    return pResultDataPackage;
}
