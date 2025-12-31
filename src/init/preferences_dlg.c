#include "../contraption/contraption.h"

struct CGadget preferences_gadgets[] = {
    {
        .type = GADGET_TITLE_BAR,
        .width = 400,
        .height = 22,
        .top = 0,
        .left = 0,
        .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
        .state = 0
    },
    {
        .type = GADGET_PANEL,
        .width = 398,
        .height = 32,
        .top = 22,
        .left = 1
    }
};

struct CWindow preferences_win = {
    .width = 400,
    .height = 300,
    .top = 100,
    .left = 300,
    .title = "Properties",
    .gadgets = preferences_gadgets,
    .gadget_count = sizeof(preferences_gadgets)/sizeof(preferences_gadgets[0]),
    .background = BACKGROUND_WINDOW,
    .flags = WINDOW_FLAG_BORDER,
};

