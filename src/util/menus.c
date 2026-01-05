#include "menus.h"
#include "../contraption/contraption.h"
#include <cmrx/ipc/rpc.h>

int menus_open(struct CWindow * top_menu, struct CWindow * menus[])
{
    for (int q = 0; q < top_menu->gadget_count; ++q)
    {
        top_menu->gadgets[q].sub_menu_id = rpc_call(&display, open_menu, menus[q]);
    }
    int menu_id = rpc_call(&display, open_menu, top_menu);
}

void menus_close(int top_menu_id, struct CWindow * top_menu)
{
    for (int q = 0; q < top_menu->gadget_count; ++q)
    {
        rpc_call(&display, close_menu, top_menu->gadgets[q].sub_menu_id);
    }
    rpc_call(&display, close_menu, top_menu_id);
}

