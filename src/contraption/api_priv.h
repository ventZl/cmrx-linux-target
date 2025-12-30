#pragma once

#include <contraption.h>

int contraption_open_connection(struct Contraption * this, void * notification_object);
int contraption_open_window(struct Contraption * this, const struct CWindow * window);
