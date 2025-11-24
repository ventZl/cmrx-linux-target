#include "resources.h"

struct FBRectangle background = { 0, 0, BACKGROUND_SIZE, BACKGROUND_SIZE };
uint32_t background_pixmap[BACKGROUND_SIZE * BACKGROUND_SIZE];

struct FBRectangle window_background = { 0, 0, WINDOW_SIZE, WINDOW_SIZE };
uint32_t window_pixmap[WINDOW_SIZE * WINDOW_SIZE];

struct FBRectangle title_background = {0, 0, TITLE_WIDTH, TITLE_HEIGHT };
uint32_t title_pixmap[TITLE_WIDTH * TITLE_HEIGHT];

struct FBRectangle selected_background = {0, 0, 1, 1};
uint32_t selected_pixmap[1] = { 0x2457f0FF };

