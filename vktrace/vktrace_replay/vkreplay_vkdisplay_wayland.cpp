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
 * Author: Joey Bzdek <joey@lunarg.com>
 */

#include "vkreplay_vkdisplay.h"
#include <linux/input.h>

extern "C" {
__attribute__((visibility("default"))) vkDisplayWayland *CreateVkDisplayWayland() { return new vkDisplayWayland(); }
}

vkDisplayWayland::vkDisplayWayland() : m_windowWidth(0), m_windowHeight(0) {
    memset(&m_surface, 0, sizeof(VkIcdSurfaceWayland));

    shell_surface_listener.ping = handle_ping;
    shell_surface_listener.configure = handle_configure;
    shell_surface_listener.popup_done = handle_popup_done;

    pointer_listener.enter = pointer_handle_enter;
    pointer_listener.leave = pointer_handle_leave;
    pointer_listener.motion = pointer_handle_motion;
    pointer_listener.button = pointer_handle_button;
    pointer_listener.axis = pointer_handle_axis;

    keyboard_listener.keymap = keyboard_handle_keymap;
    keyboard_listener.enter = keyboard_handle_enter;
    keyboard_listener.leave = keyboard_handle_leave;
    keyboard_listener.key = keyboard_handle_key;
    keyboard_listener.modifiers = keyboard_handle_modifiers;

    seat_listener.capabilities = seat_handle_capabilities;

    registry_listener.global = registry_handle_global;
    registry_listener.global_remove = registry_handle_global_remove;

    output_listener.geometry = output_handle_geometry;
    output_listener.mode = output_handle_mode;
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
        if (!m_display) throw std::runtime_error("Wayland failed to connect to the display server");

        m_registry = wl_display_get_registry(m_display);
        if (!m_registry) throw std::runtime_error("Wayland failed to get registry");

        wl_registry_add_listener(m_registry, &vkDisplayWayland::registry_listener, this);
        wl_display_roundtrip(m_display);

        if (!m_compositor) throw std::runtime_error("Wayland failed to bind compositor");

        if (!m_shell) throw std::runtime_error("Wayland failed to bind shell");
    } catch (const std::runtime_error &ex) {
        if (m_shell) wl_shell_destroy(m_shell);
        if (m_compositor) wl_compositor_destroy(m_compositor);
        if (m_registry) wl_registry_destroy(m_registry);
        if (m_display) wl_display_disconnect(m_display);

        vktrace_LogError(ex.what());
        return -1;
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
        if (width >= m_screenWidth || height >= m_screenHeight) {
            // Make window full screen
            wl_shell_surface_set_fullscreen(m_shell_surface, WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT, m_refresh, m_output);
        }

        // In Wayland, the shell_surface should resize based on the Vulkan surface automagically
    }
}

void vkDisplayWayland::process_event() {
    // Interaction is handled in the callbacks
    wl_display_dispatch_pending(m_display);
}

void vkDisplayWayland::set_window_handle(void *pHandle) { m_display = *((wl_display **)pHandle); }

// Wayland callbacks, needed to handle events from the display manager.
// Some stubs here that could be potentially removed
void vkDisplayWayland::handle_ping(void *data, wl_shell_surface *shell_surface, uint32_t serial) {
    wl_shell_surface_pong(shell_surface, serial);
}

void vkDisplayWayland::handle_configure(void *data, wl_shell_surface *shell_surface, uint32_t edges, int32_t width,
                                        int32_t height) {}

void vkDisplayWayland::handle_popup_done(void *data, wl_shell_surface *shell_surface) {}

struct wl_shell_surface_listener vkDisplayWayland::shell_surface_listener;

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

struct wl_pointer_listener vkDisplayWayland::pointer_listener;

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

struct wl_keyboard_listener vkDisplayWayland::keyboard_listener;

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

struct wl_seat_listener vkDisplayWayland::seat_listener;

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
    } else if (strcmp(interface, "wl_output") == 0) {
        display->m_output = (wl_output *)wl_registry_bind(registry, id, &wl_output_interface, 1);
        wl_output_add_listener(display->m_output, &output_listener, display);
    }
}

void vkDisplayWayland::registry_handle_global_remove(void *data, wl_registry *registry, uint32_t name) {}

struct wl_registry_listener vkDisplayWayland::registry_listener;

void vkDisplayWayland::output_handle_geometry(void *data, struct wl_output *wl_output, int x, int y, int physical_width,
                                              int physical_height, int subpixel, const char *make, const char *model,
                                              int transform) {}

void vkDisplayWayland::output_handle_mode(void *data, struct wl_output *wl_output, uint32_t flags, int width, int height,
                                          int refresh) {
    auto *display = (vkDisplayWayland *)data;
    display->m_screenWidth = width;
    display->m_screenHeight = height;
    display->m_refresh = refresh;
}

struct wl_output_listener vkDisplayWayland::output_listener;