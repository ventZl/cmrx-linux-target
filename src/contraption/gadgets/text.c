#include <server.h>
#include <render.h>
#include "button.h"
#include <resources.h>
#include "client.h"


void render_text(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);

    if (gadget->properties.state & GADGET_STATE_ACTIVATED)
    {
        contraption_render_background(&extents, &selected_background, selected_pixmap);
    }

    contraption_render_text(&extents, gadget->properties.text, gadget->properties.text_flags, gadget->properties.fg_color, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}

void text_down_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget->properties.flags & GADGET_FLAG_ENABLED && gadget->properties.event_id != EVENT_NONE)
    {
        if ((gadget->properties.state & GADGET_STATE_ACTIVATED) == 0)
        {
            gadget->properties.state |= GADGET_STATE_ACTIVATED;
            render_text(window, gadget);
        }
    }
}

void text_up_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget->properties.flags & GADGET_FLAG_ENABLED)
    {
        if (gadget->properties.event_id != EVENT_NONE)
        {
            contraption_send_event(window->owner_thread, gadget->properties.event_id, window->id);
        }
        gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
        render_text(window, gadget);
    }
}
