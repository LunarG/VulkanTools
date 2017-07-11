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

#include "vkreplay_vkreplay.h"
#include "vk_icd.h"
#define APP_NAME "vkreplay_vk"
#define IDI_ICON 101

vkDisplay::vkDisplay() : m_initedVK(false), m_windowWidth(0), m_windowHeight(0), m_frameNumber(0) {
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
    memset(&m_surface, 0, sizeof(VkIcdSurfaceAndroid));
    m_window = 0;
#else
    memset(&m_surface, 0, sizeof(VkIcdSurfaceXcb));
    m_pXcbConnection = NULL;
    m_pXcbScreen = NULL;
    m_XcbWindow = 0;
#endif
#elif defined(WIN32)
    memset(&m_surface, 0, sizeof(VkIcdSurfaceWin32));
    m_windowHandle = NULL;
    m_connection = NULL;
#endif
}

vkDisplay::~vkDisplay() {
#if defined(PLATFORM_LINUX) && !defined(ANDROID)
    if (m_XcbWindow != 0) {
        xcb_destroy_window(m_pXcbConnection, m_XcbWindow);
    }
    if (m_pXcbConnection != NULL) {
        xcb_disconnect(m_pXcbConnection);
    }
#endif
}

VkResult vkDisplay::init_vk(unsigned int gpu_idx) {
#if 0
    VkApplicationInfo appInfo = {};
    appInfo.pApplicationName = APP_NAME;
    appInfo.pEngineName = "";
    appInfo.apiVersion = VK_API_VERSION;
    VkResult res = vkInitAndEnumerateGpus(&appInfo, NULL, VK_MAX_PHYSICAL_GPUS, &m_gpuCount, m_gpus);
    if ( res == VK_SUCCESS ) {
        // retrieve the GPU information for all GPUs
        for( uint32_t gpu = 0; gpu < m_gpuCount; gpu++)
        {
            size_t gpuInfoSize = sizeof(m_gpuProps[0]);

            // get the GPU physical properties:
            res = vkGetGpuInfo( m_gpus[gpu], VK_INFO_TYPE_PHYSICAL_GPU_PROPERTIES, &gpuInfoSize, &m_gpuProps[gpu]);
            if (res != VK_SUCCESS)
                vktrace_LogWarning("Failed to retrieve properties for gpu[%d] result %d", gpu, res);
        }
        res = VK_SUCCESS;
    } else if ((gpu_idx + 1) > m_gpuCount) {
        vktrace_LogError("vkInitAndEnumerate number of gpus does not include requested index: num %d, requested %d", m_gpuCount, gpu_idx);
        return -1;
    } else {
        vktrace_LogError("vkInitAndEnumerate failed");
        return res;
    }
    // TODO add multi-gpu support always use gpu[gpu_idx] for now
    // get all extensions supported by this device gpu[gpu_idx]
    // first check if extensions are available and save a list of them
    bool foundWSIExt = false;
    for( int ext = 0; ext < sizeof( extensions ) / sizeof( extensions[0] ); ext++)
    {
        res = vkGetExtensionSupport( m_gpus[gpu_idx], extensions[ext] );
        if (res == VK_SUCCESS) {
            m_extensions.push_back((char *) extensions[ext]);
            if (!strcmp(extensions[ext], "VK_WSI_WINDOWS"))
                foundWSIExt = true;
        }
    }
    if (!foundWSIExt) {
        vktrace_LogError("VK_WSI_WINDOWS extension not supported by gpu[%d]", gpu_idx);
        return VK_ERROR_INCOMPATIBLE_DEVICE;
    }
    // TODO generalize this: use one universal queue for now
    VkDeviceQueueCreateInfo dqci = {};
    dqci.queueCount = 1;
    dqci.queueType = VK_QUEUE_UNIVERSAL;
    std::vector<float> queue_priorities (dqci.queueCount, 0.0);
    dqci.pQueuePriorities = queue_priorities.data();
    // create the device enabling validation level 4
    const char * const * extensionNames = &m_extensions[0];
    VkDeviceCreateInfo info = {};
    info.queueCreateInfoCount = 1;
    info.pQueueCreateInfos = &dqci;
    info.enabledExtensionCount = static_cast <uint32_t> (m_extensions.size());
    info.ppEnabledExtensionNames = extensionNames;
    info.flags = VK_DEVICE_CREATE_VALIDATION;
    info.maxValidationLevel = VK_VALIDATION_LEVEL_4;
    bool32_t vkTrue = VK_TRUE;
    res = vkDbgSetGlobalOption( VK_DBG_OPTION_BREAK_ON_ERROR, sizeof( vkTrue ), &vkTrue );
    if (res != VK_SUCCESS)
        vktrace_LogWarning("Could not set debug option break on error");
    res = vkCreateDevice( m_gpus[0], &info, &m_dev[gpu_idx]);
    return res;
#else
    return VK_ERROR_INITIALIZATION_FAILED;
#endif
}

int vkDisplay::init(const unsigned int gpu_idx) {
// m_gpuIdx = gpu_idx;
#if 0
    VkResult result = init_vk(gpu_idx);
    if (result != VK_SUCCESS) {
        vktrace_LogError("could not init vulkan library");
        return -1;
    } else {
        m_initedVK = true;
    }
#endif
#if defined(PLATFORM_LINUX) && !defined(ANDROID)
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;
    m_pXcbConnection = xcb_connect(NULL, &scr);
    setup = xcb_get_setup(m_pXcbConnection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0) xcb_screen_next(&iter);
    m_pXcbScreen = iter.data;
#endif
    set_pause_status(false);
    set_quit_status(false);
    return 0;
}

