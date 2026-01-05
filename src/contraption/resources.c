#include "resources.h"

#include "img/red_pill.xpm"
#include "img/yellow_pill.xpm"
#include "img/green_pill.xpm"
#include "img/silver_pill.xpm"


struct FBRectangle desktop_tile = { 0, 0, BACKGROUND_SIZE, BACKGROUND_SIZE };
uint32_t desktop_pixmap[BACKGROUND_SIZE * BACKGROUND_SIZE];

struct FBRectangle white_background = { 0, 0, 1, 1 };
uint32_t white_pixmap[1] = { 0xFFFFFFFF };

struct FBRectangle black_background = { 0, 0, 1, 1 };
uint32_t black_pixmap[1] = { 0x0FF };

struct FBRectangle window_background = { 0, 0, WINDOW_SIZE, WINDOW_SIZE };
uint32_t window_pixmap[WINDOW_SIZE * WINDOW_SIZE];

struct FBRectangle title_background = {0, 0, TITLE_WIDTH, TITLE_HEIGHT };
uint32_t title_pixmap[TITLE_WIDTH * TITLE_HEIGHT];

struct FBRectangle selected_background = {0, 0, 1, 1};
uint32_t selected_pixmap[1] = { 0x2457f0FF };

struct FBRectangle button_background = { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
uint32_t button_pixmap[BUTTON_WIDTH * BUTTON_HEIGHT];

struct Pixmap red_pill_pixmap;
struct Pixmap green_pill_pixmap;
struct Pixmap yellow_pill_pixmap;
struct Pixmap silver_pill_pixmap;

void contraption_resources_init()
{
    red_pill_pixmap = xpm_to_pixmap(red_pill_xpm);
    green_pill_pixmap = xpm_to_pixmap(green_pill_xpm);
    yellow_pill_pixmap = xpm_to_pixmap(yellow_pill_xpm);
    silver_pill_pixmap = xpm_to_pixmap(silver_pill_xpm);
}
