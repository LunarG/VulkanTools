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
    CALL_STATE vkGetPhysicalDeviceFeaturesState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceSurfaceCapabilitiesKHRState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceSurfacePresentModesKHRState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceSurfaceFormatsKHRState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceDisplayPlanePropertiesKHRState = UNCALLED;

    CALL_STATE vkGetPhysicalDeviceMemoryPropertiesState = UNCALLED;
    CALL_STATE vkGetBufferMemoryRequirementsState = UNCALLED;
    CALL_STATE vkGetImageMemoryRequirementsState = UNCALLED;
};

class SkipGetCallWarning : public layer_factory {
   public:
    // Constructor for state_tracker
    SkipGetCallWarning(){};

    // getters for the physical device state
    PHYSICAL_DEVICE_STATE* GetPhysicalDeviceState();
    PHYSICAL_DEVICE_STATE* GetPhysicalDeviceState(VkPhysicalDevice pd);

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

    VkResult PreCallGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount,
                                                               VkDisplayPlanePropertiesKHR* pProperties);
    VkResult PreCallGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex,
                                                        uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);

    void PreCallGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice,
                                                  VkPhysicalDeviceMemoryProperties* pMemoryProperties);
    VkResult PreCallAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo,
                                   const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);

    void PreCallGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
    void PreCallGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);

    VkResult PreCallBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
    VkResult PreCallBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

   private:
    PHYSICAL_DEVICE_STATE* physical_device_state;
    std::unordered_map<VkPhysicalDevice, PHYSICAL_DEVICE_STATE> physical_device_map;
};
