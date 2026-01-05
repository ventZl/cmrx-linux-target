#include "keyboard.h"

#include <cmrx/rpc/implementation.h>
#include <cmrx/ipc/rpc.h>
#include <cmrx/application.h>

IMPLEMENTATION_OF(struct Keyboard, struct KeyboardVTable);

static void keyboard_keypress(INSTANCE(this), char * key)
{
    *key = this->key;
    this->key = 0;
}


VTABLE struct KeyboardVTable keyboard_vtable = {
    keyboard_keypress
};

struct Keyboard keyboard = {
    &keyboard_vtable,
    0
};

