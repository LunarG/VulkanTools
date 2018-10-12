/*
 * Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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
 */
#include "vktrace_lib_trim_statetracker.h"
#include "vktrace_lib_trim.h"
#include "vktrace_lib_trim_descriptoriterator.h"

namespace trim {
// ObjectInfo *pObjectInfo, the target descriptorset tracking info.
// The two parameters (bindingIndex, descriptorIndex) specify
// the starting location in the descriptorset that the API call
// use to update the descriptors.
// uint32_t descriptorArrayLength, how many descriptors the API call will update.
//          Note: descriptorArrayLength may longer than all descriptors left from
//          descriptorIndex in bindingIndex and cross to next binding.
DescriptorIterator::DescriptorIterator(ObjectInfo *pObjectInfo, uint32_t bindingIndex, uint32_t descriptorIndex,
                                       uint32_t descriptorArrayLength){
    // TODO: implementation
};

DescriptorIterator &DescriptorIterator::operator*() const {
    // TODO: implementation
    DescriptorIterator *pDescriptorInfo = nullptr;
    return *(pDescriptorInfo);
};

DescriptorIterator &DescriptorIterator::operator++() {
    // TODO: implementation
    return *this;
};

DescriptorIterator DescriptorIterator::operator++(int) {
    // TODO: implementation
    return *this;
};

bool DescriptorIterator::operator==(const DescriptorIterator &iterator) {
    // TODO: implementation
    return false;
};

bool DescriptorIterator::operator!=(const DescriptorIterator &iterator) {
    // TODO: implementation
    return false;
};

bool DescriptorIterator::isEnd() {
    // TODO: implementation
    return false;
}

// Suppose the two descriptorIterator come from same descriptorset.
bool DescriptorIterator::isEqual(const DescriptorIterator &iterator1, const DescriptorIterator &iterator2) {
    // TODO: implementation
    return false;
}

void DescriptorIterator::moveToNextDescriptorInfo() {
    // TODO: implementation
}

}  // namespace trim
