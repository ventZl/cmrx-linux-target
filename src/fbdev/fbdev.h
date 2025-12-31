#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <cmrx/ipc/rpc.h>
#include <cmrx/rpc/interface.h>

void fbdev_init();

enum FBQuadrant {
    FBDEV_QUADRANT_NE = 1,
    FBDEV_QUADRANT_SE = 2,
    FBDEV_QUADRANT_NW = 4,
    FBDEV_QUADRANT_SW = 8,
    FBDEV_HALF_EAST = FBDEV_QUADRANT_NE + FBDEV_QUADRANT_SE,
    FBDEV_HALF_WEST = FBDEV_QUADRANT_NW + FBDEV_QUADRANT_SW,
    FBDEV_HALF_SOUTH = FBDEV_QUADRANT_SW + FBDEV_QUADRANT_SE,
    FBDEV_HALF_NORTH = FBDEV_QUADRANT_NW + FBDEV_QUADRANT_NE,
    FBDEV_WHOLE = FBDEV_HALF_SOUTH + FBDEV_HALF_NORTH
};

struct FBPosition {
    unsigned col, row;
};

struct FBRectangle {
    unsigned col, row, width, height;
};

typedef struct {
    unsigned int cursor;
    unsigned int width;
    unsigned int y_minimum;
    unsigned int y_maximum;
} FBTextMetrics;

enum FBFont {
    FONT_NORMAL = 0,
    FONT_BOLD = 1 << 0,
    FONT_ITALIC = 1 << 1,
    FONT_MONO = 1 << 2
};

struct FBDevVTable {
    void (*blit)(INSTANCE(this), const struct FBRectangle * destination, const struct FBRectangle * texture, uint32_t * buffer);
    void (*line)(INSTANCE(this), const struct FBPosition * from, const struct FBPosition * to, uint32_t color);
    void (*arc)(INSTANCE(this), const struct FBPosition * center, unsigned radius, uint8_t quadrant, uint32_t color);
    void (*arc_fill)(INSTANCE(this), const struct FBPosition * center, unsigned radius, uint8_t quadrant, uint32_t color);
    void (*text)(INSTANCE(this), const char * text, unsigned x, unsigned y, uint32_t rgb);
    void (*text_measure)(INSTANCE(this), const char * text, FBTextMetrics * metrics);
    void (*cull)(INSTANCE(this), const struct FBRectangle * window);
    void (*set_font)(INSTANCE(this), unsigned font);
};

struct FBDevImpl;

struct text_font_t;

struct FBDev {
    const struct FBDevVTable * vtable;
    struct FBDevImpl * impl;
    bool do_cull;
    struct FBRectangle cull_area;
    const struct text_font_t * current_font;
};

extern struct FBDev fbdev;
