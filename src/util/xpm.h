#pragma once

#include "../contraption/contraption.h"

struct Pixmap {
    struct CPixmap dim;
    uint32_t *data;
};

struct Pixmap xpm_to_pixmap(const char** xpm);
void pixmap_free(struct Pixmap *pixmap);
void pixmap_print(const struct Pixmap *pixmap);
