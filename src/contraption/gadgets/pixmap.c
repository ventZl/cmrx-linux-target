#include <server.h>
#include <render.h>
#include "pixmap.h"
#include <resources.h>
#include <pixmap.h>
#include <assert.h>

static const struct FBRectangle tile_rect = { 0, 0, TILE_SIZE, TILE_SIZE };


void render_pixmap(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct PixmapInternal * pixmap = contraption_pixmap_get(gadget->properties.pixmap_id);
    if (pixmap == NULL)
    {
        return;
    }

    struct Tile * tile = contraption_pixmap_get_tile(pixmap->first_tile);

    for (int row = 0; row < pixmap->height; row += TILE_SIZE)
    {
        for (int col = 0; col < pixmap->width; col += TILE_SIZE)
        {
            struct FBRectangle tgt_rect = {
                .row = window->properties.top + gadget->properties.top + row,
                .col = window->properties.left + gadget->properties.left + col,
                .height = TILE_SIZE,
                .width = TILE_SIZE
            };
            rpc_call(&fbdev, blit, &tgt_rect, &tile_rect, tile->pixmap);
            tile = contraption_pixmap_get_tile(tile->next);
        }
    }
}



