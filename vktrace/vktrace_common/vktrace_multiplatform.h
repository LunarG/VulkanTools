/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
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
 * Author: David Pinedo<david@lunarg.com>
 **************************************************************************/
#pragma once

#include "vktrace_trace_packet_identifiers.h"
#include "vktrace_filelike.h"
#include "vktrace_memory.h"
#include "vktrace_process.h"
#include <stdbool.h>
#include "vulkan/vk_icd.h"

#if defined(__cplusplus)
extern "C" {
#endif

// Define types needed for cross-platform vkreplay.
// Unfortunately, some of these are duplicated from Vulkan
// and platform-specific header files. Haven't figured out how
// to avoid this.
#if !defined(VK_USE_PLATFORM_XCB_KHR)
typedef VkFlags VkXcbSurfaceCreateFlagsKHR;
typedef struct xcb_connection_t xcb_connection_t;
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_visualid_t;
typedef struct VkXcbSurfaceCreateInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkXcbSurfaceCreateFlagsKHR flags;
    xcb_connection_t* connection;
    xcb_window_t window;
} VkXcbSurfaceCreateInfoKHR;
typedef VkResult(VKAPI_PTR* PFN_vkCreateXcbSurfaceKHR)(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo,
                                                       const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32(VKAPI_PTR* PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)(VkPhysicalDevice physicalDevice,
                                                                              uint32_t queueFamilyIndex,
                                                                              xcb_connection_t* connection,
                                                                              xcb_visualid_t visual_id);
typedef struct {
    VkIcdSurfaceBase base;
    xcb_connection_t* connection;
    xcb_window_t window;
} VkIcdSurfaceXcb;
#endif

#if !defined(VK_USE_PLATFORM_XLIB_KHR)
typedef VkFlags VkXlibSurfaceCreateFlagsKHR;
struct _XDisplay;
typedef struct _XDisplay Display;
typedef uint32_t CARD32;
typedef CARD32 XID;
typedef XID Window;
typedef CARD32 VisualID;
typedef XID RROutput;
typedef struct VkXlibSurfaceCreateInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkXlibSurfaceCreateFlagsKHR flags;
    Display* dpy;
    Window window;
} VkXlibSurfaceCreateInfoKHR;
typedef VkResult(VKAPI_PTR* PFN_vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo,
                                                        const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32(VKAPI_PTR* PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)(VkPhysicalDevice physicalDevice,
                                                                               uint32_t queueFamilyIndex, Display* dpy,
                                                                               VisualID visualID);
typedef struct {
    VkIcdSurfaceBase base;
    Display* dpy;
    Window window;
} VkIcdSurfaceXlib;
#endif

#if !defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
typedef XID RROutput;
#endif

#if !defined(VK_USE_PLATFORM_WAYLAND_KHR)
typedef VkFlags VkWaylandSurfaceCreateFlagsKHR;
typedef struct VkWaylandSurfaceCreateInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkWaylandSurfaceCreateFlagsKHR flags;
    struct wl_display* display;
    struct wl_surface* surface;
} VkWaylandSurfaceCreateInfoKHR;
typedef VkResult(VKAPI_PTR* PFN_vkCreateWaylandSurfaceKHR)(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo,
                                                           const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32(VKAPI_PTR* PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)(VkPhysicalDevice physicalDevice,
                                                                                  uint32_t queueFamilyIndex,
                                                                                  struct wl_display* display);
typedef struct {
    VkIcdSurfaceBase base;
    struct wl_display* display;
    struct wl_surface* surface;
} VkIcdSurfaceWayland;
#endif

#if !defined(VK_USE_PLATFORM_ANDROID_KHR)
typedef VkFlags VkAndroidSurfaceCreateFlagsKHR;
typedef uint32_t* ANativeWindow;
typedef struct VkAndroidSurfaceCreateInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkAndroidSurfaceCreateFlagsKHR flags;
    ANativeWindow* window;
} VkAndroidSurfaceCreateInfoKHR;
typedef VkResult(VKAPI_PTR* PFN_vkCreateAndroidSurfaceKHR)(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo,
                                                           const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef struct VkIcdSurfaceAndroid {
    ANativeWindow* window;
} VkIcdSurfaceAndroid;
typedef struct AHardwareBuffer {
    int notYetImplemented;
} AHardwareBuffer;
typedef struct VkAndroidHardwareBufferPropertiesANDROID {
    int notYetImplemented;
} VkAndroidHardwareBufferPropertiesANDROID;
typedef struct VkMemoryGetAndroidHardwareBufferInfoANDROID {
    int notYetImplemented;
} VkMemoryGetAndroidHardwareBufferInfoANDROID;

typedef VkResult(VKAPI_PTR* PFN_vkGetAndroidHardwareBufferPropertiesANDROID)(VkDevice device, const struct AHardwareBuffer* buffer,
                                                                             VkAndroidHardwareBufferPropertiesANDROID* pProperties);
typedef VkResult(VKAPI_PTR* PFN_vkGetMemoryAndroidHardwareBufferANDROID)(VkDevice device,
                                                                         const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo,
                                                                         struct AHardwareBuffer** pBuffer);
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
typedef void* HINSTANCE;
typedef void* HWND;
typedef void* HANDLE;
typedef const uint16_t* LPCWSTR;
typedef VkFlags VkWin32SurfaceCreateFlagsKHR;
typedef struct VkWin32SurfaceCreateInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkWin32SurfaceCreateFlagsKHR flags;
    HINSTANCE hinstance;
    HWND window;
} VkWin32SurfaceCreateInfoKHR;

