#pragma once

#include <contraption.h>

void render_titlebar(const struct CWindowInternal * window, const struct CGadgetInternal * gadget);
void pointer_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
void button_down_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void button_up_titlebar(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);

