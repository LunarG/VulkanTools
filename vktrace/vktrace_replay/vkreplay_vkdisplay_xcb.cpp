#include "vkreplay_vkdisplay.h"

extern "C" {
    __attribute__ ((visibility("default"))) vkDisplayXcb* CreateVkDisplayXcb()
    {
        return new vkDisplayXcb();
    }
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
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

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
                xcb_configure_notify_event_t *cne = (xcb_configure_notify_event_t *)event;
                if (cne->width != m_windowWidth || cne->height != m_windowHeight) {
                    // Our window has been resized, probably by the window manager.
                    // There's not really anything we can do here other than abort.
                    vktrace_LogError("bad window size, aborting!");
                    set_quit_status(true);
                }
            } break;
            case XCB_MAP_NOTIFY: {
                // If we were waiting for a MapWindow request to be processed,
                // we can now continue
                set_pause_status(false);
            } break;
        }
        free(event);
        event = xcb_poll_for_event(xcb_conn);
    }
}

void vkDisplayXcb::set_window_handle(void* pHandle) {
    m_XcbWindow = *((xcb_window_t*)pHandle);
}