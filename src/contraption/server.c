#include <cmrx/application.h>
#include "../fbdev/fbdev.h"
#include "../fbdev/pointer.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cmrx/ipc/notify.h>
#include <cmrx/ipc/thread.h>

#include "contraption.h"
#include "server.h"
#include "gadget.h"
#include "render.h"
#include "resources.h"
#include "window.h"
#include "client.h"

#include "api_priv.h"

#include <assert.h>

struct CExtent contraption_window_extents(struct CWindowInternal * window)
{
    struct CExtent rv = { .top = window->properties.top, .left = window->properties.left, .right = window->properties.top + window->properties.height, .bottom = window->properties.top + window->properties.height };
    return rv;
}


void validate_coordinate(long col, long row)
{
    assert(0 <= col && col < SCREEN_WIDTH);
    assert(0 <= row && row < SCREEN_HEIGHT);
}

void contraption_calc_extent(const struct CWindowInternal * window, const struct CGadgetInternal * gadget, struct CExtent * extents)
{
    extents->left = window->properties.left + gadget->properties.left;
    extents->top = window->properties.top + gadget->properties.top;
    extents->right = extents->left + gadget->properties.width - 1;
    extents->bottom = extents->top + gadget->properties.height - 1;
}

void contraption_swap_menu(int new_menu_id)
{
    if (display.menu_window_id != WINDOW_NONE)
    {
        contraption_unstack_window(display.menu_window_id);
    }
    if (new_menu_id != WINDOW_NONE)
    {
        struct CWindowInternal * menu_window = contraption_find_window(new_menu_id);
        assert(menu_window->is_menu == true);
        contraption_stack_window(new_menu_id);
    }
    display.menu_window_id = new_menu_id;
    display.render = true;
}

bool contraption_cursor_inside_window(const struct CWindowInternal * window, int col, int row)
{
    return ((col >= window->properties.left && col < window->properties.left + window->properties.width)
        && (row >= window->properties.top && row < window->properties.top + window->properties.height));
}

void contraption_process_pointer()
{
    struct FBPosition pos;
    rpc_call(&pointer, position, &pos);
    struct CPosition delta = { .left = pos.col - display.last_pos.left, .top = pos.row - display.last_pos.top };
    display.last_pos.top = pos.row;
    display.last_pos.left = pos.col;

    if (display.cursor_gadget_grab)
    {
        struct CExtent extents;
        contraption_calc_extent(display.cursor_window, display.cursor_gadget, &extents);

        struct CPosition rel_pos = { pos.col - extents.left, pos.row - extents.top };

        gadget_handle_pointer(display.cursor_window, display.cursor_gadget, &rel_pos, &delta);
        return;
    }

    for (unsigned q = 0; q < contraption_window_count(); ++q)
    {
        struct CWindowInternal * window = contraption_window_at_offset(q);
        if (contraption_cursor_inside_window(window, pos.col, pos.row))
            {
                if (display.cursor_window != NULL && display.cursor_window != window && display.cursor_gadget != NULL)
                {
                    gadget_handle_enter_leave(display.cursor_window, display.cursor_gadget, false);
                    display.cursor_gadget = NULL;
                }
                display.cursor_window = window;

                // Window found, now find a gadget
                struct CGadgetInternal * gadgets = (struct CGadgetInternal *) window->properties.gadgets;
                for (int q = 0; q < window->properties.gadget_count; ++q)
                {
                    struct CExtent extents;
                    contraption_calc_extent(window, &gadgets[q], &extents);

                    struct CPosition rel_pos = { pos.col - extents.left, pos.row - extents.top };

                    if (pos.col >= extents.left && pos.col <= extents.right && pos.row >= extents.top && pos.row <= extents.bottom)
                    {
                        if (display.cursor_gadget != &gadgets[q])
                        {
                            if (display.cursor_gadget != NULL)
                            {
                                gadget_handle_enter_leave(display.cursor_window, display.cursor_gadget, false);
                            }
                            gadget_handle_enter_leave(display.cursor_window, &gadgets[q], true);
                        }

                        display.cursor_gadget = &gadgets[q];
                        gadget_handle_pointer(window, &gadgets[q], &rel_pos, &delta);
                        return;
                    }
                }

                display.cursor_gadget = NULL;
                return;
            }
    }
    display.cursor_window = NULL;
//    notify_object(&display);
}


