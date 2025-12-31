#pragma once

#include <cmrx/rpc/interface.h>
#include <stdint.h>
#include <stdbool.h>

#define SCREEN_WIDTH        1280
#define SCREEN_HEIGHT       800

#define MAX_WINDOWS 64
#define MAX_GADGETS 256

#define WINDOW_NONE         -1

struct CGadget;

enum CWindowBackground {
    BACKGROUND_WINDOW = 0,
    BACKGROUND_DESKTOP,
    BACKGROUND_WHITE,
    BACKGROUND_BLACK
};

enum CWindowFlags {
    WINDOW_FLAG_BORDER = 1,
    WINDOW_FLAG_BELOW_ALL = 2,
};

struct CWindow {
    int left, top;
    int width, height;
    int min_width, min_height;
    int max_width, max_height;

    char title[64];

    enum CWindowBackground background;
    unsigned flags;

    struct CGadget * gadgets;
    unsigned gadget_count;
};

struct CWindowInternal {
    struct CWindow properties;
    int menu_window_id;
    int id;
    bool is_menu;
    unsigned owner_thread;
};

#define BUTTON_RADIUS 4

enum CGadgetType {
    GADGET_BUTTON = 0,
    GADGET_TEXT,
    GADGET_TITLE_BAR,
    GADGET_RADIO,
    GADGET_CHECKBOX,
    GADGET_TEXT_INPUT,
    GADGET_MENU_ITEM,
    GADGET_PANEL,            // passive panel rendering window background

    GADGET_KNOWN_TYPES
};

enum CGadgetFlags {
    GADGET_FLAG_ENABLED = 1,
    GADGET_FLAG_FOCUSABLE = 2,
    GADGET_FLAG_ACTIVABLE = 4,
    GADGET_FLAG_CLICKABLE = 8
};

enum CGadgetState {
    GADGET_STATE_FOCUSED = 1,
    GADGET_STATE_HOVERED = 2,
    GADGET_STATE_ACTIVATED = 4,
    GADGET_STATE_CLICKED = 8
};

enum CGadgetTextFlags {
    TEXT_ALIGN_LEFT = 1,
    TEXT_ALIGN_RIGHT = 2,
    TEXT_ALIGN_CENTER = 4,
    TEXT_ALIGN_TOP = 8,
    TEXT_ALIGN_MIDDLE = 16,
    TEXT_ALIGN_BOTTOM = 32,
    TEXT_BOLD = 64,
    TEXT_ITALIC = 128,
    TEXT_MONO = 256
};

struct CGadget {
    int left, top;
    int width, height;

    enum CGadgetType type;

    uint32_t fg_color;
    // TODO: Do proper memory management here
    const char * text;
    uint32_t text_flags;
    int sub_menu_id;
    unsigned event_id;

    uint32_t state;
    uint32_t flags;

    uint8_t text_margin_horiz;
    uint8_t text_margin_vert;
};

struct CGadgetInternal {
    struct CGadget properties;
    int window_id;
};

struct CExtent {
    unsigned top, left, right, bottom;
};

struct CPosition {
    long top, left;
};

struct ContraptionVTable {
    int (*open_window)(INSTANCE(this), const struct CWindow * window);
    void (*close_window)(INSTANCE(this), int window);
    void (*raise_window)(INSTANCE(this), int window);
    int (*open_menu)(INSTANCE(this), const struct CWindow * menu);
    void (*close_menu)(INSTANCE(this), int menu);
    void (*attach_menu)(INSTANCE(this), int window, int menu);

    int (*open_connection)(INSTANCE(this), void * notification_object);
    void (*close_connection)(INSTANCE(this));
    bool (*get_event)(INSTANCE(this), unsigned * event_id, unsigned * object_id);
};

struct Contraption {
    const struct ContraptionVTable * vtable;
    bool render;
    bool pointer;
    bool button;
    struct CWindowInternal * cursor_window;
    int menu_window_id;
    struct CGadgetInternal * cursor_gadget;
    struct CGadgetInternal * focus_gadget;
    bool cursor_gadget_grab;
    struct CWindowInternal * popup_window;
    struct CPosition last_pos;
    struct CWindowInternal * menu_opener_window;
    struct CGadgetInternal * menu_opener_gadget;
};

extern struct Contraption display;
