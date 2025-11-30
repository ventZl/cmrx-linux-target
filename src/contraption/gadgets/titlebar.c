#include <server.h>
#include "titlebar.h"
#include <resources.h>

void render_titlebar(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);
    contraption_render_background(&extents, &title_background, title_pixmap);
    contraption_render_text(&extents, window->properties.title, TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE | TEXT_BOLD, 0, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}

void pointer_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta)
{
    if (gadget->properties.state & GADGET_STATE_ACTIVATED)
    {
        window->properties.top += delta->top;
        window->properties.left += delta->left;
        contraption_render_window(window);
        display.render = true;
    }
}

void button_down_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    display.cursor_gadget->properties.state |= GADGET_STATE_ACTIVATED;
    display.cursor_gadget_grab = true;

}

void button_up_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    display.cursor_gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
    display.cursor_gadget_grab = false;

}


