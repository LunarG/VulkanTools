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

#include "vulkan.h"
#include "configurator.h"

#include "../vkconfig_core/util.h"
#include "../vkconfig_core/platform.h"

#include <vulkan/vulkan.h>

#include <QLibrary>
#include <QtGlobal>
#include <QFileInfo>
#include <QMessageBox>

#include <cassert>

static const char *GetPhysicalDeviceType(VkPhysicalDeviceType type) {
    const char *translation[] = {"Other", "Integrated GPU", "Discrete GPU", "Virtual GPU", "CPU"};
    return translation[type];
}

Version GetVulkanLoaderVersion() {
    // Check loader version
    QLibrary library(GetPlatformString(PLATFORM_STRING_VULKAN_LIBRARY));

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
        log += format("- User-Defined Layers Paths from %s:\n", label);
        for (std::size_t i = 0, n = user_defined_layer_paths.size(); i < n; ++i)
            log += format("    - %s\n", user_defined_layer_paths[i].c_str());
    } else
        log += format("- User-Defined Layers Paths from %s: None\n", label);

    return log;
}

std::string GenerateVulkanStatus() {
    std::string log;

    // return log;  // bug https://github.com/LunarG/VulkanTools/issues/1172

    const Configurator &configurator = Configurator::Get();

    // Layers override configuration
    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers)) {
        log +=
            format("- Layers override: \"%s\" configuration\n", configurator.configurations.GetActiveConfiguration()->key.c_str());
    } else {
        log += "- Layers override: None\n";
    }

    // Check Vulkan SDK path
    const std::string search_path(qgetenv("VULKAN_SDK"));
    if (!search_path.empty())
        log += format("- VULKAN_SDK environment variable: %s\n", search_path.c_str());
    else
        log += "- VULKAN_SDK environment variable not set\n";

    const Version loader_version = GetVulkanLoaderVersion();

    if (loader_version == Version::VERSION_NULL) {
        QMessageBox alert(NULL);
        alert.setWindowTitle("Vulkan Development Status failure...");
        alert.setText("Could not find a Vulkan Loader.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        log += "- Could not find a Vulkan Loader.\n";
        return log;
    } else {
        log += format("- Vulkan Loader version: %s\n", loader_version.str().c_str());
        const LoaderMessageLevel loader_debug_message = configurator.environment.GetLoaderMessage();
        if (loader_debug_message != LAODER_MESSAGE_NONE) {
            log += format("    - VK_LOADER_DEBUG=%s\n", GetLoaderDebugToken(loader_debug_message).c_str());
        }
    }

    log += GetUserDefinedLayersPathsLog("VK_LAYER_PATH environment variable", USER_DEFINED_LAYERS_PATHS_ENV);
    if (configurator.configurations.HasActiveConfiguration(configurator.layers.available_layers))
        log += GetUserDefinedLayersPathsLog("Vulkan Configurator", USER_DEFINED_LAYERS_PATHS_GUI);

    QLibrary library(GetPlatformString(PLATFORM_STRING_VULKAN_LIBRARY));
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
            log +=
                format("    - %s (%s)\n", layers_properties[i].layerName, Version(layers_properties[i].specVersion).str().c_str());
        } else {
            log += format("    - %s (%s - %s)\n", layers_properties[i].layerName,
                          Version(layers_properties[i].specVersion).str().c_str(), status.c_str());
        }
    }

    // Check Vulkan Devices

    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pNext = NULL;
    app.pApplicationName = VKCONFIG_SHORT_NAME;
    app.applicationVersion = 0;
    app.pEngineName = VKCONFIG_SHORT_NAME;
    app.engineVersion = 0;
    app.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.pApplicationInfo = &app;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;

    uint32_t gpu_count;

    VkInstance inst;
    PFN_vkCreateInstance vkCreateInstance = (PFN_vkCreateInstance)library.resolve("vkCreateInstance");
    assert(vkCreateInstance);
    err = vkCreateInstance(&inst_info, NULL, &inst);
    if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
        QMessageBox alert(NULL);
        alert.setWindowTitle("Vulkan Development Status failure...");
        alert.setText("Cannot find a compatible Vulkan installable client driver (ICD).");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        log += "- Cannot find a compatible Vulkan installable client driver (ICD).\n";
        return log;
    }

    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices =
        (PFN_vkEnumeratePhysicalDevices)library.resolve("vkEnumeratePhysicalDevices");
    err = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);

    // This can fail on a new Linux setup. Check and fail gracefully rather than crash.
    if (err != VK_SUCCESS) {
        QMessageBox alert(NULL);
        alert.setWindowTitle("Vulkan Development Status failure...");
        alert.setText("Cannot find any Vulkan Physical Devices.");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();

        log += "- Cannot find a compatible Vulkan installable client driver (ICD).\n";
        return log;
    }

    std::vector<VkPhysicalDevice> devices;
    devices.resize(gpu_count);

    err = vkEnumeratePhysicalDevices(inst, &gpu_count, &devices[0]);
    assert(!err);

    log += "- Physical Devices:\n";
    for (std::size_t i = 0, n = devices.size(); i < n; ++i) {
        VkPhysicalDeviceProperties properties;
        PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties =
            (PFN_vkGetPhysicalDeviceProperties)library.resolve("vkGetPhysicalDeviceProperties");
        vkGetPhysicalDeviceProperties(devices[i], &properties);
        log += format("    - %s (%s) with Vulkan %d.%d.%d\n", properties.deviceName, GetPhysicalDeviceType(properties.deviceType),
                      VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion),
                      VK_VERSION_PATCH(properties.apiVersion));
    }

    PFN_vkDestroyInstance vkDestroyInstance = (PFN_vkDestroyInstance)library.resolve("vkDestroyInstance");
    vkDestroyInstance(inst, NULL);

    return log;
}
