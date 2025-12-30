#pragma once

#define CONTRAPTION_CLIENT_EVENTS 8

struct ContraptionEvent {
    uint16_t event_id;
    uint16_t object_id;
};

struct ContraptionClient {
    unsigned thread_id;
    void * notification_object;
    struct ContraptionEvent event_queue[CONTRAPTION_CLIENT_EVENTS];
    unsigned events;
};

void contraption_init_client();
bool contraption_add_client(unsigned thread_id, void * notification_object);
void contraption_remove_client(unsigned thread_id);
bool contraption_send_event(unsigned thread_id, unsigned event_id, unsigned object_id);
bool contraption_receive_event(unsigned thread_id, unsigned * event_id, unsigned * object_id);
bool contraption_is_client(unsigned thread_id);

