#include <server.h>
#include <render.h>
#include "panel.h"
#include <resources.h>

void render_panel(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);
    struct FBRectangle * rect = &window_background;
    struct FBRectangle one_by_one = { 0, 0, 1, 1 };
    uint32_t byte = 0x123456;
    uint32_t * texture = window_pixmap;

    switch (gadget->properties.fg_color) {
        case BACKGROUND_DESKTOP:
            rect = &desktop_tile;
            texture = desktop_pixmap;
            break;

        case BACKGROUND_WHITE:
            rect = &white_background;
            texture = white_pixmap;
            break;

        case BACKGROUND_BLACK:
            rect = &black_background;
            texture = black_pixmap;
            break;

        case BACKGROUND_DOCK:
            rect = &one_by_one;
            byte = 0x8b99beff;
            texture = &byte;
            break;

        case BACKGROUND_WINDOW:
            // Do nothing
            break;

        default:
            rect = &one_by_one;
            byte = (gadget->properties.fg_color << 8) | 0xFF;
            texture = &byte;
    }
    contraption_render_background(&extents, rect, texture);
}


