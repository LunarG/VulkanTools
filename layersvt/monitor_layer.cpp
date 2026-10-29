/*
 * Vulkan
 *
 * Copyright (C) 2016-2022 Valve Corporation
 * Copyright (C) 2016-2026 LunarG, Inc.
 * Copyright (C) 2016-2022 Google Inc.
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
 *
 * Author: Chris Forbes <chrisforbes@google.com>
 * Author: Tony Barbour <tony@lunarg.com>
 */
#include "monitor_layer.h"

#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <initializer_list>
#include <cstdio>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include <vulkan/layer/vk_layer_settings.hpp>

#include "monitor_overlay.h"
#include "monitor_present.h"
#include "monitor_settings.h"
#include "vk_layer_table.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

#if defined(_WIN32) && !defined(NDEBUG)
#include <crtdbg.h>
#include <stdlib.h>  // NOLINT(modernize-deprecated-headers): Microsoft declares _set_abort_behavior here
#endif

namespace monitor {
namespace {

struct LayerData {
    std::unique_ptr<VkuDeviceDispatchTable> device_dispatch_table;
    std::unique_ptr<VkuInstanceDispatchTable> instance_dispatch_table;

    VkInstance instance{};
    VkPhysicalDevice gpu{};

    // Instance state for the overlay
    std::atomic<uint64_t> frame_counter{0};
    uint32_t api_version = VK_API_VERSION_1_0;  // From VkApplicationInfo
    MonitorSettings settings;

