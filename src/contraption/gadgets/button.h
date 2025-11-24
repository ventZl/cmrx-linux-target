#pragma once

#include <contraption.h>

void render_button(const struct CWindowInternal * window, const struct CGadgetInternal * gadget);
void pointer_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
void button_down_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void button_up_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);

