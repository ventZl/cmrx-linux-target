#pragma once

#include <contraption.h>

void render_text(const struct CWindowInternal * window, const struct CGadgetInternal * gadget);
void text_down_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void text_up_button(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
