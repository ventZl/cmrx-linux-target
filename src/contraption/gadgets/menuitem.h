#pragma once

#include <contraption.h>

void render_menuitem(const struct CWindowInternal * window, const struct CGadgetInternal * gadget);
void pointer_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
void button_down_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void button_up_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void enter_leave_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, bool enter);
void event_menuitem(struct CWindowInternal * window, struct CGadgetInternal * gadget, unsigned event);

