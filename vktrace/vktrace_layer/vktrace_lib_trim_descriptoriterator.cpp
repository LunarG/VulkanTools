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
DescriptorIterator::DescriptorIterator(ObjectInfo *object_info, uint32_t binding_index, uint32_t descriptor_index,
                                       uint32_t descriptorArrayLength)
    : update_descriptor_count_(descriptorArrayLength),
      descriptorset_((object_info != nullptr) ? &object_info->ObjectInfo.DescriptorSet : nullptr),
      descriptorset_layout_(
          ((object_info != nullptr) && (trim::get_DescriptorSetLayout_objectInfo(descriptorset_->layout) != nullptr))
              ? &trim::get_DescriptorSetLayout_objectInfo(descriptorset_->layout)->ObjectInfo.DescriptorSetLayout
              : nullptr),
      current_descriptor_binding_index_(binding_index),
      current_descriptor_index_(descriptor_index),
      current_iterator_index_(0) {
    if (object_info == nullptr) {
        update_descriptor_count_ = 0;  // make sure point to end of iterator
        // output error message in log
        vktrace_LogError("Failed to create descriptor iterator due to invalid ObjectInfo pointer");
    }
};

DescriptorIterator &DescriptorIterator::operator*() const {
    DescriptorIterator *descriptor_info = nullptr;
    // Vulkan has no API function for init descriptorset, the initialization of
    // descriptorset is also through update call. Here we use its
    // layout trackinfo to get descriptorType of specific binding
    // to avoid invalid descriptorType for first time update.
    switch (descriptorset_layout_->pBindings[current_descriptor_binding_index_].descriptorType) {
        case VK_DESCRIPTOR_TYPE_SAMPLER:
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
        case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
        case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
            descriptor_info = reinterpret_cast<DescriptorIterator *>(const_cast<VkDescriptorImageInfo *>(
                descriptorset_->pWriteDescriptorSets[current_descriptor_binding_index_].pImageInfo + current_descriptor_index_));
        } break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
            descriptor_info = reinterpret_cast<DescriptorIterator *>(const_cast<VkDescriptorBufferInfo *>(
                descriptorset_->pWriteDescriptorSets[current_descriptor_binding_index_].pBufferInfo + current_descriptor_index_));
        } break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
            descriptor_info = reinterpret_cast<DescriptorIterator *>(const_cast<VkBufferView *>(
                descriptorset_->pWriteDescriptorSets[current_descriptor_binding_index_].pTexelBufferView +
                current_descriptor_index_));
        } break;
        default:
            assert(false);  // something wrong or it's a new type
                            // that we cannot handle.
            break;
    }
    return *descriptor_info;
};

DescriptorIterator &DescriptorIterator::operator++() {
    MoveToNextDescriptorInfo();
    return *this;
};

DescriptorIterator DescriptorIterator::operator++(int) {
    DescriptorIterator temp_descriptor_iterator = *this;
    MoveToNextDescriptorInfo();
    return temp_descriptor_iterator;
};

bool DescriptorIterator::operator==(const DescriptorIterator &iterator) { return IsEqual(*this, iterator); };

bool DescriptorIterator::operator!=(const DescriptorIterator &iterator) { return !IsEqual(*this, iterator); };

bool DescriptorIterator::IsEnd() {
    bool end_flag = true;
    if (current_iterator_index_ < update_descriptor_count_) {
        // the current iterator index within the range, we continue to check if it's a valid descriptor location.
        if (current_descriptor_binding_index_ < descriptorset_layout_->bindingCount) {
            if ((current_descriptor_binding_index_ + 1) == descriptorset_layout_->bindingCount) {
                // it's the end binding, we need to check if current_descriptor_index_ is valid.
                if (current_descriptor_index_ <
                    descriptorset_layout_->pBindings[current_descriptor_binding_index_].descriptorCount) {
                    end_flag = false;
                }
            } else {
                // it's not the end binding
                end_flag = false;
            }
        }
    }
    return end_flag;
}

// Suppose the two descriptorIterator come from same descriptorset.
bool DescriptorIterator::IsEqual(const DescriptorIterator &iterator1, const DescriptorIterator &iterator2) {
    return (const_cast<DescriptorIterator &>(iterator1).IsEnd() && const_cast<DescriptorIterator &>(iterator2).IsEnd()) ||
           (!const_cast<DescriptorIterator &>(iterator1).IsEnd() && !const_cast<DescriptorIterator &>(iterator2).IsEnd() &&
            (iterator1.current_descriptor_binding_index_ == iterator2.current_descriptor_binding_index_) &&
            (iterator1.current_descriptor_index_ == iterator2.current_descriptor_index_));
}

void DescriptorIterator::MoveToNextDescriptorInfo() {
    if (!IsEnd()) {
        current_iterator_index_++;
        if ((current_descriptor_index_ + 1) < descriptorset_layout_->pBindings[current_descriptor_binding_index_].descriptorCount) {
            current_descriptor_index_++;
        } else {
            current_descriptor_index_ = 0;
            current_descriptor_binding_index_++;
            // Doc allow a binding has zero descriptors, we have to bypass all zero descriptors bindings
            while ((descriptorset_layout_->pBindings[current_descriptor_binding_index_].descriptorCount == 0) &&
                   (current_descriptor_binding_index_ < descriptorset_layout_->bindingCount)) {
                current_descriptor_binding_index_++;
            }
        }
    }
}

VkDescriptorType DescriptorIterator::GetCurrentDescriptorType() {
    return descriptorset_layout_->pBindings[current_descriptor_binding_index_].descriptorType;
}

}  // namespace trim
