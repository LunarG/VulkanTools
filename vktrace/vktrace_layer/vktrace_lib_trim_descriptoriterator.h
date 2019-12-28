/*
 * Copyright (c) 2016-2019 Advanced Micro Devices, Inc. All rights reserved.
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
#ifndef __VKTRACE_LIB_TRIM_DESCRIPTOR_ITERATOR_H__
#define __VKTRACE_LIB_TRIM_DESCRIPTOR_ITERATOR_H__

#include "vktrace_trace_packet_utils.h"

namespace trim {
// The following class descriptorIterator is a customized iterator which will
// be used to simplify trim descriptorset update tracking.
//
// By Doc, when target title update descriptor set, it use VkWriteDescriptorSet
// to specify which descriptors will be updated. For trim tracking process, the
// complicated case is:
//
// Target title try to update descriptors cross bindings. For example, the input
// VkWriteDescriptorSet specify a descriptor array with length is Length-A, the
// target binding is binding-B, the target start array index in the binding is
// Descriptor-Start-Index-C, suppose Length-A is longer than binding-B all left
// descriptors from Descriptor-Start-Index-C.
//
// By Doc, for the above case, the update will cross the binding of
// binding-B to descriptors of next binding, or cause more other bindings
// involved. Such case make trim tracking process complicated because we
// cannot use a simple "for" loop, some extra process has to be added in the
// loop and make it become complicated.
//
// the folowing descriptorIterator class is used to simplify the above trim
// tracking process.
//

// There are three types of descriptor array which are VkDescriptorImageInfo,
// VkDescriptorBufferInfo, VkBufferView. DescriptorInfo is used as an unified
// struct so we can use DescriptorInfo* point to any type of them.
typedef union descriptorInfo {
    VkDescriptorImageInfo image_info;
    VkDescriptorBufferInfo buffer_info;
    VkBufferView buffer_view;
} DescriptorInfo;

class DescriptorIterator {
   public:
    using value_type = DescriptorInfo;
    using difference_type = int;
    using pointer = DescriptorInfo *;
    using reference = DescriptorInfo &;
    using iterator_category = std::forward_iterator_tag;

    DescriptorIterator() = default;

    // ObjectInfo *pObjectInfo, the target descriptorset tracking info.
    // The two parameters (bindingIndex, descriptorIndex) specify
    // the starting location in the descriptorset that the API call
    // use to update the descriptors.
    // uint32_t descriptorArrayLength, how many descriptors the API call will update.
    //          Note: descriptorArrayLength may longer than all descriptors left from
    //          descriptorIndex in bindingIndex and cross to next binding.
    DescriptorIterator(ObjectInfo *object_info, uint32_t binding_index, uint32_t descriptor_index,
                       uint32_t descriptor_array_length);

    DescriptorIterator &operator*() const;

    DescriptorIterator &operator++();

    DescriptorIterator operator++(int);

    bool operator==(const DescriptorIterator &iterator);

    bool operator!=(const DescriptorIterator &iterator);

    bool IsEnd();

    VkDescriptorType GetCurrentDescriptorType();

    uint32_t GetCurrentBindingIndex();

   private:
    // The target descriptorset trim tracking info.
    ObjectInfo::_ObjectInfo::_DescriptorSet *descriptorset_;

    // The trim tracking info of the DescriptorSetLayout which the target
    // descriptorset be created from.
    ObjectInfo::_ObjectInfo::_DescriptorSetLayout *descriptorset_layout_;

    uint32_t update_descriptor_count_;
    uint32_t current_iterator_index_;

    uint32_t current_descriptor_binding_index_;
    uint32_t current_descriptor_index_;

    // Suppose the two descriptorIterator come from same descriptorset.
    bool IsEqual(const DescriptorIterator &iterator1, const DescriptorIterator &iterator2);

    void MoveToNextDescriptorInfo();
};
}  // namespace trim

#endif  // __VKTRACE_LIB_TRIM_DESCRIPTOR_ITERATOR_H__
