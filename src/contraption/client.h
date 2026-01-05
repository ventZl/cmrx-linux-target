#pragma once

#define CONTRAPTION_CLIENT_EVENTS 8

struct ContraptionEvent {
    uint16_t event_id;
    uint16_t object_id;
};

struct ContraptionClient {
    unsigned thread_id;
    const void * notification_object;
    char name[64];
    struct ContraptionEvent event_queue[CONTRAPTION_CLIENT_EVENTS];
    unsigned events;
    unsigned flags;
    int pixmap_id;
};

extern struct ContraptionClient clients[];

void contraption_init_client();
bool contraption_add_client(unsigned int thread_id, const void* notification_object, const char* name, unsigned flags);
void contraption_remove_client(unsigned thread_id);
bool contraption_send_event(unsigned thread_id, unsigned event_id, unsigned object_id);
bool contraption_receive_event(unsigned thread_id, unsigned * event_id, unsigned * object_id);
bool contraption_is_client(unsigned thread_id);
void contraption_set_client_icon(unsigned thread_id, int pixmap_id);

