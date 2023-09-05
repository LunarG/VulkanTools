/*
 * Copyright (c) 2023 The Khronos Group Inc.
 * Copyright (c) 2023 Valve Corporation
 * Copyright (c) 2023 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Author: Christophe Riccio <christophe@lunarg.com>
 */

#include "layer_test_helper.h"

#ifdef _WIN32
#include <Windows.h>
#elif __ANDROID__
#include <android/log.h>
#endif

#include <sstream>
#include <cstdlib>
#include <cctype>

// TODO: When the layer path issue is resolved with CI (set outside of the tests) remove this function
void layer_test::SetEnvironmentSetting(std::string setting, const char* val) {
#ifdef _WIN32
    _putenv_s(setting.c_str(), val);
#else
    setenv(setting.c_str(), val, 1);
#endif
}

void layer_test::UnsetEnvironmentSetting(std::string setting) {
#ifdef _WIN32
    _putenv_s(setting.c_str(), "");
#else
    unsetenv(setting.c_str());
#endif
}

std::string layer_test::GetAbsolutePath(std::string filepath) {
    std::string out;
#ifdef _WIN32
    char abs_path[_MAX_PATH];
    _fullpath(abs_path, filepath.c_str(), _MAX_PATH);
    out = abs_path;
#else
    char* abs_path = realpath(filepath.c_str(), nullptr);
    out = abs_path;
    delete[] abs_path;
#endif
    return out;
}

VkApplicationInfo layer_test::GetDefaultApplicationInfo() {
    VkApplicationInfo out{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    out.apiVersion = VK_API_VERSION_1_3;
    out.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    out.pApplicationName = "layer_tests";
    out.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    out.pEngineName = "layer_tests";

    return out;
}

VkResult layer_test::VulkanInstanceBuilder::GetPhysicalDevice(VkPhysicalDevice* phys_dev) {
    *phys_dev = VK_NULL_HANDLE;
    VkResult res;
    uint32_t gpu_count = 0;
    res = vkEnumeratePhysicalDevices(_instances, &gpu_count, nullptr);
    if (res != VK_SUCCESS) {
        return res;
    }
    std::vector<VkPhysicalDevice> gpus(gpu_count);
    res = vkEnumeratePhysicalDevices(_instances, &gpu_count, gpus.data());
    if (res != VK_SUCCESS) {
        return res;
    }
    *phys_dev = gpus[0];
    return res;
}

VkResult layer_test::VulkanInstanceBuilder::Init(const char* layer_name) {
    _layer_names.push_back(layer_name);
    return this->Init(std::vector<VkLayerSettingEXT>());
}

VkResult layer_test::VulkanInstanceBuilder::Init(const std::vector<VkLayerSettingEXT>& settings) {
    for (std::size_t i = 0, n = settings.size(); i < n; ++i) {
        if (std::find(_layer_names.begin(), _layer_names.end(), settings[i].pLayerName) == _layer_names.end()) {
            _layer_names.push_back(settings[i].pLayerName);
        }
    }

    layer_test::SetEnvironmentSetting("VK_LAYER_PATH", TEST_BINARY_PATH);

    VkApplicationInfo app_info{GetDefaultApplicationInfo()};

    const VkLayerSettingsCreateInfoEXT layer_settings_create_info{VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT, nullptr,
                                                                  settings.empty() ? 0 : static_cast<uint32_t>(settings.size()),
                                                                  settings.empty() ? nullptr : &settings[0]};

    VkInstanceCreateInfo inst_create_info = {};
    inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_create_info.pApplicationInfo = &app_info;
    inst_create_info.pNext = &layer_settings_create_info;
    inst_create_info.enabledLayerCount = static_cast<uint32_t>(_layer_names.size());
    inst_create_info.ppEnabledLayerNames = _layer_names.data();
    inst_create_info.enabledExtensionCount = _extension_names.empty() ? 0 : static_cast<uint32_t>(_extension_names.size());
    inst_create_info.ppEnabledExtensionNames = _extension_names.empty() ? nullptr : _extension_names.data();

    return vkCreateInstance(&inst_create_info, nullptr, &_instances);
}

void layer_test::VulkanInstanceBuilder::Reset() {
    if (_instances != VK_NULL_HANDLE) {
        vkDestroyInstance(_instances, nullptr);
        _instances = VK_NULL_HANDLE;
    }

    _layer_names.clear();
    _extension_names.clear();
}

bool layer_test::IsExtensionSupported(VkPhysicalDevice physical_device, const char* extension_name) {
    uint32_t property_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &property_count, nullptr);
    if (property_count == 0) return false;

    std::vector<VkExtensionProperties> properties(static_cast<std::size_t>(property_count));
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &property_count, &properties[0]);

    for (std::size_t i = 0, n = properties.size(); i < n; ++i) {
        if (strcmp(properties[i].extensionName, extension_name) == 0) {
            return true;
        }
    }

    return false;
}
