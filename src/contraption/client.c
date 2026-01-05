#include "contraption.h"
#include "client.h"
#include <cmrx/ipc/notify.h>
#include <string.h>

#define CONTRAPTION_CLIENTS 16

struct ContraptionClient clients[CONTRAPTION_CLIENTS];

void contraption_init_client()
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        clients[q].thread_id = ~0;
        clients[q].events = 0;
        clients[q].pixmap_id = PIXMAP_NONE;
    }
}

static void contraption_notify_dock(unsigned event, unsigned client_id)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id != ~0 && (clients[q].flags & CLIENT_DOCK))
        {
            contraption_send_event(clients[q].thread_id, event, client_id);
        }
    }
}

bool contraption_add_client(unsigned thread_id, const void * notification_object, const char * name, unsigned flags)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == ~0)
        {
            clients[q].thread_id = thread_id;
            int name_len = strlen(name);
            memcpy(clients[q].name, name, name_len <= 63 ? name_len : 63);
            clients[q].notification_object = notification_object;
            clients[q].events = 0;
            clients[q].flags = flags;
            if ((flags & CLIENT_SKIP_DOCK) == 0)
            {
                contraption_notify_dock(EVENT_CLIENT_CONNECTED, q);
            }
            return true;
        }
    }
    return false;
}

void contraption_remove_client(unsigned thread_id)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == thread_id)
        {
            clients[q].thread_id = ~0;
            clients[q].notification_object = 0;
            clients[q].events = 0;
            clients[q].pixmap_id = PIXMAP_NONE;
            contraption_notify_dock(EVENT_CLIENT_DISCONNECTED, q);
        }
    }
}

bool contraption_send_event(unsigned thread_id, unsigned event_id, unsigned object_id)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == thread_id)
        {
            if (clients[q].events < CONTRAPTION_CLIENT_EVENTS)
            {
                const unsigned ev_no = clients[q].events;
                clients[q].event_queue[ev_no].event_id = event_id;
                clients[q].event_queue[ev_no].object_id = object_id;
                clients[q].events++;
                notify_object(clients[q].notification_object);
                return true;
            }
            return false;
        }
    }
    return false;
}

bool contraption_receive_event(unsigned thread_id, unsigned * event_id, unsigned * object_id)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == thread_id)
        {
            if (clients[q].events > 0)
            {
                const unsigned ev_count = clients[q].events;

                *event_id = clients[q].event_queue[0].event_id;
                *object_id = clients[q].event_queue[0].object_id;
                for (int w = 0; w < (ev_count - 1); ++w)
                {
                    clients[q].event_queue[w] = clients[q].event_queue[w+1];
                }
                clients[q].events--;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool contraption_is_client(unsigned thread_id)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == thread_id)
        {
            return true;
        }
    }
    return false;
}

void contraption_set_client_icon(unsigned thread_id, int pixmap_id)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == thread_id)
        {
            clients[q].pixmap_id = pixmap_id;
            contraption_notify_dock(EVENT_CLIENT_NEW_ICON, q);
        }
    }
}
