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

#if defined(PLATFORM_LINUX) && !defined(ANDROID) && defined(VK_USE_PLATFORM_WAYLAND_KHR)
#include <linux/input.h>
#endif

#if defined(PLATFORM_LINUX) && defined(ANDROID)
#include <jni.h>
#endif

#define APP_NAME "vkreplay_vk"
#define IDI_ICON 101

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_XCB_KHR)

const char *vkDisplayXcb::NAME = "xcb";

vkDisplayXcb::vkDisplayXcb() : m_windowWidth(0), m_windowHeight(0), m_frameNumber(0) {
    memset(&m_surface, 0, sizeof(VkIcdSurfaceXcb));
    m_pXcbConnection = NULL;
    m_pXcbScreen = NULL;
    m_XcbWindow = 0;
}

vkDisplayXcb::~vkDisplayXcb() {
    if (m_XcbWindow != 0) {
        xcb_destroy_window(m_pXcbConnection, m_XcbWindow);
    }
    if (m_pXcbConnection != NULL) {
        xcb_disconnect(m_pXcbConnection);
    }
}

int vkDisplayXcb::init(const unsigned int gpu_idx) {
    // m_gpuIdx = gpu_idx;
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;
    m_pXcbConnection = xcb_connect(NULL, &scr);
    setup = xcb_get_setup(m_pXcbConnection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0) xcb_screen_next(&iter);
    m_pXcbScreen = iter.data;

    set_pause_status(false);
    set_quit_status(false);
    return 0;
}

int vkDisplayXcb::create_window(const unsigned int width, const unsigned int height) {
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
}

