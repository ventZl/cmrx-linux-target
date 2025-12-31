#include "../contraption/contraption.h"

struct CGadget dialog_gadgets[] = {
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

struct CWindow dialog_win = {
    .width = 400,
    .height = 300,
    .top = 100,
    .left = 300,
    .title = "File Browser",
    .gadgets = dialog_gadgets,
    .gadget_count = sizeof(dialog_gadgets)/sizeof(dialog_gadgets[0]),
    .background = BACKGROUND_WHITE,
    .flags = WINDOW_FLAG_BORDER,
};
