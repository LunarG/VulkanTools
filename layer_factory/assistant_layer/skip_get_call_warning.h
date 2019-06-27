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
    CALL_STATE vkGetPhysicalDeviceLayerPropertiesState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceExtensionPropertiesState = UNCALLED;
    CALL_STATE vkGetPhysicalDeviceFeaturesState = UNCALLED;
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
    PHYSICAL_DEVICE_STATE *GetPhyscialDeviceState(VkPhysicalDevice pd);

   private:
    PHYSICAL_DEVICE_STATE *physical_device_state;
    std::unordered_map<VkPhysicalDevice, PHYSICAL_DEVICE_STATE> physical_device_map;
};
