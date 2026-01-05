#include "desktop.h"
#include "contraption.h"

struct CGadget desktop_gadgets[] = {
    {
        .type = GADGET_PANEL,
        .top = 0,
        .left = 0,
        .width = SCREEN_WIDTH,
        .height = 20,
    }
};

const struct CWindow desktop_window = {
    .top = 0,
    .left = 0,
    .width = SCREEN_WIDTH,
    .height = SCREEN_HEIGHT,
    .gadgets = desktop_gadgets,
    .gadget_count = 1,
    .background = BACKGROUND_DESKTOP,
    .flags = WINDOW_FLAG_BELOW_ALL
};