    // Device state for the overlay
    LayerData *instance_data{};
    DeviceOverlay overlay;
};

// Loaders without layer manifests, such as Android's, find the layer through these
constexpr VkLayerProperties kLayerProperties = {
    "VK_LAYER_LUNARG_monitor",
    VK_HEADER_VERSION_COMPLETE,
    2,
    "Execution Monitoring Layer",
};
static_assert(kLayerName == kLayerProperties.layerName);

constexpr std::array kInstanceExtensions = {
    VkExtensionProperties{VK_EXT_LAYER_SETTINGS_EXTENSION_NAME, VK_EXT_LAYER_SETTINGS_SPEC_VERSION},
};

// Must match the device_extensions of the layer manifest
constexpr std::array kDeviceExtensions = {
    VkExtensionProperties{VK_EXT_TOOLING_INFO_EXTENSION_NAME, VK_EXT_TOOLING_INFO_SPEC_VERSION},
};

// Instances and devices are created and destroyed from any thread, so the map is always locked.
std::mutex layer_data_lock;
// Constructed on first use, so a failed allocation happens in a Vulkan call rather than when the library loads
[[nodiscard]] std::unordered_map<void *, std::unique_ptr<LayerData>> &LayerDataMap() {
    static std::unordered_map<void *, std::unique_ptr<LayerData>> map;
    return map;
}

// The data for `key`, added the first time the key is seen
[[nodiscard]] LayerData *GetLayerData(void *key) {
    const std::scoped_lock lock(layer_data_lock);
    std::unique_ptr<LayerData> &data = LayerDataMap()[key];
    if (!data) {
        data = std::make_unique<LayerData>();
    }
    return data.get();
}

// For entry points the loader can call outside the dispatch chain, with handles the layer may never have seen.
// Unlike GetLayerData, an unknown key returns null instead of adding data that nothing would free.
[[nodiscard]] LayerData *FindLayerData(void *key) {
    const std::scoped_lock lock(layer_data_lock);
    const auto it = LayerDataMap().find(key);
    return it != LayerDataMap().end() ? it->second.get() : nullptr;
}

void FreeLayerData(void *key) {
    const std::scoped_lock lock(layer_data_lock);
    LayerDataMap().erase(key);
}

// Each distinct message is only shown once. Messages already shown cost a lookup, without allocating. The message only
// has to live for the call.
void WarnOnce(const char *message) {
    static std::mutex lock;
    static ShownMessages shown;
    const std::scoped_lock guard(lock);
    if (!FirstTimeShown(shown, message)) {
        return;
    }
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_WARN, "VK_LAYER_LUNARG_monitor", "%s", message);
#else
    (void)fprintf(stderr, "VK_LAYER_LUNARG_monitor: %s\n", message);
#endif
}

[[nodiscard]] bool HasExtension(const VkDeviceCreateInfo &create_info, std::string_view name) {
    for (uint32_t i = 0; i < create_info.enabledExtensionCount; ++i) {
        if (create_info.ppEnabledExtensionNames[i] == name) {
            return true;
        }
    }
    return false;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDevice(VkPhysicalDevice gpu, const VkDeviceCreateInfo *pCreateInfo,
                                            const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    // Physical devices share their instance's dispatch key
    LayerData *instance_data = GetLayerData(get_dispatch_key(gpu));
    auto fpCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(fpGetInstanceProcAddr(instance_data->instance, "vkCreateDevice"));
    if (fpCreateDevice == nullptr) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    const VkResult result = fpCreateDevice(gpu, pCreateInfo, pAllocator, pDevice);
    if (result != VK_SUCCESS) {
        return result;
    }

    LayerData *my_device_data = GetLayerData(get_dispatch_key(*pDevice));

    // Setup device dispatch table
    my_device_data->device_dispatch_table = std::make_unique<VkuDeviceDispatchTable>();
    vkuInitDeviceDispatchTable(*pDevice, my_device_data->device_dispatch_table.get(), fpGetDeviceProcAddr);

    // The loader callback gives the layer's command buffers their dispatch pointer
    chain_info = get_chain_info(pCreateInfo, VK_LOADER_DATA_CALLBACK);
    my_device_data->overlay.set_loader_data = chain_info ? chain_info->u.pfnSetDeviceLoaderData : nullptr;

    my_device_data->gpu = gpu;
    my_device_data->overlay.device = *pDevice;
    my_device_data->instance_data = instance_data;

    VkPhysicalDeviceProperties properties = {};
    instance_data->instance_dispatch_table->GetPhysicalDeviceProperties(gpu, &properties);
    const bool maintenance2 = HasExtension(*pCreateInfo, VK_KHR_MAINTENANCE_2_EXTENSION_NAME);
    my_device_data->overlay.view_usage = ImageViewUsageAvailable(instance_data->api_version, properties.apiVersion, maintenance2);

    uint32_t family_count = 0;
    instance_data->instance_dispatch_table->GetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
    my_device_data->overlay.queue_families.resize(family_count);
    instance_data->instance_dispatch_table->GetPhysicalDeviceQueueFamilyProperties(gpu, &family_count,
                                                                                   my_device_data->overlay.queue_families.data());
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(device);
    LayerData *my_data = GetLayerData(key);
    const VkuDeviceDispatchTable &dispatch = *my_data->device_dispatch_table;
    (void)dispatch.DeviceWaitIdle(device);
    DestroyDeviceOverlay(dispatch, my_data->overlay);
    dispatch.DestroyDevice(device, pAllocator);
    // Also frees the dispatch table
    FreeLayerData(key);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                              VkInstance *pInstance) {
#if defined(_WIN32) && defined(_CRTDBG_MODE_FILE)
#ifndef NDEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
    // Avoid "Abort, Retry, Ignore" dialog boxes
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    auto fpCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(fpGetInstanceProcAddr(nullptr, "vkCreateInstance"));
    if (fpCreateInstance == nullptr) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    const VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result != VK_SUCCESS) {
        return result;
    }

    LayerData *my_data = GetLayerData(get_dispatch_key(*pInstance));
    my_data->instance = *pInstance;
    if (pCreateInfo->pApplicationInfo && pCreateInfo->pApplicationInfo->apiVersion != 0) {
        my_data->api_version = pCreateInfo->pApplicationInfo->apiVersion;
    }
    my_data->instance_dispatch_table = std::make_unique<VkuInstanceDispatchTable>();
    vkuInitInstanceDispatchTable(*pInstance, my_data->instance_dispatch_table.get(), fpGetInstanceProcAddr);

    VkuLayerSettingSet layer_settings = VK_NULL_HANDLE;
    (void)vkuCreateLayerSettingSet(kLayerProperties.layerName, vkuFindLayerSettingsCreateInfo(pCreateInfo), pAllocator, nullptr,
                                   &layer_settings);
    my_data->settings = ParseSettings(ReadRawSettings(layer_settings));
    vkuDestroyLayerSettingSet(layer_settings, pAllocator);

    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    dispatch_key key = get_dispatch_key(instance);
    GetLayerData(key)->instance_dispatch_table->DestroyInstance(instance, pAllocator);
    // Also frees the dispatch table
    FreeLayerData(key);
}

VKAPI_ATTR VkResult VKAPI_CALL QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
    LayerData *my_data = GetLayerData(get_dispatch_key(queue));
    LayerData *instance_data = my_data->instance_data;
    const uint64_t frame_number = NextFrameNumber(instance_data->frame_counter);
    const auto now = std::chrono::steady_clock::now();
    return PresentWithOverlay(*my_data->device_dispatch_table, my_data->overlay, queue, *pPresentInfo, instance_data->settings,
                              frame_number, now, WarnIf(instance_data->settings.log_warnings, WarnOnce));
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t *pToolCount,
                                                                  VkPhysicalDeviceToolPropertiesEXT *pToolProperties) {
    constexpr VkPhysicalDeviceToolPropertiesEXT kTool = {
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES_EXT,
        nullptr,
        "Monitor Layer",
        "2",
        VK_TOOL_PURPOSE_PROFILING_BIT_EXT | VK_TOOL_PURPOSE_ADDITIONAL_FEATURES_BIT_EXT,
        "The VK_LAYER_LUNARG_monitor utility layer draws the frame rate, frame time and frame number on top of the presented "
        "images.",
        "VK_LAYER_LUNARG_monitor"};

    const LayerData *my_data = GetLayerData(get_dispatch_key(physicalDevice));
    return PrependToolProperties(kTool, physicalDevice, my_data->instance_dispatch_table->GetPhysicalDeviceToolPropertiesEXT,
                                 pToolCount, pToolProperties);
}

