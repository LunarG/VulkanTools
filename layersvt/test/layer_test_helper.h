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

#pragma once

#include "layer_test_framework.h"

#include <vulkan/layer/vk_layer_settings.hpp>

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <cstdarg>

namespace layer_test {

inline std::string format(const char* message, ...) {
    std::size_t const max_string_length(4096);

    assert(message != nullptr);
    assert(std::strlen(message) >= 0 && std::strlen(message) < max_string_length);

    char buffer[max_string_length];
    va_list list;

    va_start(list, message);
    vsnprintf(buffer, max_string_length, message, list);
    va_end(list);

    return buffer;
}

void SetEnvironmentSetting(std::string setting, const char* val);
void UnsetEnvironmentSetting(std::string setting);

std::string GetAbsolutePath(std::string filepath);

VkApplicationInfo GetDefaultApplicationInfo();

bool IsExtensionSupported(VkPhysicalDevice physical_device, const char* extension_name);

class VulkanInstanceBuilder {
   public:
    ~VulkanInstanceBuilder() { this->Reset(); }

    void AddExtension(const char* extension_name) { _extension_names.push_back(extension_name); }

    VkResult Init(const char* layer_name);
    VkResult Init(const std::vector<VkLayerSettingEXT>& settings);
    VkResult GetPhysicalDevice(VkPhysicalDevice* phys_dev);

    void Reset();

    VkInstance GetInstance() { return _instances; }

   protected:
    VkInstance _instances;

    std::vector<const char*> _layer_names;
    std::vector<const char*> _extension_names;
};

}  // namespace layer_test
