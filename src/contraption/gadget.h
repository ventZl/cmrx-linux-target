#pragma once

#include <contraption.h>

enum GadgetEvent {
    EVENT_MENU_CLOSED
};

typedef void (*GadgetRenderCb_t)(const struct CWindowInternal * window, const struct CGadgetInternal * gadget);
typedef void (*GadgetPointerCb_t)(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
typedef void (*GadgetButtonCb_t)(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
typedef void (*GadgetEnterLeaveCb_t)(struct CWindowInternal * window, struct CGadgetInternal * gadget, bool enter);
typedef void (*GadgetEventCb_t)(struct CWindowInternal * window, struct CGadgetInternal * gadget, unsigned event);

struct GadgetType {
    GadgetRenderCb_t render_callback;
    GadgetPointerCb_t mouse_move_callback;
    GadgetButtonCb_t mouse_button_down_callback;
    GadgetButtonCb_t mouse_button_up_callback;
    GadgetEnterLeaveCb_t mouse_enter_leave_callback;
    GadgetEventCb_t event_callback;
};

void gadget_handle_render(const struct CWindowInternal * window, const struct CGadgetInternal * gadget);
void gadget_handle_pointer(struct CWindowInternal * window, struct CGadgetInternal * gadget, struct CPosition * rel_pos, struct CPosition * delta);
void gadget_handle_button_down(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void gadget_handle_button_up(struct CWindowInternal * window, struct CGadgetInternal * gadget, uint32_t buttons);
void gadget_handle_enter_leave(struct CWindowInternal * window, struct CGadgetInternal * gadget, bool enter);
void gadget_handle_event(struct CWindowInternal * window, struct CGadgetInternal * gadget, unsigned event);

//extern struct GadgetType gadget_types[GADGET_KNOWN_TYPES];
