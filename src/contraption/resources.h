#pragma once

#include "../fbdev/fbdev.h"
#include "contraption.h"
/* Resources used for rendering, such as textures and patterns */


#define BACKGROUND_SIZE 32
#define WINDOW_SIZE 32

#define TITLE_WIDTH 1
#define TITLE_HEIGHT 22

#define BUTTON_WIDTH 1
#define BUTTON_HEIGHT 30

struct Texture {
    struct FBRectangle size;
    uint32_t pixmap[];
};

extern struct FBRectangle background;
extern uint32_t background_pixmap[BACKGROUND_SIZE * BACKGROUND_SIZE];

extern struct FBRectangle white_background;
extern uint32_t white_pixmap[1];

extern struct FBRectangle black_background;
extern uint32_t black_pixmap[1];

extern struct FBRectangle window_background;
extern uint32_t window_pixmap[WINDOW_SIZE * WINDOW_SIZE];

extern struct FBRectangle title_background;
extern uint32_t title_pixmap[TITLE_WIDTH * TITLE_HEIGHT];

extern struct FBRectangle button_background;
extern uint32_t button_pixmap[BUTTON_WIDTH * BUTTON_HEIGHT];

extern struct FBRectangle selected_background;
extern uint32_t selected_pixmap[1];
