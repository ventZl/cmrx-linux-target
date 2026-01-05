#include "pixmap.h"
#include <cmrx/ipc/thread.h>

#include <stdio.h>
#include <assert.h>

#include <string.h>

#define TILES_MAX 1024
#define PIXMAPS_MAX 32

struct Tile tiles[TILES_MAX];
struct PixmapInternal pixmaps[PIXMAPS_MAX];

uint32_t tile_bitmap[TILES_MAX / sizeof(uint32_t)];

void contraption_pixmap_init()
{
    for (unsigned q = 0; q < sizeof(tile_bitmap)/sizeof(tile_bitmap[0]); ++q)
    {
        tile_bitmap[q] = 0;
    }

    for (unsigned q = 0; q < PIXMAPS_MAX; ++q)
    {
        pixmaps[q].owning_thread = ~0;
    }
}

static int contraption_alloc_tile()
{
    for (unsigned q = 0; q < sizeof(tile_bitmap)/sizeof(tile_bitmap[0]); ++q)
    {
        if (tile_bitmap[q] != ~0)
        {
            for (unsigned w = 0; w < 32; ++w)
            {
                if ((tile_bitmap[q] & (1 << w)) == 0)
                {
                    tile_bitmap[q] |= 1 << w;
                    return (q * sizeof(tile_bitmap[0]) * 8) + w;
                }
            }
        }
    }

    return TILE_NONE;
}

static void contraption_free_tile(unsigned tile)
{
    if (tile < 0 || tile >= TILES_MAX)
    {
        return;
    }
    unsigned q = tile / (sizeof(tile_bitmap[0]) * 8);
    unsigned w = tile % (sizeof(tile_bitmap[0]) * 8);
    tile_bitmap[q] &= ~(1 << w);
}

int contraption_pixmap_open(const struct CPixmap* pixmap)
{
    for (unsigned q = 0; q < PIXMAPS_MAX; ++q)
    {
        if (pixmaps[q].owning_thread == ~0)
        {
            pixmaps[q].owning_thread = get_tid();
            pixmaps[q].width = pixmap->width;
            pixmaps[q].height = pixmap->height;

            struct Tile * last_tile = NULL;
            // Allocate tiles
            for (int col = 0; col < pixmap->width; col += TILE_SIZE)
            {
                for (int row = 0; row < pixmap->height; row += TILE_SIZE)
                {
                    int tile_id = contraption_alloc_tile();
                    if (last_tile == NULL)
                    {
                        pixmaps[q].first_tile = tile_id;
                    }
                    else
                    {
                        last_tile->next = tile_id;
                    }
                    last_tile = &tiles[tile_id];
                }
            }
            last_tile->next = TILE_NONE;
            return q;
        }
    }
    return PIXMAP_NONE;
}

static void contraption_tile_data(struct Tile * tile, const struct PixmapInternal * pixmap, int left, int top, const uint32_t * data)
{
    memset(tile->pixmap, 0, TILE_SIZE * TILE_SIZE * sizeof(data[0]));
    for (int row = 0; row < TILE_SIZE; ++row)
    {
        for (int col = 0; col < TILE_SIZE; ++col)
        {
            if (((top + row) < pixmap->height) && ((left + col) < pixmap->width))
            {
                tile->pixmap[row * TILE_SIZE + col] = data[(top + row) * pixmap->width + (col + left)];
            }
        }
    }

}

int contraption_pixmap_data(int pixmap_id, const uint32_t* data)
{
    if (pixmap_id < 0 || pixmap_id >= PIXMAPS_MAX)
    {
        assert(0);
        return 0;
    }

    if (pixmaps[pixmap_id].owning_thread != get_tid())
    {
        assert(0);
        return 0;
    }

    struct PixmapInternal * pixmap = &pixmaps[pixmap_id];

    struct Tile * current_tile = contraption_pixmap_get_tile(pixmap->first_tile);

    for (int row = 0; row < pixmap->height; row += TILE_SIZE)
    {
        for (int col = 0; col < pixmap->width; col += TILE_SIZE)
        {
            contraption_tile_data(current_tile, pixmap, col, row, data);
            current_tile = contraption_pixmap_get_tile(current_tile->next);
        }
    }

    return 1;
}

int contraption_pixmap_close(int pixmap_id)
{
    if (pixmap_id < 0 || pixmap_id >= PIXMAPS_MAX)
    {
        return 0;
    }

    if (pixmaps[pixmap_id].owning_thread != get_tid())
    {
        return 0;
    }

    pixmaps[pixmap_id].owning_thread = ~0;
    int last_tile_id = pixmaps[pixmap_id].first_tile;
    struct Tile * last_tile = (last_tile_id != TILE_NONE ? &tiles[last_tile_id] : NULL);
    int next_tile_id = (last_tile != NULL ? last_tile->next : TILE_NONE);
    struct Tile * next_tile = (next_tile_id != TILE_NONE ? &tiles[next_tile_id] : NULL);

    do {
        if (last_tile != NULL)
        {
            contraption_free_tile(last_tile_id);
            last_tile->next = TILE_NONE;
        }
        last_tile_id = next_tile_id;
        last_tile = next_tile;
        next_tile_id = (last_tile != NULL ? last_tile->next : TILE_NONE);
        next_tile = (next_tile_id != TILE_NONE ? &tiles[next_tile_id] : NULL);
    } while (last_tile != NULL);

    return 1;
}

struct PixmapInternal * contraption_pixmap_get(int pixmap_id)
{
    if (pixmap_id < 0 || pixmap_id >= PIXMAPS_MAX)
    {
        return NULL;
    }

    if (pixmaps[pixmap_id].owning_thread == ~0)
    {
        return NULL;
    }

    return &pixmaps[pixmap_id];
}

struct Tile * contraption_pixmap_get_tile(int tile_id)
{
    if (tile_id < 0 || tile_id >= TILES_MAX)
    {
        return NULL;
    }

    return &tiles[tile_id];
}
