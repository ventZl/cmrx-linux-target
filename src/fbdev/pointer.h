#pragma once

#include <cmrx/rpc/interface.h>
#include "fbdev.h"

struct PointerVTable {
    void (*position)(INSTANCE(this), struct FBPosition * position);
    void (*buttons)(INSTANCE(this), uint32_t * buttons);
};

struct Pointer {
    const struct PointerVTable * vtable;
    struct FBPosition pos;
    uint32_t buttons;
    uint32_t buttons_pressed;
    uint32_t buttons_released;
};

#define BUTTON_LEFT 2
#define BUTTON_MID  4
#define BUTTON_RIGHT    8

#define BUTTONS_PRESSED(_but)   ((_but >> 16) & 0xFF)
#define BUTTONS_RELEASED(_but)   ((_but >> 8) & 0xFF)
#define BUTTONS_ACTIVE(_but)   (_but & 0xFF)

extern struct Pointer pointer;
