#pragma once
#include <cmrx/rpc/interface.h>

enum IconIDs {
    ICON_CMRX,
    ICON_BROWSER,
    ICON_TERMINAL,
    ICON_PRESENTATION,
    ICON_APPLICATION,
    ICON_COUNT
};

struct IconsVTable {
    int (*get_icon)(INSTANCE(this), unsigned icon_id);
};

struct Icon {
    unsigned icon_id;
    unsigned pixmap_id;
};

struct Icons {
    const struct IconsVTable * vtable;
    struct Icon icons[32];
};

extern struct Icons icon_server;
