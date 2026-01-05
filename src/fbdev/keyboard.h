#pragma once

#include <cmrx/rpc/interface.h>
#include "fbdev.h"

struct KeyboardVTable {
    void (*keypress)(INSTANCE(this), char * key);
};

struct Keyboard {
    const struct KeyboardVTable * vtable;
    char key;
};

extern struct Keyboard keyboard;
