#include <cmrx/application.h>
#include "../fbdev/fbdev.h"
#include "../fbdev/pointer.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cmrx/ipc/notify.h>

#include "contraption.h"
#include "server.h"
#include "gadget.h"
#include "resources.h"

#include <assert.h>


#define MAX_WINDOWS 64
#define MAX_GADGETS 256

struct CWindowInternal windows[MAX_WINDOWS];
struct CGadgetInternal gadgets[MAX_GADGETS];

unsigned window_stack[MAX_WINDOWS];

unsigned window_count = 0;
unsigned window_stack_count = 0;
unsigned window_id = 1;
unsigned gadget_count = 0;

int contraption_load_window(const struct CWindow* window, bool show, bool menu)
{
    int win_ref = -1;
    if (window_count < MAX_WINDOWS) {
        win_ref = window_id++;
        int win_cursor = window_count;

        windows[win_cursor].properties = *window;
        windows[win_cursor].id = win_ref;
        windows[win_cursor].is_menu = menu;
        windows[win_cursor].menu_window_id = WINDOW_NONE;
        windows[win_cursor].properties.gadgets = (struct CGadget *) &gadgets[gadget_count];

        window_count++;

        for (int q = 0; q < window->gadget_count; ++q)
        {
            gadgets[gadget_count].properties = window->gadgets[q];
            gadgets[gadget_count].window_id = win_ref;
            gadget_count++;
        }

        if (show)
        {
            contraption_stack_window(win_ref);
        }
    }

    return win_ref;
}

unsigned contraption_window_offset(const struct CWindowInternal * window)
{
    unsigned offset =  window - windows;
    assert(offset < window_count);
    return offset;
}

void contraption_stack_window(unsigned win_id)
{
    int win_cursor = contraption_window_offset(contraption_find_window(win_id));
    for (int q = window_stack_count; q > 0; --q)
    {
        window_stack[q] = window_stack[q - 1];
    }

    window_stack[0] = win_cursor;
    window_stack_count++;

    display.render = true;
    notify_object(&display);
}

struct CExtent contraption_window_extents(struct CWindowInternal * window)
{
    struct CExtent rv = { .top = window->properties.top, .left = window->properties.left, .right = window->properties.top + window->properties.height, .bottom = window->properties.top + window->properties.height };
    return rv;
}

void contraption_render_damage(struct CExtent * area)
{

}

void contraption_unstack_window(unsigned win_id)
{
    struct CWindowInternal * window = contraption_find_window(win_id);
    int win_offs = contraption_window_offset(window);
    int current_position = -1;

    assert(win_offs >= 0);
    assert(win_offs < window_count);

    struct CExtent window_extents = contraption_window_extents(window);

    for (int q = 0; q < window_count; ++q)
    {
        if (window_stack[q] == win_offs)
        {
            current_position = q;
            break;
        }
    }

    if (current_position < 0)
    {
        return;
    }

    for (int q = current_position; q < window_stack_count; ++q)
    {
        window_stack[q] = window_stack[q + 1];
    }

    window_stack_count--;
    contraption_render_damage(&window_extents);
}

struct CWindowInternal * contraption_find_window(int window_id)
{
    for (int q = 0; q < window_count; ++q)
    {
        if (windows[q].id == window_id)
        {
            return &windows[q];
        }
    }

