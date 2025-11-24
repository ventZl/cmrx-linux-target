#pragma once

#include "contraption.h"
#include "../fbdev/fbdev.h"

void validate_coordinate(long col, long row);
void contraption_calc_extent(const struct CWindowInternal * window, const struct CGadgetInternal * gadget, struct CExtent * extents);
void contraption_render_frame(const struct CExtent * extents, unsigned radius, uint32_t color_ul, uint32_t color_br);
void internal_raise_window(const struct CWindowInternal * win);
void contraption_render_background(const struct CExtent * extents, const struct FBRectangle * coord, uint32_t * background);
void contraption_render_text(const struct CExtent * extents, const char * text, uint32_t flags, uint32_t rgb_color, uint8_t margin_horiz, uint8_t margin_vert);
void contraption_render_window(struct CWindowInternal * window);
void contraption_render();
void gadget_handle_pointer(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
struct CWindowInternal * contraption_active_window();
unsigned contraption_window_offset(const struct CWindowInternal * window);
void contraption_swap_menu(int new_menu_id);
int contraption_load_window(const struct CWindow* window, bool show, bool menu);
void contraption_stack_window(unsigned win_id);
struct CWindowInternal * contraption_find_window(int window_id);
void contraption_unstack_window(unsigned win_id);
void contraption_hide_menu();
