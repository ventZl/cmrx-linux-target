#include "gadget.h"
#include "gadgets/titlebar.h"
#include "gadgets/text.h"
#include "gadgets/button.h"
#include "gadgets/menuitem.h"
#include "gadgets/panel.h"
#include "gadgets/pixmap.h"

#include <stddef.h>

static const struct GadgetType gadget_types[GADGET_KNOWN_TYPES] = {
    [GADGET_TITLE_BAR] = {
        render_titlebar,
        pointer_titlebar,
        button_down_titlebar,
        button_up_titlebar,
        NULL,
        NULL,
    },
    [GADGET_BUTTON] = {
        render_button,
        pointer_button,
        button_down_button,
        button_up_button,
        NULL,
        NULL,
    },
    [GADGET_TEXT] = {
        render_text,
        NULL,
        text_down_button,
        text_up_button,
        NULL,
        NULL,
    },
    [GADGET_MENU_ITEM] = {
        render_menuitem,
        pointer_menuitem,
        button_down_menuitem,
        button_up_menuitem,
        enter_leave_menuitem,
        event_menuitem,
    },
    [GADGET_PANEL] = {
        render_panel,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
    [GADGET_PIXMAP] = {
        render_pixmap,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    }
};

void gadget_handle_render(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    if (gadget_types[gadget->properties.type].render_callback != NULL)
    {
        gadget_types[gadget->properties.type].render_callback(window, gadget);
    }
}


void gadget_handle_pointer(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta)
{
    if (gadget_types[gadget->properties.type].mouse_move_callback != NULL)
    {
        gadget_types[gadget->properties.type].mouse_move_callback(window, gadget, rel_pos, delta);
    }
}

void gadget_handle_button_down(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget_types[gadget->properties.type].mouse_button_down_callback != NULL)
    {
        gadget_types[gadget->properties.type].mouse_button_down_callback(window, gadget, buttons);
    }
}

void gadget_handle_button_up(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget_types[gadget->properties.type].mouse_button_up_callback != NULL)
    {
        gadget_types[gadget->properties.type].mouse_button_up_callback(window, gadget, buttons);
    }
}

void gadget_handle_enter_leave(struct CWindowInternal * window, struct CGadgetInternal * gadget, bool enter)
{
    if (gadget_types[gadget->properties.type].mouse_enter_leave_callback != NULL)
    {
        gadget_types[gadget->properties.type].mouse_enter_leave_callback(window, gadget, enter);
    }
}

void gadget_handle_event(struct CWindowInternal * window, struct CGadgetInternal * gadget, unsigned event)
{
    if (gadget_types[gadget->properties.type].event_callback != NULL)
    {
        gadget_types[gadget->properties.type].event_callback(window, gadget, event);
    }

}
