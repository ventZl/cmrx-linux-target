#include <stdio.h>

#include "server.h"

#include <cmrx/rpc/implementation.h>
#include <cmrx/application.h>

IMPLEMENTATION_OF(struct Server, struct ServerVTable);

static int server_service(INSTANCE(this))
{
    printf("Service called\n");
    return 0;
}

VTABLE struct ServerVTable server_vtable = {
    server_service
};

struct Server server = {
    .vtable = &server_vtable
};
