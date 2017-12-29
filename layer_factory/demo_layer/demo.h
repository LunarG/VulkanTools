#pragma once

#include <unordered_map>
#include "vulkan/vulkan.h"
#include "vk_layer_logging.h"
#include "layer_factory.h"

class MemDemo : public layer_factory {
public:
    // Constructor for state_tracker
    MemDemo() : layer_factory(this) {
        _number_mem_objects = 0;
        _total_memory = 0;
        _present_count = 0;
    };

    void PreCallApiFunction(const char* api_name);

    VkResult PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory);

    void PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator);

    VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

private:
    uint32_t _number_mem_objects;
    VkDeviceSize _total_memory;
    uint32_t _present_count;
    std::unordered_map<VkDeviceMemory, VkDeviceSize> mem_size_map;
};

