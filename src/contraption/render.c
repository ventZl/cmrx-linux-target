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

/**
 * Subtracts subtrahend rectangle from minuend rectangle and populates subregion array.
 *
 * @param minuend outer area subtracted from
 * @param subtrahend inner area being subtracted
 * @param out array of size 4 that will hold rectangular subregions after subtraction
 * @returns count of regions that were created by subtraction. If subtracted area does not intersect with subtrahend area, the return
 * value will be -1.
 */
int contraption_subtract_rect(const struct FBRectangle * minuend, const struct FBRectangle * subtrahend, struct FBRectangle * out) {
    // Calculate bounds of the minuend rectangle
    unsigned m_left = minuend->col;
    unsigned m_right = minuend->col + minuend->width;
    unsigned m_top = minuend->row;
    unsigned m_bottom = minuend->row + minuend->height;

    // Calculate bounds of the subtrahend rectangle
    unsigned s_left = subtrahend->col;
    unsigned s_right = subtrahend->col + subtrahend->width;
    unsigned s_top = subtrahend->row;
    unsigned s_bottom = subtrahend->row + subtrahend->height;

    // Calculate intersection bounds
    unsigned i_left = (m_left > s_left) ? m_left : s_left;
    unsigned i_right = (m_right < s_right) ? m_right : s_right;
    unsigned i_top = (m_top > s_top) ? m_top : s_top;
    unsigned i_bottom = (m_bottom < s_bottom) ? m_bottom : s_bottom;

    unsigned idx = 0;

    // Check if there's an intersection
    if (i_left >= i_right || i_top >= i_bottom) {
        // No intersection, return the original rectangle
//        out[idx++] = minuend;
        return -1;
    }

    // Top rectangle (above the intersection)
    if (m_top < i_top) {
        struct FBRectangle top = {
            .col = m_left,
            .row = m_top,
            .width = minuend->width,
            .height = i_top - m_top
        };
        out[idx++] = top;
    }

    // Bottom rectangle (below the intersection)
    if (i_bottom < m_bottom) {
        struct FBRectangle bottom = {
            .col = m_left,
            .row = i_bottom,
            .width = minuend->width,
            .height = m_bottom - i_bottom
        };
        out[idx++] = bottom;
    }

    // Left rectangle (to the left of intersection, in the middle vertical section)
    if (m_left < i_left) {
        struct FBRectangle left = {
            .col = m_left,
            .row = i_top,
            .width = i_left - m_left,
            .height = i_bottom - i_top
        };
        out[idx++] = left;
    }

    // Right rectangle (to the right of intersection, in the middle vertical section)
    if (i_right < m_right) {
        struct FBRectangle right = {
            .col = i_right,
            .row = i_top,
            .width = m_right - i_right,
            .height = i_bottom - i_top
        };
        out[idx++] = right;
    }

    return idx;
}

void contraption_render_culled(struct FBRectangle * area_to_render, unsigned level)
{
    int subcount;
    do {
        if (level >= contraption_window_count())
        {
            // Nothing more to render
            return;
        }

        struct CWindowInternal * win = contraption_window_at_offset(level);
        struct FBRectangle win_extents = { .col = win->properties.left, .row = win->properties.top, .width = win->properties.width, .height = win->properties.height };
        struct FBRectangle subareas[4];
        subcount = contraption_subtract_rect(area_to_render, &win_extents, subareas);

        if (subcount >= 0)
        {
            rpc_call(&fbdev, cull, area_to_render);
            // Window at this level in stack falls into culling rectangle, there is something to render
            contraption_render_window(win);
        }
        else
        {
            // Window is completely outside the region, skip to next window
            level++;
            continue;
        }

        // Render subregions
        for (int q = 0; q < subcount; ++q)
        {
            contraption_render_culled(&subareas[q], level + 1);
        }
    } while (subcount == -1);
}

void contraption_render()
{
    struct FBRectangle desktop = { .col = 0, .row = 0, .width = 1280, .height = 800 };

    contraption_render_culled(&desktop, 0);
}

void contraption_render_damage(struct CExtent * area)
{

}

