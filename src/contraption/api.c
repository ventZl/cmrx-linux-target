#include "contraption.h"
#include "server.h"
#include "window.h"
#include "client.h"

#include <cmrx/rpc/implementation.h>
#include <cmrx/application.h>
#include <cmrx/ipc/thread.h>

IMPLEMENTATION_OF(struct Contraption, struct ContraptionVTable);

int contraption_open_window(INSTANCE(this), const struct CWindow * window)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return false;
    }

    return contraption_load_window(window, true, false, thread_id);
}

void contraption_close_window(INSTANCE(this), int window)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return;
    }

    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL)
    {
        contraption_free_window(win);
    }
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
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return false;
    }

    return contraption_load_window(menu, false, true, thread_id);
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

int contraption_open_connection(INSTANCE(this), void * notification_object)
{
    return contraption_add_client(get_tid(), notification_object);
}

static void contraption_close_connection(INSTANCE(this))
{
    contraption_remove_client(get_tid());
}

static bool contraption_get_event(INSTANCE(this), unsigned * event_id, unsigned * object_id)
{
    Thread_t thread_id = get_tid();
    return contraption_receive_event(thread_id, event_id, object_id);
}

VTABLE struct ContraptionVTable contraption_vtable = {
    contraption_open_window,
    contraption_close_window,
    contraption_raise_window,
    contraption_open_menu,
    NULL,
    contraption_attach_menu,
    contraption_open_connection,
    contraption_close_connection,
    contraption_get_event
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
    .popup_window = NULL,
    .menu_opener_window = NULL,
    .menu_opener_gadget = NULL,
};
