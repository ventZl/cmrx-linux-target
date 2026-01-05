#include "window.h"
#include "render.h"
#include "server.h"
#include "gadget.h"
#include "client.h"
#include <cmrx/ipc/notify.h>
#include <assert.h>
#include <stdio.h>

unsigned window_stack[MAX_WINDOWS];
unsigned window_stack_count = 0;

struct CWindowInternal windows[MAX_WINDOWS];
struct CGadgetInternal gadgets[MAX_GADGETS];
unsigned last_window = 0;
unsigned window_id = 1;
unsigned gadget_count = 0;

void contraption_window_init()
{
    for (int q = 0; q < MAX_WINDOWS; ++q)
    {
        windows[q].id = WINDOW_NONE;
    }
}

unsigned contraption_window_count()
{
    return window_stack_count;
}

/* Get ID of window at offset in window stack */
struct CWindowInternal * contraption_window_at_offset(unsigned offset)
{
    return &windows[window_stack[offset]];
}

unsigned contraption_window_offset(const struct CWindowInternal * window)
{
    unsigned offset =  window - windows;
    assert(offset < last_window);
    return offset;
}


struct CWindowInternal * contraption_find_window(int window_id)
{
    for (int q = 0; q < last_window; ++q)
    {
        if (windows[q].id == window_id)
        {
            return &windows[q];
        }
    }

    return NULL;
}

static struct CGadget * contraption_load_gadgets(int owning_window, const struct CGadget * new_gadgets, unsigned count)
{
    if (count > MAX_GADGETS - gadget_count)
    {
        return NULL;
    }

    struct CGadget * first_gadget = (struct CGadget *) &gadgets[gadget_count];

    for (int q = 0; q < count; ++q)
    {
        gadgets[gadget_count].properties = new_gadgets[q];
        gadgets[gadget_count].window_id = owning_window;
        gadget_count++;
    }

    return first_gadget;
}

int contraption_find_free_window()
{
    for (int q = 0; q < MAX_WINDOWS; ++q)
    {
        if (windows[q].id == WINDOW_NONE)
        {
            return q;
        }
    }
    return WINDOW_NONE;
}

int contraption_load_window(const struct CWindow* window, bool show, bool menu, unsigned owning_client)
{
    int win_ref = -1;
    int win_cursor = contraption_find_free_window();
    if (win_cursor != WINDOW_NONE) {
        win_ref = window_id++;

        struct CWindowInternal * new_window = &windows[win_cursor];

        new_window->properties = *window;
        new_window->id = win_ref;
        new_window->is_menu = menu;
        new_window->menu_window_id = WINDOW_NONE;
        new_window->owner_thread = owning_client;

        new_window->properties.gadgets = contraption_load_gadgets(win_ref, window->gadgets, window->gadget_count);

        if (new_window->properties.gadgets != NULL)
        {
            last_window++;
        }
        else
        {
            new_window->id = WINDOW_NONE;
            return -1;
        }

        if (show)
        {
            contraption_stack_window(win_ref);
        }
    }

    return win_ref;
}

/* Move gadgets up by certain amount.
 * Use to rearrange gadget arena after window was closed or its gadgets were deleted.
 */
void contraption_move_gadgets(struct CGadgetInternal * first_gadget, unsigned move_by)
{
    unsigned gadget_pos = first_gadget - gadgets;
    for (int q = gadget_pos; q < gadget_count; ++q)
    {
        gadgets[q - move_by] = gadgets[q];
    }

    gadget_count -= move_by;
}

bool contraption_window_reload(struct CWindowInternal * window, const struct CGadget* gadgets, unsigned gadget_count)
{
    //    contraption_unstack_window(window->id);

    struct CGadgetInternal * last_window_gadget = ((struct CGadgetInternal *) window->properties.gadgets) + window->properties.gadget_count;
    contraption_move_gadgets(last_window_gadget, window->properties.gadget_count);

    for (int q = 0; q < last_window; ++q)
    {
        struct CWindowInternal * win = &windows[q];
        if (win == window || win->id == WINDOW_NONE)
        {
            // Skip this window
            continue;
        }

        if (((struct CGadgetInternal *) win->properties.gadgets) >= last_window_gadget)
        {
            win->properties.gadgets = (struct CGadget *) (((struct CGadgetInternal *) win->properties.gadgets) - window->properties.gadget_count);
        }
    }
    window->properties.gadgets = contraption_load_gadgets(window->id, gadgets, gadget_count);
    window->properties.gadget_count = gadget_count;

    struct CExtent win_extent = contraption_window_extents(window);
    contraption_render_damage(&win_extent);

    return true;
}