void contraption_process_button()
{
    uint32_t buttons;
    rpc_call(&pointer, buttons, &buttons);

    // Should not happen
    assert(display.cursor_window != NULL);

    if (display.cursor_window != NULL)
    {
        // Hide menu if click went to different window than
        if ((display.popup_window != NULL) && (display.cursor_window != display.popup_window) && BUTTONS_PRESSED(buttons))
        {
            contraption_hide_menu();
       }

        // Raise window, if it is not the one on top of the stack and any button was pressed
        if (contraption_window_at_offset(0) != display.cursor_window && BUTTONS_PRESSED(buttons) && ((display.cursor_window->properties.flags & WINDOW_FLAG_BELOW_ALL) == 0))
        {
            internal_raise_window(display.cursor_window);
            contraption_render_window(display.cursor_window);
        }

        if (display.cursor_gadget != NULL)
        {
            struct CGadgetInternal * gadget = display.cursor_gadget;

            if (BUTTONS_PRESSED(buttons))
            {
                gadget_handle_button_down(display.cursor_window, display.cursor_gadget, BUTTONS_PRESSED(buttons));
            }
            if (BUTTONS_RELEASED(buttons))
            {
                gadget_handle_button_up(display.cursor_window, display.cursor_gadget, BUTTONS_RELEASED(buttons));
            }
        }
    }
}

/* Generate gray gradient of dimension [1 x steps]
 */
void generate_gradient_gray(int steps, uint32_t start_shade, uint32_t end_shade, uint32_t * gradient)
{
    int start = start_shade;
    int stop = end_shade;
    int step = (stop - start) / steps;

    for (unsigned q = 0; q < steps; ++q)
    {
        int color = start + step * q;
        gradient[q] = (color << 24) | (color << 16) | (color << 8) | 0xFF;
    }
}

int contraption_main(void * data)
{
    (void) data;

    for (int q = 0; q < BACKGROUND_SIZE * BACKGROUND_SIZE; ++q) {
        background_pixmap[q] = 0x4168A9FF;
        window_pixmap[q] = (q % 64) < 32 ? 0xEAEAEAFF : 0xF0F0F0FF;
    }

    struct CGadget desktop_gadgets[] = {
        {
            .type = GADGET_PANEL,
            .top = 0,
            .left = 0,
            .width = 1280,
            .height = 20,
        }
    };

    struct CWindow desktop_window = {
        .top = 0,
        .left = 0,
        .width = 1280,
        .height = 800,
        .gadgets = desktop_gadgets,
        .gadget_count = 1,
        .background = BACKGROUND_DESKTOP,
        .flags = WINDOW_FLAG_BELOW_ALL
    };

    contraption_init_client();
    contraption_window_init();
    contraption_open_connection(&display, &desktop_window);
    contraption_open_window(&display, &desktop_window);

    generate_gradient_gray(TITLE_HEIGHT - 1, 0xCE, 0xA8, title_pixmap);
    generate_gradient_gray(BUTTON_HEIGHT - 1, 0xDE, 0x98, button_pixmap);
/*
    int steps = TITLE_HEIGHT - 1;
    int start = 0xCE;
    int stop = 0xA8;
    int step = (stop - start) / steps;

    for (int q = 0; q < steps; ++q)
    {
        int color = start + step * q;
        title_pixmap[q] = (color << 24) | (color << 16) | (color << 8) | 0xFF;
    }*/

    title_pixmap[0] = 0xE3E3E3FF;
    title_pixmap[TITLE_HEIGHT - 1] = 0x515151FF;

    while (1)
    {
        if (display.render != 1)
        {
            wait_for_object(&display, 0);
        }
        if (display.pointer)
        {
            contraption_process_pointer();
            display.pointer = false;
        }
        if (display.button)
        {
            contraption_process_button();
            display.button = false;
        }
        if (display.render)
        {
            contraption_render();
            display.render = false;
        }
    }

}

OS_APPLICATION_MMIO_RANGE(contraption, 0, 0);
OS_APPLICATION(contraption);
OS_THREAD_CREATE(contraption, contraption_main, NULL, 16);
