#pragma once

#include <sstream>
#include <unordered_map>

static uint32_t display_rate = 60;

class MemAllocLevel : public layer_factory {
public:
    // Constructor for interceptor
    MemAllocLevel() : layer_factory(this) {
        _number_mem_objects = 0;
        _total_memory = 0;
        _present_count = 0;
    };

    // Intercept the memory allocation calls and increment the counter
    VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory) {
        _number_mem_objects++;
        _total_memory += pAllocateInfo->allocationSize;
        _mem_size_map[*pMemory] = pAllocateInfo->allocationSize;
        return VK_SUCCESS;
    };

    // Intercept the free memory calls and update totals
    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
        if (memory != VK_NULL_HANDLE) {
            _number_mem_objects--;
            VkDeviceSize this_alloc = _mem_size_map[memory];
            _total_memory -= this_alloc;
        }
    }

    VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
        _present_count++;
        if (_present_count >= display_rate) {
            _present_count = 0;
            std::stringstream message;
            message << "Memory Allocation Count: " << _number_mem_objects << "\n";
            message << "Total Memory Allocation Size: " << _total_memory << "\n\n";
            Information(message.str());
        }
        return VK_SUCCESS;
    }

private:
    // Counter for the number of currently active memory allocations
    uint32_t _number_mem_objects;
    VkDeviceSize _total_memory;
    uint32_t _present_count;
    std::unordered_map<VkDeviceMemory, VkDeviceSize> _mem_size_map;
};

MemAllocLevel high_water_mark;
