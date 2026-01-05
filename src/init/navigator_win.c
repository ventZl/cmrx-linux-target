#include "../contraption/contraption.h"

#define GADGET_ICON(_col, _row, _title) \
    GADGET_PIXMAP(_col - 16, _row, 32, 32, PIXMAP_NONE),\
    GADGET_TEXT(_col - 30, _row + 36, 64, 20, _title, 0x000000, TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE)

struct CGadget dialog_gadgets[] = {
    GADGET_TITLE_BAR(400),
    // 1
    GADGET_ICON(50, 30, "Terminal"),

    // 3
    GADGET_ICON(130, 30, "FOSDEM '26")
};

struct CWindow dialog_win = {
    .width = 400,
    .height = 300,
    .top = 100,
    .left = 300,
    .title = "Applications",
    .gadgets = dialog_gadgets,
    .gadget_count = GADGET_COUNT(dialog_gadgets),
    .background = BACKGROUND_WHITE,
    .flags = WINDOW_FLAG_BORDER,
};