VKAPI_ATTR void VKAPI_CALL GetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue *pQueue) {
    LayerData *my_data = GetLayerData(get_dispatch_key(device));
    my_data->device_dispatch_table->GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);

    const std::scoped_lock lock(my_data->overlay.lock);
    my_data->overlay.queue_family_index[*pQueue] = queueFamilyIndex;
}

VKAPI_ATTR void VKAPI_CALL GetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2 *pQueueInfo, VkQueue *pQueue) {
    LayerData *my_data = GetLayerData(get_dispatch_key(device));
    my_data->device_dispatch_table->GetDeviceQueue2(device, pQueueInfo, pQueue);

    const std::scoped_lock lock(my_data->overlay.lock);
    my_data->overlay.queue_family_index[*pQueue] = pQueueInfo->queueFamilyIndex;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                                                  const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain) {
    LayerData *my_data = GetLayerData(get_dispatch_key(device));
    const VkuDeviceDispatchTable &dispatch = *my_data->device_dispatch_table;

    // The overlay renders into the swapchain images, so they need to be color attachments
    const SwapchainPatch patch = PatchSwapchainCreateInfo(*pCreateInfo, my_data->instance_data->settings.lines);
    const VkResult result = dispatch.CreateSwapchainKHR(device, &patch.create_info, pAllocator, pSwapchain);
    if (result != VK_SUCCESS) {
        return result;
    }

    VkFormatProperties format_properties = {};
    my_data->instance_data->instance_dispatch_table->GetPhysicalDeviceFormatProperties(my_data->gpu, patch.create_info.imageFormat,
                                                                                       &format_properties);
    const bool blend = (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT) != 0;

    const char *reason = AddSwapchainOverlay(dispatch, my_data->overlay, *pSwapchain, patch, blend);
    // Checked here rather than with WarnIf, so the message is only built when it's shown
    if (reason && my_data->instance_data->settings.log_warnings) {
        WarnOnce((std::string("no overlay on some swapchains, ") + reason).c_str());
    }
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks *pAllocator) {
    LayerData *my_data = GetLayerData(get_dispatch_key(device));
    const VkuDeviceDispatchTable &dispatch = *my_data->device_dispatch_table;
    ForgetSwapchain(dispatch, my_data->overlay, swapchain);
    dispatch.DestroySwapchainKHR(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount,
                                                         const VkSwapchainCreateInfoKHR *pCreateInfos,
                                                         const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchains) {
    const LayerData *my_data = GetLayerData(get_dispatch_key(device));
    if (const char *warning = SharedSwapchainsWarning(my_data->instance_data->settings.lines)) {
        WarnIf(my_data->instance_data->settings.log_warnings, WarnOnce)(warning);
    }
    return my_data->device_dispatch_table->CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}

template <typename Function>
[[nodiscard]] PFN_vkVoidFunction ToVoidFunction(Function *function) {
    return reinterpret_cast<PFN_vkVoidFunction>(function);
}

struct Hook {
    std::string_view name;
    PFN_vkVoidFunction function;
};

[[nodiscard]] PFN_vkVoidFunction Find(std::initializer_list<Hook> hooks, std::string_view name) {
    for (const Hook &hook : hooks) {
        if (hook.name == name) {
            return hook.function;
        }
    }
    return nullptr;
}

}  // namespace

