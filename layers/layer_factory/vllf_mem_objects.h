#pragma once

#include "vector"
#include "string"
#include <algorithm>

static const uint32_t kMemoryObjectWarningLimit = 350;

class TooManyMemObjects : public layer_factory {
public:
    // Constructor for interceptor
    TooManyMemObjects() : layer_factory(this) {
        _number_mem_objects = 0;
    };

    // Intercept the memory allocation calls and increment the counter
    VkResult PreCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                   const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory) {
        _number_mem_objects++;

        if (_number_mem_objects > kMemoryObjectWarningLimit) {
            std::string message =
                "Performance Warning:  This app has > " + std::to_string(kMemoryObjectWarningLimit) + " memory objects.";
            PerformanceWarning(message);
        }
        return VK_SUCCESS;
    };

    // Intercept the free memory calls and decrement the allocation count
    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
        if (memory != VK_NULL_HANDLE) {
            _number_mem_objects--;
        }
    }

private:
    // Counter for the number of currently active memory allocations
    uint32_t _number_mem_objects;
};

TooManyMemObjects mem_object_count;
