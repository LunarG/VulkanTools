#include "skip_get_call_warning.h"
#include <sstream>
#include <iostream>

PHYSICAL_DEVICE_STATE* SkipGetCallWarning::GetPhysicalDeviceState() { return physical_device_state; }

PHYSICAL_DEVICE_STATE* SkipGetCallWarning::GetPhysicalDeviceState(VkPhysicalDevice pd) {
    auto* phys_dev_map = &physical_device_map;
    auto physicalDevice = phys_dev_map->find(pd);
    if (physicalDevice == phys_dev_map->end()) {
        return nullptr;
    }
    return &physicalDevice->second;
}

// helper function that sets the state of any get call that has a count check as well
template <typename T, typename S>
void SetStateWithCount(T& call_state, S pStruct) {
    if (!pStruct) {
        if (call_state == UNCALLED) call_state = QUERY_COUNT;
    } else {
        call_state = QUERY_DETAILS;
    }
}

// Populate physical_device_map with all the Physical Devices
VkResult SkipGetCallWarning::PostCallEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount,
                                                              VkPhysicalDevice* pPhysicalDevices, VkResult result) {
    if ((pPhysicalDevices != NULL) && ((result == VK_SUCCESS || result == VK_INCOMPLETE))) {
        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {
            PHYSICAL_DEVICE_STATE pd_state;
            physical_device_map.insert(std::make_pair(pPhysicalDevices[i], pd_state));
        }
    }

    return VK_SUCCESS;
}

void SkipGetCallWarning::PreCallGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    physical_device_state->vkGetPhysicalDeviceFeaturesState = QUERY_DETAILS;
}

// need to set the physical_device_state when creating device to access the state for API Calls that do not have a physicalDevice
// handle
VkResult SkipGetCallWarning::PreCallCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    auto pd_state = GetPhysicalDeviceState(physicalDevice);
    physical_device_state = pd_state;

    if ((pd_state->vkGetPhysicalDeviceFeaturesState == UNCALLED) && (pCreateInfo->pEnabledFeatures != NULL)) {
        std::stringstream message;
        message << "vkCreateDevice called before getting physical device features from vkGetPhysicalDeviceFeatures().";
        Warning(message.str());
    }

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                                            VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    physical_device_state->vkGetPhysicalDeviceSurfaceCapabilitiesKHRState = QUERY_DETAILS;

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                                            uint32_t* pPresentModeCount,
                                                                            VkPresentModeKHR* pPresentModes) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    auto& call_state = physical_device_state->vkGetPhysicalDeviceSurfacePresentModesKHRState;

    SetStateWithCount(call_state, pPresentModes);

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                                       uint32_t* pSurfaceFormatCount,
                                                                       VkSurfaceFormatKHR* pSurfaceFormats) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    auto& call_state = physical_device_state->vkGetPhysicalDeviceSurfaceFormatsKHRState;

    SetStateWithCount(call_state, pSurfaceFormats);

    return VK_SUCCESS;
}

// check that makes sure all necessary get calls are made before creating the swapchain
VkResult SkipGetCallWarning::PreCallCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
                                                       const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
    auto physical_device_state = GetPhysicalDeviceState();

    if (physical_device_state->vkGetPhysicalDeviceSurfaceCapabilitiesKHRState == UNCALLED) {
        std::stringstream message;
        message
            << "vkCreateSwapchainKHR called before getting surface capabilites from vkGetPhysicalDeviceSurfaceCapabilitiesKHR().";
        Warning(message.str());
    }

    if (physical_device_state->vkGetPhysicalDeviceSurfacePresentModesKHRState != QUERY_DETAILS) {
        std::stringstream message;
        message << "vkCreateSwapchainKHR called before getting surface present mode(s) from "
                   "vkGetPhysicalDeviceSurfacePresentModesKHR().";
        Warning(message.str());
    }

    if (physical_device_state->vkGetPhysicalDeviceSurfaceFormatsKHRState != QUERY_DETAILS) {
        std::stringstream message;
        message << "vkCreateSwapchainKHR called before getting surface format(s) from vkGetPhysicalDeviceSurfaceFormatsKHR().";
        Warning(message.str());
    }

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice,
                                                                               uint32_t* pPropertyCount,
                                                                               VkDisplayPlanePropertiesKHR* pProperties) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    auto& call_state = physical_device_state->vkGetPhysicalDeviceDisplayPlanePropertiesKHRState;

    SetStateWithCount(call_state, pProperties);

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex,
                                                                        uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {
    auto physical_device_state = GetPhysicalDeviceState();

    if (physical_device_state->vkGetPhysicalDeviceDisplayPlanePropertiesKHRState != QUERY_DETAILS) {
        std::stringstream message;
        message << "vkGetDisplayPlaneSupportedDiesplaysKHR called before getting diplay plane properties from "
                   "vkGetPhysicalDeviceDisplayPlanePropertiesKHR().";
        Warning(message.str());
    }

    return VK_SUCCESS;
}

void SkipGetCallWarning::PreCallGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice,
                                                                  VkPhysicalDeviceMemoryProperties* pMemoryProperties) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    physical_device_state->vkGetPhysicalDeviceMemoryPropertiesState = QUERY_DETAILS;
}

VkResult SkipGetCallWarning::PreCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo,
                                                   const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {
    auto physical_device_state = GetPhysicalDeviceState();

    if (physical_device_state->vkGetPhysicalDeviceMemoryPropertiesState == UNCALLED) {
        std::stringstream message;
        message << "vkAllocateMemory called before retrieving memoryHeaps[pAllocateInfo->memoryTypeIndex].size and memoryTypeCount "
                   "from vkGetPhysicalDeviceMemoryProperties().";
        Warning(message.str());
    }

    return VK_SUCCESS;
}

void SkipGetCallWarning::PreCallGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer,
                                                            VkMemoryRequirements* pMemoryRequirements) {
    auto physical_device_state = GetPhysicalDeviceState();
    physical_device_state->vkGetBufferMemoryRequirementsState = QUERY_DETAILS;
}

void SkipGetCallWarning::PreCallGetImageMemoryRequirements(VkDevice device, VkImage image,
                                                           VkMemoryRequirements* pMemoryRequirements) {
    auto physical_device_state = GetPhysicalDeviceState();
    physical_device_state->vkGetImageMemoryRequirementsState = QUERY_DETAILS;
}

VkResult SkipGetCallWarning::PreCallBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                                                     VkDeviceSize memoryOffset) {
    auto physical_device_state = GetPhysicalDeviceState();

    if (physical_device_state->vkGetBufferMemoryRequirementsState == UNCALLED) {
        std::stringstream message;
        message << "vkBindBufferMemory called before retrieving memoryTypeBits, alignment, and size in the VkMemoryRequirements "
                   "structure returned from vkGetBufferMemoryRequirements().";
        Warning(message.str());
    }

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory,
                                                    VkDeviceSize memoryOffset) {
    auto physical_device_state = GetPhysicalDeviceState();

    if (physical_device_state->vkGetImageMemoryRequirementsState == UNCALLED) {
        std::stringstream message;
        message << "vkBindImageMemory called before retrieving memoryTypeBits, alignment, and size in the VkMemoryRequirements "
                   "structure returned fro vkGetImagememoryRequirements().";
        Warning(message.str());
    }

    return VK_SUCCESS;
}

SkipGetCallWarning skip_get_call_warning;
