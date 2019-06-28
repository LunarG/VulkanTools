#include "skip_get_call_warning.h"
#include <sstream>
#include <iostream>

// TODO: REMOVE METHOD. THIS IS JUST FOR TESTING
void SkipGetCallWarning::PreCallApiFunction(const char *api_name) { printf("Calling %s\n", api_name); }

PHYSICAL_DEVICE_STATE *SkipGetCallWarning::GetPhysicalDeviceState() { return physical_device_state; }

PHYSICAL_DEVICE_STATE *SkipGetCallWarning::GetPhysicalDeviceState(VkPhysicalDevice pd) {
    auto *phys_dev_map = &physical_device_map;
    auto physicalDevice = phys_dev_map->find(pd);
    if (physicalDevice == phys_dev_map->end()) {
        return nullptr;
    }
    return &physicalDevice->second;
}

// Populate physical_device_map with al the Physical Devices
VkResult SkipGetCallWarning::PostCallEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount,
                                                              VkPhysicalDevice* pPhysicalDevices, VkResult result) {
    if ((pPhysicalDevices != NULL) && ((result == VK_SUCCESS || result == VK_INCOMPLETE))) {
        for (uint32_t i = 0; i < *pPhysicalDeviceCount; i++) {
            // TODO: Look at moving this to the [] accessor. Less readable but better complexity maybe.
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

    if (pd_state->vkGetPhysicalDeviceFeaturesState == UNCALLED) {
        std::stringstream message;
        message << "vkCreateDevice caled before getting physical device features from vkGetPhysicalDeviceFeatures().";
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

    if (!pPresentModes) {
        if (call_state == UNCALLED) call_state = QUERY_COUNT;
    } else {
        call_state = QUERY_DETAILS;
    }

    return VK_SUCCESS;
}

VkResult SkipGetCallWarning::PreCallGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                                       uint32_t* pSurfaceFormatCount,
                                                                       VkSurfaceFormatKHR* pSurfaceFormats) {
    auto physical_device_state = GetPhysicalDeviceState(physicalDevice);
    auto& call_state = physical_device_state->vkGetPhysicalDeviceSurfaceFormatsKHRState;

    if (!pSurfaceFormats) {
        if (call_state == UNCALLED) call_state = QUERY_COUNT;
    } else {
        call_state = QUERY_DETAILS;
    }

    return VK_SUCCESS;
}

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

SkipGetCallWarning skip_get_call_warning;
