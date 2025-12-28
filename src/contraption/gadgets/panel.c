#include <server.h>
#include <render.h>
#include "panel.h"
#include <resources.h>

void render_panel(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);
    contraption_render_background(&extents, &window_background, window_pixmap);
}