VkResult PrependToolProperties(const VkPhysicalDeviceToolPropertiesEXT &tool, VkPhysicalDevice physical_device,
                               PFN_vkGetPhysicalDeviceToolPropertiesEXT next, uint32_t *pToolCount,
                               VkPhysicalDeviceToolPropertiesEXT *pToolProperties) {
    uint32_t next_count = 0;
    if (pToolProperties == nullptr) {
        const VkResult result = next ? next(physical_device, &next_count, nullptr) : VK_SUCCESS;
        *pToolCount = next_count + 1;
        return result;
    }
    if (*pToolCount == 0) {
        return VK_INCOMPLETE;
    }

    pToolProperties[0] = tool;
    VkResult result = VK_SUCCESS;
    if (next && *pToolCount > 1) {
        next_count = *pToolCount - 1;
        result = next(physical_device, &next_count, &pToolProperties[1]);
    } else if (next) {
        // No room for the next tools, so only find out whether there are any
        result = next(physical_device, &next_count, nullptr);
        if (result == VK_SUCCESS && next_count > 0) {
            result = VK_INCOMPLETE;
        }
        next_count = 0;
    }
    *pToolCount = next_count + 1;
    return result;
}

PFN_vkVoidFunction FindInstanceHook(std::string_view name) {
    return Find(
        {
            {"vkCreateInstance", ToVoidFunction(CreateInstance)},
            {"vkCreateDevice", ToVoidFunction(CreateDevice)},
            {"vkDestroyInstance", ToVoidFunction(DestroyInstance)},
            {"vkGetInstanceProcAddr", ToVoidFunction(GetInstanceProcAddr)},
            {"vkGetPhysicalDeviceToolPropertiesEXT", ToVoidFunction(GetPhysicalDeviceToolPropertiesEXT)},
            {"vkEnumerateInstanceLayerProperties", ToVoidFunction(EnumerateInstanceLayerProperties)},
            {"vkEnumerateDeviceLayerProperties", ToVoidFunction(EnumerateDeviceLayerProperties)},
            {"vkEnumerateInstanceExtensionProperties", ToVoidFunction(EnumerateInstanceExtensionProperties)},
            {"vkEnumerateDeviceExtensionProperties", ToVoidFunction(EnumerateDeviceExtensionProperties)},
        },
        name);
}

