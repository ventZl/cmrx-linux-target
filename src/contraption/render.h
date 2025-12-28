#pragma once

#include "contraption.h"
#include "../fbdev/fbdev.h"

void contraption_render_frame(const struct CExtent * extents, unsigned radius, uint32_t color_ul, uint32_t color_br);
void contraption_render_background(const struct CExtent * extents, const struct FBRectangle * coord, uint32_t * background);
void contraption_render_text(const struct CExtent * extents, const char * text, uint32_t flags, uint32_t rgb_color, uint8_t margin_horiz, uint8_t margin_vert);
void contraption_render_window(struct CWindowInternal * window);
void contraption_render();
void contraption_render_damage(struct CExtent * area);