void vkDisplayXcb::resize_window(const unsigned int width, const unsigned int height) {
    if (width != m_windowWidth || height != m_windowHeight) {
        m_windowWidth = width;
        m_windowHeight = height;
        uint32_t values[2];
        values[0] = width;
        values[1] = height;
        xcb_configure_window(m_pXcbConnection, m_XcbWindow, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
        xcb_flush(m_pXcbConnection);

        // Make sure window is visible.
        // xcb doesn't have the equivalent of XSync, so we need to sleep
        // for a while to make sure the X server received and processed
        // the map window request. This is a kludge, but without the sleep,
        // the window doesn't get mapped for a long time.
        xcb_map_window(m_pXcbConnection, m_XcbWindow);
        xcb_flush(m_pXcbConnection);
        usleep(50000);  // 0.05 seconds
    }
}

void vkDisplayXcb::process_event() {
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
                    case 0x41:  // Space
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
}

#endif  // defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_XCB_KHR)

#if defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_WAYLAND_KHR)

const char *vkDisplayWayland::NAME = "wayland";

///////////////////////////////////////////////////////////////

vkDisplayWayland::vkDisplayWayland() : m_windowWidth(0), m_windowHeight(0), m_frameNumber(0) {
    memset(&m_surface, 0, sizeof(VkIcdSurfaceWayland));
}

vkDisplayWayland::~vkDisplayWayland() {
    if (m_keyboard) wl_keyboard_destroy(m_keyboard);
    if (m_pointer) wl_pointer_destroy(m_pointer);
    if (m_seat) wl_seat_destroy(m_seat);
    if (m_shell_surface) wl_shell_surface_destroy(m_shell_surface);
    if (m_wl_surface) wl_surface_destroy(m_wl_surface);
    if (m_shell) wl_shell_destroy(m_shell);
    if (m_compositor) wl_compositor_destroy(m_compositor);
    if (m_registry) wl_registry_destroy(m_registry);
    if (m_display) wl_display_disconnect(m_display);
}

int vkDisplayWayland::init(const unsigned int gpu_idx) {
    // m_gpuIdx = gpu_idx;
    try {
        m_display = wl_display_connect(NULL);
        if (!m_display) throw std::runtime_error("failed to connect to the display server");

        m_registry = wl_display_get_registry(m_display);
        if (!m_registry) throw std::runtime_error("failed to get registry");

        wl_registry_add_listener(m_registry, &vkDisplayWayland::registry_listener, this);
        wl_display_roundtrip(m_display);

        if (!m_compositor) throw std::runtime_error("failed to bind compositor");

        if (!m_shell) throw std::runtime_error("failed to bind shell");
    } catch (...) {
        if (m_shell) wl_shell_destroy(m_shell);
        if (m_compositor) wl_compositor_destroy(m_compositor);
        if (m_registry) wl_registry_destroy(m_registry);
        if (m_display) wl_display_disconnect(m_display);

        throw;
    }

    set_pause_status(false);
    set_quit_status(false);
    return 0;
}

int vkDisplayWayland::create_window(const unsigned int width, const unsigned int height) {
    m_wl_surface = wl_compositor_create_surface(m_compositor);
    if (!m_wl_surface) throw std::runtime_error("failed to create surface");

    m_shell_surface = wl_shell_get_shell_surface(m_shell, m_wl_surface);
    if (!m_shell_surface) throw std::runtime_error("failed to shell_surface");

    wl_shell_surface_add_listener(m_shell_surface, &vkDisplayWayland::shell_surface_listener, this);
    // set title
    // wl_shell_surface_set_title(m_shell_surface, "");
    wl_shell_surface_set_toplevel(m_shell_surface);

    m_surface.base.platform = VK_ICD_WSI_PLATFORM_WAYLAND;
    m_surface.display = m_display;
    m_surface.surface = m_wl_surface;

    return 0;
}

void vkDisplayWayland::resize_window(const unsigned int width, const unsigned int height) {
    if (width != m_windowWidth || height != m_windowHeight) {
        m_windowWidth = width;
        m_windowHeight = height;
        // In Wayland, the shell_surface should resize based on the Vulkan surface automagically
    }
}

void vkDisplayWayland::process_event() {
    // Interaction is handled in the callbacks
    wl_display_dispatch_pending(m_display);
}

// Wayland callbacks, needed to handle events from the display manager.
// Some stubs here that could be potentially removed
void vkDisplayWayland::handle_ping(void *data, wl_shell_surface *shell_surface, uint32_t serial) {
    wl_shell_surface_pong(shell_surface, serial);
}

void vkDisplayWayland::handle_configure(void *data, wl_shell_surface *shell_surface, uint32_t edges, int32_t width,
                                        int32_t height) {}

void vkDisplayWayland::handle_popup_done(void *data, wl_shell_surface *shell_surface) {}

const struct wl_shell_surface_listener vkDisplayWayland::shell_surface_listener = {
    vkDisplayWayland::handle_ping, vkDisplayWayland::handle_configure, vkDisplayWayland::handle_popup_done};

void vkDisplayWayland::pointer_handle_enter(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface,
                                            wl_fixed_t sx, wl_fixed_t sy) {}

void vkDisplayWayland::pointer_handle_leave(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {}

void vkDisplayWayland::pointer_handle_motion(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy) {}

void vkDisplayWayland::pointer_handle_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time,
                                             uint32_t button, uint32_t state) {
    if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED) {
        vkDisplayWayland *display = (vkDisplayWayland *)data;
        wl_shell_surface_move(display->m_shell_surface, display->m_seat, serial);
    }
}

void vkDisplayWayland::pointer_handle_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis,
                                           wl_fixed_t value) {}

const struct wl_pointer_listener vkDisplayWayland::pointer_listener = {vkDisplayWayland::pointer_handle_enter,
                                                                       vkDisplayWayland::pointer_handle_leave,
                                                                       vkDisplayWayland::pointer_handle_motion,
                                                                       vkDisplayWayland::pointer_handle_button,
                                                                       vkDisplayWayland::pointer_handle_axis,
                                                                       nullptr,
                                                                       nullptr,
                                                                       nullptr,
                                                                       nullptr};

void vkDisplayWayland::keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int fd, uint32_t size) {}

void vkDisplayWayland::keyboard_handle_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface,
                                             struct wl_array *keys) {}

void vkDisplayWayland::keyboard_handle_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                                             struct wl_surface *surface) {}