void contraption_free_window(struct CWindowInternal * window)
{
    contraption_unstack_window(window->id);

    struct CGadgetInternal * last_window_gadget = ((struct CGadgetInternal *) window->properties.gadgets) + window->properties.gadget_count;
    contraption_move_gadgets(last_window_gadget, window->properties.gadget_count);

    for (int q = 0; q < last_window; ++q)
    {
        struct CWindowInternal * win = &windows[q];
        if (win == window || win->id == WINDOW_NONE)
        {
            // Skip this window
            continue;
        }

        if (((struct CGadgetInternal *) win->properties.gadgets) >= last_window_gadget)
        {
            win->properties.gadgets = (struct CGadget *) (((struct CGadgetInternal *) win->properties.gadgets) - window->properties.gadget_count);
        }
    }

    contraption_send_event(window->owner_thread, EVENT_WINDOW_CLOSED, window->id);
    window->id = WINDOW_NONE;
    window->properties.gadget_count = 0;
    window->properties.gadgets = NULL;
}

void contraption_hide_menu()
{
    assert(display.menu_opener_window != NULL);
    assert(display.menu_opener_gadget != NULL);
    contraption_unstack_window(display.popup_window->id);
    gadget_handle_event(display.menu_opener_window, display.menu_opener_gadget, EVENT_MENU_CLOSED);
    display.menu_opener_window = NULL;
    display.menu_opener_gadget = NULL;
    display.popup_window = NULL;
}

void contraption_stack_window(unsigned win_id)
{
    struct CWindowInternal * stacked_win = contraption_find_window(win_id);
    int win_cursor = contraption_window_offset(stacked_win);
    for (int q = window_stack_count; q > 0; --q)
    {
        window_stack[q] = window_stack[q - 1];
    }

    window_stack[0] = win_cursor;
    window_stack_count++;

    contraption_render_window(stacked_win);
}

void contraption_stack_menu(unsigned int menu_id, struct CWindowInternal * window, struct CGadgetInternal * gadget)
{
    assert(display.menu_opener_window == NULL);
    assert(display.menu_opener_gadget == NULL);
    contraption_stack_window(menu_id);
    display.popup_window = contraption_find_window(menu_id);
    display.menu_opener_window = window;
    display.menu_opener_gadget = gadget;
    assert(display.popup_window->id == menu_id);
}


void contraption_unstack_window(unsigned win_id)
{
    struct CWindowInternal * window = contraption_find_window(win_id);
    int win_offs = contraption_window_offset(window);
    int current_position = -1;

    assert(win_offs >= 0);
    assert(win_offs < last_window);

    struct CExtent window_extents = contraption_window_extents(window);

    for (int q = 0; q < last_window; ++q)
    {
        if (window_stack[q] == win_offs)
        {
            current_position = q;
            break;
        }
    }

    if (current_position < 0)
    {
        return;
    }

    for (int q = current_position; q < window_stack_count; ++q)
    {
        window_stack[q] = window_stack[q + 1];
    }

    if (current_position == 0)
    {
        contraption_swap_menu(contraption_active_window()->menu_window_id);
    }
    window_stack_count--;
    contraption_render_damage(&window_extents);
}

void internal_raise_window(const struct CWindowInternal * win)
{
    if (win == NULL)
    {
        return;
    }

    int win_offs = win - &windows[0];

    int current_position = -1;

    if (!win->is_menu)
    {
        contraption_swap_menu(win->menu_window_id);
    }

    assert(win_offs >= 0);
    assert(win_offs < last_window);

    for (int q = 0; q < last_window; ++q)
    {
        if (window_stack[q] == win_offs)
        {
            current_position = q;
            break;
        }
    }

    if (current_position < 1)
    {
        return;
    }

    for (int q = current_position; q > 0; --q)
    {
        window_stack[q] = window_stack[q - 1];
    }

    window_stack[0] = win_offs;
    struct CExtent win_extents = contraption_window_extents(win);
    contraption_render_damage(&win_extents);

}

void contraption_window_move(struct CWindowInternal * window, unsigned col, unsigned row)
{
    if (window == NULL)
    {
        return;
    }

    window->properties.top = row;
    window->properties.left = col;
}

void contraption_window_resize(struct CWindowInternal * window, unsigned width, unsigned height)
{
    if (window == NULL)
    {
        return;
    }

    window->properties.width = width;
    window->properties.height = height;

}

struct CWindowInternal * contraption_active_window()
{
    return contraption_window_at_offset(0);
}
