#include <server.h>
#include <render.h>
#include "button.h"
#include <resources.h>
#include <client.h>
#include <gadget.h>

#define COLOR_HIGHLIGHT 0xE3E3E3FF
#define COLOR_SHADE 0x9FABBCFF

void render_button(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents, text_extents;
    contraption_calc_extent(window, gadget, &extents);
    text_extents = extents;
    uint32_t hi_color = COLOR_HIGHLIGHT, lo_color = COLOR_SHADE;
    if (gadget->properties.state & GADGET_STATE_ACTIVATED)
    {
        hi_color = COLOR_SHADE;
        lo_color = COLOR_HIGHLIGHT;
        // Simulate pressing the button by moving text one px right and down
        text_extents.top += 2;
        text_extents.left += 2;
    }
    contraption_render_background(&extents, &button_background, button_pixmap);
    contraption_render_frame(&extents, BUTTON_RADIUS, hi_color, lo_color);
    contraption_render_text(&extents, gadget->properties.text, gadget->properties.text_flags, gadget->properties.fg_color, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}

void pointer_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta)
{

}

void button_down_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget->properties.flags & GADGET_FLAG_ENABLED)
    {
        if ((gadget->properties.state & GADGET_STATE_ACTIVATED) == 0)
        {
            gadget->properties.state |= GADGET_STATE_ACTIVATED;
            render_button(window, gadget);
        }
    }
}

void button_up_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget->properties.flags & GADGET_FLAG_ENABLED)
    {
        if (gadget->properties.event_id != EVENT_NONE)
        {
            contraption_send_event(window->owner_thread, gadget->properties.event_id, window->id);
        }
        gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
        render_button(window, gadget);
    }
}


