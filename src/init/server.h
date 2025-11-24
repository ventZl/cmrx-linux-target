#pragma once

#include <cmrx/rpc/interface.h>

struct ServerVTable {
    int (*service)(INSTANCE(this));
};

struct Server {
    const struct ServerVTable * vtable;
};

extern struct Server server;

