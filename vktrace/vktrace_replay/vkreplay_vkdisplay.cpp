/*
 *
 * Copyright (C) 2015-2017 Valve Corporation
 * Copyright (C) 2015-2017 LunarG, Inc.
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
 * Author: Joey Bzdek <joey@lunarg.com>
 */

#include "vkreplay_vkdisplay.h"
#include "vkreplay_settings.h"

#if defined(PLATFORM_LINUX) && defined(ANDROID)
#include <jni.h>
#endif

#define APP_NAME "vkreplay_vk"
#define IDI_ICON 101

int GetDisplayImplementation(const char *displayServer, vktrace_replay::ReplayDisplayImp **ppDisp) {
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_BSD)) && !defined(ANDROID)
    // On linux, the option -ds will choose a display server
    if (strcasecmp(displayServer, "xcb") == 0) {
        // Attempt to load libvkdisplay_xcb and constructor
        auto xcb_handle = dlopen("libvkdisplay_xcb.so", RTLD_NOW);
        if (dlerror()) {
            vktrace_LogError("Unable to load xcb library.");
            return -1;
        }
        auto CreateVkDisplayXcb = reinterpret_cast<vkDisplayXcb *(*)()>(dlsym(xcb_handle, "CreateVkDisplayXcb"));
        *ppDisp = CreateVkDisplayXcb();
    } else if (strcasecmp(displayServer, "wayland") == 0) {
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
        // Attempt to load libvkdisplay_wayland and constructor
        auto wayland_handle = dlopen("libvkdisplay_wayland.so", RTLD_NOW);
        if (dlerror()) {
            vktrace_LogError("Unable to load wayland library.");
            return -1;
        }
        auto CreateVkDisplayWayland = reinterpret_cast<vkDisplayWayland *(*)()>(dlsym(wayland_handle, "CreateVkDisplayWayland"));
        *ppDisp = CreateVkDisplayWayland();
#else
        vktrace_LogError("vktrace not built with wayland support.");
        return -1;
#endif
    } else {
        vktrace_LogError("Invalid display server. Valid options are: xcb, wayland");
        return -1;
    }
#elif defined(PLATFORM_LINUX) && defined(ANDROID)
// Will be received from android_main
#elif defined(WIN32)
    *ppDisp = new vkDisplayWin32();
#endif
    return 0;
}

#if defined(PLATFORM_LINUX) && defined(ANDROID)
#include <jni.h>

vkDisplayAndroid::vkDisplayAndroid(struct android_app *app) : m_windowWidth(0), m_windowHeight(0) {
    memset(&m_surface, 0, sizeof(VkIcdSurfaceAndroid));
    m_window = app->window;
    m_surface.window = app->window;
    m_android_app = app;
    m_android_app->userData = this;
}

vkDisplayAndroid::~vkDisplayAndroid() {}

int vkDisplayAndroid::init(const unsigned int gpu_idx) {
    // m_gpuIdx = gpu_idx;
    set_pause_status(false);
    set_quit_status(false);
    return 0;
}

int vkDisplayAndroid::create_window(const unsigned int width, const unsigned int height) { return 0; }

void vkDisplayAndroid::resize_window(const unsigned int width, const unsigned int height) {
    if (width != m_windowWidth || height != m_windowHeight) {
        m_windowWidth = width;
        m_windowHeight = height;
        // For Android, we adjust the screen orientation based on requested width and height.
        int32_t pixel_width = ANativeWindow_getWidth(m_window);
        int32_t pixel_height = ANativeWindow_getHeight(m_window);

        // We don't change the current orientation if width == height or if the requested orientation matches the current
        // orientation.
        if ((width != height) && ((width < height) != (pixel_width < pixel_height))) {
            JavaVM *jni_vm = nullptr;
            jobject jni_activity = nullptr;
            JNIEnv *env = nullptr;

            if ((m_android_app != nullptr) && (m_android_app->activity != nullptr)) {
                jni_vm = m_android_app->activity->vm;
                jni_activity = m_android_app->activity->clazz;
            }

            if ((jni_vm != nullptr) && (jni_activity != 0) && (jni_vm->AttachCurrentThread(&env, nullptr) == JNI_OK)) {
                jclass object_class = env->GetObjectClass(jni_activity);
                jmethodID set_orientation = env->GetMethodID(object_class, "setRequestedOrientation", "(I)V");

                if (width > height) {
                    const int SCREEN_ORIENTATION_LANDSCAPE = 0;
                    env->CallVoidMethod(jni_activity, set_orientation, SCREEN_ORIENTATION_LANDSCAPE);
                } else {
                    const int SCREEN_ORIENTATION_PORTRAIT = 1;
                    env->CallVoidMethod(jni_activity, set_orientation, SCREEN_ORIENTATION_PORTRAIT);
                }

                jni_vm->DetachCurrentThread();
            }
        }

        ANativeWindow_setBuffersGeometry(m_window, width, height, ANativeWindow_getFormat(m_window));
    }
}

