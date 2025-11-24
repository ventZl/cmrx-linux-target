#include "fbdev.h"
#include "fbdev_impl.h"
#include "pointer.h"

#include <cmrx/application.h>
#include <stdio.h>
#include <unistd.h>
#include <threads.h>
#include <signal.h>

#include <extra/posix_interrupt.h>

#include <SDL3/SDL.h>

static struct FBDevImpl fbdev_impl = {
    .running = true,
    .window = NULL,
    .texture = NULL,
    .renderer = NULL,
    .blit_dirty = true
};

struct FBDev fbdev = {
    .vtable = &fbdev_vtable,
    .impl = &fbdev_impl
};

void fbdev_blend(uint32_t * fb, unsigned col, unsigned row, uint32_t rgba)
{
    uint32_t orig = fb[(row * WINDOW_WIDTH) + col] >> 8;
    uint32_t alpha = rgba & 0xFF;
    rgba = rgba >> 8;
    uint32_t out = 0xFF << 24;
    for (int q = 0; q < 3; ++q) {
        const uint32_t o_comp = orig & 0xFF;
        const uint32_t n_comp = rgba & 0xff;
        orig = orig >> 8;
        rgba = rgba >> 8;
        const uint32_t m_comp = (o_comp * (0xFF - alpha) + (n_comp * alpha))/ 0xFF;
        out = (out >> 8) | m_comp << 24;
    }
    fb[(row * WINDOW_WIDTH) + col] = out;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(struct FBDev * device)
{
    SDL_SetAppMetadata("Example Renderer Streaming Textures", "1.0", "com.example.renderer-streaming-textures");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("CMRX virtual fbdev", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &device->impl->window, &device->impl->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(device->impl->renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    device->impl->texture = SDL_CreateTexture(device->impl->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!device->impl->texture) {
        SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(struct FBDev * device, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:
            device->impl->running = false;
            return 0;  /* end the program, reporting success to the OS. */

        case SDL_EVENT_MOUSE_MOTION:
            pointer.pos.col = event->motion.x;
            pointer.pos.row = event->motion.y;
            generate_interrupt(15);
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            pointer.buttons_pressed |= 1 << event->button.button;
            generate_interrupt(15);
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            pointer.buttons_released |= 1 << event->button.button;
            generate_interrupt(15);
            break;

        default:
            // ignore
            break;
    }
    return 0;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(struct FBDev * device)
{
    SDL_FRect dst_rect;
    const Uint64 now = SDL_GetTicks();
    SDL_Surface *surface = NULL;

    /* we'll have some color move around over a few seconds. */
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    /* To update a streaming texture, you need to lock it first. This gets you access to the pixels.
     *       Note that this is considered a _write-only_ operation: the buffer you get from locking
     *       might not acutally have the existing contents of the texture, and you have to write to every
     *       locked pixel! */
    mtx_lock(&device->impl->sdl_blit_mutex);
    if (device->impl->blit_dirty)
    {
        uint32_t * pixels;
        int pitch;      // width of one line in bytes (not pixels
        if (SDL_LockTexture(device->impl->texture, NULL, (void **) &pixels, &pitch)) {

            memcpy(pixels, device->impl->fb, WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t));
            SDL_UnlockTexture(device->impl->texture);  /* upload the changes (and frees the temporary surface)! */
        }
        device->impl->blit_dirty = false;
    }
    mtx_unlock(&device->impl->sdl_blit_mutex);
    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_RenderClear(device->impl->renderer);  /* start with a blank canvas. */

    /* Just draw the static texture a few times. You can think of it like a
     *       stamp, there isn't a limit to the number of times you can draw with it. */

    mtx_lock(&device->impl->sdl_blit_mutex);
    /* Center this one. It'll draw the latest version of the texture we drew while it was locked. */
    dst_rect.x = 0.0f;
    dst_rect.y = 0.0f;
    dst_rect.w = (float) WINDOW_WIDTH;
    dst_rect.h = (float) WINDOW_HEIGHT;
    SDL_RenderTexture(device->impl->renderer, device->impl->texture, NULL, &dst_rect);
    mtx_unlock(&device->impl->sdl_blit_mutex);

    SDL_RenderPresent(device->impl->renderer);  /* put it all on the screen! */

    SDL_DelayNS(10000000);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(struct FBDev * device)
{
    SDL_DestroyTexture(device->impl->texture);
    /* SDL will clean up the window/renderer for us. */
    SDL_DestroyWindow(device->impl->window);
    _exit(0);
}

int fbdev_device_main(void * data)
{
    struct FBDev * device = data;
    if (SDL_AppInit(device) == SDL_APP_FAILURE)
    {
        fprintf(stderr, "Unable to initialize SDL!\n");
        _exit(0);
    }
    printf("SDL OK\n");
    //The main loop
    while( device->impl->running ) {
        SDL_Event e;
        //Get event data
        while( SDL_PollEvent( &e ) == true )
        {
            SDL_AppEvent(device, &e);
        }

        SDL_AppIterate(device);
    }
    SDL_AppQuit(device);
    return 0;
}

void fbdev_init()
{
    sigset_t set;

    /* We want to receive SIGALRM and SIGUSR1 synchronously
     * in the main thread. SIGUSR2 will ever only be sent to
     * the specific thread to suspend it.
     */
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGURG);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    mtx_init(&fbdev.impl->sdl_blit_mutex, mtx_plain);
    thrd_create(&fbdev.impl->sdl_main_thread, fbdev_device_main, &fbdev);
}

OS_APPLICATION_MMIO_RANGE(fbdev, 0, 0);
OS_APPLICATION(fbdev);
