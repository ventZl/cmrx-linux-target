#pragma once

#include "../fbdev/fbdev.h"
#include "contraption.h"

#define TILE_SIZE 32

#define TILE_NONE -1

struct Tile {
    uint32_t pixmap[TILE_SIZE * TILE_SIZE];
    int next;
};

struct PixmapInternal {
    uint16_t width;
    uint16_t height;
    uint32_t first_tile;
    unsigned owning_thread;
};

void contraption_pixmap_init();
int contraption_pixmap_open(const struct CPixmap * pixmap);
int contraption_pixmap_data(int pixmap_id, const uint32_t* data);
int contraption_pixmap_close(int pixmap_id);
struct PixmapInternal * contraption_pixmap_get(int pixmap_id);
struct Tile * contraption_pixmap_get_tile(int tile_id);
