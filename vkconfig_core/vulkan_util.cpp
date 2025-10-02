/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include <sstream>
#include <iostream>

std::string GetUUIDString(const uint8_t deviceUUID[VK_UUID_SIZE]) {
    std::string result;

    for (std::size_t i = 0, n = VK_UUID_SIZE; i < n; ++i) {
        result += format("%02X", deviceUUID[i]);
    }

    return result;
}

std::string GetLUIDString(const uint8_t deviceLUID[VK_LUID_SIZE]) {
    std::string result;

    for (std::size_t i = 0, n = VK_LUID_SIZE; i < n; ++i) {
        result += format("%02X", deviceLUID[i]);
    }

    return result;
}

struct VulkanFunctions {
    VulkanFunctions();
    bool Validate() const;

    PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion = nullptr;
    PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties = nullptr;
    PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices = nullptr;
    PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties = nullptr;
    PFN_vkCreateInstance CreateInstance = nullptr;
    PFN_vkDestroyInstance DestroyInstance = nullptr;
    PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2 = nullptr;
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

static VkResult CreateInstance(const VulkanFunctions &vk, bool enumerate_portability, VkInstance &instance) {
    uint32_t property_count = 0;
    VkResult err = vk.EnumerateInstanceExtensionProperties(nullptr, &property_count, nullptr);
    assert(err == VK_SUCCESS);

    std::vector<VkExtensionProperties> instance_properties(property_count);
    err = vk.EnumerateInstanceExtensionProperties(nullptr, &property_count, &instance_properties[0]);
    assert(err == VK_SUCCESS);

    // Handle Portability Enumeration requirements
    std::vector<const char *> instance_extensions;

    for (std::size_t i = 0, n = instance_properties.size(); i < n; ++i) {
        if (instance_properties[i].extensionName == std::string(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME)) {
            instance_extensions.push_back(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);
        }
#if VK_KHR_portability_enumeration
        if (enumerate_portability) {
            if (instance_properties[i].extensionName == std::string(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
                instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            }
        }
#endif
    }

    // Check Vulkan Devices

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
    if (enumerate_portability) {
        inst_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
#endif
    inst_info.pNext = nullptr;
    inst_info.pApplicationInfo = &app;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = nullptr;
    inst_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size());
    inst_info.ppEnabledExtensionNames = instance_extensions.empty() ? nullptr : &instance_extensions[0];

    return vk.CreateInstance(&inst_info, nullptr, &instance);
}

std::string VulkanPhysicalDeviceInfo::GetVersion() const {
    if (this->vendorID == 0x10DE) {
        return FormatNvidia(this->driverVersion);
    } else if ((this->vendorID == 0x8086) && (VKC_PLATFORM & PLATFORM_WINDOWS_BIT)) {
        return FormatIntelWindows(this->driverVersion);
    } else {
        return Version(this->driverVersion).str();
    }
}

DeviceInfo GetDeviceInfo(const VulkanPhysicalDeviceInfo &info) {
    DeviceInfo device_info;
    device_info.deviceName = info.deviceName;
    std::copy(std::begin(info.deviceUUID), std::end(info.deviceUUID), std::begin(device_info.deviceUUID));
    device_info.driverVersion = info.driverVersion;
    return device_info;
}

bool operator==(const DeviceInfo &a, const DeviceInfo &b) {
    if (a.deviceName != b.deviceName) {
        return false;
    }

    for (int i = 0, n = VK_UUID_SIZE; i < n; ++i) {
        if (a.deviceUUID[i] != b.deviceUUID[i]) {
            return false;
        }
    }

    if (a.driverVersion != b.driverVersion) {
        return false;
    }

    return true;
}

bool operator!=(const DeviceInfo &a, const DeviceInfo &b) { return !(a == b); }

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

        /* For multiple driver version testing only
                if (device_info.deviceName.find("llvmpipe") != std::string::npos) {
                    device_info.deviceName = "llvmpipe";
                }
        */

        device_info.driverVersion = properties2.properties.driverVersion;
        device_info.apiVersion = Version(properties2.properties.apiVersion);
        device_info.vendorID = static_cast<VendorID>(properties2.properties.vendorID);
        device_info.deviceID = properties2.properties.deviceID;
        device_info.deviceType = properties2.properties.deviceType;
        memcpy(device_info.deviceUUID, properties_deviceid.deviceUUID, sizeof(device_info.deviceUUID));
        memcpy(device_info.driverUUID, properties_deviceid.driverUUID, sizeof(device_info.driverUUID));
        memcpy(device_info.deviceLUID, properties_deviceid.deviceLUID, sizeof(device_info.deviceLUID));
    }

    vk.DestroyInstance(instance, NULL);

    return vulkan_system_info;
}

static const char *GetDefaultPrefix(bool android_sysprop) {
    if (android_sysprop) {
        return "vulkan";
    } else {
        return "";
    }
}

enum TrimMode {
    TRIM_NONE,
    TRIM_VENDOR,
    TRIM_NAMESPACE,

    TRIM_FIRST = TRIM_NONE,
    TRIM_LAST = TRIM_NAMESPACE,
};

std::string TrimPrefix(const std::string &layer_key) {
    std::string key{};
    if (layer_key.find("VK_LAYER_") == 0) {
        std::size_t prefix = std::strlen("VK_LAYER_");
        key = layer_key.substr(prefix, layer_key.size() - prefix);
    } else {
        key = layer_key;
    }
    return key;
}

