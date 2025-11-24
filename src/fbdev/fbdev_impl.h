#pragma once

#include <SDL3/SDL.h>
#include <threads.h>
#include <stdint.h>

// SDL window dimensions = framebuffer screen dimensions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

struct FBDevImpl {
    bool running;
    thrd_t sdl_main_thread;
    mtx_t sdl_blit_mutex;
    /* We will use this renderer to draw into this window every frame. */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t fb[WINDOW_HEIGHT * WINDOW_WIDTH * 4];         // crude, but works
    bool blit_dirty;
};

void fbdev_blend(uint32_t * fb, unsigned col, unsigned row, uint32_t rgba);

extern const struct FBDevVTable fbdev_vtable;
