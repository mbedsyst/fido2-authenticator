#ifndef CORE_STATE_MACHINE_STATE_HANDLERS_H_
#define CORE_STATE_MACHINE_STATE_HANDLERS_H_

#include <stdbool.h>

#include "app_events.h"


bool handle_state_idle(app_event_t event);
bool handle_state_receiving(app_event_t event);
bool handle_state_reconstructing(app_event_t event);
bool handle_state_processing(app_event_t event);
bool handle_state_deconstructing(app_event_t event);
bool handle_state_responding(app_event_t event);
bool handle_state_error(app_event_t event);

#endif