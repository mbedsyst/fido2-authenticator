#ifndef CORE_STATE_MACHINE_STATE_MACHINE_H_
#define CORE_STATE_MACHINE_STATE_MACHINE_H_

#include <stdint.h>
#include <stdbool.h>

#include "app_events.h"

typedef enum
{
    STATE_IDLE,
    STATE_RECEIVING_STARTED,
    STATE_RECONSTRUCTING,
    STATE_PROCESSING,
    STATE_DECONSTRUCTING,
    STATE_RESPONDING_STARTED,
    STATE_ERROR,
    STATE_NUM_STATES
} app_state_t;

void state_machine_init(void);
void state_machine_handle_event(app_event_t event);
app_state_t state_machine_get_state(void);


#endif