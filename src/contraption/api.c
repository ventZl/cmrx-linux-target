#include "contraption.h"
#include "server.h"

#include <cmrx/rpc/implementation.h>
#include <cmrx/application.h>

IMPLEMENTATION_OF(struct Contraption, struct ContraptionVTable);

static int contraption_open_window(INSTANCE(this), const struct CWindow * window)
{
    return contraption_load_window(window, true, false);
}

void contraption_close_window(INSTANCE(this), int window)
{

}

void contraption_raise_window(INSTANCE(this), int window)
{
    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL)
    {
        internal_raise_window(win);
    }
}

int contraption_open_menu(INSTANCE(this), const struct CWindow * menu)
{
    return contraption_load_window(menu, false, true);
}

void contraption_attach_menu(INSTANCE(this), int window, int menu)
{
    struct CWindowInternal * win = contraption_find_window(window);
    struct CWindowInternal * menu_win = contraption_find_window(menu);

    if (win != NULL && menu_win != NULL)
    {
        win->menu_window_id = menu;

        if (contraption_active_window() == win)
        {
            contraption_swap_menu(menu);
        }
    }
}

VTABLE struct ContraptionVTable contraption_vtable = {
    contraption_open_window,
    contraption_close_window,
    contraption_raise_window,
    contraption_open_menu,
    NULL,
    contraption_attach_menu
};

struct Contraption display = {
    .vtable = &contraption_vtable,
    .render = false,
    .pointer = false,
    .button = false,
    .cursor_window = NULL,
    .menu_window_id = WINDOW_NONE,
    .cursor_gadget = NULL,
    .focus_gadget = NULL,
    .cursor_gadget_grab = false,
    .popup_window = NULL
};
