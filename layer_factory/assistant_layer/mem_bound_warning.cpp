#include "mem_bound_warning.h"
#include <iostream>

VkResult MemBoundWarning::PostCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo,
                                                 const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory,
                                                 VkResult result) {
    if (result == VK_SUCCESS) {
        // create new memory entry into the map but give it no values
        std::unordered_set<uint64_t> set;
        memory_handle_map.insert(std::make_pair(*pMemory, set));
    }

    return VK_SUCCESS;
}

// helper function that update all the dictionaries for both image and buffer handles
template <typename T>
void MemBoundWarning::SetBindMemoryState(T handle, VkDeviceMemory memory) {
    uint64_t handle_64 = HandleToUint64(handle);

    if (memory != VK_NULL_HANDLE) {
        // If the entry does not exist yet, then create it
        if (handle_memory_map.count(handle_64) <= 0) {
            std::unordered_set<VkDeviceMemory> set;
            handle_memory_map.insert(std::make_pair(handle_64, set));
        }

        // insert the memory and handle into their respective maps
        auto &memory_set = handle_memory_map.find(handle_64)->second;
        memory_set.insert(memory);

        auto &handle_set = memory_handle_map.find(memory)->second;
        handle_set.insert(handle_64);
    }
}

// 3 buffer memory state tracker entry points
VkResult MemBoundWarning::PostCallBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                                   VkDeviceSize memoryOffset, VkResult result) {
    if (result == VK_SUCCESS) SetBindMemoryState<VkBuffer>(buffer, memory);

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindBufferMemory2(VkDevice device, uint32_t bindInfoCount,
                                                    const VkBindBufferMemoryInfo *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkBuffer>(pBindInfos[i].buffer, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount,
                                                       const VkBindBufferMemoryInfo *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkBuffer>(pBindInfos[i].buffer, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

// 3 buffer memory state tracker entry points
VkResult MemBoundWarning::PostCallBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset,
                                                  VkResult result) {
    if (result == VK_SUCCESS) SetBindMemoryState<VkImage>(image, memory);

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindImageMemory2(VkDevice device, uint32_t bindInfoCount,
                                                   const VkBindImageMemoryInfoKHR *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkImage>(pBindInfos[i].image, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount,
                                                      const VkBindImageMemoryInfo *pBindInfos, VkResult result) {
    if (result == VK_SUCCESS) {
        for (uint32_t i = 0; i < bindInfoCount; i++) {
            SetBindMemoryState<VkImage>(pBindInfos[i].image, pBindInfos[i].memory);
        }
    }

    return VK_SUCCESS;
}

// intercept for the sparse memory to track memory state
VkResult MemBoundWarning::PostCallQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo *pBindInfo,
                                                  VkFence fence, VkResult result) {
    for (uint32_t bindIdx = 0; bindIdx < bindInfoCount; bindIdx++) {
        const VkBindSparseInfo &bindInfo = pBindInfo[bindIdx];

        for (uint32_t j = 0; j < bindInfo.bufferBindCount; j++) {
            for (uint32_t k = 0; k < bindInfo.pBufferBinds[j].bindCount; k++) {
                auto sparse_binding = bindInfo.pBufferBinds[j].pBinds[k];
                SetBindMemoryState<VkBuffer>(bindInfo.pBufferBinds[j].buffer, sparse_binding.memory);
            }
        }
        for (uint32_t j = 0; j < bindInfo.imageOpaqueBindCount; j++) {
            for (uint32_t k = 0; k < bindInfo.pImageOpaqueBinds[j].bindCount; k++) {
                auto sparse_binding = bindInfo.pImageOpaqueBinds[j].pBinds[k];
                SetBindMemoryState<VkImage>(bindInfo.pImageOpaqueBinds[j].image, sparse_binding.memory);
            }
        }
        for (uint32_t j = 0; j < bindInfo.imageBindCount; j++) {
            for (uint32_t k = 0; k < bindInfo.pImageBinds[j].bindCount; k++) {
                auto sparse_binding = bindInfo.pImageBinds[j].pBinds[k];
                SetBindMemoryState<VkImage>(bindInfo.pImageBinds[j].image, sparse_binding.memory);
            }
        }
    }

    return VK_SUCCESS;
}

// Get memory associated with the handle being destroyed then find handle set using this memory and erase it
template <typename T>
void MemBoundWarning::RemoveBindedMemory(T handle) {
    uint64_t handle_64 = HandleToUint64(handle);
    auto memory_set = handle_memory_map.find(handle_64)->second;
    // remove all memory in the memory set. If it is not sparse memory, there will only be one item in set
    for (VkDeviceMemory memory : memory_set) {
        auto &handle_set = memory_handle_map.find(memory)->second;
        handle_set.erase(handle_64);
    }
}

VkResult MemBoundWarning::PostCallCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                                                     const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain,
                                                     VkResult result) {
    if (result == VK_SUCCESS) {
        std::vector<VkImage> images;
        swapchain_image_map.insert(std::make_pair(*pSwapchain, images));
    }

    return VK_SUCCESS;
}

VkResult MemBoundWarning::PostCallGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pSwapchainImageCount,
                                                        VkImage *pSwapchainImages, VkResult result) {
    auto images = swapchain_image_map.find(swapchain)->second;

    if (*pSwapchainImageCount > images.size()) images.resize(*pSwapchainImageCount);

    if (pSwapchainImages) {
        for (uint32_t i = 0; i < *pSwapchainImageCount; i++) {
            if (images[i] != VK_NULL_HANDLE) continue;  // Already retrieved image
            // TODO: Check if this should be a pointer
            images[i] = pSwapchainImages[i];
        }
    }

    return VK_SUCCESS;
}

void MemBoundWarning::PostCallDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain,
                                                  const VkAllocationCallbacks *pAllocator) {
    if (!swapchain) return;

    auto images = swapchain_image_map.find(swapchain)->second;
    if (images.size() > 0) {
        for (auto swapchain_image : images) {
            RemoveBindedMemory<VkImage>(swapchain_image);
        }
    }

    swapchain_image_map.erase(swapchain);
}

void MemBoundWarning::PreCallDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks *pAllocator) {
    RemoveBindedMemory<VkBuffer>(buffer);
}

void MemBoundWarning::PreCallDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks *pAllocator) {
    RemoveBindedMemory<VkImage>(image);
}

void MemBoundWarning::PreCallFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator) {
    auto handle_set = memory_handle_map.find(memory)->second;

    for (const auto &handle : handle_set) {
        char handle_string[64];
        char memory_string[64];
        uint64_t handle_64 = HandleToUint64(handle);
        uint64_t memory_64 = HandleToUint64(memory);
        sprintf(handle_string, "0x%" PRIxLEAST64, handle_64);
        sprintf(memory_string, "0x%" PRIxLEAST64, memory_64);

        std::stringstream message;
        message << "Vk Object " << handle_string << " still has a reference to Memory Object " << memory_string;
        Information(message.str());
    }
}

MemBoundWarning mem_bound_warning;