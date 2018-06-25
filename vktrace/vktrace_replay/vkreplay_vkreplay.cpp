/*
 *
 * Copyright (C) 2015-2018 Valve Corporation
 * Copyright (C) 2015-2018 LunarG, Inc.
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
 * Author: David Pinedo <david@lunarg.com>
 */

#include "vulkan/vulkan.h"
#include "vkreplay_vkreplay.h"
#include "vkreplay.h"
#include "vkreplay_settings.h"
#include "vkreplay_main.h"

#include <algorithm>

#include "vktrace_vk_vk_packets.h"
#include "vk_enum_string_helper.h"
#include "vktrace_vk_packet_id.h"
#include "vktrace_trace_packet_utils.h"

#if defined(_WIN32) || defined(_WIN64)
#define strcasecmp _stricmp  // Used for argument parsing
#endif

using namespace std;
#include "vktrace_pageguard_memorycopy.h"

vkreplayer_settings *g_pReplaySettings;

vkReplay::vkReplay(vkreplayer_settings *pReplaySettings, vktrace_trace_file_header *pFileHeader,
                   vktrace_replay::ReplayDisplayImp *display) {
    g_pReplaySettings = pReplaySettings;
    m_pDSDump = NULL;
    m_pCBDump = NULL;
    m_display = display;

    if (strcasecmp(pReplaySettings->displayServer, "xcb") == 0) {
        m_displayServer = VK_DISPLAY_XCB;
    } else if (strcasecmp(pReplaySettings->displayServer, "wayland") == 0) {
        m_displayServer = VK_DISPLAY_WAYLAND;
    }

    //    m_pVktraceSnapshotPrint = NULL;
    m_objMapper.m_adjustForGPU = false;

    m_frameNumber = 0;
    m_pFileHeader = pFileHeader;
    m_pGpuinfo = (struct_gpuinfo *)(pFileHeader + 1);
    m_platformMatch = -1;
}

std::vector<uint64_t> portabilityTable;
FileLike *traceFile;

vkReplay::~vkReplay() {
    delete m_display;
    vktrace_platform_close_library(m_libHandle);
}

int vkReplay::init(vktrace_replay::ReplayDisplay &disp) {
    int err;
#if defined(PLATFORM_LINUX)
    void *handle = dlopen("libvulkan.so", RTLD_LAZY);
#else
    HMODULE handle = LoadLibrary("vulkan-1.dll");
#endif

    if (handle == NULL) {
        vktrace_LogError("Failed to open vulkan library.");
        return -1;
    }
    init_funcs(handle);
    disp.set_implementation(m_display);
    if ((err = m_display->init(disp.get_gpu())) != 0) {
        vktrace_LogError("Failed to init vulkan display.");
        return err;
    }

    if ((err = m_display->create_window(disp.get_width(), disp.get_height())) != 0) {
        vktrace_LogError("Failed to create Window");
        return err;
    }

    m_replay_endianess = get_endianess();
    m_replay_ptrsize = sizeof(void *);
    m_replay_arch = get_arch();
    m_replay_os = get_os();
    // We save a value for m_replay_gpu and m_replay_drv_vers later when we replay vkGetPhysicalDeviceProperites
    m_replay_gpu = 0;
    m_replay_drv_vers = 0;

    // 32bit/64bit trace file is not supported by 64bit/32bit vkreplay
    if (m_replay_ptrsize != m_pFileHeader->ptrsize) {
        vktrace_LogError("%d-bit trace file is not supported by %d-bit vkreplay.", m_pFileHeader->ptrsize * 8,
                         m_replay_ptrsize * 8);
        return -1;
    }

    return 0;
}

vktrace_replay::VKTRACE_REPLAY_RESULT vkReplay::handle_replay_errors(const char *entrypointName, const VkResult resCall,
                                                                     const VkResult resTrace,
                                                                     const vktrace_replay::VKTRACE_REPLAY_RESULT resIn) {
    vktrace_replay::VKTRACE_REPLAY_RESULT res = resIn;
    if (resCall == VK_ERROR_DEVICE_LOST) {
        vktrace_LogError("API call %s returned VK_ERROR_DEVICE_LOST. vkreplay cannot continue, exiting.", entrypointName);
        exit(1);
    }
    if (resCall != resTrace) {
        vktrace_LogError("Return value %s from API call (%s) does not match return value from trace file %s.",
                         string_VkResult((VkResult)resCall), entrypointName, string_VkResult((VkResult)resTrace));
        res = vktrace_replay::VKTRACE_REPLAY_BAD_RETURN;
    }
    if (resCall != VK_SUCCESS && resCall != VK_NOT_READY) {
        vktrace_LogWarning("API call (%s) returned failed result %s", entrypointName, string_VkResult(resCall));
    }
    return res;
}
void vkReplay::push_validation_msg(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObjectHandle, size_t location,
                                   int32_t msgCode, const char *pLayerPrefix, const char *pMsg, const void *pUserData) {
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
    msgObj.pUserData = (void *)pUserData;
    m_validationMsgs.push_back(msgObj);
}

vktrace_replay::VKTRACE_REPLAY_RESULT vkReplay::pop_validation_msgs() {
    if (m_validationMsgs.size() == 0) return vktrace_replay::VKTRACE_REPLAY_SUCCESS;
    m_validationMsgs.clear();
    return vktrace_replay::VKTRACE_REPLAY_VALIDATION_ERROR;
}

int vkReplay::dump_validation_data() {
    if (m_pDSDump && m_pCBDump) {
        m_pDSDump((char *)"pipeline_dump.dot");
        m_pCBDump((char *)"cb_dump.dot");
    }
    //    if (m_pVktraceSnapshotPrint != NULL)
    //    {
    //        m_pVktraceSnapshotPrint();
    //    }
    return 0;
}

VkResult vkReplay::manually_replay_vkCreateInstance(packet_vkCreateInstance *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkInstanceCreateInfo *pCreateInfo;
    char **ppEnabledLayerNames = NULL, **saved_ppLayers = NULL;
    uint32_t savedLayerCount = 0;
    if (!m_display->m_initedVK) {
        VkInstance inst;

        const char strScreenShot[] = "VK_LAYER_LUNARG_screenshot";
        pCreateInfo = (VkInstanceCreateInfo *)pPacket->pCreateInfo;
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
                vkEnumerateInstanceLayerProperties(&count, NULL);
                VkLayerProperties *props = (VkLayerProperties *)vktrace_malloc(count * sizeof(VkLayerProperties));
                if (props && count > 0) vkEnumerateInstanceLayerProperties(&count, props);
                for (uint32_t i = 0; i < count; i++) {
                    if (!strcmp(props[i].layerName, strScreenShot)) {
                        found_ss = true;
                        break;
                    }
                }
                if (found_ss) {
                    // screenshot layer is available so enable it
                    savedLayerCount = pCreateInfo->enabledLayerCount;
                    ppEnabledLayerNames = (char **)vktrace_malloc((pCreateInfo->enabledLayerCount + 1) * sizeof(char *));
                    for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount && ppEnabledLayerNames; i++) {
                        ppEnabledLayerNames[i] = (char *)pCreateInfo->ppEnabledLayerNames[i];
                    }
                    ppEnabledLayerNames[pCreateInfo->enabledLayerCount] = (char *)vktrace_malloc(strlen(strScreenShot) + 1);
                    strcpy(ppEnabledLayerNames[pCreateInfo->enabledLayerCount++], strScreenShot);
                    saved_ppLayers = (char **)pCreateInfo->ppEnabledLayerNames;
                    pCreateInfo->ppEnabledLayerNames = ppEnabledLayerNames;
                }
                vktrace_free(props);
            }
        }

        char **saved_ppExtensions = (char **)pCreateInfo->ppEnabledExtensionNames;
        uint32_t savedExtensionCount = pCreateInfo->enabledExtensionCount;
        vector<const char *> extension_names;
        vector<string> outlist;

#if defined(PLATFORM_LINUX)
#if !defined(ANDROID)
        outlist.push_back("VK_KHR_android_surface");
#if defined(VK_KHR_XCB_SURFACE_EXTENSION_NAME)
        extension_names.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
#if defined(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME)
        extension_names.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
        outlist.push_back("VK_KHR_xlib_surface");
        outlist.push_back("VK_KHR_wayland_surface");
        outlist.push_back("VK_KHR_win32_surface");
#else
        extension_names.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
        outlist.push_back("VK_KHR_win32_surface");
        outlist.push_back("VK_KHR_xlib_surface");
        outlist.push_back("VK_KHR_xcb_surface");
        outlist.push_back("VK_KHR_wayland_surface");
        outlist.push_back("VK_KHR_mir_surface");
#endif  // ANDROID
#else
        extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        outlist.push_back("VK_KHR_xlib_surface");
        outlist.push_back("VK_KHR_xcb_surface");
        outlist.push_back("VK_KHR_wayland_surface");
        outlist.push_back("VK_KHR_mir_surface");
#endif

        // Add any extensions that are both replayable and in the packet
        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            if (find(outlist.begin(), outlist.end(), pCreateInfo->ppEnabledExtensionNames[i]) == outlist.end()) {
                extension_names.push_back(pCreateInfo->ppEnabledExtensionNames[i]);
            }
        }
        pCreateInfo->ppEnabledExtensionNames = extension_names.data();
        pCreateInfo->enabledExtensionCount = (uint32_t)extension_names.size();

        replayResult = vkCreateInstance(pPacket->pCreateInfo, NULL, &inst);

        if (replayResult == VK_SUCCESS) {
            m_objMapper.add_to_instances_map(*(pPacket->pInstance), inst);

            // Build instance dispatch table
            layer_init_instance_dispatch_table(inst, &m_vkFuncs, m_vkFuncs.GetInstanceProcAddr);
            // Not handled by codegen
            m_vkFuncs.CreateDevice = (PFN_vkCreateDevice)m_vkFuncs.GetInstanceProcAddr(inst, "vkCreateDevice");
        } else if (replayResult == VK_ERROR_LAYER_NOT_PRESENT) {
            vktrace_LogVerbose("vkCreateInstance failed with VK_ERROR_LAYER_NOT_PRESENT");
            vktrace_LogVerbose("List of requested layers:");
            for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; i++) {
                vktrace_LogVerbose("   %s", pCreateInfo->ppEnabledLayerNames[i]);
            }
        } else if (replayResult == VK_ERROR_EXTENSION_NOT_PRESENT) {
            vktrace_LogVerbose("vkCreateInstance failed with VK_ERROR_EXTENSION_NOT_PRESENT");
            vktrace_LogVerbose("List of requested extensions:");
            for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
                vktrace_LogVerbose("   %s", pCreateInfo->ppEnabledExtensionNames[i]);
            }
        }

        pCreateInfo->ppEnabledExtensionNames = saved_ppExtensions;
        pCreateInfo->enabledExtensionCount = savedExtensionCount;

        if (ppEnabledLayerNames) {
            // restore the packets CreateInfo struct
            vktrace_free(ppEnabledLayerNames[pCreateInfo->enabledLayerCount - 1]);
            vktrace_free(ppEnabledLayerNames);
            pCreateInfo->ppEnabledLayerNames = saved_ppLayers;
            pCreateInfo->enabledLayerCount = savedLayerCount;
        }
    }
    return replayResult;
}

