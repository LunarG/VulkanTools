/**************************************************************************
 *
 * Copyright 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
 * Author: Jon Ashburn <jon@lunarg.com>
 **************************************************************************/
#pragma once

#include "vktrace_multiplatform.h"

extern "C" {
#include "vktrace_platform.h"
}

#if defined(PLATFORM_LINUX) || defined(XCB_NVIDIA)
#if defined(ANDROID)
#include <android_native_app_glue.h>
#else
#if defined(VK_USE_PLATFORM_XCB_KHR)
#include <xcb/xcb.h>
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
// TODO
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#include <wayland-client.h>
#endif
#endif
#elif defined(WIN32)
#endif

/* classes to abstract the display and initialization of rendering API for presenting
 * framebuffers for display into a window on the screen.
 * Uses Bridge design pattern.
 */
namespace vktrace_replay {

class ReplayDisplayImp {
   public:
    ReplayDisplayImp() : m_initedVK(false) {}
    virtual ~ReplayDisplayImp() {}
    virtual int init(const unsigned int gpu_idx) = 0;
    virtual int create_window(const unsigned int width, const unsigned int height) = 0;
    virtual void resize_window(const unsigned int width, const unsigned int height) = 0;
    virtual void process_event() = 0;
    virtual bool get_pause_status() = 0;
    virtual void set_pause_status(bool pause) = 0;
    virtual bool get_quit_status() = 0;
    virtual void set_quit_status(bool quit) = 0;
    virtual VkSurfaceKHR get_surface() = 0;
    virtual void set_window_handle(void* pHandle) = 0;

    bool m_initedVK;
};

class ReplayDisplay {
   public:
    ReplayDisplay() : m_imp(NULL), m_width(0), m_height(0), m_gpu(0) {}

    ReplayDisplay(const unsigned int width, const unsigned int height, const unsigned int gpu)
        : m_imp(NULL), m_width(width), m_height(height), m_gpu(gpu) {}

    ReplayDisplay(const unsigned int width, const unsigned int height) : m_imp(NULL), m_width(width), m_height(height), m_gpu(-1) {}

    virtual ~ReplayDisplay() {}

    void set_implementation(ReplayDisplayImp& disp) { m_imp = &disp; }
    void set_implementation(ReplayDisplayImp* disp) { m_imp = disp; }
    int init() {
        if (m_imp)
            return m_imp->init(m_gpu);
        else
            return -1;
    }

    void set_window_handle(void* pHandle) {
        if (m_imp) {
            m_imp->set_window_handle(pHandle);
        }
    }

    void process_event() {
        if (m_imp) m_imp->process_event();
    }
    ReplayDisplayImp* get_imp() { return m_imp; }
    unsigned int get_gpu() { return m_gpu; }
    unsigned int get_width() { return m_width; }
    unsigned int get_height() { return m_height; }
    bool get_pause_status() {
        if (m_imp) {
            return m_imp->get_pause_status();
        }
        return false;
    }
    void set_pause_status(bool pause) {
        if (m_imp) m_imp->set_pause_status(pause);
    }
    bool get_quit_status() {
        if (m_imp) {
            return m_imp->get_quit_status();
        }
        return false;
    }
    void set_quit_status(bool quit) {
        if (m_imp) m_imp->set_quit_status(quit);
    }

   private:
    ReplayDisplayImp* m_imp;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_gpu;
};

}  // namespace vktrace_replay