#if defined(WIN32)
LRESULT WINAPI WindowProcVk(HWND window, unsigned int msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(window);
        // fall-thru
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0L;
        default:
            return DefWindowProc(window, msg, wp, lp);
    }
}
#endif

int vkDisplay::set_window(vktrace_window_handle hWindow, unsigned int width, unsigned int height) {
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
    m_window = hWindow;
    m_surface.window = hWindow;
#else
    m_XcbWindow = hWindow;
#endif
#elif defined(WIN32)
    m_windowHandle = hWindow;
#endif
    m_windowWidth = width;
    m_windowHeight = height;
    return 0;
}

int vkDisplay::create_window(const unsigned int width, const unsigned int height) {
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
    return 0;
#else

    uint32_t value_mask, value_list[32];
    m_XcbWindow = xcb_generate_id(m_pXcbConnection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = m_pXcbScreen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(m_pXcbConnection, XCB_COPY_FROM_PARENT, m_XcbWindow, m_pXcbScreen->root, 0, 0, width, height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, m_pXcbScreen->root_visual, value_mask, value_list);

    // Magic code that will send notification when window is destroyed
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(m_pXcbConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(m_pXcbConnection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(m_pXcbConnection, 0, 16, "WM_DELETE_WINDOW");
    this->atom_wm_delete_window = xcb_intern_atom_reply(m_pXcbConnection, cookie2, 0);

    xcb_change_property(m_pXcbConnection, XCB_PROP_MODE_REPLACE, m_XcbWindow, (*reply).atom, 4, 32, 1,
                        &(*this->atom_wm_delete_window).atom);
    free(reply);

    // TODO : Not sure of best place to put this, but I have all the info I need here so just setting it all here for now
    // m_XcbPlatformHandle.connection = m_pXcbConnection;
    // m_XcbPlatformHandle.root = m_pXcbScreen->root;
    m_surface.base.platform = VK_ICD_WSI_PLATFORM_XCB;
    m_surface.connection = m_pXcbConnection;
    m_surface.window = m_XcbWindow;
    return 0;
#endif
#elif defined(WIN32)
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
    RECT wr = {0, 0, width, height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    m_windowHandle = CreateWindow(APP_NAME, APP_NAME, WS_OVERLAPPEDWINDOW, 0, 0, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL,
                                  wcex.hInstance, NULL);

    if (m_windowHandle) {
        m_windowWidth = width;
        m_windowHeight = height;
    } else {
        vktrace_LogError("Failed to create window");
        return -1;
    }
    // TODO : Not sure of best place to put this, but I have all the info I need here so just setting it all here for now
    m_surface.base.platform = VK_ICD_WSI_PLATFORM_WIN32;
    m_surface.hinstance = wcex.hInstance;
    m_surface.hwnd = m_windowHandle;
    return 0;
#endif
}

void vkDisplay::resize_window(const unsigned int width, const unsigned int height) {
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
    m_windowWidth = width;
    m_windowHeight = height;
#else
    if (width != m_windowWidth || height != m_windowHeight) {
        uint32_t values[2];
        values[0] = width;
        values[1] = height;
        xcb_configure_window(m_pXcbConnection, m_XcbWindow, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
        xcb_flush(m_pXcbConnection);
        m_windowWidth = width;
        m_windowHeight = height;

        // Make sure window is visible.
        // xcb doesn't have the equivalent of XSync, so we need to sleep
        // for a while to make sure the X server received and processed
        // the map window request. This is a kludge, but without the sleep,
        // the window doesn't get mapped for a long time.
        xcb_map_window(m_pXcbConnection, m_XcbWindow);
        xcb_flush(m_pXcbConnection);
        usleep(50000);  // 0.05 seconds
    }
#endif
#elif defined(WIN32)
    if (width != m_windowWidth || height != m_windowHeight) {
        RECT wr = {0, 0, width, height};
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
        SetWindowPos(get_window_handle(), HWND_TOP, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE);
        m_windowWidth = width;
        m_windowHeight = height;

        // Make sure window is visible.
        ShowWindow(m_windowHandle, SW_SHOWDEFAULT);
    }
#endif
}

void vkDisplay::process_event() {
#if defined(PLATFORM_LINUX)
#if defined(ANDROID)
// TODO
#else
    xcb_connection_t *xcb_conn = this->get_connection_handle();
    xcb_generic_event_t *event = xcb_poll_for_event(xcb_conn);
    xcb_flush(xcb_conn);
    uint8_t event_code = 0;

    while (event) {
        event_code = event->response_type & 0x7f;
        switch (event_code) {
            case XCB_EXPOSE:
                // TODO: Resize window
                break;
            case XCB_CLIENT_MESSAGE:
                if ((*(xcb_client_message_event_t *)event).data.data32[0] == (*this->atom_wm_delete_window).atom) {
                    this->set_quit_status(true);
                }
                break;
            case XCB_KEY_RELEASE: {
                const xcb_key_release_event_t *key = (const xcb_key_release_event_t *)event;

                switch (key->detail) {
                    case 0x9:  // Escape
                        this->set_quit_status(true);
                        break;
                    case 0x41:
                        this->set_pause_status(!(this->get_pause_status()));
                        break;
                }
            } break;
            case XCB_CONFIGURE_NOTIFY: {
                // TODO resize here too
            } break;
        }
        free(event);
        event = xcb_poll_for_event(xcb_conn);
    }
#endif
#elif defined(WIN32)
    MSG msg = {};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            exit(0);  // Prevents crashing due to vulkan cmds being on different thread than WindowProcVk
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#endif
}
