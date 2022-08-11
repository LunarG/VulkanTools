/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
#include "configurator.h"

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/override.h"

#include <vulkan/vulkan.h>

#include <QLibrary>
#include <QtGlobal>

#include <cassert>

const char *GetVulkanLibrary() {
    static const char *TABLE[] = {
        "vulkan-1.dll",              // PLATFORM_WINDOWS
        "libvulkan",                 // PLATFORM_LINUX
        "/usr/local/lib/libvulkan",  // PLATFORM_MACOS
        "N/A",                       // PLATFORM_ANDROID
    };
    static_assert(countof(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[VKC_PLATFORM];
}

static const char *GetPhysicalDeviceType(VkPhysicalDeviceType type) {
    const char *translation[] = {"Other", "Integrated GPU", "Discrete GPU", "Virtual GPU", "CPU"};
    return translation[type];
}

Version GetVulkanLoaderVersion() {
    // Check loader version
    QLibrary library(GetVulkanLibrary());

    if (!library.load()) return Version::VERSION_NULL;

    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
    vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)library.resolve("vkEnumerateInstanceVersion");
    assert(vkEnumerateInstanceVersion);

    uint32_t version = 0;
    const VkResult result = vkEnumerateInstanceVersion(&version);
    assert(result == VK_SUCCESS);

    return Version(version);
}

static std::string GetUserDefinedLayersPathsLog(const char *label, UserDefinedLayersPaths custom_layer_path) {
    std::string log;

    const std::vector<std::string> &user_defined_layer_paths =
        Configurator::Get().environment.GetUserDefinedLayersPaths(custom_layer_path);
    if (!user_defined_layer_paths.empty()) {
        log += format("    - %s:\n", label);
        for (std::size_t i = 0, n = user_defined_layer_paths.size(); i < n; ++i)
            log += format("        - %s\n", user_defined_layer_paths[i].c_str());
    } else
        log += format("    - %s: None\n", label);

    return log;
}

VkResult CreateInstance(QLibrary &library, VkInstance &instance, bool enumerate_portability) {
    if (!enumerate_portability) return VK_ERROR_INCOMPATIBLE_DRIVER;

    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties =
        (PFN_vkEnumerateInstanceExtensionProperties)library.resolve("vkEnumerateInstanceExtensionProperties");
    assert(vkEnumerateInstanceExtensionProperties);

    uint32_t property_count = 0;
    VkResult err = vkEnumerateInstanceExtensionProperties(nullptr, &property_count, nullptr);
    assert(err == VK_SUCCESS);

    std::vector<VkExtensionProperties> instance_properties(property_count);
    err = vkEnumerateInstanceExtensionProperties(nullptr, &property_count, &instance_properties[0]);
    assert(err == VK_SUCCESS);

    // Handle Portability Enumeration requirements
    std::vector<const char *> instance_extensions;
#if VK_KHR_portability_enumeration
    for (std::size_t i = 0, n = instance_properties.size(); i < n && enumerate_portability; ++i) {
        if (instance_properties[i].extensionName == std::string(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
            instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            break;
        }
    }
#endif

    // Check Vulkan Devices

    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pNext = nullptr;
    app.pApplicationName = VKCONFIG_SHORT_NAME;
    app.applicationVersion = 0;
    app.pEngineName = VKCONFIG_SHORT_NAME;
    app.engineVersion = 0;
    app.apiVersion = VK_API_VERSION_1_0;

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

    PFN_vkCreateInstance vkCreateInstance = (PFN_vkCreateInstance)library.resolve("vkCreateInstance");
    assert(vkCreateInstance);

    return vkCreateInstance(&inst_info, nullptr, &instance);
}

std::string GenerateVulkanStatus() {
    std::string log;

    const Configurator &configurator = Configurator::Get();

    // Layers override configuration
    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
        log +=
            format("- Layers override: \"%s\" configuration\n", configurator.configurations.GetActiveConfiguration()->key.c_str());
    } else {
        log += "- Layers override: None\n";
    }

    // Check Vulkan SDK path
    const std::string search_path(configurator.path.GetPath(PATH_VULKAN_SDK));
    if (!search_path.empty())
        log += format("- VULKAN_SDK environment variable: %s\n", search_path.c_str());
    else
        log += "- VULKAN_SDK environment variable not set\n";

    const Version loader_version = GetVulkanLoaderVersion();

    if (loader_version == Version::VERSION_NULL) {
        Alert::LoaderFailure();

        log += "- Could not find a Vulkan Loader.\n";
        return log;
    } else {
        log += format("- Vulkan Loader version: %s\n", loader_version.str().c_str());
        const LoaderMessageLevel loader_debug_message = configurator.environment.GetLoaderMessage();
        if (loader_debug_message != LOADER_MESSAGE_NONE) {
            log += format("    - VK_LOADER_DEBUG=%s\n", GetLoaderDebugToken(loader_debug_message).c_str());
        }
    }

    log += "- User-Defined Layers locations:\n";
    log += GetUserDefinedLayersPathsLog("VK_LAYER_PATH variable", USER_DEFINED_LAYERS_PATHS_ENV_SET);
    log += GetUserDefinedLayersPathsLog("Per-configuration paths", USER_DEFINED_LAYERS_PATHS_GUI);
    log += GetUserDefinedLayersPathsLog("VK_ADD_LAYER_PATH variable", USER_DEFINED_LAYERS_PATHS_ENV_ADD);

    const std::string layer_settings_path(qgetenv("VK_LAYER_SETTINGS_PATH"));
    if (!layer_settings_path.empty()) {
        log += "- `vk_layer_settings.txt` location overridden by VK_LAYER_SETTINGS_PATH:\n";
        if (layer_settings_path.find("vk_layer_settings.txt") == std::string::npos) {
            log += format("    %s\n", layer_settings_path.c_str());
        } else {
            log += format("    %s\n", ExtractAbsoluteDir(layer_settings_path).c_str());
        }
    } else {
        const std::string path = GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);
        log += "- `vk_layer_settings.txt` uses the default platform path:\n";
        log += format("    %s\n", ExtractAbsoluteDir(path).c_str());
    }

    // If there is no Vulkan loader installed, we can't call any Vulkan API.
    if (loader_version == Version::VERSION_NULL) {
        return log;
    }

    Configuration *active_configuration = configurator.configurations.GetActiveConfiguration();
    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
        SurrenderConfiguration(configurator.environment);
    }

    QLibrary library(GetVulkanLibrary());
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties =
        (PFN_vkEnumerateInstanceLayerProperties)library.resolve("vkEnumerateInstanceLayerProperties");
    assert(vkEnumerateInstanceLayerProperties);

    std::uint32_t instance_layer_count = 0;
    VkResult err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
    assert(!err);

    std::vector<VkLayerProperties> layers_properties;
    layers_properties.resize(instance_layer_count);

    err = vkEnumerateInstanceLayerProperties(&instance_layer_count, &layers_properties[0]);
    assert(!err);

    log += "- Available Layers:\n";
    for (std::size_t i = 0, n = layers_properties.size(); i < n; ++i) {
        const Layer *layer = FindByKey(configurator.layers.available_layers, layers_properties[i].layerName);

        std::string status;
        if (layer != nullptr) {
            if (layer->status != STATUS_STABLE) {
                status = GetToken(layer->status);
            }
        }

        if (status.empty()) {
            log += format("    - %s\n", layers_properties[i].layerName);
        } else {
            log += format("    - %s (%s)\n", layers_properties[i].layerName, status.c_str());
        }
    }

    VkInstance inst = VK_NULL_HANDLE;
    err = CreateInstance(library, inst, false);
    if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
        // If no compatible driver were found, trying with portability enumeration
        err = CreateInstance(library, inst, true);
        if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
            Alert::InstanceFailure();

            log += "- Cannot find a compatible Vulkan installable client driver (ICD).\n";
            return log;
        }
    }
    assert(err == VK_SUCCESS);

    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices =
        (PFN_vkEnumeratePhysicalDevices)library.resolve("vkEnumeratePhysicalDevices");

    uint32_t gpu_count = 0;
    err = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);

    PFN_vkDestroyInstance vkDestroyInstance = (PFN_vkDestroyInstance)library.resolve("vkDestroyInstance");

    // This can fail on a new Linux setup. Check and fail gracefully rather than crash.
    if (err != VK_SUCCESS) {
        Alert::PhysicalDeviceFailure();
        vkDestroyInstance(inst, NULL);

        log += "- Cannot find a compatible Vulkan installable client driver (ICD).\n";
        return log;
    }

    std::vector<VkPhysicalDevice> devices;
    devices.resize(gpu_count);

    err = vkEnumeratePhysicalDevices(inst, &gpu_count, &devices[0]);
    assert(!err);

    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties =
        (PFN_vkGetPhysicalDeviceProperties)library.resolve("vkGetPhysicalDeviceProperties");

    log += "- Physical Devices:\n";
    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(devices[i], &properties);
        log += format("    - %s (%s) with Vulkan %d.%d.%d\n", properties.deviceName, GetPhysicalDeviceType(properties.deviceType),
                      VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion),
                      VK_VERSION_PATCH(properties.apiVersion));
    }

    vkDestroyInstance(inst, NULL);

    if (active_configuration != nullptr) {
        OverrideConfiguration(configurator.environment, configurator.layers.available_layers, *active_configuration);
    }

    return log;
}
