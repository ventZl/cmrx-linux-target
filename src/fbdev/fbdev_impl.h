#pragma once

#include <SDL3/SDL.h>
#include <threads.h>
#include <stdint.h>

// SDL window dimensions = framebuffer screen dimensions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

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

static inline void fbdev_putpixel(struct FBDev * fbdev, unsigned col, unsigned row, uint32_t color)
{
    uint32_t * fb = fbdev->impl->fb;

    if (fbdev->cull_area.col <= col && col < (fbdev->cull_area.col + fbdev->cull_area.width)
        && fbdev->cull_area.row <= row && row < (fbdev->cull_area.row + fbdev->cull_area.height) )
    {
        fb[row * WINDOW_WIDTH + col] = color;
    }
}

static inline uint32_t fbdev_getpixel(struct FBDev * fbdev, unsigned col, unsigned row)
{
    uint32_t * fb = fbdev->impl->fb;
    return fb[row * WINDOW_WIDTH + col];
}

void fbdev_blend(struct FBDev* fb, unsigned int col, unsigned int row, uint32_t rgba);

extern const struct FBDevVTable fbdev_vtable;
