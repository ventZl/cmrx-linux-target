#include "../contraption/contraption.h"
#include "about_dlg.h"
#include "events.h"

#define _STR2(arg)     # arg
#define _STR(arg)   _STR2(arg)
#define MAX_WINDOWS_STR _STR(MAX_WINDOWS)
#define SCREEN_WIDTH_STR _STR(SCREEN_WIDTH)
#define SCREEN_HEIGHT_STR _STR(SCREEN_HEIGHT)

struct CGadget about_gadgets[] = {
    {
        .type = GADGET_BUTTON,
        .top = 140,
        .left = 330/2 - 40,
        .width = 80,
        .height = 30,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "OK",
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
        .event_id = EVENT_BUTTON_OK

    },
    {
        .type = GADGET_TITLE_BAR,
        .top = 0,
        .left = 0,
        .width = 330,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .state = 0
    },
    {
        .type = GADGET_TEXT,
        .top = 35,
        .left = 10,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "CMRX master @ 1c68fc4!",
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_TOP | TEXT_MONO | TEXT_BOLD | TEXT_ITALIC
    },

    {
        .type = GADGET_TEXT,
        .top = 57,
        .left = 0,
        .width = 165,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "Target platform:",
        .text_margin_horiz = 5,
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_RIGHT | TEXT_ALIGN_TOP | TEXT_BOLD
    },
    {
        .type = GADGET_TEXT,
        .top = 57,
        .left = 165,
        .width = 165,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "Linux x86_64",
        .text_margin_horiz = 5,
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP
    },

    {
        .type = GADGET_TEXT,
        .top = 79,
        .left = 0,
        .width = 165,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "Screen resolution:",
        .text_margin_horiz = 5,
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_RIGHT | TEXT_BOLD
    },
    {
        .type = GADGET_TEXT,
        .top = 79,
        .left = 165,
        .width = 165,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = SCREEN_WIDTH_STR "x" SCREEN_HEIGHT_STR,
        .text_margin_horiz = 5,
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_LEFT | TEXT_ITALIC
    },

    {
        .type = GADGET_TEXT,
        .top = 101,
        .left = 0,
        .width = 165,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "Window arena:",
        .text_margin_horiz = 5,
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_RIGHT
    },
    {
        .type = GADGET_TEXT,
        .top = 101,
        .left = 165,
        .width = 165,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = MAX_WINDOWS_STR,
        .text_margin_horiz = 5,
        .fg_color = 0x000000,
        .text_flags = TEXT_ALIGN_LEFT
    },
};

struct CWindow about_win = {
    .width = 330,
    .height = 190,
    .top = 90,
    .left = 1280 / 2 - (330 / 2) ,
    .title = "About CMRX",
    .gadgets = about_gadgets,
    .gadget_count = sizeof(about_gadgets)/sizeof(about_gadgets[0]),
    .background = BACKGROUND_WINDOW,
    .flags = WINDOW_FLAG_BORDER
};