    return NULL;
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

void contraption_render_frame(const struct CExtent * extents, unsigned radius, uint32_t color_ul, uint32_t color_br)
{
    struct FBPosition top_left = { extents->left, extents->top};
    struct FBPosition top_right = { extents->right, extents->top};

    struct FBPosition left_up = { extents->left, extents->top};
    struct FBPosition left_down = { extents->left, extents->bottom};

    struct FBPosition right_up = { extents->right, extents->top};
    struct FBPosition right_down = { extents->right, extents->bottom };

    struct FBPosition bottom_left = { extents->left, extents->bottom };
    struct FBPosition bottom_right = { extents->right, extents->bottom };

    if (radius > 0)
    {
        top_left.col += BUTTON_RADIUS;
        bottom_left.col += BUTTON_RADIUS;

        top_right.col -= BUTTON_RADIUS;
        bottom_right.col -= BUTTON_RADIUS;

        left_up.row += BUTTON_RADIUS;
        right_up.row += BUTTON_RADIUS;

        left_down.row -= BUTTON_RADIUS;
        right_down.row -= BUTTON_RADIUS;

        struct FBPosition ul_corner = { extents->left + BUTTON_RADIUS, extents->top + BUTTON_RADIUS };
        struct FBPosition ur_corner = { extents->right - BUTTON_RADIUS, extents->top + BUTTON_RADIUS };
        struct FBPosition bl_corner = { extents->left + BUTTON_RADIUS, extents->bottom - BUTTON_RADIUS };
        struct FBPosition br_corner = { extents->right - BUTTON_RADIUS, extents->bottom - BUTTON_RADIUS };

        rpc_call(&fbdev, arc, &ul_corner, radius, FBDEV_QUADRANT_NW, color_ul);
        rpc_call(&fbdev, arc, &ur_corner, radius, FBDEV_QUADRANT_NE, color_br);
        rpc_call(&fbdev, arc, &bl_corner, radius, FBDEV_QUADRANT_SW, color_ul);
        rpc_call(&fbdev, arc, &br_corner, radius, FBDEV_QUADRANT_SE, color_br);
    }

    rpc_call(&fbdev, line, &top_left, &top_right, color_ul);
    rpc_call(&fbdev, line, &right_up, &right_down, color_br);
    rpc_call(&fbdev, line, &bottom_left, &bottom_right, color_br);
    rpc_call(&fbdev, line, &left_up, &left_down, color_ul);

}

void contraption_render_background(const struct CExtent * extents, const struct FBRectangle * coord, uint32_t * background)
{
    struct FBRectangle dest = {extents->left, extents->top, extents->right - extents->left, extents->bottom - extents->top };
    rpc_call(&fbdev, blit, &dest, coord, background, NULL);
}

void contraption_render_text(const struct CExtent * extents, const char * text, uint32_t flags, uint32_t rgb_color, uint8_t margin_horiz, uint8_t margin_vert)
{
    unsigned top = extents->top, left = extents->left;
    FBTextMetrics metrics;
    rpc_call(&fbdev, text_measure, text, &metrics);

    if (flags & TEXT_ALIGN_LEFT)
    {
        left += margin_horiz;
    }
    if (flags & TEXT_ALIGN_CENTER)
    {
        left += ((extents->right - extents->left) - metrics.width) / 2;
    }
    if (flags & TEXT_ALIGN_RIGHT)
    {
        left = extents->right - metrics.width - margin_horiz;
    }
    if (flags & TEXT_ALIGN_TOP)
    {
        top += margin_vert;
    }
    if (flags & TEXT_ALIGN_MIDDLE)
    {
        top += ((extents->bottom - extents->top) - (metrics.y_maximum - metrics.y_minimum)) / 2;
    }
    if (flags & TEXT_ALIGN_RIGHT)
    {
        top = extents->bottom - metrics.y_maximum - margin_vert;
    }
    rpc_call(&fbdev, text, text, left, top, rgb_color);
}

void contraption_render_window(struct CWindowInternal * window)
{
    struct FBRectangle dest = { window->properties.left, window->properties.top, window->properties.width, window->properties.height };
    struct FBPosition up_left = { window->properties.left, window->properties.top };
    struct FBPosition up_right = { window->properties.left + window->properties.width - 1, window->properties.top };
    struct FBPosition bottom_left = { window->properties.left, window->properties.top + window->properties.height - 1 };
    struct FBPosition bottom_right = { window->properties.left + window->properties.width - 1, window->properties.top + window->properties.height - 1 };

    validate_coordinate(up_left.col, up_left.row);
    validate_coordinate(up_right.col, up_right.row);
    validate_coordinate(bottom_left.col, bottom_left.row);
    validate_coordinate(bottom_right.col, bottom_right.row);

    rpc_call(&fbdev, blit, &dest, &window_background, window_pixmap, NULL);
    rpc_call(&fbdev, line, &up_left, &up_right, 0xE3E3E3FF);
    rpc_call(&fbdev, line, &up_left, &bottom_left, 0x9FABBCFF);
    rpc_call(&fbdev, line, &bottom_left, &bottom_right, 0x9FABBCFF);
    rpc_call(&fbdev, line, &up_right, &bottom_right, 0x9FABBCFF);

    struct CGadgetInternal * gadgets = (struct CGadgetInternal *) window->properties.gadgets;
    for (int q = 0; q < window->properties.gadget_count; ++q)
    {
        gadget_handle_render(window, &gadgets[q]);
    }
}

void contraption_render()
{
    struct FBRectangle dest = { 0, 0, 1280, 800 };
    rpc_call(&fbdev, blit, &dest, &background, background_pixmap, NULL);
    dest.height = 20;
    rpc_call(&fbdev, blit, &dest, &window_background, window_pixmap, NULL);

    for (unsigned q = window_stack_count; q > 0; --q)
    {
        contraption_render_window(&windows[window_stack[q - 1]]);
    }
}

struct CWindowInternal * contraption_active_window()
{
    return &windows[window_stack[0]];
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

void internal_raise_window(const struct CWindowInternal * win)
{
    if (win == NULL)
    {
        return;
    }

    int win_offs = win - &windows[0];

    int current_position = -1;

    if (!win->is_menu)
    {
        contraption_swap_menu(win->menu_window_id);
    }

    assert(win_offs >= 0);
    assert(win_offs < window_count);

    for (int q = 0; q < window_count; ++q)
    {
        if (window_stack[q] == win_offs)
        {
            current_position = q;
            break;
        }
    }

    if (current_position < 1)
    {
        return;
    }

    for (int q = current_position; q > 0; --q)
    {
        window_stack[q] = window_stack[q - 1];
    }

    window_stack[0] = win_offs;
//    display.render = true;
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
//        notify_object(&display);
        return;
    }

