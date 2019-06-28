#pragma once

#include <unordered_map>
#include "vulkan/vulkan.h"
#include "vk_layer_logging.h"
#include "layer_factory.h"

// pulled from KhronosGroup/Validation-Layers
enum CALL_STATE {
    UNCALLED,       // Function has not been called
    QUERY_COUNT,    // Function called once to query a count
    QUERY_DETAILS,  // Function called w/ a count to query details
};

// pulled from KhronosGroup/Validation-Layers
struct PHYSICAL_DEVICE_STATE {
    // Track the call state and array sizes for various query functions
    CALL_STATE vkGetPhysicalDeviceQueueFamilyPropertiesState = UNCALLED;
    // not used
    CALL_STATE vkGetPhysicalDeviceLayerPropertiesState = UNCALLED;      // depracated
    CALL_STATE vkGetPhysicalDeviceExtensionPropertiesState = UNCALLED;  // not sure
    CALL_STATE vkGetPhysicalDeviceFeaturesState = UNCALLED;             // needs to be called before vkCreateDevice
    // end not used
    CALL_STATE vkGetPhysicalDeviceSurfaceCapabilitiesKHRState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceSurfacePresentModesKHRState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceSurfaceFormatsKHRState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceDisplayPlanePropertiesKHRState = UNCALLED;
};

class SkipGetCallWarning : public layer_factory {
   public:
    // Constructor for state_tracker
    SkipGetCallWarning(){};

    void PreCallApiFunction(const char *api_name);

    PHYSICAL_DEVICE_STATE *GetPhysicalDeviceState();
    PHYSICAL_DEVICE_STATE *GetPhysicalDeviceState(VkPhysicalDevice pd);

    VkResult PostCallEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount,
                                              VkPhysicalDevice* pPhysicalDevices, VkResult result);

    void PreCallGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
    VkResult PreCallCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

    VkResult PreCallGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                            VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
    VkResult PreCallGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                            uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
    VkResult PreCallGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                                       uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
    VkResult PreCallCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);

   private:
    PHYSICAL_DEVICE_STATE *physical_device_state;
    std::unordered_map<VkPhysicalDevice, PHYSICAL_DEVICE_STATE> physical_device_map;
};