void vkDisplayWayland::keyboard_handle_key(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key,
                                           uint32_t state) {
    if (state != WL_KEYBOARD_KEY_STATE_RELEASED) return;
    vkDisplayWayland *display = (vkDisplayWayland *)data;
    switch (key) {
        case KEY_ESC:
            display->set_quit_status(true);
            break;
        case KEY_SPACE:
            display->set_pause_status(true);
            break;
    }
}

void vkDisplayWayland::keyboard_handle_modifiers(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed,
                                                 uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {}

const struct wl_keyboard_listener vkDisplayWayland::keyboard_listener = {
    vkDisplayWayland::keyboard_handle_keymap, vkDisplayWayland::keyboard_handle_enter,     vkDisplayWayland::keyboard_handle_leave,
    vkDisplayWayland::keyboard_handle_key,    vkDisplayWayland::keyboard_handle_modifiers, nullptr};

void vkDisplayWayland::seat_handle_capabilities(void *data, wl_seat *seat, uint32_t caps) {
    // Subscribe to pointer events
    vkDisplayWayland *display = (vkDisplayWayland *)data;
    if ((caps & WL_SEAT_CAPABILITY_POINTER) && !display->m_pointer) {
        display->m_pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(display->m_pointer, &pointer_listener, display);
    } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && display->m_pointer) {
        wl_pointer_destroy(display->m_pointer);
        display->m_pointer = NULL;
    }
    // Subscribe to keyboard events
    if (caps & WL_SEAT_CAPABILITY_KEYBOARD) {
        display->m_keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(display->m_keyboard, &keyboard_listener, display);
    } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD)) {
        wl_keyboard_destroy(display->m_keyboard);
        display->m_keyboard = NULL;
    }
}

const struct wl_seat_listener vkDisplayWayland::seat_listener = {vkDisplayWayland::seat_handle_capabilities, nullptr};

void vkDisplayWayland::registry_handle_global(void *data, wl_registry *registry, uint32_t id, const char *interface,
                                              uint32_t version) {
    // pickup wayland objects when they appear
    vkDisplayWayland *display = (vkDisplayWayland *)data;
    if (strcmp(interface, "wl_compositor") == 0) {
        display->m_compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shell") == 0) {
        display->m_shell = (wl_shell *)wl_registry_bind(registry, id, &wl_shell_interface, 1);
    } else if (strcmp(interface, "wl_seat") == 0) {
        display->m_seat = (wl_seat *)wl_registry_bind(registry, id, &wl_seat_interface, 1);
        wl_seat_add_listener(display->m_seat, &seat_listener, display);
    }
}

void vkDisplayWayland::registry_handle_global_remove(void *data, wl_registry *registry, uint32_t name) {}

const struct wl_registry_listener vkDisplayWayland::registry_listener = {vkDisplayWayland::registry_handle_global,
                                                                         vkDisplayWayland::registry_handle_global_remove};

#endif  // defined(PLATFORM_LINUX) && defined(VK_USE_PLATFORM_WAYLAND_KHR)

#if defined(PLATFORM_LINUX) && defined(ANDROID)
#include <jni.h>

vkDisplayAndroid::vkDisplayAndroid(struct android_app *app) : m_windowWidth(0), m_windowHeight(0), m_frameNumber(0) {
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

void vkDisplayAndroid::set_window_handle(void* pHandle) {
    // Unused
}

#endif  // defined(PLATFORM_LINUX) && defined(ANDROID)

#if defined(WIN32)

LRESULT WINAPI WindowProcVk(HWND window, unsigned int msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_KEYUP: {
            auto *const display = (vkDisplay *)GetWindowLongPtr(window, GWLP_USERDATA);
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

vkDisplayWin32::vkDisplayWin32() : m_windowWidth(0), m_windowHeight(0), m_frameNumber(0) {
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

        RECT wr = {0, 0, (LONG)width, (LONG)height};
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
        SetWindowPos(get_window_handle(), HWND_TOP, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE);

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

#endif  // defined(WIN32)