PFN_vkVoidFunction FindDeviceHook(std::string_view name, bool next_supported) {
    const PFN_vkVoidFunction core = Find(
        {
            {"vkGetDeviceProcAddr", ToVoidFunction(GetDeviceProcAddr)},
            {"vkDestroyDevice", ToVoidFunction(DestroyDevice)},
            {"vkGetDeviceQueue", ToVoidFunction(GetDeviceQueue)},
        },
        name);
    if (core || !next_supported) {
        return core;
    }
    return Find(
        {
            {"vkGetDeviceQueue2", ToVoidFunction(GetDeviceQueue2)},
            {"vkQueuePresentKHR", ToVoidFunction(QueuePresentKHR)},
            {"vkCreateSwapchainKHR", ToVoidFunction(CreateSwapchainKHR)},
            {"vkDestroySwapchainKHR", ToVoidFunction(DestroySwapchainKHR)},
            {"vkCreateSharedSwapchainsKHR", ToVoidFunction(CreateSharedSwapchainsKHR)},
        },
        name);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (const PFN_vkVoidFunction hook = FindInstanceHook(pName)) {
        return hook;
    }
    if (instance == VK_NULL_HANDLE) {
        return nullptr;
    }
    const VkuInstanceDispatchTable &dispatch = *GetLayerData(get_dispatch_key(instance))->instance_dispatch_table;
    if (dispatch.GetInstanceProcAddr == nullptr) {
        return nullptr;
    }
    return dispatch.GetInstanceProcAddr(instance, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(VkDevice device, const char *pName) {
    if (const PFN_vkVoidFunction hook = FindDeviceHook(pName, false)) {
        return hook;
    }
    if (device == VK_NULL_HANDLE) {
        return nullptr;
    }
    const VkuDeviceDispatchTable &dispatch = *GetLayerData(get_dispatch_key(device))->device_dispatch_table;
    if (dispatch.GetDeviceProcAddr == nullptr) {
        return nullptr;
    }
    const PFN_vkVoidFunction next = dispatch.GetDeviceProcAddr(device, pName);
    if (const PFN_vkVoidFunction hook = FindDeviceHook(pName, next != nullptr)) {
        return hook;
    }
    return next;
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceLayerProperties(uint32_t *pPropertyCount, VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &kLayerProperties, pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceLayerProperties(VkPhysicalDevice /*physicalDevice*/, uint32_t *pPropertyCount,
                                                              VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &kLayerProperties, pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount,
                                                                    VkExtensionProperties *pProperties) {
    if (pLayerName && pLayerName == kLayerName) {
        return util_GetExtensionProperties(static_cast<uint32_t>(kInstanceExtensions.size()), kInstanceExtensions.data(),
                                           pPropertyCount, pProperties);
    }
    return VK_ERROR_LAYER_NOT_PRESENT;
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char *pLayerName,
                                                                  uint32_t *pPropertyCount, VkExtensionProperties *pProperties) {
    if (pLayerName && pLayerName == kLayerName) {
        return util_GetExtensionProperties(static_cast<uint32_t>(kDeviceExtensions.size()), kDeviceExtensions.data(),
                                           pPropertyCount, pProperties);
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        return VK_ERROR_LAYER_NOT_PRESENT;
    }
    const LayerData *my_data = FindLayerData(get_dispatch_key(physicalDevice));
    if (!my_data || !my_data->instance_dispatch_table) {
        return VK_ERROR_LAYER_NOT_PRESENT;
    }
    return my_data->instance_dispatch_table->EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount,
                                                                                pProperties);
}

}  // namespace monitor
