#include "../contraption/contraption.h"
#include "term_win.h"
#include "events.h"

#define _STR2(arg)     # arg
#define _STR(arg)   _STR2(arg)
#define MAX_WINDOWS_STR _STR(MAX_WINDOWS)
#define SCREEN_WIDTH_STR _STR(SCREEN_WIDTH)
#define SCREEN_HEIGHT_STR _STR(SCREEN_HEIGHT)

struct CGadget term_gadgets[] = {
    {
        .type = GADGET_TITLE_BAR,
        .top = 0,
        .left = 0,
        .width = 562,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .state = 0
    },
    {
        .type = GADGET_TEXT,
        .top = 22,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "nobody@cmrx$_                                                                   ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 38,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 54,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 70,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 86,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 102,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 118,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 134,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 150,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 166,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 182,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 198,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 214,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 230,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 246,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 262,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 278,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },    {
        .type = GADGET_TEXT,
        .top = 294,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 310,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },    {
        .type = GADGET_TEXT,
        .top = 326,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 342,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },    {
        .type = GADGET_TEXT,
        .top = 358,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 374,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },    {
        .type = GADGET_TEXT,
        .top = 390,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
    {
        .type = GADGET_TEXT,
        .top = 406,
        .left = 0,
        .width = 310,
        .height = 22,
        .flags = GADGET_FLAG_ENABLED,
        .state = 0,
        .text = "                                                                                ",
        .fg_color = 0xAAAAAA,
        .text_flags = TEXT_MONO | TEXT_ALIGN_TOP | TEXT_ALIGN_LEFT
    },
};

struct CWindow term_win = {
    .width = 562,
    .height = 423,
    .top = 90,
    .left = 1280 / 2 - (330 / 2) ,
    .title = "Terminal",
    .gadgets = term_gadgets,
    .gadget_count = sizeof(term_gadgets)/sizeof(term_gadgets[0]),
    .background = BACKGROUND_BLACK,
    .flags = WINDOW_FLAG_BORDER
};

