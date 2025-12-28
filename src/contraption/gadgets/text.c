#include <server.h>
#include <render.h>
#include "button.h"
#include <resources.h>


void render_text(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);

    contraption_render_text(&extents, gadget->properties.text, gadget->properties.text_flags, gadget->properties.fg_color, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}


