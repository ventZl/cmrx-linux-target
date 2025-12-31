#include "../contraption/contraption.h"
#include "events.h"

struct CGadget init_menu_items[] = {
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "About Init",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_INIT_ABOUT,
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 19,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "Preferences...",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_INIT_PREFERENCES
    },
};

struct CWindow init_menu = {
    .top = 20,
    .left = 0,
    .width = 90,
    .height = 40,
    .gadgets = init_menu_items,
    .gadget_count = 2,
    .background = BACKGROUND_WHITE
};


struct CGadget file_menu_items[] = {
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
        .event_id = EVENT_FILE_NEW
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 19,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "Open...",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_FILE_OPEN

    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 39,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "Save",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = 0,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_FILE_SAVE

    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 59,
        .left = 0,
        .width = 90,
        .height = 20,
        .text = "Quit",
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .text_margin_horiz = 5,
        .sub_menu_id = WINDOW_NONE,
        .event_id = EVENT_FILE_QUIT

    },
};

struct CWindow file_menu = {
    .top = 20,
    .left = 0,
    .width = 90,
    .height = 80,
    .gadgets = file_menu_items,
    .gadget_count = 4,
    .background = BACKGROUND_WHITE
};

struct CGadget about_menu_items[] = {
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 0,
        .width = 40,
        .height = 20,
        .text = "Init",
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE | TEXT_BOLD,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .sub_menu_id = WINDOW_NONE,
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 39,
        .width = 40,
        .height = 20,
        .text = "File",
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .sub_menu_id = WINDOW_NONE,
    },
    {
        .type = GADGET_MENU_ITEM,
        .top = 0,
        .left = 79,
        .width = 40,
        .height = 20,
        .text = "Help",
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
        .flags = 0 //GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE
    },
};

struct CWindow init_app_menu = {
    .top = 0,
    .left = 0,
    .width = 1280,
    .height = 20,
    .gadgets = about_menu_items,
    .gadget_count = 3
};

