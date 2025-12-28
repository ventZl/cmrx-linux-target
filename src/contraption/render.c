#include "render.h"
#include "resources.h"
#include "server.h"
#include "gadget.h"
#include "window.h"

void contraption_render_frame(const struct CExtent * extents, unsigned radius, uint32_t color_ul, uint32_t color_br)
{
    struct FBPosition top_left = { extents->left, extents->top};
    struct FBPosition top_right = { extents->right, extents->top};

    struct FBPosition left_up = { extents->left, extents->top};
    struct FBPosition left_down = { extents->left, extents->bottom};

    struct FBPosition right_up = { extents->right, extents->top};
    struct FBPosition right_down = { extents->right, extents->bottom };

    struct FBPosition bottom_left = { extents->left, extents->bottom };
    struct FBPosition bottom_right = { extents->right, extents->bottom };

    if (radius > 0)
    {
        top_left.col += BUTTON_RADIUS;
        bottom_left.col += BUTTON_RADIUS;

        top_right.col -= BUTTON_RADIUS;
        bottom_right.col -= BUTTON_RADIUS;

        left_up.row += BUTTON_RADIUS;
        right_up.row += BUTTON_RADIUS;

        left_down.row -= BUTTON_RADIUS;
        right_down.row -= BUTTON_RADIUS;

        struct FBPosition ul_corner = { extents->left + BUTTON_RADIUS, extents->top + BUTTON_RADIUS };
        struct FBPosition ur_corner = { extents->right - BUTTON_RADIUS, extents->top + BUTTON_RADIUS };
        struct FBPosition bl_corner = { extents->left + BUTTON_RADIUS, extents->bottom - BUTTON_RADIUS };
        struct FBPosition br_corner = { extents->right - BUTTON_RADIUS, extents->bottom - BUTTON_RADIUS };

        rpc_call(&fbdev, arc, &ul_corner, radius, FBDEV_QUADRANT_NW, color_ul);
        rpc_call(&fbdev, arc, &ur_corner, radius, FBDEV_QUADRANT_NE, color_br);
        rpc_call(&fbdev, arc, &bl_corner, radius, FBDEV_QUADRANT_SW, color_ul);
        rpc_call(&fbdev, arc, &br_corner, radius, FBDEV_QUADRANT_SE, color_br);
    }

    rpc_call(&fbdev, line, &top_left, &top_right, color_ul);
    rpc_call(&fbdev, line, &right_up, &right_down, color_br);
    rpc_call(&fbdev, line, &bottom_left, &bottom_right, color_br);
    rpc_call(&fbdev, line, &left_up, &left_down, color_ul);

}

void contraption_render_background(const struct CExtent * extents, const struct FBRectangle * coord, uint32_t * background)
{
    struct FBRectangle dest = {extents->left, extents->top, extents->right - extents->left, extents->bottom - extents->top };
    rpc_call(&fbdev, blit, &dest, coord, background);
}

void contraption_render_text(const struct CExtent * extents, const char * text, uint32_t flags, uint32_t rgb_color, uint8_t margin_horiz, uint8_t margin_vert)
{
    unsigned top = extents->top, left = extents->left;
    FBTextMetrics metrics;
    rpc_call(&fbdev, text_measure, text, &metrics);

    if (flags & TEXT_ALIGN_LEFT)
    {
        left += margin_horiz;
    }
    if (flags & TEXT_ALIGN_CENTER)
    {
        left += ((extents->right - extents->left) - metrics.width) / 2;
    }
    if (flags & TEXT_ALIGN_RIGHT)
    {
        left = extents->right - metrics.width - margin_horiz;
    }
    if (flags & TEXT_ALIGN_TOP)
    {
        top += margin_vert;
    }
    if (flags & TEXT_ALIGN_MIDDLE)
    {
        top += ((extents->bottom - extents->top) - (metrics.y_maximum - metrics.y_minimum)) / 2;
    }
    if (flags & TEXT_ALIGN_BOTTOM)
    {
        top = extents->bottom - metrics.y_maximum - margin_vert;
    }
    rpc_call(&fbdev, text, text, left, top, rgb_color);
}

void contraption_render_window(struct CWindowInternal * window)
{
    struct FBRectangle dest = { window->properties.left, window->properties.top, window->properties.width, window->properties.height };

    struct FBRectangle * rect = &window_background;
    uint32_t * text = window_pixmap;

    switch (window->properties.background) {
        case BACKGROUND_DESKTOP:
            rect = &background;
            text = background_pixmap;
            break;

        case BACKGROUND_WHITE:
            rect = &white_background;
            text = white_pixmap;
            break;

        case BACKGROUND_BLACK:
            rect = &black_background;
            text = black_pixmap;
            break;

        default:
            // Do nothing
    }
    rpc_call(&fbdev, blit, &dest, rect, text);

    if (window->properties.flags & WINDOW_FLAG_BORDER)
    {
        struct FBPosition up_left = { window->properties.left, window->properties.top };
        struct FBPosition up_right = { window->properties.left + window->properties.width - 1, window->properties.top };
        struct FBPosition bottom_left = { window->properties.left, window->properties.top + window->properties.height - 1 };
        struct FBPosition bottom_right = { window->properties.left + window->properties.width - 1, window->properties.top + window->properties.height - 1 };

        validate_coordinate(up_left.col, up_left.row);
        validate_coordinate(up_right.col, up_right.row);
        validate_coordinate(bottom_left.col, bottom_left.row);
        validate_coordinate(bottom_right.col, bottom_right.row);

        rpc_call(&fbdev, line, &up_left, &up_right, 0xE3E3E3FF);
        rpc_call(&fbdev, line, &up_left, &bottom_left, 0x9FABBCFF);
        rpc_call(&fbdev, line, &bottom_left, &bottom_right, 0x9FABBCFF);
        rpc_call(&fbdev, line, &up_right, &bottom_right, 0x9FABBCFF);
    }

    struct CGadgetInternal * gadgets = (struct CGadgetInternal *) window->properties.gadgets;
    for (int q = 0; q < window->properties.gadget_count; ++q)
    {
        gadget_handle_render(window, &gadgets[q]);
    }
}

void contraption_render()
{
    /*    struct FBRectangle dest = { 0, 0, 1280, 800 };
     *   rpc_call(&fbdev, blit, &dest, &background, background_pixmap, NULL);
     *   dest.height = 20;
     *   rpc_call(&fbdev, blit, &dest, &window_background, window_pixmap, NULL);*/

    for (unsigned q = contraption_window_count(); q > 0; --q)
    {
        contraption_render_window(contraption_window_at_offset(q - 1));
    }
}

void contraption_render_damage(struct CExtent * area)
{

}

