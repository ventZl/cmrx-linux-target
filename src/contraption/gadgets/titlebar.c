#include <server.h>
#include <render.h>

#include <render.h>
#include <window.h>
#include "titlebar.h"
#include <resources.h>

enum PillState {
    TITLEBAR_RED_PILL_HOVER = 1 << 31,
    TITLEBAR_YELLOW_PILL_HOVER = 1 << 30,
    TITLEBAR_GREEN_PILL_HOVER = 1 << 29
};

void render_titlebar(const struct CWindowInternal * window, const struct CGadgetInternal * gadget)
{
    struct CExtent extents;
    contraption_calc_extent(window, gadget, &extents);
    contraption_render_background(&extents, &title_background, title_pixmap);

    struct CExtent pill_extents = extents;
    struct FBRectangle rect = { 0, 0, red_pill_pixmap.dim.width, red_pill_pixmap.dim.height };
    struct Pixmap * pills[] = {
        &red_pill_pixmap,
        window->properties.flags & WINDOW_FLAG_FIX_SIZE ? &silver_pill_pixmap : &yellow_pill_pixmap,
        window->properties.flags & WINDOW_FLAG_NO_HIDE ? &silver_pill_pixmap : &green_pill_pixmap
    };
    pill_extents.left += 5;
    pill_extents.top += 3;
    pill_extents.right = pill_extents.left + red_pill_pixmap.dim.width;
    pill_extents.bottom = pill_extents.top + red_pill_pixmap.dim.height;
    for (int q = 0; q < 3; ++q)
    {
        contraption_render_background(&pill_extents, &rect, pills[q]->data);
        pill_extents.left += 20;
        pill_extents.right += 20;
    }
    contraption_render_text(&extents, window->properties.title, TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE | TEXT_BOLD, 0, gadget->properties.text_margin_horiz, gadget->properties.text_margin_vert);
}

void pointer_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta)
{
    if ((rel_pos->left >=5 && rel_pos->left <=60) && (rel_pos->top >=3 && rel_pos->top <18))
    {
        // We are in pills area
        if (rel_pos->left < 22)
        {
            gadget->properties.state |= TITLEBAR_RED_PILL_HOVER;
            gadget->properties.state &= ~(TITLEBAR_YELLOW_PILL_HOVER | TITLEBAR_GREEN_PILL_HOVER);
        }
        else
        {
            gadget->properties.state &= ~TITLEBAR_RED_PILL_HOVER;
            if (rel_pos->left < 39)
            {
                gadget->properties.state |= TITLEBAR_YELLOW_PILL_HOVER;
                gadget->properties.state &= ~TITLEBAR_GREEN_PILL_HOVER;
            }
            else
            {
                gadget->properties.state |= TITLEBAR_GREEN_PILL_HOVER;
                gadget->properties.state &= ~TITLEBAR_YELLOW_PILL_HOVER;
            }
        }
    }
    else
    {
        gadget->properties.state &= ~(TITLEBAR_YELLOW_PILL_HOVER | TITLEBAR_GREEN_PILL_HOVER | TITLEBAR_RED_PILL_HOVER);
    }

    if (gadget->properties.state & GADGET_STATE_ACTIVATED)
    {
        struct CExtent old_extents = contraption_window_extents(window);
        contraption_window_move(window, window->properties.left + delta->left, window->properties.top + delta->top);
        struct CExtent new_extents = contraption_window_extents(window);
        struct CExtent render_extents = contraption_union_extents(&old_extents, &new_extents);
        contraption_render_damage(&render_extents);
    }
}

void button_down_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if ((gadget->properties.state & (TITLEBAR_YELLOW_PILL_HOVER | TITLEBAR_GREEN_PILL_HOVER | TITLEBAR_RED_PILL_HOVER)) == 0)
    {
        display.cursor_gadget->properties.state |= GADGET_STATE_ACTIVATED;
        display.cursor_gadget_grab = true;
    }

}

void button_up_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons)
{
    if (gadget->properties.state & GADGET_STATE_ACTIVATED)
    {
        display.cursor_gadget->properties.state &= ~GADGET_STATE_ACTIVATED;
        display.cursor_gadget_grab = false;
    }

    if (gadget->properties.state & TITLEBAR_RED_PILL_HOVER)
    {
        contraption_free_window(window);
    }

    if (gadget->properties.state & TITLEBAR_GREEN_PILL_HOVER)
    {
//        contraption_unstack_window(window);
    }

}


