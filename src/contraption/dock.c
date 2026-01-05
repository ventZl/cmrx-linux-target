#include <cmrx/application.h>
#include <cmrx/ipc/notify.h>
#include "server.h"
#include "api_priv.h"
#include "desktop.h"
#include "dock_win.h"
#include "img/window.xpm"
#include "img/cmrx-logo.xpm"
#include "../util/xpm.h"
#include "client.h"

#include <stdio.h>

static int client_count = 0;

struct Pixmap window_icon;
struct Pixmap cmrx_logo_icon;

unsigned client_alloc[12];

int dock_main(void * data)
{
    window_icon = xpm_to_pixmap(window_xpm);
    cmrx_logo_icon = xpm_to_pixmap(cmrx_logo_xpm);

    contraption_open_connection(&display, &desktop_window, "CMRX", CLIENT_DOCK);
    int window_icon_id = contraption_open_pixmap(&display, &window_icon.dim);
    if (window_icon_id != PIXMAP_NONE)
    {
        contraption_load_pixmap(&display, window_icon_id, window_icon.data);
    }
    int cmrx_logo_icon_id = contraption_open_pixmap(&display, &cmrx_logo_icon.dim);
    if (cmrx_logo_icon_id != PIXMAP_NONE)
    {
        contraption_load_pixmap(&display, cmrx_logo_icon_id, cmrx_logo_icon.data);
    }

    contraption_set_connection_icon(&display, cmrx_logo_icon_id);

    for (int q = 0; q < DOCK_ICONS; ++q)
    {
        dock_window.gadgets[q].pixmap_id = window_icon_id;
    }
    contraption_open_window(&display, &desktop_window);
    int dock_window_id = WINDOW_NONE;
    bool refresh_dock = false;
    bool redraw_dock = false;

    while (true)
    {
        bool is_event = wait_for_object(&desktop_window, 0);
        unsigned event;
        unsigned object;
        is_event = contraption_get_event(&display, &event, &object);
        if (is_event)
        {
            switch (event) {
                case EVENT_CLIENT_CONNECTED:
                    client_alloc[client_count] = object;
                    client_count++;
                    refresh_dock = true;
                    break;

                case EVENT_CLIENT_DISCONNECTED:
                    {
                        bool shift = false;
                        for (int q = 0; q < client_count; ++q)
                        {
                            if (client_alloc[q] == object)
                            {
                                shift = true;
                            }
                            if (shift)
                            {
                                client_alloc[q] = client_alloc[q + 1];
                                dock_window.gadgets[q].pixmap_id = dock_window.gadgets[q + 1].pixmap_id;
                            }
                        }
                        client_count--;
                    }

                case EVENT_CLIENT_NEW_ICON:
                    for (int q = 0; q < client_count; ++q)
                    {
                        if (client_alloc[q] == object)
                        {
                            dock_window.gadgets[q].pixmap_id = clients[object].pixmap_id;
                            break;
                        }
                    }
                    refresh_dock = true;
                    break;

                default:
                    break;
            }

            if (refresh_dock)
            {
                printf("Refreshing dock for %d clients\n", client_count);
                dock_window.gadget_count = client_count;
                if (dock_window_id == WINDOW_NONE)
                {
                    dock_window_id = contraption_open_window(&display, &dock_window);
                }
                else
                {
                    contraption_reload_window(&display, dock_window_id, dock_window.gadgets, dock_window.gadget_count);
                    contraption_move_window(&display, dock_window_id, (SCREEN_WIDTH / 2) - ((8 + (client_count * 40)) / 2), SCREEN_HEIGHT - 40);
                    contraption_resize_window(&display, dock_window_id, 8 + (client_count * 40), 40);
                }
                refresh_dock = false;
            }
        }
    }

    return 0;
}