static std::string TrimVendor(const std::string &layer_key) {
    static const char *separator = "_";

    const std::string &namespace_key = TrimPrefix(layer_key);

    const auto trimmed_beg = namespace_key.find_first_of(separator);
    if (trimmed_beg == std::string::npos) return namespace_key;

    assert(namespace_key.find_last_not_of(separator) != std::string::npos &&
           trimmed_beg <= namespace_key.find_last_not_of(separator));

    return namespace_key.substr(trimmed_beg + 1, namespace_key.size());
}

static std::string GetFileSettingName(const char *pLayerName, const char *pSettingName) {
    assert(pLayerName != nullptr);
    assert(pSettingName != nullptr);

    std::stringstream settingName;
    settingName << ToLowerCase(TrimPrefix(pLayerName)) << "." << pSettingName;

    return settingName.str();
}

static std::string GetEnvSettingName(const char *layer_key, const char *requested_prefix, const char *setting_key,
                                     TrimMode trim_mode, bool android_sysprop) {
    std::stringstream result;
    const std::string prefix =
        (requested_prefix == nullptr || trim_mode != TRIM_NAMESPACE) ? GetDefaultPrefix(android_sysprop) : requested_prefix;

    if (android_sysprop) {
        const std::string full_prefix = std::string("debug.") + prefix + ".";
        switch (trim_mode) {
            default:
            case TRIM_NONE: {
                result << full_prefix << GetFileSettingName(layer_key, setting_key);
                break;
            }
            case TRIM_VENDOR: {
                result << full_prefix << GetFileSettingName(TrimVendor(layer_key).c_str(), setting_key);
                break;
            }
            case TRIM_NAMESPACE: {
                result << full_prefix << setting_key;
                break;
            }
        }
    } else {
        const std::string full_prefix = std::string("VK_") + (prefix.empty() ? "" : prefix + "_");
        switch (trim_mode) {
            default:
            case TRIM_NONE: {
                result << full_prefix << ToUpperCase(TrimPrefix(layer_key)) << "_" << ToUpperCase(setting_key);
                break;
            }
            case TRIM_VENDOR: {
                result << full_prefix << ToUpperCase(TrimVendor(layer_key)) << "_" << ToUpperCase(setting_key);
                break;
            }
            case TRIM_NAMESPACE: {
                result << full_prefix << ToUpperCase(setting_key);
                break;
            }
        }
    }

    return result.str();
}

std::vector<std::string> BuildEnvVariablesList(const char *layer_key, const char *setting_key, bool android_sysprop) {
    std::vector<std::string> results;

    for (int trim_index = TRIM_FIRST; trim_index <= TRIM_LAST; ++trim_index) {
        results.push_back(GetEnvSettingName(layer_key, GetDefaultPrefix(android_sysprop), setting_key,
                                            static_cast<TrimMode>(trim_index), android_sysprop));
    }

    return results;
}

std::string GetCodeType(const std::string &layer_key) {
    std::string key = TrimVendor(layer_key);
    key.erase(std::remove_if(key.begin(), key.end(), [](char c) { return c == '_'; }), key.end());
    key[0] = std::toupper(key[0]);
    return key + "SettingData";
}

std::string GetCodeData(const std::string &layer_key) { return TrimVendor(layer_key); }

std::string GetSettingValueName(const std::string &layer_key, const std::string &setting_key, const std::string &value_key) {
    std::string layer = ::ToUpperCase(::GetCodeData(layer_key));
    std::string setting = ::ToUpperCase(setting_key);
    std::string value = ::ToUpperCase(value_key);

    return format("VL_%s_%s_%s", layer.c_str(), setting.c_str(), value.c_str());
}

const char *GetLabel(VkPhysicalDeviceType deviceType) {
    static const char *TABLES[] = {
        "",            // VK_PHYSICAL_DEVICE_TYPE_OTHER
        "Integrated",  // VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
        "Discrete",    // VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
        "Virtual",     // VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU
        "CPU"          // VK_PHYSICAL_DEVICE_TYPE_CPU
    };

    return TABLES[deviceType];
}

std::string GetLabel(VendorID vendorID) {
    switch (vendorID) {
        default:
            return "Unknown Vendor";
        case static_cast<VendorID>(VK_VENDOR_ID_KHRONOS):
            return "KHRONOS";
        case static_cast<VendorID>(VK_VENDOR_ID_MESA):
            return "Mesa";
        case VENDOR_ID_AMD:
            return "AMD";
        case VENDOR_ID_APPLE:
            return "Apple";
        case VENDOR_ID_ARM:
            return "ARM";
        case VENDOR_ID_IMGTEC:
            return "Imagination";
        case VENDOR_ID_INTEL:
            return "Intel";
        case VENDOR_ID_MICROSOFT:
            return "Microsoft";
        case VENDOR_ID_NVIDIA:
            return "Nvidia";
        case VENDOR_ID_QUALCOMM:
            return "Qualcomm";
    }
}

std::string GenerateLoaderLog() {
    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));
    setbuf(stderr, buffer);

    bool is_set = qEnvironmentVariableIsSet("VK_LOADER_DEBUG");
    std::string saved_data = is_set ? qgetenv("VK_LOADER_DEBUG").toStdString() : "";

    qputenv("VK_LOADER_DEBUG", "all");

    fprintf(stderr, "gni");

    VulkanFunctions vk;

    if (vk.Validate()) {
        VkInstance instance = VK_NULL_HANDLE;

        VkResult err = ::CreateInstance(vk, false, instance);
        if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
            err = ::CreateInstance(vk, true, instance);
            if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
                return "VK_ERROR_INCOMPATIBLE_DRIVER\n";
            }
        }
        vk.DestroyInstance(instance, nullptr);
    }

    return "";
}
