#include "contraption.h"
#include "server.h"
#include "window.h"
#include "client.h"
#include "render.h"
#include "pixmap.h"
#include "api_priv.h"
#include <string.h>

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

    if (win != NULL && win->owner_thread == thread_id)
    {
        contraption_free_window(win);
    }
}

bool contraption_reload_window(INSTANCE(this), int window, const struct CGadget * gadgets, unsigned gadget_count)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return false;
    }

    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL && win->owner_thread == thread_id)
    {
        return contraption_window_reload(win, gadgets, gadget_count);
    }

    return false;
}

void contraption_raise_window(INSTANCE(this), int window)
{
    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL)
    {
        internal_raise_window(win);
    }
}

bool contraption_move_window(INSTANCE(this), int window, unsigned int left, unsigned int top)
{
    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL)
    {
        struct CExtent old_extents = contraption_window_extents(win);
        contraption_window_move(win, left, top);
        struct CExtent new_extents = contraption_window_extents(win);
        struct CExtent render_extents = contraption_union_extents(&old_extents, &new_extents);
        contraption_render_damage(&render_extents);
        return true;
    }

    return false;
}

bool contraption_resize_window(INSTANCE(this), int window, unsigned int width, unsigned int height)
{
    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL)
    {
        struct CExtent old_extents = contraption_window_extents(win);
        contraption_window_resize(win, width, height);
        struct CExtent new_extents = contraption_window_extents(win);
        struct CExtent render_extents = contraption_union_extents(&old_extents, &new_extents);
        contraption_render_damage(&render_extents);
        return true;
    }

    return false;
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

void contraption_close_menu(INSTANCE(this), int window)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return;
    }

    struct CWindowInternal * win = contraption_find_window(window);

    if (win != NULL && win->owner_thread == thread_id)
    {
        contraption_free_window(win);
    }
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

int contraption_open_connection(INSTANCE(this), const void * notification_object, const char * name, unsigned flags)
{
    return contraption_add_client(get_tid(), notification_object, name, flags);
}

static void contraption_close_connection(INSTANCE(this))
{
    contraption_remove_client(get_tid());
}

void contraption_set_connection_icon(INSTANCE(this), int pixmap_id)
{
    contraption_set_client_icon(get_tid(), pixmap_id);
}

bool contraption_get_event(INSTANCE(this), unsigned * event_id, unsigned * object_id)
{
    Thread_t thread_id = get_tid();
    return contraption_receive_event(thread_id, event_id, object_id);
}

static bool contraption_read_gadget(INSTANCE(this), int window, unsigned gadget_id, struct CGadget * out)
{
    struct CWindowInternal * win = contraption_find_window(window);
    if (win == NULL)
    {
        return false;
    }

    if (get_tid() != win->owner_thread)
    {
        return false;
    }


    if (gadget_id >= win->properties.gadget_count)
    {
        return false;
    }

    struct CGadgetInternal * win_gadgets = (struct CGadgetInternal *) win->properties.gadgets;

    memcpy(out, &win_gadgets[gadget_id].properties, sizeof(struct CGadget));
}

static bool contraption_write_gadget(INSTANCE(this), int window, unsigned gadget_id, const struct CGadget * in)
{
    struct CWindowInternal * win = contraption_find_window(window);
    if (win == NULL)
    {
        return false;
    }

    if (get_tid() != win->owner_thread)
    {
        return false;
    }


    if (gadget_id >= win->properties.gadget_count)
    {
        return false;
    }

    struct CGadgetInternal * win_gadgets = (struct CGadgetInternal *) win->properties.gadgets;

    memcpy(&win_gadgets[gadget_id].properties, in, sizeof(struct CGadget));

    struct CExtent window_extents = contraption_window_extents(win);

    contraption_render_damage(&window_extents);
}

int contraption_open_pixmap(INSTANCE(this), const struct CPixmap * pixmap)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return false;
    }

    return contraption_pixmap_open(pixmap);
}

int contraption_load_pixmap(INSTANCE(this), int pixmap_id, const uint32_t * data)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return false;
    }

    return contraption_pixmap_data(pixmap_id, data);
}

int contraption_close_pixmap(INSTANCE(this), int pixmap_id)
{
    Thread_t thread_id = get_tid();
    if (!contraption_is_client(thread_id))
    {
        return false;
    }

    return contraption_pixmap_close(pixmap_id);
}

VTABLE struct ContraptionVTable contraption_vtable = {
    contraption_open_window,
    contraption_close_window,
    contraption_raise_window,
    contraption_reload_window,
    contraption_move_window,
    contraption_resize_window,
    contraption_open_menu,
    contraption_close_menu,
    contraption_attach_menu,
    contraption_open_connection,
    contraption_close_connection,
    contraption_set_connection_icon,
    contraption_get_event,

    contraption_read_gadget,
    contraption_write_gadget,

    contraption_open_pixmap,
    contraption_load_pixmap,
    contraption_close_pixmap
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
