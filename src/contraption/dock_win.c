#include "dock_win.h"
#include "contraption.h"
#include "dock_events.h"

#define DOCK_BUTTON(no, event) { .type = GADGET_PIXMAP, .width = 32, .height = 32, .left = 8 + ((no) * 40), .top = 4, .flags = GADGET_FLAG_ENABLED, .state = 0, .event_id = event }
#define EVENT_DOCK_CLICKED(no)          (EVENT_DOCK_BUTTON + no)


struct CGadget dock_window_gadgets[] = {
    DOCK_BUTTON(0, EVENT_DOCK_CLICKED(0)),
    DOCK_BUTTON(1, EVENT_DOCK_CLICKED(1)),
    DOCK_BUTTON(2, EVENT_DOCK_CLICKED(2)),
    DOCK_BUTTON(3, EVENT_DOCK_CLICKED(3)),
    DOCK_BUTTON(4, EVENT_DOCK_CLICKED(4)),
    DOCK_BUTTON(5, EVENT_DOCK_CLICKED(5)),
    DOCK_BUTTON(6, EVENT_DOCK_CLICKED(6)),
    DOCK_BUTTON(7, EVENT_DOCK_CLICKED(7)),
    DOCK_BUTTON(8, EVENT_DOCK_CLICKED(8)),
    DOCK_BUTTON(9, EVENT_DOCK_CLICKED(9)),
    DOCK_BUTTON(10, EVENT_DOCK_CLICKED(10)),
    DOCK_BUTTON(11, EVENT_DOCK_CLICKED(11))
};

struct CWindow dock_window = {
    .width = 40,
    .height = 40,
    .top = SCREEN_HEIGHT - 40,
    .left = (SCREEN_WIDTH / 2) - 20,
    .title = "Dock",
    .gadgets = dock_window_gadgets,
    .gadget_count = 1,
    .background = BACKGROUND_DOCK,
    .flags = 0,
};
