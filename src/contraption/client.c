#include "contraption.h"
#include "client.h"
#include <cmrx/ipc/notify.h>

#define CONTRAPTION_CLIENTS 16

struct ContraptionClient clients[CONTRAPTION_CLIENTS];

void contraption_init_client()
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        clients[q].thread_id = ~0;
        clients[q].events = 0;
    }
}

bool contraption_add_client(unsigned thread_id, void * notification_object)
{
    for (int q = 0; q < CONTRAPTION_CLIENTS; ++q)
    {
        if (clients[q].thread_id == ~0)
        {
            clients[q].thread_id = thread_id;
            clients[q].notification_object = notification_object;
            clients[q].events = 0;
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
