/*
 * Copyright (c) 2026 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Tests what the layer's exported entry points return, without a Vulkan loader or driver.

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>

#include "monitor_layer.h"

namespace {

TEST(MonitorEntryPoints, InstanceHooks) {
    EXPECT_EQ(monitor::FindInstanceHook("vkGetInstanceProcAddr"),
              reinterpret_cast<PFN_vkVoidFunction>(monitor::GetInstanceProcAddr));
    EXPECT_EQ(monitor::FindInstanceHook("vkEnumerateInstanceLayerProperties"),
              reinterpret_cast<PFN_vkVoidFunction>(monitor::EnumerateInstanceLayerProperties));
    for (const std::string_view name :
         {"vkCreateInstance", "vkDestroyInstance", "vkCreateDevice", "vkGetPhysicalDeviceToolPropertiesEXT",
          "vkEnumerateDeviceLayerProperties", "vkEnumerateInstanceExtensionProperties", "vkEnumerateDeviceExtensionProperties"}) {
        EXPECT_NE(monitor::FindInstanceHook(name), nullptr) << name;
    }
}

TEST(MonitorEntryPoints, OnlyExactNamesAreHooked) {
    EXPECT_EQ(monitor::FindInstanceHook("vkCreateInstanceX"), nullptr);
    EXPECT_EQ(monitor::FindInstanceHook("vkCreateInst"), nullptr);
    EXPECT_EQ(monitor::FindInstanceHook(""), nullptr);
    EXPECT_EQ(monitor::FindDeviceHook("vkQueuePresentKHR2", true), nullptr);
}

// Device functions are only reached through vkGetDeviceProcAddr
TEST(MonitorEntryPoints, InstanceHooksLeaveDeviceFunctionsAlone) {
    EXPECT_EQ(monitor::FindInstanceHook("vkQueuePresentKHR"), nullptr);
    EXPECT_EQ(monitor::FindInstanceHook("vkCreateSwapchainKHR"), nullptr);
}

TEST(MonitorEntryPoints, CoreDeviceFunctionsAreAlwaysHooked) {
    EXPECT_EQ(monitor::FindDeviceHook("vkGetDeviceProcAddr", false),
              reinterpret_cast<PFN_vkVoidFunction>(monitor::GetDeviceProcAddr));
    EXPECT_NE(monitor::FindDeviceHook("vkDestroyDevice", false), nullptr);
    EXPECT_NE(monitor::FindDeviceHook("vkGetDeviceQueue", false), nullptr);
}

// Hooking a function the device doesn't have would hand the application a function that calls null
TEST(MonitorEntryPoints, ExtensionFunctionsAreOnlyHookedWhenTheDeviceHasThem) {
    for (const std::string_view name : {"vkGetDeviceQueue2", "vkQueuePresentKHR", "vkCreateSwapchainKHR", "vkDestroySwapchainKHR",
                                        "vkCreateSharedSwapchainsKHR"}) {
        EXPECT_EQ(monitor::FindDeviceHook(name, false), nullptr) << name;
        EXPECT_NE(monitor::FindDeviceHook(name, true), nullptr) << name;
    }
}

TEST(MonitorEntryPoints, ProcAddrWithoutAHandle) {
    EXPECT_EQ(monitor::GetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance"), monitor::FindInstanceHook("vkCreateInstance"));
    EXPECT_EQ(monitor::GetInstanceProcAddr(VK_NULL_HANDLE, "vkCmdDraw"), nullptr);
    EXPECT_EQ(monitor::GetDeviceProcAddr(VK_NULL_HANDLE, "vkDestroyDevice"), monitor::FindDeviceHook("vkDestroyDevice", false));
    EXPECT_EQ(monitor::GetDeviceProcAddr(VK_NULL_HANDLE, "vkQueuePresentKHR"), nullptr);
}

TEST(MonitorEntryPoints, LayerProperties) {
    uint32_t count = 0;
    EXPECT_EQ(monitor::EnumerateInstanceLayerProperties(&count, nullptr), VK_SUCCESS);
    ASSERT_EQ(count, 1u);
    VkLayerProperties properties = {};
    EXPECT_EQ(monitor::EnumerateInstanceLayerProperties(&count, &properties), VK_SUCCESS);
    EXPECT_EQ(properties.layerName, monitor::kLayerName);

    VkLayerProperties device_properties = {};
    EXPECT_EQ(monitor::EnumerateDeviceLayerProperties(VK_NULL_HANDLE, &count, &device_properties), VK_SUCCESS);
    EXPECT_EQ(device_properties.layerName, monitor::kLayerName);
}

TEST(MonitorEntryPoints, InstanceExtensions) {
    const std::string layer_name(monitor::kLayerName);
    uint32_t count = 0;
    EXPECT_EQ(monitor::EnumerateInstanceExtensionProperties(layer_name.c_str(), &count, nullptr), VK_SUCCESS);
    ASSERT_EQ(count, 1u);
    VkExtensionProperties extension = {};
    EXPECT_EQ(monitor::EnumerateInstanceExtensionProperties(layer_name.c_str(), &count, &extension), VK_SUCCESS);
    EXPECT_EQ(std::string_view(extension.extensionName), VK_EXT_LAYER_SETTINGS_EXTENSION_NAME);

    EXPECT_EQ(monitor::EnumerateInstanceExtensionProperties("VK_LAYER_KHRONOS_validation", &count, nullptr),
              VK_ERROR_LAYER_NOT_PRESENT);
    EXPECT_EQ(monitor::EnumerateInstanceExtensionProperties(nullptr, &count, nullptr), VK_ERROR_LAYER_NOT_PRESENT);
}

TEST(MonitorEntryPoints, DeviceExtensions) {
    const std::string layer_name(monitor::kLayerName);
    uint32_t count = 0;
    EXPECT_EQ(monitor::EnumerateDeviceExtensionProperties(VK_NULL_HANDLE, layer_name.c_str(), &count, nullptr), VK_SUCCESS);
    ASSERT_EQ(count, 1u);
    // The layer's vkGetPhysicalDeviceToolPropertiesEXT, which loaders without a layer manifest only find out about here
    VkExtensionProperties extension = {};
    EXPECT_EQ(monitor::EnumerateDeviceExtensionProperties(VK_NULL_HANDLE, layer_name.c_str(), &count, &extension), VK_SUCCESS);
    EXPECT_EQ(std::string_view(extension.extensionName), VK_EXT_TOOLING_INFO_EXTENSION_NAME);

    // Without a physical device there is no driver to ask
    EXPECT_EQ(monitor::EnumerateDeviceExtensionProperties(VK_NULL_HANDLE, nullptr, &count, nullptr), VK_ERROR_LAYER_NOT_PRESENT);
}

// The next layer or driver, with two tools
VKAPI_ATTR VkResult VKAPI_CALL NextTools(VkPhysicalDevice /*physical_device*/, uint32_t* count,
                                         VkPhysicalDeviceToolPropertiesEXT* tools) {
    constexpr uint32_t kNextTools = 2;
    if (tools == nullptr) {
        *count = kNextTools;
        return VK_SUCCESS;
    }
    const uint32_t written = std::min(*count, kNextTools);
    for (uint32_t i = 0; i < written; ++i) {
        tools[i] = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT};
        (void)snprintf(tools[i].name, sizeof(tools[i].name), "next %u", i);
    }
    *count = written;
    return written < kNextTools ? VK_INCOMPLETE : VK_SUCCESS;
}

