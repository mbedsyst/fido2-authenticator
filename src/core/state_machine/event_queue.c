#include "event_queue.h"

#define EVENT_QUEUE_SIZE    16

K_MSGQ_DEFINE(app_event_queue, sizeof(app_event_t), EVENT_QUEUE_SIZE, 4);

bool event_queue_push(app_event_t event)
{
    return k_msgq_put(&app_event_queue, &event, K_NO_WAIT) == 0;
}

bool event_queue_pop(app_event_t *event)
{
    if(!event)
    {
        return false;
    }
    return k_msgq_get(&app_event_queue, event, K_NO_WAIT) == 0;
}

bool event_queue_is_empty(void)
{
    return k_msgq_num_used_get(&app_event_queue) == 0;
}
