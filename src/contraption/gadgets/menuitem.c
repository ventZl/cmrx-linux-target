#include <server.h>
#include <render.h>
#include "button.h"
#include <gadget.h>
#include <resources.h>
#include "window.h"
#include <client.h>

#include <stdio.h>
#include <assert.h>

void render_menuitem(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);

    uint32_t color = 0x777777;
    if (gadget->properties.flags & GADGET_FLAG_ENABLED)
    {
        if (gadget->properties.state & (GADGET_STATE_ACTIVATED | GADGET_STATE_HOVERED))
        {
            color = 0xFFFFFF;
        }
        else
        {
            color = 0;
        }
    }
    if (gadget->properties.state & (GADGET_STATE_ACTIVATED | GADGET_STATE_HOVERED))
    {
        contraption_render_background(&extents, &selected_background, selected_pixmap);
    }

    contraption_render_text(&extents, gadget->properties.text, gadget->properties.text_flags, color, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}

void pointer_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta)
{

}

void button_down_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{

}

void button_up_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget->properties.flags & GADGET_FLAG_ENABLED)
    {
        // Has submenu
        if (gadget->properties.sub_menu_id != WINDOW_NONE)
        {

            if ((gadget->properties.state & GADGET_STATE_ACTIVATED) == 0)
            {
                if (gadget->properties.sub_menu_id != WINDOW_NONE)
                {
                    contraption_stack_menu(gadget->properties.sub_menu_id, window, gadget);
                    gadget->properties.state |= GADGET_STATE_ACTIVATED;
                }
            }
            else
            {
                if (display.popup_window != NULL)
                {
                    contraption_hide_menu();
                }
            }
        }
        if (gadget->properties.event_id != EVENT_NONE)
        {
            contraption_send_event(window->owner_thread, gadget->properties.event_id, window->id);
        }
    }
}

void enter_leave_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, bool enter)
{
    if (gadget->properties.flags & GADGET_FLAG_ENABLED)
    {
        if (enter)
        {
            gadget->properties.state |= GADGET_STATE_HOVERED;
        }
        else
        {
            gadget->properties.state &= ~GADGET_STATE_HOVERED;
        }
        contraption_render_window(window);
    }
}

void event_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, unsigned event)
{
    if (event == EVENT_MENU_CLOSED)
    {
        gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
    }
}
