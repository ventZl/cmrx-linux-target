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
#include "events.h"
#include "about_dlg.h"
#include "file_dlg.h"
#include "navigator_win.h"
#include "init_menu.h"
#include "preferences_dlg.h"

int about_win_id = WINDOW_NONE;
int preferences_win_id = WINDOW_NONE;

int init_main(void * data)
{

    bool connected = rpc_call(&display, open_connection, &dialog_win);
    assert(connected);
    int dialog_win_id = rpc_call(&display, open_window, &dialog_win);

    init_app_menu.gadgets[0].sub_menu_id = rpc_call(&display, open_menu, &init_menu);
    init_app_menu.gadgets[1].sub_menu_id = rpc_call(&display, open_menu, &file_menu);

    int about_menu_id = rpc_call(&display, open_menu, &init_app_menu);
    rpc_call(&display, attach_menu, dialog_win_id, about_menu_id);

    while (1)
    {
        wait_for_object(&dialog_win, 0);
        unsigned event_id, object_id;
        bool event = rpc_call(&display, get_event, &event_id, &object_id);
        printf("Event %d in %d!\n", event_id, object_id);
        assert(event);
        switch (event_id) {
            case EVENT_INIT_ABOUT:
                if (about_win_id == WINDOW_NONE)
                {
                    about_win_id = rpc_call(&display, open_window, &about_win);
                    rpc_call(&display, attach_menu, about_win_id, about_menu_id);
                }
                break;

            case EVENT_INIT_PREFERENCES:
                if (preferences_win_id == WINDOW_NONE)
                {
                    preferences_win_id = rpc_call(&display, open_window, &preferences_win);
                    rpc_call(&display, attach_menu, preferences_win_id, about_menu_id);
                }
                break;
            case EVENT_FILE_NEW:
            case EVENT_FILE_OPEN:
            case EVENT_FILE_SAVE:
            case EVENT_FILE_QUIT:
                break;

            case EVENT_BUTTON_OK:
                if (object_id == about_win_id)
                {
                    rpc_call(&display, close_window, about_win_id);
                    about_win_id = WINDOW_NONE;
                }
        }
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