bool vkReplay::getQueueFamilyIdx(VkPhysicalDevice tracePhysicalDevice, VkPhysicalDevice replayPhysicalDevice, uint32_t traceIdx,
                                 uint32_t *pReplayIdx) {
    // Don't translate queue family index if the platform matches or we are not in compatibility mode
    if (platformMatch() || !g_pReplaySettings->compatibilityMode) {
        *pReplayIdx = traceIdx;
        return true;
    }
    if (traceIdx == VK_QUEUE_FAMILY_IGNORED) {
        *pReplayIdx = VK_QUEUE_FAMILY_IGNORED;
        return true;
    }

    // If either the trace qf list or replay qf list is empty, fail
    if (traceQueueFamilyProperties.find(tracePhysicalDevice) == traceQueueFamilyProperties.end() ||
        replayQueueFamilyProperties.find(replayPhysicalDevice) == replayQueueFamilyProperties.end()) {
        goto fail;
    }
    if (min(traceQueueFamilyProperties[tracePhysicalDevice].count, replayQueueFamilyProperties[replayPhysicalDevice].count) == 0) {
        goto fail;
    }

    // If there is exactly one qf in the replay list, use it
    if (replayQueueFamilyProperties[replayPhysicalDevice].count == 1) {
        *pReplayIdx = 0;
        return true;
    }

    // If there is a replay qf that is a identical to the trace qf, use it
    for (uint32_t i = 0; i < replayQueueFamilyProperties[replayPhysicalDevice].count; i++) {
        if (traceQueueFamilyProperties[tracePhysicalDevice].queueFamilyProperties[traceIdx].queueFlags ==
            replayQueueFamilyProperties[replayPhysicalDevice].queueFamilyProperties[i].queueFlags) {
            *pReplayIdx = i;
            return true;
        }
    }

    // If there is a replay qf that is a superset of the trace qf, us it
    for (uint32_t i = 0; i < replayQueueFamilyProperties[replayPhysicalDevice].count; i++) {
        if (traceQueueFamilyProperties[tracePhysicalDevice].queueFamilyProperties[traceIdx].queueFlags ==
            (traceQueueFamilyProperties[tracePhysicalDevice].queueFamilyProperties[traceIdx].queueFlags &
             replayQueueFamilyProperties[replayPhysicalDevice].queueFamilyProperties[i].queueFlags)) {
            *pReplayIdx = i;
            return true;
        }
    }

    // If there is a replay qf that supports Graphics, Compute and Transfer, use it
    // If there is a replay qf that supports Graphics and Compute, use it
    // If there is a replay qf that supports Graphics, use it
    for (uint32_t j = 0; j < 3; j++) {
        uint32_t mask;
        if (j == 0)
            mask = (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);
        else if (j == 1)
            mask = (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
        else
            // j == 2
            mask = (VK_QUEUE_GRAPHICS_BIT);
        for (uint32_t i = 0; i < replayQueueFamilyProperties[replayPhysicalDevice].count; i++) {
            if ((replayQueueFamilyProperties[replayPhysicalDevice].queueFamilyProperties[i].queueFlags & mask) == mask) {
                vktrace_LogWarning("Didn't find an exact match for queue family index, using index %d", i);
                *pReplayIdx = i;
                return true;
            }
        }
    }

fail:
    // Didn't find a match
    vktrace_LogError("Cannot determine replay device queue family index to use");
    return false;
}

bool vkReplay::getQueueFamilyIdx(VkDevice traceDevice, VkDevice replayDevice, uint32_t traceIdx, uint32_t *pReplayIdx) {
    VkPhysicalDevice tracePhysicalDevice;
    VkPhysicalDevice replayPhysicalDevice;

    if (tracePhysicalDevices.find(traceDevice) == tracePhysicalDevices.end() ||
        replayPhysicalDevices.find(replayDevice) == replayPhysicalDevices.end()) {
        vktrace_LogWarning("Cannot determine queue family index - has vkGetPhysicalDeviceQueueFamilyProperties been called?");
        return false;
    }

    tracePhysicalDevice = tracePhysicalDevices[traceDevice];
    replayPhysicalDevice = replayPhysicalDevices[replayDevice];

    return getQueueFamilyIdx(tracePhysicalDevice, replayPhysicalDevice, traceIdx, pReplayIdx);
}

VkResult vkReplay::manually_replay_vkCreateDevice(packet_vkCreateDevice *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    for (uint32_t i = 0; i < pPacket->pCreateInfo->queueCreateInfoCount; i++)
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)&pPacket->pCreateInfo->pQueueCreateInfos[i]);
    if (!m_display->m_initedVK) {
        VkDevice device;
        VkPhysicalDevice remappedPhysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
        VkDeviceCreateInfo *pCreateInfo;
        char **ppEnabledLayerNames = NULL, **saved_ppLayers = NULL;
        if (remappedPhysicalDevice == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCreateDevice() due to invalid remapped VkPhysicalDevice.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        const char strScreenShot[] = "VK_LAYER_LUNARG_screenshot";

        pCreateInfo = (VkDeviceCreateInfo *)pPacket->pCreateInfo;
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
                m_vkFuncs.EnumerateDeviceLayerProperties(remappedPhysicalDevice, &count, NULL);
                VkLayerProperties *props = (VkLayerProperties *)vktrace_malloc(count * sizeof(VkLayerProperties));
                if (props && count > 0) m_vkFuncs.EnumerateDeviceLayerProperties(remappedPhysicalDevice, &count, props);
                for (uint32_t i = 0; i < count; i++) {
                    if (!strcmp(props[i].layerName, strScreenShot)) {
                        found_ss = true;
                        break;
                    }
                }
                if (found_ss) {
                    // screenshot layer is available so enable it
                    ppEnabledLayerNames = (char **)vktrace_malloc((pCreateInfo->enabledLayerCount + 1) * sizeof(char *));
                    for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount && ppEnabledLayerNames; i++) {
                        ppEnabledLayerNames[i] = (char *)pCreateInfo->ppEnabledLayerNames[i];
                    }
                    ppEnabledLayerNames[pCreateInfo->enabledLayerCount] = (char *)vktrace_malloc(strlen(strScreenShot) + 1);
                    strcpy(ppEnabledLayerNames[pCreateInfo->enabledLayerCount++], strScreenShot);
                    saved_ppLayers = (char **)pCreateInfo->ppEnabledLayerNames;
                    pCreateInfo->ppEnabledLayerNames = ppEnabledLayerNames;
                }
                vktrace_free(props);
            }
        }

        // Convert all instances of queueFamilyIndex in structure
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueCreateInfoCount; i++) {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueCreateInfos &&
                getQueueFamilyIdx(pPacket->physicalDevice, remappedPhysicalDevice,
                                  pPacket->pCreateInfo->pQueueCreateInfos->queueFamilyIndex, &replayIdx)) {
                *((uint32_t *)&pPacket->pCreateInfo->pQueueCreateInfos->queueFamilyIndex) = replayIdx;
            } else {
                vktrace_LogError("vkCreateDevice failed, bad queueFamilyIndex");
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }

        replayResult = m_vkFuncs.CreateDevice(remappedPhysicalDevice, pPacket->pCreateInfo, NULL, &device);
        if (ppEnabledLayerNames) {
            // restore the packets CreateInfo struct
            vktrace_free(ppEnabledLayerNames[pCreateInfo->enabledLayerCount - 1]);
            vktrace_free(ppEnabledLayerNames);
            pCreateInfo->ppEnabledLayerNames = saved_ppLayers;
        }
        if (replayResult == VK_SUCCESS) {
            m_objMapper.add_to_devices_map(*(pPacket->pDevice), device);
            tracePhysicalDevices[*(pPacket->pDevice)] = pPacket->physicalDevice;
            replayPhysicalDevices[device] = remappedPhysicalDevice;

            // Build device dispatch table
            layer_init_device_dispatch_table(device, &m_vkDeviceFuncs, m_vkDeviceFuncs.GetDeviceProcAddr);
        } else if (replayResult == VK_ERROR_EXTENSION_NOT_PRESENT) {
            vktrace_LogVerbose("vkCreateDevice failed with VK_ERROR_EXTENSION_NOT_PRESENT");
            vktrace_LogVerbose("List of requested extensions:");
            for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
                vktrace_LogVerbose("   %s", pCreateInfo->ppEnabledExtensionNames[i]);
            }
        }
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateBuffer(packet_vkCreateBuffer *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    bufferObj local_bufferObj;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // Convert queueFamilyIndices
    if (pPacket->pCreateInfo) {
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueFamilyIndexCount; i++) {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueFamilyIndices &&
                getQueueFamilyIdx(pPacket->device, remappedDevice, pPacket->pCreateInfo->pQueueFamilyIndices[i], &replayIdx)) {
                *((uint32_t *)&pPacket->pCreateInfo->pQueueFamilyIndices[i]) = replayIdx;
            } else {
                vktrace_LogError("vkCreateBuffer failed, bad queueFamilyIndex");
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
    }

    replayResult = m_vkDeviceFuncs.CreateBuffer(remappedDevice, pPacket->pCreateInfo, NULL, &local_bufferObj.replayBuffer);
    if (replayResult == VK_SUCCESS) {
        traceBufferToDevice[*pPacket->pBuffer] = pPacket->device;
        replayBufferToDevice[local_bufferObj.replayBuffer] = remappedDevice;
        m_objMapper.add_to_buffers_map(*(pPacket->pBuffer), local_bufferObj);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateImage(packet_vkCreateImage *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    imageObj local_imageObj;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // Convert queueFamilyIndices
    if (pPacket->pCreateInfo) {
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueFamilyIndexCount; i++) {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueFamilyIndices &&
                getQueueFamilyIdx(pPacket->device, remappedDevice, pPacket->pCreateInfo->pQueueFamilyIndices[i], &replayIdx)) {
                *((uint32_t *)&pPacket->pCreateInfo->pQueueFamilyIndices[i]) = replayIdx;
            } else {
                vktrace_LogError("vkCreateImage failed, bad queueFamilyIndex");
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
    }

    replayResult = m_vkDeviceFuncs.CreateImage(remappedDevice, pPacket->pCreateInfo, NULL, &local_imageObj.replayImage);
    if (replayResult == VK_SUCCESS) {
        traceImageToDevice[*pPacket->pImage] = pPacket->device;
        replayImageToDevice[local_imageObj.replayImage] = remappedDevice;
        m_objMapper.add_to_images_map(*(pPacket->pImage), local_imageObj);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateCommandPool(packet_vkCreateCommandPool *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkCommandPool local_pCommandPool;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // No need to remap pAllocator

    // Convert queueFamilyIndex
    if (pPacket->pCreateInfo) {
        uint32_t replayIdx;
        if (getQueueFamilyIdx(pPacket->device, remappeddevice, pPacket->pCreateInfo->queueFamilyIndex, &replayIdx)) {
            *((uint32_t *)&pPacket->pCreateInfo->queueFamilyIndex) = replayIdx;
        } else {
            vktrace_LogError("vkCreateCommandPool failed, bad queueFamilyIndex");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }

    replayResult =
        m_vkDeviceFuncs.CreateCommandPool(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator, &local_pCommandPool);
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_commandpools_map(*(pPacket->pCommandPool), local_pCommandPool);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkEnumeratePhysicalDevices(packet_vkEnumeratePhysicalDevices *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    if (!m_display->m_initedVK) {
        uint32_t deviceCount = *(pPacket->pPhysicalDeviceCount);
        VkPhysicalDevice *pDevices = pPacket->pPhysicalDevices;

        VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
        if (remappedInstance == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkEnumeratePhysicalDevices() due to invalid remapped VkInstance.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        if (pPacket->pPhysicalDevices != NULL) {
            // If we are querying for the list instead of the count, use a previously acquired count
            deviceCount = m_gpu_count;
            pDevices = VKTRACE_NEW_ARRAY(VkPhysicalDevice, deviceCount);
        }
        replayResult = m_vkFuncs.EnumeratePhysicalDevices(remappedInstance, &deviceCount, pDevices);

        if (pDevices == NULL) {
            // If we are querying for the count, store it for later
            m_gpu_count = deviceCount;
        }

        if (deviceCount != *(pPacket->pPhysicalDeviceCount)) {
            vktrace_LogWarning("Number of physical devices mismatched in replay %u versus trace %u.", deviceCount,
                               *(pPacket->pPhysicalDeviceCount));
        } else if (deviceCount == 0) {
            vktrace_LogError("vkEnumeratePhysicalDevices number of gpus is zero.");
        } else if (pDevices != NULL) {
            vktrace_LogVerbose("Enumerated %d physical devices in the system.", deviceCount);
        }

        if (pDevices != NULL) {
            const uint32_t replay_device_count = deviceCount;
            uint64_t *replay_device_id = VKTRACE_NEW_ARRAY(uint64_t, replay_device_count);
            for (uint32_t i = 0; i < replay_device_count; ++i) {
                VkPhysicalDeviceProperties props;
                m_vkFuncs.GetPhysicalDeviceProperties(pDevices[i], &props);
                replay_device_id[i] = ((uint64_t)props.vendorID << 32) | (uint64_t)props.deviceID;
            }

            const uint32_t trace_device_count = *pPacket->pPhysicalDeviceCount;

            for (uint32_t i = 0; i < trace_device_count; i++) {
                // TODO: Pick a device based on matching properties. Might have to move this logic
                // First, check if device on the same index has matching vendor and device ID
                if (i < replay_device_count && m_pGpuinfo[i].gpu_id == replay_device_id[i]) {
                    m_objMapper.add_to_physicaldevices_map(pPacket->pPhysicalDevices[i], pDevices[i]);
                } else {
                    // Search the list for a matching device
                    bool found = false;
                    for (uint32_t j = 0; j < replay_device_count; ++j) {
                        if (j == i) {
                            continue;  // Already checked this
                        }
                        if (m_pGpuinfo[i].gpu_id == replay_device_id[j]) {
                            m_objMapper.add_to_physicaldevices_map(pPacket->pPhysicalDevices[i], pDevices[j]);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        // If all else fails, just map the indices.
                        if (i >= replay_device_count) {
                            m_objMapper.add_to_physicaldevices_map(pPacket->pPhysicalDevices[i], pDevices[0]);
                        } else {
                            m_objMapper.add_to_physicaldevices_map(pPacket->pPhysicalDevices[i], pDevices[i]);
                        }
                    }
                }
            }

            VKTRACE_DELETE(replay_device_id);
        }
        VKTRACE_DELETE(pDevices);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkDestroyBuffer(packet_vkDestroyBuffer *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkDestroyBuffer() due to invalid remapped VkDevice.");
        return;
    }
    VkBuffer remappedBuffer = m_objMapper.remap_buffers(pPacket->buffer);
    if (pPacket->buffer != VK_NULL_HANDLE && remappedBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkDestroyBuffer() due to invalid remapped VkBuffer.");
        return;
    }
    m_vkDeviceFuncs.DestroyBuffer(remappedDevice, remappedBuffer, pPacket->pAllocator);
    m_objMapper.rm_from_buffers_map(pPacket->buffer);
    if (replayGetBufferMemoryRequirements.find(remappedBuffer) != replayGetBufferMemoryRequirements.end())
        replayGetBufferMemoryRequirements.erase(remappedBuffer);
    return;
}

void vkReplay::manually_replay_vkDestroyImage(packet_vkDestroyImage *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkDestroyImage() due to invalid remapped VkDevice.");
        return;
    }
    VkImage remappedImage = m_objMapper.remap_images(pPacket->image);
    if (pPacket->image != VK_NULL_HANDLE && remappedImage == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkDestroyImage() due to invalid remapped VkImage.");
        return;
    }
    m_vkDeviceFuncs.DestroyImage(remappedDevice, remappedImage, pPacket->pAllocator);
    m_objMapper.rm_from_images_map(pPacket->image);
    if (replayGetImageMemoryRequirements.find(remappedImage) != replayGetImageMemoryRequirements.end())
        replayGetImageMemoryRequirements.erase(remappedImage);
    return;
}

VkResult vkReplay::manually_replay_vkQueueSubmit(packet_vkQueueSubmit *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkQueue remappedQueue = m_objMapper.remap_queues(pPacket->queue);
    if (remappedQueue == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped VkQueue.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFence remappedFence = m_objMapper.remap_fences(pPacket->fence);
    if (pPacket->fence != VK_NULL_HANDLE && remappedFence == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSubmitInfo *remappedSubmits = NULL;
    remappedSubmits = VKTRACE_NEW_ARRAY(VkSubmitInfo, pPacket->submitCount);
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
            pRemappedBuffers = VKTRACE_NEW_ARRAY(VkCommandBuffer, submit->commandBufferCount);
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
    replayResult = m_vkDeviceFuncs.QueueSubmit(remappedQueue, pPacket->submitCount, remappedSubmits, remappedFence);
    VKTRACE_DELETE(pRemappedBuffers);
    VKTRACE_DELETE(pRemappedWaitSems);
    VKTRACE_DELETE(pRemappedSignalSems);
    VKTRACE_DELETE(remappedSubmits);
    return replayResult;
}

VkResult vkReplay::manually_replay_vkQueueBindSparse(packet_vkQueueBindSparse *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkQueue remappedQueue = m_objMapper.remap_queues(pPacket->queue);
    if (remappedQueue == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkQueue.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFence remappedFence = m_objMapper.remap_fences(pPacket->fence);
    if (pPacket->fence != VK_NULL_HANDLE && remappedFence == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkBindSparseInfo *remappedBindSparseInfos = VKTRACE_NEW_ARRAY(VkBindSparseInfo, pPacket->bindInfoCount);
    VkSparseImageMemoryBind *pRemappedImageMemories = NULL;
    VkSparseMemoryBind *pRemappedBufferMemories = NULL;
    VkSparseMemoryBind *pRemappedImageOpaqueMemories = NULL;
    VkSemaphore *pRemappedWaitSems = NULL;
    VkSemaphore *pRemappedSignalSems = NULL;
    VkSparseImageMemoryBindInfo *sIMBinf = NULL;
    VkSparseBufferMemoryBindInfo *sBMBinf = NULL;
    VkSparseImageOpaqueMemoryBindInfo *sIMOBinf = NULL;

    memcpy((void *)remappedBindSparseInfos, (void *)(pPacket->pBindInfo), sizeof(VkBindSparseInfo) * pPacket->bindInfoCount);

    for (uint32_t bindInfo_idx = 0; bindInfo_idx < pPacket->bindInfoCount; bindInfo_idx++) {
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)&remappedBindSparseInfos[bindInfo_idx]);

        if (remappedBindSparseInfos[bindInfo_idx].pBufferBinds) {
            sBMBinf = VKTRACE_NEW_ARRAY(VkSparseBufferMemoryBindInfo, remappedBindSparseInfos[bindInfo_idx].bufferBindCount);
            remappedBindSparseInfos[bindInfo_idx].pBufferBinds =
                (const VkSparseBufferMemoryBindInfo *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pBufferBinds));
            memcpy((void *)sBMBinf, (void *)remappedBindSparseInfos[bindInfo_idx].pBufferBinds,
                   sizeof(VkSparseBufferMemoryBindInfo) * remappedBindSparseInfos[bindInfo_idx].bufferBindCount);

            sBMBinf->buffer = m_objMapper.remap_buffers(sBMBinf->buffer);

            if (sBMBinf->buffer == VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkBuffer.");
                goto FAILURE;
            }

            if (sBMBinf->bindCount > 0 && sBMBinf->pBinds) {
                pRemappedBufferMemories = (VkSparseMemoryBind *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pBufferBinds->pBinds));
            }

            for (uint32_t bindCountIdx = 0; bindCountIdx < sBMBinf->bindCount; bindCountIdx++) {
                devicememoryObj local_mem = m_objMapper.m_devicememorys.find(pRemappedBufferMemories[bindCountIdx].memory)->second;
                VkDeviceMemory replay_mem = m_objMapper.remap_devicememorys(pRemappedBufferMemories[bindCountIdx].memory);

                if (replay_mem == VK_NULL_HANDLE || local_mem.pGpuMem == NULL) {
                    vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkDeviceMemory.");
                    goto FAILURE;
                }
                pRemappedBufferMemories[bindCountIdx].memory = replay_mem;
            }
            sBMBinf->pBinds = pRemappedBufferMemories;
            remappedBindSparseInfos[bindInfo_idx].pBufferBinds = sBMBinf;
        }

        if (remappedBindSparseInfos[bindInfo_idx].pImageBinds) {
            sIMBinf = VKTRACE_NEW_ARRAY(VkSparseImageMemoryBindInfo, remappedBindSparseInfos[bindInfo_idx].imageBindCount);
            remappedBindSparseInfos[bindInfo_idx].pImageBinds =
                (const VkSparseImageMemoryBindInfo *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageBinds));
            memcpy((void *)sIMBinf, (void *)remappedBindSparseInfos[bindInfo_idx].pImageBinds,
                   sizeof(VkSparseImageMemoryBindInfo) * remappedBindSparseInfos[bindInfo_idx].imageBindCount);

            sIMBinf->image = m_objMapper.remap_images(sIMBinf->image);

            if (sIMBinf->image == VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkImage.");
                goto FAILURE;
            }

            if (sIMBinf->bindCount > 0 && sIMBinf->pBinds) {
                pRemappedImageMemories = (VkSparseImageMemoryBind *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageBinds->pBinds));
            }
            for (uint32_t bindCountIdx = 0; bindCountIdx < sIMBinf->bindCount; bindCountIdx++) {
                devicememoryObj local_mem = m_objMapper.m_devicememorys.find(pRemappedImageMemories[bindCountIdx].memory)->second;
                VkDeviceMemory replay_mem = m_objMapper.remap_devicememorys(pRemappedImageMemories[bindCountIdx].memory);

                if (replay_mem == VK_NULL_HANDLE || local_mem.pGpuMem == NULL) {
                    vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkDeviceMemory.");
                    goto FAILURE;
                }
                pRemappedImageMemories[bindCountIdx].memory = replay_mem;
            }
            sIMBinf->pBinds = pRemappedImageMemories;
            remappedBindSparseInfos[bindInfo_idx].pImageBinds = sIMBinf;
        }

        if (remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds) {
            sIMOBinf =
                VKTRACE_NEW_ARRAY(VkSparseImageOpaqueMemoryBindInfo, remappedBindSparseInfos[bindInfo_idx].imageOpaqueBindCount);
            remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds =
                (const VkSparseImageOpaqueMemoryBindInfo *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds));
            memcpy((void *)sIMOBinf, (void *)remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds,
                   sizeof(VkSparseImageOpaqueMemoryBindInfo) * remappedBindSparseInfos[bindInfo_idx].imageOpaqueBindCount);

            sIMOBinf->image = m_objMapper.remap_images(sIMOBinf->image);

            if (sIMOBinf->image == VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkQueueBindSparse() due to invalid remapped VkImage.");
                goto FAILURE;
            }

            if (sIMOBinf->bindCount > 0 && sIMOBinf->pBinds) {
                pRemappedImageOpaqueMemories = (VkSparseMemoryBind *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)remappedBindSparseInfos[bindInfo_idx].pImageOpaqueBinds->pBinds));
            }
            for (uint32_t bindCountIdx = 0; bindCountIdx < sIMOBinf->bindCount; bindCountIdx++) {
                devicememoryObj local_mem =
                    m_objMapper.m_devicememorys.find(pRemappedImageOpaqueMemories[bindCountIdx].memory)->second;
                VkDeviceMemory replay_mem = m_objMapper.remap_devicememorys(pRemappedImageOpaqueMemories[bindCountIdx].memory);

                if (replay_mem == VK_NULL_HANDLE || local_mem.pGpuMem == NULL) {
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
                (*(pRemappedWaitSems + i)) =
                    m_objMapper.remap_semaphores((*(remappedBindSparseInfos[bindInfo_idx].pWaitSemaphores + i)));
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
                (*(pRemappedSignalSems + i)) =
                    m_objMapper.remap_semaphores((*(remappedBindSparseInfos[bindInfo_idx].pSignalSemaphores + i)));
                if (*(pRemappedSignalSems + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueueSubmit() due to invalid remapped signal VkSemaphore.");
                    goto FAILURE;
                }
            }
        }
    }

    replayResult = m_vkDeviceFuncs.QueueBindSparse(remappedQueue, pPacket->bindInfoCount, remappedBindSparseInfos, remappedFence);

FAILURE:
    VKTRACE_DELETE(remappedBindSparseInfos);
    VKTRACE_DELETE(sIMBinf);
    VKTRACE_DELETE(sBMBinf);
    VKTRACE_DELETE(sIMOBinf);
    VKTRACE_DELETE(pRemappedSignalSems);
    VKTRACE_DELETE(pRemappedWaitSems);
    return replayResult;
}

void vkReplay::manually_replay_vkUpdateDescriptorSets(packet_vkUpdateDescriptorSets *pPacket) {
    // We have to remap handles internal to the structures so save the handles prior to remap and then restore
    // Rather than doing a deep memcpy of the entire struct and fixing any intermediate pointers, do save and restores via STL queue

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkDevice.");
        return;
    }

    // allocate a new array for the writes and clear the memory, we'll update the contents further down
    VkWriteDescriptorSet *pRemappedWrites = NULL;
    if (pPacket->descriptorWriteCount > 0) {
        pRemappedWrites = VKTRACE_NEW_ARRAY(VkWriteDescriptorSet, pPacket->descriptorWriteCount);
        memset(pRemappedWrites, 0, pPacket->descriptorWriteCount * sizeof(VkWriteDescriptorSet));
    }

    // allocate a new array for the copies, and simply copy the original data in since there are no pointers to update.
    VkCopyDescriptorSet *pRemappedCopies = NULL;
    if (pPacket->descriptorCopyCount > 0) {
        pRemappedCopies = VKTRACE_NEW_ARRAY(VkCopyDescriptorSet, pPacket->descriptorCopyCount);
        memcpy(pRemappedCopies, pPacket->pDescriptorCopies, pPacket->descriptorCopyCount * sizeof(VkCopyDescriptorSet));
    }

    bool errorBadRemap = false;

    for (uint32_t i = 0; i < pPacket->descriptorWriteCount && !errorBadRemap; i++) {
        VkDescriptorSet dstSet = m_objMapper.remap_descriptorsets(pPacket->pDescriptorWrites[i].dstSet);
        if (dstSet == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped write VkDescriptorSet.");
            errorBadRemap = true;
            break;
        }

        pRemappedWrites[i] = pPacket->pDescriptorWrites[i];
        pRemappedWrites[i].dstSet = dstSet;
        pRemappedWrites[i].pBufferInfo = nullptr;
        pRemappedWrites[i].pImageInfo = nullptr;
        pRemappedWrites[i].pTexelBufferView = nullptr;

        switch (pPacket->pDescriptorWrites[i].descriptorType) {
            case VK_DESCRIPTOR_TYPE_SAMPLER:
                pRemappedWrites[i].pImageInfo =
                    VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                memcpy((void *)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo,
                       pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
                for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                    if (pPacket->pDescriptorWrites[i].pImageInfo[j].sampler != VK_NULL_HANDLE) {
                        const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].sampler =
                            m_objMapper.remap_samplers(pPacket->pDescriptorWrites[i].pImageInfo[j].sampler);
                        if (pRemappedWrites[i].pImageInfo[j].sampler == VK_NULL_HANDLE) {
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
                pRemappedWrites[i].pImageInfo =
                    VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                memcpy((void *)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo,
                       pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
                for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                    if (pPacket->pDescriptorWrites[i].pImageInfo[j].imageView != VK_NULL_HANDLE) {
                        const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].imageView =
                            m_objMapper.remap_imageviews(pPacket->pDescriptorWrites[i].pImageInfo[j].imageView);
                        if (pRemappedWrites[i].pImageInfo[j].imageView == VK_NULL_HANDLE) {
                            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkImageView.");
                            errorBadRemap = true;
                            break;
                        }
                    }
                }
                break;
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                pRemappedWrites[i].pImageInfo =
                    VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                memcpy((void *)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo,
                       pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
                for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                    if (pPacket->pDescriptorWrites[i].pImageInfo[j].sampler != VK_NULL_HANDLE) {
                        const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].sampler =
                            m_objMapper.remap_samplers(pPacket->pDescriptorWrites[i].pImageInfo[j].sampler);
                        if (pRemappedWrites[i].pImageInfo[j].sampler == VK_NULL_HANDLE) {
                            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkSampler.");
                            errorBadRemap = true;
                            break;
                        }
                    }
                    if (pPacket->pDescriptorWrites[i].pImageInfo[j].imageView != VK_NULL_HANDLE) {
                        const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].imageView =
                            m_objMapper.remap_imageviews(pPacket->pDescriptorWrites[i].pImageInfo[j].imageView);
                        if (pRemappedWrites[i].pImageInfo[j].imageView == VK_NULL_HANDLE) {
                            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped VkImageView.");
                            errorBadRemap = true;
                            break;
                        }
                    }
                }
                break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                pRemappedWrites[i].pTexelBufferView =
                    VKTRACE_NEW_ARRAY(VkBufferView, pPacket->pDescriptorWrites[i].descriptorCount);
                memcpy((void *)pRemappedWrites[i].pTexelBufferView, pPacket->pDescriptorWrites[i].pTexelBufferView,
                       pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkBufferView));
                for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                    if (pPacket->pDescriptorWrites[i].pTexelBufferView[j] != VK_NULL_HANDLE) {
                        const_cast<VkBufferView *>(pRemappedWrites[i].pTexelBufferView)[j] =
                            m_objMapper.remap_bufferviews(pPacket->pDescriptorWrites[i].pTexelBufferView[j]);
                        if (pRemappedWrites[i].pTexelBufferView[j] == VK_NULL_HANDLE) {
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
                pRemappedWrites[i].pBufferInfo =
                    VKTRACE_NEW_ARRAY(VkDescriptorBufferInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                memcpy((void *)pRemappedWrites[i].pBufferInfo, pPacket->pDescriptorWrites[i].pBufferInfo,
                       pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorBufferInfo));
                for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                    if (pPacket->pDescriptorWrites[i].pBufferInfo[j].buffer != VK_NULL_HANDLE) {
                        const_cast<VkDescriptorBufferInfo *>(pRemappedWrites[i].pBufferInfo)[j].buffer =
                            m_objMapper.remap_buffers(pPacket->pDescriptorWrites[i].pBufferInfo[j].buffer);
                        if (pRemappedWrites[i].pBufferInfo[j].buffer == VK_NULL_HANDLE) {
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

    for (uint32_t i = 0; i < pPacket->descriptorCopyCount && !errorBadRemap; i++) {
        pRemappedCopies[i].dstSet = m_objMapper.remap_descriptorsets(pPacket->pDescriptorCopies[i].dstSet);
        if (pRemappedCopies[i].dstSet == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped destination VkDescriptorSet.");
            errorBadRemap = true;
            break;
        }

        pRemappedCopies[i].srcSet = m_objMapper.remap_descriptorsets(pPacket->pDescriptorCopies[i].srcSet);
        if (pRemappedCopies[i].srcSet == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkUpdateDescriptorSets() due to invalid remapped source VkDescriptorSet.");
            errorBadRemap = true;
            break;
        }
    }

    if (!errorBadRemap) {
        // If an error occurred, don't call the real function, but skip ahead so that memory is cleaned up!

        m_vkDeviceFuncs.UpdateDescriptorSets(remappedDevice, pPacket->descriptorWriteCount, pRemappedWrites,
                                             pPacket->descriptorCopyCount, pRemappedCopies);
    }

    for (uint32_t d = 0; d < pPacket->descriptorWriteCount; d++) {
        switch (pPacket->pDescriptorWrites[d].descriptorType) {
            case VK_DESCRIPTOR_TYPE_SAMPLER:
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                if (pRemappedWrites[d].pImageInfo != NULL) {
                    VKTRACE_DELETE((void *)pRemappedWrites[d].pImageInfo);
                    pRemappedWrites[d].pImageInfo = NULL;
                }
                break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                if (pRemappedWrites[d].pTexelBufferView != NULL) {
                    VKTRACE_DELETE((void *)pRemappedWrites[d].pTexelBufferView);
                    pRemappedWrites[d].pTexelBufferView = NULL;
                }
                break;
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
                if (pRemappedWrites[d].pBufferInfo != NULL) {
                    VKTRACE_DELETE((void *)pRemappedWrites[d].pBufferInfo);
                    pRemappedWrites[d].pBufferInfo = NULL;
                }
            default:
                break;
        }
    }
    VKTRACE_DELETE(pRemappedWrites);
    VKTRACE_DELETE(pRemappedCopies);
}

VkResult vkReplay::manually_replay_vkCreateDescriptorSetLayout(packet_vkCreateDescriptorSetLayout *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateDescriptorSetLayout() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorSetLayoutCreateInfo *pInfo = (VkDescriptorSetLayoutCreateInfo *)pPacket->pCreateInfo;
    if (pInfo != NULL) {
        if (pInfo->pBindings != NULL) {
            pInfo->pBindings = (VkDescriptorSetLayoutBinding *)vktrace_trace_packet_interpret_buffer_pointer(
                pPacket->header, (intptr_t)pInfo->pBindings);
            for (unsigned int i = 0; i < pInfo->bindingCount; i++) {
                VkDescriptorSetLayoutBinding *pBindings = (VkDescriptorSetLayoutBinding *)&pInfo->pBindings[i];
                if (pBindings->pImmutableSamplers != NULL &&
                    (pBindings->descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||
                     pBindings->descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)) {
                    pBindings->pImmutableSamplers = (const VkSampler *)vktrace_trace_packet_interpret_buffer_pointer(
                        pPacket->header, (intptr_t)pBindings->pImmutableSamplers);
                    for (unsigned int j = 0; j < pBindings->descriptorCount; j++) {
                        VkSampler *pSampler = (VkSampler *)&pBindings->pImmutableSamplers[j];
                        *pSampler = m_objMapper.remap_samplers(pBindings->pImmutableSamplers[j]);
                        if (*pSampler == VK_NULL_HANDLE) {
                            vktrace_LogError("Skipping vkCreateDescriptorSetLayout() due to invalid remapped VkSampler.");
                            return VK_ERROR_VALIDATION_FAILED_EXT;
                        }
                    }
                }
            }
        }
    }
    VkDescriptorSetLayout setLayout;
    replayResult = m_vkDeviceFuncs.CreateDescriptorSetLayout(remappedDevice, pPacket->pCreateInfo, NULL, &setLayout);
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_descriptorsetlayouts_map(*(pPacket->pSetLayout), setLayout);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkDestroyDescriptorSetLayout(packet_vkDestroyDescriptorSetLayout *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkDestroyDescriptorSetLayout() due to invalid remapped VkDevice.");
        return;
    }

    m_vkDeviceFuncs.DestroyDescriptorSetLayout(remappedDevice, pPacket->descriptorSetLayout, NULL);
    m_objMapper.rm_from_descriptorsetlayouts_map(pPacket->descriptorSetLayout);
}

VkResult vkReplay::manually_replay_vkAllocateDescriptorSets(packet_vkAllocateDescriptorSets *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkAllocateDescriptorSets() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorPool remappedPool = m_objMapper.remap_descriptorpools(pPacket->pAllocateInfo->descriptorPool);
    if (remappedPool == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkAllocateDescriptorSets() due to invalid remapped VkDescriptorPool.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorSetLayout *pRemappedSetLayouts =
        VKTRACE_NEW_ARRAY(VkDescriptorSetLayout, pPacket->pAllocateInfo->descriptorSetCount);

    VkDescriptorSetAllocateInfo allocateInfo;
    allocateInfo.pNext = NULL;
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = remappedPool;
    allocateInfo.descriptorSetCount = pPacket->pAllocateInfo->descriptorSetCount;
    allocateInfo.pSetLayouts = pRemappedSetLayouts;

    for (uint32_t i = 0; i < allocateInfo.descriptorSetCount; i++) {
        pRemappedSetLayouts[i] = m_objMapper.remap_descriptorsetlayouts(pPacket->pAllocateInfo->pSetLayouts[i]);
        if (pRemappedSetLayouts[i] == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkAllocateDescriptorSets() due to invalid remapped VkDescriptorSetLayout.");
            VKTRACE_DELETE(pRemappedSetLayouts);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }

    VkDescriptorSet *pDescriptorSets = NULL;
    replayResult = m_vkDeviceFuncs.AllocateDescriptorSets(remappedDevice, pPacket->pAllocateInfo, pDescriptorSets);
    if (replayResult == VK_SUCCESS) {
        for (uint32_t i = 0; i < pPacket->pAllocateInfo->descriptorSetCount; ++i) {
            m_objMapper.add_to_descriptorsets_map(pPacket->pDescriptorSets[i], pDescriptorSets[i]);
        }
    }

    VKTRACE_DELETE(pRemappedSetLayouts);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkFreeDescriptorSets(packet_vkFreeDescriptorSets *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkFreeDescriptorSets() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorPool remappedDescriptorPool;
    remappedDescriptorPool = m_objMapper.remap_descriptorpools(pPacket->descriptorPool);
    if (remappedDescriptorPool == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkFreeDescriptorSets() due to invalid remapped VkDescriptorPool.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDescriptorSet *localDSs = VKTRACE_NEW_ARRAY(VkDescriptorSet, pPacket->descriptorSetCount);
    uint32_t i;
    for (i = 0; i < pPacket->descriptorSetCount; ++i) {
        localDSs[i] = m_objMapper.remap_descriptorsets(pPacket->pDescriptorSets[i]);
        if (localDSs[i] == VK_NULL_HANDLE && pPacket->pDescriptorSets[i] != VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkFreeDescriptorSets() due to invalid remapped VkDescriptorSet.");
            VKTRACE_DELETE(localDSs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }

    replayResult =
        m_vkDeviceFuncs.FreeDescriptorSets(remappedDevice, remappedDescriptorPool, pPacket->descriptorSetCount, localDSs);
    if (replayResult == VK_SUCCESS) {
        for (i = 0; i < pPacket->descriptorSetCount; ++i) {
            m_objMapper.rm_from_descriptorsets_map(pPacket->pDescriptorSets[i]);
        }
    }
    VKTRACE_DELETE(localDSs);
    return replayResult;
}

void vkReplay::manually_replay_vkCmdBindDescriptorSets(packet_vkCmdBindDescriptorSets *pPacket) {
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdBindDescriptorSets() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkPipelineLayout remappedLayout = m_objMapper.remap_pipelinelayouts(pPacket->layout);
    if (remappedLayout == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdBindDescriptorSets() due to invalid remapped VkPipelineLayout.");
        return;
    }

    VkDescriptorSet *pRemappedSets = (VkDescriptorSet *)vktrace_malloc(sizeof(VkDescriptorSet) * pPacket->descriptorSetCount);
    if (pRemappedSets == NULL) {
        vktrace_LogError("Replay of CmdBindDescriptorSets out of memory.");
        return;
    }

    for (uint32_t idx = 0; idx < pPacket->descriptorSetCount && pPacket->pDescriptorSets != NULL; idx++) {
        pRemappedSets[idx] = m_objMapper.remap_descriptorsets(pPacket->pDescriptorSets[idx]);
        if (pRemappedSets[idx] == VK_NULL_HANDLE && pPacket->pDescriptorSets[idx] != VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCmdBindDescriptorSets() due to invalid remapped VkDescriptorSet.");
            vktrace_free(pRemappedSets);
            return;
        }
    }

    m_vkDeviceFuncs.CmdBindDescriptorSets(remappedCommandBuffer, pPacket->pipelineBindPoint, remappedLayout, pPacket->firstSet,
                                          pPacket->descriptorSetCount, pRemappedSets, pPacket->dynamicOffsetCount,
                                          pPacket->pDynamicOffsets);
    vktrace_free(pRemappedSets);
    return;
}

void vkReplay::manually_replay_vkCmdBindVertexBuffers(packet_vkCmdBindVertexBuffers *pPacket) {
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdBindVertexBuffers() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkBuffer *pSaveBuff = VKTRACE_NEW_ARRAY(VkBuffer, pPacket->bindingCount);
    if (pSaveBuff == NULL && pPacket->bindingCount > 0) {
        vktrace_LogError("Replay of CmdBindVertexBuffers out of memory.");
        return;
    }
    uint32_t i = 0;
    if (pPacket->pBuffers != NULL) {
        for (i = 0; i < pPacket->bindingCount; i++) {
            VkBuffer *pBuff = (VkBuffer *)&(pPacket->pBuffers[i]);
            pSaveBuff[i] = pPacket->pBuffers[i];
            *pBuff = m_objMapper.remap_buffers(pPacket->pBuffers[i]);
            if (*pBuff == VK_NULL_HANDLE && pPacket->pBuffers[i] != VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkCmdBindVertexBuffers() due to invalid remapped VkBuffer.");
                VKTRACE_DELETE(pSaveBuff);
                return;
            }
        }
    }
    m_vkDeviceFuncs.CmdBindVertexBuffers(remappedCommandBuffer, pPacket->firstBinding, pPacket->bindingCount, pPacket->pBuffers,
                                         pPacket->pOffsets);
    for (uint32_t k = 0; k < i; k++) {
        VkBuffer *pBuff = (VkBuffer *)&(pPacket->pBuffers[k]);
        *pBuff = pSaveBuff[k];
    }
    VKTRACE_DELETE(pSaveBuff);
    return;
}

VkResult vkReplay::manually_replay_vkGetPipelineCacheData(packet_vkGetPipelineCacheData *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    size_t dataSize;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPipelineCacheData() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkPipelineCache remappedpipelineCache = m_objMapper.remap_pipelinecaches(pPacket->pipelineCache);
    if (pPacket->pipelineCache != VK_NULL_HANDLE && remappedpipelineCache == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPipelineCacheData() due to invalid remapped VkPipelineCache.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // Since the returned data size may not be equal to size of the buffer in the trace packet allocate a local buffer as needed
    replayResult = m_vkDeviceFuncs.GetPipelineCacheData(remappeddevice, remappedpipelineCache, &dataSize, NULL);
    if (replayResult != VK_SUCCESS) return replayResult;
    if (pPacket->pData) {
        uint8_t *pData = VKTRACE_NEW_ARRAY(uint8_t, dataSize);
        replayResult = m_vkDeviceFuncs.GetPipelineCacheData(remappeddevice, remappedpipelineCache, &dataSize, pData);
        VKTRACE_DELETE(pData);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateComputePipelines(packet_vkCreateComputePipelines *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    uint32_t i;

    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkPipelineCache pipelineCache;
    pipelineCache = m_objMapper.remap_pipelinecaches(pPacket->pipelineCache);

    VkComputePipelineCreateInfo *pLocalCIs = VKTRACE_NEW_ARRAY(VkComputePipelineCreateInfo, pPacket->createInfoCount);
    memcpy((void *)pLocalCIs, (void *)(pPacket->pCreateInfos), sizeof(VkComputePipelineCreateInfo) * pPacket->createInfoCount);

    // Fix up stage sub-elements
    for (i = 0; i < pPacket->createInfoCount; i++) {
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)&pLocalCIs[i]);

        pLocalCIs[i].stage.module = m_objMapper.remap_shadermodules(pLocalCIs[i].stage.module);

        if (pLocalCIs[i].stage.pName)
            pLocalCIs[i].stage.pName =
                (const char *)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pLocalCIs[i].stage.pName));

        if (pLocalCIs[i].stage.pSpecializationInfo) {
            VkSpecializationInfo *si = VKTRACE_NEW(VkSpecializationInfo);
            pLocalCIs[i].stage.pSpecializationInfo = (const VkSpecializationInfo *)(vktrace_trace_packet_interpret_buffer_pointer(
                pPacket->header, (intptr_t)pLocalCIs[i].stage.pSpecializationInfo));
            memcpy((void *)si, (void *)(pLocalCIs[i].stage.pSpecializationInfo), sizeof(VkSpecializationInfo));

            if (si->mapEntryCount > 0 && si->pMapEntries)
                si->pMapEntries = (const VkSpecializationMapEntry *)(vktrace_trace_packet_interpret_buffer_pointer(
                    pPacket->header, (intptr_t)pLocalCIs[i].stage.pSpecializationInfo->pMapEntries));
            if (si->dataSize > 0 && si->pData)
                si->pData = (const void *)(vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)si->pData));
            pLocalCIs[i].stage.pSpecializationInfo = si;
        }

        pLocalCIs[i].layout = m_objMapper.remap_pipelinelayouts(pLocalCIs[i].layout);
        pLocalCIs[i].basePipelineHandle = m_objMapper.remap_pipelines(pLocalCIs[i].basePipelineHandle);
    }

    VkPipeline *local_pPipelines = VKTRACE_NEW_ARRAY(VkPipeline, pPacket->createInfoCount);

    replayResult = m_vkDeviceFuncs.CreateComputePipelines(remappeddevice, pipelineCache, pPacket->createInfoCount, pLocalCIs, NULL,
                                                          local_pPipelines);

    if (replayResult == VK_SUCCESS) {
        for (i = 0; i < pPacket->createInfoCount; i++) {
            m_objMapper.add_to_pipelines_map(pPacket->pPipelines[i], local_pPipelines[i]);
        }
    }

    for (i = 0; i < pPacket->createInfoCount; i++)
        if (pLocalCIs[i].stage.pSpecializationInfo) VKTRACE_DELETE((void *)pLocalCIs[i].stage.pSpecializationInfo);
    VKTRACE_DELETE(pLocalCIs);
    VKTRACE_DELETE(local_pPipelines);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateGraphicsPipelines(packet_vkCreateGraphicsPipelines *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // remap shaders from each stage
    VkPipelineShaderStageCreateInfo **ppRemappedStages =
        VKTRACE_NEW_ARRAY(VkPipelineShaderStageCreateInfo *, pPacket->createInfoCount);
    memset(ppRemappedStages, 0, sizeof(VkPipelineShaderStageCreateInfo *) * pPacket->createInfoCount);
    VkPipelineShaderStageCreateInfo *pRemappedStages;
    VkGraphicsPipelineCreateInfo *pLocalCIs = VKTRACE_NEW_ARRAY(VkGraphicsPipelineCreateInfo, pPacket->createInfoCount);
    uint32_t i, j, k;
    for (i = 0; i < pPacket->createInfoCount; i++) {
        pRemappedStages = VKTRACE_NEW_ARRAY(VkPipelineShaderStageCreateInfo, pPacket->pCreateInfos[i].stageCount);
        ppRemappedStages[i] = pRemappedStages;
        memcpy(pRemappedStages, pPacket->pCreateInfos[i].pStages,
               sizeof(VkPipelineShaderStageCreateInfo) * pPacket->pCreateInfos[i].stageCount);

        memcpy((void *)&(pLocalCIs[i]), (void *)&(pPacket->pCreateInfos[i]), sizeof(VkGraphicsPipelineCreateInfo));
        for (j = 0; j < pPacket->pCreateInfos[i].stageCount; j++) {
            pRemappedStages[j].module = m_objMapper.remap_shadermodules(pRemappedStages[j].module);
            if (pRemappedStages[j].module == VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkShaderModule.");
                for (k = 0; k < pPacket->createInfoCount; k++) {
                    VKTRACE_DELETE(ppRemappedStages[k]);
                }
                VKTRACE_DELETE(ppRemappedStages);
                VKTRACE_DELETE(pLocalCIs);
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }

        vktrace_interpret_pnext_pointers(pPacket->header, (void *)&pLocalCIs[i]);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pStages);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pVertexInputState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pInputAssemblyState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pTessellationState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pViewportState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pRasterizationState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pMultisampleState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pDepthStencilState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pColorBlendState);
        vktrace_interpret_pnext_pointers(pPacket->header, (void *)pLocalCIs[i].pDynamicState);

        VkPipelineShaderStageCreateInfo **ppSSCI = (VkPipelineShaderStageCreateInfo **)&(pLocalCIs[i].pStages);
        *ppSSCI = pRemappedStages;

        pLocalCIs[i].layout = m_objMapper.remap_pipelinelayouts(pPacket->pCreateInfos[i].layout);
        if (pLocalCIs[i].layout == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkPipelineLayout.");
            for (k = 0; k < pPacket->createInfoCount; k++) {
                VKTRACE_DELETE(ppRemappedStages[k]);
            }
            VKTRACE_DELETE(ppRemappedStages);
            VKTRACE_DELETE(pLocalCIs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        pLocalCIs[i].renderPass = m_objMapper.remap_renderpasss(pPacket->pCreateInfos[i].renderPass);
        if (pLocalCIs[i].renderPass == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkRenderPass.");
            for (k = 0; k < pPacket->createInfoCount; k++) {
                VKTRACE_DELETE(ppRemappedStages[k]);
            }
            VKTRACE_DELETE(ppRemappedStages);
            VKTRACE_DELETE(pLocalCIs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        pLocalCIs[i].basePipelineHandle = m_objMapper.remap_pipelines(pPacket->pCreateInfos[i].basePipelineHandle);
        if (pLocalCIs[i].basePipelineHandle == VK_NULL_HANDLE && pPacket->pCreateInfos[i].basePipelineHandle != VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkPipeline.");
            for (k = 0; k < pPacket->createInfoCount; k++) {
                VKTRACE_DELETE(ppRemappedStages[k]);
            }
            VKTRACE_DELETE(ppRemappedStages);
            VKTRACE_DELETE(pLocalCIs);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        ((VkPipelineViewportStateCreateInfo *)pLocalCIs[i].pViewportState)->pViewports =
            (VkViewport *)vktrace_trace_packet_interpret_buffer_pointer(
                pPacket->header, (intptr_t)pPacket->pCreateInfos[i].pViewportState->pViewports);
        ((VkPipelineViewportStateCreateInfo *)pLocalCIs[i].pViewportState)->pScissors =
            (VkRect2D *)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header,
                                                                      (intptr_t)pPacket->pCreateInfos[i].pViewportState->pScissors);

        ((VkPipelineMultisampleStateCreateInfo *)pLocalCIs[i].pMultisampleState)->pSampleMask =
            (VkSampleMask *)vktrace_trace_packet_interpret_buffer_pointer(
                pPacket->header, (intptr_t)pPacket->pCreateInfos[i].pMultisampleState->pSampleMask);
    }

    VkPipelineCache remappedPipelineCache;
    remappedPipelineCache = m_objMapper.remap_pipelinecaches(pPacket->pipelineCache);
    if (remappedPipelineCache == VK_NULL_HANDLE && pPacket->pipelineCache != VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateGraphicsPipelines() due to invalid remapped VkPipelineCache.");
        for (k = 0; k < pPacket->createInfoCount; k++) {
            VKTRACE_DELETE(ppRemappedStages[k]);
        }
        VKTRACE_DELETE(ppRemappedStages);
        VKTRACE_DELETE(pLocalCIs);
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    uint32_t createInfoCount = pPacket->createInfoCount;
    VkPipeline *local_pPipelines = VKTRACE_NEW_ARRAY(VkPipeline, pPacket->createInfoCount);

    replayResult = m_vkDeviceFuncs.CreateGraphicsPipelines(remappedDevice, remappedPipelineCache, createInfoCount, pLocalCIs, NULL,
                                                           local_pPipelines);

    if (replayResult == VK_SUCCESS) {
        for (i = 0; i < pPacket->createInfoCount; i++) {
            m_objMapper.add_to_pipelines_map(pPacket->pPipelines[i], local_pPipelines[i]);
        }
    }

    for (k = 0; k < pPacket->createInfoCount; k++) {
        VKTRACE_DELETE(ppRemappedStages[k]);
    }
    VKTRACE_DELETE(ppRemappedStages);
    VKTRACE_DELETE(pLocalCIs);
    VKTRACE_DELETE(local_pPipelines);

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreatePipelineLayout(packet_vkCreatePipelineLayout *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) return VK_ERROR_VALIDATION_FAILED_EXT;

    // array to store the original trace-time layouts, so that we can remap them inside the packet and then
    // restore them after replaying the API call.
    VkDescriptorSetLayout *pSaveLayouts = NULL;
    if (pPacket->pCreateInfo->setLayoutCount > 0) {
        pSaveLayouts =
            (VkDescriptorSetLayout *)vktrace_malloc(sizeof(VkDescriptorSetLayout) * pPacket->pCreateInfo->setLayoutCount);
        if (!pSaveLayouts) {
            vktrace_LogError("Replay of CreatePipelineLayout out of memory.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }
    uint32_t i = 0;
    for (i = 0; (i < pPacket->pCreateInfo->setLayoutCount) && (pPacket->pCreateInfo->pSetLayouts != NULL); i++) {
        VkDescriptorSetLayout *pSL = (VkDescriptorSetLayout *)&(pPacket->pCreateInfo->pSetLayouts[i]);
        pSaveLayouts[i] = pPacket->pCreateInfo->pSetLayouts[i];
        *pSL = m_objMapper.remap_descriptorsetlayouts(pPacket->pCreateInfo->pSetLayouts[i]);
    }
    VkPipelineLayout localPipelineLayout;
    replayResult = m_vkDeviceFuncs.CreatePipelineLayout(remappedDevice, pPacket->pCreateInfo, NULL, &localPipelineLayout);
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_pipelinelayouts_map(*(pPacket->pPipelineLayout), localPipelineLayout);
    }
    // restore packet to contain the original Set Layouts before being remapped.
    for (uint32_t k = 0; k < i; k++) {
        VkDescriptorSetLayout *pSL = (VkDescriptorSetLayout *)&(pPacket->pCreateInfo->pSetLayouts[k]);
        *pSL = pSaveLayouts[k];
    }
    if (pSaveLayouts != NULL) {
        vktrace_free(pSaveLayouts);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkCmdWaitEvents(packet_vkCmdWaitEvents *pPacket) {
    VkDevice traceDevice;
    VkDevice replayDevice;
    uint32_t srcReplayIdx, dstReplayIdx;
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkEvent *saveEvent = VKTRACE_NEW_ARRAY(VkEvent, pPacket->eventCount);
    uint32_t idx = 0;
    uint32_t numRemapBuf = 0;
    uint32_t numRemapImg = 0;
    for (idx = 0; idx < pPacket->eventCount; idx++) {
        VkEvent *pEvent = (VkEvent *)&(pPacket->pEvents[idx]);
        saveEvent[idx] = pPacket->pEvents[idx];
        *pEvent = m_objMapper.remap_events(pPacket->pEvents[idx]);
        if (*pEvent == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkEvent.");
            VKTRACE_DELETE(saveEvent);
            return;
        }
    }

    VkBuffer *saveBuf = VKTRACE_NEW_ARRAY(VkBuffer, pPacket->bufferMemoryBarrierCount);
    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++) {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *)&(pPacket->pBufferMemoryBarriers[idx]);
        saveBuf[numRemapBuf++] = pNextBuf->buffer;
        traceDevice = traceBufferToDevice[pNextBuf->buffer];
        pNextBuf->buffer = m_objMapper.remap_buffers(pNextBuf->buffer);
        if (pNextBuf->buffer == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkBuffer.");
            VKTRACE_DELETE(saveEvent);
            VKTRACE_DELETE(saveBuf);
            return;
        }
        replayDevice = replayBufferToDevice[pNextBuf->buffer];
        if (getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex, &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pBufferMemoryBarriers[idx].dstQueueFamilyIndex, &dstReplayIdx)) {
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkCmdWaitEvents failed, bad srcQueueFamilyIndex");
            VKTRACE_DELETE(saveEvent);
            VKTRACE_DELETE(saveBuf);
            return;
        }
    }
    VkImage *saveImg = VKTRACE_NEW_ARRAY(VkImage, pPacket->imageMemoryBarrierCount);
    for (idx = 0; idx < pPacket->imageMemoryBarrierCount; idx++) {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *)&(pPacket->pImageMemoryBarriers[idx]);
        saveImg[numRemapImg++] = pNextImg->image;
        traceDevice = traceImageToDevice[pNextImg->image];
        pNextImg->image = m_objMapper.remap_images(pNextImg->image);
        if (pNextImg->image == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCmdWaitEvents() due to invalid remapped VkImage.");
            VKTRACE_DELETE(saveEvent);
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
        replayDevice = replayImageToDevice[pNextImg->image];
        if (getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex, &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pImageMemoryBarriers[idx].dstQueueFamilyIndex, &dstReplayIdx)) {
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkCmdWaitEvents failed, bad srcQueueFamilyIndex");
            VKTRACE_DELETE(saveEvent);
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
    }
    m_vkDeviceFuncs.CmdWaitEvents(remappedCommandBuffer, pPacket->eventCount, pPacket->pEvents, pPacket->srcStageMask,
                                  pPacket->dstStageMask, pPacket->memoryBarrierCount, pPacket->pMemoryBarriers,
                                  pPacket->bufferMemoryBarrierCount, pPacket->pBufferMemoryBarriers,
                                  pPacket->imageMemoryBarrierCount, pPacket->pImageMemoryBarriers);

    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++) {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *)&(pPacket->pBufferMemoryBarriers[idx]);
        pNextBuf->buffer = saveBuf[idx];
    }
    for (idx = 0; idx < pPacket->memoryBarrierCount; idx++) {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *)&(pPacket->pImageMemoryBarriers[idx]);
        pNextImg->image = saveImg[idx];
    }
    for (idx = 0; idx < pPacket->eventCount; idx++) {
        VkEvent *pEvent = (VkEvent *)&(pPacket->pEvents[idx]);
        *pEvent = saveEvent[idx];
    }
    VKTRACE_DELETE(saveEvent);
    VKTRACE_DELETE(saveBuf);
    VKTRACE_DELETE(saveImg);
    return;
}

void vkReplay::manually_replay_vkCmdPipelineBarrier(packet_vkCmdPipelineBarrier *pPacket) {
    VkDevice traceDevice;
    VkDevice replayDevice;
    uint32_t srcReplayIdx, dstReplayIdx;
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdPipelineBarrier() due to invalid remapped VkCommandBuffer.");
        return;
    }

    uint32_t idx = 0;
    uint32_t numRemapBuf = 0;
    uint32_t numRemapImg = 0;
    VkBuffer *saveBuf = VKTRACE_NEW_ARRAY(VkBuffer, pPacket->bufferMemoryBarrierCount);
    VkImage *saveImg = VKTRACE_NEW_ARRAY(VkImage, pPacket->imageMemoryBarrierCount);
    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++) {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *)&(pPacket->pBufferMemoryBarriers[idx]);
        saveBuf[numRemapBuf++] = pNextBuf->buffer;
        traceDevice = traceBufferToDevice[pNextBuf->buffer];
        pNextBuf->buffer = m_objMapper.remap_buffers(pNextBuf->buffer);
        if (pNextBuf->buffer == VK_NULL_HANDLE && saveBuf[numRemapBuf - 1] != VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCmdPipelineBarrier() due to invalid remapped VkBuffer.");
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
        replayDevice = replayBufferToDevice[pNextBuf->buffer];
        if (getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex, &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pBufferMemoryBarriers[idx].dstQueueFamilyIndex, &dstReplayIdx)) {
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pBufferMemoryBarriers[idx].dstQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkCmdPipelineBarrier failed, bad srcQueueFamilyIndex");
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
    }
    for (idx = 0; idx < pPacket->imageMemoryBarrierCount; idx++) {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *)&(pPacket->pImageMemoryBarriers[idx]);
        saveImg[numRemapImg++] = pNextImg->image;
        traceDevice = traceImageToDevice[pNextImg->image];
        if (traceDevice == NULL) vktrace_LogError("DEBUG: traceDevice is NULL");
        pNextImg->image = m_objMapper.remap_images(pNextImg->image);
        if (pNextImg->image == VK_NULL_HANDLE && saveImg[numRemapImg - 1] != VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkCmdPipelineBarrier() due to invalid remapped VkImage.");
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
        replayDevice = replayImageToDevice[pNextImg->image];
        if (getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex, &srcReplayIdx) &&
            getQueueFamilyIdx(traceDevice, replayDevice, pPacket->pImageMemoryBarriers[idx].dstQueueFamilyIndex, &dstReplayIdx)) {
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].srcQueueFamilyIndex) = srcReplayIdx;
            *((uint32_t *)&pPacket->pImageMemoryBarriers[idx].dstQueueFamilyIndex) = dstReplayIdx;
        } else {
            vktrace_LogError("vkPipelineBarrier failed, bad srcQueueFamilyIndex");
            VKTRACE_DELETE(saveBuf);
            VKTRACE_DELETE(saveImg);
            return;
        }
    }
    m_vkDeviceFuncs.CmdPipelineBarrier(remappedCommandBuffer, pPacket->srcStageMask, pPacket->dstStageMask,
                                       pPacket->dependencyFlags, pPacket->memoryBarrierCount, pPacket->pMemoryBarriers,
                                       pPacket->bufferMemoryBarrierCount, pPacket->pBufferMemoryBarriers,
                                       pPacket->imageMemoryBarrierCount, pPacket->pImageMemoryBarriers);

    for (idx = 0; idx < pPacket->bufferMemoryBarrierCount; idx++) {
        VkBufferMemoryBarrier *pNextBuf = (VkBufferMemoryBarrier *)&(pPacket->pBufferMemoryBarriers[idx]);
        pNextBuf->buffer = saveBuf[idx];
    }
    for (idx = 0; idx < pPacket->imageMemoryBarrierCount; idx++) {
        VkImageMemoryBarrier *pNextImg = (VkImageMemoryBarrier *)&(pPacket->pImageMemoryBarriers[idx]);
        pNextImg->image = saveImg[idx];
    }
    VKTRACE_DELETE(saveBuf);
    VKTRACE_DELETE(saveImg);
    return;
}

VkResult vkReplay::manually_replay_vkCreateFramebuffer(packet_vkCreateFramebuffer *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateFramebuffer() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFramebufferCreateInfo *pInfo = (VkFramebufferCreateInfo *)pPacket->pCreateInfo;
    VkImageView *pAttachments = NULL, *pSavedAttachments = (VkImageView *)pInfo->pAttachments;
    bool allocatedAttachments = false;
    if (pSavedAttachments != NULL) {
        allocatedAttachments = true;
        pAttachments = VKTRACE_NEW_ARRAY(VkImageView, pInfo->attachmentCount);
        memcpy(pAttachments, pSavedAttachments, sizeof(VkImageView) * pInfo->attachmentCount);
        for (uint32_t i = 0; i < pInfo->attachmentCount; i++) {
            pAttachments[i] = m_objMapper.remap_imageviews(pInfo->pAttachments[i]);
            if (pAttachments[i] == VK_NULL_HANDLE && pInfo->pAttachments[i] != VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkCreateFramebuffer() due to invalid remapped VkImageView.");
                VKTRACE_DELETE(pAttachments);
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
        pInfo->pAttachments = pAttachments;
    }
    VkRenderPass savedRP = pPacket->pCreateInfo->renderPass;
    pInfo->renderPass = m_objMapper.remap_renderpasss(pPacket->pCreateInfo->renderPass);
    if (pInfo->renderPass == VK_NULL_HANDLE && pPacket->pCreateInfo->renderPass != VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateFramebuffer() due to invalid remapped VkRenderPass.");
        if (allocatedAttachments) {
            VKTRACE_DELETE(pAttachments);
        }
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFramebuffer local_framebuffer;
    replayResult = m_vkDeviceFuncs.CreateFramebuffer(remappedDevice, pPacket->pCreateInfo, NULL, &local_framebuffer);
    pInfo->pAttachments = pSavedAttachments;
    pInfo->renderPass = savedRP;
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_framebuffers_map(*(pPacket->pFramebuffer), local_framebuffer);
    }
    if (allocatedAttachments) {
        VKTRACE_DELETE((void *)pAttachments);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateRenderPass(packet_vkCreateRenderPass *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateRenderPass() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkRenderPass local_renderpass;
    replayResult = m_vkDeviceFuncs.CreateRenderPass(remappedDevice, pPacket->pCreateInfo, NULL, &local_renderpass);
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_renderpasss_map(*(pPacket->pRenderPass), local_renderpass);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkCmdBeginRenderPass(packet_vkCmdBeginRenderPass *pPacket) {
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);

    if (remappedCommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdBeginRenderPass() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkRenderPassBeginInfo local_renderPassBeginInfo;
    memcpy((void *)&local_renderPassBeginInfo, (void *)pPacket->pRenderPassBegin, sizeof(VkRenderPassBeginInfo));
    local_renderPassBeginInfo.pClearValues = (const VkClearValue *)pPacket->pRenderPassBegin->pClearValues;
    local_renderPassBeginInfo.framebuffer = m_objMapper.remap_framebuffers(pPacket->pRenderPassBegin->framebuffer);
    if (local_renderPassBeginInfo.framebuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdBeginRenderPass() due to invalid remapped VkFramebuffer.");
        return;
    }
    local_renderPassBeginInfo.renderPass = m_objMapper.remap_renderpasss(pPacket->pRenderPassBegin->renderPass);
    if (local_renderPassBeginInfo.renderPass == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCmdBeginRenderPass() due to invalid remapped VkRenderPass.");
        return;
    }
    m_vkDeviceFuncs.CmdBeginRenderPass(remappedCommandBuffer, &local_renderPassBeginInfo, pPacket->contents);
    return;
}

VkResult vkReplay::manually_replay_vkBeginCommandBuffer(packet_vkBeginCommandBuffer *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkCommandBuffer remappedCommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (remappedCommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkBeginCommandBuffer() due to invalid remapped VkCommandBuffer.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkCommandBufferBeginInfo *pInfo = (VkCommandBufferBeginInfo *)pPacket->pBeginInfo;
    VkCommandBufferInheritanceInfo *pHinfo = (VkCommandBufferInheritanceInfo *)((pInfo) ? pInfo->pInheritanceInfo : NULL);
    // Save the original RP & FB, then overwrite packet with remapped values
    VkRenderPass savedRP = VK_NULL_HANDLE, *pRP;
    VkFramebuffer savedFB = VK_NULL_HANDLE, *pFB;
    if (pInfo != NULL && pHinfo != NULL) {
        savedRP = pHinfo->renderPass;
        savedFB = pHinfo->framebuffer;
        pRP = &(pHinfo->renderPass);
        pFB = &(pHinfo->framebuffer);
        *pRP = m_objMapper.remap_renderpasss(savedRP);
        *pFB = m_objMapper.remap_framebuffers(savedFB);
    }
    replayResult = m_vkDeviceFuncs.BeginCommandBuffer(remappedCommandBuffer, pPacket->pBeginInfo);
    if (pInfo != NULL && pHinfo != NULL) {
        pHinfo->renderPass = savedRP;
        pHinfo->framebuffer = savedFB;
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkWaitForFences(packet_vkWaitForFences *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    uint32_t i;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkWaitForFences() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkFence *pFence = VKTRACE_NEW_ARRAY(VkFence, pPacket->fenceCount);
    for (i = 0; i < pPacket->fenceCount; i++) {
        (*(pFence + i)) = m_objMapper.remap_fences((*(pPacket->pFences + i)));
        if (*(pFence + i) == VK_NULL_HANDLE) {
            vktrace_LogError("Skipping vkWaitForFences() due to invalid remapped VkFence.");
            VKTRACE_DELETE(pFence);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
    }
    if (pPacket->result == VK_SUCCESS) {
        replayResult = m_vkDeviceFuncs.WaitForFences(remappedDevice, pPacket->fenceCount, pFence, pPacket->waitAll,
                                                     UINT64_MAX);  // mean as long as possible
    } else {
        if (pPacket->result == VK_TIMEOUT) {
            replayResult = m_vkDeviceFuncs.WaitForFences(remappedDevice, pPacket->fenceCount, pFence, pPacket->waitAll, 0);
        } else {
            replayResult =
                m_vkDeviceFuncs.WaitForFences(remappedDevice, pPacket->fenceCount, pFence, pPacket->waitAll, pPacket->timeout);
        }
    }
    VKTRACE_DELETE(pFence);
    return replayResult;
}

bool vkReplay::getMemoryTypeIdx(VkDevice traceDevice, VkDevice replayDevice, uint32_t traceIdx,
                                VkMemoryRequirements *memRequirements, uint32_t *pReplayIdx) {
    VkPhysicalDevice tracePhysicalDevice;
    VkPhysicalDevice replayPhysicalDevice;
    uint32_t i;

    if (tracePhysicalDevices.find(traceDevice) == tracePhysicalDevices.end() ||
        replayPhysicalDevices.find(replayDevice) == replayPhysicalDevices.end()) {
        goto fail;
    }

    tracePhysicalDevice = tracePhysicalDevices[traceDevice];
    replayPhysicalDevice = replayPhysicalDevices[replayDevice];

    if (min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount,
            replayMemoryProperties[replayPhysicalDevice].memoryTypeCount) == 0) {
        goto fail;
    }

    // Search for an exact match from set of bits in memoryRequirements->memoryTypeBits
    for (i = 0; i < min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount,
                        replayMemoryProperties[replayPhysicalDevice].memoryTypeCount);
         i++) {
        if (((1 << i) & memRequirements->memoryTypeBits) &&
            traceMemoryProperties[tracePhysicalDevice].memoryTypes[traceIdx].propertyFlags ==
                replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags) {
            *pReplayIdx = i;
            return true;
        }
    }

    // Didn't find an exact match, search for a superset
    // from set of bits in memoryRequirements->memoryTypeBits
    for (i = 0; i < min(traceMemoryProperties[tracePhysicalDevice].memoryTypeCount,
                        replayMemoryProperties[replayPhysicalDevice].memoryTypeCount);
         i++) {
        if (((1 << i) & memRequirements->memoryTypeBits) &&
            traceMemoryProperties[tracePhysicalDevice].memoryTypes[traceIdx].propertyFlags ==
                (traceMemoryProperties[tracePhysicalDevice].memoryTypes[traceIdx].propertyFlags &
                 replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags)) {
            *pReplayIdx = i;
            return true;
        }
    }

    // Didn't find a superset, search for mem type with both HOST_VISIBLE and HOST_COHERENT set
    // from set of bits in memoryRequirements->memoryTypeBits
    for (i = 0; i < replayMemoryProperties[replayPhysicalDevice].memoryTypeCount; i++) {
        if (((1 << i) & memRequirements->memoryTypeBits) &&
            (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) &
                replayMemoryProperties[replayPhysicalDevice].memoryTypes[i].propertyFlags) {
            *pReplayIdx = i;
            return true;
        }
    }


fail:
    // Didn't find a match
    vktrace_LogError(
        "Cannot determine memory type during vkAllocateMemory - vkGetPhysicalDeviceMemoryProperties should be called before "
        "vkAllocateMemory.");
    return false;
}

#define FSEEK(_stream, _offset, _whence)                                                               \
    assert(_whence == SEEK_SET);                                                                       \
    if (!vktrace_FileLike_SetCurrentPosition(_stream, _offset)) {                                      \
        vktrace_LogError("fseek during vkAllocateMemory() failed, can't determine memory type index"); \
        goto out;                                                                                      \
    }

#define FREAD(_ptr, _size, _nmemb, _stream)                                                            \
    assert(_nmemb == 1);                                                                               \
    if (!vktrace_FileLike_ReadRaw(_stream, _ptr, _size)) {                                             \
        vktrace_LogError("fread during vkAllocateMemory() failed, can't determine memory type index"); \
        goto out;                                                                                      \
    }

bool vkReplay::modifyMemoryTypeIndexInAllocateMemoryPacket(VkDevice remappedDevice, packet_vkAllocateMemory *pPacket) {
    bool rval = false;
    uint32_t replayMemTypeIndex;
    vktrace_trace_packet_header packetHeader1;
    VkDeviceMemory traceAllocateMemoryRval = VK_NULL_HANDLE;
    bool foundBindMem;
    VkDeviceSize bimMemoryOffset;
    packet_vkFreeMemory freeMemoryPacket;
    void *pFullBindTracePacket;
    VkMemoryRequirements memRequirements;
    size_t saveFilePos = 0;
    size_t amIdx;
    VkDeviceSize replayAllocationSize;
    static size_t amSearchPos = 0;
    VkImage remappedImage = VK_NULL_HANDLE;
    size_t bindMemIdx;
    VkImage bindMemImage = VK_NULL_HANDLE;
    bool doDestroyImage = false;

    // Should only be here if we are in compatibility mode, have a valid portability table,  and the replay platform
    // does not match the trace platform
    assert(g_pReplaySettings->compatibilityMode && m_pFileHeader->portability_table_valid && !platformMatch());

    // Save current file position so we can restore it
    saveFilePos = vktrace_FileLike_GetCurrentPosition(traceFile);

    // First find this vkAM call in portabilityTable
    pPacket->header = (vktrace_trace_packet_header *)((PBYTE)pPacket - sizeof(vktrace_trace_packet_header));
    for (amIdx = amSearchPos; amIdx < portabilityTable.size(); amIdx++) {
        FSEEK(traceFile, (long)portabilityTable[amIdx], SEEK_SET);
        FREAD(&packetHeader1, sizeof(vktrace_trace_packet_header), 1, traceFile);  // Read the packet header

        if (packetHeader1.global_packet_index == pPacket->header->global_packet_index &&
            packetHeader1.packet_id == VKTRACE_TPI_VK_vkAllocateMemory) {
            // Found it
            // Save the vkAM return value from the trace file
            traceAllocateMemoryRval = *pPacket->pMemory;
            // Save the index where we will start the next search for vkAM
            amSearchPos = amIdx + 1;
            break;
        }
    }
    if (amIdx == portabilityTable.size()) {
        // Didn't find the current vkAM packet, something is wrong with the trace file.
        // Just use the index from the trace file and attempt to continue.
        vktrace_LogError("Replay of vkAllocateMemory() failed, trace file may be corrupt.");
        goto out;
    }

    // Search forward from amIdx for vkBIM/vkBBM/vkBIM2/vkBBM2 call that binds this memory.
    // If we don't find one, generate an error and do the best we can.
    foundBindMem = false;
    pFullBindTracePacket = NULL;
    for (size_t i = amIdx + 1; !foundBindMem && i < portabilityTable.size(); i++) {
        FSEEK(traceFile, (long)portabilityTable[i], SEEK_SET);
        FREAD(&packetHeader1, sizeof(vktrace_trace_packet_header), 1, traceFile);  // Read the packet header

        if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkFreeMemory) {
            FREAD(&freeMemoryPacket, sizeof(freeMemoryPacket), 1, traceFile);
            if (freeMemoryPacket.memory == traceAllocateMemoryRval) {
                // Found a free of this memory, end the forward search
                vktrace_LogWarning("Memory allocated by vkAllocateMemory is not used.");
                break;
            }
        } else if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory ||
                   packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindBufferMemory ||
                   packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory2KHR ||
                   packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindBufferMemory2KHR) {
            pFullBindTracePacket = (void *)vktrace_malloc(packetHeader1.size);
            FSEEK(traceFile, (long)portabilityTable[i], SEEK_SET);
            FREAD(pFullBindTracePacket, packetHeader1.size, 1, traceFile);
            ((vktrace_trace_packet_header *)pFullBindTracePacket)->pBody =
                (uintptr_t)((PBYTE)pFullBindTracePacket + sizeof(vktrace_trace_packet_header));
        }

        if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory2KHR ||
            packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindBufferMemory2KHR) {
            // Search the memory bind list in vkBIM2/vkBBM2 packet for traceAllocateMemoryRval
            packet_vkBindImageMemory2KHR *pBim2Packet =
                (packet_vkBindImageMemory2KHR *)(((vktrace_trace_packet_header *)pFullBindTracePacket)->pBody);
            pBim2Packet->pBindInfos = (VkBindImageMemoryInfoKHR *)vktrace_trace_packet_interpret_buffer_pointer(
                (vktrace_trace_packet_header *)pFullBindTracePacket, (intptr_t)pBim2Packet->pBindInfos);
            for (uint32_t bindCounter = 0; bindCounter < pBim2Packet->bindInfoCount; bindCounter++) {
                if (pBim2Packet->pBindInfos[bindCounter].memory == traceAllocateMemoryRval) {
                    // Found a bind
                    foundBindMem = true;
                    bindMemIdx = i;
                    bindMemImage = pBim2Packet->pBindInfos[bindCounter].image;
                    if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory2KHR)
                        remappedImage = m_objMapper.remap_images(pBim2Packet->pBindInfos[bindCounter].image);
                    else
                        remappedImage = (VkImage)m_objMapper.remap_buffers((VkBuffer)pBim2Packet->pBindInfos[bindCounter].image);
                    bimMemoryOffset = pBim2Packet->pBindInfos[bindCounter].memoryOffset;
                    break;
                }
            }
        } else if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory ||
                   packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindBufferMemory) {
            packet_vkBindImageMemory *bim =
                (packet_vkBindImageMemory *)(((vktrace_trace_packet_header *)pFullBindTracePacket)->pBody);
            if (traceAllocateMemoryRval == bim->memory) {
                // A vkBIM/vkBBM binds memory allocated by this vkAM call.
                foundBindMem = true;
                bindMemIdx = i;
                bindMemImage = bim->image;
                if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory)
                    remappedImage = m_objMapper.remap_images(bim->image);
                else
                    remappedImage = (VkImage)m_objMapper.remap_buffers((VkBuffer)bim->image);
                bimMemoryOffset = bim->memoryOffset;
            }
        }

        if (pFullBindTracePacket) {
            vktrace_free(pFullBindTracePacket);
            pFullBindTracePacket = NULL;
        }
    }

    if (!foundBindMem) {
        // Didn't find vkBind{Image|Buffer}Memory call for this vkAllocateMemory.
        // This isn't an error - the memory is allocated but never used.
        // So just use the index from the trace file and continue.
        goto out;
    }

    if (!remappedImage) {
        // The CreateImage/Buffer command after the AllocMem command, so the image/buffer hasn't
        // been created yet. Search backwards from the bindMem cmd for the CreateImage/Buffer
        // command and execute it
        // The newly created image/buffer needs to be destroyed after getting image/buffer memory requirements to keep the sequence
        // of API calls in the trace file.
        // The destroy will prevent from creating a buffer too early which may be used unexpectedly in a later call since two
        // buffers may have the same handle if one of them is created after another one being destroyed.
        // e.g. Without destroy, a dstBuffer may be used as srcBuffer unexpectedly in vkCmdCopyBuffer if the dstBuffer's memory is
        // allocated before the creation of the expected srcBuffer with the same buffer handle. (The srcBuffer is created and
        // destroyed before the dstBuffer being created.)
        for (size_t i = bindMemIdx - 1; true; i--) {
            vktrace_trace_packet_header createPacketHeaderHeader;
            vktrace_trace_packet_header *pCreatePacketFull;
            packet_vkCreateImage *pCreatePacket;
            FSEEK(traceFile, (long)portabilityTable[i], SEEK_SET);
            FREAD(&createPacketHeaderHeader, sizeof(vktrace_trace_packet_header), 1, traceFile);
            if ((packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory &&
                 createPacketHeaderHeader.packet_id == VKTRACE_TPI_VK_vkCreateImage) ||
                (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindBufferMemory &&
                 createPacketHeaderHeader.packet_id == VKTRACE_TPI_VK_vkCreateBuffer)) {
                // Read the whole packet
                pCreatePacketFull = (vktrace_trace_packet_header *)vktrace_malloc(createPacketHeaderHeader.size);
                if (!pCreatePacketFull) {
                    vktrace_LogError("malloc failed during vkAllocateMemory()");
                    vktrace_FileLike_SetCurrentPosition(traceFile, saveFilePos);
                    return false;
                }
                FSEEK(traceFile, (long)portabilityTable[i], SEEK_SET);
                FREAD(pCreatePacketFull, createPacketHeaderHeader.size, 1, traceFile);
                pCreatePacket = (packet_vkCreateImage *)(pCreatePacketFull + 1);
                pCreatePacket->header = pCreatePacketFull;
                pCreatePacketFull->pBody = (uintptr_t)pCreatePacket;
                pCreatePacket->pImage =
                    (VkImage *)vktrace_trace_packet_interpret_buffer_pointer(pCreatePacketFull, (intptr_t)pCreatePacket->pImage);
                pCreatePacket->pCreateInfo = (VkImageCreateInfo *)vktrace_trace_packet_interpret_buffer_pointer(
                    pCreatePacketFull, (intptr_t)pCreatePacket->pCreateInfo);
                pCreatePacket->pAllocator = (VkAllocationCallbacks *)vktrace_trace_packet_interpret_buffer_pointer(
                    pCreatePacketFull, (intptr_t)pCreatePacket->pAllocator);
                if (*(pCreatePacket->pImage) == bindMemImage) {
                    VkResult replayResult;
                    // Create the image/buffer
                    if (createPacketHeaderHeader.packet_id == VKTRACE_TPI_VK_vkCreateBuffer)
                        replayResult = manually_replay_vkCreateBuffer((packet_vkCreateBuffer *)pCreatePacket);
                    else
                        replayResult = manually_replay_vkCreateImage((packet_vkCreateImage *)pCreatePacket);
                    vktrace_free(pCreatePacketFull);
                    if (replayResult != VK_SUCCESS) {
                        vktrace_LogError("vkCreateBuffer/Image failed during vkAllocateMemory()");
                        vktrace_FileLike_SetCurrentPosition(traceFile, saveFilePos);
                        return false;
                    }
                    if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory)
                        remappedImage = m_objMapper.remap_images(bindMemImage);
                    else
                        remappedImage = (VkImage)m_objMapper.remap_buffers((VkBuffer)bindMemImage);
                    doDestroyImage = true;
                    break;
                }
                vktrace_free(pCreatePacketFull);
            }
            if (i == amIdx) {
                // This image/buffer is not created before it is bound
                vktrace_LogError("Bad buffer/image in call to vkBindImageMemory/vkBindBuffer");
                vktrace_FileLike_SetCurrentPosition(traceFile, saveFilePos);
                return false;
            }
        }
    }

    // Call GIMR/GBMR for the replay image/buffer
    if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory ||
        packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory2KHR) {
        if (replayGetImageMemoryRequirements.find(remappedImage) == replayGetImageMemoryRequirements.end()) {
            m_vkDeviceFuncs.GetImageMemoryRequirements(remappedDevice, remappedImage, &memRequirements);
            replayGetImageMemoryRequirements[remappedImage] = memRequirements;
        }
        memRequirements = replayGetImageMemoryRequirements[remappedImage];
    } else {
        if (replayGetBufferMemoryRequirements.find((VkBuffer)remappedImage) == replayGetBufferMemoryRequirements.end()) {
            m_vkDeviceFuncs.GetBufferMemoryRequirements(remappedDevice, (VkBuffer)remappedImage, &memRequirements);
            replayGetBufferMemoryRequirements[(VkBuffer)remappedImage] = memRequirements;
        }
        memRequirements = replayGetBufferMemoryRequirements[(VkBuffer)remappedImage];
    }

    replayAllocationSize = memRequirements.size;
    if (bimMemoryOffset > 0) {
        // Do alignment for allocationSize in traced vkBIM/vkBBM
        VkDeviceSize traceAllocationSize = *((VkDeviceSize *)&pPacket->pAllocateInfo->allocationSize);
        VkDeviceSize alignedAllocationSize =
            ((traceAllocationSize + memRequirements.alignment - 1) / memRequirements.alignment) * memRequirements.alignment;

        // Do alignment for memory offset
        replayAllocationSize +=
            ((bimMemoryOffset + memRequirements.alignment - 1) / memRequirements.alignment) * memRequirements.alignment;
        if (alignedAllocationSize != replayAllocationSize) {
            vktrace_LogWarning("alignedAllocationSize: 0x%x does not match replayAllocationSize: 0x%x", alignedAllocationSize,
                               replayAllocationSize);
        }
    }

    if (getMemoryTypeIdx(pPacket->device, remappedDevice, pPacket->pAllocateInfo->memoryTypeIndex, &memRequirements,
                         &replayMemTypeIndex)) {
        *((uint32_t *)&pPacket->pAllocateInfo->memoryTypeIndex) = replayMemTypeIndex;
        if (*((VkDeviceSize *)&pPacket->pAllocateInfo->allocationSize) < replayAllocationSize)
            *((VkDeviceSize *)&pPacket->pAllocateInfo->allocationSize) = replayAllocationSize;
        rval = true;
    } else {
        vktrace_LogError("vkAllocateMemory() failed, couldn't find memory type for memoryTypeIndex");
    }

out:
    if (doDestroyImage) {
        // Destroy temporarily created image/buffer and clean up obj map.
        if (packetHeader1.packet_id == VKTRACE_TPI_VK_vkBindImageMemory) {
            m_vkDeviceFuncs.DestroyImage(remappedDevice, remappedImage, NULL);
            m_objMapper.rm_from_images_map(bindMemImage);
            if (replayGetImageMemoryRequirements.find(remappedImage) != replayGetImageMemoryRequirements.end())
                replayGetImageMemoryRequirements.erase(remappedImage);
        } else {
            m_vkDeviceFuncs.DestroyBuffer(remappedDevice, (VkBuffer)remappedImage, NULL);
            m_objMapper.rm_from_buffers_map((VkBuffer)bindMemImage);
            if (replayGetBufferMemoryRequirements.find((VkBuffer)remappedImage) != replayGetBufferMemoryRequirements.end())
                replayGetBufferMemoryRequirements.erase((VkBuffer)remappedImage);
        }
    }

    vktrace_FileLike_SetCurrentPosition(traceFile, saveFilePos);
    return rval;
}

VkResult vkReplay::manually_replay_vkAllocateMemory(packet_vkAllocateMemory *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    devicememoryObj local_mem;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkAllocateMemory() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (pPacket->pAllocateInfo->pNext) {
        VkDedicatedAllocationMemoryAllocateInfoNV *x = (VkDedicatedAllocationMemoryAllocateInfoNV *)(pPacket->pAllocateInfo->pNext);

        if (x->sType == VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV ||
            x->sType == VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO) {
            x->image = m_objMapper.remap_images(x->image);
            x->buffer = m_objMapper.remap_buffers(x->buffer);
            if (!(x->image == VK_NULL_HANDLE || x->buffer == VK_NULL_HANDLE))
                vktrace_LogError("Invalid handle in vkAllocateMemory pAllocate->pNext structure.");
        }
    }

    // Map memory type index from trace platform to memory type index on replay platform
    // Only do this if compatibility mode is enabled, we have a portability table, and
    // the trace and replay platforms are not identical.
    bool doAllocate = true;
    if (g_pReplaySettings->compatibilityMode && m_pFileHeader->portability_table_valid && !platformMatch())
        doAllocate = modifyMemoryTypeIndexInAllocateMemoryPacket(remappedDevice, pPacket);

    if (doAllocate)
        replayResult = m_vkDeviceFuncs.AllocateMemory(remappedDevice, pPacket->pAllocateInfo, NULL, &local_mem.replayDeviceMemory);

    if (replayResult == VK_SUCCESS) {
        local_mem.pGpuMem = new (gpuMemory);
        if (local_mem.pGpuMem) local_mem.pGpuMem->setAllocInfo(pPacket->pAllocateInfo, false);
        m_objMapper.add_to_devicememorys_map(*(pPacket->pMemory), local_mem);
    } else {
        if (doAllocate) {
            vktrace_LogError("Allocate Memory 0x%lX failed with result = 0x%X\n", *(pPacket->pMemory), replayResult);
        } else {
            vktrace_LogWarning("Skipping Allocate Memory 0x%lX, it is not bound to any image/buffer\n", *(pPacket->pMemory));
        }
    }
    return replayResult;
}

void vkReplay::manually_replay_vkFreeMemory(packet_vkFreeMemory *pPacket) {
    if (pPacket->memory == VK_NULL_HANDLE) {
        return;
    }

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkFreeMemory() due to invalid remapped VkDevice.");
        return;
    }

    if (m_objMapper.m_devicememorys.find(pPacket->memory) == m_objMapper.m_devicememorys.end()) {
        vktrace_LogWarning("Skipping vkFreeMemory() due to invalid remapped VkDeviceMemory.");
        return;
    }

    devicememoryObj local_mem;
    local_mem = m_objMapper.m_devicememorys.find(pPacket->memory)->second;
    // TODO how/when to free pendingAlloc that did not use and existing devicememoryObj
    m_vkDeviceFuncs.FreeMemory(remappedDevice, local_mem.replayDeviceMemory, NULL);
    delete local_mem.pGpuMem;
    m_objMapper.rm_from_devicememorys_map(pPacket->memory);
}

VkResult vkReplay::manually_replay_vkMapMemory(packet_vkMapMemory *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkMapMemory() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (m_objMapper.m_devicememorys.find(pPacket->memory) == m_objMapper.m_devicememorys.end()) {
        vktrace_LogError("Skipping vkMapMemory() due to invalid remapped VkDeviceMemory.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    devicememoryObj local_mem = m_objMapper.m_devicememorys.find(pPacket->memory)->second;
    void *pData;
    if (!local_mem.pGpuMem->isPendingAlloc()) {
        replayResult = m_vkDeviceFuncs.MapMemory(remappedDevice, local_mem.replayDeviceMemory, pPacket->offset, pPacket->size,
                                                 pPacket->flags, &pData);
        if (replayResult == VK_SUCCESS) {
            if (local_mem.pGpuMem) {
                local_mem.pGpuMem->setMemoryMapRange(pData, (size_t)pPacket->size, (size_t)pPacket->offset, false);
            }
        }
    } else {
        if (local_mem.pGpuMem) {
            local_mem.pGpuMem->setMemoryMapRange(NULL, (size_t)pPacket->size, (size_t)pPacket->offset, true);
        }
    }
    return replayResult;
}

void vkReplay::manually_replay_vkUnmapMemory(packet_vkUnmapMemory *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkUnmapMemory() due to invalid remapped VkDevice.");
        return;
    }

    if (m_objMapper.m_devicememorys.find(pPacket->memory) == m_objMapper.m_devicememorys.end()) {
        vktrace_LogError("Skipping vkUnmapMemory() due to invalid remapped VkDeviceMemory.");
        return;
    }

    devicememoryObj local_mem = m_objMapper.m_devicememorys.find(pPacket->memory)->second;
    if (!local_mem.pGpuMem->isPendingAlloc()) {
        if (local_mem.pGpuMem) {
            if (pPacket->pData)
                local_mem.pGpuMem->copyMappingData(pPacket->pData, true, 0, 0);  // copies data from packet into memory buffer
        }
        m_vkDeviceFuncs.UnmapMemory(remappedDevice, local_mem.replayDeviceMemory);
    } else {
        if (local_mem.pGpuMem) {
            unsigned char *pBuf = (unsigned char *)vktrace_malloc(local_mem.pGpuMem->getMemoryMapSize());
            if (!pBuf) {
                vktrace_LogError("vkUnmapMemory() malloc failed.");
            }
            local_mem.pGpuMem->setMemoryDataAddr(pBuf);
            local_mem.pGpuMem->copyMappingData(pPacket->pData, true, 0, 0);
        }
    }
}

BOOL isvkFlushMappedMemoryRangesSpecial(PBYTE pOPTPackageData) {
    BOOL bRet = FALSE;
    PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)pOPTPackageData;
    if (((uint64_t)pChangedInfoArray[0].reserve0) &
        PAGEGUARD_SPECIAL_FORMAT_PACKET_FOR_VKFLUSHMAPPEDMEMORYRANGES)  // TODO need think about 32bit
    {
        bRet = TRUE;
    }
    return bRet;
}

// after OPT speed up, the format of this packet will be different with before, the packet now only include changed block(page).
//
VkResult vkReplay::manually_replay_vkFlushMappedMemoryRanges(packet_vkFlushMappedMemoryRanges *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkFlushMappedMemoryRanges() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkMappedMemoryRange *localRanges = VKTRACE_NEW_ARRAY(VkMappedMemoryRange, pPacket->memoryRangeCount);
    memcpy(localRanges, pPacket->pMemoryRanges, sizeof(VkMappedMemoryRange) * (pPacket->memoryRangeCount));

    devicememoryObj *pLocalMems = VKTRACE_NEW_ARRAY(devicememoryObj, pPacket->memoryRangeCount);
    for (uint32_t i = 0; i < pPacket->memoryRangeCount; i++) {
        if (m_objMapper.m_devicememorys.find(pPacket->pMemoryRanges[i].memory) != m_objMapper.m_devicememorys.end()) {
            pLocalMems[i] = m_objMapper.m_devicememorys.find(pPacket->pMemoryRanges[i].memory)->second;
        }
        localRanges[i].memory = m_objMapper.remap_devicememorys(pPacket->pMemoryRanges[i].memory);
        if (localRanges[i].memory == VK_NULL_HANDLE || pLocalMems[i].pGpuMem == NULL) {
            vktrace_LogError("Skipping vkFlushMappedMemoryRanges() due to invalid remapped VkDeviceMemory.");
            VKTRACE_DELETE(localRanges);
            VKTRACE_DELETE(pLocalMems);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        if (!pLocalMems[i].pGpuMem->isPendingAlloc()) {
            if (pPacket->pMemoryRanges[i].size != 0) {
#if defined(USE_PAGEGUARD_SPEEDUP)
                if (vktrace_check_min_version(VKTRACE_TRACE_FILE_VERSION_5))
                    pLocalMems[i].pGpuMem->copyMappingDataPageGuard(pPacket->ppData[i]);
                else
                    pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size,
                                                           (size_t)pPacket->pMemoryRanges[i].offset);
#else
                pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size,
                                                       (size_t)pPacket->pMemoryRanges[i].offset);
#endif
            }
        } else {
            unsigned char *pBuf = (unsigned char *)vktrace_malloc(pLocalMems[i].pGpuMem->getMemoryMapSize());
            if (!pBuf) {
                vktrace_LogError("vkFlushMappedMemoryRanges() malloc failed.");
            }
            pLocalMems[i].pGpuMem->setMemoryDataAddr(pBuf);
#if defined(USE_PAGEGUARD_SPEEDUP)
            if (vktrace_check_min_version(VKTRACE_TRACE_FILE_VERSION_5))
                pLocalMems[i].pGpuMem->copyMappingDataPageGuard(pPacket->ppData[i]);
            else
                pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size,
                                                       (size_t)pPacket->pMemoryRanges[i].offset);
#else
            pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size,
                                                   (size_t)pPacket->pMemoryRanges[i].offset);
#endif
        }
    }

#if defined(USE_PAGEGUARD_SPEEDUP)
    replayResult = pPacket->result;  // if this is a OPT refresh-all packet, we need avoid to call real api and return original
                                     // return to avoid error message;
    if (!vktrace_check_min_version(VKTRACE_TRACE_FILE_VERSION_5) || !isvkFlushMappedMemoryRangesSpecial((PBYTE)pPacket->ppData[0]))
#endif
    {
        replayResult = m_vkDeviceFuncs.FlushMappedMemoryRanges(remappedDevice, pPacket->memoryRangeCount, localRanges);
    }

    VKTRACE_DELETE(localRanges);
    VKTRACE_DELETE(pLocalMems);

    return replayResult;
}

// InvalidateMappedMemory Ranges and flushMappedMemoryRanges are similar but keep it seperate until
// functionality tested fully
VkResult vkReplay::manually_replay_vkInvalidateMappedMemoryRanges(packet_vkInvalidateMappedMemoryRanges *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkInvalidateMappedMemoryRanges() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkMappedMemoryRange *localRanges = VKTRACE_NEW_ARRAY(VkMappedMemoryRange, pPacket->memoryRangeCount);
    memcpy(localRanges, pPacket->pMemoryRanges, sizeof(VkMappedMemoryRange) * (pPacket->memoryRangeCount));

    devicememoryObj *pLocalMems = VKTRACE_NEW_ARRAY(devicememoryObj, pPacket->memoryRangeCount);
    for (uint32_t i = 0; i < pPacket->memoryRangeCount; i++) {
        if (m_objMapper.m_devicememorys.find(pPacket->pMemoryRanges[i].memory) != m_objMapper.m_devicememorys.end()) {
            pLocalMems[i] = m_objMapper.m_devicememorys.find(pPacket->pMemoryRanges[i].memory)->second;
        }
        localRanges[i].memory = m_objMapper.remap_devicememorys(pPacket->pMemoryRanges[i].memory);
        if (localRanges[i].memory == VK_NULL_HANDLE || pLocalMems[i].pGpuMem == NULL) {
            vktrace_LogError("Skipping vkInvalidsateMappedMemoryRanges() due to invalid remapped VkDeviceMemory.");
            VKTRACE_DELETE(localRanges);
            VKTRACE_DELETE(pLocalMems);
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }

        if (!pLocalMems[i].pGpuMem->isPendingAlloc()) {
            if (pPacket->pMemoryRanges[i].size != 0) {
                pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size,
                                                       (size_t)pPacket->pMemoryRanges[i].offset);
            }
        } else {
            unsigned char *pBuf = (unsigned char *)vktrace_malloc(pLocalMems[i].pGpuMem->getMemoryMapSize());
            if (!pBuf) {
                vktrace_LogError("vkInvalidateMappedMemoryRanges() malloc failed.");
            }
            pLocalMems[i].pGpuMem->setMemoryDataAddr(pBuf);
            pLocalMems[i].pGpuMem->copyMappingData(pPacket->ppData[i], false, (size_t)pPacket->pMemoryRanges[i].size,
                                                   (size_t)pPacket->pMemoryRanges[i].offset);
        }
    }

    replayResult = m_vkDeviceFuncs.InvalidateMappedMemoryRanges(remappedDevice, pPacket->memoryRangeCount, localRanges);

    VKTRACE_DELETE(localRanges);
    VKTRACE_DELETE(pLocalMems);

    return replayResult;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceProperties(packet_vkGetPhysicalDeviceProperties *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetPhysicalDeviceProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }
    m_vkFuncs.GetPhysicalDeviceProperties(remappedphysicalDevice, pPacket->pProperties);
    m_replay_gpu = ((uint64_t)pPacket->pProperties->vendorID << 32) | (uint64_t)pPacket->pProperties->deviceID;
    m_replay_drv_vers = (uint64_t)pPacket->pProperties->driverVersion;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceProperties2KHR(packet_vkGetPhysicalDeviceProperties2KHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetPhysicalDeviceProperties2KHR() due to invalid remapped VkPhysicalDevice.");
        return;
    }
    vktrace_interpret_pnext_pointers(pPacket->header, (void *)pPacket->pProperties);
    // No need to remap pProperties
    m_vkFuncs.GetPhysicalDeviceProperties2KHR(remappedphysicalDevice, pPacket->pProperties);
    m_replay_gpu =
        ((uint64_t)pPacket->pProperties->properties.vendorID << 32) | (uint64_t)pPacket->pProperties->properties.deviceID;
    m_replay_drv_vers = (uint64_t)pPacket->pProperties->properties.driverVersion;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfaceSupportKHR(packet_vkGetPhysicalDeviceSurfaceSupportKHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceSupportKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    replayResult = m_vkFuncs.GetPhysicalDeviceSurfaceSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                remappedSurfaceKHR, pPacket->pSupported);

    return replayResult;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceMemoryProperties(packet_vkGetPhysicalDeviceMemoryProperties *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceMemoryProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    traceMemoryProperties[pPacket->physicalDevice] = *(pPacket->pMemoryProperties);
    m_vkFuncs.GetPhysicalDeviceMemoryProperties(remappedphysicalDevice, pPacket->pMemoryProperties);
    replayMemoryProperties[remappedphysicalDevice] = *(pPacket->pMemoryProperties);
    return;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceMemoryProperties2KHR(packet_vkGetPhysicalDeviceMemoryProperties2KHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceMemoryProperties2KHR() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    traceMemoryProperties[pPacket->physicalDevice] = pPacket->pMemoryProperties->memoryProperties;
    m_vkFuncs.GetPhysicalDeviceMemoryProperties2KHR(remappedphysicalDevice, pPacket->pMemoryProperties);
    replayMemoryProperties[remappedphysicalDevice] = pPacket->pMemoryProperties->memoryProperties;
    return;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceQueueFamilyProperties(packet_vkGetPhysicalDeviceQueueFamilyProperties *pPacket) {
    static std::unordered_map<VkPhysicalDevice, uint32_t> queueFamPropCnt;  // count returned when pQueueFamilyProperties is NULL
    VkQueueFamilyProperties *savepQueueFamilyProperties = NULL;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);

    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceQueueFamilyProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    if (queueFamPropCnt.find(pPacket->physicalDevice) != queueFamPropCnt.end()) {
        // This query was previously done with pQueueFamilyProperties set to null. It was a query
        // to determine the size of data to be returned. We saved the size returned during
        // that api call. Use that count instead to prevent a VK_INCOMPLETE error.
        if (queueFamPropCnt[pPacket->physicalDevice] > *pPacket->pQueueFamilyPropertyCount) {
            *pPacket->pQueueFamilyPropertyCount = queueFamPropCnt[pPacket->physicalDevice];
            savepQueueFamilyProperties = pPacket->pQueueFamilyProperties;
            pPacket->pQueueFamilyProperties = VKTRACE_NEW_ARRAY(VkQueueFamilyProperties, *pPacket->pQueueFamilyPropertyCount);
        }
    }

    // If we haven't previously allocated queueFamilyProperties for the trace physical device, allocate it.
    // If we previously allocated queueFamilyProperities for the trace physical device and the size of this
    // query is larger than what we saved last time, then free the last properties map and allocate a new map.
    if (traceQueueFamilyProperties.find(pPacket->physicalDevice) == traceQueueFamilyProperties.end() ||
        *pPacket->pQueueFamilyPropertyCount > traceQueueFamilyProperties[pPacket->physicalDevice].count) {
        if (traceQueueFamilyProperties.find(pPacket->physicalDevice) != traceQueueFamilyProperties.end()) {
            free(traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties);
            traceQueueFamilyProperties.erase(pPacket->physicalDevice);
        }
        if (pPacket->pQueueFamilyProperties) {
            traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties =
                (VkQueueFamilyProperties *)malloc(*pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            memcpy(traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties, pPacket->pQueueFamilyProperties,
                   *pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            traceQueueFamilyProperties[pPacket->physicalDevice].count = *pPacket->pQueueFamilyPropertyCount;
        }
    }

    m_vkFuncs.GetPhysicalDeviceQueueFamilyProperties(remappedphysicalDevice, pPacket->pQueueFamilyPropertyCount,
                                                     pPacket->pQueueFamilyProperties);

    // If we haven't previously allocated queueFamilyProperties for the replay physical device, allocate it.
    // If we previously allocated queueFamilyProperities for the replay physical device and the size of this
    // query is larger than what we saved last time, then free the last properties map and allocate a new map.
    if (replayQueueFamilyProperties.find(remappedphysicalDevice) == replayQueueFamilyProperties.end() ||
        *pPacket->pQueueFamilyPropertyCount > replayQueueFamilyProperties[remappedphysicalDevice].count) {
        if (replayQueueFamilyProperties.find(remappedphysicalDevice) != replayQueueFamilyProperties.end()) {
            free(replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties);
            replayQueueFamilyProperties.erase(remappedphysicalDevice);
        }
        if (pPacket->pQueueFamilyProperties) {
            replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties =
                (VkQueueFamilyProperties *)malloc(*pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            memcpy(replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties, pPacket->pQueueFamilyProperties,
                   *pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            replayQueueFamilyProperties[remappedphysicalDevice].count = *pPacket->pQueueFamilyPropertyCount;
        }
    }

    if (!pPacket->pQueueFamilyProperties) {
        // This was a query to determine size. Save the returned size so we can use that size next time
        // we're called with pQueueFamilyProperties not null. This is to prevent a VK_INCOMPLETE error.
        queueFamPropCnt[pPacket->physicalDevice] = *pPacket->pQueueFamilyPropertyCount;
    }

    if (savepQueueFamilyProperties) {
        // Restore pPacket->pQueueFamilyProperties. We do this because the replay will free the memory.
        // Note that we don't copy the queried data - it wouldn't fit, and it's not used by the replayer anyway.
        VKTRACE_DELETE(pPacket->pQueueFamilyProperties);
        pPacket->pQueueFamilyProperties = savepQueueFamilyProperties;
    }

    return;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceQueueFamilyProperties2KHR(
    packet_vkGetPhysicalDeviceQueueFamilyProperties2KHR *pPacket) {
    static std::unordered_map<VkPhysicalDevice, uint32_t>
        queueFamProp2KHRCnt;  // count returned when pQueueFamilyProperties is NULL
    VkQueueFamilyProperties2KHR *savepQueueFamilyProperties = NULL;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);

    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceQueueFamilyProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    if (queueFamProp2KHRCnt.find(pPacket->physicalDevice) != queueFamProp2KHRCnt.end()) {
        // This query was previously done with pQueueFamilyProperties set to null. It was a query
        // to determine the size of data to be returned. We saved the size returned during
        // that api call. Use that count instead to prevent a VK_INCOMPLETE error.
        if (queueFamProp2KHRCnt[pPacket->physicalDevice] > *pPacket->pQueueFamilyPropertyCount) {
            *pPacket->pQueueFamilyPropertyCount = queueFamProp2KHRCnt[pPacket->physicalDevice];
            savepQueueFamilyProperties = pPacket->pQueueFamilyProperties;
            pPacket->pQueueFamilyProperties = VKTRACE_NEW_ARRAY(VkQueueFamilyProperties2KHR, *pPacket->pQueueFamilyPropertyCount);
        }
    }

    // If we haven't previously allocated queueFamilyProperties for the trace physical device, allocate it.
    // If we previously allocated queueFamilyProperities for the trace physical device and the size of this
    // query is larger than what we saved last time, then free the last properties map and allocate a new map.
    if (traceQueueFamilyProperties.find(pPacket->physicalDevice) == traceQueueFamilyProperties.end() ||
        *pPacket->pQueueFamilyPropertyCount > traceQueueFamilyProperties[pPacket->physicalDevice].count) {
        if (traceQueueFamilyProperties.find(pPacket->physicalDevice) != traceQueueFamilyProperties.end()) {
            free(traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties);
            traceQueueFamilyProperties.erase(pPacket->physicalDevice);
        }
        if (pPacket->pQueueFamilyProperties) {
            traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties =
                (VkQueueFamilyProperties *)malloc(*pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            memcpy(traceQueueFamilyProperties[pPacket->physicalDevice].queueFamilyProperties, pPacket->pQueueFamilyProperties,
                   *pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            traceQueueFamilyProperties[pPacket->physicalDevice].count = *pPacket->pQueueFamilyPropertyCount;
        }
    }

    m_vkFuncs.GetPhysicalDeviceQueueFamilyProperties2KHR(remappedphysicalDevice, pPacket->pQueueFamilyPropertyCount,
                                                         pPacket->pQueueFamilyProperties);

    // If we haven't previously allocated queueFamilyProperties for the replay physical device, allocate it.
    // If we previously allocated queueFamilyProperities for the replay physical device and the size of this
    // query is larger than what we saved last time, then free the last properties map and allocate a new map.
    if (replayQueueFamilyProperties.find(remappedphysicalDevice) == replayQueueFamilyProperties.end() ||
        *pPacket->pQueueFamilyPropertyCount > replayQueueFamilyProperties[remappedphysicalDevice].count) {
        if (replayQueueFamilyProperties.find(remappedphysicalDevice) != replayQueueFamilyProperties.end()) {
            free(replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties);
            replayQueueFamilyProperties.erase(remappedphysicalDevice);
        }
        if (pPacket->pQueueFamilyProperties) {
            replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties =
                (VkQueueFamilyProperties *)malloc(*pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            memcpy(replayQueueFamilyProperties[remappedphysicalDevice].queueFamilyProperties, pPacket->pQueueFamilyProperties,
                   *pPacket->pQueueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
            replayQueueFamilyProperties[remappedphysicalDevice].count = *pPacket->pQueueFamilyPropertyCount;
        }
    }

    if (!pPacket->pQueueFamilyProperties) {
        // This was a query to determine size. Save the returned size so we can use that size next time
        // we're called with pQueueFamilyProperties not null. This is to prevent a VK_INCOMPLETE error.
        queueFamProp2KHRCnt[pPacket->physicalDevice] = *pPacket->pQueueFamilyPropertyCount;
    }

    if (savepQueueFamilyProperties) {
        // Restore pPacket->pQueueFamilyProperties. We do this because the replay will free the memory.
        // Note that we don't copy the queried data - it wouldn't fit, and it's not used by the replayer anyway.
        VKTRACE_DELETE(pPacket->pQueueFamilyProperties);
        pPacket->pQueueFamilyProperties = savepQueueFamilyProperties;
    }

    return;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceSparseImageFormatProperties(
    packet_vkGetPhysicalDeviceSparseImageFormatProperties *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    static std::unordered_map<VkPhysicalDevice, uint32_t> propCnt;  // count returned when pProperties is NULL
    VkSparseImageFormatProperties *savepProperties = NULL;

    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSparseImageFormatProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    if (propCnt.find(pPacket->physicalDevice) != propCnt.end()) {
        // This query was previously done with pProperties set to null. It was a query
        // to determine the size of data to be returned. We saved the size returned during
        // that api call. Use that count instead to prevent a VK_INCOMPLETE error.
        if (propCnt[pPacket->physicalDevice] > *pPacket->pPropertyCount) {
            *pPacket->pPropertyCount = propCnt[pPacket->physicalDevice];
            savepProperties = pPacket->pProperties;
            pPacket->pProperties = VKTRACE_NEW_ARRAY(VkSparseImageFormatProperties, *pPacket->pPropertyCount);
        }
    }

    m_vkFuncs.GetPhysicalDeviceSparseImageFormatProperties(remappedphysicalDevice, pPacket->format, pPacket->type, pPacket->samples,
                                                           pPacket->usage, pPacket->tiling, pPacket->pPropertyCount,
                                                           pPacket->pProperties);

    if (!pPacket->pProperties) {
        // This was a query to determine size. Save the returned size so we can use that size next time
        // we're called with pProperties not null. This is to prevent a VK_INCOMPLETE error.
        propCnt[pPacket->physicalDevice] = *pPacket->pPropertyCount;
    }

    if (savepProperties) {
        // Restore pPacket->pProperties. We do this because the replay will free the memory.
        // Note that we don't copy the queried data - it wouldn't fit, and it's not used by the replayer anyway.
        VKTRACE_DELETE(pPacket->pProperties);
        pPacket->pProperties = savepProperties;
    }

    return;
}

void vkReplay::manually_replay_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(
    packet_vkGetPhysicalDeviceSparseImageFormatProperties2KHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    static std::unordered_map<VkPhysicalDevice, uint32_t> prop2KHRCnt;  // count returned when pProperties is NULL
    VkSparseImageFormatProperties2KHR *savepProperties = NULL;

    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSparseImageFormatProperties() due to invalid remapped VkPhysicalDevice.");
        return;
    }

    if (prop2KHRCnt.find(pPacket->physicalDevice) != prop2KHRCnt.end()) {
        // This query was previously done with pProperties set to null. It was a query
        // to determine the size of data to be returned. We saved the size returned during
        // that api call. Use that count instead to prevent a VK_INCOMPLETE error.
        if (prop2KHRCnt[pPacket->physicalDevice] > *pPacket->pPropertyCount) {
            *pPacket->pPropertyCount = prop2KHRCnt[pPacket->physicalDevice];
            savepProperties = pPacket->pProperties;
            pPacket->pProperties = VKTRACE_NEW_ARRAY(VkSparseImageFormatProperties2KHR, *pPacket->pPropertyCount);
        }
    }

    m_vkFuncs.GetPhysicalDeviceSparseImageFormatProperties2KHR(remappedphysicalDevice, pPacket->pFormatInfo,
                                                               pPacket->pPropertyCount, pPacket->pProperties);

    if (!pPacket->pProperties) {
        // This was a query to determine size. Save the returned size so we can use that size next time
        // we're called with pProperties not null. This is to prevent a VK_INCOMPLETE error.
        prop2KHRCnt[pPacket->physicalDevice] = *pPacket->pPropertyCount;
    }

    if (savepProperties) {
        // Restore pPacket->pProperties. We do this because the replay will free the memory.
        // Note that we don't copy the queried data - it wouldn't fit, and it's not used by the replayer anyway.
        VKTRACE_DELETE(pPacket->pProperties);
        pPacket->pProperties = savepProperties;
    }

    return;
}

VkResult vkReplay::manually_replay_vkBindBufferMemory(packet_vkBindBufferMemory *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindBufferMemory() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }
    VkBuffer remappedbuffer = m_objMapper.remap_buffers(pPacket->buffer);
    if (pPacket->buffer != VK_NULL_HANDLE && remappedbuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindBufferMemory() due to invalid remapped VkBuffer.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }
    VkDeviceMemory remappedmemory = m_objMapper.remap_devicememorys(pPacket->memory);
    if (pPacket->memory != VK_NULL_HANDLE && remappedmemory == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindBufferMemory() due to invalid remapped VkDeviceMemory.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (g_pReplaySettings->compatibilityMode && m_pFileHeader->portability_table_valid && !platformMatch()) {
        uint64_t memOffsetTemp;
        if (replayGetBufferMemoryRequirements.find(remappedbuffer) == replayGetBufferMemoryRequirements.end()) {
            // vkBindBufferMemory is being called on a buffer for which vkGetBufferMemoryRequirements
            // was not called. This might be violation of the spec on the part of the app, but seems to
            // be done in many apps.  Call vkGetBufferMemoryRequirements for this buffer and add result to
            // replayGetBufferMemoryRequirements map.
            VkMemoryRequirements mem_reqs;
            m_vkDeviceFuncs.GetBufferMemoryRequirements(remappeddevice, remappedbuffer, &mem_reqs);
            replayGetBufferMemoryRequirements[remappedbuffer] = mem_reqs;
        }
        assert(replayGetBufferMemoryRequirements[remappedbuffer].alignment);
        memOffsetTemp = pPacket->memoryOffset + replayGetBufferMemoryRequirements[remappedbuffer].alignment - 1;
        memOffsetTemp = memOffsetTemp / replayGetBufferMemoryRequirements[remappedbuffer].alignment;
        memOffsetTemp = memOffsetTemp * replayGetBufferMemoryRequirements[remappedbuffer].alignment;
        replayResult = m_vkDeviceFuncs.BindBufferMemory(remappeddevice, remappedbuffer, remappedmemory, memOffsetTemp);
    } else {
        replayResult = m_vkDeviceFuncs.BindBufferMemory(remappeddevice, remappedbuffer, remappedmemory, pPacket->memoryOffset);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkBindImageMemory(packet_vkBindImageMemory *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindImageMemory() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }
    VkImage remappedimage = m_objMapper.remap_images(pPacket->image);
    if (pPacket->image != VK_NULL_HANDLE && remappedimage == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindImageMemory() due to invalid remapped VkImage.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }
    VkDeviceMemory remappedmemory = m_objMapper.remap_devicememorys(pPacket->memory);
    if (pPacket->memory != VK_NULL_HANDLE && remappedmemory == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindImageMemory() due to invalid remapped VkDeviceMemory.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (g_pReplaySettings->compatibilityMode && m_pFileHeader->portability_table_valid && !platformMatch()) {
        size_t memOffsetTemp;
        if (replayGetImageMemoryRequirements.find(remappedimage) == replayGetImageMemoryRequirements.end()) {
            // vkBindImageMemory is being called on a image for which vkGetImageMemoryRequirements
            // was not called. This might be violation of the spec on the part of the app, but seems to
            // be done in many apps.  Call vkGetImageMemoryRequirements for this image and add result to
            // replayGetImageMemoryRequirements map.
            VkMemoryRequirements mem_reqs;
            m_vkDeviceFuncs.GetImageMemoryRequirements(remappeddevice, remappedimage, &mem_reqs);
            replayGetImageMemoryRequirements[remappedimage] = mem_reqs;
        }

        assert(replayGetImageMemoryRequirements[remappedimage].alignment);
        memOffsetTemp = pPacket->memoryOffset + replayGetImageMemoryRequirements[remappedimage].alignment - 1;
        memOffsetTemp = memOffsetTemp / replayGetImageMemoryRequirements[remappedimage].alignment;
        memOffsetTemp = memOffsetTemp * replayGetImageMemoryRequirements[remappedimage].alignment;
        replayResult = m_vkDeviceFuncs.BindImageMemory(remappeddevice, remappedimage, remappedmemory, memOffsetTemp);
    } else {
        replayResult = m_vkDeviceFuncs.BindImageMemory(remappeddevice, remappedimage, remappedmemory, pPacket->memoryOffset);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkGetImageMemoryRequirements(packet_vkGetImageMemoryRequirements *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkGetImageMemoryRequirements() due to invalid remapped VkDevice.");
        return;
    }

    VkImage remappedImage = m_objMapper.remap_images(pPacket->image);
    if (pPacket->image != VK_NULL_HANDLE && remappedImage == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetImageMemoryRequirements() due to invalid remapped VkImage.");
        return;
    }

    m_vkDeviceFuncs.GetImageMemoryRequirements(remappedDevice, remappedImage, pPacket->pMemoryRequirements);
    replayGetImageMemoryRequirements[remappedImage] = *(pPacket->pMemoryRequirements);
    return;
}

void vkReplay::manually_replay_vkGetImageMemoryRequirements2KHR(packet_vkGetImageMemoryRequirements2KHR *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    VkImage remappedimage = VK_NULL_HANDLE;

    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetImageMemoryRequirements2KHR() due to invalid remapped VkDevice.");
        return;
    }

    if (pPacket->pInfo != nullptr) {
        remappedimage = m_objMapper.remap_images(pPacket->pInfo->image);

        if ((pPacket->pInfo->image != VK_NULL_HANDLE) && (remappedimage == VK_NULL_HANDLE)) {
            vktrace_LogError("Error detected in GetImageMemoryRequirements2KHR() due to invalid remapped VkImage.");
            return;
        }

        ((VkImageMemoryRequirementsInfo2KHR *)pPacket->pInfo)->image = remappedimage;
    }
    vktrace_interpret_pnext_pointers(pPacket->header, (void *)pPacket->pInfo);
    vktrace_interpret_pnext_pointers(pPacket->header, (void *)pPacket->pMemoryRequirements);
    m_vkDeviceFuncs.GetImageMemoryRequirements2KHR(remappeddevice, pPacket->pInfo, pPacket->pMemoryRequirements);

    replayGetImageMemoryRequirements[remappedimage] = pPacket->pMemoryRequirements->memoryRequirements;
}

void vkReplay::manually_replay_vkGetBufferMemoryRequirements(packet_vkGetBufferMemoryRequirements *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkGetBufferMemoryRequirements() due to invalid remapped VkDevice.");
        return;
    }

    VkBuffer remappedBuffer = m_objMapper.remap_buffers(pPacket->buffer);
    if (pPacket->buffer != VK_NULL_HANDLE && remappedBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetBufferMemoryRequirements() due to invalid remapped VkBuffer.");
        return;
    }

    m_vkDeviceFuncs.GetBufferMemoryRequirements(remappedDevice, remappedBuffer, pPacket->pMemoryRequirements);
    replayGetBufferMemoryRequirements[remappedBuffer] = *(pPacket->pMemoryRequirements);
    return;
}

void vkReplay::manually_replay_vkGetBufferMemoryRequirements2KHR(packet_vkGetBufferMemoryRequirements2KHR *pPacket) {
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in vkGetBufferMemoryRequirements2() due to invalid remapped VkDevice.");
        return;
    }

    VkBuffer remappedBuffer = m_objMapper.remap_buffers(pPacket->pInfo->buffer);
    if (pPacket->pInfo->buffer != VK_NULL_HANDLE && remappedBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetBufferMemoryRequirements2() due to invalid remapped VkBuffer.");
        return;
    }
    *((VkBuffer *)(&pPacket->pInfo->buffer)) = remappedBuffer;

    vktrace_interpret_pnext_pointers(pPacket->header, (void *)pPacket->pInfo);
    vktrace_interpret_pnext_pointers(pPacket->header, (void *)pPacket->pMemoryRequirements);
    m_vkDeviceFuncs.GetBufferMemoryRequirements2KHR(remappedDevice, pPacket->pInfo, pPacket->pMemoryRequirements);
    replayGetBufferMemoryRequirements[pPacket->pInfo->buffer] = pPacket->pMemoryRequirements->memoryRequirements;
    return;
}

void vkReplay::manually_replay_vkGetImageMemoryRequirements2(packet_vkGetImageMemoryRequirements2 *pPacket) {
    manually_replay_vkGetImageMemoryRequirements2KHR((packet_vkGetImageMemoryRequirements2KHR *)pPacket);
}

void vkReplay::manually_replay_vkGetBufferMemoryRequirements2(packet_vkGetBufferMemoryRequirements2 *pPacket) {
    manually_replay_vkGetBufferMemoryRequirements2KHR((packet_vkGetBufferMemoryRequirements2KHR *)pPacket);
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    packet_vkGetPhysicalDeviceSurfaceCapabilitiesKHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;

    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceCapabilitiesKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceCapabilitiesKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    m_display->resize_window(pPacket->pSurfaceCapabilities->currentExtent.width,
                             pPacket->pSurfaceCapabilities->currentExtent.height);

    replayResult = m_vkFuncs.GetPhysicalDeviceSurfaceCapabilitiesKHR(remappedphysicalDevice, remappedSurfaceKHR,
                                                                     pPacket->pSurfaceCapabilities);
    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfaceFormatsKHR(packet_vkGetPhysicalDeviceSurfaceFormatsKHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    static std::unordered_map<VkPhysicalDevice, uint32_t> surfFmtCnt;  // count returned when pSurfaceFormats is NULL
    VkSurfaceFormatKHR *savepSurfaceFormats = NULL;

    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceFormatsKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfaceFormatsKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (surfFmtCnt.find(pPacket->physicalDevice) != surfFmtCnt.end()) {
        // This query was previously done with pSurfaceFormats set to null. It was a query
        // to determine the size of data to be returned. We saved the size returned during
        // that api call. Use that count instead to prevent a VK_INCOMPLETE error.
        if (surfFmtCnt[pPacket->physicalDevice] > *pPacket->pSurfaceFormatCount) {
            *pPacket->pSurfaceFormatCount = surfFmtCnt[pPacket->physicalDevice];
            savepSurfaceFormats = pPacket->pSurfaceFormats;
            pPacket->pSurfaceFormats = VKTRACE_NEW_ARRAY(VkSurfaceFormatKHR, *pPacket->pSurfaceFormatCount);
        }
    }

    replayResult = m_vkFuncs.GetPhysicalDeviceSurfaceFormatsKHR(remappedphysicalDevice, remappedSurfaceKHR,
                                                                pPacket->pSurfaceFormatCount, pPacket->pSurfaceFormats);

    if (!pPacket->pSurfaceFormats) {
        // This was a query to determine size. Save the returned size so we can use that size next time
        // we're called with pSurfaceFormats not null. This is to prevent a VK_INCOMPLETE error.
        surfFmtCnt[pPacket->physicalDevice] = *pPacket->pSurfaceFormatCount;
    }

    if (savepSurfaceFormats) {
        // Restore pPacket->pSurfaceFormats. We do this because the replay will free the memory.
        // Note that we don't copy the queried data - it wouldn't fit, and it's not used by the replayer anyway.
        VKTRACE_DELETE(pPacket->pSurfaceFormats);
        pPacket->pSurfaceFormats = savepSurfaceFormats;
    }

    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetPhysicalDeviceSurfacePresentModesKHR(
    packet_vkGetPhysicalDeviceSurfacePresentModesKHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    static std::unordered_map<VkPhysicalDevice, uint32_t> presModeCnt;  // count returned when pPrsentModes is NULL
    VkPresentModeKHR *savepPresentModes = NULL;

    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfacePresentModesKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSurfaceKHR remappedSurfaceKHR = m_objMapper.remap_surfacekhrs(pPacket->surface);
    if (remappedSurfaceKHR == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetPhysicalDeviceSurfacePresentModesKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (presModeCnt.find(pPacket->physicalDevice) != presModeCnt.end()) {
        // This query was previously done with pSurfaceFormats set to null. It was a query
        // to determine the size of data to be returned. We saved the size returned during
        // that api call. Use that count instead to prevent a VK_INCOMPLETE error.
        if (presModeCnt[pPacket->physicalDevice] > *pPacket->pPresentModeCount) {
            *pPacket->pPresentModeCount = presModeCnt[pPacket->physicalDevice];
            savepPresentModes = pPacket->pPresentModes;
            pPacket->pPresentModes = VKTRACE_NEW_ARRAY(VkPresentModeKHR, *pPacket->pPresentModeCount);
        }
    }

    replayResult = m_vkFuncs.GetPhysicalDeviceSurfacePresentModesKHR(remappedphysicalDevice, remappedSurfaceKHR,
                                                                     pPacket->pPresentModeCount, pPacket->pPresentModes);

    if (!pPacket->pPresentModes) {
        // This was a query to determine size. Save the returned size so we can use that size next time
        // we're called with pPresentModes not null. This is to prevent a VK_INCOMPLETE error.
        presModeCnt[pPacket->physicalDevice] = *pPacket->pPresentModeCount;
    }

    if (savepPresentModes) {
        // Restore pPacket->pPresentModes. We do this because the replay will free the memory.
        // Note that we don't copy the queried data - it wouldn't fit, and it's not used by the replayer anyway.
        VKTRACE_DELETE(pPacket->pPresentModes);
        pPacket->pPresentModes = savepPresentModes;
    }

    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateSwapchainKHR(packet_vkCreateSwapchainKHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkSwapchainKHR local_pSwapchain;
    VkSwapchainKHR save_oldSwapchain, *pSC;
    VkSurfaceKHR save_surface;
    pSC = (VkSwapchainKHR *)&pPacket->pCreateInfo->oldSwapchain;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    save_oldSwapchain = pPacket->pCreateInfo->oldSwapchain;
    (*pSC) = m_objMapper.remap_swapchainkhrs(save_oldSwapchain);
    if ((*pSC) == VK_NULL_HANDLE && save_oldSwapchain != VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkSwapchainKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    save_surface = pPacket->pCreateInfo->surface;
    VkSurfaceKHR *pSurf = (VkSurfaceKHR *)&(pPacket->pCreateInfo->surface);
    *pSurf = m_objMapper.remap_surfacekhrs(*pSurf);
    if (*pSurf == VK_NULL_HANDLE && pPacket->pCreateInfo->surface != VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateSwapchainKHR() due to invalid remapped VkSurfaceKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    m_display->resize_window(pPacket->pCreateInfo->imageExtent.width, pPacket->pCreateInfo->imageExtent.height);

    // Convert queueFamilyIndices
    if (pPacket->pCreateInfo) {
        for (uint32_t i = 0; i < pPacket->pCreateInfo->queueFamilyIndexCount; i++) {
            uint32_t replayIdx;
            if (pPacket->pCreateInfo->pQueueFamilyIndices &&
                getQueueFamilyIdx(pPacket->device, remappeddevice, pPacket->pCreateInfo->pQueueFamilyIndices[i], &replayIdx)) {
                *((uint32_t *)&pPacket->pCreateInfo->pQueueFamilyIndices[i]) = replayIdx;
            } else {
                vktrace_LogError("vkSwapchainCreateInfoKHR, bad queueFamilyIndex");
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
    }

    // Get the list of VkFormats that are supported:
    VkPhysicalDevice remappedPhysicalDevice = replayPhysicalDevices[remappeddevice];
    uint32_t formatCount;
    VkResult U_ASSERT_ONLY res;
    // Note that pPacket->pCreateInfo->surface has been remapped above
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(remappedPhysicalDevice, pPacket->pCreateInfo->surface, &formatCount, NULL);
    assert(!res);
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    assert(surfFormats);
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(remappedPhysicalDevice, pPacket->pCreateInfo->surface, &formatCount, surfFormats);
    assert(!res);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (!(formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)) {
        bool found = false;
        for (uint32_t i = 0; i < formatCount; i++) {
            if (pPacket->pCreateInfo->imageFormat == surfFormats[i].format) {
                found = true;
                break;
            }
        }
        if (!found) {
            vktrace_LogWarning("Format %d is not supported for presentable images, using format %d",
                               pPacket->pCreateInfo->imageFormat, surfFormats[0].format);
            VkFormat *pFormat = (VkFormat *)&(pPacket->pCreateInfo->imageFormat);
            *pFormat = surfFormats[0].format;
        }
    }
    free(surfFormats);

    // If the present mode is not FIFO and the present mode requested is not supported by the
    // replay device, then change the present mode to FIFO
    if (pPacket->pCreateInfo->presentMode != VK_PRESENT_MODE_FIFO_KHR &&
        replayPhysicalDevices.find(remappeddevice) != replayPhysicalDevices.end()) {
        // Call GetPhysicalDeviceSurfacePresentModesKHR to get the list of supported present modes
        uint32_t presentModeCount;
        VkPresentModeKHR *pPresentModes;
        VkResult result;
        uint32_t i;
        result = m_vkFuncs.GetPhysicalDeviceSurfacePresentModesKHR(replayPhysicalDevices[remappeddevice],
                                                                   pPacket->pCreateInfo->surface, &presentModeCount, NULL);
        if (result == VK_SUCCESS) {
            pPresentModes = VKTRACE_NEW_ARRAY(VkPresentModeKHR, presentModeCount);
            result = m_vkFuncs.GetPhysicalDeviceSurfacePresentModesKHR(
                replayPhysicalDevices[remappeddevice], pPacket->pCreateInfo->surface, &presentModeCount, pPresentModes);
            if (result == VK_SUCCESS && presentModeCount) {
                for (i = 0; i < presentModeCount; i++) {
                    if (pPacket->pCreateInfo->presentMode == pPresentModes[i])
                        // Found matching present mode
                        break;
                }
                if (i == presentModeCount)
                    // Didn't find a matching present mode, so use FIFO instead.
                    *((VkPresentModeKHR *)(&pPacket->pCreateInfo->presentMode)) = VK_PRESENT_MODE_FIFO_KHR;
            }
            VKTRACE_DELETE(pPresentModes);
        }
    }

    replayResult = m_vkDeviceFuncs.CreateSwapchainKHR(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator, &local_pSwapchain);
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_swapchainkhrs_map(*(pPacket->pSwapchain), local_pSwapchain);
    }

    (*pSC) = save_oldSwapchain;
    *pSurf = save_surface;
    return replayResult;
}

void vkReplay::manually_replay_vkDestroySwapchainKHR(packet_vkDestroySwapchainKHR *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkDestroySwapchainKHR() due to invalid remapped VkDevice.");
        return;
    }

    VkSwapchainKHR remappedswapchain = m_objMapper.remap_swapchainkhrs(pPacket->swapchain);
    if (pPacket->swapchain != VK_NULL_HANDLE && remappedswapchain == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkDestroySwapchainKHR() due to invalid remapped VkSwapchainKHR.");
        return;
    }

    // Need to unmap images obtained with vkGetSwapchainImagesKHR
    while (!traceSwapchainToImages[pPacket->swapchain].empty()) {
        VkImage image = traceSwapchainToImages[pPacket->swapchain].back();
        m_objMapper.rm_from_images_map(image);
        traceSwapchainToImages[pPacket->swapchain].pop_back();
    }

    m_vkDeviceFuncs.DestroySwapchainKHR(remappeddevice, remappedswapchain, pPacket->pAllocator);
    m_objMapper.rm_from_swapchainkhrs_map(pPacket->swapchain);
}

VkResult vkReplay::manually_replay_vkGetSwapchainImagesKHR(packet_vkGetSwapchainImagesKHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetSwapchainImagesKHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkSwapchainKHR remappedswapchain;
    remappedswapchain = m_objMapper.remap_swapchainkhrs(pPacket->swapchain);
    if (remappedswapchain == VK_NULL_HANDLE && pPacket->swapchain != VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkGetSwapchainImagesKHR() due to invalid remapped VkSwapchainKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkImage packetImage[128] = {0};
    uint32_t numImages = 0;
    if (pPacket->pSwapchainImages != NULL) {
        // Need to store the images and then add to map after we get actual image handles back
        VkImage *pPacketImages = (VkImage *)pPacket->pSwapchainImages;
        numImages = *(pPacket->pSwapchainImageCount);
        for (uint32_t i = 0; i < numImages; i++) {
            packetImage[i] = pPacketImages[i];
            traceImageToDevice[packetImage[i]] = pPacket->device;
        }
    }

    replayResult = m_vkDeviceFuncs.GetSwapchainImagesKHR(remappeddevice, remappedswapchain, pPacket->pSwapchainImageCount,
                                                         pPacket->pSwapchainImages);
    if (replayResult == VK_SUCCESS) {
        if (numImages != 0) {
            VkImage *pReplayImages = (VkImage *)pPacket->pSwapchainImages;
            for (uint32_t i = 0; i < numImages; i++) {
                imageObj local_imageObj;
                local_imageObj.replayImage = pReplayImages[i];
                m_objMapper.add_to_images_map(packetImage[i], local_imageObj);
                replayImageToDevice[pReplayImages[i]] = remappeddevice;
                traceSwapchainToImages[pPacket->swapchain].push_back(packetImage[i]);
            }
        }
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkQueuePresentKHR(packet_vkQueuePresentKHR *pPacket) {
    VkResult replayResult = VK_SUCCESS;
    VkQueue remappedQueue = m_objMapper.remap_queues(pPacket->queue);
    if (remappedQueue == VK_NULL_HANDLE) {
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
    uint32_t remappedImageIndex = UINT32_MAX;

    if (pPacket->pPresentInfo->swapchainCount > 5) {
        pRemappedSwapchains = VKTRACE_NEW_ARRAY(VkSwapchainKHR, pPacket->pPresentInfo->swapchainCount);
    }

    if (pPacket->pPresentInfo->swapchainCount > 5 && pPacket->pPresentInfo->pResults != NULL) {
        pResults = VKTRACE_NEW_ARRAY(VkResult, pPacket->pPresentInfo->swapchainCount);
    }

    if (pPacket->pPresentInfo->waitSemaphoreCount > 5) {
        pRemappedWaitSems = VKTRACE_NEW_ARRAY(VkSemaphore, pPacket->pPresentInfo->waitSemaphoreCount);
    }

    if (pRemappedSwapchains == NULL || pRemappedWaitSems == NULL || pResults == NULL) {
        replayResult = VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    if (replayResult == VK_SUCCESS) {
        for (i = 0; i < pPacket->pPresentInfo->swapchainCount; i++) {
            pRemappedSwapchains[i] = m_objMapper.remap_swapchainkhrs(pPacket->pPresentInfo->pSwapchains[i]);
            if (pRemappedSwapchains[i] == VK_NULL_HANDLE) {
                vktrace_LogError("Skipping vkQueuePresentKHR() due to invalid remapped VkSwapchainKHR.");
                replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
                goto out;
            }
        }

        assert(pPacket->pPresentInfo->swapchainCount == 1 && "Multiple swapchain images not supported yet");

        if (pPacket->pPresentInfo->pImageIndices) {
            auto imageIndice = *pPacket->pPresentInfo->pImageIndices;
            remappedImageIndex = m_objMapper.remap_pImageIndex(imageIndice);
        }

        if (remappedImageIndex == UINT32_MAX) {
            vktrace_LogError("Skipping vkQueuePresentKHR() due to invalid remapped pImageIndices.");
            replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
            goto out;
        }

        present.sType = pPacket->pPresentInfo->sType;
        present.pNext = pPacket->pPresentInfo->pNext;
        present.swapchainCount = pPacket->pPresentInfo->swapchainCount;
        present.pSwapchains = pRemappedSwapchains;
        present.pImageIndices = &remappedImageIndex;
        present.waitSemaphoreCount = pPacket->pPresentInfo->waitSemaphoreCount;
        present.pWaitSemaphores = NULL;
        if (present.waitSemaphoreCount != 0) {
            present.pWaitSemaphores = pRemappedWaitSems;
            for (i = 0; i < pPacket->pPresentInfo->waitSemaphoreCount; i++) {
                (*(pRemappedWaitSems + i)) = m_objMapper.remap_semaphores((*(pPacket->pPresentInfo->pWaitSemaphores + i)));
                if (*(pRemappedWaitSems + i) == VK_NULL_HANDLE) {
                    vktrace_LogError("Skipping vkQueuePresentKHR() due to invalid remapped wait VkSemaphore.");
                    replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
                    goto out;
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

        replayResult = m_vkDeviceFuncs.QueuePresentKHR(remappedQueue, &present);

        m_frameNumber++;

        // Compare the results from the trace file with those just received from the replay.  Report any differences.
        if (present.pResults != NULL) {
            for (i = 0; i < pPacket->pPresentInfo->swapchainCount; i++) {
                if (present.pResults[i] != pPacket->pPresentInfo->pResults[i]) {
                    vktrace_LogError(
                        "Return value %s from API call (VkQueuePresentKHR) does not match return value from trace file %s for "
                        "swapchain %d.",
                        string_VkResult(present.pResults[i]), string_VkResult(pPacket->pPresentInfo->pResults[i]), i);
                }
            }
        }
    }

out:

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

VkResult vkReplay::manually_replay_vkCreateXcbSurfaceKHR(packet_vkCreateXcbSurfaceKHR *pPacket) {
    VkResult replayResult = VK_SUCCESS;
    VkSurfaceKHR local_pSurface = VK_NULL_HANDLE;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateXcbSurfaceKHR() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(PLATFORM_LINUX) && !defined(ANDROID)
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *)m_display->get_surface();
        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.connection = pSurf->connection;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXcbSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.dpy = pSurf->dpy;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXlibSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        VkIcdSurfaceWayland *pSurf = (VkIcdSurfaceWayland *)m_display->get_surface();
        VkWaylandSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.display = pSurf->display;
        createInfo.surface = pSurf->surface;
        replayResult = m_vkFuncs.CreateWaylandSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#elif defined(WIN32)
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *)m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.CreateWin32SurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#else
    vktrace_LogError("manually_replay_vkCreateXcbSurfaceKHR not implemented on this vkreplay platform");
    replayResult = VK_ERROR_FEATURE_NOT_PRESENT;
#endif

    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateXlibSurfaceKHR(packet_vkCreateXlibSurfaceKHR *pPacket) {
    VkResult replayResult = VK_SUCCESS;
    VkSurfaceKHR local_pSurface = VK_NULL_HANDLE;
    VkInstance remappedinstance = m_objMapper.remap_instances(pPacket->instance);

    if (pPacket->instance != VK_NULL_HANDLE && remappedinstance == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_ANDROID_KHR)
// TODO
#elif defined(PLATFORM_LINUX)
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.dpy = pSurf->dpy;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXlibSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *)m_display->get_surface();
        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.connection = pSurf->connection;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXcbSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        VkIcdSurfaceWayland *pSurf = (VkIcdSurfaceWayland *)m_display->get_surface();
        VkWaylandSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.display = pSurf->display;
        createInfo.surface = pSurf->surface;
        replayResult = m_vkFuncs.CreateWaylandSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if !defined(VK_USE_PLATFORM_XCB_KHR) && !defined(VK_USE_PLATFORM_WAYLAND_KHR)
#error manually_replay_vkCreateXlibSurfaceKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XCB_KHR or VK_USE_PLATFORM_WAYLAND_KHR or VK_USE_PLATFORM_ANDROID_KHR
#endif
#elif defined(WIN32)
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *)m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.CreateWin32SurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#else
    vktrace_LogError("manually_replay_vkCreateXlibSurfaceKHR not implemented on this playback platform");
    replayResult = VK_ERROR_FEATURE_NOT_PRESENT;
#endif
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateWaylandSurfaceKHR(packet_vkCreateWaylandSurfaceKHR *pPacket) {
    VkResult replayResult = VK_SUCCESS;
    VkSurfaceKHR local_pSurface = VK_NULL_HANDLE;
    VkInstance remappedinstance = m_objMapper.remap_instances(pPacket->instance);

    if (pPacket->instance != VK_NULL_HANDLE && remappedinstance == VK_NULL_HANDLE) {
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_ANDROID_KHR)
// TODO
#elif defined(PLATFORM_LINUX)
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        VkIcdSurfaceWayland *pSurf = (VkIcdSurfaceWayland *)m_display->get_surface();
        VkWaylandSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.display = pSurf->display;
        createInfo.surface = pSurf->surface;
        replayResult = m_vkFuncs.CreateWaylandSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *)m_display->get_surface();
        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.connection = pSurf->connection;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXcbSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.dpy = pSurf->dpy;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXlibSurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if !defined(VK_USE_PLATFORM_WAYLAND_KHR) && !defined(VK_USE_PLATFORM_XCB_KHR)
#error manually_replay_vkCreateWaylandSurfaceKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_WAYLAND_KHR or VK_USE_PLATFORM_XCB_KHR or VK_USE_PLATFORM_ANDROID_KHR
#endif
#elif defined(WIN32)
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *)m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.CreateWin32SurfaceKHR(remappedinstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#else
    vktrace_LogError("manually_replay_vkCreateWaylandSurfaceKHR not implemented on this playback platform");
    replayResult = VK_ERROR_FEATURE_NOT_PRESENT;
#endif
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateWin32SurfaceKHR(packet_vkCreateWin32SurfaceKHR *pPacket) {
    VkResult replayResult = VK_SUCCESS;
    VkSurfaceKHR local_pSurface = VK_NULL_HANDLE;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateWin32SurfaceKHR() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(WIN32)
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *)m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.CreateWin32SurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#elif defined(PLATFORM_LINUX) && !defined(ANDROID)
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *)m_display->get_surface();
        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.connection = pSurf->connection;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXcbSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.dpy = pSurf->dpy;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXlibSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        VkIcdSurfaceWayland *pSurf = (VkIcdSurfaceWayland *)m_display->get_surface();
        VkWaylandSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.display = pSurf->display;
        createInfo.surface = pSurf->surface;
        replayResult = m_vkFuncs.CreateWaylandSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#else
    vktrace_LogError("manually_replay_vkCreateWin32SurfaceKHR not implemented on this playback platform");
    replayResult = VK_ERROR_FEATURE_NOT_PRESENT;
#endif
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateAndroidSurfaceKHR(packet_vkCreateAndroidSurfaceKHR *pPacket) {
    VkResult replayResult = VK_SUCCESS;
    VkSurfaceKHR local_pSurface = VK_NULL_HANDLE;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateAndroidSurfaceKHR() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(WIN32)
    VkIcdSurfaceWin32 *pSurf = (VkIcdSurfaceWin32 *)m_display->get_surface();
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.hinstance = pSurf->hinstance;
    createInfo.hwnd = pSurf->hwnd;
    replayResult = m_vkFuncs.CreateWin32SurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#elif defined(PLATFORM_LINUX)
#if !defined(ANDROID)
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        VkIcdSurfaceXcb *pSurf = (VkIcdSurfaceXcb *)m_display->get_surface();
        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.connection = pSurf->connection;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXcbSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.dpy = pSurf->dpy;
        createInfo.window = pSurf->window;
        replayResult = m_vkFuncs.CreateXlibSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        VkIcdSurfaceWayland *pSurf = (VkIcdSurfaceWayland *)m_display->get_surface();
        VkWaylandSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = pPacket->pCreateInfo->pNext;
        createInfo.flags = pPacket->pCreateInfo->flags;
        createInfo.display = pSurf->display;
        createInfo.surface = pSurf->surface;
        replayResult = m_vkFuncs.CreateWaylandSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
    }
#endif
#else
    VkIcdSurfaceAndroid *pSurf = (VkIcdSurfaceAndroid *)m_display->get_surface();
    VkAndroidSurfaceCreateInfoKHR createInfo;
    createInfo.sType = pPacket->pCreateInfo->sType;
    createInfo.pNext = pPacket->pCreateInfo->pNext;
    createInfo.flags = pPacket->pCreateInfo->flags;
    createInfo.window = pSurf->window;
    replayResult = m_vkFuncs.CreateAndroidSurfaceKHR(remappedInstance, &createInfo, pPacket->pAllocator, &local_pSurface);
#endif  // ANDROID
#else
    vktrace_LogError("manually_replay_vkCreateAndroidSurfaceKHR not implemented on this playback platform");
    replayResult = VK_ERROR_FEATURE_NOT_PRESENT;
#endif
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_surfacekhrs_map(*(pPacket->pSurface), local_pSurface);
    }
    return replayResult;
}

VkResult vkReplay::manually_replay_vkCreateDebugReportCallbackEXT(packet_vkCreateDebugReportCallbackEXT *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDebugReportCallbackEXT local_msgCallback;
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkCreateDebugReportCallbackEXT() due to invalid remapped VkInstance.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    if (!g_fpDbgMsgCallback || !m_vkFuncs.CreateDebugReportCallbackEXT) {
        // just eat this call as we don't have local call back function defined
        return VK_SUCCESS;
    } else {
        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
        memset(&dbgCreateInfo, 0, sizeof(dbgCreateInfo));
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        dbgCreateInfo.flags = pPacket->pCreateInfo->flags;
        dbgCreateInfo.pfnCallback = g_fpDbgMsgCallback;
        dbgCreateInfo.pUserData = NULL;
        replayResult = m_vkFuncs.CreateDebugReportCallbackEXT(remappedInstance, &dbgCreateInfo, NULL, &local_msgCallback);
        if (replayResult == VK_SUCCESS) {
            m_objMapper.add_to_debugreportcallbackexts_map(*(pPacket->pCallback), local_msgCallback);
        }
    }
    return replayResult;
}

void vkReplay::manually_replay_vkDestroyDebugReportCallbackEXT(packet_vkDestroyDebugReportCallbackEXT *pPacket) {
    VkInstance remappedInstance = m_objMapper.remap_instances(pPacket->instance);
    if (remappedInstance == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkDestroyDebugReportCallbackEXT() due to invalid remapped VkInstance.");
        return;
    }

    VkDebugReportCallbackEXT remappedMsgCallback;
    remappedMsgCallback = m_objMapper.remap_debugreportcallbackexts(pPacket->callback);
    if (remappedMsgCallback == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkDestroyDebugReportCallbackEXT() due to invalid remapped VkDebugReportCallbackEXT.");
        return;
    }

    if (!g_fpDbgMsgCallback) {
        // just eat this call as we don't have local call back function defined
        return;
    } else {
        m_vkFuncs.DestroyDebugReportCallbackEXT(remappedInstance, remappedMsgCallback, NULL);
    }
}

VkResult vkReplay::manually_replay_vkAllocateCommandBuffers(packet_vkAllocateCommandBuffers *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappedDevice = m_objMapper.remap_devices(pPacket->device);
    if (remappedDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkAllocateCommandBuffers() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkCommandBuffer *local_pCommandBuffers = new VkCommandBuffer[pPacket->pAllocateInfo->commandBufferCount];
    VkCommandPool local_CommandPool;
    local_CommandPool = pPacket->pAllocateInfo->commandPool;
    ((VkCommandBufferAllocateInfo *)pPacket->pAllocateInfo)->commandPool =
        m_objMapper.remap_commandpools(pPacket->pAllocateInfo->commandPool);
    if (pPacket->pAllocateInfo->commandPool == VK_NULL_HANDLE) {
        vktrace_LogError("Skipping vkAllocateCommandBuffers() due to invalid remapped VkCommandPool.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    replayResult = m_vkDeviceFuncs.AllocateCommandBuffers(remappedDevice, pPacket->pAllocateInfo, local_pCommandBuffers);
    ((VkCommandBufferAllocateInfo *)pPacket->pAllocateInfo)->commandPool = local_CommandPool;

    if (replayResult == VK_SUCCESS) {
        for (uint32_t i = 0; i < pPacket->pAllocateInfo->commandBufferCount; i++) {
            m_objMapper.add_to_commandbuffers_map(pPacket->pCommandBuffers[i], local_pCommandBuffers[i]);
        }
    }
    delete[] local_pCommandBuffers;
    return replayResult;
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR(
    packet_vkGetPhysicalDeviceXcbPresentationSupportKHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in vkGetPhysicalDeviceXcbPresentationSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_FALSE;
    }

    // Convert the queue family index
    uint32_t replayIdx;
    if (getQueueFamilyIdx(pPacket->physicalDevice, remappedphysicalDevice, pPacket->queueFamilyIndex, &replayIdx)) {
        *((uint32_t *)&pPacket->queueFamilyIndex) = replayIdx;
    } else {
        vktrace_LogError("vkGetPhysicalDeviceXcbPresentationSupportKHR, bad queueFamilyIndex");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_ANDROID_KHR)
    // This is not defined for Android
    return VK_TRUE;
#elif defined(PLATFORM_LINUX)
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    // This is not defined for anndroid
    return VK_TRUE;
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        vkDisplayXcb *pDisp = (vkDisplayXcb *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                     pDisp->get_connection_handle(),
                                                                     pDisp->get_screen_handle()->root_visual));
    }
#endif
/*#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        return (m_vkFuncs.GetPhysicalDeviceXlibPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                             pSurf->dpy,
m_display->get_screen_handle()->root_visual));
    }
#endif*/
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        vkDisplayWayland *pDisp = (vkDisplayWayland *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceWaylandPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                         pDisp->get_display_handle()));
    }
#endif
#if !defined(VK_USE_PLATFORM_XCB_KHR) && !defined(VK_USE_PLATFORM_WAYLAND_KHR)
#error manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XCB_KHR, VK_USE_PLATFORM_WAYLAND_KHR, or VK_USE_PLATFORM_ANDROID_KHR
#endif
#elif defined(WIN32)
    return (m_vkFuncs.GetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceXcbPresentationSupportKHR not implemented on this playback platform");
#endif
    return VK_FALSE;
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR(
    packet_vkGetPhysicalDeviceXlibPresentationSupportKHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in vkGetPhysicalDeviceXlibPresentationSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_FALSE;
    }

    // Convert the queue family index
    uint32_t replayIdx;
    if (getQueueFamilyIdx(pPacket->physicalDevice, remappedphysicalDevice, pPacket->queueFamilyIndex, &replayIdx)) {
        *((uint32_t *)&pPacket->queueFamilyIndex) = replayIdx;
    } else {
        vktrace_LogError("vkGetPhysicalDeviceXlibPresentationSupportKHR, bad queueFamilyIndex");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_ANDROID_KHR)
    // This is not defined for Android
    return VK_TRUE;
#elif defined(PLATFORM_LINUX)
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        vkDisplayXcb *pDisp = (vkDisplayXcb *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                     pDisp->get_connection_handle(),
                                                                     pDisp->get_screen_handle()->root_visual));
    }
#endif
/*#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        return (m_vkFuncs.GetPhysicalDeviceXlibPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                             pSurf->dpy,
m_display->get_screen_handle()->root_visual));
    }
#endif*/
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        vkDisplayWayland *pDisp = (vkDisplayWayland *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceWaylandPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                         pDisp->get_display_handle()));
    }
#endif
#if !defined(VK_USE_PLATFORM_XCB_KHR) && !defined(VK_USE_PLATFORM_WAYLAND_KHR)
#error manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XCB_KHR, VK_USE_PLATFORM_WAYLAND_KHR or VK_USE_PLATFORM_ANDROID_KHR
#endif
#elif defined(WIN32)
    return (m_vkFuncs.GetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceXlibPresentationSupportKHR not implemented on this playback platform");
#endif
    return VK_FALSE;
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceWaylandPresentationSupportKHR(
    packet_vkGetPhysicalDeviceWaylandPresentationSupportKHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in vkGetPhysicalDeviceWaylandPresentationSupportKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_FALSE;
    }

    // Convert the queue family index
    uint32_t replayIdx;
    if (getQueueFamilyIdx(pPacket->physicalDevice, remappedphysicalDevice, pPacket->queueFamilyIndex, &replayIdx)) {
        *((uint32_t *)&pPacket->queueFamilyIndex) = replayIdx;
    } else {
        vktrace_LogError("vkGetPhysicalDeviceWaylandPresentationSupportKHR, bad queueFamilyIndex");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_ANDROID_KHR)
    // This is not defined for Android
    return VK_TRUE;
#elif defined(PLATFORM_LINUX)
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        vkDisplayXcb *pDisp = (vkDisplayXcb *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                     pDisp->get_connection_handle(),
                                                                     pDisp->get_screen_handle()->root_visual));
    }
#endif
/*#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        return (m_vkFuncs.GetPhysicalDeviceXlibPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                             pSurf->dpy,
m_display->get_screen_handle()->root_visual));
    }
#endif*/
#if VK_USE_PLATFORM_WAYLAND_KHR
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        vkDisplayWayland *pDisp = (vkDisplayWayland *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceWaylandPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                         pDisp->get_display_handle()));
    }
#endif
#if !defined(VK_USE_PLATFORM_XCB_KHR) && !defined(VK_USE_PLATFORM_WAYLAND_KHR)
#error manually_replay_vkGetPhysicalDeviceWaylandPresentationSupportKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XCB_KHR, VK_USE_PLATFORM_WAYLAND_KHR or VK_USE_PLATFORM_ANDROID_KHR
#endif
#elif defined(WIN32)
    return (m_vkFuncs.GetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceWaylandPresentationSupportKHR not implemented on this playback platform");
#endif
    return VK_FALSE;
}

VkBool32 vkReplay::manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR(
    packet_vkGetPhysicalDeviceWin32PresentationSupportKHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        return VK_FALSE;
    }

    // Convert the queue family index
    uint32_t replayIdx;
    if (getQueueFamilyIdx(pPacket->physicalDevice, remappedphysicalDevice, pPacket->queueFamilyIndex, &replayIdx)) {
        *((uint32_t *)&pPacket->queueFamilyIndex) = replayIdx;
    } else {
        vktrace_LogError("vkGetPhysicalDeviceWin32PresentationSupportKHR, bad queueFamilyIndex");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

#if defined(WIN32)
    return (m_vkFuncs.GetPhysicalDeviceWin32PresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex));
#elif defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_ANDROID_KHR)
    // This is not defined for Android
    return VK_TRUE;
#elif defined(PLATFORM_LINUX)
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if (m_displayServer == VK_DISPLAY_XCB) {
        vkDisplayXcb *pDisp = (vkDisplayXcb *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceXcbPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                     pDisp->get_connection_handle(),
                                                                     pDisp->get_screen_handle()->root_visual));
    }
#endif
/*#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (m_displayServer == VK_DISPLAY_XLIB) {
        VkIcdSurfaceXlib *pSurf = (VkIcdSurfaceXlib *)m_display->get_surface();
        return (m_vkFuncs.GetPhysicalDeviceXlibPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                         pSurf->dpy, m_display->get_screen_handle()->root_visual));
    }
#endif*/
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if (m_displayServer == VK_DISPLAY_WAYLAND) {
        vkDisplayWayland *pDisp = (vkDisplayWayland *)m_display;
        return (m_vkFuncs.GetPhysicalDeviceWaylandPresentationSupportKHR(remappedphysicalDevice, pPacket->queueFamilyIndex,
                                                                         pDisp->get_display_handle()));
    }
#endif
#if !defined(VK_USE_PLATFORM_XCB_KHR) && !defined(VK_USE_PLATFORM_WAYLAND_KHR)
#error manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR on PLATFORM_LINUX requires one of VK_USE_PLATFORM_XCB_KHR, VK_USE_PLATFORM_WAYLAND_KHR or VK_USE_PLATFORM_ANDROID_KHR
#endif
#else
    vktrace_LogError("manually_replay_vkGetPhysicalDeviceWin32PresentationSupportKHR not implemented on this playback platform");
#endif
    return VK_FALSE;
}

static std::unordered_map<VkDescriptorUpdateTemplateKHR, VkDescriptorUpdateTemplateCreateInfoKHR *>
    descriptorUpdateTemplateCreateInfo;

VkResult vkReplay::manually_replay_vkCreateDescriptorUpdateTemplate(packet_vkCreateDescriptorUpdateTemplate *pPacket) {
    return manually_replay_vkCreateDescriptorUpdateTemplateKHR((packet_vkCreateDescriptorUpdateTemplateKHR *)pPacket);
}

VkResult vkReplay::manually_replay_vkCreateDescriptorUpdateTemplateKHR(packet_vkCreateDescriptorUpdateTemplateKHR *pPacket) {
    VkResult replayResult;
    VkDescriptorUpdateTemplateKHR local_pDescriptorUpdateTemplate;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CreateDescriptorUpdateTemplateKHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    *((VkDescriptorUpdateTemplateCreateInfoKHR **)&pPacket->pCreateInfo->pDescriptorUpdateEntries) =
        (VkDescriptorUpdateTemplateCreateInfoKHR *)vktrace_trace_packet_interpret_buffer_pointer(
            pPacket->header, (intptr_t)pPacket->pCreateInfo->pDescriptorUpdateEntries);

    *((VkDescriptorSetLayout *)&pPacket->pCreateInfo->descriptorSetLayout) =
        m_objMapper.remap_descriptorsetlayouts(pPacket->pCreateInfo->descriptorSetLayout);

    replayResult = m_vkDeviceFuncs.CreateDescriptorUpdateTemplateKHR(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator,
                                                                     &local_pDescriptorUpdateTemplate);
    if (replayResult == VK_SUCCESS) {
        m_objMapper.add_to_descriptorupdatetemplates_map(*(pPacket->pDescriptorUpdateTemplate), local_pDescriptorUpdateTemplate);
        descriptorUpdateTemplateCreateInfo[local_pDescriptorUpdateTemplate] =
            (VkDescriptorUpdateTemplateCreateInfoKHR *)malloc(sizeof(VkDescriptorUpdateTemplateCreateInfoKHR));
        memcpy(descriptorUpdateTemplateCreateInfo[local_pDescriptorUpdateTemplate], pPacket->pCreateInfo,
               sizeof(VkDescriptorUpdateTemplateCreateInfoKHR));
        descriptorUpdateTemplateCreateInfo[local_pDescriptorUpdateTemplate]->pDescriptorUpdateEntries =
            (VkDescriptorUpdateTemplateEntryKHR *)malloc(sizeof(VkDescriptorUpdateTemplateEntryKHR) *
                                                         pPacket->pCreateInfo->descriptorUpdateEntryCount);
        memcpy((void *)descriptorUpdateTemplateCreateInfo[local_pDescriptorUpdateTemplate]->pDescriptorUpdateEntries,
               pPacket->pCreateInfo->pDescriptorUpdateEntries,
               sizeof(VkDescriptorUpdateTemplateEntryKHR) * pPacket->pCreateInfo->descriptorUpdateEntryCount);
    }
    return replayResult;
}

void vkReplay::manually_replay_vkDestroyDescriptorUpdateTemplate(packet_vkDestroyDescriptorUpdateTemplate *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in DestroyDescriptorUpdateTemplate() due to invalid remapped VkDevice.");
        return;
    }
    VkDescriptorUpdateTemplate remappedDescriptorUpdateTemplate =
        m_objMapper.remap_descriptorupdatetemplates(pPacket->descriptorUpdateTemplate);
    if (pPacket->descriptorUpdateTemplate != VK_NULL_HANDLE && remappedDescriptorUpdateTemplate == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in DestroyDescriptorUpdateTemplate() due to invalid remapped VkDescriptorUpdateTemplate.");
        return;
    }
    m_vkDeviceFuncs.DestroyDescriptorUpdateTemplate(remappeddevice, remappedDescriptorUpdateTemplate, pPacket->pAllocator);
    m_objMapper.rm_from_descriptorupdatetemplates_map(pPacket->descriptorUpdateTemplate);

    if (descriptorUpdateTemplateCreateInfo.find(remappedDescriptorUpdateTemplate) != descriptorUpdateTemplateCreateInfo.end()) {
        if (descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]) {
            if (descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries)
                free((void *)descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries);
            free(descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]);
        }
        descriptorUpdateTemplateCreateInfo.erase(remappedDescriptorUpdateTemplate);
    }
}

void vkReplay::manually_replay_vkDestroyDescriptorUpdateTemplateKHR(packet_vkDestroyDescriptorUpdateTemplateKHR *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in DestroyDescriptorUpdateTemplateKHR() due to invalid remapped VkDevice.");
        return;
    }
    VkDescriptorUpdateTemplateKHR remappedDescriptorUpdateTemplate =
        m_objMapper.remap_descriptorupdatetemplates(pPacket->descriptorUpdateTemplate);
    if (pPacket->descriptorUpdateTemplate != VK_NULL_HANDLE && remappedDescriptorUpdateTemplate == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in DestroyDescriptorUpdateTemplateKHR() due to invalid remapped VkDescriptorUpdateTemplateKHR.");
        return;
    }
    m_vkDeviceFuncs.DestroyDescriptorUpdateTemplateKHR(remappeddevice, remappedDescriptorUpdateTemplate, pPacket->pAllocator);
    m_objMapper.rm_from_descriptorupdatetemplates_map(pPacket->descriptorUpdateTemplate);

    if (descriptorUpdateTemplateCreateInfo.find(remappedDescriptorUpdateTemplate) != descriptorUpdateTemplateCreateInfo.end()) {
        if (descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]) {
            if (descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries)
                free((void *)descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries);
            free(descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]);
        }
        descriptorUpdateTemplateCreateInfo.erase(remappedDescriptorUpdateTemplate);
    }
}

void vkReplay::remapHandlesInDescriptorSetWithTemplateData(VkDescriptorUpdateTemplateKHR remappedDescriptorUpdateTemplate,
                                                           char *pData) {
    for (uint32_t i = 0; i < descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->descriptorUpdateEntryCount;
         i++) {
        for (uint32_t j = 0;
             j < descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries[i].descriptorCount;
             j++) {
            size_t offset =
                descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries[i].offset +
                j * descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries[i].stride;
            char *update_entry = pData + offset;
            switch (
                descriptorUpdateTemplateCreateInfo[remappedDescriptorUpdateTemplate]->pDescriptorUpdateEntries[i].descriptorType) {
                case VK_DESCRIPTOR_TYPE_SAMPLER: {
                    auto image_entry = reinterpret_cast<VkDescriptorImageInfo *>(update_entry);
                    image_entry->sampler = m_objMapper.remap_samplers(image_entry->sampler);
                    break;
                }
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {
                    auto image_entry = reinterpret_cast<VkDescriptorImageInfo *>(update_entry);
                    image_entry->sampler = m_objMapper.remap_samplers(image_entry->sampler);
                    image_entry->imageView = m_objMapper.remap_imageviews(image_entry->imageView);
                    break;
                }
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                    auto image_entry = reinterpret_cast<VkDescriptorImageInfo *>(update_entry);
                    image_entry->imageView = m_objMapper.remap_imageviews(image_entry->imageView);
                    break;
                }
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                    auto buffer_entry = reinterpret_cast<VkDescriptorBufferInfo *>(update_entry);
                    buffer_entry->buffer = m_objMapper.remap_buffers(buffer_entry->buffer);
                    break;
                }
                case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                    auto buffer_view_handle = reinterpret_cast<VkBufferView *>(update_entry);
                    *buffer_view_handle = m_objMapper.remap_bufferviews(*buffer_view_handle);
                    break;
                }
                default:
                    assert(0);
            }
        }
    }
}

void vkReplay::manually_replay_vkUpdateDescriptorSetWithTemplate(packet_vkUpdateDescriptorSetWithTemplate *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in UpdateDescriptorSetWithTemplate() due to invalid remapped VkDevice.");
        return;
    }

    VkDescriptorSet remappedDescriptorSet = m_objMapper.remap_descriptorsets(pPacket->descriptorSet);
    if (pPacket->descriptorSet != VK_NULL_HANDLE && remappedDescriptorSet == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in UpdateDescriptorSetWithTemplate() due to invalid remapped VkDescriptorSet.");
        return;
    }

    VkDescriptorUpdateTemplate remappedDescriptorUpdateTemplate =
        m_objMapper.remap_descriptorupdatetemplates(pPacket->descriptorUpdateTemplate);
    if (pPacket->descriptorUpdateTemplate != VK_NULL_HANDLE && remappedDescriptorUpdateTemplate == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in UpdateDescriptorSetWithTemplate() due to invalid remapped VkDescriptorUpdateTemplateKHR.");
        return;
    }

    // Map handles inside of pData
    remapHandlesInDescriptorSetWithTemplateData(remappedDescriptorUpdateTemplate, (char *)pPacket->pData);

    m_vkDeviceFuncs.UpdateDescriptorSetWithTemplate(remappeddevice, remappedDescriptorSet, remappedDescriptorUpdateTemplate,
                                                    pPacket->pData);
}

void vkReplay::manually_replay_vkUpdateDescriptorSetWithTemplateKHR(packet_vkUpdateDescriptorSetWithTemplateKHR *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in UpdateDescriptorSetWithTemplateKHR() due to invalid remapped VkDevice.");
        return;
    }

    VkDescriptorSet remappedDescriptorSet = m_objMapper.remap_descriptorsets(pPacket->descriptorSet);
    if (pPacket->descriptorSet != VK_NULL_HANDLE && remappedDescriptorSet == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in UpdateDescriptorSetWithTemplateKHR() due to invalid remapped VkDescriptorSet.");
        return;
    }

    VkDescriptorUpdateTemplateKHR remappedDescriptorUpdateTemplate =
        m_objMapper.remap_descriptorupdatetemplates(pPacket->descriptorUpdateTemplate);
    if (pPacket->descriptorUpdateTemplate != VK_NULL_HANDLE && remappedDescriptorUpdateTemplate == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in UpdateDescriptorSetWithTemplateKHR() due to invalid remapped VkDescriptorUpdateTemplateKHR.");
        return;
    }

    // Map handles inside of pData
    remapHandlesInDescriptorSetWithTemplateData(remappedDescriptorUpdateTemplate, (char *)pPacket->pData);

    m_vkDeviceFuncs.UpdateDescriptorSetWithTemplateKHR(remappeddevice, remappedDescriptorSet, remappedDescriptorUpdateTemplate,
                                                       pPacket->pData);
}

void vkReplay::manually_replay_vkCmdPushDescriptorSetKHR(packet_vkCmdPushDescriptorSetKHR *pPacket) {
    VkCommandBuffer remappedcommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (pPacket->commandBuffer != VK_NULL_HANDLE && remappedcommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CmdPushDescriptorSetKHR() due to invalid remapped VkCommandBuffer.");
        return;
    }

    // allocate a new array for the writes and clear the memory, we'll update the contents further down
    VkWriteDescriptorSet *pRemappedWrites = NULL;
    if (pPacket->pDescriptorWrites != NULL && pPacket->descriptorWriteCount > 0) {
        pRemappedWrites = VKTRACE_NEW_ARRAY(VkWriteDescriptorSet, pPacket->descriptorWriteCount);
        memset(pRemappedWrites, 0, pPacket->descriptorWriteCount * sizeof(VkWriteDescriptorSet));
    }

    // No need to remap pipelineBindPoint

    VkPipelineLayout remappedlayout = m_objMapper.remap_pipelinelayouts(pPacket->layout);
    if (pPacket->layout != VK_NULL_HANDLE && remappedlayout == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CmdPushDescriptorSetKHR() due to invalid remapped VkPipelineLayout.");
        return;
    }

    bool errorBadRemap = false;

    if (pPacket->pDescriptorWrites != NULL) {
        for (uint32_t i = 0; i < pPacket->descriptorWriteCount && !errorBadRemap; i++) {
            pRemappedWrites[i] = pPacket->pDescriptorWrites[i];
            pRemappedWrites[i].dstSet = 0;  // Ignored
            pRemappedWrites[i].pBufferInfo = nullptr;
            pRemappedWrites[i].pImageInfo = nullptr;
            pRemappedWrites[i].pTexelBufferView = nullptr;

            switch (pPacket->pDescriptorWrites[i].descriptorType) {
                case VK_DESCRIPTOR_TYPE_SAMPLER:
                    pRemappedWrites[i].pImageInfo =
                        VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                    memcpy((void *)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo,
                           pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
                    for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                        if (pPacket->pDescriptorWrites[i].pImageInfo[j].sampler != VK_NULL_HANDLE) {
                            const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].sampler =
                                m_objMapper.remap_samplers(pPacket->pDescriptorWrites[i].pImageInfo[j].sampler);
                            if (pRemappedWrites[i].pImageInfo[j].sampler == VK_NULL_HANDLE) {
                                vktrace_LogError("Skipping vkCmdPushDescriptorSet() due to invalid remapped VkSampler.");
                                errorBadRemap = true;
                                break;
                            }
                        }
                    }
                    break;
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
                    pRemappedWrites[i].pImageInfo =
                        VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                    memcpy((void *)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo,
                           pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
                    for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                        if (pPacket->pDescriptorWrites[i].pImageInfo[j].imageView != VK_NULL_HANDLE) {
                            const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].imageView =
                                m_objMapper.remap_imageviews(pPacket->pDescriptorWrites[i].pImageInfo[j].imageView);
                            if (pRemappedWrites[i].pImageInfo[j].imageView == VK_NULL_HANDLE) {
                                vktrace_LogError("Skipping vkCmdPushDescriptorSet() due to invalid remapped VkImageView.");
                                errorBadRemap = true;
                                break;
                            }
                        }
                    }
                    break;
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                    pRemappedWrites[i].pImageInfo =
                        VKTRACE_NEW_ARRAY(VkDescriptorImageInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                    memcpy((void *)pRemappedWrites[i].pImageInfo, pPacket->pDescriptorWrites[i].pImageInfo,
                           pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorImageInfo));
                    for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                        if (pPacket->pDescriptorWrites[i].pImageInfo[j].sampler != VK_NULL_HANDLE) {
                            const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].sampler =
                                m_objMapper.remap_samplers(pPacket->pDescriptorWrites[i].pImageInfo[j].sampler);
                            if (pRemappedWrites[i].pImageInfo[j].sampler == VK_NULL_HANDLE) {
                                vktrace_LogError("Skipping vkCmdPushDescriptorSet() due to invalid remapped VkSampler.");
                                errorBadRemap = true;
                                break;
                            }
                        }
                        if (pPacket->pDescriptorWrites[i].pImageInfo[j].imageView != VK_NULL_HANDLE) {
                            const_cast<VkDescriptorImageInfo *>(pRemappedWrites[i].pImageInfo)[j].imageView =
                                m_objMapper.remap_imageviews(pPacket->pDescriptorWrites[i].pImageInfo[j].imageView);
                            if (pRemappedWrites[i].pImageInfo[j].imageView == VK_NULL_HANDLE) {
                                vktrace_LogError("Skipping vkCmdPushDescriptorSet() due to invalid remapped VkImageView.");
                                errorBadRemap = true;
                                break;
                            }
                        }
                    }
                    break;
                case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                    pRemappedWrites[i].pTexelBufferView =
                        VKTRACE_NEW_ARRAY(VkBufferView, pPacket->pDescriptorWrites[i].descriptorCount);
                    memcpy((void *)pRemappedWrites[i].pTexelBufferView, pPacket->pDescriptorWrites[i].pTexelBufferView,
                           pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkBufferView));
                    for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                        if (pPacket->pDescriptorWrites[i].pTexelBufferView[j] != VK_NULL_HANDLE) {
                            const_cast<VkBufferView *>(pRemappedWrites[i].pTexelBufferView)[j] =
                                m_objMapper.remap_bufferviews(pPacket->pDescriptorWrites[i].pTexelBufferView[j]);
                            if (pRemappedWrites[i].pTexelBufferView[j] == VK_NULL_HANDLE) {
                                vktrace_LogError("Skipping vkCmdPushDescriptorSet() due to invalid remapped VkBufferView.");
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
                    pRemappedWrites[i].pBufferInfo =
                        VKTRACE_NEW_ARRAY(VkDescriptorBufferInfo, pPacket->pDescriptorWrites[i].descriptorCount);
                    memcpy((void *)pRemappedWrites[i].pBufferInfo, pPacket->pDescriptorWrites[i].pBufferInfo,
                           pPacket->pDescriptorWrites[i].descriptorCount * sizeof(VkDescriptorBufferInfo));
                    for (uint32_t j = 0; j < pPacket->pDescriptorWrites[i].descriptorCount; j++) {
                        if (pPacket->pDescriptorWrites[i].pBufferInfo[j].buffer != VK_NULL_HANDLE) {
                            const_cast<VkDescriptorBufferInfo *>(pRemappedWrites[i].pBufferInfo)[j].buffer =
                                m_objMapper.remap_buffers(pPacket->pDescriptorWrites[i].pBufferInfo[j].buffer);
                            if (pRemappedWrites[i].pBufferInfo[j].buffer == VK_NULL_HANDLE) {
                                vktrace_LogError("Skipping vkCmdPushDescriptorSet() due to invalid remapped VkBufferView.");
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
    }

    if (!errorBadRemap) {
        // If an error occurred, don't call the real function, but skip ahead so that memory is cleaned up!
        m_vkDeviceFuncs.CmdPushDescriptorSetKHR(remappedcommandBuffer, pPacket->pipelineBindPoint, remappedlayout, pPacket->set,
                                                pPacket->descriptorWriteCount, pRemappedWrites);
    }

    if (pPacket->pDescriptorWrites != NULL) {
        for (uint32_t d = 0; d < pPacket->descriptorWriteCount; d++) {
            switch (pPacket->pDescriptorWrites[d].descriptorType) {
                case VK_DESCRIPTOR_TYPE_SAMPLER:
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                    if (pRemappedWrites[d].pImageInfo != NULL) {
                        VKTRACE_DELETE((void *)pRemappedWrites[d].pImageInfo);
                        pRemappedWrites[d].pImageInfo = NULL;
                    }
                    break;
                case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                    if (pRemappedWrites[d].pTexelBufferView != NULL) {
                        VKTRACE_DELETE((void *)pRemappedWrites[d].pTexelBufferView);
                        pRemappedWrites[d].pTexelBufferView = NULL;
                    }
                    break;
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
                    if (pRemappedWrites[d].pBufferInfo != NULL) {
                        VKTRACE_DELETE((void *)pRemappedWrites[d].pBufferInfo);
                        pRemappedWrites[d].pBufferInfo = NULL;
                    }
                default:
                    break;
            }
        }
        VKTRACE_DELETE(pRemappedWrites);
    }
}

void vkReplay::manually_replay_vkCmdPushDescriptorSetWithTemplateKHR(packet_vkCmdPushDescriptorSetWithTemplateKHR *pPacket) {
    VkCommandBuffer remappedcommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (pPacket->commandBuffer != VK_NULL_HANDLE && remappedcommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CmdPushDescriptorSetWithTemplateKHR() due to invalid remapped VkCommandBuffer.");
        return;
    }

    VkDescriptorUpdateTemplateKHR remappedDescriptorUpdateTemplate =
        m_objMapper.remap_descriptorupdatetemplates(pPacket->descriptorUpdateTemplate);
    if (pPacket->descriptorUpdateTemplate != VK_NULL_HANDLE && remappedDescriptorUpdateTemplate == VK_NULL_HANDLE) {
        vktrace_LogError(
            "Error detected in CmdPushDescriptorSetWithTemplateKHR() due to invalid remapped VkDescriptorUpdateTemplateKHR.");
        return;
    }

    VkPipelineLayout remappedlayout = m_objMapper.remap_pipelinelayouts(pPacket->layout);
    if (pPacket->layout != VK_NULL_HANDLE && remappedlayout == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CmdPushDescriptorSetWithTemplateKHR() due to invalid remapped VkPipelineLayout.");
        return;
    }

    // Map handles inside of pData
    remapHandlesInDescriptorSetWithTemplateData(remappedDescriptorUpdateTemplate, (char *)pPacket->pData);

    m_vkDeviceFuncs.CmdPushDescriptorSetWithTemplateKHR(remappedcommandBuffer, remappedDescriptorUpdateTemplate, remappedlayout,
                                                        pPacket->set, pPacket->pData);
}

VkResult vkReplay::manually_replay_vkRegisterDeviceEventEXT(packet_vkRegisterDeviceEventEXT *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in RegisterDeviceEventEXT() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }
    // No need to remap pDeviceEventInfo
    // No need to remap pAllocator
    VkFence fence;
    auto result = m_vkDeviceFuncs.RegisterDeviceEventEXT(remappeddevice, pPacket->pDeviceEventInfo, pPacket->pAllocator, &fence);
    if (result == VK_SUCCESS) {
        m_objMapper.add_to_fences_map(*pPacket->pFence, fence);
    }
    return result;
}

VkResult vkReplay::manually_replay_vkRegisterDisplayEventEXT(packet_vkRegisterDisplayEventEXT *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in RegisterDisplayEventEXT() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }
    VkDisplayKHR remappeddisplay = m_objMapper.remap_displaykhrs(pPacket->display);
    if (pPacket->display != VK_NULL_HANDLE && remappeddisplay == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in RegisterDisplayEventEXT() due to invalid remapped VkDisplayKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // No need to remap pDisplayEventInfo
    // No need to remap pAllocator
    VkFence fence;
    auto result = m_vkDeviceFuncs.RegisterDisplayEventEXT(remappeddevice, remappeddisplay, pPacket->pDisplayEventInfo,
                                                          pPacket->pAllocator, &fence);
    if (result == VK_SUCCESS) {
        m_objMapper.add_to_fences_map(*pPacket->pFence, fence);
    }
    return result;
}

VkResult vkReplay::manually_replay_vkCreateObjectTableNVX(packet_vkCreateObjectTableNVX *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    VkObjectTableNVX local_pObjectTable;
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CreateObjectTableNVX() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // No need to remap pCreateINfo
    // No need to remap pAllocator
    // No need to remap pObjecTable

    // Remap fields in pCreateInfo
    *((VkObjectEntryTypeNVX **)&pPacket->pCreateInfo->pObjectEntryTypes) =
        (VkObjectEntryTypeNVX *)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfo->pObjectEntryTypes);
    *((int32_t **)&pPacket->pCreateInfo->pObjectEntryCounts) =
        (int32_t *)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfo->pObjectEntryCounts);
    *((VkObjectEntryUsageFlagsNVX **)&pPacket->pCreateInfo->pObjectEntryUsageFlags) =
        (VkObjectEntryUsageFlagsNVX *)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfo->pObjectEntryUsageFlags);

    auto result =
        m_vkDeviceFuncs.CreateObjectTableNVX(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator, &local_pObjectTable);

    if (result == VK_SUCCESS) {
        m_objMapper.add_to_objecttablenvxs_map(*(pPacket->pObjectTable), local_pObjectTable);
    }
    return result;
}

void vkReplay::manually_replay_vkCmdProcessCommandsNVX(packet_vkCmdProcessCommandsNVX *pPacket) {
    VkCommandBuffer remappedcommandBuffer = m_objMapper.remap_commandbuffers(pPacket->commandBuffer);
    if (pPacket->commandBuffer != VK_NULL_HANDLE && remappedcommandBuffer == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CmdProcessCommandsNVX() due to invalid remapped VkCommandBuffer.");
    }

    // No need to remap pProcessCommandsInfo

    // Remap fields in pProcessCommandsInfo
    *((VkIndirectCommandsTokenNVX **)&pPacket->pProcessCommandsInfo->pIndirectCommandsTokens) =
        (VkIndirectCommandsTokenNVX *)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pProcessCommandsInfo->pIndirectCommandsTokens);

    m_vkDeviceFuncs.CmdProcessCommandsNVX(remappedcommandBuffer, pPacket->pProcessCommandsInfo);
}


VkResult vkReplay::manually_replay_vkCreateIndirectCommandsLayoutNVX(packet_vkCreateIndirectCommandsLayoutNVX *pPacket) {
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    VkIndirectCommandsLayoutNVX local_pIndirectCommandsLayout;

    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in CreateObjectTableNVX() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // No need to remap pCreateInfo
    // No need to remap pAllocator

    // Remap fields in pCreateInfo
    *((VkIndirectCommandsLayoutTokenNVX **)&pPacket->pCreateInfo->pTokens) =
        (VkIndirectCommandsLayoutTokenNVX *)vktrace_trace_packet_interpret_buffer_pointer(pPacket->header, (intptr_t)pPacket->pCreateInfo->pTokens);

    auto result = m_vkDeviceFuncs.CreateIndirectCommandsLayoutNVX(remappeddevice, pPacket->pCreateInfo, pPacket->pAllocator,
                                                                  &local_pIndirectCommandsLayout);

    if (result == VK_SUCCESS) {
        m_objMapper.add_to_indirectcommandslayoutnvxs_map(*(pPacket->pIndirectCommandsLayout), local_pIndirectCommandsLayout);
    }

    return result;
}

VkResult vkReplay::manually_replay_vkBindBufferMemory2KHR(packet_vkBindBufferMemory2KHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindBufferMemory2KHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    for (size_t i = 0; i < pPacket->bindInfoCount; i++) {
        VkBuffer traceBuffer = pPacket->pBindInfos[i].buffer;
        VkBuffer remappedBuffer = m_objMapper.remap_buffers(pPacket->pBindInfos[i].buffer);
        if (traceBuffer != VK_NULL_HANDLE && remappedBuffer == VK_NULL_HANDLE) {
            vktrace_LogError("Error detected in BindBufferMemory2KHR() due to invalid remapped VkBuffer.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        *((VkBuffer *)&pPacket->pBindInfos[i].buffer) = remappedBuffer;
        *((VkDeviceMemory *)&pPacket->pBindInfos[i].memory) = m_objMapper.remap_devicememorys(pPacket->pBindInfos[i].memory);
        if (g_pReplaySettings->compatibilityMode && m_pFileHeader->portability_table_valid && !platformMatch()) {
            uint64_t memOffsetTemp;
            if (replayGetBufferMemoryRequirements.find(remappedBuffer) == replayGetBufferMemoryRequirements.end()) {
                // vkBindBufferMemory2KHR is being called with a buffer for which vkGetBufferMemoryRequirements
                // was not called. This might be violation of the spec on the part of the app, but seems to
                // be done in many apps.  Call vkGetBufferMemoryRequirements for this buffer and add result to
                // replayGetBufferMemoryRequirements map.
                VkMemoryRequirements mem_reqs;
                m_vkDeviceFuncs.GetBufferMemoryRequirements(remappeddevice, remappedBuffer, &mem_reqs);
                replayGetBufferMemoryRequirements[remappedBuffer] = mem_reqs;
            }

            assert(replayGetBufferMemoryRequirements[remappedBuffer].alignment);
            memOffsetTemp = pPacket->pBindInfos[i].memoryOffset + replayGetBufferMemoryRequirements[remappedBuffer].alignment - 1;
            memOffsetTemp = memOffsetTemp / replayGetBufferMemoryRequirements[remappedBuffer].alignment;
            memOffsetTemp = memOffsetTemp * replayGetBufferMemoryRequirements[remappedBuffer].alignment;
            *((VkDeviceSize *)&pPacket->pBindInfos[i].memoryOffset) = memOffsetTemp;
        }
    }
    replayResult = m_vkDeviceFuncs.BindBufferMemory2KHR(remappeddevice, pPacket->bindInfoCount, pPacket->pBindInfos);
    return replayResult;
}

VkResult vkReplay::manually_replay_vkBindImageMemory2KHR(packet_vkBindImageMemory2KHR *pPacket) {
    VkResult replayResult = VK_ERROR_VALIDATION_FAILED_EXT;
    VkDevice remappeddevice = m_objMapper.remap_devices(pPacket->device);
    if (pPacket->device != VK_NULL_HANDLE && remappeddevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in BindImageMemory2KHR() due to invalid remapped VkDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    for (size_t i = 0; i < pPacket->bindInfoCount; i++) {
        VkImage traceImage = pPacket->pBindInfos[i].image;
        VkImage remappedImage = m_objMapper.remap_images(pPacket->pBindInfos[i].image);
        if (traceImage != VK_NULL_HANDLE && remappedImage == VK_NULL_HANDLE) {
            vktrace_LogError("Error detected in BindImageMemory2KHR() due to invalid remapped VkImage.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        *((VkImage *)&pPacket->pBindInfos[i].image) = remappedImage;
        *((VkDeviceMemory *)&pPacket->pBindInfos[i].memory) = m_objMapper.remap_devicememorys(pPacket->pBindInfos[i].memory);
        if (g_pReplaySettings->compatibilityMode && m_pFileHeader->portability_table_valid && !platformMatch()) {
            uint64_t memOffsetTemp;
            if (replayGetImageMemoryRequirements.find(remappedImage) == replayGetImageMemoryRequirements.end()) {
                // vkBindImageMemory2KHR is being called with an image for which vkGetImageMemoryRequirements
                // was not called. This might be violation of the spec on the part of the app, but seems to
                // be done in many apps.  Call vkGetImageMemoryRequirements for this image and add result to
                // replayGetImageMemoryRequirements map.
                VkMemoryRequirements mem_reqs;
                m_vkDeviceFuncs.GetImageMemoryRequirements(remappeddevice, remappedImage, &mem_reqs);
                replayGetImageMemoryRequirements[remappedImage] = mem_reqs;
            }

            assert(replayGetImageMemoryRequirements[remappedImage].alignment);
            memOffsetTemp = pPacket->pBindInfos[i].memoryOffset + replayGetImageMemoryRequirements[remappedImage].alignment - 1;
            memOffsetTemp = memOffsetTemp / replayGetImageMemoryRequirements[remappedImage].alignment;
            memOffsetTemp = memOffsetTemp * replayGetImageMemoryRequirements[remappedImage].alignment;
            *((VkDeviceSize *)&pPacket->pBindInfos[i].memoryOffset) = memOffsetTemp;
        }
    }
    replayResult = m_vkDeviceFuncs.BindImageMemory2KHR(remappeddevice, pPacket->bindInfoCount, pPacket->pBindInfos);
    return replayResult;
}

VkResult vkReplay::manually_replay_vkGetDisplayPlaneSupportedDisplaysKHR(packet_vkGetDisplayPlaneSupportedDisplaysKHR *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetDisplayPlaneSupportedDisplaysKHR() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    // No need to remap planeIndex
    // No need to remap pDisplayCount

    // Get remapped displays
    VkDisplayKHR *remapped_displays = VKTRACE_NEW_ARRAY(VkDisplayKHR, *pPacket->pDisplayCount);
    for (uint32_t i = 0; i < *pPacket->pDisplayCount; ++i) {
        remapped_displays[i] = m_objMapper.remap_displaykhrs(*(pPacket->pDisplays + i));
    }

    if (pPacket->pDisplays != VK_NULL_HANDLE && remapped_displays == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in GetDisplayPlaneSupportedDisplaysKHR() due to invalid remapped VkDisplayKHR.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkDisplayKHR *displays = pPacket->pDisplays;

    auto result = m_vkFuncs.GetDisplayPlaneSupportedDisplaysKHR(remappedphysicalDevice, pPacket->planeIndex, pPacket->pDisplayCount,
                                                                remapped_displays);

    if (memcmp(displays, pPacket->pDisplays, sizeof(VkDisplayKHR) * (*pPacket->pDisplayCount)) != 0) {
        vktrace_LogError("Display Plane supported displays differ. Displays may not match as expected.");
    }

    return result;
}

VkResult vkReplay::manually_replay_vkEnumerateDeviceExtensionProperties(packet_vkEnumerateDeviceExtensionProperties *pPacket) {
    VkPhysicalDevice remappedphysicalDevice = m_objMapper.remap_physicaldevices(pPacket->physicalDevice);
    if (pPacket->physicalDevice != VK_NULL_HANDLE && remappedphysicalDevice == VK_NULL_HANDLE) {
        vktrace_LogError("Error detected in EnumerateDeviceExtensionProperties() due to invalid remapped VkPhysicalDevice.");
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    uint32_t propertyCount = *pPacket->pPropertyCount;
    auto pProperties = pPacket->pProperties;
    // Get mapped pPropertyCount and alloc new array if querying for pProperties
    if (pProperties != nullptr) {
        if (replayDeviceExtensionPropertyCount.find(pPacket->physicalDevice) == replayDeviceExtensionPropertyCount.end()) {
            vktrace_LogError("Error detected in EnumerateDeviceExtensionProperties() due to invalid remapped pPropertyCount.");
            return VK_ERROR_VALIDATION_FAILED_EXT;
        }
        propertyCount = replayDeviceExtensionPropertyCount[pPacket->physicalDevice];
        pProperties = VKTRACE_NEW_ARRAY(VkExtensionProperties, propertyCount);
    }

    auto result =
        m_vkFuncs.EnumerateDeviceExtensionProperties(remappedphysicalDevice, pPacket->pLayerName, &propertyCount, pProperties);

    // Map physical device to property count
    if (result == VK_SUCCESS && pPacket->pProperties == nullptr) {
        replayDeviceExtensionPropertyCount[pPacket->physicalDevice] = propertyCount;
    }

    // Clean up properties array. For portability, we will want to compare this to what is in the packet.
    if (pProperties != nullptr) {
        VKTRACE_DELETE(pProperties);
    }

    return result;
}
