/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "vulkan_util.h"

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/util.h"

#include <QLibrary>

#include <cassert>

static std::string GetUUIDString(const uint8_t deviceUUID[VK_UUID_SIZE]) {
    std::string result;

    for (std::size_t i = 0, n = VK_UUID_SIZE; i < n; ++i) {
        result += format("%02X", deviceUUID[i]);
    }

    return result;
}

struct VulkanFunctions {
    VulkanFunctions();
    bool Validate() const;

    PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion;
    PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties;
    PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices;
    PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties;
    PFN_vkCreateInstance CreateInstance;
    PFN_vkDestroyInstance DestroyInstance;
    PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2;
};

static const char *GetVulkanLibrary() {
    static const char *TABLE[] = {
        "vulkan-1.dll",              // PLATFORM_WINDOWS_X86
        "vulkan-1.dll",              // PLATFORM_WINDOWS_ARM
        "libvulkan",                 // PLATFORM_LINUX
        "/usr/local/lib/libvulkan",  // PLATFORM_MACOS
        "N/A",                       // PLATFORM_ANDROID
        "N/A",                       // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
}

VulkanFunctions::VulkanFunctions() {
    QLibrary library(GetVulkanLibrary());
    if (!library.load()) {
        return;
    }

    this->EnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)library.resolve("vkEnumerateInstanceVersion");
    this->EnumerateInstanceLayerProperties =
        (PFN_vkEnumerateInstanceLayerProperties)library.resolve("vkEnumerateInstanceLayerProperties");
    this->EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)library.resolve("vkEnumeratePhysicalDevices");
    this->EnumerateInstanceExtensionProperties =
        (PFN_vkEnumerateInstanceExtensionProperties)library.resolve("vkEnumerateInstanceExtensionProperties");
    this->CreateInstance = (PFN_vkCreateInstance)library.resolve("vkCreateInstance");
    this->DestroyInstance = (PFN_vkDestroyInstance)library.resolve("vkDestroyInstance");
    this->GetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)library.resolve("vkGetPhysicalDeviceProperties2");
}

bool VulkanFunctions::Validate() const {
    return this->EnumerateInstanceVersion != nullptr && this->EnumerateInstanceLayerProperties != nullptr &&
           this->EnumeratePhysicalDevices != nullptr && this->EnumerateInstanceExtensionProperties != nullptr &&
           this->CreateInstance != nullptr && this->DestroyInstance != nullptr && this->GetPhysicalDeviceProperties2 != nullptr;
}

VulkanSystemInfo BuildVulkanSystemInfo() {
    VulkanSystemInfo vulkan_system_info;

    VulkanFunctions vk;
    if (!vk.Validate()) {
        return vulkan_system_info;
    }

    uint32_t api_version = 0;
    VkResult result = vk.EnumerateInstanceVersion(&api_version);
    vulkan_system_info.loaderVersion = Version(api_version);
    assert(result == VK_SUCCESS);

    std::uint32_t instance_layer_count = 0;
    result = vk.EnumerateInstanceLayerProperties(&instance_layer_count, nullptr);
    assert(result == VK_SUCCESS);
    vulkan_system_info.instanceLayerProperties.resize(instance_layer_count);
    result = vk.EnumerateInstanceLayerProperties(&instance_layer_count, vulkan_system_info.instanceLayerProperties.data());
    assert(result == VK_SUCCESS);

    uint32_t instance_extension_count = 0;
    result = vk.EnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr);
    assert(result == VK_SUCCESS);
    vulkan_system_info.instanceExtensionPropertie.resize(instance_extension_count);
    result = vk.EnumerateInstanceExtensionProperties(nullptr, &instance_extension_count,
                                                     vulkan_system_info.instanceExtensionPropertie.data());
    assert(result == VK_SUCCESS);

    // Handle Portability Enumeration requirements
    std::vector<const char *> instance_extensions;
#if VK_KHR_portability_enumeration
    for (std::size_t i = 0, n = vulkan_system_info.instanceExtensionPropertie.size(); i < n; ++i) {
        if (vulkan_system_info.instanceExtensionPropertie[i].extensionName ==
            std::string(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
            instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        }
    }
#endif

    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pNext = nullptr;
    app.pApplicationName = VKCONFIG_SHORT_NAME;
    app.applicationVersion = 0;
    app.pEngineName = VKCONFIG_SHORT_NAME;
    app.engineVersion = 0;
    app.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#if VK_KHR_portability_enumeration
    if (!instance_extensions.empty()) {
        inst_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
#endif
    inst_info.pNext = nullptr;
    inst_info.pApplicationInfo = &app;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = nullptr;
    inst_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size());
    inst_info.ppEnabledExtensionNames = instance_extensions.empty() ? nullptr : &instance_extensions[0];

    VkInstance instance = VK_NULL_HANDLE;
    vk.CreateInstance(&inst_info, nullptr, &instance);

    uint32_t physical_device_count = 0;
    result = vk.EnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

    // This can fail on a new Linux setup. Check and fail gracefully rather than crash.
    if (result != VK_SUCCESS) {
        vk.DestroyInstance(instance, NULL);
        return vulkan_system_info;
    }

    std::vector<VkPhysicalDevice> devices;
    devices.resize(physical_device_count);
    result = vk.EnumeratePhysicalDevices(instance, &physical_device_count, devices.data());
    assert(result == VK_SUCCESS);

    vulkan_system_info.physicalDevices.resize(devices.size());

    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        VulkanPhysicalDeviceInfo &device_info = vulkan_system_info.physicalDevices[i];

        VkPhysicalDeviceIDPropertiesKHR properties_deviceid{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR, nullptr};
        VkPhysicalDeviceProperties2 properties2{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, &properties_deviceid};

        vk.GetPhysicalDeviceProperties2(devices[i], &properties2);

        device_info.deviceName = properties2.properties.deviceName;
        device_info.apiVersion = Version(properties2.properties.apiVersion);
        device_info.deviceUUID = GetUUIDString(properties_deviceid.deviceUUID);
        device_info.driverUUID = GetUUIDString(properties_deviceid.driverUUID);
    }

    vk.DestroyInstance(instance, NULL);

    return vulkan_system_info;
}
