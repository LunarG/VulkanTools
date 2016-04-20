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

extern "C"{
#include "vktrace_platform.h"
}

#if defined(PLATFORM_LINUX) || defined(XCB_NVIDIA)
#include <xcb/xcb.h>
typedef xcb_window_t vktrace_window_handle;
#elif defined(WIN32)
typedef HWND vktrace_window_handle;
#endif

/* classes to abstract the display and initialization of rendering API for presenting
 * framebuffers for display into a window on the screen or else fullscreen.
 * Uses Bridge design pattern.
 */
namespace vktrace_replay {

class DisplayImp {
public:
    virtual ~DisplayImp() {}
    virtual int init(const unsigned int gpu_idx) = 0;
    virtual int set_window(vktrace_window_handle hWindow, unsigned int width, unsigned int height) = 0;
    virtual int create_window(const unsigned int width, const unsigned int height) = 0;
    virtual void process_event() = 0;
};

class Display {
public:
    Display()
        : m_imp(NULL),
        m_width(0),
        m_height(0),
        m_gpu(0),
        m_fullscreen(false),
        m_hWindow(0)
    {

    }

    Display(const unsigned int width, const unsigned int height, const unsigned int gpu, const bool fullscreen) :
        m_imp(NULL),
        m_width(width),
        m_height(height),
        m_gpu(gpu),
        m_fullscreen(fullscreen),
        m_hWindow(0)
    {
    }

    Display(vktrace_window_handle hWindow, unsigned int width, unsigned int height) :
        m_imp(NULL),
        m_width(width),
        m_height(height),
        m_gpu(0),
        m_fullscreen(false),
        m_hWindow(hWindow)
    {
    }

    virtual ~Display()
    {
    }

    void set_implementation(DisplayImp & disp)
    {
        m_imp = & disp;
    }
    void set_implementation(DisplayImp * disp)
    {
        m_imp = disp;
    }
    int init()
    {
        if (m_imp)
            return m_imp->init(m_gpu);
        else
            return -1;
    }
    void process_event()
    {
        if(m_imp)
            m_imp->process_event();
    }
    unsigned int get_gpu()
    {
        return m_gpu;
    }
    unsigned int get_width()
    {
        return m_width;
    }
    unsigned int get_height()
    {
        return m_height;
    }

    vktrace_window_handle get_window_handle()
    {
        return m_hWindow;
    }

private:
    DisplayImp *m_imp;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_gpu;
    bool m_fullscreen;
    vktrace_window_handle m_hWindow;
};

}   // namespace vktrace_replay