typedef struct VkMemoryWin32HandlePropertiesKHR {
    VkStructureType sType;
    void* pNext;
    uint32_t memoryTypeBits;
} VkMemoryWin32HandlePropertiesKHR;

typedef struct VkMemoryGetWin32HandleInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkDeviceMemory memory;
    VkExternalMemoryHandleTypeFlagBitsKHR handleType;
} VkMemoryGetWin32HandleInfoKHR;

typedef struct VkImportSemaphoreWin32HandleInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkSemaphore semaphore;
    VkSemaphoreImportFlagsKHR flags;
    VkExternalSemaphoreHandleTypeFlagBitsKHR handleType;
    HANDLE handle;
    LPCWSTR name;
} VkImportSemaphoreWin32HandleInfoKHR;

typedef struct VkSemaphoreGetWin32HandleInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkSemaphore semaphore;
    VkExternalSemaphoreHandleTypeFlagBitsKHR handleType;
} VkSemaphoreGetWin32HandleInfoKHR;

typedef struct VkImportFenceWin32HandleInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkFence fence;
    VkFenceImportFlagsKHR flags;
    VkExternalFenceHandleTypeFlagBitsKHR handleType;
    HANDLE handle;
    LPCWSTR name;
} VkImportFenceWin32HandleInfoKHR;

typedef struct VkFenceGetWin32HandleInfoKHR {
    VkStructureType sType;
    const void* pNext;
    VkFence fence;
    VkExternalFenceHandleTypeFlagBitsKHR handleType;
} VkFenceGetWin32HandleInfoKHR;

typedef VkResult(VKAPI_PTR* PFN_vkCreateWin32SurfaceKHR)(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo,
                                                         const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32(VKAPI_PTR* PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)(VkPhysicalDevice physicalDevice,
                                                                                uint32_t queueFamilyIndex);
typedef VkResult(VKAPI_PTR* PFN_vkGetMemoryWin32HandleKHR)(VkDevice device,
                                                           const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo,
                                                           HANDLE* pHandle);
typedef VkResult(VKAPI_PTR* PFN_vkGetMemoryWin32HandlePropertiesKHR)(
    VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle,
    VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);
typedef VkResult(VKAPI_PTR* PFN_vkImportSemaphoreWin32HandleKHR)(
    VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
typedef VkResult(VKAPI_PTR* PFN_vkGetSemaphoreWin32HandleKHR)(VkDevice device,
                                                              const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo,
                                                              HANDLE* pHandle);
typedef VkResult(VKAPI_PTR* PFN_vkImportFenceWin32HandleKHR)(VkDevice device,
                                                             const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
typedef VkResult(VKAPI_PTR* PFN_vkGetFenceWin32HandleKHR)(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo,
                                                          HANDLE* pHandle);
typedef VkResult(VKAPI_PTR* PFN_vkGetMemoryWin32HandleNV)(VkDevice device, VkDeviceMemory memory,
                                                          VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);

#endif

#if defined(__cplusplus)
}
#endif
