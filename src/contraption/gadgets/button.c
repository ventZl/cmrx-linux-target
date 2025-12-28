#include <server.h>
#include <render.h>
#include "button.h"
#include <resources.h>


void render_button(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);
    contraption_render_frame(&extents, BUTTON_RADIUS, 0xE3E3E3FF, 0x9FABBCFF);
    contraption_render_text(&extents, gadget->properties.text, gadget->properties.text_flags, gadget->properties.fg_color, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}

void pointer_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta)
{

}

void button_down_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{

}

void button_up_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{

}


