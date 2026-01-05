#pragma once

#include <contraption.h>

int contraption_open_connection(/*struct Contraption*/ void * this, const void * notification_object, const char * name, unsigned flags);
int contraption_open_window(/*struct Contraption*/ void * this, const struct CWindow * window);
bool contraption_get_event(/* struct Contraption */ void * this, unsigned * event_id, unsigned * object_id);
bool contraption_reload_window(/* struct Contraption */ void *this, int window, const struct CGadget * gadgets, unsigned gadget_count);
bool contraption_move_window(/* struct Contraption */ void *this, int window, unsigned int left, unsigned int top);
bool contraption_resize_window(/* struct Contraption */ void *this, int window, unsigned int width, unsigned int height);
int contraption_open_pixmap(/* struct Contraption */ void *this, const struct CPixmap * pixmap);
int contraption_load_pixmap(/* struct Contraption */ void *this, int pixmap_id, const uint32_t * data);
int contraption_close_pixmap(/* struct Contraption */ void *this, int pixmap_id);
void contraption_set_connection_icon(/* struct Contraption */ void *this, int pixmap_id);

