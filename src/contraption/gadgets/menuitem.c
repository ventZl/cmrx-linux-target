#include <server.h>
#include "button.h"
#include <gadget.h>
#include <resources.h>

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
                contraption_stack_window(gadget->properties.sub_menu_id);
                gadget->properties.state |= GADGET_STATE_ACTIVATED;
                display.popup_window = contraption_find_window(gadget->properties.sub_menu_id);
                assert(display.popup_window->id == gadget->properties.sub_menu_id);
            }
            else
            {
                if (display.popup_window != NULL)
                {
                    contraption_hide_menu();
                }
                gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
            }
            display.render = 1;
//            gadget_handle_render(window, gadget);
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
    }
    display.render = 1;
}