VkPhysicalDeviceToolPropertiesEXT Tool(const char* name) {
    VkPhysicalDeviceToolPropertiesEXT tool = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT};
    (void)snprintf(tool.name, sizeof(tool.name), "%s", name);
    return tool;
}

TEST(MonitorEntryPoints, ToolCountIncludesTheLayer) {
    uint32_t count = 0;
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, NextTools, &count, nullptr), VK_SUCCESS);
    EXPECT_EQ(count, 3u);
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, nullptr, &count, nullptr), VK_SUCCESS);
    EXPECT_EQ(count, 1u);
}

TEST(MonitorEntryPoints, ToolsFitInTheApplicationsArray) {
    std::array<VkPhysicalDeviceToolPropertiesEXT, 3> tools = {Tool("unwritten"), Tool("unwritten"), Tool("unwritten")};

    // An empty array is valid, and has no room for any tool
    uint32_t count = 0;
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, NextTools, &count, tools.data()), VK_INCOMPLETE);
    EXPECT_EQ(count, 0u);
    EXPECT_EQ(std::string_view(tools[0].name), "unwritten");

    // Room for the layer's tool only
    count = 1;
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, NextTools, &count, tools.data()), VK_INCOMPLETE);
    EXPECT_EQ(count, 1u);
    EXPECT_EQ(std::string_view(tools[0].name), "monitor");
    EXPECT_EQ(std::string_view(tools[1].name), "unwritten");

    // Room for one of the next tools
    count = 2;
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, NextTools, &count, tools.data()), VK_INCOMPLETE);
    EXPECT_EQ(count, 2u);
    EXPECT_EQ(std::string_view(tools[1].name), "next 0");

    count = 3;
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, NextTools, &count, tools.data()), VK_SUCCESS);
    EXPECT_EQ(count, 3u);
    EXPECT_EQ(std::string_view(tools[0].name), "monitor");
    EXPECT_EQ(std::string_view(tools[2].name), "next 1");
}

TEST(MonitorEntryPoints, ToolsWithoutANextFunction) {
    VkPhysicalDeviceToolPropertiesEXT tool = Tool("unwritten");
    uint32_t count = 1;
    EXPECT_EQ(monitor::PrependToolProperties(Tool("monitor"), VK_NULL_HANDLE, nullptr, &count, &tool), VK_SUCCESS);
    EXPECT_EQ(count, 1u);
    EXPECT_EQ(std::string_view(tool.name), "monitor");
}

}  // namespace
