#include "icons.h"
#include <cmrx/ipc/rpc.h>
#include <cmrx/ipc/notify.h>

#include <cmrx/application.h>
#include "../contraption/contraption.h"

#include <cmrx/rpc/implementation.h>
#include "../util/xpm.h"

#include "../contraption/img/cmrx-logo.xpm"
#include "../../img/browser.xpm"
#include "../../img/present.xpm"
#include "../../img/terminal.xpm"
#include "../contraption/img/window.xpm"

#include <stdio.h>

IMPLEMENTATION_OF(struct Icons, struct IconsVTable);

static int icons_get_icon(INSTANCE(this), unsigned icon_id)
{
    if (this->icons[icon_id].icon_id == icon_id)
    {
        return this->icons[icon_id].pixmap_id;
    }

    return PIXMAP_NONE;
}

VTABLE struct IconsVTable icons_vtable = {
    icons_get_icon
};

struct Icons icon_server = {
    &icons_vtable,
};

static const char ** xpm_icons[] = {
    cmrx_logo_xpm,
    browser_xpm,
    terminal_xpm,
    present_xpm,
    window_xpm
};

int load_xpm_pixmap(const char ** xpm) {
    struct Pixmap pix = xpm_to_pixmap(xpm);
    int pixmap_id = rpc_call(&display, open_pixmap, &pix.dim);
    if (pixmap_id != PIXMAP_NONE) {
        rpc_call(&display, load_pixmap, pixmap_id, pix.data);
    }

    pixmap_free(&pix);
    return pixmap_id;
}

int icon_main(void * data)
{
    struct Icons * server = (struct Icons *) data;
    bool connected = rpc_call(&display, open_connection, server, "Icons", CLIENT_SKIP_DOCK);
    if (!connected)
    {
        return 0;
    }

    for (int q = 0; q < ICON_COUNT; ++q)
    {
        printf("Loading icon %d\n", q);
        server->icons[q].icon_id = q;
        server->icons[q].pixmap_id = load_xpm_pixmap(xpm_icons[q]);
    }

    while (wait_for_object(&server, 0))
    {
        printf("Wololo\n");
    }

}

OS_APPLICATION_MMIO_RANGE(icons, 0, 0);
OS_APPLICATION(icons);
OS_THREAD_CREATE(icons, icon_main, &icon_server, 64);
