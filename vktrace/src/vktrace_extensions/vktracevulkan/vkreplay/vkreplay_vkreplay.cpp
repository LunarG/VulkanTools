/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * All Rights Reserved
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
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Tobin Ehlis <tobin@lunarg.com>
 */

#include "vulkan/vulkan.h"
#include "vkreplay_vkreplay.h"
#include "vkreplay.h"
#include "vkreplay_settings.h"

#include <algorithm>

#include "vktrace_vk_vk_packets.h"
#include "vk_enum_string_helper.h"

using namespace std;
#include "vktrace_pageguard_memorycopy.h"

vkreplayer_settings *g_pReplaySettings;

vkReplay::vkReplay(vkreplayer_settings *pReplaySettings)
{
    g_pReplaySettings = pReplaySettings;
    m_display = new vkDisplay();
    m_pDSDump = NULL;
    m_pCBDump = NULL;
//    m_pVktraceSnapshotPrint = NULL;
    m_objMapper.m_adjustForGPU = false;

    m_frameNumber = 0;
}

vkReplay::~vkReplay()
{
    delete m_display;
    vktrace_platform_close_library(m_vkFuncs.m_libHandle);
}

int vkReplay::init(vktrace_replay::ReplayDisplay & disp)
{
    int err;
#if defined PLATFORM_LINUX
    void * handle = dlopen("libvulkan.so", RTLD_LAZY);
#else
    HMODULE handle = LoadLibrary("vulkan-1.dll" );
#endif

    if (handle == NULL) {
        vktrace_LogError("Failed to open vulkan library.");
        return -1;
    }
    m_vkFuncs.init_funcs(handle);
    disp.set_implementation(m_display);
    if ((err = m_display->init(disp.get_gpu())) != 0) {
        vktrace_LogError("Failed to init vulkan display.");
        return err;
    }
    if (disp.get_window_handle() == 0)
    {
        if ((err = m_display->create_window(disp.get_width(), disp.get_height())) != 0) {
            vktrace_LogError("Failed to create Window");
            return err;
        }
    }
    else
    {
        if ((err = m_display->set_window(disp.get_window_handle(), disp.get_width(), disp.get_height())) != 0)
        {
            vktrace_LogError("Failed to set Window");
            return err;
        }
    }
    return 0;
}

vktrace_replay::VKTRACE_REPLAY_RESULT vkReplay::handle_replay_errors(const char* entrypointName, const VkResult resCall, const VkResult resTrace, const vktrace_replay::VKTRACE_REPLAY_RESULT resIn)
{
    vktrace_replay::VKTRACE_REPLAY_RESULT res = resIn;
    if (resCall != resTrace) {
        vktrace_LogError("Return value %s from API call (%s) does not match return value from trace file %s.",
                string_VkResult((VkResult)resCall), entrypointName, string_VkResult((VkResult)resTrace));
        res = vktrace_replay::VKTRACE_REPLAY_BAD_RETURN;
    }
    if (resCall != VK_SUCCESS  && resCall != VK_NOT_READY) {
        vktrace_LogWarning("API call (%s) returned failed result %s", entrypointName, string_VkResult(resCall));
    }
    return res;
}
void vkReplay::push_validation_msg(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObjectHandle, size_t location, int32_t msgCode, const char* pLayerPrefix, const char* pMsg, const void* pUserData)
{
    struct ValidationMsg msgObj;
    msgObj.msgFlags = msgFlags;
    msgObj.objType = objType;
    msgObj.srcObjectHandle = srcObjectHandle;
    msgObj.location = location;
    strncpy(msgObj.layerPrefix, pLayerPrefix, 256);
    msgObj.layerPrefix[255] = '\0';
    msgObj.msgCode = msgCode;
    strncpy(msgObj.msg, pMsg, 256);
    msgObj.msg[255] = '\0';
    msgObj.pUserData = (void *) pUserData;
    m_validationMsgs.push_back(msgObj);
}

vktrace_replay::VKTRACE_REPLAY_RESULT vkReplay::pop_validation_msgs()
{
    if (m_validationMsgs.size() == 0)
        return vktrace_replay::VKTRACE_REPLAY_SUCCESS;
    m_validationMsgs.clear();
    return vktrace_replay::VKTRACE_REPLAY_VALIDATION_ERROR;
}

int vkReplay::dump_validation_data()
{
    if  (m_pDSDump && m_pCBDump)
    {
        m_pDSDump((char *) "pipeline_dump.dot");
        m_pCBDump((char *) "cb_dump.dot");
    }
//    if (m_pVktraceSnapshotPrint != NULL)
//    {
//        m_pVktraceSnapshotPrint();
//    }
   return 0;
}

VkResult vkReplay::manually_replay_vkCreateInstance(packet_vkCreateInstance* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkInstanceCreateInfo *pCreateInfo;
    char **ppEnabledLayerNames = NULL, **saved_ppLayers;
    if (!m_display->m_initedVK)
    {
        VkInstance inst;

        const char strScreenShot[] = "VK_LAYER_LUNARG_screenshot";
        pCreateInfo = (VkInstanceCreateInfo *) pPacket->pCreateInfo;
        if (g_pReplaySettings->screenshotList != NULL) {
            // enable screenshot layer if it is available and not already in list
            bool found_ss = false;
            for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; i++) {
                if (!strcmp(pCreateInfo->ppEnabledLayerNames[i], strScreenShot)) {
                    found_ss = true;
                    break;
                }
            }
            if (!found_ss) {
                uint32_t count;

                // query to find if ScreenShot layer is available
                m_vkFuncs.real_vkEnumerateInstanceLayerProperties(&count, NULL);
                VkLayerProperties *props = (VkLayerProperties *) vktrace_malloc(count * sizeof (VkLayerProperties));
                if (props && count > 0)
                    m_vkFuncs.real_vkEnumerateInstanceLayerProperties(&count, props);
                for (uint32_t i = 0; i < count; i++) {
                    if (!strcmp(props[i].layerName, strScreenShot)) {
                        found_ss = true;
                        break;
                    }
                }
                if (found_ss) {
                    // screenshot layer is available so enable it
                    ppEnabledLayerNames = (char **) vktrace_malloc((pCreateInfo->enabledLayerCount + 1) * sizeof (char *));
                    for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount && ppEnabledLayerNames; i++) {
                        ppEnabledLayerNames[i] = (char *) pCreateInfo->ppEnabledLayerNames[i];
                    }
                    ppEnabledLayerNames[pCreateInfo->enabledLayerCount] = (char *) vktrace_malloc(strlen(strScreenShot) + 1);
                    strcpy(ppEnabledLayerNames[pCreateInfo->enabledLayerCount++], strScreenShot);
                    saved_ppLayers = (char **) pCreateInfo->ppEnabledLayerNames;
                    pCreateInfo->ppEnabledLayerNames = ppEnabledLayerNames;
                }
                vktrace_free(props);
            }
        }

        char **saved_ppExtensions = (char **)pCreateInfo->ppEnabledExtensionNames;
        int savedExtensionCount = pCreateInfo->enabledExtensionCount;
        vector<const char *> extension_names;
        vector<string> outlist;

#if defined PLATFORM_LINUX
        extension_names.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
        outlist.push_back("VK_KHR_win32_surface");
#else
        extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        outlist.push_back("VK_KHR_xlib_surface");
        outlist.push_back("VK_KHR_xcb_surface");
        outlist.push_back("VK_KHR_wayland_surface");
        outlist.push_back("VK_KHR_mir_surface");
#endif

        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            if ( find(outlist.begin(), outlist.end(), pCreateInfo->ppEnabledExtensionNames[i]) == outlist.end() ) {
                extension_names.push_back(pCreateInfo->ppEnabledExtensionNames[i]);
            }
        }
        pCreateInfo->ppEnabledExtensionNames = extension_names.data();
        pCreateInfo->enabledExtensionCount = (uint32_t)extension_names.size();

        replayResult = m_vkFuncs.real_vkCreateInstance(pPacket->pCreateInfo, NULL, &inst);

        pCreateInfo->ppEnabledExtensionNames = saved_ppExtensions;
        pCreateInfo->enabledExtensionCount = savedExtensionCount;

        if (ppEnabledLayerNames) {
            // restore the packets CreateInfo struct
            vktrace_free(ppEnabledLayerNames[pCreateInfo->enabledLayerCount - 1]);
            vktrace_free(ppEnabledLayerNames);
            pCreateInfo->ppEnabledLayerNames = saved_ppLayers;
        }

        if (replayResult == VK_SUCCESS) {
            m_objMapper.add_to_instances_map(*(pPacket->pInstance), inst);
        }
    }
    return replayResult;
}

bool vkReplay::getQueueFamilyIdx(VkPhysicalDevice tracePhysicalDevice,
                                 VkPhysicalDevice replayPhysicalDevice,
                                 uint32_t traceIdx,
                                 uint32_t* pReplayIdx)
{
    if  (traceIdx == VK_QUEUE_FAMILY_IGNORED)
    {
        *pReplayIdx = VK_QUEUE_FAMILY_IGNORED;
        return true;
    }

    if (traceQueueFamilyProperties.find(tracePhysicalDevice) == traceQueueFamilyProperties.end() ||
        replayQueueFamilyProperties.find(replayPhysicalDevice) == replayQueueFamilyProperties.end())
    {
        goto fail;
    }

    if (min(traceQueueFamilyProperties[tracePhysicalDevice].count, replayQueueFamilyProperties[replayPhysicalDevice].count) == 0)
    {
        goto fail;
    }

    for (uint32_t i = 0; i < min(traceQueueFamilyProperties[tracePhysicalDevice].count, replayQueueFamilyProperties[replayPhysicalDevice].count); i++)
    {
        if (traceQueueFamilyProperties[tracePhysicalDevice].queueFamilyProperties[traceIdx].queueFlags == replayQueueFamilyProperties[replayPhysicalDevice].queueFamilyProperties[i].queueFlags)
        {
            *pReplayIdx = i;
            return true;
        }
    }

    // Didn't find an exact match, search for a superset
    for (uint32_t i = 0; i < min(traceQueueFamilyProperties[tracePhysicalDevice].count, replayQueueFamilyProperties[replayPhysicalDevice].count); i++)
    {
        if (traceQueueFamilyProperties[tracePhysicalDevice].queueFamilyProperties[traceIdx].queueFlags ==
            (traceQueueFamilyProperties[tracePhysicalDevice].queueFamilyProperties[traceIdx].queueFlags & replayQueueFamilyProperties[replayPhysicalDevice].queueFamilyProperties[i].queueFlags))
        {
            *pReplayIdx = i;
            return true;
        }
    }

fail:
    vktrace_LogError("Cannot determine queue family index - has vkGetPhysicalDeviceQueueFamilyProperties been called?");
    // Didn't find a match
    return false;
}

bool vkReplay::getQueueFamilyIdx(VkDevice traceDevice,
                                 VkDevice replayDevice,
                                 uint32_t traceIdx,
                                 uint32_t* pReplayIdx)
{
    VkPhysicalDevice tracePhysicalDevice;
    VkPhysicalDevice replayPhysicalDevice;

    if (tracePhysicalDevices.find(traceDevice) == tracePhysicalDevices.end() ||
        replayPhysicalDevices.find(replayDevice) == replayPhysicalDevices.end())
    {
        vktrace_LogWarning("Cannot determine queue family index - has vkGetPhysicalDeviceQueueFamilyProperties been called?");
        return false;
    }

    tracePhysicalDevice = tracePhysicalDevices[traceDevice];
    replayPhysicalDevice = replayPhysicalDevices[replayDevice];

    return getQueueFamilyIdx(tracePhysicalDevice,
                             replayPhysicalDevice,
                             traceIdx,
                             pReplayIdx);
}

