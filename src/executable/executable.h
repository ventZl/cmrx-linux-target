#pragma once

#include <cmrx/rpc/interface.h>

struct ExecutableVTable {
    void (*exec)(INSTANCE(this));
};

struct Executable {
    const struct ExecutableVTable * vtable;
};
