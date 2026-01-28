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

#include "../../img/logo.xpm"
#include "../../img/browser.xpm"
#include "../util/xpm.h"
#include "../icons/icons.h"
#include "../executable/executable.h"

int about_win_id = WINDOW_NONE;
int preferences_win_id = WINDOW_NONE;
int logo_pixmap_id = PIXMAP_NONE;

struct Pixmap init_icon;

extern struct Executable term;
extern struct Executable presentation;

int init_main(void * data)
{
    init_icon = xpm_to_pixmap(browser_xpm);
    bool connected = rpc_call(&display, open_connection, &dialog_win, "Init", 0);
    assert(connected);
    dialog_win.gadgets[1].pixmap_id = rpc_call(&icon_server, get_icon, ICON_TERMINAL);
    dialog_win.gadgets[2].event_id = EVENT_RUN_TERMINAL;

    dialog_win.gadgets[3].pixmap_id = rpc_call(&icon_server, get_icon, ICON_PRESENTATION);
    dialog_win.gadgets[4].event_id = EVENT_RUN_PRESENTATION;
    int dialog_win_id = rpc_call(&display, open_window, &dialog_win);
    assert(dialog_win_id != WINDOW_NONE);
    int init_icon_id = rpc_call(&display, open_pixmap, &init_icon.dim);
    if (init_icon_id != PIXMAP_NONE)
    {
        rpc_call(&display, load_pixmap, init_icon_id, init_icon.data);
        rpc_call(&display, set_connection_icon, init_icon_id);
    }
    init_app_menu.gadgets[0].sub_menu_id = rpc_call(&display, open_menu, &init_menu);
    init_app_menu.gadgets[1].sub_menu_id = rpc_call(&display, open_menu, &file_menu);

    int about_menu_id = rpc_call(&display, open_menu, &init_app_menu);
    rpc_call(&display, attach_menu, dialog_win_id, about_menu_id);

    while (1)
    {
        wait_for_object(&dialog_win, 0);
        unsigned event_id, object_id;
        bool event = rpc_call(&display, get_event, &event_id, &object_id);
        assert(event);
        switch (event_id) {
            case EVENT_WINDOW_CLOSED:
                if (object_id == about_win_id)
                {
                    rpc_call(&display, close_pixmap, logo_pixmap_id);
                    logo_pixmap_id = PIXMAP_NONE;
                    about_win_id = WINDOW_NONE;
                }
                break;

            case EVENT_INIT_ABOUT:
                if (about_win_id == WINDOW_NONE)
                {
                    struct Pixmap cmrx_logo = xpm_to_pixmap(logo_xpm);
                    int logo_pixmap_id = rpc_call(&display, open_pixmap, &cmrx_logo.dim);
                    about_win.gadgets[1].pixmap_id = logo_pixmap_id;
                    if (logo_pixmap_id != PIXMAP_NONE)
                    {
                        rpc_call(&display, load_pixmap, logo_pixmap_id, cmrx_logo.data);
                    }
                    pixmap_free(&cmrx_logo);
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

            case EVENT_RUN_TERMINAL:
                rpc_call(&term, exec);
                break;

            case EVENT_RUN_PRESENTATION:
                break;

            case EVENT_BUTTON_OK:
                if (object_id == about_win_id)
                {
                    rpc_call(&display, close_window, about_win_id);
                }
                break;
        }
    }

}


OS_APPLICATION_MMIO_RANGE(init, 0, 0);
OS_APPLICATION(init);
OS_THREAD_CREATE(init, init_main, NULL, 64);

