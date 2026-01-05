#include <cmrx/application.h>
#include <cmrx/ipc/rpc.h>
#include "../contraption/contraption.h"
#include <assert.h>
#include <cmrx/ipc/notify.h>
#include <cmrx/ipc/thread.h>
#include <stdio.h>
#include "../img/terminal.xpm"
#include "../util/xpm.h"
#include "../executable/executable.h"

#include "term_win.h"
#include "term_menu.h"

int term_win_id = WINDOW_NONE;
int term_col = 12;
int term_row = 0;

struct CGadget row_gadget;
struct Pixmap terminal_icon;

static int term_main(void * data)
{
    terminal_icon = xpm_to_pixmap(terminal_xpm);

    bool connected = rpc_call(&display, open_connection, &term_win, "Terminal", 0);
    assert(connected);
    int terminal_icon_id = rpc_call(&display, open_pixmap, &terminal_icon.dim);
    if (terminal_icon_id != PIXMAP_NONE)
    {
        rpc_call(&display, load_pixmap, terminal_icon_id, terminal_icon.data);
        rpc_call(&display, set_connection_icon, terminal_icon_id);
    }
    int term_win_id = rpc_call(&display, open_window, &term_win);

    term_app_menu.gadgets[0].sub_menu_id = rpc_call(&display, open_menu, &term_menu);
    term_app_menu.gadgets[1].sub_menu_id = rpc_call(&display, open_menu, &window_menu);

    int term_menu_id = rpc_call(&display, open_menu, &term_app_menu);
    rpc_call(&display, attach_menu, term_win_id, term_menu_id);

    while (1)
    {
        wait_for_object(&term_win, 0);
        unsigned event_id, object_id;
        bool event = rpc_call(&display, get_event, &event_id, &object_id);
        if (!event)
        {
            continue;
        }
        assert(event);
        switch (event_id) {
            case EVENT_KEY_PRESSED:
                if (object_id == '\n')
                {
                    term_row++;
                    term_col = 0;
                }
                rpc_call(&display, read_gadget, term_win_id, term_row + 1, &row_gadget);
                if (object_id != '\n')
                {
                    row_gadget.text[term_col++] = object_id;
                }
                if (term_col == 80)
                {
                    rpc_call(&display, write_gadget, term_win_id, term_row + 1, &row_gadget);
                    term_col = 0;
                    term_row++;
                    rpc_call(&display, read_gadget, term_win_id, term_row + 1, &row_gadget);
                }
                row_gadget.text[term_col] = '_';
                rpc_call(&display, write_gadget, term_win_id, term_row + 1, &row_gadget);
                break;

            default:
                break;
        }
    }

}

#include <cmrx/rpc/implementation.h>

IMPLEMENTATION_OF(struct Executable, struct ExecutableVTable);

static void term_exec(INSTANCE(this))
{
    thread_create(term_main, NULL, 64);
}

VTABLE struct ExecutableVTable exec_vtable = {
    term_exec
};

struct Executable term = {
    &exec_vtable
};

OS_APPLICATION_MMIO_RANGE(term, 0, 0);
OS_APPLICATION(term);
//OS_THREAD_CREATE(term, term_main, NULL, 64);
