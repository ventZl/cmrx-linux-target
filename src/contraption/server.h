#pragma once

#include "contraption.h"

void validate_coordinate(long col, long row);
void contraption_calc_extent(const struct CWindowInternal * window, const struct CGadgetInternal * gadget, struct CExtent * extents);
void gadget_handle_pointer(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
struct CWindowInternal * contraption_active_window();
void contraption_swap_menu(int new_menu_id);
void contraption_hide_menu();
struct CExtent contraption_window_extents(struct CWindowInternal * window);

/* Get offset of window in stack */
unsigned contraption_window_offset(const struct CWindowInternal * window);