    for (unsigned q = 0; q < window_count; ++q)
    {
        struct CWindowInternal * window = &windows[window_stack[q]];
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
//                        notify_object(&display);
                        return;
                    }
                }

                display.cursor_gadget = NULL;
//                notify_object(&display);
                return;
            }
    }
    display.cursor_window = NULL;
//    notify_object(&display);
}

void contraption_hide_menu()
{
    contraption_unstack_window(display.popup_window->id);
    display.menu_window_id = WINDOW_NONE;
}

void contraption_process_button()
{
    uint32_t buttons;
    rpc_call(&pointer, buttons, &buttons);

    if (display.cursor_window != NULL)
    {
        if ((display.popup_window != NULL) && (display.cursor_window != display.popup_window) && BUTTONS_PRESSED(buttons))
        {
            contraption_hide_menu();
            display.popup_window = NULL;
        }

        // Raise window, if it is not the one on top of the stack and any button was pressed
        if (&windows[window_stack[0]] != display.cursor_window && BUTTONS_PRESSED(buttons))
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
            /*
            const uint32_t flags = display.cursor_gadget->properties.flags;
                if (flags & GADGET_FLAG_FOCUSABLE)
                {
                    if (display.focus_gadget != NULL)
                    {
                        display.focus_gadget->properties.state &= ~GADGET_STATE_FOCUSED;
                    }

                    display.cursor_gadget->properties.state |= GADGET_STATE_FOCUSED;
                    display.focus_gadget = display.cursor_gadget;
                }
                if (flags & GADGET_FLAG_ACTIVABLE)
                {
                    display.cursor_gadget->properties.state |= GADGET_STATE_ACTIVATED;
                    display.cursor_gadget_grab = true;
                }
            }
            if (BUTTONS_RELEASED(buttons) & BUTTON_LEFT)
            {
                display.cursor_gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
                display.cursor_gadget_grab = false;
            }*/
        }
    }
}

int contraption_main(void * data)
{
    (void) data;

    for (int q = 0; q < BACKGROUND_SIZE * BACKGROUND_SIZE; ++q) {
        background_pixmap[q] = 0x4168A9FF;
        window_pixmap[q] = (q % 64) < 32 ? 0xEAEAEAFF : 0xF0F0F0FF;
    }

    int steps = TITLE_HEIGHT - 1;
    int start = 0xCE;
    int stop = 0xA8;
    int step = (stop - start) / steps;

    for (int q = 0; q < steps; ++q)
    {
        int color = start + step * q;
        title_pixmap[q] = (color << 24) | (color << 16) | (color << 8) | 0xFF;
    }

    title_pixmap[0] = 0xE3E3E3FF;
    title_pixmap[TITLE_HEIGHT - 1] = 0x515151FF;

    while (1)
    {
        wait_for_object(&display, 0);
        if (display.render)
        {
            contraption_render();
            display.render = false;
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
    }

}

OS_APPLICATION_MMIO_RANGE(contraption, 0, 0);
OS_APPLICATION(contraption);
OS_THREAD_CREATE(contraption, contraption_main, NULL, 16);
