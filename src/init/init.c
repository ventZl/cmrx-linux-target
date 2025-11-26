#include <cmrx/application.h>
#include <stdio.h>
#include <cmrx/sys/syscalls.h>
#include <cmrx/cmrx.h>
#include <cmrx/ipc/notify.h>
#include <cmrx/ipc/mutex.h>
#include <cmrx/ipc/thread.h>
#include <cmrx/ipc/timer.h>
#include <cmrx/ipc/signal.h>
#include <cmrx/ipc/rpc.h>
#include <cmrx/sys/irq.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../contraption/contraption.h"
#include "server.h"


int init_main(void * data)
{
    struct CGadget about_gadegets[] = {
        {
            .type = GADGET_BUTTON,
            .top = 370,
            .left = 330/2 - 40,
            .width = 80,
            .height = 30,
            .flags = GADGET_FLAG_ENABLED,
            .state = 0,
            .text = "OK",
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE

        },
        {
            .type = GADGET_TITLE_BAR,
            .top = 0,
            .left = 0,
            .width = 330,
            .height = 22,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .state = 0
        },
        {
            .type = GADGET_TEXT,
            .top = 35,
            .left = 10,
            .width = 310,
            .height = 22,
            .flags = GADGET_FLAG_ENABLED,
            .state = 0,
            .text = "Hello from Contraption!",
            .fg_color = 0x000000,
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE
        }
    };

    struct CWindow about_win = {
        .width = 330,
        .height = 430,
        .top = 90,
        .left = 1280 / 2 - (330 / 2) ,
        .title = "About CMRX",
        .gadgets = about_gadegets,
        .gadget_count = 3,
        .background = BACKGROUND_WINDOW,
        .flags = WINDOW_FLAG_BORDER
    };

    struct CGadget dialog_gadgets[] = {
        {
            .type = GADGET_TITLE_BAR,
            .width = 400,
            .height = 22,
            .top = 0,
            .left = 0,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .state = 0
        }
    };

    struct CWindow dialog_win = {
        .width = 400,
        .height = 300,
        .top = 100,
        .left = 300,
        .title = "Settings",
        .gadgets = dialog_gadgets,
        .gadget_count = 1,
        .background = BACKGROUND_WINDOW,
        .flags = WINDOW_FLAG_BORDER
    };

    int dialog_win_id = rpc_call(&display, open_window, &dialog_win);
    int about_win_id = rpc_call(&display, open_window, &about_win);

    struct CGadget file_menu_items[] = {
        {
            .type = GADGET_MENU_ITEM,
            .top = 0,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "New",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .text_margin_horiz = 5
        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 19,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "Open...",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .text_margin_horiz = 5,
            .sub_menu_id = WINDOW_NONE

        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 39,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "Save",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = 0,
            .text_margin_horiz = 5,
            .sub_menu_id = WINDOW_NONE
        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 59,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "Quit",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .text_margin_horiz = 5,
            .sub_menu_id = WINDOW_NONE
        },
    };

    struct CWindow file_menu = {
        .top = 20,
        .left = 0,
        .width = 90,
        .height = 80,
        .gadgets = file_menu_items,
        .gadget_count = 4
    };

    int file_menu_id = rpc_call(&display, open_menu, &file_menu);

    struct CGadget about_menu_items[] = {
        {
            .type = GADGET_MENU_ITEM,
            .top = 0,
            .left = 0,
            .width = 40,
            .height = 20,
            .text = "File",
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .sub_menu_id = file_menu_id,
        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 0,
            .left = 39,
            .width = 40,
            .height = 20,
            .text = "Help",
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
            .flags = 0 //GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE
        },
    };

    struct CWindow about_menu = {
        .top = 0,
        .left = 0,
        .width = 1280,
        .height = 20,
        .gadgets = about_menu_items,
        .gadget_count = 2
    };



    int about_menu_id = rpc_call(&display, open_menu, &about_menu);
    rpc_call(&display, attach_menu, about_win_id, about_menu_id);

    while (1)
    {
        sleep(1);
/*        int rv = rpc_call(&server, service);
        int my_tid = get_tid();
        printf("I am thread %d\n", my_tid);*/
    }

}

int init_main2(void * data)
{
    (void) data;
    while (1)
    {
        sleep(1);
        int my_tid = get_tid();
    }
}

OS_APPLICATION_MMIO_RANGE(init, 0, 0);
OS_APPLICATION(init);
OS_THREAD_CREATE(init, init_main, NULL, 64);
OS_THREAD_CREATE(init, init_main2, NULL, 64);