VkResult vkReplay::manually_replay_vkCreateDevice(packet_vkCreateDevice* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    if (!m_display->m_initedVK)
    {
        VkDevice device;
        VkPhysicalDevice remappedPhysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
        VkDeviceCreateInfo *pCreateInfo;
        char **ppEnabledLayerNames = NULL, **saved_ppLayers;
        if (remappedPhysicalDevice == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCreateDevice() due to invalid remapped VkPhysicalDevice.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        const char strScreenShot[] = "VK_LAYER_LUNARG_screenshot";
        //char *strScreenShotEnv = vktrace_get_global_var("_VK_SCREENSHOT");

        pCreateInfo = (VkDeviceCreateInfo *) pPacket->pCreateInfo;
        if (g_pReplaySettings->screenshotList != NULL) {
            // enable screenshot layer if it is available and not already in list
            bool found_ss = false;
            for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; i++) {
                if (!strcmp(pCreateInfo->ppEnabledLayerNames[i], strScreenShot)) {
                    found_ss = true;
                    break;
                }
            }
            if (!found_ss) {
                uint32_t count;

                // query to find if ScreenShot layer is available
                m_vkFuncs.real_vkEnumerateDeviceLayerProperties(remappedPhysicalDevice, &count, NULL);
                VkLayerProperties *props = (VkLayerProperties *) vktrace_malloc(count * sizeof (VkLayerProperties));
                if (props && count > 0)
                    m_vkFuncs.real_vkEnumerateDeviceLayerProperties(remappedPhysicalDevice, &count, props);
                for (uint32_t i = 0; i < count; i++) {
                    if (!strcmp(props[i].layerName, strScreenShot)) {
                        found_ss = true;
                        break;
                    }
                }
                if (found_ss) {
                    // screenshot layer is available so enable it
                    ppEnabledLayerNames = (char **) vktrace_malloc((pCreateInfo->enabledLayerCount+1) * sizeof(char *));
                    for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount && ppEnabledLayerNames; i++)
                    {
                        ppEnabledLayerNames[i] = (char *) pCreateInfo->ppEnabledLayerNames[i];
                    }
                    ppEnabledLayerNames[pCreateInfo->enabledLayerCount] = (char *) vktrace_malloc(strlen(strScreenShot) + 1);
                    strcpy(ppEnabledLayerNames[pCreateInfo->enabledLayerCount++], strScreenShot);
                    saved_ppLayers = (char **) pCreateInfo->ppEnabledLayerNames;
                    pCreateInfo->ppEnabledLayerNames = ppEnabledLayerNames;
                }
                vktrace_free(props);
            }
        }

        // Convert all instances of queueFamilyIndex in structure
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueCreateInfoCount; i++) {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueCreateInfos &&
                getQueueFamilyIdx(pPacket->physicalDevice,
                                  remappedPhysicalDevice,
                                  pPacket->pCreateInfo->pQueueCreateInfos->queueFamilyIndex,
                                  &replayIdx))
            {
                *((uint32_t *)&pPacket->pCreateInfo->pQueueCreateInfos->queueFamilyIndex) = replayIdx;
            }
            else
            {
                vktrace_LogError("vkCreateDevice failed, bad queueFamilyIndex");
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }

        replayResult = m_vkFuncs.real_vkCreateDevice(remappedPhysicalDevice, pPacket->pCreateInfo, NULL, &device);
        if (ppEnabledLayerNames)
        {
            // restore the packets CreateInfo struct
            vktrace_free(ppEnabledLayerNames[pCreateInfo->enabledLayerCount-1]);
            vktrace_free(ppEnabledLayerNames);
            pCreateInfo->ppEnabledLayerNames = saved_ppLayers;
        }
        if (replayResult == VK_SUCCESS)
        {
            m_objMapper.add_to_devices_map(*(pPacket->pDevice), device);
            tracePhysicalDevices[*(pPacket->pDevice)] = pPacket->physicalDevice;
            replayPhysicalDevices[device] = remappedPhysicalDevice;
        }
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateBuffer(packet_vkCreateBuffer* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    bufferObj local_bufferObj;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // Convert queueFamilyIndices
    if (pPacket->pCreateInfo)
    {
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueFamilyIndexCount; i++)
        {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueFamilyIndices &&
                getQueueFamilyIdx(pPacket->device,
                                  remappedDevice,
                                  pPacket->pCreateInfo->pQueueFamilyIndices[i],
                                  &replayIdx))
            {
                *((uint32_t*)&pPacket->pCreateInfo->pQueueFamilyIndices[i]) = replayIdx;
            } else {
                vktrace_LogError("vkCreateBuffer failed, bad queueFamilyIndex");
               return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
    }

    replayResult = m_vkFuncs.real_vkCreateBuffer(remappedDevice, pPacket->pCreateInfo, NULL, &local_bufferObj.replayBuffer);
    if (replayResult == VK_SUCCESS)
    {
        traceBufferToDevice[*pPacket->pBuffer] = pPacket->device;
        replayBufferToDevice[local_bufferObj.replayBuffer] = remappedDevice;
        m_objMapper.add_to_buffers_map(*(pPacket->pBuffer), local_bufferObj);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateImage(packet_vkCreateImage* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    imageObj local_imageObj;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // Convert queueFamilyIndices
    if (pPacket->pCreateInfo)
    {
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueFamilyIndexCount; i++)
        {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueFamilyIndices &&
                getQueueFamilyIdx(pPacket->device,
                                  remappedDevice,
                                  pPacket->pCreateInfo->pQueueFamilyIndices[i],
                                  &replayIdx))
            {
                *((uint32_t*)&pPacket->pCreateInfo->pQueueFamilyIndices[i]) = replayIdx;
            } else {
                vktrace_LogError("vkCreateImage failed, bad queueFamilyIndex");
               return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
    }

    replayResult = m_vkFuncs.real_vkCreateImage(remappedDevice, pPacket->pCreateInfo, NULL, &local_imageObj.replayImage);
    if (replayResult == VK_SUCCESS)
    {
        traceImageToDevice[*pPacket->pImage] = pPacket->device;
        replayImageToDevice[local_imageObj.replayImage] = remappedDevice;
        m_objMapper.add_to_images_map(*(pPacket->pImage), local_imageObj);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateCommandPool(packet_vkCreateCommandPool* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkCommandPool local_pCommandPool;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE)
    {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // No need to remap pAllocator

    // Convert queueFamilyIndex
    if (pPacket->pCreateInfo)
    {
        uint32_t replayIdx;
        if (getQueueFamilyIdx(pPacket->device,
                              remappeddevice,
                              pPacket->pCreateInfo->queueFamilyIndex,
                              &replayIdx))
        {
            *((uint32_t*)&pPacket->pCreateInfo->queueFamilyIndex) = replayIdx;
        } else {
            vktrace_LogError("vkCreateCommandPool failed, bad queueFamilyIndex");
           return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }

    replayResult = m_vkFuncs.real_vkCreateCommandPool(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator, &local_pCommandPool);
    if (replayResult == VK_SUCCESS)
    {
        m_objMapper.add_to_commandpools_map(*(pPacket->pCommandPool), local_pCommandPool);
    }
    return replayResult;
}


VkResult vkReplay::manually_replay_vkEnumeratePhysicalDevices(packet_vkEnumeratePhysicalDevices* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    if (!m_display->m_initedVK)
    {
        uint32_t deviceCount = *(pPacket->pPhysicalDeviceCount);
        VkPhysicalDevice *pDevices = pPacket->pPhysicalDevices;

        VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
        if (remappedInstance == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkEnumeratePhysicalDevices() due to invalid remapped VkInstance.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        if (pPacket->pPhysicalDevices != NULL)
            pDevices = VKTRACE_NEW_ARRAY(VkPhysicalDevice, deviceCount);
        replayResult = m_vkFuncs.real_vkEnumeratePhysicalDevices(remappedInstance, &deviceCount, pDevices);

        //TODO handle different number of physical devices in trace versus replay
        if (deviceCount != *(pPacket->pPhysicalDeviceCount))
        {
            vktrace_LogWarning("Number of physical devices mismatched in replay %u versus trace %u.", deviceCount, *(pPacket->pPhysicalDeviceCount));
        }
        else if (deviceCount == 0)
        {
             vktrace_LogError("vkEnumeratePhysicalDevices number of gpus is zero.");
        }
        else if (pDevices != NULL)
        {
            vktrace_LogVerbose("Enumerated %d physical devices in the system.", deviceCount);
        }
        // TODO handle enumeration results in a different order from trace to replay
        for (uint32_t i = 0; i < deviceCount; i++)
        {
            if (pDevices != NULL)
            {
                m_objMapper.add_to_physicaldevices_map(pPacket->pPhysicalDevices[i], pDevices[i]);
            }
        }
        VKTRACE_DELETE(pDevices);
    }
    return replayResult;
}

// TODO138 : Some of these functions have been renamed/changed in v138, need to scrub them and update as appropriate
//VkResult vkReplay::manually_replay_vkGetPhysicalDeviceInfo(packet_vkGetPhysicalDeviceInfo* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    if (!m_display->m_initedVK)
//    {
//        VkPhysicalDevice remappedPhysicalDevice = m_objMapper.remap(pPacket->physicalDevice);
//        if (remappedPhysicalDevice == VK_NULL_HANDLE)
//            return VK_ERROR_VALIDATION_FAILED_EXT;
//
//        switch (pPacket->infoType) {
//        case VK_PHYSICAL_DEVICE_INFO_TYPE_PROPERTIES:
//        {
//            VkPhysicalDeviceProperties deviceProps;
//            size_t dataSize = sizeof(VkPhysicalDeviceProperties);
//            replayResult = m_vkFuncs.real_vkGetPhysicalDeviceInfo(remappedPhysicalDevice, pPacket->infoType, &dataSize,
//                            (pPacket->pData == NULL) ? NULL : &deviceProps);
//            if (pPacket->pData != NULL)
//            {
//                vktrace_LogVerbose("Replay Physical Device Properties");
//                vktrace_LogVerbose("Vendor ID %x, Device ID %x, name %s", deviceProps.vendorId, deviceProps.deviceId, deviceProps.deviceName);
//                vktrace_LogVerbose("API version %u, Driver version %u, gpu Type %u", deviceProps.apiVersion, deviceProps.driverVersion, deviceProps.deviceType);
//                vktrace_LogVerbose("Max Descriptor Sets: %u", deviceProps.maxDescriptorSets);
//                vktrace_LogVerbose("Max Bound Descriptor Sets: %u", deviceProps.maxBoundDescriptorSets);
//                vktrace_LogVerbose("Max Thread Group Size: %u", deviceProps.maxThreadGroupSize);
//                vktrace_LogVerbose("Max Color Attachments: %u", deviceProps.maxColorAttachments);
//                vktrace_LogVerbose("Max Inline Memory Update Size: %llu", deviceProps.maxInlineMemoryUpdateSize);
//            }
//            break;
//        }
//        case VK_PHYSICAL_DEVICE_INFO_TYPE_PERFORMANCE:
//        {
//            VkPhysicalDevicePerformance devicePerfs;
//            size_t dataSize = sizeof(VkPhysicalDevicePerformance);
//            replayResult = m_vkFuncs.real_vkGetPhysicalDeviceInfo(remappedPhysicalDevice, pPacket->infoType, &dataSize,
//                            (pPacket->pData == NULL) ? NULL : &devicePerfs);
//            if (pPacket->pData != NULL)
//            {
//                vktrace_LogVerbose("Replay Physical Device Performance");
//                vktrace_LogVerbose("Max device clock %f, max shader ALUs/clock %f, max texel fetches/clock %f", devicePerfs.maxDeviceClock, devicePerfs.aluPerClock, devicePerfs.texPerClock);
//                vktrace_LogVerbose("Max primitives/clock %f, Max pixels/clock %f",devicePerfs.primsPerClock, devicePerfs.pixelsPerClock);
//            }
//            break;
//        }
//        case VK_PHYSICAL_DEVICE_INFO_TYPE_QUEUE_PROPERTIES:
//        {
//            VkPhysicalDeviceQueueProperties *pGpuQueue, *pQ;
//            size_t dataSize = sizeof(VkPhysicalDeviceQueueProperties);
//            size_t numQueues = 1;
//            assert(pPacket->pDataSize);
//            if ((*(pPacket->pDataSize) % dataSize) != 0)
//                vktrace_LogWarning("vkGetPhysicalDeviceInfo() for QUEUE_PROPERTIES not an integral data size assuming 1");
//            else
//                numQueues = *(pPacket->pDataSize) / dataSize;
//            dataSize = numQueues * dataSize;
//            pQ = static_cast < VkPhysicalDeviceQueueProperties *> (vktrace_malloc(dataSize));
//            pGpuQueue = pQ;
//            replayResult = m_vkFuncs.real_vkGetPhysicalDeviceInfo(remappedPhysicalDevice, pPacket->infoType, &dataSize,
//                            (pPacket->pData == NULL) ? NULL : pGpuQueue);
//            if (pPacket->pData != NULL)
//            {
//                for (unsigned int i = 0; i < numQueues; i++)
//                {
//                    vktrace_LogVerbose("Replay Physical Device Queue Property for index %d, flags %u.", i, pGpuQueue->queueFlags);
//                    vktrace_LogVerbose("Max available count %u, max atomic counters %u, supports timestamps %u.",pGpuQueue->queueCount, pGpuQueue->maxAtomicCounters, pGpuQueue->supportsTimestamps);
//                    pGpuQueue++;
//                }
//            }
//            vktrace_free(pQ);
//            break;
//        }
//        default:
//        {
//            size_t size = 0;
//            void* pData = NULL;
//            if (pPacket->pData != NULL && pPacket->pDataSize != NULL)
//            {
//                size = *pPacket->pDataSize;
//                pData = vktrace_malloc(*pPacket->pDataSize);
//            }
//            replayResult = m_vkFuncs.real_vkGetPhysicalDeviceInfo(remappedPhysicalDevice, pPacket->infoType, &size, pData);
//            if (replayResult == VK_SUCCESS)
//            {
///*                // TODO : We could pull this out into its own case of switch, and also may want to perform some
////                //   validation between the trace values and replay values
//                else*/ if (size != *pPacket->pDataSize && pData != NULL)
//                {
//                    vktrace_LogWarning("vkGetPhysicalDeviceInfo returned a differing data size: replay (%d bytes) vs trace (%d bytes)", size, *pPacket->pDataSize);
//                }
//                else if (pData != NULL && memcmp(pData, pPacket->pData, size) != 0)
//                {
//                    vktrace_LogWarning("vkGetPhysicalDeviceInfo returned differing data contents than the trace file contained.");
//                }
//            }
//            vktrace_free(pData);
//            break;
//        }
//        };
//    }
//    return replayResult;
//}

//VkResult vkReplay::manually_replay_vkGetGlobalExtensionInfo(packet_vkGetGlobalExtensionInfo* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    if (!m_display->m_initedVK) {
//        replayResult = m_vkFuncs.real_vkGetGlobalExtensionInfo(pPacket->infoType, pPacket->extensionIndex, pPacket->pDataSize, pPacket->pData);
//// TODO: Confirm that replay'd properties match with traced properties to ensure compatibility.
////        if (replayResult == VK_SUCCESS) {
////            for (unsigned int ext = 0; ext < sizeof(g_extensions) / sizeof(g_extensions[0]); ext++)
////            {
////                if (!strncmp(g_extensions[ext], pPacket->pExtName, strlen(g_extensions[ext]))) {
////                    bool extInList = false;
////                    for (unsigned int j = 0; j < m_display->m_extensions.size(); ++j) {
////                        if (!strncmp(m_display->m_extensions[j], g_extensions[ext], strlen(g_extensions[ext])))
////                            extInList = true;
////                        break;
////                    }
////                    if (!extInList)
////                        m_display->m_extensions.push_back((char *) g_extensions[ext]);
////                    break;
////                }
////            }
////        }
//    }
//    return replayResult;
//}

//VkResult vkReplay::manually_replay_vkGetPhysicalDeviceExtensionInfo(packet_vkGetPhysicalDeviceExtensionInfo* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    if (!m_display->m_initedVK) {
//        VkPhysicalDevice remappedPhysicalDevice = m_objMapper.remap(pPacket->physicalDevice);
//        if (remappedPhysicalDevice == VK_NULL_HANDLE)
//            return VK_ERROR_VALIDATION_FAILED_EXT;
//
//        replayResult = m_vkFuncs.real_vkGetPhysicalDeviceExtensionInfo(remappedPhysicalDevice, pPacket->infoType, pPacket->extensionIndex, pPacket->pDataSize, pPacket->pData);
//// TODO: Confirm that replay'd properties match with traced properties to ensure compatibility.
////        if (replayResult == VK_SUCCESS) {
////            for (unsigned int ext = 0; ext < sizeof(g_extensions) / sizeof(g_extensions[0]); ext++)
////            {
////                if (!strncmp(g_extensions[ext], pPacket->pExtName, strlen(g_extensions[ext]))) {
////                    bool extInList = false;
////                    for (unsigned int j = 0; j < m_display->m_extensions.size(); ++j) {
////                        if (!strncmp(m_display->m_extensions[j], g_extensions[ext], strlen(g_extensions[ext])))
////                            extInList = true;
////                        break;
////                    }
////                    if (!extInList)
////                        m_display->m_extensions.push_back((char *) g_extensions[ext]);
////                    break;
////                }
////            }
////        }
//    }
//    return replayResult;
//}

//VkResult vkReplay::manually_replay_vkGetSwapchainInfoWSI(packet_vkGetSwapchainInfoWSI* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    size_t dataSize = *pPacket->pDataSize;
//    void* pData = vktrace_malloc(dataSize);
//    VkSwapchainWSI remappedSwapchain = m_objMapper.remap_swapchainwsis(pPacket->swapchain);
//    if (remappedSwapchain == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkGetSwapchainInfoWSI() due to invalid remapped VkSwapchainWSI.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//    replayResult = m_vkFuncs.real_vkGetSwapchainInfoWSI(remappedSwapchain, pPacket->infoType, &dataSize, pData);
//    if (replayResult == VK_SUCCESS)
//    {
//        if (dataSize != *pPacket->pDataSize)
//        {
//            vktrace_LogWarning("SwapchainInfo dataSize differs between trace (%d bytes) and replay (%d bytes)", *pPacket->pDataSize, dataSize);
//        }
//        if (pPacket->infoType == VK_SWAP_CHAIN_INFO_TYPE_IMAGES_WSI)
//        {
//            VkSwapchainImageInfoWSI* pImageInfoReplay = (VkSwapchainImageInfoWSI*)pData;
//            VkSwapchainImageInfoWSI* pImageInfoTrace = (VkSwapchainImageInfoWSI*)pPacket->pData;
//            size_t imageCountReplay = dataSize / sizeof(VkSwapchainImageInfoWSI);
//            size_t imageCountTrace = *pPacket->pDataSize / sizeof(VkSwapchainImageInfoWSI);
//            for (size_t i = 0; i < imageCountReplay && i < imageCountTrace; i++)
//            {
//                imageObj imgObj;
//                imgObj.replayImage = pImageInfoReplay[i].image;
//                m_objMapper.add_to_map(&pImageInfoTrace[i].image, &imgObj);
//
//                gpuMemObj memObj;
//                memObj.replayGpuMem = pImageInfoReplay[i].memory;
//                m_objMapper.add_to_map(&pImageInfoTrace[i].memory, &memObj);
//            }
//        }
//    }
//    vktrace_free(pData);
//    return replayResult;
//}

VkResult vkReplay::manually_replay_vkQueueSubmit(packet_vkQueueSubmit* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkQueue remappedQueue = m_objMapper.remap_queues(pPacket->queue);
    if (remappedQueue == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped VkQueue.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFence remappedFence = m_objMapper.remap_fences(pPacket->fence);
    if (pPacket->fence != VK_NULL_HANDLE && remappedFence == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSubmitInfo *remappedSubmits = NULL;
    remappedSubmits = VKTRACE_NEW_ARRAY( VkSubmitInfo, pPacket->submitCount);
    VkCommandBuffer *pRemappedBuffers = NULL;
    VkSemaphore *pRemappedWaitSems = NULL, *pRemappedSignalSems = NULL;
    for (uint32_t submit_idx = 0; submit_idx < pPacket->submitCount; submit_idx++) {
        const VkSubmitInfo *submit = &pPacket->pSubmits[submit_idx];
        VkSubmitInfo *remappedSubmit = &remappedSubmits[submit_idx];
        memset(remappedSubmit, 0, sizeof(VkSubmitInfo));
        remappedSubmit->sType = submit->sType;
        remappedSubmit->pNext = submit->pNext;
        remappedSubmit->pWaitDstStageMask = submit->pWaitDstStageMask;
        // Remap Semaphores & CommandBuffers for this submit
        uint32_t i = 0;
        if (submit->pCommandBuffers != NULL) {
            pRemappedBuffers = VKTRACE_NEW_ARRAY( VkCommandBuffer, submit->commandBufferCount);
            remappedSubmit->pCommandBuffers = pRemappedBuffers;
            remappedSubmit->commandBufferCount = submit->commandBufferCount;
            for (i = 0; i < submit->commandBufferCount; i++) {
                *(pRemappedBuffers + i) = m_objMapper.remap_commandbuffers(*(submit->pCommandBuffers + i));
                if (*(pRemappedBuffers + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped VkCommandBuffer.");
                    VKTRACE_DELETE(remappedSubmits);
                    VKTRACE_DELETE(pRemappedBuffers);
                    return replayResult;
                }
            }
        }
        if (submit->pWaitSemaphores != NULL) {
            pRemappedWaitSems = VKTRACE_NEW_ARRAY(VkSemaphore, submit->waitSemaphoreCount);
            remappedSubmit->pWaitSemaphores = pRemappedWaitSems;
            remappedSubmit->waitSemaphoreCount = submit->waitSemaphoreCount;
            for (i = 0; i < submit->waitSemaphoreCount; i++) {
                (*(pRemappedWaitSems + i)) = m_objMapper.remap_semaphores((*(submit->pWaitSemaphores + i)));
                if (*(pRemappedWaitSems + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped wait VkSemaphore.");
                    VKTRACE_DELETE(remappedSubmits);
                    VKTRACE_DELETE(pRemappedBuffers);
                    VKTRACE_DELETE(pRemappedWaitSems);
                    return replayResult;
                }
            }
        }
        if (submit->pSignalSemaphores != NULL) {
            pRemappedSignalSems = VKTRACE_NEW_ARRAY(VkSemaphore, submit->signalSemaphoreCount);
            remappedSubmit->pSignalSemaphores = pRemappedSignalSems;
            remappedSubmit->signalSemaphoreCount = submit->signalSemaphoreCount;
            for (i = 0; i < submit->signalSemaphoreCount; i++) {
                (*(pRemappedSignalSems + i)) = m_objMapper.remap_semaphores((*(submit->pSignalSemaphores + i)));
                if (*(pRemappedSignalSems + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped signal VkSemaphore.");
                    VKTRACE_DELETE(remappedSubmits);
                    VKTRACE_DELETE(pRemappedBuffers);
                    VKTRACE_DELETE(pRemappedWaitSems);
                    VKTRACE_DELETE(pRemappedSignalSems);
                    return replayResult;
                }
            }
        }
    }
    replayResult = m_vkFuncs.real_vkQueueSubmit(remappedQueue,
                                                pPacket->submitCount,
                                                remappedSubmits,
                                                remappedFence);
    VKTRACE_DELETE(pRemappedBuffers);
    VKTRACE_DELETE(pRemappedWaitSems);
    VKTRACE_DELETE(pRemappedSignalSems);
    VKTRACE_DELETE(remappedSubmits);
    return replayResult;
}

VkResult vkReplay::manually_replay_vkQueueBindSparse(packet_vkQueueBindSparse* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkQueue remappedQueue = m_objMapper.remap_queues(pPacket->queue);
    if (remappedQueue == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkQueue.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFence remappedFence = m_objMapper.remap_fences(pPacket->fence);
    if (pPacket->fence != VK_NULL_HANDLE && remappedFence == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkBindSparseInfo* remappedBindSparseInfos = VKTRACE_NEW_ARRAY(VkBindSparseInfo, pPacket->bindInfoCount);
    VkSparseImageMemoryBind *pRemappedImageMemories = NULL;
    VkSparseMemoryBind *pRemappedBufferMemories = NULL;
    VkSparseMemoryBind *pRemappedImageOpaqueMemories = NULL;
    VkSemaphore *pRemappedWaitSems = NULL;
    VkSemaphore *pRemappedSignalSems = NULL;
    VkSparseImageMemoryBindInfo* sIMBinf = NULL;
    VkSparseBufferMemoryBindInfo* sBMBinf = NULL;
    VkSparseImageOpaqueMemoryBindInfo* sIMOBinf = NULL;

    memcpy((void*)remappedBindSparseInfos, (void*)(pPacket->pBindInfo), sizeof(VkBindSparseInfo)*pPacket->bindInfoCount);

    for (uint32_t bindInfo_idx = 0; bindInfo_idx < pPacket->bindInfoCount; bindInfo_idx++) {
        if (remappedBindSparseInfos[bindInfo_idx].pBufferBinds)
        {
            sBMBinf = VKTRACE_NEW_ARRAY(VkSparseBufferMemoryBindInfo, remappedBindSparseInfos[bindInfo_idx].bufferBindCount);
            remappedBindSparseInfos[bindInfo_idx].pBufferBinds = (const VkSparseBufferMemoryBindInfo*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pBufferBinds));
            memcpy((void*)sBMBinf, (void*)remappedBindSparseInfos[bindInfo_idx].pBufferBinds, sizeof(VkSparseBufferMemoryBindInfo)*remappedBindSparseInfos[bindInfo_idx].bufferBindCount);

            sBMBinf->buffer = m_objMapper.remap_buffers(sBMBinf->buffer);

            if (sBMBinf->buffer == VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkBuffer.");
                goto FAILURE;
            }

            if (sBMBinf->bindCount > 0 && sBMBinf->pBinds)
            {
                pRemappedBufferMemories = (VkSparseMemoryBind*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pBufferBinds->pBinds));
            }

            for (uint32_t bindCountIdx = 0; bindCountIdx < sBMBinf->bindCount; bindCountIdx++)
            {
                gpuMemObj local_mem = m_objMapper.m_devicememorys.find(pRemappedBufferMemories[bindCountIdx].memory)->second;
                VkDeviceMemory replay_mem = m_objMapper.remap_devicememorys(pRemappedBufferMemories[bindCountIdx].memory);

                if (replay_mem == VK_NULL_HANDLE || local_mem.pGpuMem == NULL)
                {
                    vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkDeviceMemory.");
                    goto FAILURE;
                }
                pRemappedBufferMemories[bindCountIdx].memory = replay_mem;
            }
            sBMBinf->pBinds = pRemappedBufferMemories;
            remappedBindSparseInfos[bindInfo_idx].pBufferBinds = sBMBinf;
        }

        if (remappedBindSparseInfos[bindInfo_idx].pImageBinds)
        {
            sIMBinf = VKTRACE_NEW_ARRAY(VkSparseImageMemoryBindInfo, remappedBindSparseInfos[bindInfo_idx].imageBindCount);
            remappedBindSparseInfos[bindInfo_idx].pImageBinds = (const VkSparseImageMemoryBindInfo*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageBinds));
            memcpy((void*)sIMBinf, (void*)remappedBindSparseInfos[bindInfo_idx].pImageBinds, sizeof(VkSparseImageMemoryBindInfo)*remappedBindSparseInfos[bindInfo_idx].imageBindCount);

            sIMBinf->image = m_objMapper.remap_images(sIMBinf->image);

            if(sIMBinf->image == VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkImage.");
                goto FAILURE;
            }

            if (sIMBinf->bindCount > 0 && sIMBinf->pBinds)
            {
                pRemappedImageMemories = (VkSparseImageMemoryBind*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageBinds->pBinds));
            }
            for (uint32_t bindCountIdx = 0; bindCountIdx < sIMBinf->bindCount; bindCountIdx++)
            {
                gpuMemObj local_mem = m_objMapper.m_devicememorys.find(pRemappedImageMemories[bindCountIdx].memory)->second;
                VkDeviceMemory replay_mem = m_objMapper.remap_devicememorys(pRemappedImageMemories[bindCountIdx].memory);

                if (replay_mem == VK_NULL_HANDLE || local_mem.pGpuMem == NULL)
                {
                    vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkDeviceMemory.");
                    goto FAILURE;
                }
                pRemappedImageMemories[bindCountIdx].memory = replay_mem;
            }
            sIMBinf->pBinds = pRemappedImageMemories;
            remappedBindSparseInfos[bindInfo_idx].pImageBinds = sIMBinf;
        }

        if (remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds)
        {
            sIMOBinf = VKTRACE_NEW_ARRAY(VkSparseImageOpaqueMemoryBindInfo, remappedBindSparseInfos[bindInfo_idx].imageOpaqueBindCount);
            remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds = (const VkSparseImageOpaqueMemoryBindInfo*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds));
            memcpy((void*)sIMOBinf, (void*)remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds, sizeof(VkSparseImageOpaqueMemoryBindInfo)*remappedBindSparseInfos[bindInfo_idx].imageOpaqueBindCount);

            sIMOBinf->image = m_objMapper.remap_images(sIMOBinf->image);

            if (sIMOBinf->image == VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkImage.");
                goto FAILURE;
            }

            if (sIMOBinf->bindCount > 0 && sIMOBinf->pBinds)
            {
                pRemappedImageOpaqueMemories = (VkSparseMemoryBind*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds->pBinds));
            }
            for (uint32_t bindCountIdx = 0; bindCountIdx < sIMOBinf->bindCount; bindCountIdx++)
            {
                gpuMemObj local_mem = m_objMapper.m_devicememorys.find(pRemappedImageOpaqueMemories[bindCountIdx].memory)->second;
                VkDeviceMemory replay_mem = m_objMapper.remap_devicememorys(pRemappedImageOpaqueMemories[bindCountIdx].memory);

                if (replay_mem == VK_NULL_HANDLE || local_mem.pGpuMem == NULL)
                {
                    vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkDeviceMemory.");
                    goto FAILURE;
                }
                pRemappedImageOpaqueMemories[bindCountIdx].memory = replay_mem;
            }
            sIMOBinf->pBinds = pRemappedImageOpaqueMemories;
            remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds = sIMOBinf;
        }

        if (remappedBindSparseInfos[bindInfo_idx].pWaitSemaphores != NULL) {
            pRemappedWaitSems = VKTRACE_NEW_ARRAY(VkSemaphore, remappedBindSparseInfos[bindInfo_idx].waitSemaphoreCount);
            remappedBindSparseInfos[bindInfo_idx].pWaitSemaphores = pRemappedWaitSems;
            for (uint32_t i = 0; i < remappedBindSparseInfos[bindInfo_idx].waitSemaphoreCount; i++) {
                (*(pRemappedWaitSems + i)) = m_objMapper.remap_semaphores((*(remappedBindSparseInfos[bindInfo_idx].pWaitSemaphores + i)));
                if (*(pRemappedWaitSems + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped wait VkSemaphore.");
                    goto FAILURE;
                }
            }
        }
        if (remappedBindSparseInfos[bindInfo_idx].pSignalSemaphores != NULL) {
            pRemappedSignalSems = VKTRACE_NEW_ARRAY(VkSemaphore, remappedBindSparseInfos[bindInfo_idx].signalSemaphoreCount);
            remappedBindSparseInfos[bindInfo_idx].pSignalSemaphores = pRemappedSignalSems;
            for (uint32_t i = 0; i < remappedBindSparseInfos[bindInfo_idx].signalSemaphoreCount; i++) {
                (*(pRemappedSignalSems + i)) = m_objMapper.remap_semaphores((*(remappedBindSparseInfos[bindInfo_idx].pSignalSemaphores + i)));
                if (*(pRemappedSignalSems + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped signal VkSemaphore.");
                    goto FAILURE;
                }
            }
        }
    }

    replayResult = m_vkFuncs.real_vkQueueBindSparse(remappedQueue,
        pPacket->bindInfoCount,
        remappedBindSparseInfos,
        remappedFence);

FAILURE:
    VKTRACE_DELETE(remappedBindSparseInfos);
    VKTRACE_DELETE(sIMBinf);
    VKTRACE_DELETE(sBMBinf);
    VKTRACE_DELETE(sIMOBinf);
    VKTRACE_DELETE(pRemappedSignalSems);
    VKTRACE_DELETE(pRemappedWaitSems);
    return replayResult;
}


//VkResult vkReplay::manually_replay_vkGetObjectInfo(packet_vkGetObjectInfo* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
//    if (remappedDevice == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkGetObjectInfo() due to invalid remapped VkDevice.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    VkObject remappedObject = m_objMapper.remap(pPacket->object, pPacket->objType);
//    if (remappedObject == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkGetObjectInfo() due to invalid remapped VkObject.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    size_t size = 0;
//    void* pData = NULL;
//    if (pPacket->pData != NULL && pPacket->pDataSize != NULL)
//    {
//        size = *pPacket->pDataSize;
//        pData = vktrace_malloc(*pPacket->pDataSize);
//        memcpy(pData, pPacket->pData, *pPacket->pDataSize);
//    }
//    // TODO only search for object once rather than at remap() and init_objMemXXX()
//    replayResult = m_vkFuncs.real_vkGetObjectInfo(remappedDevice, pPacket->objType, remappedObject, pPacket->infoType, &size, pData);
//    if (replayResult == VK_SUCCESS)
//    {
//        if (size != *pPacket->pDataSize && pData != NULL)
//        {
//            vktrace_LogWarning("vkGetObjectInfo returned a differing data size: replay (%d bytes) vs trace (%d bytes).", size, *pPacket->pDataSize);
//        }
//        else if (pData != NULL)
//        {
//            switch (pPacket->infoType)
//            {
//                case VK_OBJECT_INFO_TYPE_MEMORY_REQUIREMENTS:
//                {
//                    VkMemoryRequirements *traceReqs = (VkMemoryRequirements *) pPacket->pData;
//                    VkMemoryRequirements *replayReqs = (VkMemoryRequirements *) pData;
//                    size_t num = size / sizeof(VkMemoryRequirements);
//                    for (unsigned int i = 0; i < num; i++)
//                    {
//                        if (traceReqs->size != replayReqs->size)
//                            vktrace_LogWarning("vkGetObjectInfo(INFO_TYPE_MEMORY_REQUIREMENTS) mismatch: trace size %u, replay size %u.", traceReqs->size, replayReqs->size);
//                        if (traceReqs->alignment != replayReqs->alignment)
//                            vktrace_LogWarning("vkGetObjectInfo(INFO_TYPE_MEMORY_REQUIREMENTS) mismatch: trace alignment %u, replay aligmnent %u.", traceReqs->alignment, replayReqs->alignment);
//                        if (traceReqs->granularity != replayReqs->granularity)
//                            vktrace_LogWarning("vkGetObjectInfo(INFO_TYPE_MEMORY_REQUIREMENTS) mismatch: trace granularity %u, replay granularity %u.", traceReqs->granularity, replayReqs->granularity);
//                        if (traceReqs->memPropsAllowed != replayReqs->memPropsAllowed)
//                            vktrace_LogWarning("vkGetObjectInfo(INFO_TYPE_MEMORY_REQUIREMENTS) mismatch: trace memPropsAllowed %u, replay memPropsAllowed %u.", traceReqs->memPropsAllowed, replayReqs->memPropsAllowed);
//                        if (traceReqs->memPropsRequired != replayReqs->memPropsRequired)
//                            vktrace_LogWarning("vkGetObjectInfo(INFO_TYPE_MEMORY_REQUIREMENTS) mismatch: trace memPropsRequired %u, replay memPropsRequired %u.", traceReqs->memPropsRequired, replayReqs->memPropsRequired);
//                        traceReqs++;
//                        replayReqs++;
//                    }
//                    if (m_objMapper.m_adjustForGPU)
//                        m_objMapper.init_objMemReqs(pPacket->object, replayReqs - num, num);
//                    break;
//                }
//                default:
//                    if (memcmp(pData, pPacket->pData, size) != 0)
//                        vktrace_LogWarning("vkGetObjectInfo() mismatch on *pData: between trace and replay *pDataSize %u.", size);
//            }
//        }
//    }
//    vktrace_free(pData);
//    return replayResult;
//}

//VkResult vkReplay::manually_replay_vkGetImageSubresourceInfo(packet_vkGetImageSubresourceInfo* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
//    if (remappedDevice == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkGetImageSubresourceInfo() due to invalid remapped VkDevice.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    VkImage remappedImage = m_objMapper.remap(pPacket->image);
//    if (remappedImage == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkGetImageSubresourceInfo() due to invalid remapped VkImage.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    size_t size = 0;
//    void* pData = NULL;
//    if (pPacket->pData != NULL && pPacket->pDataSize != NULL)
//    {
//        size = *pPacket->pDataSize;
//        pData = vktrace_malloc(*pPacket->pDataSize);
//    }
//    replayResult = m_vkFuncs.real_vkGetImageSubresourceInfo(remappedDevice, remappedImage, pPacket->pSubresource, pPacket->infoType, &size, pData);
//    if (replayResult == VK_SUCCESS)
//    {
//        if (size != *pPacket->pDataSize && pData != NULL)
//        {
//            vktrace_LogWarning("vkGetImageSubresourceInfo returned a differing data size: replay (%d bytes) vs trace (%d bytes).", size, *pPacket->pDataSize);
//        }
//        else if (pData != NULL && memcmp(pData, pPacket->pData, size) != 0)
//        {
//            vktrace_LogWarning("vkGetImageSubresourceInfo returned differing data contents than the trace file contained.");
//        }
//    }
//    vktrace_free(pData);
//    return replayResult;
//}

void vkReplay::manually_replay_vkUpdateDescriptorSets(packet_vkUpdateDescriptorSets* pPacket)
{
    // We have to remap handles internal to the structures so save the handles prior to remap and then restore
    // Rather than doing a deep memcpy of the entire struct and fixing any intermediate pointers, do save and restores via STL queue

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkDevice.");
        return;
    }

    VkWriteDescriptorSet* pRemappedWrites = VKTRACE_NEW_ARRAY(VkWriteDescriptorSet, pPacket->descriptorWriteCount);
    memcpy(pRemappedWrites, pPacket->pDescriptorWrites, pPacket->descriptorWriteCount * sizeof(VkWriteDescriptorSet));

    VkCopyDescriptorSet* pRemappedCopies = VKTRACE_NEW_ARRAY(VkCopyDescriptorSet, pPacket->descriptorCopyCount);
    memcpy(pRemappedCopies, pPacket->pDescriptorCopies, pPacket->descriptorCopyCount * sizeof(VkCopyDescriptorSet));

    bool errorBadRemap = false;

    for (uint32_t i = 0; i < pPacket->descriptorWriteCount && !errorBadRemap; i++)
    {
        pRemappedWrites[i].dstSet = m_objMapper.remap_descriptorsets(pPacket->pDescriptorWrites[i].dstSet);
        if (pRemappedWrites[i].dstSet == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped write VkDescriptorSet.");
            errorBadRemap = true;
            break;
        }

        switch (pPacket->pDescriptorWrites[i].descriptorType) {
        case VK_DESCRIPTOR_TYPE_SAMPLER:
            pRemappedWrites[i].pImageInfo = VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
            memcpy((void*)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
            for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++)
            {
                if (pPacket->pDescriptorWrites[i].pImageInfo[j].sampler != VK_NULL_HANDLE)
                {
                    const_cast<VkDescriptorImageInfo*>(pRemappedWrites[i].pImageInfo)[j].sampler = m_objMapper.remap_samplers(pPacket->pDescriptorWrites[i].pImageInfo[j].sampler);
                    if (pRemappedWrites[i].pImageInfo[j].sampler == VK_NULL_HANDLE)
                    {
                        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkSampler.");
                        errorBadRemap = true;
                        break;
                    }
                }
            }
            break;
        case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
        case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
        case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
            pRemappedWrites[i].pImageInfo = VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
            memcpy((void*)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
            for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++)
            {
                if (pPacket->pDescriptorWrites[i].pImageInfo[j].imageView != VK_NULL_HANDLE)
                {
                    const_cast<VkDescriptorImageInfo*>(pRemappedWrites[i].pImageInfo)[j].imageView = m_objMapper.remap_imageviews(pPacket->pDescriptorWrites[i].pImageInfo[j].imageView);
                    if (pRemappedWrites[i].pImageInfo[j].imageView == VK_NULL_HANDLE)
                    {
                        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkImageView.");
                        errorBadRemap = true;
                        break;
                    }
                }
            }
            break;
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            pRemappedWrites[i].pImageInfo = VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
            memcpy((void*)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
            for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++)
            {
                if (pPacket->pDescriptorWrites[i].pImageInfo[j].sampler != VK_NULL_HANDLE)
                {
                    const_cast<VkDescriptorImageInfo*>(pRemappedWrites[i].pImageInfo)[j].sampler = m_objMapper.remap_samplers(pPacket->pDescriptorWrites[i].pImageInfo[j].sampler);
                    if (pRemappedWrites[i].pImageInfo[j].sampler == VK_NULL_HANDLE)
                    {
                        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkSampler.");
                        errorBadRemap = true;
                        break;
                    }
                }
                if (pPacket->pDescriptorWrites[i].pImageInfo[j].imageView != VK_NULL_HANDLE)
                {
                    const_cast<VkDescriptorImageInfo*>(pRemappedWrites[i].pImageInfo)[j].imageView = m_objMapper.remap_imageviews(pPacket->pDescriptorWrites[i].pImageInfo[j].imageView);
                    if (pRemappedWrites[i].pImageInfo[j].imageView == VK_NULL_HANDLE)
                    {
                        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkImageView.");
                        errorBadRemap = true;
                        break;
                    }
                }
            }
            break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
            pRemappedWrites[i].pTexelBufferView = VKTRACE_NEW_ARRAY(VkBufferView, pPacket->pDescriptorWrites[i].descriptorCount);
            memcpy((void*)pRemappedWrites[i].pTexelBufferView, pPacket->pDescriptorWrites[i].pTexelBufferView, pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkBufferView));
            for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++)
            {
                if (pPacket->pDescriptorWrites[i].pTexelBufferView[j] != VK_NULL_HANDLE)
                {
                    const_cast<VkBufferView*>(pRemappedWrites[i].pTexelBufferView)[j] = m_objMapper.remap_bufferviews(pPacket->pDescriptorWrites[i].pTexelBufferView[j]);
                    if (pRemappedWrites[i].pTexelBufferView[j] == VK_NULL_HANDLE)
                    {
                        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkBufferView.");
                        errorBadRemap = true;
                        break;
                    }
                }
            }
            break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
            pRemappedWrites[i].pBufferInfo = VKTRACE_NEW_ARRAY(VkDescriptorBufferInfo, pPacket->pDescriptorWrites[i].descriptorCount);
            memcpy((void*)pRemappedWrites[i].pBufferInfo, pPacket->pDescriptorWrites[i].pBufferInfo, pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorBufferInfo));
            for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++)
            {
                if (pPacket->pDescriptorWrites[i].pBufferInfo[j].buffer != VK_NULL_HANDLE)
                {
                    const_cast<VkDescriptorBufferInfo*>(pRemappedWrites[i].pBufferInfo)[j].buffer = m_objMapper.remap_buffers(pPacket->pDescriptorWrites[i].pBufferInfo[j].buffer);
                    if (pRemappedWrites[i].pBufferInfo[j].buffer == VK_NULL_HANDLE)
                    {
                        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkBufferView.");
                        errorBadRemap = true;
                        break;
                    }
                }
            }
            /* Nothing to do, already copied the constant values into the new descriptor info */
        default:
            break;
        }
    }

    for (uint32_t i = 0; i < pPacket->descriptorCopyCount && !errorBadRemap; i++)
    {
        pRemappedCopies[i].dstSet = m_objMapper.remap_descriptorsets(pPacket->pDescriptorCopies[i].dstSet);
        if (pRemappedCopies[i].dstSet == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped destination VkDescriptorSet.");
            errorBadRemap = true;
            break;
        }

        pRemappedCopies[i].srcSet = m_objMapper.remap_descriptorsets(pPacket->pDescriptorCopies[i].srcSet);
        if (pRemappedCopies[i].srcSet == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped source VkDescriptorSet.");
            errorBadRemap = true;
            break;
        }
    }

    if (!errorBadRemap)
    {
        // If an error occurred, don't call the real function, but skip ahead so that memory is cleaned up!

        m_vkFuncs.real_vkUpdateDescriptorSets(remappedDevice, pPacket->descriptorWriteCount, pRemappedWrites, pPacket->descriptorCopyCount, pRemappedCopies);
    }

    for (uint32_t d = 0; d < pPacket->descriptorWriteCount; d++)
    {
        if (pRemappedWrites[d].pImageInfo != NULL) {
            VKTRACE_DELETE((void*)pRemappedWrites[d].pImageInfo);
            pRemappedWrites[d].pImageInfo = NULL;
        }
        if (pRemappedWrites[d].pBufferInfo != NULL) {
            VKTRACE_DELETE((void*)pRemappedWrites[d].pBufferInfo);
            pRemappedWrites[d].pImageInfo = NULL;
        }
        if (pRemappedWrites[d].pTexelBufferView != NULL) {
            VKTRACE_DELETE((void*)pRemappedWrites[d].pTexelBufferView);
            pRemappedWrites[d].pTexelBufferView = NULL;
        }
    }
    VKTRACE_DELETE(pRemappedWrites);
    VKTRACE_DELETE(pRemappedCopies);
}

VkResult vkReplay::manually_replay_vkCreateDescriptorSetLayout(packet_vkCreateDescriptorSetLayout* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateDescriptorSetLayout() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorSetLayoutCreateInfo *pInfo = (VkDescriptorSetLayoutCreateInfo*) pPacket->pCreateInfo;
    if (pInfo != NULL)
    {
        if (pInfo->pBindings != NULL)
        {
            for (unsigned int i = 0; i < pInfo->bindingCount; i++)
            {
                VkDescriptorSetLayoutBinding *pBindings = (VkDescriptorSetLayoutBinding *) &pInfo->pBindings[i];
                if (pBindings->pImmutableSamplers != NULL)
                {
                    for (unsigned int j = 0; j < pBindings->descriptorCount; j++)
                    {
                        VkSampler* pSampler = (VkSampler*)&pBindings->pImmutableSamplers[j];
                        *pSampler = m_objMapper.remap_samplers(pBindings->pImmutableSamplers[j]);
                        if (*pSampler == VK_NULL_HANDLE)
                        {
                            vktrace_LogError("Skipping vkCreateDescriptorSetLayout() due to invalid remapped VkSampler.");
                            return VK_ERROR_VALIDATION_FAILED_EXT;
                        }
                    }
                }
            }
        }
    }
    VkDescriptorSetLayout setLayout;
    replayResult = m_vkFuncs.real_vkCreateDescriptorSetLayout(remappedDevice, pPacket->pCreateInfo, NULL, &setLayout);
    if (replayResult == VK_SUCCESS)
    {
        m_objMapper.add_to_descriptorsetlayouts_map(*(pPacket->pSetLayout), setLayout);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkDestroyDescriptorSetLayout(packet_vkDestroyDescriptorSetLayout* pPacket)
{
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkDestroyDescriptorSetLayout() due to invalid remapped VkDevice.");
        return;
    }

    m_vkFuncs.real_vkDestroyDescriptorSetLayout(remappedDevice, pPacket->descriptorSetLayout, NULL);
    m_objMapper.rm_from_descriptorsetlayouts_map(pPacket->descriptorSetLayout);
}

VkResult vkReplay::manually_replay_vkAllocateDescriptorSets(packet_vkAllocateDescriptorSets* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkAllocateDescriptorSets() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorPool remappedPool = m_objMapper.remap_descriptorpools(pPacket->pAllocateInfo->descriptorPool);
    if (remappedPool == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkAllocateDescriptorSets() due to invalid remapped VkDescriptorPool.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorSetLayout* pRemappedSetLayouts = VKTRACE_NEW_ARRAY(VkDescriptorSetLayout, pPacket->pAllocateInfo->descriptorSetCount);

    VkDescriptorSetAllocateInfo allocateInfo;
    allocateInfo.pNext = NULL;
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = remappedPool;
    allocateInfo.descriptorSetCount = pPacket->pAllocateInfo->descriptorSetCount;
    allocateInfo.pSetLayouts = pRemappedSetLayouts;

    for (uint32_t i = 0; i < allocateInfo.descriptorSetCount; i++)
    {
        pRemappedSetLayouts[i] = m_objMapper.remap_descriptorsetlayouts(pPacket->pAllocateInfo->pSetLayouts[i]);
        if (pRemappedSetLayouts[i] == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkAllocateDescriptorSets() due to invalid remapped VkDescriptorSetLayout.");
            VKTRACE_DELETE(pRemappedSetLayouts);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }

    VkDescriptorSet* pDescriptorSets = NULL;
    replayResult = m_vkFuncs.real_vkAllocateDescriptorSets(
                       remappedDevice,
                       pPacket->pAllocateInfo,
                       pDescriptorSets);
    if(replayResult == VK_SUCCESS)
    {
        for(uint32_t i = 0; i < pPacket->pAllocateInfo->descriptorSetCount; ++i)
        {
           m_objMapper.add_to_descriptorsets_map(pPacket->pDescriptorSets[i], pDescriptorSets[i]);
        }
    }

    VKTRACE_DELETE(pRemappedSetLayouts);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkFreeDescriptorSets(packet_vkFreeDescriptorSets* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkFreeDescriptorSets() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorPool remappedDescriptorPool;
    remappedDescriptorPool = m_objMapper.remap_descriptorpools(pPacket->descriptorPool);
    if (remappedDescriptorPool == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkFreeDescriptorSets() due to invalid remapped VkDescriptorPool.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorSet* localDSs = VKTRACE_NEW_ARRAY(VkDescriptorSet, pPacket->descriptorSetCount);
    uint32_t i;
    for (i = 0; i < pPacket->descriptorSetCount; ++i) {
        localDSs[i] = m_objMapper.remap_descriptorsets(pPacket->pDescriptorSets[i]);
        if (localDSs[i] == VK_NULL_HANDLE && pPacket->pDescriptorSets[i] != VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkFreeDescriptorSets() due to invalid remapped VkDescriptorSet.");
            VKTRACE_DELETE(localDSs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }

    replayResult = m_vkFuncs.real_vkFreeDescriptorSets(remappedDevice, remappedDescriptorPool, pPacket->descriptorSetCount, localDSs);
    if(replayResult == VK_SUCCESS)
    {
        for (i = 0; i < pPacket->descriptorSetCount; ++i) {
           m_objMapper.rm_from_descriptorsets_map(pPacket->pDescriptorSets[i]);
        }
    }
    VKTRACE_DELETE(localDSs);
    return replayResult;
}

void vkReplay::manually_replay_vkCmdBindDescriptorSets(packet_vkCmdBindDescriptorSets* pPacket)
{
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdBindDescriptorSets() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkPipelineLayout remappedLayout = m_objMapper.remap_pipelinelayouts(pPacket->layout);
    if (remappedLayout == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdBindDescriptorSets() due to invalid remapped VkPipelineLayout.");
        return;
    }

    VkDescriptorSet* pRemappedSets = (VkDescriptorSet *) vktrace_malloc(sizeof(VkDescriptorSet) * pPacket->descriptorSetCount);
    if (pRemappedSets == NULL)
    {
        vktrace_LogError("Replay of CmdBindDescriptorSets out of memory.");
        return;
    }

    for (uint32_t idx = 0; idx < pPacket->descriptorSetCount && pPacket->pDescriptorSets != NULL; idx++)
    {
        pRemappedSets[idx] = m_objMapper.remap_descriptorsets(pPacket->pDescriptorSets[idx]);
        if (pRemappedSets[idx] == VK_NULL_HANDLE && pPacket->pDescriptorSets[idx] != VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCmdBindDescriptorSets() due to invalid remapped VkDescriptorSet.");
            vktrace_free(pRemappedSets);
            return;
        }
    }

    m_vkFuncs.real_vkCmdBindDescriptorSets(remappedCommandBuffer, pPacket->pipelineBindPoint, remappedLayout, pPacket->firstSet, pPacket->descriptorSetCount, pRemappedSets, pPacket->dynamicOffsetCount, pPacket->pDynamicOffsets);
    vktrace_free(pRemappedSets);
    return;
}

void vkReplay::manually_replay_vkCmdBindVertexBuffers(packet_vkCmdBindVertexBuffers* pPacket)
{
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdBindVertexBuffers() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkBuffer *pSaveBuff = VKTRACE_NEW_ARRAY(VkBuffer, pPacket->bindingCount);
    if (pSaveBuff == NULL && pPacket->bindingCount > 0)
    {
        vktrace_LogError("Replay of CmdBindVertexBuffers out of memory.");
        return;
    }
    uint32_t i = 0;
    if (pPacket->pBuffers != NULL) {
        for (i = 0; i < pPacket->bindingCount; i++)
        {
            VkBuffer *pBuff = (VkBuffer*) &(pPacket->pBuffers[i]);
            pSaveBuff[i] = pPacket->pBuffers[i];
            *pBuff = m_objMapper.remap_buffers(pPacket->pBuffers[i]);
            if (*pBuff == VK_NULL_HANDLE && pPacket->pBuffers[i] != VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkCmdBindVertexBuffers() due to invalid remapped VkBuffer.");
                VKTRACE_DELETE(pSaveBuff);
                return;
            }
        }
    }
    m_vkFuncs.real_vkCmdBindVertexBuffers(remappedCommandBuffer, pPacket->firstBinding, pPacket->bindingCount, pPacket->pBuffers, pPacket->pOffsets);
    for (uint32_t k = 0; k < i; k++)
    {
        VkBuffer *pBuff = (VkBuffer*) &(pPacket->pBuffers[k]);
        *pBuff = pSaveBuff[k];
    }
    VKTRACE_DELETE(pSaveBuff);
    return;
}

//VkResult vkReplay::manually_replay_vkCreateGraphicsPipeline(packet_vkCreateGraphicsPipeline* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
//    if (remappedDevice == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkCreateGraphicsPipeline() due to invalid remapped VkDevice.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    // remap shaders from each stage
//    VkPipelineShaderStageCreateInfo* pRemappedStages = VKTRACE_NEW_ARRAY(VkPipelineShaderStageCreateInfo, pPacket->pCreateInfo->stageCount);
//    memcpy(pRemappedStages, pPacket->pCreateInfo->pStages, sizeof(VkPipelineShaderStageCreateInfo) * pPacket->pCreateInfo->stageCount);
//    for (uint32_t i = 0; i < pPacket->pCreateInfo->stageCount; i++)
//    {
//        pRemappedStages[i].shader = m_objMapper.remap(pRemappedStages[i].shader);
//        if (pRemappedStages[i].shader == VK_NULL_HANDLE)
//        {
//            vktrace_LogError("Skipping vkCreateGraphicsPipeline() due to invalid remapped VkShader.");
//            return VK_ERROR_VALIDATION_FAILED_EXT;
//        }
//    }
//
//    VkGraphicsPipelineCreateInfo createInfo = {
//        .sType = pPacket->pCreateInfo->sType,
//        .pNext = pPacket->pCreateInfo->pNext,
//        .stageCount = pPacket->pCreateInfo->stageCount,
//        .pStages = pRemappedStages,
//        .pVertexInputState = pPacket->pCreateInfo->pVertexInputState,
//        .pIaState = pPacket->pCreateInfo->pIaState,
//        .pTessState = pPacket->pCreateInfo->pTessState,
//        .pVpState = pPacket->pCreateInfo->pVpState,
//        .pRsState = pPacket->pCreateInfo->pRsState,
//        .pMsState = pPacket->pCreateInfo->pMsState,
//        .pDsState = pPacket->pCreateInfo->pDsState,
//        .pCbState = pPacket->pCreateInfo->pCbState,
//        .flags = pPacket->pCreateInfo->flags,
//        .layout = m_objMapper.remap(pPacket->pCreateInfo->layout)
//    };
//
//    VkPipeline pipeline;
//    replayResult = m_vkFuncs.real_vkCreateGraphicsPipeline(remappedDevice, &createInfo, &pipeline);
//    if (replayResult == VK_SUCCESS)
//    {
//        m_objMapper.add_to_map(pPacket->pPipeline, &pipeline);
//    }
//
//    VKTRACE_DELETE(pRemappedStages);
//
//    return replayResult;
//}

VkResult vkReplay::manually_replay_vkGetPipelineCacheData(packet_vkGetPipelineCacheData* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    size_t dataSize;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (remappeddevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPipelineCacheData() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkPipelineCache remappedpipelineCache = m_objMapper.remap_pipelinecaches(pPacket->pipelineCache);
    if (pPacket->pipelineCache != VK_NULL_HANDLE && remappedpipelineCache == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPipelineCacheData() due to invalid remapped VkPipelineCache.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // Since the returned data size may not be equal to size of the buffer in the trace packet allocate a local buffer as needed
    replayResult = m_vkFuncs.real_vkGetPipelineCacheData(remappeddevice, remappedpipelineCache, &dataSize, NULL);
    if (replayResult != VK_SUCCESS)
        return replayResult;
    if (pPacket->pData) {
        uint8_t *pData = VKTRACE_NEW_ARRAY(uint8_t, dataSize);
        replayResult = m_vkFuncs.real_vkGetPipelineCacheData(remappeddevice, remappedpipelineCache, pPacket->pDataSize, pData);
        VKTRACE_DELETE(pData);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateComputePipelines(packet_vkCreateComputePipelines* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    uint32_t i;

    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE)
    {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkPipelineCache pipelineCache;
    pipelineCache = m_objMapper.remap_pipelinecaches(pPacket->pipelineCache);

    VkComputePipelineCreateInfo* pLocalCIs = VKTRACE_NEW_ARRAY(VkComputePipelineCreateInfo, pPacket->createInfoCount);
    memcpy((void*)pLocalCIs, (void*)(pPacket->pCreateInfos), sizeof(VkComputePipelineCreateInfo)*pPacket->createInfoCount);

    // Fix up stage sub-elements
    for (i = 0; i<pPacket->createInfoCount; i++)
    {
        pLocalCIs[i].stage.module = m_objMapper.remap_shadermodules(pLocalCIs[i].stage.module);

        if (pLocalCIs[i].stage.pName)
            pLocalCIs[i].stage.pName = (const char*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pLocalCIs[i].stage.pName));

        if (pLocalCIs[i].stage.pSpecializationInfo)
        {
            VkSpecializationInfo* si = VKTRACE_NEW(VkSpecializationInfo);
            pLocalCIs[i].stage.pSpecializationInfo = (const VkSpecializationInfo*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pLocalCIs[i].stage.pSpecializationInfo));
            memcpy((void*)si, (void*)(pLocalCIs[i].stage.pSpecializationInfo), sizeof(VkSpecializationInfo));

            if (si->mapEntryCount > 0 && si->pMapEntries)
                si->pMapEntries = (const VkSpecializationMapEntry*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pLocalCIs[i].stage.pSpecializationInfo->pMapEntries));
            if (si->dataSize > 0 && si->pData)
                si->pData = (const void*)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)si->pData));
            pLocalCIs[i].stage.pSpecializationInfo = si;
        }

        pLocalCIs[i].layout = m_objMapper.remap_pipelinelayouts(pLocalCIs[i].layout);
        pLocalCIs[i].basePipelineHandle = m_objMapper.remap_pipelines(pLocalCIs[i].basePipelineHandle);
    }

    VkPipeline *local_pPipelines = VKTRACE_NEW_ARRAY(VkPipeline, pPacket->createInfoCount);

    replayResult = m_vkFuncs.real_vkCreateComputePipelines(remappeddevice, pipelineCache, pPacket->createInfoCount, pLocalCIs, NULL, local_pPipelines);

    if (replayResult == VK_SUCCESS)
    {
        for (i = 0; i < pPacket->createInfoCount; i++) {
            m_objMapper.add_to_pipelines_map(pPacket->pPipelines[i], local_pPipelines[i]);
        }
    }

    for (i = 0; i<pPacket->createInfoCount; i++)
        if (pLocalCIs[i].stage.pSpecializationInfo)
            VKTRACE_DELETE((void *)pLocalCIs[i].stage.pSpecializationInfo);
    VKTRACE_DELETE(pLocalCIs);
    VKTRACE_DELETE(local_pPipelines);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateGraphicsPipelines(packet_vkCreateGraphicsPipelines* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // remap shaders from each stage
    VkPipelineShaderStageCreateInfo* pRemappedStages = VKTRACE_NEW_ARRAY(VkPipelineShaderStageCreateInfo, pPacket->pCreateInfos->stageCount);
    memcpy(pRemappedStages, pPacket->pCreateInfos->pStages, sizeof(VkPipelineShaderStageCreateInfo) * pPacket->pCreateInfos->stageCount);

    VkGraphicsPipelineCreateInfo* pLocalCIs = VKTRACE_NEW_ARRAY(VkGraphicsPipelineCreateInfo, pPacket->createInfoCount);
    uint32_t i, j;
    for (i=0; i<pPacket->createInfoCount; i++) {
        memcpy((void*)&(pLocalCIs[i]), (void*)&(pPacket->pCreateInfos[i]), sizeof(VkGraphicsPipelineCreateInfo));
        for (j=0; j < pPacket->pCreateInfos[i].stageCount; j++)
        {
            pRemappedStages[j].module = m_objMapper.remap_shadermodules(pRemappedStages[j].module);
            if (pRemappedStages[j].module == VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkShaderModule.");
                VKTRACE_DELETE(pRemappedStages);
                VKTRACE_DELETE(pLocalCIs);
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
        VkPipelineShaderStageCreateInfo** ppSSCI = (VkPipelineShaderStageCreateInfo**)&(pLocalCIs[i].pStages);
        *ppSSCI = pRemappedStages;

        pLocalCIs[i].layout = m_objMapper.remap_pipelinelayouts(pPacket->pCreateInfos[i].layout);
        if (pLocalCIs[i].layout == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkPipelineLayout.");
            VKTRACE_DELETE(pRemappedStages);
            VKTRACE_DELETE(pLocalCIs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        pLocalCIs[i].renderPass = m_objMapper.remap_renderpasss(pPacket->pCreateInfos[i].renderPass);
        if (pLocalCIs[i].renderPass == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkRenderPass.");
            VKTRACE_DELETE(pRemappedStages);
            VKTRACE_DELETE(pLocalCIs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        pLocalCIs[i].basePipelineHandle = m_objMapper.remap_pipelines(pPacket->pCreateInfos[i].basePipelineHandle);
        if (pLocalCIs[i].basePipelineHandle == VK_NULL_HANDLE && pPacket->pCreateInfos[i].basePipelineHandle != VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkPipeline.");
            VKTRACE_DELETE(pRemappedStages);
            VKTRACE_DELETE(pLocalCIs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        ((VkPipelineViewportStateCreateInfo*)pLocalCIs[i].pViewportState)->pViewports =
                (VkViewport*)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfos[i].pViewportState->pViewports);
        ((VkPipelineViewportStateCreateInfo*)pLocalCIs[i].pViewportState)->pScissors =
                (VkRect2D*)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfos[i].pViewportState->pScissors);

        ((VkPipelineMultisampleStateCreateInfo*)pLocalCIs[i].pMultisampleState)->pSampleMask =
                (VkSampleMask*)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfos[i].pMultisampleState->pSampleMask);
    }

    VkPipelineCache remappedPipelineCache;
    remappedPipelineCache = m_objMapper.remap_pipelinecaches(pPacket->pipelineCache);
    if (remappedPipelineCache == VK_NULL_HANDLE && pPacket->pipelineCache != VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkPipelineCache.");
        VKTRACE_DELETE(pRemappedStages);
        VKTRACE_DELETE(pLocalCIs);
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    uint32_t createInfoCount = pPacket->createInfoCount;
    VkPipeline *local_pPipelines = VKTRACE_NEW_ARRAY(VkPipeline, pPacket->createInfoCount);

    replayResult = m_vkFuncs.real_vkCreateGraphicsPipelines(remappedDevice, remappedPipelineCache, createInfoCount, pLocalCIs, NULL, local_pPipelines);

    if (replayResult == VK_SUCCESS)
    {
        for (i = 0; i < pPacket->createInfoCount; i++) {
            m_objMapper.add_to_pipelines_map(pPacket->pPipelines[i], local_pPipelines[i]);
        }
    }

    VKTRACE_DELETE(pRemappedStages);
    VKTRACE_DELETE(pLocalCIs);
    VKTRACE_DELETE(local_pPipelines);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreatePipelineLayout(packet_vkCreatePipelineLayout* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
        return VK_ERROR_VALIDATION_FAILED_EXT;

    // array to store the original trace-time layouts, so that we can remap them inside the packet and then
    // restore them after replaying the API call.
    VkDescriptorSetLayout* pSaveLayouts = NULL;
    if (pPacket->pCreateInfo->setLayoutCount > 0) {
        pSaveLayouts = (VkDescriptorSetLayout*) vktrace_malloc(sizeof(VkDescriptorSetLayout) * pPacket->pCreateInfo->setLayoutCount);
        if (!pSaveLayouts) {
            vktrace_LogError("Replay of CreatePipelineLayout out of memory.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }
    uint32_t i = 0;
    for (i = 0; (i < pPacket->pCreateInfo->setLayoutCount) && (pPacket->pCreateInfo->pSetLayouts != NULL); i++) {
        VkDescriptorSetLayout* pSL = (VkDescriptorSetLayout*) &(pPacket->pCreateInfo->pSetLayouts[i]);
        pSaveLayouts[i] = pPacket->pCreateInfo->pSetLayouts[i];
        *pSL = m_objMapper.remap_descriptorsetlayouts(pPacket->pCreateInfo->pSetLayouts[i]);
    }
    VkPipelineLayout localPipelineLayout;
    replayResult = m_vkFuncs.real_vkCreatePipelineLayout(remappedDevice, pPacket->pCreateInfo, NULL, &localPipelineLayout);
    if (replayResult == VK_SUCCESS)
    {
        m_objMapper.add_to_pipelinelayouts_map(*(pPacket->pPipelineLayout), localPipelineLayout);
    }
    // restore packet to contain the original Set Layouts before being remapped.
    for (uint32_t k = 0; k < i; k++) {
        VkDescriptorSetLayout* pSL = (VkDescriptorSetLayout*) &(pPacket->pCreateInfo->pSetLayouts[k]);
        *pSL = pSaveLayouts[k];
    }
    if (pSaveLayouts != NULL) {
        vktrace_free(pSaveLayouts);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkCmdWaitEvents(packet_vkCmdWaitEvents* pPacket)
{
    VkDevice traceDevice;
    VkDevice replayDevice;
    uint32_t srcReplayIdx, dstReplayIdx;
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkEvent* saveEvent = VKTRACE_NEW_ARRAY(VkEvent, pPacket->eventCount);
    uint32_t idx = 0;
    uint32_t numRemapBuf = 0;
    uint32_t numRemapImg = 0;
    for (idx = 0; idx < pPacket->eventCount; idx++)
    {
        VkEvent *pEvent = (VkEvent *) &(pPacket->pEvents[idx]);
        saveEvent[idx] = pPacket->pEvents[idx];
        *pEvent = m_objMapper.remap_events(pPacket->pEvents[idx]);
        if (*pEvent == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkEvent.");
            VKTRACE_DELETE(saveEvent);
            return;
        }
    }

    VkBuffer* saveBuf = VKTRACE_NEW_ARRAY(VkBuffer, pPacket->bufferMemoryBarrierCount);
    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++)
    {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *)& (pPacket->pBufferMemoryBarriers[idx]);
        saveBuf[numRemapBuf++] = pNextBuf->buffer;
        traceDevice = traceBufferToDevice[pNextBuf->buffer];
        pNextBuf->buffer = m_objMapper.remap_buffers(pNextBuf->buffer);
        if (pNextBuf->buffer == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkBuffer.");
            VKTRACE_DELETE(saveEvent);
            VKTRACE_DELETE(saveBuf);
            return;
        }
        replayDevice = replayBufferToDevice[pNextBuf->buffer];
        if (getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex,
                              &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pBufferMemoryBarriers[idx].dstQueueFamilyIndex,
                              &dstReplayIdx))
        {
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkCmdWaitEvents failed, bad srcQueueFamilyIndex");
            return;
        }
    }
    VkImage* saveImg = VKTRACE_NEW_ARRAY(VkImage, pPacket->imageMemoryBarrierCount);
    for (idx = 0; idx < pPacket->imageMemoryBarrierCount; idx++)
    {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *) &(pPacket->pImageMemoryBarriers[idx]);
        saveImg[numRemapImg++] = pNextImg->image;
        traceDevice = traceImageToDevice[pNextImg->image];
        pNextImg->image = m_objMapper.remap_images(pNextImg->image);
        if (pNextImg->image == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkImage.");
            VKTRACE_DELETE(saveEvent);
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
        replayDevice = replayImageToDevice[pNextImg->image];
        if (getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex,
                              &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pImageMemoryBarriers[idx].dstQueueFamilyIndex,
                              &dstReplayIdx))
        {
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkCmdWaitEvents failed, bad srcQueueFamilyIndex");
            return;
        }
    }
    m_vkFuncs.real_vkCmdWaitEvents(remappedCommandBuffer, pPacket->eventCount, pPacket->pEvents, pPacket->srcStageMask, pPacket->dstStageMask, pPacket->memoryBarrierCount, pPacket->pMemoryBarriers, pPacket->bufferMemoryBarrierCount, pPacket->pBufferMemoryBarriers, pPacket->imageMemoryBarrierCount, pPacket->pImageMemoryBarriers);

    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++) {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *) &(pPacket->pBufferMemoryBarriers[idx]);
        pNextBuf->buffer = saveBuf[idx];
    }
    for (idx = 0; idx < pPacket->memoryBarrierCount; idx++) {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *) &(pPacket->pImageMemoryBarriers[idx]);
        pNextImg->image = saveImg[idx];
    }
    for (idx = 0; idx < pPacket->eventCount; idx++) {
        VkEvent *pEvent = (VkEvent *) &(pPacket->pEvents[idx]);
        *pEvent = saveEvent[idx];
    }
    VKTRACE_DELETE(saveEvent);
    VKTRACE_DELETE(saveBuf);
    VKTRACE_DELETE(saveImg);
    return;
}

void vkReplay::manually_replay_vkCmdPipelineBarrier(packet_vkCmdPipelineBarrier* pPacket)
{
    VkDevice traceDevice;
    VkDevice replayDevice;
    uint32_t srcReplayIdx, dstReplayIdx;
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdPipelineBarrier() due to invalid remapped VkCommandBuffer.");
        return;
    }

    uint32_t idx = 0;
    uint32_t numRemapBuf = 0;
    uint32_t numRemapImg = 0;
    VkBuffer* saveBuf = VKTRACE_NEW_ARRAY(VkBuffer, pPacket->bufferMemoryBarrierCount);
    VkImage* saveImg = VKTRACE_NEW_ARRAY(VkImage, pPacket->imageMemoryBarrierCount);
    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++)
    {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *) &(pPacket->pBufferMemoryBarriers[idx]);
        saveBuf[numRemapBuf++] = pNextBuf->buffer;
        traceDevice = traceBufferToDevice[pNextBuf->buffer];
        pNextBuf->buffer = m_objMapper.remap_buffers(pNextBuf->buffer);
        if (pNextBuf->buffer == VK_NULL_HANDLE && saveBuf[numRemapBuf - 1] != VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCmdPipelineBarrier() due to invalid remapped VkBuffer.");
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
        replayDevice = replayBufferToDevice[pNextBuf->buffer];
        if (getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex,
                              &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pBufferMemoryBarriers[idx].dstQueueFamilyIndex,
                              &dstReplayIdx))
        {
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkCmdPipelineBarrier failed, bad srcQueueFamilyIndex");
            return;
        }
    }
    for (idx = 0; idx < pPacket->imageMemoryBarrierCount; idx++)
    {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *) &(pPacket->pImageMemoryBarriers[idx]);
        saveImg[numRemapImg++] = pNextImg->image;
        traceDevice = traceImageToDevice[pNextImg->image];
        if (traceDevice == NULL)
            vktrace_LogError("DEBUG: traceDevice is NULL");
        pNextImg->image = m_objMapper.remap_images(pNextImg->image);
        if (pNextImg->image == VK_NULL_HANDLE && saveImg[numRemapImg - 1] != VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkCmdPipelineBarrier() due to invalid remapped VkImage.");
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
        replayDevice = replayImageToDevice[pNextImg->image];
        if (getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex,
                              &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice,
                              replayDevice,
                              pPacket->pImageMemoryBarriers[idx].dstQueueFamilyIndex,
                              &dstReplayIdx))
        {
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkPipelineBarrier failed, bad srcQueueFamilyIndex");
            return;
        }
    }
    m_vkFuncs.real_vkCmdPipelineBarrier(remappedCommandBuffer, pPacket->srcStageMask, pPacket->dstStageMask, pPacket->dependencyFlags, pPacket->memoryBarrierCount, pPacket->pMemoryBarriers, pPacket->bufferMemoryBarrierCount, pPacket->pBufferMemoryBarriers, pPacket->imageMemoryBarrierCount, pPacket->pImageMemoryBarriers);

    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++) {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *) &(pPacket->pBufferMemoryBarriers[idx]);
        pNextBuf->buffer = saveBuf[idx];
    }
    for (idx = 0; idx < pPacket->imageMemoryBarrierCount; idx++) {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *) &(pPacket->pImageMemoryBarriers[idx]);
        pNextImg->image = saveImg[idx];
    }
    VKTRACE_DELETE(saveBuf);
    VKTRACE_DELETE(saveImg);
    return;
}

VkResult vkReplay::manually_replay_vkCreateFramebuffer(packet_vkCreateFramebuffer* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateFramebuffer() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFramebufferCreateInfo *pInfo = (VkFramebufferCreateInfo *) pPacket->pCreateInfo;
    VkImageView *pAttachments, *pSavedAttachments = (VkImageView*)pInfo->pAttachments;
    bool allocatedAttachments = false;
    if (pSavedAttachments != NULL)
    {
        allocatedAttachments = true;
        pAttachments = VKTRACE_NEW_ARRAY(VkImageView, pInfo->attachmentCount);
        memcpy(pAttachments, pSavedAttachments, sizeof(VkImageView) * pInfo->attachmentCount);
        for (uint32_t i = 0; i < pInfo->attachmentCount; i++)
        {
            pAttachments[i] = m_objMapper.remap_imageviews(pInfo->pAttachments[i]);
            if (pAttachments[i] == VK_NULL_HANDLE && pInfo->pAttachments[i] != VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkCreateFramebuffer() due to invalid remapped VkImageView.");
                VKTRACE_DELETE(pAttachments);
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
        pInfo->pAttachments = pAttachments;
    }
    VkRenderPass savedRP = pPacket->pCreateInfo->renderPass;
    pInfo->renderPass = m_objMapper.remap_renderpasss(pPacket->pCreateInfo->renderPass);
    if (pInfo->renderPass == VK_NULL_HANDLE && pPacket->pCreateInfo->renderPass != VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateFramebuffer() due to invalid remapped VkRenderPass.");
        if (allocatedAttachments)
        {
            VKTRACE_DELETE(pAttachments);
        }
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFramebuffer local_framebuffer;
    replayResult = m_vkFuncs.real_vkCreateFramebuffer(remappedDevice, pPacket->pCreateInfo, NULL, &local_framebuffer);
    pInfo->pAttachments = pSavedAttachments;
    pInfo->renderPass = savedRP;
    if (replayResult == VK_SUCCESS)
    {
        m_objMapper.add_to_framebuffers_map(*(pPacket->pFramebuffer), local_framebuffer);
    }
    if (allocatedAttachments)
    {
        VKTRACE_DELETE((void*)pAttachments);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateRenderPass(packet_vkCreateRenderPass* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateRenderPass() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkRenderPass local_renderpass;
    replayResult = m_vkFuncs.real_vkCreateRenderPass(remappedDevice, pPacket->pCreateInfo, NULL, &local_renderpass);
    if (replayResult == VK_SUCCESS)
    {
        m_objMapper.add_to_renderpasss_map(*(pPacket->pRenderPass), local_renderpass);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkCmdBeginRenderPass(packet_vkCmdBeginRenderPass* pPacket)
{
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdBeginRenderPass() due to invalid remapped VkCommandBuffer.");
        return;
    }
    VkRenderPassBeginInfo local_renderPassBeginInfo;
    memcpy((void*)&local_renderPassBeginInfo, (void*)pPacket->pRenderPassBegin, sizeof(VkRenderPassBeginInfo));
    local_renderPassBeginInfo.pClearValues = (const VkClearValue*)pPacket->pRenderPassBegin->pClearValues;
    local_renderPassBeginInfo.framebuffer = m_objMapper.remap_framebuffers(pPacket->pRenderPassBegin->framebuffer);
    if (local_renderPassBeginInfo.framebuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdBeginRenderPass() due to invalid remapped VkFramebuffer.");
        return;
    }
    local_renderPassBeginInfo.renderPass = m_objMapper.remap_renderpasss(pPacket->pRenderPassBegin->renderPass);
    if (local_renderPassBeginInfo.renderPass == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCmdBeginRenderPass() due to invalid remapped VkRenderPass.");
        return;
    }
    m_vkFuncs.real_vkCmdBeginRenderPass(remappedCommandBuffer, &local_renderPassBeginInfo, pPacket->contents);
    return;
}

VkResult vkReplay::manually_replay_vkBeginCommandBuffer(packet_vkBeginCommandBuffer* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkBeginCommandBuffer() due to invalid remapped VkCommandBuffer.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkCommandBufferBeginInfo* pInfo = (VkCommandBufferBeginInfo*)pPacket->pBeginInfo;
    VkCommandBufferInheritanceInfo *pHinfo = (VkCommandBufferInheritanceInfo *) ((pInfo) ? pInfo->pInheritanceInfo : NULL);
    // Save the original RP & FB, then overwrite packet with remapped values
    VkRenderPass savedRP, *pRP;
    VkFramebuffer savedFB, *pFB;
    if (pInfo != NULL && pHinfo != NULL)
    {
        savedRP = pHinfo->renderPass;
        savedFB = pHinfo->framebuffer;
        pRP = &(pHinfo->renderPass);
        pFB = &(pHinfo->framebuffer);
        *pRP = m_objMapper.remap_renderpasss(savedRP);
        *pFB = m_objMapper.remap_framebuffers(savedFB);
    }
    replayResult = m_vkFuncs.real_vkBeginCommandBuffer(remappedCommandBuffer, pPacket->pBeginInfo);
    if (pInfo != NULL && pHinfo != NULL) {
        pHinfo->renderPass = savedRP;
        pHinfo->framebuffer = savedFB;
    }
    return replayResult;
}

// TODO138 : Can we kill this?
//VkResult vkReplay::manually_replay_vkStorePipeline(packet_vkStorePipeline* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
//    if (remappedDevice == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkStorePipeline() due to invalid remapped VkDevice.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    VkPipeline remappedPipeline = m_objMapper.remap(pPacket->pipeline);
//    if (remappedPipeline == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkStorePipeline() due to invalid remapped VkPipeline.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    size_t size = 0;
//    void* pData = NULL;
//    if (pPacket->pData != NULL && pPacket->pDataSize != NULL)
//    {
//        size = *pPacket->pDataSize;
//        pData = vktrace_malloc(*pPacket->pDataSize);
//    }
//    replayResult = m_vkFuncs.real_vkStorePipeline(remappedDevice, remappedPipeline, &size, pData);
//    if (replayResult == VK_SUCCESS)
//    {
//        if (size != *pPacket->pDataSize && pData != NULL)
//        {
//            vktrace_LogWarning("vkStorePipeline returned a differing data size: replay (%d bytes) vs trace (%d bytes).", size, *pPacket->pDataSize);
//        }
//        else if (pData != NULL && memcmp(pData, pPacket->pData, size) != 0)
//        {
//            vktrace_LogWarning("vkStorePipeline returned differing data contents than the trace file contained.");
//        }
//    }
//    vktrace_free(pData);
//    return replayResult;
//}

// TODO138 : This needs to be broken out into separate functions for each non-disp object
//VkResult vkReplay::manually_replay_vkDestroy<Object>(packet_vkDestroyObject* pPacket)
//{
//    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
//
//    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
//    if (remappedDevice == VK_NULL_HANDLE)
//    {
//        vktrace_LogError("Skipping vkDestroy() due to invalid remapped VkDevice.");
//        return VK_ERROR_VALIDATION_FAILED_EXT;
//    }
//
//    uint64_t remapHandle = m_objMapper.remap_<OBJECT_TYPE_HERE>(pPacket->object, pPacket->objType);
//    <VkObject> object;
//    object.handle = remapHandle;
//    if (object != 0)
//        replayResult = m_vkFuncs.real_vkDestroy<Object>(remappedDevice, object);
//    if (replayResult == VK_SUCCESS)
//        m_objMapper.rm_from_<OBJECT_TYPE_HERE>_map(pPacket->object.handle);
//    return replayResult;
//}

VkResult vkReplay::manually_replay_vkWaitForFences(packet_vkWaitForFences* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    uint32_t i;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkWaitForFences() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFence *pFence = VKTRACE_NEW_ARRAY(VkFence, pPacket->fenceCount);
    for (i = 0; i < pPacket->fenceCount; i++)
    {
        (*(pFence + i)) = m_objMapper.remap_fences((*(pPacket->pFences + i)));
        if (*(pFence + i) == VK_NULL_HANDLE)
        {
            vktrace_LogError("Skipping vkWaitForFences() due to invalid remapped VkFence.");
            VKTRACE_DELETE(pFence);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }
    if (pPacket->result == VK_SUCCESS)
    {
        replayResult = m_vkFuncs.real_vkWaitForFences(remappedDevice, pPacket->fenceCount, pFence, pPacket->waitAll, UINT64_MAX);// mean as long as possible
    }
    else
    {
        if (pPacket->result == VK_TIMEOUT)
        {
            replayResult = m_vkFuncs.real_vkWaitForFences(remappedDevice, pPacket->fenceCount, pFence, pPacket->waitAll, 0);
        }
        else
        {
            replayResult = m_vkFuncs.real_vkWaitForFences(remappedDevice, pPacket->fenceCount, pFence, pPacket->waitAll, pPacket->timeout);
        }
    }
    VKTRACE_DELETE(pFence);
    return replayResult;
}

bool vkReplay::getMemoryTypeIdx(VkDevice traceDevice,
                                VkDevice replayDevice,
                                uint32_t traceIdx,
                                uint32_t* pReplayIdx)
{
    VkPhysicalDevice tracePhysicalDevice;
    VkPhysicalDevice replayPhysicalDevice;
    bool foundMatch = false;
    uint32_t i,j;

    if (tracePhysicalDevices.find(traceDevice) == tracePhysicalDevices.end() ||
        replayPhysicalDevices.find(replayDevice) == replayPhysicalDevices.end())
    {
        goto fail;
    }

    tracePhysicalDevice = tracePhysicalDevices[traceDevice];
    replayPhysicalDevice = replayPhysicalDevices[replayDevice];

    if (min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount, replayMemoryProperties[replayPhysicalDevice].memoryTypeCount) == 0)
    {
        goto fail;
    }

    for (i = 0; i < min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount, replayMemoryProperties[replayPhysicalDevice].memoryTypeCount); i++)
    {
        if (traceMemoryProperties[tracePhysicalDevice].memoryTypes[traceIdx].propertyFlags == replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags)
        {
            *pReplayIdx = i;
            foundMatch = true;
            break;
        }
    }

    if (!foundMatch)
    {
        // Didn't find an exact match, search for a superset
        for (i = 0; i < min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount, replayMemoryProperties[replayPhysicalDevice].memoryTypeCount); i++)
        {
            if (traceMemoryProperties[tracePhysicalDevice].memoryTypes[traceIdx].propertyFlags ==
                (traceMemoryProperties[tracePhysicalDevice].memoryTypes[traceIdx].propertyFlags & replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags))
            {
                *pReplayIdx = i;
                foundMatch = true;
                break;
            }
        }
    }

    if (!foundMatch)
    {
        // Didn't find a superset, search for mem type with both HOST_VISIBLE and HOST_COHERENT set
        for (i = 0; i < min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount, replayMemoryProperties[replayPhysicalDevice].memoryTypeCount); i++)
        {
            if ((VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) & replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags)
            {
                *pReplayIdx = i;
                foundMatch = true;
                break;
            }
        }
    }

    if (foundMatch)
    {
        // Check to see if there are other replayMemoryProperties identical to the one that matched.
        // If there are, print a warning and use the index from the trace file.
        for (j = i+1; j < replayMemoryProperties[replayPhysicalDevice].memoryTypeCount; j++)
        {
            if (replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags == replayMemoryProperties[replayPhysicalDevice].memoryTypes[j].propertyFlags)
            {
                vktrace_LogWarning("memoryTypes propertyFlags identical in two or more entries, using idx %d from trace", traceIdx);
                *pReplayIdx = traceIdx;
                return true;
            }
         }
        return true;
     }

fail:
    // Didn't find a match
    vktrace_LogError("Cannot determine memory type during vkAllocateMemory - vkGetPhysicalDeviceMemoryProperties should be called before vkAllocateMemory.");
    return false;
}


VkResult vkReplay::manually_replay_vkAllocateMemory(packet_vkAllocateMemory* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkAllocateMemory() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    gpuMemObj local_mem;

    if (!m_objMapper.m_adjustForGPU)
    {
        uint32_t replayIdx;
        if (getMemoryTypeIdx(pPacket->device, remappedDevice, pPacket->pAllocateInfo->memoryTypeIndex, &replayIdx))
        {
            *((uint32_t*)&pPacket->pAllocateInfo->memoryTypeIndex) = replayIdx;
            replayResult = m_vkFuncs.real_vkAllocateMemory(remappedDevice, pPacket->pAllocateInfo, NULL, &local_mem.replayGpuMem);
        } else {
            vktrace_LogError("vkAllocateMemory() failed, couldn't find memory type for memoryTypeIndex");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }
    if (replayResult == VK_SUCCESS || m_objMapper.m_adjustForGPU)
    {
        local_mem.pGpuMem = new (gpuMemory);
        if (local_mem.pGpuMem)
            local_mem.pGpuMem->setAllocInfo(pPacket->pAllocateInfo, m_objMapper.m_adjustForGPU);
        m_objMapper.add_to_devicememorys_map(*(pPacket->pMemory), local_mem);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkFreeMemory(packet_vkFreeMemory* pPacket)
{
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkFreeMemory() due to invalid remapped VkDevice.");
        return;
    }

    gpuMemObj local_mem;
    local_mem = m_objMapper.m_devicememorys.find(pPacket->memory)->second;
    // TODO how/when to free pendingAlloc that did not use and existing gpuMemObj
    m_vkFuncs.real_vkFreeMemory(remappedDevice, local_mem.replayGpuMem, NULL);
    delete local_mem.pGpuMem;
    m_objMapper.rm_from_devicememorys_map(pPacket->memory);
}

VkResult vkReplay::manually_replay_vkMapMemory(packet_vkMapMemory* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkMapMemory() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    gpuMemObj local_mem = m_objMapper.m_devicememorys.find(pPacket->memory)->second;
    void* pData;
    if (!local_mem.pGpuMem->isPendingAlloc())
    {
        replayResult = m_vkFuncs.real_vkMapMemory(remappedDevice, local_mem.replayGpuMem, pPacket->offset, pPacket->size, pPacket->flags, &pData);
        if (replayResult == VK_SUCCESS)
        {
            if (local_mem.pGpuMem)
            {
                local_mem.pGpuMem->setMemoryMapRange(pData, (size_t)pPacket->size, (size_t)pPacket->offset, false);
            }
        }
    }
    else
    {
        if (local_mem.pGpuMem)
        {
            local_mem.pGpuMem->setMemoryMapRange(NULL, (size_t)pPacket->size, (size_t)pPacket->offset, true);
        }
    }
    return replayResult;
}

void vkReplay::manually_replay_vkUnmapMemory(packet_vkUnmapMemory* pPacket)
{
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkUnmapMemory() due to invalid remapped VkDevice.");
        return;
    }

    gpuMemObj local_mem = m_objMapper.m_devicememorys.find(pPacket->memory)->second;
    if (!local_mem.pGpuMem->isPendingAlloc())
    {
        if (local_mem.pGpuMem)
        {
            if (pPacket->pData)
                local_mem.pGpuMem->copyMappingData(pPacket->pData, true, 0, 0);  // copies data from packet into memory buffer
        }
        m_vkFuncs.real_vkUnmapMemory(remappedDevice, local_mem.replayGpuMem);
    }
    else
    {
        if (local_mem.pGpuMem)
        {
            unsigned char *pBuf = (unsigned char *) vktrace_malloc(local_mem.pGpuMem->getMemoryMapSize());
            if (!pBuf)
            {
                vktrace_LogError("vkUnmapMemory() malloc failed.");
            }
            local_mem.pGpuMem->setMemoryDataAddr(pBuf);
            local_mem.pGpuMem->copyMappingData(pPacket->pData, true, 0, 0);
        }
    }
}

BOOL isvkFlushMappedMemoryRangesSpecial(PBYTE pOPTPackageData)
{
    BOOL bRet = FALSE;
    PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)pOPTPackageData;
    if (((uint64_t)pChangedInfoArray[0].reserve0) & PAGEGUARD_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES) // TODO need think about 32bit
    {
        bRet = TRUE;
    }
    return bRet;
}

//after OPT speed up, the format of this packet will be different with before, the packet now only include changed block(page).
//
VkResult vkReplay::manually_replay_vkFlushMappedMemoryRanges(packet_vkFlushMappedMemoryRanges* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkFlushMappedMemoryRanges() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkMappedMemoryRange* localRanges = VKTRACE_NEW_ARRAY(VkMappedMemoryRange, pPacket->memoryRangeCount);
    memcpy(localRanges, pPacket->pMemoryRanges, sizeof(VkMappedMemoryRange) * (pPacket->memoryRangeCount));

    gpuMemObj* pLocalMems = VKTRACE_NEW_ARRAY(gpuMemObj, pPacket->memoryRangeCount);
    for (uint32_t i = 0; i < pPacket->memoryRangeCount; i++)
    {
        pLocalMems[i] = m_objMapper.m_devicememorys.find(pPacket->pMemoryRanges[i].memory)->second;
        localRanges[i].memory = m_objMapper.remap_devicememorys(pPacket->pMemoryRanges[i].memory);
        if (localRanges[i].memory == VK_NULL_HANDLE || pLocalMems[i].pGpuMem == NULL)
        {
            vktrace_LogError("Skipping vkFlushMappedMemoryRanges() due to invalid remapped VkDeviceMemory.");
            VKTRACE_DELETE(localRanges);
            VKTRACE_DELETE(pLocalMems);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        if (!pLocalMems[i].pGpuMem->isPendingAlloc())
        {
            if (pPacket->pMemoryRanges[i].size != 0)
            {
#ifdef USE_PAGEGUARD_SPEEDUP
                if(vktrace_check_min_version(VKTRACE_TRACE_FILE_VERSION_5))
                    pLocalMems[i].pGpuMem->copyMappingDataPageGuard(pPacket->ppData[i]);
                else
                    pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size, (size_t)pPacket->pMemoryRanges[i].offset);
#else
                pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size, (size_t)pPacket->pMemoryRanges[i].offset);
#endif
            }
        }
        else
        {
            unsigned char *pBuf = (unsigned char *) vktrace_malloc(pLocalMems[i].pGpuMem->getMemoryMapSize());
            if (!pBuf)
            {
                vktrace_LogError("vkFlushMappedMemoryRanges() malloc failed.");
            }
            pLocalMems[i].pGpuMem->setMemoryDataAddr(pBuf);
#ifdef USE_PAGEGUARD_SPEEDUP
            if(vktrace_check_min_version(VKTRACE_TRACE_FILE_VERSION_5))
                pLocalMems[i].pGpuMem->copyMappingDataPageGuard(pPacket->ppData[i]);
            else
                pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size, (size_t)pPacket->pMemoryRanges[i].offset);
#else
            pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size, (size_t)pPacket->pMemoryRanges[i].offset);
#endif
        }
    }

#ifdef USE_PAGEGUARD_SPEEDUP
    replayResult = pPacket->result;//if this is a OPT refresh-all packet, we need avoid to call real api and return original return to avoid error message;
    if (!vktrace_check_min_version(VKTRACE_TRACE_FILE_VERSION_5) || !isvkFlushMappedMemoryRangesSpecial((PBYTE)pPacket->ppData[0]))
#endif
    {
        replayResult = m_vkFuncs.real_vkFlushMappedMemoryRanges(remappedDevice, pPacket->memoryRangeCount, localRanges);
    }

    VKTRACE_DELETE(localRanges);
    VKTRACE_DELETE(pLocalMems);

    return replayResult;
}

//InvalidateMappedMemory Ranges and flushMappedMemoryRanges are similar but keep it seperate until
//functionality tested fully
VkResult vkReplay::manually_replay_vkInvalidateMappedMemoryRanges(packet_vkInvalidateMappedMemoryRanges* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkInvalidateMappedMemoryRanges() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkMappedMemoryRange* localRanges = VKTRACE_NEW_ARRAY(VkMappedMemoryRange, pPacket->memoryRangeCount);
    memcpy(localRanges, pPacket->pMemoryRanges, sizeof(VkMappedMemoryRange) * (pPacket->memoryRangeCount));

    gpuMemObj* pLocalMems = VKTRACE_NEW_ARRAY(gpuMemObj, pPacket->memoryRangeCount);
    for (uint32_t i = 0; i < pPacket->memoryRangeCount; i++)
    {
        pLocalMems[i] = m_objMapper.m_devicememorys.find(pPacket->pMemoryRanges[i].memory)->second;
        localRanges[i].memory = m_objMapper.remap_devicememorys(pPacket->pMemoryRanges[i].memory);
        if (localRanges[i].memory == VK_NULL_HANDLE || pLocalMems[i].pGpuMem == NULL)
        {
            vktrace_LogError("Skipping vkInvalidsateMappedMemoryRanges() due to invalid remapped VkDeviceMemory.");
            VKTRACE_DELETE(localRanges);
            VKTRACE_DELETE(pLocalMems);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        if (!pLocalMems[i].pGpuMem->isPendingAlloc())
        {
            if (pPacket->pMemoryRanges[i].size != 0)
            {
                pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size, (size_t)pPacket->pMemoryRanges[i].offset);
            }
        }
        else
        {
            unsigned char *pBuf = (unsigned char *) vktrace_malloc(pLocalMems[i].pGpuMem->getMemoryMapSize());
            if (!pBuf)
            {
                vktrace_LogError("vkInvalidateMappedMemoryRanges() malloc failed.");
            }
            pLocalMems[i].pGpuMem->setMemoryDataAddr(pBuf);
            pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size, (size_t)pPacket->pMemoryRanges[i].offset);
        }
    }

    replayResult = m_vkFuncs.real_vkInvalidateMappedMemoryRanges(remappedDevice, pPacket->memoryRangeCount, localRanges);

    VKTRACE_DELETE(localRanges);
    VKTRACE_DELETE(pLocalMems);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfaceSupportKHR(packet_vkGetPhysicalDeviceSurfaceSupportKHR* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceSupportKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    replayResult = m_vkFuncs.real_vkGetPhysicalDeviceSurfaceSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex, remappedSurfaceKHR, pPacket->pSupported);

    return replayResult;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceMemoryProperties(packet_vkGetPhysicalDeviceMemoryProperties* pPacket)
{
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceMemoryProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    traceMemoryProperties[pPacket->physicalDevice] = *(pPacket->pMemoryProperties);
    m_vkFuncs.real_vkGetPhysicalDeviceMemoryProperties(remappedphysicalDevice, pPacket->pMemoryProperties);
    replayMemoryProperties[remappedphysicalDevice] = *(pPacket->pMemoryProperties);
    return;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceQueueFamilyProperties(packet_vkGetPhysicalDeviceQueueFamilyProperties* pPacket)
{
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceQueueFamilyProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    // If we haven't previously allocated queueFamilyProperties for the trace physical device, allocate it.
    // If we previously allocated queueFamilyProperities for the trace physical device and the size of this
    // query is larger than what we saved last time, then free the last properties map and allocate a new map.
    if (traceQueueFamilyProperties.find(pPacket->physicalDevice) == traceQueueFamilyProperties.end() ||
        *pPacket->pQueueFamilyPropertyCount > traceQueueFamilyProperties[pPacket->physicalDevice].count)
    {
        if (traceQueueFamilyProperties.find(pPacket->physicalDevice) != traceQueueFamilyProperties.end())
        {
            free(traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties);
            traceQueueFamilyProperties.erase(pPacket->physicalDevice);
        }
        if (pPacket->pQueueFamilyProperties)
        {
            traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties =
                (VkQueueFamilyProperties*)malloc(*pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            memcpy(traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties,
                   pPacket->pQueueFamilyProperties,
                   *pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            traceQueueFamilyProperties[pPacket->physicalDevice].count = *pPacket->pQueueFamilyPropertyCount;
        }
    }

    m_vkFuncs.real_vkGetPhysicalDeviceQueueFamilyProperties(remappedphysicalDevice, pPacket->pQueueFamilyPropertyCount, pPacket->pQueueFamilyProperties);

    // If we haven't previously allocated queueFamilyProperties for the replay physical device, allocate it.
    // If we previously allocated queueFamilyProperities for the replay physical device and the size of this
    // query is larger than what we saved last time, then free the last properties map and allocate a new map.
    if (replayQueueFamilyProperties.find(remappedphysicalDevice) == replayQueueFamilyProperties.end() ||
        *pPacket->pQueueFamilyPropertyCount > replayQueueFamilyProperties[remappedphysicalDevice].count)
    {
        if (replayQueueFamilyProperties.find(remappedphysicalDevice) != replayQueueFamilyProperties.end())
        {
            free(replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties);
            replayQueueFamilyProperties.erase(remappedphysicalDevice);
        }
        if (pPacket->pQueueFamilyProperties)
        {
            replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties =
                (VkQueueFamilyProperties*)malloc(*pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            memcpy(replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties,
                   pPacket->pQueueFamilyProperties,
                   *pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            replayQueueFamilyProperties[remappedphysicalDevice].count = *pPacket->pQueueFamilyPropertyCount;
        }
    }


    return;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(packet_vkGetPhysicalDeviceSurfaceCapabilitiesKHR* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceCapabilitiesKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceCapabilitiesKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    m_display->resize_window(pPacket->pSurfaceCapabilities->currentExtent.width, pPacket->pSurfaceCapabilities->currentExtent.height);

    replayResult = m_vkFuncs.real_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(remappedphysicalDevice, remappedSurfaceKHR, pPacket->pSurfaceCapabilities);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfaceFormatsKHR(packet_vkGetPhysicalDeviceSurfaceFormatsKHR* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceFormatsKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceFormatsKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    replayResult = m_vkFuncs.real_vkGetPhysicalDeviceSurfaceFormatsKHR(remappedphysicalDevice, remappedSurfaceKHR, pPacket->pSurfaceFormatCount, pPacket->pSurfaceFormats);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfacePresentModesKHR(packet_vkGetPhysicalDeviceSurfacePresentModesKHR* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfacePresentModesKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfacePresentModesKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    replayResult = m_vkFuncs.real_vkGetPhysicalDeviceSurfacePresentModesKHR(remappedphysicalDevice, remappedSurfaceKHR, pPacket->pPresentModeCount, pPacket->pPresentModes);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateSwapchainKHR(packet_vkCreateSwapchainKHR* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkSwapchainKHR local_pSwapchain;
    VkSwapchainKHR save_oldSwapchain, *pSC;
    VkSurfaceKHR save_surface;
    pSC = (VkSwapchainKHR *) &pPacket->pCreateInfo->oldSwapchain;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (remappeddevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    save_oldSwapchain = pPacket->pCreateInfo->oldSwapchain;
    (*pSC) = m_objMapper.remap_swapchainkhrs(save_oldSwapchain);
    if ((*pSC) == VK_NULL_HANDLE && save_oldSwapchain != VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkSwapchainKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    save_surface = pPacket->pCreateInfo->surface;
    VkSurfaceKHR *pSurf = (VkSurfaceKHR *) &(pPacket->pCreateInfo->surface);
    *pSurf = m_objMapper.remap_surfacekhrs(*pSurf);
    if (*pSurf == VK_NULL_HANDLE && pPacket->pCreateInfo->surface != VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    m_display->resize_window(pPacket->pCreateInfo->imageExtent.width, pPacket->pCreateInfo->imageExtent.height);

    // Convert queueFamilyIndices
    if (pPacket->pCreateInfo)
    {
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueFamilyIndexCount; i++)
        {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueFamilyIndices &&
                getQueueFamilyIdx(pPacket->device,
                                  remappeddevice,
                                  pPacket->pCreateInfo->pQueueFamilyIndices[i],
                                  &replayIdx))
                {
                    *((uint32_t*)&pPacket->pCreateInfo->pQueueFamilyIndices[i]) = replayIdx;
                }
            else {
                vktrace_LogError("vkSwapchainCreateInfoKHR, bad queueFamilyIndex");
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
    }

    replayResult = m_vkFuncs.real_vkCreateSwapchainKHR(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator, &local_pSwapchain);
    if (replayResult == VK_SUCCESS)
    {
        m_objMapper.add_to_swapchainkhrs_map(*(pPacket->pSwapchain), local_pSwapchain);
    }

    (*pSC) = save_oldSwapchain;
    *pSurf = save_surface;
    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetSwapchainImagesKHR(packet_vkGetSwapchainImagesKHR* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (remappeddevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkGetSwapchainImagesKHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSwapchainKHR remappedswapchain;
    remappedswapchain = m_objMapper.remap_swapchainkhrs(pPacket->swapchain);
    if (remappedswapchain == VK_NULL_HANDLE && pPacket->swapchain != VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkSwapchainKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkImage packetImage[128] = {0};
    uint32_t numImages = 0;
    if (pPacket->pSwapchainImages != NULL) {
        // Need to store the images and then add to map after we get actual image handles back
        VkImage* pPacketImages = (VkImage*)pPacket->pSwapchainImages;
        numImages = *(pPacket->pSwapchainImageCount);
        for (uint32_t i = 0; i < numImages; i++) {
            packetImage[i] = pPacketImages[i];
            traceImageToDevice[packetImage[i]] = pPacket->device;
        }
    }

    replayResult = m_vkFuncs.real_vkGetSwapchainImagesKHR(remappeddevice, remappedswapchain, pPacket->pSwapchainImageCount, pPacket->pSwapchainImages);
    if (replayResult == VK_SUCCESS)
    {
        if (numImages != 0) {
            VkImage* pReplayImages = (VkImage*)pPacket->pSwapchainImages;
            for (uint32_t i = 0; i < numImages; i++) {
                imageObj local_imageObj;
                local_imageObj.replayImage = pReplayImages[i];
                m_objMapper.add_to_images_map(packetImage[i], local_imageObj);
                replayImageToDevice[pReplayImages[i]] = remappeddevice;
            }
        }
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkQueuePresentKHR(packet_vkQueuePresentKHR* pPacket)
{
    VkResult replayResult = VK_SUCCESS;
    VkQueue remappedQueue = m_objMapper.remap_queues(pPacket->queue);
    if (remappedQueue == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkQueuePresentKHR() due to invalid remapped VkQueue.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSemaphore localSemaphores[5];
    VkSwapchainKHR localSwapchains[5];
    VkResult localResults[5];
    VkSemaphore *pRemappedWaitSems = localSemaphores;
    VkSwapchainKHR *pRemappedSwapchains = localSwapchains;
    VkResult *pResults = localResults;
    VkPresentInfoKHR present;
    uint32_t i;

    if (pPacket->pPresentInfo->swapchainCount > 5) {
        pRemappedSwapchains = VKTRACE_NEW_ARRAY(VkSwapchainKHR, pPacket->pPresentInfo->swapchainCount);
    }

    if (pPacket->pPresentInfo->swapchainCount > 5 && pPacket->pPresentInfo->pResults != NULL) {
        pResults = VKTRACE_NEW_ARRAY(VkResult, pPacket->pPresentInfo->swapchainCount);
    }

    if (pPacket->pPresentInfo->waitSemaphoreCount > 5) {
        pRemappedWaitSems = VKTRACE_NEW_ARRAY(VkSemaphore, pPacket->pPresentInfo->waitSemaphoreCount);
    }

    if (pRemappedSwapchains == NULL || pRemappedWaitSems == NULL || pResults == NULL)
    {
        replayResult = VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    if (replayResult == VK_SUCCESS) {
        for (i=0; i<pPacket->pPresentInfo->swapchainCount; i++) {
            pRemappedSwapchains[i] = m_objMapper.remap_swapchainkhrs(pPacket->pPresentInfo->pSwapchains[i]);
            if (pRemappedSwapchains[i] == VK_NULL_HANDLE)
            {
                vktrace_LogError("Skipping vkQueuePresentKHR() due to invalid remapped VkSwapchainKHR.");
                if (pRemappedWaitSems != NULL && pRemappedWaitSems != localSemaphores) {
                    VKTRACE_DELETE(pRemappedWaitSems);
                }
                if (pResults != NULL && pResults != localResults) {
                    VKTRACE_DELETE(pResults);
                }
                if (pRemappedSwapchains != NULL && pRemappedSwapchains != localSwapchains) {
                    VKTRACE_DELETE(pRemappedSwapchains);
                }
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
        present.sType = pPacket->pPresentInfo->sType;
        present.pNext = pPacket->pPresentInfo->pNext;
        present.swapchainCount = pPacket->pPresentInfo->swapchainCount;
        present.pSwapchains = pRemappedSwapchains;
        present.pImageIndices = pPacket->pPresentInfo->pImageIndices;
        present.waitSemaphoreCount = pPacket->pPresentInfo->waitSemaphoreCount;
        present.pWaitSemaphores = NULL;
        if (present.waitSemaphoreCount != 0) {
            present.pWaitSemaphores = pRemappedWaitSems;
            for (i = 0; i < pPacket->pPresentInfo->waitSemaphoreCount; i++) {
                (*(pRemappedWaitSems + i)) = m_objMapper.remap_semaphores((*(pPacket->pPresentInfo->pWaitSemaphores + i)));
                if (*(pRemappedWaitSems + i) == VK_NULL_HANDLE)
                {
                    vktrace_LogError("Skipping vkQueuePresentKHR() due to invalid remapped wait VkSemaphore.");
                    if (pRemappedWaitSems != NULL && pRemappedWaitSems != localSemaphores) {
                        VKTRACE_DELETE(pRemappedWaitSems);
                    }
                    if (pResults != NULL && pResults != localResults) {
                        VKTRACE_DELETE(pResults);
                    }
                    if (pRemappedSwapchains != NULL && pRemappedSwapchains != localSwapchains) {
                        VKTRACE_DELETE(pRemappedSwapchains);
                    }
                    return VK_ERROR_VALIDATION_FAILED_EXT;
                }
            }
        }
        present.pResults = NULL;
    }

    if (replayResult == VK_SUCCESS) {
        // If the application requested per-swapchain results, set up to get the results from the replay.
        if (pPacket->pPresentInfo->pResults != NULL) {
            present.pResults = pResults;
        }

        replayResult = m_vkFuncs.real_vkQueuePresentKHR(remappedQueue, &present);

        m_frameNumber++;

        // Compare the results from the trace file with those just received from the replay.  Report any differences.
        if (present.pResults != NULL) {
            for (i=0; i<pPacket->pPresentInfo->swapchainCount; i++) {
                if (present.pResults[i] != pPacket->pPresentInfo->pResults[i]) {
                    vktrace_LogError("Return value %s from API call (VkQueuePresentKHR) does not match return value from trace file %s for swapchain %d.",
                                     string_VkResult(present.pResults[i]), string_VkResult(pPacket->pPresentInfo->pResults[i]), i);
                }
            }
        }
    }

    if (pRemappedWaitSems != NULL && pRemappedWaitSems != localSemaphores) {
        VKTRACE_DELETE(pRemappedWaitSems);
    }
    if (pResults != NULL && pResults != localResults) {
        VKTRACE_DELETE(pResults);
    }
    if (pRemappedSwapchains != NULL && pRemappedSwapchains != localSwapchains) {
        VKTRACE_DELETE(pRemappedSwapchains);
    }

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateXcbSurfaceKHR(packet_vkCreateXcbSurfaceKHR* pPacket)
{
    VkResult replayResult;
    VkSurfaceKHR local_pSurface;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateXcbSurfaceKHR() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined PLATFORM_LINUX
    VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *) m_display->get_surface();
    VkXcbSurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.connection = pSurf->connection;
    createInfo.window = pSurf->window;
    replayResult = m_vkFuncs.real_vkCreateXcbSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#elif defined WIN32
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *) m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.real_vkCreateWin32SurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#else
    vktrace_LogError("manually_replay_vkCreateXcbSurfaceKHR not implemented on this vkreplay platform");
    replayResult = VK_FEATURE_NOT_PRESENT;
#endif

    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateXlibSurfaceKHR(packet_vkCreateXlibSurfaceKHR* pPacket)
{
    VkResult replayResult;
    VkSurfaceKHR local_pSurface;
    VkInstance remappedinstance = m_objMapper.remap_instances(pPacket->instance);

    if (pPacket->instance != VK_NULL_HANDLE && remappedinstance == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined PLATFORM_LINUX && defined VK_USE_PLATFORM_XLIB_KHR
    VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *) m_display->get_surface();
    VkXlibSurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.dpy = pSurf->dpy;
    createInfo.window = pSurf->window;
    replayResult = m_vkFuncs.real_vkCreateXlibSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#elif defined PLATFORM_LINUX && defined VK_USE_PLATFORM_XCB_KHR
    VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *) m_display->get_surface();
    VkXcbSurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.connection = pSurf->connection;
    createInfo.window = pSurf->window;
    replayResult = m_vkFuncs.real_vkCreateXcbSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#elif defined PLATFORM_LINUX
#error manually_replay_vkCreateXlibSurfaceKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XLIB_KHR or VK_USE_PLATFORM_XCB_KHR
#elif defined WIN32
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *) m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.real_vkCreateWin32SurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#else
    vktrace_LogError("manually_replay_vkCreateXlibSurfaceKHR not implemented on this playback platform");
    replayResult = VK_FEATURE_NOT_PRESENT;
#endif
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateWin32SurfaceKHR(packet_vkCreateWin32SurfaceKHR* pPacket)
{

    VkResult replayResult;
    VkSurfaceKHR local_pSurface;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateWin32SurfaceKHR() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined WIN32
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *) m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.real_vkCreateWin32SurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#elif defined PLATFORM_LINUX
    VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *) m_display->get_surface();
    VkXcbSurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.connection = pSurf->connection;
    createInfo.window = pSurf->window;
    replayResult = m_vkFuncs.real_vkCreateXcbSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#else
    vktrace_LogError("manually_replay_vkCreateWin32SurfaceKHR not implemented on this playback platform");
    replayResult = VK_FEATURE_NOT_PRESENT;
#endif
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult  vkReplay::manually_replay_vkCreateDebugReportCallbackEXT(packet_vkCreateDebugReportCallbackEXT* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDebugReportCallbackEXT local_msgCallback;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkCreateDebugReportCallbackEXT() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (!g_fpDbgMsgCallback || !m_vkFuncs.real_vkCreateDebugReportCallbackEXT) {
        // just eat this call as we don't have local call back function defined
        return VK_SUCCESS;
    } else
    {
        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
        memset(&dbgCreateInfo, 0, sizeof(dbgCreateInfo));
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        dbgCreateInfo.flags = pPacket->pCreateInfo->flags;
        dbgCreateInfo.pfnCallback = g_fpDbgMsgCallback;
        dbgCreateInfo.pUserData = NULL;
        replayResult = m_vkFuncs.real_vkCreateDebugReportCallbackEXT(remappedInstance, &dbgCreateInfo, NULL, &local_msgCallback);
        if (replayResult == VK_SUCCESS)
        {
                m_objMapper.add_to_debugreportcallbackexts_map(*(pPacket->pCallback), local_msgCallback);
        }
    }
    return replayResult;
}

void vkReplay::manually_replay_vkDestroyDebugReportCallbackEXT(packet_vkDestroyDebugReportCallbackEXT* pPacket)
{
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkDestroyDebugReportCallbackEXT() due to invalid remapped VkInstance.");
        return;
    }

    VkDebugReportCallbackEXT remappedMsgCallback;
    remappedMsgCallback = m_objMapper.remap_debugreportcallbackexts(pPacket->callback);
    if (remappedMsgCallback == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkDestroyDebugReportCallbackEXT() due to invalid remapped VkDebugReportCallbackEXT.");
        return;
    }

    if (!g_fpDbgMsgCallback)
    {
        // just eat this call as we don't have local call back function defined
        return;
    } else
    {
        m_vkFuncs.real_vkDestroyDebugReportCallbackEXT(remappedInstance, remappedMsgCallback, NULL);
    }
}

VkResult vkReplay::manually_replay_vkAllocateCommandBuffers(packet_vkAllocateCommandBuffers* pPacket)
{
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkAllocateCommandBuffers() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkCommandBuffer *local_pCommandBuffers = new VkCommandBuffer[pPacket->pAllocateInfo->commandBufferCount];
    VkCommandPool local_CommandPool;
    local_CommandPool = pPacket->pAllocateInfo->commandPool;
    ((VkCommandBufferAllocateInfo *) pPacket->pAllocateInfo)->commandPool = m_objMapper.remap_commandpools(pPacket->pAllocateInfo->commandPool);
    if (pPacket->pAllocateInfo->commandPool == VK_NULL_HANDLE)
    {
        vktrace_LogError("Skipping vkAllocateCommandBuffers() due to invalid remapped VkCommandPool.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    replayResult = m_vkFuncs.real_vkAllocateCommandBuffers(remappedDevice, pPacket->pAllocateInfo, local_pCommandBuffers);
    ((VkCommandBufferAllocateInfo *) pPacket->pAllocateInfo)->commandPool = local_CommandPool;

    if (replayResult == VK_SUCCESS)
    {
        for (uint32_t i = 0; i < pPacket->pAllocateInfo->commandBufferCount; i++) {
            m_objMapper.add_to_commandbuffers_map(pPacket->pCommandBuffers[i], local_pCommandBuffers[i]);
        }
    }
    delete local_pCommandBuffers;
    return replayResult;
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR(packet_vkGetPhysicalDeviceXcbPresentationSupportKHR* pPacket)
{
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Error detected in vkGetPhysicalDeviceXcbPresentationSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_FALSE;
    }

#if defined PLATFORM_LINUX
    VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *) m_display->get_surface();
    m_display->get_window_handle();
    return (m_vkFuncs.real_vkGetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex, pSurf->connection, m_display->get_screen_handle()->root_visual));
#elif defined WIN32
    return (m_vkFuncs.real_vkGetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR not implemented on this playback platform");
    return VK_FALSE;
#endif
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR(packet_vkGetPhysicalDeviceXlibPresentationSupportKHR* pPacket)
{
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE)
    {
        vktrace_LogError("Error detected in vkGetPhysicalDeviceXcbPresentationSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_FALSE;
    }

#if defined PLATFORM_LINUX && defined VK_USE_PLATFORM_XLIB_KHR
    VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *) m_display->get_surface();
    m_display->get_window_handle();
    return (m_vkFuncs.real_vkGetPhysicalDeviceXlibPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex, pSurf->dpy, m_display->get_screen_handle()->root_visual));
#elif defined PLATFORM_LINUX && defined VK_USE_PLATFORM_XCB_KHR
    VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *) m_display->get_surface();
    m_display->get_window_handle();
    return (m_vkFuncs.real_vkGetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex, pSurf->connection, m_display->get_screen_handle()->root_visual));
#elif defined PLATFORM_LINUX
#error manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XLIB_KHR or VK_USE_PLATFORM_XCB_KHR
#elif defined WIN32
    return (m_vkFuncs.real_vkGetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR not implemented on this playback platform");
    return VK_FALSE;
#endif
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR(packet_vkGetPhysicalDeviceWin32PresentationSupportKHR* pPacket)
{
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE)
    {
        return VK_FALSE;
    }

#if defined WIN32
    return (m_vkFuncs.real_vkGetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#elif defined PLATFORM_LINUX
    VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *) m_display->get_surface();
    m_display->get_window_handle();
    return (m_vkFuncs.real_vkGetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex, pSurf->connection, m_display->get_screen_handle()->root_visual));

#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR not implemented on this playback platform");
    return VK_FALSE;
#endif
}
