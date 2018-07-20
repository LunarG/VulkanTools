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
#include <xcb/randr.h>
#include <xcb/xcb_ewmh.h>

extern "C" {
__attribute__((visibility("default"))) vkDisplayXcb *CreateVkDisplayXcb() { return new vkDisplayXcb(); }
}

vkDisplayXcb::vkDisplayXcb() : m_windowWidth(0), m_windowHeight(0) {
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
    if (xcb_connection_has_error(m_pXcbConnection)) {
        vktrace_LogError("failed to connect to X11 server");
        return -1;
    }
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

    m_windowWidth = width;
    m_windowHeight = height;

    xcb_create_window(m_pXcbConnection, XCB_COPY_FROM_PARENT, m_XcbWindow, m_pXcbScreen->root, 0, 0, width, height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, m_pXcbScreen->root_visual, value_mask, value_list);

    // Magic code to get screen size
    xcb_randr_get_screen_resources_cookie_t screenResCookie;
    xcb_randr_crtc_t *pCRTC;
    xcb_randr_get_crtc_info_cookie_t crtcResCookie;
    xcb_flush(m_pXcbConnection);
    screenResCookie = xcb_randr_get_screen_resources(m_pXcbConnection, m_XcbWindow);
    xcb_randr_get_screen_resources_reply_t *screenResReply;
    screenResReply = xcb_randr_get_screen_resources_reply(m_pXcbConnection, screenResCookie, 0);
    if (screenResReply)
        pCRTC = xcb_randr_get_screen_resources_crtcs(screenResReply);
    else
        return -1;
    crtcResCookie = xcb_randr_get_crtc_info(m_pXcbConnection, *pCRTC, 0);
    xcb_randr_get_crtc_info_reply_t *crtcResReply;
    crtcResReply = xcb_randr_get_crtc_info_reply(m_pXcbConnection, crtcResCookie, 0);
    if (crtcResReply) {
        m_screenWidth = crtcResReply->width;
        m_screenHeight = crtcResReply->height;
    } else
        return -1;

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
        if (width >= m_screenWidth || height >= m_screenHeight) {
            // Magic code that makes window full screen
            xcb_ewmh_connection_t EWMH;
            xcb_intern_atom_cookie_t *EWMHCookie = xcb_ewmh_init_atoms(m_pXcbConnection, &EWMH);
            xcb_ewmh_init_atoms_replies(&EWMH, EWMHCookie, NULL);
            xcb_change_property(m_pXcbConnection, XCB_PROP_MODE_REPLACE, m_XcbWindow, EWMH._NET_WM_STATE, XCB_ATOM_ATOM, 32, 1,
                                &(EWMH._NET_WM_STATE_FULLSCREEN));
        }

        uint32_t values[2];
        values[0] = width;
        values[1] = height;
        xcb_configure_window(m_pXcbConnection, m_XcbWindow, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
        xcb_flush(m_pXcbConnection);

        // Make sure window is visible.
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

void vkDisplayXcb::set_window_handle(void *pHandle) { m_XcbWindow = *((xcb_window_t *)pHandle); }
