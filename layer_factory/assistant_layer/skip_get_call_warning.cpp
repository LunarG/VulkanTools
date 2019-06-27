#include "skip_get_call_warning.h"
#include <sstream>

void SkipGetCallWarning::PreCallApiFunction(const char *api_name) { printf("Calling %s\n", api_name); }

PHYSICAL_DEVICE_STATE *SkipGetCallWarning::GetPhysicalDeviceState() { return physical_device_state; }
PHYSICAL_DEVICE_STATE *SkipGetCallWarning::GetPhyscialDeviceState(VkPhysicalDevice pd) {
    auto *phys_dev_map = &physical_device_map;
    auto physicalDevice = phys_dev_map->find(pd);
    if (physicalDevice == phys_dev_map->end()) {
        return nullptr;
    }
    return &physicalDevice->second;
}

SkipGetCallWarning skip_get_call_warning;
