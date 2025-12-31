#pragma once

#include "contraption.h"

void contraption_window_init();
int contraption_load_window(const struct CWindow* window, bool show, bool menu, unsigned owning_client);
void contraption_stack_window(unsigned win_id);
void contraption_stack_menu(unsigned menu_id, struct CWindowInternal * window, struct CGadgetInternal * gadget);
void contraption_unstack_window(unsigned win_id);
void contraption_move_window(struct CWindowInternal * window, unsigned col, unsigned row);
void contraption_resize_window(struct CWindowInternal * window, unsigned width, unsigned height);
void contraption_free_window(struct CWindowInternal * window);
struct CWindowInternal * contraption_find_window(int window_id);

/* Get count of visible windows
 * This includes menu stripes.
 */
unsigned contraption_window_count();

/* Get ID of window at offset in window stack */
struct CWindowInternal * contraption_window_at_offset(unsigned int offset);

void internal_raise_window(const struct CWindowInternal * win);

struct CWindowInternal * contraption_active_window();

void contraption_hide_menu();