void vkDisplayAndroid::process_event() {
    int events;
    struct android_poll_source *source;
    if (ALooper_pollAll(0, NULL, &events, (void **)&source) >= 0) {
        if (source) {
            source->process(m_android_app, source);
        }

        if (m_android_app->destroyRequested != 0) {
            this->set_quit_status(true);
        }
    }
}

void vkDisplayAndroid::set_window_handle(void *pHandle) { m_window = *((ANativeWindow **)pHandle); }

#endif  // defined(PLATFORM_LINUX) && defined(ANDROID)

#if defined(WIN32)

LRESULT WINAPI WindowProcVk(HWND window, unsigned int msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_KEYUP: {
            auto *const display = (vkDisplayWin32 *)GetWindowLongPtr(window, GWLP_USERDATA);
            assert(display != nullptr);
            switch (wp) {
                case VK_SPACE:
                    display->set_pause_status(!display->get_pause_status());
                    break;
                case VK_ESCAPE:
                    DestroyWindow(window);
                    PostQuitMessage(0);
                    break;
                default:
                    break;
            }
            return S_OK;
        }
        case WM_NCCREATE:
            // Changes made with SetWindowLongPtr will not take effect until SetWindowPos is called.
            SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT *)lp)->lpCreateParams);
            SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
            return DefWindowProc(window, msg, wp, lp);
        case WM_CLOSE:
            DestroyWindow(window);
        // fall-thru
        case WM_DESTROY:
            PostQuitMessage(0);
            return S_OK;
        default:
            return DefWindowProc(window, msg, wp, lp);
    }
}

vkDisplayWin32::vkDisplayWin32() : m_windowWidth(0), m_windowHeight(0) {
    memset(&m_surface, 0, sizeof(VkIcdSurfaceWin32));
    m_windowHandle = NULL;
    m_connection = NULL;
}

vkDisplayWin32::~vkDisplayWin32() {}

int vkDisplayWin32::init(const unsigned int gpu_idx) {
    // m_gpuIdx = gpu_idx;
    set_pause_status(false);
    set_quit_status(false);
    return 0;
}

int vkDisplayWin32::create_window(const unsigned int width, const unsigned int height) {
    // Register Window class
    WNDCLASSEX wcex = {};
    m_connection = GetModuleHandle(0);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcVk;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_connection;
    wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = APP_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
    if (!RegisterClassEx(&wcex)) {
        vktrace_LogError("Failed to register windows class");
        return -1;
    }

    // create the window
    RECT wr = {0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    m_windowHandle = CreateWindow(APP_NAME, APP_NAME, WS_OVERLAPPEDWINDOW, 0, 0, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL,
                                  wcex.hInstance, this);

    if (m_windowHandle) {
        m_windowWidth = width;
        m_windowHeight = height;
    } else {
        vktrace_LogError("Failed to create window");
        return -1;
    }

    // Get and save screen size
    RECT sr;
    if (!GetWindowRect(GetDesktopWindow(), &sr)) {
        vktrace_LogError("Failed to get size of screen");
        return -1;
    }
    m_screenWidth = sr.right;
    m_screenHeight = sr.bottom;

    // TODO : Not sure of best place to put this, but I have all the info I need here so just setting it all here for now
    m_surface.base.platform = VK_ICD_WSI_PLATFORM_WIN32;
    m_surface.hinstance = wcex.hInstance;
    m_surface.hwnd = m_windowHandle;

    return 0;
}

void vkDisplayWin32::resize_window(const unsigned int width, const unsigned int height) {
    if (width != m_windowWidth || height != m_windowHeight) {
        m_windowWidth = width;
        m_windowHeight = height;
        if (width >= m_screenWidth || height >= m_screenHeight) {
            // Make window full screen w/o borders
            BOOL rval;
            rval = (NULL != SetWindowLongPtr(m_windowHandle, GWL_STYLE, WS_VISIBLE | WS_POPUP));
            rval &= SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
            if (!rval) {
                vktrace_LogError("Failed to set window size to %d %d", width, height);
            }
        } else {
            RECT wr = {0, 0, (LONG)width, (LONG)height};
            AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
            SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE);
        }

        // Make sure window is visible.
        ShowWindow(m_windowHandle, SW_SHOWDEFAULT);
    }
}

void vkDisplayWin32::process_event() {
    MSG msg = {};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            exit(0);  // Prevents crashing due to vulkan cmds being on different thread than WindowProcVk
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void vkDisplayWin32::set_window_handle(void *pHandle) { m_windowHandle = *((HWND *)pHandle); }

#endif  // defined(WIN32)
