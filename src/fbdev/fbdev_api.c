#include "fbdev.h"
#include "fbdev_impl.h"

#include <cmrx/application.h>
#include <cmrx/rpc/implementation.h>

#include "renderer/text.h"
#include <fonts/NotoSans14.h>
#include <fonts/NotoSans14b.h>

#include <assert.h>

IMPLEMENTATION_OF(struct FBDev, struct FBDevVTable);

unsigned pixel_offset(unsigned col, unsigned row) {
    return (col * WINDOW_WIDTH + row) * 4;
}

static void fbdev_blit(INSTANCE(this), const struct FBRectangle * destination, const struct FBRectangle * texture, uint32_t * buffer, const struct FBRectangle * cull)
{
    mtx_lock(&this->impl->sdl_blit_mutex);

    uint32_t * fb = this->impl->fb;
    uint32_t top = 0, left = 0, width = destination->width, height = destination->height;
    if (cull != NULL)
    {
        if (cull->col > destination->col)
        {
            left = cull->col - destination->col;
        }
        if (cull->row > destination->row)
        {
            top = cull->row - destination->row;
        }

        if ((cull->row + cull->height) < (destination->row + destination->height))
        {
            height = cull->row + cull->height - destination->row;
        }

        if ((cull->col + cull->width) < (destination->col + destination->width))
        {
            width = cull->col + cull->width - destination->col;
        }
    }
    for (unsigned row = top; row < height; ++row) {
        for (unsigned col = left; col < width; ++col) {
            assert(col < WINDOW_WIDTH && row < WINDOW_HEIGHT);
            if (col < WINDOW_WIDTH && row < WINDOW_HEIGHT)
            {
                fb[(row + destination->row) * WINDOW_WIDTH + (col + destination->col)] = buffer[(row % texture->height) * texture->width + (col % texture->width)];
            }
        }
    }
    this->impl->blit_dirty = true;
    mtx_unlock(&this->impl->sdl_blit_mutex);

}

long labs(long x)
{
    if (x < 0)
        return -x;

    return x;
}

static void fbdev_line(INSTANCE(this), const struct FBPosition * from, const struct FBPosition * to, uint32_t color)
{
    mtx_lock(&this->impl->sdl_blit_mutex);

    uint32_t * fb = this->impl->fb;

    long col0 = from->col;
    long row0 = from->row;
    long dx = labs((long) to->col - (long) from->col);
    long sx = from->col < to->col ? 1 : -1;
    long dy = -labs((long) to->row - (long) from->row);
    long sy = from->row < to->row ? 1 : -1;
    long error = dx + dy;

    while (true) {
        assert(0 <= col0 &&  col0 < WINDOW_WIDTH);
        assert(0 <= row0 &&  row0 < WINDOW_HEIGHT);
        fb[(row0) * WINDOW_WIDTH + col0] = color;
        long e2 = 2 * error;
        if (e2 >= dy) {
            if (col0 == to->col) break;
            error = error + dy;
            col0 = col0 + sx;
        }
        if (e2 <= dx) {
            if (row0 == to->row) break;
            error = error + dx;
            row0 = row0 + sy;
        }
    }

    this->impl->blit_dirty = true;
    mtx_unlock(&this->impl->sdl_blit_mutex);

}

void circleBres(int xc, int yc, int r, void (*callback)(int, int, int, int, uint32_t, uint8_t, uint32_t *), uint32_t color, uint8_t quadrant, uint32_t * fb){
    int x = 0, y = r;
    int d = 3 - 2 * r;
    callback(xc, yc, x, y, color, quadrant, fb);
    while (y >= x)
    {
                if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }

        x++;

        // Draw the circle using the new coordinates
        callback(xc, yc, x, y, color, quadrant, fb);
    }
}

static void circ_outline(int xc, int yc, int x, int y, uint32_t color, uint8_t quadrant, uint32_t * fb)
{
    if (quadrant & FBDEV_QUADRANT_NE) { fb[(yc - y) * WINDOW_WIDTH + xc + x] = color; }
    if (quadrant & FBDEV_QUADRANT_SE) { fb[(yc + y) * WINDOW_WIDTH + xc + x] = color; }
    if (quadrant & FBDEV_QUADRANT_NW) { fb[(yc - y) * WINDOW_WIDTH + xc - x] = color; }
    if (quadrant & FBDEV_QUADRANT_SW) { fb[(yc + y) * WINDOW_WIDTH + xc - x] = color; }
}

static void circ_fill(int xc, int yc, int x, int y, uint32_t color, uint8_t quadrant, uint32_t * fb)
{
    if (quadrant & FBDEV_QUADRANT_NE) for (int q = 0; q < x; ++q) { fb[(yc - y) * WINDOW_WIDTH + xc + q] = color; }
    if (quadrant & FBDEV_QUADRANT_SE) for (int q = 0; q < x; ++q) { fb[(yc + y) * WINDOW_WIDTH + xc + q] = color; }
    if (quadrant & FBDEV_QUADRANT_NW) for (int q = 0; q < x; ++q) { fb[(yc - y) * WINDOW_WIDTH + xc - q] = color; }
    if (quadrant & FBDEV_QUADRANT_SW) for (int q = 0; q < x; ++q) { fb[(yc + y) * WINDOW_WIDTH + xc - q] = color; }

}

static void fbdev_arc(INSTANCE(this), const struct FBPosition * center, unsigned radius, uint8_t quadrant, uint32_t color)
{
    mtx_lock(&this->impl->sdl_blit_mutex);

    circleBres(center->col, center->row, radius, circ_outline, color, quadrant, this->impl->fb);

    this->impl->blit_dirty = true;
    mtx_unlock(&this->impl->sdl_blit_mutex);

}

static void fbdev_arc_fill(INSTANCE(this), const struct FBPosition * center, unsigned radius, uint8_t quadrant, uint32_t color)
{
    mtx_lock(&this->impl->sdl_blit_mutex);

    circleBres(center->col, center->row, radius, circ_fill, radius, quadrant, this->impl->fb);

    this->impl->blit_dirty = true;
    mtx_unlock(&this->impl->sdl_blit_mutex);

}

static void fbdev_text(INSTANCE(this), const char * text, unsigned x, unsigned y, uint32_t rgb)
{
    text_render(text, &noto_sans_14_b, false, x, y, this->impl->fb, rgb);
}

static void fbdev_text_measure(INSTANCE(this), const char * text, FBTextMetrics * metrics)
{
    FBTextMetrics own_metrics = text_measure(text, &noto_sans_14_b, false);
    *metrics = own_metrics;
}

VTABLE struct FBDevVTable fbdev_vtable = {
    fbdev_blit,
    fbdev_line,
    fbdev_arc,
    fbdev_arc_fill,
    fbdev_text,
    fbdev_text_measure
};
