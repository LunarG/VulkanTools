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
#include <atomic>
#include "vktrace_lib_pagestatusarray.h"

const uint64_t PageStatusArray::PAGE_FLAG_AMOUNT_PER_BYTE = 1;
const uint64_t PageStatusArray::PAGE_NUMBER_FROM_BIT_SHIFT = 0;

PageStatusArray::PageStatusArray(uint64_t pageCount) {
    ByteCount = (pageCount % PAGE_FLAG_AMOUNT_PER_BYTE) ? (pageCount / PAGE_FLAG_AMOUNT_PER_BYTE) + 1
                                                        : pageCount / PAGE_FLAG_AMOUNT_PER_BYTE;

    pChangedArray[0] = new uint8_t[(size_t)ByteCount];
    assert(pChangedArray[0]);

    pChangedArray[1] = new uint8_t[(size_t)ByteCount];
    assert(pChangedArray[1]);

    pReadArray[0] = new uint8_t[(size_t)ByteCount];
    assert(pReadArray[0]);

    pReadArray[1] = new uint8_t[(size_t)ByteCount];
    assert(pReadArray[1]);

    activeChangesArray = pChangedArray[0];
    capturedChangesArray = pChangedArray[1];
    activeReadArray = pReadArray[0];
    capturedReadArray = pReadArray[1];

    firstTimeLoadArray = new uint8_t[(size_t)ByteCount];
    assert(firstTimeLoadArray);

    clearAll();
}

PageStatusArray::~PageStatusArray() {
    delete[] firstTimeLoadArray;
    delete[] pChangedArray[0];
    delete[] pChangedArray[1];
    delete[] pReadArray[0];
    delete[] pReadArray[1];
}

void PageStatusArray::toggleChangedArray() {
    // TODO use atomic exchange
    uint8_t *tempArray = activeChangesArray;
    activeChangesArray = capturedChangesArray;
    capturedChangesArray = tempArray;
}

void PageStatusArray::toggleReadArray() {
    // TODO use atomic exchange
    uint8_t *tempArray = activeReadArray;
    activeReadArray = capturedReadArray;
    capturedReadArray = tempArray;
}

bool PageStatusArray::getBlockChangedArray(uint64_t index) {
    return (activeChangesArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] & (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE))) != 0;
}

bool PageStatusArray::getBlockChangedArraySnapshot(uint64_t index) {
    return (capturedChangesArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] & (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE))) != 0;
}

bool PageStatusArray::getBlockReadArray(uint64_t index) {
    return (activeReadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] & (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE))) != 0;
}

bool PageStatusArray::getBlockReadArraySnapshot(uint64_t index) {
    return (capturedReadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] & (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE))) != 0;
}

bool PageStatusArray::getBlockFirstTimeLoadArray(uint64_t index) {
    return (firstTimeLoadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] & (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE))) != 0;
}

void PageStatusArray::setBlockChangedArray(uint64_t index, bool changed) {
    if (changed) {
        activeChangesArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] |= (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE));
    } else {
        activeChangesArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] &= ~((uint8_t)(1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE)));
    }
}

void PageStatusArray::setBlockChangedArraySnapshot(uint64_t index, bool changed) {
    if (changed) {
        capturedChangesArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] |= (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE));
    } else {
        capturedChangesArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] &= ~((uint8_t)(1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE)));
    }
}

void PageStatusArray::setBlockReadArray(uint64_t index, bool changed) {
    if (changed) {
        activeReadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] |= (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE));
    } else {
        activeReadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] &= ~((uint8_t)(1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE)));
    }
}

void PageStatusArray::setBlockReadArraySnapshot(uint64_t index, bool changed) {
    if (changed) {
        capturedReadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] |= (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE));
    } else {
        capturedReadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] &= ~((uint8_t)(1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE)));
    }
}

void PageStatusArray::setBlockFirstTimeLoadArray(uint64_t index, bool loaded) {
    if (loaded) {
        firstTimeLoadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] |= (1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE));
    } else {
        firstTimeLoadArray[index >> PAGE_NUMBER_FROM_BIT_SHIFT] &= ~((uint8_t)(1 << (index % PAGE_FLAG_AMOUNT_PER_BYTE)));
    }
}

void PageStatusArray::backupChangedArray() { toggleChangedArray(); }

void PageStatusArray::backupReadArray() { toggleReadArray(); }

void PageStatusArray::clearAll() {
    memset(activeChangesArray, 0, (size_t)ByteCount);
    memset(capturedChangesArray, 0, (size_t)ByteCount);
    memset(activeReadArray, 0, (size_t)ByteCount);
    memset(capturedReadArray, 0, (size_t)ByteCount);
    memset(firstTimeLoadArray, 0, (size_t)ByteCount);
}
void PageStatusArray::clearActiveChangesArray() { memset(activeChangesArray, 0, (size_t)ByteCount); }
