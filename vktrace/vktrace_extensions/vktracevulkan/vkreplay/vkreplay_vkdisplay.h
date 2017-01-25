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
 */

#pragma once

#include "vkreplay_vkreplay.h"
#include "vk_icd.h"

class vkDisplay: public vktrace_replay::ReplayDisplayImp {
friend class vkReplay;
public:
    vkDisplay();
    ~vkDisplay();
    int init(const unsigned int gpu_idx);
    int set_window(vktrace_window_handle hWindow, unsigned int width, unsigned int height);
    int create_window(const unsigned int width, const unsigned int height);
    void resize_window(const unsigned int width, const unsigned int height);
    void process_event();
    bool get_pause_status() { return m_pause; }
    void set_pause_status(bool pause) { m_pause = pause; }
    bool get_quit_status() { return m_quit; }
    void set_quit_status(bool quit) { m_quit = quit; }
    VkSurfaceKHR get_surface() { return (VkSurfaceKHR) &m_surface; };
    // VK_DEVICE get_device() { return m_dev[m_gpuIdx];}
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
    ANativeWindow* get_window_handle() { return m_window; }
#else
    xcb_window_t get_window_handle() { return m_XcbWindow; }
    xcb_connection_t* get_connection_handle() { return m_pXcbConnection; }
    xcb_screen_t* get_screen_handle() { return m_pXcbScreen; }
#endif
#elif defined(WIN32)
    HWND get_window_handle() { return m_windowHandle; }
    HINSTANCE get_connection_handle() { return m_connection; }
#endif
private:
    VkResult init_vk(const unsigned int gpu_idx);
    bool m_initedVK;
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
    VkIcdSurfaceAndroid m_surface;
    ANativeWindow* m_window;
#else
    VkIcdSurfaceXcb m_surface;
    xcb_connection_t *m_pXcbConnection;
    xcb_screen_t *m_pXcbScreen;
    xcb_window_t m_XcbWindow;
    xcb_intern_atom_reply_t *atom_wm_delete_window;
    //VkPlatformHandleXcbKHR m_XcbPlatformHandle;
#endif
#elif defined(WIN32)
    VkIcdSurfaceWin32 m_surface;
    HWND m_windowHandle;
    HINSTANCE m_connection;
#endif
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    unsigned int m_frameNumber;
    std::vector<VkExtent2D> imageExtents;
    std::vector<VkImage> imageHandles;
    std::vector<VkDeviceMemory> imageMemory;
    std::vector<VkDevice> imageDevice;
#if 0
    VK_DEVICE m_dev[VK_MAX_PHYSICAL_GPUS];
    uint32_t m_gpuCount;
    unsigned int m_gpuIdx;
    VK_PHYSICAL_GPU m_gpus[VK_MAX_PHYSICAL_GPUS];
    VK_PHYSICAL_GPU_PROPERTIES m_gpuProps[VK_MAX_PHYSICAL_GPUS];
#endif
    std::vector<char *>m_extensions;
    bool m_pause = false;
    bool m_quit = false;
};
