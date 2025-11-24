#include "pointer.h"

#include <cmrx/rpc/implementation.h>
#include <cmrx/ipc/rpc.h>
#include <cmrx/application.h>

IMPLEMENTATION_OF(struct Pointer, struct PointerVTable);

static void pointer_position(INSTANCE(this), struct FBPosition * position)
{
    *position = this->pos;
}

static void pointer_buttons(INSTANCE(this), uint32_t * buttons)
{
    uint32_t but = this->buttons | (this->buttons_pressed << 16) | (this->buttons_released << 8);
    this->buttons |= this->buttons_pressed;
    this->buttons &= ~this->buttons_released;
    this->buttons_pressed = 0;
    this->buttons_released = 0;
    *buttons = but;
}

VTABLE struct PointerVTable pointer_vtable = {
    pointer_position,
    pointer_buttons
};

struct Pointer pointer = {
    &pointer_vtable,
    { 32, 32 },
    0,
    0,
    0
};
