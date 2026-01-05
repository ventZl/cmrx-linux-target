#include "../contraption/contraption.h"
#include "events.h"

struct CGadget term_menu_items[] = {
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "About Terminal",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_TERM_ABOUT,
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 19,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "Quit",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_TERM_QUIT
    },
};

struct CWindow term_menu = {
    .top = 20,
    .left = 0,
    .width = 90,
    .height = 40,
    .gadgets = term_menu_items,
    .gadget_count = 2,
    .background = BACKGROUND_WHITE
};


struct CGadget window_menu_items[] = {
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "New",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_WINDOW_NEW
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 19,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "Close",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_WINDOW_CLOSE

    }
};

struct CWindow window_menu = {
    .top = 20,
    .left = 0,
    .width = 90,
    .height = 80,
    .gadgets = window_menu_items,
    .gadget_count = 4,
    .background = BACKGROUND_WHITE
};

struct CGadget term_app_menu_items[] = {
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 0,
        .width = 40,
        .height = 20,
        .text = "Term",
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE | TEXT_BOLD,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .sub_menu_id = WINDOW_NONE,
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 39,
        .width = 50,
        .height = 20,
        .text = "Window",
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .sub_menu_id = WINDOW_NONE,
    },
};

struct CWindow term_app_menu = {
    .top = 0,
    .left = 0,
    .width = 1280,
    .height = 20,
    .gadgets = term_app_menu_items,
    .gadget_count = sizeof(term_app_menu_items)/sizeof(term_app_menu_items[0])
};


