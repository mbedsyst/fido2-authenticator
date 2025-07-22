#ifndef CORE_STATE_MACHINE_EVENT_QUEUE_H_
#define CORE_STATE_MACHINE_EVENT_QUEUE_H_

#include <stdbool.h>

#include <zephyr/kernel.h>

#include "app_events.h"

bool event_queue_push(app_event_t event);
bool event_queue_pop(app_event_t *event);
bool event_queue_is_empty(void);


#endif